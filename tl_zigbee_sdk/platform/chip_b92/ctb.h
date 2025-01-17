/********************************************************************************************************
 * @file    ctb.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include "gpio.h"
#include "reg_include/register.h"

/**
 * @brief    Definition of the time for each channel scan.
 */
typedef enum
{
    CTB_CYCLE_4  = 0x00,
    CTB_CYCLE_8  = 0x01,
    CTB_CYCLE_16 = 0x02,
} ctb_samp_num_sel_e;

/**
 * @brief  Definition of the number of channels and the number of channel IRQ masks.
 * @note    Channel 0 has been canceled on chip A2.
 */
typedef enum
{
    CTB_CHANNEL_NUM_2 = 0x03,
    CTB_CHANNEL_NUM_3 = 0x07,
    CTB_CHANNEL_NUM_4 = 0x0F,
    CTB_CHANNEL_NUM_5 = 0x1F,
    CTB_CHANNEL_NUM_6 = 0x3F,
    CTB_CHANNEL_NUM_7 = 0x7F,
    CTB_CHANNEL_NUM_8 = 0xFF,
} ctb_channel_count_e,
    ctb_irq_e;

/**
 * @brief   Definition of CTB trigger threshold.
 */
typedef enum
{
    CTB_THRESHOLD_1 = 0x10,
    CTB_THRESHOLD_2 = 0x20,
    CTB_THRESHOLD_3 = 0x30,
    CTB_THRESHOLD_4 = 0x40,
} ctb_threshold_e;

/**
 * @brief   Definition of the time interval of each round of scanning.
 */
typedef enum
{
    CTB_INTERVAL_0MS   = 0x00,
    CTB_INTERVAL_1MS   = 0x01,
    CTB_INTERVAL_2MS   = 0x02,
    CTB_INTERVAL_4MS   = 0x03,
    CTB_INTERVAL_8MS   = 0x04,
    CTB_INTERVAL_16MS  = 0x05,
    CTB_INTERVAL_32MS  = 0x06,
    CTB_INTERVAL_64MS  = 0x07,
    CTB_INTERVAL_128MS = 0x08,
    CTB_INTERVAL_256MS = 0x09,
    CTB_INTERVAL_512MS = 0x0a,
    CTB_INTERVAL_1S    = 0x0b,
    CTB_INTERVAL_2S    = 0x0c,
    CTB_INTERVAL_4S    = 0x0d,
} ctb_interval_sel_e;

/**
 * @brief   Definition of channel bits.
 * @note    Channel 0 has been canceled on chip A2.
 */
typedef enum
{
    CTB_CHANNEL_1 = BIT(1),
    CTB_CHANNEL_2 = BIT(2),
    CTB_CHANNEL_3 = BIT(3),
    CTB_CHANNEL_4 = BIT(4),
    CTB_CHANNEL_5 = BIT(5),
    CTB_CHANNEL_6 = BIT(6),
    CTB_CHANNEL_7 = BIT(7),
} ctb_channel_e;

/**
 * @brief       This function serves to enable CTB.
 * @param[in]   none
 * @return      none
 */
static inline void ctb_en(void)
{
    //ana_0x04[7] - 0:enable CTB, 1:disable CTB
    analog_write_reg8(0x04, analog_read_reg8(0x04) & 0x7f);
}

/**
 * @brief       This function serves to enable automatic mode.
 * @param[in]   none
 * @return      none
 * @note        In the automatic mode, 8 channels are automatically scanned in a cycle and output results.
 */
static inline void ctb_auto_en(void)
{
    //ana_0x55[5] - 1:enable automatic mode
    analog_write_reg8(0x55, analog_read_reg8(0x55) | BIT(4));
}

/**
 * @brief       This function serves to disable automatic mode.
 * @param[in]   none
 * @return      none
 */
static inline void ctb_auto_dis(void)
{
    //ana_0x55[5] - 0:disable automatic mode
    analog_write_reg8(0x55, analog_read_reg8(0x55) & 0xef);
}

/**
 * @brief       This function serves to set the time for each channel scan.
 * @param[in]   samp_num_sel - enum variable of the time for each channel scan.
 * @return      none
 */
static inline void ctb_set_samp_num_sel(ctb_samp_num_sel_e samp_num_sel)
{
    //ana_0x55[1:0] - 00:4 cycles, 01:8 cycles, 10/11:16 cycles
    analog_write_reg8(0x55, (analog_read_reg8(0x55) & 0xfc) | samp_num_sel);
}

/**
 * @brief       This function serves to enable channels of CTB.
 * @param[in]   channel_count - enum variable of the number of channels.
 * @note        Channels must be enabled continuously in sequence.
 *              For example: If channel 4 is to be used, channel 1-3 will also be opened.(Channel 0 has been canceled on chip A2.)
 * @return      none
 */
static inline void ctb_channel_en(ctb_channel_count_e channel_count)
{
    //ana_0x56[7:0] - 11111111: enable all channels
    analog_write_reg8(0x56, channel_count);
}

/**
 * @brief       This function serves to set CTB trigger threshold.
 * @param[in]   th - set trigger threshold for CTB.
 * @return      none
 */
static inline void ctb_set_th_h(unsigned char th)
{
    //ana_0x57[7:4]
    analog_write_reg8(0x57, (analog_read_reg8(0x57) & 0x0f) | (th << 4));
}

/**
 * @brief       This function serves to set the time interval of each round of scanning.
 * @param[in]   interval_sel - enum variable of the time interval of each round of scanning.
 * @return      none
 */
static inline void ctb_set_interval_sel(ctb_interval_sel_e interval_sel)
{
    //ana_0x57[3:0]
    analog_write_reg8(0x57, (analog_read_reg8(0x57) & 0xf0) | interval_sel);
}

/**
 * @brief       This function serves to enable channel irq mask.
 * @param[in]   mask - enum variable of channel irq mask.
 * @return      none
 */
static inline void ctb_set_irq_mask(ctb_irq_e mask)
{
    //ana_0x58[7:0] - 11111111: enable irq mask for all channels
    analog_write_reg8(0x58, mask);
}

/**
 * @brief       This function serves to enable power down sequence.
 * @param[in]   none
 * @return      none
 */
static inline void ctb_pwdn_en(void)
{
    //ana_0x4d[6] - 1: power down sequence enable
    analog_write_reg8(0x4d, analog_read_reg8(0x4d) | BIT(6));
}

/**
 * @brief       This function serves to initialize CTB.
 * @param[in]   none
 * @return      none
 */
void ctb_init(void);

/**
 * @brief       This function servers to get the CTB trigger channel.
 * @@param[in]  channel - enum variable of channel.
 * @retval      non-zero -  the interrupt occurred.
 * @retval      zero     -  the interrupt did not occur.
 */
static inline unsigned char ctb_get_irq_status(ctb_channel_e channel)
{
    return (analog_read_reg8(0x70) & channel);
}

/**
 * @brief      This function servers to reset the ctb.
 * @return     none
 */
static inline void ctb_rst(void)
{
    analog_write_reg8(0x55, analog_read_reg8(0x55) & 0x7f);
}

/**
 * @brief      This function servers to enable signal of CTB external capacitor.
 * @return     none
 */
static inline void ctb_ext_cap_en(void)
{
    analog_write_reg8(0x22, analog_read_reg8(0x22) | 0x02);
}
