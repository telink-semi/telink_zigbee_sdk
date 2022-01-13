/********************************************************************************************************
 * @file    flash.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
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

#pragma once

#include "mspi.h"
#include "compiler.h"


#define PAGE_SIZE 256

/**
 * @brief     flash command definition
 */
typedef enum
{
	FLASH_WRITE_STATUS_CMD		=	0x01,
	FLASH_WRITE_CMD				=	0x02,
	FLASH_READ_CMD				=	0x03,

	FLASH_WRITE_DISABLE_CMD 	= 	0x04,
	FLASH_READ_STATUS_CMD		=	0x05,
	FLASH_WRITE_ENABLE_CMD 		= 	0x06,

	FLASH_CHIP_ERASE_CMD		=	0x60,   //or 0xc7

	FLASH_PES_CMD				=	0x75,
	FLASH_PER_CMD				=	0x7A,
	FLASH_QUAD_PAGE_PROGRAM_CMD	=	0x32,
	FLASH_READ_DEVICE_ID_CMD	=	0x90,

	FLASH_FAST_READ_CMD			=	0x0B,
	FLASH_X2READ_CMD			=	0xBB,
	FLASH_DREAD_CMD				=	0x3B,
	FLASH_X4READ_CMD			=	0xEB,
	FLASH_QREAD_CMD				=	0x6B,

	FLASH_SECT_ERASE_CMD		=	0x20,   //sector size = 4KBytes
	FLASH_32KBLK_ERASE_CMD		=	0x52,
	FLASH_64KBLK_ERASE_CMD		=	0xD8,
	FLASH_GD_PUYA_READ_UID_CMD	=	0x4B,	//Flash Type = GD/PUYA
	FLASH_XTX_READ_UID_CMD		=	0x5A,	//Flash Type = XTX
	FLASH_PAGE_ERASE_CMD		=	0x81,   //caution: only P25Q40L support this function

	FLASH_POWER_DOWN			=	0xB9,
	FLASH_POWER_DOWN_RELEASE	=	0xAB,
	FLASH_GET_JEDEC_ID			=	0x9F,
	FLASH_READ_STATUS_1_CMD		=	0x35,

	FLASH_VOLATILE_SR_WRITE_CMD	=	0x50,
	FLASH_SET_BURST_WITH_WRAP_CMD	=	0x77,
	FLASH_ENABLE_SO_TO_OUTPUT_CMD	=	0x70,
	FLASH_READ_DEVICE_ID_DUAL_CME	=	0x92,
	RLASH_READ_DEVICE_ID_QUAD_CMD	=	0x94,
	FLASH_ERASE_SECURITY_REGISTERS_CMD	=	0x44,
	FLASH_PROGRAM_SECURITY_REGISTERS_CMD	=	0x42,
	FLASH_READ_SECURITY_REGISTERS_CMD	=	0x48,
	FLASH_ENABLE_RESET_CMD	=	0x99,

	FLASH_ENABLE_RESET	=	0x66,
	FLASH_DISABLE_SO_TO_OUTPUT	=	0x80,
}flash_command_e;

/**
 * @brief     flash type definition
 */
typedef enum{
	FLASH_TYPE_PUYA	= 0,
}flash_type_e;

/**
 * @brief   flash capacity definition
 * 			Call flash_read_mid function to get the size of flash capacity.
 * 			Example is as follows:
 * 			unsigned char temp_buf[4];
 * 			flash_read_mid(temp_buf);
 * 			The value of temp_buf[2] reflects flash capacity.
 */
typedef enum {
    FLASH_SIZE_64K     = 0x10,
    FLASH_SIZE_128K    = 0x11,
    FLASH_SIZE_256K    = 0x12,
    FLASH_SIZE_512K    = 0x13,
    FLASH_SIZE_1M      = 0x14,
    FLASH_SIZE_2M      = 0x15,
    FLASH_SIZE_4M      = 0x16,
    FLASH_SIZE_8M      = 0x17,
} flash_capacity_e;

typedef struct{
	unsigned char  flash_read_cmd;			/**< xip read command */
	unsigned char  flash_read_dummy:4;		/**< dummy cycle = flash_read_dummy + 1 */
	unsigned char  flash_read_data_line:2;	/**< 0:single line;  1: dual line;  2:quad line; 3:quad line */
	unsigned char  flash_read_addr_line:1;	/**< 0:single line;  1:the same to dat_line_h */
	unsigned char  flash_read_cmd_line:1; 	/**< 0:single line;  1:the same to dat_line_h */
}flash_xip_config_t;
/**
 * @brief     	This function serves to erase a page(256 bytes).
 * @param[in] 	addr	- the start address of the page needs to erase.
 * @return    	none.
 */
_attribute_text_sec_ void flash_erase_page(unsigned int addr);

/**
 * @brief 		This function serves to erase a sector.
 * @param[in]   addr	- the start address of the sector needs to erase.
 * @return 		none.
 */
_attribute_text_sec_ void flash_erase_sector(unsigned long addr);

/**
 * @brief 		This function serves to erase a block(32k).
 * @param[in]   addr	- the start address of the block needs to erase.
 * @return 		none.
 */
