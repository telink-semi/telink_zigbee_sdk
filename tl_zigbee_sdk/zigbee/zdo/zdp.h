/********************************************************************************************************
 * @file    zdp.h
 *
 * @brief   This is the header file for zdp
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
 *
 *******************************************************************************************************/

#ifndef ZDP_H
#define	ZDP_H


#define	MAX_REQUESTED_CLUSTER_NUMBER				8
#define MAX_USER_DESC_LEN							16

#define	ZDO_CMD_PAYLOAD_MAX							52

/* Scan duration of the Mgmt_nwk_update_req command. */
#define	ZDO_NWK_MANAGER_MAX_SCAN_DURATION			5
#define	ZDO_NWK_MANAGER_CHANNEL_CHANGE				0xFE
#define	ZDO_NWK_MANAGER_ATTRIBUTES_CHANGE			0xFF

/*
 * ZDP cluster identifier
 * */
typedef enum{
	//Request
	NWK_ADDR_REQ_CLID                				= 0x0000,
	IEEE_ADDR_REQ_CLID               				= 0x0001,
	NODE_DESC_REQ_CLID         						= 0x0002,
	POWER_DESC_REQ_CLID        						= 0x0003,
	SIMPLE_DESC_REQ_CLID       						= 0x0004,
	ACTIVE_EP_REQ_CLID		        				= 0x0005,
	MATCH_DESC_REQ_CLID        						= 0x0006,
	COMPLEX_DESC_REQ_CLID      						= 0x0010,//DEPRECATED
	USER_DESC_REQ_CLID         						= 0x0011,//DEPRECATED
	DISCOVERY_CACHE_REQ_CLID          				= 0x0012,//DEPRECATED
	DEVICE_ANNCE_CLID            					= 0x0013,
	USER_DESC_SET_CLID          					= 0x0014,//DEPRECATED
	SYSTEM_SERVER_DISCOVERY_REQ_CLID 				= 0x0015,
	DISCOVERY_STORE_REQ_CLID         				= 0x0016,//DEPRECATED
	NODE_DESC_STORE_REQ_CLID         				= 0x0017,//DEPRECATED
	POWER_DESC_STORE_REQ_CLID        				= 0x0018,//DEPRECATED
	ACTIVE_EP_STORE_REQ_CLID         				= 0x0019,//DEPRECATED
	SIMPLE_DESC_STORE_REQ_CLID       				= 0x001A,//DEPRECATED
	REMOVE_NODE_CACHE_REQ_CLID       				= 0x001B,//DEPRECATED
	FIND_NODE_CACHE_REQ_CLID         				= 0x001C,//DEPRECATED
	EXTENDED_SIMPLE_DESC_REQ_CLID    				= 0x001D,//DEPRECATED
	EXTENDED_ACTIVE_EP_REQ_CLID      				= 0x001E,//DEPRECATED
	PARENT_ANNCE_CLID			 					= 0x001F,
	END_DEVICE_BIND_REQ_CLID         				= 0x0020,//DEPRECATED
	BIND_REQ_CLID                    				= 0x0021,
	UNBIND_REQ_CLID                  				= 0x0022,
	BIND_REGISTER_REQ_CLID           				= 0x0023,//DEPRECATED
	REPLACE_DEVICE_REQ_CLID          				= 0x0024,//DEPRECATED
	STORE_BCKUP_BIND_ENTRY_REQ_CLID  				= 0x0025,//DEPRECATED
	REMOVE_BCKUP_BIND_ENTRY_REQ_CLID 				= 0x0026,//DEPRECATED
	BACKUP_BIND_TABLE_REQ_CLID       				= 0x0027,//DEPRECATED
	RECOVER_BIND_TABLE_REQ_CLID      				= 0x0028,//DEPRECATED
	BACKUP_SOURCE_BIND_REQ_CLID      				= 0x0029,//DEPRECATED
	RECOVER_SOURCE_BIND_REQ_CLID     				= 0x002A,//DEPRECATED
	CLEAR_ALL_BINDINGS_REQ_CLID						= 0x002B,//TODO
	MGMT_NWK_DISC_REQ_CLID           				= 0x0030,//DEPRECATED
	MGMT_LQI_REQ_CLID                				= 0x0031,
	MGMT_RTG_REQ_CLID                				= 0x0032,
	MGMT_BIND_REQ_CLID               				= 0x0033,
	MGMT_LEAVE_REQ_CLID              				= 0x0034,
	MGMT_DIRECT_JOIN_REQ_CLID        				= 0x0035,//DEPRECATED
	MGMT_PERMIT_JOINING_REQ_CLID     				= 0x0036,
	MGMT_CACHE_REQ_CLID              				= 0x0037,//DEPRECATED
	MGMT_NWK_UPDATE_REQ_CLID	     				= 0x0038,
	MGMT_NWK_ENHANCED_UPDATE_REQ_CLID				= 0x0039,//TODO
	MGMT_NWK_IEEE_JOINING_LIST_REQ_CLID 			= 0x003A,//TODO
	MGMT_NWK_BEACON_SURVEY_REQ_CLID					= 0x003C,//TODO
	SEC_START_KEY_NEGOTIATION_REQ_CLID				= 0x0040,//TODO
	SEC_GET_AUTH_TOKEN_REQ_CLID						= 0x0041,//TODO
	SEC_GET_AUTH_LEVEL_REQ_CLID						= 0x0042,//TODO
	SEC_SET_CFG_REQ_CLID							= 0x0043,//TODO
	SEC_GET_CFG_REQ_CLID							= 0x0044,//TODO
	SEC_START_KEY_UPDATE_REQ_CLID					= 0x0045,//TODO
	SEC_DECOMMISSION_REQ_CLID						= 0x0046,//TODO
	SEC_CHALLENGE_REQ_CLID							= 0x0047,//TODO

	//Response
	NWK_ADDR_RSP_CLID 								= 0x8000,
	IEEE_ADDR_RSP_CLID								= 0x8001,
	NODE_DESC_RSP_CLID 								= 0x8002,
	POWER_DESC_RSP_CLID								= 0x8003,
	SIMPLE_DESC_RSP_CLID							= 0x8004,
	ACTIVE_EP_RSP_CLID								= 0x8005,
	MATCH_DESC_RSP_CLID								= 0x8006,
	COMPLEX_DESC_RSP_CLID							= 0x8010,//DEPRECATED
	USER_DESC_RSP_CLID         						= 0x8011,//DEPRECATED
	DISCOVERY_CACHE_RSP_CLID          				= 0x8012,//DEPRECATED
	USER_DESC_CONF_RSP_CLID          				= 0x8014,//DEPRECATED
	SYSTEM_SERVER_DISCOVERY_RSP_CLID				= 0x8015,
	DISCOVERY_STORE_RSP_CLID         				= 0x8016,//DEPRECATED
	NODE_DESC_STORE_RSP_CLID         				= 0x8017,//DEPRECATED
	POWER_DESC_STORE_RSP_CLID        				= 0x8018,//DEPRECATED
	ACTIVE_EP_STORE_RSP_CLID         				= 0x8019,//DEPRECATED
	SIMPLE_DESC_STORE_RSP_CLID       				= 0x801A,//DEPRECATED
	REMOVE_NODE_CACHE_RSP_CLID       				= 0x801B,//DEPRECATED
	FIND_NODE_CACHE_RSP_CLID         				= 0x801C,//DEPRECATED
	EXTENDED_SIMPLE_DESC_RSP_CLID    				= 0x801D,//DEPRECATED
	EXTENDED_ACTIVE_EP_RSP_CLID      				= 0x801E,//DEPRECATED
	PARENT_ANNCE_RSP_CLID			    			= 0x801F,
	END_DEVICE_BIND_RSP_CLID						= 0x8020,//DEPRECATED
	BIND_RSP_CLID									= 0x8021,
	UNBIND_RSP_CLID									= 0x8022,
	BIND_REGISTER_RSP_CLID           				= 0x8023,//DEPRECATED
	REPLACE_DEVICE_RSP_CLID          				= 0x8024,//DEPRECATED
	STORE_BCKUP_BIND_ENTRY_RSP_CLID  				= 0x8025,//DEPRECATED
	REMOVE_BCKUP_BIND_ENTRY_RSP_CLID 				= 0x8026,//DEPRECATED
	BACKUP_BIND_TABLE_RSP_CLID       				= 0x8027,//DEPRECATED
	RECOVER_BIND_TABLE_RSP_CLID      				= 0x8028,//DEPRECATED
	BACKUP_SOURCE_BIND_RSP_CLID      				= 0x8029,//DEPRECATED
	RECOVER_SOURCE_BIND_RSP_CLID     				= 0x802A,//DEPRECATED
	CLEAR_ALL_BINDINGS_RSP_CLID						= 0x802B,//TODO
	MGMT_NWK_DISC_RSP_CLID           				= 0x8030,//DEPRECATED
	MGMT_LQI_RSP_CLID								= 0x8031,
	MGMT_RTG_RSP_CLID                				= 0x8032,
	MGMT_BIND_RSP_CLID								= 0x8033,
	MGMT_LEAVE_RSP_CLID								= 0x8034,
	MGMT_DIRECT_JOIN_RSP_CLID        				= 0x8035,//DEPRECATED
	MGMT_PERMIT_JOINING_RSP_CLID					= 0x8036,
	MGMT_CACHE_RSP_CLID              				= 0x8037,//DEPRECATED
	MGMT_NWK_UPDATE_NOTIFY_CLID						= 0x8038,
	MGMT_NWK_ENHANCED_UPDATE_NOTIFY_CLID 			= 0x8039,//TODO
	MGMT_NWK_IEEE_JOINING_LIST_RSP_CLID 			= 0x803A,//TODO
	MGMT_NWK_UNSOLICITED_ENHANCED_UPDATE_NOTIFY_CLID = 0x803B,//TODO
	MGMT_NWK_BEACON_SURVEY_RSP_CLID					= 0x803C,//TODO
	SEC_START_KEY_NEGOTIATION_RSP_CLID				= 0x8040,//TODO
	SEC_GET_AUTH_TOKEN_RSP_CLID						= 0x8041,//TODO
	SEC_GET_AUTH_LEVEL_RSP_CLID						= 0x8042,//TODO
	SEC_SET_CFG_RSP_CLID							= 0x8043,//TODO
	SEC_GET_CFG_RSP_CLID							= 0x8044,//TODO
	SEC_START_KEY_UPDATE_RSP_CLID					= 0x8045,//TODO
	SEC_DECOMMISSION_RSP_CLID						= 0x8046,//TODO
	SEC_CHALLENGE_RSP_CLID							= 0x8047,//TODO
}zdp_clusterId_e;


