/********************************************************************************************************
 * @file    pem.h
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
/** @page PEM
 *
 *  Introduction
 *  ===============
 *  Peripheral Event Matrix (PEM)
 *
 *  API Reference
 *  ===============
 *  Header File: pem.h
 *
 *  PEM features
 *  ===============
 *    - The function is to realize the interconnection between peripherals, routing the event signal of any peripheral A to the task input of any peripheral B,
 *    - and peripheral B regards the task signal as the enable or trigger signal.
 *
 *  How to use this driver
 *  ===============
 *    - PEM Initialization and Configuration:
 *        -# Configure the event/task module,signal num,trigger mode and clk by pem_event_config(),pem_task_config() API;
 *        -# Enable or disable pem module by pem_chn_en(), pem_chn_dis() API;
 */

#ifndef PEM_H_
#define PEM_H_

#include "reg_include/register.h"

/**
 * @brief pem channel.
 */
typedef enum
{
    PEM0 = 0,
    PEM1,
    PEM2,
    PEM3,
    PEM4,
    PEM5,
    PEM6,
    PEM7,
    PEM8,
    PEM9,
    PEM10,
    PEM11,
    PEM12,
    PEM13,
    PEM14,
    PEM15,
    PEM16,
    PEM17,
    PEM18,
    PEM19,
    PEM20,
    PEM21,
    PEM22,
    PEM23,
} pem_chn_e;

/**
 * @brief pem event module.
 */
typedef enum
{
    PEM_EVENT_MSPI = 0,
    PEM_EVENT_GSPI = 2,
    PEM_EVENT_OSR_IP = 3,
    PEM_EVENT_GPIO = 4,
    PEM_EVENT_DMA = 5,
    PEM_EVENT_MISC = 6,
    PEM_EVENT_CPU = 7,
    PEM_EVENT_TIMER = 8,
    PEM_EVENT_STIMER = 9,
    PEM_EVENT_PWM = 13,
    PEM_EVENT_UART3 = 14,
    PEM_EVENT_RZ = 15,
    PEM_EVENT_ALGM = 16,
    PEM_EVENT_UART0 = 17,
    PEM_EVENT_UART1 = 18,
    PEM_EVENT_UART2 = 19,
    PEM_EVENT_I2C = 20,
    PEM_EVENT_ZB = 23,
    PEM_EVENT_UART4 = 28,
    PEM_EVENT_I2C1 = 29,
} pem_event_module_sel_e;

/**
 * @brief pem task module.
 */
typedef enum
{
    PEM_TASK_MSPI = 0,
    PEM_TASK_GSPI = 2,
    PEM_TASK_OSR_IP = 3,
    PEM_TASK_GPIO = 4,
    PEM_TASK_DMA = 5,
    PEM_TASK_MISC = 6,
    PEM_TASK_CPU = 7,
    PEM_TASK_TIMER = 8,
    PEM_TASK_STIMER = 9,
    PEM_TASK_PWM = 13,
    PEM_TASK_UART3 = 14,
    PEM_TASK_RZ = 15,
    PEM_TASK_ALGM = 16,
    PEM_TASK_UART0 = 17,
    PEM_TASK_UART1 = 18,
    PEM_TASK_UART2 = 19,
    PEM_TASK_I2C = 20,
    PEM_TASK_ZB = 23,
    PEM_TASK_UART4 = 28,
    PEM_TASK_I2C1 = 29,
} pem_task_module_sel_e;

/**
 * @brief pem clk.
 */
typedef enum
{
    CCLK = 0,
    HCLK,
    PCLK,
    ASYNC_CLK, /**< ASYNCHRONOUS CLOCK :a signal belonging to the asynchronous clock domain is a clock that is not synchronized with CCLK HCLK PCLK.*/
} pem_clk_sel_e;

/**
 * @brief pem trigger signal select.
 */
typedef enum
{
    PULSE = 0,
    LEVEL,
} pem_lvl_e;

/**
 *  @brief  Define rising/falling types
 */
typedef enum
{
    PEM_EVENT_RISING  = 0x00,
    PEM_EVENT_FALLING = 0x04,
    PEM_EVENT_BOTH = 0x01,
} pem_event_pol_e;

/**
 *  @brief  Define pem event struct.
 */
typedef struct
{
    pem_event_module_sel_e module : 5;
    unsigned char                 : 3;
    unsigned char                 : 8;
    unsigned char sig_sel         : 3;
    unsigned char                 : 3;
    pem_clk_sel_e     clk_sel     : 2;
    unsigned char     edge_detect : 1;
    unsigned char                 : 1;
    unsigned char inv             : 1;
    unsigned char                 : 1;
    pem_lvl_e lvl                 : 1;
    unsigned char                 : 1;
    unsigned char                 : 2;
} pem_event_config_t;

/**
 *  @brief  Define pem task struct.
 */
typedef struct
{
    unsigned char                : 8;
    pem_task_module_sel_e module : 5;
    unsigned char                : 3;
    unsigned char                : 3;
    unsigned char sig_sel        : 3;
    unsigned char                : 2;
    unsigned char                : 5;
    pem_lvl_e     lvl            : 1;
    pem_clk_sel_e clk_sel        : 2;
} pem_task_config_t;

/**
 * @brief      This function servers to enable pem that selected channel.
 * @param[in] chn - pem channel.
 * @return    none
 */
static inline void pem_chn_en(pem_chn_e chn)
{
    BM_SET(reg_pem_ctr(chn), FLD_PEM_CH_EN);
}

/**
 * @brief      This function servers to disable pem that selected channel.
 * @param[in] chn - pem channel.
 * @return    none
 */
static inline void pem_chn_dis(pem_chn_e chn)
{
    BM_CLR(reg_pem_ctr(chn), FLD_PEM_CH_EN);
}

/**
 * @brief      This function serves to enable pem function.
 * @return     none.
 */
void pem_init(void);

/**
 * @brief     This function servers to configure PEM channel and some configures.
 * @param[in] chn - to select the PEM channel.
 * @param[in] pem_event_config -  to setting serves to set the configuration of PEM event.
 * @return    none
 */
void pem_event_config(pem_chn_e chn, pem_event_config_t pem_event_config);

/**
 * @brief     This function servers to configure PEM channel and some configures.
 * @param[in] chn - to select the PEM channel.
 * @param[in] pem_task_config - to setting serves to set the configuration of PEM task.
 * @return    none
 */
void pem_task_config(pem_chn_e chn, pem_task_config_t pem_task_config);

#endif /* PEM_H_ */
