/********************************************************************************************************
 * @file    system_internal.h
 *
 * @brief   Internal header for the Bluetooth SDK system layer.
 *          Collects scheduler/task model definitions, LCLL state machine,
 *          access layer types and legacy compatibility macros.
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef TLKSTK_BTTPSLL_BTTPSLL_INTERNAL_H_
#define TLKSTK_BTTPSLL_BTTPSLL_INTERNAL_H_

#include "tl_common.h"
#include "common/compiler.h"
#include "common/bit.h"

#include "debug/debug_cfg.h"
#include "ble_stack.h"
#include "ble_config.h"

/******************************* ll_schedule config begin *************************************************************************/

#ifndef ABANDONED_TASK_RE_USE_EN
#define ABANDONED_TASK_RE_USE_EN                            0
#endif

#ifndef  TLK_SCH_ACCESS_CODE_TASK_DISABLE
#define  TLK_SCH_ACCESS_CODE_TASK_DISABLE                    1
#endif

/******************************* ll_schedule config end *************************************************************************/


/******************************* general use begin *************************************************************************/

#define TLKSTK_ERROR_DEBUG        BLMS_ERR_DEBUG

/* RF IRQ flags that indicate a connection event is done (cmd done / timeouts / CRC error) */
#define BLMS_FLG_RF_CONN_DONE       (FLD_RF_IRQ_CMD_DONE  | FLD_RF_IRQ_FIRST_TIMEOUT | FLD_RF_IRQ_RX_TIMEOUT | FLD_RF_IRQ_RX_CRC_2)

/******************************* general use end *************************************************************************/

/******************************* ll_schedule begin *************************************************************************/

/* todo: in master branch, can't change this, use a parameter to control;
 * just when it's cs task, set to 10, don't influence other module */
#ifndef MAX_CONFLICT_NUM
#define MAX_CONFLICT_NUM 4
#endif

/* ---- task flag (stored in sch_task_t.scheTask_flg) ----
 * attention: can not be 0, 0 has other use */
#define TSKFLG_ACL_MASTER  1
#define TSKFLG_ACL_SLAVE   2
#define TSKFLG_CIG_MST     3
#define TSKFLG_CIG_SLV     4
#define TSKFLG_LEG_ADV     5
#define TSKFLG_EXT_ADV     6
#define TSKFLG_AUX_ADV     7
#define TSKFLG_PERD_ADV    8
#define TSKFLG_PRICHN_SCAN 9  //primary channel scan
#define TSKFLG_SECCHN_SCAN 10 //secondary channel scan
#define TSKFLG_PDA_SYNC    11
#define TSKFLG_BIG_BCST    12
#define TSKFLG_BIG_SYNC    13

#define TSKFLG_PAWR_ADV    TSKFLG_PERD_ADV //PAwR-subevent_0 task
#define TSKFLG_PAWRA_SUB   14              //PAwR-subevent_n task
#define TSKFLG_PAWRA_RSP   15              //PAwR-rsp_slots task

#define TSKFLG_PAWRS_SUB   16
#define TSKFLG_PAWRS_RSP   17

#define TSKFLG_CS          18    //channel sounding
#define TSKFLG_FLASH_OP    19    //flash operation

#if (LL_RSSI_SNIFFER_MODE_ENABLE)
#define TSKFLG_SNIFM_SEEK 20 //sniffer monitor central
#define TSKFLG_SNIFS_SEEK 21 //sniffer monitor peripheral
#endif

#define TSKFLG_SCAN_ALIGN  30

#define TSKFLG_VALID_MASK  0x7F

/*******************************************************************************
    Task slot layout (task offset -> role):
    00 ~ 03 :  ACL master
    04 ~ 07 :  ACL slave
    08      :  CIG master
    09 ~ 10 :  CIG slave
    11      :  Leg ADV
    12 ~ 15 :  Ext_ADV
    16 ~ 19 :  Aux_ADV
    20 ~ 21 :  Periodic ADV
    22 ~ 23 :  PAwR ADV Subevent
    24 ~ 25 :  PAwR ADV Rsp
    26      :  Primary channel Scan (leg_scan & Ext_Scan)
    27 ~ 30 :  secondary channel Scan
    31 ~ 34 :  Periodic ADV Sync
    35 ~ 36 :  BIS Bcst
    37      :  BIS Sync
    38 ~ 39 :  PAwR Sync Subevent
    40 ~ 41 :  PAwR Sync Rsp
    42 ~ 43 :  Channel Sounding
 ******************************************************************************/

/* ---- max task number per role ---- */
#define TSKNUM_ACL_MASTER  LL_MAX_ACL_CEN_NUM
#define TSKNUM_ACL_SLAVE   LL_MAX_ACL_PER_NUM

#define TSKNUM_CIG_MST     1
#define TSKNUM_CIG_SLV     2

#define TSKNUM_LEG_ADV     1
#define TSKNUM_EXT_ADV     4
#define TSKNUM_AUX_ADV     4  //-> 20

#define TSKNUM_PERD_ADV    2
#define TSKNUM_PAWRA_SUB   2 //for PAwR-Advertiser
#define TSKNUM_PAWRA_RSP   2 //for PAwR-Advertiser
#define TSKNUM_PRICHN_SCAN 1

#if ESL_RAM_OPTIMIZATION
#define TSKNUM_SECCHN_SCAN 1 //for multiple set LL/DDI/SCN/BV-25-C
#define TSKNUM_PDA_SYNC    1 //PERIODIC_ADV_SYNC
#else
#define TSKNUM_SECCHN_SCAN 4 //for multiple set LL/DDI/SCN/BV-25-C
#define TSKNUM_PDA_SYNC    2 //PERIODIC_ADV_SYNC
#endif

#define TSKNUM_BIG_BCST    2 //->35
#define TSKNUM_BIG_SYNC    1

#if ESL_RAM_OPTIMIZATION
#define TSKNUM_PAWRS_SUB   1 //must be same as TSKNUM_PDA_SYNC
#define TSKNUM_PAWRS_RSP   1
#else
#define TSKNUM_PAWRS_SUB   2 //must be same as TSKNUM_PDA_SYNC
#define TSKNUM_PAWRS_RSP   4 //must be same as TSKNUM_SECCHN_SCAN
#endif

#define TSKNUM_CS          2 //44
#define TSKNUM_FLASH_OP    1

/* ---- scheduler timing margins (us) ---- */
#define SCH_NEW_TASK_MARGIN_US                 (10)
#define POST_FLASH_CODE_INCREASE_TIME_US       100
#define SCH_ACL_IRQ_POST_PROCESS_MARGIN_US     50

/* ---- feature switches ---- */

#ifdef BLC_ZEPHYR_BLE_INTEGRATION
#if CONFIG_IEEE802154_TLX_BLE_COEXIST
#define THREAD_LL_FUNCTION_ENABLE              1
#endif
#endif

#ifndef THREAD_LL_FUNCTION_ENABLE
#define THREAD_LL_FUNCTION_ENABLE              0
#endif

#ifndef SCHEDULER_SIMULATE_FIXED_TASK_ENABLE
#define SCHEDULER_SIMULATE_FIXED_TASK_ENABLE   0
#endif

