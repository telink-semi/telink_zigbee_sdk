/********************************************************************************************************
 * @file    rz.h
 *
 * @brief   This is the header file for TL321X
 *
 * @author  Driver Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef RZ_H_
#define RZ_H_
#include "compiler.h"
#include "reg_include/rz_reg.h"
#include "dma.h"
#include "lib/include/clock.h"

/**
 * @brief rz jitter mode on tl.
 */
typedef enum
{
    DISABLE_JITTER_ON_T0L_T1L = 0, /**< disable jitter on T0L&T1L */
    ENABLE_JITTER_ON_T0L_T1L = 1,  /**< enable jitter on T0L&T1L */
} rz_jitter_ctrl_tl_e;

/**
 * @brief rz jitter mode on th.
 */
typedef enum
{
    DISABLE_JITTER_ON_T0H_T1H = 0, /**< disable jitter on T0H&T1H */
    ENABLE_JITTER_ON_T0H_T1H = 1,  /**< enable jitter on T0H&T1H */
} rz_jitter_ctrl_th_e;

/**
 * @brief rz jitter range.
 */
typedef enum
{
    JITTER_RANGE_FROM_0_TO_1 = 0,
    JITTER_RANGE_FROM_0_TO_3,
    JITTER_RANGE_FROM_0_TO_7,
    JITTER_RANGE_FROM_0_TO_15,
    JITTER_RANGE_FROM_0_TO_31,
    JITTER_RANGE_FROM_0_TO_63,
    JITTER_RANGE_FROM_0_TO_127,
    JITTER_RANGE_FROM_0_TO_255,
} rz_jitter_range_e;

/**
 * @brief rz bit mode on output wire.
 */
typedef enum
{
    BIT_LSB = 0, /**< LSB of bit in byte first on rz output wire */
    BIT_MSB,     /**< MSB of bit in byte first on rz output wire */
} rz_bit_ctrl_e;

/**
 * @brief rz byte mode on output wire.
 */
typedef enum
{
    LITTLE_ENDIAN_MODE = 0, /**< LSB of byte in word first on rz output wire */
    BIG_ENDIAN_MODE,        /**< MSB of byte in word first on rz output wire */
} rz_byte_ctrl_e;

/**
 * @brief rz pixel/global data align mode.
 */
typedef enum
{
    EACH_PIXEL_DATA_AT_8BITS_BOUNDARY = 0, /**< each pixel/global data at 8bits boundary */
    EACH_PIXEL_DATA_AT_32BITS_BOUNDARY,    /**< each pixel/global data at 32bits boundary */
} rz_data_align_ctrl_e;

/**
 * @brief rz global data mode.
 */
typedef enum
{
    GLOBAL_DATA_AFTER_EACH_PIXEL_DATA = 0, /**< global data is after the pixel data of each pixel data */
    GLOBAL_DATA_AFTER_ALL_PIXEL_DATA,      /**< global data is after the pixel data of all pixel data */
} rz_global_data_ctrl_e;

/**
 * @brief rz tx data mode.
 */
typedef enum
{
    SERIAL_SEQUENTIAL_MODE = 0, /**< sequential addressing mode */
    PARALLEL_RANDOM_MODE,       /**< random addressing mode */
} rz_tx_data_mode_e;

/**
 * @brief rz inv mode.
 */
typedef enum
{
    POLARITY_NO_INV = 0, /**< T0H time period output is high level, T0L time period output is low level, T1H time period output is high level,T1L time period output is low level; */
    POLARITY_INV,        /**< T0H time period output is low level, T0L time period output is high level, T1H time period output is low level,T1L time period output is high level; */
} rz_output_inv_e;

/**
 * @brief rz protocol configuration structure.
 */
typedef struct
{
    rz_output_inv_e       pola;
    rz_tx_data_mode_e     addr_mode;
    rz_bit_ctrl_e         bit_msb;
    rz_byte_ctrl_e        big_endian_mode;
    rz_global_data_ctrl_e global_data_mode;
    rz_data_align_ctrl_e  align_32bits_mode;
    rz_jitter_ctrl_tl_e   jitter_l_en;
    rz_jitter_ctrl_th_e   jitter_h_en;
    rz_jitter_range_e     jitter_range;
    unsigned char         fifo_lvl;       /**< fifo trigger level, range 0~7 */
} rz_tx_config_t;

/**
 * @brief     This  function serves to set rz dma.
 * @param[in] rz_dma_chn - dma channel.
 * @param[in] src_addr   - the address of source.
 * @param[in] size_byte  - the length of data.
 * @return    none
 */
void rz_set_dma(dma_chn_e rz_dma_chn, unsigned int *src_addr, unsigned int size_byte);

/**
 * @brief     This function servers to configure DMA channel and some configures.
 * @param[in] chn - to select the DMA channel.
 * @return    none
 */
void rz_set_dma_config(dma_chn_e chn);

/**
 * @brief      This function servers to set rz wire output polarity.
 * @param[in]  pol - rz wire output polarity
 * @return     none.
 */
