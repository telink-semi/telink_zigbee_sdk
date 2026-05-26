/********************************************************************************************************
 * @file    mac_pib.h
 *
 * @brief   This is the header file for mac_pib
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
#ifndef MAC_PIB_H
#define MAC_PIB_H


#define ZB_PHY_MAX_PACKET_SIZE                  127
#define ZB_PHY_TURNROUNDTIME                    12
#define	ZB_US_PER_SYMBOL                        16

#define ZB_PHY_SYMBOLS_PER_OCTET                2
#define ZB_PHY_SHR_DURATION                     40
#define ZB_PHY_MAX_FRAME_DURATION               (ZB_PHY_SHR_DURATION + (ZB_PHY_MAX_PACKET_SIZE + 1) * ZB_PHY_SYMBOLS_PER_OCTET)

#define ZB_MAC_BASE_SLOT_DURATION               60    //!< The number of symbols forming a superframe slot when the superframe order is equal to 0
#define ZB_MAC_NUM_SUPERFRAME_SLOTS             16    //!< The number of slots contained in any superframe
#define ZB_MAC_BASE_SUPERFRAME_DURATION         (ZB_MAC_BASE_SLOT_DURATION * ZB_MAC_NUM_SUPERFRAME_SLOTS)
                                                      //!< The number of symbols forming a superframe when the superframe order is equal to 0
#define ZB_MAC_BASE_SUPERFRAME_DURATION_US      (ZB_MAC_BASE_SLOT_DURATION * ZB_MAC_NUM_SUPERFRAME_SLOTS * ZB_US_PER_SYMBOL)
                                                      //!< a superframe(us) when the superframe order is equal to 0
#define ZB_MAC_UNIT_BACKOFF_PERIOD              (20)  //!< The number of symbols forming the basic time period used by the CSMA-CA algorithm.
#define ZB_MAC_UNIT_BACKOFF_PERIOD_US           (ZB_MAC_UNIT_BACKOFF_PERIOD * ZB_US_PER_SYMBOL) //!< the basic time period(us) used by the CSMA-CA algorithm.
#define ZB_MAC_MIN_MPDU_OVERHEAD                (9)   //!< The minimum number of octets added by the MAC sublayer to the PSDU.
#define ZB_MAC_MIN_CAP_LEHGTH                   (440) //!< The minimum number of symbols forming the CAP.
#define ZB_MAC_MAX_SIF_FRAME_SIZE               (18)  //!< The maximum size of an MPDU
#define ZB_MAC_MAX_MPDU_UNSECURITY_OVERHEAD     (25)  //!< The maximum number of octets added by the MAC sublayer to the PSDU without security
#define ZB_MAC_MAX_MAC_PAYLOAD_SIZE             (ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MIN_MPDU_OVERHEAD)
                                                      //!< The maximum number of octets that can be transmitted in the MAC Payload field
#define ZB_MAC_MAX_MAC_SAFE_PAYLOAD_SIZE        (ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MAX_MPDU_UNSECURITY_OVERHEAD)
                                                      //!< The maximum number of octets that can be transmitted in the MAC Payload field of an unsecured MAC frame
#define ZB_MAC_MAX_LOST_BEACONS                 (4)   //!< The number of consecutive lost beacons that will cause the MAC sublayer of a receiving device to declare a loss of synchronization
#define ZB_MAC_MAX_BEACON_OVERHEAD              (75)  //!< The maximum number of octets added by the MAC sublayer to the MAC payload of a beacon frame.
#define ZB_MAC_MAX_BEACON_PAYLOAD_LENGTH        (ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MAX_BEACON_OVERHEAD)
                                                      //!< The maximum size, in octets, of a beacon payload
#define ZB_MAC_GTS_DESC_PERSISTENCE_TIME        (4)   //!< The number of consecutive lost beacons that will cause the MAC sublayer of a receiving device to declare a loss of synchronization.

#define ZB_MAC_WAIT_RESP_TIME_DEFAULT           (32)
#define	ZB_MAC_FRAME_RETRIES_MAX                (3)

/**
 * @brief beacon payload
 */
