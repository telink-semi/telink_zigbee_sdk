/********************************************************************************************************
 * @file    sampleLightBLESlave_8258.c *
 * @brief   This is the source file for sampleLightBLESlave_8258
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

#include <stack/ble/ble.h>
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"


#if (__PROJECT_TL_DIMMABLE_LIGHT__ && BLE_SLAVE_ROLE_ENABLE)

extern int zb_ble_hci_cmd_handler(u16 clusterId, u8 len, u8 *payload);

static int app_bleOtaWrite(void *p);
static int app_bleOtaRead(void *p);

_attribute_data_retention_  unsigned int  tlk_flash_mid = 0;
_attribute_data_retention_  unsigned int  tlk_flash_vendor = 0;
_attribute_data_retention_  unsigned char tlk_flash_capacity;

#define ADV_IDLE_ENTER_DEEP_TIME            60  //60 s
#define CONN_IDLE_ENTER_DEEP_TIME           60  //60 s

#define MY_DIRECT_ADV_TMIE                  2000000

#define MY_APP_ADV_CHANNEL                  BLT_ENABLE_ADV_ALL
#define MY_ADV_INTERVAL_MIN                 ADV_INTERVAL_200MS
#define MY_ADV_INTERVAL_MAX                 ADV_INTERVAL_205MS

#define BLE_DEVICE_ADDRESS_TYPE             BLE_DEVICE_ADDRESS_PUBLIC

#define RX_FIFO_SIZE                        64
#define RX_FIFO_NUM                         8

#define TX_FIFO_SIZE                        40
#define TX_FIFO_NUM                         16

#define MTU_SIZE_SETTING                    64

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


    //// Ota ////
    /**********************************************************************************************/
    OTA_PS_H,                               //UUID: 2800,   VALUE: telink ota service uuid
    OTA_CMD_OUT_CD_H,                       //UUID: 2803,   VALUE:              Prop: read | write_without_rsp
    OTA_CMD_OUT_DP_H,                       //UUID: telink ota uuid,  VALUE: otaData
    OTA_CMD_OUT_DESC_H,                     //UUID: 2901,   VALUE: otaName

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


const u8 PROP_READ        = CHAR_PROP_READ;
const u8 PROP_WRITE       = CHAR_PROP_WRITE;
const u8 PROP_INDICATE    = CHAR_PROP_INDICATE;
const u8 PROP_WRITE_NORSP = CHAR_PROP_WRITE_WITHOUT_RSP;
const u8 PROP_READ_NOTIFY = CHAR_PROP_READ | CHAR_PROP_NOTIFY;
const u8 PROP_READ_WRITE_NORSP        = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP;
const u8 PROP_READ_WRITE_WRITENORSP   = CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP;
const u8 PROP_READ_WRITE              = CHAR_PROP_READ | CHAR_PROP_WRITE;
const u8 PROP_READ_WRITE_NORSP_NOTIFY = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY;


u8 my_devName[] = {'t','l','B','u','l','b'};
/*
*    Adv Packet, Response Packet
*/
u8 tbl_advData[] = {
     0x07, 0x09, 't','l','B','u','l','b',
     0x02, 0x01, 0x05,                          // BLE limited discoverable mode and BR/EDR not supported
     0x03, 0x19, 0x80, 0x01,                    // 384, Generic Remote Control, Generic category
     0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,        // incomplete list of service class UUIDs (0x1812, 0x180F)
};

u8 tbl_scanRsp [] = {
         0x07, 0x09, 't','l','B','u','l','b',
    };

const u8 my_PnPtrs [] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};

/*
 * Battery
 * */
const u16 my_batServiceUUID       = SERVICE_UUID_BATTERY;
const u16 my_batCharUUID          = CHARACTERISTIC_UUID_BATTERY_LEVEL;
static u8 batteryValueInCCC[2];
u8 my_batVal[1]            = {99};

/*
 * ota
 * */
