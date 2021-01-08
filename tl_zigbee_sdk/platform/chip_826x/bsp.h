/********************************************************************************************************
 * @file	bsp.h
 *
 * @brief	This is the header file for B86
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
#ifndef BSP_H
#define BSP_H


/****************************************************
 	 	 	 	 Reg operations
 *****************************************************/
#define REG_BASE_ADDR				0x800000

#define REG_ADDR8(a)				(*(volatile unsigned char  *)(REG_BASE_ADDR + (a)))
#define REG_ADDR16(a)				(*(volatile unsigned short *)(REG_BASE_ADDR + (a)))
#define REG_ADDR32(a)				(*(volatile unsigned long  *)(REG_BASE_ADDR + (a)))

#define WRITE_REG8(addr,v)          (*(volatile unsigned char  *)(REG_BASE_ADDR + (addr)) = (unsigned char)(v))
#define WRITE_REG16(addr,v)         (*(volatile unsigned short *)(REG_BASE_ADDR + (addr)) = (unsigned short)(v))
#define WRITE_REG32(addr,v)         (*(volatile unsigned long  *)(REG_BASE_ADDR + (addr)) = (v))

#define READ_REG8(addr)             (*(volatile unsigned char  *)(REG_BASE_ADDR + (addr)))
#define READ_REG16(addr)            (*(volatile unsigned short *)(REG_BASE_ADDR + (addr)))
#define READ_REG32(addr)            (*(volatile unsigned long  *)(REG_BASE_ADDR + (addr)))

#define write_reg8					WRITE_REG8
#define write_reg16					WRITE_REG16
#define write_reg32					WRITE_REG32

#define read_reg8					READ_REG8
#define read_reg16					READ_REG16
#define read_reg32					READ_REG32


#define TCMD_UNDER_RD				0x80
#define TCMD_UNDER_WR				0x40
#define TCMD_UNDER_BOTH				0xc0

#define TCMD_MASK					0x3f

#define TCMD_WRITE					0x3
#define TCMD_WAIT					0x7
#define TCMD_WAREG					0x8


typedef struct {
	unsigned char 	 rst0;
	unsigned char    rst1;
	unsigned char    rst2;
	unsigned char    clk0;
	unsigned char 	 clk1;
	unsigned char    clk2;
	unsigned char    clkctl;
} BSP_SysCtlTypeDef;


typedef enum {
    SYS_CLK_RC = 0,
    SYS_CLK_HS_DIV = 1,
} BSP_SysClkSelTypeDef;


typedef struct TBLCMDSET {
	unsigned short	adr;
	unsigned char	dat;
	unsigned char	cmd;
} TBLCMDSET;



static inline void mcu_reset(void){
	write_reg8(0x06f, 0x20);
}

int LoadTblCmdSet(const TBLCMDSET *pt, int size);

unsigned int rand(void);

unsigned char analog_read(unsigned char addr);
void analog_write(unsigned char addr, unsigned char v);
void analog_read_multi(unsigned char addr, unsigned char *v, int len);
void analog_write_multi(unsigned char addr, unsigned char *v, int len);

#define WriteAnalogReg(addr,v)	analog_write(addr,v)
#define ReadAnalogReg(addr)		analog_read(addr)

#endif /* BSP_H */
