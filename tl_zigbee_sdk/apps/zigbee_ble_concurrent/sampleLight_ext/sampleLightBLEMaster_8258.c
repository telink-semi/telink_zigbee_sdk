/********************************************************************************************************
 * @file    sampleLightBLEMaster_8258.c
 *
 * @brief   This is the source file for sampleLightBLEMaster_8258
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

#include "../../proj/tl_common.h"
#include <stack/ble/ble.h>
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"

#include "zigbee_ble_switch.h"

#if (__PROJECT_TL_DIMMABLE_LIGHT__ && BLE_MASTER_ROLE_ENABLE)


static void ble_master_updateIndHandler(void);
#define BLE_DEBUG    1

#if BLE_DEBUG
    #define LED_CONNECTED       LED_G     //LED_R   //GPIO_PD3
    #define LED_KEY_REPORT      LED_R     //GPIO_PD4

    #define DBG_IO_INIT      do{                                            \
                                    gpio_set_func(LED_CONNECTED, AS_GPIO);  \
                                    gpio_set_output_en(LED_CONNECTED, 1);   \
                                    gpio_write(LED_CONNECTED, 0);           \
                                    gpio_set_func(LED_KEY_REPORT, AS_GPIO);  \
                                    gpio_set_output_en(LED_KEY_REPORT, 1);   \
                                    gpio_write(LED_KEY_REPORT, 0);           \
                               }while(0)

    #define DBG_LED_CONNECTED       gpio_write(LED_CONNECTED, 1)
    #define DBG_LED_DISCONNECTED    gpio_write(LED_CONNECTED, 0)

    #define DBG_LED_KEY_REPORT  gpio_toggle(LED_KEY_REPORT)
    u8 whiteListAddr[] = {0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb};
#else
    #define DBG_IO_INIT
    #define DBG_LED_CONNECTED
    #define DBG_LED_DISCONNECTED
    #define DBG_LED_KEY_REPORT
#endif

int dongle_pairing_enable = 1;//0;
int dongle_unpair_enable = 0;

#define     TELINK_UNPAIR_KEYVALUE      0xFF  //conn state, unpair

#define CHAR_HANDLE_MAX                 10

// connection character device information
typedef struct {
    u16 conn_handle;
    u8 conn_state;
    u8 mac_adrType;
    u8 mac_addr[6];
    u8 char_handle[CHAR_HANDLE_MAX];
}dev_char_info_t;

extern dev_char_info_t cur_conn_device;

typedef struct {
    u8  num;
    u8  property;
    u16 handle;
    u16 uuid;
    u16 ref;
} att_db_uuid16_t;          //8-byte


typedef struct {
    u8  num;
    u8  property;
    u16 handle;
    u8  uuid[16];
} att_db_uuid128_t;         //20-byte

_attribute_data_retention_  unsigned int  tlk_flash_mid = 0;
_attribute_data_retention_  unsigned int  tlk_flash_vendor = 0;
_attribute_data_retention_  unsigned char tlk_flash_capacity;

//------------------ att ----------------------------//

const u8 my_MicUUID[16]     = {TELINK_MIC_DATA};
const u8 my_SpeakerUUID[16] = {TELINK_SPEAKER_DATA};
const u8 my_OtaUUID[16]     = {TELINK_SPP_DATA_OTA};
const u8 my_SppS2CUUID[16]      = {TELINK_SPP_DATA_SERVER2CLIENT};
const u8 my_SppC2SUUID[16]      = {TELINK_SPP_DATA_CLIENT2SERVER};

u8 read_by_type_req_uuid[16] = {};
u8 read_by_type_req_uuidLen;

u16 current_read_req_handle;


void host_att_set_current_readByTypeReq_uuid(u8 *uuid, u8 uuid_len)
{
    read_by_type_req_uuidLen = uuid_len;
    memcpy(read_by_type_req_uuid, uuid, uuid_len);
}

u8 *p_att_response = 0;

volatile u32 host_att_req_busy = 0;

int host_att_client_handler (u16 connHandle, u8 *p)
{
    att_readByTypeRsp_t *p_rsp = (att_readByTypeRsp_t *) p;
    if (p_att_response) {
        if ((connHandle & 7) == (host_att_req_busy & 7) && p_rsp->chanId == 0x04 &&
                (p_rsp->opcode == 0x01 || p_rsp->opcode == ((host_att_req_busy >> 16) | 1))) {
            memcpy (p_att_response, p, 32);
            host_att_req_busy = 0;
        }
    }
    return 0;
}


typedef int (*host_att_idle_func_t) (void);
host_att_idle_func_t host_att_idle_func = 0;

int host_att_register_idle_func (void *p)
{
    if (host_att_idle_func) {
        return 1;
    }

    host_att_idle_func = p;
    return 0;
}

int host_att_response (void)
{
    return host_att_req_busy == 0;
}

int host_att_service_wait_event (u16 handle, u8 *p, u32 timeout)
{
    host_att_req_busy = handle | (p[6] << 16);
    p_att_response = p;
    blm_push_fifo (handle, p);

    u32 t = clock_time ();
    while (!clock_time_exceed (t, timeout)) {
        if (host_att_response ()) {
            return 0;
        }
        if (host_att_idle_func) {
            if (host_att_idle_func ()) {
                break;
            }
        }
    }
    return 1;
}


ble_sts_t  host_att_discoveryService (u16 handle, att_db_uuid16_t *p16, int n16, att_db_uuid128_t *p128, int n128)
{
    att_db_uuid16_t *ps16 = p16;
    att_db_uuid128_t *ps128 = p128;
    int i16 = 0;
    int i128 = 0;

    ps16->num = 0;
    ps128->num = 0;

    // char discovery: att_read_by_type
        // hid discovery: att_find_info
    u8  dat[32];
    u16 s = 1;
    u16 uuid = GATT_UUID_CHARACTER;
    do {

        att_req_read_by_type (dat, s, 0xffff, (u8 *)&uuid, 2);
        if (host_att_service_wait_event(handle, dat, 1000000)) {
            return  GATT_ERR_SERVICE_DISCOVERY_TIMEOUT;         //timeout
        }

        // process response data
        att_readByTypeRsp_t *p_rsp = (att_readByTypeRsp_t *) dat;
        if (p_rsp->opcode != ATT_OP_READ_BY_TYPE_RSP) {
            break;
        }

        if (p_rsp->datalen == 21) {  //uuid128
            s = p_rsp->data[3] + p_rsp->data[4] * 256;
            if (i128 < n128) {
                p128->property = p_rsp->data[2];
                p128->handle = s;
                memcpy (p128->uuid, p_rsp->data + 5, 16);
                i128++;
                p128++;
            }
        } else if (p_rsp->datalen == 7){  //uuid16
            u8 *pd = p_rsp->data;
            while (p_rsp->l2capLen > 7) {
                s = pd[3] + pd[4] * 256;
                if (i16 < n16) {
                    p16->property = pd[2];
                    p16->handle = s;
                    p16->uuid = pd[5] | (pd[6] << 8);
                    p16->ref = 0;
                    i16 ++;
                    p16++;
                }
                p_rsp->l2capLen -= 7;
                pd += 7;
            }
        }
    } while (1);

    ps16->num = i16;
    ps128->num = i128;

    //--------- use att_find_info to find the reference property for hid ----------
    p16 = ps16;
    for (int i=0; i<i16; i++) {
        if (p16->uuid == CHARACTERISTIC_UUID_HID_REPORT) {

            att_req_find_info (dat, p16->handle, 0xffff);
            if (host_att_service_wait_event(handle, dat, 1000000)) {
                return  GATT_ERR_SERVICE_DISCOVERY_TIMEOUT;         //timeout
            }

            att_findInfoRsp_t *p_rsp = (att_findInfoRsp_t *) dat;
            if (p_rsp->opcode == ATT_OP_FIND_INFO_RSP && p_rsp->format == 1) {
                int n = p_rsp->l2capLen - 2;
                u8 *pd = p_rsp->data;
                while (n > 0) {
                    if ((pd[2]==U16_LO(GATT_UUID_CHARACTER) && pd[3]==U16_HI(GATT_UUID_CHARACTER)) ||
                        (pd[2]==U16_LO(GATT_UUID_PRIMARY_SERVICE) && pd[3]==U16_HI(GATT_UUID_PRIMARY_SERVICE))) {
                        break;
                    }

                    if (pd[2]==U16_LO(GATT_UUID_REPORT_REF) && pd[3]==U16_HI(GATT_UUID_REPORT_REF)) {
                    //-----------       read attribute ----------------

                        att_req_read (dat, pd[0]);
                        if (host_att_service_wait_event(handle, dat, 1000000)) {
                                return  GATT_ERR_SERVICE_DISCOVERY_TIMEOUT;         //timeout
                        }

                        att_readRsp_t *pr = (att_readRsp_t *) dat;
                        if (pr->opcode == ATT_OP_READ_RSP) {
                            p16->ref = pr->value[0] | (pr->value[1] << 8);
                        }

                        break;
                    }
                    n -= 4;
                    pd += 4;
                }
            }
        } //----- end for if CHARACTERISTIC_UUID_HID_REPORT

        p16++;
    }

    return  BLE_SUCCESS;
}

void app_setCurrentReadReq_attHandle(u16 handle)
{
    current_read_req_handle = handle;
}

u16 app_getCurrentReadReq_attHandle(void)
{
    return current_read_req_handle;
}

MYFIFO_INIT(blt_rxfifo, 64, 8);
MYFIFO_INIT(blt_txfifo, 40, 8);

u8  g_ble_txPowerSet = RF_POWER_INDEX_P3p01dBm;


//-------------------------------------pair--------------------------------//
#if (!BLE_HOST_SMP_ENABLE)

typedef struct{
    u8 manual_pair;
    u8 mac_type;  //addrsss type
    u8 mac[6];
    u32 pair_tick;
}man_pair_t;

man_pair_t blm_manPair;

/* define pair slave max num,
 * if exceed this max num, two methods to process new slave pairing
 * method 1: overwrite the oldest one(telink use this method)
 * method 2: not allow paring unness unpair happened  
 * 
 * */
