/********************************************************************************************************
 * @file    sampleLightMultiBLE.c
 *
 * @brief   This is the source file for sampleLightMultiBLE
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
#include "device_manage.h"

#include "stack/ble/ble_multi/controller/ll/adv/leg_adv.h"

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
#define ACL_CONN_MAX_RX_OCTETS          27  //user set value

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
#define ACL_CENTRAL_MAX_TX_OCTETS       27  //user set value
#define ACL_PERIPHR_MAX_TX_OCTETS       27  //user set value


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
#define CENTRAL_ATT_RX_MTU              23  //user set value
#define PERIPHR_ATT_RX_MTU              23  //user set value

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

    //// SPP ////
    /**********************************************************************************************/
    SPP_PS_H,                            //UUID: 2800,  VALUE: telink spp service uuid

    //server to client
    SPP_SERVER_TO_CLIENT_CD_H,           //UUID: 2803,  VALUE:              Prop: read | Notify
    SPP_SERVER_TO_CLIENT_DP_H,           //UUID: telink spp s2c uuid,  VALUE: SppDataServer2ClientData
    SPP_SERVER_TO_CLIENT_CCB_H,          //UUID: 2902,  VALUE: SppDataServer2ClientDataCCC
    SPP_SERVER_TO_CLIENT_DESC_H,         //UUID: 2901,  VALUE: TelinkSPPS2CDescriptor

    //client to server
    SPP_CLIENT_TO_SERVER_CD_H,           //UUID: 2803,  VALUE:              Prop: read | write_without_rsp
    SPP_CLIENT_TO_SERVER_DP_H,           //UUID: telink spp c2s uuid,  VALUE: SppDataClient2ServerData
    SPP_CLIENT_TO_SERVER_DESC_H,         //UUID: 2901,  VALUE: TelinkSPPC2SDescriptor

    //// Ota ////
    /**********************************************************************************************/
    OTA_PS_H,                               //UUID: 2800,   VALUE: telink ota service uuid
    OTA_CMD_OUT_CD_H,                       //UUID: 2803,   VALUE:              Prop: read | write_without_rsp
    OTA_CMD_OUT_DP_H,                       //UUID: telink ota uuid,  VALUE: otaData
    OTA_CMD_OUT_CCB_H,                      //UUID: 2902,   VALUE: my_OtaDataCCC
    OTA_CMD_OUT_DESC_H,                     //UUID: 2901,   VALUE: otaName


    ATT_END_H,

} ATT_HANDLE;


typedef struct {
    u8  type;
    u8  rf_len;
    u16 l2capLen;
    u16 chanId;
    u8  opcode;
    u16 handle;
    u8 value;
} ble_rf_packet_att_write_t;

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

static const u16 userdesc_UUID  = GATT_UUID_CHAR_USER_DESC;

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


//////////////////////// OTA //////////////////////////////////
static const u8 my_OtaServiceUUID[16] = WRAPPING_BRACES(TELINK_OTA_UUID_SERVICE);
static const u8 my_OtaUUID[16] = WRAPPING_BRACES(TELINK_SPP_DATA_OTA);
_attribute_ble_data_retention_  static u8 my_OtaData                         = 0x00;
_attribute_ble_data_retention_  static u8 my_OtaDataCCC[2] = {0,0};
static const u8 my_OtaName[] = {'O', 'T', 'A'};

////////////////////// SPP ////////////////////////////////////
static const u8 TelinkSppServiceUUID[16] = WRAPPING_BRACES(TELINK_SPP_UUID_SERVICE);
static const u8 TelinkSppDataServer2ClientUUID[16] = WRAPPING_BRACES(TELINK_SPP_DATA_SERVER2CLIENT);
static const u8 TelinkSppDataClient2ServerUUID[16] = WRAPPING_BRACES(TELINK_SPP_DATA_CLIENT2SERVER);

// Spp data from Server to Client characteristic variables
_attribute_ble_data_retention_  static u8 SppDataServer2ClientDataCCC[2] = {0};
//this array will not used for sending data(directly calling HandleValueNotify API), so cut array length from 20 to 1, saving some SRAM
_attribute_ble_data_retention_  static u8 SppDataServer2ClientData[1] = {0};  //SppDataServer2ClientData[20]
// Spp data from Client to Server characteristic variables
//this array will not used for receiving data(data processed by Attribute Write CallBack function), so cut array length from 20 to 1, saving some SRAM
_attribute_ble_data_retention_  static u8 SppDataClient2ServerData[1] = {0};  //SppDataClient2ServerData[20]

