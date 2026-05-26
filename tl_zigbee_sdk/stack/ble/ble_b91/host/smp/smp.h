/********************************************************************************************************
 * @file     smp.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
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
 *******************************************************************************************************/

#ifndef BLE_SMP_H_
#define BLE_SMP_H_

#include "stack/ble/ble_b91/ble_common.h"
#include "smp_stack.h"




/** @addtogroup SMP first pairing or connecting back definition
 * @{
 */
#define SMP_STANDARD_PAIR   							0
#define SMP_FAST_CONNECT   								1
/** @} end of group SMP first pairing or connecting back */


/** @addtogroup SMP pairing fail reason definition
 * @{
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
#define PAIRING_FAIL_REASON_NUMUERIC_FAILED			0x0C
#define PAIRING_FAIL_REASON_BREDR_PAIRING			0x0D
#define PAIRING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW	0x0E
#define PAIRING_FAIL_REASON_PAIRING_TIMEOUT			0x80 //TLK defined
#define PAIRING_FAIL_REASON_CONN_DISCONNECT			0x81 //TLK defined
#define PAIRING_FAIL_REASON_SUPPORT_NC_ONLY         0x82 //TLK defined


/** @} end of group SMP pairing fail reason */


// "SecReq" refer to "security request"
typedef enum {
	SecReq_NOT_SEND = 0,   // do not send "security request" after link layer connection established
	SecReq_IMM_SEND = BIT(0),   //"IMM" refer to immediate, send "security request" immediately after link layer connection established
	SecReq_PEND_SEND = BIT(1),  //"PEND" refer to pending,  pending "security request" for some time after link layer connection established, when pending time arrived. send it
}secReq_cfg;


//See the Core_v5.0(Vol 3/Part C/10.2, Page 2067) for more information.
typedef enum {
	LE_Security_Mode_1_Level_1 = BIT(0),  No_Authentication_No_Encryption			= BIT(0), No_Security = BIT(0),
	LE_Security_Mode_1_Level_2 = BIT(1),  Unauthenticated_Paring_with_Encryption 	= BIT(1),
	LE_Security_Mode_1_Level_3 = BIT(2),  Authenticated_Paring_with_Encryption 	    = BIT(2),
	LE_Security_Mode_1_Level_4 = BIT(3),  Authenticated_LE_Secure_Connection_Paring_with_Encryption = BIT(3),

	LE_Security_Mode_2_Level_1 = BIT(4),  Unauthenticated_Paring_with_Data_Signing 	= BIT(4),
	LE_Security_Mode_2_Level_2 = BIT(5),  Authenticated_Paring_with_Data_Signing    = BIT(5),

	LE_Security_Mode_1 = (LE_Security_Mode_1_Level_1 | LE_Security_Mode_1_Level_2 | LE_Security_Mode_1_Level_3 | LE_Security_Mode_1_Level_4)
}le_security_mode_level_t;


typedef enum {
	non_debug_mode 	= 0,  // ECDH distribute private/public key pairs
	debug_mode 		= 1,  // ECDH use debug mode private/public key pairs
} ecdh_keys_mode_t;


typedef enum {
	Non_Bondable_Mode = 0,
	Bondable_Mode     = 1,
}bonding_mode_t;


//Paring Methods select
//See the Core_v5.0(Vol 3/Part H/2.3) for more information.
typedef enum {
	LE_Legacy_Paring     = 0,   // BLE 4.0/4.2
	LE_Secure_Connection = 1,   // BLE 4.2/5.0/5.1
}paring_methods_t;



typedef enum {
	IO_CAPABILITY_UNKNOWN 			= 0xff,
	IO_CAPABILITY_DISPLAY_ONLY 		= 0,
	IO_CAPABILITY_DISPLAY_YES_NO 	= 1,	IO_CAPABILITY_DISPLAY_YESNO = 1,
	IO_CAPABILITY_KEYBOARD_ONLY 	= 2,
	IO_CAPABILITY_NO_INPUT_NO_OUTPUT= 3, 	IO_CAPABILITY_NO_IN_NO_OUT 	= 3,
	IO_CAPABILITY_KEYBOARD_DISPLAY 	= 4,
} io_capability_t;



