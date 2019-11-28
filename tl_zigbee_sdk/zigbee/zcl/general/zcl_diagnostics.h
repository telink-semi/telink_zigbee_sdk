/********************************************************************************************************
 * @file     zcl_diagnostics.h
 *
 * @brief	 APIs and attributes statement for diagnostics cluster
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
#include "../../zdo/zb_af.h"
#include "../zcl.h"


/**
 *  @brief	zcl_diagnostics_attrId DIAGNOSTICS Attribute Ids
 */
#define ZCL_DIAGNOSTICS_ATTRID_NUMBER_OF_RESETS					   			0x0000
#define ZCL_DIAGNOSTICS_ATTRID_PERSISTENT_MEMORY_WRITES						0x0001
#define ZCL_DIAGNOSTICS_ATTRID_MAC_RX_BCAST									0x0100
#define ZCL_DIAGNOSTICS_ATTRID_MAC_TX_BCAST									0x0101
#define ZCL_DIAGNOSTICS_ATTRID_MAC_RX_UCAST									0x0102
#define ZCL_DIAGNOSTICS_ATTRID_MAC_TX_UCAST                        			0x0103
#define ZCL_DIAGNOSTICS_ATTRID_MAC_TX_UCAST_RETRY                     		0x0104
#define ZCL_DIAGNOSTICS_ATTRID_MAC_TX_UCAST_FAIL                      		0x0105
#define ZCL_DIAGNOSTICS_ATTRID_APS_RX_BCAST                          		0x0106
#define ZCL_DIAGNOSTICS_ATTRID_APS_TX_BCAST                          		0x0107
#define ZCL_DIAGNOSTICS_ATTRID_APS_RX_UCAST                          		0x0108
#define ZCL_DIAGNOSTICS_ATTRID_APS_TX_UCAST_SUCCESS                   		0x0109
#define ZCL_DIAGNOSTICS_ATTRID_APS_TX_UCAST_RETRY                     		0x010A
#define ZCL_DIAGNOSTICS_ATTRID_APS_TX_UCAST_FAIL                      		0x010B
#define ZCL_DIAGNOSTICS_ATTRID_ROUTE_DISC_INITIATED                  		0x010C
#define ZCL_DIAGNOSTICS_ATTRID_NEIGHBOR_ADDED                       		0x010D
#define ZCL_DIAGNOSTICS_ATTRID_NEIGHBOR_REMOVED                     		0x010E
#define ZCL_DIAGNOSTICS_ATTRID_NEIGHBOR_STALE                       		0x010F
#define ZCL_DIAGNOSTICS_ATTRID_JOIN_INDICATION                      		0x0110
#define ZCL_DIAGNOSTICS_ATTRID_CHILD_MOVED                          		0x0111
#define ZCL_DIAGNOSTICS_ATTRID_NWK_FC_FAILURE                        		0x0112
#define ZCL_DIAGNOSTICS_ATTRID_APS_FC_FAILURE                        		0x0113
#define ZCL_DIAGNOSTICS_ATTRID_APS_UNAUTHORIZED_KEY                  		0x0114
#define ZCL_DIAGNOSTICS_ATTRID_NWK_DECRYPT_FAILURES                  		0x0115
#define ZCL_DIAGNOSTICS_ATTRID_APS_DECRYPT_FAILURES                  		0x0116
#define ZCL_DIAGNOSTICS_ATTRID_PACKET_BUFFER_ALLOCATE_FAILURES        		0x0117
#define ZCL_DIAGNOSTICS_ATTRID_RELAYED_UCAST                        		0x0118
#define ZCL_DIAGNOSTICS_ATTRID_PHY_TO_MAC_QUEUE_LIMITREACHED           		0x0119
#define ZCL_DIAGNOSTICS_ATTRID_PACKET_VALIDATE_DROP_COUNT             		0x011A
#define ZCL_DIAGNOSTICS_ATTRID_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT    	0x011B
#define ZCL_DIAGNOSTICS_ATTRID_LAST_MESSAGE_LQI                      		0x011C
#define ZCL_DIAGNOSTICS_ATTRID_LAST_MESSAGE_RSSI                     		0x011D




#if 0
/**
 *  @brief  External variable for GP Attribute info table
 */
extern const zclAttrInfo_t diagnostics_attrTbl[];
extern const u8 zcl_diagnostics_attrNum;
#endif


status_t zcl_diagnostics_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


