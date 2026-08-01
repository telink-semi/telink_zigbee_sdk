/********************************************************************************************************
 * @file    sampleLightDiretMultiBLE.c
 *
 * @brief   This is the source file for sampleLightDirectMultiBLE
 *
 * @author  Zigbee Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include "stack/ble/ble.h"
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"
#include "zbd_include.h"
#include "device_manage.h"
#include "controller/ll/adv/leg_adv.h"

int central_smp_pending = 0;        // SMP: security & encryption;
unsigned int  tlk_flash_mid = 0;
unsigned int  tlk_flash_vendor = 0;
unsigned char tlk_flash_capacity;
u32 flash_sector_mac_address = FLASH_ADDR_OF_MAC_ADDR_1M;


/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ************************************************/
/**
 * @brief   connMaxRxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 * 2. for CIS peripheral, receive ll_cis_req(36Byte), must be equal to or greater than 36
 */
#define ACL_CONN_MAX_RX_OCTETS          48  //user set value

/**
 * @brief   connMaxTxOctets
 * refer to BLE SPEC: Vol 6, Part B, "4.5.10 Data PDU length management"
 *                    Vol 6, Part B, "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 *  in this SDK, we separate this value into 2 parts: peripheralMaxTxOctets and centralMaxTxOctets,
 *  for purpose to save some SRAM costed by when peripheral and central use different connMaxTxOctets.
 *
 * usage limitation for ACL_xxx_MAX_TX_OCTETS
 * 1. should be in range of 27 ~ 251
 * 2. for CIS central, send ll_cis_req(36Byte), ACL_CENTRAL_MAX_TX_OCTETS must be equal to or greater than 36
 */
#define ACL_CENTRAL_MAX_TX_OCTETS       48  //user set value
#define ACL_PERIPHR_MAX_TX_OCTETS       48  //user set value


/**
 * @brief   ACL RX buffer size & number
 *          ACL RX buffer is shared by all connections to hold LinkLayer RF RX data.
 * usage limitation for ACL_RX_FIFO_SIZE:
 * 1. must use CAL_LL_ACL_RX_FIFO_SIZE to calculate, user can not change !!!
 *
 * usage limitation for ACL_RX_FIFO_NUM:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 4, 8, 16
 */
#define ACL_RX_FIFO_SIZE                CAL_LL_ACL_RX_FIFO_SIZE(ACL_CONN_MAX_RX_OCTETS)  //user can not change !!!
#define ACL_RX_FIFO_NUM                 8   //user set value

/**
 * @brief   ACL TX buffer size & number
 *          ACL Central TX buffer is shared by all central connections to hold LinkLayer RF TX data.
*           ACL Peripheral TX buffer is shared by all peripheral connections to hold LinkLayer RF TX data.
 * usage limitation for ACL_xxx_TX_FIFO_SIZE:
 * 1. must use CAL_LL_ACL_TX_FIFO_SIZE to calculate, user can not change !!!
 *
 * usage limitation for ACL_xxx_TX_FIFO_NUM:
 * 1. must be: (2^n) + 1, (power of 2, then add 1)
 * 2. for B91m IC: at least 9; recommended value: 9, 17, 33; other value not allowed.
 * 3. for B85m IC: at least 8; recommended value: 8, 16, 32; other value not allowed.
 *
 * only for B91: usage limitation for size * (number - 1)
 * 1. (ACL_xxx_TX_FIFO_SIZE * (ACL_xxx_TX_FIFO_NUM - 1)) must be less than 4096 (4K)
 *    so when ACL TX FIFO size equal to or bigger than 256, ACL TX FIFO number can only be 9(can not use 17 or 33), cause 256*(17-1)=4096
 */
#define ACL_CENTRAL_TX_FIFO_SIZE        CAL_LL_ACL_TX_FIFO_SIZE(ACL_CENTRAL_MAX_TX_OCTETS) //user can not change !!!
#define ACL_CENTRAL_TX_FIFO_NUM         9   //user set value

#define ACL_PERIPHR_TX_FIFO_SIZE        CAL_LL_ACL_TX_FIFO_SIZE(ACL_PERIPHR_MAX_TX_OCTETS) //user can not change !!!
#define ACL_PERIPHR_TX_FIFO_NUM         9   //user set value

/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***************************************************/


/***************** ACL connection L2CAP RX & TX data Buffer allocation, Begin **************************************/
/**
 * @brief   RX MTU size & L2CAP buffer size
 * RX MTU:
 * refer to BLE SPEC: Vol 3, Part F, "3.2.8 Exchanging MTU size" & "3.4.2 MTU exchange"; Vol 3, Part G, "4.3.1 Exchange MTU"
 * this SDK set ACL Central and Peripheral RX MTU buffer separately to save some SRAM when Central and Peripheral can use different RX MTU.
 *
 * CENTRAL_ATT_RX_MTU & PERIPHR_ATT_RX_MTU
 * 1. must equal to or bigger than 23
 * 2. if support LE Secure Connections, must equal to or bigger than 65
 *
 * CENTRAL_L2CAP_BUFF_SIZE & PERIPHR_L2CAP_BUFF_SIZE
 * 1. must use CAL_L2CAP_BUFF_SIZE to calculate, user can not change !!!
 */
#define CENTRAL_ATT_RX_MTU              247//23  //user set value
#define PERIPHR_ATT_RX_MTU              247//23  //user set value

#define CENTRAL_L2CAP_BUFF_SIZE         CAL_L2CAP_BUFF_SIZE(CENTRAL_ATT_RX_MTU) //user can not change !!!
#define PERIPHR_L2CAP_BUFF_SIZE         CAL_L2CAP_BUFF_SIZE(PERIPHR_ATT_RX_MTU) //user can not change !!!

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

/**
 * @brief   ACL RX buffer, shared by all connections to hold LinkLayer RF RX data.
 *          user should define and initialize this buffer if either ACL Central or ACL Peripheral is used.
 */
_attribute_ble_data_retention_  u8  app_acl_rx_fifo[ACL_RX_FIFO_SIZE * ACL_RX_FIFO_NUM] = {0};

#if ACL_CENTRAL_MAX_NUM
/**
 * @brief   ACL Central TX buffer, shared by all central connections to hold LinkLayer RF TX data.
 *          ACL Central TX buffer should be defined only when ACl connection central role is used.
 */
_attribute_ble_data_retention_  u8  app_acl_cen_tx_fifo[ACL_CENTRAL_TX_FIFO_SIZE * ACL_CENTRAL_TX_FIFO_NUM * ACL_CENTRAL_MAX_NUM] = {0};
#endif

/**
 * @brief   ACL Peripheral TX buffer, shared by all peripheral connections to hold LinkLayer RF TX data.
 *          ACL Peripheral TX buffer should be defined only when ACl connection peripheral role is used.
 */
_attribute_ble_data_retention_  u8  app_acl_per_tx_fifo[ACL_PERIPHR_TX_FIFO_SIZE * ACL_PERIPHR_TX_FIFO_NUM * ACL_PERIPHR_MAX_NUM] = {0};

/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/

#if ACL_CENTRAL_MAX_NUM
/***************** ACL connection L2CAP RX & TX data Buffer allocation, Begin **************************************/
/**
 * @brief   L2CAP RX Data buffer for ACL Central
 */
_attribute_ble_data_retention_  u8 app_cen_l2cap_rx_buf[ACL_CENTRAL_MAX_NUM * CENTRAL_L2CAP_BUFF_SIZE];


/**
 * @brief   L2CAP TX Data buffer for ACL Central
 *          if GATT server on ACL Central used, this buffer must be defined and initialized.
 *          if GATT server on ACL Central not used, this buffer can be saved.
 */
//_attribute_ble_data_retention_    u8 app_cen_l2cap_tx_buf[ACL_CENTRAL_MAX_NUM * CENTRAL_L2CAP_BUFF_SIZE];
#endif

/**
 * @brief   L2CAP RX Data buffer for ACL Peripheral
 */
_attribute_ble_data_retention_  u8 app_per_l2cap_rx_buf[ACL_PERIPHR_MAX_NUM * PERIPHR_L2CAP_BUFF_SIZE];

/**
 * @brief   L2CAP TX Data buffer for ACL Peripheral
 *          GATT server on ACL Peripheral use this buffer.
 */
