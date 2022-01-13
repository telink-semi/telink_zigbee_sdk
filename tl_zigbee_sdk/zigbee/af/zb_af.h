/********************************************************************************************************
 * @file    zb_af.h
 *
 * @brief   This is the header file for zb_af
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

#ifndef ZB_AF_H
#define	ZB_AF_H


#define ZDO_PROFILE_ID        					0x0000
#define HA_PROFILE_ID         					0x0104
#define LL_PROFILE_ID         					0xC05E
#define GP_PROFILE_ID							0xA1E0
#define WILDCARD_PROFILE_ID						0xFFFF

#define	ZDO_EP			      					0
#define	AF_BROADCAST_EP							0xFF

#define	MAX_ACTIVE_EP_NUMBER					8

#define	INVALID_CMP_RESULT						0xFF

/********************************************************************************
* Zigbee APFW types definition
********************************************************************************/
//These defines are used only during preprocessor phase. Please use DeviceType_t enumeration values instead.
#define	DEV_TYPE_COORDINATOR					0
#define	DEV_TYPE_ROUTER							1
#define	DEV_TYPE_ENDDEVICE						2
//Intended for specifying device role.

/* max nsdulength = aMaxPHYFrameSize -(nwkcMACFrameOverhead + nwkcMinHeaderOverhead) (D.4 aMaxMACFrameSize) */
#define	AF_NSDU_MAX_LEN							(MAX_PHY_FRM_SIZE - (ZB_MAC_FRAME_HEADER + ZB_NWK_FRAME_HEADER + ZB_APS_FRAME_HEADER))

//node frequency band bit map
#define NODE_FREQ_800							0x01//868 - 868.8 MHz
#define NODE_FREQ_900							0x04//902 - 928 MHz
#define NODE_FREQ_2400							0x08//2400 - 2483.5 MHz


typedef enum{
	DEVICE_TYPE_COORDINATOR 					= DEV_TYPE_COORDINATOR,
	DEVICE_TYPE_ROUTER      					= DEV_TYPE_ROUTER,
	DEVICE_TYPE_END_DEVICE  					= DEV_TYPE_ENDDEVICE
}device_type_t;

//ENUM NodeDescriptor_t::macCapbFlags field.
enum{
	MAC_CAP_DEVICE_TYPE_RFD 					= 0x00,
	MAC_CAP_ALTERNATE_PAN_COORDINATOR			= 0x01,
	MAC_CAP_DEVICE_TYPE_FFD						= 0x02,
	MAC_CAP_POWER_SOURCE_MAINS 					= 0x04,
	MAC_CAP_RX_ON_WHEN_IDLE 					= 0x08,
	MAC_CAP_SECURITY_CAPABLE 					= 0x40,
	MAC_CAP_ALLOCATE_ADDRESS 					= 0x80 //always set for rev13
};

//ENUM Node descriptor server mask
enum{
	ND_SERVER_MASK_NO_SERVER 					= 0x0000,
	ND_SERVER_MASK_PRIMARY_TRUST_CENTER 		= 0x0001,
	ND_SERVER_MASK_BACKUP_TRUST_CENTER 			= 0x0002,
	ND_SERVER_MASK_PRIMARY_BINDING_TABLE_CACHE 	= 0x0004,
	ND_SERVER_MASK_BACKUP_BINDING_TABLE_CACHE 	= 0x0008,
	ND_SERVER_MASK_PRIMARY_DISCOVERY_CACHE 		= 0x0010,
	ND_SERVER_MASK_BACKUP_DISCOVERY_CACHE 		= 0x0020,
	ND_SERVER_MASK_NETWORK_MANAGER 				= 0x0040,
	ND_SERVER_MASK_REV22						= (22 << 9),  //BIT(13)|BIT(11)|BIT(9)
	ND_SERVER_MASK_REV21						= (21 << 9),  //
	ND_SERVER_MASK_REV20						= (20 << 9)   //
};



/******************************************************************************************
 * @brief	The node descriptor contains information about the capabilities of the ZigBee node and is
 * 			mandatory for each node. There shall be only one node descriptor in a node.
 */