const u8 my_OtaUUID[16]         = {TELINK_SPP_DATA_OTA};
const u8 my_OtaServiceUUID[16]  = {TELINK_OTA_UUID_SERVICE};
const u16 userdesc_UUID         = GATT_UUID_CHAR_USER_DESC;
u32 my_OtaData                  = 0x00;
const u8  my_OtaName[]          = {'O', 'T', 'A'};


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
    // 000f - 0012
    {4,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),  (u8*)(&my_batServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),       (u8*)(&PROP_READ_NOTIFY), 0},               //prop
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_batVal),(u8*)(&my_batCharUUID),     (u8*)(my_batVal), 0},   //value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(batteryValueInCCC),(u8*)(&clientCharacterCfgUUID),     (u8*)(batteryValueInCCC), 0},   //value

    ////////////////////////////////////// OTA /////////////////////////////////////////////////////
    // 0013 - 0016
    {4,ATT_PERMISSIONS_READ, 2,16,(u8*)(&my_primaryServiceUUID),    (u8*)(&my_OtaServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),         (u8*)(&PROP_READ_WRITE_NORSP), 0},              //prop
    {0,ATT_PERMISSIONS_RDWR,16,sizeof(my_OtaData),(u8*)(&my_OtaUUID),   (u8 *)(&my_OtaData), &app_bleOtaWrite, &app_bleOtaRead},            //value
    {0,ATT_PERMISSIONS_READ, 2,sizeof (my_OtaName),(u8*)(&userdesc_UUID), (u8*)(my_OtaName), 0},

};

u8 g_ble_txPowerSet = RF_POWER_INDEX_P3p01dBm;

own_addr_type_t app_own_address_type = OWN_ADDRESS_PUBLIC;

u8 blt_rxfifo_b[RX_FIFO_SIZE * RX_FIFO_NUM] = {0};
my_fifo_t blt_rxfifo = {
    RX_FIFO_SIZE,
    RX_FIFO_NUM,
    0,
    0,
    blt_rxfifo_b,
};

u8 blt_txfifo_b[TX_FIFO_SIZE * TX_FIFO_NUM] = {0};
my_fifo_t blt_txfifo = {
    TX_FIFO_SIZE,
    TX_FIFO_NUM,
    0,
    0,
    blt_txfifo_b,
};

int device_in_connection_state;
static u8 g_appBleInterval = CONN_INTERVAL_50MS;
static u16 g_appBleLatency = 19;

static u32 mtuExchange_started_flg = 0;

volatile bool g_bleConnDoing = 0;

/*
 * debug various
 * */
volatile u8 T_bleDataAbandom;
volatile u8 T_final_MTU_size = 0;

#define DBG_ADV_REPORT_ON_RAM               0     //just debug
#if (DBG_ADV_REPORT_ON_RAM)  //debug adv report on ram
    #define  RAM_ADV_MAX        64
    u8 AA_advRpt[RAM_ADV_MAX][48];
    u8 AA_advRpt_index = 0;
#endif


/*
 * functions
 * */
static int app_bleOtaWrite(void *p)
{
    rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;
    u8 len = req->rf_len - 9;
    u16 cmd_type =  req->dat[0] ;
    cmd_type <<= 8;
    cmd_type |= req->dat[1] ;

    zb_ble_hci_cmd_handler(cmd_type, len, &(req->dat[2]));

    return 0;
}

static int app_bleOtaRead(void *p)
{
    my_OtaData++;  //for testing, user can fill the valid data here
    return 0;
}


static void my_att_init (void)
{
    bls_att_setAttributeTable ((u8 *)my_Attributes);
}


static void app_switch_to_indirect_adv(u8 e, u8 *p, int n)
{
    bls_ll_setAdvParam( MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
                        ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
                        0,  NULL,
                        MY_APP_ADV_CHANNEL,
                        ADV_FP_NONE);

    bls_ll_setAdvEnable(1);  //must: set adv enable
}



