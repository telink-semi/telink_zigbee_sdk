/********************************************************************************************************
 * @file    sampleSwitchBLESlave_b91.c
 *
 * @brief   This is the source file for sampleSwitchBLESlave_b91
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

#include "tl_common.h"
#include "ble.h"
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
#include "zbd_include.h"

/**
 * @brief   connMaxRxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 */
#define ACL_CONN_MAX_RX_OCTETS          60

/**
 * @brief   connMaxTxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. connMaxTxOctets should be in range of 27 ~ 251
 */
#define ACL_CONN_MAX_TX_OCTETS          27


#define ACL_TX_FIFO_SIZE                48  // ACL_CONN_MAX_TX_OCTETS + 10, then 16 Byte align
#define ACL_TX_FIFO_NUM                 17  // must be: (2^n) + 1

#define ACL_RX_FIFO_SIZE                96  // ACL_CONN_MAX_RX_OCTETS + 21, then 16 Byte align
#define ACL_RX_FIFO_NUM                 8   // must be: 2^n


#define MY_APP_ADV_CHANNEL              BLT_ENABLE_ADV_ALL
#define MY_ADV_INTERVAL_MIN             ADV_INTERVAL_200MS
#define MY_ADV_INTERVAL_MAX             ADV_INTERVAL_205MS

#define ADV_IDLE_ENTER_DEEP_TIME        120  //s
#define CONN_IDLE_ENTER_DEEP_TIME       120  //s

#define MY_DIRECT_ADV_TIME              2000000

#define BLE_DEVICE_ADDRESS_TYPE         BLE_DEVICE_ADDRESS_PUBLIC

#define MTU_SIZE_SETTING                DIRECT_ATT_MTU_RECOMMENDED

static u8  g_appBleInterval = CONN_INTERVAL_50MS;
static u16 g_appBleLatency = 19;

_attribute_data_retention_  own_addr_type_t     app_own_address_type = OWN_ADDRESS_PUBLIC;

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ********************************/
_attribute_data_retention_  u8  app_acl_rxfifo[ACL_RX_FIFO_SIZE * ACL_RX_FIFO_NUM] = {0};
_attribute_data_retention_  u8  app_acl_txfifo[ACL_TX_FIFO_SIZE * ACL_TX_FIFO_NUM] = {0};
/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/

u8 g_bleConnDoing = 0;
_attribute_data_retention_  int device_in_connection_state;
_attribute_data_retention_  u32 advertise_begin_tick;
_attribute_data_retention_  u8  sendTerminate_before_enterDeep = 0;
_attribute_data_retention_  u32 latest_user_event_tick;
_attribute_data_retention_  u8 button_detect_en;
u8 g_ble_txPowerSet = RF_POWER_INDEX_P3p25dBm;

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

    // zigbee direct security
    ZBD_SS_PS_H,
    ZBD_SS_25519_CD_H,
    ZBD_SS_25519_DP_H,
    ZBD_SS_25519_DESC_H,

    ZBD_SS_P256_CD_H,
    ZBD_SS_P256_DP_H,
    ZBD_SS_P256_DESC_H,

    //// commissioning service:23-34////
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

    COMM_IDENTIFY_CD_H,             //UUID: 2803,   VALUE:              Prop: Write
    COMM_IDENTIFY_DP_H,             //UUID: 7072377D-0007-421C-B163-491C27333A61    VALUE: struct leaveNetwork

    COMM_FIND_BIND_CD_H,                //UUID: 2803,   VALUE:              Prop: Write
    COMM_FIND_BIND_DP_H,                //UUID: 7072377D-0008-421C-B163-491C27333A61    VALUE: struct finding&binding

    ATT_END_H,

} ATT_HANDLE;


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

/*
 * various
 *
 * */
u8 my_devName[] = {'T','L','_','R','C','U'};
//////////////////////////////////////////////////////////////////////////////
//   Adv Packet, Response Packet
//////////////////////////////////////////////////////////////////////////////
u8 tbl_advData[] = {
     0x07, 0x09, 'T','L','_','R','C','U',
     0x02, 0x01, 0x05,                          // BLE limited discoverable mode and BR/EDR not supported
     0x03, 0x02, 0xF7, 0xFF,                    // commissioning service UUID
};

u8 tbl_scanRsp [] = {
    0x07, 0x09, 'T','L','_','R','C','U',
};

const u8 my_PnPtrs [] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};

