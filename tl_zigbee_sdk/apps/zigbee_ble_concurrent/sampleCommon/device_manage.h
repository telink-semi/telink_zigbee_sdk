/********************************************************************************************************
 * @file    device_manage.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef DEVICE_MANAGE_H_
#define DEVICE_MANAGE_H_

#if defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL323X)
#include "stack/ble/ble_multi/ble_common.h"
#include "stack/ble/ble_multi/hci/hci_event.h"

#ifndef ACL_CENTRAL_MAX_NUM
    #define ACL_CENTRAL_MAX_NUM 4 // ACL central maximum number
#endif

#ifndef ACL_PERIPHR_MAX_NUM
    #define ACL_PERIPHR_MAX_NUM 4 // ACL peripheral maximum number
#endif


#define INVALID_CONN_IDX         0xFF


#define DEVICE_CHAR_INFO_MAX_NUM (ACL_CENTRAL_MAX_NUM + ACL_PERIPHR_MAX_NUM)


#define CHAR_HANDLE_MAX          8

/***************** connection character device information ****************************
 *
 * Demo ACL Central device char_handle(ATT handle) define as follows, assuming that peer device(ACL peripheral) is TELINK HID device
 *   char_handle[0] :  MIC
 *   char_handle[1] :  Speaker
 *   char_handle[2] :  OTA
 *   char_handle[3] :  Consume Report In
 *   char_handle[4] :  Key Report In
 *   char_handle[5] :
 *   char_handle[6] :  BLE Module, SPP Server to Client
 *   char_handle[7] :  BLE Module, SPP Client to Server
 *************************************************************************************/

typedef struct
{
    u8 id_adrType; //identity address type
    u8 id_addr[6]; //identity address
    u8 reserved;
    u8 irk[16];
} rpa_addr_t;

//Attention: manual set 4 byte aligned
typedef struct
{
    u16 conn_handle;
    u8  conn_role;        // 0: ACL Central; 1: ACL Peripheral
    u8  conn_state;       // 1: connect;  0: disconnect

    u8 char_handle_valid; // 1: peer device's attHandle is available;   0: peer device's attHandle not available
    u8 rsvd[3];           // for 4 Byte align

    u8 peer_adrType;
    u8 peer_addr[6];
    u8 peer_RPA; //RPA: resolvable private address

    //rpa_addr_t *pPeer_RPA;    //only when peer  mac_address is RPA, this pointer is useful

    u16 char_handle[CHAR_HANDLE_MAX];
} dev_char_info_t;

extern dev_char_info_t conn_dev_list[];


extern u8 acl_conn_central_num;
extern u8 acl_conn_periphr_num;

/**
 * @brief       Used for add device information to conn_dev_list.
 * @param[in]   dev_char_info       - Pointer point to data buffer.
 * @return      0 ~ DEVICE_CHAR_INFO_MAX_NUM - 1: new connection index, insert success
 *              0xFF: insert failed
 */
int dev_char_info_insert(dev_char_info_t *dev_char_info);


/**
 * @brief       Used for add device information to conn_dev_list.
 * @param[in]   pConnEvt - LE connection complete event data buffer address.
 * @return      0 ~ DEVICE_CHAR_INFO_MAX_NUM - 1: new connection index, insert success
 *              0xFF: insert failed
 */
int dev_char_info_insert_by_conn_event(hci_le_connectionCompleteEvt_t *pConnEvt);


/**
 * @brief       Used for add device information to conn_dev_list.
 * @param[in]   pConnEvt - LE enhanced connection complete event data buffer address.
 * @return      0 ~ DEVICE_CHAR_INFO_MAX_NUM - 1: new connection index, insert success
 *              0xFF: insert failed
 */
int dev_char_info_insert_by_enhanced_conn_event(hci_le_enhancedConnCompleteEvt_t *pConnEvt);


/**
 * @brief       Used for delete device information from conn_dev_list by connHandle
 * @param[in]   connhandle       - connection handle.
 * @return      0: success
 *              1: no find
 */
int dev_char_info_delete_by_connhandle(u16 connhandle);

/**
 * @brief       Used for delete device information from conn_dev_list by peer mac_address
 * @param[in]   adr_type       - peer address type.
 * @param[in]   addr           - Pointer point to peer address.
 * @return      0: success
 *              1: no find
 */
int dev_char_info_delete_by_peer_mac_address(u8 adr_type, u8 *addr);


/**
 * @brief       Get device information by  peer device address.
 * @param[in]   adr_type       - peer address type.
 * @param[in]   addr           - Pointer point to peer address.
 * @return      0: no find
 *             !0: found
 */
dev_char_info_t *dev_char_info_search_by_peer_mac_address(u8 adr_type, u8 *addr);

/**
 * @brief       Get device information by connection handle.
 * @param[in]   connhandle       - connection handle.
 * @return      0: no find
 *             !0: found
 */
dev_char_info_t *dev_char_info_search_by_connhandle(u16 connhandle);


/**
 * @brief       Used for judge if current device conn_handle
 * @param[in]   connhandle       - connection handle.
 * @return
 */
bool dev_char_info_is_connection_state_by_conn_handle(u16 connhandle);


/**
 * @brief       Get ACL connection role by connection handle.
 * @param[in]   connhandle       - connection handle.
 * @return      0: ACL_ROLE_CENTRAL
 *              1: ACL_ROLE_PERIPHERAL
 *              2: connection handle invalid
 */
int dev_char_get_conn_role_by_connhandle(u16 connhandle);


/**
 * @brief       Get ACL connection index by connection handle.
 * @param[in]   connhandle       - connection handle.
 * @return      0xFF:     no connection index match
 *              others:   connection index
 */
int dev_char_get_conn_index_by_connhandle(u16 connhandle);


/* compatible with previous released SDK */
#define conn_master_num acl_conn_central_num
#define conn_slave_num  acl_conn_periphr_num

#endif
#endif /* DEVICE_MANAGE_H_ */
