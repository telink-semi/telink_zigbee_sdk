/********************************************************************************************************
 * @file     zcl.h
 *
 * @brief
 *
 * @author
 * @date     Dec. 1, 2016
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
#include "../zbapi/zb_api.h"
#include "../zdo/zb_af.h"
#include "zcl_config.h"


/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  ZCL_Module ZCL
 *  @{
 */
 
/** @addtogroup  ZCL_Constant ZCL Constants
 *  @{
 */
//Global Cluster Revision (0x0001 - 0xFFFE)
#define ZCL_ATTR_GLOBAL_CLUSTER_REVISION_DEFAULT	    0x0001

/*** Foundation Command IDs ***/
/** @addtogroup zcl_foundationCmd_id ZCL Foundation Command Ids
 * Idenitfy of ZCL Foundation Commands
 * @{
 */
#define ZCL_CMD_READ                                    0x00  
#define ZCL_CMD_READ_RSP                                0x01
#define ZCL_CMD_WRITE                                   0x02
#define ZCL_CMD_WRITE_UNDIVIDED                         0x03
#define ZCL_CMD_WRITE_RSP                               0x04
#define ZCL_CMD_WRITE_NO_RSP                            0x05
#define ZCL_CMD_CONFIG_REPORT                           0x06
#define ZCL_CMD_CONFIG_REPORT_RSP                       0x07
#define ZCL_CMD_READ_REPORT_CFG                         0x08
#define ZCL_CMD_READ_REPORT_CFG_RSP                     0x09
#define ZCL_CMD_REPORT                                  0x0a
#define ZCL_CMD_DEFAULT_RSP                             0x0b
#define ZCL_CMD_DISCOVER_ATTR                           0x0c
#define ZCL_CMD_DISCOVER_ATTR_RSP                       0x0d
#define ZCL_CMD_READ_STRUCTURED                         0x0e
#define ZCL_CMD_WRITE_STRUCTURED                        0x0f
#define ZCL_CMD_WRITE_STRUCTURED_RSP                    0x10
#define ZCL_CMD_DISCOVER_CMDS_RCVD                      0x11
#define ZCL_CMD_DISCOVER_CMDS_RCVD_RSP                  0x12
#define ZCL_CMD_DISCOVER_CMDS_GEN                       0x13
#define ZCL_CMD_DISCOVER_CMDS_GEN_RSP                   0x14
#define ZCL_CMD_DISCOVER_ATTR_EXTD                      0x15
#define ZCL_CMD_DISCOVER_ATTR_EXTD_RSP                  0x16
#define ZCL_CMD_MAX                                     ZCL_CMD_DISCOVER_ATTR_EXTD_RSP
/** @} end of group zcl_foundationCmd_id */

/** @addtogroup zcl_frameCtrl_bitmask ZCL Header Frame Control
 * Frame control field bitmask in ZCL header
 * @{
 */
#define ZCL_FRAME_CONTROL_TYPE                          0x03
#define ZCL_FRAME_CONTROL_MANU_SPECIFIC                 0x04
#define ZCL_FRAME_CONTROL_DIRECTION                     0x08
#define ZCL_FRAME_CONTROL_DISABLE_DEFAULT_RSP           0x10
/** @} end of group zcl_frameCtrl_bitmask */


/** @addtogroup zcl_frame_type ZCL Frame Types
 * Frame type definition for ZCL
 * @{
 */
#define ZCL_FRAME_TYPE_PROFILE_CMD                      0x00
#define ZCL_FRAME_TYPE_SPECIFIC_CMD                     0x01
/** @} end of group zcl_frame_type */

/** @addtogroup zcl_frame_durections ZCL Frame Directions
 * Client/Server directions for ZCL frame
 * @{
 */
#define ZCL_FRAME_CLIENT_SERVER_DIR                     0x00
#define ZCL_FRAME_SERVER_CLIENT_DIR                     0x01
/** @} end of group zcl_frame_durections */



/** @addtogroup zcl_txoption_radius ZCL DFLT TxOption Radius
 * ZCL Default txoptions and radius definitions
 * @{
 */