_attribute_ble_data_retention_  u8 app_per_l2cap_tx_buf[ACL_PERIPHR_MAX_NUM * PERIPHR_L2CAP_BUFF_SIZE];

u8 g_ble_txPowerSet = RF_POWER_P3dBm;

static u16  g_appBleInterval = CONN_INTERVAL_50MS;
static u16 g_appBleLatency = 19;
static u16 g_bleSlaveConnHandle = 0;
/***************** ACL connection L2CAP RX & TX data Buffer allocation, End ****************************************/

/**
 * @brief   BLE Advertising data
 */
const u8    tbl_advData[] = {
    13, DT_COMPLETE_LOCAL_NAME,                'm','u','l','t','i','-','-','l','i','g','h','t',
    2,  DT_FLAGS,                              0x05,                   // BLE limited discoverable mode and BR/EDR not supported
    3,  DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID, 0xf7, 0xff,             // commissioning service UUID
};

/**
 * @brief   BLE Scan Response Packet data
 */
const u8    tbl_scanRsp [] = {
    13, DT_COMPLETE_LOCAL_NAME,                'm','u','l','t','i','-','-','l','i','g','h','t',
};

typedef enum {
    ATT_H_START = 0,


    //// Gap ////
    /**********************************************************************************************/
    GenericAccess_PS_H,                     //UUID: 2800,   VALUE: uuid 1800
    GenericAccess_DeviceName_CD_H,          //UUID: 2803,   VALUE:              Prop: Read | Notify
    GenericAccess_DeviceName_DP_H,          //UUID: 2A00,   VALUE: device name
    GenericAccess_Appearance_CD_H,          //UUID: 2803,   VALUE:              Prop: Read
    GenericAccess_Appearance_DP_H,          //UUID: 2A01,   VALUE: appearance
    CONN_PARAM_CD_H,                        //UUID: 2803,   VALUE:              Prop: Read
    CONN_PARAM_DP_H,                        //UUID: 2A04,   VALUE: connParameter


    //// gatt ////
    /**********************************************************************************************/
    GenericAttribute_PS_H,                  //UUID: 2800,   VALUE: uuid 1801
    GenericAttribute_ServiceChanged_CD_H,   //UUID: 2803,   VALUE:              Prop: Indicate
    GenericAttribute_ServiceChanged_DP_H,   //UUID: 2A05,   VALUE: service change
    GenericAttribute_ServiceChanged_CCB_H,  //UUID: 2902,   VALUE: serviceChangeCCC


    //// device information ////
    /**********************************************************************************************/
    DeviceInformation_PS_H,                 //UUID: 2800,   VALUE: uuid 180A
    DeviceInformation_pnpID_CD_H,           //UUID: 2803,   VALUE:              Prop: Read
    DeviceInformation_pnpID_DP_H,           //UUID: 2A50,   VALUE: PnPtrs

    //// battery service ////
    /**********************************************************************************************/
    BATT_PS_H,                              //UUID: 2800,   VALUE: uuid 180f
    BATT_LEVEL_INPUT_CD_H,                  //UUID: 2803,   VALUE:              Prop: Read | Notify
    BATT_LEVEL_INPUT_DP_H,                  //UUID: 2A19    VALUE: batVal
    BATT_LEVEL_INPUT_CCB_H,                 //UUID: 2902,   VALUE: batValCCC

    //// zigbee direct security service ////
    /**********************************************************************************************/
    ZBD_SS_PS_H,
    ZBD_SS_25519_CD_H,
    ZBD_SS_25519_DP_H,
    ZBD_SS_25519_DESC_H,

    ZBD_SS_P256_CD_H,
    ZBD_SS_P256_DP_H,
    ZBD_SS_P256_DESC_H,

    //// commissioning service:26-41////
    /**********************************************************************************************/
    COMM_PS_H,                              //UUID: FFF7,   VALUE: uuid fff7
    COMM_FORM_NETWORK_CD_H,                 //UUID: 2803,   VALUE:              Prop: Write
    COMM_FORM_NETWORK_DP_H,                 //UUID: 7072377D-0001-421C-B163-491C27333A61    VALUE: struct formNetwork

    COMM_JOIN_NETWORK_CD_H,                 //UUID: 2803,   VALUE:              Prop: Write
    COMM_JOIN_NETWORK_DP_H,                 //UUID: 7072377D-0002-421C-B163-491C27333A61    VALUE: struct joinNetwork

    COMM_PERMIT_JOIN_CD_H,                  //UUID: 2803,   VALUE:              Prop: Write
    COMM_PERMIT_JOIN_DP_H,                  //UUID: 7072377D-0003-421C-B163-491C27333A61    VALUE: uint8 permitJoin

    COMM_LEAVE_NETWORK_CD_H,                //UUID: 2803,   VALUE:              Prop: Write
    COMM_LEAVE_NETWORK_DP_H,                //UUID: 7072377D-0004-421C-B163-491C27333A61    VALUE: struct leaveNetwork

    COMM_COMM_STATUS_CD_H,                  //UUID: 2803,   VALUE:              Prop: Read | Notify
    COMM_COMM_STATUS_DP_H,                  //UUID: 7072377D-0005-421C-B163-491C27333A61    VALUE: struct commission status
    COMM_COMM_STATUS_CCB_H,                 //UUID: 2902,   VALUE: commStatusCCC

    COMM_MANAGE_JOINERS_CD_H,               //UUID: 2803,   VALUE:              Prop: Write
    COMM_MANAGE_JOINERS_DP_H,               //UUID: 7072377D-0006-421C-B163-491C27333A61    VALUE: struct manage joiners

    COMM_IDENTIFY_CD_H,                     //UUID: 2803,   VALUE:              Prop: Write
    COMM_IDENTIFY_DP_H,                     //UUID: 7072377D-0007-421C-B163-491C27333A61    VALUE: struct leaveNetwork

    COMM_FIND_BIND_CD_H,                //UUID: 2803,   VALUE:              Prop: Write
    COMM_FIND_BIND_DP_H,                //UUID: 7072377D-0008-421C-B163-491C27333A61    VALUE: struct finding&binding

    //// zigbee tunnel service:42-45////
    /**********************************************************************************************/
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
    ZBD_TUNNEL_PS_H,
    ZBD_TUNNEL_NPDU_CD_H,
    ZBD_TUNNEL_NPDU_DP_H,
    ZBD_TUNNEL_NPDU_DESC_H,
#endif

    ATT_END_H,

} ATT_HANDLE;


////////////////////////////////////////// peripheral-role ATT service concerned ///////////////////////////////////////////////
typedef struct {
    /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
    u16 intervalMin;
    /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
    u16 intervalMax;
    /** Number of LL latency connection events (0x0000 - 0x03e8) */
    u16 latency;
    /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
    u16 timeout;
} gap_periConnectParams_t;

static const u16 clientCharacterCfgUUID = GATT_UUID_CLIENT_CHAR_CFG;

static const u16 serviceChangeUUID = GATT_UUID_SERVICE_CHANGE;

static const u16 my_primaryServiceUUID = GATT_UUID_PRIMARY_SERVICE;

static const u16 my_characterUUID = GATT_UUID_CHARACTER;

static const u16 my_devServiceUUID = SERVICE_UUID_DEVICE_INFORMATION;

static const u16 my_PnPUUID = CHARACTERISTIC_UUID_PNP_ID;

static const u16 my_devNameUUID = GATT_UUID_DEVICE_NAME;

static const u16 my_gapServiceUUID = SERVICE_UUID_GENERIC_ACCESS;

static const u16 my_appearanceUUID = GATT_UUID_APPEARANCE;

static const u16 my_periConnParamUUID = GATT_UUID_PERI_CONN_PARAM;

static const u16 my_appearance = GAP_APPEARANCE_UNKNOWN;

static const u16 my_gattServiceUUID = SERVICE_UUID_GENERIC_ATTRIBUTE;

static const gap_periConnectParams_t my_periConnParameters = {20, 40, 0, 1000};

_attribute_ble_data_retention_  static u16 serviceChangeVal[2] = {0};

_attribute_ble_data_retention_  static u8 serviceChangeCCC[2] = {0,0};

static const u8 my_devName[] = {'m','u','l','t','i','-','-','l','i','g','h','t'};

static const u8 my_PnPtrs [] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};

