/********************************************************************************************************
 * @file    hadm_drv.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef HADM_DRV_H
#define HADM_DRV_H

#include "lib/include/sys.h"
#include "gpio.h"

/**
 * @brief   Select how you want to start IQ sampling.
 */
typedef enum
{
    RF_HADM_IQ_SAMPLE_SYNC_MODE,
    RF_HADM_IQ_SAMPLE_RXEN_MODE
} rf_hadm_iq_sample_mode_e;

/**
 * @brief   Select whether the antenna clock is normally open or turned on when the antenna is switched.
 */
typedef enum
{
    RF_HADM_ANT_CLK_ALWAYS_ON_MODE,
    RF_HADM_ANT_CLK_SWITCH_ON_MODE
} rf_hadm_ant_clk_mode_e;

/**
 * @brief   Define function to set tx channel or rx channel.
 */
typedef enum
{
    TX_CHANNEL = 0,
    RX_CHANNEL = 1,
} rf_trx_chn_e;

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
void rf_hadm_iq_sample_init(unsigned short samp_num, unsigned char interval, unsigned char start_point, rf_iq_data_mode_e suppmode, rf_hadm_iq_sample_mode_e sample_mode);

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
void rf_hadm_ant_init(rf_hadm_ant_clk_mode_e clk_mode, unsigned char ant_interval, unsigned char ant_rxoffset, unsigned char ant_txoffset);

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
unsigned char rf_hadm_sync_flag(unsigned char *p, unsigned short sample_num, rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the packet quality indicator from the packet, which is
 *              used to identify whether the packet is data received after passing synchronisation.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the packet quality information in the packet.
 */
unsigned char rf_hadm_get_packet_quality_indicator(unsigned char *p, unsigned short sample_num, rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to get the timestamp information from the packet that is
 *              synchronised to the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the Sync timestamp information in the packet.
 */
unsigned int rf_hadm_get_pkt_rx_sync_timestamp(unsigned char *p, unsigned short sample_num, rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the timestamp information of the tx_pos from the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the timestamp information in the packet.
 */
unsigned int rf_hadm_get_pkt_tx_pos_timestamp(unsigned char *p, unsigned short sample_num, rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the timestamp information of the tx_neg from the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the timestamp information in the packet.
 */
unsigned int rf_hadm_get_pkt_tx_neg_timestamp(unsigned char *p, unsigned short sample_num, rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the timestamp information of the iq_start from the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the timestamp information in the packet.
 */
unsigned int rf_hadm_get_pkt_iq_start_timestamp(unsigned char *p, unsigned short sample_num, rf_iq_data_mode_e data_len);

/**
 * @brief       This function is mainly used to obtain the rssi information from the packet.
 * @param[in]   p           - The packet address.
 * @param[in]   sample_num  - The number of sample points that the packet contains.
 * @param[in]   data_len    - The data length of the sample point in the packet.
 * @return      Returns the rssi information in the packet.
 */
signed char rf_hadm_get_pkt_rssi_value(unsigned char *p, unsigned short sample_num, rf_iq_data_mode_e data_len);

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
void rf_set_channel_k_step(signed char chn_m, unsigned int chn_k, rf_trx_chn_e trx_mode); //general

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
    write_reg8(0x17078c, read_reg8(0x17078c) & 0x7f);
}

/**
 * @brief       This function is mainly used to set the tx_pa to auto mode.
 * @return      none.
 */
static inline void rf_tx_pa_pup_auto(void)
{
    write_reg8(0x170778, read_reg8(0x170778) & (~BIT(5)));
}

/**
 * @brief       This function is mainly used to get the gain lat value.
 * @return      Returns the value of gain lat.
 * @note        Call this function after agc disable.
 */
static inline unsigned char rf_get_gain_lat_value(void)
{
    return ((reg_rf_max_match1 >> 4) & 0x07);
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
    return (read_reg8(0x1706ec) & 0x3f);
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
    write_reg8(0x1706c3, (read_reg8(0x1706c3) & 0xc3) | (0x0b << 2));
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
    write_reg8(0x17077a, read_reg8(0x17077a) | BIT(0));
    write_reg8(0x170778, read_reg8(0x170778) | BIT(0));
}

/**
 * @brief       This function is mainly used to disable LNA.
 * @return      none.
 */
static inline void rf_lna_pup_off(void)
{
    write_reg8(0x17077a, read_reg8(0x17077a) & (~BIT(0)));
    write_reg8(0x170778, read_reg8(0x170778) | BIT(0));
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
    while (reg_rf_ll_2d_sclk != FLD_RF_STATE_MACHINE_IDLE)
        ;
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

#endif
