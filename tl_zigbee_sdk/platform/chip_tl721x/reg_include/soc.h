/********************************************************************************************************
 * @file    soc.h
 *
 * @brief   This is the header file for TL721X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#define LM_BASE               0x80000000

#define ILM_BASE              (LM_BASE + 0x40000000)
#define DLM_BASE              (LM_BASE + 0x40200000)
#define CPU_ILM_BASE          (0x00000000)
#define CPU_DLM_BASE          (0x00080000)

#define SC_BASE_ADDR          0x80140800
#define SC_BB_BASE_ADDR       0x80170C00
#define reg_mspi_clk_set      REG_ADDR8(SC_BASE_ADDR + 0x00)

enum
{
    FLD_MSPI_CLK_MOD    = BIT_RNG(0, 3),
    FLD_MSPI_DIV_IN_SEL = BIT_RNG(4, 5), //0:rc24m 1:xtl48m 2:pll0 3:pll1
};

#define reg_lspi_clk_set REG_ADDR8(SC_BASE_ADDR + 0x01)

enum
{
    FLD_LSPI_CLK_MOD    = BIT_RNG(0, 3),
    FLD_LSPI_DIV_IN_SEL = BIT_RNG(4, 5), //0:rc24m   1:xtl 24m  2:pll
};

#define reg_gspi_clk_set REG_ADDR16(SC_BASE_ADDR + 0x2)

enum
{
    FLD_GSPI_CLK_MOD    = BIT_RNG(0, 7),
    FLD_GSPI_DIV_IN_SEL = BIT_RNG(8, 9), //0:rc24m   1:xtl 24m  2:pll
};

#define reg_probe_clk_sel REG_ADDR16(SC_BASE_ADDR + 0x1a)
/*******************************      reset registers: SC_BASE_ADDR+0x20      ******************************/
#define reg_rst  REG_ADDR32(SC_BASE_ADDR + 0x20)

#define reg_rst0 REG_ADDR8(SC_BASE_ADDR + 0x20)

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

#define reg_rst1 REG_ADDR8(SC_BASE_ADDR + 0x21)

enum
{
    //RSVD
    FLD_RST1_SYS_STIMER = BIT(1),
    FLD_RST1_DMA        = BIT(2),
    FLD_RST1_ALGM       = BIT(3),
    FLD_RST1_PKE        = BIT(4),
    //RSVD
    FLD_RST1_GSPI   = BIT(6),
    FLD_RST1_SPISLV = BIT(7),
};

#define reg_rst2 REG_ADDR8(SC_BASE_ADDR + 0x22)

enum
{
    FLD_RST2_TIMER           = BIT(0),
    FLD_RST2_AUD             = BIT(1),
    FLD_RST2_I2C1            = BIT(2),
    FLD_RST2_REST_MCU_DIS    = BIT(3),
    FLD_RST2_MCU_REST_ENABLE = BIT(4),
    FLD_RST2_LM              = BIT(5),
    FLD_RST2_TRNG            = BIT(6),
    FLD_RST2_DPR             = BIT(7),
};

#define reg_rst3 REG_ADDR8(SC_BASE_ADDR + 0x23)

enum
{
    //RSVD
    FLD_RST3_TRACE = BIT(1),
    FLD_RST3_BROM  = BIT(2),
    //RSVD
    FLD_RST3_MSPI   = BIT(4),
    FLD_RST3_QDEC   = BIT(5),
    FLD_RST3_SARADC = BIT(6),
    FLD_RST3_ALG    = BIT(7),
};

#define reg_rst_1 REG_ADDR32(SC_BASE_ADDR + 0x40)
#define reg_rst4  REG_ADDR8(SC_BASE_ADDR + 0x40)

enum
{
    //RSVD
    FLD_RST4_SKE  = BIT(4),
    FLD_RST4_HASH = BIT(5),
    //RSVD
    FLD_RST4_ZB = BIT(7),
};

