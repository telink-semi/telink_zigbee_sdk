/********************************************************************************************************
 * @file     zb_common.h
 *
 * @brief    Common include file for ZigBee
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
#ifndef ZB_COMMON_H
#define ZB_COMMON_H

/**********************************************************************
 * INCLUDES
 */
#include "tl_config.h"

#include "../ss/security_service.h"
#include "../common/includes/zb_task_queue.h"
#include "../common/includes/zb_buffer.h"
#include "../mac/includes/tl_zb_mac.h"
#include "../mac/includes/tl_zb_mac_pib.h"
#include "../mac/includes/mac_phy.h"
#include "../mac/includes/mac_trx_api.h"

#include "../nwk/includes/nwk_neighbor.h"
#include "../nwk/includes/nwk_ctx.h"
#include "../nwk/includes/nwk_addr_map.h"
#include "../nwk/includes/nwk.h"
#include "../nwk/includes/nwk_nib.h"

#include "../zdo/zdo_api.h"
#include "../zdo/zdp.h"
#include "../zdo/zb_af.h"
#include "../aps/aps_api.h"
#include "../bdb/includes/bdb.h"
#include "../zcl/zll_commissioning/zcl_touchlink_attr.h"

#include "../zbapi/zb_api.h"

#if GP_SUPPORT_ENABLE
#include "../gp/cGP_stub.h"
#include "../gp/dGP_stub.h"
#endif
/**********************************************************************
 * TYPEDEFS
 */
//for .bin map
#define _CODE_MAC_  	__attribute__((section(".sect_mac")))
#define _CODE_NWK_  	__attribute__((section(".sect_nwk")))
#define _CODE_ZDO_  	__attribute__((section(".sect_zdo")))
#define _CODE_APS_  	__attribute__((section(".sect_aps")))
#define _CODE_ZCL_  	__attribute__((section(".sect_zcl")))
#define _CODE_SS_  		__attribute__((section(".sect_ss")))
#define _CODE_BDB_  	__attribute__((section(".sect_bdb")))

/*
   functions return type.
   In general, function can return OK, BLOCKED or some error. Errors
   are negative.
   Error can be "generic" or some additional error code.
*/
enum zb_ret_e{
	/* Most common return types: ok, generic error, BLOCKED, thread exit indication. */
	RET_OK              				 = 0,
	RET_ERROR           					,
	RET_BLOCKED        						,
	RET_EXIT           						,
	RET_BUSY           						,
	RET_EOF             					,
	RET_OUT_OF_RANGE    					,
	RET_EMPTY           					,
	RET_CANCELLED       					,
	RET_PENDING                             ,
	RET_NO_MEMORY                           ,
	RET_INVALID_PARAMETER                   ,
	RET_OPERATION_FAILED                    ,
	RET_BUFFER_TOO_SMALL                    ,
	RET_END_OF_LIST                         ,
	RET_ALREADY_EXISTS                      ,
	RET_NOT_FOUND                           ,  //0x10
	RET_OVERFLOW                            ,
	RET_TIMEOUT                             ,
	RET_NOT_IMPLEMENTED                     ,
	RET_NO_RESOURCES                        ,
	RET_UNINITIALIZED                       ,
	RET_NO_SERVER                           ,
	RET_INVALID_STATE                       ,
	RET_DOES_NOT_EXIST                      ,
	RET_CONNECTION_FAILED                   ,
	RET_CONNECTION_LOST                     ,
	RET_CANCELLED_BY_USER                   ,
	RET_UNAUTHORIZED                        ,
	RET_CONFLICT                            ,
	RET_COULD_NOT_OPEN_FILE                 ,
	RET_NO_MATCH                            ,
	RET_PROTOCOL_ERROR                      ,  //0x20
	RET_VERSION                             ,
	RET_MALFORMED_ADDRESS                   ,
	RET_COULD_NOT_READ_FILE                 ,
	RET_FILE_NOT_FOUND                      ,
	RET_DIRECTORY_NOT_FOUND                 ,
	RET_CONVERTION_ERROR                    ,
	RET_INCOMPATIBLE_TYPES                  ,
	RET_INCOMPATIBLE_TYPES_IN_COMPARE       ,
	RET_INCOMPATIBLE_TYPES_IN_ASSIGNMENT    ,
	RET_INCOMPATIBLE_TYPES_IN_EXPRESSION    ,
	RET_ILLEGAL_COMPARE_OPERATION           ,
	RET_FILE_CORRUPTED                      ,
	RET_PAGE_NOT_FOUND                      ,
	RET_FILE_WRITE_ERROR                    ,
	RET_FILE_READ_ERROR                     ,
	RET_FILE_PARTIAL_WRITE                  ,   //0x30
	RET_TOO_MANY_OPEN_FILES                 ,
	RET_ILLEGAL_REQUEST                     ,
	RET_INVALID_BINDING                     ,
	RET_INVALID_GROUP                       ,
	RET_TABLE_FULL                          ,
	RET_NO_ACK                              ,
	RET_ACK_OK                              ,
	RET_NO_BOUND_DEVICE                     ,   //0x38
	RET_BUF_FULL
};