#ifndef SCHEDULER_SIMULATE_FLEXIBLE_TASK_ENABLE
#define SCHEDULER_SIMULATE_FLEXIBLE_TASK_ENABLE 0
#endif

#ifndef SCHEDULER_SIMULATE_INSERT_TASK_ENABLE
#define SCHEDULER_SIMULATE_INSERT_TASK_ENABLE  0
#endif

/* SIMULATE, for scheduler study & research */
#define TSKNUM_SIMU_FIXED_TASK    (SCHEDULER_SIMULATE_FIXED_TASK_ENABLE ? 2:0)
#define TSKNUM_SIMU_FLEXIBLE_TASK (SCHEDULER_SIMULATE_FLEXIBLE_TASK_ENABLE ? 1:0) //set 1 forever
#define TSKNUM_SIMU_INSERT_TASK   (SCHEDULER_SIMULATE_INSERT_TASK_ENABLE ? 1:0)

#define TSKNUM_THREAD_FLEXIBLE_TASK   (THREAD_LL_FUNCTION_ENABLE ? 1:0)
#define TSKNUM_THREAD_INSERT_TASK     (THREAD_LL_FUNCTION_ENABLE ? 1:0)

#define TSKNUM_SNIFM_SEEK (LL_RSSI_SNIFFER_MASTER_ENABLE ? TSKNUM_ACL_MASTER : 0)
#define TSKNUM_SNIFS_SEEK (LL_RSSI_SNIFFER_SLAVE_ENABLE ? TSKNUM_ACL_SLAVE : 0)

/* total task number (used to size callback table & priority arrays) */
#define TSKNUM_MAX        (TSKNUM_ACL_MASTER + TSKNUM_ACL_SLAVE + TSKNUM_CIG_MST +                                \
                    TSKNUM_CIG_SLV + TSKNUM_LEG_ADV + TSKNUM_EXT_ADV + TSKNUM_AUX_ADV +                           \
                    TSKNUM_PERD_ADV + TSKNUM_PAWRA_SUB + TSKNUM_PAWRA_RSP +                                       \
                    TSKNUM_PRICHN_SCAN + TSKNUM_SECCHN_SCAN + TSKNUM_PDA_SYNC +                                   \
                    TSKNUM_BIG_BCST + TSKNUM_BIG_SYNC + TSKNUM_PAWRS_SUB + TSKNUM_PAWRS_RSP + TSKNUM_CS +         \
                    TSKNUM_FLASH_OP + TSKNUM_SNIFM_SEEK + TSKNUM_SNIFS_SEEK + \
                    TSKNUM_SIMU_FIXED_TASK + TSKNUM_SIMU_FLEXIBLE_TASK +TSKNUM_SIMU_INSERT_TASK + \
                    TSKNUM_THREAD_FLEXIBLE_TASK + TSKNUM_THREAD_INSERT_TASK)

/*******************************  ll_schedule end *************************************************************************/


/******************************* system begin *************************************************************************/

/* ---- LL task callback typedefs ---- */
typedef int (*ll_task_callback_t)(int);
typedef int (*ll_task_callback_2_t)(int, void*p);
typedef int (*ll_task_callback_3_t)(int, void*p0, void*p1);

typedef int (*ll_prd_sync_pawr_sync_common_t)(sync_info_t* p0, unsigned char* p1);

/* IRQ task callback table, indexed by task offset (TSKOFT_*) */
extern ll_task_callback_2_t  lcll_irq_task_cb_tbl[TSKNUM_MAX];

/* ---- extern callback declarations (IRQ / main-loop entries per role) ---- */
extern ll_task_callback_t    ll_acl_conn_irq_task_cb;
extern ll_task_callback_2_t  ll_acl_conn_mlp_task_cb;

extern ll_task_callback_2_t  ll_acl_slave_irq_task_cb;
extern ll_task_callback_2_t  ll_acl_master_irq_task_cb;

extern ll_task_callback_2_t  ll_cis_conn_irq_task_cb;
extern ll_task_callback_2_t  ll_cis_conn_mlp_task_cb;

extern ll_task_callback_2_t  ll_cig_mst_irq_task_cb;
extern ll_task_callback_2_t  ll_cig_mst_mlp_task_cb;

extern ll_task_callback_2_t  ll_cis_slv_irq_task_cb;
extern ll_task_callback_t    ll_cis_slv_mlp_task_cb;

extern ll_task_callback_2_t  ll_leg_adv_irq_task_cb;
extern ll_task_callback_t    ll_leg_adv_mlp_task_cb;
extern ll_task_callback_2_t  ll_ext_adv_irq_task_cb;
extern ll_task_callback_2_t  ll_ext_adv_mlp_task_cb;

extern ll_task_callback_t    ll_prichn_scan_irq_task_cb;

extern ll_task_callback_t    ll_leg_scan_mlp_task_cb;
extern ll_task_callback_2_t  ll_ext_scan_irq_task_cb;
extern ll_task_callback_t    ll_ext_scan_mlp_task_cb;

extern ll_task_callback_t    ll_init_mlp_task_cb;
extern ll_task_callback_t    ll_ext_init_irq_task_cb;

extern ll_task_callback_2_t  ll_prd_adv_irq_task_cb;
extern ll_task_callback_2_t  ll_prd_adv_mlp_task_cb;
extern ll_task_callback_2_t  ll_pawra_sub_irq_task_cb;
extern ll_task_callback_2_t  ll_pawra_rsp_irq_task_cb;
extern ll_task_callback_2_t  ll_pawra_mlp_task_cb;
extern ll_task_callback_2_t  ll_pda_sync_irq_task_cb;
extern ll_task_callback_t    ll_pda_sync_mlp_task_cb;

extern ll_task_callback_3_t  ll_pawr_sync_sub_irq_task_cb;
extern ll_task_callback_t    ll_pawr_sync_mlp_task_cb;
extern ll_task_callback_t    ll_pawr_sync_rspTx_irq_task_cb;

extern ll_prd_sync_pawr_sync_common_t ll_pda_sync_pawr_sync_common_cb;

extern ll_task_callback_2_t  ll_big_bcst_irq_task_cb;
extern ll_task_callback_2_t  ll_big_bcst_mlp_task_cb;

extern ll_task_callback_2_t  ll_big_sync_irq_task_cb;
extern ll_task_callback_2_t  ll_big_sync_mlp_task_cb;

extern ll_task_callback_t    ll_secchn_scan_task_cb;

extern ll_task_callback_t    ll_aoa_aod_mlp_task_cb;

extern ll_task_callback_2_t  ll_cis_map_update_cb;
extern ll_task_callback_2_t  ll_feat_page_mlp_task_cb;

/* ---- system parameters ---- */
_attribute_aligned_(4)
typedef struct __attribute__((packed)) {
    u8  stimer_irq_manual_trigger;
    u8  delay_clear_rf_status;
    u8  u8_rsvd1[1];
    u8  rf_fsm_busy;
    u32 dly_start_tick_clr_rf_sts;
} sys_param_t;

extern _attribute_aligned_(4) sys_param_t g_sysParam;

void tlkstk_system_timer_irq_handler(void);

void tlk_sys_config_delay_clear_fsm(u8 delay_clear_rf_status,u8 dly_start_tick_clr_rf_sts);

void tlk_sch_init(void);
/******************************* system end *************************************************************************/


