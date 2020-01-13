/********************************************************************************************************
 * @file     zdo_api.h
 *
 * @brief	 contains define, MACRO and interfaces used by ZDO up layers
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

#ifndef ZDO_API_H
#define	ZDO_API_H

#include "tl_common.h"
#include "../mac/includes/tl_zb_mac.h"
#include "../nwk/includes/nwk.h"
#include "../nwk/includes/nwk_ctx.h"
#include "zb_af.h"

#define	DEBUG_ZDO_LAYER		0


/***********************************************************************************
 * @brief	The structure for indication of data reception. A pointer to an instance of
 * 			this type is passed to a data indication callback registered for a given endpoint
 * 			when the data is received destined to the endpoint. The structure definition follows
 * 			APSDE-DATA indication primitive described in  Zigbee Specification r18, 2.2.4.1.3 APSDE-DATA.indication,
 * 			page 29.
 */
#define			MAC_HEADER_MAX_SIZE			23
#define			NWK_HEADER_MAX_SIZE			25
#define			SECURE_HEADER_SIZE			14
#define			APS_HEADER_MAX_SIZE			8
#define			APS_MAXPACKET_LENGTH		(127 - MAC_HEADER_MAX_SIZE - NWK_HEADER_MAX_SIZE - SECURE_HEADER_SIZE - APS_HEADER_MAX_SIZE)


#define			SUPERFRAME_TIME2US(t)			((t*15360))
typedef struct
{
  u16 server_mask;
} zdo_system_server_discovery_req_t;


/**************************************************************************
 * @brief 	Possible address modes
*/
typedef enum
{
  SHORT_ADDR_MODE,
  EXT_ADDR_MODE
} addr_mode_t;

/**********************************************************************************
 * @brief		zdo result status primitives.
*/
typedef enum
{
  //Success status
  ZDO_SUCCESS								= 0x00,
  ZDO_INVALID_PARAMETERS					= 0x01,
  ZDO_RESPONSE_WAIT_TIMEOUT			        = 0x02,
  ZDO_BUSY                           		= 0x04,
  ZDO_NO_NETWORKS                           = 0x05,
  ZDO_EXTPANID_ALREADY_EXIST	            = 0x06,
  ZDO_FAIL									= 0x07,
  ZDO_AUTHENTICATION_SUCCESS				= 0x08,
  ZDO_AUTHENTICATION_FAIL					= 0x09,
  ZDO_SECURITY_SUCCESS						= 0x0A,
  ZDO_SECURITY_FAIL							= 0x0B,
  ZDO_SECURITY_NOT_SUPPORTED				= 0x0C,
  ZDO_CMD_COMPLETED							= 0x0D,
  ZDO_KEY_ESTABLISHMENT_NOT_SUPPORTED		= 0x0E,

  ZDO_NO_KEY_PAIR_DESCRIPTOR				= 0x50,

  ZDO_UPDATE_LINK_KEY						= 0x51,

  ZDO_UPDATE_MASTER_KEY						= 0x52,

  ZDO_DELETE_KEY_PAIR						= 0x53,

  ZDO_DELETE_LINK_KEY						= 0x54,


  ZDO_TC_NOT_AVAILABLE						= 0x60,

  ZDO_INVALID_REQUEST						= 0x80,

  ZDO_DEVICE_NOT_FOUND						= 0x81,

  ZDO_INVALID_EP							= 0x82,

  ZDO_NOT_ACTIVE							= 0x83,

  ZDO_NOT_SUPPORTED							= 0x84,

  ZDO_TIMEOUT								= 0x85,

  ZDO_NO_MATCH								= 0x86,

  ZDO_NO_ENTRY								= 0x88,

  ZDO_NO_DESCRIPTOR							= 0x89,

  ZDO_INSUFFICIENT_SPACE					= 0x8A,

  ZDO_NOT_PERMITTED							= 0x8B,

  ZDO_TABLE_FULL							= 0x8C,

  ZDO_NOT_AUTHORIZED						= 0x8D,

  ZDO_NWK_UPDATE							= 0x8E,

  ZDO_NETWORK_STARTED						= 0x8F,

  ZDO_NETWORK_LOST							= 0x90,

  ZDO_NETWORK_LEFT							= 0x91,

  ZDO_CHILD_JOINED							= 0x92,

  ZDO_CHILD_REMOVED							= 0x93,

  ZDO_USER_DESCRIPTOR_UPDATE				= 0x94,

  ZDO_STATIC_ADDRESS_CONFLICT				= 0x95
} zdo_status_t;

