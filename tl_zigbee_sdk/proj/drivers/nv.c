/********************************************************************************************************
 * @file     nv.c
 *
 * @brief	 nv flash interface function file
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"

#if (FLASH_PROTECT)
const u8 protect_flash_cmd = FLASH_PROTECT_CMD;
#endif

#if FLASH_SIZE_1M
u32 g_u32MacFlashAddr = MAC_ADDR_1M_FLASH;
u32 g_u32CfgFlashAddr = CFG_ADDR_1M_FLASH;
#else
u32 g_u32MacFlashAddr = MAC_ADDR_512K_FLASH;
u32 g_u32CfgFlashAddr = CFG_ADDR_512K_FLASH;
#endif

/*********************************************************************
 * @fn      internalFlashSizeCheck
 *
 * @brief   This function is provided to get and update to the correct flash address
 * 			where are stored the right MAC address and pre-configured parameters.
 * 			NOTE: It should be called before ZB_RADIO_INIT().
 *
 * @param   None
 *
 * @return  None
 */
void internalFlashSizeCheck(void){
#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	u8 mid[4] = {0};
	flash_read_mid(mid);

	if( ((mid[2] != FLASH_CAP_SIZE_512K) && (mid[2] != FLASH_CAP_SIZE_1M)) ||
		((g_u32MacFlashAddr == MAC_ADDR_1M_FLASH) && (mid[2] != FLASH_CAP_SIZE_1M)) ){
		/* Flash space not matched. */
		while(1);
	}

	if( (g_u32MacFlashAddr == MAC_ADDR_512K_FLASH) && (mid[2] == FLASH_CAP_SIZE_1M) ){
		g_u32MacFlashAddr = MAC_ADDR_1M_FLASH;
		g_u32CfgFlashAddr = CFG_ADDR_1M_FLASH;
	}
#endif
}

nv_sts_t nv_index_update(u16 id, u8 opSect, u16 opItemIdx, nv_info_idx_t *idx){
	u32 idxStartAddr = MODULE_IDX_START(id, opSect);
	flash_write(idxStartAddr+opItemIdx*sizeof(nv_info_idx_t), sizeof(nv_info_idx_t), (u8 *)idx);
	return NV_SUCC;
}

nv_sts_t nv_index_read(u16 id, u8 itemId, u16 len, u8 opSect, u32 totalItemNum, u16 *idxNo){
	nv_sts_t ret = NV_SUCC;
	u32 idxTotalNum = totalItemNum;
	u32 idxStartAddr = MODULE_IDX_START(id, opSect) + (idxTotalNum) * sizeof(nv_info_idx_t);
	u16 opItemIdx = idxTotalNum;
	u16 readIdxNum = 4;
	nv_info_idx_t idxInfo[4];
	u8 jump = 0;
	u8 itemFlag = ITEM_FIELD_VALID;
	u16 itemTotalSize = ITEM_TOTAL_LEN(len);

	if(itemId == ITEM_FIELD_IDLE){
		itemFlag = ITEM_FIELD_IDLE;
		itemTotalSize = 0xffff;
	}

	ret = NV_ITEM_NOT_FOUND;
	s32 i = 0;
	while(idxTotalNum > 0){
		readIdxNum = (idxTotalNum > 4) ? 4 : idxTotalNum;
		idxStartAddr -= readIdxNum * sizeof(nv_info_idx_t);
		opItemIdx -= readIdxNum;
		idxTotalNum -= readIdxNum;
		flash_read(idxStartAddr, readIdxNum * sizeof(nv_info_idx_t), (u8*)idxInfo);
		for(i = readIdxNum - 1; i >= 0; i--){
			if(itemId != ITEM_FIELD_IDLE){
				if(idxInfo[i].usedState == itemFlag && idxInfo[i].size == itemTotalSize && idxInfo[i].itemId == itemId){
					jump = 1;
					break;
				}
			}else{
				if(idxInfo[i].usedState != itemFlag || idxInfo[i].size != itemTotalSize || idxInfo[i].itemId != itemId){
					jump = 1;
					break;
				}
			}
		}
		if(jump){
			opItemIdx += i;
			*idxNo = opItemIdx;
			ret = NV_SUCC;
			break;
		}
	}

	return ret;
}


