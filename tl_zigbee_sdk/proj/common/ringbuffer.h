/********************************************************************************************************
 * @file    ringbuffer.h
 *
 * @brief   This is the header file for ringbuffer
 *
 * @author  Driver & Zigbee Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
	unsigned char *buf;
    unsigned int  buf_size;
    unsigned int  read_mirror:1;
    unsigned int  read_idx:31;
    unsigned int  write_mirror:1;
    unsigned int  write_idx:31;
} ringbuffer_t;

ringbuffer_t *ringbuffer_init(ringbuffer_t *ringbuf, unsigned char *buf, unsigned int size);
unsigned int ringbuffer_data_len(ringbuffer_t *ringbuf);
unsigned int ringbuffer_put(ringbuffer_t *ringbuf, const unsigned char *data, unsigned int length);
unsigned int ringbuffer_get(ringbuffer_t *ringbuf, unsigned char *data, unsigned int length);
