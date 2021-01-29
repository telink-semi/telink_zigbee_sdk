/********************************************************************************************************
 * @file	mac_phy.c
 *
 * @brief	This is the source file for mac_phy
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
/**********************************************************************
 * INCLUDES
 */
#include "../common/includes/zb_common.h"
#include "compiler.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define	LOGICCHANNEL_TO_PHYSICAL(p)					(((p)-10)*5)

#define RF_DROP_REASON_INVALID_CRC        			0x01
#define RF_DROP_REASON_RF_BUSY           		 	0x02
#define RF_DROP_REASON_EXPECT_ACK         			0x03
#define RF_DROP_REASON_INVALIC_BEACON     			0x04
#define RF_DROP_REASON_FILTER_PANID       			0x05
#define RF_DROP_REASON_FILTER_DSTADDR     			0x06
#define RF_DROP_REASON_FILTER_LEN         			0x07
#define RF_DROP_REASON_INVALIC_FRAME_TYPE         	0x08

/**********************************************************************
 * LOCAL TYPEDEFS
 */
typedef enum{
	RF_GAIN_MODE_AUTO,
	RF_GAIN_MODE_MANU_MAX,
}rf_rxGainMode_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
u8 g_zb_txPowerSet = ZB_DEFAULT_TX_POWER_IDX;

/**********************************************************************
 * LOCAL VARIABLES
 */
_attribute_aligned_(4) u8 rf_tx_buf[ZB_RADIO_TX_HDR_LEN + 127];
_attribute_aligned_(4) u8 rf_ack_buf[ZB_RADIO_TX_HDR_LEN + 7];

u8 *rf_rxBuf = NULL;
u8 rfMode = RF_STATE_TX;
volatile u8 rf_busyFlag = 0;

volatile s8 soft_rssi;
volatile s32 sum_rssi, cnt_rssi = 1;

u8 fPaEn;
u32 rf_pa_txen_pin;
u32 rf_pa_rxen_pin;

/**********************************************************************
 * LOCAL FUNCTIONS
 */
#define ZB_SWTICH_TO_TXMODE()    do{ \
									if(fPaEn) {	\
										drv_gpio_write(rf_pa_txen_pin, 1); 		\
										drv_gpio_write(rf_pa_rxen_pin, 0); 		\
									}										\
									ZB_RADIO_TRX_SWITCH(RF_MODE_TX, LOGICCHANNEL_TO_PHYSICAL(rf_getChannel()));	\
								}while(0)

#define ZB_SWTICH_TO_RXMODE()    do{ \
									if(fPaEn) {	\
										drv_gpio_write(rf_pa_txen_pin, 0); 		\
										drv_gpio_write(rf_pa_rxen_pin, 1); 		\
									}										\
									ZB_RADIO_TRX_SWITCH(RF_MODE_RX, LOGICCHANNEL_TO_PHYSICAL(rf_getChannel()));	\
								}while(0)


/*********************************************************************
 * @fn      rf_reset
 *
 * @brief   Reset RF module and do the RF power up.
 *
 * @param   none
 *
 * @return  none
 */
void rf_reset(void)
{
	rf_setTrxState(RF_STATE_TX);

    rf_setTxPower(g_zb_txPowerSet);

    rf_setChannel(g_zbMacPib.phyChannelCur);

	if(rf_rxBuf){
		rf_setRxBuf(rf_rxBuf);
	}else{
		rf_setRxBuf(tl_getRxBuf());
	}

    ZB_RADIO_TRX_CFG(RF_PKT_BUFF_LEN);

    ZB_RADIO_RX_ENABLE;
	ZB_RADIO_TX_ENABLE;
	ZB_TIMESTAMP_ENABLE;
}

static void rf_edDetect(void)
{
    s8 rssi;
    rssi = ZB_RADIO_RSSI_GET();

    //soft_rssi = rssi;//(rssi + soft_rssi)/2;
    sum_rssi += rssi;
    if(++cnt_rssi >= 0xfffffe){
    	sum_rssi = sum_rssi / cnt_rssi;
    	cnt_rssi = 1;
    }
}

static void rf_mac_ack_build(void)
{
    /* ack buf */
    memset(rf_ack_buf, 0, 12);

    ZB_RADIO_DMA_HDR_BUILD(rf_ack_buf, 3);

    rf_ack_buf[4] = 5;
    rf_ack_buf[5] = 0x02;
    rf_ack_buf[6] = 0x00;
}

/*********************************************************************
 * @fn      rf_init
 *
 * @brief   Initialize RF module and do the RF power up.
 *
 * @param   none
 *
 * @return  none
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
 * @fn      rf_setRxBuf
 *
 * @brief   Set RX buffer to dma and enable DMA.
 *
 * @param   pBuf - the allocated rx buffer
 *
 * @return  none
 */
