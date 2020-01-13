/********************************************************************************************************
 * @file     pga.h 
 *
 * @brief    This is the header file for TLSR8278
 *
 * @author	 Driver Group
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *         
 *******************************************************************************************************/
#ifndef		pga_H
#define		pga_H

#include "bsp.h"


enum{
	PGA_AIN_C0,
	PGA_AIN_C1,
	PGA_AIN_C2,
	PGA_AIN_C3,
};

//this config for PGA value.
enum{
	PGA_GAIN_VOL_45_2DB   = 	    0x00,//45.2dB
	PGA_GAIN_VOL_43_5DB   =  		0x01,
	PGA_GAIN_VOL_42_1DB   = 	    0x02,
    PGA_GAIN_VOL_40_5DB   =  		0x03,
    PGA_GAIN_VOL_39_1DB   = 	    0x04,
    PGA_GAIN_VOL_37_4DB   =  		0x05,
    PGA_GAIN_VOL_36_0DB   = 	    0x06,
    PGA_GAIN_VOL_34_6DB   =  		0x07,
    PGA_GAIN_VOL_33_0DB   = 	    0x08,//33.0dB
    PGA_GAIN_VOL_30_1DB   =  		0x09,
    PGA_GAIN_VOL_27_0DB   = 	    0x0a,
    PGA_GAIN_VOL_24_0DB   =  		0x0b,
    PGA_GAIN_VOL_21_0DB   = 	    0x0c,
    PGA_GAIN_VOL_15_0DB   =  		0x0d,
    PGA_GAIN_VOL_9_0DB    = 	    0x0e,
    PGA_GAIN_VOL_0_0DB    =  		0x0f,
};

#define		SET_PGA_CHN_ON(v)				do{\
												unsigned char val=analog_read(0x80+124);\
												val &= (~(v));\
												analog_write(0x80+124,val);\
											}while(0)
#define		SET_PGA_LEFT_P_AIN(v)			do{\
												unsigned char val=ReadAnalogReg(0x80+125);\
												val &= 0xfc;\
												val |= (((unsigned char)(v))&0x03);\
												WriteAnalogReg(0x80+125,val);\
											}while(0)

#define		SET_PGA_LEFT_N_AIN(v)			do{\
												unsigned char val=ReadAnalogReg(0x80+125);\
												val &= 0xf3;\
												val |= (((unsigned char)(v)<<2)&0x0c);\
												WriteAnalogReg(0x80+125,val);\
											}while(0)

#define		SET_PGA_RIGHT_P_AIN(v)			do{\
												unsigned char val=ReadAnalogReg(0x80+125);\
												val &= 0xcf;\
												val |= (((unsigned char)(v)<<4)&0x30);\
												WriteAnalogReg(0x80+125,val);\
											}while(0)

#define		SET_PGA_RIGHT_N_AIN(v)			do{\
												unsigned char val=ReadAnalogReg(0x80+125);\
												val &= 0x3f;\
												val |= (((unsigned char)(v)<<6)&0xc0);\
												WriteAnalogReg(0x80+125,val);\
											}while(0)

#define		SET_PGA_GAIN_FIX_VALUE(v)		do{\
												unsigned char val=0;\
												val |= (((unsigned char)(v))&0x7f);\
												write_reg8(0xb63,val|0x80);\
											}while(0)



#endif
