/********************************************************************************************************
 * @file    zcl_nv.h
 *
 * @brief   This is the header file for zcl_nv
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
#ifndef ZCL_NV_H
#define ZCL_NV_H


/***********************************************************************
 * @brief   Save reporting table information
 *
 * @param
 *
 * @return	nv_sts_t
 *
 */
nv_sts_t zcl_reportingTab_save(void);

/***********************************************************************
 * @brief   Restore reporting table information
 *
 * @param
 *
 * @return	nv_sts_t
 *
 */
nv_sts_t zcl_reportingTab_restore(void);

/***********************************************************************
 * @brief   Save scene table information
 *
 * @param
 *
 * @return
 *
 */
nv_sts_t zcl_sceneTable_save(void);

/***********************************************************************
 * @brief   Restore scene table information
 *
 * @param
 *
 * @return
 *
 */
nv_sts_t zcl_sceneTable_restore(void);

/***********************************************************************//**
 * @brief   Save wwah apsLinkKeyAuth information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_apsLinkKeyAuthInfo_save(void);

/***********************************************************************//**
 * @brief   Restore wwah apsLinkKeyAuth information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_apsLinkKeyAuthInfo_restore(void);

/***********************************************************************//**
 * @brief   Save wwah apsAcksRequireInfo information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_apsAcksRequireInfo_save(void);

/***********************************************************************//**
 * @brief   Restore wwah apsAcksRequireInfo information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_apsAcksRequireInfo_restore(void);

/***********************************************************************//**
 * @brief   Save wwah useTCForClusterInfo information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_useTCForClusterInfo_save(void);

/***********************************************************************//**
 * @brief   Restore wwah useTCForClusterInfo information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_useTCForClusterInfo_restore(void);

 
#endif	/* ZCL_NV_H */