void rf_setRxBuf(u8 *pBuf)
{
    rf_rxBuf = pBuf;
    ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
    ZB_RADIO_RX_BUF_SET(rf_rxBuf);//todo: 826x/8258 need fix rf driver
}

/*********************************************************************
 * @fn      rf_TrxStateGet
 *
 * @brief   Get current TRX state.
 *
 * @param   none
 *
 * @return  state
 */
u8 rf_TrxStateGet(void)
{
	return rfMode;
}

/*********************************************************************
 * @fn      rf_setTrxState
 *
 * @brief   Go to specified state and set related register.
 *
 * @param   state - Specified state
 *
 * @return  none
 */
_attribute_ram_code_ void rf_setTrxState(u8 state)
{
#ifndef WIN32
    if(RF_STATE_RX == state || RF_STATE_ED == state){
    	if(TL_ZB_MAC_STATUS_GET() == ZB_MAC_STATE_ACTIVE_SCAN || RF_STATE_ED == state){
    		ZB_RADIO_MODE_AUTO_GAIN();
    	}else{
    		ZB_RADIO_MODE_MAX_GAIN();
    	}

    	ZB_SWTICH_TO_RXMODE();
        rfMode = RF_STATE_RX;
    }else if(RF_STATE_TX == state){
    	ZB_SWTICH_TO_TXMODE();
        WaitUs(ZB_TX_WAIT_US);
        rfMode = RF_STATE_TX;
    }else{
        /* Close RF */
    	rf_paShutDown();
    	ZB_RADIO_TRX_SWITCH(RF_MODE_OFF, LOGICCHANNEL_TO_PHYSICAL(rf_getChannel()));
        rfMode = RF_MODE_OFF;
    }
#endif  /* WIN32 */
}

/*********************************************************************
 * @fn      rf_setChannel
 *
 * @brief   Set specified channel to RF module.
 *
 * @param   chn - 11~26
 *
 * @return  none
 */
void rf_setChannel(u8 chn)
{
	if((chn < TL_ZB_MAC_CHANNEL_START) || (chn > TL_ZB_MAC_CHANNEL_STOP)){
		return;
	}
	g_zbMacPib.phyChannelCur = chn;
	ZB_RADIO_TRX_SWITCH(ZB_RADIO_TRX_STA_GET(), LOGICCHANNEL_TO_PHYSICAL(chn));
}

/*********************************************************************
 * @fn      rf_getChannel
 *
 * @brief   Get specified channel.
 *
 * @param   none
 *
 * @return  chn
 */
inline u8 rf_getChannel(void)
{
	return	g_zbMacPib.phyChannelCur;
}

/*********************************************************************
 * @fn      rf_setTxPower
 *
 * @brief   Set specified transmitted power.
 *
 * @param   txPower - Specified power
 *
 * @return  none
 */
void rf_setTxPower(u8 power)
{
	if(fPaEn){
		ZB_RADIO_TX_POWER_SET(ZB_RADIO_TX_0DBM);
	}else{
		ZB_RADIO_TX_POWER_SET(power);
	}
}

/*********************************************************************
 * @fn      rf_getLqi
 *
 * @brief   Get calculated Link Quality value
 *
 * @param   rssi
 *
 * @return  lqi result
 */
u8 rf_getLqi(s8 rssi)
{
	rf_rxGainMode_t mode = RF_GAIN_MODE_MANU_MAX;
	if(TL_ZB_MAC_STATUS_GET() == ZB_MAC_STATE_ACTIVE_SCAN){
		mode = RF_GAIN_MODE_AUTO;
    }

	u8 lqi = 0;
	ZB_RADIO_RSSI_TO_LQI(mode, rssi, lqi);
	return lqi;
}

/*********************************************************************
 * @fn      rf_startED
 *
 * @brief   Start ED detect
 *
 * @param   none
 *
 * @return  none
 */
void rf_startED(void)
{
#ifndef WIN32
    soft_rssi = -110;
    sum_rssi = 0;
    cnt_rssi = 0;
    ev_enable_poll(EV_POLL_ED_DETECT);
    /* Change to ED mode */
    rf_setTrxState(RF_STATE_ED);
#endif
}

/*********************************************************************
 * @fn      rf_stopED
 *
 * @brief   Stop Energy Detect
 *
 * @param   none
 *
 * @return  ED result
 */
