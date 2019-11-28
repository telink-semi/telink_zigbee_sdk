/********************************************************************************************************
 * @file     putchar.c
 *
 * @brief	 UART simulation interface
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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
 *
 *******************************************************************************************************/
#include "tl_putchar.h"
#include "user_config.h"
#include "../common/compiler.h"
#include "../common/types.h"
#include "platform_includes.h"



#ifndef		BIT_INTERVAL
#ifdef MCU_CORE_8258
#define BAUDRATE			1000000//1M
#define	BIT_INTERVAL	 	(16*1000*1000 / BAUDRATE)
#else
#define BAUDRATE			2000000//2M
#define	BIT_INTERVAL	 	(CLOCK_SYS_CLOCK_HZ / BAUDRATE)
#endif
#endif

_attribute_ram_code_ int soft_uart_putc(char byte) {
#if UART_PRINTF_MODE
	u8 j = 0;
	u32 t1 = 0, t2 = 0;
	u32 pcTxReg = (0x800583 + ((DEBUG_INFO_TX_PIN >> 8) << 3)); //register GPIO output
	u8 tmp_bit0 = read_reg8(pcTxReg) & (~(DEBUG_INFO_TX_PIN & 0xff));
	u8 tmp_bit1 = read_reg8(pcTxReg) | (DEBUG_INFO_TX_PIN & 0xff);

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
	//u8 r = irq_disable();// enable this may disturb time sequence, but if disable unrecognizable code will show
	t1 = reg_system_tick;
	for(j = 0; j < 10; j++) {
		t2 = t1;

		while(t1 - t2 < BIT_INTERVAL) {
			t1  = reg_system_tick;
		}

		write_reg8(pcTxReg, bit[j]);       //send bit0
	}
	//irq_restore(r);
	return byte;

#else
	return -1;
#endif
}

int putchar(int c) {
#if UART_PRINTF_MODE
	return soft_uart_putc((char)c);
#elif USB_PRINTF_MODE
	return hw_usb_putc(c);
#endif
    return -1;
}

