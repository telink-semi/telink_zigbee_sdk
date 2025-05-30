/********************************************************************************************************
 * @file    aps_api.h
 *
 * @brief   This is the header file for aps_api
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
#ifndef APS_API_H
#define APS_API_H


#define APS_EP_NUM_IN_GROUP_TBL         8
#define APS_GROUP_NAME_LEN              16

#ifndef APS_BINDING_TABLE_NUM
#define APS_BINDING_TABLE_NUM           8 //APS_BINDING_TABLE_NUM_DFT
#endif

#ifndef APS_GROUP_TABLE_NUM
#define APS_GROUP_TABLE_NUM             4 //APS_GROUP_TABLE_NUM_DFT
#endif

#define	APS_BINDING_TABLE_NUM_V1        8 //old struct, mustn't change it

#define APS_TX_CACHE_TABLE_NUM          16


#define APS_PARENT_ANNOUNCE_BASE_TIMER  10//s
#define APS_PARENT_ANNOUNCE_JITTER_MAX  10//s

typedef enum {
    APS_STATUS_SUCCESS                  = 0x00,

    APS_STATUS_ASDU_TOO_LONG            = 0xa0,
    APS_STATUS_DEFRAG_DEFERRED          = 0xa1,
    APS_STATUS_DEFRAG_UNSUPPORTED       = 0xa2,
    APS_STATUS_ILLEGAL_REQUEST          = 0xa3,
    APS_STATUS_INVALID_BINDING          = 0xa4,
    APS_STATUS_INVALID_GROUP            = 0xa5,
    APS_STATUS_INVALID_PARAMETER        = 0xa6,
    APS_STATUS_NO_ACK                   = 0xa7,
    APS_STATUS_NO_BOUND_DEVICE          = 0xa8,
    APS_STATUS_NO_SHORT_ADDRESS         = 0xa9,
    APS_STATUS_NOT_SUPPORTED            = 0xaa,
    APS_STATUS_SECURED_LINK_KEY         = 0xab,
    APS_STATUS_SECURED_NWK_KEY          = 0xac,
    APS_STATUS_SECURITY_FAIL            = 0xad,
    APS_STATUS_TABLE_FULL               = 0xae,
    APS_STATUS_UNSECURED                = 0xaf,
    APS_STATUS_UNSUPPORTED_ATTRIBUTE    = 0xb0,
    APS_STATUS_HANDLE_BACKING           = 0xb1,
    APS_STATUS_SHORT_ADDR_REQUESTING    = 0xb2,
    APS_STATUS_DUPLICATE_ENTRY          = 0xb8,
    APS_STATUS_INTERNAL_BUF_FULL        = 0xb9
} aps_status_t;


/* The destination address for the binding entry.*/
typedef union {
    u16 dst_group_addr;
    struct {
        union {
            addrExt_t dst_ext_addr;
            u16 dst_short_addr;
        };
        /*
         * This field shall be present only if the dstAddrMode field has a value of
         * 0x03 and, if present, shall be the destination endpoint for the binding entry.
         */
        u8 dst_endpoint;
    };
} aps_address_t;

/*
 * The transmission options for the ASDU to be transferred.
 * These are a bitwise OR of one or more.
 */
typedef enum {
    APS_TX_OPT_SECURITY_ENABLED         = BIT(0), //0x01 = Security enabled transmission
    APS_TX_OPT_DISABLE_NWK_KEY          = BIT(1), //0x02 = Disable NWK key
    APS_TX_OPT_ACK_TX                   = BIT(2), //0x04 = Acknowledged transmission
    APS_TX_OPT_FRAG_PERMITTED           = BIT(3), //0x08 = Fragmentation permitted
    APS_TX_OPT_INCLUDE_NONCE            = BIT(4), //0x10 = Include extended nonce in APS security frame
    APS_TX_OPT_UNICAST_SKIP_ROUTING     = BIT(5), //0x20 = Skip routing when unicast
    APS_TX_OPT_INTRA_PAN                = BIT(7)  //0x80 = Intra-PAN delivery
} aps_tx_options;

