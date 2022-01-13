/********************************************************************************************************
 * @file    zcl_diagnostics_attr.c
 *
 * @brief   This is the source file for zcl_diagnostics_attr
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