typedef enum{
	ZDO_ADDR_REQ_SINGLE_REQ, 	ZDO_ADDR_REQ_SINGLE_RESP = ZDO_ADDR_REQ_SINGLE_REQ,
	ZDO_ADDR_REQ_EXTENDED_REQ,	ZDO_ADDR_REQ_EXTENDED_RESP = ZDO_ADDR_REQ_EXTENDED_REQ,
}zdo_addr_req_type;

/**
 * @brief the structure for nwk_addr_req command
 *
 * */
typedef struct{
	addrExt_t ieee_addr_interest;	/*!< The IEEE address to be matched by the Remote Device */
	u8 req_type;   					/*!< zdo_addr_req_type
										Request type for this command: \n
										0x00 - Single device response \n
										0x01 - Extended response \n
										0x02-0xFF - reserved */
	u8 start_index;					/*!< If the Request type for this command is Extended response,
									  	  the StartIndex provides the starting index for the requested elements
									  	  of the associated devices list */
}zdo_nwk_addr_req_t;

/**
 * @brief the structure for ieee_addr_req command
 *
 * */
typedef struct{
	u16 nwk_addr_interest;		/*!< NWK address that is used for IEEE address mapping */
	u8 req_type;				/*!< zdo_addr_req_type
									Request type for this command:
									0x00 C Single device response
									0x01 C Extended response
									0x02-0xff C reserved */
	u8 start_index;				/*!< If the Request type for this command is Extended response,
									  the StartIndex provides the starting index for the requested elements
									  of the associated devices list */
}zdo_ieee_addr_req_t;

