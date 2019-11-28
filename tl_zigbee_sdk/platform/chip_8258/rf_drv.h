/********************************************************************************************************
 * @file     rf_drv.h
 *
 * @brief    RF configuration interface for tlsr8258
 *
 * @author   jian.zhang@telink-semi.com
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef _RF_DRV_H_
#define _RF_DRV_H_

#include "bsp.h"


#define RF_CHN_TABLE 		0x8000



/**
 *  @brief  Define RF mode
 */
typedef enum {
	 RF_MODE_BLE_2M =    		BIT(0),
	 RF_MODE_BLE_1M = 			BIT(1),
    RF_MODE_BLE_1M_NO_PN   =    BIT(2),
	RF_MODE_ZIGBEE_250K    =    BIT(3),
    RF_MODE_LR_S2_500K     =    BIT(4),
    RF_MODE_LR_S8_125K     =    BIT(5),
    RF_MODE_PRIVATE_250K   =    BIT(6),
    RF_MODE_PRIVATE_500K   =    BIT(7),
    RF_MODE_PRIVATE_1M     =    BIT(8),
    RF_MODE_PRIVATE_2M     =    BIT(9),
} RF_ModeTypeDef;


//extern  RF_ModeTypeDef g_RFMode;

typedef enum {
    RF_MODE_TX = 0,
    RF_MODE_RX = 1,
    RF_MODE_AUTO=2
} RF_StatusTypeDef;

typedef enum {
 /*VBAT*/
 RF_POWER_P10p46dBm,
 RF_POWER_P10p29dBm,
 RF_POWER_P10p01dBm,
 RF_POWER_P9p81dBm,
 RF_POWER_P9p48dBm,
 RF_POWER_P9p24dBm,
 RF_POWER_P8p97dBm,
 RF_POWER_P8p73dBm,
 RF_POWER_P8p44dBm,
 RF_POWER_P8p13dBm,
 RF_POWER_P7p79dBm,
 RF_POWER_P7p41dBm,
 RF_POWER_P7p02dBm,
 RF_POWER_P6p60dBm,
 RF_POWER_P6p14dBm,
 RF_POWER_P5p65dBm,
 RF_POWER_P5p13dBm,
 RF_POWER_P4p57dBm,
 RF_POWER_P3p94dBm,
 RF_POWER_P3p23dBm,
 /*VANT*/
 RF_POWER_P3p01dBm,
 RF_POWER_P2p81dBm,
 RF_POWER_P2p61dBm,
 RF_POWER_P2p39dBm,
 RF_POWER_P1p99dBm,
 RF_POWER_P1p73dBm,
 RF_POWER_P1p45dBm,
 RF_POWER_P1p17dBm,
 RF_POWER_P0p90dBm,
 RF_POWER_P0p58dBm,
 RF_POWER_P0p04dBm,
 RF_POWER_N0p14dBm,
 RF_POWER_N0p97dBm,
 RF_POWER_N1p42dBm,
 RF_POWER_N1p89dBm,
 RF_POWER_N2p48dBm,
 RF_POWER_N3p03dBm,
 RF_POWER_N3p61dBm,
 RF_POWER_N4p26dBm,
 RF_POWER_N5p03dBm,
 RF_POWER_N5p81dBm,
 RF_POWER_N6p67dBm,
 RF_POWER_N7p65dBm,
 RF_POWER_N8p65dBm,
 RF_POWER_N9p89dBm,
 RF_POWER_N11p4dBm,
 RF_POWER_N13p29dBm,
 RF_POWER_N15p88dBm,
 RF_POWER_N19p27dBm,
 RF_POWER_N25p18dBm,
} RF_PowerTypeDef;

extern const unsigned char rf_power_Level_list[];

#define		RF_PACKET_LENGTH_OK(p)		(p[0] == p[12]+13)
#define		RF_PACKET_CRC_OK(p)			((p[p[0]+3] & 0x51) == 0x10)

#define		RF_BLE_PACKET_LENGTH_OK(p)			(p[0] == (p[5]&0x3f)+13)
#define		RF_BLE_PACKET_CRC_OK(p)				((p[p[0]+3] & 0x01) == 0)

