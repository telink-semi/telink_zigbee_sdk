/********************************************************************************************************
 * @file     zcl.c
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


/**********************************************************************
 * INCLUDES
 */
#include "zcl_include.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */



/**********************************************************************
 * LOCAL MACROS
 */



/**********************************************************************
 * LOCAL TYPES
 */



/**********************************************************************
 * LOCAL VARIABLES
 */
zcl_ctrl_t zcl_vars;
u8 zcl_seqNum;//ZCL seqNum

const u16 zcl_attr_global_clusterRevision = ZCL_ATTR_GLOBAL_CLUSTER_REVISION_DEFAULT;

/**********************************************************************
 * LOCAL FUNCTIONS
 */
#ifdef ZCL_READ
u8 zcl_readHandler(zclIncoming_t* pCmd);
u8 zcl_readRspHandler(zclIncoming_t* pCmd);
#endif
#ifdef ZCL_WRITE
u8 zcl_writeHandler(zclIncoming_t* pCmd);
u8 zcl_writeUndividedHandler(zclIncoming_t* pCmd);
u8 zcl_writeRspHandler(zclIncoming_t* pCmd);
#endif
#ifdef ZCL_REPORT
u8 zcl_configReportHandler(zclIncoming_t* pCmd);
u8 zcl_configReportRspHandler(zclIncoming_t* pCmd);
u8 zcl_readReportCfgHandler(zclIncoming_t *pCmd);
u8 zcl_readReportCfgRspHandler(zclIncoming_t *pCmd);
u8 zcl_reportHandler(zclIncoming_t* pCmd);
#endif
#ifdef ZCL_DISCOVER
u8 zcl_discAttrsHandler(zclIncoming_t* pCmd);
u8 zcl_discAttrsRspHandler(zclIncoming_t* pCmd);
u8 zcl_discCmdsHandler(zclIncoming_t* pCmd);
u8 zcl_discAttrsExtendedHandler(zclIncoming_t* pCmd);
u8 zcl_discAttrsExtRspHandler(zclIncoming_t* pCmd);
#endif
u8 zcl_dfltRspHandler(zclIncoming_t* pCmd);
u8 zcl_sendDfltRsp(zclIncoming_t *inMsg, u8 cmdId, u8 status);

/*********************************************************************
 * @fn      zcl_reset
 *
 * @brief   Reset ZCL Module
 *
 * @param   None
 *
 * @return  None
 */
_CODE_ZCL_ void zcl_reset(void)
{
	zcl_vars.hookFn = NULL;

	/* Reset queue */
	if(zcl_vars.zb2zclQ.curNum){
		ev_queue_freeQ(&zcl_vars.zb2zclQ);
	}

	/* Reset attribute list */
	zcl_vars.clusterNum = 0;
	for(u8 i = 0; i < ZCL_CLUSTER_NUM_MAX; i++){
		zcl_vars.clusterList[i].endpoint = 0xfe;
		zcl_vars.clusterList[i].clusterID = 0xffff;
		zcl_vars.clusterList[i].attrNum = 0;
		zcl_vars.clusterList[i].attrTable = NULL;
		zcl_vars.clusterList[i].cmdHandlerFunc = NULL;
		zcl_vars.clusterList[i].clusterAppCb = NULL;
	}
}

/*********************************************************************
 * @brief      Initialize ZCL layer
 *
 * @param      fn  The hook function pointer
 *
 * @return     None
 */
_CODE_ZCL_ void zcl_init(zcl_hookFn_t fn)
{
	ev_queue_init(&zcl_vars.zb2zclQ, NULL);

	/* reset control module */
	zcl_reset();
	zcl_seqNum = (u8)zb_random();
	zcl_vars.hookFn = fn;
}

/*********************************************************************
 * @brief      Initialize ZCL layer
 *
 * @param      fn  The hook function pointer
 *
 * @return     None
 */
_CODE_ZCL_ void zcl_register(u8 endpoint, u8 clusterNum, zcl_specClusterInfo_t *info)
{
	zcl_specClusterInfo_t *p = info;
	for(u32 i = 0; i < clusterNum; i++){
		if(p->clusterRegisterFunc){
			if(p->clusterRegisterFunc(endpoint, p->attrNum, p->attrTbl, p->clusterAppCb) == ZCL_STA_INSUFFICIENT_SPACE){
				return;
			}
		}
		p++;
	}
}

/*********************************************************************
 * @fn      zcl_findCluster
 *
 * @brief   Find the cluster table by specified cluster ID and endpoint
 *
 * @param   endpoint  Specified endpoint
 * @param   clusterId Specified cluster ID
 *
 * @return  None
 */
_CODE_ZCL_ clusterInfo_t *zcl_findCluster(u8 endpoint, u16 clusterId)
{
	for(u8 i = 0; i < ZCL_CLUSTER_NUM_MAX; i++){
		if((zcl_vars.clusterList[i].endpoint == endpoint) && (zcl_vars.clusterList[i].clusterID == clusterId)){
			return &zcl_vars.clusterList[i];
		}
	}
	return NULL;
}

/*********************************************************************
 * @fn      zcl_registerCluster
 *
 * @brief   Register the cluster to ZCL
 *
 * @param   endpoint  Specified endpoint
 * @param   clusterId Specified cluster ID
 * @param   attrNum   Specified attribute number in the cluster
 * @param   pAttrTbl  Specified attributes
 *
 * @return  ZCL Status @ref zcl_error_codes
 */
