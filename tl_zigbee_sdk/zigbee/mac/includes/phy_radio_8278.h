/********************************************************************************************************
 * @file     phy_radio_8258.h
 *
 * @brief    Zigbee API for phy layer
 *
 * @author
 * @date     Dec. 1, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef  __MAC_RADIO_8278_H_
#define  __MAC_RADIO_8278_H_

#include "tl_common.h"
#include "mac_phy.h"

/* radio module reset */
#define ZB_RADIO_RESET()

/* Tx/Rx swith */
#define ZB_RADIO_TRX_SWITCH(mode ,chn) 		rf_trx_state_set(mode, chn)

/* set tx power */
#define ZB_RADIO_TX_POWER_SET(level)		rf_set_power_level_index_zgb(level)

/* set rx gain mode: max gain: only use auto gain mode */
#define ZB_RADIO_MODE_MAX_GAIN()

/* set rx gain mode: auto gain, only use auto gain mode */
#define ZB_RADIO_MODE_AUTO_GAIN()

/* get RSSI */
#define ZB_RADIO_RSSI_GET()					rf_rssi_get_154()

/* start sending a packet */
#define ZB_RADIO_TX_START(txBuf)			rf_tx_pkt(txBuf)

/* clear packet transmission is done */
#define ZB_RADIO_TX_DONE					rf_tx_finish()

/* clear Tx status(interrupt status) */
#define ZB_RADIO_TX_DONE_CLR				rf_tx_finish_clear_flag()

/* clear packet transmission is done */
#define ZB_RADIO_RX_DONE					rf_is_rx_finish()

/* clear Rx status(interrupt status) */
#define ZB_RADIO_RX_DONE_CLR				rf_rx_finish_clear_flag()

/* enable Tx */
#define ZB_RADIO_TX_ENABLE					RFDMA_TX_ENABLE

/* disable Tx */
#define ZB_RADIO_TX_DISABLE					RFDMA_TX_DISABLE

/* enable Rx */
#define ZB_RADIO_RX_ENABLE					RFDMA_RX_ENABLE

/* disable Rx */
#define ZB_RADIO_RX_DISABLE					RFDMA_RX_DISABLE

/* TRx status get */
#define ZB_RADIO_TRX_STA_GET()				rf_trx_state_get()

/* Rx buffer configure */
#define ZB_RADIO_RX_BUF_SET(addr)			rf_rx_buffer_reconfig(addr)

/* Rx buffer clear */
#define ZB_RADIO_RX_BUF_CLEAR(p)

/* set Rx mode, maxium receiver buffer size, enable Rx/Tx interrupt */
#define ZB_RADIO_TRX_CFG(size)				do{\
												rf_rx_cfg(size, 0); \
												dma_irq_disable(FLD_DMA_CHN_RF_RX|FLD_DMA_CHN_RF_TX); \
												rf_irq_enable(FLD_RF_IRQ_RX | FLD_RF_IRQ_TX); \
												irq_set_mask(FLD_IRQ_ZB_RT_EN); \
											}while(0)


/* tx packet header */
#define ZB_RADIO_TX_HDR_LEN				5

/* rx packet header */
#define ZB_RADIO_RX_HDR_LEN				5

/* get real payload length */
#define ZB_RADIO_ACTUAL_PAYLOAD_LEN(p)	RF_ZIGBEE_PACKET_PAYLOAD_LENGTH_GET(p)

/* packet CRC check */
#define ZB_RADIO_CRC_OK(p)				RF_ZIGBEE_PACKET_CRC_OK(p)

/* packet length check */
#define ZB_RADIO_PACKET_LENGTH_OK(p)	RF_ZIGBEE_PACKET_LENGTH_OK(p)

/* get time timestamp */
#define ZB_RADIO_TIMESTAMP_GET(p)		RF_ZIGBEE_PACKET_TIMESTAMP_GET(p)

#define ZB_RADION_PKT_RSSI_GET(p)		RF_ZIGBEE_PACKET_RSSI_GET(p)


/* tx Power 0dBm*/
#define ZB_RADIO_TX_0DBM				RF_POWER_P0p52dBm

/* radio initialization */
#define ZB_RADIO_INIT()					do{\
											rf_drv_init(RF_MODE_ZIGBEE_250K); \
											rf_drv_cap(CFG_FREQUENCY_OFFSET); \
										}while(0);

/* sys timer initialization for mac-csma */
#define ZB_TIMER_INIT()					hwTmr_init(TIMER_IDX_3, TIMER_MODE_SCLK)

static inline u8 ZB_RADIO_RSSI_TO_LQI(rf_rxGainMode_t mode, u8 inRssi){
	u8 lqi;
	s8 rssi = inRssi - 110;

	s16 minEd = -110;
	s16 maxEd = -15;  //AGC

	if(rssi < minEd){
		lqi = 20;
	}else if(rssi > maxEd){
		lqi = 255;
	}else{
		if(rssi > -40){
			lqi = 230;
		}else if(rssi > -60){
			lqi = 200;
		}else if(rssi > -75){
			lqi = 170;
		}else if(rssi > -85){
			lqi = 140;
		}else if(rssi > -90){
			lqi = 110;
		}else if(rssi > -95){
			lqi = 80;
		}else if(rssi > -100){
			lqi = 50;
		}
	}
	 return lqi;
}

#endif  /* __RF_H__ */