#define DFLT_APS_TX_OPTIONS                             0x00
#define DFLT_APS_RADIUS                                 0xf
/** @} end of group zcl_txoption_radius */

/** @} end of group ZCL_Constant */


/* To avoid sending default response to a broadcast or groupcast ZCL message.*/
#define UNICAST_MSG(msg)      ( ((msg)->indInfo.dst_addr < NWK_BROADCAST_ROUTER_COORDINATOR) && (((msg)->indInfo.dst_addr_mode) != APS_SHORT_GROUPADDR_NOEP) )

/** @addtogroup  ZCL_Types ZCL Types
 *  @{
 */

/**
 *  @brief  Definition for ZCL Attribute Information Entry
 */
typedef struct zclAttrInfo {
    u16 id;                     //!< Attribute ID
    u8  type;                   //!< Attribute type
    u8  access;                 //!< Attribute access control
    u8  *data;
} zclAttrInfo_t;

/**
 *  @brief  Definition for ZCL Frame control format
 */
typedef union zclFrmCtrl {
	struct {
		u8 type:2;              //!< ZCL Frame type
		u8 manufSpec:1;         //!< Manufacturer specific frame
		u8 dir:1;               //!< Direction. 0:client->server, 1:server->client
		u8 disDefResp:1;        //!< Disable default response
		u8 reserved:3;          //!< reserved
	} bf;                       //!< bit access field
	u8 byte;                    //!< byte access field
} zclFrmCtrl_t;

/**
 *  @brief  Definition for ZCL header format
 */
typedef struct zclHdr {
	zclFrmCtrl_t frmCtrl;       //!< Frame control field structure
	u16 manufCode;              //!< Manufacturer code
	u8 seqNum;                  //!< Sequence number - used to identify response frame
	u8 cmd;                     //!< Command ID
} zclHdr_t;

/**
 *  @brief  Definition for ZCL Read Attribute comamnd format
 */
typedef struct
{
	u8  numAttr;                //!< number of attributes in the list
	u16 attrID[];               //!< supported attributes list
} zclReadCmd_t;

/**
 *  @brief  Definition for ZCL Read response status command format
 */
typedef struct
{
	u16 attrID;                 //!< attribute ID
	u8  status;                 //!< should be ZCL_STATUS_SUCCESS or error
	u8  dataType;               //!< attribute data type
	u8  *data;                  //!< this structure is allocated, so the data is HERE
	                            // - the size depends on the attribute data type
} zclReadRspStatus_t;

/**
 *  @brief  Definition for ZCL Read response command format
 */
typedef struct
{
	u8 numAttr;                 	//!< number of attributes in the list
	zclReadRspStatus_t attrList[];  //!< attribute status list
} zclReadRspCmd_t;

/**
 *  @brief  Definition for ZCL write attribute record format
 */
typedef struct
{
	u16 attrID;                 //!< attribute ID
	u8  dataType;               //!< attribute data type
	u8  *attrData;              //!< this structure is allocated
} zclWriteRec_t;

/**
 *  @brief  Definition for ZCL write command format
 */
typedef struct
{
	u8 numAttr;                 //!< number of attribute records in the list
	zclWriteRec_t attrList[];  	//!< attribute records
} zclWriteCmd_t;

/**
 *  @brief  Definition for ZCL write attribute status format
 */
typedef struct
{
	u8  status;                //!< should be ZCL_STATUS_SUCCESS or error
	u16 attrID;                //!< attribute ID
} zclWriteRspStatus_t;

/**
 *  @brief  Definition for ZCL write attribute response command format
 */
typedef struct
{
	u8 numAttr;                		 //!< number of attribute status in the list
	zclWriteRspStatus_t attrList[];  //!< attribute status records
} zclWriteRspCmd_t;

/**
 *  @brief  Definition for ZCL Configure Report record format
 */
typedef struct
{
	u8  direction;             //!< to send or receive reports of the attribute
	u16 attrID;                //!< attribute ID
	u8  dataType;              //!< attribute data type
	u16 minReportInt;          //!< minimum reporting interval
	u16 maxReportInt;          //!< maximum reporting interval
	u16 timeoutPeriod;         //!< timeout period
	u8  *reportableChange;     //!< reportable change (only applicable to analog data type)
} zclCfgReportRec_t;

