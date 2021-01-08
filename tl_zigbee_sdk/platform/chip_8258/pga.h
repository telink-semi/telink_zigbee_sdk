/********************************************************************************************************
 * @file	pga.h
 *
 * @brief	This is the header file for B85
 *
 * @author	Driver & Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef		pga_H
#define		pga_H

#include "bsp.h"
#include "analog.h"

enum{
	PGA_AIN_C0,
	PGA_AIN_C1,
	PGA_AIN_C2,
	PGA_AIN_C3,
};


//set FLD_PGA_PRE_AMPLIFIER_GAIN of reg_pga_fix_value (0xb63<6>) 26dB OR 46dB

typedef enum{
	PGA_PRE_GAIN_18DB = 0,		//26dB  18dB
	PGA_PRE_GAIN_38DB = 1,		//46dB   38dB
}PGA_PreAmplifierTypeDef;


///set FLD_PGA_POST_AMPLIFIER_GAIN of reg_pga_fix_value (0xb63<0:5>) range -10~14dB
typedef enum{
	PGA_POST_GAIN_m10DB = 0,		//-10dB
	PGA_POST_GAIN_m9P5DB,
	PGA_POST_GAIN_m9DB,
	PGA_POST_GAIN_m8P5DB,
	PGA_POST_GAIN_m8DB,
	PGA_POST_GAIN_m7P5DB,
	PGA_POST_GAIN_m7DB,
	PGA_POST_GAIN_m6P5DB,
	PGA_POST_GAIN_m6DB,
	PGA_POST_GAIN_m5P5DB,
	PGA_POST_GAIN_m5DB,
	PGA_POST_GAIN_m4P5DB,
	PGA_POST_GAIN_m4DB,
	PGA_POST_GAIN_m3P5DB,
	PGA_POST_GAIN_m3DB,
	PGA_POST_GAIN_m2P5DB,
	PGA_POST_GAIN_m2DB,
	PGA_POST_GAIN_m1P5DB,
	PGA_POST_GAIN_m1DB,
	PGA_POST_GAIN_m0P5DB,
	PGA_POST_GAIN_0DB,				//0dB
	PGA_POST_GAIN_0P5DB,
	PGA_POST_GAIN_1DB,
	PGA_POST_GAIN_1P5DB,
	PGA_POST_GAIN_2DB,
	PGA_POST_GAIN_2P5DB,
	PGA_POST_GAIN_3DB,
	PGA_POST_GAIN_3P5DB,
	PGA_POST_GAIN_4DB,
	PGA_POST_GAIN_4P5DB,
	PGA_POST_GAIN_5DB,
	PGA_POST_GAIN_5P5DB,
	PGA_POST_GAIN_6DB,
	PGA_POST_GAIN_6P5DB,
	PGA_POST_GAIN_7DB,
	PGA_POST_GAIN_7P5DB,
	PGA_POST_GAIN_8DB,
	PGA_POST_GAIN_8P5DB,
	PGA_POST_GAIN_9DB,
	PGA_POST_GAIN_9P5DB,
	PGA_POST_GAIN_10DB,				//10dB
	PGA_POST_GAIN_10P5DB,
	PGA_POST_GAIN_11DB,
	PGA_POST_GAIN_11P5DB,
	PGA_POST_GAIN_12DB,
	PGA_POST_GAIN_12P5DB,
	PGA_POST_GAIN_13DB,
	PGA_POST_GAIN_13P5DB,
	PGA_POST_GAIN_14DB,				//14dB
}PGA_PostAmplifierTypeDef;


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
