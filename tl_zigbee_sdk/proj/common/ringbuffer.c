/********************************************************************************************************
 * @file    ringbuffer.c
 *
 * @brief   This is the source file for ringbuffer
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
#include "ringbuffer.h"
#include "types.h"
#include "string.h"

ringbuffer_t *ringbuffer_init(ringbuffer_t *ringbuf, unsigned char *buf, unsigned int size)
{
    if ((NULL == ringbuf) || (NULL == buf)) {
    	return NULL;
    }

    ringbuf->buf = buf;
    ringbuf->buf_size = size;
    ringbuf->read_idx = 0;
    ringbuf->read_mirror = 0;
    ringbuf->write_idx = 0;
    ringbuf->write_mirror = 0;

    return ringbuf;
}

unsigned int ringbuffer_data_len(ringbuffer_t *ringbuf)
{
	unsigned int len = 0;

	if (ringbuf->read_idx == ringbuf->write_idx) {
		if (ringbuf->read_mirror == ringbuf->write_mirror) {
			len = 0;//is empty
		} else {
			len = ringbuf->buf_size;//is full
		}
	} else {
		if (ringbuf->write_idx > ringbuf->read_idx) {
			len = ringbuf->write_idx - ringbuf->read_idx;
		} else {
			len = ringbuf->buf_size - (ringbuf->read_idx - ringbuf->write_idx);
		}
	}

	return len;
}

unsigned int ringbuffer_put(ringbuffer_t *ringbuf, const unsigned char *data, unsigned int length)
{
	unsigned int space_len = ringbuf->buf_size - ringbuffer_data_len(ringbuf);

	if (0 == space_len) {
		return 0;
	}

	if (space_len < length) {
		length = space_len;
	}

	if (ringbuf->buf_size - ringbuf->write_idx > length) {
		memcpy(&ringbuf->buf[ringbuf->write_idx], data, length);
		ringbuf->write_idx += length;
	} else {
		memcpy(&ringbuf->buf[ringbuf->write_idx], &data[0], ringbuf->buf_size - ringbuf->write_idx);
		memcpy(&ringbuf->buf[0], &data[ringbuf->buf_size - ringbuf->write_idx], length - (ringbuf->buf_size - ringbuf->write_idx));
		ringbuf->write_idx = length - (ringbuf->buf_size - ringbuf->write_idx);

		ringbuf->write_mirror = ~ringbuf->write_mirror;
	}

	return length;
}

unsigned int ringbuffer_get(ringbuffer_t *ringbuf, unsigned char *data, unsigned int length)
{
	unsigned int data_len = ringbuffer_data_len(ringbuf);

	if (0 == data_len) {
		return 0;
	}

	if (data_len < length) {
		length = data_len;
	}

	if (ringbuf->buf_size - ringbuf->read_idx > length) {
		memcpy(data, &ringbuf->buf[ringbuf->read_idx], length);
		ringbuf->read_idx += length;
	} else {
		memcpy(&data[0], &ringbuf->buf[ringbuf->read_idx], ringbuf->buf_size - ringbuf->read_idx);
		memcpy(&data[ringbuf->buf_size - ringbuf->read_idx], &ringbuf->buf[0], length - (ringbuf->buf_size - ringbuf->read_idx));
		ringbuf->read_idx = length - (ringbuf->buf_size - ringbuf->read_idx);

		ringbuf->read_mirror = ~ringbuf->read_mirror;
	}

	return length;
}
