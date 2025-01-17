/********************************************************************************************************
 * @file    mspi.h
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

#include "compiler.h"
#include "gpio.h"
#include "reg_include/mspi_reg.h"
#include "error_handler/error_handler.h"

/**
 * @brief  Define the MSPI work mode.
 */
typedef enum
{
    MSPI_MODE0 = 0,
    MSPI_MODE2,
    MSPI_MODE1,
    MSPI_MODE3,
} mspi_mode_type_e;

/**
 * @brief  Define the mode for SPI io mode.
 */
typedef enum
{
    MSPI_SINGLE_MODE = 0,
    MSPI_DUAL_MODE   = 1,
    MSPI_QUAD_MODE   = 2,
    MSPI_3_LINE_MODE = 3
} mspi_io_mode_e;

typedef enum
{
    MSPI_NORMAL = 0,
    MSPI_3LINE  = 3,
} mspi_normal_3line_mode_e;

/**
 * @brief  Define the SPI command & translate mode.
 */
typedef enum
{
    MSPI_MODE_WRITE_AND_READ = 0, //write and read at the same.must enable CmdEn
    MSPI_MODE_WRITE_ONLY,         //write
    MSPI_MODE_READ_ONLY,          // read must enable CmdEn
    MSPI_MODE_WRITE_READ,         //write_ read
    MSPI_MODE_READ_WRITE,         //read_write
    MSPI_MODE_WRITE_DUMMY_READ,   //write_dummy_read
    MSPI_MODE_READ_DUMMY_WRITE,   //read_ dummy_write must enable CmdEn
    MSPI_MODE_NONE_DATA,          //must enable CmdEn
    MSPI_MODE_DUMMY_WRITE,        //dummy_write
    MSPI_MODE_DUMMY_READ,         //dummy_read
    MSPI_MODE_RESERVED,
} mspi_tans_mode_e;

typedef enum
{
    MSPI_MODE_WR_WRITE_ONLY  = 1, //write
    MSPI_MODE_WR_DUMMY_WRITE = 8, //dummy_write
} mspi_wr_tans_mode_e;

typedef enum
{
    MSPI_MODE_RD_READ_ONLY  = 2, //must enable CmdEn
    MSPI_MODE_RD_DUMMY_READ = 9, //dummy_read
} mspi_rd_tans_mode_e;

typedef enum
{
    MSPI_MODE_WR_RD       = 3, //must enable CmdEn
    MSPI_MODE_WR_DUMMY_RD = 5, //write_dummy_read
} mspi_wr_rd_tans_mode_e;

typedef enum
{
    MSPI_RC_24M_CLK = 0,
    MSPI_PLL_CLK    = 1,
} mspi_src_e;

typedef enum
{
    MSPI_WRITE = 0,
    MSPI_READ  = 1,
    MSPI_ERASE = 2,
} mspi_func_e;

/********************************************************************************************************
 *                                          internal
 *******************************************************************************************************/

/********************************************************************************************************
 *              this is only internal interface, customers do not need to care.
 *******************************************************************************************************/

/**
 * @brief      This function serves to judge whether mspi is busy.
 * @return     0:not busy   1:busy.
 */
static _attribute_ram_code_sec_optimize_o2_ inline bool mspi_busy(void)
{
    return reg_mspi_status & FLD_MSPI_BUSY;
}

/**
 * @brief       This function servers to set the spi wait.
 * @param[in]   none.
 * @return      none.
 */
#define mspi_wait() wait_condition_fails_or_timeout(mspi_busy, g_drv_api_error_timeout_us, drv_timeout_handler, (unsigned int)DRV_API_ERROR_TIMEOUT_MSPI_WAIT);

/********************************************************************************************************
 *                                          external
 *******************************************************************************************************/
/**
 * @brief       This function set hspi command content.
 * @param[in]   cmd         - command content.
 * @return      none
 */
_attribute_ram_code_sec_ static _always_inline void mspi_set_cmd(unsigned char cmd)
{
    reg_mspi_cmd = cmd;
}

