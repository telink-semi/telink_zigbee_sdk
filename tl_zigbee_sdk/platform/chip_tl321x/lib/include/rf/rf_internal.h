/********************************************************************************************************
 * @file    rf_internal.h
 *
 * @brief   This is the header file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef     RF_INTERNAL_H
#define     RF_INTERNAL_H

#include "lib/include/sys.h"
#include "lib/include/rf/rf_dma.h"
#include "lib/include/rf/rf_common.h"
#include "gpio.h"
#include "lib/include/stimer.h"

#if(0)
/******************************************************FOR HADM************************************************************/

/**
 *  @brief This macro is used to enable interfaces related to HADM
 */
#define     RF_HADM_EN              0

/**********************************************************************************************************************
 *                                       RF global data type                                                          *
 *********************************************************************************************************************/

#if RF_HADM_EN
/**
 * @brief   Select how you want to start IQ sampling.
 */
typedef enum
{
    RF_HADM_IQ_SAMPLE_SYNC_MODE,
    RF_HADM_IQ_SAMPLE_RXEN_MODE
}rf_hadm_iq_sample_mode_e;

/**
 * @brief   Select whether the antenna clock is normally open or turned on when the antenna is switched.
 */
typedef enum
{
    RF_HADM_ANT_CLK_ALWAYS_ON_MODE,
    RF_HADM_ANT_CLK_SWITCH_ON_MODE
}rf_hadm_ant_clk_mode_e;
#endif

/**
 * @brief   select baseband transmission unit
 */
typedef enum{
    RF_WORLD_WIDTH  = 0,
    RF_DWORLD_WIDTH = 1,
    RF_QWORLD_WIDTH = 2,
}rf_trans_unit_e;

typedef enum{
    RF_NORMAL_LENGTH_PACKET     = 0,
    RF_HYPER_LENGTH_PACKET      = 1,
    RF_ADV1_HYPER_LENGTH_PACKET = 2,
    RF_ADV2_HYPER_LENGTH_PACKET = 3
}rf_pkt_len_mode_e;

#if RF_HADM_EN
typedef enum{
    RF_RX_ACL_AOA_AOD_EN  = BIT(0),
    RF_RX_ADV_AOA_AOD_EN  = BIT(1),
    RF_TX_ACL_AOA_AOD_EN  = BIT(2),
    RF_TX_ADV_AOA_AOD_EN  = BIT(3),
    RF_AOA_AOD_OFF        = 0
}rf_aoa_aod_mode_e;

/*
 * @brief  Data length type of AOA/AOD sampling.
 * |                 |                         |
 * | :-------------- | :---------------------- |
 * |     <15:8>      |          <7:0>          |
 * |   IQ byte len   |   iq data bit num mode  |
 */
typedef enum{
    IQ_8_BIT_MODE           = 0x0200,
    IQ_16_BIT_MODE          = 0x0401,
    IQ_16_BIT_LOW_MODE      = 0x0402,
    IQ_16_BIT_HIGH_MODE     = 0x0403,
    IQ_20_BIT_MODE          = 0x0504
}rf_iq_data_mode_e;



/*
 * @brief  AOA/AOD sample interval time type enumeration.
 * @note   Attention:When the time is 0.25us, it cannot be used with the 20bit iq data type, which will cause the sampling data to overflow.
 *         In normal mode, the sampling interval of AOA is 4us, and AOD will judge whether the sampling interval is 4us or 2us according to
 *         CTE info.
 */
typedef enum{
    SAMPLE_NORMAL_INTERVAL      = 0,//In this case sample interval of aoa is 4us, and aod will judge sample interval is 4us or 2us according to CTE info.
    SAMPLE_2US_INTERVAL         = 3,
    SAMPLE_1US_INTERVAL         = 4,
    SAMPLE_0P5US_INTERVAL       = 5,
    SAMPLE_0P25US_INTERVAL      = 6
}rf_aoa_aod_sample_interval_time_e;
#endif

/**
 *  @brief  set the modulation index.
 */
typedef enum {
    RF_MI_P0p00 = 0,          /**< MI = 0 */
    RF_MI_P0p076 = 76,          /**< MI = 0.076 */
    RF_MI_P0p32 = 320,          /**< MI = 0.32 */
    RF_MI_P0p50 = 500,          /**< MI = 0.5 */
    RF_MI_P0p60 = 600,          /**< MI = 0.6 */
    RF_MI_P0p70 = 700,          /**< MI = 0.7 */
    RF_MI_P0p80 = 800,          /**< MI = 0.8 */
    RF_MI_P0p90 = 900,          /**< MI = 0.9 */
    RF_MI_P1p20 = 1200,     /**< MI = 1.2 */
    RF_MI_P1p30 = 1300,     /**< MI = 1.3 */
    RF_MI_P1p40 = 1400,     /**< MI = 1.4 */
}rf_mi_value_e;

#if RF_HADM_EN
/**
 * @brief   Define function to set tx channel or rx channel.
 */
typedef enum
{
    TX_CHANNEL      = 0,
    RX_CHANNEL      = 1,
}rf_trx_chn_e;
#endif

typedef struct {
    unsigned int pn_value;
    unsigned int pn_poly;
    unsigned char rf_pn_length;
    unsigned char pn_start_byte;
} rf_pn_config_t;

/**********************************************************************************************************************
 *                                         RF function declaration                                                    *
 *********************************************************************************************************************/

/**
 * @brief This function servers to set rf baseband timer irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static inline void rf_bb_timerset_irq_mask(rf_bb_timer_irq_e mask)
{
    reg_bb_timer_irq_mask |= mask;
}

/**
 * @brief This function servers to clear rf baseband timer irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static inline void rf_bb_timer_clr_irq_mask(rf_bb_timer_irq_e mask)
{
    reg_bb_timer_irq_mask &= (~mask);
}

/**
 * @brief This function servers to clear rf baseband timer irq status.
 * @param[in]   status - the irq status.
 * @return      none.
 */
static inline void rf_bb_timer_clr_irq_status(rf_bb_timer_irq_e status)
{
    reg_bb_timer_cal_irq = (status);
}

