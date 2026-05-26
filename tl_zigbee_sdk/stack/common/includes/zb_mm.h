/********************************************************************************************************
 * @file    zb_mm.h
 *
 * @brief   This is the header file for zb_mm
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
#ifndef ZB_MM_H
#define ZB_MM_H

#define STR_CONCAT(s1, s2) s1##s2

#define TN_MEM_DEF(name, type, num) \
    static char STR_CONCAT(name, _memb_count)[num]; \
    static type STR_CONCAT(name, _memb_mem)[num]; \
    static struct mem_desc name = {sizeof(type), num, \
                                   STR_CONCAT(name, _memb_count), \
                                   (void *)STR_CONCAT(name, _memb_mem)}

struct mem_desc {
    unsigned short size;
    unsigned short num;
    char *count;
    void *mem;
};

void zb_mem_init(struct mem_desc *m);
void *zb_mem_alloc(struct mem_desc *m);
char zb_mem_free(struct mem_desc *m, void *ptr);
int zb_mem_inmemb(struct mem_desc *m, void *ptr);
int zb_mem_numfree(struct mem_desc *m);

#endif /* ZB_MM_H */
