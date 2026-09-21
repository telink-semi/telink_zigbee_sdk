/********************************************************************************************************
 * @file    ll.h
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
#ifndef LL__H_
#define LL__H_


#include "tl_common.h"
#include "stack/ble/ble_8258/ble_common.h"

/**
 * @brief	BLE link layer state
 */
#define			BLS_LINK_STATE_IDLE								0		//Standby state
#define			BLS_LINK_STATE_ADV								BIT(0)  //Legacy Advertising or Extended Advertising
#define			BLS_LINK_STATE_SCAN								BIT(1)	//Scanning state
#define			BLS_LINK_STATE_INIT								BIT(2)  //Initiating from Scanning state
#define 		BLS_LINK_STATE_CONN								BIT(3)	//ACL connection Slave role or Master role

typedef bool (*ble_lowpowerCond_t)(void);

/**
 * @brief	   irq_handler for BLE stack, process system tick interrupt and RF interrupt
 * @param	   none
 * @return	   none
 */
void		irq_blt_sdk_handler(void);


/**
 * @brief      main_loop for BLE stack, process data and event
 * @param	   none
 * @return	   none
 */
int 		blt_sdk_main_loop(void);


/**
 * @brief      for user to initialize MCU
 * @param	   none
 * @return     none
 */
void 		blc_ll_initBasicMCU (void);


/**
 * @brief      for user to initialize link layer Standby state
 * @param[in]  public_adr -  public address pointer
 * @return     none
 */
void 		blc_ll_initStandby_module(u8 *public_adr);




/**
 * @brief      this function is used to set the LE Random Device Address in the Controller
 *  		   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.4 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  randomAddr -  Random Device Address
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_ll_setRandomAddr(u8 *randomAddr);


/**
 * @brief      this function is used to read MAC address
 * @param[in]  addr -  The address where the read value(MAC address) prepare to write.
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_ll_readBDAddr(u8 *addr);

/**
 * @brief      this function is used to get LE stack current state
 * @param[in]  none.
 * @return     BLE link layer state:
 * 					BLS_LINK_STATE_IDLE
 * 					BLS_LINK_STATE_ADV
 * 					BLS_LINK_STATE_SCAN
 * 					BLS_LINK_STATE_INIT
 * 					BLS_LINK_STATE_CONN
 */
u8 			blc_ll_getCurrentState(void);

/**
 * @brief      this function is used to get the most recent average RSSI
 * @param[in]  none.
 * @return     the most recent average RSSI
 */
u8 			blc_ll_getLatestAvgRSSI(void);


/**
 * @brief      this function is used to check if any Controller event pending to be processed.
 * @param[in]  none.
 * @return     1: Controller event pending
 *             0: no Controller event pending
 */
bool		blc_ll_isControllerEventPending(void);


/**
 * @brief      this function is used to get TX FIFO Number of current state
 * @param[in]  none.
 * @return     total_fifo_num
 */
u8  		blc_ll_getTxFifoNumber (void);



/**
 * @brief		this function is used to register LinkLayer Event Callback function
 * @param[in]	e - event type when this function is triggered by LinkLayer event
 * @param[in]	p - event callback function
 * @return		none
 */
void		bls_app_registerEventCallback (u8 e, blt_event_callback_t p);


/**
 * @brief      this function is used to get current state of BRX
 * @param[in]  none.
 * @return     current state of BRX
 */
bool 		blc_ll_isBrxBusy (void);


/**
 * @brief      this function is used to set customized access code
 * @param[in]  access_code - BLE ADV access code
 * @return     none.
 */
void  		blc_ll_set_CustomizedAdvScanAccessCode(u32 access_code);


/**
 * @brief      this function is used to read buffer size
 *  		   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.2 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  pData - data pointer
 * @return     status - 0x00:  succeed
 */
ble_sts_t	blc_hci_le_readBufferSize_cmd(u8 *pData);


/**
 * @brief      for user to initialize local RPA module
 * 			   attention: must call this API if user need local RPA for privacy
 * @param	   none
 * @return     none
 */
void        blc_ll_initPrivacyLocalRpa(void);


/**
 * @brief 		this function is used to solving issue that BLE connection RF IRQ affected by Flash writing status duration
 * 				by finding idle timing to write safely.
 *              If MCU do not support multiple priority IRQ, Flash write status duration influencing BLE RF IRQ, then lead to BLE data error
 * 			    attention: it's for Flash lock & unlock in BLE connection state
 * @param[in]  	type	- the type of status.8 bit or 16 bit.
 * @param[in]  	data	- the value of status.
 * @return 		none.
 */
void blc_ll_write_flash_status(flash_status_typedef_e type , unsigned short data);

u32 get_ble_next_event_tick(void);

u32 get_ble_event_state(void);

void backup_ble_rf_context(void);

void restore_ble_rf_context(void);

void ble_radio_init(void);

void blc_ll_setScanWin(u32 winSizeUs);

u8 blt_state_get(void);

#endif /* LL__H_ */