typedef struct{
	/*Logical type field 000 ZigBee coordinator, 001 ZigBee router, 010 ZigBee end device, 011-111 Reserved*/
	device_type_t logical_type:3;
	/*The complex descriptor available field of the node descriptor is one bit in length
	and specifies whether a complex descriptor is available on this device. If this field
	is set to 1, a complex descriptor is available. If this field is set to 0, a complex
	descriptor is not available.*/
	u8	complex_descriptor_ava:1;
	/*The user descriptor available field of the node descriptor is one bit in length and
	specifies whether a user descriptor is available on this device. If this field is set to
	1, a user descriptor is available. If this field is set to 0, a user descriptor is not
	available.*/
	u8	user_descriptor_ava:1;
	u8	reserved:3;//reserved
	/*The APS flags field of the node descriptor is three bits in length and specifies the
	application support sub-layer capabilities of the node.
	This field is currently not supported and shall be set to zero. */
	u8	aps_flag:3;//APS flags
	/*The frequency band field of the node descriptor is five bits in length and specifies
	  the frequency bands that are supported by the underlying IEEE 802.15.4 radio
	  utilized by the node. For each frequency band supported by the underlying IEEE 802.15.4 radio
	  0~868-868.6MHz
	  1~Reserved
	  2~902~928MHz
	  3~2400~2483.5MHz
	  4~reserved*/
	u8	freq_band:5;//Frequency band
	/*The MAC capability flags field is eight bits in length and specifies the node
	capabilities, as required by the IEEE 802.15.4-2003 MAC sub-layer
	 */
	//todo need to replace u8 with MAC_CAPABILITY_FLAG_T
	u8	mac_capability_flag;
	/*The manufacturer code field of the node descriptor is sixteen bits in length and
	specifies a manufacturer code that is allocated by the ZigBee Alliance, relating the
	manufacturer to the device.*/
	u8	mcL8;//Manufacturer code
	u8	mcH8;
	/*The maximum buffer size field of the node descriptor is eight bits in length, with a
	valid range of 0x00-0x7f. This field specifies the maximum size, in octets, of the
	network sub-layer data unit (NSDU) for this node. This is the maximum size of
	data or commands passed to or from the application by the application support
	sub-layer, before any fragmentation or re-assembly. */
	u8	max_buff_size;//Maximum buffer size
	/*The maximum transfer size field of the node descriptor is sixteen bits in length,
	with a valid range of 0x0000-0x7fff. This field specifies the maximum size, in
	octets, of the application sub-layer data unit (ASDU) that can be transferred to this
	node in one single message transfer. This value can exceed the value of the node
	maximum buffer size field (see sub-clause 2.3.2.3.8) through the use of
	fragmentation.*/
	u16	max_in_tr_size;//Maximum incoming transfer size
	/*The server mask field of the node descriptor is sixteen bits in length, with bit
	settings signifying the system server capabilities of this node. It is used to
	facilitate discovery of particular system servers by other nodes on the system.*/
	//todo need to define server mask info
	u16	server_mask;//Server mask
	/*The maximum transfer size field of the node descriptor is sixteen bits in length,
	with a valid range of 0x0000-0x7fff. This field specifies the maximum size, in
	octets, of the application sub-layer data unit (ASDU) that can be transferred from
	this node in one single message transfer. This value can exceed the value of the
	node maximum buffer size field (see sub-clause 2.3.2.3.8) through the use of
	fragmentation.*/
	u16	max_out_tr_size;//Maximum outgoing transfer size
	/*The descriptor capability field of the node descriptor is eight bits in length, with
	bit settings signifying the descriptor capabilities of this node. It is used to facilitate
	discovery of particular features of the descriptor fields by other nodes on the
	system.0: Extended Active Endpoint lst Available; 1: Extended Simple Descriptor lst Available;
	2~27: reserved*/
	u8	desc_capability_field;//Descriptor capability field
}node_descriptor_t;


//power mode enum
typedef enum{
	POWER_MODE_RECEIVER_SYNCRONIZED_WHEN_ON_IDLE	= 0,
	POWER_MODE_RECEIVER_COMES_PERIODICALLY		 	= 1,
	POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED 	 	= 2
}power_mode_t;

//ENUM available power source and main power src enum
typedef enum{
	POWER_SRC_MAINS_POWER 			= 0x1,
	POWER_SRC_RECHARGEABLE_BATTERY 	= 0x2,
	POWER_SRC_DISPOSABLE_BATTERY 	= 0x4,
}power_source_t;

//Current PWR SRC LEVEL
typedef enum{
	POWER_LEVEL_CRITICAL_0  = 0,
	POWER_LEVEL_PERCENT_33  = 4,
	POWER_LEVEL_PERCENT_66  = 8,
	POWER_LEVEL_PERCENT_100 = 0xC
}power_source_level_t;

typedef struct{
	power_mode_t current_power_mode:4;
	power_source_t available_power_sources:4;
	power_source_t current_power_source:4;
	power_source_level_t current_power_source_level:4;
}power_descriptor_t;