#define USER_PAIR_SLAVE_MAX_NUM       1  //telink use max 1


typedef struct {
    u8 bond_mark;
    u8 adr_type;
    u8 address[6];
} macAddr_t;


typedef struct {
    u32 bond_flash_idx[USER_PAIR_SLAVE_MAX_NUM];  //mark paired slave mac address in flash
    macAddr_t bond_device[USER_PAIR_SLAVE_MAX_NUM];  //macAddr_t already defined in ble stack
    u8 curNum;
} user_salveMac_t;


/* flash erase strategy:
 * never erase flash when dongle is working, for flash sector erase takes too much time(20-100 ms)
 * this will lead to timing err
 * so we only erase flash at initiation,  and with mark 0x00 for no use symbol
 */

#define ADR_BOND_MARK       0x5A
#define ADR_ERASE_MARK      0x00
/* flash stored mac address struct:
 * every 8 bytes is a address area: first one is mark, second no use, third - eighth is 6 byte address
 *     0     1           2 - 7
 * | mark |     |    mac_address     |
 * mark = 0xff, current area is invalid, pair info end
 * mark = 0x01, current area is valid, load the following mac_address,
 * mark = 0x00, current area is invalid (previous valid address is erased)
 */

int user_bond_slave_flash_cfg_idx;  //new mac address stored flash idx


user_salveMac_t user_tbl_slaveMac;  //slave mac bond table


void user_tbl_slave_mac_delete_by_index(int index)  //remove the oldest adr in slave mac table
{
    //erase the oldest with ERASE_MARK
    u8 delete_mark = ADR_ERASE_MARK;
    flash_write_page (CFG_NV_START_FOR_BLE + user_tbl_slaveMac.bond_flash_idx[index], 1, &delete_mark);

    for (int i=index; i<user_tbl_slaveMac.curNum - 1; i++) {  //move data
        user_tbl_slaveMac.bond_flash_idx[i] = user_tbl_slaveMac.bond_flash_idx[i+1];
        memcpy( (u8 *)&user_tbl_slaveMac.bond_device[i], (u8 *)&user_tbl_slaveMac.bond_device[i+1], 8 );
    }

    user_tbl_slaveMac.curNum --;
}


