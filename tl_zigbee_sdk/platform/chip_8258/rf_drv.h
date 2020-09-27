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

#include "bit.h"
#include "bsp.h"
#include "flash.h"
#include "analog.h"

#define RF_CHN_TABLE 		0x8000



/**
 *  @brief  Define RF mode
 */
typedef enum {
	RF_MODE_BLE_2M 		   =    BIT(0),
	RF_MODE_BLE_1M 		   = 	BIT(1),
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

#define	RF_PACKET_LENGTH_OK(p)						(p[0] == p[12]+13)
#define	RF_PACKET_CRC_OK(p)							((p[p[0]+3] & 0x51) == 0x10)

#define	RF_BLE_PACKET_LENGTH_OK(p)					(p[0] == (p[5]&0x3f)+13)
#define	RF_BLE_PACKET_CRC_OK(p)						((p[p[0]+3] & 0x01) == 0)

#define RF_ZIGBEE_PACKET_LENGTH_OK(p)    			(p[0] == p[4]+9)
#define RF_ZIGBEE_PACKET_CRC_OK(p)       			((p[p[0]+3] & 0x51) == 0x10)
#define RF_ZIGBEE_PACKET_RSSI_GET(p)     			(p[p[0]+2])
#define RF_ZIGBEE_PACKET_TIMESTAMP_GET(p)           (p[p[0]-4] | (p[p[0]-3]<<8) | (p[p[0]-2]<<16) | (p[p[0]-1]<<24))
#define RF_ZIGBEE_PACKET_PAYLOAD_LENGTH_GET(p)      (p[4])

#define RF_NRF_ESB_PACKET_LENGTH_OK(p)              (p[0] == (p[4] & 0x3f) + 11)
#define RF_NRF_ESB_PACKET_CRC_OK(p)                 ((p[p[0]+3] & 0x01) == 0x00)
#define RF_NRF_ESB_PACKET_RSSI_GET(p)               (p[p[0]+2])
#define RF_NRF_SB_PACKET_PAYLOAD_LENGTH_GET(p)      (p[0] - 10)
#define RF_NRF_SB_PACKET_CRC_OK(p)                  ((p[p[0]+3] & 0x01) == 0x00)
#define RF_NRF_SB_PACKET_CRC_GET(p)                 ((p[p[0]-8]<<8) + p[p[0]-7]) //Note: here assume that the MSByte of CRC is received first
#define RF_NRF_SB_PACKET_RSSI_GET(p)                (p[p[0]+2])


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

static inline void rf_reset_sn (void)
{
	write_reg8  (0x800f01, 0x3f);
	write_reg8  (0x800f01, 0x00);
}

static inline void rf_drv_cap(unsigned long addr)
{
	unsigned char cap = 0xff;
	flash_read_page(addr, 1, &cap);
	if(cap != 0xff){
		cap &= 0x3f;
		WriteAnalogReg(0x8a, (ReadAnalogReg(0x8a) & 0xc0) | cap);
	}
}

void rf_set_power_level (unsigned char level);

void rf_set_power_level_index (RF_PowerTypeDef level);     //@@@

void rf_drv_init (RF_ModeTypeDef rf_mode);   				//@@@

void rf_set_channel (signed char chn, unsigned short set);


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

void  rf_rx_buffer_set(unsigned char *  RF_RxAddr, int size, unsigned char  PingpongEn);

void  rf_rx_cfg(int size, unsigned char  PingpongEn);

void rf_start_stx(void* addr, unsigned int tick);

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

void rf_rx_buffer_reconfig(unsigned int RF_RxAddr);

extern signed char rf_ed_detecct_154(void);
extern unsigned char rf_stop_ed_154(void);
#endif