/**
 * @brief This function servers to get rf baseband timer irq status.
 * @param[in]   status - the irq status.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned char rf_bb_timer_get_irq_status(rf_bb_timer_irq_e status)
{
    return (reg_bb_timer_cal_irq & status);
}

/**
 * @brief This function servers to set tick irq capture.
 * @param[in] tick - the value of irq tick.
 * @return    none.
 */
static inline void rf_bb_timer_set_irq_capture(unsigned int tick)
{
    reg_bb_timer_irq_level = (tick);
}

/**
 * @brief This function servers to set rf baseband timer  tick.
 * @param[in] tick - the value of tick.
 * @return    none.
 */
static inline void rf_bb_timer_set_tick(unsigned int tick)
{
    reg_bb_timer_tick = (tick);
}

/**
 * @brief       This function is used to enable the ldo rxtxlf bypass function, and the calibration value
 *              written by the software will take effect after enabling.
 * @param[in]   none.
 * @return      none.
 */
static inline void rf_ldot_ldo_rxtxlf_bypass_en(void)
{
    write_reg8(0x17074e,0x45);//CBPF_TRIM_I && CBPF_TRIM_Q
    write_reg8(0x17074c,0x02);//LNA_ITRIM=0x01(default)(change to 0x02[TBD])
    write_reg8(0x1706e4,read_reg8(0x1706e4)|BIT(1));
}

/**
 * @brief       This function is used to close the ldo rxtxlf bypass function, and the hardware will
 *              automatically perform the calibration function after closing.
 * @param[in]   none.
 * @return      none.
 */
static inline void rf_ldot_ldo_rxtxlf_bypass_dis(void)
{
    write_reg8(0x17074e,0x40);//CBPF_TRIM_I && CBPF_TRIM_Q
    write_reg8(0x17074c,0x11);//LNA_ITRIM=0x01(default)(change to 0x02[TBD])
    write_reg8(0x1706e4,read_reg8(0x1706e4)&(~BIT(1)));
}

/**
 * @brief      This function serves to optimize RF performance
 *             This function must be called every time rx is turned on,
 *             and is called by an internal function.
 *             If there are other requirements that need to be called,
 *             turn off rx first, then call it again to make sure the Settings take effect
 * @param[in]  none
 * @return     none
 * @note       1.Call this function after turning on rx 30us, and the calibration value set by the function
 *                will take effect after calling rf_ldot_ldo_rxtxlf_bypass_en;if automatic calibration is
 *                required, you can use rf_ldot_ldo_rxtxlf_bypass_dis to turn off the bypass function; how to
 *                use it can refer to bqb.c file or rf_emi_rx in emi.c
 *             2. After using rf_ldot_ldo_rxtxlf_bypass_dis to turn off the bypass function and enter tx/rx
 *                automatic calibration, to use this function again, you need to call the rf_set_rxpara function
 *                again after entering rx 30us.
 *
 */

static inline void rf_set_rxpara(void)
{
    unsigned char reg_calibration=0;
    reg_calibration = ((read_reg8(0x1706ed)&0xf)<<2)|((read_reg8(0x1706ec)&0xc0)>>6);
    if(reg_calibration>10)  reg_calibration -= 10;
    write_reg8(0x1706e5,(read_reg8(0x1706e5)&0xc0)|reg_calibration);
}

/**
 * @brief   This function serve to judge whether it is in a certain state.
 * @param[in]   status  - Option of rf state machine status.
 * @return      -#0:Not in parameter setting state
 *              -#1:In parameter setting state
 */
static inline unsigned short rf_get_state_machine_status(state_machine_status_e status)
{
    return  status == read_reg8(0x170224);
}

/**
 * @brief   This function serve to get ptx initial pid value.
 * @return  The  value of ptx pid before update.
 */
static inline unsigned char rf_get_ptx_pid(void)
{
    return ((reg_rf_ll_ctrl_1 & 0xc0)>>6);
}

/**
 * @brief     This function serves to set hybee_1M_old  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_1M_old_mode(void);

/**
 * @brief     This function serves to set hybee_2M_old  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_2M_old_mode(void);

/**
 * @brief     This function serves to set hybee_500K_new  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_500K_new_mode(void);

/**
 * @brief     This function serves to set hybee_1M_new  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_1M_new_mode(void);

/**
 * @brief     This function serves to set hybee_2M_new  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_2M_new_mode(void);

/**
 * @brief     This function serves to  set hybee_500K_2byte_sfd mode of RF.
 * @return    none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_500K_2byte_sfd_mode(void);

/**
 * @brief     This function serves to  set hybee_1M_2byte_sfd mode of RF.
 * @return    none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_1M_2byte_sfd_mode(void);

/**
 * @brief     This function serves to  set hybee_2M_2byte_sfd mode of RF.
 * @return    none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_2M_2byte_sfd_mode(void);

/**
 * @brief     This function serves to  set hybee_2M_2byte_sfd_new of RF.
 * @return    none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_hybee_2M_2byte_sfd_new_mode(void);

/**
 * @brief     This function serves to set zigbee_hr_2m  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_zigbee_hr_2m_mode(void);

/**
 * @brief     This function serves to set lowrate_20K  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_lowrate_20K_mode(void);

/**
 * @brief     This function serves to set lowrate_25K  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_lowrate_25K_mode(void);

/**
 * @brief     This function serves to set lowrate_100K  mode of RF.
 * @return     none.
 * @note      TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_lowrate_100K_mode(void);


/**
 * @brief       This function is mainly used to set the energy when sending a single carrier.
 * @param[in]   level       - The slice corresponding to the energy value.
 * @return      none.
 */
void rf_set_power_level_singletone(rf_power_level_e level);

/**
 * @brief       This function is used to  set the modulation index of the receiver.
 *              This function is common to all modes,the order of use requirement:configure mode first,
 *              then set the the modulation index,default is 0.5 in drive,both sides need to be consistent
 *              otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]   mi_value- the value of modulation_index*100.
 * @return      none.
 */
void rf_set_rx_modulation_index(rf_mi_value_e mi_value);

/**
 * @brief       This function is used to  set the modulation index of the sender.
 *              This function is common to all modes,the order of use requirement:configure mode first,
 *              then set the the modulation index,default is 0.5 in drive,both sides need to be consistent
 *              otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]   mi_value- the value of modulation_index*100.
 * @return      none.
 */
