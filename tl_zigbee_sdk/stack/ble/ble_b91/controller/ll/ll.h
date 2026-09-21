/********************************************************************************************************
 * @file     ll.h
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

#ifndef LL__H_
#define LL__H_


#include "tl_common.h"
#include "stack/ble/ble_b91/ble_common.h"
#include "stack/ble/ble_b91/controller/ll/ll_stack.h"

/**
 * @brief	BLE link layer state
 */
#define			BLS_LINK_STATE_IDLE								0
#define			BLS_LINK_STATE_ADV								BIT(0)
#define			BLS_LINK_STATE_SCAN								BIT(1)
#define			BLS_LINK_STATE_INIT								BIT(2)
#define 		BLS_LINK_STATE_CONN								BIT(3)


/**
 * @brief	Telink defined LinkLayer Event Callback Declaration
 */
typedef void (*blt_event_callback_t)(u8 e, u8 *p, int n);


/**
 * @brief	Telink defined LinkLayer Callback Declaration for phyTest
 */
typedef int (*blc_main_loop_phyTest_callback_t)(void);

typedef bool (*ble_lowpowerCond_t)(void);


/**
 * @brief	Telink defined LinkLayer Event Type
 */
#define 		BLT_EV_MAX_NUM									20

#define			BLT_EV_FLAG_ADV									0
#define			BLT_EV_FLAG_ADV_DURATION_TIMEOUT				1
#define			BLT_EV_FLAG_SCAN_RSP							2
#define			BLT_EV_FLAG_CONNECT								3
#define			BLT_EV_FLAG_TERMINATE							4
#define			BLT_EV_FLAG_LL_REJECT_IND		    			5
#define			BLT_EV_FLAG_RX_DATA_ABANDON						6
#define 		BLT_EV_FLAG_PHY_UPDATE							7
#define			BLT_EV_FLAG_DATA_LENGTH_EXCHANGE				8
#define			BLT_EV_FLAG_GPIO_EARLY_WAKEUP					9
#define			BLT_EV_FLAG_CHN_MAP_REQ							10
#define			BLT_EV_FLAG_CONN_PARA_REQ						11
#define			BLT_EV_FLAG_CHN_MAP_UPDATE						12
#define			BLT_EV_FLAG_CONN_PARA_UPDATE					13
#define			BLT_EV_FLAG_SUSPEND_ENTER						14
#define			BLT_EV_FLAG_SUSPEND_EXIT						15
#define			BLT_EV_FLAG_VERSION_IND_REV						16



#if (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	my_fifo_t			blt_rxfifo;
	u8					blt_rxfifo_b[];

	my_fifo_t			blt_txfifo;
	u8					blt_txfifo_b[];
#endif
typedef struct {
	u16		connEffectiveMaxRxOctets;
	u16		connEffectiveMaxTxOctets;
	u16 	connMaxRxOctets;
	u16 	connMaxTxOctets;
	u16		connRemoteMaxRxOctets;
	u16 	connRemoteMaxTxOctets;
	u16		supportedMaxRxOctets;
	u16		supportedMaxTxOctets;

	u8	 	connInitialMaxTxOctets;  //u8 is enough
	u8		connMaxTxRxOctets_req;
	u8		connRxDiff100;
	u8		connTxDiff100;
}ll_data_extension_t;

extern _attribute_aligned_(4) ll_data_extension_t  bltData;

#if (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
extern my_fifo_t		hci_tx_fifo;
#endif
/**
 * @brief	This function is used to obtain the effective maximum TX data length
 * @param	none
 * @return	bltData.connEffectiveMaxTxOctets
 */
static inline u8 blc_ll_get_connEffectiveMaxTxOctets(void)
{
	#if (LL_FEATURE_ENABLE_LE_DATA_LENGTH_EXTENSION)
		return bltData.connEffectiveMaxTxOctets;
	#else
		return 27;
	#endif
}


#if (MCU_CORE_TYPE == MCU_CORE_B91)
/**
 * @brief	This function is used to obtain the effective maximum RX data length
 * @param	none
 * @return	bltData.connEffectiveMaxRxOctets
 */
