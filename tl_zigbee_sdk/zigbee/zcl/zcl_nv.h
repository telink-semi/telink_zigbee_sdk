/********************************************************************************************************
 * @file     zcl_nv.h
 *
 * @brief	 APIs statement for cluster flash
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
#pragma once

#include "tl_common.h"

/***********************************************************************
 * @brief   Save reporting table information
 *
 * @param
 *
 * @return	nv_sts_t
 *
 */
nv_sts_t zcl_reportingTab_save(void);

/***********************************************************************
 * @brief   Restore reporting table information
 *
 * @param
 *
 * @return	nv_sts_t
 *
 */
nv_sts_t zcl_reportingTab_restore(void);

/***********************************************************************
 * @brief   Save scene table information
 *
 * @param
 *
 * @return
 *
 */
nv_sts_t zcl_sceneTable_save(void);

/***********************************************************************
 * @brief   Restore scene table information
 *
 * @param
 *
 * @return
 *
 */
nv_sts_t zcl_sceneTable_restore(void);





/***********************************************************************//**
 * @brief       Save onOff attributes information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_attr_onOffParams_save(void);

/***********************************************************************//**
 * @brief       Restore onOff attributes information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_attr_onOffParams_restore(void);

/***********************************************************************//**
 * @brief       Save level attributes information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_attr_levelParams_save(void);

/***********************************************************************//**
 * @brief       Restore level attributes information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_attr_levelParams_restore(void);

/***********************************************************************//**
 * @brief       Save color control attributes information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_attr_colorCtrlParams_save(void);

/***********************************************************************//**
 * @brief       Restore color control attributes information
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
nv_sts_t zcl_attr_colorCtrlParams_restore(void);






 