int user_tbl_slave_mac_add(u8 adr_type, u8 *adr)  //add new mac address to table
{
    u8 add_new = 0;
    if (user_tbl_slaveMac.curNum >= USER_PAIR_SLAVE_MAX_NUM) { //salve mac table is full
        //slave mac max, telink use  method 1: overwrite the oldest one
        user_tbl_slave_mac_delete_by_index(0);  //overwrite, delete index 0 (oldest) of table
        add_new = 1;  //add new
    } else {//slave mac table not full
        add_new = 1;
    }

    if (add_new) {
        user_tbl_slaveMac.bond_device[user_tbl_slaveMac.curNum].bond_mark = ADR_BOND_MARK;
        user_tbl_slaveMac.bond_device[user_tbl_slaveMac.curNum].adr_type = adr_type;
        memcpy(user_tbl_slaveMac.bond_device[user_tbl_slaveMac.curNum].address, adr, 6);

        user_bond_slave_flash_cfg_idx += 8;  //inc flash idx to get the new 8 bytes area
        flash_write_page (CFG_NV_START_FOR_BLE + user_bond_slave_flash_cfg_idx, 8, (u8 *)&user_tbl_slaveMac.bond_device[user_tbl_slaveMac.curNum] );

        user_tbl_slaveMac.bond_flash_idx[user_tbl_slaveMac.curNum] = user_bond_slave_flash_cfg_idx;  //mark flash idx
        user_tbl_slaveMac.curNum++;

        return 1;  //add OK
    }

    return 0;
}

/* search mac address in the bond slave mac table:
 * when slave paired with dongle, add this addr to table
 * re_poweron slave, dongle will search if this AdvA in slave adv pkt is in this table
 * if in, it will connect slave directly
 * this function must in ramcode
 */
int user_tbl_slave_mac_search(u8 adr_type, u8 * adr)
{
    for (int i=0; i< user_tbl_slaveMac.curNum; i++) {
        if ( user_tbl_slaveMac.bond_device[i].adr_type == adr_type && \
            !memcmp(user_tbl_slaveMac.bond_device[i].address ,adr, 6)) {  //match

            return (i+1);  //return index+1( 1 - USER_PAIR_SLAVE_MAX_NUM)
        }
    }

    return 0;
}

//when rc trigger unpair, use this function to delete the slave mac
int user_tbl_slave_mac_delete_by_adr(u8 adr_type, u8 *adr)  //remove adr from slave mac table by adr
{
    for (int i=0;i<user_tbl_slaveMac.curNum;i++) {
        if ( user_tbl_slaveMac.bond_device[i].adr_type == adr_type && \
            !memcmp(user_tbl_slaveMac.bond_device[i].address ,adr, 6)) {  //match

            //erase the match adr
            u8 delete_mark = ADR_ERASE_MARK;
            flash_write_page (CFG_NV_START_FOR_BLE + user_tbl_slaveMac.bond_flash_idx[i], 1, &delete_mark);

            for (int j=i; j< user_tbl_slaveMac.curNum - 1;j++) { //move data
                user_tbl_slaveMac.bond_flash_idx[j] = user_tbl_slaveMac.bond_flash_idx[j+1];
                memcpy((u8 *)&user_tbl_slaveMac.bond_device[j], (u8 *)&user_tbl_slaveMac.bond_device[j+1], 8);
            }

            user_tbl_slaveMac.curNum --;
            return 1; //delete OK
        }
    }

    return 0;
}

void user_tbl_slave_mac_delete_all(void)  //delete all the  adr in slave mac table
{
    u8 delete_mark = ADR_ERASE_MARK;
    for (int i=0; i< user_tbl_slaveMac.curNum; i++){
        flash_write_page (CFG_NV_START_FOR_BLE + user_tbl_slaveMac.bond_flash_idx[i], 1, &delete_mark);
        memset( (u8 *)&user_tbl_slaveMac.bond_device[i], 0, 8);
        //user_tbl_slaveMac.bond_flash_idx[i] = 0;  //do not  concern
    }

    user_tbl_slaveMac.curNum = 0;
}

/* unpair cmd proc
 * when press unpair button on dongle or send unpair cmd from slave
 * dongle will call this function to process current unpair cmd
 */
void user_tbl_salve_mac_unpair_proc(void)
{
    //telink will delete all adr when unpair happens, you can change to your own strategy
    //slaveMac_curConnect is for you to use
    user_tbl_slave_mac_delete_all();
}

u8 adbg_flash_clean;
#define DBG_FLASH_CLEAN   0

/*
 * when flash stored too many addr, it may exceed a sector max(4096), so we must clean this sector
 * and rewrite the valid addr at the beginning of the sector(0x11000)
 */
void    user_bond_slave_flash_clean (void)
{
#if DBG_FLASH_CLEAN
    if (user_bond_slave_flash_cfg_idx < 8*8)  //debug, max 8 area, then clean flash
#else
    if (user_bond_slave_flash_cfg_idx < 2048)  //max 2048/8 = 256,rest available sector is big enough, no need clean
#endif
    {
        return;
    }

    adbg_flash_clean = 1;

    flash_erase_sector (CFG_NV_START_FOR_BLE);

    user_bond_slave_flash_cfg_idx = -8;  //init value for no bond slave mac

    //rewrite bond table at the beginning of 0x11000
    for (int i=0; i< user_tbl_slaveMac.curNum; i++) {
        //u8 add_mark = ADR_BOND_MARK;

        user_bond_slave_flash_cfg_idx += 8;  //inc flash idx to get the new 8 bytes area
        flash_write_page (CFG_NV_START_FOR_BLE + user_bond_slave_flash_cfg_idx, 8, (u8*)&user_tbl_slaveMac.bond_device[i] );

        user_tbl_slaveMac.bond_flash_idx[i] = user_bond_slave_flash_cfg_idx;  //update flash idx
    }
}

void user_master_host_pairing_flash_init(void)
{
    u8 *pf = (u8 *)CFG_NV_START_FOR_BLE;

    //traversing 8 bytes area in sector 0x11000 to find all the valid slave mac adr
    for (user_bond_slave_flash_cfg_idx=0; user_bond_slave_flash_cfg_idx<4096; user_bond_slave_flash_cfg_idx+=8) { 
        if ( pf[user_bond_slave_flash_cfg_idx] == ADR_BOND_MARK ) {  //valid adr
            if (user_tbl_slaveMac.curNum < USER_PAIR_SLAVE_MAX_NUM) {
                user_tbl_slaveMac.bond_flash_idx[user_tbl_slaveMac.curNum] = user_bond_slave_flash_cfg_idx;
                flash_read_page (CFG_NV_START_FOR_BLE + user_bond_slave_flash_cfg_idx, 8, (u8 *)&user_tbl_slaveMac.bond_device[user_tbl_slaveMac.curNum] );
                user_tbl_slaveMac.curNum ++;
            } else { //slave mac in flash more than max, we think it's code bug
                write_reg32(0x9000,0x12345678);  //for debug
                irq_disable();
                while(1);
            }
        } else if (pf[user_bond_slave_flash_cfg_idx] == 0xff) {//end  
            break;
        }
    }

    user_bond_slave_flash_cfg_idx -= 8; //back to the newest addr 8 bytes area flash ixd(if no valid addr, will be -8)

    user_bond_slave_flash_clean ();
}

