/* SPDX-License-Identifier: Apache-2.0
 * Host boundary harness. The run script inserts the actual policy source here;
 * only platform includes are substituted. Vendor OTA itself is not emulated. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32;
#define SDK_B85_512K_DUAL_BOOT 1
#define BLE_CONCURRENT_MODE 1
#define OTA_SUCCESS 0
#define OTA_DATA_PACKET_SEQ_ERR 1
#define OTA_FIRMWARE_MARK_ERR 10
#define CMD_OTA_START 0xff01
#define CMD_OTA_END 0xff02
#define CMD_OTA_START_EXT 0xff03
typedef struct __attribute__((packed)) { u8 type,rf_len; u16 l2cap,channel; u8 opcode; u16 handle; u8 data[248]; } rf_packet_att_data_t;
typedef struct { u16 cmd; u8 pdu,version,rsv[16]; } ota_startExt_t;
typedef void (*drv_b85_ota_start_cb)(void);
typedef void (*drv_b85_ota_result_cb)(int);
typedef void (*drv_b85_ota_progress_cb)(u8);
static u8 flash[0x80000]; static u32 mid=0x1360eb;
u32 ota_program_offset;
static void (*vendor_start)(void); static void (*vendor_result)(int);
static int forwarded,results,last_result,initialized;
void flash_read_page(u32 a,u32 n,u8 *p) { assert(a+n<=sizeof(flash)); memcpy(p,flash+a,n); }
int drv_b85_flash_can_write(u32,u32);
static void guarded_program(u32 a,u32 n,u8 *p) {
    assert(a+n<=sizeof(flash));
    if(drv_b85_flash_can_write(a,n)) for(u32 i=0;i<n;++i) flash[a+i]&=p[i];
}
void (*flash_write_page)(u32,u32,u8 *)=guarded_program;
u32 flash_read_mid(void) { return mid; }
void blc_ota_initOtaServer_module(void) { ++initialized; }
void blc_ota_registerOtaStartCmdCb(void (*cb)(void)) { vendor_start=cb; }
void blc_ota_registerOtaResultIndicationCb(void (*cb)(int)) { vendor_result=cb; }
int blc_ota_setOtaProcessTimeout(int n) { assert(n==1000); return 0; }
int blc_ota_setOtaDataPacketTimeout(int n) { assert(n==5); return 0; }
u16 crc16(const u8 *p,int n) { u16 c=0xffff; while(n--) c=(c<<1)^*p++; return c; }
int otaWrite(void *p) {
    rf_packet_att_data_t *pkt=p; ++forwarded;
    if(pkt->data[0]==1 && pkt->data[1]==0xff) vendor_start();
    if(pkt->data[0]==3 && pkt->data[1]==0xff) vendor_start();
    return 0;
}
/* POLICY_SOURCE */
static void result(int r) { ++results; last_result=r; }
static void reset(u32 slot) {
    memset(flash,0xff,sizeof(flash)); memcpy(flash+slot+8,"KNLT",4);
    forwarded=results=0; assert(drv_b85_ota_prepare(0x1141,0x0101,0,result,0));
    assert(ota_program_offset==(slot ? 0 : 0x40000));
}
static rf_packet_att_data_t pkt;
static void start(void) {
    memset(&pkt,0,sizeof(pkt)); pkt.l2cap=5; pkt.rf_len=9;
    pkt.data[0]=1; pkt.data[1]=0xff; drv_b85_ota_write(&pkt);
}
static void data(u16 index,const u8 *bytes,unsigned n) {
    memset(&pkt,0,sizeof(pkt)); pkt.l2cap=n+7; pkt.rf_len=n+11;
    pkt.data[0]=index; pkt.data[1]=index>>8; memcpy(pkt.data+2,bytes,n);
    u16 crc=crc16(pkt.data,n+2); pkt.data[n+2]=crc; pkt.data[n+3]=crc>>8;
    drv_b85_ota_write(&pkt);
}
static int supply_ok;
static int fresh_supply(void) { return supply_ok; }
int main(void) {
    for(u32 slot=0;;slot=0x40000) {
        reset(slot);
        assert(!drv_b85_flash_allow(slot,4096));
        assert(!drv_b85_flash_allow(slot+8,1));
        assert(drv_b85_flash_allow(ota_program_offset,208*1024));
        assert(!drv_b85_flash_allow(ota_program_offset,208*1024+1));
        assert(!drv_b85_flash_allow(0xffffffff,2));
        assert(!drv_b85_flash_allow(0x7ffff,2));
        assert(!drv_b85_flash_allow(0x33fff,2));
        assert(drv_b85_flash_allow(0x34000,0xc000));
        assert(drv_b85_flash_allow(0x74000,0xc000));
        vendor_result(3); assert(!drv_b85_flash_allow(slot+8,1));
        vendor_result(0); assert(!drv_b85_flash_allow(slot+8,1));
        assert(!drv_b85_flash_allow(slot+8,2));
        memcpy(flash+(slot ? 0 : 0x40000)+8,"KNLT",4);
        int old=initialized;
        assert(!drv_b85_ota_prepare(0x1141,0x0101,0,result,0)); assert(initialized==old);
        assert(!drv_b85_flash_allow(slot,1));
        if(slot) break;
    }
    reset(0); mid=0x1460eb;
    assert(!drv_b85_ota_prepare(0x1141,0x0101,0,result,0)); mid=0x1360eb;
    u8 header[32]={0}; memcpy(header+8,"KNLT",4);
    header[18]=0x41; header[19]=0x11; header[20]=1; header[21]=1; header[25]=0x40;
    reset(0); start(); assert(forwarded==1);
    data(0,header,16); assert(forwarded==1); /* hold until the model is known */
    start(); assert(forwarded==1); /* repeated START cannot reset transfer */
    data(1,header+16,16); assert(forwarded==3 && !results);
    memcpy(flash+0x40009,"NLT",3);
    blt_ota_writeBootMark(); /* vendor success hook precedes result callback */
    assert(!memcmp(flash+0x40008,"KNLT",4));
    assert(!memcmp(flash+8,"\0NLT",4));
    assert(!drv_b85_flash_allow(8,1));
    reset(0); start(); data(0,header,16); header[20]=2;
    data(1,header+16,16); assert(results==1 && last_result==10 && forwarded==1);
    header[20]=1;
    reset(0); start(); data(0,header,16); data(0,header,16);
    assert(results==1 && last_result==1 && forwarded==1);
    reset(0); start(); pkt.l2cap=1; drv_b85_ota_write(&pkt); assert(forwarded==1);
    reset(0); drv_b85_flash_set_voltage_check(fresh_supply);
    supply_ok=0; assert(!drv_b85_flash_can_write(0x40000,16));
    assert(!drv_b85_flash_can_write(0x7a000,16));
    assert(!drv_b85_ota_prepare(0x1141,0x0101,0,result,0));
    supply_ok=1; assert(drv_b85_ota_prepare(0x1141,0x0101,0,result,0));
    assert(drv_b85_flash_can_write(0x40000,16));
    assert(!drv_b85_flash_can_write(0,16));
    puts("PASS B85 512K guard: slots, overflow, activation, header gate, repeat START");
}
