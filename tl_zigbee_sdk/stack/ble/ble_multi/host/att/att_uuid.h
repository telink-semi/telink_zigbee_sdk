/********************************************************************************************************
 * @file    att_uuid.h
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
#ifndef _ATT_UUID_H_
#define _ATT_UUID_H_

#include "tl_common.h"


/** @Bluetooth UUID Lengths */
#define ATT_16_UUID_LEN  2  /*!< \brief Length in bytes of a 16 bit UUID */
#define ATT_128_UUID_LEN 16 /*!< \brief Length in bytes of a 128 bit UUID */

/**@}*/


/** @brief This is a 'tentative' type and should be used as a pointer only */
typedef struct __attribute__((packed))
{
    u8 uuidLen;

    union
    {
        u8  u[0];
        u16 u16;
        u8  u128[ATT_128_UUID_LEN];
    } uuidVal;
} uuid_t;

typedef struct __attribute__((packed))
{
    u8  uuidLen;
    u16 uuidVal;
} uuid16_t;

//Initialize a 16-bit UUID.
#define UUID16_INIT(value)              \
    {                                   \
        .uuidLen     = ATT_16_UUID_LEN, \
        .uuidVal.u16 = (value),         \
    }

// Initialize a 128-bit UUID.
#define UUID128_INIT(value...)            \
    {                                     \
        .uuidLen      = ATT_128_UUID_LEN, \
        .uuidVal.u128 = {value},          \
    }

/** @Helper to declare a 16-bit UUID inline.
 *  @return Pointer to a generic UUID.
 */
#define UUID16_DEF(value)       ((uuid_t *)((uuid_t[]){UUID16_INIT(value)}))
#define UUID16_CPY(uuid, value) memcpy((uuid_t *)&uuid, UUID16_DEF(value), 1 + ATT_16_UUID_LEN)


/** @Helper to declare a 128-bit UUID inline.
 *  @return Pointer to a generic UUID.
 */
#define UUID128_DEF(value...)    ((uuid_t *)((uuid_t[]){UUID128_INIT(value)}))
#define UUID128_CPY(uuid, value) memcpy((uuid_t *)&uuid, UUID128_DEF(value), 1 + ATT_128_UUID_LEN)


/**
 * @brief       UUID Compare.
 * @param[in]   u1 -
 * @param[in]   u2 -
 * @return      int.
 */
int blc_uuid_cmp(const uuid_t *u1, const uuid_t *u2);

/**
 * @brief       UUID Create.
 * @param[in]   uuid -
 * @param[in]   data -
 * @param[in]   dataLen -
 * @return      bool.
 */
bool blc_uuid_create(uuid_t *uuid, const u8 *data, u8 dataLen);

/**
 * @brief       UUID16 switch to UUID128 then Compare.
 * @param[in]   pUuid16 -
 * @param[in]   pUuid128 -
 * @return      bool.
 */
bool blt_uuid_cmp16to128(const u8 *pUuid16, const u8 *pUuid128);

/**
 * @brief       UUID16 Compare.
 * @param[in]   pUuid16 -
 * @param[in]   uuidLen -
 * @param[in]   pUuid -
 * @return      bool.
 */
bool blt_uuid_cmp16or128(const u8 *pUuid16, u8 uuidLen, const u8 *pUuid);


#endif /* ATT_UUID_H_ */
