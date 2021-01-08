/********************************************************************************************************
 * @file	irq.h
 *
 * @brief	This is the header file for B87
 *
 * @author	Driver & Zigbee Group
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
#pragma once

#include "register.h"
#include "bit.h"

/**
 * @brief      This function servers to enable IRQ.
 * @param[in]  none
 * @return     if return 1 is enable.else disable.
 */
static inline unsigned char irq_enable(void){
	unsigned char r = reg_irq_en;		// don't worry,  the compiler will optimize the return value if not used
	reg_irq_en = 1;
	return r;
}

/**
 * @brief      This function servers to disable IRQ.
 * @param[in]  none
 * @return     if return 0 is disable.else enable.
 */
static inline unsigned char irq_disable(void){
	unsigned char r = reg_irq_en;		// don't worry,  the compiler will optimize the return value if not used
	reg_irq_en = 0;
	return r;
}

/**
 * @brief      This function servers to restore IRQ.
 * @param[in]  none
 * @return     if return 1 is irg restore.
 */
static inline void irq_restore(unsigned char en){
	reg_irq_en = en;
}

/**
 * @brief      This function servers to set IRQ mask.
 * @param[in]  variable of msk.
 * @return     none.
 */
static inline void irq_enable_type(unsigned long msk){
	BM_SET(reg_irq_mask, msk);
}

/**
 * @brief      This function servers to clear IRQ mask.
 * @param[in]  msk - variable of msk.
 * @return     none.
 */
static inline void irq_disable_type(unsigned long msk){
	BM_CLR(reg_irq_mask, msk);
}

/**
 * @brief      This function servers to set IRQ mask.
 * @param[in]  variable of msk.
 * @return     none.
 */
static inline void irq_set_mask(unsigned long msk){
	BM_SET(reg_irq_mask, msk);
}

/**
 * @brief      This function servers to clear IRQ mask.
 * @param[in]  variable of msk.
 * @return     none.
 */
static inline void irq_clr_mask(unsigned long msk){
	BM_CLR(reg_irq_mask, msk);
}

/**
 * @brief      This function servers to enable one interrupt
 * @param[in]  none
 * @return     the value of IRQ register.
 */
static inline unsigned long irq_get_mask(void){
	return reg_irq_mask;
}

/**
 * @brief      This function servers to get an IRQ source.
 * @param[in]  none.
 * @return     IRQ source.
 */
static inline unsigned long irq_get_src(void){
	return reg_irq_src;
}

/**
 * @brief      This function servers to clear all IRQ source.
 * @param[in]  none.
 * @return     none.
 */
static inline void irq_clr_all_src(void)
{
	reg_irq_src = 0xffffffff;
}

/**
 * @brief      This function servers to clear the specified IRQ source.
 * @param[in]  msk - variable of msk.
 * @return     none.
 */
static inline void irq_clr_src(unsigned long msk)
{
    reg_irq_src = msk;
}

/**
 * @brief      This function servers to set the mask of RF IRQ.
 * @param[in]  msk - variable of msk.
 * @return     none.
 */
static inline void rf_irq_enable(unsigned int msk)
{
    reg_rf_irq_mask |= msk;
}

/**
 * @brief      This function servers to clear the mask of RF IRQ.
 * @param[in]  msk - variable of msk.
 * @return     none.
 */
static inline void rf_irq_disable(unsigned int msk)
{
    reg_rf_irq_mask &= (~msk);
}

/**
 * @brief      This function servers to get the RF IRQ source.
 * @param[in]  none.
 * @return     the state of RF IRQ register.
 */
static inline unsigned short rf_irq_src_get(void)
{
    return reg_rf_irq_status;
}

/**
 * @brief      This function servers to clear the RF IRQ source.
 * @param[in]  msk - variable of msk.
 * @return     none.
 */
static inline void rf_irq_clr_src(unsigned short msk)
{
    reg_rf_irq_status = msk;
}

