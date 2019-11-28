/********************************************************************************************************
 * @file     utlist.h
 *
 * @brief    list interface header file
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
#ifndef UTLIST_H
#define UTLIST_H

#define LIST_ADD(head, ele)   \
do {                          \
  (ele)->next = head;        \
  head = ele;               \
} while (0)


#define LIST_DELETE(head, ele)   						\
do {                                                    \
	__typeof(head) lst;                                 \
	if ((head) == (ele)) {                               \
		(head) = (head)->next;                            \
	} else {                                            \
		lst = head;                                     \
		while (lst->next && (lst->next != (ele))) {      \
			lst = lst->next;                              \
		}                                               \
		if (lst->next) {                                \
			lst->next = ((ele)->next);                  \
		}                                               \
	}                                                   \
} while (0)



#define LIST_EXIST(head, ele, result)        		\
do {											\
	for(result = head; result; result = (result)->next){	\
		if (result == ele){						\
			break;								\
		}										\
	} 			       						 	\
} while(0) 

#endif /* UTLIST_H */

