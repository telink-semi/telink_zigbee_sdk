/********************************************************************************************************
 * @file    stimer.h
 *
 * @brief   This is the header file for TL721X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
/** @page STIMER
 *
 *  Introduction
 *  ===============
 *  stimer use 24M clock count, have stimer irq.
 *
 *  API Reference
 *  ===============
 *  Header File: stimer.h
 */
#ifndef STIMER_H_
#define STIMER_H_

#include <stdbool.h>
#include "compiler.h"
#include "reg_include/stimer_reg.h"
#include "dma.h"
#include "gpio.h"
#include "pem.h"

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
#ifndef SYS_TIMER_AUTO_MODE
    #define SYS_TIMER_AUTO_MODE 1
#endif

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 * @brief define system clock tick per us/ms/s.
 */
enum
{
    SYSTEM_TIMER_TICK_1US = 24,
    SYSTEM_TIMER_TICK_1MS = 24000,
    SYSTEM_TIMER_TICK_1S  = 24000000,

    SYSTEM_TIMER_TICK_625US  = 15000, //625*24
    SYSTEM_TIMER_TICK_1250US = 30000, //1250*24
};

/**
 * @brief define stimer capture mode.
 */
typedef enum
{
    CAPT_RISING_EDGE         = 0,
    CAPT_FALLING_EDGE        = 1,
    CAPT_RISING_FALLING_EDGE = 2,
} stimer_capt_mode_e;

/**
 * @brief   This enumeration defines how many 32k ticks are reached, update the tick value of the increased system timer into the register.
 *          //32k_cnt = 2^(16-(cal_32k_mode>>4)), system_timer_cnt = 750*32k_cnt.(system timer is 24M).
 */
typedef enum
{
    STIMER_TRACK_32KCNT_2     = 0xf0, /* 2 32k ticks, corresponds 0x5dc system timer ticks. */
    STIMER_TRACK_32KCNT_4     = 0xe0, /* 4 32k ticks, corresponds 0xbb8 system timer ticks. */
    STIMER_TRACK_32KCNT_8     = 0xd0, /* 8 32k ticks, corresponds 0x1770 system timer ticks. */
    STIMER_TRACK_32KCNT_16    = 0xc0, /* 16 32k ticks, corresponds 0x2ee0 system timer ticks. */
    STIMER_TRACK_32KCNT_32    = 0xb0, /* 32 32k ticks, corresponds 0x5dc0 system timer ticks. */
    STIMER_TRACK_32KCNT_64    = 0xa0, /* 64 32k ticks, corresponds 0xbb80 system timer ticks. */
    STIMER_TRACK_32KCNT_128   = 0x90, /* 128 32k ticks, corresponds 0x17700 system timer ticks. */
    STIMER_TRACK_32KCNT_256   = 0x80, /* 256 32k ticks, corresponds 0x2ee00 system timer ticks. */
    STIMER_TRACK_32KCNT_512   = 0x70, /* 512 32k ticks, corresponds 0x5dc00 system timer ticks. */
    STIMER_TRACK_32KCNT_1024  = 0x60, /* 1024 32k ticks, corresponds 0xbb800 system timer ticks. */
    STIMER_TRACK_32KCNT_2048  = 0x50, /* 2048 32k ticks, corresponds 0x177000 system timer ticks. */
    STIMER_TRACK_32KCNT_4096  = 0x40, /* 4096 32k ticks, corresponds 0x2ee000 system timer ticks. */
    STIMER_TRACK_32KCNT_8192  = 0x30, /* 8192 32k ticks, corresponds 0x5dc000 system timer ticks. */
    STIMER_TRACK_32KCNT_16384 = 0x20, /* 16384 32k ticks, corresponds 0xbb8000 system timer ticks. */
    STIMER_TRACK_32KCNT_32768 = 0x10, /* 32768 32k ticks, corresponds 0x1770000 system timer ticks. */
    STIMER_TRACK_32KCNT_65536 = 0x00, /* 65536 32k ticks, corresponds 0x2ee0000 system timer ticks. */
} stimer_track_cnt_e;

