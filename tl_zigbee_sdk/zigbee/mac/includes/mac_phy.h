/********************************************************************************************************
 * @file     mac_phy.h
 *
 * @brief    Zigbee API for phy layer
 *
 * @author
 * @date     Dec. 1, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef  __MAC_PHY_H__
#define  __MAC_PHY_H__

#include "tl_common.h"


/*********************************************************************
 * CONSTANTS
 */

/**
 * Default RF Channel when power up
 */
#define RF_PKT_BUFF_LEN		144//(128+32) //(128+16)


/*
 * Definition for RF TX power
 */
    /**
     *  @brief  Definition for TX power level
     */
#define PHY_TX_POWER_MAX           0
#define PHY_TX_POWER_5DB           1
#define PHY_TX_POWER_0DB           2
#define PHY_TX_POWER_N_5DB         3
#define PHY_TX_POWER_N_10DB        4
#define PHY_TX_POWER_N_13DB        5
#define PHY_TX_POWER_N_18DB        6
#define PHY_TX_POWER_N_23DB        7
#define PHY_TX_POWER_N_27DB        8
#define PHY_TX_POWER_N_30DB        9
#define PHY_TX_POWER_N_37DB        10
#define PHY_TX_POWER_MIN           100

#define CCA_THRESHOLD       		-50
#define RSSI_PASS_THRESHOLD			-30

/*********************************************************************
 * ENUMS
 */
enum {
    RF_SUCC,
    RF_CSMA_FAILURE,
};

enum {
    RF_RX_OFF,
    RF_RX_ON,
};


enum {
    CCA_ED_ONLY,
    CCA_CS_ONLY,
    CCA_ED_AND_CS,
    CCA_ED_OR_CS,
};


enum {
    RF_STATE_TX = 0,
    RF_STATE_RX,
    RF_STATE_ED,
    RF_STATE_OFF,
};


/*
 * Definition for RF Setting identifier.
 */
enum {
	SUCC,
    RF_BLACK_LIST_FULL,
    RF_BLACK_LIST_NO_MEMBER,
};

/*
 * Definition for RF Setting identifier.
 */
enum {
    /* General ID */
    RF_ID_CHANNEL               = 0x00,
    RF_ID_TX_POWER,
    RF_ID_RX_ONOFF,
    RF_ID_GENERAL_END           = 0x9F,
    

    /* Protocol Specific ID*/
    RF_ID_ZIGBEE                = 0xA0,
    RF_ID_MAC_SHORT_ADDR,
    RF_ID_MAC_EXT_ADDR,
    RF_ID_MAC_PANID,
    RF_ID_MAC_FRAME_PENDING,
    RF_ID_MAC_PAN_COORD,
    RF_ID_MAC_RECV_START,
    RF_ID_BECON_RECV,

    RF_ID_BLE                   = 0xB0,
};

typedef enum{
	RF_GAIN_MODE_AUTO,
	RF_GAIN_MODE_MANU_MAX,
}rf_rxGainMode_t;

/*********************************************************************
 * TYPES
 */

/*
 * Definition for RF callback function type
 */
typedef void ( *rf_txCB_t )( void* arg );
typedef void ( *rf_rxCB_t )( void* arg ); 
typedef void ( *rf_ackCB_t )( int fPendingFrame, u8 seq );

typedef void ( *rf_protocolSpecificInit_t) (void);
typedef void ( *rf_protocolSpecificReset_t) (void);
typedef void ( *rf_protocolSpecificSet_t) (u8 id, u8 *pValue, u8 len);


/**
 *  @brief  Definition for Telink RX packet format
 */
typedef struct {
    u32 dmaLen;
    u8  rssi;
    u8  rseverd[7];
    u8  payloadLen;
    u8  payload[1];
} rf_recvPkt_t;


/**
 *  @brief  Definition for Telink TX packet format
 */
typedef struct {
    u32 dmaLen;
    u8  rfLen;
    u8  payload[1];
} rf_sendPkt_t;


/*
 * Definition for RF Rx buffer format
 */
typedef struct {
    u8	*psdu;
    u32 timeStamp;
    s8  rssi;
    u8  psduLen;
} rx_buf_t;

/*
 * Definition for Protocol Specific RF functions
 */
typedef struct {
    rf_protocolSpecificInit_t initFunc;
    rf_protocolSpecificReset_t resetFunc;
} rf_specificFunc_t;


/*********************************************************************
 * Public Variables
 */

extern u8 rf_tx_buf[];




/*********************************************************************
 * Public Functions
 */


/*********************************************************************
 * @fn      rf_init
 *
 * @brief   Initialize RF module and do the RF power up.
 *
 * @param   none
 *
 * @return  none
 */
void rf_init(void);

/*********************************************************************
 * @fn      rf_setCBFunc
 *
 * @brief   Set TX finish callback function and RX callback function
 *
 * @param   rxCbFunc - The specified RX callback function
 *          txCbFunc - The specified TX callback function
 *
 * @return  none
 */
void rf_setCBFunc(rf_txCB_t txCbFunc, rf_rxCB_t rxCbFunc);