typedef enum {
    APS_DSTADDR_EP_NOTPRESETNT          = 0x00, /* for bind, without address and Endpoint */
    APS_SHORT_GROUPADDR_NOEP,                   /* for group-casting: only need group address */
    APS_SHORT_DSTADDR_WITHEP,                   /* for unicasting with nwk address, with Endpoint */
    APS_LONG_DSTADDR_WITHEP,                    /* for unicasting with ieee address, with Endpoint */
} aps_dst_addr_mode;

typedef enum {
    APS_SHORT_SRCADDR_WITHEP            = 0x02, /* for unicasting with nwk address, with Endpoint */
    APS_LONG_SRCADDR_WITHEP,                    /* for unicasting with ieee address, with Endpoint */
    APS_LONG_SRCADDR_NOEP                       /* for unicasting with ieee address, without Endpoint */
} aps_src_addr_mode;

typedef enum {
    APS_CMD_HANDLE_TRANSPORT_KEY        = 0x40,
    APS_CMD_HANDLE_TXKEYCMD_RELAY,
    APS_CMD_HANDLE_UPDATE_DEVICE,
    APS_CMD_HANDLE_REMOVE_DEVICE,
    APS_CMD_HANDLE_REQUEST_KEY,
    APS_CMD_HANDLE_SWITCH_KEY,
    APS_CMD_HANDLE_VERIFY_KEY,
    APS_CMD_HANDLE_CONFIRM_KEY,
    APS_CMD_HANDLE_CMD_TUNNEL,
    APS_CMD_HANDLE_DEVICE_ANNOUNCE,
    APS_CMD_HANDLE_NWK_KEY,
    APS_CMD_HANDLE_ACK
} aps_handle_e;

/*
 * APS_DataReq_t apsdeData.h "aps.h"
 *
 * The structure for request parameters of APS_DataReq() function
 *
 * The structure represents parameters of data request, i.e. the request for sending data across the network. A pointer
 * to an instance of the structure should to be passed as an argument to APS_DataReq() function.
 * The structure definition follows APSDE-DATA request primitive described in
 * Zigbee Specification r18, 2.2.4.1.1 APSDE-DATA.request, page 23.
 */
typedef struct {
    /* The set of octets comprising the ASDU to be transferred. */
    u8 *asdu;
    /*
     * Endian "[LE]" The identifier of the profile for which
     * this frame is intended.
     */
    u16	profile_id;
    /*
     * Endian "[LE]" The identifier of the cluster for which
     * this frame is intended.
     */
    u16	cluster_id;
    /* The endpoint on the request originator node from
     * which the data frame is being transferred.
     */
    u8 src_endpoint;
    /*
     * The number of octets comprising the ASDU to be transferred.
     * The maximum length of an individual APS frame payload is given
     * as NsduLength-apscMinHeaderOverhead. Assuming the possibility
     * of fragmentation, a maximum-sized single ASDU consists of 256 such blocks.
     */
    u8 asdu_length;
    /*
     * The transmission options for the ASDU to be transferred.
     * See structure definition for details.
     */
    u8 tx_options;//aps_tx_options
    /*
     * The distance, in hops, that a transmitted frame will be allowed to
     * travel via the network
     */
    u8 radius;
    /*
     * The addressing mode for identifying the destination of a data request.
     * May take any non-reserved value from the following list:
     * APS_NO_ADDRESS (0x00) - used for binding; set dstAddress and dstEndpoint
     * APS_GROUP_ADDRESS (0x01) - used for group transmission; 16-bit group address should be specified in dstAddress; dstEndpoint is not set
     * APS_SHORT_ADDRESS (0x02) - identifying the destination (unicast or broadcast) with  a 16-bit short address specified
     * in dstAddress and the endpoint set in dstEndpoint
     * APS_EXT_ADDRESS (0x03) - identifying the destination with a 64-bit extended address specified in dstAddress and the endpoint set in dstEndpoint
     * 0x04..0xff - reserved values, must not be used by the application
     */
    u8 dst_addr_mode;//aps_dst_addr_mode
    bool enableNWKsecurity;
    u8 handle;
    u8 apsCnt;

    aps_address_t aps_addr;//9-bytes

    bool useAlias;
    u16 aliasSrcAddr;
    u8 aliasSeqNum;

    u8 extFrameCtrl;
    u8 blockNum;
} aps_data_req_t;