nv_sts_t nv_sector_read(u16 id, u8 sectTotalNum, nv_sect_info_t *sectInfo){
	nv_sts_t ret = NV_SUCC;
	nv_sect_info_t s;
	u32 moduleStartAddr = MODULES_START_ADDR(id);
	s32 i = 0;

	for(i = 0; i < sectTotalNum; i++){
		flash_read(moduleStartAddr, sizeof(nv_sect_info_t), (u8 *)&s);
		if(s.usedFlag == NV_SECTOR_VALID && s.idName == id){
			memcpy(sectInfo, &s, sizeof(nv_sect_info_t));
			break;
		}
		moduleStartAddr += NV_SECTOR_SIZE(id);
	}
	if( i == sectTotalNum){
		ret = NV_ITEM_NOT_FOUND;
	}
	return ret;
}

nv_sts_t nv_write_item(u16 id, u8 itemId, u8 opSect, u16 opItemIdx, u16 len, u8 *buf){
	/* write index to flash */
	nv_info_idx_t idxInfo;
	memset((u8 *)&idxInfo, 0, sizeof(nv_info_idx_t));
	u32 idxStartAddr = MODULE_IDX_START(id, opSect);
	u32 offset = 0;

	if(opItemIdx == 0){
		offset = MODULE_CONTEXT_START(id, opSect, len);
	}else{
		flash_read(idxStartAddr + (opItemIdx - 1)*sizeof(nv_info_idx_t), sizeof(nv_info_idx_t), (u8*)&idxInfo);
		offset = ((idxInfo.offset + idxInfo.size + 3) & (~0x03));
	}
	idxInfo.usedState = ITEM_FIELD_OPERATION;
	idxInfo.size = len + sizeof(itemHdr_t);
	idxInfo.offset = offset;
	idxInfo.itemId = itemId;

	flash_write(idxStartAddr+opItemIdx*sizeof(nv_info_idx_t), sizeof(nv_info_idx_t), (u8 *)&idxInfo);

	/* write context to flash */
	if((u32)buf < 0x808000){
		/* if need copy th data from flash, read it into ram, and then write it to flash */
		u8 copyLen = 48;
		u8 *pTemp = (u8 *)ev_buf_allocate(copyLen);
		if(pTemp){
			u8 wLen = 0;
			u16 toalLen = idxInfo.size; //len;
			u32 sAddr = (u32)buf;
			u32 dAddr = idxInfo.offset;
			while(toalLen > 0){
				wLen = (toalLen > copyLen) ? copyLen : toalLen;
				flash_read(sAddr, wLen, pTemp);
				flash_write(dAddr, wLen, pTemp);
				toalLen -= wLen;
				sAddr += wLen;
				dAddr += wLen;
			}
			ev_buf_free(pTemp);
		}else{
			return NV_NOT_ENOUGH_SAPCE;
		}
	}else{
		u32 checkSum = 0;
		itemHdr_t hdr;
		u32 payloadAddr = idxInfo.offset+sizeof(itemHdr_t);

		for(s32 i = 0; i < len; i++){
			checkSum += buf[i];
		}
		flash_write(payloadAddr, len, buf);

		/* check the checksum */
		u8 vCheck[16];
		u16 tLen = len;
		u16 unitLen;
		u32 checkSumRead = 0;
		while(tLen > 0){
			unitLen = (tLen > 16) ? 16 : tLen;
			flash_read(payloadAddr, unitLen, vCheck);
			for(s32 i = 0; i < unitLen; i++){
				checkSumRead += vCheck[i];
			}
			tLen -= unitLen;
			payloadAddr += unitLen;
		}

		if(checkSum == checkSumRead){
			hdr.itemId = itemId;
			hdr.size = len;
			hdr.used = ITEM_FIELD_VALID;
			hdr.checkSum = checkSum;
			flash_write(idxInfo.offset, sizeof(itemHdr_t), (u8*)&hdr);
		}else{
			return NV_CHECK_SUM_ERROR;
		}
	}

	/* check the correct, if correct, change the state in the index */
	u8 staOffset = OFFSETOF(nv_info_idx_t, usedState);
	idxInfo.usedState = ITEM_FIELD_VALID;
	flash_write(idxStartAddr+opItemIdx*sizeof(nv_info_idx_t)+staOffset, 1, (u8 *)&idxInfo.usedState);

	return NV_SUCC;
}


