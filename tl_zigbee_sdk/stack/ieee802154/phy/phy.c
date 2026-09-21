/********************************************************************************************************
 * @file    phy.c
 *
 * @brief   This is the source file for phy
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tlstk_common.h"
#include "phy.h"
#if ZB_MAC_SECURITY
#include "mac_security.h"
#endif
#include "mac_pib.h"
#include "mac.h"
#include "mac_trx.h"
#if BLE_CONCURRENT_MODE
#include "zigbee_ble_switch.h"
#endif

#define RF_SRX_MODE                             0

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define	LOGICCHANNEL_TO_PHYSICAL(p)             (((p) - 10) * 5)

#define RF_DROP_REASON_INVALID_CRC              0x01
#define RF_DROP_REASON_RF_BUSY                  0x02
#define RF_DROP_REASON_EXPECT_ACK               0x03
#define RF_DROP_REASON_INVALIC_BEACON           0x04
#define RF_DROP_REASON_FILTER_PANID             0x05
#define RF_DROP_REASON_FILTER_DSTADDR           0x06
#define RF_DROP_REASON_FILTER_LEN               0x07
#define RF_DROP_REASON_INVALIC_FRAME_TYPE       0x08


#if defined(MCU_CORE_TL721X)
#define ZB_TICK_US                              8
#define ZB_CURRENT_TICK()                       rf_bb_timer_get_tick()
#else
#define ZB_TICK_US                              S_TIMER_CLOCK_1US
#define ZB_CURRENT_TICK()                       clock_time()
#endif

#define ZB_PHR_LENGTH                           1
#define ZB_OCTET_DURATION_US                    32
#define ZB_PACKET_TIMESTAMP_GET(len)            ((len + ZB_PHR_LENGTH) * ZB_OCTET_DURATION_US * ZB_TICK_US)
#define ZB_ACK_TX_WAIT_US                       (128 + 32)

/**********************************************************************
 * LOCAL TYPEDEFS
 */
typedef enum {
    RF_GAIN_MODE_AUTO,
    RF_GAIN_MODE_MANU_MAX,
} rf_rxGainMode_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
u8 g_zb_txPowerSet = ZB_DEFAULT_TX_POWER_IDX;

/**********************************************************************
 * LOCAL VARIABLES
 */
_attribute_data_no_init_ _attribute_aligned_(4) u8 rf_tx_buf[ZB_RADIO_TX_HDR_LEN + 127];
_attribute_data_no_init_ _attribute_aligned_(4) u8 rf_ack_buf[ZB_RADIO_TX_HDR_LEN + 7];

volatile u8 *rf_rxBuf = NULL;
volatile u8 rfMode = RF_STATE_TX;
volatile u8 rf_busyFlag = 0;

volatile s8 soft_rssi;
volatile s32 sum_rssi, cnt_rssi = 1;

u8 fPaEn = 0;
u32 rf_pa_txen_pin = 0;
u32 rf_pa_rxen_pin = 0;

u8 fPtaEn = 0;
u32 rf_pta_priority_pin = 0;
u32 rf_pta_active_pin = 0;

#if PTA_ENABLE
static bool isWLANActive(void);
#endif

/**********************************************************************
 * LOCAL FUNCTIONS
 */
#if RF_SRX_MODE
//switch single RX to manual TX
#define ZB_SWITCH_TO_TXMODE()   do { \
                                    if (rfMode != RF_STATE_TX || ZB_RADIO_TRX_STA_GET() != RF_MODE_TX) { \
                                        rfMode = RF_STATE_TX; \
                                        if (fPaEn) { \
                                            drv_gpio_write(rf_pa_txen_pin, 1); \
                                            drv_gpio_write(rf_pa_rxen_pin, 0); \
                                        } \
                                        if (fPtaEn) { \
                                            drv_gpio_write(rf_pta_priority_pin, 1); \
                                        } \
                                        ZB_RADIO_TRX_OFF_AUTO_MODE(); \
                                        ZB_RADIO_TRX_SWITCH(RF_MODE_TX, LOGICCHANNEL_TO_PHYSICAL(rf_getChannel())); \
                                    } \
                                } while(0)

