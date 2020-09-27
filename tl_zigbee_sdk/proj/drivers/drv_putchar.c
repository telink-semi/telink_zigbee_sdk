/********************************************************************************************************
 * @file     drv_putchar.c
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

#include "../tl_common.h"


#if defined (MCU_CORE_826x)
#ifndef	BAUDRATE
#define BAUDRATE			2000000//2M
#endif
#define	BIT_INTERVAL	 	(CLOCK_SYS_CLOCK_HZ / BAUDRATE)
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
#ifndef	BAUDRATE
#define BAUDRATE			1000000//1M
#endif
#define	BIT_INTERVAL	 	((16*1000*1000) / BAUDRATE)
#endif


_attribute_ram_code_ void soft_uart_putc(unsigned char byte)
{
#if UART_PRINTF_MODE
	u8 j = 0;
	u32 t1 = 0, t2 = 0;

	static u8 init_flag = 1;
	if(init_flag){
		DEBUG_TX_PIN_INIT();
		init_flag = 0;
	}

	u32 pcTxReg = (REG_GPIO_OUT_BASE_ADDR + ((DEBUG_INFO_TX_PIN >> 8) << 3));
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
	//u32 r = disable_irq();// enable this may disturb time sequence, but if disable unrecognizable code will show
	t1 = clock_time();
	for(j = 0; j < 10; j++){
		t2 = t1;

		while(t1 - t2 < BIT_INTERVAL){
			t1 = clock_time();
		}

		write_reg8(pcTxReg, bit[j]);       //send bit0
	}
	//restore_irq(r);

#endif
}

#if 0
void hw_usb_putc(unsigned char byte)
{

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