const u16 clientCharacterCfgUUID = GATT_UUID_CLIENT_CHAR_CFG;
const u16 characterPresentFormatUUID = GATT_UUID_CHAR_PRESENT_FORMAT;
const u16 my_primaryServiceUUID = GATT_UUID_PRIMARY_SERVICE;
static const u16 my_characterUUID = GATT_UUID_CHARACTER;
const u16 my_devServiceUUID = SERVICE_UUID_DEVICE_INFORMATION;
const u16 my_PnPUUID = CHARACTERISTIC_UUID_PNP_ID;
const u16 my_devNameUUID = GATT_UUID_DEVICE_NAME;

//device information
const u16 my_gapServiceUUID = SERVICE_UUID_GENERIC_ACCESS;
// Appearance Characteristic Properties
const u16 my_appearanceUIID = 0x2a01;
const u16 my_periConnParamUUID = 0x2a04;
u16 my_appearance = GAP_APPEARE_UNKNOWN;
gap_periConnectParams_t my_periConnParameters = {20, 40, 0, 1000};


const u16 my_gattServiceUUID = SERVICE_UUID_GENERIC_ATTRIBUTE;
const u16 serviceChangeUIID = GATT_UUID_SERVICE_CHANGE;
u16 serviceChangeVal[2] = {0};
static u8 serviceChangeCCC[2]={0,0};


const u8 PROP_READ = CHAR_PROP_READ;
const u8 PROP_WRITE = CHAR_PROP_WRITE;
const u8 PROP_INDICATE = CHAR_PROP_INDICATE;
const u8 PROP_WRITE_NORSP = CHAR_PROP_WRITE_WITHOUT_RSP;
const u8 PROP_READ_NOTIFY = CHAR_PROP_READ | CHAR_PROP_NOTIFY;
const u8 PROP_READ_WRITE_NORSP = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP;
const u8 PROP_READ_WRITE_WRITENORSP = CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP;
const u8 PROP_READ_WRITE = CHAR_PROP_READ | CHAR_PROP_WRITE;
const u8 PROP_READ_WRITE_NORSP_NOTIFY = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY;
const u8 PROP_WRITE_INDICATE = CHAR_PROP_WRITE | CHAR_PROP_INDICATE;
/*
 * Battery
 * */
const u16 my_batServiceUUID                 = SERVICE_UUID_BATTERY;
const u16 my_batCharUUID                    = CHARACTERISTIC_UUID_BATTERY_LEVEL;
static u8 batteryValueInCCC[2];
u8        my_batVal[1]                      = {99};

////////////////////////// zigbee direct security  /////////////////////////////////////////////////
static const u8 my_zbdSecurityServiceUUID[16]   = {ZBD_SEC_SERVICE_UUID_SERVICE};
static const u8 my_zbdSecurityCharUUID25519[16]  = {CHARACTERISTIC_UUID_ZBD_SS_SPEKE_AES128};
static const u8 my_zbdSecurityCharUUIDP256[16]  = {CHARACTERISTIC_UUID_ZBD_SS_ECDHE_SHA256};
static u8 zbdSec25519ValueCCC[2] = {0};
static u8 zbdSec256ValueCCC[2] = {0};
const u16 my_zbdSecuritycfgUUID                 = GATT_UUID_CLIENT_CHAR_CFG;

/* The max length is decided by the message2 */
u8  my_zbdSecirutyData[ZBD_SS_PACKET_LEN_MAX] = {0};
s32 my_zbdSecirutyDataLen = 0;
u16 my_zbdSecRecvHnadle = 0;
/* zigbee direct security */
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
    u8 ret = blc_gatt_pushHandleValueIndicate(BLS_CONN_HANDLE, my_zbdSecRecvHnadle, p, len);
    if (GATT_ERR_DATA_PENDING_DUE_TO_SERVICE_DISCOVERY_BUSY == ret ) {
        extern void  blc_att_setServerDataPendingTime_upon_ClientCmd(u8 num_10ms);
        blc_att_setServerDataPendingTime_upon_ClientCmd(0);
        blc_gatt_pushHandleValueIndicate (BLS_CONN_HANDLE, my_zbdSecRecvHnadle, p, len);
    }

    return 0;
}

