/********************************************************************************************************
 * @file    list.h
 *
 * @brief   This is the header file for list
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

#define LIST_CONCAT2(a, b)      a##b
#define LIST_CONCAT(a, b)       LIST_CONCAT2(a, b)

#define LIST(name)              \
    static void *LIST_CONCAT(name, _list) = NULL; \
    static list_t name = (list_t)&LIST_CONCAT(name, _list);


typedef void ** list_t;

void listInit(list_t list);
void *listHead(list_t list);
void *listTail(list_t list);
void *listPop (list_t list);
void listPush(list_t list, void *item);
void *listChop(list_t list);
void listAdd(list_t list, void *item);
void listRemove(list_t list, void *item);
int listLength(list_t list);
void listCopy(list_t dest, list_t src);
void listInsert(list_t list, void *prevItem, void *newItem);