_CODE_ZCL_ status_t zcl_registerCluster(u8 endpoint, u16 clusterId, u8 attrNum, const zclAttrInfo_t *pAttrTbl, cluster_cmdHdlr_t cmdHdlrFn, cluster_forAppCb_t cb)
{
	if(zcl_vars.clusterNum >= ZCL_CLUSTER_NUM_MAX){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	if(zcl_findCluster(endpoint, clusterId)){
		return ZCL_STA_DUPLICATE_EXISTS;
	}

	zcl_vars.clusterList[zcl_vars.clusterNum].endpoint = endpoint;
	zcl_vars.clusterList[zcl_vars.clusterNum].clusterID = clusterId;
	zcl_vars.clusterList[zcl_vars.clusterNum].attrNum = attrNum;
	zcl_vars.clusterList[zcl_vars.clusterNum].cmdHandlerFunc = cmdHdlrFn;
	zcl_vars.clusterList[zcl_vars.clusterNum].clusterAppCb = cb;
	zcl_vars.clusterList[zcl_vars.clusterNum++].attrTable = pAttrTbl;

	return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      zcl_getDataTypeLen
 *
 * @brief   get data length from data type
 *
 * @param   dataType
 *
 * @return  Length
 */
_CODE_ZCL_ u8 zcl_getDataTypeLen(u8 dataType)
{
	switch(dataType)
	{
		case ZCL_DATA_TYPE_BOOLEAN:
			return sizeof(u8);

		case ZCL_DATA_TYPE_DATA8:
		case ZCL_DATA_TYPE_BITMAP8:
		case ZCL_DATA_TYPE_UINT8:
		case ZCL_DATA_TYPE_INT8:
		case ZCL_DATA_TYPE_ENUM8:
			return sizeof(u8);

		case ZCL_DATA_TYPE_DATA16:
		case ZCL_DATA_TYPE_BITMAP16:
		case ZCL_DATA_TYPE_UINT16:
		case ZCL_DATA_TYPE_INT16:
		case ZCL_DATA_TYPE_ENUM16:
		case ZCL_DATA_TYPE_SEMI_PREC:
		case ZCL_DATA_TYPE_CLUSTER_ID:
		case ZCL_DATA_TYPE_ATTR_ID:
			return sizeof(u16);

		case ZCL_DATA_TYPE_DATA24:
		case ZCL_DATA_TYPE_BITMAP24:
		case ZCL_DATA_TYPE_UINT24:
		case ZCL_DATA_TYPE_INT24:
			return 3;

		case ZCL_DATA_TYPE_DATA32:
		case ZCL_DATA_TYPE_BITMAP32:
		case ZCL_DATA_TYPE_UINT32:
		case ZCL_DATA_TYPE_INT32:
		case ZCL_DATA_TYPE_SINGLE_PREC:
		case ZCL_DATA_TYPE_UTC:
		case ZCL_DATA_TYPE_DATE:
		case ZCL_DATA_TYPE_BAC_OID:
			return sizeof(u32);

		case ZCL_DATA_TYPE_DATA40:
		case ZCL_DATA_TYPE_BITMAP40:
		case ZCL_DATA_TYPE_UINT40:
		case ZCL_DATA_TYPE_INT40:
			return 5;

		case ZCL_DATA_TYPE_DATA48:
		case ZCL_DATA_TYPE_BITMAP48:
		case ZCL_DATA_TYPE_UINT48:
		case ZCL_DATA_TYPE_INT48:
			return 6;

		case ZCL_DATA_TYPE_DATA56:
		case ZCL_DATA_TYPE_BITMAP56:
		case ZCL_DATA_TYPE_UINT56:
		case ZCL_DATA_TYPE_INT56:
			return 7;

		case ZCL_DATA_TYPE_DATA64:
		case ZCL_DATA_TYPE_BITMAP64:
		case ZCL_DATA_TYPE_UINT64:
		case ZCL_DATA_TYPE_INT64:
		case ZCL_DATA_TYPE_DOUBLE_PREC:
		case ZCL_DATA_TYPE_IEEE_ADDR:
			return 8;

		case ZCL_DATA_TYPE_128_BIT_SEC_KEY:
			return 16;

		case ZCL_DATA_TYPE_NO_DATA:
		case ZCL_DATA_TYPE_UNKNOWN:

		default:
			return 0;
	}
}

/*********************************************************************
 * @fn      zcl_analogDataType
 *
 * @brief   check if analog data
 *
 * @param   dataType
 *
 * @return  TRUE or FALSE
 */
_CODE_ZCL_ bool zcl_analogDataType(u8 dataType)
{
	switch(dataType)
	{
		case ZCL_DATA_TYPE_UINT8:
		case ZCL_DATA_TYPE_UINT16:
		case ZCL_DATA_TYPE_UINT24:
		case ZCL_DATA_TYPE_UINT32:
		case ZCL_DATA_TYPE_UINT40:
		case ZCL_DATA_TYPE_UINT48:
		case ZCL_DATA_TYPE_UINT56:
		case ZCL_DATA_TYPE_UINT64:
		case ZCL_DATA_TYPE_INT8:
		case ZCL_DATA_TYPE_INT16:
		case ZCL_DATA_TYPE_INT24:
		case ZCL_DATA_TYPE_INT32:
		case ZCL_DATA_TYPE_INT40:
		case ZCL_DATA_TYPE_INT48:
		case ZCL_DATA_TYPE_INT56:
		case ZCL_DATA_TYPE_INT64:
		case ZCL_DATA_TYPE_SEMI_PREC:
		case ZCL_DATA_TYPE_SINGLE_PREC:
		case ZCL_DATA_TYPE_DOUBLE_PREC:
		case ZCL_DATA_TYPE_TOD:
		case ZCL_DATA_TYPE_DATE:
		case ZCL_DATA_TYPE_UTC:
			return TRUE;
			break;

		default:
			return FALSE;
			break;
	}

}

/*********************************************************************
 * @fn      zcl_getAttrSize
 *
 * @brief   get attribute data length
 *
 * @param   dataType
 * @param   pData
 *
 * @return  Data length
 */
_CODE_ZCL_ u16 zcl_getAttrSize(u8 dataType, u8 *pData)
{
	u16 dataLen = 0;

	if((dataType == ZCL_DATA_TYPE_LONG_CHAR_STR) || (dataType == ZCL_DATA_TYPE_LONG_OCTET_STR)){
		dataLen = BUILD_U16(pData[0], pData[1]) + 2; //long string length + 2 for length field
	}else if((dataType == ZCL_DATA_TYPE_CHAR_STR) || (dataType == ZCL_DATA_TYPE_OCTET_STR)){
		dataLen = *pData + 1; //string length + 1 for length field
	}else if(dataType == ZCL_DATA_TYPE_STRUCT){
		u8 itemNum = *pData;
		dataLen += 2;
		for(u8 i = 0; i < itemNum; i++){
			u8 dtUnion = pData[dataLen];
			u8 lenTemp = zcl_getDataTypeLen(dtUnion);
			dataLen += (lenTemp + 1);
		}
	}else{
		dataLen = zcl_getDataTypeLen(dataType);
	}

	return dataLen;
}

/*********************************************************************
 * @fn      isLittleEndianMachineCheck
 *
 * @brief   check if little-endian
 *
 * @param
 *
 * @return
 */
_CODE_ZCL_ static int isLittleEndianMachineCheck(void)
{
  u16 test = 0x0001;
  return (*((u8 *)(&test)));
}

/*********************************************************************
 * @fn      zcl_analogDataBuild
 *
 * @brief   build analog data
 *
 * @param   dataType
 * @param   pData	- in pointer
 * @param   pBuf	- out pointer
 *
 * @return  None
 */
_CODE_ZCL_ void zcl_analogDataBuild(u8 dataType, u8 *pData, u8 *pBuf)
{
	int curByteIdx;
	int remainingBytes;
	int step;

	remainingBytes = zcl_getAttrSize(dataType, pData);

	if(isLittleEndianMachineCheck()){
		step = 1;
		curByteIdx = 0;
	}else{
		step = -1;
		curByteIdx = remainingBytes - 1;
	}

	while(remainingBytes--){
		pData[curByteIdx] = *(pBuf++);
		curByteIdx += step;
	}
}

/*********************************************************************
 * @fn      zcl_findAttribute
 *
 * @brief   Find the attribute table by specified endpoint, cluster ID and endpoint
 *
 * @param   endpoint  Specified endpoint
 * @param   clusterId Specified cluster ID
 * @param   attrId	  Specified attribute ID
 *
 * @return  None
 */
_CODE_ZCL_ zclAttrInfo_t *zcl_findAttribute(u8 endpoint, u16 clusterId, u16 attrId)
{
	clusterInfo_t *pClusterList = zcl_findCluster(endpoint, clusterId);
	if(!pClusterList){
		return NULL;
	}

	for(u8 i = 0; i < pClusterList->attrNum; i++){
		if(pClusterList->attrTable[i].id == attrId){
			return (zclAttrInfo_t *)&pClusterList->attrTable[i];
		}
	}

	return NULL;
}

/*********************************************************************
 * @fn      zcl_getAttrVal
 *
 * @brief   Get the attribute value of specified attribute
 *
 * @param   endpoint  Specified endpoint
 * @param   clusterId Specified cluster ID
 * @param   attrId    Specified attribute ID
 * @param   len       The length of required attribute
 * @param   val       The value of required attribute
 *
 * @return  ZCL Status
 */
_CODE_ZCL_ status_t zcl_getAttrVal(u8 endpoint, u16 clusterId, u16 attrId, u16 *len, u8 *val)
{
	zclAttrInfo_t *pAttrList = zcl_findAttribute(endpoint, clusterId, attrId);
	if(!pAttrList){
		/* Not found */
		*len = 0;
		return ZCL_STA_NOT_FOUND;
	}

	if(!pAttrList->data){
		/* Found attribute data pointer is NULL */
		*len = 0;
		return ZCL_STA_NOT_FOUND;
	}else{
		*len = zcl_getAttrSize(pAttrList->type, pAttrList->data);
		memcpy(val, pAttrList->data, *len);

		return ZCL_STA_SUCCESS;
	}
}

/*********************************************************************
 * @fn      zcl_setAttrVal
 *
 * @brief   Set the attribute value of specified attribute
 *
 * @param   endpoint  Specified endpoint
 * @param   clusterId Specified cluster ID
 * @param   attrId    Specified attribute ID
 * @param   val    	  value pointer to write
 *
 * @return  ZCL Status
 */
_CODE_ZCL_ status_t zcl_setAttrVal(u8 endpoint, u16 clusterId, u16 attrId, u8 *val)
{
	zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, clusterId, attrId);
	if(!pAttrEntry){
		return ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}

	u16 len = zcl_getAttrSize(pAttrEntry->type, val);
	memcpy(pAttrEntry->data, val, len);

	return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      zcl_attrRead
 *
 * @brief   Read attribute
 *
 * @param   endpoint  Specified endpoint
 * @param   clusterId Specified cluster ID
 * @param   attrId	  Specified attribute ID
 * @param   status	  out of status
 *
 * @return  zclAttrInfo_t
 */
_CODE_ZCL_ zclAttrInfo_t *zcl_attrRead(u8 endpoint, u16 clusterId, u16 attrId, status_t *status)
{
	zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, clusterId, attrId);
	if(!pAttrEntry){
		/* Not found */
		*status = ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}else if(!(pAttrEntry->access & ACCESS_CONTROL_READ)){
		*status = ZCL_STA_WRITE_ONLY;
	}else if(!pAttrEntry->data){
		*status = ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}else{
		*status = ZCL_STA_SUCCESS;
	}

	if(*status == ZCL_STA_SUCCESS){
		return pAttrEntry;
	}else{
		return NULL;
	}
}

/*********************************************************************
 * @fn      zcl_attrWrite
 *
 * @brief   Write attribute
 *
 * @param   endpoint  Specified endpoint
 * @param   clusterId Specified cluster ID
 * @param   pWriteRec In pointer
 * @param   enable	  If enable to write
 *
 * @return  ZCL Status
 */
_CODE_ZCL_ status_t zcl_attrWrite(u8 endpoint, u16 clusterId, zclWriteRec_t *pWriteRec, bool enable)
{
	zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, clusterId, pWriteRec->attrID);
	if(!pAttrEntry){
		/* Not found */
		return ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}

	if(pAttrEntry->type != pWriteRec->dataType){
		return ZCL_STA_INVALID_DATA_TYPE;
	}

	if(!(pAttrEntry->access & ACCESS_CONTROL_WRITE)){
		return ZCL_STA_READ_ONLY;
	}

	if(!pAttrEntry->data){
		return ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}

	if(enable){
		return zcl_setAttrVal(endpoint, clusterId, pWriteRec->attrID, pWriteRec->attrData);
	}else{
		return ZCL_STA_SUCCESS;
	}
}

/*********************************************************************
 * @fn      zcl_buildHdr
 *
 * @brief   build ZCL layer header
 *
 * @param
 *
 * @return header length
 */
_CODE_ZCL_ u8 zcl_buildHdr(u8 *buf, u8 clusterSpec, u8 dir, u8 disDefRsp, u32 manufCode, u8 seqNum, u8 cmd)
{
	u8 *pBuf = buf;
	u16 mfgCode = 0xFFFF;

	((zclFrmCtrl_t *)pBuf)->bf.type = (clusterSpec) ? ZCL_FRAME_TYPE_SPECIFIC_CMD : ZCL_FRAME_TYPE_PROFILE_CMD;
	((zclFrmCtrl_t *)pBuf)->bf.manufSpec = (manufCode & 0x80000000) ? 1 : 0;
	((zclFrmCtrl_t *)pBuf)->bf.dir = dir;
	((zclFrmCtrl_t *)pBuf)->bf.disDefResp = disDefRsp;
	((zclFrmCtrl_t *)pBuf)->bf.reserved = 0;
    ((zclHdr_t*)pBuf)->cmd = 1;
    pBuf++;
	if(manufCode & 0x80000000){
		mfgCode = (u16)(manufCode & 0x0000FFFF);
		memcpy(pBuf, &mfgCode, sizeof(mfgCode));
		pBuf += sizeof(mfgCode);
	}
	*pBuf++ = seqNum;
	*pBuf++ = cmd;

	return (pBuf - buf);
}

/*********************************************************************
 * @fn      zcl_sendCmd
 *
 * @brief   Used to send Profile and Cluster Specific Command messages.
 *
 *          NOTE: The calling application is responsible for incrementing
 *                the Sequence Number.
 *
 * @param   srcEp - source endpoint
 * @param   pDstEpInfo - destination endpoint infomation
 * @param   clusterID - cluster ID
 * @param   cmd - command ID
 * @param   specific - whether the command is Cluster Specific
 * @param   direction - client/server direction of the command
 * @param   disableDefaultRsp - disable Default Response command
 * @param   manuCode - manufacturer code for proprietary extensions to a profile
 * @param   seqNo - identification number for the transaction
 * @param   cmdPldLen - length of the command to be sent
 * @param   cmdPld - command to be sent
 *
 * @return  status_t
 */
