/********************************************************************************************************
 * @file     bsp.h
 *
 * @brief    bsp interface header for tlsr8258
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

#ifndef BSP_H_
#define BSP_H_



#define REG_BASE_ADDR			0x800000



#define REG_ADDR8(a)			(*(volatile unsigned char*) (REG_BASE_ADDR + (a)))
#define REG_ADDR16(a)			(*(volatile unsigned short*)(REG_BASE_ADDR + (a)))
#define REG_ADDR32(a)			(*(volatile unsigned long*) (REG_BASE_ADDR + (a)))

#define write_reg8(addr,v)		(*(volatile unsigned char*)  (REG_BASE_ADDR + (addr)) = (unsigned char)(v))
#define write_reg16(addr,v)		(*(volatile unsigned short*) (REG_BASE_ADDR + (addr)) = (unsigned short)(v))
#define write_reg32(addr,v)		(*(volatile unsigned long*)  (REG_BASE_ADDR + (addr)) = (v))

#define read_reg8(addr)			(*(volatile unsigned char*) (REG_BASE_ADDR + (addr)))
#define read_reg16(addr)		(*(volatile unsigned short*)(REG_BASE_ADDR + (addr)))
#define read_reg32(addr)		(*(volatile unsigned long*) (REG_BASE_ADDR + (addr)))


#define WRITE_REG8				write_reg8
#define WRITE_REG16				write_reg16
#define WRITE_REG32				write_reg32

#define READ_REG8				read_reg8
#define READ_REG16				read_reg16
#define READ_REG32				read_reg32






#define TCMD_UNDER_RD		0x80
#define TCMD_UNDER_WR		0x40
#define TCMD_UNDER_BOTH		0xc0
#define TCMD_MASK			0x3f

#define TCMD_WRITE			0x3
#define TCMD_WAIT			0x7
#define TCMD_WAREG			0x8

typedef struct TBLCMDSET {
	unsigned short	adr;
	unsigned char	dat;
	unsigned char	cmd;
} TBLCMDSET;

int LoadTblCmdSet(const TBLCMDSET * pt, int size);

void sub_wr_ana(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);
void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);

extern unsigned long normalSpGet(void);

void mcu_reset(void);
#define SYSTEM_RESET()	mcu_reset()

#endif /* BSP_H_ */
