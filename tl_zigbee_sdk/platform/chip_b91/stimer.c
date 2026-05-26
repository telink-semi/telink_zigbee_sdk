/********************************************************************************************************
 * @file    stimer.c
 *
 * @brief   This is the source file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "stimer.h"

unsigned int g_track_32kcnt = 16;

/**
 * @brief     This function performs to set delay time by us.
 * @param[in] microsec - need to delay.
 * @return    none
*/
void delay_us(unsigned int microsec)
{
    unsigned long t = stimer_get_tick();
    while (!clock_time_exceed(t, microsec)) {
    }
}

/*
 * @brief     This function performs to set delay time by ms.
 * @param[in] millisec - need to delay.
 * @return    none
*/
void delay_ms(unsigned int millisec)
{
    unsigned long t = stimer_get_tick();
    while (!clock_time_exceed(t, millisec * 1000)) {
    }
}
