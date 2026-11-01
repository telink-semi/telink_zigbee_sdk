/* SPDX-License-Identifier: Apache-2.0
 * B85 512K hardware-dual-boot safety policy. The vendor server retains ownership
 * of packet sequence, flash transfer, checksums, result handling and activation.
 */
#include "../tl_common.h"
#if SDK_B85_512K_DUAL_BOOT && BLE_CONCURRENT_MODE
#include "ble.h"
#include "drv_b85_ota.h"
#define SLOT_SIZE (208UL*1024)
#define SLOT_B 0x40000UL
#define MARKER 0x544c4e4bUL
static u32 active=0xffffffff, target=0xffffffff;
static u16 expected_manufacturer,expected_image;
static u8 session,activation,header_valid,first_pending,pdu_size,transfer_busy;
static u8 first_packet[32] __attribute__((aligned(4)));
static drv_b85_ota_start_cb start_cb;
static drv_b85_ota_result_cb result_cb;
static drv_b85_ota_progress_cb progress_cb;
static u16 le16(const u8 *p) { return p[0] | (u16)p[1]<<8; }
static u32 le32(const u8 *p) { return le16(p) | (u32)le16(p+2)<<16; }

void drv_b85_flash_guard_init(void)
{
    u32 a,b;
    flash_read_page(8,4,(u8 *)&a); flash_read_page(SLOT_B+8,4,(u8 *)&b);
    active=0xffffffff;
    /* Neither scan order nor configured alternate address proves the running slot
     * when both markers survive a power cut. Fail closed for image mutations. */
    if((a==MARKER)!=(b==MARKER)) active=a==MARKER ? 0 : SLOT_B;
    target=active==0 ? SLOT_B : active==SLOT_B ? 0 : 0xffffffff;
    session=activation=0;
}
int drv_b85_flash_allow(u32 address,u32 length)
{
    if(!length || address>=0x80000 || length>0x80000-address) return 0;
    if(address>=0x34000 && address+length<=0x40000) return 1;
    if(address>=0x74000 && address+length<=0x80000) return 1;
    if(session && target!=0xffffffff && address>=target && address+length<=target+SLOT_SIZE) return 1;
    /* Vendor activation writes exactly one byte to invalidate the old image. */
    return activation && address==active+8 && length==1;
}
static void started(void) { transfer_busy=1; if(start_cb) start_cb(); }
static void completed(int result)
{
    /* Keep inactive-slot cleanup enabled after a failed transfer. */
    activation=result==OTA_SUCCESS; transfer_busy=0;
    if(result_cb) result_cb(result);
}
int drv_b85_ota_prepare(u16 manufacturer,u16 image_type,drv_b85_ota_start_cb start,
                       drv_b85_ota_result_cb result,drv_b85_ota_progress_cb progress)
{
    drv_b85_flash_guard_init();
    if(target==0xffffffff || ((flash_read_mid()>>16)&0xff)!=0x13) return 0;
    expected_manufacturer=manufacturer; expected_image=image_type;
    start_cb=start; result_cb=result; progress_cb=progress;
    header_valid=first_pending=transfer_busy=0; pdu_size=16;
    ota_program_offset=target;
    session=1; /* init may erase only the inactive slot */
    blc_ota_initOtaServer_module();
    blc_ota_registerOtaStartCmdCb(started);
    blc_ota_registerOtaResultIndicationCb(completed);
    blc_ota_setOtaProcessTimeout(300);
    blc_ota_setOtaDataPacketTimeout(5);
    return 1;
}
static int reject_image(int result)
{
    /* Nothing was forwarded to the writer before the complete header check.
     * Reset the idle receiver through its official initializer. */
    blc_ota_initOtaServer_module();
    first_pending=header_valid=transfer_busy=0;
    if(result_cb) result_cb(result);
    return 0;
}
static int valid_header_tail(const u8 *data)
{
    /* data points to firmware offset 0x10. */
    u32 size=le32(data+8);
    return le16(data+2)==expected_manufacturer && le16(data+4)==expected_image &&
           size>=32 && size<=SLOT_SIZE;
}
int drv_b85_ota_write(void *packet)
{
    if(!session) return 0;
    rf_packet_att_data_t *p=packet;
    if(p->l2cap<5 || p->l2cap>247 || p->rf_len!=p->l2cap+4) return 0;
    u8 *data=(u8 *)packet+9;
    unsigned length=p->l2cap-3;
    u16 command=le16(data);
    if(command==CMD_OTA_START || command==CMD_OTA_START_EXT) {
        if(transfer_busy) return 0;
        if(command==CMD_OTA_START_EXT && length<sizeof(ota_startExt_t)) return 0;
        first_pending=header_valid=activation=0;
        pdu_size=command==CMD_OTA_START_EXT && length>=3 ? data[2] : 16;
        return otaWrite(packet);
    }
    if(command==CMD_OTA_END) {
        if(!header_valid) return reject_image(OTA_FIRMWARE_MARK_ERR);
        if(progress_cb) progress_cb(2);
        return otaWrite(packet);
    }
    if(command>=0xff00) return otaWrite(packet);
    /* Use the vendor packet CRC; malformed input remains the vendor's concern. */
    if(length<20 || (length-4)%16 || length>(unsigned)pdu_size+4 || pdu_size<16 ||
       crc16(data,length-2)!=le16(data+length-2)) return otaWrite(packet);
    if(!header_valid) {
        if(command==0 && pdu_size==16) {
            if(first_pending) return reject_image(OTA_DATA_PACKET_SEQ_ERR);
            if(le32(data+2+8)!=MARKER) return reject_image(OTA_FIRMWARE_MARK_ERR);
            memcpy(first_packet,packet,29); first_pending=1;
            return 0;
        }
        if(command==0 && pdu_size>=32 && length>=36) {
            if(le32(data+2+8)!=MARKER || !valid_header_tail(data+18)) return reject_image(OTA_FIRMWARE_MARK_ERR);
        } else if(command==1 && pdu_size==16 && first_pending) {
            if(!valid_header_tail(data+2)) return reject_image(OTA_FIRMWARE_MARK_ERR);
            otaWrite(first_packet); first_pending=0;
        } else return reject_image(OTA_FIRMWARE_MARK_ERR);
        header_valid=1;
    }
    int result=otaWrite(packet);
    /* A readback, not a received ATT packet alone, establishes progress. */
    u32 offset=(u32)command*pdu_size;
    if(offset<SLOT_SIZE) {
        u8 check[4]; flash_read_page(target+offset,4,check);
        if(!memcmp(check,data+2,4) && progress_cb) progress_cb(1);
    }
    return result;
}
#endif