nv_sts_t nv_flashReadByIndex(u8 id, u8 itemId, u8 opSect, u16 opIdx, u16 len, u8 *buf){
	nv_sts_t ret = NV_SUCC;
	u32 idxStartAddr = MODULE_IDX_START(id, opSect);
	nv_info_idx_t idx;
	flash_read(idxStartAddr + opIdx * sizeof(nv_info_idx_t), sizeof(nv_info_idx_t), (u8 *)&idx);
	if(idx.usedState != ITEM_FIELD_VALID){
		return NV_ITEM_NOT_FOUND;
	}

	itemHdr_t hdr;
	flash_read(idx.offset, sizeof(itemHdr_t), (u8*)&hdr);
	if(hdr.size == len && hdr.used == ITEM_FIELD_VALID && hdr.itemId == itemId){
		flash_read(idx.offset + sizeof(itemHdr_t), len, buf);
	}else{
		ret = NV_ITEM_NOT_FOUND;
	}
	return ret;
}

nv_sts_t nv_itemDeleteByIndex(u8 id, u8 itemId, u8 opSect, u16 opIdx){
	nv_sts_t ret = NV_SUCC;
	u32 idxStartAddr = MODULE_IDX_START(id, opSect);
	nv_info_idx_t idx;
	u8 staOffset = OFFSETOF(nv_info_idx_t, usedState);
	idx.usedState = ITEM_FIELD_INVALID;
	flash_write(idxStartAddr+opIdx*sizeof(nv_info_idx_t)+staOffset, 1, &(idx.usedState));
	return ret;
}


