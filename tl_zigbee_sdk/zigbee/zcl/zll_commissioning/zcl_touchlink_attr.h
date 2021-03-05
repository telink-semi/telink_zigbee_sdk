/********************************************************************************************************
 * @file	zcl_touchlink_attr.h
 *
 * @brief	This is the header file for zcl_touchlink_attr
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef ZCL_TOUCHLINK_ATTR_H
#define ZCL_TOUCHLINK_ATTR_H


/*********************************************************************
 * CONSTANTS
 */

#define ZB_INTER_PAN_TRANS_ID_LIFETIME 		(8 * 1000)
#define ZB_MAX_PERMIT_JOIN_DURATION			(60* 1000)
#define ZB_MAX_LOST_PARENT_RETRY_ATTEMPTS	(10)
#define ZB_MIN_STARTUP_DELAY_TIME 			(2 * 1000)  //2s
#define ZB_RX_WINDOW_DURATION				(5 * 1000)  //5s
#define ZB_SCAN_TIME_BASE_DURATION 			(250)     	//250ms

/*********************************************************************
 * ENUMS
 */

typedef enum {
	LL_ROLE_COORD,
	LL_ROLE_ROUTER,
	LL_ROLE_END_DEVICE
} ll_role_t;



typedef enum {
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


#endif	/* ZCL_TOUCHLINK_ATTR_H */