//switch manual TX to single RX
#define ZB_SWITCH_TO_RXMODE()   do { \
                                    if (rfMode != RF_STATE_RX || ZB_RADIO_TRX_STA_GET() != RF_MODE_AUTO) { \
                                        rfMode = RF_STATE_RX; \
                                        if (fPaEn) { \
                                            drv_gpio_write(rf_pa_txen_pin, 0); \
                                            drv_gpio_write(rf_pa_rxen_pin, 1); \
                                        } \
                                        if (fPtaEn) { \
                                            drv_gpio_write(rf_pta_priority_pin, 0); \
                                        } \
                                        ZB_RADIO_TRX_SWITCH(RF_MODE_AUTO, LOGICCHANNEL_TO_PHYSICAL(rf_getChannel())); \
                                    } \
                                    ZB_RADIO_SRX_START(clock_time()); \
                                } while(0)
#else
//switch manual RX to manual TX
#define ZB_SWITCH_TO_TXMODE()   do { \
                                    if (rfMode != RF_STATE_TX || ZB_RADIO_TRX_STA_GET() != RF_MODE_TX) { \
                                        rfMode = RF_STATE_TX; \
                                        if (fPaEn) { \
                                            drv_gpio_write(rf_pa_txen_pin, 1); \
                                            drv_gpio_write(rf_pa_rxen_pin, 0); \
                                        } \
                                        if (fPtaEn) { \
                                            drv_gpio_write(rf_pta_priority_pin, 1); \
                                        } \
                                        ZB_RADIO_TRX_SWITCH(RF_MODE_TX, LOGICCHANNEL_TO_PHYSICAL(rf_getChannel())); \
                                    } \
                                } while(0)

//switch manual TX to manual RX
#define ZB_SWITCH_TO_RXMODE()   do { \
                                    if (rfMode != RF_STATE_RX || ZB_RADIO_TRX_STA_GET() != RF_MODE_RX) { \
                                        rfMode = RF_STATE_RX; \
                                        if (fPaEn) { \
                                            drv_gpio_write(rf_pa_txen_pin, 0); \
                                            drv_gpio_write(rf_pa_rxen_pin, 1); \
                                        } \
                                        if (fPtaEn) { \
                                            drv_gpio_write(rf_pta_priority_pin, 0); \
                                        } \
                                        ZB_RADIO_TRX_SWITCH(RF_MODE_RX, LOGICCHANNEL_TO_PHYSICAL(rf_getChannel())); \
                                    } \
                                } while(0)
#endif

#define ZB_SWITCH_TO_OFFMODE()  do { \
                                    if (rfMode != RF_STATE_OFF || ZB_RADIO_TRX_STA_GET() != RF_MODE_OFF) { \
                                        rfMode = RF_STATE_OFF; \
                                        if (fPaEn) { \
                                            drv_gpio_write(rf_pa_txen_pin, 0); \
                                            drv_gpio_write(rf_pa_rxen_pin, 0); \
                                        } \
                                        if (fPtaEn) { \
                                            drv_gpio_write(rf_pta_priority_pin, 0); \
                                        } \
                                        ZB_RADIO_TRX_SWITCH(RF_MODE_OFF, LOGICCHANNEL_TO_PHYSICAL(rf_getChannel())); \
                                    } \
                                } while(0)

_attribute_ram_code_ u32 mac_currentTickGet(void)
{
    return clock_time();
}

bool tl_stackBusy(void)
{
    return ((rf_busyFlag || tl_zbMacStateBusy()) ? TRUE : FALSE);
}

/*********************************************************************
 * @fn     rf_reset
 *
 * @brief  Reset RF module and do the RF power up.
 *
 * @param  none
 *
 * @return none
 */