void user_master_host_pairing_management_init(void)
{
    user_master_host_pairing_flash_init();
}
#endif  //end of !BLE_HOST_SMP_ENABLE


_attribute_ram_code_
void host_pair_unpair_proc(void)
{
#if (!BLE_HOST_SMP_ENABLE)
    if (blm_manPair.manual_pair && clock_time_exceed(blm_manPair.pair_tick, 2000000)) {
        blm_manPair.manual_pair = 0;
    }
#endif

    //terminate and unpair proc
    static int master_disconnect_flag;
    if (dongle_unpair_enable) {
        if (!master_disconnect_flag && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN) {
            if ( blm_ll_disconnect(cur_conn_device.conn_handle, HCI_ERR_REMOTE_USER_TERM_CONN) == BLE_SUCCESS) {
                master_disconnect_flag = 1;
                dongle_unpair_enable = 0;

                #if (BLE_HOST_SMP_ENABLE)
                    tbl_bond_slave_unpair_proc(cur_conn_device.mac_adrType, cur_conn_device.mac_addr); //by telink stack host smp
                #else
                    user_tbl_salve_mac_unpair_proc();
                #endif
            }
        }
    }
    if (master_disconnect_flag && blc_ll_getCurrentState() != BLS_LINK_STATE_CONN) {
        master_disconnect_flag = 0;
    }
}



//-------------------------------------Host--------------------------------//
#define SMP_PENDING                 1   //security management
#define SDP_PENDING                 2   //service discovery

int app_host_smp_sdp_pending = 0;       //security & service discovery


dev_char_info_t cur_conn_device;

////////////////////////////////////////////////////////////////////
u32 host_update_conn_param_req = 0;
u16 host_update_conn_min;
u16 host_update_conn_latency;
u16 host_update_conn_timeout;

u16 final_MTU_size = 23;

int master_connected_led_on = 0;

int master_auto_connect = 0;
int user_manual_paring;

const u8 telink_adv_trigger_paring[] = {5, 0xFF, 0x11, 0x02, 0x01, 0x00};
const u8 telink_adv_trigger_unpair[] = {5, 0xFF, 0x11, 0x02, 0x01, 0x01};

const u8 telink_adv_trigger_paring_8258[] = {7, 0xFF, 0x11, 0x02, 0x01, 0x00, 0x58, 0x82};
const u8 telink_adv_trigger_unpair_8258[] = {7, 0xFF, 0x11, 0x02, 0x01, 0x01, 0x58, 0x82};


#if (BLE_HOST_SIMPLE_SDP_ENABLE)
    int host_att_client_handler (u16 connHandle, u8 *p);
    ble_sts_t  host_att_discoveryService (u16 handle, att_db_uuid16_t *p16, int n16, att_db_uuid128_t *p128, int n128);


    #define ATT_DB_UUID16_NUM       20
    #define ATT_DB_UUID128_NUM      8

    u8  conn_char_handler[8] = {0};

    u8  serviceDiscovery_adr_type;
    u8  serviceDiscovery_address[6];


    extern const u8 my_MicUUID[16];
    extern const u8 my_SpeakerUUID[16];
    extern const u8 my_OtaUUID[16];


    /**
     * @brief       this function serves to find handle of uuid128
     * @param[in]   p - pointer of data attribute
     * @param[in]   uuid - pointer of uuid
     * @return      0 - fail to find handle of uuid128
     *              1 - the handle of uuid128 that find
     */
    u16 blm_att_findHandleOfUuid128 (att_db_uuid128_t *p, const u8 * uuid)
    {
        for (int i=0; i<p->num; i++) {
            if (memcmp (p[i].uuid, uuid, 16) == 0) {
                return p[i].handle;
            }
        }
        return 0;
    }

    /**
     * @brief       this function serves to find handle of uuid16
     * @param[in]   p - pointer of data attribute
     * @param[in]   uuid
     * @param[in]   ref - HID Report
     * @return      0 - fail to find handle of uuid16
     *              1 - the handle of uuid16 that find
     */
    u16 blm_att_findHandleOfUuid16 (att_db_uuid16_t *p, u16 uuid, u16 ref)
    {
        for (int i=0; i<p->num; i++) {
            if (p[i].uuid == uuid && p[i].ref == ref) {
                return p[i].handle;
            }
        }
        return 0;
    }

    void app_service_discovery ()
    {

        att_db_uuid16_t     db16[ATT_DB_UUID16_NUM];
        att_db_uuid128_t    db128[ATT_DB_UUID128_NUM];
        memset (db16, 0, ATT_DB_UUID16_NUM * sizeof (att_db_uuid16_t));
        memset (db128, 0, ATT_DB_UUID128_NUM * sizeof (att_db_uuid128_t));


        if ( IS_CONNECTION_HANDLE_VALID(cur_conn_device.conn_handle) && \
             host_att_discoveryService (cur_conn_device.conn_handle, db16, ATT_DB_UUID16_NUM, db128, ATT_DB_UUID128_NUM) == BLE_SUCCESS)    // service discovery OK
        {
            //int h = current_connHandle & 7;
            conn_char_handler[0] = blm_att_findHandleOfUuid128 (db128, my_MicUUID);         //MIC
            conn_char_handler[1] = blm_att_findHandleOfUuid128 (db128, my_SpeakerUUID);     //Speaker
            conn_char_handler[2] = blm_att_findHandleOfUuid128 (db128, my_OtaUUID);         //OTA


            conn_char_handler[3] = blm_att_findHandleOfUuid16 (db16, CHARACTERISTIC_UUID_HID_REPORT,
                        HID_REPORT_ID_CONSUME_CONTROL_INPUT | (HID_REPORT_TYPE_INPUT<<8));      //consume report

            conn_char_handler[4] = blm_att_findHandleOfUuid16 (db16, CHARACTERISTIC_UUID_HID_REPORT,
                        HID_REPORT_ID_KEYBOARD_INPUT | (HID_REPORT_TYPE_INPUT<<8));             //normal key report

            conn_char_handler[5] = blm_att_findHandleOfUuid16 (db16, CHARACTERISTIC_UUID_HID_REPORT,
                        HID_REPORT_ID_MOUSE_INPUT | (HID_REPORT_TYPE_INPUT<<8));                //mouse report

            //save current service discovery conn address
            serviceDiscovery_adr_type = cur_conn_device.mac_adrType;
            memcpy(serviceDiscovery_address, cur_conn_device.mac_addr, 6);
        }

        app_host_smp_sdp_pending = 0;  //service discovery finish

    }

    #define         HID_HANDLE_CONSUME_REPORT           conn_char_handler[3]
    #define         HID_HANDLE_KEYBOARD_REPORT          conn_char_handler[4]
    #define         HID_HANDLE_MOUSE_REPORT             conn_char_handler[5]
    #define         AUDIO_HANDLE_MIC                    conn_char_handler[0]