//////////////////////// Battery /////////////////////////////////////////////////
static const u16 my_batServiceUUID = SERVICE_UUID_BATTERY;
static const u16 my_batCharUUID = CHARACTERISTIC_UUID_BATTERY_LEVEL;
_attribute_ble_data_retention_  static u8 batteryValueInCCC[2] = {0,0};
_attribute_ble_data_retention_  static u8 my_batVal[1]  = {99};


//// GAP attribute values
static const u8 my_devNameCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(GenericAccess_DeviceName_DP_H), U16_HI(GenericAccess_DeviceName_DP_H),
    U16_LO(GATT_UUID_DEVICE_NAME), U16_HI(GATT_UUID_DEVICE_NAME)
};
static const u8 my_appearanceCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(GenericAccess_Appearance_DP_H), U16_HI(GenericAccess_Appearance_DP_H),
    U16_LO(GATT_UUID_APPEARANCE), U16_HI(GATT_UUID_APPEARANCE)
};
static const u8 my_periConnParamCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(CONN_PARAM_DP_H), U16_HI(CONN_PARAM_DP_H),
    U16_LO(GATT_UUID_PERI_CONN_PARAM), U16_HI(GATT_UUID_PERI_CONN_PARAM)
};

//// GATT attribute values
static const u8 my_serviceChangeCharVal[5] = {
    CHAR_PROP_INDICATE,
    U16_LO(GenericAttribute_ServiceChanged_DP_H), U16_HI(GenericAttribute_ServiceChanged_DP_H),
    U16_LO(GATT_UUID_SERVICE_CHANGE), U16_HI(GATT_UUID_SERVICE_CHANGE)
};

//// device Information  attribute values
static const u8 my_PnCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(DeviceInformation_pnpID_DP_H), U16_HI(DeviceInformation_pnpID_DP_H),
    U16_LO(CHARACTERISTIC_UUID_PNP_ID), U16_HI(CHARACTERISTIC_UUID_PNP_ID)
};

//// Battery attribute values
static const u8 my_batCharVal[5] = {
    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
    U16_LO(BATT_LEVEL_INPUT_DP_H), U16_HI(BATT_LEVEL_INPUT_DP_H),
    U16_LO(CHARACTERISTIC_UUID_BATTERY_LEVEL), U16_HI(CHARACTERISTIC_UUID_BATTERY_LEVEL)
};


////////////////////////// zigbee direct security  /////////////////////////////////////////////////
static const u8 my_zbdSecurityServiceUUID[16]   = {ZBD_SEC_SERVICE_UUID_SERVICE};
static const u8 my_zbdSecurityCharUUID25519[16]      = {CHARACTERISTIC_UUID_ZBD_SS_SPEKE_AES128};
static const u8 my_zbdSecurityCharUUIDP256[16]      = {CHARACTERISTIC_UUID_ZBD_SS_ECDHE_SHA256};
static u8 zbdSec25519ValueCCC[2] = {0};
static u8 zbdSec256ValueCCC[2] = {0};
const u16 my_zbdSecuritycfgUUID                 = GATT_UUID_CLIENT_CHAR_CFG;

static const u8 my_ss25519Val[19] = {
    CHAR_PROP_WRITE | CHAR_PROP_INDICATE,
    U16_LO(ZBD_SS_25519_DP_H),
    U16_HI(ZBD_SS_25519_DP_H),
    CHARACTERISTIC_UUID_ZBD_SS_SPEKE_AES128
};

static const u8 my_ss256Val[19] = {
    CHAR_PROP_WRITE | CHAR_PROP_INDICATE,
    U16_LO(ZBD_SS_P256_DP_H),
    U16_HI(ZBD_SS_P256_DP_H),
    CHARACTERISTIC_UUID_ZBD_SS_ECDHE_SHA256
};

/* The max length is decided by the message2 */
u8  my_zbdSecirutyData[ZBD_SS_PACKET_LEN_MAX] = {0};
s32 my_zbdSecirutyDataLen = 0;
u16 my_zbdSecRecvHnadle = 0;
int app_zbdSecurityWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;
    s32 len = req->l2cap-3;
    my_zbdSecirutyDataLen = len;
    if (len > 0 && len <= ZBD_SS_PACKET_LEN_MAX) {
        memcpy(my_zbdSecirutyData, &req->dat[0], len);//req->rf_len - 7);
        if (zbd_ss_semIndicate(my_zbdSecirutyData, len)) {
            return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
        } else {
            my_zbdSecRecvHnadle = req->handle;
        }
    }

    return ATT_SUCCESS;
}

int app_zbd_ssSemNotifyX(void *p, u8 len)
{
    u8 ret = blc_gatt_pushHandleValueIndicate(g_bleSlaveConnHandle, my_zbdSecRecvHnadle, p, len);

    if (GATT_ERR_DATA_PENDING_DUE_TO_SERVICE_DISCOVERY_BUSY == ret) {
        blc_gap_setSingleServerDataPendingTime_upon_ClientCmd(g_bleSlaveConnHandle, 0);
        blc_gatt_pushHandleValueIndicate (g_bleSlaveConnHandle, my_zbdSecRecvHnadle, p, len);
    }
    return 0;
}

////////////////////////// zigbee direct commissioning /////////////////////////////////////////////////
const u16 my_commServiceUUID                    = SERVICE_UUID_COMMISSIONING;
const u8 my_formNetworkCharUUID[16]             = {CHARACTERISTIC_UUID_FORM_NETWORK};
const u8 my_joinNetworkCharUUID[16]             = {CHARACTERISTIC_UUID_JOIN_NETWORK};
const u8 my_permitJoinCharUUID[16]              = {CHARACTERISTIC_UUID_PERMIT_JOINING};
const u8 my_leaveNetworkCharUUID[16]            = {CHARACTERISTIC_UUID_LEAVE_NETWORK};
const u8 my_commissioningStatusCharUUID[16]     = {CHARACTERISTIC_UUID_COMMISSIONING_STATUS};
const u16 my_commissioningStatusCfgUUID         = GATT_UUID_CLIENT_CHAR_CFG;
const u8 my_manageJoinersCharUUID[16]           = {CHARACTERISTIC_UUID_MANAGE_JOINERS};
const u8 my_identifyCharUUID[16]                = {CHARACTERISTIC_UUID_IDENTIFY};
const u8 my_findBindCharUUID[16]                = {CHARACTERISTIC_UUID_FINDING_BINDING};

static const u8 my_formVal[19] = {
        CHAR_PROP_WRITE,
        U16_LO(COMM_FORM_NETWORK_DP_H),
        U16_HI(COMM_FORM_NETWORK_DP_H),
        CHARACTERISTIC_UUID_FORM_NETWORK};

static const u8 my_joinVal[19] = {
        CHAR_PROP_WRITE,
        U16_LO(COMM_JOIN_NETWORK_DP_H),
        U16_HI(COMM_JOIN_NETWORK_DP_H),
        CHARACTERISTIC_UUID_JOIN_NETWORK};

static const u8 my_permitJoinVal[19] = {
        CHAR_PROP_WRITE,
        U16_LO(COMM_PERMIT_JOIN_DP_H),
        U16_HI(COMM_PERMIT_JOIN_DP_H),
        CHARACTERISTIC_UUID_PERMIT_JOINING};

static const u8 my_leaveVal[19] = {
        CHAR_PROP_WRITE,
        U16_LO(COMM_LEAVE_NETWORK_DP_H),
        U16_HI(COMM_LEAVE_NETWORK_DP_H),
        CHARACTERISTIC_UUID_LEAVE_NETWORK};

static const u8 my_connstatusVal[19] = {
        CHAR_PROP_READ | CHAR_PROP_NOTIFY,
        U16_LO(COMM_COMM_STATUS_DP_H),
        U16_HI(COMM_COMM_STATUS_DP_H),
        CHARACTERISTIC_UUID_JOIN_NETWORK};

static const u8 my_manajoinVal[19] = {
        CHAR_PROP_WRITE,
        U16_LO(COMM_MANAGE_JOINERS_DP_H),
        U16_HI(COMM_MANAGE_JOINERS_DP_H),
        CHARACTERISTIC_UUID_MANAGE_JOINERS};

static const u8 my_identifyVal[19] = {
        CHAR_PROP_READ | CHAR_PROP_WRITE,
        U16_LO(COMM_IDENTIFY_DP_H),
        U16_HI(COMM_IDENTIFY_DP_H),
        CHARACTERISTIC_UUID_IDENTIFY};

