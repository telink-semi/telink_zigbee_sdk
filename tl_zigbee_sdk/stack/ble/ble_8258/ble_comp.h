/********************************************************************************************************
 * @file    ble_comp.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef BLE_COMP_H_
#define BLE_COMP_H_


/* for some API name in old version SDK compatible with new version SDK */

///////////////////////////////////controller//////////////////////////////////////
#define blt_ll_exchangeDataLength						blc_ll_exchangeDataLength
#define blc_ll_modifyBluetoothVersion					blc_contr_setBluetoothVersion
#define blc_pm_modefy_brx_early_set                     blc_pm_modify_brx_early_set
#define blc_ll_set_CustomedAdvScanAccessCode            blc_ll_set_CustomizedAdvScanAccessCode

#define	ll_resolvingList_reset							blc_ll_clearResolvingList
#define	ll_resolvingList_add							blc_ll_addDeviceToResolvingList
#define	ll_resolvingList_setAddrResolutionEnable		blc_ll_setAddressResolutionEnable

#define	ll_whiteList_reset								blc_ll_clearWhiteList
#define	ll_whiteList_add								blc_ll_addDeviceToWhiteList
#define	ll_whiteList_delete								blc_ll_removeDeviceFromWhiteList


#define blc_ll_setExtAdvEnable_1                        blc_ll_setExtAdvEnable

/////////////////////////////////////L2CAP/////////////////////////////////////////////
#define blc_l2cap_reg_att_cli_hander					blc_l2cap_reg_att_cli_handler
#define blc_l2cap_reg_att_sig_hander					blc_l2cap_reg_att_sig_handler


////////////////////////////////////host//////////////////////////////////////
#define	blt_att_setEffectiveMtuSize						blc_att_setEffectiveMtuSize
#define blt_att_resetEffectiveMtuSize					blc_att_resetEffectiveMtuSize
#define blc_gatt_pushWriteComand						blc_gatt_pushWriteCommand

#define blm_smp_configParingSecurityInfoStorageAddr		blm_smp_configPairingSecurityInfoStorageAddr
#define	blc_smp_setSecurityParamters					blc_smp_setSecurityParameters

////////////////////////////////////ota//////////////////////////////////////
#define bls_ota_set_fwSize_and_fwBootAddr				blc_ota_setFirmwareSizeAndBootAddress
#define	bls_ota_registerStartCmdCb						blc_ota_registerOtaStartCmdCb
#define	bls_ota_registerVersionReqCb					blc_ota_registerOtaFirmwareVersionReqCb
#define	bls_ota_registerResultIndicateCb				blc_ota_registerOtaResultIndicationCb
#define bls_ota_setTimeout(tm_us)						blc_ota_setOtaProcessTimeout( (tm_us)/1000000 )

////////////////////////////////////APP///////////////////////////////////////
#define blc_app_loadCustomizedParameters 				blc_app_loadCustomizedParameters_normal




/* for some macro name in old version SDK compatible with new version SDK */

///////////////////////////////////controller//////////////////////////////////////
#define BLT_EV_FLAG_RX_DATA_ABANDOM                  	BLT_EV_FLAG_RX_DATA_ABANDON
/////////////////////////////////////L2CAP/////////////////////////////////////////////

////////////////////////////////////host//////////////////////////////////////

#define ATT_RX_MTU_SIZE_MAX								ATT_MTU_MAX_SDK_DFT_BUF

#define GATT_ERR_SERVICE_DISCOVERY_TIEMOUT				GATT_ERR_SERVICE_DISCOVERY_TIMEOUT
#define GAP_EVT_SMP_TK_DISPALY							GAP_EVT_SMP_TK_DISPLAY
#define GAP_EVT_MASK_SMP_TK_DISPALY						GAP_EVT_MASK_SMP_TK_DISPLAY
#define GAP_EVT_MASK_SMP_PARING_BEAGIN                  GAP_EVT_MASK_SMP_PAIRING_BEGIN
#define GAP_EVT_MASK_SMP_PARING_SUCCESS                 GAP_EVT_MASK_SMP_PAIRING_SUCCESS
#define GAP_EVT_MASK_SMP_PARING_FAIL                    GAP_EVT_MASK_SMP_PAIRING_FAIL
#define GAP_EVT_SMP_PARING_BEAGIN		                GAP_EVT_SMP_PAIRING_BEGIN	// Refer to SMP message sequence and event chart above
#define GAP_EVT_SMP_PARING_SUCCESS			            GAP_EVT_SMP_PAIRING_SUCCESS	// Refer to SMP message sequence and event chart above
#define GAP_EVT_SMP_PARING_FAIL			                GAP_EVT_SMP_PAIRING_FAIL

