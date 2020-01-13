/********************************************************************************************************
 * @file     security_service.h
 *
 * @brief	 APIs and constructs for security service
 *
 * @author
 * @date     May. 27, 2017
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
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
#ifndef SECURITY_SERVICE_H
#define SECURITY_SERVICE_H	1

#include "tl_common.h"
#include "../aps/aps_api.h"
#include "../include/tl_config.h"


#if SECURITY_ENABLE
#define		SECURITY_MODE_STANDARD 			1
#define		SECURITY_MODE_HIGH				(!SECURITY_MODE_STANDARD)
#define		SS_UPDATE_FRAMECOUNT_THRES		1024
#endif
typedef enum{
	SS_KEYREQ_TYPE_APPLK = 0x02,
	SS_KEYREQ_TYPE_TCLK = 0x04
}ss_keyReqType_e;

enum
{
  SS_SECUR_NO_ENCR,
  SS_SECUR_NWK_ENCR,
  SS_SECUR_APS_ENCR,
  SS_SECUR_MAC_ENCR
};
/**
   Key id - see 4.5.1.1.2
 */
enum{
  SS_SECUR_DATA_KEY,
  SS_SECUR_NWK_KEY,
  SS_SECUR_KEY_TRANSPORT_KEY,
  SS_SECUR_KEY_LOAD_KEY
};

//Key types
typedef enum{
	SS_TC_MASTER_KEY,//removed from zb 3.0
	SS_STANDARD_NETWORK_KEY = 0x01,
	SS_APP_MASTER_KEY,//removed from zb 3.0
	SS_APP_LINK_KEY = 0x03,
	SS_TC_LINK_KEY = 0x04,
	SS_HIGH_SECUR_NETWORK_KEY,//removed from zb 3.0
} ss_seKeyType_e;

typedef enum{
	SS_UNIQUE_LINK_KEY = 0x00,
	SS_GLOBAL_LINK_KEY = 0x01
}ss_linkKeytype_e;

typedef enum{
	SS_SEMODE_CENTRALIZED,
	SS_SEMODE_DISTRIBUTED
}ss_securityMode_e;

typedef enum{
	SS_PROVISIONAL_KEY,
	SS_UNVERIFIED_KEY,
	SS_VERIFIED_KEY
}ss_keyAttributes_e;


typedef struct
{
  u8		key[CCM_KEY_SIZE];
  u8		keySeqNum;
  u8		keyType;
}ss_material_set_t;

typedef enum{
	SS_PRECONFIGURED_NOKEY,
	SS_PRECONFIGURED_GLOBALLINKKEY,
	SS_PRECONFIGURED_UNIQUELLINKKEY,
	SS_PRECONFIGURED_NWKKEY
}ss_preconfiguredKey_e;


typedef struct{

	//This value set to TRUE will only allow devices know to TC join or rejoin
	bool	useWhiteList;
	// 0 ~ not support; 1 ~ support but not require; 2 ~ require the use of install code by JD, the useWhiteList would set to TRUE
	//interface need to be supported to input install code to TC
	u8		allowInstallCode;
	//Indicates whether or not devices are required to attempt to update their TCLK after joining. In centralized security network,
	//this value must be set to TRUE, the joining device must attempt TCLK update after joining the network.
	bool	updateTCLKrequired;
#ifdef ZB_COORDINATOR_ROLE
	//This values indicates whether or not TC allow new device join to the network, set to false in centralized security NWK will
	//reject any join request
	bool	allowJoins;
	//If TC allows rejoin using well known or default keys, a setting of FALSE means rejoins are only allowed with TCLK where the
	//KeyAttributes of the apsDeviceKeyPairSet entry indicates VERIFIED_KEY. This value set to FALSE in centralized security NWK.
	bool	allowRejoins;
	//0 ~ never; 1 ~ any device may request; 2 ~ only devices in the apsDeviceKeyPaireSet.
	//Set to 0 in network with higher level protocols for establishing link keys. Set to 1/2 in centralized security networks
	u8		allowTCLKrequest;

	u8		allowAppLKrequest;
#endif

}ss_tcPolicy_t;


typedef enum{
	KEYTYPE_PRO_MASK = BIT(0),
	KEYTYPE_UNVERIFY_MASK = BIT(1),
	KEYTYPE_VERIFIED_MASK = BIT(2)
}nvKeytpeMask_e;

typedef enum{
	SS_DEVKEYPAIR_SYNID_KEYATTR,
	SS_DEVKEYPAIR_SYNID_INCOMMINGFRAMECNT,
	SS_DEVKEYPAIR_SYNID_ALL
}ss_devKeyPairSyn_id;


typedef struct{
	addrExt_t			device_address;

	u8					linkKey[CCM_KEY_SIZE];		/* The actual value of the link key. */

	ss_keyAttributes_e 	keyAttr;
	ss_linkKeytype_e 	apsLinkKeyType;
	u8					used;
	u8					rsv;//used as mapping index to stack item

	u32					outgoingFrameCounter;
	u32					incomingFrmaeCounter;
}ss_dev_pair_set_t;