/**
 * @brief the structure for node_descriptor_req command
 *
 * */
typedef struct{
	u16 nwk_addr_interest;		/*!< NWK address for the request */
}zdo_node_descriptor_req_t;

/**
 * @brief the structure for power_descriptor_req command
 *
 * */
typedef	zdo_node_descriptor_req_t		zdo_power_descriptor_req_t;

/**
 * @brief the structure for simple_descriptor_req command
 *
 * */
typedef struct _attribute_packed_{
	u16 nwk_addr_interest;			/*!< NWK address for the request */
	u8  endpoint;					/*!< The endpoint on the destination */
}zdo_simple_descriptor_req_t;

/**
 * @brief the structure for active_ep_req command
 *
 * */
typedef	zdo_node_descriptor_req_t		zdo_active_ep_req_t;

/**
 * @brief the structure for match_descriptor_req command
 *
 * */
typedef struct{
	u16	nwk_addr_interest;		/*!< NWK address for the request */
	u16	profile_id;				/*!< NWK address for the request */
	u8	num_in_clusters;		/*!< The number of Input Clusters provided for matching within the InClusterList */
	u8	num_out_clusters;		/*!< The number of Output Clusters provided for matching within OutClusterList */
	u16 cluster_list[2 * MAX_REQUESTED_CLUSTER_NUMBER];
}zdo_match_descriptor_req_t;

