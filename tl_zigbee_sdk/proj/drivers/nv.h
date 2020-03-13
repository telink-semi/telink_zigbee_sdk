/********************************************************************************************************
 * @file     nv.h
 *
 * @brief	 nv flash interface function header file.
 * 			 If undefined FLASH_SIZE_1M or defined FLASH_SIZE_1M ZERO, using 512k flash configuration.
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
#pragma once

#include "../common/types.h"
#include "user_config.h"


#define FLASH_4K_PAGE_NUM				16

#define FLASH_PAGE_SIZE					256
#define	FLASH_SECTOR_SIZE				4096//4K

/* Flash address of MAC address. */
enum{
	MAC_ADDR_512K_FLASH		= 0x76000,
	MAC_ADDR_1M_FLASH		= 0xFF000,
};

/* Flash address of factory pre-configured parameters. */
enum{
	CFG_ADDR_512K_FLASH		= 0x77000,
	CFG_ADDR_1M_FLASH		= 0xFE000,
};

extern u32 g_u32MacFlashAddr;
extern u32 g_u32CfgFlashAddr;

/*******************************************************************************************************
 * Following configuration could NOT be changed by customer.
 */
/* NV modules start address */
#if FLASH_SIZE_1M
#define NV_BASE_ADDRESS					0x80000
#define MODULES_START_ADDR(id)			(NV_BASE_ADDRESS + FLASH_SECTOR_SIZE * (2 * id))
#else
#define	NV_BASE_ADDRESS					0x34000//start from 208K address
#define	NV_BASE_ADDRESS2				0x7A000//start from 488K address
#define MODULES_START_ADDR(id)			((id<6) ? (NV_BASE_ADDRESS + FLASH_SECTOR_SIZE * (2 * id)) : (NV_BASE_ADDRESS2 + FLASH_SECTOR_SIZE * (2 * (id-6))))
#endif


/* Chipset pre-configured parameters */
#define MAC_BASE_ADD					(g_u32MacFlashAddr)
#define FACTORY_CFG_BASE_ADD			(g_u32CfgFlashAddr)

/* 8 bytes for IEEE address */
#define CFG_MAC_ADDRESS              	(MAC_BASE_ADD)
#define CFG_TELINK_USB_ID				(MAC_BASE_ADD + 0x40)

/*0x76180~0x76fff is for vendor specific use, to store parameters which don't need to
 * be calibrated, can be defined by user*/

/*The block start from 0x77000~0x77fff is used to store the parameters which need to be calibrated */

/*0x77000~0x7703f, 1 byte, used for frequency offset calibration*/
#define CFG_FREQUENCY_OFFSET			(FACTORY_CFG_BASE_ADD)
/*0x77040~0x7707F, 2 byte, 0x77040 for BLE TP_GAIN_0, 0x77041 for BLE TP_GAIN_1,
  0x77042 for zigbee TP_GAIN_0, 0x77043 for zigbee TP_GAIN_1 */
#define CFG_TP_GAIN                  	(FACTORY_CFG_BASE_ADD + 0x42)
/*Not supported for current SDK*/
#define CFG_32K_COUNTER_CALIBRATION	 	(FACTORY_CFG_BASE_ADD + 0x80)
/*Not supported for current SDK*/
#define CFG_ADC_CALIBRATION           	(FACTORY_CFG_BASE_ADD + 0xC0)
/*Not supported for current SDK*/
#define CFG_24M_CRYSTAL_CALIBRATION     (FACTORY_CFG_BASE_ADD + 0x100)
/*Not supported for current SDK*/
#define CFG_T_SENSOR_CALIBRATION     	(FACTORY_CFG_BASE_ADD + 0x140)
/*UID-based Firmware Encryption data(16 bytes), 0x77180 ~ 0x7718F*/
#define CFG_FIRMWARE_ENCRYPTION			(FACTORY_CFG_BASE_ADD + 0x180)

/*******************************************************************************************************
 * Following configuration could be changed by customer.
 */
#if FLASH_SIZE_1M
/* pre-install key
	0x77050:	preCnfLinkKey;	 		pre-configure link key for central network(global: ZC/ZR/ZED; unique: only for ZR/ZED)
	0x77061:	distributeLinkKey;		distribute link key for distribute network
	0x77072:	preCnfTouchlinkLinkKey;	 touch-link key for distribute network
 */
#define CFG_PRE_INSTALL_CODE			(0xFD000)

/* One sector for factory reset by power up/down N times */
#define CFG_FACTORY_RST_CNT			  	(0xFC000)
#else
/* pre-install key
	0x77050:	preCnfLinkKey;	 		pre-configure link key for central network(global: ZC/ZR/ZED; unique: only for ZR/ZED)
	0x77061:	distributeLinkKey;		distribute link key for distribute network
	0x77072:	preCnfTouchlinkLinkKey;	 touch-link key for distribute network
 */
#define CFG_PRE_INSTALL_CODE			(0x78000)

/* One sector for factory reset by power up/down N times */
#define CFG_FACTORY_RST_CNT			  	(0x79000)

