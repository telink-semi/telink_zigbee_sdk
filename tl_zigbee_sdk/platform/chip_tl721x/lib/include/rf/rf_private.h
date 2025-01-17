/********************************************************************************************************
 * @file    rf_private.h
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
#ifndef RF_PRIVATE_H
#define RF_PRIVATE_H

/**********************************************************************************************************************
 *                                         RF_PRIVATE global macro                                                    *
 *********************************************************************************************************************/

/******************************************************FOR TPLL************************************************************/

/**
 *  @brief Those setting of offset according to private tpll packet format, so this setting for ble only.
 */
#define RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN 4

/**
 *  @brief According to the packet format find the information of packet through offset.
 */
#define rf_pri_tpll_dma_rx_offset_crc(p)         (p[RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN] + 5)  //data len:2
#define rf_pri_tpll_dma_rx_offset_time_stamp(p)  (p[RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN] + 7)  //data len:4
#define rf_pri_tpll_dma_rx_offset_freq_offset(p) (p[RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN] + 11) //data len:2
#define rf_pri_tpll_dma_rx_offset_rssi(p)        (p[RF_PRI_TPLL_DMA_RFRX_OFFSET_RFLEN] + 13) //data len:1, signed
#define rf_pri_tpll_packet_crc_ok(p)             ((p[((p[4] & 0x3f) + 11 + 3)] & 0x01) == 0x00)
#define rf_pri_sb_packet_crc_ok(p)               ((p[(reg_rf_sblen & 0x3f) + 4 + 9] & 0x01) == 0x00)
#define rf_ant_packet_crc_ok(p)                  ((p[(reg_rf_sblen & 0x3f) + 4 + 9] & 0x01) == 0x00)

/*********************************************************************************************************************
 *                                         RF_PRIVATE function declaration                                           *
 *********************************************************************************************************************/

/**
 * @brief     This function serves to set pri_250K  mode of RF.
 * @return    none.
 */
void rf_set_pri_250K_mode(void);

/**
 * @brief     This function serves to  set pri_500K  mode of RF.
 * @return    none.
 */
void rf_set_pri_500K_mode(void);

/**
 * @brief     This function serves to set pri_1M  mode of RF.
 * @return    none.
 */
void rf_set_pri_1M_mode(void);

/**
 * @brief     This function serves to set pri_2M  mode of RF.
 * @return    none.
 */
void rf_set_pri_2M_mode(void);

/**
 * @brief     This function serves to set pri_generic_250K  mode of RF.
 * @return    none.
 */
void rf_set_pri_generic_250K_mode(void);

/**
  * @brief     This function serves to set pri_generic_500K  mode of RF.
  * @return   none.
  */
void rf_set_pri_generic_500K_mode(void);

/**
 * @brief     This function serves to set pri_generic_1M  mode of RF.
 * @return    none.
 */
void rf_set_pri_generic_1M_mode(void);

/**
 * @brief     This function serves to set pri_generic_2M  mode of RF.
 * @return    none.
 */
void rf_set_pri_generic_2M_mode(void);

/**
 * @brief       This function serves to set pri sb mode enable.
 * @return      none.
 */
void rf_private_sb_en(void);

/**
 * @brief       This function serves to set pri sb mode payload length.
 * @param[in]   pay_len  - In private sb mode packet payload length.
 * @return      none.
 */
void rf_set_private_sb_len(int pay_len);

/**
 * @brief   This function serve to set access code.This function will first get the length of access code from register 0x170005
 *          and then set access code in addr.
 * @param[in]   pipe_id -The number of pipe.0<= pipe_id <=7.
 * @param[in]   acc -The value access code
 * @note        For compatibility with previous versions the access code should be bit transformed by bit_swap();
 */
void rf_set_pipe_access_code(unsigned int pipe_id, unsigned char *addr);

/**
 * @brief     This function is used to set the size of each field in the header of a generic packet.
 * @param[in] h0_size      - The size of H0 field in bits.(0 <=  h0_size <= 16)
 * @param[in] length_size  - The size of length field.
 * @param[in] h1_size      - The size of H1 field in bits.(0 <=  h0_size <= 16)
 * @return    none.
 * @note      Attention:The sum of the sizes (in bits) of H0, LENGTH and H1 must be an integer multiple of 8 bits.
 */
void rf_set_pri_generic_header_size(unsigned char h0_size, unsigned char length_size, unsigned char h1_size);

/**
 * @brief     This function is used to set a fixed offset for the extracted length field.
 * @param[in] length_adj  - The fixed offset for the extracted length field.
 *                          Length_adj range:-31 <=  length_adj <=31
 * @return    none.
 * @note      If length_adj is configured,the sum of length + length_adj represents the number of payload + crc octets
 */
void rf_set_pri_generic_length_adj(signed char length_adj);

/**
 * @brief     This function is used to set the PID position in the header field.
 * @param[in] pid_start_bit  - The bit in the header field starting with the PID.
 *            (0 is the first bit of header)
 * @return    none.
 */
void rf_set_pri_generic_pid_start_bit(unsigned char pid_start_bit);

/**
 * @brief     This function serves to enable the 2-bit PID in the header field.
 * @param[in] none.
 * @return          none.
 */
void rf_set_pri_generic_pid_en(void);

/**
 * @brief     This function is used to set the no_ack position in the header field.
 * @param[in] noack_start_bit  - The bit in the header field starting with the no_ack.
 *            (0 is the first bit of header)
 * @return    none.
 */
void rf_set_pri_generic_noack_start_bit(unsigned char noack_start_bit);

/**
 * @brief     This function serves to enable the 2-bit no_ack in the header field.
 * @param[in] none.
 * @return    none.
 */
void rf_set_pri_generic_noack_en(void);

/**
 * @brief     This function is used to set the size of the H0 field in the header of a generic packet.
 * @param[in] h0_size     - The size of H0 field in bits.(0 <=  h0_size <= 16)
 * @return    none.
 */
void rf_set_pri_generic_header_h0_size(unsigned char h0_size);

/**
 * @brief     This function is used to set the size of the H1 field in the header of a generic packet.
 * @param[in] h1_size      - The size of H1 field in bits.(0 <=  h0_size <= 16)
 * @return    none.
 */
void rf_set_pri_generic_header_h1_size(unsigned char h1_size);

/**
 * @brief     This function is used to set the size of the length field in the header of a generic packet.
 * @param[in] length_size            - The size of length field in bits.
 * @return    none.
 * @note      If length is present (non-zero size), its value determines the number of octetsremaining in the packet after the header is complete.
 *            That is, payload octets + crc octets.
 */
void rf_set_pri_generic_header_length_size(unsigned char length_size);


#endif