_CODE_ZCL_ status_t zcl_sendCmd(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 cmd, u8 specific,
				  	  	  	  	u8 direction, u8 disableDefaultRsp, u32 manuCode, u8 seqNo, u16 cmdPldLen, u8 *cmdPld)
{
	u8 *asdu = (u8 *)ev_buf_allocate(sizeof(zclHdr_t) + cmdPldLen);
	if(!asdu){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pAsdu = asdu;

	/* Build ZCL header */
	pAsdu += zcl_buildHdr(asdu, specific, direction, disableDefaultRsp, manuCode, seqNo, cmd);
	
	/* Add ZCL payload */
	memcpy(pAsdu, cmdPld, cmdPldLen);
	u16 asdulength = pAsdu - asdu + cmdPldLen;

	u8 apsCnt = 0;
	u8 status = af_dataSend(srcEp, pDstEpInfo, clusterId, asdulength, asdu, &apsCnt);

	ev_buf_free(asdu);

	return (status == RET_OK) ? ZCL_STA_SUCCESS : ZCL_STA_INSUFFICIENT_SPACE;
}

_CODE_ZCL_ status_t zcl_sendInterPANCmd(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 cmd, u8 specific,
										u8 direction, u8 disableDefaultRsp,	u32 manuCode, u8 seqNo, u16 cmdPldLen, u8 *cmdPld)
{
	u8 *asdu = (u8 *)ev_buf_allocate(8 + cmdPldLen);
	if(!asdu){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pAsdu = asdu;

	/* Build ZCL header */
	pAsdu += zcl_buildHdr(asdu, specific, direction, disableDefaultRsp, manuCode, seqNo, cmd);

	/* Add ZCL payload */
	memcpy(pAsdu, cmdPld, cmdPldLen);
	u16 asdulength = pAsdu - asdu + cmdPldLen;

	if(ZCL_CLUSTER_TOUCHLINK_COMMISSIONING == clusterId){
		pDstEpInfo->txOptions |= APS_TX_OPT_INTRA_PAN;
	}

	u8 apsCnt = 0;
	u8 status = af_dataSend(srcEp, pDstEpInfo, clusterId, asdulength, asdu, &apsCnt);

	ev_buf_free(asdu);

	return (status == RET_OK) ? ZCL_STA_SUCCESS : ZCL_STA_INSUFFICIENT_SPACE;
}

/*********************************************************************
 * @fn      zcl_foundationCmdHandler
 *
 * @brief   Foundation command handler
 *
 * @param   pCmd
 *
 * @return  status_t
 */
_CODE_ZCL_ status_t zcl_foundationCmdHandler(zclIncoming_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pCmd->hdr.cmd)
	{
#ifdef ZCL_READ
		case ZCL_CMD_READ:
			status = zcl_readHandler(pCmd);
			break;
		case ZCL_CMD_READ_RSP:
			status = zcl_readRspHandler(pCmd);
			break;
#endif
#ifdef ZCL_WRITE
		case ZCL_CMD_WRITE:
			status = zcl_writeHandler(pCmd);
			break;
		case ZCL_CMD_WRITE_UNDIVIDED:
			status = zcl_writeUndividedHandler(pCmd);
			break;
		case ZCL_CMD_WRITE_RSP:
			status = zcl_writeRspHandler(pCmd);
			break;
		case ZCL_CMD_WRITE_NO_RSP:
			status = zcl_writeHandler(pCmd);
			break;
#endif
#ifdef ZCL_REPORT
		case ZCL_CMD_CONFIG_REPORT:
			status = zcl_configReportHandler(pCmd);
			break;
		case ZCL_CMD_CONFIG_REPORT_RSP:
			status = zcl_configReportRspHandler(pCmd);
			break;
		case ZCL_CMD_READ_REPORT_CFG:
			status = zcl_readReportCfgHandler(pCmd);
			break;
		case ZCL_CMD_READ_REPORT_CFG_RSP:
			status = zcl_readReportCfgRspHandler(pCmd);
			break;
		case ZCL_CMD_REPORT:
			status = zcl_reportHandler(pCmd);
			break;
#endif
		case ZCL_CMD_DEFAULT_RSP:
			status  = zcl_dfltRspHandler(pCmd);
			break;
#ifdef ZCL_DISCOVER
		case ZCL_CMD_DISCOVER_ATTR:
			status = zcl_discAttrsHandler(pCmd);
			break;
		case ZCL_CMD_DISCOVER_ATTR_RSP:
			status = zcl_discAttrsRspHandler(pCmd);
			break;
		case ZCL_CMD_DISCOVER_CMDS_RCVD:
		case ZCL_CMD_DISCOVER_CMDS_RCVD_RSP:
		case ZCL_CMD_DISCOVER_CMDS_GEN:
		case ZCL_CMD_DISCOVER_CMDS_GEN_RSP:
			status = ZCL_STA_UNSUP_GENERAL_COMMAND;
			break;
		case ZCL_CMD_DISCOVER_ATTR_EXTD:
			status = zcl_discAttrsExtendedHandler(pCmd);
			break;
		case ZCL_CMD_DISCOVER_ATTR_EXTD_RSP:
			status = zcl_discAttrsExtRspHandler(pCmd);
			break;
#endif
		default:
			status = ZCL_STA_UNSUP_GENERAL_COMMAND;
			break;
	}

	return status;
}

/*********************************************************************
 * @fn      zcl_cmdHandler
 *
 * @brief   ZCL command handler
 *
 * @param   pCmd
 *
 * @return  None
 */
_CODE_ZCL_ void zcl_cmdHandler(u8 *pCmd)
{
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pCmd;
	u8 status = ZCL_STA_SUCCESS;
	u8 toAppFlg = 0;

	zclIncoming_t inMsg;
	TL_SETSTRUCTCONTENT(inMsg,0);
	inMsg.msg = pApsdeInd;

	/* Parse Header */
	if(pApsdeInd->asdu[0] & ZCL_FRAME_CONTROL_MANU_SPECIFIC){
		if(pApsdeInd->asduLen >= 5){
			memcpy(&inMsg.hdr, pApsdeInd->asdu, 5);
			inMsg.pData = pApsdeInd->asdu + 5;
			inMsg.dataLen = pApsdeInd->asduLen - 5;
		}else{
			status = ZCL_STA_FAILURE;
		}
	}else{
		if(pApsdeInd->asduLen >= 3){
			inMsg.hdr.frmCtrl.byte = pApsdeInd->asdu[0];
			inMsg.hdr.manufCode = 0;
			inMsg.hdr.seqNum = pApsdeInd->asdu[1];
			inMsg.hdr.cmd = pApsdeInd->asdu[2];
			inMsg.pData = &pApsdeInd->asdu[3];
			inMsg.dataLen = pApsdeInd->asduLen - 3;
		}else{
			status = ZCL_STA_FAILURE;
		}
	}

	if(status == ZCL_STA_FAILURE){
		ev_buf_free(pCmd);
		return;
	}

	u16 devEnableAttrLen = 0;
	bool devEnable = TRUE;

	/* Command dispatch */
	if(inMsg.hdr.frmCtrl.bf.type == ZCL_FRAME_TYPE_PROFILE_CMD){
		/* Foundation type message */
		if(inMsg.hdr.frmCtrl.bf.manufSpec){
			// We don't support any manufacturer specific command
			status = ZCL_STA_UNSUP_MANU_GENERAL_COMMAND;
		}else if(inMsg.hdr.cmd > ZCL_CMD_MAX){
			// Unsupported message
			status = ZCL_STA_UNSUP_GENERAL_COMMAND;
		}else{
			status = zcl_foundationCmdHandler(&inMsg);
			if((status != ZCL_STA_SUCCESS) && (status != ZCL_STA_CMD_HAS_RESP)){
				status = ZCL_STA_FAILURE;
			}
			toAppFlg = 1;
		}
	}else{
		/* Cluster specific command */
		if(inMsg.hdr.frmCtrl.bf.manufSpec){
			// We don't support any manufacturer specific command
			status = ZCL_STA_UNSUP_MANU_CLUSTER_COMMAND;
		}else{
			clusterInfo_t *pCluster = zcl_findCluster(pApsdeInd->indInfo.dst_ep, pApsdeInd->indInfo.cluster_id);
			if(!pCluster){
				status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			}else{
				/* Check if basic device enable support */
				zcl_getAttrVal(pApsdeInd->indInfo.dst_ep, ZCL_CLUSTER_GEN_BASIC, ZCL_ATTRID_BASIC_DEV_ENABLED, &devEnableAttrLen, &devEnable);

				if(devEnable || (pCluster->clusterID == ZCL_CLUSTER_GEN_IDENTIFY)){
					inMsg.clusterAppCb = pCluster->clusterAppCb;
					status = pCluster->cmdHandlerFunc(&inMsg);

					devEnable = TRUE;
				}
			}
		}
	}

	if(devEnable){
		if((inMsg.hdr.frmCtrl.bf.disDefResp == 0 || status != ZCL_STA_SUCCESS) && UNICAST_MSG(inMsg.msg) && (status != ZCL_STA_CMD_HAS_RESP)){
			/* send default response */
			zcl_sendDfltRsp(&inMsg, inMsg.hdr.cmd, status);
		}
	}

	if(zcl_vars.hookFn && toAppFlg && inMsg.attrCmd){
		zcl_vars.hookFn(&inMsg);
		ev_buf_free(inMsg.attrCmd);
	}

	ev_buf_free(pCmd);
}

/*********************************************************************
 * @fn      zcl_task
 *
 * @brief   ZCL layer maintenance
 *
 * @param   arg
 *
 * @return  None
 */
_CODE_ZCL_ void zcl_task(void *arg)
{
	if(zcl_vars.zb2zclQ.curNum == 0){
		return;
	}

	u8 *pCmd = ev_queue_pop(&zcl_vars.zb2zclQ);
	if(!pCmd){
		return;
	}

	zcl_cmdHandler(pCmd);
}

/*********************************************************************
 * @fn      zcl_rx_handler
 *
 * @brief   Receive handler for data from APS/AF layer
 *
 * @param   pData     Received data
 *
 * @return  None
 */
_CODE_ZCL_ void zcl_rx_handler(void *pData)
{
	ev_queue_push(&zcl_vars.zb2zclQ, pData);
	TL_SCHEDULE_TASK(zcl_task, NULL);
}


/***************************************************************************
 **************************** Read *****************************************
 ***************************************************************************/
#ifdef ZCL_READ
/*********************************************************************
 * @fn      zcl_read
 *
 * @brief   Send a Read Attribute command
 *
 * @param   srcEP - Application's endpoint
 * @param   pDstEpInfo - destination address
 * @param   clusterID - cluster ID
 * @param   readCmd - read command to be sent
 * @param   direction - direction of the command
 * @param   seqNum - transaction sequence number
 *
 * @return  status_t
 */
_CODE_ZCL_ status_t zcl_read(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadCmd_t *readCmd)
{
    u16 len = readCmd->numAttr * 2; // Attribute ID

    if(len == 0){
    	return ZCL_STA_INVALID_FIELD;
    }

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if(!buf){
    	return ZCL_STA_INSUFFICIENT_SPACE;
    }

	u8 *pBuf = buf;

	for(u8 i = 0; i < readCmd->numAttr; i++){
		*pBuf++ = LO_UINT16(readCmd->attrID[i]);
		*pBuf++ = HI_UINT16(readCmd->attrID[i]);
	}

    u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_READ, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_readWithMfgCode(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u32 manuCode, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadCmd_t *readCmd)
{
    u16 len = readCmd->numAttr * 2; // Attribute ID

    if(len == 0){
    	return ZCL_STA_INVALID_FIELD;
    }

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if(!buf){
    	return ZCL_STA_INSUFFICIENT_SPACE;
    }

	u8 *pBuf = buf;

	for(u8 i = 0; i < readCmd->numAttr; i++){
		*pBuf++ = LO_UINT16(readCmd->attrID[i]);
		*pBuf++ = HI_UINT16(readCmd->attrID[i]);
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_READ, FALSE, direction, disableDefaultRsp, manuCode, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_readRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadRspCmd_t *readRspCmd)
{
	u16 len = 0;
	zclReadRspStatus_t *pReadRspStatus = NULL;

	for(u8 i = 0; i < readRspCmd->numAttr; i++){
		pReadRspStatus = &(readRspCmd->attrList[i]);
		len += 2 + 1;	//attr ID + status
		if(pReadRspStatus->status == ZCL_STA_SUCCESS){
			len++;	//data type
			if(pReadRspStatus->data != NULL){
				len += zcl_getAttrSize(pReadRspStatus->dataType, pReadRspStatus->data);	//date length
			}else{
				//data type err
				return ZCL_STA_INVALID_FIELD;
			}
		}
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	for(u8 i = 0; i < readRspCmd->numAttr; i++){
		pReadRspStatus = &(readRspCmd->attrList[i]);
		/* Fill attribute ID */
		*pBuf++ = LO_UINT16(pReadRspStatus->attrID);
		*pBuf++ = HI_UINT16(pReadRspStatus->attrID);
		/* Fill status */
		*pBuf++ = pReadRspStatus->status;

		if(pReadRspStatus->status == ZCL_STA_SUCCESS){
			/* Fill data type */
			*pBuf++ = pReadRspStatus->dataType;
			/* Fill attribute data */
			if(pReadRspStatus->data != NULL){
				u16 dataLen = zcl_getAttrSize(pReadRspStatus->dataType, pReadRspStatus->data);
				memcpy(pBuf, pReadRspStatus->data, dataLen);
				pBuf += dataLen;
			}
		}
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_READ_RSP, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ zclReadRspCmd_t *zcl_parseInReadRspCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 numAttr = 0;
	u16 dataLen = 0;
	u16 attrDataLen = 0;
	zclReadRspStatus_t *pReadRspStatus = NULL;

	while(pBuf < (pCmd->pData + pCmd->dataLen)){
		u8 status;

		numAttr++;
		pBuf += 2;

		status = *pBuf++;
		if(status == ZCL_STA_SUCCESS){
			u8 dataType = *pBuf++;
			attrDataLen = zcl_getAttrSize(dataType, pBuf);
			pBuf += attrDataLen;

			dataLen += attrDataLen;
		}
	}

	/* get the length of Read Response Command */
	u16 len = sizeof(zclReadRspCmd_t) + numAttr * sizeof(zclReadRspStatus_t);
	zclReadRspCmd_t *p = (zclReadRspCmd_t *)ev_buf_allocate(len + dataLen);
	if(p != NULL){
		pBuf = pCmd->pData;
		u8 *pData = (u8 *)((u8 *)p + len);

		p->numAttr = numAttr;
		for(u8 i = 0; i < p->numAttr; i++){
			pReadRspStatus = &(p->attrList[i]);
			/* Build attribute ID */
			pReadRspStatus->attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
			/* Build status */
			pReadRspStatus->status = *pBuf++;
			if(pReadRspStatus->status == ZCL_STA_SUCCESS){
				/* Build data type */
				pReadRspStatus->dataType = *pBuf++;
				/* Build attribute data, and put the data pointer into the struct */
				attrDataLen = zcl_getAttrSize(pReadRspStatus->dataType, pBuf);
				memcpy(pData, pBuf, attrDataLen);
				pReadRspStatus->data = pData;
				pBuf += attrDataLen;

				pData += attrDataLen;
			}
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_readRspHandler(zclIncoming_t *pCmd)
{
	zclReadRspCmd_t *pReadRspCmd = NULL;

	/* Parse In Read Response Command */
	pReadRspCmd = zcl_parseInReadRspCmd(pCmd);
	if(pReadRspCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pReadRspCmd;
	}

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ zclReadCmd_t *zcl_parseInReadCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;

	zclReadCmd_t *p = (zclReadCmd_t *)ev_buf_allocate(sizeof(zclReadCmd_t) + pCmd->dataLen);
	if(p != NULL){
		p->numAttr = pCmd->dataLen / 2;	//attr ID
		for(u8 i = 0; i < p->numAttr; i++){
			p->attrID[i] = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_readHandler(zclIncoming_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u16 clusterId = pCmd->msg->indInfo.cluster_id;
	u8 endpoint = pCmd->msg->indInfo.dst_ep;

	/* diagnostics last message LQI */
	g_sysDiags.lastMessageLQI = pCmd->msg->indInfo.lqi;
	/* diagnostics last message RSSI */
	g_sysDiags.lastMessageRSSI = pCmd->msg->indInfo.rssi;

	/* Parse In Read Command */
	zclReadCmd_t *pReadCmd = zcl_parseInReadCmd(pCmd);
	if(pReadCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pReadCmd;
	}

	if(!pCmd->msg->indInfo.security_status){
		return ZCL_STA_FAILURE;
	}

	/* Build Read Response Command */
	u16 len = sizeof(zclReadRspCmd_t) + (pReadCmd->numAttr * sizeof(zclReadRspStatus_t));
	zclReadRspCmd_t *pReadRspCmd = (zclReadRspCmd_t *)ev_buf_allocate(len);
	if(!pReadRspCmd){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	pReadRspCmd->numAttr = pReadCmd->numAttr;

	for(u8 i = 0; i < pReadCmd->numAttr; i++){
		zclAttrInfo_t *pAttrEntry = zcl_attrRead(endpoint, clusterId, pReadCmd->attrID[i], &status);
		if(pAttrEntry){
			pReadRspCmd->attrList[i].dataType = pAttrEntry->type;
			pReadRspCmd->attrList[i].data = pAttrEntry->data;
		}
		pReadRspCmd->attrList[i].attrID = pReadCmd->attrID[i];
		pReadRspCmd->attrList[i].status = status;
	}

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pCmd->msg->indInfo.src_ep;
	dstEp.profileId = pCmd->msg->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pCmd->msg->indInfo.src_short_addr;

	status = zcl_readRsp(endpoint, &dstEp, clusterId, TRUE, !pCmd->hdr.frmCtrl.bf.dir, pCmd->hdr.seqNum, pReadRspCmd);

	ev_buf_free((void *)pReadRspCmd);

	if(status == ZCL_STA_SUCCESS){
		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}
#endif

/***************************************************************************
 **************************** Write ****************************************
 ***************************************************************************/
#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      zcl_writeReq
 *
 * @brief   Send a Write Attribute command
 *
 * @param   srcEP - Application's endpoint
 * @param   pDstEpInfo - destination address
 * @param   clusterID - cluster ID
 * @param   cmd - command ID
 * @param   readCmd - read command to be sent
 * @param   direction - direction of the command
 * @param   seqNum - transaction sequence number
 *
 * @return  None
 */
_CODE_ZCL_ status_t zcl_writeReq(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 cmd, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclWriteCmd_t *writeCmd)
{
	u16 len = 0;
	zclWriteRec_t *pWriteRec = NULL;

	for(u8 i = 0; i < writeCmd->numAttr; i++){
		pWriteRec = &(writeCmd->attrList[i]);
		len += 2 + 1;	//attrID + dataType
		len += zcl_getAttrSize(pWriteRec->dataType, pWriteRec->attrData);	//attribute data length
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	for(u8 i = 0; i < writeCmd->numAttr; i++){
		pWriteRec = &(writeCmd->attrList[i]);
		/* Fill attribute ID */
		*pBuf++ = LO_UINT16(pWriteRec->attrID);
		*pBuf++ = HI_UINT16(pWriteRec->attrID);
		/* Fill data type */
		*pBuf++ = pWriteRec->dataType;
		/* Fill attribute value */
		u16 dataLen = zcl_getAttrSize(pWriteRec->dataType, pWriteRec->attrData);
		memcpy(pBuf, pWriteRec->attrData, dataLen);
		pBuf += dataLen;
	}


	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, cmd, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_writeWithMfgCode( u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u32 manuCode, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclWriteCmd_t *writeCmd)
{
	u16 len = 0;
	zclWriteRec_t *pWriteRec = NULL;

	for(u8 i = 0; i < writeCmd->numAttr; i++){
		pWriteRec = &(writeCmd->attrList[i]);
		len += 2 + 1;	//attrID + dataType
		len += zcl_getAttrSize(pWriteRec->dataType, pWriteRec->attrData);	//attribute data length
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	for(u8 i = 0; i < writeCmd->numAttr; i++){
		pWriteRec = &(writeCmd->attrList[i]);
		/* Fill attribute ID */
		*pBuf++ = LO_UINT16(pWriteRec->attrID);
		*pBuf++ = HI_UINT16(pWriteRec->attrID);
		/* Fill data type */
		*pBuf++ = pWriteRec->dataType;
		/* Fill attribute value */
		u16 dataLen = zcl_getAttrSize(pWriteRec->dataType, pWriteRec->attrData);
		memcpy(pBuf, pWriteRec->attrData, dataLen);
		pBuf += dataLen;
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_WRITE, FALSE, direction, disableDefaultRsp, manuCode, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_writeRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclWriteRspCmd_t *writeRspCmd)
{
	u16 len = writeRspCmd->numAttr * (1 + 2);	//status + attrID

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	for(u8 i = 0; i < writeRspCmd->numAttr; i++){
		*pBuf++ = writeRspCmd->attrList[i].status;
		*pBuf++ = LO_UINT16(writeRspCmd->attrList[i].attrID);
		*pBuf++ = HI_UINT16(writeRspCmd->attrList[i].attrID);
	}

	if((writeRspCmd->numAttr == 1) && (writeRspCmd->attrList[0].status == ZCL_STA_SUCCESS)){
		len = 1;
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_WRITE_RSP, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ zclWriteRspCmd_t *zcl_parseInWriteRspCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 i = 0;

	u16 len = sizeof(zclWriteRspCmd_t) + pCmd->dataLen;
	zclWriteRspCmd_t *p = (zclWriteRspCmd_t *)ev_buf_allocate(len);
	if(p != NULL){
		if(pCmd->dataLen == 1){
			p->attrList[i++].status = *pBuf;
		}else{
			while(pBuf < (pCmd->pData + pCmd->dataLen)){
				p->attrList[i].status = *pBuf++;
				p->attrList[i++].attrID = BUILD_U16(pBuf[0], pBuf[1]);
				pBuf += 2;
			}
		}
		p->numAttr = i;
	}

	return p;
}

_CODE_ZCL_ status_t zcl_writeRspHandler(zclIncoming_t *pCmd)
{
	zclWriteRspCmd_t *pWriteRspCmd = NULL;

	/* Parse In Write Response Command */
	pWriteRspCmd = zcl_parseInWriteRspCmd(pCmd);
	if(pWriteRspCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pWriteRspCmd;
	}

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ zclWriteCmd_t *zcl_parseInWriteCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 numAttr = 0;
	u16 dataLen = 0;
	u16 attrDataLen = 0;

	while(pBuf < (pCmd->pData + pCmd->dataLen)){
		numAttr++;
		pBuf += 2;

		u8 dataType = *pBuf++;

		attrDataLen = zcl_getAttrSize(dataType, pBuf);
		pBuf += attrDataLen;

		dataLen += attrDataLen;
	}

	/* get the length of Write Command */
	u16 len = sizeof(zclWriteCmd_t) + numAttr * sizeof(zclWriteRec_t);
	zclWriteCmd_t *p = (zclWriteCmd_t *)ev_buf_allocate(len + dataLen);
	if(p != NULL){
		pBuf = pCmd->pData;
		u8 *pData = (u8 *)((u8 *)p + len);

		p->numAttr = numAttr;
		for(u8 i = 0; i < numAttr; i++){
			zclWriteRec_t *pWriteRec = &(p->attrList[i]);
			/* Build attribute ID */
			pWriteRec->attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
			/* Build data Type */
			pWriteRec->dataType = *pBuf++;
			/* Build attribute data pointer */
			attrDataLen = zcl_getAttrSize(pWriteRec->dataType, pBuf);
			memcpy(pData, pBuf, attrDataLen);
			pWriteRec->attrData = pData;

			pBuf += attrDataLen;

			pData += attrDataLen;
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_writeHandler(zclIncoming_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u16 clusterId = pCmd->msg->indInfo.cluster_id;
	u8 endpoint = pCmd->msg->indInfo.dst_ep;
	zclWriteRspCmd_t *pWriteRspCmd = NULL;
	u8 succWriteAttrCnt = 0;
	bool rspSend = FALSE;

	/* Parse In Write Command */
	zclWriteCmd_t *pWriteCmd = zcl_parseInWriteCmd(pCmd);
	if(pWriteCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pWriteCmd;
	}

	if(pCmd->hdr.cmd == ZCL_CMD_WRITE){
		/* Build Write Response Command */
		u16 len = sizeof(zclWriteRspCmd_t) + (pWriteCmd->numAttr * sizeof(zclWriteRspStatus_t));
		pWriteRspCmd = (zclWriteRspCmd_t *)ev_buf_allocate(len);
		if(!pWriteRspCmd){
			return ZCL_STA_INSUFFICIENT_SPACE;
		}

		rspSend = TRUE;
	}

	for(u8 i = 0; i < pWriteCmd->numAttr; i++){
		zclWriteRec_t *pWriteRec = &(pWriteCmd->attrList[i]);

		status = zcl_attrWrite(endpoint, clusterId, pWriteRec, TRUE);

		if(rspSend){
			pWriteRspCmd->attrList[i].status = status;
			pWriteRspCmd->attrList[i].attrID = pWriteRec->attrID;

			if(status == ZCL_STA_SUCCESS){
				succWriteAttrCnt++;
			}
		}
	}
	
	if(rspSend){
		pWriteRspCmd->numAttr = pWriteCmd->numAttr;
		if(pWriteCmd->numAttr == succWriteAttrCnt){
			pWriteRspCmd->numAttr = 1;
			pWriteRspCmd->attrList[0].status = ZCL_STA_SUCCESS;
		}

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstEp = pCmd->msg->indInfo.src_ep;
		dstEp.profileId = pCmd->msg->indInfo.profile_id;
		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pCmd->msg->indInfo.src_short_addr;

		status = zcl_writeRsp(endpoint, &dstEp, clusterId, TRUE, !pCmd->hdr.frmCtrl.bf.dir, pCmd->hdr.seqNum, pWriteRspCmd);

		if(status == ZCL_STA_SUCCESS)
			status = ZCL_STA_CMD_HAS_RESP;

		ev_buf_free((void *)pWriteRspCmd);
	}

	return status;
}

_CODE_ZCL_ status_t zcl_writeUndividedHandler(zclIncoming_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u16 clusterId = pCmd->msg->indInfo.cluster_id;
	u8 endpoint = pCmd->msg->indInfo.dst_ep;
	zclWriteRec_t *pWriteRec = NULL;
	bool needWrite = TRUE;

	/* Parse In Write Command */
	zclWriteCmd_t *pWriteCmd = zcl_parseInWriteCmd(pCmd);
	if(pWriteCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pWriteCmd;
	}

	u16 len = sizeof(zclWriteRspCmd_t) + pWriteCmd->numAttr * sizeof(zclWriteRspStatus_t);
	zclWriteRspCmd_t *pWriteRspCmd = (zclWriteRspCmd_t *)ev_buf_allocate(len);
	if(!pWriteRspCmd){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	for(u8 i = 0; i < pWriteCmd->numAttr; i++){
		pWriteRec = &(pWriteCmd->attrList[i]);

		status = zcl_attrWrite(endpoint, clusterId, pWriteRec, FALSE);

		pWriteRspCmd->attrList[i].status = status;
		pWriteRspCmd->attrList[i].attrID = pWriteRec->attrID;

		if(status != ZCL_STA_SUCCESS){
			needWrite = FALSE;
		}
	}
	
	pWriteRspCmd->numAttr = pWriteCmd->numAttr;

	if(needWrite){
		for(u8 i = 0; i < pWriteCmd->numAttr; i++){
			pWriteRec = &(pWriteCmd->attrList[i]);
			
			zcl_attrWrite(endpoint, clusterId, pWriteRec, TRUE);
		}
		
		pWriteRspCmd->numAttr = 1;
		pWriteRspCmd->attrList[0].status = ZCL_STA_SUCCESS;
	}

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pCmd->msg->indInfo.src_ep;
	dstEp.profileId = pCmd->msg->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pCmd->msg->indInfo.src_short_addr;
	
	status = zcl_writeRsp(endpoint, &dstEp, clusterId, TRUE, !pCmd->hdr.frmCtrl.bf.dir, pCmd->hdr.seqNum, pWriteRspCmd);

	if(status == ZCL_STA_SUCCESS)
		status = ZCL_STA_CMD_HAS_RESP;

	ev_buf_free((void *)pWriteRspCmd);
	
	return status;
}
#endif

/***************************************************************************
 **************************** Report ***************************************
 ***************************************************************************/
#ifdef ZCL_REPORT
_CODE_ZCL_ status_t zcl_configReport(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclCfgReportCmd_t *cfgReportCmd)
{
	u16 len = 0;
	zclCfgReportRec_t *pReportRec = NULL;

	for(u8 i = 0; i < cfgReportCmd->numAttr; i++){
		pReportRec = &(cfgReportCmd->attrList[i]);

		len += 1 + 2;	//direction + attrID

		if(pReportRec->direction == ZCL_SEND_ATTR_REPORTS){
			len += 1 + 2 + 2;	//dataType + minIntervals + maxIntervals

			if(zcl_analogDataType(pReportRec->dataType)){
				len += zcl_getAttrSize(pReportRec->dataType, pReportRec->reportableChange);
			}
		}else{
			len += 2;	//timeout period
		}
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	for(u8 i = 0; i < cfgReportCmd->numAttr; i++){
		pReportRec = &(cfgReportCmd->attrList[i]);

		*pBuf++ = pReportRec->direction;		//direction
		*pBuf++ = LO_UINT16(pReportRec->attrID);//attrID
		*pBuf++ = HI_UINT16(pReportRec->attrID);

		if(pReportRec->direction == ZCL_SEND_ATTR_REPORTS){
			*pBuf++ = pReportRec->dataType; 			  //dataType
			*pBuf++ = LO_UINT16(pReportRec->minReportInt);//minReportInt
			*pBuf++ = HI_UINT16(pReportRec->minReportInt);
			*pBuf++ = LO_UINT16(pReportRec->maxReportInt);//maxReportInt
			*pBuf++ = HI_UINT16(pReportRec->maxReportInt);

			if(zcl_analogDataType(pReportRec->dataType)){
				u16 dataLen = zcl_getAttrSize(pReportRec->dataType, pReportRec->reportableChange);
				memcpy(pBuf, pReportRec->reportableChange, dataLen);
				pBuf += dataLen;
			}
		}else{
			*pBuf++ =  LO_UINT16(pReportRec->timeoutPeriod);//timeoutPeriod
			*pBuf++ =  HI_UINT16(pReportRec->timeoutPeriod);
		}
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_CONFIG_REPORT, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_configReportRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclCfgReportRspCmd_t *cfgReportRspCmd)
{
	u16 len = cfgReportRspCmd->numAttr * (1 + 1 + 2);	//status + direction + attrID

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	for(u8 i = 0; i < cfgReportRspCmd->numAttr; i++){
		*pBuf++ = cfgReportRspCmd->attrList[i].status;
		*pBuf++ = cfgReportRspCmd->attrList[i].direction;
		*pBuf++ = LO_UINT16(cfgReportRspCmd->attrList[i].attrID);
		*pBuf++ = HI_UINT16(cfgReportRspCmd->attrList[i].attrID);
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_CONFIG_REPORT_RSP, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_readReportConfig(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadReportCfgCmd_t *readReportCfgCmd)
{
	u16 len = readReportCfgCmd->numAttr * (1 + 2);	//direction + attrID

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	for(u8 i = 0; i < readReportCfgCmd->numAttr; i++){
		*pBuf++ = readReportCfgCmd->attrList[i].direction;
		*pBuf++ = LO_UINT16(readReportCfgCmd->attrList[i].attrID);
		*pBuf++ = HI_UINT16(readReportCfgCmd->attrList[i].attrID);
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_READ_REPORT_CFG, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_readReportConfigRsp( u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclReadReportCfgRspCmd_t *readReportCfgRspCmd)
{
	u16 len = 0;
	zclReportCfgRspRec_t *pRec = NULL;

	for(u8 i = 0; i < readReportCfgRspCmd->numAttr; i++){
		pRec = &(readReportCfgRspCmd->attrList[i]);

		len += 1 + 1 + 2;	//status + direction + attrID

		if(pRec->status == ZCL_STA_SUCCESS){
			if(pRec->direction == ZCL_SEND_ATTR_REPORTS){
				len += 1 + 2 + 2;	//dataType + minInterval + maxInterval
				if(zcl_analogDataType(pRec->dataType)){
					len += zcl_getAttrSize(pRec->dataType, pRec->reportableChange);
				}
			}else{
				len += 2;	//timeout period
			}
		}
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	for(u8 i = 0; i < readReportCfgRspCmd->numAttr; i++){
		pRec = &(readReportCfgRspCmd->attrList[i]);

		*pBuf++ = pRec->status;					//status;
		*pBuf++ = pRec->direction;				//direction;
		*pBuf++ = LO_UINT16(pRec->attrID);		//attrID;
		*pBuf++ = HI_UINT16(pRec->attrID);		//attrID;

		if(pRec->status == ZCL_STA_SUCCESS){
			if(pRec->direction == ZCL_SEND_ATTR_REPORTS){
				*pBuf++ = pRec->dataType;
				*pBuf++ = LO_UINT16(pRec->minReportInt);
				*pBuf++ = HI_UINT16(pRec->minReportInt);
				*pBuf++ = LO_UINT16(pRec->maxReportInt);
				*pBuf++ = HI_UINT16(pRec->maxReportInt);

				if(zcl_analogDataType(pRec->dataType)){
					u16 dataLen = zcl_getAttrSize(pRec->dataType, pRec->reportableChange);
					memcpy(pBuf, pRec->reportableChange, dataLen);
					pBuf += dataLen;
				}
			}else{
				*pBuf++ = LO_UINT16(pRec->timeoutPeriod);
				*pBuf++ = HI_UINT16(pRec->timeoutPeriod);
			}
		}
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_READ_REPORT_CFG_RSP, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_report(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 direction, u8 seqNo, u16 clusterId, u16 attrID, u8 dataType, u8 *pData)
{
    u16 len = 0;
    u8 attrSize = zcl_getAttrSize(dataType, pData);

    len += 2 + 1;	//attrID + data type
    len += attrSize;

    u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(attrID);
	*pBuf++ = HI_UINT16(attrID);
	*pBuf++ = dataType;
	memcpy(pBuf, pData, attrSize);

    u32 manuCode = 0;
#if (__PROJECT_ONOFFSWITCH__)
    if(attrID ==  ZCLATTRID_RESERVED_XIAOMI){
    	manuCode = 0x80001234;
    }
#endif
	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_REPORT, FALSE, direction, disableDefaultRsp, manuCode, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ zclCfgReportCmd_t *zcl_parseInCfgReportCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 direction;
	u8 dataType;
	u8 numAttr = 0;
	u8 reportChangeLen;
	u16 dataLen = 0;
	zclCfgReportRec_t *pCfgReportRec = NULL;

	while(pBuf < (pCmd->pData + pCmd->dataLen)){
		numAttr++;

		direction = *pBuf++;
		pBuf += 2;

		if(direction == ZCL_SEND_ATTR_REPORTS){
			dataType = *pBuf++;
			pBuf += 4;

			if(zcl_analogDataType(dataType)){
				reportChangeLen = zcl_getDataTypeLen(dataType);
				pBuf += reportChangeLen;

				dataLen += reportChangeLen;
			}else{
				pBuf++;
			}
		}else{
			pBuf += 2;
		}
	}

	u8 len = sizeof(zclCfgReportCmd_t) + numAttr * sizeof(zclCfgReportRec_t);
	zclCfgReportCmd_t *p = (zclCfgReportCmd_t *)ev_buf_allocate(len + dataLen);
	if(p != NULL){
		memset((u8 *)p , 0, len + dataLen);

		pBuf = pCmd->pData;
		u8 *pData = (u8 *)((u8 *)p + len);

		p->numAttr = numAttr;

		for(u8 i = 0; i < numAttr; i++){
			pCfgReportRec = &(p->attrList[i]);

			pCfgReportRec->direction = *pBuf++;
			pCfgReportRec->attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
			if(pCfgReportRec->direction == ZCL_SEND_ATTR_REPORTS){
				pCfgReportRec->dataType = *pBuf++;
				pCfgReportRec->minReportInt = BUILD_U16(pBuf[0], pBuf[1]);
				pBuf += 2;
				pCfgReportRec->maxReportInt = BUILD_U16(pBuf[0], pBuf[1]);
				pBuf += 2;

				if(zcl_analogDataType(pCfgReportRec->dataType)){
					zcl_analogDataBuild(pCfgReportRec->dataType, pData, pBuf);
					pCfgReportRec->reportableChange = pData;

					reportChangeLen = zcl_getDataTypeLen(pCfgReportRec->dataType);
					pBuf += reportChangeLen;

					pData += reportChangeLen;
				}
			}else{
				pCfgReportRec->timeoutPeriod = BUILD_U16(pBuf[0], pBuf[1]);
				pBuf += 2;
			}
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_configureReporting(u8 endpoint, u16 profileId, u16 clusterId, zclCfgReportRec_t *pCfgReportRec)
{
	zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, clusterId, pCfgReportRec->attrID);
	if(!pAttrEntry){
		return ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}

	if(pAttrEntry->type != pCfgReportRec->dataType){
		return ZCL_STA_INVALID_DATA_TYPE;
	}

	if(!(pAttrEntry->access & ACCESS_CONTROL_REPORTABLE)){
		return ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}

	if(pCfgReportRec->direction == ZCL_EXPECT_ATTR_REPORTS){
		//TODO: timeout period
		return ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}

	reportCfgInfo_t *pEntry = zcl_reportCfgInfoEntryFind(endpoint, clusterId, pCfgReportRec->attrID);

	if(pCfgReportRec->maxReportInt == 0xFFFF){
		/* the configuration information for that attribute need not be maintained. */
		if(!pEntry){
			return ZCL_STA_NOT_FOUND;
		}
	}else if(pCfgReportRec->maxReportInt == 0){
		if(pCfgReportRec->minReportInt == 0xFFFF){
			/* revert to default reporting configuration. */
			if(!pEntry){
				return ZCL_STA_NOT_FOUND;
			}else{
				zcl_reportCfgInfoEntryRst(pEntry);
				return ZCL_STA_SUCCESS;
			}
		}
	}

	if(!pEntry){
		pEntry = zcl_reportCfgInfoEntryFreeGet();
		if(!pEntry){
			return ZCL_STA_INSUFFICIENT_SPACE;
		}
	}

	zcl_reportCfgInfoEntryUpdate(pEntry, endpoint, profileId, clusterId, pCfgReportRec);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_configReportHandler(zclIncoming_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u16 profileId = pCmd->msg->indInfo.profile_id;
	u16 clusterId = pCmd->msg->indInfo.cluster_id;
	u8 endpoint = pCmd->msg->indInfo.dst_ep;
	u8 cfgReportChange = 0;

	/* Parse In Configure Report Command */
	zclCfgReportCmd_t *pCfgReportCmd = zcl_parseInCfgReportCmd(pCmd);
	if(pCfgReportCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pCfgReportCmd;
	}

	if(pCfgReportCmd->numAttr == 0){
		return ZCL_STA_INVALID_FIELD;
	}

	u8 len = sizeof(zclCfgReportRspCmd_t) + pCfgReportCmd->numAttr * sizeof(zclCfgReportStatus_t);
	zclCfgReportRspCmd_t *pCfgReportRspCmd = (zclCfgReportRspCmd_t *)ev_buf_allocate(len);
	if(!pCfgReportRspCmd){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	pCfgReportRspCmd->numAttr = pCfgReportCmd->numAttr;

	for(u8 i = 0; i < pCfgReportCmd->numAttr; i++){
		zclCfgReportRec_t *pCfgReportRec = &(pCfgReportCmd->attrList[i]);

		status = zcl_configureReporting(endpoint, profileId, clusterId, pCfgReportRec);
		if(status == ZCL_STA_SUCCESS){
			cfgReportChange = 1;
		}

		pCfgReportRspCmd->attrList[i].status = status;
		pCfgReportRspCmd->attrList[i].attrID = pCfgReportRec->attrID;
		pCfgReportRspCmd->attrList[i].direction = pCfgReportRec->direction;
	}

	if(cfgReportChange){
		cfgReportChange = 0;
		zcl_reportingTab_save();
	}

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pCmd->msg->indInfo.src_ep;
	dstEp.profileId = pCmd->msg->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pCmd->msg->indInfo.src_short_addr;

	status = zcl_configReportRsp(endpoint, &dstEp, clusterId, TRUE, !pCmd->hdr.frmCtrl.bf.dir, pCmd->hdr.seqNum, pCfgReportRspCmd);

	if(status == ZCL_STA_SUCCESS)
		status = ZCL_STA_CMD_HAS_RESP;

	ev_buf_free((void *)pCfgReportRspCmd);

	return status;
}

_CODE_ZCL_ zclCfgReportRspCmd_t *zcl_parseInCfgReportRspCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 numAttr = pCmd->dataLen / (1 + 1 + 2);	//status + direction + attrID

	u16 len = sizeof(zclCfgReportRspCmd_t) + numAttr * sizeof(zclCfgReportStatus_t);
	zclCfgReportRspCmd_t *p = (zclCfgReportRspCmd_t *)ev_buf_allocate(len);
	if(p != NULL){
		p->numAttr = numAttr;
		for(u8 i = 0; i < p->numAttr; i++){
			p->attrList[i].status = *pBuf++;
			p->attrList[i].direction = *pBuf++;
			p->attrList[i].attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
		}
	}

	return p;
}

_CODE_ZCL_ u8 zcl_configReportRspHandler(zclIncoming_t *pCmd)
{
	zclCfgReportRspCmd_t *pCfgReportRspCmd = NULL;

	/* Parse In Configure Report Response Command */
	pCfgReportRspCmd = zcl_parseInCfgReportRspCmd(pCmd);
	if(pCfgReportRspCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pCfgReportRspCmd;
	}

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ zclReadReportCfgCmd_t *zcl_parseInReadReportCfgCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 numAttr = pCmd->dataLen / (1 + 2);	//direction + attrID

	u16 len = sizeof(zclReadReportCfgCmd_t) + numAttr * sizeof(zclReadReportCfgRec_t);
	zclReadReportCfgCmd_t *p = (zclReadReportCfgCmd_t *)ev_buf_allocate(len);
	if(p != NULL){
		p->numAttr = numAttr;
		for(u8 i = 0; i < numAttr; i++){
			p->attrList[i].direction = *pBuf++;
			p->attrList[i].attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_readReportCfgHandler(zclIncoming_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u16 clusterId = pCmd->msg->indInfo.cluster_id;
	u8 endpoint = pCmd->msg->indInfo.dst_ep;

	/* Parse In Read Report Configure Command */
	zclReadReportCfgCmd_t *pReadReportCfgCmd = zcl_parseInReadReportCfgCmd(pCmd);
	if(pReadReportCfgCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pReadReportCfgCmd;
	}

	u8 len = sizeof(zclReadReportCfgRspCmd_t) + pReadReportCfgCmd->numAttr * sizeof(zclReportCfgRspRec_t);
	zclReadReportCfgRspCmd_t *pReadReportCfgRspCmd = (zclReadReportCfgRspCmd_t *)ev_buf_allocate(len);
	if(!pReadReportCfgRspCmd){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	pReadReportCfgRspCmd->numAttr = pReadReportCfgCmd->numAttr;

	for(u8 i = 0; i < pReadReportCfgCmd->numAttr; i++){
		zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, clusterId, pReadReportCfgCmd->attrList[i].attrID);
		if(pAttrEntry){
			if(pReadReportCfgCmd->attrList[i].direction == ZCL_SEND_ATTR_REPORTS){
				if(!(pAttrEntry->access & ACCESS_CONTROL_REPORTABLE)){
					status = ZCL_STA_UNREPORTABLE_ATTRIBUTE;
				}else{
					reportCfgInfo_t *pEntry = zcl_reportCfgInfoEntryFind(endpoint, clusterId, pReadReportCfgCmd->attrList[i].attrID);
					if(!pEntry){
						status = ZCL_STA_NOT_FOUND;
					}else{
						pReadReportCfgRspCmd->attrList[i].dataType = pEntry->dataType;
						pReadReportCfgRspCmd->attrList[i].minReportInt = pEntry->minInterval;
						pReadReportCfgRspCmd->attrList[i].maxReportInt = pEntry->maxInterval;
						if(zcl_analogDataType(pEntry->dataType)){
							pReadReportCfgRspCmd->attrList[i].reportableChange = pEntry->reportableChange;
						}
					}
				}
			}else{
				//TODO:
				status = ZCL_STA_UNSUPPORTED_ATTRIBUTE;
			}
		}else{
			status = ZCL_STA_UNSUPPORTED_ATTRIBUTE;
		}

		pReadReportCfgRspCmd->attrList[i].status = status;
		pReadReportCfgRspCmd->attrList[i].direction = pReadReportCfgCmd->attrList[i].direction;
		pReadReportCfgRspCmd->attrList[i].attrID = pReadReportCfgCmd->attrList[i].attrID;
	}

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pCmd->msg->indInfo.src_ep;
	dstEp.profileId = pCmd->msg->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pCmd->msg->indInfo.src_short_addr;

	status = zcl_readReportConfigRsp(endpoint, &dstEp, clusterId, TRUE, !pCmd->hdr.frmCtrl.bf.dir, pCmd->hdr.seqNum, pReadReportCfgRspCmd);

	if(status == ZCL_STA_SUCCESS)
		status = ZCL_STA_CMD_HAS_RESP;

	ev_buf_free((u8 *)pReadReportCfgRspCmd);

	return status;
}

_CODE_ZCL_ zclReadReportCfgRspCmd_t *zcl_parseInReadReportCfgRspCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 numAttr = 0;
	u8 status;
	u8 direction;
	u8 dataType;
	u8 reportChangeLen = 0;
	u16 dataLen = 0;
	zclReportCfgRspRec_t *pRec = NULL;

	while(pBuf < (pCmd->pData + pCmd->dataLen)){
		numAttr++;

		status = *pBuf++;
		direction = *pBuf++;
		pBuf += 2;

		if(status == ZCL_STA_SUCCESS){
			if(direction == ZCL_SEND_ATTR_REPORTS){
				dataType = *pBuf++;
				pBuf += 4;

				if(zcl_analogDataType(dataType)){
					reportChangeLen = zcl_getDataTypeLen(dataType);
					pBuf += reportChangeLen;

					dataLen += reportChangeLen;
				}
			}else{
				pBuf += 2;
			}
		}
	}

	u16 len = sizeof(zclReadReportCfgRspCmd_t) + numAttr * sizeof(zclReportCfgRspRec_t);
	zclReadReportCfgRspCmd_t *p = (zclReadReportCfgRspCmd_t *)ev_buf_allocate(len + dataLen);
	if(p != NULL){
		pBuf = pCmd->pData;
		u8 *pData = (u8 *)((u8 *)p + len);

		p->numAttr = numAttr;
		for(u8 i = 0; i < numAttr; i++){
			pRec = &(p->attrList[i]);

			pRec->status = *pBuf++;
			pRec->direction = *pBuf++;
			pRec->attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;

			if(pRec->status == ZCL_STA_SUCCESS){
				if(pRec->direction == ZCL_SEND_ATTR_REPORTS){
					pRec->dataType = *pBuf++;
					pRec->minReportInt = BUILD_U16(pBuf[0], pBuf[1]);
					pBuf += 2;
					pRec->maxReportInt = BUILD_U16(pBuf[0], pBuf[1]);
					pBuf += 2;
					if(zcl_analogDataType(pRec->dataType)){
						zcl_analogDataBuild(pRec->dataType, pData, pBuf);
						pRec->reportableChange = pData;

						reportChangeLen = zcl_getDataTypeLen(pRec->dataType);
						pBuf += reportChangeLen;

						pData += reportChangeLen;
					}
				}else{
					pRec->timeoutPeriod = BUILD_U16(pBuf[0], pBuf[1]);
					pBuf += 2;
				}
			}
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_readReportCfgRspHandler(zclIncoming_t *pCmd)
{
	zclReadReportCfgRspCmd_t *pReadReportCfgRspCmd = NULL;

	/* Parse In Read Report Configure Response Command */
	pReadReportCfgRspCmd = zcl_parseInReadReportCfgRspCmd(pCmd);
	if(pReadReportCfgRspCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pReadReportCfgRspCmd;
	}

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ zclReportCmd_t *zcl_parseInReportCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 numAttr = 0;
	u8 dataType;
	u16 attrDataLen = 0;
	u16 dataLen = 0;
	zclReport_t *pReportRec = NULL;

	while(pBuf < (pCmd->pData + pCmd->dataLen)){
		numAttr++;
		pBuf += 2;

		dataType = *pBuf++;

		attrDataLen = zcl_getAttrSize(dataType, pBuf);
		pBuf += attrDataLen;

		dataLen += attrDataLen;
	}

	u16 len = sizeof(zclReportCmd_t) + numAttr * sizeof(zclReport_t);
	zclReportCmd_t* p = (zclReportCmd_t *)ev_buf_allocate(len + dataLen);
	if(p != NULL){
		pBuf = pCmd->pData;
		u8 *pData = (u8 *)((u8 *)p + len);

		p->numAttr = numAttr;
		for(u8 i = 0; i < numAttr; i++){
			pReportRec = &(p->attrList[i]);

			pReportRec->attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
			pReportRec->dataType = *pBuf++;

			attrDataLen = zcl_getAttrSize(pReportRec->dataType, pBuf);
			memcpy(pData, pBuf, attrDataLen);

			pReportRec->attrData = pData;

			pBuf += attrDataLen;
			pData += attrDataLen;
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_reportHandler(zclIncoming_t *pCmd)
{
	zclReportCmd_t *pReportCmd = NULL;

	/* Parse In Report Command */
	pReportCmd = zcl_parseInReportCmd(pCmd);
	if(pReportCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pReportCmd;
	}

	return ZCL_STA_SUCCESS;
}
#endif

/***************************************************************************
 **************************** Default **************************************
 ***************************************************************************/

_CODE_ZCL_ status_t zcl_sendDfltRsp(zclIncoming_t *inMsg, u8 cmdId, u8 status)
{
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)inMsg->msg;
	u8 srcEp = pApsdeInd->indInfo.dst_ep;
	u8 buf[2];

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	u8 dir = inMsg->hdr.frmCtrl.bf.dir ? 0 : 1;

	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
	dstEp.dstEp = pApsdeInd->indInfo.src_ep;
	dstEp.profileId = pApsdeInd->indInfo.profile_id;
	dstEp.radius = 0;
	dstEp.txOptions = 0;

	buf[0] = cmdId;
	buf[1] = status;

	return zcl_sendCmd(srcEp, &dstEp, pApsdeInd->indInfo.cluster_id, ZCL_CMD_DEFAULT_RSP, FALSE, dir, TRUE, 0, inMsg->hdr.seqNum, 2, buf);
}

_CODE_ZCL_ zclDefaultRspCmd_t *zcl_parseInDftRspCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;

	zclDefaultRspCmd_t *p = (zclDefaultRspCmd_t *)ev_buf_allocate(sizeof(zclDefaultRspCmd_t));
	if(p != NULL){
		p->commandID = *pBuf++;
		p->statusCode = *pBuf;
	}

	return p;
}

_CODE_ZCL_ status_t zcl_dfltRspHandler(zclIncoming_t *pCmd)
{
	zclDefaultRspCmd_t *pDfltRspCmd = NULL;

	/* Parse In Default Response Command */
	pDfltRspCmd = zcl_parseInDftRspCmd(pCmd);
	if(pDfltRspCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pDfltRspCmd;
	}

	return ZCL_STA_SUCCESS;
}

/***************************************************************************
 **************************** Discover *************************************
 ***************************************************************************/
#ifdef ZCL_DISCOVER

_CODE_ZCL_ status_t zcl_discAttrs(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclDiscoverAttrCmd_t *discAttrCmd)
{
	u8 buf[3];

	buf[0] = LO_UINT16(discAttrCmd->startAttr);
	buf[1] = HI_UINT16(discAttrCmd->startAttr);
	buf[2] = discAttrCmd->maxAttrIDs;

	return zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_DISCOVER_ATTR, FALSE, direction, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_discAttrsRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclDiscoverAttrRspCmd_t *discAttrRspCmd)
{
	u16 len = 1;	//complete

	len += discAttrRspCmd->numAttr * (2 + 1);	//attrID + dataType

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = discAttrRspCmd->discComplete;
	for(u8 i = 0; i < discAttrRspCmd->numAttr; i++){
		*pBuf++ = LO_UINT16(discAttrRspCmd->attrList[i].attrID);
		*pBuf++ = HI_UINT16(discAttrRspCmd->attrList[i].attrID);
		*pBuf++ = discAttrRspCmd->attrList[i].dataType;
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_DISCOVER_ATTR_RSP, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ status_t zcl_discAttrsExtended(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclDiscoverAttrCmd_t *discAttrCmd)
{
	u8 buf[3];

	buf[0] = LO_UINT16(discAttrCmd->startAttr);
	buf[1] = HI_UINT16(discAttrCmd->startAttr);
	buf[2] = discAttrCmd->maxAttrIDs;

	return zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_DISCOVER_ATTR_EXTD, FALSE, direction, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_discAttrsExtendedRsp(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u8 disableDefaultRsp, u8 direction, u8 seqNo, zclDiscoverAttrExtRspCmd_t *discAttrExtRspCmd)
{
	u16 len = 1;	//complete

	len += discAttrExtRspCmd->numAttr * (2 + 1 + 1);	//attrID + dataType + accessControl

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = discAttrExtRspCmd->discComplete;
	for(u8 i = 0; i < discAttrExtRspCmd->numAttr; i++){
		*pBuf++ = LO_UINT16(discAttrExtRspCmd->extAttrInfo[i].attrID);
		*pBuf++ = HI_UINT16(discAttrExtRspCmd->extAttrInfo[i].attrID);
		*pBuf++ = discAttrExtRspCmd->extAttrInfo[i].dataType;
		*pBuf++ = discAttrExtRspCmd->extAttrInfo[i].accessControl;
	}

	u8 status = zcl_sendCmd(srcEp, pDstEpInfo, clusterId, ZCL_CMD_DISCOVER_ATTR_EXTD_RSP, FALSE, direction, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return status;
}

_CODE_ZCL_ zclDiscoverAttrCmd_t *zcl_parseInDiscAttrsCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;

	zclDiscoverAttrCmd_t *p = (zclDiscoverAttrCmd_t *)ev_buf_allocate(sizeof(zclDiscoverAttrCmd_t));
	if(p != NULL){
		p->startAttr = BUILD_U16(pBuf[0], pBuf[1]);
		pBuf += 2;
		p->maxAttrIDs = *pBuf;
	}

	return p;
}

_CODE_ZCL_ status_t zcl_discAttrsHandler(zclIncoming_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u16 clusterId = pCmd->msg->indInfo.cluster_id;
	u8 endpoint = pCmd->msg->indInfo.dst_ep;
	u8 numAttr = 0;
	bool status_disComplete = TRUE;

	/* Parse In Discover Attributes Command */
	zclDiscoverAttrCmd_t *pDiscAttrCmd = zcl_parseInDiscAttrsCmd(pCmd);
	if(pDiscAttrCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pDiscAttrCmd;
	}

	/* find out number of attribute supported */
	clusterInfo_t *pCluster = NULL;
	zclAttrInfo_t *pAttrEntry = NULL;

	pCluster = zcl_findCluster(endpoint, clusterId);
	if(pCluster){
		for(u8 i = 0; i < pCluster->attrNum; i++){
			if(numAttr >= pDiscAttrCmd->maxAttrIDs){
				break;
			}

			pAttrEntry = (zclAttrInfo_t *)&pCluster->attrTable[i];

			if(pAttrEntry->id >= pDiscAttrCmd->startAttr){
				numAttr++;
			}
		}
	}

	u16 len = sizeof(zclDiscoverAttrRspCmd_t) + numAttr * sizeof(zclDiscoverAttrInfo_t);
	zclDiscoverAttrRspCmd_t *pDiscAttrRspCmd = (zclDiscoverAttrRspCmd_t *)ev_buf_allocate(len);
	if(!pDiscAttrRspCmd){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	if(numAttr > 0){
		u8 j = 0;
		pCluster = zcl_findCluster(endpoint, clusterId);
		if(pCluster){
			for(u8 i = 0; i < pCluster->attrNum; i++){
				pAttrEntry = (zclAttrInfo_t *)&pCluster->attrTable[i];

				if(pAttrEntry->id >= pDiscAttrCmd->startAttr){
					if(j >= numAttr){
						status_disComplete = FALSE;
						break;
					}
					pDiscAttrRspCmd->attrList[j].attrID = pAttrEntry->id;
					pDiscAttrRspCmd->attrList[j++].dataType = pAttrEntry->type;
				}
			}
		}
	}
	pDiscAttrRspCmd->discComplete = status_disComplete;
	pDiscAttrRspCmd->numAttr = numAttr;

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pCmd->msg->indInfo.src_ep;
	dstEp.profileId = pCmd->msg->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pCmd->msg->indInfo.src_short_addr;

	status = zcl_discAttrsRsp(endpoint, &dstEp, clusterId, TRUE, !pCmd->hdr.frmCtrl.bf.dir, pCmd->hdr.seqNum, pDiscAttrRspCmd);

	if(status == ZCL_STA_SUCCESS)
		status = ZCL_STA_CMD_HAS_RESP;

	ev_buf_free((void *)pDiscAttrRspCmd);

	return status;
}

_CODE_ZCL_ status_t zcl_discAttrsExtendedHandler(zclIncoming_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u16 clusterId = pCmd->msg->indInfo.cluster_id;
	u8 endpoint = pCmd->msg->indInfo.dst_ep;
	u8 numAttr = 0;
	bool status_discComplete = TRUE;

	/* Parse In Discover Attributes Command */
	zclDiscoverAttrCmd_t *pDiscAttrCmd = zcl_parseInDiscAttrsCmd(pCmd);
	if(pDiscAttrCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pDiscAttrCmd;
	}

	/* find out number of attribute supported */
	clusterInfo_t *pCluster = NULL;
	zclAttrInfo_t *pAttrEntry = NULL;

	pCluster = zcl_findCluster(endpoint, clusterId);
	if(pCluster){
		for(u8 i = 0; i < pCluster->attrNum; i++){
			if(numAttr >= pDiscAttrCmd->maxAttrIDs){
				break;
			}

			pAttrEntry = (zclAttrInfo_t *)&pCluster->attrTable[i];

			if(pAttrEntry->id >= pDiscAttrCmd->startAttr){
				numAttr++;
			}
		}
	}

	u16 len = sizeof(zclDiscoverAttrExtRspCmd_t) + numAttr * sizeof(zclExtAttrInfo_t);
	zclDiscoverAttrExtRspCmd_t *pDiscAttrExtRspCmd = (zclDiscoverAttrExtRspCmd_t *)ev_buf_allocate(len);
	if(!pDiscAttrExtRspCmd){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	if(numAttr > 0){
		u8 j = 0;
		pCluster = zcl_findCluster(endpoint, clusterId);
		if(pCluster){
			for(u8 i = 0; i < pCluster->attrNum; i++){
				pAttrEntry = (zclAttrInfo_t *)&pCluster->attrTable[i];

				if(pAttrEntry->id >= pDiscAttrCmd->startAttr){
					if(j >= numAttr){
						status_discComplete = FALSE;
						break;
					}
					pDiscAttrExtRspCmd->extAttrInfo[j].attrID = pAttrEntry->id;
					pDiscAttrExtRspCmd->extAttrInfo[j].dataType = pAttrEntry->type;
					pDiscAttrExtRspCmd->extAttrInfo[j++].accessControl = pAttrEntry->access & ACCESS_CONTROL_EXT_MASK;
				}
			}
		}
	}
	pDiscAttrExtRspCmd->discComplete = status_discComplete;
	pDiscAttrExtRspCmd->numAttr = numAttr;

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pCmd->msg->indInfo.src_ep;
	dstEp.profileId = pCmd->msg->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pCmd->msg->indInfo.src_short_addr;

	status = zcl_discAttrsExtendedRsp(endpoint, &dstEp, clusterId, TRUE, !pCmd->hdr.frmCtrl.bf.dir, pCmd->hdr.seqNum, pDiscAttrExtRspCmd);

	if(status == ZCL_STA_SUCCESS)
		status = ZCL_STA_CMD_HAS_RESP;

	ev_buf_free((void *)pDiscAttrExtRspCmd);

	return status;
}

_CODE_ZCL_ zclDiscoverAttrRspCmd_t *zcl_parseInDiscAttrsRspCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 numAttr = (pCmd->dataLen - 1 /* complete */) / (2 + 1);	//attrID + dataType

	u16 len = sizeof(zclDiscoverAttrRspCmd_t) + numAttr * sizeof(zclDiscoverAttrInfo_t);
	zclDiscoverAttrRspCmd_t *p = (zclDiscoverAttrRspCmd_t *)ev_buf_allocate(len);
	if(p != NULL){
		p->discComplete = *pBuf++;
		p->numAttr = numAttr;
		for(u8 i = 0; i < numAttr; i++){
			p->attrList[i].attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
			p->attrList[i].dataType = *pBuf++;
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_discAttrsRspHandler(zclIncoming_t *pCmd)
{
	zclDiscoverAttrRspCmd_t *pDiscAttrRspCmd = NULL;

	/* Parse In Discover Attributes Response Command */
	pDiscAttrRspCmd = zcl_parseInDiscAttrsRspCmd(pCmd);
	if(pDiscAttrRspCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pDiscAttrRspCmd;
	}

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ zclDiscoverCmdsCmd_t *zcl_parseInDiscCmdsCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;

	zclDiscoverCmdsCmd_t *p = (zclDiscoverCmdsCmd_t *)ev_buf_allocate(sizeof(zclDiscoverCmdsCmd_t));
	if(p != NULL){
		p->startCmdID = *pBuf++;
		p->maxCmdID = *pBuf++;
	}

	return p;
}

_CODE_ZCL_ status_t zcl_discCmdsHandler(zclIncoming_t* pCmd)
{

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ zclDiscoverAttrExtRspCmd_t *zcl_parseInDiscAttrsExtRspCmd(zclIncoming_t *pCmd)
{
	u8 *pBuf = pCmd->pData;
	u8 numAttr = (pCmd->dataLen - 1 /* complete */) / (2 + 1 + 1);	//attrID + dataType + accessControl

	u16 len = sizeof(zclDiscoverAttrExtRspCmd_t) + numAttr * sizeof(zclExtAttrInfo_t);
	zclDiscoverAttrExtRspCmd_t *p = (zclDiscoverAttrExtRspCmd_t *)ev_buf_allocate(len);
	if(p != NULL){
		p->discComplete = *pBuf++;
		p->numAttr = numAttr;
		for(u8 i = 0; i < numAttr; i++){
			p->extAttrInfo[i].attrID = BUILD_U16(pBuf[0], pBuf[1]);
			pBuf += 2;
			p->extAttrInfo[i].dataType = *pBuf++;
			p->extAttrInfo[i].accessControl = *pBuf++;
		}
	}

	return p;
}

_CODE_ZCL_ status_t zcl_discAttrsExtRspHandler(zclIncoming_t *pCmd)
{
	zclDiscoverAttrExtRspCmd_t *pDiscAttrExtRspCmd = NULL;

	/* Parse In Discover Extended Attributes Response Command */
	pDiscAttrExtRspCmd = zcl_parseInDiscAttrsExtRspCmd(pCmd);
	if(pDiscAttrExtRspCmd == NULL){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		pCmd->attrCmd = (void *)pDiscAttrExtRspCmd;
	}

	return ZCL_STA_SUCCESS;
}

#endif


