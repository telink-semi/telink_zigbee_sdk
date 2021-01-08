/********************************************************************************************************
 * @file	stack_cfg.h
 *
 * @brief	This is the header file for stack_cfg
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
#pragma once


/**
 *  @brief  Working channel
 *          Valid value: 11 ~ 26
 */
#define DEFAULT_CHANNEL                           15


/**
 *  @brief  Security
 */
#define SECURITY_ENABLE                           1


/**
 *  @brief  NVRAM
 */
#define NV_ENABLE                                 1


/**********************************************************************
 * Following parameter need user to adjust according the app requirement
 */
/**
 *  @brief  ZCL: MAX number of cluster list, in cluster number add  + out cluster number
 *
 */
#define	ZCL_CLUSTER_NUM_MAX						10

/**
 *  @brief  ZCL: maximum number for zcl reporting table
 *
 */
#define ZCL_REPORTING_TABLE_NUM					4

/**
 *  @brief  ZCL: maximum number for zcl scene table
 *
 */
#define	ZCL_SCENE_TABLE_NUM						8

/**
 *  @brief  APS: MAX number of groups size in the group table
 *          In each group entry, there is 8 endpoints existed.
 */
#define APS_GROUP_TABLE_NUM                   	8

/**
 *  @brief  APS: MAX number of binding table size
 */
#define APS_BINDING_TABLE_NUM                 	8


/**********************************************************************
 * Following configuration will calculated automatically
 */

/**
   Auto definition for the role
 */
#if (COORDINATOR)
    #define ZB_ROUTER_ROLE                        1
    #define ZB_COORDINATOR_ROLE                   1
#elif (ROUTER)
    #define ZB_ROUTER_ROLE                        1
#elif (END_DEVICE)
    #define ZB_ED_ROLE                            1
#endif

#if ZB_ROUTER_ROLE
	#define GP_SUPPORT_ENABLE					  1
#endif

#if (SECURITY_ENABLE)
    #define ZB_SECURITY
#endif


