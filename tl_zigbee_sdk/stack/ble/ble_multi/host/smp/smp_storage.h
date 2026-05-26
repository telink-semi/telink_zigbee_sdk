/********************************************************************************************************
 * @file    smp_storage.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef SMP_STORAGE_H_
#define SMP_STORAGE_H_

/**
 * @brief   strategy for how to deal with new paring device when bonding device number reach maximum number(set by API "blc_smp_setBondingDeviceMaxNumber")
 */
typedef enum
{
    NEW_DEVICE_OVERWRITE_OLD_WITH_PAIRING_ORDER = 0,

    NEW_DEVICE_REJECT_WHEN_PER_MAX_BONDING_NUM = 4, //When peripheral reach it's max bonding centrals, it reject new pair req.
    //  NEW_DEVICE_REJECT_WHEN_CEN_MAX_BONDING_NUM  = 5,        //Not supported now.
} dev_exceed_max_strategy_t;

/*
 * @brief   smp parameter need save to flash.
 */
typedef struct
{
    //0x00
    u8 flag;
    u8 role_dev_idx; //[7]:1 for ACL Central, 0 for ACL Peripheral;   [2:0] ACL Peripheral device index

    // peer_addr_type & peer_addr must be together(SiHui 20200916), cause using flash read packed "type&address" in code
    u8 peer_addr_type;  //address used in link layer connection
    u8 peer_addr[6];

    u8 peer_id_adrType; //peer identity address information in key distribution, used to identify
    u8 peer_id_addr[6];

    //0x10
    u8 local_peer_ltk[16]; //ACL Peripheral: local_ltk; ACL Central: peer_ltk

    //0x20
    u8 encrypt_key_size;
    u8 local_id_adrType;
    u8 local_id_addr[6];

    u8 random[8]; //8


    //0x30
    u8 peer_irk[16];

    //0x40
    u8 local_irk[16]; // local_csrk can be generated based on this key, to save flash area (delete this note at last, customers can not see it)

    //0x50
    u16 ediv;     //2
    u8  rsvd[14]; //14  peer_csrk info address if needed(delete this note at last, customers can not see it)
} smp_param_save_t;

/**
 * @brief      This function is used to configure the bonding storage address and size.
 * @param[in]  address - SMP bonding storage start address.
 * @param[in]  size_byte - SMP bonding storage size(e.g.: 2*4096).
 * @return     none.
 */
void blc_smp_configPairingSecurityInfoStorageAddressAndSize(int address, int size_byte); //address and size must be 4K aligned


/**
 * @brief      This function is used to configure the number of ACL Central and ACL Peripheral devices that can be bound.
 * @param[in]  cen_max_bonNum - The number of ACL Central devices that can be bound.
 * @param[in]  per_max_bondNum - The number of ACL Peripheral devices that can be bound.
 * @return     ble_sts_t.
 */
ble_sts_t blc_smp_setBondingDeviceMaxNumber(int cen_max_bonNum, int per_max_bondNum);


/**
 * @brief      This function is used to get the starting address of the current binding information storage area.
 * @param[in]  none.
 * @return     The starting address of the current binding information storage area.
 */
u32 blc_smp_getBondingInfoCurStartAddr(void);


/**
 * @brief      This function is used to obtain binding information according to the ACL Peripheral address and address type.
 *             This API is for ACL Central only, to search if current ACL Peripheral device is already paired with ACL Central
 * @param[in]  peer_addr_type - Address type.
 * @param[in]  peer_addr - Address.
 * @return     0: Failed to get binding information;
 *             others: FLASH address of the information area.
 */
u32 blc_smp_searchBondingPeripheralDevice_by_PeerMacAddress(u8 peer_addr_type, u8 *peer_addr);


/**
 * @brief      This function is used to delete binding information according to the peer device address and device address type.
 * @param[in]  peer_addr_type - Address type.
 * @param[in]  peer_addr - Address.
 * @return     0: Failed to delete binding information;
 *             others: FLASH address of the deleted information area.
 */
int blc_smp_deleteBondingPeripheralInfo_by_PeerMacAddress(u8 peer_addr_type, u8 *peer_addr);


/**
 * @brief      This function is used to configure the bonding strategy.
 * @param[in]  strategy - The strategy. Refer to the structure 'dev_exceed_max_strategy_t'.
 * @return     none.
 */
void blc_smp_setDevExceedMaxStrategy(dev_exceed_max_strategy_t strategy);