#else  //no service discovery

    //need define att handle same with slave
    #define         HID_HANDLE_MOUSE_REPORT
    #define         HID_HANDLE_CONSUME_REPORT           25
    #define         HID_HANDLE_KEYBOARD_REPORT          29
    #define         AUDIO_HANDLE_MIC                    52

#endif



#if (BLE_HOST_SMP_ENABLE)
int app_host_smp_finish (void)  //smp finish callback
{
    #if (BLE_HOST_SIMPLE_SDP_ENABLE)  //smp finish, start sdp
        if (app_host_smp_sdp_pending == SMP_PENDING) {
            //new slave device, should do service discovery again
            if (cur_conn_device.mac_adrType != serviceDiscovery_adr_type || \
                memcmp(cur_conn_device.mac_addr, serviceDiscovery_address, 6)) {
                //app_register_service(&app_service_discovery);
                ble_master_serviceCbRegister(&app_service_discovery);
                app_host_smp_sdp_pending = SDP_PENDING; //service discovery busy
            } else {
                app_host_smp_sdp_pending = 0;  //no need sdp
            }
        }
    #else
        app_host_smp_sdp_pending = 0;  //no sdp
    #endif

    return 0;
}
#endif


int blm_le_adv_report_event_handle(u8 *p)
{
    event_adv_report_t *pa = (event_adv_report_t *)p;
    s8 rssi = pa->data[pa->len];

     //if previous connection smp&sdp not finish, can not create a new connection
    if (app_host_smp_sdp_pending) {
        return 1;
    }

    /****************** Button press or Adv pair packet triggers pair ***********************/
    int master_auto_connect = 0;
    int user_manual_paring = 0;

    //manual paring methods 1: button triggers
    user_manual_paring = dongle_pairing_enable && (rssi > -56);  //button trigger pairing(rssi threshold, short distance)

    //manual paring methods 2: special paring adv data
    if (!user_manual_paring) {  //special adv pair data can also trigger pairing
        user_manual_paring = (memcmp(pa->data, telink_adv_trigger_paring_8258, sizeof(telink_adv_trigger_paring_8258)) == 0) && (rssi > -56);
    }


    #if (BLE_HOST_SMP_ENABLE)
        master_auto_connect = tbl_bond_slave_search(pa->adr_type, pa->mac);
    #else
        //search in slave mac table to find whether this device is an old device which has already paired with master
        master_auto_connect = user_tbl_slave_mac_search(pa->adr_type, pa->mac);
    #endif


    if (master_auto_connect || user_manual_paring) {
        /* send create connection cmd to controller, trigger it switch to initiating state, after this cmd,
         * controller will scan all the adv packets it received but not report to host, to find the specified
         * device(adr_type & mac), then send a connection request packet after 150us, enter to connection state
         * and send a connection complete event(HCI_SUB_EVT_LE_CONNECTION_COMPLETE)
         */
    #if !BLE_DEBUG
        u8 status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS, INITIATE_FP_ADV_SPECIFY,
                                 pa->adr_type, pa->mac, BLE_ADDR_PUBLIC,
                                 CONN_INTERVAL_50MS, CONN_INTERVAL_50MS, 0, CONN_TIMEOUT_4S,
                                 0, 0xFFFF);
    #else
        if (memcmp(pa->mac, whiteListAddr, 6) || pa->adr_type !=0) {
            return 1;
        }
        u8 status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS, INITIATE_FP_ADV_WL,
                                         pa->adr_type, pa->mac, BLE_ADDR_PUBLIC,
                                         CONN_INTERVAL_50MS, CONN_INTERVAL_50MS, 0, CONN_TIMEOUT_4S,
                                         0, 0xFFFF);
    #endif
        if (status == BLE_SUCCESS) {  //create connection success
            #if (!BLE_HOST_SMP_ENABLE)
                if (user_manual_paring && !master_auto_connect) {  //manual pair
                    blm_manPair.manual_pair = 1;
                    blm_manPair.mac_type = pa->adr_type;
                    memcpy(blm_manPair.mac, pa->mac, 6);
                    blm_manPair.pair_tick = clock_time();
                }
            #endif
        }

    }


    /****************** Adv unpair packet triggers unpair ***********************/
    int adv_unpair_en = !memcmp(pa->data, telink_adv_trigger_unpair_8258, sizeof(telink_adv_trigger_unpair_8258));
    if (adv_unpair_en) {
        int device_is_bond;

        #if (BLE_HOST_SMP_ENABLE)
            device_is_bond = tbl_bond_slave_search(pa->adr_type, pa->mac);
            if (device_is_bond) { //this adv mac is bonded in master
                tbl_bond_slave_delete_by_adr(pa->adr_type, pa->mac);  //by telink stack host smp
            }
        #else
            device_is_bond = user_tbl_slave_mac_search(pa->adr_type, pa->mac);
            if (device_is_bond) { //this adv mac is bonded in master
                user_tbl_slave_mac_delete_by_adr(pa->adr_type, pa->mac);  //by user application code
            }
        #endif
    }

    return 0;
}


