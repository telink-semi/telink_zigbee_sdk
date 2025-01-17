/********************************************************************************************************
 * @file    soc.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

#include "bit.h"
#include "error_handler/error_handler.h"

/*Attention: for all the 2-byte address operation (like REG_ADDR16,write_reg16,read_reg16) ,
 *           the address parameter should  be multiple of 2. For example:REG_ADDR16(0x2),REG_ADDR16(0x4).
 *
 *           for all the 4-byte address operation (like REG_ADDR32,write_reg32,read_reg32) ,
 *           the address parameter should  be multiple of 4. For example:write_reg32(0x4),write_reg32(0x8).
*/
#define FLASH_R_BASE_ADDR     0x20000000
#define REG_RW_BASE_ADDR      0x80000000
#define REG_ADDR8(a)          (*(volatile unsigned char *)(REG_RW_BASE_ADDR | (a)))
#define REG_ADDR16(a)         (*(volatile unsigned short *)(REG_RW_BASE_ADDR | (a)))
#define REG_ADDR32(a)         (*(volatile unsigned long *)(REG_RW_BASE_ADDR | (a)))

#define write_reg8(addr, v)   (*(volatile unsigned char *)(REG_RW_BASE_ADDR | (addr)) = (unsigned char)(v))
#define write_reg16(addr, v)  (*(volatile unsigned short *)(REG_RW_BASE_ADDR | (addr)) = (unsigned short)(v))
#define write_reg32(addr, v)  (*(volatile unsigned long *)(REG_RW_BASE_ADDR | (addr)) = (unsigned long)(v))

#define read_reg8(addr)       (*(volatile unsigned char *)(REG_RW_BASE_ADDR | (addr)))
#define read_reg16(addr)      (*(volatile unsigned short *)(REG_RW_BASE_ADDR | (addr)))
#define read_reg32(addr)      (*(volatile unsigned long *)(REG_RW_BASE_ADDR | (addr)))

#define write_sram8(addr, v)  (*(volatile unsigned char *)((addr)) = (unsigned char)(v))
#define write_sram16(addr, v) (*(volatile unsigned short *)((addr)) = (unsigned short)(v))
#define write_sram32(addr, v) (*(volatile unsigned long *)((addr)) = (unsigned long)(v))

#define read_sram8(addr)      (*(volatile unsigned char *)((addr)))
#define read_sram16(addr)     (*(volatile unsigned short *)((addr)))
#define read_sram32(addr)     (*(volatile unsigned long *)((addr)))
#define TCMD_UNDER_BOTH       0xc0
#define TCMD_UNDER_RD         0x80
#define TCMD_UNDER_WR         0x40

#define TCMD_MASK             0x3f

#define TCMD_WRITE            0x3
#define TCMD_WAIT             0x7
#define TCMD_WAREG            0x8
//#if 1 //optimize
/*
 * IRAM area:0x00000~0x3FFFF BIT(19) is 0,BIT(17~0) 256K is address offset
 * DRAM area:0x80000~0xBFFFF BIT(19) is 1,BIT(17~0) 256K is address offset
 * ILM area:0xc0000000~0xc0040000 BIT(31~30) is 3,BIT(21) is 0, BIT(20~18) do not care  BIT(17~0) 256K is address offset 256K is address offset
 * DLM area:0xc0200000~0xc0240000 BIT(31~30) is 3,BIT(21) is 1, BIT(20~18) do not care  BIT(17~0) 256K is address offset 256K is address offset
 * BIT(19) is used to distinguish from IRAM to DRAM, BIT(21) is used to distinguish from ILM to DLM.
 * so we can write it as follow
 * #define  convert_ram_addr_cpu2bus  (((((addr))&0x80000)? ((addr)| 0xc0200000) : ((addr)|0xc0000000)))
 * BIT(20~17) are invalid address line ,IRAM address is less than 0x80000, (address-0x80000)must borrow from BIT(21)
 *   #define convert(addr) ((addr)-0x80000+0xc0200000)
 *  to simplify
 *  #define convert(addr) ((addr)+0xc0180000)
 * */
//#define convert_ram_addr_cpu2bus(addr)   ((unsigned int)(addr)+0xc0180000)
//#else  //no optimize
//#define  convert_ram_addr_cpu2bus  (((((unsigned int)(addr)) >=0x80000)?(((unsigned int)(addr))-0x80000+0xc0200000) : (((unsigned int)(addr)) + 0xc0000000)))
//#endif
// go further, if the source and destination  address is not in the sram(IRAM/DRAM)  interval, no address translation
#define convert_ram_addr_cpu2bus(addr) (unsigned int)(addr) < 0xc0000 ? ((unsigned int)(addr) + 0xc0180000) : (unsigned int)(addr)
//When using convert_ram_addr_cpu2bus(addr) formula to write to the register, BIT(20~18) may not be zero,  need to clear the corresponding bit to zero
#define convert_ram_addr_bus2cpu(addr) (((((unsigned int)(addr)) >= 0xc0200000) ? (((unsigned int)(addr)) + 0x80000 - 0xc0200000) : (((unsigned int)(addr) & (~0x1c0000)) - 0xc0000000)))


