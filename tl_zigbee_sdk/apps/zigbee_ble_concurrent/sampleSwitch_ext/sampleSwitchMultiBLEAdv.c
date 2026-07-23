/********************************************************************************************************
 * @file    sampleSwitchMultiBLEAdv.c
 *
 * @brief   This is the source file for sampleSwitchMultiBLEAdv
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
#include "sampleSwitch.h"
#include "device_manage.h"
#include "zigbee_ble_switch.h"
#include "controller/ll/adv/leg_adv.h"

_attribute_ble_data_retention_      int central_smp_pending = 0;        // SMP: security & encryption;
_attribute_data_retention_  unsigned int  tlk_flash_mid = 0;
_attribute_data_retention_  unsigned int  tlk_flash_vendor = 0;
_attribute_data_retention_  unsigned char tlk_flash_capacity;
_attribute_data_retention_ u32 flash_sector_mac_address = FLASH_ADDR_OF_MAC_ADDR_2M;

_attribute_data_retention_  u32 task_begin_tick;
_attribute_data_retention_  u8 ble_no_task_deepsleep;

#define TASK_IDLE_ENTER_DEEP_TIME           60  //60 s

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
#define PERIPHR_ATT_RX_MTU              23  //user set value

#define PERIPHR_L2CAP_BUFF_SIZE         CAL_L2CAP_BUFF_SIZE(PERIPHR_ATT_RX_MTU) //user can not change !!!

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

/**
 * @brief   ACL RX buffer, shared by all connections to hold LinkLayer RF RX data.
 *          user should define and initialize this buffer if either ACL Central or ACL Peripheral is used.
 */
_attribute_ble_data_retention_  u8  app_acl_rx_fifo[ACL_RX_FIFO_SIZE * ACL_RX_FIFO_NUM] = {0};

/**
 * @brief   ACL Peripheral TX buffer, shared by all peripheral connections to hold LinkLayer RF TX data.
 *          ACL Peripheral TX buffer should be defined only when ACl connection peripheral role is used.
 */
_attribute_ble_data_retention_  u8  app_acl_per_tx_fifo[ACL_PERIPHR_TX_FIFO_SIZE * ACL_PERIPHR_TX_FIFO_NUM * ACL_PERIPHR_MAX_NUM] = {0};

/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/


/**
 * @brief   L2CAP RX Data buffer for ACL Peripheral
 */
_attribute_ble_data_retention_  u8 app_per_l2cap_rx_buf[ACL_PERIPHR_MAX_NUM * PERIPHR_L2CAP_BUFF_SIZE];

/**
 * @brief   L2CAP TX Data buffer for ACL Peripheral
 *          GATT server on ACL Peripheral use this buffer.
 */
_attribute_ble_data_retention_  u8 app_per_l2cap_tx_buf[ACL_PERIPHR_MAX_NUM * PERIPHR_L2CAP_BUFF_SIZE];
/***************** ACL connection L2CAP RX & TX data Buffer allocation, End ****************************************/

u8 g_ble_txPowerSet = RF_POWER_P3dBm;

/**
 * @brief   BLE Advertising data
 */
const u8 tbl_advData[] = {
     14, DT_COMPLETE_LOCAL_NAME,                'm','u','l','t','i','-','-','s','w','i','t','c','h',
     2,  DT_FLAGS,                              0x05,                   // BLE limited discoverable mode and BR/EDR not supported
     3,  DT_APPEARANCE,                         0x80, 0x01,             // 384, Generic Remote Control, Generic category
     5,  DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID, 0x12, 0x18, 0x0F, 0x18, // incomplete list of service class UUIDs (0x1812, 0x180F)
};

/**
 * @brief   BLE Scan Response Packet data
 */