int blm_le_connection_establish_event_handle(u8 *p)
{
    event_connection_complete_t *pCon = (event_connection_complete_t *)p;
    if (pCon->status == BLE_SUCCESS) {   // status OK
        #if 1//(UI_LED_ENABLE)
            //led show connection state
            master_connected_led_on = 1;
            //gpio_write(GPIO_LED_RED, LED_ON_LEVAL);     //red on
            //gpio_write(GPIO_LED_WHITE, !LED_ON_LEVAL);  //white off
            //gpio_write(LED_GREEN_PIN, 1);
            DBG_LED_CONNECTED;
        #endif

        cur_conn_device.conn_handle = pCon->handle;   //mark conn handle, in fact this equals to BLM_CONN_HANDLE

        //save current connect address type and address
        cur_conn_device.mac_adrType = pCon->peer_adr_type;
        memcpy(cur_conn_device.mac_addr, pCon->mac, 6);


        #if (BLE_HOST_SMP_ENABLE)
            app_host_smp_sdp_pending = SMP_PENDING; //pair & security first
        #else

            //manual paring, device match, add this device to slave mac table
            if (blm_manPair.manual_pair && blm_manPair.mac_type == pCon->peer_adr_type && !memcmp(blm_manPair.mac, pCon->mac, 6)) {
                blm_manPair.manual_pair = 0;

                user_tbl_slave_mac_add(pCon->peer_adr_type, pCon->mac);
            }


            #if (BLE_HOST_SIMPLE_SDP_ENABLE)
                //new slave device, should do service discovery again
                if (pCon->peer_adr_type != serviceDiscovery_adr_type || memcmp(pCon->mac, serviceDiscovery_address, 6)) {
                    //app_register_service(&app_service_discovery);
                    ble_master_serviceCbRegister(&app_service_discovery);
                    app_host_smp_sdp_pending = SDP_PENDING;  //service discovery busy
                } else {
                    app_host_smp_sdp_pending = 0;  //no need sdp
                }
            #endif
        #endif
    }
    return 0;
}


int blm_disconnect_event_handle(u8 *p)
{
    event_disconnection_t   *pd = (event_disconnection_t *)p;

    //terminate reason
    //connection timeout
    if (pd->reason == HCI_ERR_CONN_TIMEOUT) {

    }
    //peer device(slave) send terminate cmd on link layer
    else if (pd->reason == HCI_ERR_REMOTE_USER_TERM_CONN) {

    }
    //master host disconnect( blm_ll_disconnect(current_connHandle, HCI_ERR_REMOTE_USER_TERM_CONN) )
    else if (pd->reason == HCI_ERR_CONN_TERM_BY_LOCAL_HOST) {

    }
     //master create connection, send conn_req, but did not received acked packet in 6 connection event
    else if (pd->reason == HCI_ERR_CONN_FAILED_TO_ESTABLISH) {
        /* 
         * When controller is in initiating state, find the specified device, send connection request to slave,
         * but slave lost this rf packet, there will no ack packet from slave, after 6 connection events, master
         * controller send a disconnect event with reason HCI_ERR_CONN_FAILED_TO_ESTABLISH
         * if slave got the connection request packet and send ack within 6 connection events, controller
         * send connection establish event to host(telink defined event)
         */
    } else {

    }

    #if 1//(UI_LED_ENABLE)
        //led show none connection state
        if (master_connected_led_on) {
            master_connected_led_on = 0;
            //gpio_write(GPIO_LED_WHITE, LED_ON_LEVAL);   //white on
            //gpio_write(GPIO_LED_RED, !LED_ON_LEVAL);    //red off
            //gpio_write(LED_GREEN_PIN, 0);
            DBG_LED_DISCONNECTED;
        }
    #endif


    cur_conn_device.conn_handle = 0;  //when disconnect, clear conn handle


    //if previous connection smp&sdp not finished, clear this flag
    if (app_host_smp_sdp_pending) {
        app_host_smp_sdp_pending = 0;
    }

    host_update_conn_param_req = 0; //when disconnect, clear update conn flag


    //MTU size reset to default 23 bytes when connection terminated
    blt_att_resetEffectiveMtuSize(BLM_CONN_HANDLE);  //stack API, user can not change

    final_MTU_size = 23;

    //should set scan mode again to scan slave adv packet
    blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,
                            OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
    blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);

    return 0;
}


int blm_le_conn_update_event_proc(u8 *p)
{
    #if (BLE_MASTER_OTA_ENABLE)
        event_connection_update_t *pCon = (event_connection_update_t *)p;

        extern void host_ota_update_conn_complete(u16, u16, u16);
        host_ota_update_conn_complete( pCon->interval, pCon->latency, pCon->timeout );
    #endif


    return 0;
}

int blm_le_phy_update_complete_event_proc(u8 *p)
{
//  hci_le_phyUpdateCompleteEvt_t *pPhyUpt = (hci_le_phyUpdateCompleteEvt_t *)p;

    return 0;
}

void app_bleAdvIntervalSwitch(u16 minInterval, u16 maxInterval)
{
    bls_ll_setAdvParam( minInterval, maxInterval,
                        ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC,
                        0,  NULL,
                        BLT_ENABLE_ADV_ALL,
                        ADV_FP_NONE);

    bls_ll_setAdvEnable(1);  //must: set adv enable
}

