/********************************************************************************************************
 * @file    trng.h
 *
 * @brief   This is the header file for TL321X
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
#ifndef TRNG_H
#define TRNG_H


#include "lib/include/trng/trng_basic.h"


#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief       get rand(for internal test)
 * @param[in]   rand                byte buffer rand
 * @param[in]   bytes               byte length of rand
 * @return      TRNG_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1. Directly from the entropy source to take the random number,
            depending on the current configuration, the power-up default true random number mode,
            if you get the true random number and the current mode is configured as a post-processing, you can use trng_disable, trng_set_mode, trng_enable interface function to set to the true random mode, and then call the function,
            if you get the pseudo-random please use get_rand.
  @endverbatim
 */
    unsigned int get_rand_internal(unsigned char *rand, unsigned int bytes);

    /**
 * @brief       get rand with fast speed(with entropy reducing, for such as clearing tmp buffer)
 * @param[in]   rand                byte buffer rand
 * @param[in]   bytes               byte length of rand
 * @return      TRNG_SUCCESS(success), other(error)
 */
    unsigned int get_rand_fast(unsigned char *rand, unsigned int bytes);

#ifndef CONFIG_TRNG_GENERATE_BY_HARDWARE
    unsigned int get_rand_register(void);
#endif

    /**
 * @brief       get rand(without entropy reducing)
 * @param[in]   rand                byte buffer rand
 * @param[in]   bytes               byte length of rand
 * @return      TRNG_SUCCESS(success), other(error)
 * * @note
  @verbatim
      -# 1. This interface uses pseudo-random mode by default, through post-processing, the randomness will be better and easier to pass trng-related authentication
      -# 2. After the call the mode configuration is changed to post-processing mode.
  @endverbatim
 */
    unsigned int get_rand(unsigned char *rand, unsigned int bytes);

#ifdef __cplusplus
}
#endif

#endif
