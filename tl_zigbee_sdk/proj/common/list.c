/********************************************************************************************************
 * @file     list.c
 *
 * @brief    list link file
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