void rf_set_tx_modulation_index(rf_mi_value_e mi_value);


/****************************************************************************************************************************************
 *                                         RF : Private generic configuration interface function                                                                *
 ****************************************************************************************************************************************/



/**
 * @brief     This function sets the bit order of the length field.
 * @param[in] length_ord  - The bit order of LENGTH field.
 *            0: LSBit first  1: MSBit first
 * @return    none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_set_pri_generic_length_bit_ord(unsigned char length_bit_ord);

/****************************************************************************************************************************************
 *                                         RF : Zigbee/hybee New Package Format Configuration Interface                                                             *
 ****************************************************************************************************************************************/
/**
 *  @brief      This function serves to disable zigbee/hybee PHR.
 *  @return     none.
 */
void rf_hybee_phr_dis(void);

/**
 *  @brief      This function serves to set_no_phr_payload_len.
 *  @param[in]  tx_pkt_length- zigbee/hybee PSDU length(PSDU length range:0~0x7ff)
 *  @return     none.
 */
void rf_hybee_set_no_phr_payload_len(unsigned int tx_pkt_length);

/**
 *  @brief      This function serves to enable phr extend.
 *  @return     none.
 */
void rf_hybee_phr_extend_en(void);

/**
 *  @brief      This function serves to disable hybee phr extend.
 *  @return     none.
 */
void rf_hybee_phr_extend_dis(void);

/**
 *  @brief      This function serves to set_hybee_gap_len.
 *  @param[in]  gap_length - zigbee/hybee gap length(Gap length range:0~63)
 *  @return     none.
 */
void rf_hybee_set_gap_len(unsigned char gap_length);


/****************************************************************************************************************************************
 *                                         RF : ptx/prx Configuration Interface                                                             *
 ****************************************************************************************************************************************/

/**
 * @brief   This function serve to set the private ack enable,mainly used in prx/ptx.
 * @param[in]   none
 * @return      none
 */
void rf_set_ptx_prx_ack_en(void);

/**
 * @brief   This function serve to initial the ptx setting.
 * @return  none.
 */
void rf_ptx_config(void);

/**
 * @brief   This function to set retransmit and retransmit delay.
 * @param[in]   retry_times - Number of retransmit, 0: retransmit OFF
 * @param[in]   retry_delay - Retransmit delay time.
 * @return      none.
 */
void rf_set_ptx_retry(unsigned char retry_times, unsigned short retry_delay);

/**
 * @brief   This function serves to set RF ptx trigger.
 * @param[in]   addr    -   The address of tx_packet.
 * @param[in]   tick    -   Trigger ptx after (tick-current tick),If the difference is less than 0, trigger immediately.
 * @return  none.
 * @note        addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void rf_start_ptx(unsigned int tick);

/**
 * @brief   This function serve to initial the prx setting.
 * @return  none.
 */
void rf_prx_config(void);

/**
 * @brief   This function is used to set the initial PID value of PTX.
 * @param[in]   tx_pid  -Initial value of pid for PTX. (PID range:0~3)
 * @return  none
 */
static inline void rf_set_ptx_init_pid(unsigned char tx_pid)
{
    reg_rf_ll_ctrl_1 |= ((tx_pid&0x03) << 6);
}

/**
 * @brief   This function is used to set the initial PID value of PRX.
 * @param[in]   rx_pid  -Initial value of pid for PTX. (PID range:0~3)
 * @return  none
 */
static inline void rf_set_prx_init_pid(unsigned char rx_pid)
{
    reg_rf_ll_ctrl_1 |= ((rx_pid&0x03) << 4);
}

/**
 * @brief   This function serve to set the private ack enable,mainly used in prx/ptx.
 * @param[in]   rf_mode     -   Must be one of the private mode.
 * @return      none
 */
void rf_set_pri_tx_ack_en(rf_mode_e rf_mode);

/**
 * @brief   This function serves to set RF prx trigger.
 * @param[in]   tick    -   Trigger prx after (tick-current tick),If the difference is less than 0, trigger immediately.
 * @return  none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_start_prx(unsigned int tick);

/**
 * @brief   This function serves to judge whether the FIFO is empty.
 * @param pipe_id specify the pipe.
 * @return TX FIFO empty bit.
 *          -#0 TX FIFO NOT empty.
 *          -#1 TX FIFO empty.
 */
unsigned char rf_is_rx_fifo_empty(unsigned char pipe_id);

/**
 * @brief   This function serve to disable the private ack ,mainly used in prx/ptx.
 * @param[in]   none
 * @return      none
 */
void rf_set_ptx_prx_ack_dis(void);


/**
 * @brief   This function serve to initial the ptx/prx setting.
 * @return  none.
 */
void rf_ptx_prx_config(void);

/**
 * @brief   This function serve to enable new/old crc value check for PRX new/old pkt
 * @return  none.
 */
void rf_prx_crc_check_en(void);

/**
 * @brief   This function serve to disable new/old crc value check for PRX new/old pkt
 * @return  none.
 */
void rf_prx_crc_check_dis(void);

#if(0)
//ble Old path mode
/**
 * @brief     This function serves to set ble_500K  mode of RF.
 * @return    none.
 * @note      TODO:Old path mode, not released temporarily.
 */
void rf_set_ble_500K_mode(void);

/**
 * @brief     This function serves to  set ble_125K  mode of RF.
 * @return    none.
 * @note      TODO:Old path mode, not released temporarily.
 */
void rf_set_ble_125K_mode(void);
#endif

/****************************************************************************************************************************************
 *                                         RF : Ble hardware encryption interface function                                                              *
 ****************************************************************************************************************************************/

/**
 *  @brief      This function serves to set the encryption master mode for ble.
 *  @return     none.
 */
void rf_set_ble_crypt_master_mode(void);

/**
 *  @brief      This function serves to set the encryption slave mode for ble.
 *  @return     none.
 */
void rf_set_ble_crypt_slave_mode(void);

/**
 *  @brief      This function serves to enable ble crypt mode
 *  @return     none.
 */
void rf_ble_crypt_en(void);

