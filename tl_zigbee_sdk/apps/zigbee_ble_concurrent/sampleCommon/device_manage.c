/********************************************************************************************************
 * @file    device_manage.c
 *
 * @brief   This is the source file for BLE SDK
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
#if defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || defined(MCU_CORE_TL521X)
#include "tl_common.h"
#include "stack/ble/ble.h"
#include "device_manage.h"

/*
 * Used for store information of connected devices.
 *
 * 0 ~ (ACL_CENTRAL_MAX_NUM - 1) is for ACL Central,  ACL_CENTRAL_MAX_NUM ~ (ACL_CENTRAL_MAX_NUM + ACL_PERIPHR_MAX_NUM - 1) s for ACL Peripheral
 *
 * e.g.    ACL_CENTRAL_MAX_NUM  ACL_PERIPHR_MAX_NUM       ACL Central             ACL Peripheral
 *                0                     1                    none               conn_dev_list[0]
 *                0                     2                    none               conn_dev_list[0..1]
 *                0                     3                    none               conn_dev_list[0..2]
 *                0                     4                    none               conn_dev_list[0..3]
 *
 *                1                     0               conn_dev_list[0]               none
 *                1                     1               conn_dev_list[0]        conn_dev_list[1]
 *                1                     2               conn_dev_list[0]        conn_dev_list[1..2]
 *                1                     3               conn_dev_list[0]        conn_dev_list[1..3]
 *                1                     4               conn_dev_list[0]        conn_dev_list[1..4]
 *
 *                2                     0               conn_dev_list[0..1]            none
 *                2                     1               conn_dev_list[0..1]     conn_dev_list[2]
 *                2                     2               conn_dev_list[0..1]     conn_dev_list[2..3]
 *                2                     3               conn_dev_list[0..1]     conn_dev_list[2..4]
 *                2                     4               conn_dev_list[0..1]     conn_dev_list[2..5]
 *
 *                3                     0               conn_dev_list[0..2]            none
 *                3                     1               conn_dev_list[0..2]     conn_dev_list[3]
 *                3                     2               conn_dev_list[0..2]     conn_dev_list[3..4]
 *                3                     3               conn_dev_list[0..2]     conn_dev_list[3..5]
 *                3                     4               conn_dev_list[0..2]     conn_dev_list[3..6]
 *
 *                4                     0               conn_dev_list[0..3]            none
 *                4                     1               conn_dev_list[0..3]     conn_dev_list[4]
 *                4                     2               conn_dev_list[0..3]     conn_dev_list[4..5]
 *                4                     3               conn_dev_list[0..3]     conn_dev_list[4..6]
 *                4                     4               conn_dev_list[0..3]     conn_dev_list[4..7]
 */
_attribute_ble_data_retention_ dev_char_info_t conn_dev_list[DEVICE_CHAR_INFO_MAX_NUM];


_attribute_ble_data_retention_ u8 acl_conn_central_num = 0; //current ACL Central number in connection state
_attribute_ble_data_retention_ u8 acl_conn_periphr_num = 0; //current ACL Peripheral number in connection state

/**
 * @brief       Used for add device information to conn_dev_list.
 * @param[in]   dev_char_info       - Pointer point to data buffer.
 * @return      0 ~ DEVICE_CHAR_INFO_MAX_NUM - 1: new connection index, insert success
 *              0xFF: insert failed
 */
int dev_char_info_insert(dev_char_info_t *dev_char_info)
{
    int index = INVALID_CONN_IDX;
    if (dev_char_info->conn_role == ACL_ROLE_CENTRAL) {
        for (int i = 0; i < ACL_CENTRAL_MAX_NUM; i++) {
            if (conn_dev_list[i].conn_state == 0) {
                index = i;
                acl_conn_central_num++;
                break;
            }
        }
    } else if (dev_char_info->conn_role == ACL_ROLE_PERIPHERAL) {
        for (int i = ACL_CENTRAL_MAX_NUM; i < ACL_CENTRAL_MAX_NUM + ACL_PERIPHR_MAX_NUM; i++) {
            if (conn_dev_list[i].conn_state == 0) {
                index = i;
                acl_conn_periphr_num++;
                break;
            }
        }
    }


    if (index != INVALID_CONN_IDX) {
        memcpy(&conn_dev_list[index], dev_char_info, sizeof(dev_char_info_t));
        conn_dev_list[index].conn_state = 1;
    }

    return index;
}

