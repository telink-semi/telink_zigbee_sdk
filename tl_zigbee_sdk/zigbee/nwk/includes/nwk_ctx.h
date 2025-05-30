/********************************************************************************************************
 * @file    nwk_ctx.h
 *
 * @brief   This is the header file for nwk_ctx
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
#ifndef NWK_CTX_H
#define NWK_CTX_H


/**
 * Startup control attribute
 */
typedef enum {
    STARTUP_CONTROL_PART_OF_NETWORK,
    STARTUP_CONTROL_FORM_A_NETWORK,
    STARTUP_CONTROL_REJOIN_NETWORK,
    STARTUP_CONTROL_START_FROM_SCRATCH
} StartupControl_attr_t;

/**
   Network layer running state
*/
typedef enum {
    NLME_STATE_IDLE,
    NLME_STATE_FORMATION,       //ed_scan, active_scan, mlme_start
    NLME_STATE_ROUTER_START,    //start_router, mlme_start
    NLME_STATE_DISC,            //active_scan
    NLME_STATE_REJOIN,          //active_scan
    NLME_STATE_DIRECT_JOIN,     //orphan_scan
    NLME_STATE_PANID_CONFLICT,  //mlme_start
    NLME_STATE_ED_SCAN,         //ed_scan
} nlme_state_t;

typedef enum {
    NLME_IDLE,
    NLME_JOINING,
    NLME_LEAVING,
} user_state_t;

typedef struct {
    u16 nwkFwdDstAddr;
    u16 nwkFwdSrcAddr;
    u16 senderAddr;
    u16 lastSendFailAddr;
} manyToOne_repair_t;

typedef struct {
    u16 nwkFwdDstAddr;
    u16 nwkFwdSrcAddr;
} srcRoute_repair_t;

/**
   Place to store values between multiply operations
*/
typedef struct {
    /* For coordinator to store formation information */
    struct {
        u8 energy_detect[TL_ZB_MAC_CHANNEL_NUM];
        u8 channel_pan_count[TL_ZB_MAC_CHANNEL_NUM];
    } formationInfo;
    /* End [32 bytes] */

    /* For router and end device  to store assocJoin/rejoin information */
    union {
        tl_zb_addition_neighbor_entry_t *pAssocJoinParent;
        tl_zb_addition_neighbor_entry_t *pRejoinParent;
    } join;
    /* End [4 bytes]*/

    /* For coordinator or router store current joining device info */
    ev_timer_event_t *curJoiningDevTimerEvt;//TODO: removed

    u32	scanChannels;

    u8 scanDuration;
    u8 is_factory_new:1;        /*!< Device is factory new */
    u8 permit_join:1;           /*!< True if permit join is in progress */
    u8 joined:1;                /*!< Non-zero if we are joined into the network */
    u8 router_started:1;        /*!< not used */
    u8 is_tc:1;                 /*!< True if we are Trust Center */
    u8 joined_pro:1;
    u8 panIdConflict:1;         /*!< if we find panID conflict */
    u8 joinAccept:1;
    u8 known_panids_cnt:4;
    u8 discoverRoute:1;         /*!< Always TRUE for NLDE data request! see 2.2.4.1.1.3 */
    u8 panidUpdateRecv:1;       /*!< receive network update for panid conflict*/
    u8 resv:1;
    u8 parentIsChanged:1;       /*!< A flag indicates that the device's parent is changed through rejoin */
    u8 user_state:4;            /*!< Current network user_state_t */
    u8 state:4;                 /*!< Current network subsystem state nlme_state_t */

    u8 leaveRejoin;             /*!< if we need rejoin after leave *///TODO: removed
    u16 new_panid;

    /* For PANID conflict resolution */
    u16 known_panids[PANID_TABLE_SIZE];

    manyToOne_repair_t manyToOneRepair; /* saved for check nlde-data.confirm status when unicast data frame using many-to-one */
    srcRoute_repair_t srcRouteRepair;
} nwk_ctx_t;

extern nwk_ctx_t g_zbNwkCtx;


#define ZB_SET_DISCOVER_ROUTE(v)        (g_zbNwkCtx.discoverRoute = v)
#define ZB_GET_DISCOVER_ROUTE           (g_zbNwkCtx.discoverRoute)

#define ZB_SET_USER_STATE(v)            (g_zbNwkCtx.user_state = v)
#define ZB_GET_USER_STATE               (g_zbNwkCtx.user_state)

#endif /* NWK_CTX_H */