#define LM_BASE                        0x80000000

#define ILM_BASE                       (LM_BASE + 0x40000000)
#define DLM_BASE                       (LM_BASE + 0x40200000)
#define CPU_ILM_BASE                   (0x00000000)
#define CPU_DLM_BASE                   (0x00080000)

/*******************************       sc registers: 0x1401c0      ******************************/
#define SC_BASE_ADDR     0x1401c0

#define reg_mspi_clk_set REG_ADDR8(SC_BASE_ADDR + 0x00)

enum
{
    FLD_MSPI_CLK_MOD    = BIT_RNG(0, 5),
    FLD_MSPI_DIV_IN_SEL = BIT(6), //0:rc24m   1:hs_mux
    FLD_MSPI_DIV_RSTN   = BIT(7), //0:rset clkdiv  1:release
};

#define reg_lspi_clk_set REG_ADDR8(SC_BASE_ADDR + 0x01)

enum
{
    FLD_LSPI_CLK_MOD    = BIT_RNG(0, 5),
    FLD_LSPI_DIV_IN_SEL = BIT(6), //0:rc24m   1:hs_mux
    FLD_LSPI_DIV_RSTN   = BIT(7), //0:rset clkdiv  1:release
};

#define reg_gspi_clk_set REG_ADDR16(SC_BASE_ADDR + 0x2)

enum
{
    FLD_GSPI_CLK_MOD    = BIT_RNG(0, 7),
    FLD_GSPI_DIV_IN_SEL = BIT(14), //0:rc24m   1:hs_mux
    FLD_GSPI_DIV_RSTN   = BIT(15), //0:rset clkdiv  1:release
};

#define reg_efuse_addr REG_ADDR8(SC_BASE_ADDR + 0x08)
#define reg_efuse_rdat REG_ADDR8(SC_BASE_ADDR + 0x09)
#define reg_efuse_wdat REG_ADDR8(SC_BASE_ADDR + 0x0a)
#define reg_efuse_ctrl REG_ADDR8(SC_BASE_ADDR + 0x0b)

enum
{
    FLD_EFUSE_WREN    = BIT(0),
    FLD_EFUSE_RDEN    = BIT(1),
    FLD_EFUSE_CLKEN   = BIT(2),
    FLD_EFUSE_WR_TRIG = BIT(3),
    //RSVD
    //RSVD
    //RSVD
    FLD_EFUSE_BUSY = BIT(7),
};

#define reg_efuse_b0      REG_ADDR32(SC_BASE_ADDR + 0x0c)
#define reg_efuse_read_en REG_ADDR8(SC_BASE_ADDR + 0x34)

/*******************************      reset registers: 1401e0      ******************************/
#define reg_rst  REG_ADDR32(0x1401e0)

#define reg_rst0 REG_ADDR8(0x1401e0)

enum
{
    FLD_RST0_LSPI  = BIT(0),
    FLD_RST0_I2C   = BIT(1),
    FLD_RST0_UART0 = BIT(2),
    FLD_RST0_USB   = BIT(3),
    FLD_RST0_PWM   = BIT(4),
    //RSVD
    FLD_RST0_UART1 = BIT(6),
    FLD_RST0_SWIRE = BIT(7),
};

#define reg_rst1 REG_ADDR8(0x1401e1)

enum
{
    FLD_RST1_CHG        = BIT(0),
    FLD_RST1_SYS_STIMER = BIT(1),
    FLD_RST1_DMA        = BIT(2),
    FLD_RST1_ALGM       = BIT(3),
    FLD_RST1_PKE        = BIT(4),
    //RSVD
    FLD_RST1_GSPI   = BIT(6),
    FLD_RST1_SPISLV = BIT(7),
};

#define reg_rst2 REG_ADDR8(0x1401e2)

enum
{
    FLD_RST2_TIMER = BIT(0),
    FLD_RST2_AUD   = BIT(1),
    FLD_RST2_I2C1  = BIT(2),
    //RSVD
    FLD_RST2_MCU  = BIT(4),
    FLD_RST2_LM   = BIT(5),
    FLD_RST2_TRNG = BIT(6),
    FLD_RST2_DPR  = BIT(7),
};

#define reg_rst3 REG_ADDR8(0x1401e3)

