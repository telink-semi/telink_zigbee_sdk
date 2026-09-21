/********************************************************************************************************
 * @file    smp.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    12,2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef BLE_SMP_H_
#define BLE_SMP_H_

#include "stack/ble/ble_8258/ble_common.h"


/**
 * @brief  SMP type definition: first pairing or connecting back
 */
#define SMP_STANDARD_PAIR   							0
#define SMP_FAST_CONNECT   								1





/**
 * @brief  SMP pairing fail reason definition
 */
#define PAIRING_FAIL_REASON_PASSKEY_ENTRY			0x01
#define PAIRING_FAIL_REASON_OOB_NOT_AVAILABLE		0x02
#define PAIRING_FAIL_REASON_AUTH_REQUIRE			0x03
#define PAIRING_FAIL_REASON_CONFIRM_FAILED			0x04
#define PAIRING_FAIL_REASON_PAIRING_NOT_SUPPORTED	0x05
#define PAIRING_FAIL_REASON_ENCRYPT_KEY_SIZE		0x06
#define PAIRING_FAIL_REASON_CMD_NOT_SUPPORT			0x07 //-- core 4.2
#define PAIRING_FAIL_REASON_UNSPECIFIED_REASON		0x08
#define PAIRING_FAIL_REASON_REPEATED_ATTEMPT		0x09
#define PAIRING_FAIL_REASON_INVALID_PARAMETER		0x0A
#define PAIRING_FAIL_REASON_DHKEY_CHECK_FAIL		0x0B
#define PAIRING_FAIL_REASON_NUMERIC_FAILED			0x0C
#define PAIRING_FAIL_REASON_BREDR_PAIRING			0x0D
#define PAIRING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW	0x0E
#define PAIRING_FAIL_REASON_PAIRING_TIMEOUT			0x80 //TLK defined
#define PAIRING_FAIL_REASON_CONN_DISCONNECT			0x81 //TLK defined
#define PAIRING_FAIL_REASON_SUPPORT_NC_ONLY         0x82 //TLK defined



/**
 * @brief  security request configuration
 */
typedef enum {
	SecReq_NOT_SEND = 0,   // do not send "security request" after link layer connection established
	SecReq_IMM_SEND = BIT(0),   //"IMM" refer to immediate, send "security request" immediately after link layer connection established
	SecReq_PEND_SEND = BIT(1),  //"PEND" refer to pending,  pending "security request" for some time after link layer connection established, when pending time arrived. send it
}secReq_cfg;


/**
 * @brief  security mode and level
 * 		   See Spec Vol 3/Part C/10.2 for more information.
 */
typedef enum {
	LE_Security_Mode_1_Level_1 = BIT(0),  No_Authentication_No_Encryption			= BIT(0), No_Security = BIT(0),
	LE_Security_Mode_1_Level_2 = BIT(1),  Unauthenticated_Pairing_with_Encryption 	= BIT(1),
	LE_Security_Mode_1_Level_3 = BIT(2),  Authenticated_Pairing_with_Encryption 	    = BIT(2),
	LE_Security_Mode_1_Level_4 = BIT(3),  Authenticated_LE_Secure_Connection_Pairing_with_Encryption = BIT(3),

	LE_Security_Mode_2_Level_1 = BIT(4),  Unauthenticated_Pairing_with_Data_Signing 	= BIT(4),
	LE_Security_Mode_2_Level_2 = BIT(5),  Authenticated_Pairing_with_Data_Signing    = BIT(5),

	LE_Security_Mode_1 = (LE_Security_Mode_1_Level_1 | LE_Security_Mode_1_Level_2 | LE_Security_Mode_1_Level_3 | LE_Security_Mode_1_Level_4)
}le_security_mode_level_t;


/**
 * @brief  ECDH keys mode
 */
typedef enum {
	non_debug_mode 	= 0,  // ECDH distribute private/public key pairs
	debug_mode 		= 1,  // ECDH use debug mode private/public key pairs
} ecdh_keys_mode_t;


/**
 * @brief  bonding mode
 */
typedef enum {
	Non_Bondable_Mode = 0,
	Bondable_Mode     = 1,
}bonding_mode_t;


/**
 * @brief  security mode and level
 * 		   See BLE SPEC: Vol 3, Part H, "2.3 PAIRING METHODS" for more information.
 */
typedef enum {
	LE_Legacy_Pairing     = 0,   // BLE 4.0/4.2
	LE_Secure_Connection = 1,   // BLE 4.2/5.0/5.1
}pairing_methods_t;


/**
 * @brief  IO capability
 */
