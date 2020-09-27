/********************************************************************************************************
 * @file     phy_radio.h
 *
 * @brief    Zigbee API for phy layer
 *
 * @author
 * @date     June 20, 2020
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
#ifndef  PHY_RADIO_H
#define  PHY_RADIO_H



/* radio module reset */
#define ZB_RADIO_RESET()

/* Tx/Rx swith */
#define ZB_RADIO_TRX_SWITCH(mode ,chn) 		rf_set_trx_state(mode, chn)

/* set tx power */
#define ZB_RADIO_TX_POWER_SET(level)		rf_set_power_level_index(level)

/* set rx gain mode: max gain: only use auto gain mode */
#define ZB_RADIO_MODE_MAX_GAIN()

/* set rx gain mode: auto gain, only use auto gain mode */
#define ZB_RADIO_MODE_AUTO_GAIN()

/* get RSSI */
#define ZB_RADIO_RSSI_GET()					rf_get_rssi()

/* start sending a packet */
#define ZB_RADIO_TX_START(txBuf)			rf_tx_pkt(txBuf)

/* clear packet transmission is done */
#define ZB_RADIO_TX_DONE					rf_tx_finish()

/* clear Tx status(interrupt status) */
#define ZB_RADIO_TX_DONE_CLR				rf_tx_finish_clear_flag()

/* clear Rx status(interrupt status) */
#define ZB_RADIO_RX_DONE_CLR				rf_rx_finish_clear_flag()

/* enable Tx */
#define ZB_RADIO_TX_ENABLE

/* disable Tx */
#define ZB_RADIO_TX_DISABLE

/* enable Rx */
#define ZB_RADIO_RX_ENABLE					RFDMA_RX_ENABLE

/* disable Rx */
#define ZB_RADIO_RX_DISABLE					RFDMA_RX_DISABLE

/* TRx status get */
#define ZB_RADIO_TRX_STA_GET()				rf_get_trx_state()

/* Rx buffer configure */
#define ZB_RADIO_RX_BUF_SET(addr)			rf_set_rx_buffer(addr)

/* Rx buffer clear */
#define ZB_RADIO_RX_BUF_CLEAR(p)			do{ \
												p[0] = 0;		\
											}while(0)

/* set Rx mode, maxium receiver buffer size, enable Rx/Tx interrupt */
#define ZB_RADIO_TRX_CFG(size)				do{ \
												rf_set_rx_maxlen(size);							\
												rf_rx_dma_cfg(1, size); 						\
												rf_tx_dma_cfg(1, 128);							\
												rf_set_irq_mask(FLD_RF_IRQ_RX|FLD_RF_IRQ_TX); 	\
												plic_interrupt_enable(IRQ15_ZB_RT); 			\
											}while(0)

#define ZB_RADIO_DMA_HDR_BUILD(pBuf, len)	do{	\
												u8 *p = pBuf;								\
												u32 txLen = rf_tx_packet_dma_len(len + 1);	\
												p[3] = (txLen >> 24) & 0xFF;				\
												p[2] = (txLen >> 16) & 0xFF;				\
												p[1] = (txLen >> 8) & 0xFF;					\
												p[0] = txLen & 0xFF;						\
											}while(0)

/* tx packet header */
#define ZB_RADIO_TX_HDR_LEN					5

/* rx packet header */
#define ZB_RADIO_RX_HDR_LEN					5

/* get real payload length */
#define ZB_RADIO_ACTUAL_PAYLOAD_LEN(p)		rf_zigbee_packet_payload_length_get(p)

/* packet CRC check */
#define ZB_RADIO_CRC_OK(p)					rf_zigbee_packet_crc_ok(p)

/* packet length check */
#define ZB_RADIO_PACKET_LENGTH_OK(p)		rf_zigbee_packet_length_ok(p)

/* enable timestamp */
#define ZB_TIMESTAMP_ENABLE

/* get time timestamp */
#define ZB_RADIO_TIMESTAMP_GET(p)			rf_zigbee_dma_rx_offset_time_stamp(p)

#define ZB_RADION_PKT_RSSI_GET(p)			rf_zigbee_dma_rx_offset_rssi(p)


/* tx Power 0dBm*/
#define ZB_RADIO_TX_0DBM					RF_POWER_INDEX_P0p01dBm

/* radio initialization */
#define ZB_RADIO_INIT()						do{ \
												rf_mode_init(); 			\
												rf_set_zigbee_250K_mode();	\
											}while(0)

/* sys timer initialization for mac-csma */
#define ZB_TIMER_INIT()						drv_hwTmr_init(TIMER_IDX_3, TIMER_MODE_SCLK)


typedef enum{
	RF_GAIN_MODE_AUTO,
	RF_GAIN_MODE_MANU_MAX,
}rf_rxGainMode_t;

#define ZB_RADIO_RSSI_TO_LQI(mode, inRssi, lqi)		do{ \
														(void)mode;					\
														s8 rssi = inRssi - 110;		\
														s16 minEd = -110;			\
														s16 maxEd = -15;  /* AGC */	\
														if(rssi < minEd){			\
															lqi = 20;				\
														}else if(rssi > maxEd){		\
															lqi = 255;				\
														}else{						\
															if(rssi > -40){			\
																lqi = 230;			\
															}else if(rssi > -60){	\
																lqi = 200;			\
															}else if(rssi > -75){	\
																lqi = 170;			\
															}else if(rssi > -85){	\
																lqi = 140;			\
															}else if(rssi > -90){	\
																lqi = 110;			\
															}else if(rssi > -95){	\
																lqi = 80;			\
															}else if(rssi > -100){	\
																lqi = 50;			\
															}						\
														}							\
													}while(0)

#endif	/* PHY_RADIO_H */