////////////////////////// zigbee direct commissioning /////////////////////////////////////////////////
const u16 my_commServiceUUID                    = SERVICE_UUID_COMMISSIONING;
const u8 my_formNetworkCharUUID[16]             = {CHARACTERISTIC_UUID_FORM_NETWORK};
const u8 my_joinNetworkCharUUID[16]             = {CHARACTERISTIC_UUID_JOIN_NETWORK};
const u8 my_permitJoiningCharUUID[16]           = {CHARACTERISTIC_UUID_PERMIT_JOINING};
const u8 my_leaveNetworkCharUUID[16]            = {CHARACTERISTIC_UUID_LEAVE_NETWORK};
const u8 my_commissioningStatusCharUUID[16]     = {CHARACTERISTIC_UUID_COMMISSIONING_STATUS};
const u16 my_commissioningStatusCfgUUID         = GATT_UUID_CLIENT_CHAR_CFG;
const u8 my_identifyCharUUID[16]                = {CHARACTERISTIC_UUID_IDENTIFY};
const u8 my_findBindCharUUID[16]                = {CHARACTERISTIC_UUID_FINDING_BINDING};

u32 my_NetworkData = 0x00;
u8 my_commissStatus[COMMISSION_STATUS_COMMAND_LEN_MAX] = {0};
u8 my_identifyTime[12] = {0};

u8 my_commissChangeCCC[2] = {0};

extern void blc_att_setReadRsp_len(int len);

int app_zbd_commStatusHandle(void)
{
    u8 realLen = 0;
    memset(my_commissStatus, 0, sizeof(my_commissStatus));
    zbd_commissStatusNotifSend(BLS_CONN_HANDLE, COMM_COMM_STATUS_DP_H, my_commissStatus, &realLen);

    return 0;
}

void app_zbd_commissioningResultNotify(u8 status)
{
    zbd_commissionRetNotify(BLS_CONN_HANDLE, COMM_COMM_STATUS_DP_H, status);
    return;
}