#define    RF_ZIGBEE_PACKET_LENGTH_OK(p)    (p[0]  == p[4]+9)
#define    RF_ZIGBEE_PACKET_CRC_OK(p)       ((p[p[0]+3] & 0x51) == 0x10)
#define    RF_ZIGBEE_PACKET_RSSI_GET(p)     (p[p[0]+2])
#define    RF_ZIGBEE_PACKET_TIMESTAMP_GET(p)           (p[p[0]-4] | (p[p[0]-3]<<8) | (p[p[0]-2]<<16) | (p[p[0]-1]<<24))
#define    RF_ZIGBEE_PACKET_PAYLOAD_LENGTH_GET(p)      (p[4])
#define    RF_NRF_ESB_PACKET_LENGTH_OK(p)              (p[0] == (p[4] & 0x3f) + 11)
#define    RF_NRF_ESB_PACKET_CRC_OK(p)                 ((p[p[0]+3] & 0x01) == 0x00)
#define    RF_NRF_ESB_PACKET_RSSI_GET(p)               (p[p[0]+2])
#define    RF_NRF_SB_PACKET_PAYLOAD_LENGTH_GET(p)      (p[0] - 10)
#define    RF_NRF_SB_PACKET_CRC_OK(p)                  ((p[p[0]+3] & 0x01) == 0x00)
#define    RF_NRF_SB_PACKET_CRC_GET(p)                 ((p[p[0]-8]<<8) + p[p[0]-7]) //Note: here assume that the MSByte of CRC is received first
#define    RF_NRF_SB_PACKET_RSSI_GET(p)                (p[p[0]+2])
static inline void rf_ble_tx_on ()
{
	write_reg8  (0x800f02, 0x45 | BIT(4));	// TX enable
	write_reg32 (0x800f04, 0x38);
}

static inline void rf_ble_tx_done ()
{
	write_reg8  (0x800f02, 0x45);	// TX enable
	write_reg32 (0x800f04, 0x50);
}

void rf_update_tp_value (unsigned char tp0, unsigned char tp1);


/////////////////////  RF BaseBand ///////////////////////////////
static inline void reset_baseband(void)
{
	REG_ADDR8(0x61) = BIT(0);		//reset baseband
	REG_ADDR8(0x61) = 0;			//release reset signal
}

static inline void reset_sn_nesn(void)
{
	REG_ADDR8(0xf01) =  0x01;
}

static inline void 	tx_settle_adjust(unsigned short txstl_us)
{
	REG_ADDR16(0xf04) = txstl_us;  //adjust TX settle time
}

static inline void rf_set_tx_pipe (unsigned char pipe)
{
	write_reg8 (0x800f15, 0xf0 | pipe);
}

static inline void rf_set_ble_crc (unsigned char *p)
{
	write_reg32 (0x800424, p[0] | (p[1]<<8) | (p[2]<<16));
}

static inline void rf_set_ble_crc_value (unsigned int crc)
{
	write_reg32 (0x800424, crc);
}

static inline void rf_set_ble_crc_adv ()
{
	write_reg32 (0x800424, 0x555555);
}

static inline void rf_set_ble_access_code (unsigned char *p)
{
	write_reg32 (0x800408, p[3] | (p[2]<<8) | (p[1]<<16) | (p[0]<<24));
}

static inline void rf_set_ble_access_code_value (unsigned int ac)
{
	write_reg32 (0x800408, ac);
}


static inline void rf_set_ble_access_code_adv (void)
{
	write_reg32 (0x800408, 0xd6be898e);
}
static inline void rf_access_code_comm (unsigned int acc)
{
	write_reg32 (0x800408, acc);
	write_reg8 (0x800405, read_reg8(0x405)|0x80);
}

static inline unsigned int rf_access_code_16to32 (unsigned short code)
{
	unsigned int r = 0;
	for (int i=0; i<16; i++) {
		r = r << 2;
		r |= code & BIT(i) ? 1 : 2;
	}
	return r;
}

static inline unsigned short rf_access_code_32to16 (unsigned int code)
{
	unsigned short r = 0;
	for (int i=0; i<16; i++) {
		r = r << 1;

		r |= (code & BIT(i*2)) ? 1 : 0;

	}
	return r;
}

static inline void rf_reset_sn (void)
{
	write_reg8  (0x800f01, 0x3f);
	write_reg8  (0x800f01, 0x00);
}