typedef enum
{
    STIMER_MANUAL_MODE                   = 0x00, /**< Write a register to start the system timer. */
    STIMER_AUTO_MODE_W_TRIG              = 0x01, /**< When you write the tick value of the system timer,
                                                the system timer starts automatically. */
    STIMER_AUTO_MODE_W_AND_NXT_32K_START = 0x02, /**< Configure this mode:
                                                When the first 32k scale rises, the system timer starts automatically.*/
    STIMER_AUTO_MODE_W_AND_NXT_32K_DONE  = 0x03, /**< After the tick value is written to the system timer,
                                                the system timer automatically starts when the first 32k tick rises. */
} stimer_enable_mode_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/
extern unsigned int g_track_32kcnt;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief       This function servers to set system timer irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static _always_inline void stimer_set_irq_mask(stimer_irq_mask_e mask)
{
    reg_system_irq_mask |= mask;
}

/**
 * @brief       This function servers to clear system timer irq mask.
 * @param[in]   mask - the irq mask.
 * @return      none.
 */
static _always_inline void stimer_clr_irq_mask(stimer_irq_mask_e mask)
{
    reg_system_irq_mask &= (~mask);
}

/**
 * @brief       This function servers to clear system timer irq status.
 * @param[in]   status - the irq status.
 * @return      none.
 */
static _always_inline void stimer_clr_irq_status(stimer_irq_status_e status)
{
    reg_system_cal_irq = (status);
}

/**
 * @brief       This function servers to get system timer irq status.
 * @param[in]   status - the irq status.
 * @retval      non-zero -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static _always_inline unsigned char stimer_get_irq_status(stimer_irq_status_e status)
{
    return (reg_system_cal_irq & status);
}

/**
 * @brief       This function servers to set tick irq capture.
 * @param[in]   tick - the value of irq tick.
 * @return      none.
 */
static _always_inline void stimer_set_irq_capture(unsigned int tick)
{
    reg_system_irq_level = (tick);
}

/**
 * @brief       This function is used to set the tick value of the system timer.
 * @param[in]   tick - the value of system timer tick.
 * @return      none.
 */
static _always_inline void stimer_set_tick(unsigned int tick)
{
    reg_system_tick = (tick);
}

/**
 * @brief     This function performs to get system timer tick.
 * @return    system timer tick value.
 */
static _always_inline unsigned int stimer_get_tick(void)
{
    return reg_system_tick;
}

/**
 * @brief     This function serves to set timeout by us.
 * @param[in] ref  - reference tick of system timer .
 * @param[in] us   - count by us.
 * @return    true - timeout, false - not timeout
 */
static _always_inline _Bool clock_time_exceed(unsigned int ref, unsigned int us)
{
    return ((unsigned int)(stimer_get_tick() - ref) > us * SYSTEM_TIMER_TICK_1US);
}

/**
 * @brief     This function performs to set delay time by us.
 * @param[in] microsec - need to delay.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void delay_us(unsigned int microsec);

/**
 * @brief     This function performs to set delay time by ms.
 * @param[in] millisec - need to delay.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void delay_ms(unsigned int millisec);

/**
 * @brief     This function servers to enable system timer input capture mode.
 * @return    none.
 */
static _always_inline void stimer_set_input_capt_enable(void)
{
    reg_system_timer_ctrl |= FLD_SYSTEM_CAPT_EN;
}

/**
 * @brief     This function servers to disable stimer input capture mode.
 * @return    none.
 */
static _always_inline void stimer_set_input_capt_disable(void)
{
    reg_system_timer_ctrl &= ~(FLD_SYSTEM_CAPT_EN);
}

/**
 * @brief     This function servers to set system timer input capture mode.
 * @param[in] capt_mode     - input capture mode.
 * @return    none.
 */
static _always_inline void stimer_set_input_capt_mode(stimer_capt_mode_e capt_mode)
{
    reg_system_timer_ctrl = (reg_system_timer_ctrl & (~FLD_SYSTEM_CAPT_MODE)) | capt_mode;
}

/**
 * @brief     This function performs to get system timer capture value.
 * @return    system timer capture value.
 */
static _always_inline unsigned int stimer_get_input_capt_value(void)
{
    return reg_system_timer_capt;
}

/**
 * @brief     This function performs to enable systimer pem.
 * @param[in] stimer_pem_en  - systimer pem mode enable.
 * @return    none.
 */
