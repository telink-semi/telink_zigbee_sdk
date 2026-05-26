/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
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
#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
#include "app_ui.h"
#if BLE_CONCURRENT_MODE
#include "zigbee_ble_switch.h"
#include "stack/ble/ble.h"
#endif
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void led_on(u32 pin)
{
    drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
    drv_gpio_write(pin, LED_OFF);
}

void light_on(void)
{
    led_on(LED_POWER);
}

void light_off(void)
{
    led_off(LED_POWER);
}

void light_init(void)
{
    led_off(LED_POWER);
}

static s32 zclLightTimerCb(void *arg)
{
    u32 interval = 0;

    if (g_switchAppCtx.sta == g_switchAppCtx.oriSta) {
        g_switchAppCtx.times--;
        if (g_switchAppCtx.times <= 0) {
            g_switchAppCtx.timerLedEvt = NULL;
            return -1;
        }
    }

    g_switchAppCtx.sta = !g_switchAppCtx.sta;
    if (g_switchAppCtx.sta) {
        light_on();
        interval = g_switchAppCtx.ledOnTime;
    } else {
        light_off();
        interval = g_switchAppCtx.ledOffTime;
    }

    return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
    u32 interval = 0;
    g_switchAppCtx.times = times;

    if (!g_switchAppCtx.timerLedEvt) {
        if (g_switchAppCtx.oriSta) {
            light_off();
            g_switchAppCtx.sta = 0;
            interval = ledOffTime;
        } else {
            light_on();
            g_switchAppCtx.sta = 1;
            interval = ledOnTime;
        }

        g_switchAppCtx.ledOnTime = ledOnTime;
        g_switchAppCtx.ledOffTime = ledOffTime;
        g_switchAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
    }
}

void light_blink_stop(void)
{
    if (g_switchAppCtx.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&g_switchAppCtx.timerLedEvt);

        g_switchAppCtx.times = 0;
        if (g_switchAppCtx.oriSta) {
            light_on();
        } else {
            light_off();
        }
    }
}

static void buttonKeepPressed(u8 keyCnt, u8 keyCode)
{
    if (keyCnt >= 2) {
        printf("Local factory reset...\n");
        zb_factoryReset();
    } else {
        switch (keyCode) {
            case VK_SW1:
            case VK_SW2:
                {
                    epInfo_t dstEpInfo;
                    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

                    dstEpInfo.profileId = HA_PROFILE_ID;
                    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
                    dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
                    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;

                    move_t move;
                    move.rate = 60;
                    move.optPresent = 0;
                    move.optionsMask = 0;
                    move.optionsOverride = 0;
                    move.moveMode = (keyCode == VK_SW1) ? LEVEL_MOVE_UP : LEVEL_MOVE_DOWN;

                    if (move.moveMode == LEVEL_MOVE_UP) {
                        printf("[Level] Move up\n");
                    } else {
                        printf("[Level] Move down\n");
                    }

                    zcl_level_moveCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &move);
                }
                break;
            case VK_SW3:
                printf("[TL] Touch Link commissioning\n");
                bdb_networkTouchLinkStart(BDB_COMMISSIONING_ROLE_INITIATOR);
                break;
            case VK_SW4:
                printf("[TL] Touch Link reset to FN\n");
                zcl_touchLinkReset2FN();
                bdb_networkTouchLinkStart(BDB_COMMISSIONING_ROLE_INITIATOR);
                break;
            default:
                break;
        }
    }
}

static void buttonShortPressed(u8 keyCode)
{
    if (zb_isDeviceFactoryNew()) {
        g_switchAppCtx.keyPressedCnt++;

        printf("Press [%d/3] times to start BDB steer\n", g_switchAppCtx.keyPressedCnt);

        if (g_switchAppCtx.keyPressedCnt >= 3) {
            g_switchAppCtx.keyPressedCnt = 0;
            printf("[BDB] Steer\n");
            bdb_networkSteerStart();
        }
    } else {
        switch (keyCode) {
            case VK_SW1:
            case VK_SW2:
                {
                    epInfo_t dstEpInfo;
                    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

                    dstEpInfo.profileId = HA_PROFILE_ID;
                    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
                    dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
                    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;

                    if (keyCode == VK_SW1) {
                        printf("[On/Off] On\n");
                        zcl_onOff_onCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
                    } else {
                        printf("[On/Off] Off\n");
                        zcl_onOff_offCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
                    }
                }
                break;
            case VK_SW3:
                printf("[BDB] Steer\n");
                bdb_networkSteerStart();
                break;
            case VK_SW4:
                printf("[BDB] Find & Bind\n");
                //BDB_ATTR_GROUP_ID_SET(0x1234);
                bdb_findAndBindStart(BDB_COMMISSIONING_ROLE_INITIATOR);
                break;
            default:
                break;
        }
    }
}

static void keyScan_keyReleasedCB(u8 keyCode)
{
    if (g_switchAppCtx.state == APP_KEY_SHORT_PRESSED) {
        buttonShortPressed(keyCode);
    } else if (g_switchAppCtx.state == APP_KEY_KEEP_PRESSED) {
        if ((keyCode == VK_SW1) || (keyCode == VK_SW2)) {
            epInfo_t dstEpInfo;
            TL_SETSTRUCTCONTENT(dstEpInfo, 0);

            dstEpInfo.profileId = HA_PROFILE_ID;
            dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
            dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
            dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;

            stop_t stop;
            stop.optPresent = 0;
            stop.optionsMask = 0;
            stop.optionsOverride = 0;

            printf("[Level] Move stop\n");
            zcl_level_stopCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &stop);
        }
    }
}

static void keyScan_keyPressedCB(u8 keyCode)
{
    if ((keyCode >= VK_SW1) && (keyCode <= VK_SW4)) {
        g_switchAppCtx.keyPressedTime = clock_time();
    }

#if BLE_CONCURRENT_MODE
#if BLE_ACTIVE_BY_UI
    if (APP_BLE_STATE_IDLE()) {
        ble_task_restart();
        extern void ble_advertiseTickUpdate(void);
        ble_advertiseTickUpdate();
    }
#endif
#endif
}

void app_key_handler(void)
{
    static u8 valid_keyCode = 0xff;
    static u8 valid_keyCnt = 0;

    if (g_switchAppCtx.state == APP_KEY_PRESSED) {
        if (clock_time_exceed(g_switchAppCtx.keyPressedTime, 3 * 1000 * 1000)) {
            buttonKeepPressed(valid_keyCnt, valid_keyCode);
            g_switchAppCtx.state = APP_KEY_KEEP_PRESSED;
        }
    } else {
        if (g_switchAppCtx.keyPressedCnt) {
            if (clock_time_exceed(g_switchAppCtx.keyPressedTime, 1 * 1000 * 1000)) {
                g_switchAppCtx.keyPressedCnt = 0;
                printf("Exit continuous key counting\n");
            }
        }
    }

    if (kb_scan_key(0, 1)) {
        valid_keyCnt = kb_event.cnt;
        printf("keyScan: cnt = %d, key = %d\n", valid_keyCnt, kb_event.keycode[0]);
        if (kb_event.cnt) {
            valid_keyCode = kb_event.keycode[0];
            keyScan_keyPressedCB(valid_keyCode);
            g_switchAppCtx.state = APP_KEY_PRESSED;
        } else {
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            g_switchAppCtx.state = APP_KEY_RELEASED;
        }
    }
}

#endif  /* __PROJECT_TL_SWITCH__ */