static void ble_remote_terminate(u8 e,u8 *p, int n)
{ //*p is terminate reason
    device_in_connection_state = 0;
    mtuExchange_started_flg = 0;

    if (*p == HCI_ERR_CONN_TIMEOUT) {

    } else if (*p == HCI_ERR_REMOTE_USER_TERM_CONN) {  //0x13

    } else if (*p == HCI_ERR_CONN_TERM_MIC_FAILURE) {

    } else {

    }

    bls_ll_setAdvEnable(0);  //adv disable

    if (*p != HCI_ERR_OP_CANCELLED_BY_HOST) {
        bls_ll_setAdvEnable(1);  //adv enable
    }
}

static void  ble_exception_data_abandom(u8 e,u8 *p, int n)
{
    T_bleDataAbandom++;
}

_attribute_ram_code_ void   user_set_rf_power (u8 e, u8 *p, int n)
{
    rf_set_power_level_index (g_ble_txPowerSet);   //same to ZB
    //rf_set_power_level_index (0);
}

static s32 app_bleIntervalChange(void *arg)
{
    bls_l2cap_requestConnParamUpdate (g_appBleInterval, g_appBleInterval, g_appBleLatency, CONN_TIMEOUT_4S);  // 1 S

    return -1;
}

/*Negotiate the connection interval after 10 seconds to increase the probability of successful negotiation*/
void app_bleConnIntervalSet(u8 interval, u16 latency)
{
    g_appBleInterval = interval;
    g_appBleLatency  = latency;

    TL_ZB_TIMER_SCHEDULE(app_bleIntervalChange, NULL, 10*1000); 
}

static void task_connect (u8 e, u8 *p, int n)
{
    /* interval:    n*1.25 ms
     * lantency:    (n+1)*8*1.25 ms
     * timeout:     n*10 ms
     * */
    bls_l2cap_requestConnParamUpdate (g_appBleInterval, g_appBleInterval, g_appBleLatency, CONN_TIMEOUT_4S);

    app_bleConnIntervalSet(g_appBleInterval, g_appBleLatency);

    device_in_connection_state = 1;//one zero
}

void task_conn_update_req (u8 e, u8 *p, int n)
{

}

void task_conn_update_done (u8 e, u8 *p, int n)
{

}

static void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static)
{
//  u8  mac_public[6]        = {0x00, 0x00, 0x00, 0x38, 0xC1, 0xA4};  //company id: 0xA4C138
//  u8  mac_random_static[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xC0};

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
        mac_public[3] = 0x38;             //company id: 0xA4C138
        mac_public[4] = 0xC1;
        mac_public[5] = 0xA4;

        flash_write_page (flash_addr, 6, mac_public);
    }
}

static int app_host_event_callback (u32 h, u8 *para, int n)
{
    u8 event = h & 0xFF;

    switch(event) {
        case GAP_EVT_SMP_PARING_BEAGIN:
        {
            g_bleConnDoing = 1;
        }
        break;

        case GAP_EVT_SMP_PARING_SUCCESS:
        {
            g_bleConnDoing = 0;
        }
        break;

        case GAP_EVT_SMP_PARING_FAIL:
        {
            g_bleConnDoing = 0;
        }
        break;

        case GAP_EVT_SMP_CONN_ENCRYPTION_DONE:
        {
            if(!mtuExchange_started_flg){  //master do not send MTU exchange request in time
                blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, MTU_SIZE_SETTING);
            }
        }
        break;

        case GAP_EVT_ATT_EXCHANGE_MTU:
        {
            gap_gatt_mtuSizeExchangeEvt_t *pEvt = (gap_gatt_mtuSizeExchangeEvt_t *)para;
            T_final_MTU_size = pEvt->effective_MTU;
            mtuExchange_started_flg = 1;   //set MTU size exchange flag here
        }
        break;


        default:
        break;
    }

    return 0;
}

#if (BLE_REMOTE_OTA_ENABLE)
static void app_enter_ota_mode(void)
{
    //ota_is_working = 1;
    light_blink_start(0, 200, 200);

    bls_ota_setTimeout(260 * 1000 * 1000); //set OTA timeout  120 seconds
}
#endif