typedef enum {
	IO_CAPABILITY_UNKNOWN 			= 0xff,
	IO_CAPABILITY_DISPLAY_ONLY 		= 0,
	IO_CAPABILITY_DISPLAY_YES_NO 	= 1,	IO_CAPABILITY_DISPLAY_YESNO = 1,
	IO_CAPABILITY_KEYBOARD_ONLY 	= 2,
	IO_CAPABILITY_NO_INPUT_NO_OUTPUT= 3, 	IO_CAPABILITY_NO_IN_NO_OUT 	= 3,
	IO_CAPABILITY_KEYBOARD_DISPLAY 	= 4,
} io_capability_t;

/**
 * @brief	Keypress Notification type
 */
typedef enum {
	KEYPRESS_NTF_PKE_START			=	0x00,
	KEYPRESS_NTF_PKE_DIGIT_ENTERED	=	0x01,
	KEYPRESS_NTF_PKE_DIGIT_ERASED	=	0x02,
	KEYPRESS_NTF_PKE_CLEARED		=	0x03,
	KEYPRESS_NTF_PKE_COMPLETED		=	0x04,
} keypress_notify_t;

/**
 * @brief	local IRK generating strategy when controller use identity address in RF packet
 * 			default use "LOCIRK_BINDING_WITH_DEVICE" if user not set
 */
typedef enum {
	LOCIRK_BINDING_WITH_DEVICE = 0,  //every device have unique local IRK, never change
	LOCIRK_RANDOM_GENERATE     = 1,  //random generate
}loc_irk_gen_str_t;

typedef struct  {
    /** Public Key. */
    u8 public_key[64];

    /** Private Key. */
    u8 private_key[32];
}smp_sc_oob_key_t;

typedef struct  {
    /** Random Number. */
    u8 random[16]; //big--endian

    /** Confirm Value. */
    u8 confirm[16]; //big--endian
}smp_sc_oob_data_t;
/**
 * @brief      This function is used to set local IRK generating strategy.
 * @param[in]  str - local IRK generating strategy.
 * @return     none.
 */
void		blc_smp_setLocalIrkGenerateStrategy (loc_irk_gen_str_t  str);


/**
 * @brief      This function is used to set security level.
 * @param[in]  mode_level - The security level value can refer to the structure 'le_security_mode_level_t'.
 * @return     none.
 */
void 		blc_smp_setSecurityLevel(le_security_mode_level_t  mode_level);


/**
 * @brief      This function is used to enable ECDH to generate public key-private key pairs in advance.
 * @param[in]  enable - 1: Turn on ECDH to generate public key-private key pairs in advance.
 *                      0: Disable this mode.
 * @return     none.
 */
void 		blc_smp_preMakeEcdhKeysEnable(u8 enable);


/**
 * @brief      This function is used to set pairing method.
 * @param[in]  method - The pairing method value can refer to the structure 'pairing_methods_t'.
 *                      0: LE legacy pairing;
 *                      1: LE secure connection
 * @return     none.
 */
void 		blc_smp_setPairingMethods (pairing_methods_t  method);

/**
 * @brief      This function is used to set if support Secure Connections.
 * @param[in]  SC_en  - 0: not support Secure Connections;
 *                      1: support Secure Connections.
 * @return     none.
 */
void		blc_smp_enableSecureConnections(int SC_en); //replace API: blc_smp_setPairingMethods

/**
 * @brief      This function is used to set whether the device uses the ECDH DEBUG key.
 * @param[in]  mode - The ECDH key mode value can refer to the structure 'ecdh_keys_mode_t'.
 *                    0: non debug mode;
 *                    1: debug mode.
 * @return     none.
 */
void 		blc_smp_setEcdhDebugMode(ecdh_keys_mode_t mode);


/**
 * @brief      This function is used to set bonding mode.
 * @param[in]  mode - The bonding mode value can refer to the structure 'bonding_mode_t'.
 *                    0: non bondable mode;
 *                    1: bondable mode.
 * @return     none.
 */
void 		blc_smp_setBondingMode(bonding_mode_t mode);


/**
 * @brief      This function is used to set if enable authentication MITM protection.
 * @param[in]  MITM_en - 0: Disable MITM protection;
 *                       1: Enable MITM protection.
 * @return     none.
 */
void 		blc_smp_enableAuthMITM (int MITM_en);


/**
 * @brief      This function is used to set if enable OOB authentication.
 * @param[in]  OOB_en - 0: Disable OOB authentication;
 *                      1: Enable OOB authentication.
 * @return     none.
 */
void 		blc_smp_enableOobAuthentication (int OOB_en);


/**
 * @brief      This function is used to set device's IO capability.
 * @param[in]  ioCapability - The IO capability's value can refer to the structure 'io_capability_t'.
 * @return     none.
 */
void 		blc_smp_setIoCapability (io_capability_t  ioCapability);


/**
 * @brief      This function is used to set device's Keypress.
 * @param[in]  keyPress_en - 0: Disable Keypress;
 *                           1: Enable Keypress.
 * @return     none.
 */
