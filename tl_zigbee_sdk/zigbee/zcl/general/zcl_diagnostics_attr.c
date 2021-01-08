/********************************************************************************************************
 * @file	zcl_diagnostics_attr.c
 *
 * @brief	This is the source file for zcl_diagnostics_attr
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
#ifdef ZCL_DIAGNOSTICS

/* Attribute default */
#ifdef ZCL_ATTR_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT_ENABLE
u16 zcl_attr_averageMACRetryPerAPSMessageSent = 0;
#endif

/* Attribute record list */
const zclAttrInfo_t diagnostics_attrTbl[] =
{
#ifdef ZCL_ATTR_NUMBER_OF_RESETS_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_NUMBER_OF_RESETS						, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.numberOfResets                   },
#endif
#ifdef ZCL_ATTR_PERSISTENT_MEMORY_WRITES_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_PERSISTENT_MEMORY_WRITES				, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.persistentMemoryWrites           },
#endif
#ifdef ZCL_ATTR_MAC_RX_BCAST_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_MAC_RX_BCAST							, ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.macRxBcast                       },
#endif
#ifdef ZCL_ATTR_MAC_TX_BCAST_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_MAC_TX_BCAST							, ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.macTxBcast                       },
#endif
#ifdef ZCL_ATTR_MAC_RX_UCAST_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_MAC_RX_UCAST							, ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.macRxUcast                       },
#endif
#ifdef ZCL_ATTR_MAC_TX_UCAST_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_MAC_TX_UCAST                      		, ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.macTxUcast                       },
#endif
#ifdef ZCL_ATTR_MAC_TX_UCAST_RETRY_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_MAC_TX_UCAST_RETRY                 		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.macTxUcastRetry                  },
#endif
#ifdef ZCL_ATTR_MAC_TX_UCAST_FAIL_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_MAC_TX_UCAST_FAIL                  		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.macTxUcastFail                   },
#endif
#ifdef ZCL_ATTR_APS_RX_BCAST_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_RX_BCAST                      		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsRxBcast                       },
#endif
#ifdef ZCL_ATTR_APS_TX_BCAST_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_TX_BCAST                      		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsTxBcast                       },
#endif
#ifdef ZCL_ATTR_APS_RX_UCAST_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_RX_UCAST                      		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsRxUcast                       },
#endif
#ifdef ZCL_ATTR_APS_TX_UCAST_SUCCESS_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_TX_UCAST_SUCCESS               		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsTxUcastSuccess                },
#endif
#ifdef ZCL_ATTR_APS_TX_UCAST_RETRY_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_TX_UCAST_RETRY                 		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsTxUcastRetry                  },
#endif
#ifdef ZCL_ATTR_APS_TX_UCAST_FAIL_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_TX_UCAST_FAIL                  		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsTxUcastFail                   },
#endif
#ifdef ZCL_ATTR_ROUTE_DISC_INITIATED_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_ROUTE_DISC_INITIATED              		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.routeDiscInitiated               },
#endif
#ifdef ZCL_ATTR_NEIGHBOR_ADDED_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_NEIGHBOR_ADDED                   		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.neighborAdded                    },
#endif
#ifdef ZCL_ATTR_NEIGHBOR_REMOVED_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_NEIGHBOR_REMOVED                 		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.neighborRemoved                  },
#endif
#ifdef ZCL_ATTR_NEIGHBOR_STALE_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_NEIGHBOR_STALE                   		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.neighborStale                    },
#endif
#ifdef ZCL_ATTR_JOIN_INDICATION_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_JOIN_INDICATION                  		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.joinIndication                   },
#endif
#ifdef ZCL_ATTR_CHILD_MOVED_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_CHILD_MOVED                      		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.childMoved                       },
#endif
#ifdef ZCL_ATTR_NWK_FC_FAILURE_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_NWK_FC_FAILURE                    		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.nwkFCFailure                     },
#endif
#ifdef ZCL_ATTR_APS_FC_FAILURE_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_FC_FAILURE                    		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsFCFailure                     },
#endif
#ifdef ZCL_ATTR_APS_UNAUTHORIZED_KEY_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_UNAUTHORIZED_KEY              		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsUnauthorizedKey               },
#endif
#ifdef ZCL_ATTR_NWK_DECRYPT_FAILURES_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_NWK_DECRYPT_FAILURES              		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.nwkDecryptFailures               },
#endif
#ifdef ZCL_ATTR_APS_DECRYPT_FAILURES_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_APS_DECRYPT_FAILURES              		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.apsDecryptFailures               },
#endif
#ifdef ZCL_ATTR_PACKET_BUFFER_ALLOCATE_FAILURES_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_PACKET_BUFFER_ALLOCATE_FAILURES    		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.packetBufferAllocateFailures     },
#endif
#ifdef ZCL_ATTR_RELAYED_UCAST_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_RELAYED_UCAST                    		, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.relayedUcast                     },
#endif
#ifdef ZCL_ATTR_PHY_TO_MAC_QUEUE_LIMITREACHED_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_PHY_TO_MAC_QUEUE_LIMITREACHED       	, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.phytoMACqueuelimitreached        },
#endif
#ifdef ZCL_ATTR_PACKET_VALIDATE_DROP_COUNT_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_PACKET_VALIDATE_DROP_COUNT         	  	, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&g_sysDiags.packetValidateDropCount          },
#endif
#ifdef ZCL_ATTR_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT	, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&zcl_attr_averageMACRetryPerAPSMessageSent   },
#endif
#ifdef ZCL_ATTR_LAST_MESSAGE_LQI_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_LAST_MESSAGE_LQI                  		, ZCL_DATA_TYPE_UINT8 , ACCESS_CONTROL_READ, (u8*)&g_sysDiags.lastMessageLQI                   },
#endif
#ifdef ZCL_ATTR_LAST_MESSAGE_RSSI_ENABLE
	{ZCL_DIAGNOSTICS_ATTRID_LAST_MESSAGE_RSSI                 		, ZCL_DATA_TYPE_INT8  , ACCESS_CONTROL_READ, (u8*)&g_sysDiags.lastMessageRSSI                  },
#endif
	{ZCL_ATTRID_GLOBAL_CLUSTER_REVISION								, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_diagnostics_attrNum = ( sizeof(diagnostics_attrTbl) / sizeof(zclAttrInfo_t) );

#endif	/* ZCL_DIAGNOSTICS */
#endif