void rf_set_power_level (unsigned char level);

void rf_set_power_level_index (RF_PowerTypeDef level);     //@@@

void 	rf_drv_init (RF_ModeTypeDef rf_mode);   				//@@@

void	rf_set_channel (signed char chn, unsigned short set);

void 	rf_set_ble_channel (signed char chn_num);

//manual mode
static inline void rf_set_rxmode (void)
{
    write_reg8 (0x800428, 0x80 | BIT(0));	// rx enable
    write_reg8 (0x800f02, 0x45 | BIT(5));		// RX enable
}

static inline void rf_set_txmode (void)
{
	write_reg8  (0x800f02, 0x45 | BIT(4));	// TX enable
}

//maunal mode off
static inline void rf_set_tx_rx_off(void)
{
	write_reg8 (0x800f16, 0x29);
	write_reg8 (0x800428, 0x80);	// rx disable
	write_reg8 (0x800f02, 0x45);	// reset tx/rx state machine
}
//auto mode off
static inline void rf_set_tx_rx_off_auto_mode(void)
{
 write_reg8 (0xf00, 0x80);
}
//auto
static inline void rf_stop_trx (void)
{
	write_reg8  (0x800f00, 0x80);			// stop
}

void	rf_send_packet (void* addr);

void  rf_rx_buffer_set(unsigned char *  RF_RxAddr, int size, unsigned char  PingpongEn);

void  rf_rx_cfg(int size, unsigned char  PingpongEn);

void rf_start_btx (void* addr, unsigned int tick);
void rf_start_brx  (void* addr, unsigned int tick);

void rf_start_stx  (void* addr, unsigned int tick);
void rf_start_srx  (unsigned int tick);

void rf_start_stx2rx  (void* addr, unsigned int tick);
void rf_start_srx2tx  (void* addr, unsigned int tick);

extern int rf_trx_state_set(RF_StatusTypeDef rf_status, signed char rf_channel);
extern RF_StatusTypeDef rf_trx_state_get(void);
extern void rf_tx_pkt(unsigned char *rf_txaddr);
static inline unsigned char rf_tx_finish(void)
{
    return (READ_REG8(0xf20) & BIT(1));
}
static inline void rf_tx_finish_clear_flag(void)
{
    WRITE_REG8(0xf20, READ_REG8(0xf20) | 0x02);
}

static inline void rf_rx_finish_clear_flag(void)
{
    WRITE_REG8(0xf20, READ_REG8(0xf20) | 0x01);
}

static inline void rf_rx_buffer_reconfig(unsigned short RF_RxAddr)
{
	reg_dma2_addr = (unsigned short)(RF_RxAddr);
}


extern void rf_acc_len_set(unsigned char len);
static inline unsigned char rf_acc_len_get(unsigned char len)
{
    return (READ_REG8(0x405) & 0x07); //access_byte_num[2:0]
}
extern void rf_acc_code_set(unsigned char pipe, const unsigned char *addr);
extern void rf_acc_code_get(unsigned char pipe, unsigned char *addr);
static inline void rf_rx_acc_code_enable(unsigned char pipe)
{
    WRITE_REG8(0x407, (READ_REG8(0x407)&0xc0) | pipe); //rx_access_code_chn_en
}
static inline void rf_tx_acc_code_select(unsigned char pipe)
{
    WRITE_REG8(0xf15, (READ_REG8(0xf15)&0xf8) | pipe); //Tx_Channel_man[2:0]
}
static inline void rf_nordic_shockburst(int len)
{
    WRITE_REG8(0x404, READ_REG8(0x404)|0x03); //select shockburst header mode
    WRITE_REG8(0x406, len);
}
extern unsigned short crc16_ccitt_cal(unsigned char *input, unsigned int len, unsigned short init_val);
extern void rf_tx_500k_simulate_100k(unsigned char *preamble, unsigned char preamble_len,
                                     unsigned char *acc_code, unsigned char acc_len,
                                     unsigned char *payload, unsigned char pld_len,
                                     unsigned char *tx_buf, unsigned short crc_init);
extern signed char rf_ed_detecct_154(void);
extern unsigned char rf_stop_ed_154(void);
#endif
