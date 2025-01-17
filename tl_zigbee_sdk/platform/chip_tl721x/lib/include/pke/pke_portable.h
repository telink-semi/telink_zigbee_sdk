/********************************************************************************************************
 * @file    pke_portable.h
 *
 * @brief   This is the header file for TL721X
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
#ifndef PKE_PORTABLE_H
#define PKE_PORTABLE_H
#include "driver.h"


#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief Initialize PKE-related generic configurations.
 * @note        Only after calling this function can other PKE related functions be called.
 *              Otherwise, other PKE function settings will not take effect.
 * @return None.
 */
    void pke_dig_en(void);

    /**
 * @brief     Resets PKE module,before using PKE, it is needed to call pke_reset() to avoid affecting the use of PKE.
 * @return    none
 */
    static inline void pke_reset(void)
    {
        reg_rst1 &= ~FLD_RST1_PKE;
        reg_rst1 |= FLD_RST1_PKE;
    }

    /**
 * @brief     Enable the clock of PKE module.
 * @return    none
 */
    static inline void pke_clk_en(void)
    {
        reg_clk_en1 |= FLD_CLK1_PKE_EN;
    }

#ifdef __cplusplus
}
#endif

#endif