static const u8 my_findbindVal[19] = {
        CHAR_PROP_WRITE,
        U16_LO(COMM_FIND_BIND_DP_H),
        U16_HI(COMM_FIND_BIND_DP_H),
        CHARACTERISTIC_UUID_FINDING_BINDING};

u32 my_NetworkData = 0x00;
u8 my_commissStatus[COMMISSION_STATUS_COMMAND_LEN_MAX] = {0};
u8 my_identifyTime[12] = {0};

u8 my_commissChangeCCC[2] = {0};

extern void blc_att_setReadRsp_len(int len);

int app_zbd_commStatusHandle(void)
{
    u8 realLen = 0;
    memset(my_commissStatus, 0, sizeof(my_commissStatus));
    zbd_commissStatusNotifSend(g_bleSlaveConnHandle, COMM_COMM_STATUS_DP_H, my_commissStatus, &realLen);

    return 0;
}

int app_formNetworkWrite(u16 connHandle, void *p)
{
    //data_len = rf_len - 7
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;
    if ((req->l2cap >= 3) && (req->l2cap <= COMMISSION_COMMAND_LEN_MAX-3)) {
        return zbd_commissionCharaWriteHandle(connHandle, req->handle, COMM_COMM_STATUS_DP_H, FORM_NETWORK_OPERATION, req->l2cap-3, req->dat);
    } else {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }
}

int app_joinNetworkWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

    if ((req->l2cap >= 3) && (req->l2cap <= COMMISSION_COMMAND_LEN_MAX-3)) {
        return zbd_commissionCharaWriteHandle(connHandle, req->handle, COMM_COMM_STATUS_DP_H, JOIN_NETWORK_OPERATION, req->l2cap-3, req->dat);
    } else {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }
}

int app_permitJoinWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

    if ((req->l2cap >= 3) && (req->l2cap <= COMMISSION_COMMAND_LEN_MAX-3)) {
        return zbd_commissionCharaWriteHandle(connHandle, req->handle, COMM_COMM_STATUS_DP_H, PERMIT_JOINING_OPERATION, req->l2cap-3, req->dat);
    } else {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }
}

int app_leaveNetworkWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

    if ((req->l2cap >= 3) && (req->l2cap <= COMMISSION_COMMAND_LEN_MAX-3)) {
        return zbd_commissionCharaWriteHandle(connHandle, req->handle, COMM_COMM_STATUS_DP_H, LEAVE_NETWORK_OPERATION, req->l2cap-3, req->dat);
    } else {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }
}

int app_commissStatusRead(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

    u8 realLen = 0;
    memset(my_commissStatus, 0, sizeof(my_commissStatus));
    zbd_commissStatusReadHandle(connHandle, req->handle, my_commissStatus, &realLen);

    if (realLen == 0) {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    } else {
        blc_att_setReadRsp_len(realLen);
    }

    return 0;
}

int app_manageJoinersWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

    if ((req->l2cap >= 3) && (req->l2cap <= COMMISSION_COMMAND_LEN_MAX-3)) {
        return zbd_commissionCharaWriteHandle(connHandle, req->handle, COMM_COMM_STATUS_DP_H, MANAGE_JOINERS_DOMAIN, req->l2cap-3, req->dat);
    } else {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }
}

int app_identifyWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

    if ((req->l2cap >= 3) && (req->l2cap <= COMMISSION_COMMAND_LEN_MAX-3)) {
        return zbd_commissionCharaWriteHandle(connHandle, req->handle, COMM_COMM_STATUS_DP_H, IDENTIFY_OPERATION, req->l2cap-3, req->dat);
    } else {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }
}

int app_identifyRead(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;
    u8 realLen = 0;
    memset(my_identifyTime, 0, sizeof(my_identifyTime));
    zbd_commissIdentifyTimeRead(connHandle, req->handle, my_identifyTime, &realLen);

    if (realLen == 0) {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }

    blc_att_setReadRsp_len(realLen);
    return 0;
}

int app_findBindWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

    if ((req->l2cap >= 3) && (req->l2cap <= COMMISSION_COMMAND_LEN_MAX-3)) {
        return zbd_commissionCharaWriteHandle(connHandle, req->handle, COMM_COMM_STATUS_DP_H, FINDING_BINDING_DOMAIN, req->l2cap-3, req->dat);
    } else {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }
}

void app_zbd_commissioningResultNotify(u8 status)
{
    zbd_commissionRetNotify(g_bleSlaveConnHandle, COMM_COMM_STATUS_DP_H, status);
    return;
}

////////////////////////// zigbee direct tunnel  /////////////////////////////////////////////////
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
static const u8 my_zbdTunnelServiceUUID[16]   = {ZBD_TUNNEL_UUID_SERVICE};
static const u8 my_zbdTunnelCharUUID[16]      = {CHARACTERISTIC_UUID_ZDTS_NPDU};
u8 my_zbdTunnelName[2] = {0};
const u16 my_zbdTunnelSemUUID                   = GATT_UUID_CLIENT_CHAR_CFG;
u8  my_zbdTunnelData[76] = {0};

static const u8 my_tunnelCharVal[19] = {
        CHAR_PROP_WRITE | CHAR_PROP_INDICATE,
        U16_LO(ZBD_TUNNEL_NPDU_DP_H), U16_HI(ZBD_TUNNEL_NPDU_DP_H),
        CHARACTERISTIC_UUID_ZDTS_NPDU
};

int app_zbdTunnelWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

    memcpy(my_zbdTunnelData, req->dat, req->l2cap-3);

    return zbd_tunnelNpduWriteHandle(connHandle, ZBD_TUNNEL_NPDU_DP_H, req->l2cap-3, req->dat);
}
#endif


