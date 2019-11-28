/********************************************************************************************************
 * @file     zcl_nv.c
 *
 * @brief	 the flash APIS for reading or writing cluster attributes
 *
 * @author
 * @date     June. 10, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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


/**********************************************************************
 * INCLUDES
 */
#include "zcl_include.h"
#include "zcl_nv.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      zcl_reportingTab_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_reportingTab_save(void)
{
	nv_sts_t st = NV_SUCC;

#if NV_ENABLE
	st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_REPORT, sizeof(zcl_reportingTab_t), (u8*)&reportingTab);
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif

	return st;
}

/*********************************************************************
 * @fn      zcl_reportingTab_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_reportingTab_restore(void)
{
	nv_sts_t st = NV_SUCC;

#if NV_ENABLE
	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_REPORT, sizeof(zcl_reportingTab_t), (u8*)&reportingTab);
	for(u8 i = 0; i < reportingTab.reportNum; i++){
		memset(reportingTab.reportCfgInfo[i].prevData, 0, REPORTABLE_CHANGE_MAX_ANALOG_SIZE);
	}
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif

	return st;
}
#endif /* ZCL_REPORT */

/*********************************************************************
 * @fn      zcl_sceneTable_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_sceneTable_save(void)
{
	nv_sts_t st = NV_SUCC;

#ifdef ZCL_SCENE
#if NV_ENABLE
	st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_SCENE_TABLE, sizeof(zcl_sceneTable_t), (u8*)&g_zcl_sceneTab);
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

	return st;
}

/*********************************************************************
 * @fn      zcl_sceneTable_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_sceneTable_restore(void)
{
	nv_sts_t st = NV_SUCC;

#ifdef ZCL_SCENE
#if NV_ENABLE
	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_SCENE_TABLE, sizeof(zcl_sceneTable_t), (u8*)&g_zcl_sceneTab);
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

	return st;
}