nv_sts_t nv_flashWriteNew(u8 single, u16 id, u8 itemId, u16 len, u8 *buf){
	nv_sts_t ret = NV_SUCC;
	nv_sect_info_t sectInfo;
	u8 opSect = 0;
	u32 moduleStartAddr = MODULES_START_ADDR(id);

	s32 i = 0;

	/* check item length, if  */
	if(NV_SECTOR_SIZE(id) < ITEM_TOTAL_LEN(len) + MODULE_INFO_SIZE(id)){
		return NV_NOT_ENOUGH_SAPCE;
	}

	/* search valid operation sub-sector */
	ret = nv_sector_read(id, MODULE_SECTOR_NUM, &sectInfo);
	if(ret != NV_SUCC){
#if 0
		for(s32 j = 0; j < MODULE_SECTOR_NUM; j++){
			flash_erase(moduleStartAddr + j * FLASH_SECTOR_SIZE);
		}
#endif
		opSect = 0;
	}else{
		opSect = sectInfo.opSect;
	}


	/*
	 * search the index, find a valid space to write
	 * read 4 indexes once */
	u32 idxStartAddr = MODULE_IDX_START(id, opSect);
	u16 wItemIdx = 0;
	nv_info_idx_t idxInfo[4];
	u8 sectorUpdate = 0;
	s32 idxTotalNum = MODULE_IDX_NUM(id);
	ret = nv_index_read(id, ITEM_FIELD_IDLE, len, opSect, idxTotalNum, &wItemIdx);
	if(ret == NV_SUCC){
		flash_read(idxStartAddr + wItemIdx * sizeof(nv_info_idx_t), sizeof(nv_info_idx_t), (u8 *)idxInfo );
		if((wItemIdx == idxTotalNum - 1) ||
				(idxInfo[0].offset + idxInfo[0].size + ITEM_TOTAL_LEN(len)) > MODULE_SECT_END(id, opSect)){
			sectorUpdate = 1;
		}else{
			wItemIdx += 1;
		}
	}

	u8 oldSect = opSect;
	if(sectorUpdate){
		wItemIdx = 0;
		opSect = (opSect + 1) & (MODULE_SECTOR_NUM - 1);

		u8 nv_realSectNum = NV_SECTOR_SIZE(id)/FLASH_SECTOR_SIZE;
		u32 eraseAddr = moduleStartAddr + opSect * NV_SECTOR_SIZE(id);
		for(s32 k = 0; k < nv_realSectNum; k++){
			//flash_erase(moduleStartAddr + opSect * FLASH_SECTOR_SIZE);
			flash_erase(eraseAddr);
			eraseAddr += FLASH_SECTOR_SIZE;
		}

		u32 sizeusedAddr = 0;
		u8 readIdxNum = 0;
		sizeusedAddr = MODULE_CONTEXT_START(id, opSect, len);
		idxStartAddr = MODULE_IDX_START(id, oldSect);
		idxTotalNum = MODULE_IDX_NUM(id);
		/* copy valid items to new sector */
		while(idxTotalNum > 0){
			readIdxNum = (idxTotalNum > 4) ? 4 : idxTotalNum;
			flash_read(idxStartAddr, readIdxNum * sizeof(nv_info_idx_t), (u8*)idxInfo);
			for( i = 0; i < readIdxNum; i++){
				if(idxInfo[i].usedState == ITEM_FIELD_VALID &&
					(idxInfo[i].itemId != itemId || ((idxInfo[i].itemId == itemId) && !single))){
					ret = nv_write_item(id, idxInfo[i].itemId, opSect, wItemIdx, idxInfo[i].size-sizeof(itemHdr_t), (u8*)idxInfo[i].offset);
					if(ret != NV_SUCC){
						return ret;
					}
					sizeusedAddr += idxInfo[i].size;
					sizeusedAddr = ((sizeusedAddr + 0x03) & (~0x03));
					wItemIdx += 1;
				}
			}
			idxTotalNum -= readIdxNum;
			idxStartAddr += readIdxNum * sizeof(nv_info_idx_t);
		}

		idxTotalNum = MODULE_IDX_NUM(id);
		if(wItemIdx == idxTotalNum || (sizeusedAddr + ITEM_TOTAL_LEN(len)) > MODULE_SECT_END(id, opSect)){
			return NV_NOT_ENOUGH_SAPCE;
		}
	}


	/* search the same item id, need to delete after update */
	u8 deleteItemEn = 0;
	u16 deleteIdx = 0;
	idxTotalNum = MODULE_IDX_NUM(id);
	if(single){
		if(NV_SUCC == nv_index_read(id, itemId, len, opSect, idxTotalNum, &deleteIdx)){
			deleteItemEn = 1;
		}
	}

	/* sector is full, and then need write in the another sector */
	ret = nv_write_item(id, itemId, opSect, wItemIdx, len, buf);

	/* the last item set as invalid */
	idxStartAddr = MODULE_IDX_START(id, opSect);
	if(deleteItemEn){
		u8 staOffset = OFFSETOF(nv_info_idx_t, usedState);
		idxInfo[0].usedState = ITEM_FIELD_INVALID;
		flash_write(idxStartAddr+deleteIdx*sizeof(nv_info_idx_t)+staOffset, 1, &(idxInfo[0].usedState));
	}


	if(ret == NV_SUCC){
		if(sectorUpdate){
			sectInfo.idName = id;
			sectInfo.opSect = opSect;
			sectInfo.usedFlag = NV_SECTOR_INVALID;
			flash_write(MODULE_SECT_START(id, oldSect), sizeof(nv_sect_info_t), (u8*)&sectInfo);

			sectInfo.usedFlag = NV_SECTOR_VALID;
			sectInfo.opSect = opSect;
			flash_write(MODULE_SECT_START(id, opSect), sizeof(nv_sect_info_t), (u8*)&sectInfo);
		}else{
			 if(wItemIdx == 0){
				sectInfo.idName = id;
				sectInfo.usedFlag = NV_SECTOR_VALID;
				sectInfo.opSect = opSect;
				flash_write(MODULE_SECT_START(id, opSect), sizeof(nv_sect_info_t), (u8*)&sectInfo);
			 }
		}
	}

	return ret;
}


