/********************************************************************************************************
 * @file    ecc_curve.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    12,2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef ECC_CURVE_H_
#define ECC_CURVE_H_

/* Curve selection options */
typedef enum{
//	ECC_use_secp160r1,
	ECC_use_secp192r1,
	ECC_use_secp224r1,
	ECC_use_secp256r1,
//	ECC_use_secp256k1,
}ecc_curve_t;





#endif /* ECC_CURVE_H_ */
