/********************************************************************************************************
 * @file	drv_timer.h
 *
 * @brief	This is the header file for drv_timer
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
#pragma once


#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define S_TIMER_CLOCK_1US				sysTimerPerUs
	#define H_TIMER_CLOCK_1US				(CLOCK_SYS_CLOCK_HZ / 1000000)
#elif defined(MCU_CORE_B91)
	#define S_TIMER_CLOCK_1US				sysTimerPerUs
	#define H_TIMER_CLOCK_1US				(sys_clk.pclk)

	#define clock_time()					stimer_get_tick()
	#define WaitUs(us)						delay_us(us)
	#define WaitMs(ms)						delay_ms(ms)
#endif

#define TIMER_TICK_1US_GET(idx)				((idx < TIMER_IDX_3) ? H_TIMER_CLOCK_1US : S_TIMER_CLOCK_1US)
 
/**
 * hardware_timer_index Hardware Timer Index
 */
#define TIMER_IDX_0             			0 //!< Timer0
#define TIMER_IDX_1             			1 //!< Timer1
#define TIMER_IDX_2             			2 //!< Timer2, for Watch dog.
#define TIMER_IDX_3							3 //!< SYS Timer, for MAC-CSMA.
#define TIMER_NUM               			4 //!< Total number of timer


/**
 * hardware_timer_mode Hardware Timer Mode
 */
#define TIMER_MODE_SCLK         			0 //!< Timer running in the system clock mode, it will free run from 0 to 0xffffffff
#define TIMER_MODE_GPIO         			1
#define TIMER_MODE_WIDTH_GPIO   			2
#define TIMER_MODE_TICK_MODE    			3

/**
 * hardware_timer_state Hardware Timer State
 */
#define TIMER_IDLE              			0 //!< Indicating the timer is not running
#define TIMER_WOF               			1 //!< Waiting for overflow
#define TIMER_WTO               			2 //!< Waiting for time out


/**
 *  @brief  Definition for 64 bit timer unit
 */
typedef struct{
    u32 low;
    u32 high;
}ext_clk_t;

/**
 *  @brief  Status of Hardware Timer
 */
typedef enum hw_timer_sts_e{
	HW_TIMER_SUCC 		= 0,
    HW_TIMER_IS_RUNNING = 1,
	HW_TIMER_INVALID,
}hw_timer_sts_t;

/**
 *  @brief  Definition for Timer callback function type
 */
typedef int (*timerCb_t)(void *p);


 /**
  * @brief       Initialize the specified hardware timer
  *
  * @param[in]   tmrIdx - Index of timer @ref hardware_timer_index
  * @param[in]   mode   - Specify the timer running mode @ref hardware_timer_mode
  *
  * @return      None
  */
void drv_hwTmr_init(u8 tmrIdx, u8 mode);

 /**
  * @brief       Set a new timer through relative time value
  *
  * @param[in]   tmrIdx   - Index of timer @ref hardware_timer_index
  * @param[in]   interval - The time interval from current time. The unit is microsecond
  * @param[in]   func     - Pointer to the callback function to be invoked when the timer expiry
  * @param[in]   arg      - Argument of callback function
  *
  * @return      Status
  */
hw_timer_sts_t drv_hwTmr_set(u8 tmrIdx, u32 t_us, timerCb_t func, void *arg);

/**
  * @brief       Cancel an existed timer
  *
  * @param[in]   tmrIdx  - Index of timer @ref hardware_timer_index
  *
  * @return      None
  */
void drv_hwTmr_cancel(u8 tmrIdx);

/**
  * @brief       Interrupt handler of Timer
  *
  * @param       None
  *
  * @return      None
  */
void drv_timer_irq0_handler(void);
void drv_timer_irq1_handler(void);
void drv_timer_irq3_handler(void);



