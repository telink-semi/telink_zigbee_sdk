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
#if (__PROJECT_TL_CONTACT_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleSensor.h"
#include "app_ui.h"

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

    if (g_sensorAppCtx.sta == g_sensorAppCtx.oriSta) {
        g_sensorAppCtx.times--;
        if (g_sensorAppCtx.times <= 0) {
            g_sensorAppCtx.timerLedEvt = NULL;
            return -1;
        }
    }

    g_sensorAppCtx.sta = !g_sensorAppCtx.sta;
    if (g_sensorAppCtx.sta) {
        light_on();
        interval = g_sensorAppCtx.ledOnTime;
    } else {
        light_off();
        interval = g_sensorAppCtx.ledOffTime;
    }

    return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
    u32 interval = 0;
    g_sensorAppCtx.times = times;

    if (!g_sensorAppCtx.timerLedEvt) {
        if (g_sensorAppCtx.oriSta) {
            light_off();
            g_sensorAppCtx.sta = 0;
            interval = ledOffTime;
        } else {
            light_on();
            g_sensorAppCtx.sta = 1;
            interval = ledOnTime;
        }

        g_sensorAppCtx.ledOnTime = ledOnTime;
        g_sensorAppCtx.ledOffTime = ledOffTime;
        g_sensorAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
    }
}

void light_blink_stop(void)
{
    if (g_sensorAppCtx.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&g_sensorAppCtx.timerLedEvt);

        g_sensorAppCtx.times = 0;
        if (g_sensorAppCtx.oriSta) {
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
                    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

                    g_zcl_iasZoneAttrs.zoneStatus = (keyCode == VK_SW1) ? ZONE_STATUS_BIT_ALARM1 : 0;

                    zoneStatusChangeNoti_t statusChangeNotification;
                    statusChangeNotification.zoneStatus = g_zcl_iasZoneAttrs.zoneStatus;
                    statusChangeNotification.extStatus = 0;
                    statusChangeNotification.zoneId = g_zcl_iasZoneAttrs.zoneId;
                    statusChangeNotification.delay = 0;

                    if (g_zcl_iasZoneAttrs.zoneState == ZONE_STATE_ENROLLED) {
                        printf("[IASZ] Zone Notification\n");
                        zcl_iasZone_statusChangeNotificationCmd(SAMPLE_SENSOR_ENDPOINT, &dstEpInfo, TRUE, &statusChangeNotification);
                    } else {
                        printf("[IASZ] Zone Not enrolled\n");
                    }
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
        g_sensorAppCtx.keyPressedCnt++;

        printf("Press [%d/3] times to start BDB steer\n", g_sensorAppCtx.keyPressedCnt);

        if (g_sensorAppCtx.keyPressedCnt >= 3) {
            g_sensorAppCtx.keyPressedCnt = 0;
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
                    dstEpInfo.dstEp = SAMPLE_SENSOR_ENDPOINT;
                    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;

                    if (keyCode == VK_SW1) {
                        printf("[On/Off] On\n");
                        zcl_onOff_onCmd(SAMPLE_SENSOR_ENDPOINT, &dstEpInfo, FALSE);
                    } else {
                        printf("[On/Off] Off\n");
                        zcl_onOff_offCmd(SAMPLE_SENSOR_ENDPOINT, &dstEpInfo, FALSE);
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
    if (g_sensorAppCtx.state == APP_KEY_SHORT_PRESSED) {
        buttonShortPressed(keyCode);
    } else if (g_sensorAppCtx.state == APP_KEY_KEEP_PRESSED) {

    }
}

static void keyScan_keyPressedCB(u8 keyCode)
{
    if ((keyCode >= VK_SW1) && (keyCode <= VK_SW4)) {
        g_sensorAppCtx.keyPressedTime = clock_time();
    }
}

void app_key_handler(void)
{
    static u8 valid_keyCode = 0xff;
    static u8 valid_keyCnt = 0;

    if (g_sensorAppCtx.state == APP_KEY_PRESSED) {
        if (clock_time_exceed(g_sensorAppCtx.keyPressedTime, 3 * 1000 * 1000)) {
            buttonKeepPressed(valid_keyCnt, valid_keyCode);
            g_sensorAppCtx.state = APP_KEY_KEEP_PRESSED;
        }
    } else {
        if (g_sensorAppCtx.keyPressedCnt) {
            if (clock_time_exceed(g_sensorAppCtx.keyPressedTime, 1 * 1000 * 1000)) {
                g_sensorAppCtx.keyPressedCnt = 0;
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
            g_sensorAppCtx.state = APP_KEY_PRESSED;
        } else {
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            g_sensorAppCtx.state = APP_KEY_RELEASED;
        }
    }
}

#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