/******************************* scheduler begin *************************************************************************/

/* SiHui: special design, keep it enable all time, to detect all kinds of scheduler relative bug */
#ifndef SCHEDULER_DEBUG_EN
#define SCHEDULER_DEBUG_EN                                    1
#endif

/* ctx/crx_start preset ctx/crx_post generation time point (system timer capture value preset),
 * if an end RF signal such as cmd_done is received, it ends early, and the difference between
 * the early end time point and the preset time point cannot be too large. Once the execution
 * of the task at the end of the premature exceeds the preset value, the system timer status is
 * set, and the timer capture value is set after the next task is obtained, which immediately
 * causes an interrupt. */

/* 20230720 by SiHui/QiuWei/HaoJie
 * for stuck code FF0A, SiHui thought if error US is very small(e.g. 10uS), set a post system tick for capture value,
 * no need stuck, STimer IRQ will trigger, next task will occur, and all task can endure some time delay margin.
 * But QiuWei propose that the STimer of a post capture value may not trigger IRQ, then it will wait for whole timer round,
 * e.g. B92, 24M clock, about 3 minute.
 * (1). HaoJie have a test environment, can trigger FF0A in about 10 minute. We add some debug, find that set post capture value
 * (FF0A happen, but do not stuck code) make about 3 minute waiting time for scheduler.  It seems that QiuWei is right.
 * (2). refer to SiHui's mail "System Timer/System Timer IRQ Capture" in 20191014. We know that for Kite, a post capture
 * value in a STimer IRQ handler can not trigger new IRQ quickly. QiuWei tested that Eagle is same situation. We can solve this
 * problem by using same method described in email. Actually, all B85m/B91m and later IC need consider this special problem.
 * (3) For HaoJie's environment, do a longer time test after code fix this problem. No problem in about one hour, we think that
 * this code fixing is valid. */
#ifndef FIX_STIMER_SET_CAPTURE_ERR
#define FIX_STIMER_SET_CAPTURE_ERR                                1
#endif

#define LBIT(n)                          ( 1<<(n) )

/* ---- task offset (index into callback table / priority array) ----
 * sniffer and simulate task not used at same time */
#define TSKOFT_ACL_CONN                                 0
#define TSKOFT_ACL_MASTER                               0                                               //00~03
#define TSKOFT_ACL_SLAVE                                ( TSKNUM_ACL_MASTER )                           //04~07
#define TSKOFT_CIG_MST                                  ( TSKOFT_ACL_SLAVE      + TSKNUM_ACL_SLAVE )    //08~08
#define TSKOFT_CIG_SLV                                  ( TSKOFT_CIG_MST        + TSKNUM_CIG_MST )      //09~10
#define TSKOFT_LEG_ADV                                  ( TSKOFT_CIG_SLV        + TSKNUM_CIG_SLV )      //11
#define TSKOFT_EXT_ADV                                  ( TSKOFT_LEG_ADV        + TSKNUM_LEG_ADV )      //12~15
#define TSKOFT_AUX_ADV                                  ( TSKOFT_EXT_ADV        + TSKNUM_EXT_ADV )      //16~19

#define TSKOFT_PERD_ADV                                 ( TSKOFT_AUX_ADV        + TSKNUM_AUX_ADV )      //20~21
#define TSKOFT_PAWRA_SUB                                ( TSKOFT_PERD_ADV       + TSKNUM_PERD_ADV )     //22~23
#define TSKOFT_PAWRA_RSP                                ( TSKOFT_PAWRA_SUB      + TSKNUM_PAWRA_SUB )    //24~25

#define TSKOFT_PRICHN_SCAN                              ( TSKOFT_PAWRA_RSP      + TSKNUM_PAWRA_RSP )    //26
#define TSKOFT_SECCHN_SCAN                              ( TSKOFT_PRICHN_SCAN    + TSKNUM_PRICHN_SCAN )  //27~30
#define TSKOFT_PDA_SYNC                                 ( TSKOFT_SECCHN_SCAN    + TSKNUM_SECCHN_SCAN )  //31~32
#define TSKOFT_BIG_BCST                                 ( TSKOFT_PDA_SYNC       + TSKNUM_PDA_SYNC )     //33~34
#define TSKOFT_BIG_SYNC                                 ( TSKOFT_BIG_BCST       + TSKNUM_BIG_BCST )     //35-35
#define TSKOFT_PAWRS_SUB                                ( TSKOFT_BIG_SYNC       + TSKNUM_BIG_SYNC )     //36~37
#define TSKOFT_PAWRS_RSP                                ( TSKOFT_PAWRS_SUB      + TSKNUM_PAWRS_SUB)     //38~41

#define TSKOFT_CS                                       ( TSKOFT_PAWRS_RSP      + TSKNUM_PAWRS_RSP)     //42~43
#define TSKOFT_FLASH_OP                                 ( TSKOFT_CS             + TSKNUM_CS )           //44
#define TSKOFT_SNIFM_SEEK                               ( TSKOFT_FLASH_OP       + TSKNUM_FLASH_OP )     //45~48
#define TSKOFT_SNIFS_SEEK                               ( TSKOFT_SNIFM_SEEK     + TSKNUM_SNIFM_SEEK )   //49~52
/* task offset for simulate task */
#define TSKOFT_SIMU_FIXED_TASK                          (TSKOFT_SNIFS_SEEK      + TSKNUM_SNIFS_SEEK)    //53~54
#define TSKOFT_SIMU_FLEXIBLE_TASK                       (TSKOFT_SIMU_FIXED_TASK + TSKNUM_SIMU_FIXED_TASK) //55
#define TSKOFT_SIMU_INSERT_TASK                         (TSKOFT_SIMU_FLEXIBLE_TASK + TSKNUM_SIMU_FLEXIBLE_TASK) //56

/* task offset for thread task */
#define TSKOFT_THREAD_FLEXIABLE_TASK                    (TSKOFT_SIMU_INSERT_TASK + TSKNUM_SIMU_INSERT_TASK )  //57
#define TSKOFT_THREAD_INSERT_TASK                       (TSKOFT_THREAD_FLEXIABLE_TASK + TSKNUM_THREAD_FLEXIBLE_TASK )  //58