/**
 * @brief       This function servers to enable cmd which will sent a byte cmd.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_cmd_en(void)
{
    BM_SET(reg_mspi_ctrl1, FLD_MSPI_CMD_EN);
}

/**
 * @brief       This function servers to disable cmd which will not sent cmd.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_cmd_dis(void)
{
    BM_CLR(reg_mspi_ctrl1, FLD_MSPI_CMD_EN);
}

/**
 * @brief       This function servers enable cmd  format,the format of cmd phase is the same as the data phase(Dual/Quad).
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_cmd_fmt_en(void)
{
    BM_SET(reg_mspi_ctrl1, FLD_MSPI_CMD_FMT);
}

/**
 * @brief       This function servers disable cmd  format.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_cmd_fmt_dis(void)
{
    BM_CLR(reg_mspi_ctrl1, FLD_MSPI_CMD_FMT);
}

/**
 * @brief       This function servers to enable mspi quad mode.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_quad_mode_en(void)
{
    BM_SET(reg_mspi_ctrl1, FLD_MSPI_DUAD);
}

/**
 * @brief       This function servers to disable mspi quad mode.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_quad_mode_dis(void)
{
    BM_CLR(reg_mspi_ctrl1, FLD_MSPI_DUAD);
}

/**
 * @brief       This function servers to enable mspi dual mode.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_dual_mode_en(void)
{
    BM_SET(reg_mspi_ctrl1, FLD_MSPI_DUAL);
}

/**
 * @brief       This function servers to disable mspi dual mode.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_dual_mode_dis(void)
{
    BM_CLR(reg_mspi_ctrl1, FLD_MSPI_DUAL);
}

/**
 * @brief       This function servers to enable mspi 3line mode.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_3line_mode_en(void)
{
    BM_SET(reg_mspi_ctrl3, FLD_MSPI_3LINE);
}

/**
 * @brief       This function servers to disable mspi 3line mode..
 * @param[in]   none.
 * @return      none.
 */
_attribute_ram_code_sec_ static inline void mspi_3line_mode_dis(void)
{
    BM_CLR(reg_mspi_ctrl3, FLD_MSPI_3LINE);
}

/**
 * @brief       This function to enable address format.the format of addr phase is the same as the data phase(Dual/Quad).
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_addr_fmt_en(void)
{
    BM_SET(reg_mspi_ctrl1, FLD_MSPI_ADDR_FMT);
}

/**
 * @brief       This function to disable address format.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_addr_fmt_dis(void)
{
    BM_CLR(reg_mspi_ctrl1, FLD_MSPI_ADDR_FMT);
}

/**
 * @brief       This function enable address phase.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_addr_en(void)
{
    BM_SET(reg_mspi_ctrl1, FLD_MSPI_ADDR_EN);
}

/**
 * @brief       This function disable address phase.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_addr_dis(void)
{
    BM_CLR(reg_mspi_ctrl1, FLD_MSPI_ADDR_EN);
}

/**
 * @brief       This function servers to set mspi address length.
 * @param[in]   len     - 2'b00:1bye  2'b01:2bytes  2'b10:3bytes  2'b11:4bytes
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_addr_len(unsigned char len)
{
    reg_mspi_ctrl1 |= ((len - 1) & 0x3) << 2;
}

/**
 * @brief       This function serves to set the minimum time between the edge of
 *              MSPI_CS and the edges of SPI_CLK.
 * @param[in]   time - the minimum time between the edge of MSPI_CS and the edges of SPI_CLK.
 *              the actual duration is (SPI_CLK period*(cs2sclk+1)).
 * @return      none.
 */
_attribute_ram_code_sec_ static inline void mspi_set_cs2clk_time(unsigned char time)
{
    reg_mspi_timing |= (time & FLD_MSPI_CS2SCLK);
}

/**
 * @brief       This function serves to set the minimum time that SPI CS should stay HIGH.
 * @param[in]   time - the minimum time that SPI CS should stay HIGH.
 *              the actual duration is (SPI_CLK period*(csht+1))
 * @return      none.
 */
_attribute_ram_code_sec_ static inline void mspi_set_cs_high_time(unsigned char time)
{
    reg_mspi_timing |= ((time << 3) & FLD_MSPI_CSHT);
}

/**
 * @brief       This function servers to set address.
 * @param[in]   addr - address.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_address(unsigned int addr)
{
    reg_mspi_addr = addr;
}

/**
 * @brief       This function set MSPI rx_cnt.
 * @param[in]   cnt         - rx amount of data.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_rx_cnt(unsigned int cnt)
{
    reg_mspi_rx_cnt = (cnt - 1) & 0xffffff;
}

/**
 * @brief       This function set SPI tx_cnt.
 * @param[in]   cnt         - tx amount of data.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_tx_cnt(unsigned int cnt)
{
    reg_mspi_tx_cnt = (cnt - 1) & 0xffffff;
}

/**
 * @brief       This function serve to set txfifo threshold.
 * @param[in]   thres   - the mspi txfifo threshold.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_txfifo_thres(unsigned char thres)
{
    reg_mspi_txfifo_thres = (thres & FLD_MSPI_TXFIFO_THRES);
}

/**
 * @brief       This function serve to set rxfifo threshold.
 * @param[in]   thres   - the mspi rxfifo threshold.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_rxfifo_thres(unsigned char thres)
{
    reg_mspi_rxfifo_thres = (thres & FLD_MSPI_RXFIFO_THRES);
}

/**
 * @brief       This function serve to enable xip.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static _always_inline void mspi_set_xip_en(void)
{
    reg_mspi_ctrl4 |= FLD_MSPI_XIP_ENABLE;
}

/**
 * @brief       This function serve to disable xip.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static _always_inline void mspi_set_xip_dis(void)
{
    reg_mspi_ctrl4 &= (~FLD_MSPI_XIP_ENABLE);
}

/**
 * @brief       This function serve to stop xip.
 * @param[in]   en  - 1: stop xip; 0: not stop xip
 * @return      none
 */