/**
 *  @brief      This function serves to set ble crypt
 *  @param[in]  *skey - aes key: 128 bit key for the encryption of the data
 *  @param[in]  *iv - Initialization Vector:64 bit Initialization Vector
 *  @param[in]  *txccmpktcnt - 40 bit tx ccm packet cnt
 *  @param[in]  *rxccmpktcnt - 40 bit rx ccm packet cnt
 *  @return      none.
 */
void rf_ble_crypt_setup(unsigned char* skey, unsigned char* iv);

/**
*  @brief      This function serves to set ble crypt tx cnt
*  @param[in]  *txccmpktcnt - 40 bit tx ccm packet cnt
*  @param[in]  *rxccmpktcnt - 40 bit rx ccm packet cnt
*/
void rf_set_ble_crypt_tx_cnt(unsigned char* txccmpktcnt);

/**
*  @brief      This function serves to set ble crypt rx cnt
*  @param[in]  *rxccmpktcnt - 40 bit rx ccm packet cnt
*/
void rf_set_ble_crypt_rx_cnt(unsigned char* rxccmpktcnt);

/**
 *  @brief   This function serves to disable the tx ccm packet count auto-update feature
 *  @return  none.
 */
void rf_ble_crypt_txpktcnt_updata_dis(void);

/**
 *  @brief   This function serves to disable the rx ccm packet count auto-update feature
 *  @return  none.
 */
void rf_ble_crypt_rxpktcnt_updata_dis(void);

/**
 *  @brief   This function serves to reset the tlk_crypt module in software.
 *  @return  none.
 */
void rf_ble_crypt_software_rst(void);

/****************************************************************************************************************************************
 *                                         RF : zigbee dual channel interface function                                                              *
 ****************************************************************************************************************************************/

/**
 *  @brief      This function serves to set the range of chn group corresponding to the process of obtaining fcal calibration values at different frequency points
 *  @param[in]  *fcal_chn_range  - chn group range pointer(chn_num < fcal_chn_range)
 *  @return     none
 *  @note       If the frequency point is set using the rf_set_dual_chn() interface when obtaining FCAL calibration values for different chn,
 *              this interface needs to be used to set the range of chn.
 *
*/
void rf_set_fcal_chn_group_range_ctn(unsigned char *fcal_chn_range);

/**
 * @brief       This function serves to set rf channel for zigbee double channel.The actual channel set by this function is 2402+chn*2.
 * @param[in]   chn  -That you want to set the channel as 2402+chn*2.
 * @param[in]   chn1 -That you want to set the channel1 as 2402+chn*2.
 * @return      none.
 */
void rf_zigbee_set_rx_dual_chn(signed char chn,signed char chn1);


/****************************************************************************************************************************************
 *                                         RF User-defined package related functions                                                    *
 ****************************************************************************************************************************************/

/**
 * @brief       This function serves to enable user-defined packet sending function.
 *              In this mode, users can define the meaning of the contents of the package according to their own needs.
 *              Only ble1M_NO_PN mode supports user-defined packet.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:Only ble1M_NO_PN mode supports user-defined packet.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_ble1m_no_pn_tx_customer_mode_en(void)
{
    BM_SET(reg_rf_tx_mode2,FLD_RF_R_CUSTOM_MADE);
}

/**
 * @brief       This function serves to disable user-defined packet sending function.
 *              By default, the user-defined packet sending function is turned off, and normal protocol packets are sent;
 *              the function rf_ble1M_no_pn_tx_customer_mode_en needs to be called when the user-defined packet sending
 *              function is needed.
 *              Only ble1M_NO_PN mode supports user-defined packet.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:Only ble1M_NO_PN mode supports user-defined packet.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_ble1m_no_pn_tx_customer_mode_dis(void)
{
    BM_CLR(reg_rf_tx_mode2,FLD_RF_R_CUSTOM_MADE);
}

/**
 * @brief       This function serves to disable user-defined packet sending function.
 *              After enabling the hardware to calculate the CRC, the hardware will automatically
 *              add the CRC information to the header information when sending and receiving packets.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:The hardware CRC is enabled by default. If you need to turn off this function,
 *              call the rf_tx_hw_crc_dis function.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_tx_hw_crc_en(void)
{
    BM_SET(reg_rf_tx_mode1,FLD_RF_CRC_EN);
}

/**
 * @brief       This function is used to disable the hardware to calculate the value of crc.
 *              After disabling the hardware to calculate the CRC, if you want to add the CRC
 *              information to the package, you need to calculate it by the user software, and
 *              then add it to the package information.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:In this case, the receiver can only use software to check whether
 *              the CRC is wrong, and the hardware CRC flag is no longer accurate.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_tx_hw_crc_dis(void)
{
    BM_CLR(reg_rf_tx_mode1,FLD_RF_CRC_EN);
}



/****************************************************************************************************************************************
 *                                         RF : functions used by hyperlength                                                           *
 ****************************************************************************************************************************************/


/**
 * @brief       This function is used to set tx  package of ordinary length format or a certain hyper length format package.
 *              After configuring rf initialization, call this function to set the packet length type, the default is the
 *              normal length.If it is powered on to send a normal-length packet, this function does not need to be called.
 * @param[in]   mode - Packet length mode.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_tx_pkt_len_mode(rf_pkt_len_mode_e mode)
{
    reg_rf_tx_hlen_mode = mode;
}

/**
 * @brief       This function is used to set rx  package of ordinary length format or a certain hyper length format package.
 *              After configuring the RF initialization, call this function to configure the length mode of the receiving
 *              packet before entering the packet receiving mode. The default is the normal length. If it is powered on to
 *              receive a normal length packet, this function does not need to be called.
 * @param[in]   mode - Packet length mode.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_rx_pkt_len_mode(rf_pkt_len_mode_e mode)
{
    reg_rf_burst_size = ((reg_rf_burst_size & (~FLD_RF_RX_HLEN_MODE))|(mode << 2));
}


/**
 * @brief       This function is mainly used to enable the sending and receiving of BIS/CIS format packets.After initializing the RF,
 *              you can choose whether you need to call this function according to whether you need to send or receive a BIS/CIS format
 *              packet. Power-on initialization RF defaults to receive and send packets in common format.
 * @param[in]   none.
 * @return      none.
 * @note        Attention:These two package formats do not support AOA and AOD functions temporarily.
 *              TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_bis_cis_en(void)
{
    BM_SET(reg_rf_rxtmaxlen1,FLD_RF_RX_ISO_PDU);
}

/**
 * @brief       This function disables the new BIS and CIS packet formats in core5.2.
 *              After enabling the BIS/CIS function, if you want to resume sending packets in the normal format, you need to call this
 *              function to turn off the BIS/CIS function.
 * @param[in]   none.
 * @return      none.
 * @note        TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
static inline void rf_set_bis_cis_dis(void)
{
    BM_CLR(reg_rf_rxtmaxlen1,FLD_RF_RX_ISO_PDU);
}

#if RF_HADM_EN
/****************************************************************************************************************************************
 *                                         RF : AOA/AOD related functions                                                               *
 ****************************************************************************************************************************************/

