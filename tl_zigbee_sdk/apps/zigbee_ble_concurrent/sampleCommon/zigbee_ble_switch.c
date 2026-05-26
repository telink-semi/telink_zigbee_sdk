/********************************************************************************************************
 * @file    zigbee_ble_switch.c
 *
 * @brief   This is the source file for zigbee_ble_switch
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#include "tl_common.h"
#include "zb_common.h"
#if BLE_SUPPORT_CONTROLLER_ONLY
#include "hci_tr_h4.h"
#endif
#include "ble.h"
#include "zigbee_ble_switch.h"

#if PM_ENABLE
#include "app_pm.h"
#endif

#if defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL323X)
#include "device_manage.h"
#endif

#if BLE_MASTER_ROLE_ENABLE
ble_master_cb_t g_bleMasterHandler = {
    .serviceCb = NULL,
    .updateCb = NULL
};
extern int blm_disconnect;
#endif

app_dualModeInfo_t g_dualModeInfo = {
    .slot     = DUALMODE_SLOT_BLE,
    .bleState = BLS_LINK_STATE_IDLE,
};

volatile u8 switch_to_ble = 0;

extern u8 g_ble_txPowerSet;
extern int blt_sdk_main_loop(void);

void switch_to_zb_context(void)
{
    backup_ble_rf_context();

    ZB_RADIO_RX_DISABLE;

    restore_zb_rf_context();
    //switch tx power for zb mode
    ZB_RADIO_TX_POWER_SET(g_zb_txPowerSet);

    ZB_RADIO_RX_ENABLE;

    CURRENT_SLOT_SET(DUALMODE_SLOT_ZIGBEE);
}


_attribute_ram_code_ void switch_to_ble_context(void)
{
    /* disable zb rx dma to avoid un-excepted rx interrupt*/
    ZB_RADIO_TX_DISABLE;

    ZB_RADIO_RX_DISABLE;

    restore_ble_rf_context();
    /* switch tx power for ble mode */
    ZB_RADIO_TX_POWER_SET(g_ble_txPowerSet);

    CURRENT_SLOT_SET(DUALMODE_SLOT_BLE);
}

#if SCAN_IN_ADV_STATE
/*
 * ble is doing scan in the state of ADV
 *
 * @param ref_tick  beginning time the ADV
 *
 * @param index     is to control the scan interval, scan interval is (ADV-interval * index/16)
 *
 * */
static u32  blc_scan_busy_in_adv(u32 ref_tick, u8 index)
{
    unsigned int ret = 0;
    if (blts.scan_extension_mask & BLS_FLAG_SCAN_IN_ADV_MODE) {
        ret = ((unsigned int)((ref_tick + (blta.adv_interval >> 4) * (index & 0x0f)) - reg_system_tick)) < BIT(30);
    }
    return ret;
}
#endif


int is_switch_to_ble(void)
{
    if ((get_ble_next_event_tick() - (clock_time() + ZIGBEE_AFTER_TIME)) > BIT(31)) {
        return 1;
    } else {
        return 0;
    }
}

int is_switch_to_zigbee(void)
{
    if ((get_ble_next_event_tick() - (clock_time() + BLE_IDLE_TIME)) < BIT(31)) {
        return 1;
    } else {
        return 0;
    }
}


void zb_task(void)
{
    ev_main();

#if (MODULE_WATCHDOG_ENABLE)
    drv_wd_clear();
#endif

    tl_zbTaskProcedure();

#if BLE_SUPPORT_CONTROLLER_ONLY
    if (APP_BLE_STATE_IDLE()) {
        HCI_Tr_H4RxHandler();
    }
    extern void HCI_Handler(void);
    HCI_Handler();
#endif
}

