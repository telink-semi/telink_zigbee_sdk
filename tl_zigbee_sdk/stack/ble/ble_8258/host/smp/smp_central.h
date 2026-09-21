/********************************************************************************************************
 * @file    smp_central.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef SMP_CENTRAL_H_
#define SMP_CENTRAL_H_


/*
 * @brief      data structure of ACL master role saving SMP parameter.
 */
typedef struct __attribute__((packed)){
	u8 		flag;

	/* peer address in air packet used for link layer connection */
	u8		peer_addr_type;
	u8		peer_addr[6];

	u8 		stack_data_1[10]; //data for task, user do not need concern

	/* peer identity address information in key distribution, used to identify */
	u8		peer_id_adrType;
	u8		peer_id_addr[6];

	u8 		local_irk[16];

	u8 		stack_data_2[7];  //data for task, user do not need concern

	u8 		peer_irk[16];
}smp_param_master_t;



typedef int  (*smp_finish_callback_t)(void);



#define SLAVE_TRIGGER_SMP_FIRST_PAIRING				0   	//first pair, slave send security_request to trigger master's pairing&encryption
#define MASTER_TRIGGER_SMP_FIRST_PAIRING			BIT(0)

#define SLAVE_TRIGGER_SMP_AUTO_CONNECT				0   	//auto connect, slave send security_request to trigger master's encryption
#define MASTER_TRIGGER_SMP_AUTO_CONNECT				BIT(1)




/**
 * @brief      This function is used to initialize SMP module of central device.
 * @param[in]  none
 * @return     none
 */
void 	blc_smp_central_init (void);


/**
 * @brief      This function is used to set security trigger of central device.
 * @param[in]  trigger - security trigger
 * @return     none
 */
void    blm_host_smp_setSecurityTrigger(u8 trigger);


/**
 * @brief      This function is used to configure pairing security information address.
 * @param[in]  addr - Callback function triggered when SMP is completed.
 * @return     none.
 */
void 	blm_smp_configPairingSecurityInfoStorageAddr (int addr);


/**
 * @brief      This function is used to deal smp function in master role.
 * @param[in]  conn_handle - connection handle
 * @param[in]  p - packet pointer
 * @return     none.
 */
void 	blm_host_smp_handler(u16 conn_handle, u8 *p);


/**
 * @brief      This function is used to search bonding message in slave role.
 * @param[in]  adr_type - address type
 * @param[in]  addr - data address
 * @return     status 0:success  other:fail.
 */
int 	tbl_bond_slave_search(u8 adr_type, u8 * addr);


/**
 * @brief      This function is used to delete bonding message in slave role.
 * @param[in]  adr_type - address type
 * @param[in]  addr - data address
 * @return     status 0:success  other:fail.
 */
int 	tbl_bond_slave_delete_by_adr(u8 adr_type, u8 *addr);


/**
 * @brief      This function is used to delete bonding message .
 * @param[in]  adr_type - address type
 * @param[in]  addr - data address
 * @return     none.
 */
void 	tbl_bond_slave_unpair_proc(u8 adr_type, u8 *addr);


/**
 * @brief      This function is used to delete bonding message in slave role.
 * @param[in]  adr_type - address type
 * @param[in]  addr - data address
 * @return     none.
 */
void	blm_smp_encChangeEvt(u8 status, u16 connhandle, u8 enc_enable);


/**
 * @brief      This function is used to register call back of system finish
 * @param[in]  cb - Callback function triggered when SMP is completed.
 * @return     none.
 */
void 	blm_smp_registerSmpFinishCb (smp_finish_callback_t cb);




/**
 * @brief      This function is used to get the number of paired devices.
 * 			   attention: only used for Central(Master)
 * @param[in]  none.
 * @return     The number of paired devices.
 */
u8		blm_smp_getPairedDeviceNumber(void);


/**
 * @brief      This function is used to obtain device SMP information based on Index.
 * 			   attention: only used for Central(Master)
 * @param[in]  index - Device paired index number.
 * @param[out] smp_param_load - The value can refer to the structure 'smp_param_master_t'.
 * @return     0: Failed to load binding information;
 *             others: FLASH address of the information area.
 */
u32 	blm_smp_loadParametersByIndex(u8 index, smp_param_master_t* smp_param_load);




#endif /* SMP_CENTRAL_H_ */
