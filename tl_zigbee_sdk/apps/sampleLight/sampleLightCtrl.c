/********************************************************************************************************
 * @file    sampleLightCtrl.c
 *
 * @brief   This is the source file for sampleLightCtrl
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
#include "zcl_include.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"
#include <math.h>

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define PWM_FREQUENCY           1000//1KHz
#define PMW_MAX_TICK            (PWM_CLOCK_SOURCE / PWM_FREQUENCY)

#define clamp(a, min, max)      ((a) < (min) ? (min) : ((a) > (max) ? (max) : (a)))

/**********************************************************************
 * TYPEDEFS
 */
typedef struct {
    u8 r;
    u8 g;
    u8 b;
} color_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
#if COLOR_RGB_SUPPORT
color_t g_color;
#endif

/**********************************************************************
 * FUNCTIONS
 */
extern void sampleLight_onOffInit(void);
#if ZCL_LIGHT_COLOR_CONTROL_SUPPORT
extern void sampleLight_colorInit(void);
#elif ZCL_LEVEL_CTRL_SUPPORT
extern void sampleLight_levelInit(void);
#endif

/*********************************************************************
 * @fn      light_adjust
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_adjust(void)
{
    sampleLight_onOffInit();
#if ZCL_LIGHT_COLOR_CONTROL_SUPPORT
    sampleLight_colorInit();
#elif ZCL_LEVEL_CTRL_SUPPORT
    sampleLight_levelInit();
#endif
}

#if ZCL_LEVEL_CTRL_SUPPORT || ZCL_LIGHT_COLOR_CONTROL_SUPPORT
/*********************************************************************
 * @fn      pwmSet
 *
 * @brief
 *
 * @param   chn	    - PWM channel
 *          level   - level
 *
 * @return  None
 */
void pwmSet(u8 chn, u8 level)
{
    u32 cmp_tick = ((u32)level * PMW_MAX_TICK) / ZCL_LEVEL_ATTR_MAX_LEVEL;
    drv_pwm_cfg(chn, (u16)cmp_tick, PMW_MAX_TICK);
}
#endif

/*********************************************************************
 * @fn      light_init
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_init(void)
{
#if COLOR_RGB_SUPPORT || COLOR_CCT_SUPPORT || ZCL_LEVEL_CTRL_SUPPORT
    drv_pwm_init();
#endif

#if COLOR_RGB_SUPPORT
    R_LIGHT_PWM_SET();
    G_LIGHT_PWM_SET();
    B_LIGHT_PWM_SET();
    pwmSet(R_LIGHT_PWM_CHANNEL, 0);
    pwmSet(G_LIGHT_PWM_CHANNEL, 0);
    pwmSet(B_LIGHT_PWM_CHANNEL, 0);
#elif COLOR_CCT_SUPPORT
    WARM_LIGHT_PWM_SET();
    COOL_LIGHT_PWM_SET();
    pwmSet(WARM_LIGHT_PWM_CHANNEL, 0);
    pwmSet(COOL_LIGHT_PWM_CHANNEL, 0);
#elif ZCL_LEVEL_CTRL_SUPPORT
    COOL_LIGHT_PWM_SET();
    pwmSet(COOL_LIGHT_PWM_CHANNEL, 0);
#else
    drv_gpio_write(COOL_LIGHT_GPIO, 0);
#endif
}

#if COLOR_RGB_SUPPORT
/*********************************************************************
 * @fn      hsvToRGB
 *
 * @brief
 *
 * @param   [in]hue         -  hue attribute value
 *          [in]saturation  -  saturation attribute value
 *          [in]level       -  level attribute value
 *          [out]R          -  R light level
 *          [out]G          -  G light level
 *          [out]B          -  B light level
 *
 * @return  None
 */
static void hsvToRGB(u8 hue, u8 saturation, u8 level, u8 *R, u8 *G, u8 *B)
{
    u8 region, p, q, t;
    u32 h, s, v, remainder;

    if (saturation == 0) {
        *R = level;
        *G = level;
        *B = level;
    } else {
        h = hue;
        s = saturation;
        v = level;

        region = h / 43;
        remainder = (h - (region * 43)) * 6;
        p = (v * (255 - s)) >> 8;
        q = (v * (255 - ((s * remainder) >> 8))) >> 8;
        t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

        switch (region) {
        case 0:
            *R = v;
            *G = t;
            *B = p;
            break;
        case 1:
            *R = q;
            *G = v;
            *B = p;
            break;
        case 2:
            *R = p;
            *G = v;
            *B = t;
            break;
        case 3:
            *R = p;
            *G = q;
            *B = v;
            break;
        case 4:
            *R = t;
            *G = p;
            *B = v;
            break;
        case 5:
        default:
            *R = v;
            *G = p;
            *B = q;
            break;
        }
    }
}

