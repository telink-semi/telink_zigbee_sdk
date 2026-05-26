/********************************************************************************************************
 * @file    zb_primitive_task.h
 *
 * @brief   This is the header file for zb_primitive_task
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
#ifndef ZB_PRIMITIVE_TASK_H
#define ZB_PRIMITIVE_TASK_H


#if 0
u8 my_tl_zbPrimitivePost(u8 layerQ, u8 primitive, void *arg, u32 line, char *file);
#define tl_zbPrimitivePost(layerQ, primitive, arg)      my_tl_zbPrimitivePost(layerQ, primitive, arg, __LINE__, __FILE__)
#else
u8 tl_zbPrimitivePost(u8 layerQ, u8 primitive, void *arg);
#endif

#endif /* ZB_PRIMITIVE_TASK_H */
