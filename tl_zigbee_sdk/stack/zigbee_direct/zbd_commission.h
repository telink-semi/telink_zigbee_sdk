/********************************************************************************************************
 * @file    zbd_commission.h
 *
 * @brief   This is the header file for zbd_commission
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
#ifndef _ZBD_COMMISSION_H_
#define _ZBD_COMMISSION_H_


#include "zbd_common.h"

/**
 * @brief Commissioning Service TLV IDs
 */
typedef enum {
    EXTENDED_PANID_TYPE_ID,                     //0x00
    SHORT_PANID_TYPE_ID,                        //0x01
    NETWORK_CHANNEL_TYPE_ID,                    //0x02
    NETWORK_KEY_TYPE_ID,                        //0x03
    LINK_KEY_TYPE_ID,                           //0x04
    DEVICE_TYPE_TYPE_ID,                        //0x05
    NWK_ADDRESS_TYPE_ID,                        //0x06
    JOINING_METHOD_TYPE_ID,                     //0x07
    IEEE_ADDRESS_TYPE_ID,                       //0x08
    TRUST_CENTER_ADDRESS_TYPE_ID,               //0x09
    NETWORK_STATUS_TYPE_ID,                     //0x0A
    NWK_UPDATEID_TYPE_ID,                       //0x0B
    NWK_ACTIVE_KEY_SEQNUM_TYPE_ID,              //0x0C
    ADMIN_KEY_TYPE_ID,                          //0x0D
    STATUS_CODE_TYPE_ID,                        //0x0E
    EXTENDED_STATUS_CODE_TYPE_ID,               //0x0F
} ZBD_COMM_TLVID_E;

#define DIRECT_ADV_EXTENSION_LEN                    9

//Device Type TLV
#define ZIGBEE_COORDINATOR                          0x00
#define ZIGBEE_ROUTER                               0x01
#define ZIGBEE_END_DEVICE                           0x02

//Joining method TLV
#define JOIN_MAC_ASSOCIATION                        0x00
#define JOIN_SECURE_REJOINING                       0x01
#define JOIN_OUT_OF_BAND_COMMISSIONING              0x02

//Joined status
#define NWKSTATUS_NOT_COMMISSIONED                  0x00
#define NWKSTATUS_COMMISSIONING_IN_PROGRESS         0x01
#define NWKSTATUS_COMMISSIONED                      0x02
//#define JOINED_NETWORK_NO_PARENT                  0x03  //applicable to end devices

//Status of the Zigbee network
#define NETWORK_CLOSED                              0x00
#define NETWORK_OPEN                                0x01

//The type of the Zigbee network
#define DISTRIBUTED_NETWORK                         0x00
#define CENTRALIZED_NETWORK                         0x01

//Zigbee Direct Manage Joiners TLV IDs
typedef enum {
    PROVISIONAL_LINK_KEY,
    IEEE_ADDRESS,
    MANAGE_JOINERS_COMMAND,
} ZBD_MANAJOINERS_ID_E;

//Zigbee Direct Manage Joiners TLV IDs
typedef enum {
    DROP_ALL_JOINERS_KEYS,
    ADD_A_JOINER_KEY,
    REMOVE_A_JOINER_KEY,
} ZBD_MANAJOINERS_COMMAND_E;

#define COMMISSION_SERVICE_TIMEOUT                  3//3s

#define COMMISSION_COMMAND_LEN_MIN                  0
#define COMMISSION_COMMAND_LEN_MAX                  128

#define COMMISSION_STATUS_COMMAND_LEN_MAX           80

/* status code domain/operation values.
 * The 8-bit status code within the context/scope of the domain/operation
 */
typedef enum {
    GENERAL_UNSPECIFIC_OPERATION,                   //0x00
    FORM_NETWORK_OPERATION,                         //0x01
    JOIN_NETWORK_OPERATION,                         //0x02
    PERMIT_JOINING_OPERATION,                       //0x03
    LEAVE_NETWORK_OPERATION,                        //0x04
    MANAGE_JOINERS_DOMAIN,                          //0x05
    IDENTIFY_OPERATION,                             //0x06
    FINDING_BINDING_DOMAIN,                         //0x07
} ZBD_COMM_OPRCODE_E;

/**********************************************************************
 * TYPEDEFS
 */
