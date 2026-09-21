/********************************************************************************************************
 * @file    rf_drv.h
 *
 * @brief   This is the header file for B85
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
#ifndef _RF_DRV_H_
#define _RF_DRV_H_

#include "bit.h"
#include "bsp.h"
#include "register.h"
#include "flash.h"
#include "analog.h"


#define RF_CHN_TABLE 		0x8000


/**
 *  @brief  Define RF mode
 */
typedef enum {
    RF_MODE_BLE_2M              = BIT(0),
    RF_MODE_BLE_1M              = BIT(1),
    RF_MODE_BLE_1M_NO_PN        = BIT(2),
    RF_MODE_ZIGBEE_250K         = BIT(3),
    RF_MODE_LR_S2_500K          = BIT(4),
    RF_MODE_LR_S8_125K          = BIT(5),
    RF_MODE_PRIVATE_250K        = BIT(6),
    RF_MODE_PRIVATE_500K        = BIT(7),
    RF_MODE_PRIVATE_1M          = BIT(8),
    RF_MODE_PRIVATE_2M          = BIT(9),
    RF_MODE_ANT                 = BIT(10),
    RF_MODE_BLE_2M_NO_PN        = BIT(11),
} RF_ModeTypeDef;


/**
 *  @brief  Define RF Tx/Rx/Auto mode
 */
typedef enum {
    RF_MODE_TX          = 0,
    RF_MODE_RX          = 1,
    RF_MODE_AUTO        = 2,
    RF_MODE_OFF         = 3
} RF_StatusTypeDef;

/**
 *  @brief  Define power list of RF
 */
typedef enum {
     /*VBAT*/
     RF_POWER_P10p46dBm = 63, //  10.46 dbm
     RF_POWER_P10p29dBm = 61, //  10.29 dbm
     RF_POWER_P10p01dBm = 58, //  10.01 dbm
     RF_POWER_P9p81dBm  = 56, //   9.81 dbm
     RF_POWER_P9p48dBm  = 53, //   9.48 dbm
     RF_POWER_P9p24dBm  = 51, //   9.24 dbm
     RF_POWER_P8p97dBm  = 49, //   8.97 dbm
     RF_POWER_P8p73dBm  = 47, //   8.73 dbm
     RF_POWER_P8p44dBm  = 45, //   8.44 dbm
     RF_POWER_P8p13dBm  = 43, //   8.13 dbm
     RF_POWER_P7p79dBm  = 41, //   7.79 dbm
     RF_POWER_P7p41dBm  = 39, //   7.41 dbm
     RF_POWER_P7p02dBm  = 37, //   7.02 dbm
     RF_POWER_P6p60dBm  = 35, //   6.60 dbm
     RF_POWER_P6p14dBm  = 33, //   6.14 dbm
     RF_POWER_P5p65dBm  = 31, //   5.65 dbm
     RF_POWER_P5p13dBm  = 29, //   5.13 dbm
     RF_POWER_P4p57dBm  = 27, //   4.57 dbm
     RF_POWER_P3p94dBm  = 25, //   3.94 dbm
     RF_POWER_P3p23dBm  = 23, //   3.23 dbm
     /*VANT*/
     RF_POWER_P3p01dBm  = BIT(7) | 63,   //   3.01 dbm
     RF_POWER_P2p81dBm  = BIT(7) | 61,   //   2.81 dbm
     RF_POWER_P2p61dBm  = BIT(7) | 59,   //   2.61 dbm
     RF_POWER_P2p39dBm  = BIT(7) | 57,   //   2.39 dbm
     RF_POWER_P1p99dBm  = BIT(7) | 54,   //   1.99 dbm
     RF_POWER_P1p73dBm  = BIT(7) | 52,   //   1.73 dbm
     RF_POWER_P1p45dBm  = BIT(7) | 50,   //   1.45 dbm
     RF_POWER_P1p17dBm  = BIT(7) | 48,   //   1.17 dbm
     RF_POWER_P0p90dBm  = BIT(7) | 46,   //   0.90 dbm
     RF_POWER_P0p58dBm  = BIT(7) | 44,   //   0.58 dbm
     RF_POWER_P0p04dBm  = BIT(7) | 41,   //   0.04 dbm
     RF_POWER_N0p14dBm  = BIT(7) | 40,   //  -0.14 dbm
     RF_POWER_N0p97dBm  = BIT(7) | 36,   //  -0.97 dbm
     RF_POWER_N1p42dBm  = BIT(7) | 34,   //  -1.42 dbm
     RF_POWER_N1p89dBm  = BIT(7) | 32,   //  -1.89 dbm
     RF_POWER_N2p48dBm  = BIT(7) | 30,   //  -2.48 dbm
     RF_POWER_N3p03dBm  = BIT(7) | 28,   //  -3.03 dbm
     RF_POWER_N3p61dBm  = BIT(7) | 26,   //  -3.61 dbm
     RF_POWER_N4p26dBm  = BIT(7) | 24,   //  -4.26 dbm
     RF_POWER_N5p03dBm  = BIT(7) | 22,   //  -5.03 dbm
     RF_POWER_N5p81dBm  = BIT(7) | 20,   //  -5.81 dbm
     RF_POWER_N6p67dBm  = BIT(7) | 18,   //  -6.67 dbm
     RF_POWER_N7p65dBm  = BIT(7) | 16,   //  -7.65 dbm
     RF_POWER_N8p65dBm  = BIT(7) | 14,   //  -8.65 dbm
     RF_POWER_N9p89dBm  = BIT(7) | 12,   //  -9.89 dbm
     RF_POWER_N11p4dBm  = BIT(7) | 10,   //  -11.4 dbm
     RF_POWER_N13p29dBm = BIT(7) | 8,   //  -13.29 dbm
     RF_POWER_N15p88dBm = BIT(7) | 6,   //  -15.88 dbm
     RF_POWER_N19p27dBm = BIT(7) | 4,   //  -19.27 dbm
     RF_POWER_N25p18dBm = BIT(7) | 2,   //  -25.18 dbm

     RF_POWER_N30dBm    = 0xff,         //  -30 dbm
     RF_POWER_N50dBm    = BIT(7) | 0,   //  -50 dbm
} RF_PowerTypeDef;