/*********************************************************************
 * @fn      rf_setAckCB
 *
 * @brief   Initialize ZigBee ACK packet Callback function.
 *
 * @param   ackCbFunc - Pointer to the ack callback function
 *
 * @return  none
 */
void rf_setAckCB(rf_ackCB_t ackCbFunc);

/*********************************************************************
 * @fn      rf_regProtocolSpecific
 *
 * @brief   Register protocol specific RF functions.
 *            Note: This function must be called before rf_init() and rf_reset()
 *
 * @param   txCbFunc - tx done callback function
 * @param   rxCbFunc - rx done callback function
 *
 * @return  none
 */
void rf_regProtocolSpecific(rf_specificFunc_t* funcs);

/*********************************************************************
 * @fn      rf_reset
 *
 * @brief   Reset RF module.
 *
 * @param   none
 *
 * @return  none
 */
void rf_reset(void);


/*********************************************************************
 * @fn      rf_setTrxState
 *
 * @brief   Go to specified state and set related register.
 *
 * @param   state - Specified state
 *
 * @return  none
 */
void rf_setTrxState(u8 state);


/*********************************************************************
 * @fn      rf_setChannel
 *
 * @brief   Set specified channel to RF module.
 *
 * @param   ch - 11~26
 *
 * @return  none
 */
void rf_setChannel(u8 ch);

/*********************************************************************
 * @fn      rf_setTxPower
 *
 * @brief   Set specified transmitted power.
 *
 * @param   txPower - Specified power
 *
 * @return  none
 */
void rf_setTxPower(u8 txPower);


/*********************************************************************
 * @fn      rf_setCCAMode
 *
 * @brief   Set specified mode to CCA.
 *
 * @param   mode - CCA mode
 *
 * @return  none
 */
void rf_setCCAMode(u8 mode);


/*********************************************************************
 * @fn      rf_setCCALevel
 *
 * @brief   Set CCA detect threshold.
 *
 * @param   threshold - the threshold value
 *
 * @return  none
 */
void rf_setCCALevel(u8 threshold);

/*********************************************************************
 * @fn      rf_getRssi
 *
 * @brief   Get RSSI value from last received frame.
 *
 * @param   none
 *
 * @return  rssi value
 */
s8 rf_getRssi(void);

/*********************************************************************
 * @fn      rf_getLqi
 *
 * @brief   Get calculated Link Quality value from specified rssi
 *
 * @param   rssi
 *
 * @return  lqi result
 */
u8 rf_getLqi(u8 rssi);

/*********************************************************************
 * @fn      rf_getED
 *
 * @brief   Get calculated ED value
 *
 * @param   none
 *
 * @return  ed value
 */
u8 rf_getED(void);
void rf_startED(void);
u8 rf_stopED(void);




/*********************************************************************
 * @fn      rf_getCCAState
 *
 * @brief   Set CCA detect threshold.
 *
 * @param   threshold - the threshold value
 *
 * @return  none
 */
u8 rf_getCCAState(void);

/*********************************************************************
 * @fn      rf_enableRx
 *
 * @brief   Turn on/off the receiver if it's not already on/off.
 *
 * @param   fEnable - 1 means on and 0 means off
 *
 * @return  none
 */
void rf_enableRx(u8 fEnable);

/*********************************************************************
 * @fn      rf_tx
 *
 * @brief   Transmit the data through CSMA/CA mechnism.
 *
 * @param   buf - The buffer to be transmitted
 * @param   len - The length of byte to be transmitted
 *
 * @return  none
 */
void rf_tx(u8* buf, u8 len);

/*********************************************************************
 * @fn      rf_paInit
 *
 * @brief   Initialize PA.
 *
 * @param   none
 *
 * @param   none
 *
 * @return  none
 */
void rf_paInit(u32 TXEN_pin, u32 RXEN_pin);
void rf_paTX(void);
void rf_paRX(void);
void rf_paShutDown(void);


u8 rf_performCCA(void);
void rf_setRxBuf(u8* pBuf);
void rf_setRxBackupBuf(u8* pBuf);

void rf_setRxState(u8 fEnable);
u8 rf_getChannel(void);
u8 rf_TrxStateGet(void);
/*********************************************************************
 * @fn      rf_set
 *
 * @brief   Set rf parameter according to specified RF ID   
 *
 * @param   id     - The specified RF ID
 *          pValue - The detailed rf parameter
 *          len    - The length will be set to
 *
 * @return  none
 */
void rf_set(u8 id, u8 *pValue, u8 len);

void mac_resetPhy(void);

void rf802154_tx(u8* buf, u8 len);

void rf802154_tx_ready(u8* buf, u8 len);

void mac_phyReconfig(void);


extern u8 g_zb_txPowerSet;
#define RF_TX_POWER_DEFAULT_SET(v)		g_zb_txPowerSet = v

/*
 *
 * make sure the API is correct for different platform
 *
 * */
#if defined (MCU_CORE_826x)
	#include "./phy_radio_826x.h"
#endif

#if defined (MCU_CORE_HAWK)
	#include "./phy_radio_hawk.h"
#endif

#if defined (MCU_CORE_8258)
	#include "./phy_radio_8258.h"
#endif

#endif  /* __RF_H__ */
