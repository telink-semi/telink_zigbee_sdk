/********************************************************************************************************
 * @file    att.h
 *
 * @brief   This is the header file for BLE SDK
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
#pragma once

typedef struct __attribute__((packed))
{
    unsigned char opcode;
    unsigned char data[0];
} attr_pkt_t;

#ifndef MCU_CORE_D25F_ENABLE
#include "stack/ble/ble_multi/host/ble_host.h"
#endif
#include "att_cmd.h"
#include "atts.h"
#include "attc.h"

#define gap_server_para_t     gap_ms_para_t
#define blt_gap_getServerPara blc_gap_getMasterSlavePara
