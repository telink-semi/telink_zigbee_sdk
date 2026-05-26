/********************************************************************************************************
 * @file    qdec.c
 *
 * @brief   This is the source file for TL321X
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
#include "qdec.h"

/**
 * @brief      This function servers to set input port.
 * @param[in]  chn_a - input types of a channel.
 * @param[in]  chn_b - input types of b channel.
 * @return     none.
 */
void qdec_set_pin(qdec_chn_e chn_a, qdec_chn_e chn_b)
{
    reg_qdec_channel_a = chn_a;
    reg_qdec_channel_b = chn_b;
}

/**
 * @brief      This function servers to set qdec mode,qdec mode:common mode and double accuracy mode.
 *             common mode:the qdec counter value is increased/decreased by 1 only when the same rising/falling edges are detected from the two phase signals.
 *             double accuracy mode:the qdec counter value is increased/decreased by 1 on each rising/falling edge of the two phase signals;
 *             the counter value will be increased/decreased by 2 for one wheel rolling.
 * @param[in]  mode - mode type to select.
 * @return     none.
 */
void qdec_set_mode(qdec_mode_e mode)
{
    if (mode == COMMON_MODE) {
        reg_qdec_mode &= (~FLD_QDEC_DOUBLE0);
    } else if (mode == DOUBLE_ACCURACY_MODE) {
        reg_qdec_mode |= FLD_QDEC_DOUBLE0;
    }
}

/**
 * @brief      This function servers to initial qedc source clock.
 * @param[in]  none.
 * @return     none.
 */
void qdec_clk_en(void)
{
    clock_32k_init(CLK_32K_RC);
    clock_cal_32k_rc();
    reg_clk_en3 |= FLD_CLK3_QDEC_EN;
    qdec_reset();
}

/**
 * @brief      This function servers to read hardware counting value,
 *             After reading the value, if there is no input to channels A and B, then reading the value is 0 through this function,
 *             If the count is not read, the qdec read real time counting value increases or decreases with the direction of the wheel according to the qdec mode.
 * @param[in]  none.
 * @return     hardware counting value.
 */
signed char qdec_get_count_value(void)
{
    //before reading the hardware counting value,write 1 to reg_qdec_load.
    reg_qdec_load |= FLD_QDEC_COUNT0_RELOAD;
    // Wait for the count0_reload register to change from 1 to 0 before reading the value, otherwise it will read the old value
    while (FLD_QDEC_COUNT0_RELOAD == (reg_qdec_load & FLD_QDEC_COUNT0_RELOAD)) {
    }
    return reg_qdec_count0;
}

/**
 * @brief      This function servers to reset qdec and the qdec counter value is cleared zero.
 * @param[in]  none.
 * @return     none.
 */
void qdec_reset(void)
{
    reg_rst3 &= ~(FLD_RST3_QDEC);
    reg_rst3 |= FLD_RST3_QDEC;
}

/**
 * @brief      This function servers to set hardware debouncing.
 * @param[in]  thrsh - any signal with width lower than the threshold will be regarded as jitter,
 *             effective signals input from Channel A and B should contain high/low level with width more than the threshold.
 * @return     none.
 */
void qdec_set_debouncing(qdec_thrsh_e thrsh)
{
    //the threshold: 2^(n+1) *clk_32kHz *3 (n=0xd1[2:0]))
    reg_qdec_dbntime &= (~FLD_QDEC_DBNTIME);
    reg_qdec_dbntime |= thrsh;
}