/**
 * @brief       This function is used to set how many words as the transmission unit of baseband and dma.
 *              You don't need to call this function for normal use. By default, the unit is 1 world!
 *              After configuring the DMA, call this function to adjust the DMA rate.
 * @param[in]   rf_trans_unit_e size    - the unit of burst size .Identify how many bytes of data are
 *                                        handled by DMA each time
 * @return      none.
 */
void rf_set_baseband_trans_unit(rf_trans_unit_e size);

/**
 * @brief       This function enables the sending and receiving functions of AOA/AOD in ordinary format packets or ADV format packets.
 *              After configuring the RF function, if you want to send or receive a packet with AOA/AOD information, you can call this
 *              function to make the chip enter the corresponding mode to receive or send the packet. The default state is a normal
 *              package without AOA/AOD information.
 * @param[in]   mode - AOA/AOD broadcast package or normal package trx mode.
 * @return      none.
 */
static inline void rf_aoa_aod_set_trx_mode(rf_aoa_aod_mode_e mode)
{
    reg_rf_rxsupp = ((reg_rf_rxsupp & 0xf0) | mode);
}

/**
 * @brief       This function is used to calibrate AOA, AOD sampling frequency offset.This function is mainly used to set the position
 *              of iq data sampling point in sampleslot to optimize the sampling data result. By default, sampling is performed at the
 *              middle position of iqsampleslot, and the sampling point is 0.125us ahead of time for each decrease of 1 code.
 *              Each additional code will move the sampling point back by 0.125us
 * @param[in]   samp_locate:Compare the parameter with the default value, reduce one code to advance 0.125us, increase or decrease 1 to move
 *                          back 0.125us.
 * @return      none.
 */
static inline void rf_aoa_aod_sample_point_adjust(unsigned char samp_locate)
{
    reg_rf_samp_offset = samp_locate;
}


/**
 * @brief       This function is used to set the position of the first antenna switch after the reference.The default is in the middle of the
 *              first switch_slot; and the switch point is 0.125us ahead of time for each decrease of 1 code.
 *              Each additional code will move the switch point back by 0.125us
 * @param[in]   swt_offset : Compare the parameter with the default value, reduce 1 to advance 0.125us, increase or decrease 1 to move
 *                          back 0.125us.
 * @return      none.
 */
void rf_aoa_rx_ant_switch_point_adjust(unsigned short swt_offset);


/**
 * @brief       This function is used to set the position of the first antenna switch after the AOD transmitter reference.The default is in the middle of the
 *              first switch_slot; and the switch point is 0.125us ahead of time for each decrease of 1 code. Each additional code will move
 *              the switch point back by 0.125us
 * @param[in]   swt_offset : Compare the parameter with the default value, reduce 1 to advance 0.125us, increase or decrease 1 to move
 *                          back 0.125us.
 * @return      none.
 */
void rf_aod_tx_ant_switch_point_adjust(unsigned short swt_offset);


/**
 * @brief       This function is mainly used to set the IQ data sample interval time. In normal mode, the sampling interval of AOA is 4us, and AOD will judge whether
 *              the sampling interval is 4us or 2us according to CTE info.The 4us/2us sampling interval corresponds to the 2us/1us slot mode stipulated in the protocol.
 *              Since the current hardware only supports the antenna switching interval of 4us/2us, setting the sampling interval to 1us or less will cause multiple
 *              sampling at the interval of one antenna switching. Therefore, the sampling data needs to be processed by the upper layer according to the needs, and
 *              currently it is mostly used Used in the debug process.
 *              After configuring RF, you can call this function to configure slot time.
 * @param[in]   time_us - AOA or AOD slot time mode.
 * @return      none.
 * @note        Attention:(1)When the time is 0.25us, it cannot be used with the 20bit iq data type, which will cause the sampling data to overflow.
 *                        (2)Since only the antenna switching interval of 4us/2us is supported, the sampling interval of 1us and shorter time intervals
 *                            will be sampled multiple times in one antenna switching interval. Suggestions can be used according to specific needs.
 */
void rf_aoa_aod_sample_interval_time(rf_aoa_aod_sample_interval_time_e time_us);


/**
 * @brief       This function is mainly used to set the type of AOA/AODiq data. The default data type is 8bit. This configuration can be done before starting to receive
 *              the package.
 * @param[in]   mode    - The length of each I or Q data.
 * @return      none.
 */
void rf_aoa_aod_iq_data_mode(rf_iq_data_mode_e mode);


/****************************************************************************************************************************************
 *                                         RF : HADM related functions                                                                  *
 ****************************************************************************************************************************************/

/**
 * @brief       This function is mainly used to initialize some parameter settings of the HADM IQ sample.
 * @param[in]   samp_num    - Number of groups to sample IQ data.
 * @param[in]   interval    - The interval time between each IQ sampling is (interval + 1)*0.125us.
 * @param[in]   start_point - Set the starting point of the sample.If it is rx_en mode, sampling starts
 *                            at 0.25us+start_point*0.125us after settle. If it is in sync mode, sampling
 *                            starts at (start_point + 1) * 0.125us after sync.
 * @param[in]   suppmode    - The length of each I or Q data.
 * @param[in]   sample_mode - IQ sampling starts after syncing packets or after the rx_en is pulled up.
 * @return      none.
 */
void rf_hadm_iq_sample_init(unsigned short samp_num,unsigned char interval,unsigned char start_point,rf_iq_data_mode_e suppmode,rf_hadm_iq_sample_mode_e sample_mode);


