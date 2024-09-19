/********************************************************************************************************
 * @file    dma.h
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
/** @page DMA
 *
 *  Header File: dma.h
 */
#ifndef DMA_H_
#define DMA_H_
#include "compiler.h"
#include "reg_include/register.h"
/**
 * @brief Supports up to 8 DMA channels.
 */
typedef enum{
    DMA0=0,
    DMA1,
    DMA2,
    DMA3,
    DMA4,
    DMA5,
    DMA6,
    DMA7,
}dma_chn_e;

/**
 * @brief Define DMA burst size,Source burst size. It indicates the number of transfers before DMA channel re-arbitration.
 * @note  DMA_BURST_1_WORD by default, only the following peripherals support DMA_BURST_2_WORD or above: Audio/MSPI/LSPI/OSR.
 */
typedef enum{
    DMA_BURST_1_WORD = 0,
    DMA_BURST_2_WORD = 1,
    DMA_BURST_4_WORD = 2,
    DMA_BURST_8_WORD = 3,
}dma_burst_size_e;

/**
 * @brief  Define DMA chain interrupt mode.
 */
typedef enum{
    DMA_CONTINUE_MODE=0, /**< The transmission of DMA chain is continuous, interrupts are only generated when the last chain transmission is completed.*/
    DMA_INTERRUPT_MODE,  /**< The transmission of DMA chain is continuous, interrupts are generated at the completion of each chain transmission.*/
    DMA_TERMINAL_MODE,   /**< The transmission of DMA chain is automatically stopped at the completion of each chain transmission,
                            DMA should be triggered again for the next chain transfer and interrupts are generated at the completion of each chain transmission.*/
}dma_llp_irq_mode_e;

/**
 * @brief Define DMA Interrupt Request enumeration.
 */
typedef enum{
    DMA_CHN0_IRQ        = BIT(0),
    DMA_CHN1_IRQ        = BIT(1),
    DMA_CHN2_IRQ        = BIT(2),
    DMA_CHN3_IRQ        = BIT(3),
    DMA_CHN4_IRQ        = BIT(4),
    DMA_CHN5_IRQ        = BIT(5),
    DMA_CHN6_IRQ        = BIT(6),
    DMA_CHN7_IRQ        = BIT(7),
}dma_irq_chn_e;

/**
 * @brief Define Peripheral request (dst_req_sel and src_req_sel) enumeration. Peripheral request and DMA ack constitute hardware handshake(request/ack ) pairs
 */
typedef enum{
    DMA_REQ_LSPI_TX=0,
    DMA_REQ_LSPI_RX,

    DMA_REQ_UART0_TX,
    DMA_REQ_UART0_RX,

    DMA_REQ_GSPI_TX,
    DMA_REQ_GSPI_RX,

    DMA_REQ_I2C_TX,
    DMA_REQ_I2C_RX,

    DMA_REQ_UART2_TX,
    DMA_REQ_UART2_RX,

    DMA_REQ_PWM_TX,

    DMA_REQ_TRNG_RX,

    DMA_REQ_ALGM_TX,
    DMA_REQ_ALGM_RX,

    DMA_REQ_UART1_TX,
    DMA_REQ_UART1_RX,

    DMA_REQ_AUDIO0_TX,
    DMA_REQ_AUDIO0_RX,

    DMA_REQ_AUDIO1_TX,
    DMA_REQ_AUDIO1_RX,

    DMA_REG_MSPI_TX,
    DMA_REG_MSPI_RX,

    DMA_REQ_IRLEARN_RX,

    DMA_REQ_SAR_ADC_RX,

    DMA_REQ_AUDIO2_TX,
    DMA_REQ_AUDIO2_RX,

    DMA_REQ_TIMER0_RX,
    DMA_REQ_TIMER1_RX,

    DMA_REQ_CHACHA_TX,
    DMA_REQ_CHACHA_RX,

    DMA_REQ_STIMER_RX,

    DMA_REQ_RZ_TX,

    DMA_REQ_SKE_TX,
    DMA_REQ_SKE_RX,

    DMA_REQ_HASH_TX,
    DMA_REQ_HASH_RX,
}dma_req_sel_e;

/**
 * @brief  Define DMA address control enumeration.
 */
