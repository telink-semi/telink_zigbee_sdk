/********************************************************************************************************
 * @file	bsp.h
 *
 * @brief	This is the header file for B87
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
#ifndef BSP_H_
#define BSP_H_



/**
 *  Reg operations
 */
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

#define TCMD_UNDER_BOTH			0xc0
#define TCMD_UNDER_RD			0x80
#define TCMD_UNDER_WR			0x40

#define TCMD_MASK				0x3f

#define TCMD_WRITE				0x3
#define TCMD_WAIT				0x7
#define TCMD_WAREG				0x8


/**
 *  command table for special registers
 */
typedef struct TBLCMDSET {
	unsigned short	adr;
	unsigned char	dat;
	unsigned char	cmd;
} TBLCMDSET;

/**
 * @brief      This function performs a series of operations of writing digital or analog registers
 *             according to a command table
 * @param[in]  pt - pointer to a command table containing several writing commands
 * @param[in]  size  - number of commands in the table
 * @return     number of commands are carried out
 */

extern int LoadTblCmdSet(const TBLCMDSET * pt, int size);


/**
 * @brief      This function writes a byte data to analog register
 * @param[in]  addr - the address of the analog register needs to write
 * @param[in]  value  - the data will be written to the analog register
 * @param[in]  e - the end address of value
 * @param[in]  s - the start address of the value
 * @return     none
 */
extern void sub_wr_ana(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);

/**
 * @brief      This function writes a byte data to a specified analog register
 * @param[in]  addr - the address of the analog register needs to write
 * @param[in]  value  - the data will be written to the analog register
 * @param[in]  e - the end address of value
 * @param[in]  s - the start address of the value
 * @return     none
 */
extern void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);

/**
 * @brief      This function reboot mcu.
 * @return     none
 */
static inline void mcu_reset(void)
{
	write_reg8(0x06f,0x20);
}


#endif /* BSP_H_ */
