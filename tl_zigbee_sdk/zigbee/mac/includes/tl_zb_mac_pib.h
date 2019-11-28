/********************************************************************************************************
 * @file     tl_zb_mac_pib.h
 *
 * @brief    mac layer information base
 *
 * @author
 * @date     Oct. 12, 2016
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
#ifndef TL_ZB_MAC_PIB_H
#define TL_ZB_MAC_PIB_H 1

#include "tl_common.h"
#include "tl_zb_mac.h"


/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  TELINK_ZIGBEE_MAC telink mac
 *  @{
 */


#define 	MAC_DEVICE_TABLE_SIZE 		4
#define		MAC_INVALID_PANID			0xffff

/*
 * PHY constants
 */
#define ZB_PHY_MAX_PACKET_SIZE			127
#define ZB_PHY_TURNROUNDTIME			12
#define	ZB_US_PER_SYMBOL				16

/*
 * PHY PIB
 */
#define ZB_PHY_SYMBOLS_PER_OCTET		2
#define ZB_PHY_SHR_DURATION				40
#define ZB_PHY_MAX_FRAME_DURATION		(ZB_PHY_SHR_DURATION + (ZB_PHY_MAX_PACKET_SIZE + 1) * ZB_PHY_SYMBOLS_PER_OCTET)


/** @addtogroup zb_mac_constant Zigbee Mac Constants
 *  Definition for zigbee Mac constants
 *  @{
 */
#define ZB_MAC_BASE_SLOT_DURATION				60	 //!< The number of symbols forming a superframe slot when the superframe order is equal to 0
#define ZB_MAC_NUM_SUPERFRAME_SLOTS				16	 //!< The number of slots contained in any superframe
#define ZB_MAC_BASE_SUPERFRAME_DURATION			(ZB_MAC_BASE_SLOT_DURATION * ZB_MAC_NUM_SUPERFRAME_SLOTS)
													 //!< The number of symbols forming a superframe when the superframe order is equal to 0
#define ZB_MAC_BASE_SUPERFRAME_DURATION_US		(ZB_MAC_BASE_SLOT_DURATION * ZB_MAC_NUM_SUPERFRAME_SLOTS * ZB_US_PER_SYMBOL)
													 //!< a superframe(us) when the superframe order is equal to 0
#define ZB_MAC_UNIT_BACKOFF_PERIOD				(20) //!< The number of symbols forming the basic time period used by the CSMA-CA algorithm.
#define ZB_MAC_UNIT_BACKOFF_PERIOD_US			(ZB_MAC_UNIT_BACKOFF_PERIOD * ZB_US_PER_SYMBOL) //!< the basic time period(us) used by the CSMA-CA algorithm.
#define ZB_MAC_MIN_MPDU_OVERHEAD				(9)	  	//!< The minimum number of octets added by the MAC sublayer to the PSDU.
#define ZB_MAC_MIN_CAP_LEHGTH					(440) 	//!< The minimum number of symbols forming the CAP.
#define ZB_MAC_MAX_SIF_FRAME_SIZE				(18)	//!< The maximum size of an MPDU
#define ZB_MAC_MAX_MPDU_UNSECURITY_OVERHEAD		(25)	//!< The maximum number of octets added by the MAC sublayer to the PSDU without security
#define ZB_MAC_MAX_MAC_PAYLOAD_SIZE				(ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MIN_MPDU_OVERHEAD)
														//!< The maximum number of octets that can be transmitted in the MAC Payload field
#define ZB_MAC_MAX_MAC_SAFE_PAYLOAD_SIZE		(ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MAX_MPDU_UNSECURITY_OVERHEAD)
														//!< The maximum number of octets that can be transmitted in the MAC Payload field of an unsecured MAC frame
#define ZB_MAC_MAX_LOST_BEACONS					(4)		//!< The number of consecutive lost beacons that will cause the MAC sublayer of a receiving device to declare a loss of synchronization
#define ZB_MAC_MAX_BEACON_OVERHEAD				(75)    //!< The maximum number of octets added by the MAC sublayer to the MAC payload of a beacon frame.
#define ZB_MAC_MAX_BEACON_PAYLOAD_LENGTH		(ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MAX_BEACON_OVERHEAD)
														//!< The maximum size, in octets, of a beacon payload
#define ZB_MAC_GTS_DESC_PERSISTENCE_TIME		(4)		//!< The number of consecutive lost beacons that will cause the MAC sublayer of a receiving device to declare a loss of synchronization.

/** @} end of group zb_mac_constant */


/* re-transmit timers of the MAC layer */
#define ZB_MAC_WAIT_RESP_TIME_DEFAULT			(32)
#define	ZB_MAC_FRAME_RETRIES_MAX				(3)


#ifdef ZB_MAC_SECURITY
typedef struct zb_mac_device_table_s{
	u32    frameCounter;
	u16    shortAddr;
	addrExt_t extAddr;
	u16    panId;
}tl_zb_mac_device_table_t;
#endif



/** @addtogroup zigbee_mac_pib_type Zigbee Mac PIB Types
 *  @{
 */
typedef struct{
	u32			beaconTxTime;			//!< The time that the device transmitted its last beacon frame
	addrExt_t	coordExtAddress;		//!< coordinator ieee address
	addrExt_t  	extAddress;				//!< ieee address
	u16			ackWaitDuration;		//!< the duration for waiting for an acknowledgment frame
	u16  		coordShortAddress;		//!< coordinator short address
	u16			panId;					//!< pan identifier
	u16 		shortAddress;  			//!< short address(joined in a network)
	u16  		superframeOrder;		//!< super-frame order
	u16 		transactionPersistenceTime;//! The maximum time (in unit periods) that a transaction is	stored by a coordinator and	indicated in its beacon.
	u16 		frameTotalWaitTime;
	u16			syncSymbolOffset;//4
	zb_mac_beacon_payload_t beaconPayload;//15
	u8  		beaconPayloadLen;
	u8  		associationPermit;
	u8			associatedPanCoord;
	u8  		autoReq;//4
	u8			battLifeExt;
	u8  		battLifeExtPeriods;
	u8  		beaconOrder;
	u8			beaconSeqNum;
	u8  		seqNum;
    u8  		frameRetryNum;
    u8			respWaitTime;
    u8			maxCsmaBackoffs;
    u8 			promiscuousMode;
    u8 			timeStampSupported;
    u8			securityEnabled;

    u8  		gtsPermit;
    u8  		minBe;
    u8  		maxBe;
    u8			phyPageCur;
    u8			phyChannelCur;
    u8 			rxOnWhenIdle;
    u8			txPower;
    u8 			resv[3];//76-bytes

#ifdef ZB_MAC_SECURITY
    tl_zb_mac_device_table_t devTable[MAC_DEVICE_TABLE_SIZE];
	u8            devTableEntry;
	u32           frameCounter;
	u8            secKey[16];
#endif

} tl_zb_mac_pib_t;
/** @} end of group zigbee_mac_pib_type */



#define MAC_IB()		g_zbMacPib

/* PIB access and min/max table type */
typedef struct
{
    u8     offset;
    u8     len;
    u8     min;
    u8     max;
} mac_pibTbl_t;

#endif /* TL_ZB_MAC_PIB_H */
