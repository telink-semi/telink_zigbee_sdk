/********************************************************************************************************
 * @file	stimer.h
 *
 * @brief	This is the header file for B91
 *
 * @author	B.Y
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
/**	@page STIMER
 *
 *	Introduction
 *	===============
 *	TLSRB91 stimer use 16M clock count, have stimer irq. 
 *
 *	API Reference
 *	===============
 *	Header File: uart.h
 */
#ifndef STIMER_H_
#define STIMER_H_

#include "reg_include/stimer_reg.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/**
 * @brief This function servers to set stimer irq mask.
 * @param[in]	mask - the irq mask.
 * @return  	none.
 */
static inline void stimer_set_irq_mask(stimer_irq_e mask)
{
	reg_system_irq_mask |= mask;
}

/**
 * @brief This function servers to clear stimer irq mask.
 * @param[in] 	mask - the irq mask.
 * @return  	none.
 */
static inline void stimer_clr_irq_mask(stimer_irq_e mask)
{
	reg_system_irq_mask &= (~mask);
}

/**
 * @brief This function servers to clear stimer irq status.
 * @param[in] 	status - the irq status.
 * @return  	none.
 */
static inline void stimer_clr_irq_status(stimer_irq_e status)
{
	reg_system_cal_irq = (status);
}

/**
 * @brief This function servers to get stimer irq status.
 * @param[in] 	status - the irq status.
 * @return      none.
 */
static inline unsigned char stimer_get_irq_status(stimer_irq_e status)
{
	return (reg_system_cal_irq & status);
}

/**
 * @brief This function servers to set tick irq capture.
 * @param[in] tick - the value of irq tick.
 * @return    none.
 */
static inline void stimer_set_irq_capture(unsigned int tick)
{
	reg_system_irq_level = (tick);
}

/**
 * @brief This function servers to set stimer tick.
 * @param[in] tick - the value of tick.
 * @return    none.
 */
static inline void stimer_set_tick(unsigned int tick)
{
	reg_system_tick = (tick);
}

/**
 * @brief This function servers to enable stimer.
 * @return  none.
 */
static inline void stimer_enable(void)
{
	reg_system_ctrl |= FLD_SYSTEM_TIMER_EN;
}

/**
 * @brief This function servers to disable stimer.
 * @return  none.
 */
static inline void stimer_disable(void)
{
	reg_system_ctrl &= ~(FLD_SYSTEM_TIMER_EN);
}



#define SYS_TIMER_INIT()				do{	\
											plic_interrupt_enable(IRQ1_SYSTIMER);	\
										}while(0)

#define SYS_TIMER_START()				do{ \
											stimer_set_irq_mask(FLD_SYSTEM_IRQ);	\
										}while(0)

#define SYS_TIMER_STOP()				do{ \
											stimer_clr_irq_mask(FLD_SYSTEM_IRQ);	\
										}while(0)

#define SYS_TIMER_INTERVAL_SET(cyc)		do{	\
											stimer_set_irq_capture(cyc);			\
										}while(0)

#define SYS_TIMER_STATE_CLEAR()			do{	\
											stimer_clr_irq_status(FLD_SYSTEM_IRQ);	\
										}while(0)

#endif /* STIMER_H_ */