/**
 * @brief the structure for complex_descriptor_req command
 *
 * */
typedef	zdo_node_descriptor_req_t		zdo_complex_descriptor_req_t;

/**
 * @brief the structure for user_descriptor_req command
 *
 * */
typedef	zdo_node_descriptor_req_t		zdo_user_descriptor_req_t;

/**
 * @brief the structure for device_announce command
 *
 * */
typedef struct _attribute_packed_{
	u16	nwk_addr_local;					/*!< NWK address for the Local Device */
	addrExt_t ieee_addr_local;			/*!< IEEE address for the Local Device */
	capability_info_t mac_capability;	/*!< Capability of the local device */
}zdo_device_annce_req_t;

/**
 * @brief the structure for parent_announce command
 *
 * */
typedef struct{
	u8 numberOfChildren;	/*!< The number of the children */
	u8 *childInfo;			/*!< the pointer to the IEEE address of the child bound to the parent */
}zdo_parent_annce_req_t;

/**
 * 	@brief		This is generated from a local device wishing to configure the
 * 				user descriptor on a remote device. This command shall be unicast either to
 * 				the remote device itself or to an alternative device that contains the discovery
 * 				information of the remote device.
 **/
typedef struct{
	u8 len;			//0~16
	u8 info[MAX_USER_DESC_LEN];
}user_descriptor_t;

typedef struct{
	u16 nwk_addr_interest;
	user_descriptor_t user_desc;
}zdo_user_descriptor_set_req_t;

typedef struct{
	u16 server_mask;
}zdo_system_server_discovery_req_t;

/**
 * @brief the structure for End_Device_Bind_req
 *
 * */
typedef struct{
	u16	binding_target_addr;/*!< The address of the target for the binding */

	u16	profile_id;			/*!< ProfileID which is to be matched
							   between two End_Device_Bind_req
							   received at the ZigBee Coordinator
							   within the timeout value pre-configured
							   in the ZigBee Coordinator.*/
	addrExt_t src_ext_addr; /*!< The IEEE address of the device generating the request */
	u16	in_cluster_lst[MAX_REQUESTED_CLUSTER_NUMBER]; /*!< List of Input ClusterIDs to be used for matching */
	u16	out_cluster_lst[MAX_REQUESTED_CLUSTER_NUMBER];/*!< List of Output ClusterIDs to be used for matching */

	u8	src_endpoint;		/*!< The endpoint on the device generating the request */
	u8	num_in_clusters; 	/*!< The number of Input Clusters provided for end device binding within the InClusterList */
	u8	num_out_clusters; 	/*!< The number of Output Clusters provided for matching within OutClusterList */
}zdo_edBindReq_t;

typedef enum{
	SHORT_GROUPADDR_NODSTENDPOINT 	= 0x01,
	LONG_EXADDR_DSTENDPOINT 		= 0x03
}bind_req_dst_addr_mode;

typedef struct _attribute_packed_{
	addrExt_t src_addr; 	/*!< the device who build the binding table */

	u8 src_endpoint;		/*!< The source endpoint for the binding entry */
	u8 cid16_l;				/*!< The identifier of the cluster on the source device that is bound to the destination low 8-bit */
	u8 cid16_h;				/*!< The identifier of the cluster on the source device that is bound to the destination high 8-bit */
	u8 dst_addr_mode;		/*!< bind_req_dst_addr_mode
								0x01 - 16-bit group address for dstAddr and dstEp not present
								0x03 - 64-bit extAddr for dstAddr and estEp present */
	union _attribute_packed_{
		struct{
			addrExt_t dst_ext_addr;
			u8 dst_endpoint;
		};
		u16	dst_group_addr;
	};						/*!< The destination address for the binding entry */
}zdo_bind_req_t;

typedef zdo_bind_req_t 					zdo_unbind_req_t;
typedef zdo_bind_req_t 					zdo_bindTabListRec_t;

