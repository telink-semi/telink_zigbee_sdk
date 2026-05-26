/********************************************************************************************************
 * @file    zbd_security_service.h
 *
 * @brief   This is the header file for zbd_security_service
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/
#ifndef _ZBD_SECURITY_SERVICE_H_
#define _ZBD_SECURITY_SERVICE_H_

#include "zbd_session.h"

#define KEY_VERIFY      0   /* for verifying the session message, using the vector test A, PSK and private,
                               PSK: ZigbeeAlliance18
                               ZVD:
                               EUI(00:1f:ee:00:00:00:00:01),
                               d(0xa0,0xf1,0xd9,0x2a,0x82,0xc8,0xd8,0xfe,0x43,0x4d,0x98,0x55,0x8c,0xe2,0xb3,0x47,
                                 0x17,0x11,0x98,0x54,0x2f,0x11,0x2d,0x05,0x58,0xf5,0x6b,0xd6,0x88,0x07,0x99,0x52)
                               ZDD:
                               EUI(00:1f:ee:00:00:00:00:02)
                               d(0x48,0x33,0x62,0x41,0xf3,0x0d,0x23,0xe5,0x5f,0x30,0xd1,0xc8,0xed,0x61,0x0c,0x4b,
                                 0x02,0x35,0x39,0x81,0x84,0xb8,0x14,0xa2,0x9c,0xb4,0x5a,0x67,0x2a,0xca,0xe5,0x48)*/

#define ZDB_ROLE_ZDD                1
#define ZDB_ROLE_ZVD                0

#define PUBLIC_POINT_LEN_P256       64
#define PUBLIC_POINT_LEN_25519      32
#define MAC_TAG_LEN_P256            32
#define MAC_TAG_LEN_25519           16

#define ZBD_SS_PACKET_LEN_MAX       82

typedef enum {
    ARRAY_CMP_EQUAL  = 0,
    ARRAY_CMP_LARGER = 1,
    ARRAY_CMP_LESS   = 2,
} ARRAY_CMP_e;

/**
 * @brief Zigbee direct security service role
 */
typedef enum {
    ZBD_SS_ROLE_ZVD = 0, /*! ZVD */
    ZBD_SS_ROLE_ZDD = 1, /*! ZDD */
} ZBD_SS_ROLE_e;

/**
 * @brief Opcode that identifies the Session Establishment message
 */
typedef enum {
    ZBD_SS_OPCODE_SEM1 = 1, /*! Session Establishment Message-1 */
    ZBD_SS_OPCODE_SEM2 = 2, /*! Session Establishment Message-2 */
    ZBD_SS_OPCODE_SEM3 = 3, /*! Session Establishment Message-3 */
    ZBD_SS_OPCODE_SEM4 = 4, /*! Session Establishment Message-4 */
    ZBD_SS_OPCODE_RESV = 5, /*! 5..255 Reserve */
} ZBD_SS_OPCODE_e;

/**
 * @brief TLV id for zigbee direct security service
 */
typedef enum {
    ZBD_SS_TLV_ID_KNR_SKNM   = 0,   /*! Key Negotiation Req Selected Key Negotiation Method TLV (ID=0) */
    ZBD_SS_TLV_ID_KNR_P256   = 1,   /*! Key Negotiation Req P-256 Public Point TLV (ID=1) */
    ZBD_SS_TLV_ID_KNR_X25519 = 2,   /*! Key Negotiation Req Curve25519 Public Point TLV (ID=2) */
    ZBD_SS_TLV_ID_NKSN       = 3,   /*! Network Key Sequence Number TLV (ID=3) */
    ZBD_SS_TLV_ID_MACTAG     = 4,   /*! MacTag TLV (ID=4) */
} ZBD_SS_TLV_ID_e;

/**
 * @brief The context of the Key Negotiation Req P-256/X25519 Public Point TLV
 */
typedef struct {
    u8 knm;	//ZBD_SS_SKNM_e
    u8 pskId; //ZBD_SS_PSKID_e
} zbd_ss_sknm_tlv_t;

/**
 * @brief The context of the Key Negotiation Req P-256/X25519 Public Point TLV
 */
typedef struct {
    u8 deviceEui64[8];
    union {
        u8 p256[64];
        u8 x25519[32];
    } Q;
} zbd_ss_knr_pp_tlv_t;