static const attribute_t my_Attributes[] = {

    {ATT_END_H - 1, 0 , 0, 0, NULL, NULL, NULL, NULL},  // total num of attribute

    // 0001 - 0007  gap
    {7,ATT_PERMISSIONS_READ,2,2,(u8*)(size_t)(&my_primaryServiceUUID),  (u8*)(size_t)(&my_gapServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_devNameCharVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(my_devNameCharVal), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_devName), (u8*)(size_t)(&my_devNameUUID), (u8*)(size_t)(my_devName), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_appearanceCharVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(my_appearanceCharVal), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof (my_appearance), (u8*)(size_t)(&my_appearanceUUID),    (u8*)(size_t)(&my_appearance), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_periConnParamCharVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(my_periConnParamCharVal), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof (my_periConnParameters),(u8*)(size_t)(&my_periConnParamUUID),  (u8*)(size_t)(&my_periConnParameters), 0, 0},


    // 0008 - 000b gatt
    {4,ATT_PERMISSIONS_READ,2,2,(u8*)(size_t)(&my_primaryServiceUUID),  (u8*)(size_t)(&my_gattServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_serviceChangeCharVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(my_serviceChangeCharVal), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof (serviceChangeVal), (u8*)(size_t)(&serviceChangeUUID), (u8*)(&serviceChangeVal), 0, 0},
    {0,ATT_PERMISSIONS_RDWR,2,sizeof (serviceChangeCCC),(u8*)(size_t)(&clientCharacterCfgUUID), (u8*)(serviceChangeCCC), 0, 0},


    // 000c - 000e  device Information Service
    {3,ATT_PERMISSIONS_READ,2,2,(u8*)(size_t)(&my_primaryServiceUUID),  (u8*)(size_t)(&my_devServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_PnCharVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(my_PnCharVal), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_PnPtrs),(u8*)(size_t)(&my_PnPUUID), (u8*)(size_t)(my_PnPtrs), 0, 0},

    ////////////////////////////////////// Battery Service /////////////////////////////////////////////////////
    // 000f - 0012
    {4,ATT_PERMISSIONS_READ,2,2,(u8*)(size_t)(&my_primaryServiceUUID),  (u8*)(size_t)(&my_batServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_batCharVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(my_batCharVal), 0, 0},              //prop
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_batVal),(u8*)(size_t)(&my_batCharUUID),     (u8*)(my_batVal), 0, 0},    //value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(batteryValueInCCC),(u8*)(size_t)(&clientCharacterCfgUUID),     (u8*)(batteryValueInCCC), 0, 0},    //value

    ////////////////////////////////////// Zigbee direct security service /////////////////////////////////////////////////////
    //0x13-0x19
    {7,ATT_PERMISSIONS_READ, 2,  16, (u8*)(size_t)(&my_primaryServiceUUID),  (u8*)(size_t)(&my_zbdSecurityServiceUUID), 0,  0},
    {0,ATT_PERMISSIONS_READ, 2, sizeof(my_ss25519Val), (u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_ss25519Val),  0,  0},              //
    {0,ATT_PERMISSIONS_WRITE, 16, sizeof(my_zbdSecirutyData), (u8*)(size_t)(&my_zbdSecurityCharUUID25519), (u8*)(&my_zbdSecirutyData), &app_zbdSecurityWrite, 0},   //value
    {0,ATT_PERMISSIONS_RDWR, 2,  sizeof (zbdSec25519ValueCCC),  (u8*)(size_t)(&my_zbdSecuritycfgUUID),  (u8*)(zbdSec25519ValueCCC), 0, 0},

    {0,ATT_PERMISSIONS_READ, 2,  sizeof(my_ss256Val), (u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_ss256Val), 0, 0},                //
    {0,ATT_PERMISSIONS_WRITE, 16, sizeof(my_zbdSecirutyData), (u8*)(size_t)(&my_zbdSecurityCharUUIDP256), (u8*)(&my_zbdSecirutyData), &app_zbdSecurityWrite, 0},    //value
    {0,ATT_PERMISSIONS_RDWR, 2,  sizeof (zbdSec256ValueCCC),  (u8*)(size_t)(&my_zbdSecuritycfgUUID),  (u8*)(zbdSec256ValueCCC), 0, 0},

    ////////////////////////////////////// Commissioning Service /////////////////////////////////////////////////////
    // 001a - 002b
    {18,ATT_PERMISSIONS_READ,2,2,(u8*)(size_t)(&my_primaryServiceUUID), (u8*)(size_t)(&my_commServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_formVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_formVal), 0, 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(size_t)(&my_formNetworkCharUUID), (u8*)(&my_NetworkData), &app_formNetworkWrite, 0},   //value

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_joinVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_joinVal), 0, 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(size_t)(&my_joinNetworkCharUUID), (u8*)(&my_NetworkData), &app_joinNetworkWrite, 0},   //value

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_permitJoinVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_permitJoinVal), 0, 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(size_t)(&my_permitJoinCharUUID), (u8*)(&my_NetworkData), &app_permitJoinWrite, 0},  //value

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_leaveVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_leaveVal), 0, 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(size_t)(&my_leaveNetworkCharUUID), (u8*)(&my_NetworkData), &app_leaveNetworkWrite, 0}, //value

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_connstatusVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_connstatusVal), 0, 0},               //prop
    {0,ATT_PERMISSIONS_READ,16,sizeof(my_commissStatus),(u8*)(size_t)(&my_commissioningStatusCharUUID), (u8*)(my_commissStatus), 0, &app_commissStatusRead},    //value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(my_commissChangeCCC),(u8*)(size_t)(&my_commissioningStatusCfgUUID), (u8*)(my_commissChangeCCC), 0, 0}, //value

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_manajoinVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_manajoinVal), 0, 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(size_t)(&my_manageJoinersCharUUID), (u8*)(&my_NetworkData), &app_manageJoinersWrite, 0},   //value

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_identifyVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_identifyVal), 0, 0},                //prop
    {0,ATT_PERMISSIONS_RDWR,16,sizeof(my_identifyTime),(u8*)(size_t)(&my_identifyCharUUID), (u8*)(&my_identifyTime), &app_identifyWrite, &app_identifyRead}, //value

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_findbindVal),(u8*)(&my_characterUUID),       (u8*)(&my_findbindVal), 0, 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(size_t)(&my_findBindCharUUID), (u8*)(&my_NetworkData), &app_findBindWrite, 0}, //value

    ////////////////////////////////////// Zigbee direct tunnel service /////////////////////////////////////////////////////
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
    //0x2c-0x2f
    {4,ATT_PERMISSIONS_READ, 2,  16,                         (u8*)(&my_primaryServiceUUID),  (u8*)(&my_zbdTunnelServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ, 2,  sizeof(my_tunnelCharVal), (u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(&my_tunnelCharVal), 0, 0},             //
    {0,ATT_PERMISSIONS_WRITE, 16, sizeof(my_zbdTunnelData), (u8*)(&my_zbdTunnelCharUUID), (u8*)(&my_zbdTunnelData), &app_zbdTunnelWrite, 0}, //value
    {0,ATT_PERMISSIONS_RDWR, 2,  sizeof(my_zbdTunnelName),  (u8*)(&my_zbdTunnelSemUUID),  (u8*)(my_zbdTunnelName), 0, 0},
#endif
};


void app_zbd_advDataSet(void)
{
#ifdef ZCL_ZBD_CONFIG
    zcl_zbdAttr_t *pState = zcl_zbdAttrGet();
    if (!pState->interfaceState) {
        blc_ll_setAdvData(tbl_advData, sizeof(tbl_advData));
        return;
    }else
#endif
    {
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
        if (nwk_neTblLinkTypeNodeCount() >= DIRECT_CONNECTION_MAX) {
            blc_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
        } else
#endif
        {
            u8 advDataExt[31] = {0};
            u8 advLen = sizeof(tbl_advData);
            memcpy(advDataExt, tbl_advData, advLen);
            u8 supportTunnel = 0;
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
            supportTunnel = 1;
#endif
            zbd_extendAdvData(advDataExt, (u8*)&(advLen), supportTunnel);
            blc_ll_setAdvData(advDataExt, advLen);
        }
    }
    return;
}

int app_zbd_advDataUpdateHandle(void)
{
    app_zbd_advDataSet();
    return 0;
}

int app_zbd_openProviTimeoutCb(void)
{
    blc_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );

    return 0;
}

int app_zbd_zvdBleDisconnect(u8 ret)
{
	printf("app_zbd_zvdBleDisconnect,ret:%d\n", ret);
    if (ret) {
        blc_ll_disconnect(g_bleSlaveConnHandle, HCI_ERR_OP_CANCELLED_BY_HOST);
    } 
    return 0;
}

int app_session_build_ret_cb(ZBD_SESS_TYPE_e session, u8 ret)
{
//  printf("app_session_build_ret_cb, session:%d, ret:%d\n",session, ret);
    if (ret) {
        blc_ll_disconnect(g_bleSlaveConnHandle, HCI_ERR_OP_CANCELLED_BY_HOST);
    }
    return 0;
}

/**
 * @brief   GATT initialization.
 *          !!!Note: this function is used to register ATT table to BLE Stack.
 * @param   none.
 * @return  none.
 */
void my_gatt_init (void)
{
    bls_att_setAttributeTable ((u8 *)(size_t)my_Attributes);
}


void zbd_ble_loopHandle(void)
{
    if (g_bleSlaveConnHandle) {
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
        zbd_tunnelPendingDataSend(g_bleSlaveConnHandle);
#endif
        zbd_ss_transTaskProc();
    }
    zbd_zbNwkParaChangeDetect();
}

#ifdef ZCL_ZBD_CONFIG
static void sampleLight_zbdMatchDescRspPush(void *arg){
    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
    zdo_match_descriptor_resp_t *rsp = (zdo_match_descriptor_resp_t *)p->zpdu;

    if ((rsp->status == ZDO_SUCCESS)&&(rsp->matchLen > 0)) {
        set_device_zdTcAware(1);
    }
    return;
}


/*********************************************************************
 * @fn      sampleLight_checkZbdAware
 *
 * @brief   Discover the zigbee Direct configuration client cluster
 *
 * @param
 *
 * @return  None
 */
void sampleLight_checkZbdAware(void){
    if (ss_securityModeIsDistributed() || !zb_isDeviceJoinedNwk()) {
        return;
    }

    zdo_match_descriptor_req_t req;
    memset(&req, 0, sizeof(zdo_match_descriptor_req_t));
    req.profile_id = HA_PROFILE_ID;
    req.nwk_addr_interest = 0x0000;
    req.num_in_clusters = 0;
    req.num_out_clusters = 1;
    req.cluster_list[0] = ZCL_CLUSTER_GEN_ZBD_CONFIG;
    u8 sn = 0;
    zb_zdoMatchDescReq(0x0000, &req, &sn, sampleLight_zbdMatchDescRspPush);
}
#endif

