/********************************************************************************************************
 * @file    drv_flash.c
 *
 * @brief   This is the source file for drv_flash
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

#include "../tl_common.h"

#if (FLASH_W_PROTECT)
flash_lock_t *p_drv_flashLockOp = NULL;

#if defined(MCU_CORE_8258)
flash_lock_t  g_flashLockTable[] = {
	{0x11460c8, flash_lock_all_mid011460c8, flash_unlock_mid011460c8},
	{0x1060c8, flash_lock_all_mid1060c8,   flash_unlock_mid1060c8},
	{0x13325e, flash_lock_all_mid13325e,   flash_unlock_mid13325e},
	{0x134051, flash_lock_all_mid134051,   flash_unlock_mid134051},
	{0x136085, flash_lock_all_mid136085,   flash_unlock_mid136085},
	{0x1360c8, flash_lock_all_mid1360c8,   flash_unlock_mid1360c8},
	{0x1360eb, flash_lock_all_mid1360eb,   flash_unlock_mid1360eb},
	{0x14325e, flash_lock_all_mid14325e,   flash_unlock_mid14325e},
	{0x1460c8, flash_lock_all_mid1460c8,   flash_unlock_mid1460c8}
};
#elif defined(MCU_CORE_B91)
flash_lock_t  g_flashLockTable[] = {
	{0x146085, flash_lock_all_mid146085, flash_unlock_mid146085},
	{0x156085, flash_lock_all_mid156085, flash_unlock_mid156085},
	{0x166085, flash_lock_all_mid166085, flash_unlock_mid166085}
};
#else
flash_lock_t  g_flashLockTable[] = {
	{0x0, NULL, NULL},
};
#endif


bool flash_load(void){
	unsigned int mid = flash_read_mid();
	for(int i = 0; i < sizeof(g_flashLockTable)/sizeof(flash_lock_t); i++){
		if(mid == g_flashLockTable[i].mid){
			p_drv_flashLockOp = &g_flashLockTable[i];
			return TRUE;
		}
	}

	return FALSE;
}

static void flash_lock(void){
	if(p_drv_flashLockOp && p_drv_flashLockOp->lock){
		p_drv_flashLockOp->lock();
	}
}

static void flash_unlock(void){
	if(p_drv_flashLockOp && p_drv_flashLockOp->unlock){
		p_drv_flashLockOp->unlock();
	}
}
#endif


void flash_write(u32 addr, u32 len, u8 *buf){
#if (MODULE_WATCHDOG_ENABLE)
	drv_wd_clear();
#endif

#if (VOLTAGE_DETECT_ENABLE)
	if(drv_get_adc_data() < BATTERY_SAFETY_THRESHOLD){return;}
#endif

#if (FLASH_W_PROTECT)
	flash_unlock();
#endif

	flash_write_page(addr, len, buf);

#if (FLASH_W_PROTECT)
	flash_lock();
#endif
}

void flash_read(u32 addr, u32 len, u8 *buf){
	flash_read_page(addr, len, buf);
}

void flash_erase(u32 addr){
#if (MODULE_WATCHDOG_ENABLE)
	drv_wd_clear();
#endif

#if (VOLTAGE_DETECT_ENABLE)
	if(drv_get_adc_data() < BATTERY_SAFETY_THRESHOLD){return;}
#endif

#if (FLASH_W_PROTECT)
	flash_unlock();
#endif

	flash_erase_sector(addr);

#if (FLASH_W_PROTECT)
	flash_lock();
#endif
}

#ifdef CFS_ENABLE
_attribute_ram_code_ void cfs_flash_write_page(u32 addr, u32 len, u8 *buf){
	u32 r = drv_disable_irq();
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
	drv_restore_irq(r);
}

_attribute_ram_code_ void cfs_flash_read_page(u32 addr, u32 len, u8 *buf){
	u32 r = drv_disable_irq();
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
	drv_restore_irq(r);
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