static inline void stimer_set_pem(stimer_ctrl_e stimer_pem_en)
{
    reg_system_timer_ctrl |= stimer_pem_en;
}

/**
 * @brief     This function performs to disable systimer pem.
 * @param[in] stimer_pem_dis  - systimer pem mode disable.
 * @return    none.
 */
static inline void stimer_pem_dis(stimer_ctrl_e stimer_pem_dis)
{
    reg_system_timer_ctrl &= (~stimer_pem_dis);
}

/**
 * @brief       This function serves to set the input capture function.
 *              If this feature is used, it will take up the gpio2risc0_irq interrupt and cannot be used elsewhere.
 * @param[in]   pin         - the pin needs to set.
 * @param[in]   capt_mode   - input capture mode.
 * @return      none.
 */
void stimer_set_input_capt_pin(gpio_pin_e pin, stimer_capt_mode_e capt_mode);

/**
 * @brief       This  function serves to set stimer dma.
 * @param[in]   stimer_dma_chn - dma channel.
 * @return      none
 */
void stimer_set_input_capt_dma_config(dma_chn_e chn);

/**
 * @brief       This  function serves to set stimer dma.
 * @param[in]   stimer_dma_chn - dma channel.
 * @param[in]   dst_addr    - the address of destination.
 * @param[in]   len         - the length of data.
 * @return      none
 */
void stimer_set_input_capt_value_dma(dma_chn_e chn, unsigned int *dst_addr, unsigned int len);

/**
 * @brief     This function servers to configure DMA head node.
 * @param[in] chn          - to select the DMA channel.
 * @param[in] dst_addr     - the dma address of destination.
 * @param[in] data_len     - to configure DMA length.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void stimer_set_input_capt_dma_chain_llp(dma_chn_e dma_chn, unsigned int *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
 * @brief     This function servers to configure DMA cycle chain node.
 * @param[in] chn         - to select the DMA channel.
 * @param[in] config_addr - to servers to configure the address of the current node.
 * @param[in] llpointer   - to configure the address of the next node configure.
 * @param[in] dst_addr    - the dma address of destination.
 * @param[in] data_len    - to configure DMA length.
 * @return    none
 */
void stimer_input_capt_dma_add_list_element(dma_chn_e dma_chn, unsigned int *dst_addr, unsigned int data_len, dma_chain_config_t *config_addr, dma_chain_config_t *llpointer);

/**********************************************************************************************************************
 *                          Internal interface, provided only for driver use.                                         *
 *********************************************************************************************************************/

/**
 * @brief   This function is used to enable system timer in manual mode.
 * @return  none.
 */
static _always_inline void stimer_enable_in_manual_mode(void)
{
    reg_system_ctrl |= FLD_SYSTEM_TIMER_EN;
}

/**
 * @brief   This interface is used to query whether the system timer is started in automatic or manual mode.
 * @return  Start mode of the system timer. 4:auto mode. 0:manual mode.
 */
static _always_inline unsigned char stimer_get_enable_mode(void)
{
    return (reg_system_ctrl & FLD_SYSTEM_TIMER_AUTO);
}

/**
 * @brief   This function is used to enable automatic mode for system timer.
 * @return  none.
 */
static _always_inline void stimer_set_auto_enable_mode(void)
{
    reg_system_ctrl |= FLD_SYSTEM_TIMER_AUTO;
}

/**
 * @brief   This function is used to disable automatic mode for system timer.
 * @return  none.
 */
static _always_inline void stimer_set_manual_enable_mode(void)
{
    reg_system_ctrl &= ~(FLD_SYSTEM_TIMER_AUTO);
}

/**
 * @brief   This function is used to enable the function to trigger the operation at the next 32k rise edge.
 * @return  none.
 */
static _always_inline void stimer_set_run_upon_nxt_32k_enable(void)
{
    reg_system_up_32k |= FLD_SYSTEM_RUN_UPON_NXT_32K;
}

/**
 * @brief   This function is used to disable the function to trigger the operation at the next 32k rise edge.
 * @return  none.
 */
static _always_inline void stimer_set_run_upon_nxt_32k_disable(void)
{
    reg_system_up_32k &= ~(FLD_SYSTEM_RUN_UPON_NXT_32K);
}

