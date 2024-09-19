/********************************************************************************************************
 * @file    flash_common.c
 *
 * @brief   This is the source file for B85
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
#include "flash_common.h"

/**
 *	If you add a new flash, you need to check the following location and add the corresponding flash model:
	#1 need to add flash_midxxx.c and flash_midxxx.h to the driver/flash directory, and introduce the header file flash_midxxx.h into flash_type.h.
	#2 need to add the corresponding mid to flash_mid_e in flash.h to check if Flash_CapacityDef contains the size of the added flash.
	#3 Check that flash_get_vendor() in flash.c and flash_vendor_e in flash.h already contain the added flash model.
	#4 need to add the corresponding mid in flash_support_mid[], and the Flash Type/uid CMD/MID/Company/Sector Erase Time(MAX) in the following table.
	#5 need to add a flash test area of corresponding size in Test_Demo/app_flash_test.c.
	#6 need to add the corresponding new flash to Flash_Deno/app.c in flash_lock()/flash_unlock(), as well as add the implementation of flash_midxxx_test()
	and add the case of flash_midxxx_test() in user_init().

 	If add flash type, need pay attention to the read uid command and the bit number of status register.
	Flash Type	uid CMD			MID		Company		Sector Erase Time(MAX)
	ZB25WD40B	0x4b		0x13325E	ZB			500ms
	ZB25WD80B	0x4b		0x14325E	ZB			500ms
	GD25LD05C	0x4b(AN)	0x1060C8	GD			500ms
	GD25LD40C	0x4b		0x1360C8	GD			500ms
	GD25LD40E	0x4b		0x1360C8	GD			500ms
	GD25LD80C	0x4b(AN)	0x1460C8	GD			500ms
	GD25LD80E	0x4b(AN)	0x1460C8	GD			500ms
	GD25LE80C	0x4b		0x1460C8	GD			300ms
	GD25LQ80C	0x4b		0x1460C8	GD			300ms
	MD25D40D	0x4b(AN)	0x134051	GD			400ms
	P25Q40L		0x4b		0x136085	PUYA		12ms
	P25D40SU    0x4b        0x136085    PUYA        12ms
	TH25D40LA	0x4b		0x1360EB	TH			12ms
	TH25D40UA	0x4b		0x1360EB	TH			12ms

	The uid of the early ZB25WD40B (mid is 0x13325E) is 8 bytes. If you read 16 bytes of uid,
	the next 8 bytes will be read as 0xff. Later, the uid of ZB25WD40B has been switched to 16 bytes.
 */

const flash_hal_handler_t flash_list[] = {
	//64K
	{0x1060C8,flash_get_lock_block_mid1060c8, flash_unlock_mid1060c8, flash_lock_mid1060c8,FLASH_LOCK_LOW_32K_MID1060C8},
	//512K
	{0x1360C8,flash_get_lock_block_mid1360c8, flash_unlock_mid1360c8, flash_lock_mid1360c8,FLASH_LOCK_LOW_256K_MID1360C8},
	{0x13325E,flash_get_lock_block_mid13325e, flash_unlock_mid13325e, flash_lock_mid13325e,FLASH_LOCK_LOW_256K_MID13325E},
	{0x134051,flash_get_lock_block_mid134051, flash_unlock_mid134051, flash_lock_mid134051,FLASH_LOCK_LOW_256K_MID134051},
	{0x136085,flash_get_lock_block_mid136085, flash_unlock_mid136085, flash_lock_mid136085,FLASH_LOCK_LOW_256K_MID136085},
	{0x1360EB,flash_get_lock_block_mid1360eb, flash_unlock_mid1360eb, flash_lock_mid1360eb,FLASH_LOCK_LOW_256K_MID1360EB},
	//1M
	{0x14325E,flash_get_lock_block_mid14325e, flash_unlock_mid14325e, flash_lock_mid14325e,FLASH_LOCK_LOW_768K_MID14325E},
	{0x1460C8,flash_get_lock_block_mid1460c8, flash_unlock_mid1460c8, flash_lock_mid1460c8,FLASH_LOCK_LOW_768K_MID1460C8},
	{0x011460C8,flash_get_lock_block_mid011460c8, flash_unlock_mid011460c8, flash_lock_mid011460c8,FLASH_LOCK_LOW_512K_MID011460C8},
};

const unsigned int FLASH_CNT = sizeof(flash_list)/sizeof(flash_hal_handler_t);

/**
 * @brief		This function serves to read flash mid and uid,and check the correctness of mid and uid.
 * @param[out]	flash_mid	- Flash Manufacturer ID.
 * @param[out]	flash_uid	- Flash Unique ID.
 * @return		0: flash no uid or not a known flash model 	 1:the flash model is known and the uid is read.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
int flash_read_mid_uid_with_check(unsigned int *flash_mid, unsigned char *flash_uid)
{
	unsigned char no_uid[16]={0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01};
	int i,f_cnt=0;
	*flash_mid = flash_read_mid();

	for(i=0; i<FLASH_CNT; i++){
		if(flash_list[i].mid == *flash_mid){
			flash_read_uid(FLASH_READ_UID_CMD_GD_PUYA_ZB_TH, (unsigned char *)flash_uid);
			break;
		}
	}
	if(i == FLASH_CNT){
		return 0;
	}

	for(i=0; i<16; i++){
		if(flash_uid[i] == no_uid[i]){
			f_cnt++;
		}
	}

	if(f_cnt == 16){	//no uid flash
		return 0;
	}else{
		return 1;
	}
}
