/********************************************************************************************************
 * @file    ev_rtc.h
 *
 * @brief   This is the header file for ev_rtc
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

typedef struct {
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 min;
    u8 sec;
} utcTime_t;

void ev_rtc_update(u32 updateTime);

extern UTCTime ev_rtc_utc2Second(utcTime_t *time);
extern void ev_rtc_secondsSet(UTCTime seconds);
extern UTCTime ev_rtc_secondsGet(void);
extern void ev_rtc_second2utc(utcTime_t *time, UTCTime utcSeconds);
