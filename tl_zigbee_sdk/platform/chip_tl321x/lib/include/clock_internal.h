/********************************************************************************************************
 * @file    clock_internal.h
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
/** @page CLOCK_INTERNAL
 *
 *  Introduction
 *  ===============
 *  TL321X clock setting.
 *
 *  API Reference
 *  ===============
 *  Header File: clock_internal.h
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "compiler.h"
#include "reg_include/register.h"

/**********************************************************************************************************************
 *                                          internal
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *                  This is just for internal debug purpose, users are prohibited from calling.
 *********************************************************************************************************************/
#define PLL_192M_CCLK_96M_HCLK_48M_PCLK_48M_MSPI_64M clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV3)

#define PLL_192M_CCLK_96M_HCLK_48M_PCLK_48M_MSPI_48M clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV4)
#define PLL_192M_CCLK_96M_HCLK_48M_PCLK_24M_MSPI_48M clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, CLK_DIV4)

#define PLL_96M_CCLK_96M_HCLK_48M_PCLK_48M_MSPI_48M clock_init(BASEBAND_PLL, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV2)
#define PLL_96M_CCLK_96M_HCLK_48M_PCLK_24M_MSPI_48M clock_init(BASEBAND_PLL, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, CLK_DIV2)

#define PLL_96M_CCLK_48M_HCLK_48M_PCLK_48M_MSPI_48M clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV1_TO_HCLK_DIV1_TO_PCLK, CLK_DIV2)
#define PLL_96M_CCLK_48M_HCLK_48M_PCLK_24M_MSPI_48M clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV1_TO_HCLK_DIV2_TO_PCLK, CLK_DIV2)
#define PLL_96M_CCLK_48M_HCLK_24M_PCLK_24M_MSPI_48M clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, CLK_DIV2)

#endif
