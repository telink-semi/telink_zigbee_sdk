/********************************************************************************************************
 * @file    rf_zigbee.h
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
#ifndef RF_ZIGBEE_H
#define RF_ZIGBEE_H

/*********************************************************************************************************************
 *                                         RF_ZIGBEE  global macro                                                   *
 *********************************************************************************************************************/

/**
 *  @brief Those setting of offset according to zigbee packet format, so this setting for zigbee only.
 */
#define RF_ZIGBEE_DMA_RFRX_OFFSET_RFLEN 4

/**
 *  @brief According to the packet format find the information of packet through offset.
 */
#define rf_zigbee_dma_rx_offset_crc(p)         (p[RF_ZIGBEE_DMA_RFRX_OFFSET_RFLEN] + 3)  //data len:2
#define rf_zigbee_dma_rx_offset_time_stamp(p)  (p[RF_ZIGBEE_DMA_RFRX_OFFSET_RFLEN] + 5)  //data len:4
#define rf_zigbee_dma_rx_offset_freq_offset(p) (p[RF_ZIGBEE_DMA_RFRX_OFFSET_RFLEN] + 9)  //data len:2
#define rf_zigbee_dma_rx_offset_rssi(p)        (p[RF_ZIGBEE_DMA_RFRX_OFFSET_RFLEN] + 11) //data len:1, signed
#define rf_zigbee_packet_crc_ok(p)             ((p[(p[4] + 9 + 3)] & 0x51) == 0x0)
#define rf_zigbee_get_payload_len(p)           (p[4])
#define rf_zigbee_packet_length_ok(p)          (1)

/**
 *  @brief According to different packet format find the crc check digit.
 */
#define rf_hybee_packet_crc_ok(p) ((p[(p[4] + 9 + 3)] & 0x51) == 0x0)

/*********************************************************************************************************************
 *                                         RF_ZIGBEE function declaration                                            *
 *********************************************************************************************************************/

/**
 * @brief     This function serves to set zigbee_250K  mode of RF.
 * @return    none.
 */
void rf_set_zigbee_250K_mode(void);

/**
 * @brief     This function serves to set hybee_1M  mode of RF.
 * @return     none.
 */
void rf_set_hybee_1M_mode(void);

/**
 * @brief     This function serves to set hybee_2M  mode of RF.
 * @return     none.
 */
void rf_set_hybee_2M_mode(void);

/**
 * @brief     This function serves to set hybee_500K  mode of RF.
 * @return     none.
 */
void rf_set_hybee_500K_mode(void);

#endif
