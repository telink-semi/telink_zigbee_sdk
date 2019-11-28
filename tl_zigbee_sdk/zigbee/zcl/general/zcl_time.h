/********************************************************************************************************
 * @file     zcl_timer.h
 *
 * @brief	 APIs statement for timer cluster
 *
 * @author
 * @date     June. 10, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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
#pragma once

#include "tl_common.h"
#include "../../zdo/zb_af.h"
#include "../zcl.h"


#define ZCL_ATTRID_TIME                      0x0000
#define ZCL_ATTRID_TIME_STATUS               0x0001
#define ZCL_ATTRID_TIMEZONE                  0x0002
#define ZCL_ATTRID_DST_START                 0x0003
#define ZCL_ATTRID_DST_END                   0x0004
#define ZCL_ATTRID_DST_SHIFT                 0x0005
#define ZCL_ATTRID_STANDARD_TIME             0x0006
#define ZCL_ATTRID_LOCAL_TIME                0x0007
#define ZCL_ATTRID_LAST_SET_TIME             0x0008
#define ZCL_ATTRID_VALID_UNTIL_TIME          0x0009




status_t zcl_time_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);




