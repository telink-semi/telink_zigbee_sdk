/********************************************************************************************************
 * @file	comm_cfg.h
 *
 * @brief	This is the header file for comm_cfg
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
#pragma once


/**********************************************************************
 * User configuration whether to use boot loader mode.
 *
 * NOTE:
 * We do not recommend using BootLoader mode on 512K flash chips,
 * because in boot loader mode, it can only support up to 196k firmware
 * according to the current default Flash Map.
 *
 * Please refer to the drv_nv.h file, and check if the firmware size
 * exceeds the maximum limit.
 *
 * The user can configure the CHIP_TYPE used by the corresponding
 * project in the version.h file according to the actual size
 * of the flash at hand.
 *
 * If BOOT_LOADER_MODE is 0, it means that the SDK uses Normal Mode,
 * hardware multi-address (0x0000 or 0x40000) startup mode.
 * If BOOT_LOADER_MODE is 1, it means that the SDK uses Boot Loader Mode.
 *
 * Normal mode is used by default.
 */
#define BOOT_LOADER_MODE					0



/* Boot loader address. */
#define BOOT_LOADER_IMAGE_ADDR				0x0

/* APP image address. */
#if (BOOT_LOADER_MODE)
	#define APP_IMAGE_ADDR					0x8000
#else
	#define APP_IMAGE_ADDR					0x0
#endif


/* Chip IDs */
#define TLSR_8267							0x00
#define TLSR_8269							0x01
#define TLSR_8258_512K						0x02
#define TLSR_8258_1M						0x03
#define TLSR_8278							0x04
#define TLSR_9518							0x05

/* Image types */
#if (BOOT_LOADER_MODE)
	#define IMAGE_TYPE_BOOT_FLAG			1
#else
	#define IMAGE_TYPE_BOOT_FLAG			0
#endif
#define IMAGE_TYPE_BOOTLOADER				(0xFF)
#define IMAGE_TYPE_GW						(0x00 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_LIGHT					(0x01 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_SWITCH					(0x02 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_CONTACT_SENSOR			(0x03 | (IMAGE_TYPE_BOOT_FLAG << 7))