int app_formNetworkWrite(u16 connHandle, void *p)
{
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
        blc_gatt_pushErrorResponse(connHandle, req->handle, ATT_OP_READ_REQ, ATT_ERR_ZIGBEE_DIRECT_APP_ERROR);
        return 1;
    } else {
        blc_att_setReadRsp_len(realLen);
    }

    return 0;
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
    u8 realLen = 0;
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;
    memset(my_identifyTime, 0, sizeof(my_identifyTime));
    zbd_commissIdentifyTimeRead(connHandle, req->handle, my_identifyTime, &realLen);

    if (realLen == 0) {
        blc_gatt_pushErrorResponse(connHandle, req->handle, ATT_OP_READ_REQ, ATT_ERR_ZIGBEE_DIRECT_APP_ERROR);
        return 1;
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

const attribute_t my_Attributes[] = {

    {ATT_END_H - 1, 0,0,0,0,0}, // total num of attribute


    // 0001 - 0007  gap
    {7,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),  (u8*)(&my_gapServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_READ_NOTIFY), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_devName), (u8*)(&my_devNameUUID), (u8*)(my_devName), 0},
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_READ), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof (my_appearance), (u8*)(&my_appearanceUIID),    (u8*)(&my_appearance), 0},
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_READ), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof (my_periConnParameters),(u8*)(&my_periConnParamUUID),  (u8*)(&my_periConnParameters), 0},


    // 0008 - 000b gatt
    {4,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),  (u8*)(&my_gattServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_INDICATE), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof (serviceChangeVal), (u8*)(&serviceChangeUIID),     (u8*)(&serviceChangeVal), 0},
    {0,ATT_PERMISSIONS_RDWR,2,sizeof (serviceChangeCCC),(u8*)(&clientCharacterCfgUUID), (u8*)(serviceChangeCCC), 0},


    // 000c - 000e  device Information Service
    {3,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),  (u8*)(&my_devServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_READ), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof (my_PnPtrs),(u8*)(&my_PnPUUID), (u8*)(my_PnPtrs), 0},


    ////////////////////////////////////// Battery Service /////////////////////////////////////////////////////
    // 00-0f - 0012
    {4,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),  (u8*)(&my_batServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_READ_NOTIFY), 0},               //prop
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_batVal),(u8*)(&my_batCharUUID),     (u8*)(my_batVal), 0},   //value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(batteryValueInCCC),(u8*)(&clientCharacterCfgUUID),     (u8*)(batteryValueInCCC), 0},   //value

    ////////////////////////////////////// Zigbee direct security service /////////////////////////////////////////////////////
    //0x13-0x19
    {7,ATT_PERMISSIONS_READ, 2,  16,                         (u8*)(&my_primaryServiceUUID),  (u8*)(&my_zbdSecurityServiceUUID), 0,  0},
    {0,ATT_PERMISSIONS_READ, 2,  1,                          (u8*)(&my_characterUUID),       (u8*)(&PROP_WRITE_INDICATE),  0,  0},              //
    {0,ATT_PERMISSIONS_WRITE, 16, sizeof(my_zbdSecirutyData), (u8*)(&my_zbdSecurityCharUUID25519), (u8*)(&my_zbdSecirutyData), &app_zbdSecurityWrite, 0},   //value
    {0,ATT_PERMISSIONS_RDWR, 2,  sizeof (zbdSec25519ValueCCC),  (u8*)(&my_zbdSecuritycfgUUID),  (u8*)(zbdSec25519ValueCCC), 0},

    {0,ATT_PERMISSIONS_READ, 2,  1,                          (u8*)(&my_characterUUID),       (u8*)(&PROP_WRITE_INDICATE), 0, 0},                //
    {0,ATT_PERMISSIONS_WRITE, 16, sizeof(my_zbdSecirutyData), (u8*)(&my_zbdSecurityCharUUIDP256), (u8*)(&my_zbdSecirutyData), &app_zbdSecurityWrite, 0},    //value
    {0,ATT_PERMISSIONS_RDWR, 2,  sizeof (zbdSec256ValueCCC),  (u8*)(&my_zbdSecuritycfgUUID),  (u8*)(zbdSec256ValueCCC), 0},

    ////////////////////////////////////// Commissioning Service /////////////////////////////////////////////////////
    // 001a - 0029
    {16,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),     (u8*)(&my_commServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_WRITE), 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(&my_formNetworkCharUUID), (u8*)(&my_NetworkData), &app_formNetworkWrite, 0},   //value

    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_WRITE), 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(&my_joinNetworkCharUUID), (u8*)(&my_NetworkData), &app_joinNetworkWrite, 0},   //value

    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_WRITE), 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(&my_permitJoiningCharUUID), (u8*)(&my_NetworkData), &app_permitJoinWrite, 0},  //value

    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_WRITE), 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(&my_leaveNetworkCharUUID), (u8*)(&my_NetworkData), &app_leaveNetworkWrite, 0}, //value

    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_READ_NOTIFY), 0},               //prop
    {0,ATT_PERMISSIONS_READ,16,sizeof(my_commissStatus),(u8*)(&my_commissioningStatusCharUUID), (u8*)(my_commissStatus), 0, &app_commissStatusRead},    //value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(my_commissChangeCCC),(u8*)(&my_commissioningStatusCfgUUID),    (u8*)(my_commissChangeCCC), 0}, //value

    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_READ_WRITE), 0},                //prop
    {0,ATT_PERMISSIONS_RDWR,16,sizeof(my_NetworkData),(u8*)(&my_identifyCharUUID), (u8*)(&my_identifyTime), &app_identifyWrite, &app_identifyRead}, //value

    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_WRITE), 0},             //prop
    {0,ATT_PERMISSIONS_WRITE,16,sizeof(my_NetworkData),(u8*)(&my_findBindCharUUID), (u8*)(&my_NetworkData), &app_findBindWrite, 0}, //value
};

void app_zbd_advDataSet(void)
{
#ifdef ZCL_ZBD_CONFIG
    zcl_zbdAttr_t *pState = zcl_zbdAttrGet();
    if (!pState->interfaceState) {
        bls_ll_setAdvData(tbl_advData, sizeof(tbl_advData));
        return;
    } else
#endif
    {
        u8 advDataExt[ADV_MAX_DATA_LEN] = {0};
        u8 advLen = sizeof(tbl_advData);
        memcpy(advDataExt, tbl_advData, advLen);
        zbd_extendAdvData(advDataExt, (u8*)&(advLen), 0);
        bls_ll_setAdvData(advDataExt, advLen);
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
    bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );

    return 0;
}

