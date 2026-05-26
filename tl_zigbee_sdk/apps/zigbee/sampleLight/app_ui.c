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
#if (__PROJECT_TL_DIMMABLE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"
#include "app_ui.h"
#include "gp.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
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

void led_init(void)
{
    led_off(LED_POWER);
    led_off(LED_PERMIT);
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

static void buttonKeepPressed(u8 keyCnt, u8 keyCode)
{
    if (keyCnt >= 2) {
        printf("Local factory reset...\n");
        zb_factoryReset();
    } else {
        switch (keyCode) {
            case VK_SW1:
                if (zb_isDeviceJoinedNwk()) {
                    /* toggle local permit Joining */
                    u8 duration = zb_getMacAssocPermit() ? 0 : 180;
                    zb_nlmePermitJoiningRequest(duration);
                }
                break;
            case VK_SW2:
                break;
            case VK_SW3:
                break;
            case VK_SW4:
                break;
            default:
                break;
        }
    }
}

static void buttonShortPressed(u8 keyCode)
{
    switch (keyCode) {
        case VK_SW1:
            {
                zcl_onOffAttr_t *pCurOnOffAttr = zcl_onoffAttrGet();
                if (pCurOnOffAttr) {
                    gLightCtx.sta = !pCurOnOffAttr->onOff;
                    if (gLightCtx.sta) {
                        sampleLight_onOffUpdate(ZCL_ONOFF_STATUS_ON);
                    } else {
                        sampleLight_onOffUpdate(ZCL_ONOFF_STATUS_OFF);
                    }
                }
            }
            break;
        case VK_SW2:
            break;
        case VK_SW3:
            printf("[BDB] Steer\n");
            bdb_networkSteerStart();
            break;
        case VK_SW4:
            printf("[BDB] Find & Bind\n");
            bdb_findAndBindStart(BDB_COMMISSIONING_ROLE_TARGET);
            break;
        default:
            break;
    }
}

static void keyScan_keyReleasedCB(u8 keyCode)
{
    if (gLightCtx.state == APP_KEY_SHORT_PRESSED) {
        buttonShortPressed(keyCode);
    } else if (gLightCtx.state == APP_KEY_KEEP_PRESSED) {

    }
}

static void keyScan_keyPressedCB(u8 keyCode)
{
    if ((keyCode >= VK_SW1) && (keyCode <= VK_SW4)) {
        gLightCtx.keyPressedTime = clock_time();
    }
}

void app_key_handler(void)
{
    static u8 valid_keyCode = 0xff;
    static u8 valid_keyCnt = 0;

    if (gLightCtx.state == APP_KEY_PRESSED) {
        if (clock_time_exceed(gLightCtx.keyPressedTime, 3 * 1000 * 1000)) {
            buttonKeepPressed(valid_keyCnt, valid_keyCode);
            gLightCtx.state = APP_KEY_KEEP_PRESSED;
        }
    }

    if (kb_scan_key(0, 1)) {
        valid_keyCnt = kb_event.cnt;
        printf("keyScan: cnt = %d, key = %d\n", valid_keyCnt, kb_event.keycode[0]);
        if (kb_event.cnt) {
            valid_keyCode = kb_event.keycode[0];
            keyScan_keyPressedCB(valid_keyCode);
            gLightCtx.state = APP_KEY_PRESSED;
        } else {
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            gLightCtx.state = APP_KEY_RELEASED;
        }
    }
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