/**
 * @brief       Used for add device information to conn_dev_list.
 * @param[in]   pConnEvt - LE connection complete event data buffer address.
 * @return      0 ~ DEVICE_CHAR_INFO_MAX_NUM - 1: new connection index, insert success
 *              0xFF: insert failed
 */
int dev_char_info_insert_by_conn_event(hci_le_connectionCompleteEvt_t *pConnEvt)
{
    int index = INVALID_CONN_IDX;
    if (pConnEvt->role == ACL_ROLE_CENTRAL) {
        for (int i = 0; i < ACL_CENTRAL_MAX_NUM; i++) {
            if (conn_dev_list[i].conn_state == 0) {
                index = i;
                acl_conn_central_num++;
                break;
            }
        }
    } else if (pConnEvt->role == ACL_ROLE_PERIPHERAL) {
        for (int i = ACL_CENTRAL_MAX_NUM; i < ACL_CENTRAL_MAX_NUM + ACL_PERIPHR_MAX_NUM; i++) {
            if (conn_dev_list[i].conn_state == 0) {
                index = i;
                acl_conn_periphr_num++;
                break;
            }
        }
    }

    if (index != INVALID_CONN_IDX) {
        memset(&conn_dev_list[index], 0, sizeof(dev_char_info_t));

        conn_dev_list[index].conn_handle  = pConnEvt->connHandle;
        conn_dev_list[index].conn_role    = pConnEvt->role;
        conn_dev_list[index].conn_state   = 1;
        conn_dev_list[index].peer_adrType = pConnEvt->peerAddrType;
        memcpy(conn_dev_list[index].peer_addr, pConnEvt->peerAddr, 6);
    }

    return index;
}

/**
 * @brief       Used for add device information to conn_dev_list.
 * @param[in]   pConnEvt - LE enhanced connection complete event data buffer address.
 * @return      0 ~ DEVICE_CHAR_INFO_MAX_NUM - 1: new connection index, insert success
 *              0xFF: insert failed
 */
int dev_char_info_insert_by_enhanced_conn_event(hci_le_enhancedConnCompleteEvt_t *pConnEvt)
{
    int index = INVALID_CONN_IDX;
    if (pConnEvt->role == ACL_ROLE_CENTRAL) {
        for (int i = 0; i < ACL_CENTRAL_MAX_NUM; i++) {
            if (conn_dev_list[i].conn_state == 0) {
                index = i;
                acl_conn_central_num++;
                break;
            }
        }
    } else if (pConnEvt->role == ACL_ROLE_PERIPHERAL) {
        for (int i = ACL_CENTRAL_MAX_NUM; i < ACL_CENTRAL_MAX_NUM + ACL_PERIPHR_MAX_NUM; i++) {
            if (conn_dev_list[i].conn_state == 0) {
                index = i;
                acl_conn_periphr_num++;
                break;
            }
        }
    }

    if (index != INVALID_CONN_IDX) {
        memset(&conn_dev_list[index], 0, sizeof(dev_char_info_t));

        conn_dev_list[index].conn_handle  = pConnEvt->connHandle;
        conn_dev_list[index].conn_role    = pConnEvt->role;
        conn_dev_list[index].conn_state   = 1;
        conn_dev_list[index].peer_adrType = pConnEvt->PeerAddrType;
        memcpy(conn_dev_list[index].peer_addr, pConnEvt->PeerAddr, 6);
    }

    return index;
}

/**
 * @brief       Used for delete device information from conn_dev_list by connHandle
 * @param[in]   connhandle       - connection handle.
 * @return      0: success
 *              1: no find
 */
int dev_char_info_delete_by_connhandle(u16 connhandle)
{
    foreach (i, DEVICE_CHAR_INFO_MAX_NUM) {
        if (conn_dev_list[i].conn_handle == connhandle && conn_dev_list[i].conn_state) //match
        {
            if (conn_dev_list[i].conn_role == ACL_ROLE_CENTRAL) {
                acl_conn_central_num--;
            } else {
                acl_conn_periphr_num--;
            }

            memset(&conn_dev_list[i], 0, sizeof(dev_char_info_t));

            return 0;
        }
    }

    return 1; //not find.
}