/* ---- single-task mask (one bit per task offset) ---- */
#define TSKMSK_ACL_CONN_0                               BIT(0)
#define TSKMSK_ACL_MASTER_0                             BIT(0)
#define TSKMSK_ACL_SLAVE_0                              BIT(LL_MAX_ACL_CEN_NUM)
#define TSKMSK_CIG_MASTER_0                             BIT(TSKOFT_CIG_MST)
#define TSKMSK_CIG_SLAVE_0                              BIT(TSKOFT_CIG_SLV)
#define TSKMSK_EXT_ADV_0                                BIT(TSKOFT_EXT_ADV)
#define TSKMSK_AUX_ADV_0                                BIT(TSKOFT_AUX_ADV)
#define TSKMSK_PERD_ADV_0                               BIT(TSKOFT_PERD_ADV)
#define TSKMSK_PAWRA_SUB_0                              BIT(TSKOFT_PAWRA_SUB) //pawr
#define TSKMSK_PAWRA_RSP_0                              BIT(TSKOFT_PAWRA_RSP) //pawr
#define TSKMSK_SECCHN_SCAN_0                            BIT(TSKOFT_SECCHN_SCAN)
#define TSKMSK_PDA_SYNC_0                               BIT(TSKOFT_PDA_SYNC)
#define TSKMSK_BIG_BCST_0                               BIT(TSKOFT_BIG_BCST)
#define TSKMSK_BIG_SYNC_0                               BIT(TSKOFT_BIG_SYNC)
#define TSKMSK_PAWRS_SUB_0                              BIT(TSKOFT_PAWRS_SUB) //pawr sync
#define TSKMSK_PAWRS_RSP_0                              BIT(TSKOFT_PAWRS_RSP)
#define TSKMSK_CS_0                                     BIT(TSKOFT_CS)//channel sounding
#define TSKMSK_FLASH_OP_0                               BIT(TSKOFT_FLASH_OP)
#define TSKMSK_SNIFM_SEEK_0                             BIT(TSKOFT_SNIFM_SEEK)
#define TSKMSK_SNIFS_SEEK_0                             BIT(TSKOFT_SNIFS_SEEK)
/* task mask for simulate task */
#define TSKMSK_SIMU_FIXED_TASK_0                        BIT(TSKOFT_SIMU_FIXED_TASK)
#define TSKMSK_SIMU_FLEXIBLE_TASK                       BIT(TSKOFT_SIMU_FLEXIBLE_TASK)
#define TSKMSK_SIMU_INSERT_TASK                         BIT(TSKOFT_SIMU_INSERT_TASK)
/* task mask for thread task */
#define TSKMSK_THREAD_FLEXIBLE_TASK_0                   BIT(TSKOFT_THREAD_FLEXIABLE_TASK)
#define TSKMSK_THREAD_INSERT_TASK_0                     BIT(TSKOFT_THREAD_INSERT_TASK)

/* ---- group task mask (range covering all instances of a role) ---- */
#define TSKMSK_ACL_CONN_ALL                             ((1<<LL_MAX_ACL_CONN_NUM) - 1)
#define TSKMSK_ACL_MASTER_ALL                           ((1<<LL_MAX_ACL_CEN_NUM) - 1)
#define TSKMSK_ACL_SLAVE_ALL                            BIT_RNG(TSKOFT_ACL_SLAVE,       TSKOFT_CIG_MST - 1)
#define TSKMSK_CIG_MASTER_ALL                           BIT_RNG(TSKOFT_CIG_MST,         TSKOFT_CIG_SLV - 1)
#define TSKMSK_CIG_SLAVE_ALL                            BIT_RNG(TSKOFT_CIG_SLV,         TSKOFT_LEG_ADV - 1)
#define TSKMSK_LEG_ADV                                  BIT(TSKOFT_LEG_ADV)  //task number is 1 forever
#define TSKMSK_EXT_ADV_ALL                              BIT_RNG(TSKOFT_EXT_ADV,         TSKOFT_AUX_ADV - 1)    //0x00 00 f0 00
#define TSKMSK_AUX_ADV_ALL                              BIT_RNG(TSKOFT_AUX_ADV,         TSKOFT_PERD_ADV - 1)   //0x00 0f 00 00
#define TSKMSK_PERD_ADV_ALL                             BIT_RNG(TSKOFT_PERD_ADV,        TSKOFT_PAWRA_SUB - 1)  //0x00 30 00 00
#define TSKMSK_PAWRA_SUB_ALL                            BIT_RNG(TSKOFT_PAWRA_SUB,       TSKOFT_PAWRA_RSP - 1)
#define TSKMSK_PAWRA_RSP_ALL                            BIT_RNG(TSKOFT_PAWRA_RSP,       TSKOFT_PRICHN_SCAN - 1)
#define TSKMSK_PRICHN_SCAN                              BIT(TSKOFT_PRICHN_SCAN) //task number is 1 forever       00 00 00 40
#define TSKMSK_SECCHN_SCAN_ALL                          BIT_RNG(TSKOFT_SECCHN_SCAN,     TSKOFT_PDA_SYNC - 1) //00 00 00 78
#define TSKMSK_PDA_SYNC_ALL                             BIT_RNG(TSKOFT_PDA_SYNC,        TSKOFT_BIG_BCST - 1)
#define TSKMSK_BIG_BCST_ALL                             BIT_RNG(TSKOFT_BIG_BCST,        TSKOFT_BIG_SYNC - 1)
#define TSKMSK_BIG_SYNC_ALL                             BIT_RNG(TSKOFT_BIG_SYNC,        TSKOFT_PAWRS_SUB - 1)
#define TSKMSK_PAWRS_SUB_ALL                            BIT_RNG(TSKOFT_PAWRS_SUB,       TSKOFT_PAWRS_RSP - 1)
#define TSKMSK_PAWRS_RSP_ALL                            BIT_RNG(TSKOFT_PAWRS_RSP,       TSKOFT_CS - 1)
#define TSKMSK_CS_ALL                                   BIT_RNG(TSKOFT_CS,              TSKOFT_FLASH_OP - 1)
#define TSKMSK_FLASH_OP_ALL                             BIT_RNG(TSKOFT_FLASH_OP,        TSKOFT_SNIFM_SEEK-1)
#define TSKMSK_SNIFM_SEEK_ALL                           BIT_RNG(TSKOFT_SNIFM_SEEK,      TSKOFT_SNIFS_SEEK - 1)
#define TSKMSK_SNIFS_SEEK_ALL                           BIT_RNG(TSKOFT_SNIFS_SEEK,      TSKOFT_SIMU_FIXED_TASK - 1)
/* task mask for simulate task */
#define TSKMSK_SIMU_FIXED_TASK_ALL                      BIT_RNG(TSKOFT_SIMU_FIXED_TASK, TSKOFT_SIMU_FLEXIBLE_TASK - 1)
#define TSKMSK_SIMU_FLEXIBLE_TASK_ALL                   BIT_RNG(TSKOFT_SIMU_FLEXIBLE_TASK, TSKOFT_SIMU_INSERT_TASK - 1)
#define TSKMSK_SIMU_INSERT_TASK_ALL                     BIT(TSKOFT_SIMU_INSERT_TASK)  // more insert task need to handle co-exist strategy question
/* task mask for thread task */
#define TSKMSK_THREAD_FLEXIBLE_TASK_ALL                 BIT(TSKOFT_THREAD_FLEXIABLE_TASK)
#define TSKMSK_THREAD_INSERT_TASK_ALL                   BIT(TSKOFT_THREAD_INSERT_TASK)


/************************************* LCLL state  **************************************************
 * LC:  Link Control
 * LL:  Link Layer & Low Latency
 * _S:  Start
 * _E:  End (post) */

/* general */
#define LCLL_STATE_NONE                                    0
#define LCLL_STATE_SCHE_START                              LBIT(0)
#define LCLL_STATE_SCHE_INSERT                             LBIT(1)