typedef enum {
    SECURITY_IN_APSLAYER        = BIT(0),
    SECURITY_IN_NWKLAYER        = BIT(1)
} security_with_e;

typedef struct {
    /*
     * The addressing mode used to identify the destination
     * in the data frame that has been received.
     * May take any non-reserved value from the following list:
     * APS_NO_ADDRESS (0x00) - used for binding; set dstAddress and dstEndpoint
     * APS_GROUP_ADDRESS (0x01) - used for group transmission; 16-bit group address should be specified in dstAddress; dstEndpoint is not set
     * APS_SHORT_ADDRESS (0x02) - identifying the destination (unicast or broadcast) with  a 16-bit short address specified
     * in dstAddress and the endpoint set in dstEndpoint
     * APS_EXT_ADDRESS (0x03) - identifying the destination with a 64-bit extended address specified in dstAddress and the endpoint set in dstEndpoint
     * 0x04..0xff - reserved values, must not be used by the application
     */
    u8 dst_addr_mode;
    /* The target endpoint on the local entity to which the ASDU is directed. */
    u8 dst_ep;
    /*
     * The individual device address or group addressto which
     * the ASDU is directed. Value size depends on the dstAddrMode parameter.
     */
    u16 dst_addr;
    /*
     * The addressing mode for the source address used in this primitive
     * and of the APDU that has been received. May possess any non-reserved value
     * from the following list:
     * APS_SHORT_ADDRESS (0x02) - short address is used in srcAddress and srcEndpoint is specified
     * APS_EXT_ADDRESS (0x03) - extended address is used in srcAddress and srcEndpoint is specified
     */
    u8 src_addr_mode;
    u8 src_ep;
    /* The identifier of the profile from which this frame originates. */
    u16	profile_id;
    /* The identifier of the received object. */
    u16	cluster_id;
    /* The number of octets comprising the ASDU being indicated by the APSDE. */
    u16	asduLength;
    u8 *asdu;
    /* Timestamp for the received packet based on the local clock, as provided by the NWK layer. */
    u32	rx_tick;
    /*
     * The individual device address of the entity from which the ASDU has been received.
     * Value type and size is depend of the srcAddrMode parameter.
     */
    union {
      u16 src_short_addr;
      u8 src_ext_addr[8];
    };

    u16	srcMacAddr;//MAC address indicate the device send this packet

    /* The status of the incoming frame processing. */
    u8 status;
    /*
     * APS_UNSECURED_STATUS = 0xaf if the ASDU was received without any security.
     * APS_SECURED_NWK_KEY_STATUS = 0xac if the received ASDU was secured with the NWK key,
     * APS_SECURED_LINK_KEY_STATUS = 0xab if the ASDU was secured with a link key,
     * APS_SECURED_TRUST_CENTER_LINK_KEY_STATUS = 0xb1 if the ASDU was secured with the trust center link key,
     * APS_SECURED_HASH_OF_TRUST_CENTER_LINK_KEY_STATUS = 0xb2 if secured with hash of the trust center link key.
     */
    u8 security_status;//security_with_e
    /* The link quality indication delivered by the NLDE. */
    u8 lqi;
    s8 rssi;
    u8 aps_counter;
} aps_data_ind_t;

//Structure for parameter of apsdeDataCnf callback function, for external usage
typedef struct {
    union {
    	u16 addr_short;
        addrExt_t addr_long;
    } dstAddr;

    u8 dstAddrMode;
    u8 dstEndpoint;
    u8 srcEndpoint;
    u8 status;

    u32 tx_time;

    u8 handle;
    u8 apsCnt;
    u16 clusterId;
} apsdeDataConf_t;

typedef struct _attribute_packed_ {
    addrExt_t ext_src_addr;

    u8 src_ep;
    u8 cid16_l;
    u8 cid16_h;
    u8 dst_addr_mode; //01 group; 03 ext
    /* The destination address for the binding entry.*/
    union _attribute_packed_ {
        struct {
            addrExt_t dst_ext_addr;
            /*
             * This field shall be present only if the dstAddrMode field has a value of
             * 0x03 and, if present, shall be the destination endpoint for the binding entry.
             */
            u8 dst_ep;
        };
        u16 dst_group_addr;
    };
} aps_me_bind_req_t;