u8 rf_stopED(void)
{
#ifndef WIN32
    u8 ed;
    u32 temp;

    if(cnt_rssi == 0) cnt_rssi = 1;
    soft_rssi = sum_rssi/cnt_rssi;

    ev_disable_poll(EV_POLL_ED_DETECT);/*WISE_FIX_ME*/
    /* Transfer the RSSI value to ED value */
    if(soft_rssi <= -106){
        ed = 0;
    }else if(soft_rssi >= -6){
        ed = 0xff;
    }else{
    	temp = (soft_rssi + 106) * 255;
        ed = temp/100;
    }

    return ed;

#else
    return 0;
#endif
}

_attribute_ram_code_ u8 rf_performCCA(void)
{
	u32 t1 = clock_time();
	s8 rssi_peak = -110;
	s8 rssi_cur = -110;
	s32 rssiSum = 0;
	s32 cnt = 1;

	rssi_cur = ZB_RADIO_RSSI_GET();
	rssiSum += rssi_cur;
	while(!clock_time_exceed(t1,128)){
		rssi_cur = ZB_RADIO_RSSI_GET();
		rssiSum += rssi_cur;
		cnt++;
	}
	rssi_peak = rssiSum/cnt;

	if(rssi_peak > CCA_THRESHOLD || (rf_busyFlag & TX_BUSY)){//Return if currently in TX state
		return PHY_CCA_BUSY;
	}else{
		return PHY_CCA_IDLE;
	}
}




void rf802154_tx_ready(u8 *buf, u8 len)
{
  	/* Fill the telink RF header */
	ZB_RADIO_DMA_HDR_BUILD(rf_tx_buf, len);

  	rf_tx_buf[4] = len + 2;
  	memcpy(rf_tx_buf + 5, buf, len);
}

_attribute_ram_code_ void rf802154_tx(void)
{
 	rf_setTrxState(RF_STATE_TX);

 	ZB_RADIO_TX_DONE_CLR;
 	ZB_RADIO_RX_DONE_CLR;
 	ZB_RADIO_TX_START(rf_tx_buf);//Manual mode
}



/*********************************************************************
 * @fn      rf_paInit
 *
 * @brief   Initialize PA.
 *
 * @param   none
 *
 * @param   none
 *
 * @return  none
 */
void rf_paInit(u32 TXEN_pin, u32 RXEN_pin)
{
    rf_pa_txen_pin = TXEN_pin;
    rf_pa_rxen_pin = RXEN_pin;

    drv_gpio_func_set(rf_pa_txen_pin);
    drv_gpio_output_en(rf_pa_txen_pin, 1);
    drv_gpio_write(rf_pa_txen_pin, 0);

    drv_gpio_func_set(rf_pa_rxen_pin);
    drv_gpio_output_en(rf_pa_rxen_pin, 1);
    drv_gpio_write(rf_pa_rxen_pin, 1);

    fPaEn = 1;
}

_attribute_ram_code_ void rf_paShutDown(void)
{
    if(fPaEn){
    	drv_gpio_write(rf_pa_txen_pin, 0); // PA TX_DIS
    	drv_gpio_write(rf_pa_rxen_pin, 0); // PA RX_DIS
    }
}

/*********************************************************************
 * @fn      rf_rx_irq_handler
 *
 * @brief   RX irq handler
 *
 * @param   none
 *
 * @return  none
 */
