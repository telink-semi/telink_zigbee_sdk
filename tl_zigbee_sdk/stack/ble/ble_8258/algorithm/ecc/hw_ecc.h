/********************************************************************************************************
 * @file    hw_ecc.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
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
#ifndef HW_ECC_H_
#define HW_ECC_H_

#include "stack/ble/ble_8258/algorithm/ecc/ecc_curve.h"


/* hECC_RNG_Function type
The RNG function should fill 'size' random bytes into 'dest'. It should return 1 if
'dest' was filled with random data, or 0 if the random data could not be generated.
The filled-in values should be either truly random, or from a cryptographically-secure PRNG.
A correctly functioning RNG function must be set (using hECC_set_rng()) before calling
hECC_make_key(). */

typedef int (*hECC_rng_func)(unsigned char *dest, unsigned size);


/**
 * @brief		The function that will be used to generate random bytes.
 * @param[in]	resister predefined TRNG function
 * @return		none
 */
void			hwECC_set_rng(hECC_rng_func rng_func);


/**
 * @brief		get ECCP key pair(the key pair could be used in ECDH).
 * @param[out]	public_key	- public key, big--endian.
 * @param[out]	private_key	- private key, big--endian.
 * @param[in]	curve_sel	- ecc_curve select, e.g.: p-256r1.
 * @return		1(success), 0(error).
 */
unsigned char 	hwECC_make_key(unsigned char *public_key, unsigned char *private_key, ecc_curve_t curve_sel);


/**
 * @brief		ECDH compute key.
 * @param[in]	local_prikey	- local private key, big--endian.
 * @param[in]	public_key		- peer public key, big--endian.
 * @param[out]	dhkey			- output dhkey, big--endian.
 * @param[in]	curve_sel		- ecc_curve select, e.g.: p-256r1.
 * @Return		1(success); 0(error).
 */
unsigned char	hwECC_shared_secret(const unsigned char *public_key, const unsigned char *private_key,
									unsigned char *secret, ecc_curve_t curve_sel);




#endif /* HW_ECC_H_ */