/**
 *  @brief  Definition for ZCL configure report command format
 */
typedef struct
{
	u8 numAttr;                    //!< number of attribute IDs in the list
	zclCfgReportRec_t attrList[];  //!< attribute ID list
} zclCfgReportCmd_t;

/**
 *  @brief  Definition for ZCL configure report status format
 */
typedef struct
{
	u8  status;                //!< should be ZCL_STATUS_SUCCESS or error
	u8  direction;             //!< whether attributes are reported or reports of attributes are received
	u16 attrID;                //!< attribute ID
} zclCfgReportStatus_t;

/**
 *  @brief  Definition for ZCL configure report response command format
 */
typedef struct
{
	u8 numAttr;                			//!< number of attribute status in the list
	zclCfgReportStatus_t attrList[]; 	//!< attribute status records
} zclCfgReportRspCmd_t;

/**
 *  @brief  Definition for ZCL read reporting configuration command format
 */
typedef struct
{
	u8  direction;             //!< to send or receive reports of the attribute
	u16 attrID;                //!< attribute ID
} zclReadReportCfgRec_t;

/**
 *  @brief  Definition for ZCL read report configuration comand format
 */
typedef struct
{
	u8 numAttr;                			//!< number of attributes in the list
	zclReadReportCfgRec_t attrList[]; 	//!< attribute ID list
} zclReadReportCfgCmd_t;

/**
 *  @brief  Definition for ZCL attribute reporting configuration record format
 */
typedef struct
{
	u8  status;                //!< status field
	u8  direction;             //!< to send or receive reports of the attribute
	u16 attrID;                //!< attribute ID
	u8  dataType;              //!< attribute data type
	u16 minReportInt;          //!< minimum reporting interval
	u16 maxReportInt;          //!< maximum reporting interval
	u16 timeoutPeriod;         //!< timeout period
	u8  *reportableChange;     //!< reportable change (only applicable to analog data type)
} zclReportCfgRspRec_t;

/**
 *  @brief  Definition for ZCL read reporting configuration response command format
 */
typedef struct
{
	u8 numAttr;                			//!< number of records in the list
	zclReportCfgRspRec_t attrList[]; 	//!< attribute reporting configuration list
} zclReadReportCfgRspCmd_t;

/**
 *  @brief  Definition for ZCL attribute report command format
 */
typedef struct
{
	u16 attrID;                //!< atrribute ID
	u8  dataType;              //!< attribute data type
	u8  *attrData;             //!< this structure is allocated, so the data is HERE
} zclReport_t;

/**
 *  @brief  Definition for ZCL report command format
 */
typedef struct
{
	u8 numAttr;                //!< number of reports in the list
	zclReport_t attrList[];    //!< attribute report list
} zclReportCmd_t;

/**
 *  @brief  Definition for ZCL default response command format
 */
typedef struct
{
	u8  commandID;
	u8  statusCode;
} zclDefaultRspCmd_t;

/**
 *  @brief  Definition for ZCL discover attribute command format
 */
typedef struct
{
	u16 startAttr;             //!< specifies the minimum value of the identifier to begin attribute discovery.
	u8  maxAttrIDs;            //!< maximum number of attribute IDs required
} zclDiscoverAttrCmd_t;

/**
 *  @brief  Definition for ZCL discover attribute information format
 */
typedef struct
{
	u16 attrID;                //!< attribute ID
	u8  dataType;              //!< attribute data type (see Table 17 in Spec)
} zclDiscoverAttrInfo_t;

// Discover Attributes Response Command format
/**
 *  @brief  Definition for ZCL discover attribute response command format
 */
typedef struct
{
	u8 discComplete;           			//!< whether or not there're more attributes to be discovered
	u8 numAttr;                			//!< number of attributes in the list
	zclDiscoverAttrInfo_t attrList[];   //!< supported attributes list
} zclDiscoverAttrRspCmd_t;