#define reg_rst5 REG_ADDR8(SC_BASE_ADDR + 0x41)

enum
{
    //RSVD
    FLD_RST5_UART2 = BIT(1),
    //RSVD
    FLD_RST5_IR_LEARN = BIT(4),
    FLD_RST5_KEY_SCAN = BIT(5),
    FLD_RST5_PEM      = BIT(6),
    FLD_RST5_CHACHA20 = BIT(7),
};

#define reg_rst6 REG_ADDR8(SC_BASE_ADDR + 0x42)

enum
{
    //RSVD
    FLD_RST6_RZ = BIT(1),

};

#define reg_rst7    REG_ADDR8(SC_BASE_ADDR + 0x43)


#define reg_clk_en  REG_ADDR32(SC_BASE_ADDR + 0x24)

#define reg_clk_en0 REG_ADDR8(SC_BASE_ADDR + 0x24)

enum
{
    FLD_CLK0_LSPI_EN  = BIT(0),
    FLD_CLK0_I2C_EN   = BIT(1),
    FLD_CLK0_UART0_EN = BIT(2),
    FLD_CLK0_USB_EN   = BIT(3),
    FLD_CLK0_PWM_EN   = BIT(4),
    FLD_CLK0_DBGEN    = BIT(5),
    FLD_CLK0_UART1_EN = BIT(6),
    FLD_CLK0_SWIRE_EN = BIT(7),
};

#define reg_clk_en1 REG_ADDR8(SC_BASE_ADDR + 0x25)

enum
{
    //RSVD
    FLD_CLK1_SYS_STIMER_EN = BIT(1),
    FLD_CLK1_DMA_EN        = BIT(2),
    FLD_CLK1_ALGM_EN       = BIT(3),
    FLD_CLK1_PKE_EN        = BIT(4),
    FLD_CLK1_MACHINETIME   = BIT(5),
    FLD_CLK1_GSPI_EN       = BIT(6),
    FLD_CLK1_SPISLV_EN     = BIT(7),

};

#define reg_clk_en2 REG_ADDR8(SC_BASE_ADDR + 0x26)

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

#define reg_clk_en3 REG_ADDR8(SC_BASE_ADDR + 0x27)

enum
{
    //RSVD
    FLD_CLK3_TRACE_EN = BIT(1),
    FLD_CLK3_BROM_EN  = BIT(2),
    //RSVD
    FLD_CLK3_MSPI_EN   = BIT(4),
    FLD_CLK3_QDEC_EN   = BIT(5),
    FLD_CLK3_SARADC_EN = BIT(6),
    //RSVD
};

#define reg_pwdn_en REG_ADDR8(SC_BASE_ADDR + 0x2f)

enum
{
    FLD_SUSPEND_EN_O     = BIT(0),
    FLD_RAMCRC_CLREN_TGL = BIT(4),
    FLD_RST_ALL          = BIT(5),
    FLD_STALL_EN_TRG     = BIT(7),
};

#define reg_clk_en_1 REG_ADDR32(SC_BASE_ADDR + 0x44)
#define reg_clk_en4  REG_ADDR8(SC_BASE_ADDR + 0x44)

enum
{
    //RSVD
    FLD_CLK4_SKE_EN  = BIT(4),
    FLD_CLK4_HASH_EN = BIT(5),
    FLD_CLK4_CCLK_EN = BIT(6),
    FLD_CLK4_ZB_EN   = BIT(7),
};

#define reg_clk_en5 REG_ADDR8(SC_BASE_ADDR + 0x45)

enum
{
    //RSVD
    FLD_CLK5_UART2_EN = BIT(1),
    //RSVD
    FLD_CLK5_IR_LEARN_EN = BIT(4),
    FLD_CLK5_KEYSCAN_EN  = BIT(5),
    FLD_CLK5_PEM_EN      = BIT(6),
    FLD_CLK5_CHACHA20_EN = BIT(7),
};

