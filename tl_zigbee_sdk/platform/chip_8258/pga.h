/********************************************************************************************************
 * @file     pga.h
 *
 * @brief    gain adjust configuration interface for tlsr8258
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
#ifndef		pga_H
#define		pga_H

#include "bsp.h"

enum PREAMPValue{
	DB26,
	DB46,
};

enum POSTAMPValue{
	DB_N10= 0x00,
	DB_N5 = 0x0a,
	DB_0  = 0x14,
	DB_5  = 0x1d,
	DB_10 = 0x28,
	DB_14 = 0x30,
};

typedef enum{
	PGA_LEFT_CHN  = BIT(6),
	PGA_RIGHT_CHN = BIT(7)
}PGA_CHN_Typdef;

#define		SET_PGA_CHN_ON(v)				do{\
												unsigned char val=ReadAnalogReg(0x80+124);\
												val &= (~(v));\
												WriteAnalogReg(0x80+124,val);\
											}while(0)
enum{
	PGA_AIN_C0,
	PGA_AIN_C1,
	PGA_AIN_C2,
	PGA_AIN_C3,
};

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

enum{
	PGA_POST_GAIN		= BIT_RNG(0,5),
	PGA_PRE_GAIN_26DB	= BIT(6)
};
#define		REG_PGA_GAIN				READ_REG8(0xb63)


/**************************************************************
*
*	@brief	pga initiate function, call this function to enable the PGA module
*			the input channel deafult set to ANA_C<3> and ANA_C<2>
*
*	@para	chn - select PGA_LEFT_CHN or PGA_RIGHT_CHN to initiate the PGA
*
*	@return	None
*
*/
extern void pgaInit(unsigned char chn);

/**************************************************************
*
*	@brief	adjust pre-amplifier gain value
*
*	@para	preGV - enum var of PREAMPValue, 0DB or 20DB
*
*	@return	'1' adjusted; '0' adjust error
*
*/
unsigned char preGainAdjust(enum PREAMPValue preGV);

/**************************************************************
*
*	@brief	adjust post-amplifier gain value
*
*	@para	posGV - enum var of POSTAMPValue, 0,3,6 or 9dB
*
*	@return	'1' adjusted; '0' adjust error
*
*/
unsigned char postGainAdjust(enum POSTAMPValue posGV);


#endif