/**
 * @brief       This function is mainly used to set the sample interval.
 * @param[in]   ant_interval- Set the interval for IQ sample, (interval + 1)*0.125us.
 * @return      none.
 * @note        The max sample rate is 4Mhz.
 */
void rf_hadm_sample_interval_time(unsigned char interval);

/**
 * @brief       This function is mainly used to initialize the parameters related to HADM antennas.
 * @param[in]   clk_mode    - Set whether the antenna-related clock is always on or only when switching antennas.
 * @param[in]   ant_interval- Set the interval for antenna switching, (interval + 1)*0.125us.
 * @param[in]   ant_rxoffset- Adjust the switching start point of the rx-side antenna,(ant_rxoffset + 1)*0.125us.
 * @param[in]   ant_txoffset- Adjust the switching start point of the tx-side antenna,(ant_rxoffset + 1)*0.125us.
 * @return      none.
 */
void rf_hadm_ant_init(rf_hadm_ant_clk_mode_e clk_mode,unsigned char ant_interval,unsigned char ant_rxoffset,unsigned char ant_txoffset);

/**
 * @brief       This function is mainly used to set the antenna switching interval.
 * @param[in]   ant_interval- Set the interval for antenna switching, (interval + 1)*0.125us.
 * @return      none.
 */
void rf_set_hadm_ant_interval(unsigned char ant_interval);

/**
 * @brief       This function is mainly used to set the starting position of the antenna switching at the rx-side.
 * @param[in]   ant_rxoffset- Adjust the switching start point of the rx-side antenna,(ant_rxoffset + 1)*0.125us.
 * @return      none.
 */
void rf_set_hadm_rx_ant_offset(unsigned char ant_rxoffset);

/**
 * @brief       This function is mainly used to set the starting position of the antenna switching at the tx-side.
 * @param[in]   ant_txoffset- Adjust the switching start point of the rx-side antenna,(ant_txoffset + 1)*0.125us.
 * @return      none.
 */
void rf_set_hadm_tx_ant_offset(unsigned char ant_txoffset);

/**
 * @brief       This function is mainly used to set the clock working mode of the antenna.
 * @para[in]    clk_mode    - Open all the time or only when switching antennas.
 * @return      none.
 */
void rf_hadm_ant_clk_mode(rf_hadm_ant_clk_mode_e clk_mode);

/**
 * @brief       This function is mainly used to set the way IQ sampling starts.
 * @para[in]    sample_mode - IQ sampling starts after syncing packets or after the rx_en is pulled up.
 * @return      none.
 */
void rf_hadm_iq_sample_mode(rf_hadm_iq_sample_mode_e sample_mode);

/**
 * @brief       This function is mainly used to set the starting position of IQ sampling.
 * @para[in]    start_point  - Set the starting point of the sample.If it is rx_en mode, sampling starts
 *                            at 0.25us+start_point*0.125us after settle. If it is in sync mode, sampling
 *                            starts at (start_point + 1) * 0.125us after sync.
 * @return      none.
 */
void rf_hadm_iq_start_point(unsigned char pos);

/**
 * @brief       This function is mainly used to set the number of IQ samples in groups.
 * @para[in]    samp_num    - Number of groups to sample IQ data.
 * @return      none.
 */
void rf_hadm_iq_sample_number(unsigned short samp_num);

/**
 * @brief       Mainly used to set thresholds when sync data packets.
 * @para[in]    thres_value  - The value of thresholds.
 * @return      none.
 */
void rf_set_ble_sync_threshold(unsigned char thres_value);

/**
 * @brief       This function is mainly used to enable the IQ sampling function.
 * @return      none.
 */
void rf_iq_sample_enable(void);

/**
 * @brief       This function is mainly used to disable the IQ sampling function.
 * @return      none.
 */
void rf_iq_sample_disable(void);

/**
 * @brief       This function is mainly used to obtain the synchronisation flag bit from the packet, which is
 *              used to identify whether the packet is data received after passing synchronisation.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the Sync flag information in the packet.
 */