#define		ZDO_CB_NO_FREE			0xff
typedef void (*zdo_callback)(void *p);
typedef u8 (*zdo_nwkupdate_cb)(void *p);

typedef struct{
	zdo_callback		zdpStartDevCnfCb;
	zdo_callback		zdpResetCnfCb;
	zdo_callback		zdpDevAnnounceIndCb;
	zdo_callback		zdpLeaveIndCb;
	zdo_callback		zdpLeaveCnfCb;
	zdo_nwkupdate_cb	zdpNwkUpdateIndCb;
	zdo_callback		zdpPermitJoinIndCb;
	zdo_callback		zdoNlmeSyncCnfCb;
}zdo_appIndCb_t;

typedef bool (*zdo_leaveCallback)(void *p);

typedef struct{
	zdo_leaveCallback	zdpLeaveCnfCb;
}zdo_touchLinkCb_t;

typedef void (*zdo_txCnfCb)(void *p);


typedef struct{
	u8 *zdu;
	u8 *buff_addr;
	union{
		u16				dst_nwk_addr; //!< Destination short address. Specify this field if short address mode is applied, i.e. dstAddrMode equals SHORT_ADDR_MODE
		addrExt_t		st_ext_addr;//!< Destination extended address. Specify this field if extended address mode is applied, i.e. dstAddrMode equals EXT_ADDR_MODE
	};
	u16	cluster_id;
	u8	zduLen;
	u8	dst_addr_mode;
	zdo_callback zdoRspReceivedIndCb;
	u8	zdpSeqNum;
}zdo_zdp_req_t;

/**
 *  @brief Structure for parameter of startDev callback function
 */
typedef struct{
    zdo_status_t	status;
    u8				channel_num;
    u16				pan_id;
    u16				short_addr;
}zdo_start_device_confirm_t;


typedef struct{
	/*Integer value representing the time duration (in OctetDurations) between each NWK discovery attempt.
	 It is employed within ZDO to provide a time duration between the NLME-NETWORKDISCOVERY.request attempts*/
	u32		config_nwk_indirectPollRate;//in ms

	u16		config_nwk_time_btwn_scans;//in ms, default value, 100ms on 2.4GHz
	u16		config_rejoin_interval;//The units of this attribute are seconds and the default value is ZDO_REJOIN_INTERVAL.
	/* The units of this attribute are seconds and the default value is ZDO_MAX_REJOIN_INTERVAL. If set ZERO, means it will not do rejoin. */
	u16		config_max_rejoin_interval;

	/*The :Config_NWK_Scan_Attempts is employed within ZDO to call the NLME-NETWORKDISCOVERY.
	 request primitive the indicated number of times (for routers and end devices).Integer value
	 representing the number of scan attempts to make before the NWK layer decides which ZigBee
	 coordinator or router to associate with*/
	u8		config_nwk_scan_attempts;//This attribute has default value of 5 and valid values between 1 and 255.
	u8		config_permit_join_duration; //< Permit join duration, 0x00 - disable join, 0xff - join is allowed forever
	u8		config_parent_link_retry_threshold;//number of retry parent syns before judged as connection lost and the default value is ZDO_MAX_PARENT_THRESHOLD_RETRY
}zdo_attrCfg_t;


extern zdo_attrCfg_t zdo_cfg_attributes;

typedef struct{
	extPANId_t	nwkExtendedPANID;
	u8			apsDesignatedCoordinator;
	u32			apsChannelMask;
	extPANId_t	apsUseExtendedPANID;
	u8			apsUseInsecureJoin;
}zdo_startup_attr_t;

#define			MAX_REQUESTED_CLUSTER_NUMBER	8

/**
 *  @brief Structure for parameter of nlmeLeaveConf callback function
 */
typedef struct
{
  u8 status; /*!< MAC status codes */
  addrExt_t device_address; /*!< 64 bit IEEE address */
}nlmeLeaveConf_t;