/**
 * @brief      This function is used to set the maximum number of devices that can be bound.
 * @param[in]  device_num - Set the maximum number of devices that can be bound.
 * @return     none.
 */
void 		blc_smp_param_setBondingDeviceMaxNumber ( int device_num);


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
 * @brief      This function is used to set paring method.
 * @param[in]  method - The paring method value can refer to the structure 'paring_methods_t'.
 *                      0: LE legacy paring;
 *                      1: LE secure connection
 * @return     none.
 */
void 		blc_smp_setParingMethods (paring_methods_t  method);       //select paring methods


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
void 		blc_smp_setBondingMode(bonding_mode_t mode);			   //set bonding_mode


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
void 		blc_smp_enableOobAuthentication (int OOB_en);    		   //enable OOB authentication


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
 * @brief      This function is used to set numeric compare confirm YES or NO.
 * @param[in]  YES_or_NO - 1: numeric compare confirm YES;
 *                         0: numeric compare confirm NO.
 * @return     none.
 */
void		blc_smp_setNumericComparisonResult(bool YES_or_NO);


//////////////////////////////////////////////////////////////////////////////////////
#define PARING_FAIL_REASON_PASSKEY_ENTRY			PAIRING_FAIL_REASON_PASSKEY_ENTRY
#define PARING_FAIL_REASON_OOB_NOT_AVAILABLE		PAIRING_FAIL_REASON_OOB_NOT_AVAILABLE
#define PARING_FAIL_REASON_AUTH_REQUIRE				PAIRING_FAIL_REASON_AUTH_REQUIRE
#define PARING_FAIL_REASON_CONFIRM_FAILED			PAIRING_FAIL_REASON_CONFIRM_FAILED
#define PARING_FAIL_REASON_PARING_NOT_SUPPORTED		PAIRING_FAIL_REASON_PAIRING_NOT_SUPPORTED
#define PARING_FAIL_REASON_ENCRYPT_KEY_SIZE			PAIRING_FAIL_REASON_ENCRYPT_KEY_SIZE
#define PARING_FAIL_REASON_CMD_NOT_SUPPORT			PAIRING_FAIL_REASON_CMD_NOT_SUPPORT
#define PARING_FAIL_REASON_UNSPECIFIED_REASON		PAIRING_FAIL_REASON_UNSPECIFIED_REASON
#define PARING_FAIL_REASON_REPEATED_ATTEMPT			PAIRING_FAIL_REASON_REPEATED_ATTEMPT
#define PARING_FAIL_REASON_INVALID_PARAMETER		PAIRING_FAIL_REASON_INVALID_PARAMETER
#define PARING_FAIL_REASON_DHKEY_CHECK_FAIL			PAIRING_FAIL_REASON_DHKEY_CHECK_FAIL
#define PARING_FAIL_REASON_NUMUERIC_FAILED			PAIRING_FAIL_REASON_NUMUERIC_FAILED
#define PARING_FAIL_REASON_BREDR_PARING				PAIRING_FAIL_REASON_BREDR_PAIRING
#define PARING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW	PAIRING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW
#define PARING_FAIL_REASON_PARING_TIMEOUT			PAIRING_FAIL_REASON_PAIRING_TIMEOUT
#define PARING_FAIL_REASON_CONN_DISCONNECT			PAIRING_FAIL_REASON_CONN_DISCONNECT
#define PARING_FAIL_REASON_SUPPORT_NC_ONLY			PAIRING_FAIL_REASON_SUPPORT_NC_ONLY

#define IO_CAPABILITY_DISPLAY_ONLY					0x00
#define IO_CAPABILITY_DISPLAY_YESNO					0x01
#define IO_CAPABILITY_KEYBOARD_ONLY					0x02
#define IO_CAPABILITY_NO_IN_NO_OUT					0x03
#define	IO_CAPABILITY_KEYBOARD_DISPLAY				0x04

#define	blc_smp_setSecurityParameter				blc_smp_setSecurityParameters

#endif /* BLE_SMP_H_ */