_attribute_ram_code_sec_ static _always_inline void mspi_set_xip_stop(unsigned char en)
{
    reg_mspi_ctrl4 |= ((en << 2) & FLD_MSPI_XIP_STOP);
}

/**
 * @brief       This function serve to stop xip and wait it complete.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_optimize_o2_ static inline void mspi_stop_xip(void)
{
    mspi_set_xip_stop(1);

    while (0 != (reg_mspi_status & 0x30))
        ;

    mspi_wait();
    mspi_set_xip_dis();
}

/**
 * @brief       This function serve to set xip page_mode.
 * para[in]     en - 1: enable xip page mode; 0: disable xip page mode.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_xip_page_mode(unsigned char en)
{
    reg_mspi_ctrl4 |= (en & FLD_MSPI_XIP_PAGE_MODE_EN);
}

/**
 * @brief       This function serve to set xip timeout_mode.
 * para[in]     en - 1: enable xip page mode; 0: disable xip page mode.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_xip_timeout_mode(unsigned char en)
{
    reg_mspi_ctrl4 |= ((en << 1) & FLD_MSPI_XIP_TIMEOUT_MODE_EN);
}

/**
 * @brief       This function serve to set xip page size.
 * para[in]     size - the xip page size.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_xip_page_size(unsigned char size)
{
    reg_mspi_page_size = size;
}

/**
 * @brief       This function serve to set xip timeout cnt.
 * para[in]     cnt - xip timeout cnt.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_xip_timeout_cnt(unsigned char cnt)
{
    reg_mspi_xip_timeout_cnt = cnt;
}

/**
 * @brief       This function serve to set xip write transfer mode.
 * para[in]     cnt - xip write transfer mode.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_xip_write_transmode(mspi_wr_tans_mode_e mode)
{
    reg_mspi_xip_trans_mod = (mode & FLD_MSPI_XIP_WR_TRANS_MODE);
}

/**
 * @brief       This function serve to set xip read transfer mode.
 * para[in]     cnt - xip write transfer mode.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_xip_read_transmode(mspi_rd_tans_mode_e mode)
{
    reg_mspi_xip_trans_mod = ((mode << 4) & FLD_MSPI_XIP_RD_TRANS_MODE);
}

/**
 * @brief       This function serve to set xip addr offset.
 * para[in]     offset - xip addr offset, address offset = xip_addr_offset << 24
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_xip_addr_offset(unsigned char offset)
{
    reg_mspi_xip_addr_offset = offset;
}

/**
 * @brief       This function serve to get txfifo entries.
 * @param[in]   none.
 * @return      txfifo entries.
 */
_attribute_ram_code_sec_ static inline unsigned char mspi_get_txfifo_entries(void)
{
    return (reg_mspi_txfifo_status & FLD_MSPI_TXFIFO_ENTRIES);
}

/**
 * @brief       This function serve to know weather txfifo is full.
 * @param[in]   none.
 * @return      1: txfifo is full.
 */
_attribute_ram_code_sec_ static inline unsigned char mspi_txfifo_is_full(void)
{
    return (reg_mspi_txfifo_status & FLD_MSPI_TXFIFO_FULL);
}

/**
 * @brief       This function serve to get rxfifo entries.
 * @param[in]   none.
 * @return      txfifo entries.
 */
_attribute_ram_code_sec_ static inline unsigned char mspi_get_rxfifo_entries(void)
{
    return (reg_mspi_rxfifo_status & FLD_MSPI_RXFIFO_ENTRIES);
}

/**
 * @brief       This function serve to know weather rxfifo is empty.
 * @param[in]   none.
 * @return      1: txfifo is full.
 */
_attribute_ram_code_sec_ static inline unsigned char mspi_txfifo_is_empty(void)
{
    return (reg_mspi_rxfifo_status & FLD_MSPI_RXFIFO_EMPTY);
}

/**
 * @brief    This function serves to reset mspi modules.
 * @param[in]   none.
 * @return   none
 */
_attribute_ram_code_sec_ static inline void mspi_soft_reset(void)
{
    reg_mspi_status = FLD_MSPI_SOFT_RESET;
    while (FLD_MSPI_SOFT_RESET == (reg_mspi_status & FLD_MSPI_SOFT_RESET))
        ;
}

