/********************************************************************************************************
 * @file    mac_hook.c
 *
 * @brief   This is the source file for mac_hook
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
#include "zb_common.h"

mac_params_t g_mac_params = {
    .pib_dft = &macPibDefault,
    .pib     = &g_zbMacPib,
    .diags   = &g_sysDiags.mac_diags
};

#if (ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE)
void tl_mac_beacon_payload_update(void)
{
    tl_zbNwkBeaconPayloadUpdate();
}

bool tl_mac_beacon_allow(u8 lqi)
{
    bool allow = FALSE;
    if (g_zbNwkCtx.joined && (lqi >= NWK_NEIGHBORTBL_ADD_LQITHRESHOLD)) {
        allow = TRUE;
    }
    return allow;
}

u8 tl_mac_beacon_tlv_len(void)
{
    u8 len = 0;
    if (g_nwkNIBAdd.wideBeaconTLVsLen || g_nwkNIBAdd.devBeaconTLVsLen) {
    	len += sizeof(tlv_t);
    	len += g_nwkNIBAdd.wideBeaconTLVsLen ? g_nwkNIBAdd.wideBeaconTLVsLen : 0;
    	len += g_nwkNIBAdd.devBeaconTLVsLen ? g_nwkNIBAdd.devBeaconTLVsLen : 0;
    }
    return len;
}

bool tl_mac_beacon_tlv_fill(u8 *ptr)
{
    if (g_nwkNIBAdd.wideBeaconTLVsLen || g_nwkNIBAdd.devBeaconTLVsLen) {
        *ptr++ = G_TLV_BEACON_APPENDIX_ENCAP;
        *ptr++ = (g_nwkNIBAdd.wideBeaconTLVsLen + g_nwkNIBAdd.devBeaconTLVsLen) - 1;
        if (g_nwkNIBAdd.wideBeaconTLVsLen) {
            memcpy(ptr, g_nwkNIBAdd.wideBeaconAppendixTLVs, g_nwkNIBAdd.wideBeaconTLVsLen);
            ptr += g_nwkNIBAdd.wideBeaconTLVsLen;
        }
        if (g_nwkNIBAdd.devBeaconTLVsLen) {
            memcpy(ptr, g_nwkNIBAdd.devBeaconAppendixTLVs, g_nwkNIBAdd.devBeaconTLVsLen);
            ptr += g_nwkNIBAdd.devBeaconTLVsLen;
        }
    }

    return TRUE;
}
#endif