typedef enum{
    DMA_ADDR_INCREMENT=0, /**< Increment address,For example,if the transfer direction is: sram -> peripheral, the source address (sram) needs to be increased */
    DMA_ADDR_DECREMENT,   /**< Decrement address,Not usually used.*/
    DMA_ADDR_FIX,         /**< Fixed address,For example,if the transfer direction is: sram -> peripheral, the destination address (peripheral fifo) needs to be fixed.*/
}dma_addr_ctrl_e;

/**
 * @brief  Define DMA source or destination handshake mode.
 */
typedef enum{
    DMA_NORMAL_MODE=0, /**< The SRAM as either source or destination does not require a handshake mechanism because there is no empty-full situation. */
    DMA_HANDSHAKE_MODE,/**<
                           For the peripheral FIFO as the source, there is the case that the FIFO is empty when DMA fetching data, so the handshake mechanism is needed.
                           For the peripheral FIFO as the destination, there is the case that the FIFO is full when DMA writing data, so the handshake mechanism is needed.
                           */
}dma_mode_e;

/**
 * @brief Define DMA source or destination transfer width.
 * @note  <p> For peripheral-related DMA transfer, source width and destination width are only supported for DMA_CTR_WORD_WIDTH,so the corresponding SRAM(rx buff) length can only be a multiple of WORD_WIDTH.
 *        <p> If both the destination and source address are SRAM, all 3 width are supported.
 */
typedef enum{
    DMA_CTR_BYTE_WIDTH=0,/**< byte transfer.  */
    DMA_CTR_HWORD_WIDTH,/**<  half word transfer. */
    DMA_CTR_WORD_WIDTH, /**<  word transfer.  */
}dma_ctr_width_e;

/**
 * @brief Define the peripherals of DMA transfer width.
 * @note <p> DMA supports the following three types of transfer bytes, For Peripheral-related DMA transfer, transfer width is only supported for DMA_WORD_WIDTH.
 *       <p> If the destination address and source address of DMA are both SRAM, either one can be chosen.
 */
typedef enum{
    DMA_BYTE_WIDTH=1, /**< data is transmitted in bytes. */
    DMA_HWORD_WIDTH=2,/**< data is transmitted in half word. */
    DMA_WORD_WIDTH=4, /**< data is transmitted in word. */
}dma_transfer_width_e;

/**
 *  @brief  Define DMA IRQ MASK.
 */
typedef enum{
    TC_MASK   =     0,/**<
                               <pre>
                                Channel terminal count interrupt mask,Channel terminal count interrupt mask, the TC status is asserted when a channel transfer finishes.
                                The tc interrupt status bits need to be manually cleared,interfaces to get and clear tc interrupt:dma_get_tc_irq_status() / dma_clr_tc_irq_status()</pre>
                             */
    ERR_MASK,         /**<
                                <pre>
                                Channel error interrupt mask, the error status is asserted when channel transfer encounters the following error events: Bus error/Unaligned address/Unaligned transfer width/Reserved configuration.
                                the err interrupt status bits need to be manually cleared,interfaces to get and clear err interrupt:dma_get_err_irq_status() / dma_clr_err_irq_status() </pre>
                             */
    ABT_MASK,         /**<
                                <pre> Channel abort interrupt mask, the abort status is asserted when channel transfer is aborted.
                                 the abort interrupt status bits need to be manually cleared,interfaces to get and clear err interrupt:dma_get_abt_irq_status() / dma_clr_abt_irq_status() </pre>
                            */
}dma_irq_mask_e;