void zbd_bdbInitCb(u8 status, u8 joinedNetwork)
{
    if (status == BDB_INIT_STATUS_SUCCESS) {
        if (joinedNetwork) {
            if (zbd_sessionTypeGet() == ZBD_SESS_TYPE_NONE) {
                zdb_sessionStateSet(ZBD_OPEN_TO_CONNECT_ZVD);
            }
#ifdef ZCL_ZBD_CONFIG
            extern void sampleLightAnJoinTimerStart(void);
            sampleLight_checkZbdAware();
            sampleLightAnJoinTimerStart();
#endif
        } else {
            zdb_sessionStateSet(ZBD_OPEN_TO_BE_PROVISIONED);
        }
    }
}

void zbd_bdbcommissioningCb(u8 status)
{
    /*report the commissioning result,send ble notify*/
    zbd_commissionRetNotify(g_bleSlaveConnHandle, COMM_COMM_STATUS_DP_H, status);

    if (status == BDB_COMMISSION_STA_SUCCESS) {
        if (zbd_sessionTypeGet() == ZBD_SESS_TYPE_NONE) {
            zdb_sessionStateSet(ZBD_OPEN_TO_CONNECT_ZVD);
        }
#ifdef ZCL_ZBD_CONFIG
        extern void sampleLightAnJoinTimerStart(void);
        sampleLight_checkZbdAware();
        sampleLightAnJoinTimerStart();
#endif
    }
}
/**
 * @brief      BLE Adv report event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int AA_dbg_adv_rpt = 0;
u32 tick_adv_rpt = 0;
u8 whiteListAddr[] = {0xbb, 0xbb, 0xbb, 0x38, 0xc1, 0xa4};
int app_le_adv_report_event_handle(u8 *p)
{
    event_adv_report_t *pa = (event_adv_report_t *)p;
    s8 rssi = pa->data[pa->len];

    #if 0  //debug, print ADV report number every 5 seconds
        AA_dbg_adv_rpt ++;
        if (clock_time_exceed(tick_adv_rpt, 5000000)) {
            tlkapi_send_string_data(APP_LOG_EN, "[APP][EVT] Adv report", pa->mac, 6);
            tick_adv_rpt = clock_time();
        }
    #endif

    /*********************** Central Create connection demo: Key press or ADV pair packet triggers pair  ********************/
    #if (ACL_CENTRAL_SMP_ENABLE)
        if (central_smp_pending) {     //if previous connection SMP not finish, can not create a new connection
            return 1;
        }
    #endif


    int central_auto_connect = 0;
    int user_manual_pairing = 0;

    user_manual_pairing = (!memcmp(whiteListAddr, pa->mac, 6)) && (rssi > -66);  //RSSI threshold, short distance

    #if (ACL_CENTRAL_SMP_ENABLE)
        central_auto_connect = blc_smp_searchBondingPeripheralDevice_by_PeerMacAddress(pa->adr_type, pa->mac);
    #endif

    if (central_auto_connect || user_manual_pairing) {

        /* send create connection command to Controller, trigger it switch to initiating state. After this command, Controller
         * will scan all the ADV packets it received but not report to host, to find the specified device(mac_adr_type & mac_adr),
         * then send a "CONN_REQ" packet, enter to connection state and send a connection complete event
         * (HCI_SUB_EVT_LE_CONNECTION_COMPLETE) to Host
         * */
        u8 status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_WINDOW_100MS, INITIATE_FP_ADV_SPECIFY,  \
                                 pa->adr_type, pa->mac, OWN_ADDRESS_PUBLIC, \
                                 CONN_INTERVAL_31P25MS, CONN_INTERVAL_48P75MS, 0, CONN_TIMEOUT_4S, \
                                 0, 0xFFFF);


        if (status == BLE_SUCCESS) { //create connection success
            tlkapi_send_string_data(APP_LOG_EN, "[APP][CMD] create connection success", pa->mac, 6);
        }
    }
    /*********************** Central Create connection demo code end  *******************************************************/
    return 0;
}

static s32 app_bleIntervalChange(void *arg)
{
    bls_l2cap_requestConnParamUpdate (g_bleSlaveConnHandle, g_appBleInterval, g_appBleInterval, g_appBleLatency, CONN_TIMEOUT_4S);  // 1 S

    return -1;
}

/*Negotiate the connection interval after 10 seconds to increase the probability of successful negotiation*/
void app_bleConnIntervalSet(u8 interval, u16 latency)
{
    g_appBleInterval = interval;
    g_appBleLatency  = latency;

    TL_ZB_TIMER_SCHEDULE(app_bleIntervalChange, NULL, 10*1000); 
}

/**
 * @brief      BLE Connection complete event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_le_connection_complete_event_handle(u8 *p)
{
    hci_le_connectionCompleteEvt_t *pConnEvt = (hci_le_connectionCompleteEvt_t *)p;

    if (pConnEvt->status == BLE_SUCCESS) {

        dev_char_info_insert_by_conn_event(pConnEvt);

        if (pConnEvt->role == ACL_ROLE_CENTRAL) { // central role, process SMP and SDP if necessary
            #if (ACL_CENTRAL_SMP_ENABLE)
                central_smp_pending = pConnEvt->connHandle; // this connection need SMP
            #endif

        } else {
        	zbd_ss_init(app_zbd_ssSemNotifyX);
    #if ZIGBEE_DIRECT_TUNNEL_ENABLE
            zbd_tunnelInit(ZBD_TUNNEL_NPDU_DP_H);
    #endif

            g_bleSlaveConnHandle = pConnEvt->connHandle;
            bls_l2cap_requestConnParamUpdate(pConnEvt->connHandle, g_appBleInterval, g_appBleInterval, g_appBleLatency, CONN_TIMEOUT_4S);        // 1 second
            app_bleConnIntervalSet(g_appBleInterval, g_appBleLatency);
        }
    }

    return 0;
}

/**
 * @brief      BLE Disconnection event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_disconnect_event_handle(u8 *p)
{
    hci_disconnectionCompleteEvt_t  *pDisConn = (hci_disconnectionCompleteEvt_t *)p;

    tlkapi_send_string_data(APP_LOG_EN, "[APP][EVT] disconnect event", &pDisConn->connHandle, 3);

    g_bleSlaveConnHandle = 0;

    if (pDisConn->connHandle == g_bleSlaveConnHandle) {
        zbd_sessionExit();
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
        zbd_tunnelPendingDataClear();
        zdo_nlmeForgetAllTrustLink();
#endif

        g_bleSlaveConnHandle = 0;
    }

    //terminate reason
    if (pDisConn->reason == HCI_ERR_CONN_TIMEOUT) {   //connection timeout

    } else if (pDisConn->reason == HCI_ERR_REMOTE_USER_TERM_CONN) {     //peer device send terminate command on link layer

    } else if (pDisConn->reason == HCI_ERR_CONN_TERM_BY_LOCAL_HOST) {

    } else {

    }


    /* if previous connection SMP & SDP not finished, clear flag */
    #if (ACL_CENTRAL_SMP_ENABLE)
        if (central_smp_pending == pDisConn->connHandle) {
            central_smp_pending = 0;
        }
    #endif

    dev_char_info_delete_by_connhandle(pDisConn->connHandle);
    return 0;
}

