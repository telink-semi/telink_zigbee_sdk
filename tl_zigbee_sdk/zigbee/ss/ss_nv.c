/********************************************************************************************************
 * @file	ss_nv.c
 *
 * @brief	This is the source file for ss_nv
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
#include "../common/includes/zb_common.h"


#ifdef ZB_SECURITY

_CODE_SS_ void zdo_ssInfoSaveToFlash(void){
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

