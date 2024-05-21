/********************************************************************************************************
 * @file    dma_reg.h
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
#ifndef DMA_REG_H
#define DMA_REG_H
#include "soc.h"
/*******************************    dma registers:  0x100400     ******************************/
#define REG_DMA_BASE                 0x100400


#define reg_dma_soft_reset       REG_ADDR8(0x100420)
enum{
	FLD_DMA_SOFT_RESET        = BIT(0),
};

#define reg_dma_err_isr					REG_ADDR8(0x100430)
#define reg_dma_abt_isr					REG_ADDR8(0x100431)
#define reg_dma_tc_isr					REG_ADDR8(0x100432)

enum{
	FLD_DMA_CHANNEL0_IRQ		= BIT(0),
	FLD_DMA_CHANNEL1_IRQ		= BIT(1),
	FLD_DMA_CHANNEL2_IRQ		= BIT(2),
	FLD_DMA_CHANNEL3_IRQ		= BIT(3),
	FLD_DMA_CHANNEL4_IRQ		= BIT(4),
	FLD_DMA_CHANNEL5_IRQ		= BIT(5),
	FLD_DMA_CHANNEL6_IRQ		= BIT(6),
	FLD_DMA_CHANNEL7_IRQ		= BIT(7),
};



#define reg_dma_car       REG_ADDR8(REG_DMA_BASE+0x40)
enum{
    FLD_DMA_ABT_CHN0     =  BIT(0),
	FLD_DMA_ABT_CHN1     =  BIT(1),
	FLD_DMA_ABT_CHN2     =  BIT(2),
	FLD_DMA_ABT_CHN3     =  BIT(3),
	FLD_DMA_ABT_CHN4     =  BIT(4),
	FLD_DMA_ABT_CHN5     =  BIT(5),
	FLD_DMA_ABT_CHN6     =  BIT(6),
	FLD_DMA_ABT_CHN7     =  BIT(7),
};


#define reg_dma_ctrl(i)				REG_ADDR32(( 0x00100444 +(i)*0x14))
enum{
	FLD_DMA_CHANNEL_ENABLE			= BIT(0),
	FLD_DMA_CHANNEL_TC_MASK			= BIT(1),
	FLD_DMA_CHANNEL_ERR_MASK		= BIT(2),
	FLD_DMA_CHANNEL_ABT_MASK		= BIT(3),
	FLD_DMA_CHANNEL_DST_REQ_SEL		= BIT_RNG(4,8),
	FLD_DMA_CHANNEL_SRC_REQ_SEL		= BIT_RNG(9,13),
	FLD_DMA_CHANNEL_DST_ADDR_CTRL	= BIT_RNG(14,15),
	FLD_DMA_CHANNEL_SRC_ADDR_CTRL	= BIT_RNG(16,17),
	FLD_DMA_CHANNEL_DST_MODE		= BIT(18),
	FLD_DMA_CHANNEL_SRC_MODE		= BIT(19),
	FLD_DMA_CHANNEL_DST_WIDTH		= BIT_RNG(20,21),
	FLD_DMA_CHANNEL_SRC_WIDTH		= BIT_RNG(22,23),
};

#define reg_dma_ctr0(i)			    REG_ADDR8(( 0x00100444 +(i)*0x14))

#define reg_dma_ch_cr3(i)          REG_ADDR8(( 0x00100447 +(i)*0x14))

#define reg_dma_ctr3(i)			    REG_ADDR8((0x00100447 +(i)*0x14))

enum{
	FLD_DMA_SRC_BURST_SIZE    		=	BIT_RNG(0,2),
	FLD_DMA_R_NUM_EN    	        =	BIT(4),
	FLD_DMA_PRIORITY    	        =	BIT(5),
	FLD_DMA_W_NUM_EN   	            =	BIT(6),
	FLD_DMA_AUTO_ENABLE_EN           =	BIT(7),
};



#define reg_dma_src_addr(i)			REG_ADDR32 (( 0x00100448 +(i)*0x14))
#define reg_dma_dst_addr(i)			REG_ADDR32 (( 0x0010044c +(i)*0x14))


/**
 * Due to the design constraints of the peripheral, the DMA must be transferred by word.
 * When the peripheral is used as receiver, FLD_DMA_TX_SIZE_IDX is invalid, the byte length of the DMA = FLD_DMA_TX_SIZE*4;
 * When the peripheral is used as transmitter, FLD_DMA_TX_SIZE_IDX is valid. If FLD_DMA_TX_SIZE_IDX is not equal to 0, the byte length of the DMA = (FLD_DMA_TX_SIZE-1)*4 + FLD_DMA_TX_SIZE_IDX;
 * If FLD_DMA_TX_SIZE_IDX is equal to 0, the byte length of DMA = FLD_DMA_TX_SIZE*4.
 */
#define reg_dma_size(i)			    REG_ADDR32 (( 0x00100450 +(i)*0x14))
enum{
	FLD_DMA_TX_SIZE    		=	BIT_RNG(0,21),
	FLD_DMA_TX_SIZE_IDX    	=	BIT_RNG(22,23),
};

#define reg_dma_cr3_size(i)			(*(volatile unsigned long*) ( 0x00100452 +(i)*0x14))

enum{
	FLD_DMA_TSR2_SIZE_IDX    	=	BIT_RNG(6,7),
};

#define reg_dma_llp(i)			    REG_ADDR32 (( 0x00100454 +(i)*0x14))


#define reg_dma_llp_int_mode(i)	REG_ADDR8(REG_DMA_BASE+0x113+(((i)>3) ? 1 : 0))



/**
 * The following registers are related to rf, refer to the definition in rf_reg.h
 * #define    CHNADDR         0x100400
 * #define reg_rf_bb_auto_ctrl				REG_ADDR8(0x10050c)
 * enum{
 *	FLD_RF_TX_MULTI_EN					=	BIT(0),
 *	FLD_RF_RX_MULTI_EN					=	BIT(1),
 *	FLD_RF_CH_0_RNUM_EN_BK				=	BIT(2),
 *  FLD_RF_CH_1_RNUM_EN_BK				=	BIT(3),
 *	FLD_RF_CH1_RX_ERR_EN				=	BIT(4),
 *	FLD_RF_DMA_REQ_D1_EN				=	BIT(5),
 * };
 * #define reg_rf_bb_tx_chn_dep				REG_ADDR8(0x1004f3)
 * #define reg_rf_bb_tx_size					REG_ADDR8(0x1004f0)
 * #define reg_rf_bb_tx_size_h					REG_ADDR8(0x1004f1)
 *
 * #define reg_rf_dma_rx_wptr				REG_ADDR8(0x001004f4)
 * #define reg_rf_dma_rx_rptr				REG_ADDR8(0x001004f5)
 *
 * #define reg_rf_dma_tx_rptr(i)			REG_ADDR8(0x00100501 + (i << 1))
 * #define reg_rf_dma_tx_wptr(i)			REG_ADDR8(0x00100500 + (i << 1))
 *
 * #define reg_rf_bb_rx_size					REG_ADDR8(CHNADDR+0xf6)
 * #define reg_rf_bb_rx_size_h					REG_ADDR8(CHNADDR+0xf7)
 *
 * #define reg_rf_rx_wptr_mask					REG_ADDR8(CHNADDR+0x10d)
 *
 */

#endif