/**
 * @brief		Describes the parameters of the LQI response
 **/
typedef struct{
	u8 start_index;
}zdo_mgmt_lqi_req_t;

/**
 * @brief the structure for Mgmt_Bind_req
 *
 * */
typedef struct{
	u8 start_index;	/*!< Starting Index for the requested elements of the Binding Table */
}zdo_mgmt_bind_req_t;

/**
 * @brief the structure for Mgmt_Leave_req
 *
 * */
typedef struct{
	addrExt_t device_addr;		/*!< The 64-bit IEEE address of the entity to be re-moved from the network or NULL if the device removes itself from the network */
	struct{
		u8 reserved:6;
		u8 remove_children:1; 	/*!< This field has a value of 1 if the device being asked to leave the network is also being asked to remove its child devices,
									if any. Otherwise, it has a value of 0 */
		u8 rejoin:1;			/*!< This field has a value of 1 if the device being asked to leave from the current parent is requested to rejoin the network.
									Otherwise, it has a value of 0 */
	}lr_bitfields;
}zdo_mgmt_leave_req_t;

/**
 * @brief the structure for Mgmt_Permit_Joining_req
 *
 * */
typedef struct{
	u8	permit_duration;			/*!< The length of time in seconds during which the ZigBee coordinator or router will allow associations.
							  	  	  	  The value 0x00 and 0xff indicate that per-mission is disabled or enabled, respectively, without a specified time limit */
	u8	trust_center_significance; 	/*!< This field shall always have a value of 1, indicating a request to change the Trust Center policy.
										  If a frame is received with a value of 0, it shall be treated as having a value of 1 */
}zdo_mgmt_permit_join_req_t;

/**
 * @brief the structure for Mgmt_NWK_Update_req
 *
 * */
typedef struct{
	u32	scan_ch;				/*!< scan channels, Bitmap */
	u8  scan_duration;			/*!< length of time to scan each channel. 0xfe ~ for channel change
									0xff ~ for apsChannelMask and nwkManagerAddr attributes change */
	union{
		u8 scan_cnt;			/*!<  the number of energy scans to be conducted and reported */
		u8 nwk_update_id; 		/*!< The value of the nwkUpdateId contained in this request.
									This value is set by the Network Channel Manager prior to sending the message.
									This field shall only be present of the ScanDuration is 0xfe or 0xff.
									If the ScanDuration is 0xff, then the value in the nwkUpdateID shall be ignored */
	};
	u16 nwk_manager_addr; 		/*!< This field shall be present only if the ScanDuration is set to 0xff, and, where present,
									indicates the NWK address for the device with the Network Manager bit set in its Node De-scriptor */
}zdo_mgmt_nwk_update_req_t;

/**
 * @brief the structure for NWK_addr_rsp
 *
 * */
#define	MAX_RSPNUM			6		/*!< supported maximum number of the associated device */

typedef struct _attribute_packed_{
	u8 seq_num;  			/*! Sequence number which same with the request value */
	u8 status;	  			/*! Response status */
	addrExt_t ieee_addr_remote; /*!< 64-bit address for the Remote Device */
	u16	nwk_addr_remote;  	/*!< 16-bit address for the Remote Device */
	u8  num_assoc_dev; 		/*!< Count of the number of 16-bit short addresses to follow.
									If the RequestType in the request is Extended Response and there are no associated devices on the Remote Device,
									this field shall be set to 0.
									If an error occurs or the Request Type in the request is for a Single Device Response,
									this field shall not be included in the frame */
	u8  start_index;		/*!< Starting index into the list of associated devices for this report.
									If the RequestType in the request is Extended Response and there are no associated devices on the Remote Device,
									this field shall not be included in the frame.
									If an error occurs or the Request Type in the request is for a Single Device Response,
									this field shall not be included in the frame */
	u16 nwk_addr_assoc_dev_lst[MAX_RSPNUM]; /*!< A list of 16-bit addresses, one corresponding to each associated device to Remote Device;
													The number of 16-bit network addresses contained in this field is specified in the NumAssocDev field.
													If the RequestType in the request is Extended Response and there are no associated devices on the Remote Device,
													this field shall not be included in the frame.
													If an error occurs or the Request Type in the request is for a Single Device Response,
													this field shall not be included in the frame */
}zdo_nwk_addr_resp_t;

/**
 * @brief the structure for IEEE_addr_rsp
 *
 * */