/*********************************************************************
 * @fn      hwLight_colorUpdate_HSV2RGB
 *
 * @brief
 *
 * @param   hue	       -  hue attribute value
 *          saturation -  saturation attribute value
 *          level      -  level attribute value
 *
 * @return  None
 */
static void hwLight_colorUpdate_HSV2RGB(u8 hue, u8 saturation, u8 level)
{
    hsvToRGB(hue, saturation, level, &g_color.r, &g_color.g, &g_color.b);

    pwmSet(PWM_R_CHANNEL, g_color.r);
    pwmSet(PWM_G_CHANNEL, g_color.g);
    pwmSet(PWM_B_CHANNEL, g_color.b);
}

#ifndef COLOR_X_Y_DISABLE
/*********************************************************************
 * @fn      xyToRGB
 *
 * @brief   convert xyY color space to RGB
 *
 * @param   [in]currentX  -  current X value
 *          [in]currentY  -  current Y value
 *          [in]level     -  level attribute value
 *          [out]R        -  R light level
 *          [out]G        -  G light level
 *          [out]B        -  B light level
 *
 * @return  None
 */
static void xyToRGB(u16 currentX, u16 currentY, u8 level, u8 *R, u8 *G, u8 *B)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    //lookup table?
    return;
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL721X)
    float x, y, z;
    float X, Y, Z;
    float r, g, b;

    //refer https://en.wikipedia.org/wiki/CIE_1931_color_space#CIE_xy_chromaticity_diagram_and_the_CIE_xyY_color_space

    x = ((float)currentX) / 65535.0f;
    y = ((float)currentY) / 65535.0f;
    z = 1.0f - x - y;

    // Y - given brightness in 0 - 1 range
    Y = ((float)level) / 254.0f;
    X = (Y / y) * x;
    Z = (Y / y) * z;

    X = X / 100.0f;
    Y = Y / 100.0f;
    Z = Z / 100.0f;

    r = (X * 3.2406f) - (Y * 1.5372f) - (Z * 0.4986f);
    g = -(X * 0.9689f) + (Y * 1.8758f) + (Z * 0.0415f);
    b = (X * 0.0557f) - (Y * 0.2040f) + (Z * 1.0570f);

    r = (r <= 0.0031308f ? 12.92f * r : (1.055f) * pow(r, (1.0f / 2.4f)) - 0.055f);
    g = (g <= 0.0031308f ? 12.92f * g : (1.055f) * pow(g, (1.0f / 2.4f)) - 0.055f);
    b = (b <= 0.0031308f ? 12.92f * b : (1.055f) * pow(b, (1.0f / 2.4f)) - 0.055f);

    r = clamp(r, 0, 1);
    g = clamp(g, 0, 1);
    b = clamp(b, 0, 1);

    *R = (u8)(r * 255);
    *G = (u8)(g * 255);
    *B = (u8)(b * 255);
#endif
}

/*********************************************************************
 * @fn      hwLight_colorUpdate_XY2RGB
 *
 * @brief
 *
 * @param   currentX  -  current X value
 *          currentY  -  current Y value
 *          level     -  level attribute value
 *
 * @return  None
 */
static void hwLight_colorUpdate_XY2RGB(u16 currentX, u16 currentY, u8 level)
{
    xyToRGB(currentX, currentY, level, &g_color.r, &g_color.g, &g_color.b);

    pwmSet(PWM_R_CHANNEL, g_color.r);
    pwmSet(PWM_G_CHANNEL, g_color.g);
    pwmSet(PWM_B_CHANNEL, g_color.b);
}
#endif

#if COLOR_CCT_SUPPORT
/*********************************************************************
 * @fn      temperatureToRGB
 *
 * @brief   convert Mireds to centiKelvins. k = 1,000,000/mired
 *
 * @param   [in]colorTemperatureMireds  -  colorTemperatureMireds attribute value
 *          [in]level                   -  level attribute value
 *          [out]R                      -  R light level
 *          [out]G                      -  G light level
 *          [out]B                      -  B light level
 *
 * @return  None
 */