extern const RF_PowerTypeDef rf_power_Level_list[60];
/**
 *  @brief  Define power index list of RF
 */
typedef enum {
	 /*VBAT*/
	 RF_POWER_INDEX_P10p46dBm,
	 RF_POWER_INDEX_P10p29dBm,
	 RF_POWER_INDEX_P10p01dBm,
	 RF_POWER_INDEX_P9p81dBm,
	 RF_POWER_INDEX_P9p48dBm,
	 RF_POWER_INDEX_P9p24dBm,
	 RF_POWER_INDEX_P8p97dBm,
	 RF_POWER_INDEX_P8p73dBm,
	 RF_POWER_INDEX_P8p44dBm,
	 RF_POWER_INDEX_P8p13dBm,
	 RF_POWER_INDEX_P7p79dBm,
	 RF_POWER_INDEX_P7p41dBm,
	 RF_POWER_INDEX_P7p02dBm,
	 RF_POWER_INDEX_P6p60dBm,
	 RF_POWER_INDEX_P6p14dBm,
	 RF_POWER_INDEX_P5p65dBm,
	 RF_POWER_INDEX_P5p13dBm,
	 RF_POWER_INDEX_P4p57dBm,
	 RF_POWER_INDEX_P3p94dBm,
	 RF_POWER_INDEX_P3p23dBm,
	 /*VANT*/
	 RF_POWER_INDEX_P3p01dBm,
	 RF_POWER_INDEX_P2p81dBm,
	 RF_POWER_INDEX_P2p61dBm,
	 RF_POWER_INDEX_P2p39dBm,
	 RF_POWER_INDEX_P1p99dBm,
	 RF_POWER_INDEX_P1p73dBm,
	 RF_POWER_INDEX_P1p45dBm,
	 RF_POWER_INDEX_P1p17dBm,
	 RF_POWER_INDEX_P0p90dBm,
	 RF_POWER_INDEX_P0p58dBm,
	 RF_POWER_INDEX_P0p04dBm,
	 RF_POWER_INDEX_N0p14dBm,
	 RF_POWER_INDEX_N0p97dBm,
	 RF_POWER_INDEX_N1p42dBm,
	 RF_POWER_INDEX_N1p89dBm,
	 RF_POWER_INDEX_N2p48dBm,
	 RF_POWER_INDEX_N3p03dBm,
	 RF_POWER_INDEX_N3p61dBm,
	 RF_POWER_INDEX_N4p26dBm,
	 RF_POWER_INDEX_N5p03dBm,
	 RF_POWER_INDEX_N5p81dBm,
	 RF_POWER_INDEX_N6p67dBm,
	 RF_POWER_INDEX_N7p65dBm,
	 RF_POWER_INDEX_N8p65dBm,
	 RF_POWER_INDEX_N9p89dBm,
	 RF_POWER_INDEX_N11p4dBm,
	 RF_POWER_INDEX_N13p29dBm,
	 RF_POWER_INDEX_N15p88dBm,
	 RF_POWER_INDEX_N19p27dBm,
	 RF_POWER_INDEX_N25p18dBm,
} RF_PowerIndexTypeDef;

