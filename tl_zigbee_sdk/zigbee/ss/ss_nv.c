/********************************************************************************************************
 * @file     ss_nv.c
 *
 * @brief	 APIs for storing/loading security information from NV
 *
 * @author
 * @date     May. 27, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
#include "../include/zb_common.h"


#ifdef ZB_SECURITY

_CODE_SS_ void zdo_ssInfoSaveToFlash(){
#if NV_ENABLE
	/*
	 * user can process network key (Encrypt) here :
	 * ss_ib.nwkSecurMaterialSet[0].key and ss_ib.nwkSecurMaterialSet[1].key
	 *
	 * */
	nv_flashWriteNew(1, NV_MODULE_APS, NV_ITEM_APS_SSIB, sizeof(ss_ib), (u8*)&ss_ib);
#endif
}

_CODE_SS_ u8 zdo_ssInfoInit(void){
	u8 ret = NV_ITEM_NOT_FOUND;
#if NV_ENABLE
	ret = nv_flashReadNew(1, NV_MODULE_APS, NV_ITEM_APS_SSIB, sizeof(ss_ib), (u8*)&ss_ib);

	/*
	 * user can process network key(Decrypt) here :
	 * ss_ib.nwkSecurMaterialSet[0].key and ss_ib.nwkSecurMaterialSet[1].key
	 *
	 * */
#endif
	return ret;
}

_CODE_SS_ u8 zdo_ssInfoKeyGet(void){
	u8 ret = NV_ITEM_NOT_FOUND;
#if NV_ENABLE
	ss_info_base_t ss;
	ret = nv_flashReadNew(1, NV_MODULE_APS, NV_ITEM_APS_SSIB, sizeof(ss_ib), (u8*)&ss);
	if(ret == NV_SUCC){
		ret = ss.activeSecureMaterialIndex;
	}
#endif
	return ret;
}

#endif