void rf_reset(void)
{
    rf_setTrxState(RF_STATE_OFF);

    rf_setTxPower(g_zb_txPowerSet);

    rf_setChannel(g_mac_pib->phyChannelCur);

    if (rf_rxBuf) {
        rf_setRxBuf((u8 *)rf_rxBuf);
    } else {
        rf_setRxBuf(tl_getRxBuf());
    }

    ZB_RADIO_TRX_CFG(RF_PKT_BUFF_LEN);

    ZB_RADIO_RX_ENABLE;
    ZB_RADIO_TX_ENABLE;
    ZB_TIMESTAMP_ENABLE;
}

static void rf_edDetect(void)
{
    s8 rssi = ZB_RADIO_RSSI_GET();
    sum_rssi += rssi;
    if (++cnt_rssi >= 0xfffffe) {
        sum_rssi = sum_rssi / cnt_rssi;
        cnt_rssi = 1;
    }
}

static void rf_mac_ack_build(void)
{
    memset(rf_ack_buf, 0, 12);

    ZB_RADIO_DMA_HDR_BUILD(rf_ack_buf, 3);

    rf_ack_buf[4] = 5;
    rf_ack_buf[5] = 0x02;
    rf_ack_buf[6] = 0x00;
}

/*********************************************************************
 * @fn     rf_init
 *
 * @brief  Initialize RF module and do the RF power up.
 *
 * @param  none
 *
 * @return none
 */
void rf_init(void)
{
    rf_reset();

    rf_mac_ack_build();

    /* Register ED-Scan polling function, but disable it at beginning. */
    ev_on_poll(EV_POLL_ED_DETECT, rf_edDetect);
    ev_disable_poll(EV_POLL_ED_DETECT);
}

/*********************************************************************
 * @fn     rf_setRxBuf
 *
 * @brief  Set RX buffer to dma and enable DMA.
 *
 * @param  pBuf - the allocated rx buffer
 *
 * @return none
 */
void rf_setRxBuf(u8 *pBuf)
{
    rf_rxBuf = pBuf;
    ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
    ZB_RADIO_RX_BUF_SET((u8 *)rf_rxBuf);//todo: 826x/8258 need fix rf driver
}

/*********************************************************************
 * @fn     rf_getTrxState
 *
 * @brief  Get current TRX state.
 *
 * @param  none
 *
 * @return state
 */
_always_inline u8 rf_getTrxState(void)
{
    return rfMode;
}

/*********************************************************************
 * @fn     rf_setTrxState
 *
 * @brief  Go to specified state and set related register.
 *
 * @param  state - Specified state
 *
 * @return none
 */
#if defined(MCU_CORE_B91) || defined(MCU_CORE_TL721X) || \
    defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || \
    defined(MCU_CORE_TL521X)
_attribute_ram_code_
#endif
void rf_setTrxState(u8 state)
{
    if (RF_STATE_RX == state || RF_STATE_ED == state) {
        if (TL_ZB_MAC_STATUS_GET() == ZB_MAC_STATE_ACTIVE_SCAN || RF_STATE_ED == state) {
            ZB_RADIO_MODE_AUTO_GAIN();
        } else {
            ZB_RADIO_MODE_MAX_GAIN();
        }

        ZB_SWITCH_TO_RXMODE();
    } else if (RF_STATE_TX == state) {
        ZB_SWITCH_TO_TXMODE();
        WaitUs(ZB_TX_WAIT_US);
    } else {
        /* Close RF */
        ZB_SWITCH_TO_OFFMODE();
    }
}

/*********************************************************************
 * @fn     rf_setChannel
 *
 * @brief  Set specified channel to RF module.
 *
 * @param  chn - 11~26
 *
 * @return none
 */