#if DUAL_MODE
//NOTE: firmware must less then 192K if UDAL_MODE used
typedef enum{
	TYPE_TLK_MESH  						= 0x000000A3,// don't change, must same with telink mesh SDK
	TYPE_SIG_MESH  						= 0x0000003A,// don't change, must same with telink mesh SDK
	TYPE_TLK_BLE_SDK 					= 0x000000C3,// don't change, must same with telink mesh SDK
	TYPE_TLK_ZIGBEE  					= 0x0000003C,// don't change, must same with telink mesh SDK
	TYPE_DUAL_MODE_STANDBY				= 0x00000065,// dual mode state was standby to be selected
	TYPE_DUAL_MODE_RECOVER 				= 0x00000056,// don't change, must same with telink mesh SDK, recover for mesh
}telink_sdk_type_t;

#define CFG_TELINK_SDK_TYPE				(0x73000)
#define CFG_TELINK_SIG_MESH_CRC			(0x73040)
#define CFG_TELINK_SIG_MESH_CODE_4K		(0x75000)
#define CFG_TELINK_DUAL_MODE_ENABLE		(0x76080)
#endif	/* DUAL_MODE */

#endif

/******************************************** END ***********************************************************/


/* flash write protect */
#define FLASH_PROTECT_NONE              0x00  // unprotect
#define FLASH_PROTECT_CMD_GIGADEVICE  	0x6C  //flash of telink evb
#define FLASH_PROTECT_CMD_ADESTO      	0x3C  //flash of customer selection

#define FLASH_PROTECT_CMD       		FLASH_PROTECT_CMD_ADESTO

#define PAGE_AVALIABLE_SIZE(offset)     (FLASH_PAGE_SIZE - offset)

#define NV_HEADER_TABLE_SIZE           	10
#define NV_ALIGN_LENTH(len)             ((((len) % 4) == 0) ? len : (((len)/4 + 1) * 4))

#define INVALID_NV_VALUE            	0xFF

#define NV_SECTOR_VALID					0x5A5A
#define NV_SECTOR_INVALID				0x5050
#define NV_SECTOR_IDLE					0xFFFF

#define ITEM_FIELD_VALID				0x5A
#define ITEM_FIELD_INVALID				0x50
#define ITEM_FIELD_OPERATION			0xFA
#define ITEM_FIELD_IDLE					0xFF

#define FLASH_FIELD_IDLE				0xFFFF
#define FLASH_FIELD_IDLE_WORD			0xFFFFFFFF

/* sector info(4Bytes) + index info(8Bytes) + index info(8Bytes) + ... */
typedef struct{
	u16 usedFlag;
	u8  idName;
	u8  opSect;
}nv_sect_info_t;

typedef struct{
	u32 offset;
	u16 size;
	u8  itemId;
	u8  usedState;
}nv_info_idx_t;

/* item:  item_hdr(8Bytes) + payload*/
typedef struct{
	u32 checkSum;
	u16 size;
	u8  itemId;
	u8  used;
}itemHdr_t;

typedef struct{
	u16 opIndex;
	u8 	opSect;
}itemIfno_t;


//If OTA enabled, the maximum space used for nv module t is 56K, thus the item num cannot over 14
/*****************************************************************************************************************************
 * Store zigbee information in flash.
 * 		Module ID				|			512K Flash				|			1M Flash				|
 * -----------------------------|-----------------------------------|-----------------------------------|
 * NV_MODULE_ZB_INFO			|		0x34000 - 0x36000			|		0x80000 - 0x82000			|
 * NV_MODULE_ADDRESS_TABLE		|		0x36000 - 0x38000			|		0x82000 - 0x84000			|
 * NV_MODULE_APS				|		0x38000 - 0x3a000			|		0x84000 - 0x86000			|
 * NV_MODULE_ZCL				|		0x3a000 - 0x3c000			|		0x86000 - 0x88000			|
 * NV_MODULE_NWK_FRAME_COUNT	|		0x3c000 - 0x3e000			|		0x88000 - 0x8a000			|
 * NV_MODULE_OTA				|		0x3e000 - 0x40000			|		0x8a000 - 0x8c000			|
 * NV_MODULE_APP				|		0x7a000 - 0x7c000			|		0x8c000 - 0x8e000			|
 * NV_MODULE_KEYPAIR			|		0x7c000 - 0x80000			|		0x8e000 - 0x96000			|
 * 								|	*16K - can store 127 nodes		|	*32K - can store 302 nodes		|
 * NV_MAX_MODULS
 */
typedef enum {
	NV_MODULE_ZB_INFO 				= 0,
	NV_MODULE_ADDRESS_TABLE 		= 1,
    NV_MODULE_APS 					= 2,
    NV_MODULE_ZCL 					= 3,
	NV_MODULE_NWK_FRAME_COUNT 		= 4,
	NV_MODULE_OTA 					= 5,
	NV_MODULE_APP 					= 6,
	NV_MODULE_KEYPAIR 				= 7,
    NV_MAX_MODULS
}nv_module_t;

