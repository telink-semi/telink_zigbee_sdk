/********************************************************************************************************
 * @file     drv_flash.h
 *
 * @brief	 flash read/write interface header file
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
#pragma once

#include "../common/types.h"
#include "../common/compiler.h"

typedef void (*drv_flash_write)(unsigned long addr, unsigned long len, unsigned char *buf);
typedef void (*drv_flash_read)(unsigned long addr, unsigned long len, unsigned char *buf);
typedef void (*drv_flash_erase)(unsigned long addr);

typedef struct{
	drv_flash_write write;
	drv_flash_read read;
	drv_flash_erase erase;
}drv_flash_t;

void flash_write(u32 addr, u32 len, u8 *buf);
void flash_read(u32 addr, u32 len, u8 *buf);
void flash_erase(u32 addr);
