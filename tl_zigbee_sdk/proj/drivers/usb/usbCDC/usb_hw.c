/********************************************************************************************************
 * @file     usb_hw.c
 *
 * @brief    This file provides set of functions to manage the USB interface
 *
 * @author   xiaodong.zong@telink-semi.com; jian.zhang@telink-semi.com
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
#include "../../../tl_common.h"
#include "drivers.h"
//#include "usb_hw.h"
//#define SET_FLD(x, mask)    			SETB(x, mask)
//#define CLR_FLD(x, mask)    			CLRB(x, mask)
//#define FLIP_FLD(x, mask)    			FLIPB(x, mask)

#if 0
inline  void  USBHW_Ep8FifoModeSet(unsigned char dat)
{
	REG_USB_EP8_FIFO_MODE = dat;
}


inline void USBHW_PrinterThresholdSet(unsigned char th) {
	REG_USB_EP8_SEND_THRE = th;
}

inline unsigned long USBHW_CtrlEpIrqGet(void) {
	return REG_CTRL_EP_IRQ_STA;
}

inline void USBHW_CtrlEpIrqClr(int irq) {
	REG_CTRL_EP_IRQ_STA = irq;
}
inline void USBHW_CtrlEpCtrlWrite(unsigned char data) {
	REG_CTRL_EP_CTRL = data;
}

// Reset the buffer pointer
inline void USBHW_CtrlEpPtrReset(void) {
	REG_CTRL_EP_PTR = 0;
}


inline unsigned char USBHW_CtrlEpDataRead(void) {
	return REG_CTRL_EP_DAT;
}


inline void USBHW_CtrlEpDataWrite(unsigned char data) {
	REG_CTRL_EP_DAT = data;
}

inline unsigned char USBHW_IsCtrlEpBusy() {
	return REG_CTRL_EP_IRQ_STA & FLD_USB_EP_BUSY;
}

inline unsigned char USBHW_EpDataRead(unsigned long ep) {
	return REG_USB_EP_DAT(ep & 0x07);
}

inline void USBHW_EpDataWrite(unsigned long ep, unsigned char data) {
	REG_USB_EP_DAT(ep & 0x07) = data;
}

inline unsigned long USBHW_IsEpBusy(unsigned long ep) {
	return REG_USB_EP_CTRL(ep & 0x07) & FLD_USB_EP_BUSY;
}

inline void USBHW_DataEpAck(unsigned long ep) {
	REG_USB_EP_CTRL(ep & 0x07) = FLD_USB_EP_BUSY;
}

inline void USBHW_DataEpStall(unsigned long ep) {
	REG_USB_EP_CTRL(ep & 0x07) = FLD_USB_EP_STALL;
}

inline void USBHW_EpPtrReset(unsigned long ep) {
	REG_USB_EP_PTR(ep & 0x07) = 0;
}
#endif

// some API:
// Endpont8 is the alias of endpoint0
void USBHW_ManualInterruptDisable(int m) {
	//SET_FLD(reg_ctrl_ep_irq_mode, m);
	// SET_FLD(0x800104, m);
	REG_CTRL_EP_IRQ_MODE |= m;

}

void USBHW_ManualInterruptEnable(int m) {
	//CLR_FLD(reg_ctrl_ep_irq_mode, m);
	// CLR_FLD(0x800104, m);
	REG_CTRL_EP_IRQ_MODE &= (~m);
	//WRITE_REG8 (0x800104,0x5f);
}

void USBHW_EpWrite(unsigned long ep,unsigned char * data, int len) {
	int i;
	REG_USB_EP_PTR(ep) = 0;

	for (i = 0; i < len; i++)
		REG_USB_EP_DAT(ep) = data[i];

	REG_USB_EP_CTRL(ep) = FLD_EP_DAT_ACK;		// ACK
}

// handy help function
void USBHW_CtrlEpU16Write(unsigned short v){
	USBHW_CtrlEpDataWrite(v & 0xff);
	USBHW_CtrlEpDataWrite(v >> 8);
}

unsigned short USBHW_CtrlEpU16Read(void){
	unsigned short v = USBHW_CtrlEpDataRead();
	return (USBHW_CtrlEpDataRead() << 8) | v;
}