typedef enum {
	NV_ITEM_ID_INVALID				= 0,/* Item id 0 should not be used. */

	NV_ITEM_ZB_INFO 				= 1,
	NV_ITEM_ADDRESS_FOR_NEIGHBOR,
	NV_ITEM_ADDRESS_FOR_BIND,
	NV_ITEM_APS_SSIB,
	NV_ITEM_APS_GROUP_TABLE,
	NV_ITEM_APS_BINDING_TABLE,

	NV_ITEM_NWK_FRAME_COUNT,

	NV_ITEM_SS_KEY_PAIR,

	NV_ITEM_OTA_HDR_SERVERINFO,
	NV_ITEM_OTA_CODE,

	NV_ITEM_ZCL_REPORT 				= 0x20,
	NV_ITEM_ZCL_ON_OFF,
	NV_ITEM_ZCL_LEVEL,
	NV_ITEM_ZCL_COLOR_CTRL,
	NV_ITEM_ZCL_SCENE_TABLE,
	NV_ITEM_ZCL_GP_PROXY_TABLE,
	NV_ITEM_ZCL_GP_SINK_TABLE,

	NV_ITEM_APP_SIMPLE_DESC,

	NV_ITEM_APP_TEST1,

	NV_ITEM_ID_MAX					= 0xFF,/* Item id 0xFF should not be used. */
}nv_item_t;

typedef enum nv_sts_e {
	NV_SUCC,
    NV_INVALID_MODULS = 1,
    NV_INVALID_ID ,
    NV_ITEM_NOT_FOUND,
    NV_NOT_ENOUGH_SAPCE,
    NV_ITEM_LEN_NOT_MATCH,
    NV_CHECK_SUM_ERROR,
    NV_ENABLE_PROTECT_ERROR,
    NV_NO_MEDIA
} nv_sts_t;

#if FLASH_SIZE_1M
#define NV_SECTOR_SIZE(id)						((id == NV_MODULE_KEYPAIR) ?  (4 * FLASH_SECTOR_SIZE) : FLASH_SECTOR_SIZE)
#define MODULE_INFO_SIZE(id)					((id == NV_MODULE_OTA || id == NV_MODULE_KEYPAIR || id == NV_MODULE_ADDRESS_TABLE) ? ((id == NV_MODULE_KEYPAIR) ? (12*FLASH_PAGE_SIZE) : (4*FLASH_PAGE_SIZE)) : (2*FLASH_PAGE_SIZE))
#else
#define NV_SECTOR_SIZE(id)						((id == NV_MODULE_KEYPAIR) ?  (2 * FLASH_SECTOR_SIZE) : FLASH_SECTOR_SIZE)
#define MODULE_INFO_SIZE(id)					((id == NV_MODULE_OTA || id == NV_MODULE_KEYPAIR || id == NV_MODULE_ADDRESS_TABLE) ? (4*FLASH_PAGE_SIZE) : (2*FLASH_PAGE_SIZE))
#endif

#define MODULE_SECTOR_NUM						(2)

#define MODULE_IDX_SIZE(id)						(MODULE_INFO_SIZE(id) - sizeof(nv_sect_info_t))
#define MODULE_SECT_START(id, sectNo)   		(MODULES_START_ADDR(id) + sectNo * (NV_SECTOR_SIZE(id)))
#define MODULE_SECT_END(id, sectNo)     		(MODULES_START_ADDR(id) + (sectNo + 1) * (NV_SECTOR_SIZE(id)))
#define MODULE_IDX_START(id, sectNo)    		(MODULE_SECT_START(id, sectNo) + sizeof(nv_sect_info_t))
#define MODULE_IDX_NUM(id)    					(MODULE_IDX_SIZE(id)/(sizeof(nv_info_idx_t)))
#define MODULE_CONTEXT_START(id, sectNo, len)  	(MODULE_SECT_START(id, sectNo)+MODULE_INFO_SIZE(id))
#define ITEM_TOTAL_LEN(len)						(len + sizeof(itemHdr_t))
#define FRAMECOUNT_PAYLOAD_START(opSect)		(((MODULE_SECT_START(NV_MODULE_NWK_FRAME_COUNT, opSect)+sizeof(nv_sect_info_t)) + 0x03) & (~0x03))
#define FRAMECOUNT_NUM_PER_SECT()				((((FLASH_SECTOR_SIZE - sizeof(nv_sect_info_t)) & (~0x03)))/4)




nv_sts_t nv_resetAll(void);
nv_sts_t nv_resetModule(u8 modules);
nv_sts_t nv_flashWriteNew(u8 single, u16 id, u8 itemId, u16 len, u8 *buf);
nv_sts_t nv_flashReadNew(u8 single, u8 id, u8 itemId, u16 len, u8 *buf);
nv_sts_t nv_itemDeleteByIndex(u8 id, u8 itemId, u8 opSect, u16 opIdx);
nv_sts_t nv_flashReadByIndex(u8 id, u8 itemId, u8 opSect, u16 opIdx, u16 len, u8 *buf);
nv_sts_t nv_resetToFactoryNew(void);
nv_sts_t nv_nwkFrameCountSaveToFlash(u32 frameCount);
nv_sts_t nv_nwkFrameCountFromFlash(u32 *frameCount);

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
void internalFlashSizeCheck(void);