/****************************************************************************
 * @brief	The simple descriptor contains information specific to each endpoint contained in this node.
 * 			The simple descriptor is mandatory for each endpoint present in the node.
*/
typedef struct{
	u16	app_profile_id;			//APP profile ID specifies the profile which supported on this EP.
	u16	app_dev_id;				//APP DEV ID specifies the device description supported on this EP.

	u8	endpoint;				//end-point num of the simple descriptor 1 ~ 240
	u8	app_dev_ver:4;			//APP DEV version specifies the version of the device description supported
	u8	reserved:4;				//Reserved
	u8	app_in_cluster_count;	//The number of input clusters supported on this EP
	u8	app_out_cluster_count;	//The number of output clusters supported on this EP

	u16	*app_in_cluster_lst;	//Input cluster list address
	u16	*app_out_cluster_lst;	//Output cluster list address
}af_simple_descriptor_t;

/**
 *  @brief  Definition for endpoint information type
 */
typedef struct{
	tl_zb_addr_t dstAddr;

	u16  profileId;
	u16	 aliasSrcAddr;

	bool useAlias;
	u8	 aliasSeqNum;
	u8 	 dstAddrMode;
	u8   dstEp;

	aps_tx_options txOptions;
	u8   radius;
}epInfo_t;

/**
   Endpoints descriptor
 */
typedef void (*af_endpoint_cb_t)(void *p);
typedef void (*af_dataCnf_cb_t)(void *p);

typedef struct{
	af_endpoint_cb_t cb_rx;		/* data indication callback for this endoint */
	af_dataCnf_cb_t	cb_cnf;		/* data transmission confirm callback for this endpoint */
	af_simple_descriptor_t *correspond_simple_desc; /* the pointer to the simple descriptor for this endpoint */
	u8 ep;						/* the endpoint */
	u8 rsv[3];
}af_endpoint_descriptor_t;




/****************************************************************************************************
 * @brief		APP framework layer initialization
 *
 * @param[in]	none
 *
 * @return		none
 */
void af_init(void);

/****************************************************************************************************
 * @brief		AF layer node power descriptor set interface
 *
 * @param[in]	value: power_descriptor_t
 *
 * @return		none
 */
void af_powerDescriptorSet(power_descriptor_t *value);

/****************************************************************************************************
 * @brief       update the field of the power mode in the node power descriptor
 *
 * @param[in]   mode: the power mode will be used
 *
 * @return      none
 */
void af_powerDescPowerModeUpdate(power_mode_t mode);

/****************************************************************************************************
 * @brief		AF layer node descriptor set interface
 *
 * @param[in]	value: node_descriptor_t
 *
 * @return		none
 */
void af_nodeDescriptorSet(node_descriptor_t *value);

/****************************************************************************************************
 * @brief       update the field of the Manufacturer code in the node descriptor
 *
 * @param[in]   manuCode: the Manufacturer code will be used
 *
 * @return      none
 */
void af_nodeDescManuCodeUpdate(u16 manuCode);

/****************************************************************************************************
 * @brief       update the field of the RX on when idle in the node descriptor
 *
 * @param[in]   enable
 *
 * @return      none
 */
void af_nodeDescRxOnWhenIdleUpdate(bool enable);

/****************************************************************************************************
 * @brief       update the field of the stack revision in the node descriptor
 *
 * @param[in]   revision: the stack revision will be used
 *
 * @return      none
 */
void af_nodeDescStackRevisionSet(u8 revision);

/****************************************************************************************************
 * @brief       get the the stack revision of the node
 *
 * @param[in]   none
 *
 * @return      stack revision
 */
u8 af_nodeDescStackRevisionGet(void);

/****************************************************************************************************
 * @brief       get the field of the mac capability in node descriptor
 *
 * @param[in]   none
 *
 * @return      mac capability
 */
u8 af_nodeMacCapabilityGet(void);

/****************************************************************************************************
 * @brief       get the node descriptor device type
 *
 * @param[in]   none
 *
 * @return      device type
 */
device_type_t af_nodeDevTypeGet(void);

/***************************************************************************************************
 * @brief	Copy simple descriptor interface used building simple descriptor rsp message
 *
 * @param	*dst - destination address to store simple descriptor info
 *
 * 			*sd  - local simple descriptor
 *
 * @return	length of the simple descriptor info
 */
u8 af_simpleDescriptorCopy(u8 *dst, af_simple_descriptor_t *sd);

/***************************************************************************************************
 * @brief	Copy node descriptor information
 *
 * @param	*dst - dst address to hold node descriptor info
 *
 * @return	none
 */
void af_nodeDescriptorCopy(node_descriptor_t *dst);

