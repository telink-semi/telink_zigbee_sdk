/********************************************************************************************************
 * @file    trng.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef TRNG_H
#define TRNG_H


#include "lib/include/trng/trng_basic.h"


#ifdef __cplusplus
extern "C"
{
#endif


    /**
 * @brief           get rand(for internal test)
 * @param[in]       rand                 - byte buffer rand
 * @param[in]       bytes                - byte length of rand
 * @return          TRNG_SUCCESS(success), other(error)
 */
    unsigned int get_rand_internal(unsigned char *rand, unsigned int bytes);

    /**
 * @brief           Get random data with fast speed (with entropy reducing, suitable for clearing temporary buffers).
 * @param[in]       rand                 - Byte buffer to store the random data.
 * @param[in]       bytes                - Byte length of the rand buffer.
 * @return          TRNG_SUCCESS on success, other error codes on failure.
 */
    unsigned int get_rand_fast(unsigned char *rand, unsigned int bytes);

    /**
 * @brief           get rand(without entropy reducing)
 * @param[in]       rand                 - byte buffer rand
 * @param[in]       bytes                - byte length of rand
 * @return          TRNG_SUCCESS(success), other(error)
 */
    unsigned int get_rand(unsigned char *rand, unsigned int bytes);


#ifdef __cplusplus
}
#endif

#endif