static void temperatureToRGB(u16 temperatureMireds, u8 level, u8 *R, u8 *G, u8 *B)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    return;
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL721X)
    float r, g, b;
    u16 tR, tG, tB;

    //algorithm credits to Tanner Helland:
    //https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html

    float centiKelvins = 10000 / temperatureMireds;

    r = (centiKelvins <= 66) ? 255
                             : (329.698727446f * pow(centiKelvins - 60, -0.1332047592f));
    g = (centiKelvins <= 66) ? (99.4708025861f * log(centiKelvins) - 161.1195681661f)
                             : (288.1221695283f * pow(centiKelvins - 60, -0.0755148492f));
    b = (centiKelvins >= 66) ? 255
                             : ((centiKelvins <= 19) ? 0 : (138.5177312231 * log(centiKelvins - 10) - 305.0447927307));

    tR = (u8)clamp(r, 0, 255);
    tG = (u8)clamp(g, 0, 255);
    tB = (u8)clamp(b, 0, 255);

    *R = (u8)(tR * level / ZCL_LEVEL_ATTR_MAX_LEVEL);
    *G = (u8)(tG * level / ZCL_LEVEL_ATTR_MAX_LEVEL);
    *B = (u8)(tB * level / ZCL_LEVEL_ATTR_MAX_LEVEL);
#endif
}
#endif

#elif COLOR_CCT_SUPPORT
/*********************************************************************
 * @fn      temperatureToCW
 *
 * @brief
 *
 * @param   [in]colorTemperatureMireds  -  colorTemperatureMireds attribute value
 *          [in]level                   -  level attribute value
 *          [out]C                      -  cool light level
 *          [out]W                      -  warm light level
 *
 * @return  None
 */
static void temperatureToCW(u16 temperatureMireds, u8 level, u8 *C, u8 *W)
{
    zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

    *W = (u8)(((temperatureMireds - pColor->colorTempPhysicalMinMireds) * level) / (pColor->colorTempPhysicalMaxMireds - pColor->colorTempPhysicalMinMireds));
    *C = level - (*W);
}
#endif

#if COLOR_CCT_SUPPORT
/*********************************************************************
 * @fn      hwLight_colorUpdate_colorTemperature
 *
 * @brief
 *
 * @param   colorTemperatureMireds  -  colorTemperatureMireds attribute value
 *          level                   -  level attribute value
 *
 * @return  None
 */
static void hwLight_colorUpdate_colorTemperature(u16 colorTemperatureMireds, u8 level)
{
#if COLOR_RGB_SUPPORT && COLOR_CCT_SUPPORT
    temperatureToRGB(colorTemperatureMireds, level, &g_color.r, &g_color.g, &g_color.b);

    pwmSet(PWM_R_CHANNEL, g_color.r);
    pwmSet(PWM_G_CHANNEL, g_color.g);
    pwmSet(PWM_B_CHANNEL, g_color.b);
#elif COLOR_CCT_SUPPORT
    u8 c = 0;
    u8 w = 0;

    temperatureToCW(colorTemperatureMireds, level, &c, &w);

    pwmSet(COOL_LIGHT_PWM_CHANNEL, c);
    pwmSet(WARM_LIGHT_PWM_CHANNEL, w);
#endif
}
#endif

#if ZCL_LIGHT_COLOR_CONTROL_SUPPORT
/*********************************************************************
 * @fn      light_colorUpdate
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
static void light_colorUpdate(void)
{
    zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();
    zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

#if COLOR_RGB_SUPPORT
    if (pColor->colorMode == ZCL_COLOR_MODE_CURRENT_HUE_SATURATION) {
        if (pColor->enhancedColorMode == ZCL_ENHANCED_COLOR_MODE_CURRENT_HUE_SATURATION) {
            pColor->currentHue = (pColor->enhancedCurrentHue & 0xFF00) >> 8;
            if (pColor->currentHue > ZCL_COLOR_ATTR_HUE_MAX) {
                pColor->currentHue = ZCL_COLOR_ATTR_HUE_MAX;
            }
        } else {
            pColor->enhancedCurrentHue = (u16)pColor->currentHue << 8;
        }

        hwLight_colorUpdate_HSV2RGB(pColor->currentHue, pColor->currentSaturation, pLevel->curLevel);
    } else
#ifndef COLOR_X_Y_DISABLE
    if (pColor->colorMode == ZCL_COLOR_MODE_CURRENT_X_Y) {
        hwLight_colorUpdate_XY2RGB(pColor->currentX, pColor->currentY, pLevel->curLevel);
    } else
#endif
#endif	/* COLOR_RGB_SUPPORT */
#if COLOR_CCT_SUPPORT
    if (pColor->colorMode == ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS) {
        hwLight_colorUpdate_colorTemperature(pColor->colorTemperatureMireds, pLevel->curLevel);
    } else
