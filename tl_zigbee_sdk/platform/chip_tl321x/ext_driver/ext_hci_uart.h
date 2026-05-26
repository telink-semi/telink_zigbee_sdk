/********************************************************************************************************
 * @file    ext_hci_uart.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef DRIVERS_TL321X_EXT_DRIVER_EXT_HCI_UART_H_
#define DRIVERS_TL321X_EXT_DRIVER_EXT_HCI_UART_H_


#include "common/types.h"
#include "uart.h"



/**
  * @brief extend UART HCI  Complete Callback
  */
 typedef void (* CpltCallback)(unsigned int * param);
/**
  * @brief extend UART HCI Init Structure definition
  */
typedef struct
{
  unsigned int   baudrate;    //This member configures the baud rate for UART communication.

  unsigned int   tx_Pin;      //

  unsigned int   rx_Pin;      //

  unsigned int   HwFlowCtl;   //Specifies whether the hardware flow control mode is enabled or disabled

  unsigned int   cts_Pin;      //uart_parity_e

  unsigned int   rts_Pin;      //uart_parity_e

  CpltCallback  RxCpltCallback;         // UART Tx Complete Callback
  CpltCallback  TxCpltCallback;         // UART Rx Complete Callback

} ext_hci_InitTypeDef;

/**
  * @brief  extend UART HCI Status structures definition
  */
typedef enum
{
  EXT_UART_OK       = 0x00U,
  EXT_UART_ERROR    = 0x01U,
  EXT_UART_BUSY     = 0x02U,
}ext_hci_StatusTypeDef_e ;


/**
 * @brief   hci uart initialization
 * @param   none
 * @return  none
 */
ext_hci_StatusTypeDef_e ext_hci_uartInit(ext_hci_InitTypeDef * uart);

/**
 * @brief  Check whether the transmission is complete
 * @param  none
 * @retval 0: busy  1:idle
 */
unsigned char ext_hci_getTxCompleteDone(void);

/**
 * @brief       This function serves to send data by DMA, this function tell the DMA to get data from the RAM and start.
 * @param[in]   addr     - pointer to the buffer containing data need to send.
 * @param[in]   len      - DMA transmission length.The maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return      1  dma start send.
 *              0  the length is error.
 * @note        addr: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
unsigned char ext_hci_uartSendData(unsigned char *addr, unsigned int len);

/**
 * @brief       This function serves to receive data function by DMA, this function tell the DMA to get data from the uart data fifo.
 *              regardless of the length of the DMA configuration, write_num is available,the dma automatically writes back the data receive length to the first four bytes of the rec_buff without manual calculation,
 *              so need addr = the receive buff addr +4.
 * @param[in]   addr     - pointer to the buffer  receive data.
 * @param[in]   rev_size - the receive length of DMA,The maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return      none
 * @note
 *              - addr must be aligned by word (4 bytes), otherwise the program will enter an exception.
 *              - A length greater than XX_len itself and a multiple of 4 is denoted as CEILING(XX_len,4). For example: XX_len=3 ,CEILING(XX_len,4)=4; XX_len=21 ,CEILING(Tx_length, 4)=24. \n
 *                The actual length sent by master  is denoted as Tx_len, The length (param[in]-rev_size) of the interface configuration is denoted as Rx_len. \n
 *                when CEILING(Tx_len,4) > CEILING(Rx_len,4), When the length of the DMA carry reaches Rx_len, the DMA will not stop working and the excess data will not be carried into the buff. \n
 *                For example:Tx_len=21,Rx_len=20,When the DMA stops working the buff is written with a length of 21 and only 20 bytes of data are stored.It is recommended to configure the appropriate Rx_len to avoid this situation.
 */
void ext_hci_uartReceData(unsigned char *addr, unsigned int len);

#endif /* DRIVERS_TL321X_EXT_DRIVER_EXT_HCI_UART_H_ */