static inline void rz_set_polarity(rz_output_inv_e pol)
{
    reg_rz_ctrl1 = (reg_rz_ctrl1 & (~FLD_RZ_POLA)) | MASK_VAL(FLD_RZ_POLA, pol);
}

/**
 * @brief      This function servers to set bit in byte first on rz output wire.
 * @param[in]  bit_ctrl - bit in byte first on rz output wire
 * @return     none.
 */
static inline void rz_set_msb_lsb_mode(rz_bit_ctrl_e bit_ctrl)
{
    reg_rz_ctrl1 = (reg_rz_ctrl1 & (~FLD_RZ_BIT_MSB)) | MASK_VAL(FLD_RZ_BIT_MSB, bit_ctrl);
}

/**
 * @brief      This function servers to set byte in word first on rz output wire.
 * @param[in]  byte_ctrl - byte in word first on rz output wire
 * @return     none.
 */
static inline void rz_set_big_little_endian_mode(rz_byte_ctrl_e byte_ctrl)
{
    reg_rz_ctrl1 = (reg_rz_ctrl1 & (~FLD_RZ_BIG_ENDIAN_MODE)) | MASK_VAL(FLD_RZ_BIG_ENDIAN_MODE, byte_ctrl);
}

/**
 * @brief      This function servers to set each pixel/global data align mode.
 * @param[in]  data_align_ctrl - each pixel/global data align mode
 * @return     none.
 */
static inline void rz_set_data_align_mode(rz_data_align_ctrl_e data_align_ctrl)
{
    reg_rz_ctrl1 = (reg_rz_ctrl1 & (~FLD_RZ_ALIGN_32BITS_MODE)) | MASK_VAL(FLD_RZ_ALIGN_32BITS_MODE, data_align_ctrl);
}

/**
 * @brief      This function servers to set global data mode.
 * @param[in]  global_data_ctrl - global data mode
 * @return     none.
 */
static inline void rz_set_global_data_mode(rz_global_data_ctrl_e global_data_ctrl)
{
    reg_rz_ctrl1 = (reg_rz_ctrl1 & (~FLD_RZ_GLOBAL_DATA_MODE)) | MASK_VAL(FLD_RZ_GLOBAL_DATA_MODE, global_data_ctrl);
}

/**
 * @brief      This function refer to set rz tx data mode.
 * @param[in]  tx_data_mode - tx data mode
 * @return     none
 */
static inline void rz_set_tx_data_mode(rz_tx_data_mode_e tx_data_mode)
{
    reg_rz_ctrl1 = (reg_rz_ctrl1 & (~FLD_RZ_ADDR_MODE)) | MASK_VAL(FLD_RZ_ADDR_MODE, tx_data_mode);
}

/**
 * @brief      This function refer to set the high and low level time of 0 code.
 * @param[in]  target_high_tick - 0 code high level time, unit: us
 * @param[in]  target_low_tick  - 0 code low level time,  unit: us
 * @return     none
 * @note       The range of tick that can be set is approximately: 0~0x7ff, and the corresponding time is approximately: 0~85000ns.
 *             One tick corresponds to 1000/sys_clk.pclk ns
 *             and n ticks correspond to n*1000/sys_clk.pclk ns.
 */
static inline void rz_set_t0h_t0l_time(unsigned short target_high_tick, unsigned short target_low_tick)
{
    reg_rz_t0h = target_high_tick;
    reg_rz_t0l = target_low_tick;
}

/**
 * @brief      This function refer to set the high and low level time of 1 code.
 * @param[in]  target_high_tick - 1 code high level time, unit: us
 * @param[in]  target_low_tick  - 1 code low level time,  unit: us
 * @return     none
 * @note       The range of tick that can be set is approximately: 0~0x7ff, and the corresponding time is approximately: 0~85000ns.
 *             One tick corresponds to 1000/sys_clk.pclk ns
 *             and n ticks correspond to n*1000/sys_clk.pclk ns.
 */
static inline void rz_set_t1h_t1l_time(unsigned short target_high_tick, unsigned short target_low_tick)
{
    reg_rz_t1h = target_high_tick;
    reg_rz_t1l = target_low_tick;
}

/**
 * @brief      This function refer to set the high and low level time of reset code.
 * @param[in]  target_high_tick - rst code high level time, unit: us
 * @param[in]  target_low_tick  - rst code low level time,  unit: us
 * @return     none
 * @note       The range of tick that can be set is approximately: 0~0xffff, and the corresponding time is approximately: 0~7407us.
 *             One tick corresponds to 1000/sys_clk.pclk ns
 *             and n ticks correspond to n*1000/sys_clk.pclk ns.
 */
static inline void rz_set_tsrh_tsrl_time(unsigned short target_high_tick, unsigned short target_low_tick)
{
    reg_rz_tsrh = target_high_tick;
    reg_rz_tsrl = target_low_tick;
}

/**
 * @brief      This function refer to set the pixel ic number.
 * @param[in]  pixel_ic_num - pixel ic number. pixel_ic_num must be greater than 1
 * @return     none
 * @note       The actual number of pixel ics is reg_rz_pixel_num plus 1.
 */