typedef zdo_nwk_addr_resp_t 			zdo_ieee_addr_resp_t;

typedef struct _attribute_packed_{
	u8 	seq_num;  			/*! Sequence number which same with the request value */
	u8 	status;	  			/*! Response status */
	u16 nwk_addr_interest;
	node_descriptor_t node_descriptor;
}zdo_node_descript_resp_t;

typedef struct _attribute_packed_{
	u8 	seq_num;  			/*! Sequence number which same with the request value */
	u8 	status;	  			/*! Response status */
	u16 nwk_addr_interest;  /*! NWK address of the power descriptor request */
	power_descriptor_t power_descriptor;   /*! Power descriptor of the device */
}zdo_power_descriptor_resp_t;

typedef struct _attribute_packed_{
	u8	endpoint;				/*! end-point num of the simple descriptor 1 ~ 240 */
	u16	app_profile_id;			/*! APP profile ID specifies the profile which supported on this EP. */
	u16	app_dev_id;				/*! APP DEV ID specifies the device description supported on this EP. */
	u8	app_dev_ver:4;			/*! APP DEV version specifies the version of the device description supported */
	u8	reserved:4;				/*! Reserved */
	u8	listInfo[1];			/*! The cluster list supported on this EP */
}simple_descriptor_field_t;

typedef struct _attribute_packed_{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u16	nwk_addr_interest;		/*! NWK address for the request.*/
	u8	length;					/*! Length in bytes of the Simple Descriptor to follow. */
	simple_descriptor_field_t simple_descriptor; /*! Simple Descriptor */
}zdo_simple_descriptor_resp_t;

/**
 * @brief the structure for Active_EP_rsp
 *
 * */
typedef struct _attribute_packed_{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u16	nwk_addr_interest; 		/*!< NWK address for the request */
	u8	active_ep_count;  		/*!< The count of active endpoints on the Remote Device */
	u8	active_ep_lst[MAX_REQUESTED_CLUSTER_NUMBER]; /*!< List of bytes each of which represents an 8-bit endpoint */
}zdo_active_ep_resp_t;

/**
 *  @brief Structure for Match_Desc_rsp
 */
typedef struct _attribute_packed_{
	u8 	seq_num;  			/*! Sequence number which same with the request value */
	u8 	status;	  			/*! Response status */
	u16 nwk_addr_interest; 	/*!< NWK address for the request */
	u8 	matchLen; 			/*!< The count of endpoints on the Re-mote Device that match the request criteria */
	u8 	matchList[MAX_REQUESTED_CLUSTER_NUMBER]; /*!< List of bytes each of which represents an 8-bit endpoint */
}zdo_match_descriptor_resp_t;

/**
 * @brief the structure for Complex_Desc_rsp
 *
 * */
typedef struct _attribute_packed_{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u16 nwk_addr_interest;		/*!< NWK address for the request */
	u8  length;          		/*!< Length in bytes of the ComplexDescriptor field */
	u8	compDesc[1];
}zdo_complex_descriptor_resp_t;

/**
 * @brief the structure for User_Desc_rsp
 *
 * */
typedef struct _attribute_packed_{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u16 nwk_addr_interest;		/*!< NWK address for the request */
	u8  length;
	u8  user_desc[MAX_USER_DESC_LEN]; /*!< detailed user descriptor */
}zdo_user_descriptor_resp_t;

/**
 * @brief the structure for System_Server_Discovery_rsp
 *
 * */
typedef struct{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u16 server_mask; 			/*!< Mask of the server supported  */
}zdo_system_server_discovery_resp_t;

typedef struct{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u16 nwk_addr_interest;
}zdo_user_descriptor_conf_resp_t;

typedef struct{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u8	numberOfChildren;
	u8	*childInfo;
} zdo_parent_annce_rsp_t;

typedef struct{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
}zdo_bind_resp_t;

typedef zdo_bind_resp_t 		zdo_end_dev_bind_resp_t;
typedef zdo_bind_resp_t 		zdo_unbind_resp_t;

typedef struct _attribute_packed_{
	addrExt_t ext_pan_id;
	addrExt_t ext_addr;
	u16	network_addr;
	u8  deviceType:2;
	u8	rxOnWhenIdle:2;
	u8  relationship:3;
	u8  reserved1:1;
	u8  permitJoining:2;
	u8  reserved2:6;
	u8	depth;
	u8  lqi;
}neighbor_tbl_lst_t;

