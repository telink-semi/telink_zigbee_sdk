/********************************************************************************************************
 * @file    sw_ecc.h
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
/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */
/* Refer to micro-ecc:ver = 1.0.0, url=https://github.com/kmackay/micro-ecc */
#ifndef SW_ECC_H_
#define SW_ECC_H_

#include "stack/ble/ble_8258/algorithm/ecc/ecc_curve.h"

/* sECC_RNG_Function type
The RNG function should fill 'size' random bytes into 'dest'. It should return 1 if
'dest' was filled with random data, or 0 if the random data could not be generated.
The filled-in values should be either truly random, or from a cryptographically-secure PRNG.
A correctly functioning RNG function must be set (using swECC_set_rng()) before calling
swECC_make_key(). */
typedef int (*sECC_RNG_Function)(unsigned char *dest, unsigned size);


/**
 * @brief		The function that will be used to generate random bytes.
 * @param[in]	resister predefined TRNG function
 * @return		none
 */
void			swECC_set_rng(sECC_RNG_Function rng_func);


/**
 * @brief		get ECCP key pair(the key pair could be used in ECDH).
 * @param[out]	public_key	- public key, big--endian.
 * @param[out]	private_key	- private key, big--endian.
 * @param[in]	curve_sel	- ecc_curve select, e.g.: p-256r1.
 * @return		PKE_SUCCESS(success), other(error).
 */
unsigned char 	swECC_make_key(unsigned char *public_key, unsigned char *private_key, ecc_curve_t curve_sel);


/**
 * @brief		ECDH compute key.
 * @param[in]	local_prikey	- local private key, big--endian.
 * @param[in]	public_key		- peer public key, big--endian.
 * @param[out]	dhkey			- output dhkey, big--endian.
 * @param[in]	curve_sel		- ecc_curve select, e.g.: p-256r1.
 * @Return		0(success); other(error).
 */
unsigned char	swECC_shared_secret(const unsigned char *public_key, const unsigned char *private_key,
									unsigned char *secret, ecc_curve_t curve_sel);

#endif /* SW_ECC_H_ */


