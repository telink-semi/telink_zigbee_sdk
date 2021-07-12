/********************************************************************************************************
 * @file	wwah.h
 *
 * @brief	This is the header file for wwah
 *
 * @author	Zigbee Group
 * @date	2021
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
#ifndef WWAH_H
#define WWAH_H


typedef enum{
	WWAH_TYPE_CLIENT,
	WWAH_TYPE_SERVER
}wwah_type_e;

/**
 *  @brief Defined for wwah cluster attributes
 */
typedef struct{
	bool disableOTADowngrades;
	bool mgmtLeaveWithoutRejoinEnabled;
	u8 	 nwkRetryCount;
	u8 	 macRetryCount;
	bool routerCheckInEnabled;
	bool touchlinkInterpanEnabled;
	bool parentClassificationEnabled;
	bool configurationModeEnabled;
	u8	 currentDebugReportID;
	bool tcSecurityOnNwkKeyRotationEnabled;
	u8	 pendingNwkUpdateChannel;
	u16	 pendingNwkUpdatePanID;
	u16	 otaMaxOfflineDuration;
}zcl_wwahAttr_t;


extern const zcl_specClusterInfo_t g_wwahClusterList[];
extern u8 WWAH_CB_CLUSTER_NUM;
extern zcl_wwahAttr_t g_zcl_wwahAttrs;


void wwah_init(wwah_type_e type, af_simple_descriptor_t *simpleDesc);
status_t zcl_wwahCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

#endif /* WWAH_H */