#define	RF_TRX_MODE                                     0xe0
#define	RF_TRX_OFF                                      0x45

/*According to the BLE packet structure, the maximum payload length is 255Bytes.
  Combined with the DMA len calculation formula, two bytes can cover all situations.
  changed by kunhe, confirmed by zhiwei; 20230904.*/
#define	RF_BLE_PACKET_LENGTH_OK(p)                      (((p[1] << 8) | p[0]) == p[5] + 13)
/*Change the DMA length index to payload length index. changed by kunhe, confirmed by zhiwei; 20230904.*/
#define	RF_BLE_PACKET_CRC_OK(p)                         ((p[p[5] + 13 + 3] & 0x01) == 0x0)

#define RF_ZIGBEE_PACKET_LENGTH_OK(p)                   (p[0] == p[4] + 9)
#define RF_ZIGBEE_PACKET_CRC_OK(p)                      ((p[p[0] + 3] & 0x51) == 0x10)
#define RF_ZIGBEE_PACKET_RSSI_GET(p)                    (p[p[0] + 2])
#define RF_ZIGBEE_PACKET_TIMESTAMP_GET(p)               (p[p[0] - 4] | (p[p[0] - 3] << 8) | (p[p[0] - 2] << 16) | (p[p[0] - 1] << 24))
#define RF_ZIGBEE_PACKET_PAYLOAD_LENGTH_GET(p)          (p[4])


/*******************************            General APIs            **************************/

/**
 * @brief      This function serves to reset RF BaseBand
 * @param[in]  none.
 * @return     none.
 */
static inline void reset_baseband(void)
{
    REG_ADDR8(0x61) |= BIT(0); //reset_baseband
    REG_ADDR8(0x61) &= ~BIT(0);//release reset signal

    reg_rst1 |= FLD_RST1_ZB;
    reg_rst1 &= (~FLD_RST1_ZB);
}

/**
 * @brief      This function serves to optimize RF performance
 *                         This function must be called every time rx is turned on,
 *                         and is called by an internal function.
 *                         If there are other requirements that need to be called,
 *                         turn off rx first, then call it again to make sure the Settings take effect
 * @param[in]  none
 * @return     none
 * @note           1.Call this function after turning on rx 30us, and the calibration value set by the function
 *                            will take effect after calling rf_ldot_ldo_rxtxlf_bypass_en;if automatic calibration is
 *                            required, you can use rf_ldot_ldo_rxtxlf_bypass_dis to turn off the bypass function; how to
 *                            use it can refer to bqb.c file or rf_emi_rx in emi.c
 *                         2. After using rf_ldot_ldo_rxtxlf_bypass_dis to turn off the bypass function and enter tx/rx
 *                            automatic calibration, to use this function again, you need to call the rf_set_rxpara function
 *                            again after entering rx 30us.
 *
 */
