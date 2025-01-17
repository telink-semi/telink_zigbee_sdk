/********************************************************************************************************
 * @file    drv_flash.c
 *
 * @brief   This is the source file for drv_flash
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
#include "../tl_common.h"

#if FLASH_PROTECT_ENABLE
/*
 * Only the firmware area will be locked, the NV data area will not be locked.
 *
 * The SDK will select how much space to lock based on the size of the flash, to ensure
 * that the firmware area is protected while the NV data area is not locked. If the
 * conditions cannot be met, give up using the flash lock function.
 */
#define IS_FLASH_LOCK_NOT_ALLOWED()     ((NV_BASE_ADDRESS < 0x40000) ? 1 : 0)

drv_flash_opt_t *pFlashOpt = NULL;
drv_flash_opt_t g_flashOptTable = {0};
bool g_flashLocked = FALSE;

const drv_flash_opt_t c_flashOptList[] = {
#if defined(MCU_CORE_8258)
    //512K
    {0x13325e, flash_unlock_mid13325e, flash_lock_mid13325e, FLASH_LOCK_LOW_256K_MID13325E},
    {0x134051, flash_unlock_mid134051, flash_lock_mid134051, FLASH_LOCK_LOW_256K_MID134051},
    {0x136085, flash_unlock_mid136085, flash_lock_mid136085, FLASH_LOCK_LOW_256K_MID136085},
    {0x1360c8, flash_unlock_mid1360c8, flash_lock_mid1360c8, FLASH_LOCK_LOW_256K_MID1360C8},
    {0x1360eb, flash_unlock_mid1360eb, flash_lock_mid1360eb, FLASH_LOCK_LOW_256K_MID1360EB},
    //1M
    //{0x14325e, flash_unlock_mid14325e, flash_lock_mid14325e, FLASH_LOCK_NONE_MID14325E},
    //{0x1460c8, flash_unlock_mid1460c8, flash_lock_mid1460c8, FLASH_LOCK_NONE_MID1460C8},
    {0x11460c8, flash_unlock_mid011460c8, flash_lock_mid011460c8, FLASH_LOCK_LOW_512K_MID011460C8}
#elif defined(MCU_CORE_8278)
    //128K
    //{0x11325e, flash_unlock_mid11325e, flash_lock_mid11325e, FLASH_LOCK_NONE_MID11325E},
    //{0x1160c8, flash_unlock_mid1160c8, flash_lock_mid1160c8, FLASH_LOCK_NONE_MID1160C8},
    //512K
    {0x13325e, flash_unlock_mid13325e, flash_lock_mid13325e, FLASH_LOCK_LOW_256K_MID13325E},
    {0x1360c8, flash_unlock_mid1360c8, flash_lock_mid1360c8, FLASH_LOCK_LOW_256K_MID1360C8},
    //1M
    //{0x14325e, flash_unlock_mid14325e, flash_lock_mid14325e, FLASH_LOCK_NONE_MID14325E},
    //{0x1460c8, flash_unlock_mid1460c8, flash_lock_mid1460c8, FLASH_LOCK_NONE_MID1460C8},
    {0x146085, flash_unlock_mid146085, flash_lock_mid146085, FLASH_LOCK_LOW_512K_MID146085},
    //2M
    {0x1570cd, flash_unlock_mid1570cd, flash_lock_mid1570cd, FLASH_LOCK_LOW_512K_MID1570CD},
#elif defined(MCU_CORE_B91)
    //1M
    {0x146085, flash_unlock_mid146085, flash_lock_mid146085, FLASH_LOCK_LOW_512K_MID146085},
    //2M
    {0x156085, flash_unlock_mid156085, flash_lock_mid156085, FLASH_LOCK_LOW_512K_MID156085},
    //4M
    {0x166085, flash_unlock_mid166085, flash_lock_mid166085, FLASH_LOCK_LOW_512K_MID166085}
#elif defined(MCU_CORE_B92)
    //1M
    {0x146085, flash_unlock_mid146085, flash_lock_mid146085, FLASH_LOCK_LOW_512K_MID146085},
    {0x1460c8, flash_unlock_mid1460c8, flash_lock_mid1460c8, FLASH_LOCK_LOW_512K_MID1460C8},
    //2M
    {0x156085, flash_unlock_mid156085, flash_lock_mid156085, FLASH_LOCK_LOW_512K_MID156085},
    {0x1560c8, flash_unlock_mid1560c8, flash_lock_mid1560c8, FLASH_LOCK_LOW_512K_MID1560C8},
    //4M
    {0x166085, flash_unlock_mid166085, flash_lock_mid166085, FLASH_LOCK_LOW_512K_MID166085},
    {0x1660c8, flash_unlock_mid1660c8, flash_lock_mid1660c8, FLASH_LOCK_LOW_512K_MID1660C8},
    //16M
    {0x186085, flash_unlock_mid186085, flash_lock_mid186085, FLASH_LOCK_LOW_512K_MID186085}
#elif defined(MCU_CORE_TL721X)
    //2M
    {0x156085, flash_unlock_mid156085_with_device_num, flash_lock_mid156085_with_device_num, FLASH_LOCK_LOW_512K_MID156085},
#elif defined(MCU_CORE_TL321X)
    //512K
    {0x136085, flash_unlock_mid136085, flash_lock_mid136085, FLASH_LOCK_LOW_256K_MID136085},
    //1M
    {0x146085, flash_unlock_mid146085, flash_lock_mid146085, FLASH_LOCK_LOW_512K_MID146085},
    //2M
    {0x156085, flash_unlock_mid156085, flash_lock_mid156085, FLASH_LOCK_LOW_512K_MID156085},
    //4M
    {0x166085, flash_unlock_mid166085, flash_lock_mid166085, FLASH_LOCK_LOW_512K_MID166085},
#else
    {0, NULL, NULL, 0}
#endif
};