/**
 *  @brief  Definition for ZCL string data type
 */
typedef struct
{
	u8 *pStr;
	u8 strLen;
} UTF8String_t;

/**
 *  @brief  Definition for ZCL discover commands information
 */
typedef struct
{
	u8 startCmdID;
	u8 maxCmdID;
} zclDiscoverCmdsCmd_t;

/**
 *  @brief  Definition for ZCL discover commands response format
 */
typedef struct
{
	u8 discComplete;
	u8 cmdType;
	u8 numCmd;
	u8 *pCmdID;
} zclDiscoverCmdsRspCmd_t;

/**
 *  @brief  Definition for ZCL extended attributes information
 */
typedef struct
{
	u16 attrID;                //!< attribute ID
	u8	dataType;              //!< attribute data type
	u8	accessControl;		   //!< attribute access control
} zclExtAttrInfo_t;

/**
 *  @brief  Definition for ZCL discover extended attributes response format
 */
typedef struct
{
	u8 discComplete;           //!< whether or not there're more attributes to be discovered
	u8 numAttr;                //!< number of attributes in the list
	zclExtAttrInfo_t extAttrInfo[];	//!< supported extended attributes list
} zclDiscoverAttrExtRspCmd_t;


#define REPORTABLE_CHANGE_MAX_ANALOG_SIZE	8

/**
 *  @brief  If minInterval is 0, then there is no minimum limit;
 *  		if maxInterval is 0xffff, then the configuration info for that attribute need not be maintained;
 *  		if minInterval is 0xffff and maxInterval is 0, than back to default reporting configuration, reportable change field set to 0.
 */
typedef struct
{
	u16	profileID;
	u16 clusterID;
	u16 attrID;
	u16 minIntDft;
	u16 maxIntDft;
	u16 minInterval;//seconds
	u16 maxInterval;//seconds
	u16	minIntCnt;
	u16 maxIntCnt;
	u8  endPoint;
	u8	dataType;
	u8	reportableChange[REPORTABLE_CHANGE_MAX_ANALOG_SIZE];
	u8	prevData[REPORTABLE_CHANGE_MAX_ANALOG_SIZE];
	u8	used;
} reportCfgInfo_t;

/**
 *  @brief  Definition for ZCL reporting table
 */
typedef struct {
	u8 reportNum;
	reportCfgInfo_t	reportCfgInfo[ZCL_REPORTING_TABLE_NUM];
} zcl_reportingTab_t;

extern zcl_reportingTab_t reportingTab;

/**
 *  @brief  Definition for the incoming cluster command address information
 */
typedef struct {
	u16 profileId;
	u16 srcAddr;
	u16 dstAddr;
	u8  srcEp;
	u8	dstEp;
	u8	dirCluster;
} zclIncomingAddrInfo_t;

/**
 *  @brief  Definition for the application function pointer type to handle incoming cluster command.
 */
