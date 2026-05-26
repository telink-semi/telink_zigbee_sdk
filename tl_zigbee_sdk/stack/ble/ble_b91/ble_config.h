/********************************************************************************************************
 * @file     ble_config.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
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

#pragma once


//#include "platform.h"
#include "tl_common.h"

//#define MCU_CORE_B91    0x01
#define MCU_CORE_TYPE	MCU_CORE_B91

#if(MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	#define  MAX_DEV_NAME_LEN 				18

	#ifndef DEV_NAME
	#define DEV_NAME                        "tModule"
	#endif
#endif


#if (MCU_CORE_TYPE == MCU_CORE_825x)
	#define	FIX_HW_CRC24_EN									1
	#define HW_ECDH_EN                                      0
#elif (MCU_CORE_TYPE == MCU_CORE_827x)
	#define	FIX_HW_CRC24_EN									0
	#define HW_ECDH_EN                                      1
#elif (MCU_CORE_TYPE == MCU_CORE_B91)
	#define	FIX_HW_CRC24_EN									0
	#define HW_ECDH_EN                                      1
	#define BLT_CONN_MASTER_EN								0
	#define BLT_SCAN_EN										1
	#define BLT_ADV_IN_CONN_SLAVE_EN						0
	#define BLT_SCAN_IN_CONN_SLAVE_EN						0
#else
	#error "unsupported mcu type !"
#endif



///////////////////  Feature ////////////////////////////
#ifndef			BLT_CONN_MASTER_EN
#define			BLT_CONN_MASTER_EN								1
#endif

#ifndef			BLT_SCAN_EN
#define			BLT_SCAN_EN										1
#endif

#ifndef			BLT_ADV_IN_CONN_SLAVE_EN
#define			BLT_ADV_IN_CONN_SLAVE_EN						1
#endif

#ifndef			BLT_SCAN_IN_CONN_SLAVE_EN
#define			BLT_SCAN_IN_CONN_SLAVE_EN						1
#endif

#ifndef         DOCKKIT_MODIFY_ATT_RSP_LEN_EN
#define         DOCKKIT_MODIFY_ATT_RSP_LEN_EN                   1
#endif

#ifndef			BIS_IN_BIG_NUM_MAX
#define			BIS_IN_BIG_NUM_MAX								4
#endif


#ifndef			LL_CIS_IN_CIG_NUM_MAX
#define 		LL_CIS_IN_CIG_NUM_MAX              				4
#endif

///////////////////  Feature ////////////////////////////


#ifndef SECURE_CONNECTION_ENABLE
#define SECURE_CONNECTION_ENABLE							1
#endif









#ifndef  LL_MASTER_MULTI_CONNECTION
#define  LL_MASTER_MULTI_CONNECTION							0
#endif




#ifndef		BLS_ADV_INTERVAL_CHECK_ENABLE
#define		BLS_ADV_INTERVAL_CHECK_ENABLE					0
#endif





//conn param update/map update
#ifndef	BLS_PROC_MASTER_UPDATE_REQ_IN_IRQ_ENABLE
#define BLS_PROC_MASTER_UPDATE_REQ_IN_IRQ_ENABLE			1
#endif


#ifndef LE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT_EN
#define LE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT_EN			0
#endif





#ifndef DEEP_RET_ENTRY_CONDITIONS_OPTIMIZATION_EN
#define	DEEP_RET_ENTRY_CONDITIONS_OPTIMIZATION_EN			0
#endif


#ifndef HOST_CONTROLLER_DATA_FLOW_IMPROVE_EN
#define	HOST_CONTROLLER_DATA_FLOW_IMPROVE_EN				1
#endif



//Link layer feature enable flag default setting
#ifndef BLE_CORE42_DATA_LENGTH_EXTENSION_ENABLE
#define BLE_CORE42_DATA_LENGTH_EXTENSION_ENABLE						1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_2M_PHY
#define LL_FEATURE_SUPPORT_LE_2M_PHY								1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_CODED_PHY
#define LL_FEATURE_SUPPORT_LE_CODED_PHY								1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_EXTENDED_ADVERTISING
#define LL_FEATURE_SUPPORT_LE_EXTENDED_ADVERTISING					1
#endif

#ifndef LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING
#define LL_FEATURE_SUPPORT_LE_PERIODIC_ADVERTISING					0
#endif

#ifndef LL_FEATURE_SUPPORT_CHANNEL_SELECTION_ALGORITHM2
#define LL_FEATURE_SUPPORT_CHANNEL_SELECTION_ALGORITHM2				1
#endif




//core_5.2 feature begin
#ifndef LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_MASTER
#define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_MASTER		1
#endif

#ifndef LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_SLAVE
#define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_SLAVE		1
#endif

#ifndef LL_FEATURE_SUPPORT_ISOCHRONOUS_BROADCASTER
#define LL_FEATURE_SUPPORT_ISOCHRONOUS_BROADCASTER					1
#endif

#ifndef LL_FEATURE_SUPPORT_SYNCHRONIZED_RECEIVER
#define LL_FEATURE_SUPPORT_SYNCHRONIZED_RECEIVER					1
#endif

#ifndef LL_FEATURE_SUPPORT_ISOCHRONOUS_CHANNELS
#define LL_FEATURE_SUPPORT_ISOCHRONOUS_CHANNELS						1
#endif
//core_5.2 feature end




///////////////////////////////////////dbg channels///////////////////////////////////////////
#ifndef	DBG_CHN0_TOGGLE
#define DBG_CHN0_TOGGLE
#endif

#ifndef	DBG_CHN0_HIGH
#define DBG_CHN0_HIGH
#endif

#ifndef	DBG_CHN0_LOW
#define DBG_CHN0_LOW
#endif

#ifndef	DBG_CHN1_TOGGLE
#define DBG_CHN1_TOGGLE
#endif

#ifndef	DBG_CHN1_HIGH
#define DBG_CHN1_HIGH
#endif

#ifndef	DBG_CHN1_LOW
#define DBG_CHN1_LOW
#endif

#ifndef	DBG_CHN2_TOGGLE
#define DBG_CHN2_TOGGLE
#endif

#ifndef	DBG_CHN2_HIGH
#define DBG_CHN2_HIGH
#endif

#ifndef	DBG_CHN2_LOW
#define DBG_CHN2_LOW
#endif

#ifndef	DBG_CHN3_TOGGLE
#define DBG_CHN3_TOGGLE
#endif

#ifndef	DBG_CHN3_HIGH
#define DBG_CHN3_HIGH
#endif

#ifndef	DBG_CHN3_LOW
#define DBG_CHN3_LOW
#endif

#ifndef	DBG_CHN4_TOGGLE
#define DBG_CHN4_TOGGLE
#endif

#ifndef	DBG_CHN4_HIGH
#define DBG_CHN4_HIGH
#endif

#ifndef	DBG_CHN4_LOW
#define DBG_CHN4_LOW
#endif

#ifndef	DBG_CHN5_TOGGLE
#define DBG_CHN5_TOGGLE
#endif

#ifndef	DBG_CHN5_HIGH
#define DBG_CHN5_HIGH
#endif

#ifndef	DBG_CHN5_LOW
#define DBG_CHN5_LOW
#endif

#ifndef	DBG_CHN6_TOGGLE
#define DBG_CHN6_TOGGLE
#endif

#ifndef	DBG_CHN6_HIGH
#define DBG_CHN6_HIGH
#endif

#ifndef	DBG_CHN6_LOW
#define DBG_CHN6_LOW
#endif

#ifndef	DBG_CHN7_TOGGLE
#define DBG_CHN7_TOGGLE
#endif

#ifndef	DBG_CHN7_HIGH
#define DBG_CHN7_HIGH
#endif

#ifndef	DBG_CHN7_LOW
#define DBG_CHN7_LOW
#endif



#ifndef	BLE_IRQ_DBG_EN
#define BLE_IRQ_DBG_EN					0
#endif

#ifndef	BLE_ADV_DBG_EN
#define BLE_ADV_DBG_EN					0
#endif

#ifndef	TX_FIFO_DBG_EN
#define TX_FIFO_DBG_EN					0
#endif


#ifndef		DEBUG_PAIRING_ENCRYPTION
#define 	DEBUG_PAIRING_ENCRYPTION		0
#endif



#define	log_hw_ref()

// 4-byte sync word: 00 00 00 00
#define	log_sync(en)
//4-byte (001_id-5bits) id0: timestamp align with hardware gpio output; id1-31: user define
#define	log_tick(en,id)

//1-byte (000_id-5bits)
//#define	log_event(en,id)

//1-byte (01x_id-5bits) 1-bit data: id0 & id1 reserved for hardware
#define	log_task(en,id,b)

//2-byte (10-id-6bits) 8-bit data
#define	log_b8(en,id,d)
//3-byte (11-id-6bits) 16-bit data
#define	log_b16(en,id,d)




#define	log_tick_irq(en,id)
#define	log_tick_irq_2(en,id,t)


#define	log_event_irq(en,id)


#define	log_task_irq(en,id,b)

#define	log_task_begin_irq(en,id)
#define	log_task_end_irq(en,id)

#define	log_task_begin_irq_2(en,id,t)
#define	log_task_end_irq_2(en,id,t)



#define	log_b8_irq(en,id,d)

#define	log_b16_irq(en,id,d)