#define FLASH_OPT_LIST_NUM      (sizeof(c_flashOptList) / sizeof(drv_flash_opt_t))
#endif	/* FLASH_PROTECT_ENABLE */

void flash_loadOpt(void)
{
#if FLASH_PROTECT_ENABLE
#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X)
    g_flashOptTable.mid = flash_read_mid();
#elif defined(MCU_CORE_TL721X)
    g_flashOptTable.mid = flash_read_mid_with_device_num(SLAVE0);
#else
    return;
#endif

    if (IS_FLASH_LOCK_NOT_ALLOWED()) {
        pFlashOpt = NULL;
        return;
    }

    for (u8 i = 0; i < FLASH_OPT_LIST_NUM; i++) {
        if (g_flashOptTable.mid == c_flashOptList[i].mid) {
            g_flashOptTable.unlock = c_flashOptList[i].unlock;
            g_flashOptTable.lock = c_flashOptList[i].lock;
            g_flashOptTable.blockSize = c_flashOptList[i].blockSize;

            pFlashOpt = &g_flashOptTable;
            return;
        }
    }
#endif
}

void flash_lock(void)
{
#if FLASH_PROTECT_ENABLE
    if (pFlashOpt && pFlashOpt->lock) {
        if (g_flashLocked) {
            return;
        }

#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X)
        if (pFlashOpt->lock(pFlashOpt->blockSize) == 1) {
            g_flashLocked = TRUE;
        }
#elif defined(MCU_CORE_TL721X)
        if (pFlashOpt->lock(SLAVE0, pFlashOpt->blockSize) == 1) {
            g_flashLocked = TRUE;
        }
#endif
    }
#endif
}

void flash_unlock(void)
{
#if FLASH_PROTECT_ENABLE
    if (pFlashOpt && pFlashOpt->unlock) {
        if (!g_flashLocked) {
            return;
        }

#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X)
        if (pFlashOpt->unlock() == 1) {
            g_flashLocked = FALSE;
        }
#elif defined(MCU_CORE_TL721X)
        if (pFlashOpt->unlock(SLAVE0) == 1) {
            g_flashLocked = FALSE;
        }
#endif
    }
#endif
}

void flash_write(u32 addr, u32 len, u8 *buf)
{
#if (MODULE_WATCHDOG_ENABLE)
    drv_wd_clear();
#endif

#if (VOLTAGE_DETECT_ENABLE)
    if (drv_get_adc_data() < BATTERY_SAFETY_THRESHOLD) {
        return;
    }
#endif

    flash_write_page(addr, len, buf);
}

bool flash_writeWithCheck(u32 addr, u32 len, u8 *buf)
{
    s32 toalLen = (s32)len;
    s8 copyLen = 48;
    u8 pTemp[48];
    u8 wLen = 0;
    u32 sAddr = (u32)addr;
    u8 *pSrc = buf;

#if (MODULE_WATCHDOG_ENABLE)
    drv_wd_clear();
#endif

#if (VOLTAGE_DETECT_ENABLE)
    if (drv_get_adc_data() < BATTERY_SAFETY_THRESHOLD) {
        return FALSE;
    }
#endif

    flash_write_page(addr, len, buf);

    while (toalLen > 0) {
        wLen = (toalLen > copyLen) ? copyLen : toalLen;
        flash_read(sAddr, wLen, pTemp);
        if (memcmp(pTemp, pSrc, wLen)) {
            return FALSE;
        }
        toalLen -= wLen;
        sAddr += wLen;
        pSrc += wLen;
    }

    return TRUE;
}


void flash_read(u32 addr, u32 len, u8 *buf)
{
    flash_read_page(addr, len, buf);
}

void flash_erase(u32 addr)
{
#if (MODULE_WATCHDOG_ENABLE)
    drv_wd_clear();
#endif

#if (VOLTAGE_DETECT_ENABLE)
    if (drv_get_adc_data() < BATTERY_SAFETY_THRESHOLD) {
        return;
    }
#endif

    flash_erase_sector(addr);
}