enum {
    START_VAR_ID = 0,
    MAC_PIB_ID,
    NWK_NIB_ID,
    APS_AIB_ID,
    ZDO_HANDLE_ID,
    PIB_NUM,
};

enum {
    ADDR_MAP_ID = 0,
    ADDR_MAP_CTRL_ID,
    NWK_NEIGHBOR_ID,
    NWK_NEIGHBOR_CTRL_ID,
    APS_BINDING_ID,
    APS_BINDING_CTRL_ID,
    APS_GROUP_ID,
    APS_GROUP_CTRL_ID,
    TAB_NUM,
};

enum{
	REJOIN_INSECURITY,
	REJOIN_SECURITY
};

#define	TL_ZB_PROFILE_ID				1
#define	TL_RETURN_INVALID				0xff


#define ZB_BEACON_INTERVAL_USEC 15360 /* in microseconds */


/**
 One second timeout
*/
#define ZB_TIME_ONE_SECOND ZB_MILLISECONDS_TO_BEACON_INTERVAL(1000)
/**
  Convert time from beacon intervals to millisecinds

  Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
*/
#define ZB_TIME_BEACON_INTERVAL_TO_MSEC(t) (ZB_BEACON_INTERVAL_USEC / 100 * (t) / 10)

/**
  Convert time from millisecinds to beacon intervals

  Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
*/
#define ZB_MILLISECONDS_TO_BEACON_INTERVAL(ms) (((10l * (ms) + 3) / (ZB_BEACON_INTERVAL_USEC / 100)))

/**
   Return 1 if the number is a power of 2, works only if x > 0
 */
#define MAGIC_IS_POWER_OF_TWO(x) ( ((x) & ((x) - 1)) == 0 )

//APS
#define			APS_ACK_WAIT_DURATION									3*ZB_TIME_ONE_SECOND


#define			APS_POLL_AFTER_REQ_TMO 									10
#define SHORT_ADDR_LEN       2        //!< Network/short address length
#define EXT_ADDR_LEN         8        //!< Extended/long/IEEE address length
#define SRC_ID_LEN			 4
#define SEC_KEY_LEN			 16


#define	COPY_U16TOBUFFER(buf,data)					\
	((((u8 *)buf)[0])		=	((u8 )data));			\
	((((u8 *)buf)[1])		=	((u8 )(data>>8)))			\


#define	TL_SETSTRUCTCONTENT(s,v)					(memset((u8 *)&s,v,sizeof(s)))
#define	COPY_BUFFERTOU16(data,buf)								\
		(data)	=	(buf)[0] + ((buf)[1]<<8) 						\