/**
 *  @brief Structure for parameter of nlmeLeaveInd callback function
*/
typedef struct
{
  addrExt_t device_address; /*!< 64 bit IEEE address of leaving device. If it is all zero, the device it self
                                 has left */
  u8 rejoin; /*!< if join after leave */
}nlmeLeaveInd_t;


/**************************************************************************
 * @brief 		Information about the addresses of a device.
*/
typedef struct
{
	u16 	short_addr; //!< Short address
	u64   	ext_addr; //!< Extended address
} node_addr_t;

typedef struct{
	u16 	short_addr; //!< Short address
	u64   	ext_addr; //!< Extended address
	u8		mac_capability_info; //!< Device capability information
} child_info_t;


typedef struct{
	u32        scanned_channels;
	u16        total_transmissions;
	u16        transmissions_fail;
	u8         size;
	u8         energyValues[1];
} ed_scan_t;

/**************************************************************************
 * 	@brief 		This information is provided to inform an application about new network parameters if a device
 * 				rejoined a network.
*/
typedef struct{
	u16	 	parent_short_addr; //!< Parent short address
	u16		panId; //!< PAN ID of a network
	u16		assigned_short_addr; //!< New short address
	u8		current_ch; //!< Current channel
} zdo_network_info_t;

typedef enum{
	ZDO_ADDR_REQ_SINGLE_RESP,
	ZDO_ADDR_REQ_EXTENDED_RESP
}zdo_addr_req_type;

/**************************************************************************
 * @brief		Describes the parameters of the LQI response
*/
typedef struct{
	u8   start_index;
}zdo_mgmt_lqi_req_t;

/**
 * @brief the structure for nwk_addr_req command
 *
 * */
typedef struct{
	addrExt_t ieee_addr_interest;		 /*!< The IEEE address to be matched by the Remote Device */
	zdo_addr_req_type   req_type;      /*!< Request type for this command: \n
										0x00 每 Single device response \n
										0x01 每 Extended response \n
										0x02-0xFF 每 reserved */
	u8   start_index;					/*!< If the Request type for this command is Extended response,
									  	  the StartIndex provides the starting index for the requested elements
									  	  of the associated devices list */
} zdo_nwk_addr_req_t;


/**
 * @brief the structure for ieee_addr_req command
 *
 * */
typedef struct{
	u16 nwk_addr_interest;		/*!< NWK address that is used for IEEE address mapping */
	zdo_addr_req_type req_type;	/*!< Request type for this command:
									0x00 每 Single device response
									0x01 每 Extended response
									0x02-0xff 每 reserved */
	u8 start_index;				/*!< If the Request type for this command is Extended response,
									  the StartIndex provides the starting index for the requested elements
									  of the associated devices list */
} zdo_ieee_addr_req_t;


/**
 * @brief the structure for node_descriptor_req command
 *
 * */
typedef struct{
	u16 nwk_addr_interest;		/*!< NWK address for the request */
} zdo_node_descriptor_req_t;


/**
 * @brief the structure for simple_descriptor_req command
 *
 * */
typedef struct{
	u16 nwk_addr_interest;			/*!< NWK address for the request */
	u8  endpoint;						/*!< The endpoint on the destination */
} zdo_simple_descriptor_req_t;


/**
 * @brief the structure for match_descriptor_req command
 *
 * */
typedef struct{
	u16	nwk_addr_interest;		/*!< NWK address for the request */
	u16	profile_id;				/*!< NWK address for the request */
	u8	num_in_clusters;		/*!< The number of Input Clusters provided for matching within the InClusterList */
	u8	num_out_clusters;		/*!< The number of Output Clusters provided for matching within OutClusterList */
	u16 cluster_list[2*MAX_REQUESTED_CLUSTER_NUMBER];
} zdo_match_descriptor_req_t;


/**
 * @brief the structure for device_announce command
 *
 * */
typedef struct{
	u16			nwk_addr_local;			/*!< NWK address for the Local Device */
	addrExt_t	ieee_addr_local;		/*!< IEEE address for the Local Device */
	capability_info_t	mac_capability;	/*!< Capability of the local device */
} zdo_device_annce_req_t;


/**
 * @brief the structure for parent_announce command
 *
 * */