/**
 * @brief      BLE Connection update complete event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_le_connection_update_complete_event_handle(u8 *p)
{
    hci_le_connectionUpdateCompleteEvt_t *pUpt = (hci_le_connectionUpdateCompleteEvt_t *)p;
    tlkapi_send_string_data(APP_LOG_EN, "[APP][EVT] Connection Update Event", &pUpt->connHandle, 8);

    if (pUpt->status == BLE_SUCCESS) {

    }

    return 0;
}

//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
/**
 * @brief      BLE controller event handler call-back.
 * @param[in]  h       event type
 * @param[in]  p       Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_controller_event_callback (u32 h, u8 *p, int n)
{
    (void)n; //unused, remove warning

    if (h & HCI_FLAG_EVENT_BT_STD) {       //Controller HCI event
        u8 evtCode = h & 0xff;

        //------------ disconnect -------------------------------------
        if (evtCode == HCI_EVT_DISCONNECTION_COMPLETE) {  //connection terminate
            app_disconnect_event_handle(p);
        } else if (evtCode == HCI_EVT_LE_META) { //LE Event
            u8 subEvt_code = p[0];

            //------hci le event: le connection complete event---------------------------------
            if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_COMPLETE) { // connection complete

                app_le_connection_complete_event_handle(p);
            } else if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT) { // ADV packet
                //after controller is set to scan state, it will report all the adv packet it received by this event

                app_le_adv_report_event_handle(p);
            } else if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE)  { // connection update
            //------hci le event: le connection update complete event-------------------------------
                app_le_connection_update_complete_event_handle(p);
            }
        }
    }

    return 0;
}


/**
 * @brief      BLE host event handler call-back.
 * @param[in]  h       event type
 * @param[in]  para    Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_host_event_callback (u32 h, u8 *para, int n)
{
    (void)n; //unused, remove warning

    u8 event = h & 0xFF;

    switch(event) {
        case GAP_EVT_SMP_PAIRING_BEGIN:
        {

        }
        break;

        case GAP_EVT_SMP_PAIRING_SUCCESS:
        {

        }
        break;

        case GAP_EVT_SMP_PAIRING_FAIL:
        {
            #if (ACL_CENTRAL_SMP_ENABLE)
                gap_smp_pairingFailEvt_t *pEvt = (gap_smp_pairingFailEvt_t *)para;

                if (dev_char_get_conn_role_by_connhandle(pEvt->connHandle) == ACL_ROLE_CENTRAL) {
                    if (central_smp_pending == pEvt->connHandle) {
                        central_smp_pending = 0;
                        tlkapi_send_string_data(APP_LOG_EN, "[APP][SMP] paring fail", &pEvt->connHandle, sizeof(gap_smp_pairingFailEvt_t));
                    }
                }
            #endif
        }
        break;

        case GAP_EVT_SMP_CONN_ENCRYPTION_DONE:
        {
            gap_smp_connEncDoneEvt_t *pEvt = (gap_smp_connEncDoneEvt_t *)para;
            tlkapi_send_string_data(APP_LOG_EN, "[APP][SMP] Connection encryption done event", &pEvt->connHandle, sizeof(gap_smp_connEncDoneEvt_t));
        }
        break;

        case GAP_EVT_SMP_SECURITY_PROCESS_DONE:
        {
            gap_smp_connEncDoneEvt_t* pEvt = (gap_smp_connEncDoneEvt_t*)para;
            tlkapi_send_string_data(APP_LOG_EN, "[APP][SMP] Security process done event", &pEvt->connHandle, sizeof(gap_smp_connEncDoneEvt_t));

            if (dev_char_get_conn_role_by_connhandle(pEvt->connHandle) == ACL_ROLE_CENTRAL) {

                #if (ACL_CENTRAL_SMP_ENABLE)
                    if (dev_char_get_conn_role_by_connhandle(pEvt->connHandle) == ACL_ROLE_CENTRAL) {
                        if (central_smp_pending == pEvt->connHandle) {
                            central_smp_pending = 0;
                        }
                    }
                #endif

            }else{
                g_bleSlaveConnHandle = pEvt->connHandle;
            }
        }
        break;

        case GAP_EVT_SMP_TK_DISPLAY:
        {

        }
        break;

        case GAP_EVT_SMP_TK_REQUEST_PASSKEY:
        {

        }
        break;

        case GAP_EVT_SMP_TK_REQUEST_OOB:
        {

        }
        break;

        case GAP_EVT_SMP_TK_NUMERIC_COMPARE:
        {

        }
        break;

        case GAP_EVT_ATT_EXCHANGE_MTU:
        {

        }
        break;

        case GAP_EVT_GATT_HANDLE_VALUE_CONFIRM:
        {

        }
        break;

        default:
        break;
    }

    return 0;
}


/**
 * @brief      BLE GATT data handler call-back.
 * @param[in]  connHandle     connection handle.
 * @param[in]  pkt             Pointer point to data packet buffer.
 * @return
 */
int app_gatt_data_handler (u16 connHandle, u8 *pkt)
{
    if (dev_char_get_conn_role_by_connhandle(connHandle) == ACL_ROLE_CENTRAL ) {  //GATT data for Central
        rf_packet_att_t *pAtt = (rf_packet_att_t*)pkt;

        //so any ATT data before service discovery will be dropped
        dev_char_info_t* dev_info = dev_char_info_search_by_connhandle (connHandle);
        if (dev_info) {
            //-------   user process ------------------------------------------------
//          u16 attHandle = pAtt->handle;
            if (pAtt->opcode == ATT_OP_HANDLE_VALUE_NOTI) {

            } else if (pAtt->opcode == ATT_OP_HANDLE_VALUE_IND) {

            }
        }

        if (!(pAtt->opcode & 0x01)) {
            switch (pAtt->opcode) {
                case ATT_OP_FIND_INFO_REQ:
                case ATT_OP_FIND_BY_TYPE_VALUE_REQ:
                case ATT_OP_READ_BY_TYPE_REQ:
                case ATT_OP_READ_BY_GROUP_TYPE_REQ:
                    blc_gatt_pushErrResponse(connHandle, pAtt->opcode, pAtt->handle, ATT_ERR_ATTR_NOT_FOUND);
                    break;
                case ATT_OP_READ_REQ:
                case ATT_OP_READ_BLOB_REQ:
                case ATT_OP_READ_MULTI_REQ:
                case ATT_OP_WRITE_REQ:
                case ATT_OP_PREPARE_WRITE_REQ:
                    blc_gatt_pushErrResponse(connHandle, pAtt->opcode, pAtt->handle, ATT_ERR_INVALID_HANDLE);
                    break;
                case ATT_OP_EXECUTE_WRITE_REQ:
                case ATT_OP_HANDLE_VALUE_CFM:
                case ATT_OP_WRITE_CMD:
                case ATT_OP_SIGNED_WRITE_CMD:
                    //ignore
                    break;
                default://no action
                    break;
            }
        }
    } else {   //GATT data for Peripheral

    }
    return 0;
}

/**
 * @brief       This function is used to initialize the MAC address
 * @param[in]   flash_addr - flash address for MAC address
 * @param[in]   mac_public - public address
 * @param[in]   mac_random_static - random static MAC address
 * @return      none
 */
