/* SPDX-License-Identifier: Apache-2.0 */
#pragma once
/* Optional B85/512K policy around the official BLE OTA server. No transport replacement. */
typedef void (*drv_b85_ota_start_cb)(void);
typedef void (*drv_b85_ota_result_cb)(int);
typedef void (*drv_b85_ota_progress_cb)(u8 stage);
int drv_b85_ota_prepare(u16 manufacturer,u16 image_type,drv_b85_ota_start_cb start,
                       drv_b85_ota_result_cb result,drv_b85_ota_progress_cb progress);
int drv_b85_ota_write(void *packet);
void drv_b85_flash_guard_init(void);
int drv_b85_flash_allow(u32 address,u32 length);