#if SCAN_IN_ADV_STATE
#define DBG_ADV_REPORT_ON_RAM               1     //just debug
#if (DBG_ADV_REPORT_ON_RAM)  //debug adv report on ram
    #define  RAM_ADV_MAX        32
    u8 AA_advRpt[RAM_ADV_MAX][48];
    u8 AA_advRpt_index = 0;
#endif

static int controller_event_callback (u32 h, u8 *p, int n)
{
    if (h &HCI_FLAG_EVENT_BT_STD) {      //ble controller hci event
        u8 evtCode = h & 0xff;

        if (evtCode == HCI_EVT_LE_META) {
            u8 subEvt_code = p[0];
            if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT) {  // ADV packet
                //after controller is set to scan state, it will report all the adv packet it received by this event

                event_adv_report_t *pa = (event_adv_report_t *)p;
                s8 rssi = (s8)pa->data[pa->len];//rssi has already plus 110.

                #if (DBG_ADV_REPORT_ON_RAM)
                    if (pa->len > 31) {
                        pa->len = 31;
                    }
                    memcpy( (u8 *)AA_advRpt[AA_advRpt_index++],  p, pa->len + 11);
                    if (AA_advRpt_index >= RAM_ADV_MAX) {
                        AA_advRpt_index = 0;
                    }
                #endif
            }
        }
    }
    return 0;
}
#endif

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

void user_ble_init(bool isRetention)
{
    (void)isRetention;
    bls_smp_configPairingSecurityInfoStorageAddr(CFG_NV_START_FOR_BLE);

    // blc_app_loadCustomizedParameters();  //load customized freq_offset cap value

////////////////// BLE stack initialization ////////////////////////////////////
    u8  mac_public[6];
    u8  mac_random_static[6];
    blc_initMacAddress(CFG_MAC_ADDRESS, mac_public, mac_random_static);

    #if(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_PUBLIC)
        app_own_address_type = OWN_ADDRESS_PUBLIC;
    #elif(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_RANDOM_STATIC)
        app_own_address_type = OWN_ADDRESS_RANDOM;
        blc_ll_setRandomAddr(mac_random_static);
    #endif

    blc_flash_read_mid_get_vendor_set_capacity();

#if PA_ENABLE
    /* external RF PA used */
    g_ble_txPowerSet = ZB_RADIO_TX_0DBM;   //set to 0dBm
    ble_rf_pa_init(0, PA_TX, PA_RX);
#endif

    ////// Controller Initialization  //////////
    blc_ll_initBasicMCU();                      //mandatory
    blc_ll_initStandby_module(mac_public);              //mandatory
    blc_ll_initAdvertising_module(mac_public);  //adv module:        mandatory for BLE slave,
    blc_ll_initSlaveRole_module();              //slave module:      mandatory for BLE slave,
    blc_ll_initPowerManagement_module();        //pm module:         optional


    ////// Host Initialization  //////////
    blc_gap_peripheral_init();    //gap initialization
    my_att_init (); //gatt initialization
    blc_l2cap_register_handler (blc_l2cap_packet_receive);      //l2cap initialization

    //Smp Initialization may involve flash write/erase(when one sector stores too much information,
    //   is about to exceed the sector threshold, this sector must be erased, and all useful information
    //   should re_stored) , so it must be done after battery check
#if (APP_SECURITY_ENABLE)
    blc_smp_peripheral_init();
#else
    blc_smp_setSecurityLevel(No_Security);
#endif

    blc_gap_registerHostEventHandler( app_host_event_callback );
    blc_gap_setEventMask( GAP_EVT_MASK_SMP_PARING_BEAGIN            |  \
                          GAP_EVT_MASK_SMP_PARING_SUCCESS           |  \
                          GAP_EVT_MASK_SMP_PARING_FAIL              |  \
                          GAP_EVT_MASK_SMP_CONN_ENCRYPTION_DONE     |  \
                          GAP_EVT_MASK_ATT_EXCHANGE_MTU);

///////////////////// USER application initialization ///////////////////
    bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
    bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

    ////////////////// config adv packet /////////////////////
#if (APP_SECURITY_ENABLE)
    u8 bond_number = blc_smp_param_getCurrentBondingDeviceNumber();  //get bonded device number
    smp_param_save_t  bondInfo;
    if (bond_number) {  //at least 1 bonding device exist
        bls_smp_param_loadByIndex( bond_number - 1, &bondInfo);  //get the latest bonding device (index: bond_number-1 )

    }

    if (bond_number) {  //set direct adv
        //set direct adv
        u8 status = bls_ll_setAdvParam( ADV_INTERVAL_200MS, ADV_INTERVAL_205MS,
                                        ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY, app_own_address_type,
                                        bondInfo.peer_addr_type,  bondInfo.peer_addr,
                                        MY_APP_ADV_CHANNEL,
                                        ADV_FP_NONE);
        if (status != BLE_SUCCESS) { write_reg8(0x40002, 0x11);  while(1); }  //debug: adv setting err

        //it is recommended that direct adv only last for several seconds, then switch to indirect adv
        bls_ll_setAdvDuration(MY_DIRECT_ADV_TMIE, 1);
        bls_app_registerEventCallback (BLT_EV_FLAG_ADV_DURATION_TIMEOUT, &app_switch_to_indirect_adv);

    }
    else   //set indirect adv
#endif
    {
        u8 status = bls_ll_setAdvParam(  MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
                                         ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
                                         0,  NULL,
                                         MY_APP_ADV_CHANNEL,
                                         ADV_FP_NONE);
        if (status != BLE_SUCCESS) { 
            write_reg8(0x40002, 0x11);  
            while(1); 
        }  //debug: adv setting err
    }

    bls_ll_setAdvEnable(1);  //adv enable

#if SCAN_IN_ADV_STATE
    //scan setting
    blc_ll_initScanning_module(mac_public);
    blc_hci_le_setEventMask_cmd(HCI_LE_EVT_MASK_ADVERTISING_REPORT);
    blc_hci_registerControllerEventHandler(controller_event_callback);

#if 1  //report all adv
    blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,
                              OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
#else //report adv only in whitelist
    ll_whiteList_reset();
    u8 test_adv[6] = {0x33, 0x33, 0x33, 0x33, 0x33, 0x33};
    ll_whiteList_add(BLE_ADDR_PUBLIC, test_adv);
    blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,
                              OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_WL);