typedef struct{
	u8		numberOfChildren;	/*!< The number of the children */
	u8		*childInfo;			/*!< the pointer to the IEEE address of the child bound to the parent */
} zdo_parent_annce_req_t;

/**
 * @brief the structure for power_descriptor_req command
 *
 * */
typedef		zdo_node_descriptor_req_t		zdo_power_descriptor_req_t;

/**
 * @brief the structure for active_ep_req command
 *
 * */
typedef		zdo_node_descriptor_req_t		zdo_active_ep_req_t;

/**
 * @brief the structure for complex_descriptor_req command
 *
 * */
typedef		zdo_node_descriptor_req_t		zdo_complex_descriptor_req_t;

/**
 * @brief the structure for user_descriptor_req command
 *
 * */
typedef		zdo_node_descriptor_req_t		zdo_user_descriptor_req_t;

/**************************************************************************
 * 	@brief		This is generated from a local device wishing to configure the
 * 	user descriptor on a remote device. This command shall be unicast either to
 * 	the remote device itself or to an alternative device that contains the discovery
 * 	information of the remote device.
*/

typedef struct{
	u8 len;//0~16
	u8 info[16];
}user_descriptor_t;

typedef struct{
	u16 nwk_addr_interest;
	user_descriptor_t user_desc;
} zdo_user_descriptor_set_req_t;

/**
 * @brief the structure for End_Device_Bind_req
 *
 * */
typedef struct{
	u16	binding_target_addr; /*!< The address of the target for the binding */

	u16	profile_id;			/*!< ProfileID which is to be matched
							   between two End_Device_Bind_req
							   received at the ZigBee Coordinator
							   within the timeout value pre-configured
							   in the ZigBee Coordinator.*/
	addrExt_t	src_ext_addr;  /*!< The IEEE address of the device generating the request */
	u16	in_cluster_lst[MAX_REQUESTED_CLUSTER_NUMBER]; /*!< List of Input ClusterIDs to be used for matching */
	u16	out_cluster_lst[MAX_REQUESTED_CLUSTER_NUMBER];/*!< List of Output ClusterIDs to be used for matching */

	u8	src_endpoint;	/*!< The endpoint on the device generating the request */
	u8	num_in_clusters; /*!< The number of Input Clusters provided for end device binding within the InClusterList */
	u8	num_out_clusters; /*!< The number of Output Clusters provided for matching within OutClusterList */
} zdo_edBindReq_user_t;


/**
 * @brief the structure for Bind_req
 *
 * */
typedef struct{
	u8	dstAddrMode;
	union{
		struct{
			addrExt_t	dstExtAddr;
			u8			dstEp;
		};
		u16				dstGroupId;
	}dstAddr;
}zdo_bind_dstAddr_t;

typedef struct{
	addrExt_t   src_addr; 		/*!< the device who build the binding table */
	u8			src_endpoint;	/*!< The source endpoint for the binding entry */
	u8			cid16_l;		/*!< The identifier of the cluster on the source device that is bound to the destination
	 	 	 	 	 	 	 	 low 8-bit */
	u8			cid16_h;		/*!< The identifier of the cluster on the source device that is bound to the destination
	 	 	 	 	 	 	 	 	 high 8-bit */
	u8			dst_addr_mode; /*!< destination address mode
									0x01 - 16-bit group address for dstAddr and dstEp not present
									0x03 - 64-bit extAddr for dstAddr and estEp present */
	union{
		struct{
			addrExt_t		dst_ext_addr;
			u8				dst_endpoint;
		};
		u16					dst_group_addr;
	};				/*!< The destination address for the binding entry */
}zdo_bind_req_t;


typedef zdo_bind_req_t zdo_bind_indication_t;

typedef zdo_bind_req_t zdo_unbind_req_t;

typedef zdo_bind_req_t zdo_bindTabListRec_t;

typedef zdo_unbind_req_t zdo_unbind_indication_t;


/**
 * @brief the structure for Mgmt_Bind_req
 *
 * */
typedef struct{
	u8	start_index;	/*!< Starting Index for the requested elements of the Binding Table */
} zdo_mgmt_bind_req_t;


/**
 * @brief the structure for Mgmt_Leave_req
 *
 * */