//SPP data descriptor
static const u8 TelinkSPPS2CDescriptor[] = "Telink SPP: Module->Phone";
static const u8 TelinkSPPC2SDescriptor[] = "Telink SPP: Phone->Module";

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


//// Telink spp  attribute values
static const u8 TelinkSppDataServer2ClientCharVal[19] = {
    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
    U16_LO(SPP_SERVER_TO_CLIENT_DP_H), U16_HI(SPP_SERVER_TO_CLIENT_DP_H),
    TELINK_SPP_DATA_SERVER2CLIENT
};
static const u8 TelinkSppDataClient2ServerCharVal[19] = {
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
    U16_LO(SPP_CLIENT_TO_SERVER_DP_H), U16_HI(SPP_CLIENT_TO_SERVER_DP_H),
    TELINK_SPP_DATA_CLIENT2SERVER
};

//// OTA attribute values
static const u8 my_OtaCharVal[19] = {
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY,
    U16_LO(OTA_CMD_OUT_DP_H), U16_HI(OTA_CMD_OUT_DP_H),
    TELINK_SPP_DATA_OTA,
};

int spp_onReceiveData(u16 connHandle, ble_rf_packet_att_write_t *p)
{
    (void)connHandle; //unused, remove warning
    (void)p; //unused, remove warning

    return 0;
}

int app_bleOtaWrite(u16 connHandle, void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;
    u8 len = req->rf_len - 9;
    u16 cmd_type =  req->dat[0] ;
    cmd_type <<= 8;
    cmd_type |= req->dat[1] ;

    extern int zb_ble_hci_cmd_handler(u16 clusterId, u8 len, u8 *payload);
    zb_ble_hci_cmd_handler(cmd_type, len, &(req->dat[2]));
    return 0;
}

int app_bleOtaRead(u16 connHandle, void *p)
{
    my_OtaData++;  //for testing, user can fill the valid data here
    return 0;
}

#if BLE_OTA_SERVER_ENABLE
/**
 * @brief      this function is used to register the function for OTA start.
 * @param[in]  none
 * @return     none
 */
void app_enter_ota_mode(void)
{
#if FLASH_PROTECT_ENABLE
    flash_unlock();
#endif
}

/**
 * @brief       no matter whether the OTA result is successful or fail.
 *              code will run here to tell user the OTA result.
 * @param[in]   result    OTA result:success or fail(different reason)
 * @return      none
 */
void app_ota_result(int result)
{
    if (result == OTA_SUCCESS) {  //led for debug: OTA success

    } else {  //OTA fail

    }
#if FLASH_PROTECT_ENABLE
    flash_lock();
#endif
}
#endif

// TM : to modify
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
    // 002a - 002d
    {4,ATT_PERMISSIONS_READ,2,2,(u8*)(size_t)(&my_primaryServiceUUID),  (u8*)(size_t)(&my_batServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_batCharVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(my_batCharVal), 0, 0},              //prop
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_batVal),(u8*)(size_t)(&my_batCharUUID),     (u8*)(my_batVal), 0, 0},    //value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(batteryValueInCCC),(u8*)(size_t)(&clientCharacterCfgUUID),     (u8*)(batteryValueInCCC), 0, 0},    //value


    // 002e - 0035 SPP for data test
    {8,ATT_PERMISSIONS_READ,2,16,(u8*)(size_t)(&my_primaryServiceUUID),     (u8*)(size_t)(&TelinkSppServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(TelinkSppDataServer2ClientCharVal),(u8*)(size_t)(&my_characterUUID),       (u8*)(size_t)(TelinkSppDataServer2ClientCharVal), 0, 0},                //prop
    {0,ATT_PERMISSIONS_READ,16,sizeof(SppDataServer2ClientData),(u8*)(size_t)(&TelinkSppDataServer2ClientUUID), (u8*)(SppDataServer2ClientData), 0, 0}, //value
    {0,ATT_PERMISSIONS_RDWR,2,2,(u8*)(size_t)&clientCharacterCfgUUID,(u8*)(&SppDataServer2ClientDataCCC), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(TelinkSPPS2CDescriptor),(u8*)(size_t)&userdesc_UUID,(u8*)(size_t)(&TelinkSPPS2CDescriptor), 0, 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(TelinkSppDataClient2ServerCharVal),(u8*)(size_t)(&my_characterUUID),       (u8*)(size_t)(TelinkSppDataClient2ServerCharVal), 0, 0},                //prop
    {0,ATT_PERMISSIONS_RDWR,16,sizeof(SppDataClient2ServerData),(u8*)(size_t)(&TelinkSppDataClient2ServerUUID), (u8*)(SppDataClient2ServerData), (att_readwrite_callback_t)&spp_onReceiveData, 0},  //value
    {0,ATT_PERMISSIONS_READ,2,sizeof(TelinkSPPC2SDescriptor),(u8*)(size_t)&userdesc_UUID,(u8*)(size_t)(&TelinkSPPC2SDescriptor), 0, 0},

    ////////////////////////////////////// OTA /////////////////////////////////////////////////////
    // 0036 - 0039
    {5,ATT_PERMISSIONS_READ, 2,16,(u8*)(size_t)(&my_primaryServiceUUID),    (u8*)(size_t)(&my_OtaServiceUUID), 0, 0},
    {0,ATT_PERMISSIONS_READ, 2, sizeof(my_OtaCharVal),(u8*)(size_t)(&my_characterUUID), (u8*)(size_t)(my_OtaCharVal), 0, 0},                //prop
#if BLE_OTA_SERVER_ENABLE
    {0,ATT_PERMISSIONS_RDWR,16,sizeof(my_OtaData),(u8*)(size_t)(&my_OtaUUID),   (&my_OtaData), &otaWrite, NULL},            //value
#else
    {0,ATT_PERMISSIONS_RDWR,16,sizeof(my_OtaData),(u8*)(size_t)(&my_OtaUUID),   (&my_OtaData), &app_bleOtaWrite, &app_bleOtaRead},          //value
#endif
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(my_OtaDataCCC),(u8*)(size_t)(&clientCharacterCfgUUID),     (u8*)(my_OtaDataCCC), 0, 0},    //value
    {0,ATT_PERMISSIONS_READ, 2,sizeof (my_OtaName),(u8*)(size_t)(&userdesc_UUID), (u8*)(size_t)(my_OtaName), 0, 0},
};

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