/* LE */
#define LCLL_STATE_LEGADV                                  BIT(2)
#define LCLL_STATE_EXTADV_S                                BIT(3)
#define LCLL_STATE_EXTADV_E                                BIT(4)
#define LCLL_STATE_PRICHN_SCAN_S                           BIT(5)    //primary channel scan start
#define LCLL_STATE_PRICHN_SCAN_E                           BIT(6)    //primary channel scan end
#define LCLL_STATE_SECCHN_SCAN_S                           BIT(7)    //secondary channel scan start
#define LCLL_STATE_SECCHN_SCAN_E                           BIT(8)    //secondary channel scan end
#define LCLL_STATE_PDA_SYNC_S                              BIT(9)    //periodic ADV sync start
#define LCLL_STATE_PDA_SYNC_E                              BIT(10)   //periodic ADV sync end
#define LCLL_STATE_BTX_S                                   BIT(11)
#define LCLL_STATE_BTX_E                                   BIT(12)
#define LCLL_STATE_BRX_S                                   BIT(13)
#define LCLL_STATE_BRX_E                                   BIT(14)

#define LCLL_STATE_CIG_E                                   BIT(15)
#define LCLL_STATE_CTX_S                                   BIT(16)   //CIS BTX Start
#define LCLL_STATE_CTX_E                                   BIT(17)   //CIS BTX End
#define LCLL_STATE_CRX_S                                   BIT(18)   //CIS BRX Start
#define LCLL_STATE_CRX_E                                   BIT(19)   //CIS BRX End

#define LCLL_STATE_BIG_E                                   BIT(20)
#define LCLL_STATE_BBCST_S                                 BIT(21)   //BIS BCST Start
#define LCLL_STATE_BBCST_E                                 BIT(22)   //BIS BCST End
#define LCLL_STATE_BSYNC_S                                 BIT(23)   //BIS SYNC Start
#define LCLL_STATE_BSYNC_E                                 BIT(24)   //BIS SYNC End

#define LCLL_STATE_PAWRA_SLOT_GRP_E                        BIT(25)   //PAwR_A Rsp_Slots Group End
#define LCLL_STATE_PAWRA_SLOT_S                            BIT(26)   //PAwR_A Slot Start
#define LCLL_STATE_PAWRA_SLOT_E                            BIT(27)   //PAwR_A Slot End

#define LCLL_STATE_PAWRS_SUB_S                             BIT(28)
#define LCLL_STATE_PAWRS_SUB_E                             BIT(29)

#define LCLL_STATE_CS_INIT_TX_S                            BIT(30)
#define LCLL_STATE_CS_INIT_TX_E                            BIT(31)
#define LCLL_STATE_CS_INIT_RX_S                            BIT(32)
#define LCLL_STATE_CS_INIT_E                               BIT(33)

#define LCLL_STATE_CS_REFL_STEP_TX_S                       BIT(34)   //TODO yuexin 2024/07/08
#define LCLL_STATE_CS_REFL_STEP_S                          BIT(35)
#define LCLL_STATE_CS_REFL_STEP_E                          BIT(36)
#define LCLL_STATE_CS_SUBEVENT_E                           BIT(37)

/* BIT(39)~BIT(46) is used for sniffer */
#define LCLL_STATE_SNIFS_SEEK_S                            BIT(39)   //ACL Sniffer Slave Seek Start
#define LCLL_STATE_SNIFS_SEEK_E                            BIT(40)   //ACL Sniffer Slave Seek End
#define LCLL_STATE_SNIFS_S                                 BIT(41)   //ACL Sniffer Slave Sync Start
#define LCLL_STATE_SNIFS_E                                 BIT(42)   //ACL Sniffer Slave Sync End

#define LCLL_STATE_SNIFM_SEEK_S                            BIT(43)   //ACL Sniffer Master Seek Start
#define LCLL_STATE_SNIFM_SEEK_E                            BIT(44)   //ACL Sniffer Master Seek End
#define LCLL_STATE_SNIFM_S                                 BIT(45)   //ACL Sniffer Master Sync Start
#define LCLL_STATE_SNIFM_E                                 BIT(46)   //ACL Sniffer Master Sync End

/* BIT(47)~BIT(52) is used for simulate */
#define LCLL_STATE_SIMU_FIXED_S                            BIT(47)
#define LCLL_STATE_SIMU_FIXED_E                            BIT(48)
#define LCLL_STATE_SIMU_FLEXIBLE_S                         BIT(49)
#define LCLL_STATE_SIMU_FLEXIBLE_E                         BIT(50)
#define LCLL_STATE_SIMU_INSERT_S                           BIT(51)
#define LCLL_STATE_SIMU_INSERT_E                           BIT(52)

#define LCLL_STATE_FLASH_OP_S                              BIT(53)
#define LCLL_STATE_FLASH_OP_E                              BIT(54)

/* thread task */
#define LCLL_STATE_THREAD_FLEXIBLE_S                       BIT(55)
#define LCLL_STATE_THREAD_FLEXIBLE_E                       BIT(56)
#define LCLL_STATE_THREAD_INSERT_S                         BIT(57)
#define LCLL_STATE_THREAD_INSERT_E                         BIT(58)

/* ---- scheduler update trigger masks (grouped by category) ---- */
//LE
#define LCLL_STATE_UPDATE_SCHEDULER_LE                     (LCLL_STATE_LEGADV | LCLL_STATE_EXTADV_E |  \
                                                        LCLL_STATE_PRICHN_SCAN_E | LCLL_STATE_SECCHN_SCAN_E | LCLL_STATE_PDA_SYNC_E | \
                                                        LCLL_STATE_BTX_E | LCLL_STATE_BRX_E | LCLL_STATE_PAWRS_SUB_E | BLMS_STATE_CS_SUBEVENT_E | BLMS_STATE_PAWRA_SLOT_GRP_E)
//Sniff
#define LCLL_STATE_UPDATE_SCHEDULER_SNIFF                  (LCLL_STATE_SNIFS_E | LCLL_STATE_SNIFS_SEEK_E | LCLL_STATE_SNIFM_E | LCLL_STATE_SNIFM_SEEK_E)
//Simulate
#define LCLL_STATE_UPDATE_SCHEDULER_SIMU                   (LCLL_STATE_SIMU_FIXED_E | LCLL_STATE_SIMU_FLEXIBLE_E | LCLL_STATE_SIMU_INSERT_E)
//thread
#define LCLL_STATE_UPDATE_SCHEDULER_THREAD                 (LCLL_STATE_THREAD_FLEXIBLE_E | LCLL_STATE_THREAD_INSERT_E)
//Total
#define LCLL_STATE_UPDATE_SCHEDULER_TOTAL                  (g_lcll_state & (LCLL_STATE_SCHE_START | LCLL_STATE_SCHE_INSERT | LCLL_STATE_UPDATE_SCHEDULER_LE | \
                                                                        LCLL_STATE_UPDATE_SCHEDULER_SNIFF | LCLL_STATE_UPDATE_SCHEDULER_SIMU | LCLL_STATE_FLASH_OP_E | LCLL_STATE_UPDATE_SCHEDULER_THREAD))

#define LCLL_STATE_TASK_START                              (g_lcll_state & (BLMS_STATE_BTX_S | BLMS_STATE_BRX_S | BLMS_STATE_PDA_SYNC_S | BLMS_STATE_EXTADV_S |\
                                                                         BLMS_STATE_SECCHN_SCAN_S | BLMS_STATE_PAWRS_SUB_S))