/**
 * @brief TLV id for zigbee direct security service
 */
typedef struct {
    zbd_ss_sknm_tlv_t pskKnm;
    zbd_ss_knr_pp_tlv_t knrpp;
    u8 nwkKeySeqno;
} zbd_ss_info_t;

/**
 * @brief The call back to sent session establishment messages
 */
typedef int (*app_zbd_semXCb_t)(void *p, u8 len);

/**
 * @brief Zigbee direct security service context
 */
typedef struct {
    ev_timer_event_t *waitNextMessageTimer;
    app_zbd_semXCb_t xCb;     /*! the call back to sent session establishment messages */
    u8 d[32];                 /*! private key d */
    u8 rmtEUI[8];             /*! remote ieee address */
    u8 Qi[64];                /*! ZVD public point, 32bytes for x25519, 64bytes for p256 */
    u8 macTagI[32];           /*! ZVD macTag */
    u8 Qr[64];                /*! ZDD public point: Qr = dr * G */
    u8 macTagR[32];           /*! ZDD macTag */
    u8 psk[16];               /*! pre-shared key: is to generate base point G */
    u8 sk[16];                /*! session key */
    u8 G[32];                 /*! base point: G = H * (PSK) */
    u8 rmtMacTag[32];         /*! remote macTag for validation */
    zbd_ss_sknm_tlv_t pskKnm; /*! pre-shared key and Key Negotiation Method */
    u8 nextSessMessId;  /*! ZBD_SS_OPCODE_e, the number of the next session message*/
    u8 nwkSeqNum;
    u8 limited;
} zbd_ss_ctx_t;

/**
 * @brief Generate private key d, base point G, public point Q
 *        Session Establishment Message 1 is sent from ZVD to ZDD and is used to initiate the Session
 *        Establishment protocol and send PSK Type (Pre-shared Secrets Bitmask) and ephemeral public
 *        key Qi.
 *
 * @input role: ZBD_SS_ROLE_ZVD or ZBD_SS_ROLE_ZDD
 * @input psk: pre-shared key(is to generate G)
 * @output d:  private key
 * @output G:  base point
 * @output Q:  public point
 */
void zbd_ss_25519dGQGenerate(ZBD_SS_ROLE_e role, u8 *psk);

/**
 * @brief Start a task to send session establishment messages
 *
 * @param  opCode - session establishment messages id
 * @param  pl     - session establishment messages payload
 *
 * @return status
 */
u8 zbd_ss_transTaskCreate(ZBD_SS_OPCODE_e opCode, void *pl);

/**
 * @brief  The callback on reception of the session establishment messages
 *
 * @param  pkt    - the received packet
 * @param  pktLen - the packet total length
 *
 * @return status
 */
u8 zbd_ss_semIndicate(void *pkt, u8 pktLen);

/**
 * @brief To check and process the session establishment message
 */
void zbd_ss_transTaskProc(void);

/**
 * @brief To reset the p_zbd_ss_ctx when the session establishment succeeds or fails
 */
void zbd_ss_reset(void);

/**
 * @brief Register the callback function for sending session message
 */
void zbd_ss_init(app_zbd_semXCb_t xCb);

/**
 * @brief Set the selected Pre-shared Secret Enumeration by the input message
 */
void ZBD_SS_PSKID_SET(ZBD_SS_PSKID_e pskId);

/**
 * @brief Set the pre-shared key
 */
void ZBD_SS_PSK_SET(u8 *psk);

void ZBD_SS_Qi_GET(u8 *Q);

void ZBD_SS_Qr_GET(u8 *Q);

#if ZDB_ROLE_ZVD
/**
 * @brief  Establish session by ZVD
 *
 * @param  sess  - session type
 * @param  pskId - pre-shared key type
 * @param  psk   - pre-shared key
 *
 * @return none
 */
void zbd_sessionEstablish(ZBD_SESS_TYPE_e sess, ZBD_SS_PSKID_e pskId, u8 *psk);

/**
 * @brief building Session Establishment Message 1
 *        Session Establishment Message 1 is sent from ZVD to ZDD and is used to initiate the Session
 *        Establishment protocol and send PSK Type (Pre-shared Secrets Bitmask) and ephemeral public
 *        key Qi.
 */
u8 zbd_ss_sem1Building(void *pkt, zbd_ss_info_t *info, u8 nwkKeyEn);
#endif

#endif