typedef	aps_me_bind_req_t       aps_me_unbind_req_t;

//APSME-ADD-GROUP.request Parameters
typedef struct {
    u16 group_addr; //the 16 bit group address of the group being added
    u8 ep;          //the ep to which the given group is being added
} aps_add_group_req_t;

typedef aps_add_group_req_t     aps_delete_group_req_t;

//Group table entry
typedef struct _attribute_packed_ {
    u16 group_addr;
    u8 endpoints[APS_EP_NUM_IN_GROUP_TBL];
    u8 n_endpoints;
    u8 group_name[APS_GROUP_NAME_LEN];
    u8 rsv;
} aps_group_tbl_ent_t;

#define APS_BIND_DST_ADDR_GROUP 0
#define APS_BIND_DST_ADDR_LONG  1

typedef struct {
    aps_address_t aps_addr;
    u8 dst_addr_mode; //aps_dst_addr_mode
} bind_dst_list;

typedef struct {
    u8 *txData;
    u8 txCnt;
    u8 totalCnt;
    u8 resv[2];
    bind_dst_list list[APS_BINDING_TABLE_NUM];
} bind_dst_list_tbl;

//APS bind source table struct
typedef struct {
    u16 profile_id;     //profile id
    u16 device_id;      //profile id

    u16 cluster_id;     //cluster id
    u16 src_addr_ref;   //source address as ref from nwkAddressMap

    u8 src_ep;          //source endpoint
    u8 used;
    u16 dst_tbl_idx;
} aps_bind_src_tbl_t;

typedef struct {
    u16 dst_addr;       /* destination address as ref from nwkAddressMap */
    u8 dst_end;         /* destination endpoint */
    u8 reserved;
} aps_bind_long_dst_addr_t;

typedef struct {
    u8 dst_addr_mode;   //destination address mode flag,
                        //0 - group address, otherwise long address plus dst ep
    u8 used;
    u16 src_table_index;//index from zb_asp_src_table_t

    union {
        u16 group_addr; //group address
        aps_bind_long_dst_addr_t long_addr; //zb_asp_long_dst_addr_t
    };
} aps_bind_dst_table_t;

typedef struct {
    u8 srcTblIdx;
    u8 dstTblIdx;
} boundTblMapList_t;

typedef struct {
    aps_bind_src_tbl_t src_table;
    aps_bind_dst_table_t dst_table;
} aps_bind_tbl_t ;

typedef struct {
    u8 src_n_elements;	//number of elements in src tbl
    u8 dst_n_elements;
    u8 bound_cnt;
    u8 resv;
    aps_bind_tbl_t table[APS_BINDING_TABLE_NUM_V1];
    boundTblMapList_t BoudList[APS_BINDING_TABLE_NUM_V1 * APS_BINDING_TABLE_NUM_V1];
} aps_binding_table_t _attribute_aligned_(4);

typedef struct _attribute_packed_ {
    addrExt_t extAddr;
    u8 dstEp;
} aps_binding_extaddr_t;

typedef struct _attribute_packed_ {
    u16 clusterId;      /* cluster ID */
    u8 srcEp;           /* source endpoint */
    u8 dstAddrMode;     /* destination address mode */
    union _attribute_packed_ {
        u16 groupAddr;                          //group address
        aps_binding_extaddr_t dstExtAddrInfo;   //zb_asp_long_dst_addr_t
    };
    u8 used;
} aps_binding_entry_t;

typedef struct {
    union {
    	u16 addr_short;
        addrExt_t addr_long;
    } dstAddr;

    u8 *payload;

    u8 dstAddrMode;
    u8 dstEndpoint;
    u8 ep;              //srcEp
    u8 handler;         //real data handler

    u8 apsCount;        //real data aps count
    u8 zdpSeqnoAddrReq; //the aps count for nwk address req
    u8 used:1;
    u8 addrReqNeed:1;
    u8 ackNeed:1;
    u8 interPAN:1;
    u8 state:4;
    u8 retries;

    s8 apsAckWaitTimeOut;
    s8 apsAddrWaitTimeout;
    u8 extFrameCtrl;
    u8 blockNum;
    u16 clusterId;
    u8 resv[2];
} aps_tx_cache_list_t;