#define	COPY_U32TOBUFFER(buf,data)								\
			((((u8 *)buf)[0])		=	((u8 )data));			\
			((((u8 *)buf)[1])		=	((u8 )(data>>8)));		\
			((((u8 *)buf)[2])		=	((u8 )(data>>16)));		\
			((((u8 *)buf)[3])		=	((u8 )(data>>24)))		\

#define	COPY_U24TOBUFFER(buf,data)								\
			((((u8 *)buf)[0])		=	((u8 )data));			\
			((((u8 *)buf)[1])		=	((u8 )(data>>8)));		\
			((((u8 *)buf)[2])		=	((u8 )(data>>16)));		\

#define	COPY_BUFFERTOU16_BE(data,buf)								\
		(data)	=	(buf)[1] + ((buf)[0]<<8) 						\

#define	COPY_U16TOBUFFER_BE(buf,data)					\
	((((u8 *)buf)[0])		=	((u8 )(data>>8)));			\
	((((u8 *)buf)[1])		=	((u8 )data))			\

#define	COPY_BUFFERTOU32_BE(data,buf)								\
			(data) = ((buf)[0]<<24) + ((buf)[1]<<16) + ((buf)[2]<<8) + (buf)[3]\

#define	COPY_U32TOBUFFER_BE(buf,data)						\
	((((u8 *)buf)[0])		=	((u8 )(data>>24)));			\
	((((u8 *)buf)[1])		=	((u8 )(data>>16)));			\
	((((u8 *)buf)[2])		=	((u8 )(data>>8)));			\
	((((u8 *)buf)[3])		=	((u8 )data));				\

#define ZB_IS_16BIT_SECURITY_KEY_ZERO(key) (!memcmp((key), g_null_securityKey, SEC_KEY_LEN))
#define ZB_IS_64BIT_ADDR_ZERO(addr) 	(!memcmp((addr), g_zero_addr, EXT_ADDR_LEN))
#define ZB_IS_64BIT_ADDR_INVAILD(addr) 	(!memcmp((addr), g_invalid_addr, EXT_ADDR_LEN))
#define ZB_64BIT_ADDR_ZERO(addr)       	memset((addr), 0, EXT_ADDR_LEN)
#define ZB_64BIT_ADDR_COPY(dst, src) 	memcpy(dst, src, EXT_ADDR_LEN)
#define ZB_64BIT_ADDR_CMP(one, two) 	((bool)!memcmp((one), (two), EXT_ADDR_LEN))

#define ZB_EXTPANID_IS_ZERO 		ZB_IS_64BIT_ADDR_ZERO
#define ZB_EXTPANID_ZERO 			ZB_64BIT_ADDR_ZERO
#define ZB_EXTPANID_COPY 			ZB_64BIT_ADDR_COPY
#define ZB_EXTPANID_CMP 			ZB_64BIT_ADDR_CMP

#define ZB_IEEE_ADDR_IS_ZERO 		ZB_IS_64BIT_ADDR_ZERO
#define ZB_IEEE_ADDR_IS_INVAILD 	ZB_IS_64BIT_ADDR_INVAILD
#define ZB_IEEE_ADDR_ZERO 			ZB_64BIT_ADDR_ZERO
#define	ZB_IEEE_ADDR_INVALID(addr)	ZB_64BIT_ADDR_COPY(addr,g_invalid_addr)
#define ZB_IEEE_ADDR_COPY 			ZB_64BIT_ADDR_COPY
#define ZB_IEEE_ADDR_CMP 			ZB_64BIT_ADDR_CMP

#define ZB_SEC_KEY_IS_NULL			ZB_IS_16BIT_SECURITY_KEY_ZERO