/***************************************************************************************************
 * @brief	Copy power descriptor information
 *
 * @param	*dst - dst address to hold node descriptor info
 *
 * @return	none
 */
void af_powerDescriptorCopy(power_descriptor_t *dst);

/**************************************************************************************************
 * @brief		Interface used to get available EP number
 *
 * @param[in]	none
 *
 * @return		available EP number
 */
u8 af_availableEpNumGet(void);

/**************************************************************************************************
 * @brief	Interface to get the af endpoint descriptor
 *
 * @param	none
 *
 * @return	the point to the af endpoint descriptor
 */
af_endpoint_descriptor_t *af_epDescriptorGet(void);

/**************************************************************************************************
 * @brief	Interface to get ZDO simple descriptor
 *
 * @param	none
 *
 * @return	the point to the ZDO simple descriptor
 */
af_endpoint_descriptor_t *af_zdoSimpleDescriptorGet(void);

/**************************************************************************************************
 * @brief	Get simple descriptor according to the endpoint
 *
 * @param	ep - end point to compare
 *
 * @return	point to the simple descriptor info
 */
af_simple_descriptor_t *af_simpleDescGet(u8 ep);

/**************************************************************************************************
 * @brief	Check whether simple descriptor request's profile identifier matched with local registered end point info
 *
 * @param	profileID - profile identifier to compare
 *
 * @param	ep 		  - end point to compare
 *
 * @return	TRUE or FALSE
 */
bool af_profileMatchedLocal(u16 profileID, u8 ep);

/**************************************************************************************************
 * @brief	Check whether the cluster identifier matched with local registered end point info
 *
 * @param	clusterID	- cluster identifier to compare
 *
 * @param	ep 			- end point to compare
 *
 * @return	TURE or FALSE
 */
bool af_clusterMatchedLocal(u16 clusterID, u8 ep);

/**************************************************************************************************
 * @brief	check if the cluster Id matched to the simple descriptor
 *
 * @param	clusterID  cluster identifier
 *
 * @param	pSimpleDesc  simple descriptor
 *
 * @return	1: match,  0: don't match
 */
bool af_clsuterIdMatched(u16 clusterID, af_simple_descriptor_t *pSimpleDesc);

/**************************************************************************************
 * @brief	check if the profile Id matched to the simple descriptor
 *
 * @param	profileID  cluster identifier
 *
 * @param	pSimpleDesc  simple descriptor
 *
 * @return	1: match,  0: don't match
 */
bool af_profileIdMatched(u16 profileID, af_simple_descriptor_t *pSimpleDesc);

/**************************************************************************************************
 * @brief       Register an endpoint and it's simple descriptor
 *
 * @param[in]   endpoint     - endpoint
 *
 * @param[in]   simple_desc  - pointer to simple descriptor
 *
 * @param[in]   cb           - callback function to handle the APS data sent to this endpoint
 * 				cnfCb		 - data send confirm callback function
 *
 * @return      TRUE or FALSE
 */
bool af_endpointRegister(u8 ep, af_simple_descriptor_t *simple_desc, af_endpoint_cb_t rx_cb, af_dataCnf_cb_t cnfCb);

/**************************************************************************************************
 * @brief       Un-register an endpoint and it's simple descriptor
 *
 * @param[in]   ep     		- endpoint
 *
 * @return      TRUE or FALSE
 */
bool af_endpointUnregister(u8 ep);

/***********************************************************************************************************
 * @brief       Send AF data
 *
 * @param[in]   srcEp 		- source endpoint
 * @param[in]   pDstEpInfo  - destination information
 * @param[in]   clusterId  	- cluster identifier
 * @param[in]   cmdPldLen   - data length
 * @param[in]   cmdPld		- data payload
 * @param[in]   apsCnt   	- the APS count
 *
 * @return      Status
 */
u8 af_dataSend(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u16 cmdPldLen, u8 *cmdPld, u8 *apsCnt);

/***********************************************************************************************************
 * @brief	Interface to receive APS data confirm
 *
 * @param	arg: apsdeDataConf_t
 *
 * @return	none
 */
void af_dataCnfHandler(void *arg);

/***********************************************************************************************************
 * @brief	Interface to receive APS layer indication data
 *
 * @param	arg: aps_data_ind_t
 *
 * @return	none
 */
void af_aps_data_entry(void *arg);

/***********************************************************************************************************
 * @brief	Interface to receive APS layer indication fragment data
 *
 * @param	arg: aps_data_ind_t
 *
 * @return	none
 */
void af_aps_data_fragment_entry(void *arg);


#endif	/* ZB_AF_H */