typedef struct{
	addrExt_t device_addr;		/*!< The 64-bit IEEE address of the entity to be re-moved from the network or NULL if the device removes itself from the network */
	struct {
		u8   reserved : 6;
		u8   remove_children : 1; /*!< This field has a value of 1 if the device being asked to leave the network is also being asked to remove its child devices,
									if any. Otherwise, it has a value of 0 */

		u8   rejoin : 1;		/*!< This field has a value of 1 if the device being asked to leave from the current parent is requested to rejoin the network.
									Otherwise, it has a value of 0 */
	}lr_bitfields;
} zdo_mgmt_leave_req_t;

/**
 * @brief the structure for Mgmt_Permit_Joining_req
 *
 * */
typedef struct{
	u8	permit_duration;			/*!< The length of time in seconds during which the ZigBee coordinator or router will allow associations.
							  	  	  	  The value 0x00 and 0xff indicate that per-mission is disabled or enabled, respectively, without a specified time limit */
	u8	trust_center_significance; 	/*!< This field shall always have a value of 1, indicating a request to change the Trust Center policy.
										  If a frame is received with a value of 0, it shall be treated as having a value of 1 */
} zdo_mgmt_permit_join_req_t;



/**
 * @brief the structure for Mgmt_NWK_Update_req
 *
 * */
typedef struct{
	u32	scan_ch;				/*!< scan channels, Bitmap */
	u8  scan_duration;			/*!< length of time to scan each channel. 0xfe ~ for channel change
									0xff ~ for apsChannelMask and nwkManagerAddr attributes change */
	union{
		u8     scan_cnt;		/*!<  the number of energy scans to be conducted and reported */
		u8     nwk_update_id; 	/*!< The value of the nwkUpdateId contained in this request.
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
#define	MAX_RSPNUM			6			/*!< supported maximum number of the associated device */
typedef struct{
	u8 			seq_num;  			/*! Sequence number which same with the request value */
	u8 			status;	  			/*! Response status */
	addrExt_t	ieee_addr_remote; /*!< 64-bit address for the Remote Device */
	u16			nwk_addr_remote;  /*!< 16-bit address for the Remote Device */
	u8     		num_assoc_dev; /*!< Count of the number of 16-bit short addresses to follow.
									If the RequestType in the request is Extended Response and there are no associated devices on the Remote Device,
									this field shall be set to 0.
									If an error occurs or the Request Type in the request is for a Single Device Response,
									this field shall not be included in the frame */
	u8     		start_index;	/*!< Starting index into the list of associated devices for this report.
									If the RequestType in the request is Extended Response and there are no associated devices on the Remote Device,
									this field shall not be included in the frame.
									If an error occurs or the Request Type in the request is for a Single Device Response,
									this field shall not be included in the frame */
	u16 		nwk_addr_assoc_dev_lst[MAX_RSPNUM]; /*!< A list of 16-bit addresses, one corresponding to each associated device to Remote Device;
															The number of 16-bit network addresses contained in this field is specified in the NumAssocDev field.
															If the RequestType in the request is Extended Response and there are no associated devices on the Remote Device,
															this field shall not be included in the frame.
															If an error occurs or the Request Type in the request is for a Single Device Response,
															this field shall not be included in the frame */
}zdo_nwk_addr_resp_t ;

/**
 * @brief the structure for IEEE_addr_rsp
 *
 * */
typedef zdo_nwk_addr_resp_t zdo_ieee_addr_resp_t;


typedef struct{
	u8 					seq_num;  			/*! Sequence number which same with the request value */
	u8 					status;	  			/*! Response status */
	u16      			nwk_addr_interest;
	node_descriptor_t 	node_descriptor;
} zdo_node_descript_resp_t;


typedef struct{
	u8 						seq_num;  			/*! Sequence number which same with the request value */
	u8 						status;	  			/*! Response status */
	u16       				nwk_addr_interest;  /*! NWK address of the power descriptor request */
	power_descriptor_t		power_descriptor;   /*! Power descriptor of the device */
} zdo_power_descriptor_resp_t;


typedef struct{
	u8		endpoint;				/*! end-point num of the simple descriptor 1 ~ 240 */
	u16		app_profile_id;			/*! APP profile ID specifies the profile which supported on this EP. */
	u16		app_dev_id;				/*! APP DEV ID specifies the device description supported on this EP. */
	u8		app_dev_ver:4;			/*! APP DEV version specifies the version of the device description supported */
	u8		reserved:4;				/*! Reserved */
	u8		listInfo[1];			/*! The cluster list supported on this EP */
} simple_descriptor_field_t;

typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u16		nwk_addr_interest;		/*! NWK address for the request.*/
	u8		length;					/*! Length in bytes of the Simple Descriptor to follow. */
	simple_descriptor_field_t	simple_descriptor; /*! Simple Descriptor */
} zdo_simple_descriptor_resp_t;

/**
 * @brief the structure for Active_EP_rsp
 *
 * */
typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u16		nwk_addr_interest; /*!< NWK address for the request */
	u8		active_ep_count;  /*!< The count of active endpoints on the Remote Device */
	u8		active_ep_lst[MAX_REQUESTED_CLUSTER_NUMBER]; /*!< List of bytes each of which represents an 8-bit endpoint */
} zdo_active_ep_resp_t;


typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u8	numberOfChildren;
	u8	*childInfo;
} zdo_parent_annce_rsp_t;

/**
 *  @brief Structure for Match_Desc_rsp
 */
typedef struct{
	u8 			seq_num;  			/*! Sequence number which same with the request value */
	u8 			status;	  			/*! Response status */
	u16 		nwk_addr_interest; 	/*!< NWK address for the request */
	u8 			matchLen; 			/*!< The count of endpoints on the Re-mote Device that match the request criteria */
	u8 			matchList[MAX_REQUESTED_CLUSTER_NUMBER]; /*!< List of bytes each of which represents an 8-bit endpoint */
} zdo_match_descriptor_resp_t;


/**
 * @brief the structure for Complex_Desc_rsp
 *
 * */
typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u16     nwk_addr_interest;		/*!< NWK address for the request */
	u8      length;          		/*!< Length in bytes of the ComplexDescriptor field */
	u8		compDesc[1];
} zdo_complex_descriptor_resp_t;

/**
 * @brief the structure for User_Desc_rsp
 *
 * */
typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u16     nwk_addr_interest;		/*!< NWK address for the request */
	u8      length;
	u8  	user_desc[1]; 			/*!< detailed user descriptor */
} zdo_user_descriptor_resp_t;


typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u16      nwk_addr_interest;
} zdo_user_descriptor_conf_resp_t;



/**
 * @brief the structure for System_Server_Discovery_rsp
 *
 * */
typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u16     server_mask; 			/*!< Mask of the server supported  */
} zdo_system_server_discovery_resp_t;


/* */
typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
}zdo_bind_resp_t;  //Rsp_user_t;

typedef zdo_bind_resp_t 	zdo_unbind_resp_t;

typedef zdo_bind_resp_t 	zdo_end_dev_bind_resp_t;

typedef zdo_bind_resp_t 	zdo_mgmt_permit_joining_resp_t;


#define 		ZDP_BINDING_TABLE_LIST_SIZE  				1
typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u8 		bind_tbl_entries;
	u8 		start_index;
	u8 		bind_tbl_lst_cnt;
	zdo_bindTabListRec_t bind_tbl_lst[ZDP_BINDING_TABLE_LIST_SIZE];
} zdo_mgmt_bind_resp_t;


#define		ZDO_MGMTLQIRSPHDR_LEN			3
#define		ZDO_MGMTLQIRSPTBLSIZE_MAX		2
typedef struct{
	addrExt_t	ext_pan_id;
	addrExt_t	ext_addr;
	u16			network_addr;
	u8  		deviceType : 2;
	u8			rxOnWhenIdle : 2;
	u8  		relationship : 3;
	u8  		reserved1 : 1;
	u8  		permitJoining : 2;
	u8  		reserved2 : 6;
	u8			depth;
	u8  		lqi;
} neighbor_tbl_lst_t;

typedef struct{
	u8 		seq_num;  				/*! Sequence number which same with the request value */
	u8 		status;	  				/*! Response status */
	u8   	neighbor_tbl_entries;
	u8   	start_index;
	u8   	neighbor_tbl_lst_cnt;
	neighbor_tbl_lst_t neighbor_tbl_lst[1];
} zdo_mgmt_lqi_resp_t;