_attribute_ram_code_ __attribute__((optimize("-Os"))) void rf_rx_irq_handler(void)
{
    u8 *p = rf_rxBuf;
    u8 fAck = 0;
    u8 fDrop = 0;
    s32 txTime = 0;
    s32 txDelayUs = 0;

    ZB_RADIO_RX_DISABLE;

    g_sysDiags.macRxIrqCnt++;

#if 0
    static s32 rfRxCnt = 0;
    s8 rssi = ZB_RADION_PKT_RSSI_GET(p) - 110;
    T_rxRssiBuf[rfRxCnt++ & 0x3f] = rssi;

    T_rxRssiPass[0]++;
    if(rssi < RSSI_PASS_THRESHOLD){
    	ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
		ZB_RADIO_RX_ENABLE;
		return;
    }
    T_rxRssiPass[1]++;
#endif

    /*************************************************************
     * rf_busyFlag & TX_BUSY, means a packet received before during csma delay call back interrupt called,
     * should drop this packet
     */
    if( (!ZB_RADIO_CRC_OK(p)) || (!ZB_RADIO_PACKET_LENGTH_OK(p)) || (rf_busyFlag&TX_BUSY) ){
    	/* diagnostics MAC layer receive error packet */
    	g_sysDiags.macRxCrcFail++;

    	ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
        ZB_RADIO_RX_ENABLE;
        return;
    }

    /* Parse necessary field to be used later */
    u8 len = (u8)ZB_RADIO_ACTUAL_PAYLOAD_LEN(p);
    u8 *macPld = p + ZB_RADIO_RX_HDR_LEN;


    /*----------------------------------------------------------
     *  Do the filter
     */
    u8 *pSrcAddr = zb_macDataFilter(macPld, len, &fDrop, &fAck);
    if(fDrop){
		/* Drop the packet and recover the DMA */
    	ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
		ZB_RADIO_RX_ENABLE;
		return;
    }

    /* switch to tx in advance to let the pll stable */
 	if(macPld[0] & MAC_FCF_ACK_REQ_BIT){
 		ZB_SWTICH_TO_TXMODE();
 		txTime = clock_time();
 	}

	/* if don't have enough buffer, use current rxBuf, and drop it */
	u8 *rxNextBuf = tl_getRxBuf();
	if(!rxNextBuf){
		if(macPld[0] & MAC_FCF_ACK_REQ_BIT){
			ZB_SWTICH_TO_RXMODE();
		}

    	/* diagnostics PHY to MAC queue limit */
    	g_sysDiags.phytoMACqueuelimitreached++;

		ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
		ZB_RADIO_RX_ENABLE;
		return;
	}

    /*----------------------------------------------------------
	 *  Send ACK
	 */
	if(macPld[0] & MAC_FCF_ACK_REQ_BIT){
		rf_ack_buf[ZB_RADIO_TX_HDR_LEN+2] = macPld[2];  //seqnno
		rf_ack_buf[ZB_RADIO_TX_HDR_LEN] = 0x02;

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
		u16 fcf = (macPld[1] << 8) | macPld[0];     // frame control
		u8  pldOft = tl_zbMacHdrSize(fcf);
		u8  frameType = (fcf & MAC_FCF_FRAME_TYPE_MASK) >> MAC_FCF_FRAME_TYPE_POS;

		bool cmdDataReq = ((frameType == MAC_FRAME_COMMAND) && (macPld[pldOft] == MAC_CMD_DATA_REQUEST));

		/* if has pending data, set the pending bit as 1 */
		u8 srcAddrMode = ZB_ADDR_NO_ADDR;
		if((macPld[1] & MAC_FCF_SRC_ADDR_BIT) == 0x80){
			srcAddrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
		}else if((macPld[1] & MAC_FCF_SRC_ADDR_BIT) == 0xc0){
			srcAddrMode = ZB_ADDR_64BIT_DEV;
		}
		if(cmdDataReq && srcAddrMode){
			extern u8 tl_zbMacPendingDataCheck(u8 addrMode, u8 *addr, u8 send);
			if(MAC_SUCCESS == tl_zbMacPendingDataCheck(srcAddrMode, pSrcAddr, 1)){
				rf_ack_buf[ZB_RADIO_TX_HDR_LEN] |= (1 << 4);// (rf_framePending<<4 );
			}
		}
#else
		(void)pSrcAddr;
#endif

		txDelayUs = (clock_time() - txTime) / S_TIMER_CLOCK_1US;
		if(txDelayUs < ZB_TX_WAIT_US){
			WaitUs(ZB_TX_WAIT_US - txDelayUs);
		}

		ZB_RADIO_TX_START(rf_ack_buf);//Manual Mode
		rf_busyFlag |= (TX_ACKPACKET|TX_BUSY);
		while(!ZB_RADIO_TX_DONE);
	}

	/* Use the backup buffer to receive next packet */
	rf_rxBuf = rxNextBuf;
	ZB_RADIO_RX_BUF_CLEAR(rf_rxBuf);
	ZB_RADIO_RX_BUF_SET(rf_rxBuf);
	ZB_RADIO_RX_ENABLE;

	/* zb_mac_receive_data handler */
	zb_macDataRecvHander(p, macPld, len, fAck, ZB_RADIO_TIMESTAMP_GET(p), ZB_RADION_PKT_RSSI_GET(p) - 110);
}


/*********************************************************************
 * @fn      rf_tx_irq_handler
 *
 * @brief   Tx Interrupt handler for RF module.
 *
 * @param   none
 *
 * @return  none
 */
_attribute_ram_code_ __attribute__((optimize("-Os"))) void rf_tx_irq_handler(void)
{
    g_sysDiags.macTxIrqCnt++;

    ZB_SWTICH_TO_RXMODE();

	rfMode = RF_STATE_RX;
    rf_busyFlag &= ~TX_BUSY;//Clear TX busy flag after receive TX done signal

    if(rf_busyFlag & TX_ACKPACKET){
    	rf_busyFlag &= ~TX_ACKPACKET;//Clear ACK TX done
    }else{
		zb_macDataSendHander();
	}
}

