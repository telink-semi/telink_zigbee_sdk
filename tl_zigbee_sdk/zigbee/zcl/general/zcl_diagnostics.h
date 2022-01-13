/********************************************************************************************************
 * @file    zcl_diagnostics.h
 *
 * @brief   This is the header file for zcl_diagnostics
 *
 * @author  Zigbee Group
 * @date    2021
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

#ifndef ZCL_DIAGNOSTICS_H
#define ZCL_DIAGNOSTICS_H



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


status_t zcl_diagnostics_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


#endif	/* ZCL_DIAGNOSTICS_H */