/**
 * @brief Define DMA control struct.
 * -# dst_req_sel: if dstmode is DMA_HANDSHAKE_MODE, dst_req_sel is set to the value defined in dma_req_sel_e(peripherals tx),otherwise dstmode is DMA_NORMAL_MODE and does not need to be set,just write 0.
 * -# src_req_sel: if srcmode is DMA_HANDSHAKE_MODE, src_req_sel is set to the value defined in dma_req_sel_e(peripherals rx),otherwise srcmode is DMA_NORMAL_MODE and does not need to be set,just write 0.
 * -# dst_addr_ctrl: see dma_addr_ctrl_e.
 * -# src_addr_ctrl: see dma_addr_ctrl_e.
 * -# dstmode: see dma_mode_e.
 * -# srcmode: see dma_mode_e.
 * -# dstwidth:see dma_ctr_width_e.
 * -# srcwidth:see dma_ctr_width_e.
 * -# src_burst_size:see dma_burst_size_e.
 * -# read_num_en:0 - need to manually configure reg_dma_size. 1 - The hardware automatically uses the value of the first four bytes of the source address as the transfer length instead of reg_dma_size(Not usually used).
 * -# priority:Generally it is set to 0, because setting to 1 will block the transmission of the other channels set to 0.
 * -# write_num_en:1- the hardware automatically writes the length of the received data to the SRAM(4 bytes) before the destination address. 0-Hardware will not write data length to SRAM
 * -# auto_en:only DMA0  rf tx is supported. After auto_enable_en is enabled, DMA0 is automatically enabled based on the baseband generation requests.
 */
typedef struct {
    unsigned int :2;
    unsigned int dst_req_sel:6;    /**< DstReqSel: 7:2 */
    unsigned int src_req_sel:6;    /**< SrcReqSel: 13:8 */
    unsigned int dst_addr_ctrl:2;  /**< DstAddrCtrl: 15:14 */
    unsigned int src_addr_ctrl:2;  /**< SrcAddrCtrl: 17:16 */
    unsigned int dstmode:1;        /**< DstMode: 18 */
    unsigned int srcmode:1;        /**< SrcMode: 19 */
    unsigned int dstwidth:2;       /**< DstWidth: 21:20 */
    unsigned int srcwidth:2;       /**< SrcWidth: 23:22 */
    unsigned int src_burst_size:3; /**< SrcBurstSize: 26:24 */
    unsigned int vacant_bit:1;     /**< vacant: 27 */
    unsigned int read_num_en:1;    /**< Rnum_en: 28 */
    unsigned int priority:1;       /**< Pri: 29 */
    unsigned int write_num_en:1;   /**< wnum_en: 30 */
    unsigned int auto_en:1;        /**< auto_en: 31 */
}dma_config_t;

/**
 * @brief Define DMA chain struct.
 * @note If volatile is not added, the compiler optimization will affect the configuration of the chain, which will lead to the abnormal work of pwm/audio(when using the chain function).
 */
typedef struct {
    volatile unsigned int dma_chain_ctl;
    volatile unsigned int dma_chain_src_addr;
    volatile unsigned int dma_chain_dst_addr;
    volatile unsigned int dma_chain_data_len;
    volatile unsigned int dma_chain_llp_ptr;
}dma_chain_config_t ;

/**
 * @brief      This function sets DMA chain transfer interrupt mode.
 * @param[in]  chn      - DMA channel
 * @param[in]  llp_mode - DMA chain interrupt mode
 * @return     none
 */
static inline void dma_set_llp_irq_mode(dma_chn_e chn,dma_llp_irq_mode_e llp_mode)
{
    reg_dma_llp_int_mode(chn) = (reg_dma_llp_int_mode(chn) & (~BIT_RNG((chn % 4) << 1, ((chn % 4) << 1) + 1))) | (llp_mode << ((chn % 4) << 1));
}

/**
 * @brief     This function configures DMA control register.
 * @param[in] chn    - dma channel
 * @param[in] config - the ptr dma_config that configured control register
 * @return    none
 * @note      When a certain DMA channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1),it is needed to disable DMA before writing to the DMA register.
 */
static _always_inline void dma_config(dma_chn_e chn ,dma_config_t *config)
{
    reg_dma_ctrl(chn) = (reg_dma_ctrl(chn)&(~BIT_RNG(2,31)))|(*(unsigned int*)config);
}

/**
 * @brief      This function servers to enable DMA that selected channel.
 * @param[in] chn - dma channel.
 * @return    none
 */
static inline void dma_chn_en(dma_chn_e chn)
{
    BM_SET(reg_dma_ctr0(chn),FLD_DMA_CHANNEL_ENABLE);
}

/**
 * @brief      This function servers to disable DMA that selected channel.
 * @param[in] chn - dma channel.
 * @return    none
 */
