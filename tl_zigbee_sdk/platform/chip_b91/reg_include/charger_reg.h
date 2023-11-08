/********************************************************************************************************
 * @file    charger_reg.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef CHARGER_REG_H_
#define CHARGER_REG_H_

/*
 * analog:0x19
 */
#define  reg_charger_vchg      0x19
enum{
   VCHG_TRIM =BIT_RNG(4,7),
};
/*
 * analog:0x1c
 */
#define  reg_charger_mode     0x1c
enum{
  	AUTO_MANUAL_SEL = BIT(7),
	MANUAL_MODE = BIT_RNG(4,6),
};
/*
 * analog:0x1a
 */
#define  reg_charger_ichg     0x1a
enum{
   ICHG_TRIM =BIT_RNG(0,3),
};
/*
 * analog:0x22
 */
#define  reg_charger_status    0x22
enum{
   CHG_USB_ON_OFF            = BIT(6),
};

#endif /* DRIVERS_REG_INCLUDE_CHARGER_REG_H_ */
