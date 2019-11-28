/********************************************************************************************************
 * @file     drv_flash.c
 *
 * @brief	 flash read/write interface file
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
#include "drv_flash.h"
#include "platform_includes.h"

const drv_flash_t myFlashDrv = {
	.write = flash_write_page,
	.read = flash_read_page,
	.erase = flash_erase_sector,
};

void flash_op(u8 opmode, u32 addr, u32 len, u8 *buf){
	u32 re = addr%256;

	u32 pageReLen = (re)?(256 -re):256;

	u32 writeLen = 0;
	u32 remainLen = len;

	do{
		if(remainLen <= pageReLen){
			writeLen = remainLen;
			remainLen = 0;
		}else{
			remainLen -= pageReLen;
			writeLen = pageReLen;
			pageReLen = 256;
		}
		if(opmode){
			myFlashDrv.write(addr,writeLen,buf);
		}else{
			myFlashDrv.read(addr,writeLen,buf);
		}
		buf += writeLen;
		addr += writeLen;
#if(MODULE_WATCHDOG_ENABLE)
		wd_clear();
#endif
	}while(remainLen);
}

void flash_write(u32 addr, u32 len, u8 *buf){
	flash_op(1, addr, len, buf);
}

void flash_read(u32 addr, u32 len, u8 *buf){
	flash_op(0, addr, len, buf);
}


void flash_erase(u32 addr){
#if(MODULE_WATCHDOG_ENABLE)
	wd_clear();
#endif
	myFlashDrv.erase(addr);
#if(MODULE_WATCHDOG_ENABLE)
		wd_clear();
#endif
}

#ifdef CFS_ENABLE
_attribute_ram_code_ void cfs_flash_write_page(u32 addr, u32 len, u8 *buf){
	u8 r = irq_disable();
	// important:  buf must not reside at flash, such as constant string.  If that case, pls copy to memory first before write
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_CMD);
	flash_send_addr(addr);

	u32 i;
	for(i = 0; i < len; ++i){
		mspi_write(~(buf[i]));		/* write data */
		mspi_wait();
	}
	mspi_high();
	flash_wait_done();
	irq_restore(r);
}

_attribute_ram_code_ void cfs_flash_read_page(u32 addr, u32 len, u8 *buf){
	u8 r = irq_disable();
	flash_send_cmd(FLASH_READ_CMD);
	flash_send_addr(addr);

	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();
	/* get data */
	for(int i = 0; i < len; ++i){
		*buf++ = ~(mspi_get());
		mspi_wait();
	}
	mspi_high();
	irq_restore(r);
}

void cfs_flash_op(u8 opmode, u32 addr, u32 len, u8 *buf){
	u32 re = addr%256;

	u32 pageReLen = (re)?(256 -re):256;

	u32 writeLen = 0;
	u32 remainLen = len;

	do{
		if(remainLen <= pageReLen){
			writeLen = remainLen;
			remainLen = 0;
		}else{
			remainLen -= pageReLen;
			writeLen = pageReLen;
			pageReLen = 256;
		}
		if(opmode){
			cfs_flash_write_page(addr,writeLen,buf);
		}else{
			cfs_flash_read_page(addr,writeLen,buf);
		}
		buf += writeLen;
		addr += writeLen;
	}while(remainLen);
}

void cfs_flash_write(u32 addr, u32 len, u8 *buf){
	cfs_flash_op(1, addr, len, buf);
}

void cfs_flash_read(u32 addr, u32 len, u8 *buf){
	cfs_flash_op(0, addr, len, buf);
}
#endif