static inline void dma_chn_dis(dma_chn_e chn)
{
    BM_CLR(reg_dma_ctr0(chn),FLD_DMA_CHANNEL_ENABLE);
}

/**
 * @brief      This function servers to set DMA irq mask.
 * @param[in] chn  - dma channel.
 * @param[in] mask - dma irq mask.
 * @return    none
 */
static inline void dma_set_irq_mask(dma_chn_e chn,dma_irq_mask_e mask)
{
    switch (mask)
    {
        case TC_MASK:
             reg_dma_ctr0(chn) |= FLD_DMA_CHANNEL_TC_MASK;
             break;
        case ERR_MASK:
             reg_dma_err_mask |= BIT(chn);
             break;
        case ABT_MASK:
             reg_dma_abt_mask |= BIT(chn);
             break;
        default:
            break;
    }
}

/**
 * @brief      This function servers to clr DMA irq mask.
 * @param[in] chn  - dma channel.
 * @param[in] mask - dma irq mask.
 * @return    none
 */
static inline void dma_clr_irq_mask(dma_chn_e chn,dma_irq_mask_e mask)
{
    switch (mask)
    {
        case TC_MASK:
            reg_dma_ctr0(chn) &= (~FLD_DMA_CHANNEL_TC_MASK);
            break;
        case ERR_MASK:
             reg_dma_err_mask &= (~BIT(chn));
             break;
        case ABT_MASK:
             reg_dma_abt_mask &= (~BIT(chn));
             break;
        default:
            break;
    }
}

/**
 * @brief     This function servers to get the terminal count irq status channel.
 * @return   the dma terminal count irq status channel.
 */
static inline unsigned char dma_get_tc_irq_status(dma_irq_chn_e tc_chn)
{
    return reg_dma_tc_isr&tc_chn;
}

/**
 * @brief    This function servers to clear the irq of terminal count status.
 * @param[in] tc_chn -terminal count irq status channel.
 * @return    none
 */
static inline void dma_clr_tc_irq_status(dma_irq_chn_e tc_chn)
{
    reg_dma_tc_isr = tc_chn;
}

/**
 * @brief     This function servers to get the error irq status channel.
 * @return   the dma error irq status channel.
 */
static inline unsigned char dma_get_err_irq_status(dma_irq_chn_e err_chn)
{
    return reg_dma_err_isr&err_chn;
}

/**
 * @brief    This function servers to clear the abort status of channel.
 * @param[in] err_chn -error status channel.
 * @return    none
 */
static inline void dma_clr_err_irq_status(dma_irq_chn_e err_chn)
{
    reg_dma_err_isr = err_chn;
}

/**
 * @brief    This function servers to get the abort status of channel.
 * @return   the dma abort irq status channel.
 */
static inline unsigned char dma_get_abt_irq_status(dma_irq_chn_e abt_chn)
{
    return reg_dma_abt_isr&abt_chn;
}

/**
 * @brief    This function servers to clear the abort status of channel.
 * @param[in] abt_chn -abort irq status channel.
 * @return    none
 */
static inline void dma_clr_abt_irq_status(dma_irq_chn_e abt_chn)
{
    reg_dma_abt_isr = abt_chn;
}

/**
 * @brief   This function sets the DMA to tx/rx size byte.
 * @param[in] chn - DMA channel
 * @param[in] size_byte  - The address of dma tx/rx size .The maximum transmission length of DMA is 0xFFFFFC bytes and this length cannot be exceeded.
 *                         However,some peripherals can be set to a length less than 0xFFFFFC. For example, the maximum length of audio is 0x10000.
 * @param[in] byte_width -  dma   tx/rx  width
 * @return    none 
 * @note  <p>  If a channel is enabled with zero total transfer size, the error event will be triggered and the transfer will be terminated.
 *        <p> When a certain DMA channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1), it is needed to disable DMA before writing to the DMA register.
 */
static inline void dma_set_size(dma_chn_e chn,unsigned int size_byte,dma_transfer_width_e byte_width)
{
    reg_dma_size(chn) =((size_byte+byte_width-1)/byte_width)|( (size_byte % byte_width)<<22);
}

/**
 * @brief   this  function set  the DMA to tx/rx size byte.
 * @param[in] chn - DMA channel
 * @param[in] byte_width
 * @return    dma set size
 */
