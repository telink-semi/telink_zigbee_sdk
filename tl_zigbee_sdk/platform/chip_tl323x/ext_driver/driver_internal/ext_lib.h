/********************************************************************************************************
 * @file    ext_lib.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef DRIVERS_TL323X_EXT_DRIVER_EXT_LIB_H_
#define DRIVERS_TL323X_EXT_DRIVER_EXT_LIB_H_


#include "types.h"
#include "../../uart.h"
#include "../../lib/include/pm/pm.h"
#include "lib/include/rf/rf_common.h"
#include <stdbool.h>
#include "../ext_pm.h"
#include "../../lib/include/stimer.h"



/******************************* debug_start ******************************************************************/
void sub_wr_ana(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);
void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);
/******************************* debug_end ********************************************************************/


/******************************* dbgport start ******************************************************************/
void dbg_bb_set_pin(gpio_pin_e pin);

void rf_enable_bb_debug(void);
/******************************* dbgport end ********************************************************************/


/******************************* dbgErrorCode start ******************************************************************/
/* for debug (write ram)*/
#define DBG_SRAM_ADDR                   0x00014

#define PKE_OPERAND_MAX_WORD_LEN      (0x08)
#define PKE_OPERAND_MAX_BIT_LEN       (0x100)
#define ECC_MAX_WORD_LEN              PKE_OPERAND_MAX_WORD_LEN
/*
 * addr - only 0x00012 ~ 0x00021 can be used !!! */
#define write_dbg32(addr, value)        write_sram32(addr, value)

#define write_log32(err_code)           write_sram32(0x00014, err_code)
/******************************* dbgErrorCode end ********************************************************************/


/******************************* ext_aes start ******************************************************************/
#define HW_AES_CCM_ALG_EN                                       0


void aes_encryption_le(u8* key, u8* plaintext, u8 *encrypted_data);
void aes_encryption_be(u8* key, u8* plaintext, u8 *encrypted_data);

bool aes_resolve_irk_rpa(u8 *key, u8 *addr);


#define big_integer_compare     uint32_BigNumCmp
#define pke_eccp_point_mul      eccp_pointMul
#define pke_eccp_point_verify   eccp_pointVerify


#define ext_crypto_hw_init_enable()   do{ \
pke_reset(); \
pke_clk_en(); \
ske_reset();  \
ske_clk_en();  \
}while(0);

/******************************* ext_aes end ********************************************************************/



/******************************* ext_stimer start ******************************************************************/
#define SYSTICK_NUM_PER_US              24

#define SSLOT_TICK_NUM                  1875/4    //attention: not use "()" for purpose !!!    625uS*24/32=625*3/4=1875/4=468.75
#define SSLOT_TICK_REVERSE              4/1875    //attention: not use "()" for purpose !!!


/**
 * @brief define system clock tick per us/ms/s.
 */
enum{
    SYSTEM_TIMER_TICK_125US     = 3000,   //125*24
};

/**
 * @brief    This function serves to enable system timer interrupt.
 * @return  none
 */
#define systimer_irq_enable()               plic_interrupt_enable(IRQ_SYSTIMER)

#define systimer_irq_disable()              plic_interrupt_disable(IRQ_SYSTIMER)

#define systimer_set_irq_mask()             stimer_set_irq_mask(FLD_SYSTEM_IRQ_MASK)
#define systimer_clr_irq_mask()             stimer_clr_irq_mask(stimer_clr_irq_mask)

#define systimer_get_irq_status()           stimer_get_irq_status(FLD_SYSTEM_IRQ)

#define systimer_clr_irq_status()           stimer_clr_irq_status(FLD_SYSTEM_IRQ);

#define systimer_set_irq_capture(tick)      stimer_set_irq_capture(tick)

#define systimer_get_irq_capture()          (reg_system_irq_level)


static inline int tick1_exceed_tick2(u32 tick1, u32 tick2)
{
    return (u32)(tick1 - tick2) < BIT(30);
}


static inline int tick1_closed_to_tick2(unsigned int tick1, unsigned int tick2, unsigned int tick_distance)
{
    return (unsigned int)(tick1 + tick_distance - tick2) < (tick_distance<<1);
}

static inline int tick1_out_range_of_tick2(unsigned int tick1, unsigned int tick2, unsigned int tick_distance)
{
    return (unsigned int)(tick1 + tick_distance - tick2) > (tick_distance<<1);
}

/******************************* ext_stimer end ********************************************************************/