void rf_setChannel(u8 chn)
{
    if ((chn < TL_ZB_MAC_CHANNEL_START) ||
        (chn > TL_ZB_MAC_CHANNEL_STOP)) {
        return;
    }
    g_mac_pib->phyChannelCur = chn;

    u32 r = drv_disable_irq();

    u8 phySta = ZB_RADIO_TRX_STA_GET();
    ZB_RADIO_TRX_SWITCH(phySta, LOGICCHANNEL_TO_PHYSICAL(chn));

    if (phySta == RF_MODE_RX || phySta == RF_MODE_AUTO) {
        rfMode = RF_STATE_RX;
    } else if (phySta == RF_MODE_TX) {
        rfMode = RF_STATE_TX;
    } else if (phySta == RF_MODE_OFF) {
        rfMode = RF_STATE_OFF;
    }

#if RF_SRX_MODE
    if (phySta == RF_MODE_AUTO && rfMode == RF_STATE_RX) {
        ZB_RADIO_SRX_START(clock_time());
    }
#endif

    drv_restore_irq(r);
}

/*********************************************************************
 * @fn     rf_getChannel
 *
 * @brief  Get specified channel.
 *
 * @param  none
 *
 * @return chn
 */
_always_inline u8 rf_getChannel(void)
{
    return g_mac_pib->phyChannelCur;
}

/*********************************************************************
 * @fn     rf_setTxPower
 *
 * @brief  Set specified transmitted power.
 *
 * @param  txPower - Specified power
 *
 * @return none
 */
void rf_setTxPower(u8 power)
{
    if (fPaEn) {
        ZB_RADIO_TX_POWER_SET(ZB_RADIO_TX_0DBM);
    } else {
        ZB_RADIO_TX_POWER_SET(power);
    }
}

/*********************************************************************
 * @fn     rf_getLqi
 *
 * @brief  Get calculated Link Quality value
 *
 * @param  rssi
 *
 * @return lqi
 */
u8 rf_getLqi(s8 rssi)
{
    rf_rxGainMode_t mode = RF_GAIN_MODE_MANU_MAX;
    if (TL_ZB_MAC_STATUS_GET() == ZB_MAC_STATE_ACTIVE_SCAN) {
        mode = RF_GAIN_MODE_AUTO;
    }

    u8 lqi = 0;
    ZB_RADIO_RSSI_TO_LQI(mode, rssi, lqi);
    return lqi;
}

/*********************************************************************
 * @fn     rf_lqi2cost
 *
 * @brief  Get calculated cost value
 *
 * @param  lqi
 *
 * @return cost
 */
u8 rf_lqi2cost(u8 lqi)
{
    u8 cost = 0;
    ZB_LQI_TO_PATH_COST(lqi, cost);
    return cost;
}

/*********************************************************************
 * @fn     rf_startEDScan
 *
 * @brief  Start ED detect
 *
 * @param  none
 *
 * @return none
 */
void rf_startEDScan(void)
{
    soft_rssi = -110;
    sum_rssi = 0;
    cnt_rssi = 0;

    ev_enable_poll(EV_POLL_ED_DETECT);
    rf_setTrxState(RF_STATE_ED);
}

/*********************************************************************
 * @fn     rf_stopEDScan
 *
 * @brief  Stop Energy Detect
 *
 * @param  none
 *
 * @return result
 */
u8 rf_stopEDScan(void)
{
    if (cnt_rssi == 0) {
        cnt_rssi = 1;
    }
    soft_rssi = sum_rssi / cnt_rssi;

    ev_disable_poll(EV_POLL_ED_DETECT);

    u8 ed = rf_getLqi(soft_rssi);

    return ed;
}

volatile s8 T_rssiPeak = 0;
#if defined(MCU_CORE_B91) || defined(MCU_CORE_TL721X) || \
    defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || \
    defined(MCU_CORE_TL521X)