//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
int controller_event_callback (u32 h, u8 *p, int n)
{
    static u32 event_cb_num;
    event_cb_num++;

    if (h &HCI_FLAG_EVENT_BT_STD) {      //ble controller hci event
        u8 evtCode = h & 0xff;

        //------------ disconnect -------------------------------------
        if (evtCode == HCI_EVT_DISCONNECTION_COMPLETE) { //connection terminate

            blm_disconnect_event_handle(p);
        }
#if (BLE_HOST_SMP_ENABLE)
        else if (evtCode == HCI_EVT_ENCRYPTION_CHANGE) {
            event_enc_change_t *pe = (event_enc_change_t *)p;
            blm_smp_encChangeEvt(pe->status, pe->handle, pe->enc_enable);
        } else if (evtCode == BLM_CONN_ENC_REFRESH) {
            event_enc_refresh_t *pe = (event_enc_refresh_t *)p;
            blm_smp_encChangeEvt(pe->status, pe->handle, 1);
        }
#endif
        else if (evtCode == HCI_EVT_LE_META) {
            u8 subEvt_code = p[0];

            //------hci le event: le connection complete event---------------------------------
            if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_COMPLETE)  // connection complete
            {
                /* 
                 * After controller is set to initiating state by host (blc_ll_createConnection(...) )
                 * it will scan the specified device(adr_type & mac), when find this adv packet, send a connection request packet to slave
                 * and enter to connection state, send connection complete event. but notice that connection complete not
                 * equals to connection establish. connection complete means that master controller set all the ble timing
                 * get ready, but has not received any slave packet, if slave rf lost the connection request packet, it will
                 * not send any packet to master controller
                 */
            }
            //------hci le event: le connection establish event---------------------------------
            else if(subEvt_code == HCI_SUB_EVT_LE_CONNECTION_ESTABLISH)  //connection establish(telink private event)
            {
                /* 
                 * notice that: this connection event is defined by telink, not a standard ble controller event
                 * after master controller send connection request packet to slave, when slave received this packet
                 * and enter to connection state, send a ack packet within 6 connection event, master will send
                 * connection establish event to host(HCI_SUB_EVT_LE_CONNECTION_ESTABLISH)
                 */

                blm_le_connection_establish_event_handle(p);
            }
            //--------hci le event: le adv report event ----------------------------------------
            else if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT)  // ADV packet
            {
                //after controller is set to scan state, it will report all the adv packet it received by this event

                blm_le_adv_report_event_handle(p);
            }
            //------hci le event: le connection update complete event-------------------------------
            else if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE)  // connection update
            {
                 /* 
                 * after master host send update conn param req cmd to controller( blm_ll_updateConnection(...) ),
                 * when update take effect, controller send update complete event to host
                 */
                blm_le_conn_update_event_proc(p);
            }
            //------hci le event: le phy update complete event-------------------------------
            else if (subEvt_code == HCI_SUB_EVT_LE_PHY_UPDATE_COMPLETE) // connection update
            {
                /* 
                 * 2 situation can trigger this event:
                 *  1) master host trigger: by calling API  blc_ll_setPhy(...)
                 *  2) peer slave device trigger: send "LL_PHY_REQ" on linklayer
                 * when update take effect, controller send update complete event to host
                 */
                blm_le_phy_update_complete_event_proc(p);
            }
        }
    }

    return 0;
}

_attribute_ram_code_ void host_update_conn_proc(void)
{
    //at least 50ms later and make sure smp/sdp is finished
    if ( host_update_conn_param_req && clock_time_exceed(host_update_conn_param_req, 50000)
        && !app_host_smp_sdp_pending) {
        host_update_conn_param_req = 0;

        if (blc_ll_getCurrentState() == BLS_LINK_STATE_CONN) {  //still in connection state
            blm_ll_updateConnection (cur_conn_device.conn_handle,
                    CONN_INTERVAL_50MS, CONN_INTERVAL_50MS, 0, CONN_TIMEOUT_4S,
                                              0, 0 );
        }
    }
}

volatile int app_l2cap_handle_cnt = 0;
int app_l2cap_handler (u16 conn_handle, u8 *raw_pkt)
{
    app_l2cap_handle_cnt ++;  //debug

    //l2cap data packeted, make sure that user see complete l2cap data
    rf_packet_l2cap_t *ptrL2cap = blm_l2cap_packet_pack (conn_handle, raw_pkt);
    if (!ptrL2cap) {
        return 0;
    }

    //l2cap data channel id, 4 for ATT, 5 for Signal, 6 for SMP
    if (ptrL2cap->chanId == L2CAP_CID_ATTR_PROTOCOL) { //att data
        #if (BLE_HOST_SIMPLE_SDP_ENABLE)
            if (app_host_smp_sdp_pending == SDP_PENDING) { 
                /* ATT service discovery is ongoing
                 * when service discovery function is running, all the ATT data from slave
                 * will be processed by it,  user can only send your own att cmd after  service discovery is over 
                 */
                host_att_client_handler (conn_handle, (u8 *)ptrL2cap); //handle this ATT data by service discovery process
            }
        #endif

        rf_packet_att_t *pAtt = (rf_packet_att_t*)ptrL2cap;
        u16 attHandle = pAtt->handle0 | pAtt->handle1<<8;


        if (pAtt->opcode == ATT_OP_EXCHANGE_MTU_REQ || pAtt->opcode == ATT_OP_EXCHANGE_MTU_RSP) {
            rf_packet_att_mtu_exchange_t *pMtu = (rf_packet_att_mtu_exchange_t*)ptrL2cap;

            if (pAtt->opcode ==  ATT_OP_EXCHANGE_MTU_REQ) {
                blc_att_responseMtuSizeExchange(conn_handle, ATT_RX_MTU_SIZE_MAX);
            }

            u16 peer_mtu_size = (pMtu->mtu[0] | pMtu->mtu[1]<<8);
            final_MTU_size = min(ATT_RX_MTU_SIZE_MAX, peer_mtu_size);

            blt_att_setEffectiveMtuSize(conn_handle , final_MTU_size); //stack API, user can not change
        } else if (pAtt->opcode == ATT_OP_READ_BY_TYPE_RSP) {  //slave ack ATT_OP_READ_BY_TYPE_REQ data
            #if (BLE_MASTER_OTA_ENABLE)
                //when use ATT_OP_READ_BY_TYPE_REQ to find ota atthandle, should get the result
                extern void host_find_slave_ota_attHandle(u8 *p);
                host_find_slave_ota_attHandle( (u8 *)pAtt );
            #endif
            //u16 slave_ota_handle;
        } else if (pAtt->opcode == ATT_OP_HANDLE_VALUE_NOTI) { //slave handle notify
            if (attHandle == HID_HANDLE_CONSUME_REPORT) {
                static u32 app_key;
                app_key++;
//              att_keyboard_media (conn_handle, pAtt->dat);
            } else if (attHandle == HID_HANDLE_KEYBOARD_REPORT) {
                static u32 app_key;
                app_key++;
                DBG_LED_KEY_REPORT;
//              att_keyboard (conn_handle, pAtt->dat);

            } else if (attHandle == AUDIO_HANDLE_MIC) {
                static u32 app_mic;
                app_mic ++;
//              att_mic (conn_handle, pAtt->dat);
            } else {

            }
        } else if (pAtt->opcode == ATT_OP_HANDLE_VALUE_IND) {

        }
    } else if (ptrL2cap->chanId == L2CAP_CID_SIG_CHANNEL) { //signal

        if (ptrL2cap->opcode == L2CAP_CMD_CONN_UPD_PARA_REQ) { //slave send conn param update req on l2cap

            rf_packet_l2cap_connParaUpReq_t  * req = (rf_packet_l2cap_connParaUpReq_t *)ptrL2cap;

            u32 interval_us = req->min_interval*1250;  //1.25ms unit
            u32 timeout_us = req->timeout*10000; //10ms unit
            u32 long_suspend_us = interval_us * (req->latency+1);

            //interval < 200ms
            //long suspend < 11S
            // interval * (latency +1)*2 <= timeout
            if ( interval_us < 200000 && long_suspend_us < 20000000 && (long_suspend_us*2<=timeout_us) ) {
                /*
                 * when master host accept slave's conn param update req, should send a conn param update response on l2cap
                 * with CONN_PARAM_UPDATE_ACCEPT; if not accept,should send  CONN_PARAM_UPDATE_REJECT
                 */
                blc_l2cap_SendConnParamUpdateResponse(conn_handle, req->id, CONN_PARAM_UPDATE_ACCEPT);  //send SIG Connection Param Update Response


                //if accept, master host should mark this, add will send  update conn param req on link layer later
                //set a flag here, then send update conn param req in mainloop
                host_update_conn_param_req = clock_time() | 1 ; //in case zero value
                host_update_conn_min = req->min_interval;  //backup update param
                host_update_conn_latency = req->latency;
                host_update_conn_timeout = req->timeout;
            } else {
                blc_l2cap_SendConnParamUpdateResponse(conn_handle, req->id, CONN_PARAM_UPDATE_REJECT);  //send SIG Connection Param Update Response
            }
        }
    } else if (ptrL2cap->chanId == L2CAP_CID_SMP) {//smp
        #if (BLE_HOST_SMP_ENABLE)
            if (app_host_smp_sdp_pending == SMP_PENDING) {
                blm_host_smp_handler(conn_handle, (u8 *)ptrL2cap);
            }

        #endif
    } else {

    }

    return 0;
}