typedef status_t (*cluster_forAppCb_t)(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

/**
 *  @brief  Definition for incoming ZCL message format
 */
typedef struct
{
	cluster_forAppCb_t clusterAppCb;
	apsdeDataInd_t  *msg;      //!< incoming message
	zclHdr_t     	hdr;       //!< ZCL header parsed
	u8              *pData;    //!< pointer to data after header
	u16             dataLen;   //!< length of remaining data
	void            *attrCmd;  //!< pointer to the parsed attribute or command
} zclIncoming_t;

/**
 *  @brief  Definition for function pointer type to handle incoming message.
 */
typedef status_t (*cluster_cmdHdlr_t)(zclIncoming_t *pInHdlrMsg);


/**
 *  @brief  Definition for function pointer type to register a clutser.
 */
typedef status_t (*cluster_registerFunc_t)(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

/**
 *  @brief  Definition for ZCL Hook function format.
 */
typedef void (*zcl_hookFn_t)(zclIncoming_t *pInHdlrMsg);


/**
 *  @brief  Definition for ZCL layer registered cluster information, include attributes and command handler.
 */
typedef struct {
	const zclAttrInfo_t *attrTable;
	cluster_cmdHdlr_t 	cmdHandlerFunc;
	cluster_forAppCb_t 	clusterAppCb;
	u16 clusterID;
	u8 endpoint;
	u8 attrNum;
} clusterInfo_t;

typedef struct {
	u16 clusterId;
	u16 attrNum;
	const zclAttrInfo_t 	*attrTbl;
	cluster_registerFunc_t	clusterRegisterFunc;
	cluster_forAppCb_t 		clusterAppCb;
}zcl_specClusterInfo_t;


#ifndef ZCL_CLUSTER_NUM_MAX
#define ZCL_CLUSTER_NUM_MAX		8
#endif

typedef struct {
	ev_queue_t zb2zclQ;
	zcl_hookFn_t hookFn;
	u16 reserved;
	u8 curState;
	u8 clusterNum;
	clusterInfo_t clusterList[ZCL_CLUSTER_NUM_MAX];
} zcl_ctrl_t;


/**
 *  @brief  Definition for ZCL layer sequence number.
 */
extern u8 zcl_seqNum;
#define ZCL_SEQ_NUM		(zcl_seqNum++)

extern const u16 zcl_attr_global_clusterRevision;

/** @addtogroup  ZCL_Fountions ZCL APIs
 *  @{
 */


/**
 * @brief   Reset ZCL Module
 *
 * @param   None
 *
 * @return  None
 */
void zcl_reset(void);

/**
 * @brief      Initialize ZCL layer
 *
 * @param      fn   The hook function pointer
 *
 * @return     None
 */
void zcl_init(zcl_hookFn_t fn);


/**
 * @brief      register zc for application
 *
 * @param      endpoint   the endpoint
 *
 * @param      clusterNum the number of the cluster in this EP
 *
 * @param      info   	   the cluster info for this endpoint
 *
 * @return     None
 */
void zcl_register(u8 endpoint, u8 clusterNum, zcl_specClusterInfo_t *info);


/**
 * @brief      Find the cluster table by specified cluster ID and endpoint
 *
 * @param[in]  endpoint  - Specified endpoint
 * @param[in]  clusterId - Specified cluster ID
 *
 * @return     pointer
 */
clusterInfo_t *zcl_findCluster(u8 endpoint, u16 clusterId);

/**
 * @brief      Register the cluster to ZCL
 *
 * @param[in]  endpoint  - Specified endpoint
 * @param[in]  clusterId - Specified cluster ID
 * @param[in]  attrNum   - Specified attribute number in the cluster
 * @param[in]  pAttrTbl  - Specified attributes
 * @oaran[in]  cmdHdlrFn - Specified cluster commands handler function
 *
 * @return     ZCL Status @ref zcl_error_codes
 */
status_t zcl_registerCluster(u8 endpoint, u16 clusterId, u8 attrNum, const zclAttrInfo_t *pAttrTbl, cluster_cmdHdlr_t cmdHdlrFn, cluster_forAppCb_t cb);

/**
 * @brief       Used to send Profile and Cluster Specific Command messages.
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   clusterId - cluster ID
 * @param[in]   cmd - command ID
 * @param[in]   specific - whether the command is Cluster Specific
 * @param[in]   direction - client/server direction of the command
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   manuCode - manufacturer code for proprietary extensions to a profile
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   cmdPldLen - length of the command to be sent
 * @param[in]   cmdPld - command to be sent
 *
 * @return      ZCL Status @ref zcl_error_codes
 */
status_t zcl_sendCmd(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 cmd, u8 specific,
				 	 u8 direction, u8 disableDefaultRsp, u32 manuCode, u8 seqNo, u16 cmdPldLen, u8 *cmdPld);


status_t zcl_sendInterPANCmd(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 cmd, u8 specific,
							 u8 direction, u8 disableDefaultRsp, u32 manuCode, u8 seqNo, u16 cmdPldLen, u8 *cmdPld);

/**
 * @brief      Receive handler for data from APS/AF layer
 *
 * @param[in]  pData - Received data
 *
 * @return     None
 */
void zcl_rx_handler(void *pData);

/**
 * @brief      Check if analog data
 *
 * @param[in]  type - ZCL attribute date type
 *
 * @return     TRUE / FALSE
 */
bool zcl_analogDataType(u8 dataType);

/**
 * @brief      Get data length according to data type
 *
 * @param[in]  type - ZCL attribute date type
 *
 * @return     length
 */
u8 zcl_getDataTypeLen(u8 dataType);

/**
 * @brief      Get the length of ZCL attribute date
 *
 * @param[in]  type - ZCL attribute date type
 * @param[in]  pData - pointer to data
 *
 * @return     attribute length
 */
u16 zcl_getAttrSize(u8 dataType, u8 *pData);

/**
 * @brief      Get ZCL attribute value according to specified cluster and attribute ID 
 *
 * @param[in]  endpoint  - Specified endpoint
 * @param[in]  clusterId - Specified cluster ID
 * @param[in]  attrId    - Specified Attribute ID
 * @param[out] len       - Pointer to returned the length of the attribute
 * @param[out] val       - Pointer to returned attribute value
 *
 * @return     ZCL Status @ref zcl_error_codes
 */
status_t zcl_getAttrVal(u8 endpoint, u16 clusterId, u16 attrId, u16 *len, u8 *val);

/**
 * @brief      Set ZCL attribute value according to specified attribute entry
 *
 * @param[in]  endpoint  - Specified endpoint
 * @param[in]  clusterId - Specified cluster ID
 * @param[in]  attrId    - Specified Attribute ID
 * @param[in]  val       - value to write
 *
 * @return     ZCL Status @ref zcl_error_codes
 */
status_t zcl_setAttrVal(u8 endpoint, u16 clusterId, u16 attrId, u8 *val);

/**
 * @brief      Find the cluster table by specified cluster ID and endpoint
 *
 * @param[in]  endpoint  - Specified endpoint
 * @param[in]  clusterId - Specified cluster ID
 * @param[in]  attrId	 - Specified attribute ID
 *
 * @return     pointer
 */
zclAttrInfo_t *zcl_findAttribute(u8 endpoint, u16 clusterId, u16 attrId);





/******************************************************************************************************************************
 ************************************** ZCL Foundation command APIs ***********************************************************
 ******************************************************************************************************************************/
#ifdef ZCL_READ
/**
 * @brief       API to send ZCL read command
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   clusterId - cluster ID
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   direction - specified the command direction
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   readCmd - payload of zclRead command.
 *
 * @return      status_t
 */
status_t zcl_read(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadCmd_t *readCmd);
#define zcl_sendReadCmd(a,b,c,d,e,f)	(zcl_read((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_readWithMfgCode(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u32 manuCode, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadCmd_t *readCmd);
#define zcl_sendReadWithMfgCodeCmd(a,b,c,d,e,f,g)	(zcl_readWithMfgCode((a), (b), (c), (d), (e), (f), ZCL_SEQ_NUM, (g)))

#endif /* ZCL_READ */

#ifdef ZCL_WRITE
/**
 * @brief       API to send ZCL write command
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   clusterId - cluster ID
 * @param[in]   cmd - command ID
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   direction - specified the command direction
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   writeCmd - payload of zclWrite command.
 *
 * @return      status_t
 */
status_t zcl_writeReq(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 cmd, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclWriteCmd_t *writeCmd);
#define zcl_sendWriteCmd(a,b,c,d,e,f)			(zcl_writeReq((a), (b), (c), ZCL_CMD_WRITE, (d), (e), ZCL_SEQ_NUM, (f)))
#define zcl_sendWriteUndividedCmd(a,b,c,d,e,f)	(zcl_writeReq((a), (b), (c), ZCL_CMD_WRITE_UNDIVIDED, (d), (e), ZCL_SEQ_NUM, (f)))
#define zcl_sendWriteNoRspCmd(a,b,c,d,e,f)		(zcl_writeReq((a), (b), (c), ZCL_CMD_WRITE_NO_RSP, (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_writeWithMfgCode(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u32 manuCode, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclWriteCmd_t *writeCmd);
#define zcl_sendWriteWithMfgCodeCmd(a,b,c,d,e,f,g)	(zcl_writeWithMfgCode((a), (b), (c), (d), (e), (f), ZCL_SEQ_NUM, (g)))

#endif /* ZCL_WRITE */

#ifdef ZCL_REPORT
/**
 * @brief       API to send ZCL configure report command
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   clusterId - cluster ID
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   direction - specified the command direction
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   cfgReportCmd - payload of zclConfigReport command.
 *
 * @return      None
 */
status_t zcl_configReport(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclCfgReportCmd_t *cfgReportCmd);
#define zcl_sendCfgReportCmd(a,b,c,d,e,f)	(zcl_configReport((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_configReportRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclCfgReportRspCmd_t *cfgReportRspCmd);
#define zcl_sendCfgReportRspCmd(a,b,c,d,e,f)	(zcl_configReportRsp((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_readReportConfig(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadReportCfgCmd_t *readReportCfgCmd);
#define zcl_sendReadReportCfgCmd(a,b,c,d,e,f)	(zcl_readReportConfig((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_readReportConfigRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadReportCfgRspCmd_t *readReportCfgRspCmd);
#define zcl_sendReadReportCfgRspCmd(a,b,c,d,e,f)	(zcl_readReportConfigRsp((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_report(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 direction, u8 seqNo, u16 clusterId, u16 attrID, u8 dataType, u8 *pData);
#define zcl_sendReportCmd(a,b,c,d,e,f,g,h) 	(zcl_report((a), (b), (c), (d), ZCL_SEQ_NUM, (e), (f), (g), (h)))

//for internal
void zcl_reportingTabInit(void);
u8 zcl_reportingEntryActiveNumGet(void);
reportCfgInfo_t *zcl_reportCfgInfoEntryFind(u8 endpoint, u16 clusterId, u16 attrID);
reportCfgInfo_t *zcl_reportCfgInfoEntryFreeGet(void);
void zcl_reportCfgInfoEntryClear(reportCfgInfo_t *pEntry);
void zcl_reportCfgInfoEntryRst(reportCfgInfo_t *pEntry);
void zcl_reportCfgInfoEntryUpdate(reportCfgInfo_t *pEntry, u8 endPoint, u16 profileId, u16 clusterId, zclCfgReportRec_t *pCfgReportRec);
status_t zcl_configureReporting(u8 endpoint, u16 profileId, u16 clusterId, zclCfgReportRec_t *pCfgReportRec);

//for application
bool reportableChangeValueChk(u8 dataType, u8 *curValue, u8 *prevValue, u8 *reportableChange);
void reportNoMinLimit(void);
void reportAttrTimerStart(u16 seconds);
void reportAttrTimerStop(void);

#endif /* ZCL_REPORT */

#ifdef ZCL_DISCOVER
/**
 * @brief       API to send ZCL discover attribute command
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   clusterId - cluster ID
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   direction - specified the command direction
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   discAttrCmd - payload of zclDiscoverAttr command.
 *
 * @return      status_t
 */
status_t zcl_discAttrs(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclDiscoverAttrCmd_t *discAttrCmd);
#define zcl_sendDiscAttrsCmd(a,b,c,d,e,f)	(zcl_discAttrs((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_discAttrsRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclDiscoverAttrRspCmd_t *discAttrRspCmd);
#define zcl_sendDiscAttrsRspCmd(a,b,c,d,e,f)	(zcl_discAttrsRsp((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_discAttrsExtended(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclDiscoverAttrCmd_t *discAttrCmd);
#define zcl_sendDiscAttrsExtendedCmd(a,b,c,d,e,f)	(zcl_discAttrsExtended((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

status_t zcl_discAttrsExtendedRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclDiscoverAttrExtRspCmd_t *discAttrExtRspCmd);
#define zcl_sendDiscAttrsExtendedRspCmd(a,b,c,d,e,f)	(zcl_discAttrsExtendedRsp((a), (b), (c), (d), (e), ZCL_SEQ_NUM, (f)))

#endif /* ZCL_DISCOVER */

/** @} end of group ZCL_Fountions */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */


