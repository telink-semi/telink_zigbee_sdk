/********************************************************************************************************
 * @file    controller.h
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
#ifndef CONTROLLER_H_
#define CONTROLLER_H_




/**
 * @brief	Telink defined Controller Event Callback Declaration
 */
typedef void (*blt_event_callback_t)(u8 e, u8 *p, int n);


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
#define			BLT_EV_FLAG_PHY_UPDATE							7
#define			BLT_EV_FLAG_DATA_LENGTH_EXCHANGE				8
#define			BLT_EV_FLAG_GPIO_EARLY_WAKEUP					9
#define			BLT_EV_FLAG_CHN_MAP_REQ							10
#define			BLT_EV_FLAG_CONN_PARA_REQ						11
#define			BLT_EV_FLAG_CHN_MAP_UPDATE						12
#define			BLT_EV_FLAG_CONN_PARA_UPDATE					13
#define			BLT_EV_FLAG_SUSPEND_ENTER						14
#define			BLT_EV_FLAG_SUSPEND_EXIT						15
#define			BLT_EV_FLAG_VERSION_IND_REV						16
#define			BLT_EV_FLAG_SCAN_REQ							17
#define			BLT_EV_FLAG_ADV_TX_EACH_CHANNEL					18  //event triggers before transmitting ADV packet for all ADV channel (37/38/39)




/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_CONNECT"
 */
typedef struct{
	u8	initA[6];			//initiator address
	u8	advA[6];			//advertising address
	u32	accessCode;			//access code
	u8	crcinit[3];
	u8	winSize;
	u16	winOffset;
	u16 connReq_interval;	//connection interval
	u16 connReq_latency;    //connection latency
	u16 connReq_timeout;    //connection timeout
	u8	chm_map[5];			//channel map
	u8	hop_sca;			//hop & sca
}tlk_contr_evt_connect_t;

/**
 *  @brief  this macro is used to get RX address type from the callback data of controller event "BLT_EV_FLAG_CONNECT"
 *
 * please refer to BLUETOOTH CORE SPECIFICATION Vol 6, Part B, 2.3 ADVERTISING PHYSICAL CHANNEL PDU,
 * "Figure: Advertising physical channel PDU header" for RX address type
 */
#define GET_RXADD_FROM_CONNECT_EVT_DATA(p)         ( ((*(u8*)(p - 2)) & BIT(7)) ? 1 : 0 )  //RxAdd is BIT(7)

/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_TERMINATE"
 */
typedef struct{
	u8	terminate_reason;
}tlk_contr_evt_terminate_t;


/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_DATA_LENGTH_EXCHANGE"
 */
typedef struct {
	u16		connEffectiveMaxRxOctets;	//effective maximum RX Octets
	u16		connEffectiveMaxTxOctets;   //effective maximum TX Octets
	u16 	connMaxRxOctets;		    //local maximum RX Octets
	u16 	connMaxTxOctets;			//local maximum TX Octets
	u16		connRemoteMaxRxOctets;		//remote maximum RX Octets
	u16 	connRemoteMaxTxOctets;		//remote maximum TX Octets
}tlk_contr_evt_dataLenExg_t;


/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_CHN_MAP_REQ"
 */
typedef struct {
	u8		chn_map[5];		//old channel map before update take effect
}tlk_contr_evt_chnMapRequest_t;


/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_CHN_MAP_UPDATE"
 */
typedef struct {
	u8		chn_map[5];		//new channel map after update take effect
}tlk_contr_evt_chnMapUpdate_t;


/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_CONN_PARA_REQ"
 */
typedef struct {
	u8 		winSize;
	u16 	winOffset;
	u16 	interval;
	u16 	latency;
	u16 	timeout;
	u16 	instant;
}tlk_contr_evt_connParaReq_t;


/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_CONN_PARA_UPDATE"
 */
typedef struct {
	u16		conn_interval;	//new connection interval after update take effect, 1.25 mS unit
	u16		conn_latency;	//new connection latency after update take effect
	u16		conn_timeout;	//new connection timeout after update take effect, 10 mS unit
}tlk_contr_evt_connParaUpdate_t;


/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_VERSION_IND_REV"
 */
typedef struct{
	u8  verNum;
	u16 companyId;
	u16 subVerNum;
}tlk_contr_evt_versionIndRev_t;

/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_SCAN_REQ"
 */
typedef struct {
	u8  type   :4;
	u8  rfu1   :1;
	u8  chan_sel:1;
	u8  txAddr :1;
	u8  rxAddr :1;

	u8  rf_len;

	u8	scanA[6];
	u8	advA[6];
}tlk_contr_evt_scanReq_t;

/**
 *  @brief  Event Parameters for "BLT_EV_FLAG_ADV_TX_EACH_CHANNEL"
 */
typedef struct{
	u8 *	pAdvData;			  //point to first ADV data
	u32 	advPacketHeader_tick; //ADV packet header Stimer tick
} tlk_contr_evt_advTxEachChn_t;




/**
 * @brief      this function is used check if any controller initialization incorrect.
 * 			   attention: this function must be called after all controller Initialization finished.
 * @param	   none
 * @return     status - 0x00:  succeed, no error
* 			  		    other: error code
 */
init_err_t	blc_contr_checkControllerInitialization(void);



/**
 * @brief      this function is used to set Bluetooth core specification version in controller
 * @param[in]  version - core specification version
 * @return     none
 */
void 		blc_contr_setBluetoothVersion(core_version_t version);



/**
 * @brief      this function is used to reset BLE controller.
 * @param	   none
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t  	blc_hci_reset(void);


#endif /* CONTROLLER_H_ */