int app_zbd_zvdBleDisconnect(u8 ret)
{
    if (ret) {
        bls_ll_terminateConnection(HCI_ERR_OP_CANCELLED_BY_HOST);
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


void my_att_init (void)
{
    bls_att_setAttributeTable ((u8 *)my_Attributes);
}

void app_switch_to_indirect_adv(u8 e, u8 *p, int n)
{
    bls_ll_setAdvParam( MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
                        ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
                        0,  NULL,
                        MY_APP_ADV_CHANNEL,
                        ADV_FP_NONE);

    bls_ll_setAdvEnable(1);  //must: set adv enable
}

static s32 app_bleIntervalChange(void *arg)
{
    bls_l2cap_requestConnParamUpdate (g_appBleInterval, g_appBleInterval, g_appBleLatency, 400);

    return -1;
}

void app_bleConnIntervalSet(u8 interval, u16 latency)
{
    g_appBleInterval = interval;
    g_appBleLatency  = latency;

    TL_ZB_TIMER_SCHEDULE(app_bleIntervalChange, NULL, 20 * 1000); 
}

void zbd_ble_loopHandle(void)
{
    if (device_in_connection_state) {
        zbd_ss_transTaskProc();
    }
    zbd_zbNwkParaChangeDetect();
}

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_CONNECT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void task_connect (u8 e, u8 *p, int n)
{
    bls_l2cap_requestConnParamUpdate (g_appBleInterval, g_appBleInterval, g_appBleLatency, 400);

    app_bleConnIntervalSet(g_appBleInterval, g_appBleLatency);

    latest_user_event_tick = clock_time();

    device_in_connection_state = 1;//
    blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, MTU_SIZE_SETTING);

    zbd_ss_init(app_zbd_ssSemNotifyX);

#if (UI_LED_ENABLE && !TEST_CONN_CURRENT_ENABLE)
    gpio_write(GPIO_LED_RED, LED_ON_LEVAL);  //yellow light on
#endif
}

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_TERMINATE"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void task_terminate(u8 e,u8 *p, int n) //*p is terminate reason
{
    device_in_connection_state = 0;
    zbd_sessionExit();

    if (*p == HCI_ERR_CONN_TIMEOUT) {

    } else if (*p == HCI_ERR_REMOTE_USER_TERM_CONN) {  //0x13

    } else if (*p == HCI_ERR_CONN_TERM_MIC_FAILURE) {

    } else {

    }

#if (1 || PM_ENABLE)
     //user has push terminate pkt to ble TX buffer before deepsleep
    if (sendTerminate_before_enterDeep == 1) {
        sendTerminate_before_enterDeep = 2;
    }
#endif

    bls_ll_setAdvEnable(0);  //adv disable

    advertise_begin_tick = clock_time();

    if (*p != HCI_ERR_OP_CANCELLED_BY_HOST) {
        bls_ll_setAdvEnable(1);  //adv enable
    }
}

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void   user_set_rf_power (u8 e, u8 *p, int n)
{
    rf_set_power_level_index (g_ble_txPowerSet);
}

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
#if (1 || PM_ENABLE)
_attribute_ram_code_ void  ble_remote_set_sleep_wakeup (u8 e, u8 *p, int n)
{
    if ( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && ((u32)(bls_pm_getSystemWakeupTick() - clock_time())) > 80 * SYSTEM_TIMER_TICK_1MS){  //suspend time > 30ms.add gpio wakeup
        bls_pm_setWakeupSource(PM_WAKEUP_PAD);  //gpio pad wakeup suspend/deepsleep
    }
}
#endif

/**
 * @brief       This function is used to initialize the MAC address
 * @param[in]   flash_addr - flash address for MAC address
 * @param[in]   mac_public - public address
 * @param[in]   mac_random_static - random static MAC address
 * @return      none
 */
void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static)
{
    u8 mac_read[8];
    flash_read_page(flash_addr, 8, mac_read);

    u8 value_rand[5];
    generateRandomNum(5, value_rand);

    u8 ff_six_byte[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    if ( memcmp(mac_read, ff_six_byte, 6) ) {
        memcpy(mac_public, mac_read, 6);  //copy public address from flash
    } else {  //no public address on flash
        mac_public[0] = value_rand[0];
        mac_public[1] = value_rand[1];
        mac_public[2] = value_rand[2];

        //TODO
        //company id:
        mac_public[3] = 0xD1;             //company id: 0xC119D1
        mac_public[4] = 0x19;
        mac_public[5] = 0xC4;


        flash_write_page (flash_addr, 6, mac_public);
    }

    mac_random_static[0] = mac_public[0];
    mac_random_static[1] = mac_public[1];
    mac_random_static[2] = mac_public[2];
    mac_random_static[5] = 0xC0;            //for random static

    u16 high_2_byte = (mac_read[6] | mac_read[7]<<8);
    if (high_2_byte != 0xFFFF) {
        memcpy( (u8 *)(mac_random_static + 3), (u8 *)(mac_read + 6), 2);
    } else {
        mac_random_static[3] = value_rand[3];
        mac_random_static[4] = value_rand[4];

        flash_write_page (flash_addr + 6, 2, (u8 *)(mac_random_static + 3) );
    }
}

/**
 * @brief      callback function of Host Event
 * @param[in]  h - Host Event type
 * @param[in]  para - data pointer of event
 * @param[in]  n - data length of event
 * @return     0
 */

int app_host_event_callback (u32 h, u8 *para, int n)
{
    u8 event = h & 0xFF;

    switch(event)
    {
        case GAP_EVT_SMP_PAIRING_BEGIN:
        {
            g_bleConnDoing = 1;
        }
        break;

        case GAP_EVT_SMP_PAIRING_SUCCESS:
        {
            g_bleConnDoing = 0;
        }
        break;

        case GAP_EVT_SMP_PAIRING_FAIL:
        {
            g_bleConnDoing = 0;
        }
        break;
        case GAP_EVT_ATT_EXCHANGE_MTU:
        {
            // gap_gatt_mtuSizeExchangeEvt_t *pEvt = (gap_gatt_mtuSizeExchangeEvt_t *)para;
            // final_MTU_size = pEvt->effective_MTU;
            // mtuExchange_started_flg = 1;   //set MTU size exchange flag here
        }
        break;


        default:
        break;
    }

    return 0;
}

extern void sampleSwitchAnJoinTimerStart(void);
void zbd_bdbInitCb(u8 status, u8 joinedNetwork)
{
    if (status == BDB_INIT_STATUS_SUCCESS) {
        if (joinedNetwork) {
            if(zbd_sessionTypeGet() == ZBD_SESS_TYPE_NONE){
                zdb_sessionStateSet(ZBD_OPEN_TO_CONNECT_ZVD);
            }
#ifdef ZCL_ZBD_CONFIG
            sampleSwitchAnJoinTimerStart();
#endif
        } else {
            zdb_sessionStateSet(ZBD_OPEN_TO_BE_PROVISIONED);
        }
    }
}

void zbd_bdbcommissioningCb(u8 status)
{
    if (status != BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE) {
        app_zbd_commissioningResultNotify(status);
    }

    if (status == BDB_COMMISSION_STA_SUCCESS) {
        if (zbd_sessionTypeGet() == ZBD_SESS_TYPE_NONE) {
            zdb_sessionStateSet(ZBD_OPEN_TO_CONNECT_ZVD);
        }
#ifdef ZCL_ZBD_CONFIG
        sampleSwitchAnJoinTimerStart();
#endif
    }
}

void user_ble_normal_init(void)
{
    u8  mac_public[6];
    u8  mac_random_static[6];
    blc_initMacAddress(CFG_MAC_ADDRESS, mac_public, mac_random_static);

    #if(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_PUBLIC)
        app_own_address_type = OWN_ADDRESS_PUBLIC;
    #elif(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_RANDOM_STATIC)
        app_own_address_type = OWN_ADDRESS_RANDOM;
        blc_ll_setRandomAddr(mac_random_static);
    #endif

    //////////// Controller Initialization  Begin /////////////////////////
    blc_ll_initBasicMCU();                      //mandatory
    blc_ll_initStandby_module(mac_public);      //mandatory
    blc_ll_initAdvertising_module();    //adv module:        mandatory for BLE slave,
    blc_ll_initConnection_module();             //connection module  mandatory for BLE slave/master
    blc_ll_initSlaveRole_module();              //slave module:      mandatory for BLE slave,

    blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CONN_MAX_TX_OCTETS);

    blc_ll_initAclConnTxFifo(app_acl_txfifo, ACL_TX_FIFO_SIZE, ACL_TX_FIFO_NUM);
    blc_ll_initAclConnRxFifo(app_acl_rxfifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);

    u8 check_status = blc_controller_check_appBufferInitialization();
    if (check_status != BLE_SUCCESS) {
        /* here user should set some log to know which application buffer incorrect */
        //write_log32(0x88880000 | check_status);
        while(1);
    }
    //////////// Controller Initialization  End /////////////////////////

    //////////// Host Initialization  Begin /////////////////////////
    /* Host Initialization */
    /* GAP initialization must be done before any other host feature initialization !!! */
    blc_gap_peripheral_init();    //gap initialization
    my_att_init (); //gatt initialization

    /* L2CAP Initialization */
    blc_l2cap_register_handler (blc_l2cap_packet_receive);

    //re-configure the nv address for ble paring info, must before blc_smp_peripheral_init
    bls_smp_configParingSecurityInfoStorageAddr(FLASH_SMP_PAIRING_ADDR);
    blc_smp_setSecurityLevel(No_Security);
    //////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////

    ////////////////// config adv packet /////////////////////
    u8 status = bls_ll_setAdvParam(  MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
                                    ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
                                    0,  NULL,
                                    BLT_ENABLE_ADV_ALL,
                                    ADV_FP_NONE );
    if(status != BLE_SUCCESS) { while(1); }  //debug: adv setting err

    blc_gap_registerHostEventHandler( app_host_event_callback );
    blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_BEGIN            |  \
                          GAP_EVT_MASK_SMP_PAIRING_SUCCESS          |  \
                          GAP_EVT_MASK_SMP_PAIRING_FAIL             |  \
                          GAP_EVT_MASK_SMP_CONN_ENCRYPTION_DONE );

    zbd_sessionInit(app_zbd_openProviTimeoutCb, app_session_build_ret_cb, (u8 *)(size_t)my_Attributes);
    zbd_commissionInit(app_zbd_commStatusHandle, app_zbd_advDataUpdateHandle);
    app_zbd_advDataSet();

    bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

    bls_ll_setAdvEnable(BLC_ADV_ENABLE);  //adv enable

    //set rf power index, user must set it after every suspend wakeup, cause relative setting will be reset in suspend
    user_set_rf_power(0, 0, 0);


    bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
    bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &task_terminate);
    bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_EXIT, &user_set_rf_power);

    ///////////////////// Power Management initialization///////////////////