#define IO_CAPABLITY_DISPLAY_ONLY						IO_CAPABILITY_DISPLAY_ONLY
#define IO_CAPABLITY_DISPLAY_YESNO						IO_CAPABILITY_DISPLAY_YESNO
#define IO_CAPABLITY_KEYBOARD_ONLY						IO_CAPABILITY_KEYBOARD_ONLY
#define IO_CAPABLITY_NO_IN_NO_OUT						IO_CAPABILITY_NO_IN_NO_OUT
#define	IO_CAPABLITY_KEYBOARD_DISPLAY					IO_CAPABILITY_KEYBOARD_DISPLAY

#define PARING_FAIL_REASON_PASSKEY_ENTRY				PAIRING_FAIL_REASON_PASSKEY_ENTRY
#define PARING_FAIL_REASON_OOB_NOT_AVAILABLE			PAIRING_FAIL_REASON_OOB_NOT_AVAILABLE
#define PARING_FAIL_REASON_AUTH_REQUIRE					PAIRING_FAIL_REASON_AUTH_REQUIRE
#define PARING_FAIL_REASON_CONFIRM_FAILED				PAIRING_FAIL_REASON_CONFIRM_FAILED
#define PARING_FAIL_REASON_PARING_NOT_SUPPORTED			PAIRING_FAIL_REASON_PAIRING_NOT_SUPPORTED
#define PARING_FAIL_REASON_ENCRYPT_KEY_SIZE				PAIRING_FAIL_REASON_ENCRYPT_KEY_SIZE
#define PARING_FAIL_REASON_CMD_NOT_SUPPORT				PAIRING_FAIL_REASON_CMD_NOT_SUPPORT
#define PARING_FAIL_REASON_UNSPECIFIED_REASON			PAIRING_FAIL_REASON_UNSPECIFIED_REASON
#define PARING_FAIL_REASON_REPEATED_ATTEMPT				PAIRING_FAIL_REASON_REPEATED_ATTEMPT
#define PARING_FAIL_REASON_INVAILD_PARAMETER			PAIRING_FAIL_REASON_INVAILD_PARAMETER
#define PARING_FAIL_REASON_DHKEY_CHECK_FAIL				PAIRING_FAIL_REASON_DHKEY_CHECK_FAIL
#define PARING_FAIL_REASON_NUMUERIC_FAILED				PAIRING_FAIL_REASON_NUMUERIC_FAILED
#define PARING_FAIL_REASON_BREDR_PARING					PAIRING_FAIL_REASON_BREDR_PAIRING
#define PARING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW		PAIRING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW
#define PARING_FAIL_REASON_PARING_TIEMOUT				PAIRING_FAIL_REASON_PAIRING_TIEMOUT
#define PARING_FAIL_REASON_CONN_DISCONNECT				PAIRING_FAIL_REASON_CONN_DISCONNECT
#define PARING_FAIL_REASON_SUPPORT_NC_ONLY				PAIRING_FAIL_REASON_SUPPORT_NC_ONLY

#define DATA_FRAGM_ALLOWED								DATA_FRAGMENT_ALLOWED

/* for some data structure name in old version SDK compatible with new version SDK */
#define gap_smp_paringSuccessEvt_t						gap_smp_pairingSuccessEvt_t



#define event_disconnection_t         					hci_disconnectionCompleteEvt_t


/* these old ATT APIs not recommended to use anymore */
extern void att_req_read (u8 *p, u16 attHandle);
extern void att_req_read_blob (u8 *p, u16 attHandle, u16 offset);
extern void att_req_read_multi (u8 *p, u16* h, u8 n);
extern void att_req_write (u8 *p, u16 attHandle, u8 *buf, int len);
extern void att_req_signed_write_cmd (u8 *p, u16 attHandle, u8 *pd, int n, u8 *sign);
extern void att_req_prep_write (u8 *p, u16 attHandle, u8 *pd, u16 offset, int len);
extern void att_req_write_cmd (u8 *p, u16 attHandle, u8 *buf, int len);
extern void att_req_read_by_type (u8 *p, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);
extern void att_req_read_by_group_type (u8 *p, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);
extern void att_req_find_info(u8 *p, u16 start_attHandle, u16 end_attHandle);
extern void att_req_find_by_type (u8 *p, u16 start_attHandle, u16 end_attHandle, u8 *uuid, u8* attr_value, int len);

extern ble_sts_t  	bls_att_pushNotifyData (u16 attHandle, u8 *p, int len);
extern ble_sts_t	bls_att_pushIndicateData (u16 attHandle, u8 *p, int len);


/* the OTA API not recommended to use anymore */
extern void bls_ota_clearNewFwDataArea(void);

#endif /* BLE_COMP_H_ */