typedef union {
    u8 flag;
    struct {
        u8 version:4;   /*Version of this Zigbee Direct extension to the BLE Advertisement. shall be 0x1*/
        u8 zdts:1;      /*if ZDD supports the Zigbee Direct Tunnel Service*/
        u8 permitJoin:1;/*permit join status*/
        u8 resv:2;
    } statusBits;
} zbd_adv_flag_t;

typedef struct _attribute_packed_{
    u8 channelCnt;
    u32 channelMask[0];
} tl_zb_channelList_t;

typedef union {
    u8 status;
    struct {
        u8 joinedStatus:3;
        u8 openStatus:1;
        u8 networkType:1;
        u8 resv:3;
    } statusBits;
} zbd_comm_netStatus_t;

typedef struct {
    u8 operCode;
    u8 statusCode;
} zbd_comm_statuscode_t;

typedef int (*app_zbd_commChange_t)(void);
typedef int (*app_zbd_advDataChange_t)(void);


typedef struct {
  u16 bleConnHandle;
  u16 bleServiceHandle;
  u16 bleAttrHandle;
  u16 attDataLen;
  u8* attrData;
} zbd_comm_req_t;

/**
 * @brief  Register callback function to send commissioning status fail notification for the user
 *
 * @param  cb - application callback during commissioning command handle
 *
 * @return none
 */
void zbd_commissionInit(app_zbd_commChange_t commCb, app_zbd_advDataChange_t advCb);

/**
 * @brief  Zigbee Direct extension to the BLE Advertisement
 *
 * @param  advDataExt - the original BLE advertisement data
 * @param  len        - the original BLE advertisement length
 * @param  suppTunnel - the app whether support direct tunnel service
 *
 * @return none
 */
void zbd_extendAdvData(u8 *advDataExt, u8 *len, u8 suppTunnel);

/**
 * @brief  The function is used to get the commissioning status.
 *
 * @param  connHandle - connection handle
 * @param  attHandle  - attribute handle
 * @param  statusNoti - the commissioning status content
 * @param  realLen    - the length of commissioning status content
 *
 * @return ZBD_RET_STA_E
 */
ZBD_RET_E zbd_commissStatusReadHandle(u16 connHandle, u16 attHandle, u8 *statusNoti, u8 *realLen);

/**
 * @brief      The function is used to get the identify time.
 *
 * @param  connHandle   - connection handle
 * @param  attHandle    - attribute handle
 * @param  identifyData - the identify time content
 * @param  realLen      - the length of identify time content
 *
 * @return none
 */
void zbd_commissIdentifyTimeRead(u16 connHandle, u16 attHandle, u8 *identifyData, u8 *realLen);

/**
 * @brief  The function is used to check the update of zigbee network, and to send commissioning notification,
 *         update the BLE advertisement.
 *
 * @param  none
 *
 * @return none
 */
void zbd_zbNwkParaChangeDetect(void);

/**
 * @brief  The function is used to send commissioning status notification.
 *
 * @param  connHandle - connection handle
 * @param  attHandle  - attribute handle
 * @param  statusNoti - the commissioning status content
 * @param  realLen    - the length of commissioning status content content
 *
 * @return ZBD_RET_STA_E
 */
ZBD_RET_E zbd_commissStatusNotifSend(u16 connHandle, u16 attHandle, u8 *statusNoti, u8 *realLen);

/**
 * @brief  The function is used to send commissioning status notification.
 *
 * @param  connHandle - connection handle
 * @param  attHandle  - attribute handle
 * @param  retStatus  - the return status
 *
 * @return none
 */
u8 zbd_commissDomainStatusNotifSend(u16 connHandle, u16 attHandle, ZBD_RET_E retStatus);

/**
 * @brief  The function is used to handle zigbee form network command.
 *
 * @param  connHandle   - connection handle
 * @param  attHandle    - attribute handle.
 * @param  statusHandle - report status attribute handle.
 * @param  operCode     - operate code.
 * @param  totalLen     - the length of the command
 * @param  commandData  - the pointer to the command
 *
 * @return none
 */
u8 zbd_commissionCharaWriteHandle(u16 connHandle, u16 attHandle, u16 statusHandle, u16 operCode, u8 totalLen, u8 *commandData);

/**
 * @brief  The function is used to report the zigbee commissioning result.
 *
 * @param  connHandle - connection handle
 * @param  attHandle  - attribute handle
 * @param  status     - commissioning status
 *
 * @return none
 */
void zbd_commissionRetNotify(u16 connHandle, u16 attrHandle, u8 status);
#endif
