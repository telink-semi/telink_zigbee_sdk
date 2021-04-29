/********************************************************************************************************
 * @file	bootloader.c
 *
 * @brief	This is the source file for bootloader
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#if (__PROJECT_TL_BOOT_LOADER__)

#include "tl_common.h"
#include "bootloader.h"



/* FLASH address */
#define APP_RUNNING_ADDR				APP_IMAGE_ADDR
#define APP_NEW_IMAGE_ADDR				FLASH_ADDR_OF_OTA_IMAGE

/* SRAM address */
#if defined(MCU_CORE_826x)
	#define MCU_RAM_START_ADDR			0x808000
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define MCU_RAM_START_ADDR			0x840000
#elif defined(MCU_CORE_B91)
	#define MCU_RAM_START_ADDR			0x0000
#endif

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define REBOOT()					WRITE_REG8(0x602, 0x88)
#elif defined(MCU_CORE_B91)
	#define REBOOT()					((void(*)(void))(FLASH_R_BASE_ADDR + APP_IMAGE_ADDR))()
#endif

#define FW_START_UP_FLAG				0x4B
#define FW_RAMCODE_SIZE_MAX				RESV_FOR_APP_RAM_CODE_SIZE



static bool is_valid_fw_bootloader(u32 addr_fw){
	u8 startup_flag = 0;
    flash_read(addr_fw + FLASH_TLNK_FLAG_OFFSET, 1, &startup_flag);

    return ((startup_flag == FW_START_UP_FLAG) ? TRUE : FALSE);
}

void bootloader_with_ota_check(u32 addr_load, u32 new_image_addr){
	if(new_image_addr != addr_load){
		if(is_valid_fw_bootloader(new_image_addr)){
			u8 buf[256];

			flash_read(new_image_addr, 256, buf);
			u32 fw_size = *(u32 *)(buf + 0x18);

			if(fw_size <= FLASH_OTA_IMAGE_MAX_SIZE){
				for(int i = 0; i < fw_size; i += 256){
					if((i & 0xfff) == 0){
						flash_erase(addr_load + i);
					}

					flash_read(new_image_addr + i, 256, buf);
					flash_write(addr_load + i, 256, buf);
				}
			}

			buf[0] = 0;
			flash_write(new_image_addr + FLASH_TLNK_FLAG_OFFSET, 1, buf);   //clear OTA flag

			//erase the new firmware
			for(int i = 0; i < ((fw_size + 4095)/4096); i++) {
				flash_erase(new_image_addr + i*4096);
			}
		}
	}

    if(is_valid_fw_bootloader(addr_load)){
#if !defined(MCU_CORE_B91)
    	u32 ramcode_size = 0;
        flash_read(addr_load + 0x0c, 2, (u8 *)&ramcode_size);
        ramcode_size *= 16;

        if(ramcode_size > FW_RAMCODE_SIZE_MAX){
            ramcode_size = FW_RAMCODE_SIZE_MAX; // error, should not run here
        }
        flash_read(addr_load, ramcode_size, (u8 *)MCU_RAM_START_ADDR); // copy ram code
#endif
        REBOOT();
    }
}

void bootloader_init(void){
	bootloader_with_ota_check(APP_RUNNING_ADDR, APP_NEW_IMAGE_ADDR);
}

#endif	/* __PROJECT_TL_BOOT_LOADER__ */