/**
 * @brief   BLE Advertising data
 */
const u8    tbl_advData[] = {
     13, DT_COMPLETE_LOCAL_NAME,                'm','u','l','t','i','-','-','l','i','g','h','t',
     2,  DT_FLAGS,                              0x05,                   // BLE limited discoverable mode and BR/EDR not supported
     3,  DT_APPEARANCE,                         0x80, 0x01,             // 384, Generic Remote Control, Generic category
     5,  DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID, 0x12, 0x18, 0x0F, 0x18, // incomplete list of service class UUIDs (0x1812, 0x180F)
};

/**
 * @brief   BLE Scan Response Packet data
 */
const u8    tbl_scanRsp [] = {
     13, DT_COMPLETE_LOCAL_NAME,                'm','u','l','t','i','-','-','l','i','g','h','t',
};





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

/* Negotiate the connection interval after 20 seconds to increase the probability of successful negotiation */
void app_bleConnIntervalSet(u8 interval, u16 latency)
{
    g_appBleInterval = interval;
    g_appBleLatency  = latency;

    TL_ZB_TIMER_SCHEDULE(app_bleIntervalChange, NULL, 20 * 1000);
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

    /* SMP Initialization */
    #if (ACL_PERIPHR_SMP_ENABLE || ACL_CENTRAL_SMP_ENABLE)
        blc_smp_configPairingSecurityInfoStorageAddressAndSize(FLASH_SMP_PAIRING_ADDR, FLASH_SMP_PAIRING_MAX_SIZE);
    #endif

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
    blc_ll_setAdvData(tbl_advData, sizeof(tbl_advData));
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

#if (BLE_OTA_SERVER_ENABLE)
    extern u32 mcuBootAddrGet(void);
    u32 bootAddr = mcuBootAddrGet();
    u32 newAddr = (bootAddr) ? 0 : FLASH_ADDR_OF_OTA_IMAGE;
    blc_ota_setProgramOffset(newAddr);
    blc_ota_setFirmwareSizeAndBootAddress(FLASH_OTA_IMAGE_MAX_SIZE, FLASH_ADDR_OF_OTA_IMAGE);

    /*it will clear the old firmware in ble stack.*/
    #if FLASH_PROTECT_ENABLE
        flash_unlock();
    #endif

    blc_ota_initOtaServer_module();
    blc_ota_setOtaProcessTimeout(300);      //adjust according to the conn interval and the firmware size by user

    blc_ota_registerOtaStartCmdCb(app_enter_ota_mode);
    blc_ota_registerOtaResultIndicationCb(app_ota_result);

    #if FLASH_PROTECT_ENABLE
        flash_lock();
    #endif
#endif

    tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] acl connection demo init", 0, 0);
}

int blt_sdk_main_loop(void)
{
    ////////////////////////////////////// BLE entry /////////////////////////////////
    blc_sdk_main_loop();

    return 0; //must return 0 due to SDP flow
}

