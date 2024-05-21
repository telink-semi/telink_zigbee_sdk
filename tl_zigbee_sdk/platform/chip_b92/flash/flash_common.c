/********************************************************************************************************
 * @file    flash_common.c
 *
 * @brief   This is the source file for B92
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "flash_type.h"


/**
 * @brief 		This function is used to enable the four-wire function of flash.
 * @param[in]  	flash_mid	- the mid of flash.
 * @return 		1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char flash_4line_en(unsigned int flash_mid)
{
	switch(flash_mid)
	{
	case MID146085:
		return flash_write_status_mid146085(FLASH_QE_ENABLE_MID146085, FLASH_WRITE_STATUS_QE_MID146085);
		break;
	case MID156085:
		return flash_write_status_mid156085(FLASH_QE_ENABLE_MID156085, FLASH_WRITE_STATUS_QE_MID156085);
		break;
	case MID166085:
		return flash_write_status_mid166085(FLASH_QE_ENABLE_MID166085, FLASH_WRITE_STATUS_QE_MID166085);
		break;
	case MID186085:
		return flash_write_status_mid186085(FLASH_QE_ENABLE_MID186085, FLASH_WRITE_STATUS_QE_MID186085);
		break;
	case MID1560c8:
		return flash_write_status_mid1560c8(FLASH_QE_ENABLE_MID1560c8, FLASH_WRITE_STATUS_QE_MID1560c8);
		break;
	case MID1460c8:
		return flash_write_status_mid1460c8(FLASH_QE_ENABLE_MID1460c8, FLASH_WRITE_STATUS_QE_MID1460c8);
		break;
	default:
		return 3;
		break;
	}
}

/**
 * @brief 		This function is used to disable the four-wire function of flash.
 * @param[in]  	flash_mid	- the mid of flash.
 * @return 		1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char flash_4line_dis(unsigned int flash_mid)
{
	switch(flash_mid)
	{
	case MID146085:
		return flash_write_status_mid146085(FLASH_QE_DISABLE_MID146085, FLASH_WRITE_STATUS_QE_MID146085);
		break;
	case MID156085:
		return flash_write_status_mid156085(FLASH_QE_DISABLE_MID156085, FLASH_WRITE_STATUS_QE_MID156085);
		break;
	case MID166085:
		return flash_write_status_mid166085(FLASH_QE_DISABLE_MID166085, FLASH_WRITE_STATUS_QE_MID166085);
		break;
	case MID186085:
		return flash_write_status_mid186085(FLASH_QE_DISABLE_MID186085, FLASH_WRITE_STATUS_QE_MID186085);
		break;
	case MID1560c8:
		return flash_write_status_mid1560c8(FLASH_QE_DISABLE_MID1560c8, FLASH_WRITE_STATUS_QE_MID1560c8);
		break;
	case MID1460c8:
		return flash_write_status_mid1460c8(FLASH_QE_DISABLE_MID1460c8, FLASH_WRITE_STATUS_QE_MID1460c8);
		break;
	default:
		return 3;
		break;
	}
}

