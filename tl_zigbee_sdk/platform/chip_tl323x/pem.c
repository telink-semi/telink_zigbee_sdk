/********************************************************************************************************
 * @file    pem.c
 *
 * @brief   This is the source file for tl323x
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
#include "pem.h"

/**
 * @brief      This setting serves to set the configuration of stimer PEM event.
 */
pem_event_config_t g_pem_event_config = {
    .module      = PEM_EVENT_PWM_0,
    .sig_sel     = 0,
    .clk_sel     = PCLK,
    .lvl         = PULSE,
    .edge_detect = RISING_EDGE,
    .inv         = 0,
};

/**
 * @brief      This setting serves to set the configuration of stimer PEM task.
 */
pem_task_config_t g_pem_task_config = {
    .module  = PEM_TASK_STIMER,
    .sig_sel = 0,
    .clk_sel = PCLK,
    .lvl     = PULSE,
};

/**
 * @brief      This function serves to enable pem function.
 * @return     none.
 */
void pem_init(void)
{
    BM_SET(reg_rst5, FLD_RST5_PEM);
    BM_SET(reg_clk_en5, FLD_CLK5_PEM_EN);
}

/**
 * @brief     This function servers to configure PEM channel and some configures.
 * @param[in] chn - to select the PEM channel.
 * @param[in] event_signal - to select the event signal.
 * @param[in] event_module - to select the event module.
 * @return    none
 */
void pem_event_config(pem_chn_e chn, unsigned int event_signal, pem_event_module_sel_e event_module)
{
    g_pem_event_config.module  = event_module;
    g_pem_event_config.sig_sel = event_signal;
    reg_pem_ctr(chn)           = (reg_pem_ctr(chn) & (~(FLD_PEM_EVENT_MODULE_SEL | FLD_PEM_EVENT_SIG_SEL | FLD_PEM_EVENT_CLK_SEL | FLD_PEM_EVENT_CHANGE_LVL_TO_EDGE_SEL | FLD_PEM_EVENT_LVL))) | (*(unsigned int *)&g_pem_event_config);
}

/**
 * @brief     This function servers to configure PEM channel and some configures.
 * @param[in] chn - to select the PEM channel.
 * @param[in] event_signal - to select the event signal.
 * @param[in] task_module - to select the task module.
 * @return    none
 */
void pem_task_config(pem_chn_e chn, unsigned int task_signal, pem_task_module_sel_e task_module)
{
    g_pem_task_config.module  = task_module;
    g_pem_task_config.sig_sel = task_signal;
    reg_pem_ctr(chn)          = (reg_pem_ctr(chn) & (~(FLD_PEM_TASK_MODULE_SEL | FLD_PEM_TASK_SIG_SEL | FLD_PEM_TASK_LVL | FLD_PEM_TASK_CLK_SEL))) | (*(unsigned int *)&g_pem_task_config);
}