#define reg_clk_en6 REG_ADDR8(SC_BASE_ADDR + 0x46)

enum
{
    FLD_CLK6_RZ_EN = BIT(0),
};

#define reg_clk_en7  REG_ADDR8(SC_BASE_ADDR + 0x47)


#define reg_hclk_sel REG_ADDR8(SC_BASE_ADDR + 0x28)

enum
{
    FLD_CLK_SCLK_DIV = BIT_RNG(0, 3),
    FLD_CLK_SCLK_SEL = BIT_RNG(4, 6),
};

#define reg_clkzb_set REG_ADDR8(SC_BASE_ADDR + 0x56)

#define reg_i2s0_step REG_ADDR16(SC_BASE_ADDR + 0x06)

enum
{
    FLD_I2S0_STEP     = BIT_RNG(0, 14),
    FLD_I2S0_CLK_EN_0 = BIT(15),
};

#define reg_i2s1_step REG_ADDR16(SC_BASE_ADDR + 0x1c)

enum
{
    FLD_I2S1_STEP     = BIT_RNG(0, 14),
    FLD_I2S1_CLK_EN_0 = BIT(15),
};

#define reg_i2s2_step REG_ADDR16(SC_BASE_ADDR + 0x08)

enum
{
    FLD_I2S2_STEP     = BIT_RNG(0, 14),
    FLD_I2S2_CLK_EN_0 = BIT(15),
};

#define reg_i2s0_mod  REG_ADDR16(SC_BASE_ADDR + 0x2a)

#define reg_i2s1_mod  REG_ADDR16(SC_BASE_ADDR + 0x1e)

#define reg_i2s2_mod  REG_ADDR16(SC_BASE_ADDR + 0x0a)

#define reg_dmic_mod  REG_ADDR16(SC_BASE_ADDR + 0x36)

#define reg_dmic_step REG_ADDR16(SC_BASE_ADDR + 0x2c)

enum
{
    FLD_DMIC_STEP = BIT_RNG(0, 14),
    FLD_DMIC_SEL  = BIT(15),
};

#define AUDIO_SRAM_EMA_ADDR     (SC_BASE_ADDR + 0x68)
#define AUDIO_SRAM_EMA_DATA_LEN 2
#define reg_audio_reg_cfg0      REG_ADDR8(SC_BASE_ADDR + 0x68)

enum
{
    FLD_AUDIO_REG_EMA  = BIT_RNG(0, 2),
    FLD_AUDIO_REG_EMAW = BIT_RNG(3, 4),
    FLD_AUDIO_REG_EMAS = BIT(5),
};

#define reg_audio_reg_cfg1 REG_ADDR8(SC_BASE_ADDR + 0x69)

enum
{
    FLD_AUDIO_REG_RAWL  = BIT(0),
    FLD_AUDIO_REG_RAWLM = BIT_RNG(1, 2),
    FLD_AUDIO_REG_WABL  = BIT(3),
    FLD_AUDIO_REG_WABLM = BIT_RNG(4, 5),
};

#define USB_SRAM_EMA_ADDR     (SC_BASE_ADDR + 0x6c)
#define USB_SRAM_EMA_DATA_LEN 2
#define reg_usb_reg_cfg0      REG_ADDR8(SC_BASE_ADDR + 0x6c)

enum
{
    FLD_USB_REG_EMA  = BIT_RNG(0, 2),
    FLD_USB_REG_EMAW = BIT_RNG(3, 4),
    FLD_USB_REG_EMAS = BIT(5),
};

#define reg_usb_reg_cfg1 REG_ADDR8(SC_BASE_ADDR + 0x6d)

enum
{
    FLD_USB_REG_RAWL  = BIT(0),
    FLD_USB_REG_RAWLM = BIT_RNG(1, 2),
    FLD_USB_REG_WABL  = BIT(3),
    FLD_USB_REG_WABLM = BIT_RNG(4, 5),
};

