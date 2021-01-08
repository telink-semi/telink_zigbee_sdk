/********************************************************************************************************
 * @file	list.c
 *
 * @brief	This is the source file for list
 *
 * @author	Driver & Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#include "types.h"
#include "list.h"

struct list{
	struct list *next;
};

void listInit(list_t list){
	*list = NULL;
}

void *listHead(list_t list){
	return *list;
}

void listCopy(list_t dest, list_t src){
	*dest = *src;
}

void *listTail(list_t list){
	struct list *l;

	if(*list == NULL){
		return NULL;
	}

	for(l = *list; l->next != NULL; l = l->next);

	return l;
}

void listAdd(list_t list, void *item){
	struct list *l;

	((struct list *)item)->next = NULL;

	l = listTail(list);
	if(l == NULL){
		*list = item;
	}else{
		l->next = item;
	}
}

void listPush(list_t list, void *item){
	((struct list *)item)->next = *list;
	*list = item;
}

void *listChop(list_t list){
	struct list *l, *p;

	if(*list == NULL){
		return NULL;
	}

	if(((struct list *)*list)->next == NULL){
		l = *list;
		*list = NULL;
		return l;
	}

	for(l = *list; l->next->next != NULL; l = l->next);

	p = l->next;
	l->next = NULL;

	return p;
}

void *listPop(list_t list){
	if(*list != NULL){
		*list = ((struct list *)*list)->next;
	}

	return *list;
}

void listRemove(list_t list, void *item){
	struct list *l, *p;

	if(*list == NULL){
		return;
	}

	p = NULL;
	for(l = *list; l != NULL; l = l->next){
		if(l == item){
			if(p == NULL){
				*list = l->next;
			}else{
				p->next = l->next;
			}
			l->next = NULL;
			return;
		}
		p = l;
	}
}

int listLength(list_t list){
	struct list *l;
	int num = 0;

	for(l = *list; l != NULL; l = l->next){
		++num;
	}

	return num;
}

void listInsert(list_t list, void *prevItem, void *newItem){
	if(prevItem == NULL){
		listPush(list, newItem);
	}else{
		((struct list *)newItem)->next = ((struct list *)prevItem)->next;
		((struct list *)prevItem)->next = newItem;
	}
}
