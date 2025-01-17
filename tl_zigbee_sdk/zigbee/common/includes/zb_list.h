/********************************************************************************************************
 * @file    zb_list.h
 *
 * @brief   This is the header file for zb_list
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
#ifndef ZB_LIST_H
#define ZB_LIST_H

enum {
    ZB_LIST_ACTIVE,
    ZB_LIST_FREE,
};


#define ZB_LIST_ADD(listName, head, ele)        do{ \
                                                    if (listName == ZB_LIST_FREE) { \
                                                        (ele)->freeNext = head; \
                                                    } else { \
                                                        (ele)->activeNext = head; \
                                                    } \
                                                    head = ele; \
                                                }while(0)


#define ZB_LIST_DELETE(listName, head, ele)     do{ \
                                                    __typeof(head) list; \
                                                    list = head; \
                                                    if (listName == ZB_LIST_FREE) { \
                                                        if (ele == head) { \
                                                            head = (head)->freeNext; \
                                                        } else { \
                                                            while (list->freeNext && (list->freeNext != (ele))) { \
                                                                list = list->freeNext; \
                                                            } \
                                                            if (list->freeNext) { \
                                                                list->freeNext = ele->freeNext; \
                                                            } \
                                                        } \
                                                    } else { \
                                                        if (ele == head) { \
                                                            head = (head)->activeNext; \
                                                        } else { \
                                                            while (list->activeNext && (list->activeNext != (ele))) { \
                                                                list = list->activeNext; \
                                                            } \
                                                            if (list->activeNext) { \
                                                                list->activeNext = ele->activeNext; \
                                                            } \
                                                        } \
                                                    } \
                                                }while(0)

#endif /* ZB_LIST_H */