typedef struct {
    u32 aps_channel_mask;               //The mask of allowable channels for this device to use for network operations.
    addrExt_t aps_use_ext_panid;        //The 64-bit address of a network to form or to join.

    bool aps_designated_coordinator;    //TRUE if the device should become the ZigBee Coordinator on startup, FALSE if otherwise.
    u8 aps_parent_announce_timer;
    u8 aps_nonmember_radius;            //The value to be used for the NonmemberRadius parameter when using NWK layer multicast. Default value 2.
    u8 aps_interframe_delay;
    u8 aps_max_window_size;
    u8 aps_fragment_payload_size;

    u8 aps_use_insecure_join:1;         //A flag controlling the use of insecure join at startup. Default TRUE
    u8 aps_authenticated:1;             //authenticated or not
    u8 aps_updateDevice_holdApsSecurity:1; //if set, will not send update device with aps encryption
    u8 aps_zdo_restricted_mode:1;       //Is the configuration mode restricted
    u8 aps_reserved:4;
} aps_pib_attributes_t;

typedef void (*apsDataIndCb_t)(void *p);

extern u16 GROUP_MESSAGE_SEND_ADDRESS;
extern u8 APS_INTERFRAME_DELAY;
extern u8 APS_MAX_WINDOW_SIZE;
extern u8 APS_FRAGMEMT_PAYLOAD_SIZE;
extern u8 APS_BINDING_TABLE_SIZE;
extern u8 APS_GROUP_TABLE_SIZE;
extern u8 APS_TX_CACHE_TABLE_SIZE;
extern u8 APS_MAX_FRAME_RETRIES;
extern u8 APS_ACK_EXPIRY;
extern aps_binding_entry_t g_apsBindingTbl[];
extern aps_group_tbl_ent_t aps_group_tbl[];
extern aps_tx_cache_list_t aps_txCache_tbl[];
extern aps_pib_attributes_t aps_ib;

#define APS_IB() aps_ib

#define ZB_SET_UPDATE_DEVICE_APS_SEC_HOLD(v)    (aps_ib.aps_updateDevice_holdApsSecurity = v)


/***********************************************************************//**
 * @brief   Set channel mask of APSIB.
 *
 * @param	chnMask
 *
 * @return	aps_status_t
 *
 **************************************************************************/
aps_status_t apsSetChnMsk(u32 chnMask);

/************************************************************************//**
 * @brief 	Initialization internal state and main variables of APS-layer.
 *   	 	It must be called before any operation with APS.
 *
 * @return 	None.
 *
 **************************************************************************/
void aps_init(void);

/***********************************************************************//**
 * @brief   get the sequence number for the current aps frame
 *
 * @param
 *
 * @return	the sequence number
 *
 **************************************************************************/
u8 aps_get_current_counter_value(void);

/***********************************************************************//**
 * @brief   get the sequence number for the new aps frame
 *
 * @param
 *
 * @return	the sequence number
 *
 **************************************************************************/
u8 aps_get_counter_value(void);


aps_status_t aps_me_bind_req(aps_me_bind_req_t *amr);

aps_status_t aps_me_unbind_req(aps_me_unbind_req_t *amr);

aps_status_t aps_search_dst_from_bind_tbl(aps_data_req_t *apsreq, bind_dst_list_tbl *bindList);

/***********************************************************************//**
 * @brief   get the entry of the binding table
 *
 * @param	None
 *
 * @return	the point to the binding table
 *
 **************************************************************************/
aps_binding_entry_t *aps_bindingTblEntryGet(void);

/***********************************************************************//**
 * @brief   get binding table number
 *
 * @param
 *
 * @return	the number of the valid binding table
 *
 **************************************************************************/
u8 aps_bindingTblEntryNum(void);

/***********************************************************************//**
 * @brief   delete bind information searched by destination address
 *
 * @param dst_addr_ref:idx in address map
 *
 * @return
 *
 **************************************************************************/