/**
 * @brief      This function is used to clear all binding information stored in the local FLASH.
 * @param[in]  none.
 * @return     none.
 */
void blc_smp_eraseAllBondingInfo(void);


/**
 * @brief      This function is used to check whether the bound storage area reaches the almost full warning threshold.
 * @param[in]  none.
 * @return     0: The warning threshold is not reached.
 *             1: The warning threshold is reached..
 */
bool blc_smp_isBondingInfoStorageLowAlarmed(void);


/**
 * @brief      This function is used to load bonding information according to the peer device address and device address type.
 * @param[in]  isCentral - ACL Connection role,  0: ACL Peripheral role, others: ACL Central role.
 * @param[in]  perDevIdx - ACL Peripheral device index under multiple local device mode
 *                1. if "isCentral" is none zero:  ACL Central device
 *                    so "perDevIdx" invalid, whatever value will be neglected
 *                2. if "isCentral" is 0: ACL Peripheral device
 *                    this value is only valid when multiple local device function enabled by using API "blc_ll_setMultipleLocalDeviceEnable"
 *                    notice that multiple local device is a very special function, we believe that most users will not involve this function.
 *                    user can only use this function when they fully understand it.
 *                   (A). if user do use multiple local device function, "perDevIdx" is ACL Peripheral device index
 *                   (B). if user do not use multiple local device function
 *                        there is only one local device,"perDevIdx" must be 0.
 *                        if user set a value not equal to 0, SDK code will change it to 0 automatically to avoid error.
 * @param[in]  addr_type - Address type.
 * @param[in]  addr - Address.
 * @param[out] smp_param_load - bonding information.
 * @return     0: Failed to find the binding information; others: FLASH address of the bonding information area.
 */
u32 blc_smp_loadBondingInfoByAddr(u8 isCentral, u8 perDevIdx, u8 addr_type, u8 *addr, smp_param_save_t *smp_param_load);


/**
 * @brief      This function is used to get the bonding information numbers.
 * @param[in]  isCentral - ACL Connection role, 0: ACL Peripheral role, others: ACL Central role.
 * @param[in]  perDevIdx - ACL Peripheral device index under multiple local device mode
 *                1. if "isCentral" is none zero:  ACL Central device
 *                    so "perDevIdx" invalid, whatever value will be neglected
 *                2. if "isCentral" is 0: ACL Peripheral device
 *                    this value is only valid when multiple local device function enabled by using API "blc_ll_setMultipleLocalDeviceEnable"
 *                    notice that multiple local device is a very special function, we believe that most users will not involve this function.
 *                    user can only use this function when they fully understand it.
 *                   (A). if user do use multiple local device function, "perDevIdx" is ACL Peripheral device index
 *                   (B). if user do not use multiple local device function
 *                        there is only one local device,"perDevIdx" must be 0.
 *                        if user set a value not equal to 0, SDK code will change it to 0 automatically to avoid error.
 * @return     0: The number of bound devices is 0; others: Number of bound devices.
 */
u16 blc_smp_param_getCurrentBondingDeviceNumber(u8 isCentral, u8 perDevIdx);


/**
 * @brief      This function is used to load bonding information according to the index.
 * @param[in]  isCentral - ACL Connection role, 0: ACL Peripheral role, others: ACL Central role.
 * @param[in]  perDevIdx - ACL Peripheral device index under multiple local device mode
 *                1. if "isCentral" is none zero:  ACL Central device
 *                    so "perDevIdx" invalid, whatever value will be neglected
 *                2. if "isCentral" is 0: ACL Peripheral device
 *                    this value is only valid when multiple local device function enabled by using API "blc_ll_setMultipleLocalDeviceEnable"
 *                    notice that multiple local device is a very special function, we believe that most users will not involve this function.
 *                    user can only use this function when they fully understand it.
 *                   (A). if user do use multiple local device function, "perDevIdx" is ACL Peripheral device index
 *                   (B). if user do not use multiple local device function
 *                        there is only one local device,"perDevIdx" must be 0.
 *                        if user set a value not equal to 0, SDK code will change it to 0 automatically to avoid error.
 * @param[in]  index - bonding index.
 * @param[out] smp_param_load - bonding information.
 * @return     0: Failed to find the binding information; others: FLASH address of the bonding information area.
 */
u32 blc_smp_loadBondingInfoFromFlashByIndex(u8 isCentral, u8 perDevIdx, u16 index, smp_param_save_t *smp_param_load);


#endif /* SMP_STORAGE_H_ */