typedef struct _attribute_packed_{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u8  neighbor_tbl_entries;
	u8  start_index;
	u8  neighbor_tbl_lst_cnt;
	neighbor_tbl_lst_t neighbor_tbl_lst[1];
}zdo_mgmt_lqi_resp_t;

typedef struct _attribute_packed_{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
	u8 	bind_tbl_entries;
	u8 	start_index;
	u8 	bind_tbl_lst_cnt;
	zdo_bindTabListRec_t bind_tbl_lst[1];
}zdo_mgmt_bind_resp_t;

typedef struct{
	u8 	seq_num;  				/*! Sequence number which same with the request value */
	u8 	status;	  				/*! Response status */
}zdo_mgmt_leave_resp_t;

typedef zdo_bind_resp_t 	zdo_mgmt_permit_joining_resp_t;

typedef struct _attribute_packed_{
	u32 scanned_channels;
	u16 total_transmissions;
	u16 transmissions_fail;
	u8  size;
	u8  energyValues[1];
}ed_scan_t;

/**
 * @brief 		The type used to pack information about network updates in an argument of zdo_mgmt_nwk_update_noti() function
 * 				The struct of this type comprises information about certain network updates. The type of the event is indicated
 * 				by the zdo_mgmt_nwk_update_noti_t::status field. Additional event data is provided by the fields of the
 * 				unnamed union.
 **/
typedef struct _attribute_packed_{
	u8 seq_num;  /*! Sequence number which same with the request value */
	u8 status;	 /*! Response status */
	ed_scan_t scan_result;		/*! ED scan result */
}zdo_mgmt_nwk_update_noti_t;



typedef struct{
	u8 seq_num;
	union{
		zdo_nwk_addr_req_t					nwk_addr_req;
		zdo_ieee_addr_req_t              	ieee_addr_req;
		zdo_node_descriptor_req_t        	node_desc_req;
		zdo_power_descriptor_req_t          power_desc_req;
		zdo_simple_descriptor_req_t         simple_desc_req;
		zdo_active_ep_req_t              	active_ep_req;
		zdo_match_descriptor_req_t          match_desc_req;
		zdo_complex_descriptor_req_t        complex_desc_req;
		zdo_user_descriptor_req_t           user_desc_req;
		zdo_device_annce_req_t           	device_annce;
		zdo_parent_annce_req_t				parent_annce;
		zdo_user_descriptor_set_req_t       user_desc_set_req;
		zdo_system_server_discovery_req_t 	system_server_discovery_req;

		zdo_edBindReq_t						end_device_bind_req;
		zdo_bind_req_t                  	bind_req;
		zdo_unbind_req_t                	unbind_req;

		zdo_mgmt_lqi_req_t               	mgmt_lqi_req;
		zdo_mgmt_bind_req_t              	mgmt_bind_req;
		zdo_mgmt_leave_req_t             	mgmt_leave_req;
		zdo_mgmt_permit_join_req_t     		mgmt_permit_join_req;
		zdo_mgmt_nwk_update_req_t         	mgmt_nwk_update_req;
	};
}zdo_zdp_req_frame_t;

typedef struct{
	union{
		zdo_nwk_addr_resp_t					nwk_addr_resp;
		zdo_ieee_addr_resp_t				ieee_addr_resp;
		zdo_node_descript_resp_t			node_desc_resp;
		zdo_power_descriptor_resp_t     	power_desc_resp;
		zdo_simple_descriptor_resp_t		simple_desc_resp;
		zdo_active_ep_resp_t				active_ep_resp;
		zdo_match_descriptor_resp_t			match_desc_resp;
		zdo_complex_descriptor_resp_t   	complex_desc_resp;
		zdo_user_descriptor_resp_t      	user_desc_resp;
		zdo_system_server_discovery_resp_t  system_server_discovery_resp;
		zdo_user_descriptor_conf_resp_t     user_desc_Conf_resp;
		zdo_parent_annce_rsp_t				parent_annce_resp;

		zdo_end_dev_bind_resp_t				end_device_bind_resp;
		zdo_bind_resp_t						bind_resp;
		zdo_unbind_resp_t					unbind_resp;

		zdo_mgmt_lqi_resp_t               	mgmt_lqi_resp;
		zdo_mgmt_bind_resp_t              	mgmt_bind_resp;
		zdo_mgmt_leave_resp_t				mgmt_leave_resp;
		zdo_mgmt_permit_joining_resp_t		mgmt_permit_joining_resp;
		zdo_mgmt_nwk_update_noti_t         	mgmt_nwk_update_notify;
	};
}zdo_zdp_resp_frame_t;