static inline unsigned int dma_get_size(dma_chn_e chn,dma_transfer_width_e byte_width)
{
    return ((((reg_dma_size1(chn)&0x3f)<<16)|reg_dma_size0(chn))-((reg_dma_size1(chn)>>6)==0 ? 0:1))*byte_width+(reg_dma_size1(chn)>>6);
}

/**
 * @brief   This  function calculates the DMA to tx/rx size byte.
 * @param[in] size_byte  - the address of dma   tx/rx size
 * @param[in] byte_width -  dma   tx/rx  width
 * @return    none
 */
static inline unsigned int dma_cal_size(unsigned int size_byte,dma_transfer_width_e byte_width)
{
     return (((size_byte+byte_width-1)/byte_width)|( (size_byte % byte_width)<<22));
}

/**
 * @brief   This function sets source and destination address for DMA.
   @verbatim
                   src_address          dst_address
           tx      sram                 interface fifo.
           rx      interface fifo       sram
   @endverbatim
 * @param[in]  chn      - DMA channel
 * @param[in]  src_addr - the address of source.
 * @param[in]  dst_addr - the address of destination.
 * @return    none
 * @note      <p> The src_addr and dst_addr must be aligned by word (4 bytes), otherwise the program will enter an exception.
 *            <p>  When a certain DMA channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1), it is needed  to disable DMA before writing to the DMA register.
 */
static inline void dma_set_address(dma_chn_e chn,unsigned int src_addr,unsigned int dst_addr)
{
    reg_dma_src_addr(chn)= (unsigned int)src_addr;
    reg_dma_dst_addr(chn)= (unsigned int)dst_addr;
}

/**
 * @brief   This function sets source address for DMA.
 * @param[in]  chn - DMA channel
 * @param[in]  src_addr - the address of source.
 * @return     none
 * @note      <p> The src_addr must be aligned by word (4 bytes), otherwise the program will enter an exception.
 *            <p>  When a certain DMA channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1),it is needed to disable DMA before writing to the DMA register.
 **/
static inline void dma_set_src_address(dma_chn_e chn,unsigned int src_addr)
{
    reg_dma_src_addr(chn)= (unsigned int)src_addr;
}

/**
 * @brief   This function sets destination address for DMA,
 * @param[in]  chn - DMA channel
 * @param[in]  dst_addr - the address of destination.
 * @return     none
 * @note       <p>  dst_addr must be aligned by word (4 bytes), otherwise the program will enter an exception
 *             <p>  When a certain dma channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1),need to disable dma before writing to the dma register.
 **/
static inline void dma_set_dst_address(dma_chn_e chn,unsigned int dst_addr)
{
    reg_dma_dst_addr(chn)= (unsigned int)dst_addr;
}

/**
 * @brief   This function resets DMA,the DMA logic and registers will be reset.
 * @return    none
 */
static inline void dma_reset(void)
{
    reg_rst1 &= ~(FLD_RST1_DMA);
    reg_rst1 |= FLD_RST1_DMA;
}

/**
 * @brief      This function servers to enable DMA wnum
 * @param[in]  chn - dma channel.
 * @return     none
 */
static inline void dma_set_wnum_en(dma_chn_e chn)
{
    reg_dma_ctr3(chn) |= FLD_DMA_W_NUM_EN;
}

/**
 * @brief      This function servers to disable DMA wnum.
 * @param[in]  chn - dma channel.
 * @return     none
 */
static inline void dma_set_wnum_dis(dma_chn_e chn)
{
    reg_dma_ctr3(chn) &= ~FLD_DMA_W_NUM_EN;
}

/**
 * @brief     This function sets DMA burst size.
 *            DMA does not support the burst function between 64 bytes before the address of 1k byte,and will switch back to the set burst mode after the 1k byte address
 * @param[in] chn        - dma channel
 * @param[in] burst_size - dma burst mode
 * @return    none
 */
static inline void dma_set_burst_size(dma_chn_e chn ,dma_burst_size_e burst_size)
{
    reg_dma_ctr3(chn) = (reg_dma_ctr3(chn)&(~FLD_DMA_SRC_BURST_SIZE))|burst_size;
}

#endif