_attribute_ram_code_
#endif
u8 rf_performCCA(void)
{
#if PTA_ENABLE
    if (isWLANActive()) {
        return PHY_CCA_BUSY;
    }
#endif

    u32 t1 = clock_time();
    s8 rssi_peak = -110;
    s8 rssi_cur = -110;
    s32 rssiSum = 0;
    s32 cnt = 1;

#if BLE_CONCURRENT_MODE
    if (CURRENT_SLOT_GET() == DUALMODE_SLOT_BLE) {
        return PHY_CCA_BUSY;
    }
#endif

    if (rf_getTrxState() == RF_STATE_TX || !RF_DMA_BUSY()) {
        rf_setTrxState(RF_STATE_OFF);
        rf_setTrxState(RF_STATE_RX);
        WaitUs(85);
    }

    rssi_cur = ZB_RADIO_RSSI_GET();
    rssiSum += rssi_cur;
    while (!clock_time_exceed(t1, 128)) {
        rssi_cur = ZB_RADIO_RSSI_GET();
        rssiSum += rssi_cur;
        cnt++;

#if RF_SRX_MODE
        if (ZB_RADIO_RX_DONE) {
            ZB_RADIO_RX_DONE_CLR;
            if (ZB_RADIO_TRX_STA_GET() == RF_MODE_AUTO) {
                ZB_RADIO_SRX_START(clock_time());
            }
        }
#endif
    }
    rssi_peak = rssiSum / cnt;
    T_rssiPeak = rssi_peak;

    if (rssi_peak > CCA_THRESHOLD || (rf_busyFlag & TX_BUSY)) {
        //return if currently is TX state
        return PHY_CCA_BUSY;
    } else {
        return PHY_CCA_IDLE;
    }
}

_always_inline void rf802154_tx_ready(u8 *buf, u8 len)
{
    //fill the telink RF header
    ZB_RADIO_DMA_HDR_BUILD(rf_tx_buf, len);

    rf_tx_buf[4] = len + 2;

    for (u8 i = 0; i < len; i++) {
        rf_tx_buf[i + 5] = buf[i];
    }
}

#if defined(MCU_CORE_B91) || defined(MCU_CORE_TL721X) || \
    defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || \
    defined(MCU_CORE_TL521X)
_attribute_ram_code_
#endif
void rf802154_tx(void)
{
    rf_setTrxState(RF_STATE_TX);

    ZB_RADIO_TX_DONE_CLR;
    ZB_RADIO_RX_DONE_CLR;
    ZB_RADIO_TX_START(rf_tx_buf);//Manual mode
}

#if PA_ENABLE
/*********************************************************************
 * @fn     rf_paInit
 *
 * @brief  Initialize PA.
 *
 * @param  tx_pin
 * @param  rx_pin
 *
 * @return none
 */
void rf_paInit(u32 tx_pin, u32 rx_pin)
{
    rf_pa_txen_pin = tx_pin;
    rf_pa_rxen_pin = rx_pin;

    drv_gpio_func_set(rf_pa_txen_pin);
    drv_gpio_output_en(rf_pa_txen_pin, 1);
    drv_gpio_write(rf_pa_txen_pin, 0);

    drv_gpio_func_set(rf_pa_rxen_pin);
    drv_gpio_output_en(rf_pa_rxen_pin, 1);
    drv_gpio_write(rf_pa_rxen_pin, 1);

    fPaEn = 1;
    g_zb_txPowerSet = ZB_RADIO_TX_0DBM; // chip PA is set as 0dBm
}

_attribute_ram_code_ void rf_paShutDown(void)
{
    if (fPaEn) {
        drv_gpio_write(rf_pa_txen_pin, 0); // PA TX_DIS
        drv_gpio_write(rf_pa_rxen_pin, 0); // PA RX_DIS
    }
}
#endif

#if PTA_ENABLE
/*********************************************************************
 * @fn     rf_ptaInit
 *
 * @brief  Initialize PTA.
 *
 * @param  priority_pin
 * @param  active_pin
 *
 * @return none
 */
void rf_ptaInit(u32 priority_pin, u32 active_pin)
{
    rf_pta_priority_pin = priority_pin;
    rf_pta_active_pin = active_pin;

    drv_gpio_func_set(rf_pta_priority_pin);
    drv_gpio_output_en(rf_pta_priority_pin, 1);
    drv_gpio_input_en(rf_pta_priority_pin, 0);
    drv_gpio_write(rf_pta_priority_pin, 0);

    drv_gpio_func_set(rf_pta_active_pin);
    drv_gpio_output_en(rf_pta_active_pin, 0);
    drv_gpio_input_en(rf_pta_active_pin, 1);

    fPtaEn = 1;
}

