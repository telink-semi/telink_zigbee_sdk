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
#if (__PROJECT_TL_GW__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleGateway.h"
#include "app_ui.h"
#include "gp.h"


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
    led_on(LED_POWER);
}

void localPermitJoinState(void)
{
    static bool assocPermit = 0;
    if (assocPermit != zb_getMacAssocPermit()) {
        assocPermit = zb_getMacAssocPermit();
        if (assocPermit) {
            led_on(LED_PERMIT);
        } else {
            led_off(LED_PERMIT);
        }
    }
}

static s32 zclLightTimerCb(void *arg)
{
    u32 interval = 0;

    if (g_appGwCtx.sta == g_appGwCtx.oriSta) {
        g_appGwCtx.times--;
        if (g_appGwCtx.times <= 0) {
            g_appGwCtx.timerLedEvt = NULL;
            return -1;
        }
    }

    g_appGwCtx.sta = !g_appGwCtx.sta;
    if (g_appGwCtx.sta) {
        light_on();
        interval = g_appGwCtx.ledOnTime;
    } else {
        light_off();
        interval = g_appGwCtx.ledOffTime;
    }

    return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
    u32 interval = 0;
    g_appGwCtx.times = times;

    if (!g_appGwCtx.timerLedEvt) {
        if (g_appGwCtx.oriSta) {
            light_off();
            g_appGwCtx.sta = 0;
            interval = ledOffTime;
        } else {
            light_on();
            g_appGwCtx.sta = 1;
            interval = ledOnTime;
        }

        g_appGwCtx.ledOnTime = ledOnTime;
        g_appGwCtx.ledOffTime = ledOffTime;
        g_appGwCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
    }
}

void light_blink_stop(void)
{
    if (g_appGwCtx.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&g_appGwCtx.timerLedEvt);

        g_appGwCtx.times = 0;
        if (g_appGwCtx.oriSta) {
            light_on();
        } else {
            light_off();
        }
    }
}

static ev_timer_event_t *brc_toggleEvt = NULL;
static s32 brc_toggleCb(void *arg)
{
    static bool toggle = 0;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstEp = SAMPLE_GW_ENDPOINT;
    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;
    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.txOptions = 0;
    dstEpInfo.radius = 0;

    toggle = !toggle;
    if (toggle) {
        zcl_onOff_onCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE);
    } else {
        zcl_onOff_offCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE);
    }

    return 0;
}

void brc_toggle(void)
{
    if (!brc_toggleEvt) {
        brc_toggleEvt = TL_ZB_TIMER_SCHEDULE(brc_toggleCb, NULL, 1000);
    } else {
        TL_ZB_TIMER_CANCEL(&brc_toggleEvt);
    }
}

static void buttonKeepPressed(u8 keyCnt, u8 keyCode)
{
    if (keyCnt >= 2) {
        //printf("Local factory reset...\n");
        //zb_factoryReset();
    } else {
        switch (keyCode) {
            case VK_SW1:
                if (zb_isDeviceJoinedNwk()) {
                    u8 duration = zb_getMacAssocPermit() ? 0 : 180;
                    u8 sn = 0;

                    printf("[Permit on] %d seconds\n", duration);
                    zb_mgmtPermitJoinReq(NWK_BROADCAST_ROUTER_COORDINATOR, duration, TRUE, &sn, NULL);
                    gpsCommissionModeInvork();
                }
                break;
            case VK_SW2:
                if (zb_isDeviceJoinedNwk()) {
                    printf("[On/Off] Timer test\n");
                    brc_toggle();
                }
                break;
            case VK_SW3:
            case VK_SW4:
                {
                    epInfo_t dstEpInfo;
                    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

                    dstEpInfo.profileId = HA_PROFILE_ID;
                    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
                    dstEpInfo.dstEp = SAMPLE_GW_ENDPOINT;
                    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;

                    move_t move;
                    move.rate = 60;
                    move.optPresent = 0;
                    move.optionsMask = 0;
                    move.optionsOverride = 0;
                    move.moveMode = (keyCode == VK_SW3) ? LEVEL_MOVE_UP : LEVEL_MOVE_DOWN;

                    if (move.moveMode == LEVEL_MOVE_UP) {
                        printf("[Level] Move up\n");
                    } else {
                        printf("[Level] Move down\n");
                    }
                    zcl_level_moveCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE, &move);
                }
                break;
            default:
                break;
        }
    }
}

static void buttonShortPressed(u8 keyCode)
{
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstEp = SAMPLE_GW_ENDPOINT;
    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;

    switch (keyCode) {
        case VK_SW1:
            printf("[On/Off] On\n");
            zcl_onOff_onCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE);
            break;
        case VK_SW2:
            printf("[On/Off] Off\n");
            zcl_onOff_offCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE);
            break;
        case VK_SW3:
        case VK_SW4:
            {
                static u8 level = ZCL_LEVEL_ATTR_MAX_LEVEL / 2;

                if (keyCode == VK_SW3) {
                    level += 30;
                } else {
                    level -= 30;
                }

                moveToLvl_t moveToLevel;
                moveToLevel.optPresent = 0;
                moveToLevel.optionsMask = 0;
                moveToLevel.optionsOverride = 0;
                moveToLevel.transitionTime = 10;
                moveToLevel.level = level;

                printf("[Level] Move to level %d\n", moveToLevel.level);
                zcl_level_move2levelCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE, &moveToLevel);
            }
            break;
        default:
            break;
    }
}

static void keyScan_keyReleasedCB(u8 keyCode)
{
    if (g_appGwCtx.state == APP_KEY_SHORT_PRESSED) {
        buttonShortPressed(keyCode);
    } else if (g_appGwCtx.state == APP_KEY_KEEP_PRESSED) {
        if ((keyCode == VK_SW3) || (keyCode == VK_SW4)) {
            epInfo_t dstEpInfo;
            TL_SETSTRUCTCONTENT(dstEpInfo, 0);

            dstEpInfo.profileId = HA_PROFILE_ID;
            dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
            dstEpInfo.dstEp = SAMPLE_GW_ENDPOINT;
            dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;

            stop_t stop;
            stop.optPresent = 0;
            stop.optionsMask = 0;
            stop.optionsOverride = 0;

            printf("[Level] Move stop\n");
            zcl_level_stopCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE, &stop);
        }
    }
}

static void keyScan_keyPressedCB(u8 keyCode)
{
    if ((keyCode >= VK_SW1) && (keyCode <= VK_SW4)) {
        g_appGwCtx.keyPressedTime = clock_time();
    }
}

void app_key_handler(void)
{
    static u8 valid_keyCode = 0xff;
    static u8 valid_keyCnt = 0;

    if (g_appGwCtx.state == APP_KEY_PRESSED) {
        if (clock_time_exceed(g_appGwCtx.keyPressedTime, 3 * 1000 * 1000)) {
            buttonKeepPressed(valid_keyCnt, valid_keyCode);
            g_appGwCtx.state = APP_KEY_KEEP_PRESSED;
        }
    }

    if (kb_scan_key(0, 1)) {
        valid_keyCnt = kb_event.cnt;
        printf("keyScan: cnt = %d, key = %d\n", valid_keyCnt, kb_event.keycode[0]);
        if (kb_event.cnt) {
            valid_keyCode = kb_event.keycode[0];
            keyScan_keyPressedCB(valid_keyCode);
            g_appGwCtx.state = APP_KEY_PRESSED;
        } else {
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            g_appGwCtx.state = APP_KEY_RELEASED;
        }
    }
}

#endif  /* __PROJECT_TL_GW__ */