static inline void rf_set_rxpara(void)
{
    unsigned char reg_calibration = 0;

    reg_calibration = ((read_reg8(0x12ed) & 0xf) << 2) | ((read_reg8(0x12ec) & 0xc0) >> 6);
    if(reg_calibration > 7) reg_calibration -= 7;
    write_reg8(0x12e5, (read_reg8(0x12e5) & 0xc0) | reg_calibration);
}

/**
 * @brief      This function serves to initiate the mode of RF
 * @param[in]  rf_mode  -  mode of RF
 * @return     none.
 */
void rf_drv_init(RF_ModeTypeDef rf_mode);

/**
 * @brief       This function serves to set RF power.
 * @param[in]   RF_PowerTypeDef - the RF power types.
 * @return      none.
 */
void rf_set_power_level(RF_PowerTypeDef level);

/**
 * @brief       This function serves to set RF power level index.
 * @param[in]   RF_PowerIndexTypeDef - the RF power level index types.
 * @return      none.
 */
void rf_set_power_level_index(RF_PowerIndexTypeDef level);

/**
 * @brief          This function serves to get rssi
 * @param[in]      none
 * @return         none
 */
static inline signed char rf_rssi_get_154(void)
{
    return (((signed char)(read_reg8(0x441))) - 110);
}

/**
 * @brief       This function serves to set RF's channel.
 * @param[in]   chn - RF channel.
 * @param[in]   set - the value to set.
 * @return      none.
 */
void rf_set_channel(signed char chn, unsigned short set);

/**
 * @brief   This function serves to set RF access command.
 * @param[in]  acc - the command.
 * @return  none.
 */
static inline void rf_access_code_comm(unsigned int acc)
{
    write_reg32(0x800408, acc);
    //notice: This state will be reset after reset baseband
    //setting access code needs to writ 0x405 access code trigger bit 1 to enable in long range mode,,and the mode of  BLE1M and BLE2M need not.
    write_reg8(0x800405, read_reg8(0x405) | 0x80);
}

/**
 * @brief   This function serves to reset RF Tx/Rx mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_tx_rx_off(void)
{
    write_reg8(0xf16, 0x29);
    write_reg8(0x428, RF_TRX_MODE); //rx disable
    write_reg8(0xf02, RF_TRX_OFF);  //reset tx/rx state machine
}

/**
 * @brief   This function serves to turn off RF auto mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_tx_rx_off_auto_mode(void)
{
    write_reg8(0xf00, 0x80);
}

/**
 *      @brief          This function serves to judge RF Tx/Rx state.
 *      @param[in]      rf_status - Tx/Rx status.
 *      @param[in]      rf_channel - RF channel.
 *      @return         failed -1,else success.
 */
int rf_trx_state_set(RF_StatusTypeDef rf_status, signed char rf_channel);

/**
 *      @brief          This function serves to get RF status.
 *      @param[in]      none.
 *      @return         RF Rx/Tx status.
 */
RF_StatusTypeDef rf_trx_state_get(void);

/**
 * @brief   This function serves to set RF Tx mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_txmode(void)
{
    write_reg8(0xf02, RF_TRX_OFF | BIT(4));	//TX enable
}

/**
 * @brief   This function serves to set RF tx settle time.
 * @tx_stl_us  tx settle time,the unit is us.The max value of this param is 0xfff;The default settling time value is 150us.
 * 			   The typical value is 113us (tx_settle time should not be less than this value).
 * @return  none.
 * @note	   Attention:It is not necessary to call this function to adjust the settling time in the normal sending state.
 */
static inline void tx_settle_adjust(unsigned short txstl_us)
{
    REG_ADDR16(0xf04) = (txstl_us - 1);
}

/**
 * @brief   This function serves to set pipe for RF Tx.
 * @param   pipe - RF Optional range .
 * @return  none
 */
static inline void rf_set_tx_pipe(unsigned char pipe)
{
    write_reg8(0xf15, 0xf0 | pipe);
}

/**
 *      @brief          This function serves to set RF Tx packet.
 *      @param[in]      addr - the address RF to send packet.
 *      @return         none.
 */
void rf_tx_pkt(unsigned char *addr);

/**
 *      @brief          This function serves to determine whether sending a packet of data is finished
 *      @param[in]      none.
 *      @return         Yes: 1, NO: 0.
 */