_attribute_ram_code_ bool isWLANActive(void)
{
    if (fPtaEn) {
        if (drv_gpio_read(rf_pta_active_pin)) {
            WaitUs(2);
            if (drv_gpio_read(rf_pta_active_pin)) {
                return TRUE;
            }
        }
    }

    return FALSE;
}
#endif

/*********************************************************************
 * @fn     rf_rx_irq_handler
 *
 * @brief  RX Interrupt handler
 *
 * @param  none
 *
 * @return none
 */
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
_attribute_ram_code_ __attribute__((optimize("-Os")))
#else
_attribute_ram_code_
#endif
void rf_rx_irq_handler(void)
{
    u8 *p = (u8 *)rf_rxBuf;
    u8 fAck = 0;
    u8 fDrop = 0;

    if (RF_DMA_BUSY()) {
    	return;
    }

    ZB_RADIO_RX_DISABLE;

    g_mac_diags->macRxIrqCnt++;

    if (!ZB_RADIO_CRC_OK(p) || !ZB_RADIO_PACKET_LENGTH_OK(p)) {
    	//diagnostics MAC layer receive error packet
    	g_mac_diags->macRxCrcFail++;

    	ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
        ZB_RADIO_RX_ENABLE;
#if RF_SRX_MODE
        ZB_SWITCH_TO_RXMODE();
#endif
        return;
    }

    //parse necessary field to be used later
    u8 len = (u8)ZB_RADIO_ACTUAL_PAYLOAD_LEN(p);
    u8 *macPld = p + ZB_RADIO_RX_HDR_LEN;
    u32 timestamp = ZB_RADIO_TIMESTAMP_GET(p) + ZB_PACKET_TIMESTAMP_GET(len);

    //do filter
    u8 *pSrcAddr = zb_macDataFilter(macPld, len, &fDrop, &fAck);
    if (fDrop) {
        //drop the packet and recover DMA
    	ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
        ZB_RADIO_RX_ENABLE;
#if RF_SRX_MODE
        ZB_SWITCH_TO_RXMODE();
#endif
        return;
    }

    //switch to TX in advance to let the pll stable
    if (macPld[0] & MAC_FCF_ACK_REQ_BIT) {
        ZB_SWITCH_TO_TXMODE();
    }

    //have no enough buffer, use current rxBuf, and drop it
    u8 *rxNextBuf = tl_getRxBuf();
    if (!rxNextBuf) {
        if (macPld[0] & MAC_FCF_ACK_REQ_BIT) {
            ZB_SWITCH_TO_RXMODE();
        }

        //diagnostics PHY to MAC queue limit
        g_mac_diags->phytoMACqueuelimitreached++;

        ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
        ZB_RADIO_RX_ENABLE;
#if RF_SRX_MODE
        ZB_SWITCH_TO_RXMODE();
#endif
        return;
    }

    //use the backup buffer to receive next packet
    rf_rxBuf = rxNextBuf;
    ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
    ZB_RADIO_RX_BUF_SET((u8 *)rf_rxBuf);

    //prepare and send ACK
    if (macPld[0] & MAC_FCF_ACK_REQ_BIT) {
        rf_ack_buf[ZB_RADIO_TX_HDR_LEN + 2] = macPld[2]; //sequence number
        rf_ack_buf[ZB_RADIO_TX_HDR_LEN] = 0x02;

#if (ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE)
        u16 fcf = (macPld[1] << 8) | macPld[0]; //frame control
        tl_zb_mac_mhr_t mHdr;
        mHdr.frameCtrl = fcf;
#if ZB_MAC_SECURITY
        mHdr.sec.securityLevel = SECURITY_LEVEL_NONE;
#endif
        u8 pldOft = tl_zbMacHdrSize(&mHdr);
        u8 frameType = (fcf & MAC_FCF_FRAME_TYPE_MASK) >> MAC_FCF_FRAME_TYPE_POS;
        bool cmdDataReq = ((frameType == MAC_FRAME_COMMAND) && (macPld[pldOft] == MAC_CMD_DATA_REQUEST));

        //if there is pending data, set the pending bit to 1
        u8 srcAddrMode = ZB_ADDR_NO_ADDR;
        if ((macPld[1] & MAC_FCF_SRC_ADDR_BIT) == 0x80) {
            srcAddrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
        } else if ((macPld[1] & MAC_FCF_SRC_ADDR_BIT) == 0xc0) {
            srcAddrMode = ZB_ADDR_64BIT_DEV;
        }
        if (cmdDataReq && srcAddrMode) {
            extern u8 tl_zbMacPendingDataCheck(u8 addrMode, u8 *addr, u8 send);
            if (MAC_SUCCESS == tl_zbMacPendingDataCheck(srcAddrMode, pSrcAddr, 1)) {
                rf_ack_buf[ZB_RADIO_TX_HDR_LEN] |= (1 << 4);
            }
        }
#else
        (void)pSrcAddr;
#endif

        u32 txDelayUs = (ZB_CURRENT_TICK() - timestamp) / ZB_TICK_US;

        u32 curTick = clock_time();
        if (txDelayUs < ZB_ACK_TX_WAIT_US) {
            while(!clock_time_exceed(curTick, (ZB_ACK_TX_WAIT_US - txDelayUs)));
        }

        //clear RF irq_mask
        ZB_RADIO_IRQ_MASK_CLR;
        //send ACK
        ZB_RADIO_TX_START(rf_ack_buf);

        //wait until TX done or timeout
        curTick = clock_time();
        while(!ZB_RADIO_TX_DONE && !clock_time_exceed(curTick, 1000));

        //clear TX done status
        ZB_RADIO_TX_DONE_CLR;
        //set RF irq_mask
        ZB_RADIO_IRQ_MASK_SET;
        //switch to RX mode
        ZB_SWITCH_TO_RXMODE();
    }

    //enable RX DMA again
    ZB_RADIO_RX_ENABLE;
#if RF_SRX_MODE
    ZB_SWITCH_TO_RXMODE();
#endif

    //handle received data
    zb_macDataRecvHandler(p, macPld, len, fAck, ZB_RADIO_TIMESTAMP_GET(p), ZB_RADION_PKT_RSSI_GET(p) - 110);
}