void 		blc_smp_enableKeypress (int keyPress_en);


/**
 * @brief      This function is used to set device's security parameters.
 * @param[in]  mode - The bonding mode value can refer to the structure 'bonding_mode_t'.
 * @param[in]  MITM_en - 0: Disable MITM protection;  1: Enable MITM protection.
 * @param[in]  OOB_en - 0: Disable OOB authentication; 1: Enable OOB authentication.
 * @param[in]  keyPress_en - 0: Disable Keypress; 1: Enable Keypress.
 * @param[in]  ioCapability - The IO capability's value can refer to the structure 'io_capability_t'.
 * @return     none.
 */
void 		blc_smp_setSecurityParameters (bonding_mode_t mode, int MITM_en, int OOB_en,
										  int keyPress_en, io_capability_t ioCapability);


/**
 * @brief      This function is used to set TK by OOB method.
 * @param[in]  oobData - TK's value, size: 16 byte.
 * @return     none.
 */
void 		blc_smp_setTK_by_OOB (u8 *oobData);


/**
 * @brief      This function is used to check whether the PinCode needs to be input.
 * @param[in]  none.
 * @return     1: Need to enter PinCode
 * 			   0: No need to enter PinCode
 */
int 		blc_smp_isWaitingToSetPasskeyEntry(void);


/**
 * @brief      This function is used to check whether it is needed to confirm NC YES/NO.
 * @param[in]  none.
 * @return     1: Need to confirm NC YES/NO
 * 			   0: No need to confirm NC YES/NO
 */
int 		blc_smp_isWaitingToCfmNumericComparison(void);


/**
 * @brief      This function is used to set TK by passkey entry method.
 * @param[in]  pinCodeInput - TK's value, input range [000000, 999999].
 * @return     1:Succeed; 0:Failed.
 */
int 		blc_smp_setTK_by_PasskeyEntry (u32 pinCodeInput);


/**
 * @brief      This function is used to send keypress notify during TK input phrase.
 * param[in]   connHandle - Current ACL connection handle.
 * @param[in]  ntfType - refer to 'keypress_notify_t'.
 * @return     True: send SUCC
 * 			   False: send FAIL
 */
bool		blc_smp_sendKeypressNotify (u16 connHandle, keypress_notify_t ntfType);


/**
 * @brief      This function is used to set numeric compare confirm YES or NO.
 * @param[in]  YES_or_NO - 1: numeric compare confirm YES;
 *                         0: numeric compare confirm NO.
 * @return     none.
 */
void		blc_smp_setNumericComparisonResult(bool YES_or_NO);



/**
 * @brief      This function is used to set manual pin code for debug in passkey entry mode.
 * 			   attention: 1. PinCode should be generated randomly each time, so this API is not standard usage for security,
 * 			              	 it is violation of security protocols.
 * 			              2. If you set manual pin code with this API in correct range(1~999999), you can neglect callback
 * 			                 event "GAP_EVT_MASK_SMP_TK_DISPLAY", because the pin code displayed is the value you have set by this API.
 * 			              3. pinCodeInput value 0 here is used to exit manual set mode, but not a Pin Code.
 * @param[in]  connHandle - connection handle
 * @param[in]  pinCodeInput - 0           :  exit  manual set mode, generated Pin Code randomly by SDK library.
 * 							  other value :  enter manual set mode, pinCodeInput in range [1, 999999] will be TK's value.
 * 							  				 if bigger than 999999, generated Pin Code randomly by SDK library
 * @return     none.
 */
void 		blc_smp_manualSetPinCode_for_debug(u16 connHandle, u32 pinCodeInput);

/**
 * @brief      This function is used to generate security connection OOB data.
 * @param[in]  oob_data - OOB data
 * @param[in]  oob_key - OOB key
 * @return     1
 */
int         blc_smp_generateScOobData(smp_sc_oob_data_t *oob_data, smp_sc_oob_key_t *oob_key);

/**
 * @brief      This function is used to set security connection OOB data.
 * @param[in]  connHandle - connection handle
 * @param[in]  oobd_local - OOB local data
 * @param[in]  oobd_remote - OOB remote data
 * @return     status - 0x00:  succeed
 * 			  		   other:  failed
 */
int          blc_smp_setScOobData(u16 connHandle, const smp_sc_oob_data_t *oobd_local, const smp_sc_oob_data_t *oobd_remote);



/**
 * @brief      This function is used to generate local IRK
 * 			   attention: 1. this API is involved only when local RPA is used.
 * 			              2. user must use this API to generate IRK, other method is not allowed.
 * @param[in]  none
 * @param[out] pLocalIrk - pointer to local IRK buffer
 * @return     none
 */
void         blc_smp_generateLocalIrk(u8* pLocalIrk);


#endif /* BLE_SMP_H_ */