_attribute_text_sec_ void flash_erase_32kblock(unsigned int addr);

/**
 * @brief 		This function serves to erase a block(64k).
 * @param[in]   addr	- the start address of the block needs to erase.
 * @return 		none.
 */
_attribute_text_sec_ void flash_erase_64kblock(unsigned int addr);

/**
 * @brief     	This function serves to erase a chip.
 * @return    	none.
 */
_attribute_text_sec_ void flash_erase_chip(void);

/**
 * @brief 		This function writes the buffer's content to a page.
 * @param[in]   addr	- the start address of the page.
 * @param[in]   len		- the length(in byte) of content needs to write into the page.
 * @param[in]   buf		- the start address of the content needs to write into.
 * @return 		none.
 */
_attribute_text_sec_ void flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief 		This function reads the content from a page to the buf.
 * @param[in]   addr	- the start address of the page.
 * @param[in]   len		- the length(in byte) of content needs to read out from the page.
 * @param[out]  buf		- the start address of the buffer.
 * @return 		none.
 */
_attribute_text_sec_ void flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief 		This function write the status of flash.
 * @param[in]  	data	- the value of status.
 * @return 		none.
 */
_attribute_text_sec_ void flash_write_status(unsigned short data);

/**
 * @brief 		This function reads the status of flash.
 * @return 		the value of status.
 */
_attribute_text_sec_ unsigned short flash_read_status(void);

/**
 * @brief		Deep Power Down mode to put the device in the lowest consumption mode
 * 				it can be used as an extra software protection mechanism,while the device
 * 				is not in active use,since in the mode,  all write,Program and Erase commands
 * 				are ignored,except the Release from Deep Power-Down and Read Device ID(RDI)
 * 				command.This release the device from this mode
 * @return 		none.
 */
_attribute_text_sec_ void flash_deep_powerdown(void);

/**
 * @brief		The Release from Power-Down or High Performance Mode/Device ID command is a
 * 				Multi-purpose command.it can be used to release the device from the power-Down
 * 				State or High Performance Mode or obtain the devices electronic identification
 * 				(ID)number.Release from Power-Down will take the time duration of tRES1 before
 * 				the device will resume normal operation and other command are accepted.The CS#
 * 				pin must remain high during the tRES1(8us) time duration.
 * @return      none.
 */
_attribute_text_sec_ void flash_release_deep_powerdown(void);

/**
 * @brief	  	This function serves to read MID of flash(MAC id). Before reading UID of flash,
 * 				you must read MID of flash. and then you can look up the related table to select
 * 				the idcmd and read UID of flash
 * @param[in] 	buf		- store MID of flash
 * @return    	none.
 */
_attribute_text_sec_ unsigned int flash_read_mid(void);

/**
 * @brief	  	This function serves to read UID of flash
 * @param[in] 	idcmd	- different flash vendor have different read-uid command. E.g: GD/PUYA:0x4B; XTX: 0x5A
 * @param[in] 	buf		- store UID of flash
 * @return    	none.
 */
_attribute_text_sec_ void flash_read_uid(unsigned char idcmd, unsigned char *buf);

/**
 * @brief		This function serves to read flash mid and uid,and check the correctness of mid and uid.
 * @param[out]	flash_mid	- Flash Manufacturer ID
 * @param[out]	flash_uid	- Flash Unique ID
 * @return		0: flash no uid or not a known flash model 	 1:the flash model is known and the uid is read.
 */
_attribute_text_sec_ int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid);

/**
 * @brief 		This function serves to set the protection area of the flash.
 * @param[in]   type	- flash type include Puya.
 * @param[in]   data	- refer to Driver API Doc.
 * @return 		none.
 */
_attribute_text_sec_ void flash_lock(flash_type_e type, unsigned short data);

/**
 * @brief 		This function serves to flash release protection.
 * @param[in]   type	- flash type include Puya.
 * @return 		none.
 */
_attribute_text_sec_ void flash_unlock(flash_type_e type);

/**
 * @brief 		This function serves to set priority threshold. when the interrupt priority > Threshold flash process will disturb by interrupt.
 * @param[in]   preempt_en	- 1 can disturb by interrupt, 0 can disturb by interrupt.
 * @param[in]	threshold	- priority Threshold.
 * @return    	none.
 */
_attribute_text_sec_ void flash_plic_preempt_config(unsigned char preempt_en, unsigned char threshold);
/**
 * @brief 		This function is used to update the configuration parameters of xip(eXecute In Place),
 * 				this configuration will affect the speed of MCU fetching,
 * 				this parameter needs to be consistent with the corresponding parameters in the flash datasheet.
 * @param[in]	config	- xip configuration,reference structure flash_xip_config_t
 * @return none
 */
_attribute_text_sec_ void flash_set_xip_config(flash_xip_config_t config);
/**
 * @brief		This function serves to set flash write command.This function interface is only used internally by flash,
 * 				and is currently included in the H file for compatibility with other SDKs. When using this interface,
 * 				please ensure that you understand the precautions of flash before using it.
 * @param[in]	cmd	- set command.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ void flash_send_cmd(unsigned char cmd);

