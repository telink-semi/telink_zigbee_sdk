/********************************************************************************************************
 * @file     irq.h
 *
 * @brief    IRQ configuration interface for tlsr8258
 *
 * @author   jian.zhang@telink-semi.com
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

#include "register.h"


static inline unsigned char irq_enable(){
	unsigned char r = reg_irq_en;		// don't worry,  the compiler will optimize the return value if not used
	reg_irq_en = 1;
	return r;
}
static inline unsigned char irq_disable(){
	unsigned char r = reg_irq_en;		// don't worry,  the compiler will optimize the return value if not used
	reg_irq_en = 0;
	return r;
}
static inline void irq_restore(unsigned char en){
	reg_irq_en = en;
}

// enable one interrupt
static inline unsigned long irq_get_mask(){
	return reg_irq_mask;
}

static inline void irq_set_mask(unsigned long msk){
	BM_SET(reg_irq_mask, msk);
}

static inline void irq_clr_mask(unsigned long msk){
	BM_CLR(reg_irq_mask, msk);
}

// interrupt source
static inline unsigned long irq_get_src(){
	return reg_irq_src;
}

static inline void irq_clr_src(){
	reg_irq_src = 0xffffffff;	// set to clear
}

static inline void irq_clr_src2(unsigned long msk){
    reg_irq_src |= msk;   // set to clear
}
static inline void irq_enable_type(unsigned long msk){
	irq_set_mask(msk);
}
static inline void irq_disable_type(unsigned long msk){
	irq_clr_mask(msk);
}


#define IRQ_Enable				 irq_enable
#define IRQ_Disable				 irq_disable
#define IRQ_Restore				 irq_restore
#define IRQ_MaskSet				 irq_set_mask
static inline void rf_irq_enable(unsigned int msk)
{
    reg_rf_irq_mask |= msk;
}
static inline void rf_irq_disable(unsigned int msk)
{
    reg_rf_irq_mask &= (~msk);
}
static inline unsigned short rf_irq_src_get(void)
{
    return reg_rf_irq_status;
}
static inline void rf_irq_src_clr(unsigned short msk)
{
    reg_rf_irq_status |= msk;
}