static inline void rz_set_pixel_ic_num(unsigned char pixel_ic_num)
{
    reg_rz_pixel_num = (pixel_ic_num - 1);
}

/**
 * @brief      This function refer to set the global data number.
 * @param[in]  global_data - global data number
 * @return     none
 */
static inline void rz_set_global_data_num(unsigned char global_data)
{
    reg_rz_global_data_num = global_data;
}

/**
 * @brief      This function refer to set the number of bits per pixel ic.
 * @param[in]  per_pixel_data - the number of bits per pixel ic
 * @return     none
 * @note       The actual number of pixel ics is reg_rz_data_bit_num_per_pixel plus 1.
 */
static inline void rz_data_bit_num_per_pixel(unsigned char per_pixel_data)
{
    reg_rz_data_bit_num_per_pixel = (per_pixel_data - 1);
}

/**
 * @brief      This function refer to set the rz mask.
 * @param[in]  mask - the irq mask.
 * @return     none
 */
static inline void rz_set_irq_mask(rz_mask_e mask)
{
    reg_rz_mask |= mask;
}

/**
 * @brief      This function refer to clear the rz mask.
 * @param[in]  mask - the irq mask.
 * @return     none
 */
static inline void rz_clr_irq_mask(rz_mask_e mask)
{
    reg_rz_mask &= (~mask);
}

/**
 * @brief      This function refer to get the rz mask.
 * @return     irq mask value.
 */
static inline unsigned char rz_get_irq_mask(void)
{
    return reg_rz_mask & 0x0f;
}

/**
 * @brief      This function servers to enable rz module.
 * @return     none
 */
static inline void rz_en(void)
{
    reg_rz_fifo_sts |= FLD_RZ_EN;
}

/**
 * @brief      This function servers to disable rz module.
 * @return     none
 */
static inline void rz_dis(void)
{
    reg_rz_fifo_sts &= ~FLD_RZ_EN;
}

/**
 * @brief      This function servers to get rz irq status.
 * @param[in]  status - the irq status.
 * @return     irq status.
 */
static inline unsigned char rz_get_irq_status(rz_int_e status)
{
    return (reg_rz_int & status);
}

/**
 * @brief      This function servers to clear rz irq status.
 * @param[in]  status - the irq status             .
 * @return     none                                  .
 */
static inline void rz_clr_irq_status(rz_int_e status)
{
    reg_rz_int = (status);
}

/**
 * @brief  This function servers to get tx fifo cnt.
 * @return the count of txfifo.
 */
static inline unsigned char rz_get_txfifo_cnt(void)
{
    return (reg_rz_fifo_sts & FLD_RZ_TX_BUF_CNT);
}

/**
 * @brief  This function servers to clear tx fifo cnt manually.
 * @return none.
 */
static inline void rz_clr_txfifo_manual(void)
{
    reg_rz_ctrl0 |= FLD_RZ_TX_CLR;
}

/**
 * @brief  This function servers to clear tx fifo cnt automatically.
 * @return none.
 */
static inline void rz_clr_txfifo_auto(void)
{
    reg_rz_ctrl2 |= FLD_RZ_AUTO_TXCLR_EN;
}

/**
 * @brief     This function servers to config jitter range on t0H/t1H/t0L/t1L.
 * @param[in] jitter_range - jitter range.
 * @return    none.
 */
static inline void rz_jitter_range_config(rz_jitter_range_e jitter_range)
{
    reg_rz_ctrl2 = (reg_rz_ctrl2 & (~FLD_RZ_JITTER_RANGE)) | MASK_VAL(FLD_RZ_JITTER_RANGE, jitter_range);
}

/**
 * @brief      This function refer to set the fifo lvl.
 * @param[in]  fifo_lvl - set the fifo depth size.the range is less than 8.
 * @return     none
 */
static inline void rz_set_fifo_lvl(unsigned char fifo_lvl)
{
    reg_rz_ctrl2 = (reg_rz_ctrl2 & (~FLD_RZ_FIFO_LVL)) | MASK_VAL(FLD_RZ_FIFO_LVL, fifo_lvl);
}

/**
 * @brief     This function servers to enable/disable jitter on t0H/t1H.
 * @param[in] jitter_ctrl_th - jitter mode.
 * @return    none.
 */
void rz_t0h_t1h_jitter_config(rz_jitter_ctrl_th_e jitter_ctrl_th);

/**
 * @brief     This function servers to enable/disable jitter on t0L/t1L.
 * @param[in] jitter_ctrl_tl - jitter mode.
 * @return    none.
 */
void rz_t0l_t1l_jitter_config(rz_jitter_ctrl_tl_e jitter_ctrl_tl);

/**
 * @brief      This function configures rz control register.
 * @param[in]  rz_tx_config - the prt of rz config that configured control register
 * @return     none
 */
void rz_set_tx_config(rz_tx_config_t *rz_tx_config);

#endif /* RZ_H_ */