#define SOC_SRAM_EMA_ADDR     (SC_BASE_ADDR + 0x70)
#define SOC_SRAM_EMA_DATA_LEN 4
#define reg_soc_sram_cfg0      REG_ADDR8(SC_BASE_ADDR + 0x70)

enum
{
    FLD_SRAM_EMA  = BIT_RNG(0, 2),
    FLD_SRAM_EMAW = BIT_RNG(3, 4),
    FLD_SRAM_EMAS = BIT(5),
};

#define reg_soc_sram_cfg1 REG_ADDR8(SC_BASE_ADDR + 0x71)

enum
{
    FLD_SRAM_RAML  = BIT(0),
    FLD_SRAM_RAWLM = BIT_RNG(1, 2),
    FLD_SRAM_WABL  = BIT(3),
    FLD_SRAM_WABLM = BIT_RNG(4, 6),
    FLD_SRAM_STOV  = BIT(7),
};

#define reg_soc_reg_cfg0 REG_ADDR8(SC_BASE_ADDR + 0x72)

enum
{
    FLD_REG_EMA  = BIT_RNG(0, 2),
    FLD_REG_EMAW = BIT_RNG(3, 4),
    FLD_REG_EMAS = BIT(5),
};

#define reg_soc_reg_cfg1 REG_ADDR8(SC_BASE_ADDR + 0x73)

enum
{
    FLD_REG_RAWL  = BIT(0),
    FLD_REG_RAWLM = BIT_RNG(1, 2),
    FLD_REG_WABL  = BIT(3),
    FLD_REG_WABLM = BIT_RNG(4, 5),
};

#define reg_wakeup_en REG_ADDR8(SC_BASE_ADDR + 0x2e)

enum
{
    FLD_USB_PWDN_I    = BIT(0),
    FLD_GPIO_WAKEUP_I = BIT(1),
    FLD_QDEC_RESUME   = BIT(2),
    FLD_USB_RESUME    = BIT(4),
    FLD_STANDBY_EX    = BIT(5),
};

/**
 * this register is to configure RF related reset.
*/
#define reg_n22_rst  REG_ADDR16(SC_BB_BASE_ADDR + 0x18)
#define reg_n22_rst0 REG_ADDR8(SC_BB_BASE_ADDR + 0x18)

enum
{
    FLD_RST0_ZB     = BIT(2), // Clears IRQ status and some internal states of the link layer.
    FLD_RST0_ZB_PON = BIT(6), // Restores all RF-related registers to their default value.
                              // RF related registers include: baseband, linklayer, modem, radio, bb_dma, bb_timer, pdzb.
    FLD_RST0_DMA_BB = BIT(7), // Resets the BB DMA status, and related registers will be cleared. After configuration, BB DMA needs to be reconfigured.
};

#define reg_n22_rst1 REG_ADDR8(SC_BB_BASE_ADDR + 0x19)

enum
{
    FLD_RST1_RSTL_BB     = BIT(2), // Clears RF state machine and some internal states of the link layer.
    FLD_RST1_RST_MDM     = BIT(3), // Clears all digital logic states related to mdm, the related configuration will not be lost after reset.
    FLD_RST1_RSTL_STIMER = BIT(4), // Resets BB STIMER status. RW registers remain, read-only registers will be cleared, and BB timer tick will be restarted.
};

#define reg_n22_clk_en  REG_ADDR16(SC_BB_BASE_ADDR + 0x1a)
#define reg_n22_clk_en0 REG_ADDR8(SC_BB_BASE_ADDR + 0x1a)

enum
{
    FLD_CLK0_ZB_HCLK_EN = BIT(2),
    FLD_CLK0_DMA_BB_EN  = BIT(7),
};

#define reg_n22_clk_en1 REG_ADDR8(SC_BB_BASE_ADDR + 0x1b)

enum
{
    FLD_CLK1_CLK_BB      = BIT(3),
    FLD_CLK1_CLKZB32K_LP = BIT(4),
};
