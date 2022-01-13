/********************************************************************************************************
 * @file    drv_putchar.c
 *
 * @brief   This is the source file for drv_putchar
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#include "drv_putchar.h"


#if UART_PRINTF_MODE
_attribute_ram_code_ void soft_uart_putc(unsigned char byte)
{
	u8 j = 0;
	u32 t1 = 0, t2 = 0;

	u8 tmp_bit0 = TX_PIN_OUTPUT_REG & (~(DEBUG_INFO_TX_PIN & 0xff));
	u8 tmp_bit1 = TX_PIN_OUTPUT_REG | (DEBUG_INFO_TX_PIN & 0xff);

	u8 bit[10] = {0};
	bit[0] = tmp_bit0;
	bit[1] = (byte & 0x01) ? tmp_bit1 : tmp_bit0;
	bit[2] = ((byte >> 1) & 0x01) ? tmp_bit1 : tmp_bit0;
	bit[3] = ((byte >> 2) & 0x01) ? tmp_bit1 : tmp_bit0;
	bit[4] = ((byte >> 3) & 0x01) ? tmp_bit1 : tmp_bit0;
	bit[5] = ((byte >> 4) & 0x01) ? tmp_bit1 : tmp_bit0;
	bit[6] = ((byte >> 5) & 0x01) ? tmp_bit1 : tmp_bit0;
	bit[7] = ((byte >> 6) & 0x01) ? tmp_bit1 : tmp_bit0;
	bit[8] = ((byte >> 7) & 0x01) ? tmp_bit1 : tmp_bit0;
	bit[9] = tmp_bit1;
	//u32 r = drv_disable_irq();// enable this may disturb time sequence, but if disable unrecognizable code will show
	t1 = clock_time();
	for(j = 0; j < 10; j++){
		t2 = t1;

		while(t1 - t2 < BIT_INTERVAL){
			t1 = clock_time();
		}

		TX_PIN_OUTPUT_REG = bit[j];       //send bit0
	}
	//drv_restore_irq(r);
}

#elif USB_PRINTF_MODE

#define USB_PRINT_TIMEOUT	 10		//  about 10us at 30MHz

static int usb_putc(unsigned char c)
{
	int i = 0;
	while(i ++ < USB_PRINT_TIMEOUT){
		if(!(reg_usb_ep8_fifo_mode & FLD_USB_ENP8_FULL_FLAG)){
			reg_usb_ep8_dat = c;
			return c;
		}
	}
	return -1;
}

static int hw_usb_putc(unsigned char c)
{
	if(reg_usb_host_conn){
		return usb_putc(c);
	}
	return -1;
}

#endif

void drv_putchar(unsigned char byte)
{
#if UART_PRINTF_MODE
	soft_uart_putc(byte);
#elif USB_PRINTF_MODE
	hw_usb_putc(byte);
#endif
}