/**
 * @brief       This function clear tx_fifo.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_tx_fifo_clr(void)
{
    BM_SET(reg_mspi_status, FLD_MSPI_TXFIFO_CLR_LEVEL);
    while (FLD_MSPI_TXFIFO_CLR_LEVEL == (reg_mspi_status & FLD_MSPI_TXFIFO_CLR_LEVEL))
        ;
}

/**
 * @brief       This function clear rx_fifo.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_rx_fifo_clr(void)
{
    BM_SET(reg_mspi_status, FLD_MSPI_RXFIFO_CLR_LEVEL);
    while (FLD_MSPI_RXFIFO_CLR_LEVEL == (reg_mspi_status & FLD_MSPI_RXFIFO_CLR_LEVEL))
        ;
}

/**
 * @brief       This function servers to set dummy cycle cnt.
 * @param[in]   dummy_cnt   - the cnt of dummy clock.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_dummy_cnt(unsigned char dummy_cnt)
{
    reg_mspi_ctrl2 = (reg_mspi_ctrl2 & (~FLD_MSPI_DUMMY)) | ((dummy_cnt - 1) & FLD_MSPI_DUMMY);
}

/**
 * @brief       This function servers to set spi transfer mode.
 * @param[in]   mode    - transfer mode.
 * @return      none
 */
_attribute_ram_code_sec_ static _always_inline void mspi_set_transmode(mspi_tans_mode_e mode)
{
    reg_mspi_ctrl2 = (reg_mspi_ctrl2 & (~FLD_MSPI_TRANSMODE)) | ((mode & 0x0f) << 4);
}

/**
 * @brief       This function serves to set mspi working mode.
 * @param[in]   mode - the selected working mode of MSPI module.
 *              bit2:CPHA-Clock Phase; bit3:CPOL:CPHA-Clock Polarity
 *              MODE0:  CPHA = 0, CPOL = 0;
 *              MODE1:  CPHA = 0, CPOL = 1;
 *              MODE2:  CPHA = 1, CPOL = 0;
 *              MODE3:  CPHA = 1, CPOL = 1;
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_work_mode(mspi_mode_type_e mode)
{
    reg_mspi_ctrl3 = (reg_mspi_ctrl3 & (~FLD_MSPI_WORK_MODE)) | ((mode << 2) & FLD_MSPI_WORK_MODE); // select MSPI mode, support four modes
}

/**
 * @brief       This function servers to set normal mode.
 * @param[in]   none.
 * @return      none.
 */
_attribute_ram_code_sec_ static inline void mspi_set_normal_mode(void)
{
    mspi_dual_mode_dis();
    mspi_3line_mode_dis();
    mspi_quad_mode_dis();
}

/**
 * @brief       This function servers to set dual mode.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_dual_mode(void)
{
    mspi_dual_mode_en(); //quad  precede over dual
    mspi_3line_mode_dis();
    mspi_quad_mode_dis();
}

/**
 * @brief   This function servers to set quad mode.
 * @param[in]   none.
 * @return  none
 */
_attribute_ram_code_sec_ static inline void mspi_set_quad_mode(void)
{
    mspi_quad_mode_en();
    mspi_dual_mode_dis();
    mspi_3line_mode_dis();
}

/**
 * @brief       This function servers to set 3line mode.
 * @param[in]   none.
 * @return      none
 */
_attribute_ram_code_sec_ static inline void mspi_set_3line_mode(void)
{
    /*must disable dual and quad*/
    mspi_3line_mode_en();
    mspi_dual_mode_dis();

    mspi_quad_mode_dis();
}

/**
 * @brief       This function servers to set mspi io  mode.
 * @param[in]   mode    - single/dual/quad /3line.
 * @return      none
  */
_attribute_ram_code_sec_ static inline void mspi_set_io_mode(mspi_io_mode_e mode)
{
    switch (mode) {
    case MSPI_SINGLE_MODE:
        mspi_set_normal_mode();
        break;
    case MSPI_DUAL_MODE:
        mspi_set_dual_mode();
        break;
    case MSPI_QUAD_MODE:
        mspi_set_quad_mode();
        break;
    case MSPI_3_LINE_MODE:
        mspi_set_3line_mode();
        break;
    }
}

/**
 * @brief       This function servers to write mspi fifo.
 * @param[in]   data    - the pointer to the data for write.
 * @param[in]   len     - write length.
 * @return      none
 */
_attribute_ram_code_sec_noinline_ void mspi_write(unsigned char *data, unsigned int len);

/**
 * @brief       This function servers to read mspi fifo.
 * @param[in]   data    - the pointer to the data for read.
 * @param[in]   len     - write length.
 * @return      none
 */
_attribute_ram_code_sec_noinline_ void mspi_read(unsigned char *data, unsigned int len);
