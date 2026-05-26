/********************************************************************************************************
 * @file    swap.h
 *
 * @brief   This is the header file for swap
 *
 * @author  Driver & Zigbee Group
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

void swapN(u8 *p, int n);
void swapX(const u8 *src, u8 *dst, int len);

void swap24(u8 dst[3], const u8 src[3]);
void swap32(u8 dst[4], const u8 src[4]);
void swap48(u8 dst[6], const u8 src[6]);
void swap56(u8 dst[7], const u8 src[7]);
void swap64(u8 dst[8], const u8 src[8]);
void swap128(u8 dst[16], const u8 src[16]);