const u8 tbl_scanRsp [] = {
     14, DT_COMPLETE_LOCAL_NAME,                'm','u','l','t','i','-','-','s','w','i','t','c','h',
};


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

    if (h &HCI_FLAG_EVENT_BT_STD) {       //Controller HCI event
        u8 evtCode = h & 0xff;

        //------------ disconnect -------------------------------------
        if (evtCode == HCI_EVT_DISCONNECTION_COMPLETE) {  //connection terminate

        } else if (evtCode == HCI_EVT_LE_META) {  //LE Event
            u8 subEvt_code = p[0];
            //------hci le event: le connection complete event---------------------------------
            if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_COMPLETE) {  // connection complete

            } else if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT) {
                //after controller is set to scan state, it will report all the adv packet it received by this event
            } else if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE) {

            } else if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_SET_TERMINATED) {
                printf("Recv conn request and the adv stop....\n");
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

    switch (event) {
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

        }
        break;

        case GAP_EVT_SMP_CONN_ENCRYPTION_DONE:
        {

        }
        break;

        case GAP_EVT_SMP_SECURITY_PROCESS_DONE:
        {

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

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void  app_set_kb_wakeup (u8 e, u8 *p, int n)
{
    (void)e;
    (void)p;
    (void)n;
    #if (PM_ENABLE)
        /* suspend time > 80ms.add GPIO wake_up */
        if(((u32)(blc_pm_getWakeupSystemTick() - clock_time())) > 80 * SYSTEM_TIMER_TICK_1MS){
            app_pm_wakeupPinCfg();
            blc_pm_setWakeupSource(PM_WAKEUP_PAD);  //GPIO PAD wake_up
        }
    #endif
}


/** newadd
 * @brief       user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]   none
 * @return      none
 */
void user_ble_normal_init(void){
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
    //////////// LinkLayer Initialization  End /////////////////////////

    //////////// HCI Initialization  Begin /////////////////////////
    blc_hci_registerControllerEventHandler(app_controller_event_callback); //controller hci event to host all processed in this func

    //bluetooth event
    blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE);

    //bluetooth low energy(LE) event
    blc_hci_le_setEventMask_cmd(        HCI_LE_EVT_MASK_CONNECTION_COMPLETE  \
                                    |   HCI_LE_EVT_MASK_ADVERTISING_REPORT \
                                    |   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE);
    //////////// HCI Initialization  End /////////////////////////

    /* Check if any Stack(Controller & Host) Initialization error after all BLE initialization done.
     * attention: user can not delete !!! */
    u32 error_code1 = blc_contr_checkControllerInitialization();
    u32 error_code2 = blc_host_checkHostInitialization();
    if (error_code1 != INIT_SUCCESS || error_code2 != INIT_SUCCESS) {
        /* It's recommended that user set some UI alarm to know the exact error, e.g. LED shine, print log */
        tlkapi_printf(APP_LOG_EN, "[APP][INI] Stack INIT ERROR 0x%04x, 0x%04x", error_code1, error_code2);
        while(1);
    }

//////////////////////////// BLE stack Initialization  End //////////////////////////////////



//////////////////////////// User Configuration for BLE application ////////////////////////////
    blc_ll_configLegacyAdvEnableStrategy(LEG_ADV_EN_STRATEGY_3);
    blc_ll_setAdvData(tbl_advData, sizeof(tbl_advData));
    blc_ll_setScanRspData(tbl_scanRsp, sizeof(tbl_scanRsp));
    blc_ll_setAdvParam(ADV_INTERVAL_200MS, ADV_INTERVAL_200MS, ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, 0, NULL, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
    blc_ll_setAdvEnable(BLC_ADV_ENABLE);  //ADV enable
    printf("enable adv in init...\n");

#if PA_ENABLE
    g_ble_txPowerSet = RF_POWER_P0dBm;
    ble_rf_pa_init(0, PA_TX, PA_RX);    //for ble
#endif

    rf_set_power_level_index (g_ble_txPowerSet);

#if (PM_ENABLE)
    blc_ll_initPowerManagement_module();
    blc_pm_setSleepMask(PM_SLEEP_LEG_ADV | PM_SLEEP_ACL_PERIPHR);

    #if (PM_DEEPSLEEP_RETENTION_ENABLE)
        blc_pm_setDeepsleepRetentionEnable(PM_DeepRetn_Enable);
        blc_pm_setDeepsleepRetentionThreshold(95);

        #if defined(MCU_CORE_TL721X)
            #if VOLTAGE_DETECT_ENABLE
                blc_pm_setDeepsleepRetentionEarlyWakeupTiming(1275); //enable voltage detect
            #else
                blc_pm_setDeepsleepRetentionEarlyWakeupTiming(930); //for tl721x 120M
            #endif
            blc_pm_setDeepsleepRetentionType(DEEPSLEEP_MODE_RET_SRAM_LOW64K);
        #elif defined(MCU_CORE_TL321X)
            #if VOLTAGE_DETECT_ENABLE
                blc_pm_setDeepsleepRetentionEarlyWakeupTiming(1300); //enable voltage detect
            #else
                blc_pm_setDeepsleepRetentionEarlyWakeupTiming(1050); //for tl321x 48M
            #endif
            blc_pm_setDeepsleepRetentionType(DEEPSLEEP_MODE_RET_SRAM_LOW64K);
        #elif defined(MCU_CORE_TL323X)
            #if VOLTAGE_DETECT_ENABLE
                blc_pm_setDeepsleepRetentionEarlyWakeupTiming(1980); //enable voltage detect
            #else
                blc_pm_setDeepsleepRetentionEarlyWakeupTiming(1180); //for tl323x 48M
            #endif
            blc_pm_setDeepsleepRetentionType(DEEPSLEEP_MODE_RET_SRAM_LOW96K);
        #elif defined(MCU_CORE_TL521X)
            #if VOLTAGE_DETECT_ENABLE
                blc_pm_setDeepsleepRetentionEarlyWakeupTiming(1300); //enable voltage detect
            #else
                blc_pm_setDeepsleepRetentionEarlyWakeupTiming(1050); //for tl521x 72M
            #endif
            blc_pm_setDeepsleepRetentionType(DEEPSLEEP_MODE_RET_SRAM_LOW64K);
        #endif
    #else
        blc_pm_setDeepsleepRetentionEnable(PM_DeepRetn_Disable);
    #endif

    blc_ll_registerTelinkControllerEventCallback(BLT_EV_FLAG_SLEEP_ENTER, &app_set_kb_wakeup);
#endif

    tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] acl connection demo init", 0, 0);
    task_begin_tick = clock_time();
}


void user_ble_init(bool isRetention)
{
    if (!isRetention) {
        user_ble_normal_init();
    } else {
        blc_ll_initBasicMCU();
        blc_ll_recoverDeepRetention();
        /* switch tx power for ble mode */
        ZB_RADIO_TX_POWER_SET(g_ble_txPowerSet);
    }
}


void app_process_power_management(void)
{
#if (PM_ENABLE)
    //Log needs to be output ASAP, and UART invalid after suspend. So Log disable sleep.
    //User tasks can go into suspend, but no deep sleep. So we use manual latency.
    if (tlkapi_debug_isBusy()) {
        blc_pm_setSleepMask(PM_SLEEP_DISABLE);
    } else {
        int user_task_flg = g_switchAppCtx.keyPressedCnt;

        blc_pm_setSleepMask(PM_SLEEP_LEG_ADV | PM_SLEEP_LEG_SCAN | PM_SLEEP_ACL_PERIPHR | PM_SLEEP_ACL_CENTRAL);

        if (user_task_flg) {
            bls_pm_setManualLatency(0);
        }
    }
#endif
}

int blt_sdk_main_loop(void){
    ////////////////////////////////////// BLE entry /////////////////////////////////
    blc_sdk_main_loop();

    ////////////////////////////////////// PM entry /////////////////////////////////
    app_process_power_management();

    return 0; //must return 0 due to SDP flow
}

/**
 * @brief      power management code for application
 * @param      none
 * @return     none
 */
_attribute_ram_code_ u32 blt_pm_proc(void)
{
    if ((ble_no_task_deepsleep == 2) && (APP_BLE_STATE_IDLE())) {  //Terminate OK
        return 1;
    }

    if (!APP_BLE_STATE_IDLE() && clock_time_exceed(task_begin_tick , TASK_IDLE_ENTER_DEEP_TIME * 1000000)) {
        blc_ll_setScanEnable (BLC_SCAN_DISABLE, DUP_FILTER_DISABLE);
        blc_ll_setAdvEnable(BLC_ADV_DISABLE);  //ADV disable

        for (u8 i = 0; i < DEVICE_CHAR_INFO_MAX_NUM; i++) {
            if (conn_dev_list[i].conn_state == 1) {
                blc_ll_disconnect(conn_dev_list[i].conn_handle, HCI_ERR_REMOTE_USER_TERM_CONN);
            }
        }
        ble_no_task_deepsleep = 1;
    }

    if (ble_no_task_deepsleep == 1) {
        if (!blc_ll_getCurrentConnectionNumber()) {
            ble_no_task_deepsleep = 2;
        }
    }

    return 0;
}

void ble_advertiseTickUpdate(void)
{
    task_begin_tick = clock_time();
}