static inline u8 blc_ll_get_connEffectiveMaxRxOctets(void)
{
	#if (LL_FEATURE_ENABLE_LE_DATA_LENGTH_EXTENSION)
		return bltData.connEffectiveMaxRxOctets;
	#else
		return 27;
	#endif
}
#endif

/**
 * @brief	irq_handler for BLE stack, process system tick interrupt and RF interrupt
 * @param	none
 * @return	none
 */
void		irq_blt_sdk_handler (void);


/**
 * @brief   main_loop for BLE stack, process data and event
 * @param	none
 * @return	none
 */
int 		blt_sdk_main_loop(void);




/**
 * @brief   LinkLayer initialization after deepSleep retention wake_up
 * @param	none
 * @return	none
 */
void 		blc_ll_recoverDeepRetention(void);



/**
 * @brief      for user to initialize MCU
 * @param	   none
 * @return     none
 */
void 		blc_ll_initBasicMCU (void);


/**
 * @brief      for user to initialize link layer Standby state
 * @param[in]  *public_adr -  public address pointer
 * @return     none
 */
void 		blc_ll_initStandby_module (u8 *public_adr);




/**
 * @brief      this function is used to set the LE Random Device Address in the Controller
 * @param[in]  *randomAddr -  Random Device Address
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_ll_setRandomAddr(u8 *randomAddr);


/**
 * @brief      this function is used to read MAC address
 * @param[in]  *addr -  The address where the read value(MAC address) prepare to write.
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
 * @return     bltParam.ll_recentAvgRSSI
 */
u8 			blc_ll_getLatestAvgRSSI(void);

#if (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
u16   		blc_ll_setInitTxDataLength (u16 maxTxOct);   //core4.2 long data packet
#endif

/**
 * @brief      this function is used to pend Controller event
 * @param[in]  none.
 * @return     blc_tlkEvent_pending
 */
bool		blc_ll_isControllerEventPending(void);


/**
 * @brief      this function is used to get TX FIFO Number of current state
 * @param[in]  none.
 * @return     total_fifo_num
 */
u8  		blc_ll_getTxFifoNumber (void);

ble_sts_t 	blt_ll_exchangeDataLength (u8 opcode, u16 maxTxOct);   ///core4.2 data extension

#if (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
#define 	blc_ll_exchangeDataLength	blt_ll_exchangeDataLength  ///Compatible with previous versions
#endif


/**
 * @brief		this function is used to register LinkLayer Event Callback function
 * @param[in]	e -
 * @param[in]	p -
 * @return		none
 */
void		bls_app_registerEventCallback (u8 e, blt_event_callback_t p);







bool 		blc_ll_isBrxBusy (void);



void  blc_ll_set_CustomedAdvScanAccessCode(u32 access_code);


#if (MCU_CORE_TYPE == MCU_CORE_B91)
/**
 * @brief      this function is used check if any controller buffer initialized by application incorrect.
 * 			   attention: this function must be called at the end of BLE LinkLayer Initialization.
 * @param	   none
 * @return     status, 0x00:  succeed, no buffer error
 * 					   other: buffer error code
 */
ble_sts_t	blc_controller_check_appBufferInitialization(void);
#endif


u32 get_ble_end_event_tick(void);
u32 get_ble_next_event_tick(void);
u32 get_ble_event_state(void);

void restore_ble_rf_context(void);
void backup_ble_rf_context(void);
void ble_radio_init(void);
u8 blt_state_get(void);

/**
 * @brief	    condition function if the ble is allowed to enter low power mode
 * @param[in]	cb the condition function
 * @return		1: allowed to enter low power mode
 */
void bls_pm_conditionCbRegister(ble_lowpowerCond_t cb);

/**
 * @brief		release the condition function, so that  the ble won't be allowed to enter low power mode
 * @param[in]	none
 * @return		none
 */
void bls_pm_conditionCbUnregister(void);


/**
 * @brief		if the callback for allowing to enter low power mode is valid
 * @param[in]	none
 * @return
 */
bool bls_pm_conditionCbIsValid(void);

#endif /* LL__H_ */