static inline unsigned char rf_tx_finish(void)
{
    return ((read_reg8(0xf20) & BIT(1)) == 0x02);
}

/**
*	@brief	  	This function serves to clear the Tx finish flag bit.
*				After all packet data are sent, corresponding Tx finish flag bit
*				will be set as 1.By reading this flag bit, it can check whether
*				packet transmission is finished. After the check, its needed to
*				manually clear this flag bit so as to avoid misjudgment.
*   @param      none
*	@return	 	none
*/
static inline void rf_tx_finish_clear_flag(void)
{
    write_reg8(0xf20, 0x02);
}

/**
 *      @brief          This function serves to determine whether sending a packet of rf_tx_acc_code_select is finished
 *      @param[in]      none.
 *      @return         Yes: 1, NO: 0.
 */
static inline unsigned char rf_rx_finish(void)
{
    return ((read_reg8(0xf20) & BIT(0)) == 0x01);
}

/**
 *      @brief          This function is to set rx buffer
 *
 *      @param[out]     RF_RxAddr       Pointer for Rx buffer in RAM(Generally it is starting
 *                                                      address of an array.Should be 4-byte aligned)
 *      @param[in]      size            Rx buffer size (It is an integral multiple of 16)
 *      @param[in]      PingpongEn      Enable/Disable Ping-Pong buffer Enable or Disable
 *                                                      Note:
 *                                                      When PingpongEn is set as 0, received RF data will
 *                                                      be stored in RAM pointed by RF_RxAddr.
 *                                                      When PingpongEn is set as 1, received RF data will
 *                                                      be stored in buffer0 and buffer1 successively.
 *                                                      The RAM size reserved for received RF data should be
 *                                                      double of Size.
 *
 *      @return         none
 */
void rf_rx_buffer_set(unsigned char *RF_RxAddr, int size, unsigned char PingpongEn);

/**
 *      @brief          This function serves to  reconfigure the buffer
 *      @param[out]     RF_RxAddr       Pointer for Rx buffer in RAM(Generally it is starting
 *                                                      address of an array.Should be 4-byte aligned)
 *      @return         Yes: 1, NO: 0.
 */
static inline void rf_rx_buffer_reconfig(unsigned char *RF_RxAddr)
{
    reg_dma2_addr = (unsigned short)((unsigned int)RF_RxAddr);
}

/**
 *      @brief          This function serves to clear the Rx finish flag bit.
 *                              After all packet data are resive, corresponding Rx finish flag bit
 *                              will be set as 1.By reading this flag bit, it can check whether
 *                              packet transmission is finished. After the check, it's needed to
 *                              manually clear this flag bit so as to avoid misjudgment.
 *  @param      none
 *      @return         none
 */
static inline void rf_rx_finish_clear_flag(void)
{
    write_reg8(0xf20, 0x01);
}

/**
 * @brief      This function to correct cap value
 * @param[in]  addr - flash address, where is the correct cap value
 * @return     none
 */
static inline void rf_drv_cap(unsigned long addr)
{
    unsigned char frequency_offset_value = 0xff;

    flash_read_page(addr, 1, &frequency_offset_value);
    if (0xff != frequency_offset_value) {
        analog_write(0x8a, (analog_read(0x8a) & 0x40) | (frequency_offset_value & 0x3f));
    }
}

/**
 * @brief   This function serves to set RF Tx mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_rxmode(void)
{
    write_reg8(0x428, RF_TRX_MODE | BIT(0));
    write_reg8(0xf02, RF_TRX_OFF | BIT(5));
}

/**
 *      @brief          This function serves to start Tx.
 *      @param[in]      addr   Tx packet address in RAM. Should be 4-byte aligned.
 *      @param[in]      tick  Tick value of system timer.
 *      @return         none
 */
void rf_start_stx(void *addr, unsigned int tick);

/**
 *      @brief          This function serves to start Rx.
 *      @param[in]      tick  Tick value of system timer.
 *      @return         none
 */
void rf_start_srx(unsigned int tick);

/**
 * @brief   This function serves to set the ble channel.
 * @param   chn_num - channel numbers.
 * @return  none.
 */