void aps_bindingTblEntryDelByDstExtAddr(addrExt_t extAddr);

/***********************************************************************//**
 * @brief   restore the old format binding table for the sdk version earlier than 3.6.7.5
 *
 * @param   oldTbl, the temperate buffer used to restore the old binding table
 *
 * @param   tblSize, the size of the total old binding table
 *
 * @param   eleNum,  the number of old binding table number
 * @return
 *
 **************************************************************************/
void aps_userBindingTblRestore(void *oldTbl, u16 tblSize, u16 eleNum);

/***********************************************************************//**
 * @brief   clear all binding table information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
void aps_bindingTab_clear(void);

/***********************************************************************//**
 * @brief   group table initialization(restore th table from NV)
 *
 * @param	none
 *
 * @return	none
 *
 **************************************************************************/
u8 aps_groupTblNvInit(void);

/***********************************************************************//**
 * @brief   clear group table
 *
 * @param	none
 *
 * @return	none
 *
 **************************************************************************/
void aps_groupTblReset(void);

/***********************************************************************//**
 * @brief   send a group add request command
 *
 * @param	req:  pointer to group add request command
 *
 * @return	the group table entry
 *
 **************************************************************************/
aps_status_t aps_me_group_add_req(aps_add_group_req_t *req);

/***********************************************************************//**
 * @brief   search a group in group table
 *
 * @param	groupAddr:  group identifier
 *
 * @param	endpoint
 *
 * @return	the group table entry
 *
 **************************************************************************/
aps_group_tbl_ent_t *aps_group_search(u16 groupAddr, u8 endpoint);

/***********************************************************************//**
 * @brief   get all of the group in the group table
 *
 * @param	counter:  total group count
 *
 * @param	group_list: group list
 *
 * @return	None
 *
 **************************************************************************/
void aps_group_list_get(u8 *counter, u16 *group_list);

/***********************************************************************//**
 * @brief   send a group delete request command
 *
 * @param	req:  pointer to group delete request command
 *
 * @return	None
 *
 **************************************************************************/
aps_status_t aps_me_group_delete_req(aps_delete_group_req_t *req);

/***********************************************************************//**
 * @brief   send a group delete all request command
 *
 * @param	ep:  the end point which groups will be deleted
 *
 * @return	None
 *
 **************************************************************************/
aps_status_t aps_me_group_delete_all_req(u8 ep);

/***********************************************************************//**
 * @brief   get valid group count in the group table
 *
 * @param	None
 *
 * @return	valid group count in the group table
 *
 **************************************************************************/
u8 aps_group_entry_num_get(void);

/***********************************************************************//**
 * @brief   set global group count according to the group table
 *
 * @param	None
 *
 * @return	none
 *
 **************************************************************************/
void aps_init_group_num_set(void);

/***********************************************************************//**
 * @brief   get the group entry depend on the group_addr
 *
 * @param	group_addr the group address
 *
 * @return	 the point to the group table entry
 *
 **************************************************************************/
aps_group_tbl_ent_t *aps_group_search_by_addr(u16 group_addr);

/***********************************************************************//**
 * @brief   get the endpoint information according to the group address
 *
 * @param	group_addr the group address
 *
 * @epNum   endpoint number in this group address
 *
 * @return	the list to the endpoint information
 *
 **************************************************************************/
u8 *aps_group_ep_info_get(u16 group_addr, u8 *epNum);

u8 aps_duplicate_check(u16 src_addr, u8 aps_counter);

void apsCleanToStopSecondClock(void);

void tl_apsDataIndRegister(apsDataIndCb_t cb);

/***********************************************************************//**
 * @brief       Send an APS data request
 *
 * @param[in]   dataReq - Pointer to the apsde data request structure
 *
 * @param[in]   asdu    - Pointer to the asdu
 *
 * @param[in]   length  - Length of asdu
 *
 * @return      Status
 *
 **************************************************************************/
u8 apsDataRequest(aps_data_req_t *dataReq, u8 *asdu, u8 length);
u8 apsDataFragmentRequest(aps_data_req_t *dataReq, u8 *asdu, u16 length);


#endif	/* APS_API_H */