/**************************************************************************
 * @brief 		The type used to pack information about network updates in an argument of zdo_mgmt_nwk_update_noti() function
 * 				The struct of this type comprises information about certain network updates. The type of the event is indicated
 * 				by the zdo_mgmt_nwk_update_noti_t::status field. Additional event data is provided by the fields of the
 * 				unnamed union.
*/
typedef struct{
	u8 		seq_num;  						/*! Sequence number which same with the request value */
	u8 		status;	  						/*! Response status */
	union{
		u16					conflict_addr;  /*! Conflicting address */
		ed_scan_t			scan_result;	/*! ED scan result */
		zdo_network_info_t	nwk_update_inf;	/*! Network information updated */
		child_info_t		childInfo; 		/*! Information about joined device or failed to authenticate child event */
		node_addr_t			child_addr;		/*! Addresses of removed device */
		u64					device_ext_addr;/*! Extended address of updated device */
	};
} zdo_mgmt_nwk_update_noti_t;


typedef struct{
	u8 		seq_num;  						/*! Sequence number which same with the request value */
	u8 		status;	  						/*! Response status */
} zdo_mgmt_leave_resp_t;

/******************************************************************************
 * @brief	This struct contains response to one of the ZDP requests
*/

#define				ZDO_ZDP_RSP_FRAME_HEADERSIZE				2//status + seq_num

typedef struct{
	union{
		zdo_power_descriptor_resp_t     	power_desc_resp;
		zdo_complex_descriptor_resp_t   	complex_desc_resp;
		zdo_user_descriptor_resp_t      	user_desc_resp;
		zdo_user_descriptor_conf_resp_t     user_desc_Conf_resp;
		zdo_system_server_discovery_resp_t  system_server_discovery_resp;
		zdo_mgmt_bind_resp_t              	mgmt_bind_resp;
		zdo_mgmt_nwk_update_noti_t         	mgmt_nwk_update_noti;
		zdo_mgmt_lqi_resp_t               	mgmt_lqi_resp;
		zdo_mgmt_permit_joining_resp_t		mgmt_permit_joining_resp;
		u8									payload;
	};
} zdo_zdp_resp_frame_t;

#define			ZDO_CMD_PAYLOAD_SMALL		8

#define			ZDO_CMD_PAYLOAD_MEDIUM		16

#define			ZDO_CMD_PAYLOAD_LARGE		32

#define			ZDO_CMD_PAYLOAD_MAX			52
typedef struct{
	u8 seq_num;
	union{
		zdo_nwk_addr_req_t					nwk_addr_req;
		zdo_ieee_addr_req_t              	ieee_addr_req;
		zdo_node_descriptor_req_t        	node_desc_req;
		zdo_power_descriptor_req_t          power_desc_req;
		zdo_simple_descriptor_req_t         simple_desc_req;
		zdo_device_annce_req_t           	device_annce;
		zdo_active_ep_req_t              	active_ep_req;
		zdo_complex_descriptor_req_t        complex_desc_req;
		zdo_user_descriptor_req_t           user_desc_req;
		zdo_user_descriptor_set_req_t       user_desc_set_req;
		zdo_system_server_discovery_req_t 	system_server_discovery_req;
		zdo_match_descriptor_req_t          match_desc_req;
		zdo_bind_req_t                  	bind_req;
		zdo_unbind_req_t                	unbind_req;
		zdo_mgmt_bind_req_t              	mgmt_bind_req;
		zdo_mgmt_leave_req_t             	mgmt_leave_req;
		zdo_mgmt_permit_join_req_t     		mgmt_permit_Join_req;
		zdo_mgmt_nwk_update_req_t         	mgmt_nwk_update_req;
		zdo_mgmt_lqi_req_t               	mgmt_lqi_req;
	};
} zdo_zdp_req_frame_t;

typedef void (*zdo_start_dveic_cb)(zdo_start_device_confirm_t *param);

/************************************************************************
 * @brief	zdo layer initiate function. This function reset Application layer attributes
 *
 * @param	none
 *
 * @return	none
 */
void zdo_init(void );