void rf_set_ble_channel(signed char chn_num);

/**
 * @brief     This function performs to enable RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_on(void)
{
    write_reg8(0xf02, 0x45 | BIT(4));  // TX enable
}

/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_done(void)
{
    write_reg8(0xf02, 0x45);
}

/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 */
static inline void reset_sn_nesn(void)
{
    REG_ADDR8(0xf01) = 0x01;
}

/**
 * @brief   This function serves to reset the RF sn.
 * @param   none.
 * @return  none.
 */
static inline void rf_reset_sn(void)
{
    write_reg8(0xf01, 0x3f);
    write_reg8(0xf01, 0x00);
}

/**
 * @brief   This function serves to set CRC value for RF.
 * @param[in]  crc - CRC value.
 * @return  none.
 */
static inline void rf_set_ble_crc_value(unsigned int crc)
{
    write_reg32(0x424, crc);
}

/**
 * @brief   This function serves to set CRC advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_ble_crc_adv(void)
{
    write_reg32(0x424, 0x555555);
}

/**
 * @brief   This function serves to set RF access code value.
 * @param[in]   ac - the address value.
 * @return  none
 */
static inline void rf_set_ble_access_code_value(unsigned int ac)
{
    write_reg32(0x408, ac);
}

/**
 * @brief   This function serves to judge the statue of  RF receive.
 * @param   none.
 * @return  none.
 */
static inline unsigned char is_rf_receiving_pkt(void)
{
    //if the value of [5:4] of the reg_0x448 is 0b10 or 0b11, it means that the RF is in the receiving packet phase.(confirmed by junwen)
    return (((read_reg8(0x448) >> 5) & 1) == 1);
}

/**
 * @brief      This function
 * @param[in]  size - the expected size of dma2
 * @param[in]  PingpongEn - the flag of g_RFRxPingpongEn
 * @return     none
 */
void rf_rx_cfg(int size, unsigned char PingpongEn);

/**
 * @brief      This function is to disable PN
 * @return     none
 */
void rf_pn_disable(void);

/**
 * @brief           This function is used to enable the ldo rxtxlf bypass function, and the calibration value
 *                              written by the software will take effect after enabling.
 * @param[in]   none.
 * @return              none.
 */
static inline void rf_ldot_ldo_rxtxlf_bypass_en(void)
{
    write_reg8(0x12e4, read_reg8(0x12e4) | BIT(1));
}

/**
 * @brief           This function is used to close the ldo rxtxlf bypass function, and the hardware will
 *                              automatically perform the calibration function after closing.
 * @param[in]   none.
 * @return              none.
 */
static inline void rf_ldot_ldo_rxtxlf_bypass_dis(void)
{
    write_reg8(0x12e4, read_reg8(0x12e4) & (~BIT(1)));
}

/**
 * @brief          This function serves to set the which irq enable.
 * @param[in]      mask     - Options that need to be enabled.
 * @return         Yes: 1, NO: 0.
 */
static inline void rf_set_irq_mask(rf_irq_e mask)
{
    BM_SET(reg_rf_irq_mask, mask);
}

/**
 * @brief          This function is used to clear the irq mask that needs to be cleared.
 * @param[in]      mask     - Options that need irq value.
 * @return         none.
 */
static inline void rf_clr_irq_mask(rf_irq_e mask)
{
    BM_CLR(reg_rf_irq_mask, mask);
}

/**
 * @brief          This function serves to judge whether it is in a certain state.
 * @param[in]      mask     - Options that need irq status.
 * @return         Yes: 1, NO: 0.
 */
static inline unsigned short rf_get_irq_status(rf_irq_e status)
{
    return ((unsigned short)BM_IS_SET(reg_rf_irq_status, status));
}

/**
 * @brief        This function serves to clear the irq finish flag bit that needs to be cleared.
 *              When the status flag bit is 1, this flag bit needs to be cleared manually to avoid the next misjudgment.
 * @param[in]    status     - Options that need irq status.
 * @return       none.
 */
static inline void rf_clr_irq_status(rf_irq_e status)
{
     reg_rf_irq_status = status;
}

#endif
