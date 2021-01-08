/********************************************************************************************************
 * @file	zcl_nv.c
 *
 * @brief	This is the source file for zcl_nv
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
/**********************************************************************
 * INCLUDES
 */
#include "zcl_include.h"


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
	st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_SCENE_TABLE, sizeof(zcl_sceneTable_t) * ZCL_SCENE_TABLE_NUM, (u8*)&g_zcl_sceneTab);
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
	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_SCENE_TABLE, sizeof(zcl_sceneTable_t) * ZCL_SCENE_TABLE_NUM, (u8*)&g_zcl_sceneTab);
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

	return st;
}




