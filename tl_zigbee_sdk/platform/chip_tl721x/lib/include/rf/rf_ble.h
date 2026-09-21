/********************************************************************************************************
 * @file    rf_ble.h
 *
 * @brief   This is the header file for TL721X
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
#ifndef RF_BLE_H
#define RF_BLE_H

/**********************************************************************************************************************
 *                                         RF_BLE global macro                                                        *
 *********************************************************************************************************************/
/**
 *  @brief Those setting of offset according to ble packet format, so this setting for ble only.
 */
#define RF_BLE_DMA_RFRX_LEN_HW_INFO   0
#define RF_BLE_DMA_RFRX_OFFSET_HEADER 4
#define RF_BLE_DMA_RFRX_OFFSET_RFLEN  5
#define RF_BLE_DMA_RFRX_OFFSET_DATA   6

/**
 *  @brief According to the packet format find the information of packet through offset.
 */
#define rf_ble_dma_rx_offset_crc24(p)       (p[RF_BLE_DMA_RFRX_OFFSET_RFLEN] + 6)  //data len:3
#define rf_ble_dma_rx_offset_time_stamp(p)  (p[RF_BLE_DMA_RFRX_OFFSET_RFLEN] + 9)  //data len:4
#define rf_ble_dma_rx_offset_freq_offset(p) (p[RF_BLE_DMA_RFRX_OFFSET_RFLEN] + 13) //data len:2
#define rf_ble_dma_rx_offset_rssi(p)        (p[RF_BLE_DMA_RFRX_OFFSET_RFLEN] + 15) //data len:1, signed
#define rf_ble_packet_length_ok(p)          (*((unsigned int *)p) == p[5] + 13)    //dma_len must 4 byte aligned
#define rf_ble_packet_crc_ok(p)             ((p[(p[5] + 5 + 11)] & 0x01) == 0x0)

#define rf_set_ble_crc_value(crc)           rf_set_crc_init_value(crc)

/*********************************************************************************************************************
 *                                         RF_BLE function declaration                                               *
 *********************************************************************************************************************/

/**
 * @brief     This function serves to set ble_1M  mode of RF.
 * @return    none.
 */
void rf_set_ble_1M_mode(void);

/**
 * @brief     This function serves to  set ble_1M_NO_PN  mode of RF.
 * @return    none.
 */
void rf_set_ble_1M_NO_PN_mode(void);

/**
 * @brief     This function serves to set ble_2M  mode of RF.
 * @return    none.
 */
void rf_set_ble_2M_mode(void);

/**
 * @brief     This function serves to set ble_2M_NO_PN  mode of RF.
 * @return    none.
 */
void rf_set_ble_2M_NO_PN_mode(void);

/**
 * @brief     This function serves to set ble_500K  mode of RF.
 * @return    none.
 */
void rf_set_ble_500K_mode(void);

/**
 * @brief     This function serves to  set ble_125K  mode of RF.
 * @return    none.
 */
void rf_set_ble_125K_mode(void);

/**
 * @brief    This function serves to set CRC advantage.
 * @return   none.
 */
static _always_inline void rf_set_ble_crc_adv(void)
{
    write_reg32(0x170114, 0x555555);
}

/**
 * @brief       This function serves to set RF baseband channel.This function is suitable for ble open PN mode.
 * @param[in]   chn_num  - Bluetooth channel set according to Bluetooth protocol standard.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void rf_set_ble_chn(signed char chn_num);

/**
 * @brief       This function serves to start Rx of auto mode. In this mode,
 *              RF module stays in Rx status until a packet is received or it fails to receive packet when timeout expires.
 *              Timeout duration is set by the parameter "tick".
 *              The address to store received data is set by the function "addr".
 * @param[in]   addr   - The address to store received data.
 * @param[in]   tick   - It indicates timeout duration in Rx status.Max value: 0xffffff (16777215)
 * @return      none
 * @note        addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
_attribute_ram_code_sec_noinline_ void rf_start_brx(void *addr, unsigned int tick);


/**
 * @brief       This function serves to start tx of auto mode. In this mode,
 *              RF module stays in tx status until a packet is sent or it fails to sent packet when timeout expires.
 *              Timeout duration is set by the parameter "tick".
 *              The address to store send data is set by the function "addr".
 * @param[in]   addr   - The address to store send data.
 * @param[in]   tick   - It indicates timeout duration in Rx status.Max value: 0xffffff (16777215)
 * @return      none
 * @note        addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
_attribute_ram_code_sec_noinline_ void rf_start_btx(void *addr, unsigned int tick);

#endif