/**
 * @brief       Used for delete device information from conn_dev_list by peer mac_address
 * @param[in]   adr_type       - peer address type.
 * @param[in]   addr           - Pointer point to peer address.
 * @return      0: success
 *              1: no find
 */
int dev_char_info_delete_by_peer_mac_address(u8 adr_type, u8 *addr)
{
    foreach (i, DEVICE_CHAR_INFO_MAX_NUM) {
        if (conn_dev_list[i].conn_state) {
            int mac_match = 0;
            if (IS_RESOLVABLE_PRIVATE_ADDR(adr_type, addr)) {
                //TODO
            } else {
                if (adr_type == conn_dev_list[i].peer_adrType && (!memcmp(addr, conn_dev_list[i].peer_addr, 6))) {
                    mac_match = 1;
                }
            }

            //u16 connhandle = conn_dev_list[i].conn_handle;

            if (mac_match) {
                if (conn_dev_list[i].conn_role == ACL_ROLE_CENTRAL) {
                    acl_conn_central_num--;
                } else {
                    acl_conn_periphr_num--;
                }

                memset(&conn_dev_list[i], 0, sizeof(dev_char_info_t));

                return 0;
            }
        }
    }

    return 1; //no find the peer device address.
}

/**
 * @brief       Get device information by connection handle.
 * @param[in]   connhandle       - connection handle.
 * @return      0: no find
 *             !0: found
 */
dev_char_info_t *dev_char_info_search_by_connhandle(u16 connhandle)
{
    foreach (i, DEVICE_CHAR_INFO_MAX_NUM) {
        if (conn_dev_list[i].conn_handle == connhandle && conn_dev_list[i].conn_state) {
            return &conn_dev_list[i]; // find the peer device
        }
    }

    return 0; // no find the peer device
}

/**
 * @brief       Get device information by  peer device address.
 * @param[in]   adr_type       - peer address type.
 * @param[in]   addr           - Pointer point to peer address.
 * @return      0: no find
 *             !0: found
 */
dev_char_info_t *dev_char_info_search_by_peer_mac_address(u8 adr_type, u8 *addr)
{
    foreach (i, DEVICE_CHAR_INFO_MAX_NUM) {
        int mac_match = 0;
        if (IS_RESOLVABLE_PRIVATE_ADDR(adr_type, addr)) {
            //TODO
        } else {
            if (adr_type == conn_dev_list[i].peer_adrType && (!memcmp(addr, conn_dev_list[i].peer_addr, 6))) {
                mac_match = 1;
            }
        }

        if (mac_match) {
            return &conn_dev_list[i]; // find the peer device
        }
    }

    return 0; // no find the peer device
}

/**
 * @brief       Get device information by connection handle.
 * @param[in]   connhandle       - connection handle.
 * @return      0: no find
 *             !0: found
 */
bool dev_char_info_is_connection_state_by_conn_handle(u16 connhandle)
{
    foreach (i, DEVICE_CHAR_INFO_MAX_NUM) {
        if (conn_dev_list[i].conn_handle == connhandle && conn_dev_list[i].conn_state) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @brief       Get ACL connection role by connection handle.
 * @param[in]   connhandle       - connection handle.
 * @return      0: ACL_ROLE_CENTRAL
 *              1: ACL_ROLE_PERIPHERAL
 *              2: connection handle invalid
 */
int dev_char_get_conn_role_by_connhandle(u16 connhandle)
{
    foreach (i, DEVICE_CHAR_INFO_MAX_NUM) {
        if (conn_dev_list[i].conn_handle == connhandle && conn_dev_list[i].conn_state) {
            return conn_dev_list[i].conn_role;
        }
    }

    return 2; //no connection match
}

/**
 * @brief       Get ACL connection index by connection handle.
 * @param[in]   connhandle       - connection handle.
 * @return      0xFF:     no connection index match
 *              others:   connection index
 */
int dev_char_get_conn_index_by_connhandle(u16 connhandle)
{
    foreach (i, DEVICE_CHAR_INFO_MAX_NUM) {
        if (conn_dev_list[i].conn_handle == connhandle && conn_dev_list[i].conn_state) {
            return i;
        }
    }

    return INVALID_CONN_IDX; //no connection index match
}

#endif