/******************************* ext_pm start ******************************************************************/
#ifndef PM_32k_RC_CALIBRATION_ALGORITHM_EN
#define PM_32k_RC_CALIBRATION_ALGORITHM_EN              1
#endif

#define SYS_NEED_REINIT_EXT32K              BIT(1)
#define WAKEUP_STATUS_TIMER_CORE            ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define WAKEUP_STATUS_TIMER_PAD             ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)

extern  unsigned int            g_sleep_32k_rc_cnt;
extern  unsigned int            g_sleep_stimer_tick;

extern unsigned int ota_program_bootAddr;
extern unsigned int ota_firmware_max_size;
extern unsigned int ota_program_offset;

/**
 * @brief   pm 32k rc calibration algorithm.
 */
typedef struct __attribute__((packed))   pm_clock_drift
{
    unsigned int    ref_tick;
    unsigned int    ref_tick_32k;
    unsigned int    ref_tick_enter;
    unsigned int    ref_tick_32k_enter;
    int             offset;
    int             offset_dc;
//  int             offset_cur;
    unsigned int    offset_cal_tick;
    int             tc;
    int             rc32;
    int             rc32_wakeup;
    int             rc32_rt;
    int             s0;
    unsigned char   calib;
    unsigned char   ref_no;
} pm_clock_drift_t;


extern pm_clock_drift_t pmbcd;

static inline unsigned int pm_ble_get_latest_offset_cal_time(void)
{
    return pmbcd.offset_cal_tick;
}

static inline unsigned int pm_ble_get_latest_ref_tick(void){
    return pmbcd.ref_tick;
}

static inline unsigned int pm_ble_get_latest_ref_tick_32k(void){
    return pmbcd.ref_tick_32k;
}

/**
 * @brief       Calculate the offset value based on the difference of 16M tick.
 * @param[in]   offset_tick - the 16M tick difference between the standard clock and the expected clock.
 * @param[in]   rc32_cnt    - 32k count
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_ble_cal_32k_rc_offset (int offset_tick, int rc32_cnt);

/**
 * @brief       This function reset calibrates the value
 * @param[in]   none
 * @return      none.
 */
void pm_ble_32k_rc_cal_reset(void);
#define PM_MIN_SLEEP_US         1500  //B92 todo

/**
 * @brief   internal oscillator or crystal calibration for environment change such as voltage, temperature
 *          to keep some critical PM or RF performance stable
 *          attention: this is a stack API, user can not call it
 * @param   none
 * @return  none
 */
void mcu_oscillator_crystal_calibration(void);

typedef int (*suspend_handler_t)(void);
typedef unsigned int (*pm_tim_recover_handler_t)(unsigned int);

extern  suspend_handler_t           func_before_suspend;
extern  pm_tim_recover_handler_t    pm_tim_recover;



/**
 * @brief   This function serves to recover system timer from tick of internal 32k RC.
 * @param   none.
 * @return  none.
 */
unsigned int pm_tim_recover_32k_rc(unsigned int now_tick_32k);

/**
 * @brief   This function serves to recover system timer from tick of external 32k crystal.
 * @param   none.
 * @return  none.
 */
unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k);
/******************************* ext_pm end ********************************************************************/


/******************************* mcu_security start ******************************************************************/
#define SECBOOT_DESC_SECTOR_NUM     1
#define SECBOOT_DESC_SIZE           0x1000  //4K for secure boot descriptor size

#define DESCRIPTOR_PUBKEY_OFFSET    0x0004

#define DESCRIPTOR_WATCHDOG_OFFSET  0x008C


#define DESC_SECTOR_DATA_LEN      144


typedef struct __attribute__((packed))  {
    unsigned char   tlnk_mark[4];
    unsigned char   public_key[64];
    unsigned char   signature[64];
    unsigned int    run_code_adr;   //4 byte
    unsigned int    run_code_size;
    unsigned char   watdog_v[4];
} sb_desc_sector_t;


typedef struct __attribute__((packed))  {
    unsigned char   fw_enc_en;
    unsigned char   secboot_en;
    unsigned short  sb_desc_adr_k; //unit: 4KB
} mcu_secure_t;
extern mcu_secure_t  mcuSecur;

bool mcu_securuty_read_efuse(void);
bool mcu_securuty_read_idcode(void);
bool efuse_get_pubkey_hash(u8* pHash);
/******************************* mcu_security end ********************************************************************/

#endif /* DRIVERS_TL323X_EXT_DRIVER_EXT_LIB_H_ */
