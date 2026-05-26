/********************************************************************************************************
 * @file    ext_hci_uart.c
 *
 * @brief   This is the source file for BLE SDK
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
#include "ext_hci_uart.h"
#include "lib/include/clock.h"


#ifndef HCI_UART_EXT_DRIVER_EN
#define HCI_UART_EXT_DRIVER_EN  0
#endif

#ifndef HCI_TR_EN
#define HCI_TR_EN               0
#endif


#if (HCI_UART_EXT_DRIVER_EN||HCI_TR_EN)
/**
 * @brief  UART Select interface
 */
#ifndef  EXT_HCI_UART_CHANNEL
#define  EXT_HCI_UART_CHANNEL               UART0
#endif

#ifndef  EXT_HCI_UART_IRQ
#define  EXT_HCI_UART_IRQ                   IRQ_UART0
#endif

#ifndef  EXT_HCI_UART_DMA_CHN_RX
#define  EXT_HCI_UART_DMA_CHN_RX            DMA2   //uart dma
#endif

#ifndef  EXT_HCI_UART_DMA_CHN_TX
#define  EXT_HCI_UART_DMA_CHN_TX            DMA3
#endif

/*** RTS ***/
#define RTS_INVERT    1 /*!< 0: RTS PIN will change from low to high. !! use for RTS auto mode(default auto mode)*/
#define RTS_THRESH    5 /*!< RTS trigger threshold. range: 1-16. */
/*** CTS ***/
#define STOP_VOLT     1         //0 :Low level stops TX.  1 :High level stops TX.



_attribute_data_retention_sec_ static volatile unsigned char  uart_dma_send_flag = 1;
_attribute_data_retention_sec_ static volatile CpltCallback   RxCpltCallback;
_attribute_data_retention_sec_ static volatile CpltCallback   TxCpltCallback;
_attribute_data_retention_sec_ static volatile unsigned char  *ReceAddr;

/**
 * @brief   hci uart initialization
 * @param   none
 * @return  none
 */
ext_hci_StatusTypeDef_e ext_hci_uartInit(ext_hci_InitTypeDef * uart)
{

    unsigned short div;
    unsigned char bwpc;
      /* Check the UART handle allocation */
   if (uart == NULL)
   {
        return EXT_UART_OK;
   }
    uart_reset(EXT_HCI_UART_CHANNEL);
    uart_set_pin(EXT_HCI_UART_CHANNEL,uart->tx_Pin,uart->rx_Pin);

    uart_cal_div_and_bwpc(uart->baudrate, sys_clk.pclk*1000*1000, &div, &bwpc);
    uart_set_rx_timeout_with_exp(EXT_HCI_UART_CHANNEL, bwpc, 12, UART_BW_MUL2,0);

    uart_init(EXT_HCI_UART_CHANNEL, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

    uart_set_tx_dma_config(EXT_HCI_UART_CHANNEL, EXT_HCI_UART_DMA_CHN_TX);
    uart_set_rx_dma_config(EXT_HCI_UART_CHANNEL,  EXT_HCI_UART_DMA_CHN_RX);

    uart_clr_irq_status(EXT_HCI_UART_CHANNEL,UART_TXDONE_IRQ_STATUS);
    uart_set_irq_mask(EXT_HCI_UART_CHANNEL, UART_TXDONE_MASK);
    plic_interrupt_enable(EXT_HCI_UART_IRQ);

    core_interrupt_enable();

    dma_set_irq_mask(EXT_HCI_UART_DMA_CHN_RX, TC_MASK);
    plic_interrupt_enable(IRQ_DMA);

     //cts function
     if((uart->HwFlowCtl != NULL) && (uart->cts_Pin != NULL))
     {
        uart_cts_config(EXT_HCI_UART_CHANNEL,uart->cts_Pin,STOP_VOLT);
        uart_set_cts_en(EXT_HCI_UART_CHANNEL);
     }
     //rts function
     if((uart->HwFlowCtl != NULL) && (uart->rts_Pin != NULL))
     {
            uart_set_rts_en(EXT_HCI_UART_CHANNEL);
            uart_rts_config(EXT_HCI_UART_CHANNEL,uart->rts_Pin,RTS_INVERT,UART_RTS_MODE_AUTO);
            uart_rts_trig_level_auto_mode(EXT_HCI_UART_CHANNEL, RTS_THRESH);
     }
     uart_dma_send_flag = 1;
     TxCpltCallback = uart->TxCpltCallback;
     RxCpltCallback = uart->RxCpltCallback;
     return EXT_UART_OK;
}

/**
 * @brief  Transmit interrupt function
 */
_attribute_ram_code_
_attribute_ram_code_sec_ void ext_hci_irq_handler(void){
     if(uart_get_irq_status(EXT_HCI_UART_CHANNEL,UART_TXDONE_IRQ_STATUS))
     {
         uart_clr_irq_status(EXT_HCI_UART_CHANNEL,UART_TXDONE_IRQ_STATUS);
         uart_dma_send_flag = 1;
     }
     if(TxCpltCallback != NULL)
     {
         TxCpltCallback(NULL);
     }
}
PLIC_ISR_REGISTER(ext_hci_irq_handler, EXT_HCI_UART_IRQ )

/**
 * @brief  Receive interrupt function
 */
_attribute_ram_code_
void ext_hci_dma_irq_handler(void)
{
    if(dma_get_tc_irq_status( BIT(EXT_HCI_UART_DMA_CHN_RX))){
         if((uart_get_irq_status(EXT_HCI_UART_CHANNEL,UART_RX_ERR)))
         {
            uart_clr_irq_status(EXT_HCI_UART_CHANNEL,UART_RXBUF_IRQ_STATUS);
         }
         dma_clr_tc_irq_status( BIT(EXT_HCI_UART_DMA_CHN_RX));
         if(RxCpltCallback != NULL)
         {
             RxCpltCallback(NULL);
         }
    } // if(dma_get_tc_irq_status( BIT(EXT_HCI_UART_DMA_CHN_RX))){
}

PLIC_ISR_REGISTER(ext_hci_dma_irq_handler, IRQ_DMA)


/**
 * @brief  Check whether the transmission is complete
 * @param  none
 * @retval 0: busy  1:idle
 */
unsigned char ext_hci_getTxCompleteDone(void)
{
    return uart_dma_send_flag;
}

/**
 * @brief       This function serves to send data by DMA, this function tell the DMA to get data from the RAM and start.
 * @param[in]   addr     - pointer to the buffer containing data need to send.
 * @param[in]   len      - DMA transmission length.The maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return      1  dma start send.
 *              0  the length is error.
 * @note        addr: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
_attribute_ram_code_sec_
unsigned char ext_hci_uartSendData(unsigned char *addr, unsigned int len)
{
    uart_dma_send_flag = 0;
    return uart_send_dma(EXT_HCI_UART_CHANNEL, addr, len);

}

_attribute_ram_code_sec_
void ext_hci_uartReceData(unsigned char *addr, unsigned int len)
{
    uart_receive_dma(EXT_HCI_UART_CHANNEL, addr, len);
    ReceAddr = addr;
}


#endif
