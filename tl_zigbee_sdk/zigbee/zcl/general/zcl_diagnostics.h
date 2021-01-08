/********************************************************************************************************
 * @file	zcl_diagnostics.h
 *
 * @brief	This is the header file for zcl_diagnostics
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


status_t zcl_diagnostics_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


#endif	/* ZCL_DIAGNOSTICS_H */
