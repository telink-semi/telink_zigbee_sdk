/********************************************************************************************************
 * @file    pem.h
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
    PEM_EVENT_LSPI,
    PEM_EVENT_GSPI,
    PEM_EVENT_SECURITY_IP,
    PEM_EVENT_GPIO,
    PEM_EVENT_DMA,
    PEM_EVENT_QDEC,
    PEM_EVENT_CPU,
    PEM_EVENT_TIMER,
    PEM_EVENT_STIMER,
    PEM_EVENT_SAR_ADC,
    PEM_EVENT_AUDIO,
    PEM_EVENT_IR_LEARN,
    PEM_EVENT_PWM_0,
    PEM_EVENT_PWM_1,
    PEM_EVENT_RZ,
    PEM_EVENT_ALGM,
    PEM_EVENT_UART0,
    PEM_EVENT_UART1,
    PEM_EVENT_UART2,
    PEM_EVENT_I2C,
    PEM_EVENT_KS,
    PEM_EVENT_USB,
    PEM_EVENT_ZB,
} pem_event_module_sel_e;

/**
 * @brief pem task module.
 */
typedef enum
{
    PEM_TASK_MSPI = 0,
    PEM_TASK_LSPI,
    PEM_TASK_GSPI,
    PEM_TASK_GPIO = 4,
    PEM_TASK_DMA,
    PEM_TASK_QDEC,
    PEM_TASK_CPU,
    PEM_TASK_TIMER,
    PEM_TASK_STIMER,
    PEM_TASK_SAR_ADC,
    PEM_TASK_AUDIO,
    PEM_TASK_IR_LEARN,
    PEM_TASK_PWM_0,
    PEM_TASK_PWM_1,
    PEM_TASK_RZ,
    PEM_TASK_ALGM,
    PEM_TASK_UART0,
    PEM_TASK_UART1,
    PEM_TASK_UART2,
    PEM_TASK_I2C,
    PEM_TASK_KS,
    PEM_TASK_USB,
    PEM_TASK_ZB,
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
 * @brief edge detection.
 * @note  convert the level signal into an edge signal.
 */
typedef enum
{
    RISING_EDGE = 0,
    RISING_FALLING_EDGE,
} pem_edge_detect_e;

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
    pem_edge_detect_e edge_detect : 1;
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
 * @param[in] task_signal - to select the PEM channel.
 * @param[in] task_module - to select the PEM task module.
 * @return    none
 */
void pem_task_config(pem_chn_e chn, unsigned int task_signal, pem_task_module_sel_e task_module);

/**
 * @brief     This function servers to configure PEM channel and some configures.
 * @param[in] chn - to select the PEM channel.
 * @param[in] event_signal - to select the event signal.
 * @param[in] event_module - to select the PEM event module.
 * @return    none
 */
void pem_event_config(pem_chn_e chn, unsigned int event_signal, pem_event_module_sel_e event_module);

#endif /* PEM_H_ */
