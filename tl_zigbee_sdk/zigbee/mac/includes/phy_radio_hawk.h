/********************************************************************************************************
 * @file     phy_radio_hawk.h
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
#ifndef  __PHY_RADIO_HAWK_H_
#define  __PHY_RADIO_HAWK_H_

#include "tl_common.h"
#include "mac_phy.h"

/* radio module reset */
#define ZB_RADIO_RESET()					 RF_reset()

/* Tx/Rx swith */
#define ZB_RADIO_TRX_SWITCH(mode ,chn) 		RF_TrxStateSet(mode, chn)

/* set tx power */
#define ZB_RADIO_TX_POWER_SET(level)		RF_PowerLevelSet(level)

/* set rx gain mode: max gain */
#define ZB_RADIO_MODE_MAX_GAIN()			RF_SetGainManualMax()

/* set rx gain mode: auto gain */
#define ZB_RADIO_MODE_AUTO_GAIN()			RF_SetAgc()

/* get RSSI */
#define ZB_RADIO_RSSI_GET()					RF_GetRssi()

/* start sending a packet */
#define ZB_RADIO_TX_START(txBuf)			RF_TxPkt(txBuf)

/* clear packet transmission is done */
#define ZB_RADIO_TX_DONE					RF_TxFinish()

/* clear Tx status(interrupt status) */
#define ZB_RADIO_TX_DONE_CLR				RF_TxFinishClearFlag()

/* clear Rx status(interrupt status) */
#define ZB_RADIO_RX_DONE_CLR				RF_RxFinishClearFlag()

/* enable Tx */
#define ZB_RADIO_TX_ENABLE					RFDMA_TX_ENABLE

/* disable Tx */
#define ZB_RADIO_TX_DISABLE					RFDMA_TX_DISABLE

/* enable Rx */
#define ZB_RADIO_RX_ENABLE					RFDMA_RX_ENABLE

/* disable Rx */
#define ZB_RADIO_RX_DISABLE					RFDMA_RX_DISABLE

/* TRx status get */
#define ZB_RADIO_TRX_STA_GET()				RF_TrxStateGet()

/* Rx buffer configure */
#define ZB_RADIO_RX_BUF_SET(addr)			{reg_dma_rf_rx_addr = addr;}

/* Rx buffer clear */
#define ZB_RADIO_RX_BUF_CLEAR(p)				do{  \
													p[12] = 0;		\
													*((u32*)p) = 0; \
													}while(0)

/* set Rx mode, maxium receiver buffer size, enable Rx/Tx interrupt */
#define ZB_RADIO_TRX_CFG(len)	do{				\
											reg_dma2_ctrl = FLD_DMA_WR_MEM | ((len)>>4);   \
											reg_dma_chn_irq_msk  &= ~(FLD_DMA_RF_RX|FLD_DMA_RF_TX); \
											reg_irq_mask |= FLD_IRQ_ZB_RT_EN;   \
											reg_rf_irq_mask = FLD_RF_IRQ_RX | FLD_RF_IRQ_TX; \
										}while(0)

/* get real payload length */
#define ZB_RADIO_ACTUAL_PAYLOAD_LEN(p)	(p[12])

/* packet CRC check */
#define ZB_RADIO_CRC_OK(p)				RF_PACKET_CRC_OK(p)

/* packet length check */
#define ZB_RADIO_PACKET_LENGTH_OK(p)	RF_PACKET_LENGTH_OK(p)

/* get time timestamp */
#define ZB_RADIO_TIMESTAMP_GET(p)		RF_GET_TIMESTAMP(p)

#define ZB_RADION_PKT_RSSI_GET(p)		RF_GET_PKT_RSSI(p)

/* tx packet header */
#define ZB_RADIO_TX_HDR_LEN				5

/* rx packet header */
#define ZB_RADIO_RX_HDR_LEN				13


static inline u8 ZB_RADIO_RSSI_TO_LQI(rf_rxGainMode_t mode, u8 inRssi){
	 u8 lqi;
	 s8 rssi = inRssi - 110;

	 s16 minEd = -106;
	 s16 maxEd = -58;  //maxGain

	 if(mode == RF_GAIN_MODE_AUTO){
		 maxEd = -6;   //Agc
	 }

	 lqi = 255*(rssi - minEd)/(maxEd - minEd); //LQI = 255* (rssi - MIN_ED_m106)/(MAX_ED_m6 - MIN_ED_m110)
	 return lqi;
}

/* tx Power 0dBm*/
#define ZB_RADIO_TX_0DBM	RF_POWER_0dBm

/* radio initialization */
#define ZB_RADIO_INIT()		RF_Init(RF_MODE_ZIGBEE_250K)

/* hardware timer initialization for mac-csma */
#define ZB_TIMER_INIT()		hwTmr_init(Timer1, M0_SYS_CLK)


#endif  /* __RF_H__ */
