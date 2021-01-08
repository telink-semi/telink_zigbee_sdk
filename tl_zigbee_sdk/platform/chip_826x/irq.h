/********************************************************************************************************
 * @file	irq.h
 *
 * @brief	This is the header file for B86
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


// enalbe interrupt
static inline unsigned char irq_enable(void){
	unsigned char r = reg_irq_en;		// don't worry,  the compiler will optimize the return value if not used
	reg_irq_en = 1;
	return r;
}

static inline unsigned char irq_disable(void){
	unsigned char r = reg_irq_en;		// don't worry,  the compiler will optimize the return value if not used
	reg_irq_en = 0;
	return r;
}

static inline void irq_restore(unsigned char en){
	reg_irq_en = en;
}

static inline unsigned int irq_get_mask(void){
	return reg_irq_mask;
}

static inline void irq_set_mask(unsigned long msk){
    SET_FLD(reg_irq_mask, msk);
}

static inline void irq_clr_mask(unsigned long msk){
    CLR_FLD(reg_irq_mask, msk);
}

static inline unsigned long irq_get_src(void){
	return reg_irq_src;
}

static inline void irq_clr_all_src(void){
	reg_irq_src = ONES_32;	// set to clear
}

static inline void irq_enable_type(unsigned long msk){
	irq_set_mask(msk);
}

static inline void irq_disable_type(unsigned long msk){
	irq_clr_mask(msk);
}

static inline void irq_init(unsigned long irq_mask){
	reg_irq_mask = irq_mask;
}

static inline void dma_init(unsigned char dma_mask){
	reg_dma_chn_irq_msk = dma_mask;
}

static inline void dma_irq_clear(void){
	reg_dma_chn_irq_msk = 0;
}

static inline void dma_irq_enable(unsigned char msk)
{
	reg_dma_chn_irq_msk |= msk;
}

static inline void dma_irq_disable(unsigned char msk)
{
	reg_dma_chn_irq_msk &= ~msk;
}

static inline void dma_channel_enable_all(void){
	reg_dma_chn_en = 0xff;
}

static inline void dma_channel_disable_all(void){
	reg_dma_chn_en = 0;
}

/**
 * @brief      Get IRQ status of RF.
 * @param[in]  none
 * @return     none
 */
static inline unsigned short rf_irq_src_get(void){
    return reg_rf_irq_status;
}

static inline void rf_irq_clr_src(unsigned short msk)
{
    reg_rf_irq_status = msk;
}

/**
 * @brief      Get IRQ status of uart.
 * @param[in]  irq_src - select tx or rx irq.
 * @return     none
 */
static inline unsigned char dma_chn_irq_status_get(void){
    return reg_dma_irq_src;
}

/**
 * @brief      Clear IRQ status of uart.
 * @param[in]  irq_src - select tx or rx irq.
 * @return     none
 */
static inline void dma_chn_irq_status_clr(unsigned char irq_status){
	reg_dma_irq_src = irq_status;
}

