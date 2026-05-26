/********************************************************************************************************
 * @file     ext_misc.c
 *
 * @brief   This is the source file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include "ext_misc.h"

/******************************* flash_start *****************************************************************/
_attribute_ram_code_ unsigned int flash_get_jedec_id_ram(void)
{
#if SUPPORT_PFT_ARCH
	reg_irq_threshold = 1;
#else
	unsigned int r  = core_interrupt_disable();
#endif
	extern void flash_send_cmd(flash_command_e cmd);
	flash_send_cmd(FLASH_GET_JEDEC_ID);
	unsigned char manufacturer = mspi_read();
	unsigned char mem_type = mspi_read();
	unsigned char cap_id = mspi_read();
	mspi_high();
	CLOCK_DLY_5_CYC;
#if SUPPORT_PFT_ARCH
	reg_irq_threshold = 0;
#else
	core_restore_interrupt(r);
#endif
	return (unsigned int)((manufacturer << 24 | mem_type << 16 | cap_id));
}

#if 0
_attribute_text_code_ unsigned int flash_get_jedec_id(void)
{
	__asm__("csrci 	mmisc_ctl,8");//disable BTB
	unsigned int id;
	id = flash_get_jedec_id_ram();
	__asm__("csrsi 	mmisc_ctl,8");//enable BTB

	return id;
}
#endif

/******************************* flash_end *****************************************************************/



/******************************* uart_start *****************************************************************/
 _attribute_ram_code_ void uart_receive_dma_set(dma_chn_e chn, unsigned char * addr,unsigned int rev_size)
{
	 dma_chn_dis(chn);
	/*In order to be able to receive data of unknown length(A0 doesn't support),the DMA SIZE is set to the longest value 0xffffffff.After entering suspend and wake up, and then continue to receive, 
	DMA will no longer move data from uart fifo, because DMA thinks that the last transmission was not completed and must disable dma_chn first.modified by minghai,confirmed qiangkai 2020.11.26.*/
	dma_set_address(chn,reg_uart_data_buf_adr(UART0),(unsigned int)convert_ram_addr_cpu2bus(addr));
	if(0xff== g_chip_version)
	{
		dma_set_size(chn, rev_size, DMA_WORD_WIDTH);
	}
	else
	{
	    reg_dma_size(chn)=0xffffffff;
	}

	dma_chn_en(chn);
}
/******************************* uart_end *******************************************************************/