/**********************************************************************************************
 * @brief	This function would start the device, make it act as the ED, router or coordinator.
  			If act as the ED it would start the NWK discovery procedure, if act as coordinator it would
  			start the network formation procedure.Performs network formation for coordinator and network join for router or end device.
  			This function makes a request for starting a network. Before calling this function a device
  			cannot participate in network interactions with other nodes. If node type is the coordinator,
  			a new network is created,  while nodes of the router or the end device type search for and
  			join an existing network. Before calling the function make sure that certain node and network parameters
  			affecting the result of a network start are set correctly. Node parameters include extended address,
  			short address if static addressing scheme is used, and device type. Network parameters are comprised of
  			channel mask, channel page, extended PANID, and maximum network depth. Special attention should be given to
  			security options. Note that for a coordinator extended PANID must be always set to a nonzero value. In the
  			case of a router or an end device it may equal 0, which will cause joining to the first suitable network,
  			while a nonzero value makes it possible to join only a network which PANID equal the specified value. The
  			argument's zdo_start_nwk_conf field must contain a pointer to a callback function, typically named zdo_start_nwk_conf().
  			Upon successful request completion ZDO_SUCCESS is returned. Successful netwrok join will also cause a notification of
  			obtained parent node via zdo_mgmt_nwk_update_noti() function reporting ZDO_CHILD_JOINED.

 * @param	none
 *
 * @return	ZDO_SUCCESS: device start success
 */
zdo_status_t zdo_dvice_start(void );

/**********************************************************************************************************
 * @brief	Device reset request interface called by up layers
 *
 * @param	warm_start: TRUE or False
 *
 * @return	none
 */
void zdo_reset_req(bool warm_start);


/**********************************************************************************************
 * @brief	Adds ZDO layer call back function to the list. Which used when device send a ZDP request command that
 * 			requires a response.
 *
 * @param	zdo_callback - call back function
 * 			sn - transaction sequence number
 * 			cnt - call back execute times
 * @return	none
 */
zdo_status_t zdo_cb_add(zdo_callback cb,u16 sn, u8 cnt);


/******************************************************************************************
 * @brief	External interface used for enable permit join feature
 *
 * @param	permit_join: TRUE/FALSE, enable/disable permit join feature
 * 			permit_join_cnf_cb: setting result call back function
 *
 * @return	none
 */
void zdo_user_mgmt_permit_join(u8 permit_join);


/******************************************************************************************
 * @brief	External interface used for enable permit join feature
 *
 * @param	permit_join: TRUE/FALSE, enable/disable permit join feature
 * 			permit_join_cnf_cb: setting result call back function
 *
 * @return	none
 */
void zdo_nlmeChannelShift(u8 ch);

/******************************************************************************************
 * @brief	start network discovery
 *
 * @param	nlme_nwkDisc_req_t: the NWK-Discovery.req primitive
 *
 * @return	none
 */
void zdo_nwk_discovery_Start(nlme_nwkDisc_req_t *pReq, nwkDiscoveryUserCb_t cb);


void zdo_zdpCbTblRegister(zdo_appIndCb_t *cbTbl);

void zdo_touchLinkCbRegister(zdo_touchLinkCb_t *cb);

/************************************************************************************
 * @brief	Internal used interface to translate the channel map to the logic channel num
 *
 * @param	chp: channel map
 *
 * @return	logic channel num
 */
u8 zdo_channel_page2num(u32 chp);


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

/****************************************************************************************************************************
 * @brief	Interface called when need to join or rejoin a network, e.g. secure fail, device start fail.. This interface would construct
 * 			a nlme_join_req_t info to call nwk_nlme_join_req function to start the join procedure
 *
 * @param	chm: channel map used to rejoin, if join this parameter should be 0
 *
 * @return	TRUE/FALSE
 */

zdo_status_t zdo_nwk_rejoin_req(rejoinNwk_method_t method, u32 chm);


void zdo_devAnnce(u16 nwkAddr, addrExt_t ieeeAddr, u8 capabilities);

void zdo_af_set_link_retry_threshold(u8 threshold);
void zdo_af_set_rejoin_interval(u16 interval);
void zdo_af_set_max_rejoin_interval(u16 interval);
void zdo_af_set_scan_attempts(u8 attempts);

void zdo_nlmeForgetDev(addrExt_t nodeIeeeAddr, bool rejoin);
#endif