typedef struct {
    u8 protocol_id;
    u8 stack_profile:4;
    u8 protocol_version:4;

    u8 long_uptime:1;           //wwah, has been up for more than 24 hours
    u8 tc_connectivity:1;       //wwah, active communication path to TC
    u8 router_capacity:1;
    u8 device_depth:4;
    u8 end_device_capacity:1;

    extPANId_t extended_panid;
    u8 txoffset[3];
    u8 nwk_update_id;
} zb_mac_beacon_payload_t;

/**
 * @brief MAC PIB
 */
typedef struct {
    u32 beaconTxTime;
    addrExt_t coordExtAddress;
    addrExt_t extAddress;
    u16 ackWaitDuration;
    u16 coordShortAddress;
    u16 panId;
    u16 shortAddress;
    u16 superframeOrder;
    u16 transactionPersistenceTime;
    u16 frameTotalWaitTime;
    u16 syncSymbolOffset;//4
    zb_mac_beacon_payload_t beaconPayload;//15
    u8 beaconPayloadLen;
    u8 associationPermit;
    u8 associatedPanCoord;
    u8 autoReq;//4
    u8 battLifeExt;
    u8 battLifeExtPeriods;
    u8 beaconOrder;
    u8 beaconSeqNum;
    u8 seqNum;
    u8 frameRetryNum;
    u8 respWaitTime;
    u8 maxCsmaBackoffs;
    u8 promiscuousMode;
    u8 timeStampSupported;
    u8 securityEnabled;

    u8 gtsPermit;
    u8 minBe;
    u8 maxBe;
    u8 phyPageCur;
    u8 phyChannelCur;
    u8 rxOnWhenIdle;
    u8 txPower;
    u8 resv[3];//76-bytes

#if ZB_MAC_SECURITY
    void *keyTable;
    void *devTable;
    void *secLvlTable;
    u8 keyTableEntries;
    u8 devTableEntries;
    u8 secLvlTableEntries;
    u8 autoReqSecLvl;
    u32 frameCounter;
    u8 autoReqKeySource[8];
    u8 autoReqKeyIdMode;
    u8 autoReqKeyIndex;
    u16 panCoordShortAddr;
    u8 defaultKeySource[8];
    addrExt_t panCoordExtAddr;
#endif
} tl_zb_mac_pib_t;


extern tl_zb_mac_pib_t *g_mac_pib;

#define MAC_IB()                                (g_mac_pib)
#define ZB_PIB_SHORT_ADDRESS()                  (g_mac_pib->shortAddress)
#define ZB_PIB_EXTENDED_ADDRESS()               (g_mac_pib->extAddress)
#define ZB_PIB_RX_ON_WHEN_IDLE()                (g_mac_pib->rxOnWhenIdle)
#define ZB_PIB_ACK_WAIT_DURATION()              (g_mac_pib->ackWaitDuration)
#define ZB_PIB_MAC_DSN()                        (g_mac_pib->seqNum)
#define ZB_PIB_MAC_BSN()                        (g_mac_pib->beaconSeqNum)
#define ZB_PIB_MAC_DSN_INC()                    (g_mac_pib->seqNum++)
#define ZB_PIB_MAC_BSN_INC()                    (g_mac_pib->beaconSeqNum++)
#define ZB_PIB_BEACON_PAYLOAD()                 (g_mac_pib->beaconPayload)


/**
 * @brief  Generate IEEE address
 *
 * @param  none
 *
 * @return none
 */
void generateIEEEAddr(void);

/**
 * @brief  Set MAC PIB attribute
 *
 * @param  attribute - attribute id
 * @param  value     - value of the attribute
 * @param  index     - if the attribute is related to security,the means of index are entry number
 *                     otherwise the index means length, the length of the attribute value
 *
 * @return MAC_SUCCESS or others
 */
u8 tl_zbMacAttrSet(u8 attribute, u8 *value, u8 index);

/**
 * @brief  Get MAC PIB attribute value
 *
 * @param  attribute - attribute id
 * @param  value     - value of the attribute
 * @param  len       - the length of the attribute value
 *
 * @return MAC_SUCCESS or others
 */
u8 tl_zbMacAttrGet(u8 attribute, u8 *value, u8 *len);

#endif /* MAC_PIB_H */