/* ---- system timer IRQ trigger types ---- */
#define SYS_IRQ_TRIG_DONE                                  0
#define SYS_IRQ_TRIG_NEW_TASK                              1
#define SYS_IRQ_TRIG_SCHE_START                            2
#define SYS_IRQ_TRIG_SCHE_INSERT                           3
#define SYS_IRQ_TRIG_SCHE_CONTINUE                         4

//for pawr adv rsp task to use
#define SYS_IRQ_TRIG_PAWRA_SLOT_START                      6
#define SYS_IRQ_TRIG_PAWRA_SLOT_POST                       7

#define SYS_IRQ_TRIG_CS_INIT_TX_START                      (27)
#define SYS_IRQ_TRIG_CS_INIT_SRX                           (28)
#define SYS_IRQ_TRIG_CS_STEP_POST                          (29)
#define SYS_IRQ_TRIG_CS_REFL_RX_START                      (30)
#define SYS_IRQ_TRIG_CS_REFL_TX_START                      (31)
#define SYS_IRQ_TRIG_CS_REFL_TX_POST                       (32)

/* actually no need to distinguish */
#define STATE_CHANGE_GENERAL                               BIT(0)

/* ---- IRQ flag enum (passed to task callbacks) ---- */
typedef enum {
    FLAG_SCHEDULE_START         = BIT(30),
    FLAG_SCHEDULE_DONE          = BIT(29),
    FLAG_SCHEDULE_BUILD         = BIT(28),
    FLAG_SCHEDULE_INSERT        = BIT(27),

    FLAG_IRQ_RX                 = BIT(26),
    FLAG_IRQ_TX                 = BIT(25),

    FLAG_INSERT_SCHTSK_CONFLICT = BIT(24),
    FLAG_SCHEDULE_MIANLOOP      = BIT(23),
    FLAG_SCHEDULE_CONTINUE      = BIT(22),
    FLAG_IRQ_DONE               = BIT(21),

    FLAG_SCHEDULE_TASK_IDX_MASK = 0x0F,  //0~15
} flag_irq_e;

/* ---- main-loop flag enum ---- */
typedef enum {
    FLAG_CHECK_INIT             = BIT(30),
    FLAG_MODULE_MAINLOOP        = BIT(29),
    FLAG_MODULE_RESET           = BIT(28),   //HCI reset
    FLAG_MODULE_SET_HOST_CHM    = BIT(27),
    FLAG_ACL_CONN_EXIT_CHECK    = BIT(26),
} flag_mlp_e;

/* aliases for sch_task_t begin/end fields */
#define tick_s_begin    begin
#define tick_s_end      end

/* ---- scheduler task node (element of the scheduler linked list) ---- */
_attribute_aligned_(4)
typedef struct sch_task {
    u32  begin;             //stimer tick begin, right align  //tick_s_begin
    u32  end;               //stimer tick begin, left align   //tick_s_end

    u8    scheTask_oft;     //task offset (index into callback table)
    u8    scheTask_flg;     //task type (TSKFLG_*)
    u8    debug_count;
    u8    priority;         //task priority (task_pri_t)
    const void* sch;

    struct sch_task *next;
} sch_task_t; //scheduler task

/* ---- flash operation scheduler task (wraps sch_task_t with op queue indices) ---- */
_attribute_aligned_(4) typedef struct __attribute__((packed)) flash_op_sch_task_t {
    sch_task_t task;
    u8  op_queue_start;
    u8  op_queue_end;
    u16 rsvd16;
} flash_op_sch_task_t;

extern sch_task_t g_sch_header;

/* ---- scheduler manager (global runtime state) ---- */
_attribute_aligned_(4)
typedef struct __attribute__((packed)) {
    u8  state_chng;             //state change
    u8  sche_run_flag;
    u8  u8_rsvd0;
    u8  sequence;

    u8  sche_process_en;
    u8  lklt_taskNum;
    u8  lastTsk_past;
    u8  cal_time_en;            //not used now

    u8  immediate_task;         //not used now
    u8  build_index;
    u8  cur_task_oft;           //mark current running task offset
    u8  sonosFlash_extTskDur_buildFlg;

    u32 sonosFlash_extTskDur_us;

    u32 update;
    u64 task_mask;              //task matrix for scheduler running status
    u64 task_ui_en;             //task matrix for some user enable or disable task

    u32 tick_irq_task_start;
    u32 tick_mark_real_irqTaskStart; //mark the latest real happened "tick_irq_task_start"

    u32 start_tick_preAllocWin;     //start tick of next pre-allocate window area
    u32 sche_endTick_dft;           //default scheduler end tick for a new round rebuilding scheduler
    u32 sche_endTick_maxPri;        //mark the end tick for the task with the highest priority in rebuilding scheduler process

    u32 system_irq_tick;
    u32 lastTsk_endTick;

    u32 bSlot_idx_next;

    u32 bSlot_ref_tick;
    u32 bSlot_ref_idx;

    sch_task_t *pTask_head;
    sch_task_t *pTask_next;
    sch_task_t *pTask_cur;
//  sch_task_t *pTask_pre;  //now not used

    u32 tick_sch_build_begin;
    u32 sche_process_us;
#if (DBG_SCHE_CAL_TIME_EN)
    u32 sche_max_us;
#endif

#if (ABANDONED_TASK_RE_USE_EN)
    sch_task_t *pTask_abandon_head;
#endif
} sch_mng_t; //scheduler manage

extern sch_mng_t g_scheMng;


#define ALLOCATION_WINDOW_BASE_US_MIN       7500    //7.5ms minimum
#define ALLOCATION_WINDOW_BASE_US_MAX       BIT(30) //~1073s ceiling (room for future second-level expansion)
#define ALLOCATION_WINDOW_BASE_US_DEFAULT   75000   //75ms default

/* ---- scheduler configuration (application-configurable parameters) ---- */
_attribute_aligned_(4)
typedef struct __attribute__((packed)) {
    u32 allocation_window_base_us;   //base allocation window duration in us, default 75000 (75ms)
    u32 reserved[3];                 //reserved for future extensions
} sch_cfg_t;


/**
 * @brief Initialize scheduler configuration to default values.
 */
void tlk_sch_config_init(void);

/**
 * @brief Set the base allocation window duration (us) for scheduler task building.
 * @param base_us - base window in microseconds, range [7500 ~ bit(30)]
 */
void tlk_sch_config_set_allocation_window_base(u32 base_us);

/**
 * @brief Enable a task by task mask in the scheduler.
 *        Wraps tlk_sch_addUiEnable, triggers schedule insert IRQ, and marks state change.
 *        The task MUST already be registered in the scheduler.
 * @param tskmsk     - task mask to enable (must be non-zero)
 * @param fast_start - 1: attempt fast schedule insertion (with safety checks); 0: update state only, no IRQ trigger
 */
void tlk_sch_task_enable(u64 tskmsk, u8 fast_start);

/**
 * @brief Disable a task by task mask in the scheduler.
 *        Wraps tlk_sch_removeUiEnable, triggers schedule insert IRQ, and marks state change.
 * @param tskmsk     - task mask to disable (must be non-zero)
 * @param fast_start - 1: attempt fast schedule insertion (with safety checks); 0: update state only, no IRQ trigger
 */
void tlk_sch_task_disable(u64 tskmsk, u8 fast_start);