typedef struct{
	u32						ssTimeoutPeriod;
	u32						outgoingFrameCounter;
	u32						prevOutgoingFrameCounter;
	ss_dev_pair_set_t		*keyPairSetNew;
	ss_material_set_t		nwkSecurMaterialSet[SECUR_N_SECUR_MATERIAL];
	u16						devKeyPairNum;
	addrExt_t				trust_center_address;
	u8						securityLevel:4;
	u8						secureAllFrames:1;
	u8						activeSecureMaterialIndex:2;
	u8						reserved:1;
	u8						activeKeySeqNum;
	ss_preconfiguredKey_e	preConfiguredKeyType;//pre-configured type, should be set during init state which used for ZDO auth
	ss_tcPolicy_t			tcPolicy;
	u8						*touchLinkKey;
	u8						*distibuteLinkKey;
	u8						tcLinkKeyType;
	u8						*tcLinkKey;
	//ss_linkKeytype_e		linkKeyType;
}ss_info_base_t;


typedef struct{
	/*The extended 64-bit address of the device
	that is the parent of the child device that is
	requested to be removed, or the router
	device that is requested to be removed.*/
	addrExt_t			parentAddr;
	/*The extended 64-bit address of the target
	device that is requested to be removed. If a
	router device is requested to be removed,
	then the ParentAddress shall be the same
	as the TargetAddress.*/
	addrExt_t			targetExtAddr;
}ss_apsDevRemoveReq_t;

typedef struct{
	/*The extended 64-bit address of the child
	device that is requested to be removed*/
	addrExt_t			childExtAddr;
	/*The extended 64-bit address of the device
	 requesting that a child device be removed.*/
	addrExt_t			tcAddr;
}ss_apsDevRemoveInd_t;

typedef struct{
	addrExt_t			dstAddr;
	u8					keyType;
	u8					key[CCM_KEY_SIZE];
	u8					relayByParent;
	u8					keySeqNum;
	addrExt_t			partnerAddr;  //for application key
	u8					initatorFlag;
	u8 					nwkSecurity;
}ss_apsmeTxKeyReq_t;

typedef struct{
	addrExt_t			destddr;
	u8					keySeqNum;
}ss_apsKeySwitchReq_t;

#define AES_BLOCK_SIZE     16

struct CCM_FLAGS_TAG {
    union {
        struct {
            u8 L : 3;
            u8 M : 3;
            u8 aData :1;
            u8 reserved :1;
        } bf;
        u8 val;
    };
};

typedef struct CCM_FLAGS_TAG ccm_flags_t;

enum AES_OPT {
    AES_ENCRYPTION = 0,
    AES_DECRYPTION,
};

extern ss_info_base_t ss_ib;
#define SS_IB()							ss_ib

// for CCM
u8 aes_ccmAuthTran(u8 M, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *result);
u8 aes_ccmEncTran(u8 M, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *result);
u8 aes_ccmDecTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *mic);
u8 aes_ccmDecAuthTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *mic);

bool ss_keyPreconfigured(void);
void ss_zdoNwkKeyConfigure(const u8 *key, u8 i,u8 keyType);
u8 ss_apsAuxHdrfill(void *p, bool nwkKey/*, u8 cmdId*/);
u8 ss_apsDecryptFrame(void *p);
u8 ss_apsSecureFrame(void *p, u8 apsHdrAuxLen,u8 apsHdrLen, addrExt_t extAddr);
u8 ss_apsDeviceRemoveReq(ss_apsDevRemoveReq_t *p);

u8 ss_devKeyPairFind(addrExt_t extAddr, ss_dev_pair_set_t *keyPair);
void ss_devKeyPairSave(ss_dev_pair_set_t *keyPair);
s32 ss_apsSSTimeoutHandler(void *arg);
void ss_apsSSTimeoutCancel(void *arg);
u16 ss_devKeyPairInfoGet(void);
u8 ss_devKeyPairDelete(addrExt_t extAddr);

/*
 * @brief 	get the MAC address form key pair table so as to get the information of the node which have joined the network
 *
 * @param	start_idx  			the index of the key pair table
 *
 * @param	num		   			the count of the node want to be read out
 *
 * @param   *validNum			the valid count will be read out
 *
 * @param   nodeMacAddrList		the node MAC address list
 *
 * */
u16 ss_nodeMacAddrFromdevKeyPair(u16 start_idx, u8 num, u8 *validNum, addrExt_t *nodeMacAddrList);

u32 ss_outgoingFrameCntGet(void);

void ss_zdoUseKey(u8 index);

void ss_apsKeySwitchReq(void *p);

void ss_zdoNwkKeyUpdateReq(void *p);

aps_status_t ss_apsmeRequestKeyReq(u8 keyType, addrExt_t dstAddr, addrExt_t partnerAddr);

/*
 * @brief 	save ssib to flash
 *
 * */
void zdo_ssInfoSaveToFlash();


/*
 * @brief 	get ssib from flash
 *
 * */
u8 zdo_ssInfoInit(void);

/*
 * @brief 	update ssib to flash
 *
 * */
void zdo_ssInfoUpdate(void);

/*
 * @brief 	get network key index from flash
 *
 * */
u8 zdo_ssInfoKeyGet(void);


/* generate network key, and then store it in the ib table */
void ss_nwkKeyGenerate(u8 *nwkKey);

/* store network key in ib table */
void ss_nwkKeyStore(u8 *nwkKey);


/*
 * @brief	security configuration
 *
 * @param	enSecurity  security enable
 *
 * @param	type  		pre-configure key mode
 *
 * */
void ss_zdoInit(bool enSecurity,ss_preconfiguredKey_e type);


/*
 * @brief	security configuration
 *
 * @param	m  security mode, distribute network or central network
 *
 * */
void ss_securityModeSet(ss_securityMode_e m);

/*
 * @brief	judge if it's a distribute security mode
 *
 *
 * */
bool ss_securityModeIsDistributed(void);

#endif	/* SECURITY_SERVICE_H */
