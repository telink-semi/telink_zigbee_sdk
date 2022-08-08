/********************************************************************************************************
 * @file    gp_sec.h
 *
 * @brief   This is the header file for gp_sec
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

#ifndef GP_SEC_H
#define GP_SEC_H


u8 gpCcmStar(u8 appId, gpdId_t gpdId,
		     u8 gpdfSecKey, u8 gpdfSecLevel,
		     u8 endpoint, u32 secFrameCounter,
		     u8 gpdAsduLen, u8 *gpdAsdu,
		     bool autoComm, bool rxAfterTx,
		     u32 mic, u8 *pKey);

u8 gpdKeyWithTCLK(u8 appId, gpdId_t gpdId,
				  u8 dataLen, u8 *pData,
				  u32 frameCounter, u8 *mic,
				  u8 *pKey, bool enOrDecrypt);

#endif	/* GP_SEC_H */
