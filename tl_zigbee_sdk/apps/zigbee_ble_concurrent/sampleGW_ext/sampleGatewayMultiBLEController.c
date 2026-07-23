/********************************************************************************************************
 * @file    sampleGatewayMultiBLEController.c
 *
 * @brief   This is the source file for sampleGatewayMultiBLEController.c
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
#include "sampleGateway.h"
#include "hci_tr.h"
#include "hci/hci_vendor.h"

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
#define ACL_CONN_MAX_RX_OCTETS          251  //user set value

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
#define ACL_CENTRAL_MAX_TX_OCTETS       251  //user set value
#define ACL_PERIPHR_MAX_TX_OCTETS       27  //user set value

#define ACL_CONN_MAX_TX_OCTETS          251 // !!! max(acl_centarl_max_tx, acl_peripheral_max_tx)

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
 *          ACL Peripheral TX buffer is shared by all peripheral connections to hold LinkLayer RF TX data.
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


/***************************** HCI TX & RX data FIFO allocation, Begin *********************************************/
/**
 * @brief   LE_ACL_Data_Packet_Length, refer to BLE SPEC "7.8.2 LE Read Buffer Size command"
 * usage limitation:
 * 1. only used for BLE controller project
 * 2. must greater than maximum of ACL_PERIPHR_MAX_TX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS
 * 4. can not exceed 252(for maximum tx_octets when DLE used)
 */
#define LE_ACL_DATA_PACKET_LENGTH       ACL_CONN_MAX_TX_OCTETS

#define HCI_RX_ACL_FIFO_SIZE            CALCULATE_HCI_ACL_DATA_FIFO_SIZE(LE_ACL_DATA_PACKET_LENGTH)
#define HCI_RX_ACL_FIFO_NUM             8

#define HCI_TX_FIFO_SIZE                HCI_ACL_IN_FIFO_SIZE(0xFF)
#define HCI_TX_FIFO_NUM                 8

#define HCI_RX_FIFO_SIZE                HCI_ACL_IN_FIFO_SIZE(0xFF)
#define HCI_RX_FIFO_NUM                 8

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


#if (APP_WORKAROUND_TX_FIFO_4K_LIMITATION_EN && (ACL_CENTRAL_MAX_TX_OCTETS > 230 || ACL_PERIPHR_MAX_TX_OCTETS > 230))
u8  app_acl_cache_Txfifo[260*32] = {0};
#endif

/***************************** HCI TX & RX data FIFO allocation, Begin *********************************************/
//_attribute_iram_noinit_data_
u8  app_hci_rxfifo[HCI_RX_FIFO_SIZE * HCI_RX_FIFO_NUM] = {0};
//_attribute_iram_noinit_data_
u8  app_hci_txfifo[HCI_TX_FIFO_SIZE * HCI_TX_FIFO_NUM] = {0};
//_attribute_iram_noinit_data_
u8  app_hci_rxAclfifo[HCI_RX_ACL_FIFO_SIZE * HCI_RX_ACL_FIFO_NUM] = {0};

/****************************** HCI TX & RX data FIFO allocation, ENd *********************************************/

u8 g_ble_txPowerSet = RF_POWER_P3dBm;

unsigned char app_vendor_cb(u8 pCmdparaLen, u8 opCode_ocf,hci_vendor_CmdParams_t* pCmd, hci_vendor_EndStatusParam_t *pRetParam)
{
    u8 re_length;
    re_length = 0;
    switch(opCode_ocf) {//1byte
        case 0x03:  //for zigbee use
            //
            u16 cmd_type =  pCmd[1];
            cmd_type <<= 8;
            cmd_type |= pCmd[2];

            u16 payloadLen = pCmd[3];
            payloadLen <<= 8;
            payloadLen |= pCmd[4];

            extern int zb_ble_hci_cmd_handler(u16 clusterId, u8 len, u8 *payload);
            zb_ble_hci_cmd_handler(cmd_type, payloadLen, &(pCmd[6]));

            re_length = 0xff;

            break;
        default:
            //TODO
            break;
    }
    return re_length;
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

    blc_ll_configLegacyAdvEnableStrategy(LEG_ADV_EN_STRATEGY_3);

    blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS, ACL_PERIPHR_MAX_TX_OCTETS);

    /* all ACL connection share same RX FIFO */
    blc_ll_initAclConnRxFifo(app_acl_rx_fifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);