typedef struct{
	u8  *zpdu;
	u16	src_addr; /*! the nwk address of the node that send this response */
	u16	clusterId;/*! cluster identifier */
	u8 	seq_num;  /*! Sequence number which same with the request value */
	u8 	status;	  /*! Response status */
	u8  length;
}zdo_zdpDataInd_t;


typedef void (*zdo_callback)(void *p);

typedef enum{
	SHORT_ADDR_MODE,
	EXT_ADDR_MODE
}addr_mode_t;

typedef struct{
	u8 *zdu;
	u8 *buff_addr;
	union{
		u16	dst_nwk_addr; 		//!< Destination short address. Specify this field if short address mode is applied, i.e. dstAddrMode equals SHORT_ADDR_MODE
		addrExt_t st_ext_addr;  //!< Destination extended address. Specify this field if extended address mode is applied, i.e. dstAddrMode equals EXT_ADDR_MODE
	};
	u16	cluster_id;
	u8	zduLen;
	u8	dst_addr_mode;//addr_mode_t
	zdo_callback zdoRspReceivedIndCb;
	u8	zdpSeqNum;
}zdo_zdp_req_t;

/********************************************************************************************
 * @brief	Interface used to send request packet to APS layer, put request parameter info
 * 			to the start address of the zb_buff_t, thus no need a memory space for parameter transmission
 * 			across layers
 *
 * @param	req - ZDO ZDP request struct, see zdo_zdp_req_t
 *
 * @return	none
 */
void zdo_send_req(zdo_zdp_req_t *req);

/***************************************************************************************
 * @brief	Interface to process NWK address request CMD
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_nwkAddrIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process IEEE address request CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_ieeeAddrIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process node descriptor, power descriptor and simple descriptor request CMD
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_descriptorsIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process active endpoint request CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_activeEpIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process match descriptor request CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_matchDescriptorIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process device announce CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_deviceAnnounceIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process parent announce CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_parentAnnounceIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process system server discovery CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_SysServerDiscoveryIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process end device bind request CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_endDeviceBindIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface called when receive bind/unbind request message from a remote device
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_bindOrUnbindIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process MGMT lqi request CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_mgmtLqiIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process MGMT bind request CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_mgmtBindIndicate(void *buf);

/*****************************************************************************************
 * @brief	Interface to process mgmt leave cmd, after receive MGMT_LEAVE_REQ_CLID, and also this function
 * 			can be called by local. After receive this cmd, this interface try to add the leave entry
 * 			to the leave req table in the NWK layer. If add failed, send out leave req immediately. If success,
 * 			would call nlme_leave_request later
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_mgmtLeaveIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process MGMT permit join request CMD.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_mgmtPermitJoinIndicate(void *buf);

/**************************************************************************************************************
 * @brief	Interface invoked when receive MGMT_NWK_UPDATE_REQ_CLID, process channel update, attributes change or ED
 * 			scan requests
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_mgmtNwkUpdateIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process parent announce response.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_parentAnnounceNotify(void *buf);

/***************************************************************************************
 * @brief	Interface to process nwk or ieee address response.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_remoteAddrNotify(void *buf);

/***************************************************************************************
 * @brief	Interface to prepare and send mgmt_nwk_update_notify.
 *
 * @param	buf: result of scan
 *
 * @return	none
 */
void zdo_nwkUpdateNotifyRespSend(void *buf);

/***************************************************************************************
 * @brief	Interface to process complex descriptor request.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_complexDescIndicate(void *buf);

/***************************************************************************************
 * @brief	Interface to process user descriptor request.
 *
 * @param	buf: received packet info with APS data indication primitive
 *
 * @return	none
 */
void zdo_userDescIndicate(void *buf);

/**************************************************************************************************
 * @brief	ZDO layer callback function find and execute interface
 *
 * @param	tsn: transaction sequence number used for callback function sort or internal defined SQ number(start 0xf001
 * 				 check the details of zdo_user_cb_seq_enum)
 * 			buf: call back data address
 *
 * @return	none
 */
void zdp_cb_process(u16 tsn, void *buf);

/**************************************************************************************************
 * @brief	ZDP initialization
 *
 * @param	none
 *
 * @return	none
 */
void zdp_init(void);


#endif	/* ZDP_H */

