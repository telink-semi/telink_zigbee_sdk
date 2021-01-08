/********************************************************************************************************
 * @file	zcl_nv.h
 *
 * @brief	This is the header file for zcl_nv
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
#ifndef ZCL_NV_H
#define ZCL_NV_H


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






 
#endif	/* ZCL_NV_H */
