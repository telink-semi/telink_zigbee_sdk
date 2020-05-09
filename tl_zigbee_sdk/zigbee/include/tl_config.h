/********************************************************************************************************
 * @file     tl_config.h
 *
 * @brief    zigbee configuration header file
 *
 * @author
 * @date     May. 27, 2017
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
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
#pragma once

#include "tl_common.h"

/**********************************************************************
 * for TEST
 */
#define 		ZB_MAC_TEST_ENABLE		0
#define 		ZB_NWK_TEST_ENABLE		0

/**********************************************************************
 * for zigbee stack common config
 */

//Stack profile
#define ZB_STACK_PROFILE 		2

#define ZB_FRAME_TYPE_MASK		0x03

#define ZB_PROTOCOL_VER_MASK	0x3c
#define ZB_PROTOCOL_VER_POS		2

//Protocol version
#define ZB_PROTOCOL_VERSION 	2

#define ZB_PROTOCOL_VERSION_GP	3


//Default value for mac frame version subfield
#define MAC_FRAME_VERSION 		MAC_FRAME_IEEE_802_15_4_2003

//Address assign (NOTE: We only use stochastic address assign for ZigBee PRO)
//If defined, use distributed address assign for tree and for mesh routing (ZigBee 2007).
#ifndef ZB_NWK_DISTRIBUTED_ADDRESS_ASSIGN
//If defined, use stochastic address assign (ZigBee PRO).
  #define ZB_NWK_STOCHASTIC_ADDRESS_ASSING
#endif

/* Some defaults for ZDO startup */
/********************************************************************
 * @brief	ZDO layer external configure items
 */

//all channels: 11 ~ 26
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   			0x07FFF800 /* 0000.0111 1111.1111 1111.1000 0000.0000*/


/*******************************ZOO Default Configuration Attribute Definitions***********************************************/
#define ZDO_PERMIT_JOIN_DURATION					0

#define POLL_RATE_QUARTERSECONDS					250//1 qs = 250 ms

#define POLL_NO_DATA_MAX_COUNT						3

//Integer value representing the number of scan attempts to make before
//the NWK layer decides which ZigBee coordinator or router to associate with.
//This attribute has default value of 5 and valid values between 1 and 255.
#define	ZDO_NWK_SCAN_ATTEMPTS						2

//Integer value representing the time duration between each NWK discovery attempt described by :Config_NWK_Scan_Attempts.
//This attribute has a default value 0xc35 (100 milliseconds) and valid values between 1 and 65535 milliseconds.
#define ZDO_NWK_TIME_BTWN_SCANS 					100

//The value in milliseconds, for the device to request indirect transmission messages from the parent.
#define ZDO_NWK_INDIRECT_POLL_RATE					4 * POLL_RATE_QUARTERSECONDS//1000 ms

//Contents of the link retry threshold for parent link.
#define	ZDO_MAX_PARENT_THRESHOLD_RETRY				5

//Contents of the rejoin interval in seconds
#define ZDO_REJOIN_TIMES							2
#define	ZDO_REJOIN_INTERVAL							6
#define ZDO_MAX_REJOIN_INTERVAL						30
#define ZDO_REJOIN_BACKOFF_TIME						30
#define ZDO_MAX_REJOIN_BACKOFF_TIME					90
/******************************************************************************************************************************/

#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
#if ZB_ROUTER_ROLE
#if ZB_COORDINATOR_ROLE
	#define NWK_ROUTE_RECORD_TABLE_NUM				127
#endif
	#define TL_ZB_NWK_ADDR_MAP_NUM					128
	#define ROUTING_TABLE_NUM						48
#endif
#elif defined (MCU_CORE_826x)
#if ZB_ROUTER_ROLE
#if ZB_COORDINATOR_ROLE
	#define NWK_ROUTE_RECORD_TABLE_NUM				64
#endif
	#define TL_ZB_NWK_ADDR_MAP_NUM					64
	#define ROUTING_TABLE_NUM						24
#endif
#endif

/*************************************************************************************/
#define POLL_RATE									(4 * POLL_RATE_QUARTERSECONDS)//4 qs = 1s
#define RESPONSE_POLL_RATE							POLL_RATE_QUARTERSECONDS//250ms
#define QUEUE_POLL_RATE								POLL_RATE_QUARTERSECONDS//250ms
#define REJOIN_POLL_RATE							(2 * POLL_RATE_QUARTERSECONDS)//500ms
/*************************************************************************************/

//CCM key size. Hard-coded
#define	CCM_KEY_SIZE								16

//NLS5 - All devices shall maintain at least 2 NWK keys with the frame
//counters consistent with the security mode of the network (Standard or High).
#define	SECUR_N_SECUR_MATERIAL						2


//If defined, 2007 stack profile is implemented
#define ZB_STACK_PROFILE_2007


//SECUR: CCM M parameter. Fixed to 4 for security level 5
#define	ZB_CCM_M									4


//if defined, enable APS security
#define APS_FRAME_SECURITY

extern u8	g_zb_txPowerSet;



extern const addrExt_t g_zero_addr;
extern const addrExt_t g_invalid_addr;
extern const u8 g_null_securityKey[];

extern const u8 nwkKeyDefault[];

extern const u8 tcLinkKeyCentralDefault[];		/* default global tc link key */

extern const u8 linkKeyDistributedCertification[];	/* Certification distribute link key */
extern const u8 linkKeyDistributedMaster[];

extern const u8 touchLinkKeyCertification[];		/* Certification touch-link key */
extern const u8 touchLinkKeyMaster[];
