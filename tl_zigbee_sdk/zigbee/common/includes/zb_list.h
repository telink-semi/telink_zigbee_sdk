/********************************************************************************************************
 * @file	zb_list.h
 *
 * @brief	This is the header file for zb_list
 *
 * @author	Zigbee Group
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
#ifndef ZB_LIST_H
#define ZB_LIST_H

enum{
	ZB_LIST_ACTIVE,
	ZB_LIST_FREE,
};


#define ZB_LIST_ADD(listName, head, ele)	do{	\
												if(listName == ZB_LIST_FREE){	\
													(ele)->freeNext = head;		\
												}else{							\
													(ele)->activeNext = head;	\
												}								\
												head = ele;						\
											}while(0)


#define ZB_LIST_DELETE(listName, head, ele)	do{	\
												__typeof(head) list;											\
												list = head;													\
												if(listName == ZB_LIST_FREE){									\
													if(ele == head){											\
														head = (head)->freeNext; 								\
													}else{ 														\
														while(list->freeNext && (list->freeNext != (ele))){		\
															list = list->freeNext;								\
														}														\
														if(list->freeNext){										\
															list->freeNext = ele->freeNext;						\
														}														\
													}															\
												}else{															\
													if(ele == head){											\
														head = (head)->activeNext; 								\
													}else{ 														\
														while(list->activeNext && (list->activeNext != (ele))){	\
															list = list->activeNext;							\
														}														\
														if(list->activeNext){									\
															list->activeNext = ele->activeNext;					\
														}														\
													}															\
												}																\
											}while(0)

#endif /* ZB_LIST_H */