/*********************************************************************
 * @fn     rf_tx_irq_handler
 *
 * @brief  TX Interrupt handler for RF module.
 *
 * @param  none
 *
 * @return none
 */
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
_attribute_ram_code_ __attribute__((optimize("-Os")))
#else
_attribute_ram_code_
#endif
void rf_tx_irq_handler(void)
{
    //clear TX busy flag after TX done
    rf_busyFlag &= ~TX_BUSY;

    g_mac_diags->macTxIrqCnt++;

    ZB_SWITCH_TO_RXMODE();

    zb_macDataSendHandler();
}

inline bool zb_rfSwitchAllow(void)
{
    return (rf_busyFlag != TX_BUSY);
}

inline bool zb_rfTxDoing(void)
{
    return (rf_busyFlag & TX_BUSY);
}

void restore_zb_rf_context(void)
{
#if defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || \
    defined(MCU_CORE_TL521X)
    rf_reset_register_value();
#else
    reset_baseband();
#endif

    ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
    CLEAR_ALL_RFIRQ_STATUS;
    rf_clr_irq_mask(FLD_RF_IRQ_ALL);

    ZB_RADIO_INIT();
    ZB_RADIO_TRX_CFG(RF_PKT_BUFF_LEN);
    ZB_RADIO_RX_BUF_SET((u8 *)rf_rxBuf);

    rf_setChannel(rf_getChannel());
    rf_setTrxState(rfMode);
}