#endif
    blc_ll_addScanningInAdvState();  //add scan in adv state
#endif

    //set rf power index, user must set it after every suspend wakeup, cause relative setting will be reset in suspend
    user_set_rf_power(0, 0, 0);
    bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_EXIT, &user_set_rf_power);


    //ble event call back
    bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
    bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &ble_remote_terminate);
    bls_app_registerEventCallback (BLT_EV_FLAG_RX_DATA_ABANDOM, &ble_exception_data_abandom);


    bls_app_registerEventCallback (BLT_EV_FLAG_CONN_PARA_REQ, &task_conn_update_req);
    bls_app_registerEventCallback (BLT_EV_FLAG_CONN_PARA_UPDATE, &task_conn_update_done);


#if (BLE_REMOTE_OTA_ENABLE)
    //bls_ota_set_fwSize_and_fwBootAddr(64, 0x40000);
    extern u8 mcuBootAddrGet(void);
    u8 bootAddr = mcuBootAddrGet();
    u32 baseAddr = (bootAddr) ? 0 : NV_ADDR_FOR_OTA;
    bls_ota_set_fwSize_and_fwBootAddr(256, baseAddr);

    ////////////////// OTA relative ////////////////////////
    bls_ota_clearNewFwDataArea(); //must
    bls_ota_registerStartCmdCb(app_enter_ota_mode);
    //bls_ota_registerResultIndicateCb(app_debug_ota_result);  //debug
#endif
    ///////////////////// Power Management initialization///////////////////
    bls_pm_setSuspendMask(SUSPEND_DISABLE);
}

#endif