/**
 * @brief Reset scheduler runtime state to default values.
 *        Restores g_scheMng, bltPri, bltFutTask, g_sch_header, and debug counters
 *        to their power-on defaults. Disables system timer IRQ.
 *        NOTE: g_sch_cfg is NOT reset here — it is application-configured at power-on
 *        and should persist across scheduler resets.
 */
void tlk_sch_stop_scheduler(void);

/**
 * @brief Get scheduler task switch time (us) for FSU T_MCES lower bound.
 *        Returns g_scheMng.sche_process_us (sliding average of scheduler build time).
 *        Caller should apply margin (e.g. *2) when using as MCES lower bound.
 */
u32 tlk_sch_get_mces_time_us(void);



/* change:
 * bltSche.sSlot_tick_irq          => g_scheMng.tick_irq_task_start
 * bltSche.sSlot_idx_irq_real      => g_scheMng.tick_mark_real_irqTaskStart
 * bltSche.bSlot_tick_irq_real     => g_scheMng.tick_mark_real_irqTaskStart
 * bltSche.sSlot_idx_next          => g_scheMng.start_tick_preAllocWin */
#define bSlot_tick_irq_real           tick_mark_real_irqTaskStart

/* ---- task priority enum (higher value = higher priority) ---- */
typedef enum {
    TASK_PRIORITY_LOW             = 10,

    TASK_PRIORITY_MID             = 100,

    TASK_PRIORITY_PDA_SYNCED_FIRST = 200,

    TASK_PRIORITY_HIGH_THRES      = 220,

    TASK_PRIORITY_AUX_ADV         = 225,
    TASK_PRIORITY_AUX_SCAN_DFT    = 225,

    TASK_PRIORITY_PERD_ADV_DFT    = 228,

    TASK_PRIORITY_CONN_CREATE     = 230,

    TASK_PRIORITY_CONN_UPDATE     = 240,
    TASK_PRIORITY_PDA_SYNCING_DFT = 240,

    TASK_PRIORITY_MAX             = 500,  //bigger than TASK_PRIORITY_CONN_UPDATE + CONN_INTERVAL_100MS
} task_pri_t;

void tlk_sch_incSchedulerTaskPriority(u8 task_offset, int inc);
void tlk_sch_incSchedulerTaskCalPriority(u8 task_offset, int inc);

/* global variable: LinkController/LinkLayer state variable */
extern volatile u64 g_lcll_state;

/* ---- scheduler API ---- */
int tlk_sch_startScheduler(void);                       //internal API
void tlk_sch_buildTaskLinkList(void);
int tlk_sch_updateScheduler(void);
void tlk_sch_procStateChange(void);                     //Telink_stack_system process state change

int tlk_sch_addTask2ExistLinklist(sch_task_t *pStart_schTsk, int task_num_max, u32 sch_size);
int tlk_sch_addTask2AbandonTaskLinklist(sch_task_t *, int);

int tlk_sch_linkAllPostTask(sch_task_t *pTsk_cur, sch_task_t *pStart_schTsk, int start_idx, int task_num_max, u32 sch_size);

/* ---- IRQ-side inline helpers (operate on g_scheMng) ---- */
__INLINE void tlk_sch_addTaskMask(u64 tskmsk) {
    g_scheMng.task_mask |= (tskmsk);
}

__INLINE void tlk_sch_removeTaskMask(u64 tskmsk) {
    g_scheMng.task_mask &= ~(tskmsk);
}

__INLINE u64 tlk_sch_getTaskMask(void) {
    return g_scheMng.task_mask;
}

__INLINE void tlk_sch_addUpdate(u32 updt) {
    g_scheMng.update |= (updt);
}

__INLINE void tlk_sch_manualSetCurrentTaskOffset(u8 task_oft) {
    g_scheMng.cur_task_oft = task_oft;
}

/* ---- main-loop-side inline helpers (operate on task_ui_en / state_chng) ---- */
__INLINE void tlk_sch_addUiEnable(u64 tskmsk) {
    g_scheMng.task_ui_en |= (tskmsk);
}

__INLINE void tlk_sch_removeUiEnable(u64 tskmsk) {
    g_scheMng.task_ui_en &= ~(tskmsk);
}

__INLINE void tlk_sch_addStateChange(u8 change_mask) {
    g_scheMng.state_chng |= change_mask;
}

/* ---- tick comparison (handles 32-bit wrap-around) ---- */
_attribute_ble_ram_code_noinline_ int tick1_bigger_than_tick2(unsigned int tick1, unsigned int tick2);
_attribute_ble_ram_code_noinline_ int tick1_bigger_than_or_equal_to_tick2(unsigned int tick1, unsigned int tick2);
_attribute_ble_ram_code_noinline_ int tick1_smaller_than_tick2(unsigned int tick1, unsigned int tick2);
_attribute_ble_ram_code_noinline_ int tick1_smaller_than_or_equal_to_tick2(unsigned int tick1, unsigned int tick2);

extern int dbg_check_cnt;

static inline void debug_while_loop_reset(void)
{
#if (SCHEDULER_DEBUG_EN)
    dbg_check_cnt = 0;
#endif
}

static inline int debug_while_loop_cnt(int err_cnt, u32 err_code)
{
#if (SCHEDULER_DEBUG_EN)
    dbg_check_cnt++;
    if (dbg_check_cnt > err_cnt) {
        TLKSTK_ERROR_DEBUG(SCHEDULER_DEBUG_EN, err_code);
        return 1;
    }
#endif
    return 0;
}

/******************************* scheduler end *************************************************************************/


/******************************* access begin *************************************************************************/

/* Access layer: manages BLE primary channel scan task scheduling.
 * Uses a singly linked list built from sch_task_t nodes, sorted by priority. */

/* ---- access error codes ---- */
typedef enum {
    TLK_ACCESS_ERR_OK            = 0,
    TLK_ACCESS_ERR_INVALID_PARAM = 1,
    TLK_ACCESS_ERR_QUEUE_FULL    = 3,
} tlk_access_err_t;

#define ACCESS_INSERT_TASK_MAX      8    //max insert task pool size

/* scheTask_flg bit7: access layer enabled flag (task registered in access pool) */
#define SCH_TASK_FLG_ACCESS_ENABLED  BIT(7)

#define TSKMSK_ACCESS_ALL           (TSKMSK_PRICHN_SCAN)

#define LCLL_STATE_ACCESS_START     (LCLL_STATE_PRICHN_SCAN_S)
#define LCLL_STATE_ACCESS_END       (LCLL_STATE_PRICHN_SCAN_E)

/* Access insert task entry (metadata only; priority/handle/next/enabled are in sch_task_t) */
typedef struct {
    sch_task_t *pSchTask;       //pointer to scheduler task (has priority, next, scheTask_oft)
    u32 min_duration_us;        //minimum duration needed (us)
} access_insert_task_t;

/* Access management structure */
_attribute_aligned_(4) typedef struct __attribute__((packed))
{
    u8  access_align_offset;                        //align task offset, 0 means none
    u8  insert_task_num;                            //number of insert tasks
    u32 insert_ref_tick;                            //reference tick for timer
    u64 task_last_mask;                             //previous task mask for change detection

    u32 prio_bitmap;                                //bitmap: bit N set = priority N has enabled tasks
    access_insert_task_t insert_tasks[ACCESS_INSERT_TASK_MAX];  //task pool
    sch_task_t *pTask_head;                         //head of priority-sorted linked list (unchanged after sort)
    sch_task_t *pCur_task;                          //current cursor for round-robin
    u8  task_order_dirty;                           //1: linked list needs rebuild
} acs_mng_t;