nv_sts_t nv_flashReadNew(u8 single, u8 id, u8 itemId, u16 len, u8 *buf){
	nv_sts_t ret = NV_SUCC;
	nv_sect_info_t sectInfo;
	u8 opSect = 0;
	s32 idxTotalNum = 0;
	u16 opIdx = 0;

	ret = nv_sector_read(id, MODULE_SECTOR_NUM, &sectInfo);
	if(ret != NV_SUCC){
		return ret;
	}

	opSect = sectInfo.opSect;
	idxTotalNum = MODULE_IDX_NUM(id);

	ret = NV_ITEM_NOT_FOUND;
	ret = nv_index_read(id, itemId, len, opSect, idxTotalNum, &opIdx);
	if(single){
		if(ret == NV_SUCC){
			ret = nv_flashReadByIndex(id, itemId, opSect, opIdx, len, buf);
		}
	}else{
		itemIfno_t *pInfo = (itemIfno_t*)buf;
		pInfo->opIndex = opIdx;
		pInfo->opSect = opSect;
	}

	return ret;
}


nv_sts_t nv_nwkFrameCountSearch(u8 id, u8 opSect, u32 *frameCount, u32 *validAddr){
	nv_sts_t ret = NV_ITEM_NOT_FOUND;
	u16 frmCntTotalNum = FRAMECOUNT_NUM_PER_SECT();
	u32 sizeUsed = frmCntTotalNum * 4;
	u32 pageUsed = (sizeUsed & 0xff) ? sizeUsed/FLASH_PAGE_SIZE+1 : sizeUsed/FLASH_PAGE_SIZE;
	u16 checkSize = 0;
	*validAddr = FRAMECOUNT_PAYLOAD_START(opSect);

	u32 *opPageAddr = (u32 *)(MODULE_SECT_END(id, opSect) - FLASH_PAGE_SIZE);
	s32 i = 0;
	u32 checkValid[4];

	for(i = 0; i < pageUsed-1; i++){
		flash_read((u32)opPageAddr, 4, (u8 *)checkValid);
		if(checkValid[0] != 0xffffffff){
			checkSize = FLASH_PAGE_SIZE;
			break;
		}else{
			*validAddr = (u32)opPageAddr;	//???validAddr = opPageAddr;
		}
		opPageAddr -= FLASH_PAGE_SIZE/4;
	}
	if( i == pageUsed-1){
		opPageAddr = (u32 *)(FRAMECOUNT_PAYLOAD_START(opSect));
		checkSize = (FLASH_PAGE_SIZE - sizeof(nv_sect_info_t)) & (~0x03);
	}

	opPageAddr = (u32 *)(((u32)opPageAddr + FLASH_PAGE_SIZE) & (~(FLASH_PAGE_SIZE - 1)));
	u8 unitSize = 16;
	while(checkSize){
		u8 readSize = (checkSize > unitSize) ? unitSize : checkSize;
		opPageAddr -= readSize/4;
		checkSize -= readSize;

		flash_read((u32)opPageAddr, readSize, (u8 *)checkValid);
		if(checkValid[0] != 0xffffffff){
			*frameCount = checkValid[0];
			for(s32 i = 1; i < readSize/4; i++){
				if(checkValid[i] != 0xffffffff){
					*frameCount = checkValid[i];
					if(i == (readSize/4 - 1)){
						*validAddr = (u32)(opPageAddr + i + 1);
						return NV_SUCC;
					}
				}else{
					*validAddr = (u32)(opPageAddr + i);
					return NV_SUCC;
				}
			}
		}else{
			*validAddr = (u32)opPageAddr;
		}
	}

	return ret;
}