#endif
    {
		//do nothing
    }
}

#elif ZCL_LEVEL_CTRL_SUPPORT

/*********************************************************************
 * @fn      hwLight_levelUpdate
 *
 * @brief
 *
 * @param   level - level attribute value
 *
 * @return  None
 */
static void hwLight_levelUpdate(u8 level)
{
    pwmSet(COOL_LIGHT_PWM_CHANNEL, level);
}

/*********************************************************************
 * @fn      light_levelUpdate
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
static void light_levelUpdate(void)
{
    zcl_levelAttr_t *pLevel = zcl_levelAttrGet();
    hwLight_levelUpdate(pLevel->curLevel);
}
#endif

/*********************************************************************
 * @fn      hwLight_onOffUpdate
 *
 * @brief
 *
 * @param   onOff - onOff attribute value
 *
 * @return  None
 */
static void hwLight_onOffUpdate(bool onOff)
{
    if (onOff) {
#if COLOR_RGB_SUPPORT
        drv_pwm_start(R_LIGHT_PWM_CHANNEL);
        drv_pwm_start(G_LIGHT_PWM_CHANNEL);
        drv_pwm_start(B_LIGHT_PWM_CHANNEL);
#elif COLOR_CCT_SUPPORT
        drv_pwm_start(WARM_LIGHT_PWM_CHANNEL);
        drv_pwm_start(COOL_LIGHT_PWM_CHANNEL);
#elif ZCL_LEVEL_CTRL_SUPPORT
        drv_pwm_start(COOL_LIGHT_PWM_CHANNEL);
#else
        drv_gpio_write(COOL_LIGHT_GPIO, 1);
#endif
    } else {
#if COLOR_RGB_SUPPORT
        drv_pwm_stop(R_LIGHT_PWM_CHANNEL);
        drv_pwm_stop(G_LIGHT_PWM_CHANNEL);
        drv_pwm_stop(B_LIGHT_PWM_CHANNEL);
#elif COLOR_CCT_SUPPORT
        drv_pwm_stop(WARM_LIGHT_PWM_CHANNEL);
        drv_pwm_stop(COOL_LIGHT_PWM_CHANNEL);
#elif ZCL_LEVEL_CTRL_SUPPORT
        drv_pwm_stop(COOL_LIGHT_PWM_CHANNEL);
#else
        drv_gpio_write(COOL_LIGHT_GPIO, 0);
#endif
    }
}

/*********************************************************************
 * @fn      light_onOffUpdate
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
static void light_onOffUpdate(void)
{
    zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();
    hwLight_onOffUpdate(pOnOff->onOff);
}

/*********************************************************************
 * @fn      light_refresh
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_refresh(lightSta_e sta)
{
    switch (sta) {
#if ZCL_ON_OFF_SUPPORT
    case LIGHT_STA_ON_OFF:
        light_onOffUpdate();
        break;
#endif
#if ZCL_LIGHT_COLOR_CONTROL_SUPPORT
    case LIGHT_STA_LEVEL:
    case LIGHT_STA_COLOR:
        light_colorUpdate();
        break;
#elif ZCL_LEVEL_CTRL_SUPPORT
    case LIGHT_STA_LEVEL:
        light_levelUpdate();
        break;
#endif
    default:
        return;
        break;
    }

    gLightCtx.lightAttrsChanged = TRUE;
}

#if ZCL_LEVEL_CTRL_SUPPORT || ZCL_LIGHT_COLOR_CONTROL_SUPPORT
/*********************************************************************
 * @fn      light_applyUpdate
 *
 * @brief
 *
 * @param
 *
 * @return  None
 */
