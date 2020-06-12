/********************************************************************************************************
 * @file     usbcdc.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
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

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
    extern "C" {
#endif


typedef void (*cdc_handlerFn_t)(u8 *pData);

typedef struct {
    u16 len;
    u8  data[1];
} usbcdc_txBuf_t;

typedef struct {
    u8 *rxBuf;
    usbcdc_txBuf_t *txBuf;

	/* Following variables are used in the RX more than CDC_TXRX_EPSIZE */
	ev_time_event_t *timer;

	cdc_handlerFn_t rxCb;
    cdc_handlerFn_t txCb;

	u16 lenToSend;
	u16 lastSendIndex;
	u8  lastIndex;
} cdc_ctrl_t;

typedef enum usbcdc_sts_e {
	USB_SUCCESS = 0,
    USB_BUSY 	= 1,
    USB_MULTIBLOCK,
} usbcdc_sts_t;


/**
 * @brief This function handles the USB CDC class-specific requests from host
 * @param[in]   bRequest the request type
 * @param[in]   wValue word-sized field that varies according to request
 * @param[in]   wIndex word-sized field that varies according to request
 *              typically used to pass an index or offset
 * @param[in]   wLength number of bytes to transfer if there is a Data stage
 * @return none
 */
void usbcdc_processControlRequest(u8 bRequest, u16 wValue, u16 wIndex, u16 wLength);

/**
 * @brief This function used to send or receive data as the USB CDC device
 * @param   none
 * @return  none
 */
void usbcdc_dataHandler(void);

/**
 * @brief This function initializes the USB CDC device
 * @param   none
 * @return none
 */
void usbcdc_init(void);

/**
 * @brief This function sets the USB CDC tx and rx callback function
 * @param[in]   rxFunc rx callback function
 * @param[in]   txCb tx callback function
 * @return none
 */
void usbcdc_setCb(cdc_handlerFn_t rxFunc, cdc_handlerFn_t txCb);

/**
 * @brief This function sets the USB CDC rx buffer
 * @param[in]   buf pointer to the rx buffer
 * @return none
 */
void usbcdc_setRxBuf(u8 *buf);

/**
 * @brief This function used to send one byte to host
 * @param   none
 * @return 1: send out successfully 0: the USB interface is busy
 */
u8 usbWriteByte(u8 byte);

/**
 * @brief This function sends data to host as the USB CDC device
 * @param[in]   buf pointer to the data need to send
 * @param[in]   len length in byte of the data need to send
 * @return none
 */
usbcdc_sts_t usbcdc_sendData(usbcdc_txBuf_t *txBuf);//u8 *buf, u8 len);

/**
 * @brief This function determines whether the USB interface is available
 * @param   none
 * @return 1: the USB interface is available 0: the USB interface is busy
 */
u8 usbcdc_isAvailable(void);

/**
 * @brief This function used to check whether the Tx of the control endpoint is busy
 * @param   none
 * @return 1: the control endpoint is busy 0: the control endpoint is idle
 */
u8 usbcdc_isTxBusy(void);



/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
    }
#endif
