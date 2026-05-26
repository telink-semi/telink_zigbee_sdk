/********************************************************************************************************
 * @file    zcl_zbd.h
 *
 * @brief   This is the header file for zcl_basic
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/
#ifndef ZCL_ZBD_H
#define ZCL_ZBD_H


/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */
#define ZBD_INTERFACE_DISABLE                      0x00
#define ZBD_INTERFACE_ENABLE                       0x01

/** @getIntfStateRsp_t  set interface response
 *  @{
 */
typedef struct {
    u8 status;
    u8 currState;
} getIntfStateRsp_t;

/** @addtogroup zcl_ZBD_cmdId zbd attribute Ids
 * @{
 */
#define ZCL_ATTRID_ZBD_INTERFACE_STATE              0x0000
#define ZCL_ATTRID_ZBD_ANONYMOUS_JOIN_TIMEOUT       0x0001
/** @addtogroup zcl_ZBD_cmdId zbd Command Ids
 * @{
 */
#define ZCL_CMD_ZBD_SET_IF_STATE                     0x00
#define ZCL_CMD_ZBD_SET_AJOIN_TIMEOUT                0x01

#define ZCL_CMD_ZBD_SET_IF_STATE_RSP                 0x00

#define ZCL_ATTRID_ZBD_INSTATE_MIN                   0x0
#define ZCL_ATTRID_ZBD_INSTATE_MAX                   0x1

#define ZCL_ATTRID_ZBD_AJOIN_MIN                     0x0
#define ZCL_ATTRID_ZBD_AJOIN_MAX                     0x100000


/**
 *  @brief  Definition format for set interface state command
 */
typedef struct {
    u8 state;
} zcl_inteStateCmd_t;

/**
 *  @brief  Definition format for set anonymous join timeout time
 */
typedef struct {
    u24 time;
} zcl_anJoinTimeCmd_t;

/**
 *  @brief  Definition format for zbd command payload
 */
typedef union {
    zcl_inteStateCmd_t interface;
    zcl_anJoinTimeCmd_t anJoinTimeout;
} zcl_zbd_cmdPayload_t;

/**
 * @brief       This function register ZBD cluster server to ZCL. Only after registered,
 *              basic attributes could be used.
 *
 * @param[in]   attrNum  - used attribute number
 * @param[in]   pAttrTbl - pointer to server side attribute table. It is implemented in zcl_basic_attr.c
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_zbd_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


/**
 * @brief       API to send Identify command in IDENTIFY cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   state - specified the zigbee direct interface state
 * @manuCode[in]   manuCode - manufacture code, can set to 0 if general command
 * @return      None
 */
status_t zcl_zbd_setIfState(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 state);
#define zcl_zbd_setIfStateCmd(a,b,c,d)  (zcl_zbd_setIfState((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send Identify command in IDENTIFY cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   timeSet - specified the zigbee direct AnonymousJoinTimeout
 * @manuCode[in]   manuCode - manufacture code, can set to 0 if general command
 * @return      None
 */
status_t zcl_zbd_setAnjTimeout(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u24 timeSet);
#define zcl_zbd_setAnjTimeoutCmd(a,b,c,d)   (zcl_zbd_setAnjTimeout((a), (b), (c), ZCL_SEQ_NUM, (d)))
/** @} end of zbd ZCL_Module */

/** @} end of zbd TELINK_ZIGBEE_STACK */

#endif  /* ZCL_ZBD_H */
