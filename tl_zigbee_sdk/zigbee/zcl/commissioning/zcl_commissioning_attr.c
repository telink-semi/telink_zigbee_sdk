/********************************************************************************************************
 * @file     zcl_commissioning_attr.c
 *
 * @brief	 attribute configuration for commissioning cluster
 *
 * @author
 * @date     June. 10, 2017
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
#if 0
#include "tl_common.h"

/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_COMMISSIONING

/* Startup attributes set*/
	/* General startup attributes */
u16 zcl_attr_short_address = 0x00;/* NIB */
extPANId_t zcl_attr_extended_PAN_id = { 0 };/* NIB/AIB */
u16 zcl_attr_PAN_id = MAC_INVALID_PANID;/* PIB */
u32 zcl_attr_channel_mask = ZB_TRANSCEIVER_ALL_CHANNELS_MASK; /* AIB */
u8 zcl_attr_protocol_version = ZB_PROTOCOL_VERSION;/* Network layer constant */
u8 zcl_attr_stack_profile = ZB_STACK_PROFILE;/* NIB */
u8 zcl_attr_startup_control = 0;
	/* Security attributes */
addrExt_t zcl_attr_trust_center_address = {0, 0, 0, 0, 0, 0, 0, 0};/* AIB */
u8 zcl_attr_trust_center_master_key[16] = { 0, 0, 0, 0, 0, 0, 0, 0 };/* AIB */
u8 zcl_attr_network_key[16] = { 0, 0, 0, 0, 0, 0, 0, 0 };/* NIB */
u8 zcl_attr_use_insecure_join = true;/* AIB */
u8 zcl_attr_preconfigured_link_key[16] = { 0, 0, 0, 0, 0, 0, 0, 0 };/* AIB */
u8 zcl_attr_network_key_sequence_number = 0;/* NIB */
u8 zcl_attr_network_key_type = 0;/* NIB */
u16 zcl_attr_network_manager_address = 0x0000;/* NIB */

/* Join parameters attribute set */
u8 zcl_attr_scan_attempts = 0;
u16 zcl_attr_time_between_scans = 0;
u16 zcl_attr_rejoin_interval = 0;
u16 zcl_attr_max_rejoin_interval = 0;

u16 indirect_poll_rate = 0;
u8 zcl_attr_parent_retry_threshold = 0;

u8 zcl_attr_concentrator_flag = 0;
u8 zcl_attr_concentrator_radius = 0;
u8 zcl_attr_concentrator_discovery_time = 0;

/* Attribute record list */
const zclAttrInfo_t commissioning_attrTbl[] =
{
	{ ZCL_ATTR_SHORT_ADDRESS,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_short_address },
	{ ZCL_ATTR_EXTENDED_PAN_ID,  ZCL_DATA_TYPE_IEEE_ADDR,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_extended_PAN_id },
	{ ZCL_ATTR_PAN_ID,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_PAN_id },
	{ ZCL_ATTR_CHANNEL_MASK,  ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_channel_mask },
	{ ZCL_ATTR_PROTOCOL_VERSION,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_protocol_version },
	{ ZCL_ATTR_STACK_PROFILE,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_stack_profile },
	{ ZCL_ATTR_STARTUP_CONTROL,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_startup_control },

	{ ZCL_ATTR_TRUST_CENTER_ADDRESS,  ZCL_DATA_TYPE_IEEE_ADDR,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_trust_center_address },
	{ ZCL_ATTR_TRUST_CENTER_MASTER_KEY,  ZCL_DATA_TYPE_128_BIT_SEC_KEY,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)zcl_attr_trust_center_master_key },
	{ ZCL_ATTR_NETWORK_KEY,  ZCL_DATA_TYPE_128_BIT_SEC_KEY,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_network_key },
	{ ZCL_ATTR_USE_INSECURE_JOIN,  ZCL_DATA_TYPE_BOOLEAN,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_use_insecure_join },
	{ ZCL_ATTR_PRECONFIGURED_LINK_KEY,  ZCL_DATA_TYPE_128_BIT_SEC_KEY,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_preconfigured_link_key },
	{ ZCL_ATTR_NETWORK_KEY_SEQUENCE_NO,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_network_key_sequence_number },
	{ ZCL_ATTR_NETWORK_KEY_TYPE,  ZCL_DATA_TYPE_BOOLEAN,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_network_key_type },
	{ ZCL_ATTR_NETWORK_MANAGER_ADDRESS,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_network_manager_address },

	{ ZCL_ATTR_SCAN_ATTEMPTS,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_scan_attempts },
	{ ZCL_ATTR_TIME_BETWEEN_SCANS,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_time_between_scans },
	{ ZCL_ATTR_REJOIN_INTERVAL,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_rejoin_interval },
	{ ZCL_ATTR_MAX_REJOIN_INTERVAL,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_max_rejoin_interval },

	{ ZCL_ATTR_INDIRECT_POLL_RATE,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&indirect_poll_rate },
	{ ZCL_ATTR_PARENT_RETRY_THRESHOLD,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_parent_retry_threshold },

	{ ZCL_ATTR_CONCENTRATOR_FLAG,  ZCL_DATA_TYPE_BOOLEAN,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_concentrator_flag },
	{ ZCL_ATTR_CONCENTRATOR_RADIUS,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_concentrator_radius },
	{ ZCL_ATTR_CONCENTRATOR_DISCOVERY_TIME,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_concentrator_discovery_time },
};

const u8 zcl_commissioning_attrNum = ( sizeof(commissioning_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_COMMISSIONING */
#endif