unsigned char rf_hadm_sync_flag(unsigned char *p,unsigned short sample_num,rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the packet quality indicator from the packet, which is
 *              used to identify whether the packet is data received after passing synchronisation.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the packet quality information in the packet.
 */
unsigned char rf_hadm_get_packet_quality_indicator(unsigned char *p,unsigned short sample_num,rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to get the timestamp information from the packet that is
 *              synchronised to the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the Sync timestamp information in the packet.
 */
unsigned int rf_hadm_get_pkt_rx_sync_timestamp(unsigned char *p,unsigned short sample_num,rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the timestamp information of the tx_pos from the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the timestamp information in the packet.
 */
unsigned int rf_hadm_get_pkt_tx_pos_timestamp(unsigned char *p,unsigned short sample_num,rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the timestamp information of the tx_neg from the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the timestamp information in the packet.
 */
unsigned int rf_hadm_get_pkt_tx_neg_timestamp(unsigned char *p,unsigned short sample_num,rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the timestamp information of the iq_start from the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the timestamp information in the packet.
 */
unsigned int rf_hadm_get_pkt_iq_start_timestamp(unsigned char *p,unsigned short sample_num,rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the rssi information from the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the rssi information in the packet.
 */
signed char rf_hadm_get_pkt_rssi_value(unsigned char *p,unsigned short sample_num,rf_iq_data_mode_e data_len);

/**
 * @brief       This function serves to set RF's channel.The step of this function is in KHz.
 *              The frequency set by this function is (chn+2400) MHz+chn_k KHz.
 * @param[in]   chn_m - RF channel. The unit of this parameter is MHz, and its set frequency
 *                      point is (2400+chn)MHz.
 * @param[in]   chn_k - The unit of this parameter is KHz, which means to shift chn_k KHz to
 *                      the right on the basis of chn.Its value ranges from 0 to 999.
 * @param[in]   trx_mode - Defines the frequency point setting of tx mode or rx mode.
 * @return      none.
 */
void rf_set_channel_k_step(signed char chn_m,unsigned int chn_k,rf_trx_chn_e trx_mode);//general

/**
 * @brief       This function is mainly used for frezee agc.
 * @return      none.
 * @note        It should be noted that this function should be called after receiving the package.
 */
void rf_agc_disable(void);

/**
 * @brief       This function is mainly used to set the sequence related to Fast Settle in HADM.
 * @return      none.
 */
void rf_fast_settle_sequence_set(void);

/**
 * @brief       This function is mainly used to set the adc_pup to auto mode.
 * @return      none.
 */
static inline void rf_adc_pup_auto(void)
{
    write_reg8(0x17078c,read_reg8(0x17078c)&0x7f);
}

/**
 * @brief       This function is mainly used to set the tx_pa to auto mode.
 * @return      none.
 */
static inline void rf_tx_pa_pup_auto(void)
{
    write_reg8(0x170778,read_reg8(0x170778)&(~BIT(5)));
}

/**
 * @brief       This function is mainly used to get the gain lat value.
 * @return      Returns the value of gain lat.
 * @note        Call this function after agc disable.
 */
static inline unsigned char rf_get_gain_lat_value(void)
{
    return ((reg_rf_max_match1>>4)&0x07);
}

/**
 * @brief       This function is mainly used to get the cal_trim value of LDO.
 * @return      Returns the cal_trim value of the LDO.
 * @note        Call this function after packets are sent and received.
 */
static inline unsigned char rf_get_ldo_cal_trim_value(void)
{
    return read_reg8(0x1706ea);
}

/**
 * @brief       This function is mainly used to get the rxtxhf_trim value of LDO.
 * @return      Returns the rxtxhf_trim value of the LDO.
 * @note        Call this function after packets are sent and received.
 */
static inline unsigned char rf_get_ldo_rxtxhf_trim_value(void)
{
    return (read_reg8(0x1706ec)&0x3f);
}

/**
 * @brief       This function is mainly used to get the rxtxlf_trim value of LDO.
 * @return      Returns the rxtxlf_trim value of the LDO.
 * @note        Call this function after packets are sent and received.
 */
static inline unsigned char rf_get_ldo_rxtxlf_trim_value(void)
{
    return (((read_reg8(0x1706ed) & 0x0f) << 2) + ((read_reg8(0x1706ec) & 0xc0) >> 6));
}

/**
 * @brief       This function is mainly used to get the pll_trim value of LDO.
 * @return      Returns the pll_trim value of the LDO.
 * @note        Call this function after packets are sent and received.
 */
static inline unsigned char rf_get_ldo_pll_trim_value(void)
{
    return (read_reg8(0x1706ee) & 0x3f);
}

/**
 * @brief       This function is mainly used to get the vco_trim value of LDO.
 * @return      Returns the vco_trim value of the LDO.
 * @note        Call this function after packets are sent and received.
 */
static inline unsigned char rf_get_ldo_vco_trim_value(void)
{
    return (((read_reg8(0x1706ef) & 0x0f) << 2) + ((read_reg8(0x1706ee) & 0xc0) >> 6));
}

/**
 * @brief       This function is mainly used to get the fcal_dcap value.
 * @return      Returns the fcal_dcap value.
 * @note        Call this function after the packet has been sent.
 */
static inline unsigned short rf_get_fcal_dcap_value(void)
{
    write_reg8(0x1706c3,(read_reg8(0x1706c3)&0xc3)|(0x0b<<2));
    return (read_reg16(0x1706c0));
}

/**
 * @brief       This function is mainly used to get the hpmc_gain value.
 * @return      Returns the hpmc_gain value.
 * @note        Call this function after the packet has been sent.
 */
static inline unsigned short rf_get_hpmc_gain_value(void)
{
    return (read_reg16(0x1706fe));
}

/**
 * @brief       This function is mainly used to get the rccal_code value.
 * @return      Returns the rccal_code value.
 * @note        Call this function after the packet has been received.
 */
static inline unsigned short rf_get_rccal_code_value(void)
{
    return (read_reg16(0x1706ca));
}

/**
 * @brief       This function is mainly used to get the idac_code value.
 * @return      Returns the idac_code value.
 * @note        Call this function after the packet has been received.
 */
static inline unsigned char rf_get_dcoc_idac_code_value(void)
{
    return (read_reg8(0x1706d8));
}

/**
 * @brief       This function is mainly used to get the qdac_code value.
 * @return      Returns the qdac_code value.
 * @note        Call this function after the packet has been received.
 */
static inline unsigned char rf_get_dcoc_qdac_code_value(void)
{
    return (read_reg8(0x1706da));
}

/**
 * @brief       This function is mainly used to get the dcoc_offset_code value.
 * @return      Returns the dcoc_offset_code value.
 * @note        Call this function after the packet has been received.
 */
static inline unsigned short rf_get_dcoc_offset_code_value(void)
{
    return (read_reg16(0x1706dc));
}

/**
 * @brief       This function is mainly used to enable LNA.
 * @return      none.
 */
static inline void rf_lna_pup(void)
{
    write_reg8(0x17077a,read_reg8(0x17077a)|BIT(0));
    write_reg8(0x170778,read_reg8(0x170778)|BIT(0));
}

/**
 * @brief       This function is mainly used to disable LNA.
 * @return      none.
 */
static inline void rf_lna_pup_off(void)
{
    write_reg8(0x17077a,read_reg8(0x17077a)&(~BIT(0)));
    write_reg8(0x170778,read_reg8(0x170778)|BIT(0));
}

/**
 * @brief       This function is mainly used to set the value of the dac_pup.
 * @param[in]   value   - The value of dac_pup.
 * @return      none.
 */
void rf_seq_dac_pup_ow(unsigned char value);

/**
 * @brief       This function is mainly used to set the value of the pa_pup.
 * @param[in]   value   - The value of pa_pup.
 * @return      none.
 */
void rf_seq_tx_pa_pup_ow(unsigned char value);

/**
 * @brief       This function is mainly used to open the PA module.
 * @param[in]   pwr     - The slice value of power.
 * @return      none.
 */
void rf_pa_pwr_on(unsigned char pwr);

/**
 * @brief       This function is mainly used to close the PA module.
 * @return      none.
 */
void rf_pa_pwr_off(void);

/**
 * @brief       This function is mainly used to enable the HADM extension function of the hd_info.
 * @return      none.
 */
static inline void rf_hadm_hd_info_enable(void)
{
    reg_rf_mode_ctrl0 |= FLD_RF_INFO_EXTENSION;
}

/**
 * @brief       This function is mainly used to disable the HADM extension function of the hd_info.
 * @return      none.
 */
static inline void rf_hadm_hd_info_disable(void)
{
    reg_rf_mode_ctrl0 &= (~FLD_RF_INFO_EXTENSION);
}

/**
 * @brief       This function is mainly used to get the timestamp information in the process of sending
 *              and receiving packets; in the packet receiving stage, this register stores the sync moment
 *              timestamp, and this information remains unchanged until the next sending and receiving packets.
 *              In the send packet stage, the register stores the timestamp value of the tx_on moment, which
 *              remains unchanged until the next send/receive packet..
 * @return      TX:timestamp value of the tx_on moment.
 *              RX:timestamp value of the sync moment.
 */
static inline unsigned int rf_get_hadm_timestamp(void)
{
    return reg_rf_timestamp;
}

/**
 * @brief       This function is mainly used to return the timestamp of the start point of tx_en through a register.
 * @return      The timestamp of the start point of tx.
 */
static inline unsigned int rf_get_hadm_tx_pos_timestamp(void)
{
    return reg_rf_tr_turnaround_pos_time;
}

/**
 * @brief       This function is mainly used to return the timestamp of the end point of tx_en through a register.
 * @return      The timestamp of the end point of tx.
 */
static inline unsigned int rf_get_hadm_tx_neg_timestamp(void)
{
    return reg_rf_tr_turnaround_neg_time;
}

/**
 * @brief       This function is mainly used to return the timestamp of the IQ sampling start point through the register.
 * @return      The timestamp of IQ sampling.
 */
static inline unsigned int rf_get_hadm_iq_start_timestamp(void)
{
    return reg_rf_iqstart_tstamp;
}

/**
 * @brief       This function is mainly used to wait for the state machine to return to the idle state.
 * @return      none.
 */
static inline void rf_wait_ll_sm_idle(void)
{
    while(reg_rf_ll_2d_sclk != FLD_RF_STATE_MACHINE_IDLE);
}

/**
 * @brief       This function is mainly used to set the preparation and enable of manual fcal.
 * @return      none.
 */
void rf_manual_fcal_setup(void);

/**
 * @brief       This function is mainly used to set the relevant value after manual fcal.
 * @return      none.
 * @note        The function needs to be called after the rf_manual_fcal_setup call 22us.
 */
void rf_manual_fcal_done(void);


/**
 * @brief       This function is mainly used for agc auto run.
 * @return      none.
 * @note        It needs to be called before sending and receiving packets after the tone interaction is complete.
 */
void rf_agc_enable(void);

/**
 * @brief       This function is mainly used for the disable hpmc trim function.
 * @return      none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_dis_hpmc_trim(void);

/**
 * @brief       This function is mainly used for the disable ldo trim function.
 * @return      none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_dis_ldo_trim(void);

/**
 * @brief       This function is mainly used for the disable dcoc trim function.
 * @return      none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_dis_dcoc_trim(void);

/**
 * @brief       This function is mainly used for the disable rccal trim function.
 * @return      none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_dis_rccal_trim(void);

/**
 * @brief       This function is mainly used for the disable fcal trim function.
 * @return      none.
 * @note    TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)
 */
void rf_dis_fcal_trim(void);

/**
 * @brief       This function serves to set PN whitening LFSR polynomial length for RF.
 * @param[in]   rf_pn_length - PN whitening LFSR polynomial length( 0 < rf_pn_length < 32)
 * @return      none.
 * @note        Attention:In ble mode, this interface is not required to configure PN
 */
void rf_set_pn_poly_length(unsigned char rf_pn_length);

/**
 * @brief       This function serves to set PN init value for RF.
 * @param[in]   pn_value - The PN init value to be set for RF.
 * @return      none.
 * @note        Attention:In ble mode, this interface is not required to configure PN
 */
void rf_set_pn_init_value(unsigned int pn_value);

/**
 * @brief       This function serves to set PN polynomial for RF.
 * @param[in]   pn_poly - PN polynomial, (The MSbit of polynomial is omitted)
                For example, to set PN polynomial:
                pn_poly = x7+x4+1
                the pn_poly value would be 0x11(bit<0:6> 001 0001),The value of the highest bit is related to pn length.
 * @return      none.
 * @note        Attention:1.In ble mode, this interface is not required to configure PN. 2.the default polynomial is x7+x4+1.
 */
void rf_set_pn_poly(unsigned int pn_poly);

/**
 * @brief       This function serves to enable pn for RF.
 * @return      none.
 */
void rf_pn_enable(void);

/**
 * @brief       This function serves to set CRC start byte for RF.
 * @param[in]   pn_start_byte - PN start byte position, 0 is the first byte of header
 *              ble:The starting position of the PDU header.
 *              zigbee:The starting position is the first byte after SHR.
 * @return      none.
 * @note        Attention:In ble mode, this interface is not required to configure PN
 */
void rf_set_pn_start_byte(unsigned char pn_start_byte);

/**
 * @brief       This function serves to set pn for RF.
 * @param[in]   rf_pn_length - PN whitening LFSR polynomial length( 0 < rf_pn_length < 32)
 *              pn_value - The PN init value to be set for RF.
 *              pn_poly -  PN polynomial, (The MSbit of polynomial is omitted)
 * @return      none.
 * @note        Attention:In ble mode, this interface is not required to configure PN
 */
void rf_set_pn_config(const rf_pn_config_t *config);

//Clock alignment related
/**
 * @brief       This function serves to enable synchronization alignment the baseband timer to the system timer.
 * @param[in]   none
 * @return      none
 */
void rf_bb_timer_sync_to_stimer_en(void);

/**
 * @brief       This function serves to trigger synchronization alignment the baseband timer to the system timer.
 * @param[in]   none
 * @return      none
 */
void rf_bb_timer_sync_to_stimer_trig(void);

/**
 * @brief       This function serves to set the RF state machine timer source.
 * @param[in]   clk BB_TIMER or STIMER
 * @return      none
 */
void rf_set_timer_source(rf_timer_source clk);

/**
 * @brief     This function performs to get rf baseband timer tick.
 * @return    rf baseband timer tick value.
 */
static inline unsigned int rf_bb_timer_get_tick(void)
{
    return reg_bb_timer_tick;
}

#endif
#endif
#endif