_attribute_no_inline_ void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static)
{
    flash_sector_mac_address = flash_addr;

    int rand_mac_byte3_4_read_OK = 0;
    u8  mac_read[8];
    flash_read_page(flash_addr, 8, mac_read);

    u8 value_rand[5];
    generateRandomNum(5, value_rand);

    u8 ff_six_byte[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    if (memcmp(mac_read, ff_six_byte, 6)) { //read MAC address on flash success
        memcpy(mac_public, mac_read, 6);    //copy public address from flash

        if (mac_read[6] != 0xFF && mac_read[7] != 0xFF) {
            mac_random_static[3]     = mac_read[6];
            mac_random_static[4]     = mac_read[7];
            rand_mac_byte3_4_read_OK = 1;
        }
    } else {                                       //no MAC address on flash

#if (BUILT_IN_MAC_ON_DEVICE)
        if (get_device_mac_address(mac_read, 8)) { //read device MAC address
            memcpy(mac_public, mac_read, 6);       //copy public address from device

            mac_random_static[3]     = mac_read[6];
            mac_random_static[4]     = mac_read[7];
            rand_mac_byte3_4_read_OK = 1;
        } else
#endif
        {
            mac_public[0] = value_rand[0];
            mac_public[1] = value_rand[1];
            mac_public[2] = value_rand[2];

            /* company id */
            mac_public[3] = U32_BYTE0(PDA_COMPANY_ID);
            mac_public[4] = U32_BYTE1(PDA_COMPANY_ID);
            mac_public[5] = U32_BYTE2(PDA_COMPANY_ID);

            flash_write_page(flash_addr, 6, mac_public); //store public address on flash for future use
        }
    }


    mac_random_static[0] = mac_public[0];
    mac_random_static[1] = mac_public[1];
    mac_random_static[2] = mac_public[2];
    mac_random_static[5] = 0xC0; //for random static

    if (!rand_mac_byte3_4_read_OK) {
        mac_random_static[3] = value_rand[3];
        mac_random_static[4] = value_rand[4];

        flash_write_page(flash_addr + 6, 2, (u8 *)(mac_random_static + 3)); //store random address on flash for future use
    }
}

void blc_flash_read_mid_get_vendor_set_capacity(void)
{
    /* attention: tlk_flash_mid/tlk_flash_vendor/tlk_flash_capacity will be used by application and stack later
     * so do not change code here */
#if defined(MCU_CORE_TL721X)
    tlk_flash_mid = flash_read_mid_with_device_num(SLAVE0);
#else
    tlk_flash_mid = flash_read_mid();
#endif
    tlk_flash_vendor = flash_get_vendor(tlk_flash_mid);
    tlk_flash_capacity = ((tlk_flash_mid & 0x00ff0000)>>16);
}


/** newadd
 * @brief       user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]   none
 * @return      none
 */
void user_ble_init(bool isRetention)
{
    (void)isRetention;
//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
    u8  mac_public[6];
    u8  mac_random_static[6];

    blc_initMacAddress(CFG_MAC_ADDRESS, mac_public, mac_random_static);

    blc_flash_read_mid_get_vendor_set_capacity();
    //////////// LinkLayer Initialization  Begin /////////////////////////
    blc_ll_initBasicMCU();

    blc_ll_initStandby_module(mac_public);

    blc_ll_setRandomAddr(mac_random_static);

    blc_ll_initLegacyAdvertising_module();

#if ACL_CENTRAL_MAX_NUM
    blc_ll_initLegacyScanning_module();
#endif

    blc_ll_initLegacyInitiating_module();

    blc_ll_initAclConnection_module();
#if ACL_CENTRAL_MAX_NUM
    blc_ll_initAclCentralRole_module();
#endif

    blc_ll_initAclPeriphrRole_module();

    blc_ll_setMaxConnectionNumber(ACL_CENTRAL_MAX_NUM, ACL_PERIPHR_MAX_NUM);

    blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS, ACL_PERIPHR_MAX_TX_OCTETS);

    /* all ACL connection share same RX FIFO */
    blc_ll_initAclConnRxFifo(app_acl_rx_fifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);
#if ACL_CENTRAL_MAX_NUM
    /* ACL Central TX FIFO */
    blc_ll_initAclCentralTxFifo(app_acl_cen_tx_fifo, ACL_CENTRAL_TX_FIFO_SIZE, ACL_CENTRAL_TX_FIFO_NUM, ACL_CENTRAL_MAX_NUM);
#endif
    /* ACL Peripheral TX FIFO */
    blc_ll_initAclPeriphrTxFifo(app_acl_per_tx_fifo, ACL_PERIPHR_TX_FIFO_SIZE, ACL_PERIPHR_TX_FIFO_NUM, ACL_PERIPHR_MAX_NUM);

    blc_ll_setAclCentralBaseConnectionInterval(CONN_INTERVAL_31P25MS);
    //////////// LinkLayer Initialization  End /////////////////////////

    //////////// HCI Initialization  Begin /////////////////////////
    blc_hci_registerControllerDataHandler (blc_l2cap_pktHandler);

    blc_hci_registerControllerEventHandler(app_controller_event_callback); //controller hci event to host all processed in this func

    //bluetooth event
    blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE);

    //bluetooth low energy(LE) event
    blc_hci_le_setEventMask_cmd(        HCI_LE_EVT_MASK_CONNECTION_COMPLETE  \
                                    |   HCI_LE_EVT_MASK_ADVERTISING_REPORT \
                                    |   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE);

    //////////// HCI Initialization  End /////////////////////////

    //////////// Host Initialization  Begin /////////////////////////
    /* Host Initialization */
    /* GAP initialization must be done before any other host feature initialization !!! */
    blc_gap_init();

    /* L2CAP data buffer Initialization */
#if ACL_CENTRAL_MAX_NUM
    blc_l2cap_initAclCentralBuffer(app_cen_l2cap_rx_buf, CENTRAL_L2CAP_BUFF_SIZE, NULL, 0);
#endif
    blc_l2cap_initAclPeripheralBuffer(app_per_l2cap_rx_buf, PERIPHR_L2CAP_BUFF_SIZE, app_per_l2cap_tx_buf, PERIPHR_L2CAP_BUFF_SIZE);

    blc_att_setCentralRxMtuSize(CENTRAL_ATT_RX_MTU); ///must be placed after "blc_gap_init"
    blc_att_setPeripheralRxMtuSize(PERIPHR_ATT_RX_MTU);   ///must be placed after "blc_gap_init"

    /* GATT Initialization */
    my_gatt_init();
    blc_gatt_register_data_handler(app_gatt_data_handler);

    blc_att_enableReadReqReject(1);
    blc_att_enableWriteReqReject(1);
    
    /* SMP Initialization */
    blc_smp_configPairingSecurityInfoStorageAddressAndSize(FLASH_SMP_PAIRING_ADDR, FLASH_SMP_PAIRING_MAX_SIZE);

    #if (ACL_PERIPHR_SMP_ENABLE)  //Peripheral SMP Enable
        blc_smp_setSecurityLevel_periphr(Unauthenticated_Pairing_with_Encryption);  //LE_Security_Mode_1_Level_2
    #else
        blc_smp_setSecurityLevel_periphr(No_Security);
    #endif

    #if (ACL_CENTRAL_SMP_ENABLE)
        blc_smp_setSecurityLevel_central(Unauthenticated_Pairing_with_Encryption);  //LE_Security_Mode_1_Level_2
    #else
        blc_smp_setSecurityLevel_central(No_Security);
    #endif

    blc_smp_smpParamInit();

    //host(GAP/SMP/GATT/ATT) event process: register host event callback and set event mask
    blc_gap_registerHostEventHandler( app_host_event_callback );
    blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_BEGIN            |  \
                          GAP_EVT_MASK_SMP_PAIRING_SUCCESS          |  \
                          GAP_EVT_MASK_SMP_PAIRING_FAIL             |  \
                          GAP_EVT_MASK_SMP_SECURITY_PROCESS_DONE);
    //////////// Host Initialization  End /////////////////////////

    /* Check if any Stack(Controller & Host) Initialization error after all BLE initialization done.
     * attention: user can not delete !!! 
     */
    u32 error_code1 = blc_contr_checkControllerInitialization();
    u32 error_code2 = blc_host_checkHostInitialization();
    if(error_code1 != INIT_SUCCESS || error_code2 != INIT_SUCCESS){
        /* It's recommended that user set some UI alarm to know the exact error, e.g. LED shine, print log */
        printf("[APP][INI] Stack INIT ERROR 0x%04x, 0x%04x", error_code1, error_code2);
        while(1);
    }

//////////////////////////// BLE stack Initialization  End //////////////////////////////////

//////////////////////////// User Configuration for BLE application ////////////////////////////
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
    zdo_nlmeForgetAllTrustLink();
#endif
    app_zbd_advDataSet();
    blc_ll_setScanRspData(tbl_scanRsp, sizeof(tbl_scanRsp));
    blc_ll_setAdvParam(ADV_INTERVAL_200MS, ADV_INTERVAL_200MS, ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, 0, NULL, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
    blc_ll_setAdvEnable(BLC_ADV_ENABLE);  //ADV enable

#if ACL_CENTRAL_MAX_NUM
    blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_WINDOW_50MS, OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
    blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);
#endif

#if PA_ENABLE
    g_ble_txPowerSet = RF_POWER_P0dBm;
    ble_rf_pa_init(0, PA_TX, PA_RX);    //for ble
#endif

    rf_set_power_level_index (g_ble_txPowerSet);

#if (BLE_LATENCY_WORK_MODE)
    blc_ll_initPowerManagement_module();
    blc_pm_setSleepMask(PM_SLEEP_LEG_ADV | PM_SLEEP_LEG_SCAN | PM_SLEEP_ACL_PERIPHR | PM_SLEEP_ACL_CENTRAL);
    bls_pm_disableHardwarePm(1);
#endif

    zbd_commissionInit(app_zbd_commStatusHandle, app_zbd_advDataUpdateHandle);
    zbd_sessionInit(app_zbd_openProviTimeoutCb, app_session_build_ret_cb, (u8 *)(size_t)my_Attributes);
#if ZIGBEE_DIRECT_TUNNEL_ENABLE
    aps_trustLinkJoinRet(app_zbd_zvdBleDisconnect);
#endif

    tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] acl connection demo init", 0, 0);
}

int blt_sdk_main_loop(void)
{
    ////////////////////////////////////// BLE entry /////////////////////////////////
    blc_sdk_main_loop();

    return 0; //must return 0 due to SDP flow
}