#if(1 || PM_ENABLE)
    blc_ll_initPowerManagement_module();

    #if (PM_DEEPSLEEP_RETENTION_ENABLE)
        //bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
        bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
        blc_pm_setDeepsleepRetentionThreshold(95, 95);
        blc_pm_setDeepsleepRetentionEarlyWakeupTiming(TEST_CONN_CURRENT_ENABLE ? 370 : 400);
        //blc_pm_setDeepsleepRetentionType(DEEPSLEEP_MODE_RET_SRAM_LOW64K); //default use 32k deep retention
    #else
        bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
    #endif

    bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_ENTER, &ble_remote_set_sleep_wakeup);
#else
    bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif

    advertise_begin_tick = clock_time();
}


/**
 * @brief       user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]   none
 * @return      none
 */
_attribute_ram_code_ void user_init_deepRetn(void)
{
    blc_ll_initBasicMCU();   //mandatory
    rf_set_power_level_index (g_ble_txPowerSet);
    blc_ll_recoverDeepRetention();
}

void user_ble_init(bool isRetention){
    sendTerminate_before_enterDeep = 0;
    if (isRetention) {
        user_init_deepRetn();
    } else {
        user_ble_normal_init();
    }
}

/**
 * @brief      power management code for application
 * @param      none
 * @return     none
 */
_attribute_ram_code_ u32 blt_pm_proc(void)
{
    if (sendTerminate_before_enterDeep == 2) {  //Terminate OK
        return 1;
    }

    if (!blc_ll_isControllerEventPending()) {  //no controller event pending
        //adv 60s, deepsleep
        if (blc_ll_getCurrentState() == BLS_LINK_STATE_ADV && !sendTerminate_before_enterDeep && \
            clock_time_exceed(advertise_begin_tick , ADV_IDLE_ENTER_DEEP_TIME * 1000000)) {
            return 1;
        }
        //conn 60s no event(key/voice/led), enter deepsleep
        else if (device_in_connection_state && \
                clock_time_exceed(latest_user_event_tick, CONN_IDLE_ENTER_DEEP_TIME * 1000000) ) {

            bls_ll_terminateConnection(HCI_ERR_REMOTE_USER_TERM_CONN); //push terminate cmd into ble TX buffer
            //bls_ll_setAdvEnable(BLC_ADV_DISABLE);   //disable adv, remove it to avoid to call it in the zigbee slot
            sendTerminate_before_enterDeep = 1;
        }
    }
    return 0;
}

void ble_advertiseTickUpdate(void)
{
    advertise_begin_tick = clock_time();
}
