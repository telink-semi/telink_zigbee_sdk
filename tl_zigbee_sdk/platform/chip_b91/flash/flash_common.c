/********************************************************************************************************
 * @file    flash_common.c
 *
 * @brief   This is the source file for B91
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
   # If add flash type, need pay attention to the read uid command and the bit number of status register
   # If the external flash is to be used as internal flash, the flash needs to be reevaluated.

    +--------------+------------+---------+----------+---------+---------------------------------+-------------------+----------------------------+-------+
    | Package Type | Flash Type | uid CMD |   MID    | Company |         tRES1                   | Sector Erase Time | 16 Bytes Program Time(MAX) | note  |
    |    note (5)  |            |         |          |         | (<25us, otherwise see note (1)) |        (MAX)      |        note (4)            |       |
    +--------------+------------+---------+----------+---------+---------------------------------+-------------------+----------------------------+-------+
    |   Internal   | P25Q80U	|   0x4b  | 0x146085 |	PUYA   |           8us                   |        20ms	     |			   \              |  (2)  |
    |              | P25Q16SU	|   0x4b  | 0x156085 |	PUYA   |           8us                   |        30ms	     |	          1988us          |       |
    |              | P25Q32SU	|   0x4b  | 0x166085 |	PUYA   |           8us                   |        30ms	     |	          1738us          |       |
    +--------------+------------+---------+----------+---------+---------------------------------+-------------------+----------------------------+-------+
    |   External   | PY25Q128H	|   0x4b  | 0x182085 |	PUYA   |           20us                  |        240ms	     |	          153us           |  (3)  |
    +--------------+------------+---------+----------+---------+---------------------------------+-------------------+----------------------------+-------+

    Note:
    1 If tRES1 > 25us, update the delay of EFUSE_LOAD_AND_FLASH_WAKEUP_LOOP_NUM in the S file.
      If tRES1 > 150us, this flash model cannot be used, because the chip hardware boot program only waits for 150us.
    2 Flash trim scheme has been added for P25Q80U.If other types of flash adds this scheme, user need to modify "flash_trim" and "flash_trim_check" function.
    3 PY25Q128H power supply from VBAT(not VDDF) for the following reasons:
      (1) The power supply range of PY25Q128H is 2.7-3.6V, not wide-voltage flash.
      (2) Due to the chip power-on startup will limit the current to the flash, resulting in power on the chip has been ready, but the high-voltage flash power-up is slower resulting in not yet ready,
		  this time the chip fetch instruction from flash will cause problems, so flash power supply from VBAT, which allows the chip and the flash to be synchronized to power up.
    4 16 Bytes Program Time:
      If the flash is ETOX process, BLE requires 16bytes program time below 200us, if it is SONOS process, BLE requires 16bytes program time below 2.5ms,
      if the 16bytes program time of the corresponding flash exceeds the above standard or it is a flash of other process, you need to further confirm with BLE to see if the flash is available.
    5 The chip's power supply to internal the flash is VDDF(2.8V), and the internal flash must be wide-voltage flash
 **/
const flash_hal_handler_t flash_list[] = {
	//1M
	{0x146085,flash_get_lock_block_mid146085, flash_unlock_mid146085, flash_lock_mid146085,FLASH_LOCK_LOW_512K_MID146085,flash_write_status_mid146085,FLASH_WRITE_STATUS_QE_MID146085,FLASH_QE_ENABLE_MID146085,FLASH_QE_DISABLE_MID146085},
	//2M
	{0x156085,flash_get_lock_block_mid156085, flash_unlock_mid156085, flash_lock_mid156085,FLASH_LOCK_LOW_1M_MID156085,flash_write_status_mid156085,FLASH_WRITE_STATUS_QE_MID156085,FLASH_QE_ENABLE_MID156085,FLASH_QE_DISABLE_MID156085},
	//4M
	{0x166085,flash_get_lock_block_mid166085, flash_unlock_mid166085, flash_lock_mid166085,FLASH_LOCK_LOW_2M_MID166085,flash_write_status_mid166085,FLASH_WRITE_STATUS_QE_MID166085,FLASH_QE_ENABLE_MID166085,FLASH_QE_DISABLE_MID166085},
	//16M
	{0x182085,flash_get_lock_block_mid182085, flash_unlock_mid182085, flash_lock_mid182085,FLASH_LOCK_LOW_8M_MID182085,flash_write_status_mid182085,FLASH_WRITE_STATUS_QE_MID182085,FLASH_QE_ENABLE_MID182085,FLASH_QE_DISABLE_MID182085}
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
_attribute_text_sec_ int flash_read_mid_uid_with_check(unsigned int *flash_mid, unsigned char *flash_uid)
{
	unsigned char no_uid[16]={0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01};
	unsigned int i,f_cnt=0;
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

/**
 * @brief 		This function is used to enable the four-wire function of flash.
 * @param[in]  	flash_mid	- the mid of flash.
 * @return 		1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char flash_4line_en(unsigned int flash_mid)
{
	unsigned int i=0;

	for(i=0; i<FLASH_CNT; i++){
		if(flash_list[i].mid == flash_mid){
			return flash_list[i].flash_write_status(flash_list[i].qe_en, flash_list[i].flash_qe_mask);
		}
	}
	return 3;
}

/**
 * @brief 		This function is used to disable the four-wire function of flash.
 * @param[in]  	flash_mid	- the mid of flash.
 * @return 		1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char flash_4line_dis(unsigned int flash_mid)
{
	unsigned int i=0;

	for(i=0; i<FLASH_CNT; i++){
		if(flash_list[i].mid == flash_mid){
			return flash_list[i].flash_write_status(flash_list[i].qe_dis, flash_list[i].flash_qe_mask);
		}
	}
	return 3;
}
