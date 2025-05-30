/********************************************************************************************************
 * @file    zcl_time.h
 *
 * @brief   This is the header file for zcl_time
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
#ifndef ZCL_TIME_H
#define ZCL_TIME_H


#define ZCL_ATTRID_TIME                         0x0000
#define ZCL_ATTRID_TIME_STATUS                  0x0001
#define ZCL_ATTRID_TIMEZONE                     0x0002
#define ZCL_ATTRID_DST_START                    0x0003
#define ZCL_ATTRID_DST_END                      0x0004
#define ZCL_ATTRID_DST_SHIFT                    0x0005
#define ZCL_ATTRID_STANDARD_TIME                0x0006
#define ZCL_ATTRID_LOCAL_TIME                   0x0007
#define ZCL_ATTRID_LAST_SET_TIME                0x0008
#define ZCL_ATTRID_VALID_UNTIL_TIME             0x0009


status_t zcl_time_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

#endif	/* ZCL_TIME_H */