#if ACL_CENTRAL_MAX_NUM
    /* ACL Central TX FIFO */
    blc_ll_initAclCentralTxFifo(app_acl_cen_tx_fifo, ACL_CENTRAL_TX_FIFO_SIZE, ACL_CENTRAL_TX_FIFO_NUM, ACL_CENTRAL_MAX_NUM);
#endif
    /* ACL Peripheral TX FIFO */
    blc_ll_initAclPeriphrTxFifo(app_acl_per_tx_fifo, ACL_PERIPHR_TX_FIFO_SIZE, ACL_PERIPHR_TX_FIFO_NUM, ACL_PERIPHR_MAX_NUM);

#if (APP_WORKAROUND_TX_FIFO_4K_LIMITATION_EN && (ACL_CENTRAL_MAX_TX_OCTETS > 230 || ACL_PERIPHR_MAX_TX_OCTETS > 230))
    /* extend TX FIFO size for MAX_TX_OCTETS > 230 if user want use 16 or 32 FIFO */
    blc_ll_initAclConnCacheTxFifo(app_acl_cache_Txfifo, 260, 32);
#endif

    blc_ll_setMaxConnectionNumber(ACL_CENTRAL_MAX_NUM, ACL_PERIPHR_MAX_NUM);
    blc_ll_setAclCentralBaseConnectionInterval(CONN_INTERVAL_10MS);
    blc_ll_setCreateConnectionTimeout(50000);//[!!!important]

    rf_set_power_level_index (g_ble_txPowerSet);
    //////////// LinkLayer Initialization  End /////////////////////////

    //////////// HCI Initialization  Begin /////////////////////////
    /* HCI RX FIFO */
   if (blc_ll_initHciRxFifo(app_hci_rxfifo, HCI_RX_FIFO_SIZE, HCI_RX_FIFO_NUM) != BLE_SUCCESS) {  while(1); }
   /* HCI TX FIFO */
   if (blc_ll_initHciTxFifo(app_hci_txfifo, HCI_TX_FIFO_SIZE, HCI_TX_FIFO_NUM) != BLE_SUCCESS) { while(1); }

   /* HCI RX ACL FIFO (host to controller)*/
   if(blc_ll_initHciAclDataFifo(app_hci_rxAclfifo, HCI_RX_ACL_FIFO_SIZE, HCI_RX_ACL_FIFO_NUM) != BLE_SUCCESS) { while(1); }

   /* HCI Data && Event */
    blc_hci_registerControllerDataHandler (blc_hci_sendACLData2Host);
    blc_hci_registerControllerEventHandler(blc_hci_send_data); //controller hci event to host all processed in this func

    //bluetooth event
    blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE);
    //bluetooth low energy(LE) event, all enable
    blc_hci_le_setEventMask_cmd( 0xFFFFFFFF );
    blc_hci_le_setEventMask_2_cmd( 0x7FFFFFFF );

    blc_hci_le_setEventMask_cmd( 0x0000001F );
    blc_hci_le_setEventMask_2_cmd( 0x0 );

    //////////// HCI Initialization  End /////////////////////////

//    blc_ll_setAutoExchangeDataLengthEnable(0);

    u32 error_code = blc_contr_checkControllerInitialization();
    if (error_code != INIT_SUCCESS) {
        /* It's recommended that user set some UI alarm to know the exact error, e.g. LED shine, print log */
        //write_log32(0x88880000 | error_code);
        tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] Controller INIT ERROR", &error_code, 1);
        while(1);
    }

    /* HCI Transport initialization */
    HCI_TransportInit();

    blt_hci_vendor_setFuVendorCallback(app_vendor_cb);
    tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] BQB controller Init", 0, 0);

    blc_att_holdAttributeResponsePayloadDuringPairingPhase(1);

#if (BLE_LATENCY_WORK_MODE)
    blc_ll_initPowerManagement_module();
    blc_pm_setSleepMask(PM_SLEEP_LEG_ADV | PM_SLEEP_LEG_SCAN | PM_SLEEP_ACL_PERIPHR | PM_SLEEP_ACL_CENTRAL);
    bls_pm_disableHardwarePm(1);
#endif
}


int blt_sdk_main_loop(void)
{
    ////////////////////////////////////// BLE entry /////////////////////////////////
    HCI_TransportPoll();

    blc_sdk_main_loop();

    return 0; //must return 0 due to SDP flow
}

