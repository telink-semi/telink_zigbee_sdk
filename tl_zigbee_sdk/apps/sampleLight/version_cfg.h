/********************************************************************************************************
 * @file     version_cfg.h
 *
 * @brief    application configuration
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

#include "../common/version_comm.h"

#if defined(MCU_CORE_826x)
	#if (CHIP_8269)
		#define CHIP_TYPE	TLSR_8269
	#else
		#define CHIP_TYPE	TLSR_8267
	#endif
#elif defined(MCU_CORE_8258)
		#define CHIP_TYPE	TLSR_8258
#elif defined(MCU_CORE_8278)
		#define CHIP_TYPE	TLSR_8278
#elif defined(MCU_CORE_B91)
		#define CHIP_TYPE	TLSR_9518
#endif

#define APP_RELEASE									0x10//app release 1.0
#define APP_BUILD									0x01//app build 01
#define STACK_RELEASE								0x30//stack release 3.0
#define STACK_BUILD									0x01//stack build 01

/*********************************************************************************************
 * During OTA upgrade, the upgraded device will check the rules of the following three fields.
 * Refer to ZCL OTA specification for details.
 */
#define MANUFACTURER_CODE                  			0x1141//Telink ID

#define	IMAGE_TYPE								  	((CHIP_TYPE << 8) | IMAGE_TYPE_LIGHT)

#define	FILE_VERSION					  			((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)

