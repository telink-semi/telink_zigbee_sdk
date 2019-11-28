/********************************************************************************************************
 * @file     list.h
 *
 * @brief	 link list header
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma once

#define LIST_CONCAT2(a, b) a##b
#define LIST_CONCAT(a, b) LIST_CONCAT2(a, b)

#define LIST(name) 		\
		static void *LIST_CONCAT(name, _list) = NULL; \
		static list_t name = (list_t)&LIST_CONCAT(name, _list)


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