nv_sts_t nv_nwkFrameCountSaveToFlash(u32 frameCount){
	nv_sts_t ret = NV_SUCC;
	nv_sect_info_t sectInfo;
	u8 opSect = 0;
	u8 id = NV_MODULE_NWK_FRAME_COUNT;
	u32 moduleStartAddr = MODULES_START_ADDR(id);

	/* search valid operation sub-sector */
	ret = nv_sector_read(id, MODULE_SECTOR_NUM, &sectInfo);
	if(ret != NV_SUCC){
		for(s32 j = 0; j < MODULE_SECTOR_NUM; j++){
			flash_erase(moduleStartAddr + j * FLASH_SECTOR_SIZE);
		}
		opSect = 0;
	}else{
		opSect = sectInfo.opSect;
	}

	u32 lastFrmCnt;
	u32 wAddr;
	u8 oldSect = opSect;
	u8 sectorUpdate = 0;
	if(NV_SUCC == nv_nwkFrameCountSearch(id, opSect, &lastFrmCnt, &wAddr)){
		if(wAddr == (MODULE_SECT_END(id, opSect))){
			opSect = (opSect + 1) & (MODULE_SECTOR_NUM - 1);
			flash_erase(moduleStartAddr + opSect * FLASH_SECTOR_SIZE);
			flash_write(FRAMECOUNT_PAYLOAD_START(opSect), 4, (u8*)&lastFrmCnt);
			wAddr = FRAMECOUNT_PAYLOAD_START(opSect) + 4;
			sectorUpdate = 1;
		}
	}

	flash_write(wAddr, 4, (u8*)&frameCount);

	if(sectorUpdate || (wAddr == FRAMECOUNT_PAYLOAD_START(opSect))){
		sectInfo.usedFlag = NV_SECTOR_VALID;
		sectInfo.opSect = opSect;
		sectInfo.idName = id;
		flash_write((u32)MODULE_SECT_START(id, opSect), sizeof(nv_sect_info_t), (u8 *)&sectInfo);

		if(sectorUpdate){
			sectInfo.usedFlag = NV_SECTOR_INVALID;
			flash_write((u32)MODULE_SECT_START(id, oldSect), sizeof(nv_sect_info_t), (u8 *)&sectInfo);
		}
	}

	return ret;
}


nv_sts_t nv_nwkFrameCountFromFlash(u32 *frameCount){
	nv_sts_t ret = NV_SUCC;
	u8 id = NV_MODULE_NWK_FRAME_COUNT;
	nv_sect_info_t sectInfo;
	u32 lastFrmCnt;
	u32 wAddr;
	u8 opSect = 0;

	/* search valid operation sub-sector */
	ret = nv_sector_read(id, MODULE_SECTOR_NUM, &sectInfo);
	if(ret != NV_SUCC){
		return ret;
	}
	opSect = sectInfo.opSect;

	ret = nv_nwkFrameCountSearch(id, opSect, &lastFrmCnt, &wAddr);
	if(ret == NV_SUCC){
		*frameCount = lastFrmCnt;
	}

	return ret;
}

nv_sts_t nv_resetModule(u8 modules) {
	u32 eraseAddr = MODULES_START_ADDR(modules);
	u8 sectNumber = NV_SECTOR_SIZE(modules)/FLASH_SECTOR_SIZE;
	for(s32 i = 0; i < MODULE_SECTOR_NUM; i++){
		for(s32 j = 0; j < sectNumber; j++){
			flash_erase(eraseAddr);
			eraseAddr += FLASH_SECTOR_SIZE;
		}
	}
	return SUCCESS;
}


nv_sts_t nv_resetAll(void) {
#if NV_ENABLE
	foreach(i, NV_MAX_MODULS) {
		nv_resetModule(i);
	}
#endif
	return SUCCESS;
}


nv_sts_t nv_resetToFactoryNew(void) {
#if NV_ENABLE
	foreach(i, NV_MAX_MODULS) {
		if(i != NV_MODULE_NWK_FRAME_COUNT){
			nv_resetModule(i);
		}
	}
#endif
	return SUCCESS;
}

nv_sts_t nv_init(u8 rst) {
	if(rst) {
		/* if reset is true, erase all flash for NV */
		nv_resetAll();
	}
	return NV_SUCC;
}