enum
{
    FLD_RST3_ZB       = BIT(0),
    FLD_RST3_MSTCLK   = BIT(1),
    FLD_RST3_LPCLK    = BIT(2),
    FLD_RST3_ZB_CRYPT = BIT(3),
    FLD_RST3_MSPI     = BIT(4),
    FLD_RST3_QDEC     = BIT(5),
    FLD_RST3_SARADC   = BIT(6),
    FLD_RST3_ALG      = BIT(7),
};

#define reg_clk_en  REG_ADDR32(0x1401e4)

#define reg_clk_en0 REG_ADDR8(0x1401e4)

enum
{
    FLD_CLK0_LSPI_EN  = BIT(0),
    FLD_CLK0_I2C_EN   = BIT(1),
    FLD_CLK0_UART0_EN = BIT(2),
    FLD_CLK0_USB_EN   = BIT(3),
    FLD_CLK0_PWM_EN   = BIT(4),
    FLD_CLK0_DBGEN_EN = BIT(5),
    FLD_CLK0_UART1_EN = BIT(6),
    FLD_CLK0_SWIRE_EN = BIT(7),
};

#define reg_clk_en1 REG_ADDR8(0x1401e5)

enum
{
    FLD_CLK1_CHG_EN         = BIT(0),
    FLD_CLK1_SYS_TIMER_EN   = BIT(1),
    FLD_CLK1_DMA_EN         = BIT(2),
    FLD_CLK1_ALGM_EN        = BIT(3),
    FLD_CLK1_PKE_EN         = BIT(4),
    FLD_CLK1_MACHINETIME_EN = BIT(5),
    FLD_CLK1_GSPI_EN        = BIT(6),
    FLD_CLK1_SPISLV_EN      = BIT(7),

};

#define reg_clk_en2 REG_ADDR8(0x1401e6)

enum
{
    FLD_CLK2_TIMER_EN = BIT(0),
    FLD_CLK2_AUD_EN   = BIT(1),
    FLD_CLK2_I2C1_EN  = BIT(2),
    //RSVD
    FLD_CLK2_MCU_EN  = BIT(4),
    FLD_CLK2_LM_EN   = BIT(5),
    FLD_CLK2_TRNG_EN = BIT(6),
    FLD_CLK2_DPR_EN  = BIT(7),
};

#define reg_clk_en3 REG_ADDR8(0x1401e7)

enum
{
    FLD_CLK3_ZB_PCLK_EN   = BIT(0),
    FLD_CLK3_ZB_MSTCLK_EN = BIT(1),
    FLD_CLK3_ZB_LPCLK_EN  = BIT(2),
    //RSVD
    FLD_CLK3_MSPI_EN = BIT(4),
    FLD_CLK3_QDEC_EN = BIT(5),
    //RSVD
    //RSVD
};

#define reg_clk_sel0 REG_ADDR8(0x1401e8)

enum
{
    FLD_CLK_SCLK_DIV = BIT_RNG(0, 3),
    FLD_CLK_SCLK_SEL = BIT_RNG(4, 6),
};

#define reg_i2s_step(i) REG_ADDR16(SC_BASE_ADDR + 0x06 + ((i) * 0x16)) //i2s0-0x06 , i2s1-0x1c

enum
{
    FLD_I2S_STEP   = BIT_RNG(0, 14),
    FLD_I2S_CLK_EN = BIT(15),
};

#define reg_i2s_mod(i) REG_ADDR16(SC_BASE_ADDR + 0x2a - ((i) * 0x0C)) //i2s0-0x2a , i2s1-0x1e


#define reg_dmic_step  REG_ADDR16(SC_BASE_ADDR + 0x2c)

enum
{
    FLD_DMIC_STEP = BIT_RNG(0, 14),
    FLD_DMIC_SEL  = BIT(15),
};

#define reg_pwdn_en       REG_ADDR8(SC_BASE_ADDR + 0x2f)

#define reg_dmic_mod      REG_ADDR16(SC_BASE_ADDR + 0x36)

#define reg_probe_clk_sel REG_ADDR8(SC_BASE_ADDR + 0x1a)

#define reg_wakeup_en     REG_ADDR8(SC_BASE_ADDR + 0x2e)

enum
{
    FLD_USB_PWDN_I    = BIT(0),
    FLD_GPIO_WAKEUP_I = BIT(1),
    FLD_QDEC_WAKEUP_I = BIT(2),
    FLD_USB_RESUME    = BIT(4),
    FLD_STANDBY_EX    = BIT(5),
};

#define reg_wakeup_status 0x64

typedef enum
{
    FLD_WKUP_PAD   = BIT(0),
    FLD_WKUP_DIG   = BIT(1),
    FLD_WKUP_TIMER = BIT(2),
    FLD_WKUP_CMP   = BIT(3),
    FLD_WKUP_MDEC  = BIT(4),
    FLD_WKUP_CTB   = BIT(5),
    FLD_WKUP_VAD   = BIT(6),
    FLD_VBUS_ON    = BIT(7),
} wakeup_status_e;
