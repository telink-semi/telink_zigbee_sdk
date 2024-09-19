/********************************************************************************************************
 * @file    emi_internal.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef  EMI_INTERNAL_H_
#define  EMI_INTERNAL_H_

#include "lib/include/rf.h"

#define EMI_ACCESS_CODE_V1                      0x29417671
/**
 *  @brief  Define emi_tx_burst_cfg_t struct.
 */
typedef struct{
   signed char rf_chn;               /**< RF channel number.Default: 2 (Frequency channel 2402 MHz) */
   unsigned char pkt_type;           /**< The type of data sent (0:pbrs9  1:0x0f  2:0x55  3:0xaa  4:0xf0  5:0x00  6:0xff)*/
   unsigned char emi_tx_payload_len; /**< Length of the EMI TX payload;Default: 37 */
   unsigned char emi_pkt_duty_cycle; /**< Duty cycle of the EMI packet;Default: 50 (50% duty cycle) */
   unsigned int  emi_access_code;    /**< Access code for EMI transmission Default:0x29417671*/
   rf_mode_e rf_mode;                /**< RF mode setting. */
   rf_power_level_e power_level;     /**< RF power level.  */
}emi_cfg_param_t;

extern emi_cfg_param_t emi_cfg_param;

/**
 * @brief      This function serves to set rx mode and channel
 * @param[in]  none
 * @return     none
 * @note The V1 version can modify RF related configurations through the emi_tx_burst_cfg_t structure
 */
void rf_emi_rx_setup_v1(void);

/**
 * @brief    This function serves to update the number of receiving packet and the RSSI
 * @return   none
 */
void rf_emi_rx_loop_v1(void);

/**
 * @brief    This function serves to get the number of packets received.
 * @return   the number of packets received.
 */
unsigned int rf_emi_get_rxpkt_cnt_v1(void);

/**
 * @brief    This function serves to get the RSSI of packets received
 * @return   the RSSI of packets received
 */
char rf_emi_get_rssi_avg_v1(void);

/**
 * @brief   This function is used to get the packet transmitting time for different payload lengths and modes
 * @param   payload_len: payload length
 * @param   mode: rf mode
 * @return  the packet transmit time
 */
unsigned int emi_get_pkt_time(unsigned char payload_len, rf_mode_e rf_mode);

/**
 * @brief      This function serves to send packets in the burst mode
 * @param[in]  none
 * @return     none
 * @note The V1 version can modify RF related configurations through the emi_tx_burst_cfg_t structure
 */
void rf_emi_tx_burst_loop_v1(void);

/**
 * @brief      This function serves to set the burst mode
 * @param[in]  none
 * @return     none
 * @note The V1 version can modify RF related configurations through the emi_tx_burst_cfg_t structure
 */
void rf_emi_tx_burst_setup_v1(void);

#endif