void light_applyUpdate(u8 *curLevel, u16 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u8 minLevel, u8 maxLevel, bool wrap)
{
    if ((*stepLevel256 > 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) > maxLevel)) {
        *curLevel256 = (wrap) ? ((u16)minLevel * 256 + ((*curLevel256 + *stepLevel256) - (u16)maxLevel * 256) - 256)
                              : ((u16)maxLevel * 256);
    } else if ((*stepLevel256 < 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) < minLevel)) {
        *curLevel256 = (wrap) ? ((u16)maxLevel * 256 - ((u16)minLevel * 256 - ((s32)*curLevel256 + *stepLevel256)) + 256)
                              : ((u16)minLevel * 256);
    } else {
        *curLevel256 += *stepLevel256;
    }

    if (*stepLevel256 > 0) {
        *curLevel = (*curLevel256 + 127) / 256;
    } else {
        *curLevel = *curLevel256 / 256;
    }

    if (*remainingTime == 0) {
        *curLevel256 = ((u16)*curLevel) * 256;
        *stepLevel256 = 0;
    } else if (*remainingTime != 0xFFFF) {
        *remainingTime = *remainingTime - 1;
    }
}

/*********************************************************************
 * @fn      light_applyUpdate_16
 *
 * @brief
 *
 * @param
 *
 * @return  None
 */
void light_applyUpdate_16(u16 *curLevel, u32 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u16 minLevel, u16 maxLevel, bool wrap)
{
    if ((*stepLevel256 > 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) > maxLevel)) {
        *curLevel256 = (wrap) ? ((u32)minLevel * 256 + ((*curLevel256 + *stepLevel256) - (u32)maxLevel * 256) - 256)
                              : ((u32)maxLevel * 256);
    } else if ((*stepLevel256 < 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) < minLevel)) {
        *curLevel256 = (wrap) ? ((u32)maxLevel * 256 - ((u32)minLevel * 256 - ((s32)*curLevel256 + *stepLevel256)) + 256)
                              : ((u32)minLevel * 256);
    } else {
        *curLevel256 += *stepLevel256;
    }

    if (*stepLevel256 > 0) {
        *curLevel = (*curLevel256 + 127) / 256;
    } else {
        *curLevel = *curLevel256 / 256;
    }

    if (*remainingTime == 0) {
        *curLevel256 = ((u32)*curLevel) * 256;
        *stepLevel256 = 0;
    } else if (*remainingTime != 0xFFFF) {
        *remainingTime = *remainingTime - 1;
    }
}
#endif

/*********************************************************************
 * @fn      light_blink_TimerEvtCb
 *
 * @brief
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 light_blink_TimerEvtCb(void *arg)
{
    u32 interval = 0;

    gLightCtx.sta = !gLightCtx.sta;
    if (gLightCtx.sta) {
        hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
        interval = gLightCtx.ledOnTime;
    } else {
        hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
        interval = gLightCtx.ledOffTime;
    }

    if (gLightCtx.sta == gLightCtx.oriSta) {
        if (gLightCtx.times) {
            gLightCtx.times--;
            if (gLightCtx.times <= 0) {
                if (gLightCtx.oriSta) {
                    hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
                } else {
                    hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
                }

                gLightCtx.timerLedEvt = NULL;
                return -1;
            }
        }
    }

    return interval;
}

/*********************************************************************
 * @fn      light_blink_start
 *
 * @brief
 *
 * @param   times       - counts
 * @param   ledOnTime	- on times, ms
 * @param   ledOffTime	- off times, ms
 *
 * @return  None
 */
void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
    u32 interval = 0;
    zcl_onOffAttr_t *pOnoff = zcl_onoffAttrGet();

    if (!gLightCtx.timerLedEvt) {
        gLightCtx.times = times;
        gLightCtx.ledOnTime = ledOnTime;
        gLightCtx.ledOffTime = ledOffTime;

        gLightCtx.oriSta = pOnoff->onOff;

        gLightCtx.sta = gLightCtx.oriSta;
        interval = gLightCtx.sta ? ledOnTime : ledOffTime;

        gLightCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(light_blink_TimerEvtCb, NULL, interval);
    }
}

/*********************************************************************
 * @fn      light_blink_stop
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_blink_stop(void)
{
    if (gLightCtx.timerLedEvt) {
        TL_ZB_TIMER_CANCEL(&gLightCtx.timerLedEvt);

        gLightCtx.times = 0;
        if (gLightCtx.oriSta) {
            hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
        } else {
            hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
        }
    }
}

#endif	/* __PROJECT_TL_DIMMABLE_LIGHT__ */
