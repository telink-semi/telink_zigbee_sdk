/********************************************************************************************************
 * @file     flash_826x.h
 *
 * @brief    system flash configuration for tlsr826x
 *
 * @author
 * @date     Dec. 1, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

#include "compiler.h"
#include "types.h"


enum{
	FLASH_WRITE_STATUS_CMD	=	0x01,
	FLASH_WRITE_CMD			=	0x02,
	FLASH_READ_CMD			=	0x03,
	FLASH_WRITE_ENABLE_CMD 	= 	0x06,
	FLASH_WRITE_DISABLE_CMD = 	0x04,
	FLASH_READ_STATUS_CMD	=	0x05,
	FLASH_SECT_ERASE_CMD	=	0x20,
	FLASH_BLK_ERASE_CMD		=	0xD8,
	FLASH_POWER_DOWN		=	0xB9,
	FLASH_GET_JEDEC_ID		=	0x9F,
};

_attribute_ram_code_ void flash_erase_sector(unsigned long  addr);
_attribute_ram_code_ void flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf);
_attribute_ram_code_ void flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf);
_attribute_ram_code_ u32 flash_get_jedec_id();
_attribute_ram_code_ u8 flash_write_status(u8 data);

_attribute_ram_code_ void flash_read_mid(unsigned char *buf);
_attribute_ram_code_ void flash_read_uid(unsigned char idcmd,unsigned char *buf);