/**
 * @brief   This function is used to enable the function to update the tick value of system timer at the next 32k rise edge.
 * @return  none.
 */
static _always_inline void stimer_set_update_upon_nxt_32k_enable(void)
{
    reg_system_up_32k |= FLD_SYSTEM_UPDATE_UPON_32K;
}

/**
 * @brief   This function is used to disable the function to update the tick value of system timer at the next 32k rise edge.
 * @return  none.
 */
static _always_inline void stimer_set_update_upon_nxt_32k_disable(void)
{
    reg_system_up_32k &= ~(FLD_SYSTEM_UPDATE_UPON_32K);
}

/**
 * @brief   This function is used to wait for the system timer to write successfully.
 * @return  none.
 */
static _always_inline void stimer_wait_write_done(void)
{
    while ((reg_system_st & FLD_SYSTEM_CMD_SET_DLY_DONE) == 0)
        ;
}

/**
 * @brief   This function is used to enable 32k tracing.
 * @return  none.
 */
static _always_inline void stimer_32k_tracking_enable(void)
{
    reg_system_ctrl |= FLD_SYSTEM_32K_TRACK_EN;
}

/**
 * @brief   This function is used to disable 32k tracing.
 * @return  none.
 */
static _always_inline void stimer_32k_tracking_disable(void)
{
    reg_system_ctrl &= ~(FLD_SYSTEM_32K_TRACK_EN);
}

/**
 * @brief   This interface is used to configure the 32k track mode.
 *          (2^(16-(cnt>>4)))cycles of 32k clock indicates the tick number of the system timer.
 * @return  none.
 */
static _always_inline void stimer_set_32k_track_cnt(stimer_track_cnt_e cnt)
{
    reg_system_ctrl = (reg_system_ctrl & 0x0f) | cnt;
    g_track_32kcnt  = (1 << (16 - (cnt >> 4)));
}

/**
 * @brief   This interface is used to obtain 32k track count.
 * @return  32k track count.
 */
static _always_inline unsigned char stimer_get_32k_track_cnt(void)
{
    return g_track_32kcnt;
}

/**
 * @brief     This function performs to get the number of cycles of the RC 32k clock converted to system timer clock.
 * @return    none.
 */
static _always_inline unsigned int stimer_get_tracking_32k_value(void)
{
    return reg_system_cal_latch_32k;
}

/**
 * @brief     This interface is used to configure the write mode of the system timer to the 32k timer.
 * @return    none.
 */
static _always_inline void stimer_set_32k_write_mode(void)
{
    reg_system_ctrl |= FLD_SYSTEM_32K_WR_EN;
}

/**
 * @brief     This interface is used to configure the read mode of the system timer to the 32k timer.
 * @return    none.
 */
static _always_inline void stimer_set_32k_read_mode(void)
{
    reg_system_ctrl &= ~FLD_SYSTEM_32K_WR_EN;
}

/**
 * @brief     This interface is used to wait for the completion of reading 32k timers using system timers.
 * @return    none.
 */
static _always_inline void stimer_wait_read_32k_done(void)
{
    while (reg_system_st & FLD_SYSTEM_RD_BUSY)
        ;
}

/**
 * @brief     This interface is used to configure the 32k tick value to be written.
 * @return    none.
 */
static _always_inline void stimer_set_32k_tick(unsigned int tick)
{
    reg_system_timer_set_32k = tick;
}

/**
 * @brief     This interface is used to trigger the system timer to write the 32k timer.
 * @return    none.
 */
static _always_inline void stimer_set_32k_tick_write_trig(void)
{
    reg_system_st = FLD_SYSTEM_CMD_SYNC;
}

/**
 * @brief     This interface is used to wait for the completion of writing 32k timers using system timers.
 * @return    none.
 */
static _always_inline void stimer_wait_write_32k_done(void)
{
    while (reg_system_st & FLD_SYSTEM_CMD_SYNC)
        ;
}

/**
 * @brief       This function is used to start the system timer.
 * @param[in]   mode    - starting mode.
 * @param[in]   tick    - The initial value of the tick at startup.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void stimer_enable(stimer_enable_mode_e mode, unsigned int tick);

/**
 * @brief       This function is used to stop the system timer.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void stimer_disable(void);

#endif /* STIMER_H_ */
