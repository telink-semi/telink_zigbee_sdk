/********************************************************************************************************
 * @file     zb_list.h
 *
 * @brief    list operate function head file
 *
 * @author
 * @date     May. 27, 2017
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef ZB_LIST_H
#define ZB_LIST_H 1

enum{
	ZB_LIST_ACTIVE,
	ZB_LIST_FREE,
};


#define ZB_LIST_ADD(listName, head, ele)	\
	do{		\
		if(listName == ZB_LIST_FREE){				\
			(ele)->freeNext = head;	\
		}else{						\
			(ele)->activeNext = head;	\
		}							\
		head = ele;					\
	}while(0)


#define ZB_LIST_DELETE(listName, head, ele)			\
	do{	\
		__typeof(head) list;		\
		list = head;		\
		if(listName == ZB_LIST_FREE){		\
			if(ele == head){	\
				head = (head)->freeNext; \
			}else{ \
				while(list->freeNext && (list->freeNext != (ele))){	\
					list = list->freeNext;	\
				}	\
				if(list->freeNext){	\
					list->freeNext = ele->freeNext;	\
				}	\
			}	\
		}else{	\
			if(ele == head){	\
				head = (head)->activeNext; \
			}else{ \
				while(list->activeNext && (list->activeNext != (ele))){	\
					list = list->activeNext;	\
				}	\
				if(list->activeNext){	\
					list->activeNext = ele->activeNext;	\
				}	\
			}	\
		}	\
	}while(0)

#endif /* ZB_LIST_H */