void zb_ble_switch_proc(void)
{
    u32 r = 0;

#if defined(MCU_CORE_8258) || defined(MCU_CORE_B91)
    APP_BLE_STATE_SET(BLE_BLT_STATE_GET());
#endif

    if (CURRENT_SLOT_GET() == DUALMODE_SLOT_BLE) {
        g_dualModeInfo.bleTaskTick = clock_time();

        /* BLE task */
#if SCAN_IN_ADV_STATE
        /* add scan functionality after advertising during ADV state, just for slave mode */
        do {
            /* disable PM during scan */
#if PM_ENABLE
            if (BLE_BLT_STATE_GET() == BLS_LINK_STATE_ADV) {
                bls_pm_setSuspendMask(SUSPEND_DISABLE);
            }
#endif
            blt_sdk_main_loop();
        } while ((BLE_BLT_STATE_GET() == BLS_LINK_STATE_ADV) && blc_scan_busy_in_adv(g_dualModeInfo.bleTaskTick, 2));//MY_ADV_INTERVAL*3/4

        /* enable PM after scan */
#if PM_ENABLE
        bls_pm_setSuspendMask(SUSPEND_ADV | SUSPEND_CONN);
#endif

#else

#if ZIGBEE_DIRECT_ENABLE
        extern void zbd_ble_loopHandle(void);
        zbd_ble_loopHandle();
#endif
        blt_sdk_main_loop();
#endif

        r = drv_disable_irq();

        if (((get_ble_event_state() && is_switch_to_zigbee()) || APP_BLE_STATE_IDLE()) && (!switch_to_ble)) {
            /* ready to switch to ZIGBEE mode */
            switch_to_zb_context();

            drv_restore_irq(r);

            zb_task();
        } else {
            drv_restore_irq(r);
        }
    } else {
        /* now in ZIGBEE mode */
        r = drv_disable_irq();

#if BLE_SUPPORT_CONTROLLER_ONLY
        if (!zb_rfTxDoing() && (((is_switch_to_ble() || HCI_RxHaveData()) && !APP_BLE_STATE_IDLE()) || switch_to_ble))
#else
        if (!zb_rfTxDoing() && ((is_switch_to_ble() && !APP_BLE_STATE_IDLE()) || switch_to_ble))
#endif
        {
            /* ready to switch to BLE mode */
            switch_to_ble_context();

            drv_restore_irq(r);
            return;
        }
        drv_restore_irq(r);
        zb_task();
    }
}

void concurrent_mode_main_loop(void)
{
    zb_ble_switch_proc();

#if BLE_MASTER_ROLE_ENABLE
    if (CURRENT_SLOT_GET() == DUALMODE_SLOT_BLE && g_bleMasterHandler.serviceCb) {
        g_bleMasterHandler.serviceCb();
        g_bleMasterHandler.serviceCb = NULL;
    }
#endif

#if PM_ENABLE
    extern void app_key_handler(void);
    app_key_handler();
	app_pm_task();
#endif
}

u8 ble_task_stop(void)
{
    u32 r = drv_disable_irq();
    ble_sts_t ret = BLE_SUCCESS;

#if defined(MCU_CORE_8258) || defined(MCU_CORE_B91)
    if (APP_BLE_STATE_GET() == BLS_LINK_STATE_CONN) {
        ret = bls_ll_terminateConnection(HCI_ERR_OP_CANCELLED_BY_HOST);//cut any ble connections
    } else {
        ret = bls_ll_setAdvEnable(0);

        /* rf irq is cleared in the "bls_ll_setAdvEnable",
         * so that the rf tx/rx interrupt will be missed if the "bls_ll_setAdvEnable" is called in Zigbee mode
         */
        if (ret == BLE_SUCCESS) {
            ZB_RF_ISR_RECOVERY;
        }
    }
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X)
    if (!APP_BLE_STATE_IDLE()) {
        ble_sts_t ret1 = BLE_SUCCESS;
        ret = blc_ll_setAdvEnable(BLC_ADV_DISABLE);
        ret = blc_ll_setScanEnable (BLC_SCAN_DISABLE, DUP_FILTER_DISABLE);

        for (u8 i = 0; i < DEVICE_CHAR_INFO_MAX_NUM; i++) {
            if (conn_dev_list[i].conn_state == 1) {
                blc_ll_disconnect(conn_dev_list[i].conn_handle, HCI_ERR_REMOTE_USER_TERM_CONN);
            }
        }
        ret = ret & ret1;
    }
#endif
    drv_restore_irq(r);
    return ret;
}

u8 ble_task_restart(void)
{
    u32 r = drv_disable_irq();
#if defined(MCU_CORE_8258) || defined(MCU_CORE_B91)
    ble_sts_t ret = bls_ll_setAdvEnable(1);
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X)
    ble_sts_t ret = blc_ll_setAdvEnable(1);
#endif
    /* rf irq is cleared in the "bls_ll_setAdvEnable",
     * so that the rf tx/rx interrupt will be missed if the "bls_ll_setAdvEnable" is called in Zigbee mode
     */
    if (ret == BLE_SUCCESS) {
        ZB_RF_ISR_RECOVERY;
    }

    drv_restore_irq(r);
    return ret;
}

#if BLE_MASTER_ROLE_ENABLE
void ble_master_serviceCbRegister(master_service_t cb)
{
    g_bleMasterHandler.serviceCb = cb;
}

void ble_master_updateIndCbRegister(master_update_t cb)
{
    g_bleMasterHandler.updateCb = cb;
}
#endif