#define ZB_MAC_FRAME_HEADER			9
#define ZB_NWK_FRAME_HEADER			(8 + NWK_MAX_SOURCE_ROUTE * 2 + 14 + 4)
#define ZB_APS_FRAME_HEADER			10
/*typedef struct{
	u8	altPanCoord: 	1;
	u8	devType: 		1;
	u8	powerSrc: 		1;
	u8	rcvOnWhenIdle: 	1;
	u8  reserved:		2;
	u8	secuCapability: 1;
	u8	allocAddr: 		1;
}capability_info_t;*/

#define	MAC_CAP_ALTPANCOORD			BIT(0)
#define	MAC_CAP_DEVTYPE				BIT(1)
#define	MAC_CAP_POWERSRC			BIT(2)
#define	MAC_CAP_RXONIDLE			BIT(3)
#define	MAC_CAP_SECURITYCAP			BIT(6)
#define	MAC_CAP_ALLOCADDR			BIT(7)

typedef struct{
	tl_zb_mac_pib_t 	macPib;  //76B
	nwk_nib_t			nwkNib;  //60B
	touchlink_attr_t	touchlinkAttr;  //8B
	bdb_attr_t			bdbAttr;		//48B
}zb_info_t;

zb_info_t g_zbInfo;


#define g_zbMacPib			g_zbInfo.macPib
#define g_zbNIB				g_zbInfo.nwkNib
#define g_touchlinkAttr		g_zbInfo.touchlinkAttr
#define g_bdbAttrs			g_zbInfo.bdbAttr


/* diagnostics for stack */
typedef struct{
	u16 numberOfResets;
	u16 persistentMemoryWrites;

	u32 macRxCrcFail;
	u32 macTxCcaFail;
	u32 macRxBcast;
	u32 macTxBcast;
	u32 macRxUcast;
	u32 macTxUcast;
	u16 macTxUcastRetry;
	u16 macTxUcastFail;

	u16 nwkTxCnt;
	u16 nwkTxEnDecryptFail;

	u16 apsRxBcast;
	u16 apsTxBcast;
	u16 apsRxUcast;
	u16 apsTxUcastSuccess;
	u16 apsTxUcastRetry;
	u16 apsTxUcastFail;

	u16 routeDiscInitiated;
	u16 neighborAdded;
	u16 neighborRemoved;
	u16 neighborStale;
	u16 joinIndication;
	u16 childMoved;

	u16 nwkFCFailure;
	u16 apsFCFailure;
	u16 apsUnauthorizedKey;
	u16 nwkDecryptFailures;
	u16 apsDecryptFailures;
	u16 packetBufferAllocateFailures;
	u16 relayedUcast;
	u16 phytoMACqueuelimitreached;
	u16 packetValidateDropCount;

	u8  lastMessageLQI;
	s8  lastMessageRSSI;
	u8	macTxIrqTimeoutCnt;
	u8	macTxIrqCnt;
	u8	macRxIrqCnt;
	u8	phyLengthError;
	u8	panIdConflict;
	u8	panIdModified;
}sys_diagnostics_t;

sys_diagnostics_t g_sysDiags;



void zb_info_save(void *arg);
u8 zb_info_load(void);

/**
   Return random value

   TODO: implement it!
 */
u16 zb_random();
#define ZB_RANDOM() zb_random()

typedef void (*zb_ibResetCb_t)(void *data);


void zb_reset(void);

void zb_init(void);

void os_reset(u8 isRetention);

void os_init(u8 isRetention);

void zb_extAddr_init(void);
/*! @} */


/*! \internal \addtogroup ZB_BASE */
/*! @{ */

/**
   Load Informational Bases from NVRAM or file
 */
u8 zb_ib_load(void);

void zb_ib_set_defaults(void) ;

void zb_rejoin_mode_set(u8 mode);


u32 brcTransRecordTblSizeGet(void);
nwk_brcTransRecordEntry_t *brcTransRecordEntryGet(u8 idx);
boundTblMapList_t *bindTblMapListGet(void);
u32 neighborTblSizeGet(void);
u32 addrMapTblSizeGet(void);

#endif /* ZB_COMMON_H */
