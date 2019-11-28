/********************************************************************************************************
 * @file     zcl_touchlink_attr.h
 *
 * @brief	 attribute configuration and APIs for zigbee light link touchlink
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
#pragma once

#include "tl_common.h"
#include "../../ss/security_service.h"

/*********************************************************************
 * CONSTANTS
 */

#define ZB_INTER_PAN_TRANS_ID_LIFETIME 		(8 * 1000 * 1000)
#define ZB_MAX_PERMIT_JOIN_DURATION			(60* 1000 * 1000)
#define ZB_MAX_LOST_PARENT_RETRY_ATTEMPTS	10
#define ZB_MIN_STARTUP_DELAY_TIME 			(2 * 1000 * 1000)  //2s
#define ZB_RX_WINDOW_DURATION				(5 * 1000 * 1000)  //5s
#define ZB_SCAN_TIME_BASE_DURATION 			(250 * 1000)     //250ms

/*********************************************************************
 * ENUMS
 */

typedef enum {
	LL_ROLE_COORD,
	LL_ROLE_ROUTER,
	LL_ROLE_END_DEVICE
} ll_role_t;



typedef enum
{
	 DEVELOPMENT_KEY = 0,
	 MASTER_KEY = 4,
	 CERTIFICATION_KEY = 15
}TOUCHLINK_KEY_TYPE; // zll_key_encryption_algorithms;

/*********************************************************************
 * STRUCTURES
 */

typedef struct {
	u16 magicNum;
	bool isFactoryNew;
	u8 currentChannel;
	u16 panId;
	u16 nwkAddr;
	addrExt_t ieeeAddr;
	extPANId_t extPanId;
	u8 parentRef;
	u16 freeNwkAddressRangeBegin;
	u16 freeNwkAddressRangeEnd;
	u16 freeGroupAddressRangeBegin;
	u16 freeGroupAddressRangeEnd;
	u16 base_neighbor_used;
	u16 ext_neighbor_used;
	u8 child_num;
	u8 ed_child_num;
	u16 n_elements;
	u16 n_sorted_elements;

#ifdef ZB_SECURITY
	ss_material_set_t secur_material_set[SECUR_N_SECUR_MATERIAL];
	u8              active_key_seq_number;
	u8           security_level:4;
	u8           secure_all_frames:1;
	u8           active_secur_material_i:2;
#endif
} zll_nv_params_t;

typedef struct
{
	/*   Define Address range values even if this device is not address
	 * assignment capable with value Zero.*/
	u16 freeNwkAddressRangeBegin; /* Start of range of network addresses this device can assign to its child devices */
	u16 freeNwkAddressRangeEnd;  /* End of range of network addresses this device can assign  to its child devices */
	u16 freeGroupAddressRangeBegin; /* Start of range of network addresses this device can assign to its child devices */
	u16 freeGroupAddressRangeEnd;  /* End of range of network addresses this device can assign  to its child devices */
} touchlink_attr_t;

/**
 *  @brief  External variable for ZLL Attribute
 */
//extern touchlink_attr_t g_touchlinkAttr;

/*********************************************************************
 * Public Functions
 */

void ll_reset(void);
void ll_get_encrypted_network_key(u8 key_index, u32 transaction_ID,
		u32 response_ID, u8 *encrypted_network_key);
void ll_decrypt_network_key(u8 key_index, u8 *encrypted_network_key,
		u32 transaction_ID,	u32 response_ID);
