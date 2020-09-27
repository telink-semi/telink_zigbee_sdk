/********************************************************************************************************
 * @file     s7816.h 
 *
 * @brief    This is the header file for TLSR8278
 *
 * @author	 Driver Group
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/


#ifndef S7816_H_
#define S7816_H_

#include "gpio.h"

/**
 *  @brief  Define 7816 TRx pin
 */
typedef enum{
	S7816_UART0_RTX_A4=GPIO_PA4,
	S7816_UART0_RTX_B3=GPIO_PB3,
	S7816_UART0_RTX_D3=GPIO_PD3,

	S7816_UART1_RTX_C7=GPIO_PC7,
	S7816_UART1_RTX_D7=GPIO_PD7,
	S7816_UART1_RTX_E2=GPIO_PE2,
}S7816_RTx_PinDef;

typedef enum {
	S7816_TX,
	S7816_RX,
}S7816_Half_Duplex_ModeDef;

typedef enum {
	S7816_UART0,
	S7816_UART1,
}S7816_UART_CHN;



 static inline void s7816_tx_triggle_en(S7816_UART_CHN chn)
{
	reg_uart_state(chn) =0x80;//triggle tx_en setting 2'b10
}
/**
 * @brief      	This function is used to initiate 7816 module of MCU
 * @param[in]  	Div	-set the divider of clock of 7816 module:
 * 				7816clk = sysclk/(0x7b[6:4]*2),	0x7b[7]:enable 7816clk
 * 				7816clk:  0x40-2Mhz   0x20-4Mhz
 * 				baudrate: 0x40-16194  0x20-32388
 * @return     	none
 */
extern void s7816_set_clk(unsigned char Div);


/**
 * @brief      	This function is used to initiate 7816 module of MCU
 * @param[in]  	S7816_RTx_PinDef-select the I/O 	pin of 7816 module
 * @param[in]	Pin_7816_RST	-select the RST 	pin of 7816 module
 * @param[in]	Pin_7816_VCC	-select the VCC 	pin of 7816 module
 * @return     	none
 */
extern void s7816_set_pin(S7816_RTx_PinDef Pin_7816_TRX);




/**
 * @brief      	This function is used to transform half duplex mode of 7816
 * @param[in]  	mode	- half_duplex_mode_TX/RX is transformed by setting 0x9b[5]
 * 						0x9b[5]=1:half_duplex_mode_RX;0x9b[5]=0:half_duplex_mode_TX
 * @return     	none
 */
extern void s7816_set_half_duplex(S7816_Half_Duplex_ModeDef mode,S7816_UART_CHN chn);




#endif /* S7816_H_ */