extern acs_mng_t g_tlk_sch_acs_mng;

/* ---- access layer APIs ---- */
tlk_access_err_t tlk_sch_access_init(void);
void tlk_sch_access_task_update(void);
void tlk_sch_access_allocate_insert_task(void);
void tlk_sch_access_reset_insert(void);

/* ---- insert task management APIs ---- */
tlk_access_err_t tlk_sch_access_add_insert_task(sch_task_t *pSchTask, u32 min_duration_us);
tlk_access_err_t tlk_sch_access_remove_insert_task(sch_task_t *pSchTask);
u32 tlk_sch_access_get_remaining_time_us(void);
u32 tlk_sch_get_scheduler_busy(void);
/******************************* access end *************************************************************************/


/************* special define to compatible with some BLE old code, speed up the BLE development, Begin **********************/

/* global variable aliases */
#define bltSche                     g_scheMng
#define blms_state                  g_lcll_state

/* function aliases */
#define blt_sche_addTaskMask        tlk_sch_addTaskMask
#define blt_sche_removeTaskMask     tlk_sch_removeTaskMask
#define blt_sche_enableTask         tlk_sch_addUiEnable
#define blt_sche_disableTask        tlk_sch_removeUiEnable
#define blt_sche_addUpdate          tlk_sch_addUpdate

/* state aliases: BLMS_STATE_* -> LCLL_STATE_* */
#define BLMS_STATE_NONE                             LCLL_STATE_NONE

#define BLMS_STATE_LEGADV                           LCLL_STATE_LEGADV
#define BLMS_STATE_EXTADV_S                         LCLL_STATE_EXTADV_S
#define BLMS_STATE_EXTADV_E                         LCLL_STATE_EXTADV_E

#define BLMS_STATE_PRICHN_SCAN_S                    LCLL_STATE_PRICHN_SCAN_S
#define BLMS_STATE_PRICHN_SCAN_E                    LCLL_STATE_PRICHN_SCAN_E
#define BLMS_STATE_SECCHN_SCAN_S                    LCLL_STATE_SECCHN_SCAN_S
#define BLMS_STATE_SECCHN_SCAN_E                    LCLL_STATE_SECCHN_SCAN_E
#define BLMS_STATE_PDA_SYNC_S                       LCLL_STATE_PDA_SYNC_S
#define BLMS_STATE_PDA_SYNC_E                       LCLL_STATE_PDA_SYNC_E

#define BLMS_STATE_BTX_S                            LCLL_STATE_BTX_S
#define BLMS_STATE_BTX_E                            LCLL_STATE_BTX_E
#define BLMS_STATE_BRX_S                            LCLL_STATE_BRX_S
#define BLMS_STATE_BRX_E                            LCLL_STATE_BRX_E

#define BLMS_STATE_CIG_E                            LCLL_STATE_CIG_E
#define BLMS_STATE_CTX_S                            LCLL_STATE_CTX_S
#define BLMS_STATE_CTX_E                            LCLL_STATE_CTX_E
#define BLMS_STATE_CRX_S                            LCLL_STATE_CRX_S
#define BLMS_STATE_CRX_E                            LCLL_STATE_CRX_E

#define BLMS_STATE_BIG_E                            LCLL_STATE_BIG_E
#define BLMS_STATE_BBCST_S                          LCLL_STATE_BBCST_S   //BIS BCST Start
#define BLMS_STATE_BBCST_E                          LCLL_STATE_BBCST_E   //BIS BCST End
#define BLMS_STATE_BSYNC_S                          LCLL_STATE_BSYNC_S   //BIS SYNC Start
#define BLMS_STATE_BSYNC_E                          LCLL_STATE_BSYNC_E   //BIS SYNC End

#define BLMS_STATE_PAWRA_SLOT_GRP_E                 LCLL_STATE_PAWRA_SLOT_GRP_E  //PAwR_A Rsp_Slots Group End
#define BLMS_STATE_PAWRA_SLOT_S                     LCLL_STATE_PAWRA_SLOT_S      //PAwR_A Slot Start
#define BLMS_STATE_PAWRA_SLOT_E                     LCLL_STATE_PAWRA_SLOT_E      //PAwR_A Slot End

#define BLMS_STATE_PAWRS_SUB_S                      LCLL_STATE_PAWRS_SUB_S
#define BLMS_STATE_PAWRS_SUB_E                      LCLL_STATE_PAWRS_SUB_E

#define BLMS_STATE_CS_INIT_TX_S                     LCLL_STATE_CS_INIT_TX_S
#define BLMS_STATE_CS_INIT_TX_E                     LCLL_STATE_CS_INIT_TX_E
#define BLMS_STATE_CS_INIT_RX_S                     LCLL_STATE_CS_INIT_RX_S
#define BLMS_STATE_CS_INIT_E                        LCLL_STATE_CS_INIT_E

#define BLMS_STATE_CS_REFL_STEP_TX_S                LCLL_STATE_CS_REFL_STEP_TX_S
#define BLMS_STATE_CS_REFL_STEP_S                   LCLL_STATE_CS_REFL_STEP_S
#define BLMS_STATE_CS_REFL_STEP_E                   LCLL_STATE_CS_REFL_STEP_E
#define BLMS_STATE_CS_SUBEVENT_E                    LCLL_STATE_CS_SUBEVENT_E

#define BLMS_STATE_SNIFS_SEEK_S                     LCLL_STATE_SNIFS_SEEK_S
#define BLMS_STATE_SNIFS_SEEK_E                     LCLL_STATE_SNIFS_SEEK_E
#define BLMS_STATE_SNIFS_S                          LCLL_STATE_SNIFS_S
#define BLMS_STATE_SNIFS_E                          LCLL_STATE_SNIFS_E
#define BLMS_STATE_SNIFM_SEEK_S                     LCLL_STATE_SNIFM_SEEK_S
#define BLMS_STATE_SNIFM_SEEK_E                     LCLL_STATE_SNIFM_SEEK_E
#define BLMS_STATE_SNIFM_S                          LCLL_STATE_SNIFM_S
#define BLMS_STATE_SNIFM_E                          LCLL_STATE_SNIFM_E

/* scheduler API aliases */
#define blt_ll_setSchedulerTaskPriority             tlk_sch_setSchedulerTaskPriority
#define blt_ll_incSchedulerTaskPriority             tlk_sch_incSchedulerTaskPriority
#define blt_ll_incSchedulerTaskCalPriority          tlk_sch_incSchedulerTaskCalPriority

#define blt_ll_addTask2ExistLinklist(sch, taskNum)              tlk_sch_addTask2ExistLinklist(sch, taskNum, sizeof(sch))
#define blt_ll_addTask2ExistLinklistWithTaskSize(sch, taskNum, size)  tlk_sch_addTask2ExistLinklist(sch, taskNum, size)

#define blt_ll_calculate_sSlot_next(var)

/************* special define to compatible with some BLE old code, speed up the BLE development, End  **********************/

#endif /* TLKSTK_BTTPSLL_BTTPSLL_INTERNAL_H_ */