void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static)
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
 * @brief       This function is used to read flash mid, get flash vendor, and set flash capacity
 * @param[in]   none
 * @return      none
 */
void blc_flash_read_mid_get_vendor_set_capacity(void)
{
    /* 
     * attention: blc_flash_mid/tlk_flash_vendor/tlk_flash_capacity will be used by application and stack later
     * so do not change code here 
     */
    tlk_flash_mid = flash_read_mid();
    tlk_flash_vendor = flash_get_vendor(tlk_flash_mid);
    tlk_flash_capacity = ((tlk_flash_mid & 0x00ff0000)>>16);

}

///////////////////////////////////////////
void user_ble_init(bool isRetention)
{
    (void)isRetention;
    bls_smp_configPairingSecurityInfoStorageAddr(CFG_NV_START_FOR_BLE);

///////////// BLE stack Initialization ////////////////
    u8  mac_public[6];
    u8  mac_random_static[6];

    /* 
     * for 512K Flash, flash_sector_mac_address equals to 0x76000
     * for 1M   Flash, flash_sector_mac_address equals to 0xFF000
     */
    blc_initMacAddress(CFG_MAC_ADDRESS, mac_public, mac_random_static);

    blc_flash_read_mid_get_vendor_set_capacity();
#if PA_ENABLE
    /* external RF PA used */
    g_ble_txPowerSet = ZB_RADIO_TX_0DBM;   //set to 0dBm
    ble_rf_pa_init(0, PA_TX, PA_RX);
#endif

    ////// Controller Initialization  //////////
    blc_ll_initBasicMCU();
    blc_ll_initStandby_module(mac_public);              //mandatory
    blc_ll_initScanning_module(mac_public);             //scan module:       mandatory for BLE master,
    blc_ll_initInitiating_module();                     //initiate module:   mandatory for BLE master,
    blc_ll_initConnection_module();                     //connection module  mandatory for BLE slave/master
    blc_ll_initMasterRoleSingleConn_module();           //master module:     mandatory for BLE master,

    rf_set_power_level_index (g_ble_txPowerSet);

    ////// Host Initialization  //////////
    blc_gap_central_init();                                     //gap initialization
    blc_l2cap_register_handler (app_l2cap_handler);             //l2cap initialization
    blc_hci_registerControllerEventHandler(controller_event_callback); //controller hci event to host all processed in this func


    //bluetooth event
    blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE | HCI_EVT_MASK_ENCRYPTION_CHANGE);

    //bluetooth low energy(LE) event
    blc_hci_le_setEventMask_cmd(        HCI_LE_EVT_MASK_CONNECTION_COMPLETE         \
                                    |   HCI_LE_EVT_MASK_ADVERTISING_REPORT          \
                                    |   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE  \
                                    |   HCI_LE_EVT_MASK_PHY_UPDATE_COMPLETE         \
                                    |   HCI_LE_EVT_MASK_CONNECTION_ESTABLISH );         //connection establish: telink private event


    #if (BLE_HOST_SMP_ENABLE)
        bls_smp_configPairingSecurityInfoStorageAddr(CFG_NV_START_FOR_BLE);
        blm_smp_registerSmpFinishCb(app_host_smp_finish);

        blc_smp_central_init();

        //SMP trigger by master
        blm_host_smp_setSecurityTrigger(MASTER_TRIGGER_SMP_FIRST_PAIRING | MASTER_TRIGGER_SMP_AUTO_CONNECT);
    #else  //referenced paring&bonding without standard paring in BLE spec
        blc_smp_setSecurityLevel(No_Security);

        user_master_host_pairing_management_init();
    #endif

    extern int host_att_register_idle_func (void *p);
    host_att_register_idle_func(zb_ble_switch_proc);  // (main_idle_loop);

    ble_master_updateIndCbRegister(ble_master_updateIndHandler);   //for zb/ble concurrent mode

#if !BLE_DEBUG
    blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,
                            OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
    blc_ll_setScanWin(30000);//30ms
    blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);
#else
    //debug
    blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,
                            OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_WL);
    blc_ll_setScanWin(30000);//30ms
    blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);

    ll_whiteList_reset();
    ll_whiteList_add(0, whiteListAddr);

    DBG_IO_INIT;
#endif
}

static void ble_master_updateIndHandler(void)
{
    if (host_update_conn_param_req) {
        host_update_conn_proc();
    }
}

#endif

