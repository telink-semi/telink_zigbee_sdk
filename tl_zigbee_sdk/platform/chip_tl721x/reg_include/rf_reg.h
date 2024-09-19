/********************************************************************************************************
 * @file    rf_reg.h
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
#ifndef  RF_REG_H
#define  RF_REG_H
#include "soc.h"

///*******************************      RF      ******************************/
/*******************************      baseband clock registers: 0x170a00       ******************************/
#define BB_TIMER_BASE_ADDR              0x170a00
#define reg_bb_timer_tick               REG_ADDR32(BB_TIMER_BASE_ADDR)
#define reg_bb_timer_irq_level          REG_ADDR32(BB_TIMER_BASE_ADDR+0x4)

#define reg_bb_timer_irq_mask               REG_ADDR8(BB_TIMER_BASE_ADDR+0x8)
enum{
    FLD_BB_TIMER_IRQ_MASK   =   BIT_RNG(0,1),
    FLD_BB_TIMER_IRQ_WAIT   =   BIT(2),
    FLD_BB_TIMER_TRIG_PAST_EN   =   BIT(3),
};

#define reg_bb_timer_cal_irq        REG_ADDR8(BB_TIMER_BASE_ADDR+0x9)

typedef enum{
    FLD_BB_TIMER_IRQ_CLR_BB_TIMER       =   BIT(0),//clear IRQ systimer level irq  0x09[0]
    FLD_BB_TIMER_IRQ_CLR_CAL            =   BIT(1),//calibration latch result update irq (clear 32k cal irq 09[1] )
}rf_bb_timer_irq_e;

#define reg_bb_timer_ctrl           REG_ADDR8(BB_TIMER_BASE_ADDR+0xa)
enum{
    FLD_BB_TIMER_32K_WR_EN      =   BIT(0),
    FLD_BB_TIMER_TIMER_EN       =   BIT(1),
    FLD_BB_TIMER_TIMER_AUTO     =   BIT(2),
    FLD_BB_TIMER_32K_TRACK_EN   =   BIT(3),
    FLD_BB_TIMER_32K_CAL_MODE   =   BIT_RNG(4,7),

};

#define reg_bb_timer_st         REG_ADDR8(BB_TIMER_BASE_ADDR+0xb)

enum{

    FLD_BB_TIMER_CMD_STOP               =   BIT(1),
    FLD_BB_TIMER_CMD_SYNC               =   BIT(3),
    FLD_BB_TIMER_CLK_32K                =   BIT(4),
    FLD_BB_TIMER_CLR_RD_DONE            =   BIT(5),
    FLD_BB_TIMER_RD_BUSY                =   BIT(6),
    FLD_BB_TIMER_CMD_SET_DLY_DONE       =   BIT(7),

};

#define reg_bb_timer_ctrl1          REG_ADDR8(BB_TIMER_BASE_ADDR+0x19)
enum{

    FLD_BB_TIMER_PEM_EVENT_EN           =   BIT(3),
    FLD_BB_TIMER_PEM_TASK_0_EN          =   BIT(4),
    FLD_BB_TIMER_PEM_TASK_1_EN          =   BIT(5),
    FLD_BB_TIMER_R_TMR_MAN_SEL          =   BIT(7),

};



///*******************************      baseband dma registers: 0x170800      ******************************/
#define BB_DMA_BASE_ADDR                   0x170800


#define reg_bb_dma_ctr0(i)              REG_ADDR8(( BB_DMA_BASE_ADDR+0x44 +(i)*0x14))
#define reg_bb_dma_ctrl(i)              REG_ADDR32(BB_DMA_BASE_ADDR+0x44 +(i)*0x14)
enum{
    FLD_BB_DMA_CHANNEL_ENABLE           = BIT(0),
    FLD_BB_DMA_CHANNEL_TC_MASK          = BIT(1),
    FLD_BB_DMA_CHANNEL_ERR_MASK     = BIT(2),
    FLD_BB_DMA_CHANNEL_ABT_MASK     = BIT(3),
    FLD_BB_DMA_CHANNEL_DST_REQ_SEL      = BIT_RNG(4,8),
    FLD_BB_DMA_CHANNEL_SRC_REQ_SEL      = BIT_RNG(9,13),
    FLD_BB_DMA_CHANNEL_DST_ADDR_CTRL    = BIT_RNG(14,15),
    FLD_BB_DMA_CHANNEL_SRC_ADDR_CTRL    = BIT_RNG(16,17),
    FLD_BB_DMA_CHANNEL_DST_MODE     = BIT(18),
    FLD_BB_DMA_CHANNEL_SRC_MODE     = BIT(19),
    FLD_BB_DMA_CHANNEL_DST_WIDTH        = BIT_RNG(20,21),
    FLD_BB_DMA_CHANNEL_SRC_WIDTH        = BIT_RNG(22,23),
};

#define reg_bb_dma_ctr3(i)              REG_ADDR8(BB_DMA_BASE_ADDR+0x47+(i)*0x14)
enum{
    FLD_BB_DMA_SRC_BURST_SIZE           =   BIT_RNG(0,2),
    FLD_BB_DMA_R_NUM_EN             =   BIT(4),
    FLD_BB_DMA_PRIORITY             =   BIT(5),
    FLD_BB_DMA_W_NUM_EN                 =   BIT(6),
    FLD_BB_DMA_AUTO_ENABLE_EN           =   BIT(7),
};

#define reg_bb_dma_src_addr(i)          REG_ADDR32(BB_DMA_BASE_ADDR+0x48+(i)*0x14)
#define reg_bb_dma_dst_addr(i)          REG_ADDR32(BB_DMA_BASE_ADDR+0x4c+(i)*0x14)

#define reg_bb_dma_size0(i)         REG_ADDR16(BB_DMA_BASE_ADDR+0x50+(i)*0x14)
enum{
    FLD_BB_DMA_TANS_SIZE0               = BIT_RNG(0,15),
};
#define reg_bb_dma_size1(i)         REG_ADDR8(BB_DMA_BASE_ADDR+0x52+(i)*0x14)
enum{
    FLD_BB_DMA_TANS_SIZE1               = BIT_RNG(0,5),
    FLD_BB_DMA_TANS_IDX             = BIT_RNG(6,7),
};

#define reg_bb_dma_size(i)              REG_ADDR32 (BB_DMA_BASE_ADDR+0x50+(i)*0x14)

enum{
    FLD_BB_DMA_TX_SIZE          =   BIT_RNG(0,21),
    FLD_BB_DMA_TX_SIZE_IDX      =   BIT_RNG(22,23),
};

#define reg_rf_bb_auto_ctrl             REG_ADDR8(0x17090c)
enum{
    FLD_RF_TX_MULTI_EN                  =   BIT(0),
    FLD_RF_RX_MULTI_EN                  =   BIT(1),
    FLD_RF_CH_0_RNUM_EN_BK              =   BIT(2),
    FLD_RF_CH_1_RNUM_EN_BK              =   BIT(3),
    FLD_RF_CH1_RX_ERR_EN                =   BIT(4),
    FLD_RF_DMA_REQ_D1_EN                =   BIT(5),
};

#define reg_rf_bb_tx_chn_dep                REG_ADDR8(0x1708f3)
#define reg_rf_bb_tx_size                   REG_ADDR8(0x1708f0)
#define reg_rf_bb_tx_size_h                 REG_ADDR8(0x1708f1)

#define reg_rf_dma_rx_wptr              REG_ADDR8(0x1708f4)
#define reg_rf_dma_rx_rptr              REG_ADDR8(0x1708f5)

#define reg_rf_dma_tx_rptr(i)           REG_ADDR8(0x170901 + (i << 1))
enum{
    FLD_BB_DMA_RPTR_MASK =          BIT_RNG(0,4),
    FLD_BB_DMA_RPTR_SET =           BIT(5),
    FLD_BB_DMA_RPTR_NEXT =          BIT(6),
    FLD_BB_DMA_RPTR_CLR =           BIT(7),
};

#define reg_rf_dma_tx_wptr(i)           REG_ADDR8(0x170900 + (i << 1))
enum{
    FLD_BB_DMA_WPTR_MASK =         BIT_RNG(0,4),
};

#define reg_rf_bb_rx_size                   REG_ADDR8(BB_DMA_BASE_ADDR+0xf6)
#define reg_rf_bb_rx_size_h                 REG_ADDR8(BB_DMA_BASE_ADDR+0xf7)


#define reg_rf_rx_wptr_mask                 REG_ADDR8(BB_DMA_BASE_ADDR+0x10d)


//******************************      RF_Register_list      ******************************/
#define    REG_BASEBAND_BASE_ADDR           0x170000
#define    REG_TL_RADIO_BASE_ADDR           0X170600

#define    reg_rf_tx_mode1              REG_ADDR8(REG_BASEBAND_BASE_ADDR)
enum{
    FLD_RF_DMA_EN               =   BIT(0),
    FLD_RF_CRC_EN               =   BIT(1),
    FLD_RF_TX_MODE              =   BIT_RNG(2,4),
    FLD_RF_TX_TEST_DATA         =   BIT(5),
    FLD_RF_TX_TEST_EN           =   BIT(6),
    FLD_RF_BB_DEC               =   BIT(7),
};


#define    reg_rf_tx_mode2              REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x01)
enum{
    FLD_RF_ZB_PN_EN             =   BIT(0),
    FLD_RF_TLK_CRYPT_ENABLE     =   BIT(1),
    FLD_RF_TLK_MST_SLV          =   BIT(2),
    FLD_RF_V_PN_EN              =   BIT(3),
    FLD_RF_R_CUSTOM_MADE        =   BIT(4),
    FLD_RF_V_INTLV_EN           =   BIT(5),
    FLD_RF_TX_OUT_PLR           =   BIT(6),
    FLD_RF_TX_EN_TGL            =   BIT(6),
};


#define    reg_rf_preamble_trail        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x02)
enum{
    FLD_RF_PREAMBLE_LEN         =   BIT_RNG(0,4),
    FLD_RF_TRAILER_LEN          =   BIT_RNG(5,7),
};


#define    reg_rf_bbdbg                 REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x03)
enum{
    FLD_RF_SLOW_EN              =   BIT(0),
    FLD_RF_SLOW_SEL             =   BIT(1),
    FLD_RF_XMODE_EN             =   BIT(2),
    FLD_RF_REV_ORDER            =   BIT(3),
    FLD_RF_TX_XOR_MODE          =   BIT(4),
    FLD_RF_R_DBG_SEL0           =   BIT(5),
    FLD_RF_MODEM1M_PAT          =   BIT(6),
    FLD_RF_R_DBG_SEL1           =   BIT(7),
};


#define    reg_rf_format                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x04)
enum{
    FLD_RF_HEAD_MODE            =   BIT_RNG(0,2),
    FLD_RF_BLE_1M               =   BIT(4),
    FLD_RF_BLE_WT               =   BIT(5),
    FLD_RF_TX_NOACK             =   BIT(6),
    FLD_RF_PN_AUTO              =   BIT(7),
};


#define  reg_rf_acclen                  REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x05)
enum{
    FLD_RF_ACC_LEN              =   BIT_RNG(0,2),
    FLD_LOW_RATE_EN             =   BIT(3),
    FLD_RF_LR_TX_SEL            =   BIT(4),
    FLD_RF_BLE_LR               =   BIT(5),
    FLD_RF_R_DBG_SEL2           =   BIT(6),
};



#define    reg_rf_sblen             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x06)


#define    reg_rf_rxchn             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x07)
enum{
    FLD_RF_TX_ANT_SWITCH        =   BIT_RNG(0,1),
    FLD_RF_RX_ANT_SWITCH        =   BIT(2),
    FLD_RF_TEST_TXON            =   BIT(6),
    FLD_RF_TEST_TXON_EN         =   BIT(7),
};

#define    reg_rf_access_code       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0x08)
#define    reg_rf_access_0          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x08)
#define    reg_rf_access_1          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x09)
#define    reg_rf_access_2          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x0a)
#define    reg_rf_access_3          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x0b)
#define    reg_rf_access_4          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x0c)
#define    reg_rf_access_5          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x0d)
#define    reg_rf_access_6          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x0e)
#define    reg_rf_access_7          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x0f)
#define    reg_rf_access_8          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x10)
#define    reg_rf_access_9          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x11)
#define    reg_rf_access_10         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x12)
#define    reg_rf_access_11         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x13)
#define    reg_rf_access_12         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x14)
#define    reg_rf_access_13         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x15)
#define    reg_rf_access_14         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x16)
#define    reg_rf_access_15         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x17)
#define    reg_rf_access_16         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x18)
#define    reg_rf_access_17         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x19)
#define    reg_rf_access_18         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x1a)
#define    reg_rf_access_19         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x1b)
#define    reg_rf_access_20         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x1c)
#define    reg_rf_access_21         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x1d)



#define    reg_rf_access_code_base_pipe0   0x170008

#define     reg_rf_tx_hlen_mode     REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x1e)
enum{
    FLD_RF_TX_HLEN_MODE         =   BIT_RNG(0,1),
    FLD_RF_TX_HLEN_WIDTH        =   BIT(2),
    FLD_RF_RD_EMPTY_MUX         =   BIT(3),
    FLD_RF_TX_VLD_EN            =   BIT(4),
};

#define    reg_rf_txfifo            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x1f)
enum{
    FLD_RF_TX_FIFO_FULL         =   BIT(0),
    FLD_RF_TX_ACT_2D            =   BIT(1),
};

#define    reg_rf_pn                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x20)
enum{
    FLD_RF_CH_IDX               =   BIT_RNG(0,5),
};

#define    reg_rf_rxgth1            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x21)
enum{
    FLD_RF_R_PILOT_LEN_O        =   BIT_RNG(0,3),
    FLD_RF_R_ZB_SFD_O           =   BIT(4),
    FLD_RF_R_PKTLEN_O           =   BIT(5),
    FLD_RF_R_SN_LEN_O           =   BIT(6),
    FLD_RF_R_LEN_0_EN_O         =   BIT(7),
};


#define    reg_rf_rxsfd0_num         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x22)
enum{
    FLD_RF_RXCHN_MAN_EN         =   BIT(3),
    FLD_RF_RXCHN_MAN            =   BIT_RNG(4,6),
};

#define     reg_rf_ant_num      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x23)
enum{
    FLD_RF_ANT_NUM          =   BIT_RNG(0,5),
};

#define     reg_rf_ant_sel_man      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x24)
enum{
    FLD_RF_ANT_SEL_MAN          =   BIT_RNG(0,5),
};

#define     reg_rf_aux_hl_adv       REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x25)
enum{
    FLD_RF_AUX_HL_ADV           =   BIT_RNG(0,3),
};

#define    reg_rf_ctrl_0             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x27)
enum{
    FLD_RF_H1M_ODD              =   BIT(0),
    FLD_RF_REV_ZB_SAMP          =   BIT(1),
    FLD_RF_SFD_LAST_CHIP        =   BIT(2),
};


#define    reg_rf_rxmode             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x28)
enum{
    FLD_RF_RX_ENABLE            =   BIT(0),
};


#define    reg_rf_rxclk_on           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x2a)
enum{
    FLD_RF_CLKON_O              =   BIT_RNG(0,1),
};


#define    reg_rf_rxclk_auto         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x2b)
enum{
    FLD_RF_CLK_AUTO             =   BIT_RNG(0,1),
};


#define    reg_rf_rxdma_hs           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x2c)
enum{
    FLD_RF_RXDMA_HS             =   BIT(0),
};

#define    reg_rf_latch_agc_gain             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x2d)
enum{
    FLD_RF_LAT_CNT              =   BIT_RNG(0,3),
    FLD_RF_LAT_AT_END           =   BIT(4),
};

#define     reg_rf_iq_samp_num0     REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x2e)
#define     reg_rf_iq_samp_num1     REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x2f)

#define    reg_rf_rxtcrcpkt          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x30)
enum{
    FLD_RF_FILTER_CRC_PKT       =   BIT(0),
    FLD_RF_EN_TS_RX             =   BIT(2),
    FLD_RF_EN_TS_TX             =   BIT(3),
    FLD_RF_LEN_SET_O            =   BIT_RNG(4,5),
};


#define    reg_rf_rxtmaxlen0             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x31)
#define    reg_rf_rxtmaxlen1             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x32)
enum
{
    FLD_RF_MAX_LEN              =   BIT_RNG(0,6),
    FLD_RF_RX_ISO_PDU           =   BIT(7),
};


#define    reg_rf_rxlatf             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x33)
enum{
    FLD_RF_R_LATF_CNT_O         =   BIT_RNG(0,3),
    FLD_RF_R_LATF_AT_END_O      =   BIT(4),
    FLD_RF_R_IQ_SAMP_MODE       =   BIT(5),

};


#define    reg_rf_burst_size             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x34)
enum{
    FLD_RF_DMA_BURST_SIZE           =   BIT_RNG(0,1),
    FLD_RF_RX_HLEN_MODE             =   BIT_RNG(2,3),
    FLD_RF_HLEN_WIDTH               =   BIT(4),//FLD_RF_SEL_MAN                 =   BIT_RNG(4,7),
};

#define     reg_rf_ant_interval      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x35)

#define     reg_rf_ant_msb           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x36)
enum{
    FLD_RF_ANT_INTERVAL_MSB             =   BIT(0),
    FLD_RF_TX_ANT_OFFSET_MSB            =   BIT(1),
    FLD_RF_RX_ANT_OFFSET_MSB            =   BIT(2),
};


#define    reg_rf_rxsupp             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x38)
enum{
    FLD_RF_RX_SUPP_D_EN         =   BIT(0),
    FLD_RF_RX_SUPP_A_EN         =   BIT(1),
    FLD_RF_TX_SUPP_D_EN         =   BIT(2),
    FLD_RF_TX_SUPP_A_EN         =   BIT(3),
};


#define    reg_rf_tx_antoffset       REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x39)
#define    reg_rf_rx_antoffset       REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x3a)
#define    reg_rf_samp_offset        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x3b)


#define    reg_rf_man_ant_slot       REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x3c)
enum{
//  FLD_RF_ANT_SEL_MAN          =   BIT_RNG(0,2),
    FLD_RF_ANT_SEL_MAN_EN       =   BIT(3),
    FLD_RF_SLOT_1US_MAN_EN      =   BIT(4),
    FLD_RF_SLOT_1US_MAN         =   BIT(5),
    FLD_RF_ANT_PAT              =   BIT_RNG(6,7),
};


#define    reg_rf_sof_offset         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x3d)
enum{
    FLD_RF_ANT_SOF_OFFSET       =   BIT_RNG(0,1),
    FLD_RF_SAMP_SOF_OFFSET      =   BIT_RNG(2,3),
    FLD_RF_SUPP_MODE            =   BIT_RNG(4,6),
};


#define    reg_rf_mode_ctrl0         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x3e)
enum{
    FLD_RF_INTV_MODE            =   BIT_RNG(0,1),
    FLD_RF_IQ_SAMP_EN           =   BIT(2),
    FLD_RF_INFO_EXTENSION       =   BIT(3),
    FLD_RF_IQ_SAMP_INTERVAL     =   BIT_RNG(4,7),
};


#define    reg_rf_iq_samp_start      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x3f)


#define    reg_rf_dec_err            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x40)
enum{
    FLD_RF_SS                   =   BIT_RNG(0,2),
    FLD_RF_TLK_MIC_ERR          =   BIT(3),
    FLD_RF_PKT_DEC_ERR          =   BIT_RNG(4,7),
};

#define    reg_rf_timestamp         REG_ADDR32(REG_BASEBAND_BASE_ADDR+0x50)
#define    reg_rf_tstamp0           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x50)
enum{
    FLD_RF_R_TSTAMP0             =  BIT_RNG(0,7),
};


#define    reg_rf_tstamp1            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x51)
enum{
    FLD_RF_R_TSTAMP1             =  BIT_RNG(0,7),
};


#define    reg_rf_tstamp2            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x52)
enum{
    FLD_RF_R_TSTAMP2             =  BIT_RNG(0,7),
};


#define    reg_rf_tstamp3            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x53)
enum{
    FLD_RF_R_TSTAMP3             =  BIT_RNG(0,7),
};
#define    reg_rf_iqstart_tstamp             REG_ADDR32(REG_BASEBAND_BASE_ADDR+0x54)
#define    reg_rf_iqstart_tstamp0            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x54)
enum{
    FLD_RF_IQSTART_TSTAMP0            = BIT_RNG(0,7),
};

#define    reg_rf_iqstart_tstamp1            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x55)
enum{
    FLD_RF_IQSTART_TSTAMP1            = BIT_RNG(0,7),
};

#define    reg_rf_iqstart_tstamp2            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x56)
enum{
    FLD_RF_IQSTART_TSTAMP2            = BIT_RNG(0,7),
};

#define    reg_rf_iqstart_tstamp3            REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x57)
enum{
    FLD_RF_IQSTART_TSTAMP3            = BIT_RNG(0,7),
};

#define reg_rf_max_match0                   REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x58)
enum
{
    FLD_RF_MATCH_REG_SYNC0                  =   BIT_RNG(0,7),
};

#define reg_rf_max_match1                   REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x59)
enum
{
    FLD_RF_MATCH_REG_SYNC1                  =   BIT(0),
    FLD_RF_FILT_AGC_GAIN                    =   BIT_RNG(4,6),
    FLD_RF_S2_S8_FLAG                       =   BIT(7),
};
/*******************rx pkt filter************************/
#define reg_rf_pkt_flt_cntl                 REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x60)
enum{
    FLD_RF_PKT_FLT_EN              =    BIT(0),
    FLD_RF_PKT_FLT_INI             =    BIT(1),
    FLD_RF_RXFIFO_CLR_EN           =    BIT(2),
    FLD_RF_FLT_REV_BYTE_EN         =    BIT(3),
    FLD_RF_FLT_BYTE_EN             =    BIT(4),
};

#define reg_rf_pkt_flt_start                    REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x61)
enum
{
    FLD_RF_PKT_START                    =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_end                  REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x62)
enum
{
    FLD_RF_PKT_END                  =   BIT_RNG(0,7),
};

#define reg_rf_pkt_match_threshold                  REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x63)
enum
{
    FLD_RF_PKT_MATCH_TH             =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_match0               REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x64)
enum
{
    FLD_RF_PKT_MATCH0               =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_match1               REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x65)
enum
{
    FLD_RF_PKT_MATCH1               =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_match2               REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x66)
enum
{
    FLD_RF_PKT_MATCH2               =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_match3               REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x67)
enum
{
    FLD_RF_PKT_MATCH3               =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_match4               REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x68)
enum
{
    FLD_RF_PKT_MATCH4               =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_match5               REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x69)
enum
{
    FLD_RF_PKT_MATCH5               =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_match6               REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x6a)
enum
{
    FLD_RF_PKT_MATCH6               =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_match7               REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x6b)
enum
{
    FLD_RF_PKT_MATCH7               =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_mask0                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x6c)
enum
{
    FLD_RF_PKT_MASK0                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_mask1                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x6d)
enum
{
    FLD_RF_PKT_MASK1                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_mask2                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x6e)
enum
{
    FLD_RF_PKT_MASK2                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_mask3                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x6f)
enum
{
    FLD_RF_PKT_MASK3                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_mask4                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x70)
enum
{
    FLD_RF_PKT_MASK4                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_mask5                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x71)
enum
{
    FLD_RF_PKT_MASK5                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_mask6                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x72)
enum
{
    FLD_RF_PKT_MASK6                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_flt_mask7                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x73)
enum
{
    FLD_RF_PKT_MASK7                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_match_idx                REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x74)
enum
{
    FLD_RF_PKT_MATCH_IDX                =   BIT_RNG(0,7),
};

#define reg_rf_pkt_match_value              REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x75)
enum
{
    FLD_RF_PKT_MATCH_VALUE              =   BIT_RNG(0,7),
};

#define    reg_rf_rxdma_adr          0x170080
#define    reg_rf_rxdma_fifo0        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x80)
enum{
    FLD_RF_RXDMA_FIFO0           =  BIT_RNG(0,7),
};


#define    reg_rf_rxdma_fifo1        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x81)
enum{
    FLD_RF_RXDMA_FIFO1           =  BIT_RNG(0,7),
};


#define    reg_rf_rxdma_fifo2        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x82)
enum{
    FLD_RF_RXDMA_FIFO2           =  BIT_RNG(0,7),
};


#define    reg_rf_rxdma_fifo3        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x83)
enum{
    FLD_RF_RXDMA_FIFO3           =  BIT_RNG(0,7),
};

#define    reg_rf_txdma_adr         0x170084
#define    reg_rf_txdma_fifo0        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x84)
enum{
    FLD_RF_TXDMA_FIFO0           =  BIT_RNG(0,7),
};


#define    reg_rf_txdma_fifo1        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x85)
enum{
    FLD_RF_TXDMA_FIFO1           =  BIT_RNG(0,7),
};


#define    reg_rf_txdma_fifo2        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x86)
enum{
    FLD_RF_TXDMA_FIFO2           =  BIT_RNG(0,7),
};


#define    reg_rf_txdma_fifo3        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x87)
enum{
    FLD_RF_TXDMA_FIFO3           =  BIT_RNG(0,7),
};


#define reg_rf_hpm_pos_time_addr           0x170088
#define reg_rf_hpm_pos_time0           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x88)
#define reg_rf_hpm_pos_time1           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x89)
#define reg_rf_hpm_pos_time2           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x8a)
#define reg_rf_hpm_pos_time3           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x8b)

#define reg_rf_hpm_neg_time_addr           0x17008c
#define reg_rf_hpm_neg_time0           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x8c)
#define reg_rf_hpm_neg_time1           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x8d)
#define reg_rf_hpm_neg_time2           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x8e)
#define reg_rf_hpm_neg_time3           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x8f)

#define reg_rf_tr_turnaround_pos_time_addr  0x170090
#define reg_rf_tr_turnaround_pos_time       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0x90)
#define reg_rf_tr_turnaround_pos_time0      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x90)
#define reg_rf_tr_turnaround_pos_time1      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x91)
#define reg_rf_tr_turnaround_pos_time2      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x92)
#define reg_rf_tr_turnaround_pos_time3      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x93)

#define reg_rf_tr_turnaround_neg_time_addr  0x170094
#define reg_rf_tr_turnaround_neg_time       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0x94)
#define reg_rf_tr_turnaround_neg_time0      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x94)
#define reg_rf_tr_turnaround_neg_time1      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x95)
#define reg_rf_tr_turnaround_neg_time2      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x96)
#define reg_rf_tr_turnaround_neg_time3      REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x97)

#define reg_rf_pa_pup_pos_time_addr         0x170098
#define reg_rf_pa_pup_pos_time0             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x98)
#define reg_rf_pa_pup_pos_time1             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x99)
#define reg_rf_pa_pup_pos_time2             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x9a)
#define reg_rf_pa_pup_pos_time3             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x9b)

#define reg_rf_pa_pup_neg_time_addr         0x17009c
#define reg_rf_pa_pup_neg_time0             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x9c)
#define reg_rf_pa_pup_neg_time1             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x9d)
#define reg_rf_pa_pup_neg_time2             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x9e)
#define reg_rf_pa_pup_neg_time3             REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x9f)

#define reg_rf_en_freq_comp_time_addr       0x1700a0
#define reg_rf_en_freq_comp_time0           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0xa0)
#define reg_rf_en_freq_comp_time1           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0xa1)
#define reg_rf_en_freq_comp_time2           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0xa2)
#define reg_rf_en_freq_comp_time3           REG_ADDR8(REG_BASEBAND_BASE_ADDR+0xa3)



/**************************************AES-CCM***********************************************/
#define reg_rf_tlk_crypt_skey(i)            REG_ADDR8(REG_BASEBAND_BASE_ADDR+ 0xa4 + (i))
#define reg_rf_tlk_crypt_iv(i)              REG_ADDR8(REG_BASEBAND_BASE_ADDR+ 0xb4 + (i))

#define reg_rf_aad_mask                     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0xbc)
#define reg_rf_tlk_crypt_soft_rst           REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0xbd)
enum{
    FLD_RF_TLK_CRYPT_SOFT_RST           =   BIT(0), //tlk_crypt software reset, active high /**< W1C */
    FLD_RF_TLK_CRYPT_DSB                =   BIT(1),//TLK BLE encryption/decryption disable
};


/**************************************************************************************/

/*********************************Freq_hopping***************************************************/

/**************************************************************************************/

/*******************************AOA/AOD_ANT 6bit******************************************************/
#define    reg_rf_ant_lut_0          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0xe0)
enum{
    FLD_RF_ANT_LUT0              =  BIT_RNG(0,5),
    FLD_RF_ANT_LUT1              =  BIT_RNG(6,7),
};
/***********************************************************************************************/


/***************************************CRC********************************************************/
#define    reg_rf_crc_config1         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x110)
enum{
    FLD_RF_CRC_START_BYTE             = BIT_RNG(0,5),
};

#define    reg_rf_crc_config2         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x111)
enum{
    FLD_RF_CRC_LENGTH                 = BIT_RNG(0,2),
    FLD_RF_CRC_BYTE_ORD               = BIT(3),

};
#define    reg_rf_crc_init           REG_ADDR32(REG_BASEBAND_BASE_ADDR+0x114)
#define    reg_rf_crc_init0          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x114)
#define    reg_rf_crc_init1          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x115)
#define    reg_rf_crc_init2          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x116)
#define    reg_rf_crc_init3          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x117)

#define    reg_rf_crc_xorout         REG_ADDR32(REG_BASEBAND_BASE_ADDR+0x118)
#define    reg_rf_crc_xorout0        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x118)
#define    reg_rf_crc_xorout1        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x119)
#define    reg_rf_crc_xorout2        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x11a)
#define    reg_rf_crc_xorout3        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x11b)

#define    reg_rf_crc_poly           REG_ADDR32(REG_BASEBAND_BASE_ADDR+0x11c)
#define    reg_rf_crc_poly0          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x11c)
#define    reg_rf_crc_poly1          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x11d)
#define    reg_rf_crc_poly2          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x11e)
#define    reg_rf_crc_poly3          REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x11f)

/***********************************************************************************************/
#define    reg_rf_tx_mic_val0        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x120)
#define    reg_rf_tx_mic_val1        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x121)
#define    reg_rf_tx_mic_val2        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x122)
#define    reg_rf_tx_mic_val3        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x123)
#define    reg_rf_rx_mic_val0        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x124)
#define    reg_rf_rx_mic_val1        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x125)
#define    reg_rf_rx_mic_val2        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x126)
#define    reg_rf_rx_mic_val3        REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x127)
/*****************************************PN_CFG******************************************************/
#define    reg_rf_pn_cfg         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x128)
enum{
    FLD_RF_PN_LENGTH             =  BIT_RNG(0,4),
};

#define    reg_rf_pn_poly0         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x129)
#define    reg_rf_pn_poly1         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x12a)
#define    reg_rf_pn_poly2         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x12b)

#define    reg_rf_pn_init0         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x12c)
#define    reg_rf_pn_init1         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x12d)
#define    reg_rf_pn_init2         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x12e)

/***********************************************************************************************/

#define    reg_rf_lr_cfg0         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x130)
enum{
    FLD_RF_LR_CFG0             =    BIT_RNG(0,7),
};

#define    reg_rf_lr_cfg1         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x131)
enum{
    FLD_RF_LR_CFG1             =    BIT_RNG(0,2),
};

#define    reg_rf_zb_cfg1         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x132)
enum{
    FLD_RF_ZB_PHR_EN             =  BIT(0),
    FLD_RF_ZB_PHR_EXTEND_EN      =  BIT(1),
    FLD_RF_ZB_HYBEE_NEW          =  BIT(2),
    FLD_RF_ZB_PHR_CRC_EN         =  BIT(3),
    FLD_RF_ZB_SFD_ORD            =  BIT(4),
    FLD_RF_ZB_PSDU_LENGTH_HIGH   =  BIT_RNG(5,7),
};

#define    reg_rf_zb_cfg2         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x133)
enum{
    FLD_RF_ZB_PSDU_LENGTH_LOW   =   BIT_RNG(0,7),
};

#define    reg_rf_zb_cfg3         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x134)
enum{
    FLD_RF_ZB_GAP_LENGTH   =    BIT_RNG(0,5),
    FLD_RF_ZB_SFD_LENGTH   =    BIT_RNG(6,7),
};

#define    reg_rf_new_2g4_cfg0         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x138)
enum{
    FLD_RF_NEW_2G4_H0_SIZE   =  BIT_RNG(0,4),
};

#define    reg_rf_new_2g4_cfg1         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x139)
enum{
    FLD_RF_NEW_2G4_H1_SIZE   =  BIT_RNG(0,4),
};

#define    reg_rf_new_2g4_cfg2         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x13a)
enum{
    FLD_RF_NEW_2G4_LENGTH_SIZE      =   BIT_RNG(0,4),
    FLD_RF_NEW_2G4_LENGTH_BIT_ORD   =   BIT(7),
};

#define    reg_rf_new_2g4_cfg3         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x13b)
enum{
    FLD_RF_NEW_2G4_PID_START_BIT      = BIT_RNG(0,5),
    FLD_RF_NEW_2G4_PID_EN             = BIT(7),
};

#define    reg_rf_new_2g4_cfg4         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x13c)
enum{
    FLD_RF_NEW_2G4_NOACK_START_BIT      =   BIT_RNG(0,5),
    FLD_RF_NEW_2G4_NOACK_EN             =   BIT(7),
};

#define    reg_rf_new_2g4_cfg5         REG_ADDR8(REG_BASEBAND_BASE_ADDR+0x13d)
enum{
    FLD_RF_NEW_2G4_LENGTH_ADJ      =    BIT_RNG(0,5),
};

/***********************************************************************************************/

/**
 * BB_LL
 */
#define               REG_BB_LL_BASE_ADDR          0x170200


#define    reg_rf_ll_cmd             REG_ADDR8(REG_BB_LL_BASE_ADDR)
enum{
    FLD_RF_R_CMD                 =  BIT_RNG(0,3),
    FLD_RF_R_STOP                =  0,
    FLD_RF_R_BTX                 =  1,
    FLD_RF_R_BRX                 =  2,
    FLD_RF_R_PTX                 =  3,
    FLD_RF_R_PRX                 =  4,
    FLD_RF_R_STX                 =  5,
    FLD_RF_R_SRX                 =  6,
    FLD_RF_R_STR                 =  7,
    FLD_RF_R_SRT                 =  8,
    FLD_RF_R_CMD_TRIG            =  BIT(7),
};


#define    reg_rf_ll_rest_pid        REG_ADDR8(REG_BB_LL_BASE_ADDR+0x01)
enum{
    FLD_RF_R_RESET_PID_0         =  BIT(0),
    FLD_RF_R_RESET_PID_1         =  BIT(1),
    FLD_RF_R_RESET_PID_2         =  BIT(2),
    FLD_RF_R_RESET_PID_3         =  BIT(3),
    FLD_RF_R_RESET_PID_4         =  BIT(4),
    FLD_RF_R_RESET_PID_5         =  BIT(5),
    FLD_RF_R_CRC_CHECK_EN         = BIT(6),
    FLD_RF_R_CRC_LAT_RST         =  BIT(7),
};


#define    reg_rf_ll_ctrl0          REG_ADDR8(REG_BB_LL_BASE_ADDR+0x02)
enum{
    FLD_RF_R_MD_EN               =  BIT(0),
    FLD_RF_R_ID_EN               =  BIT(1),
    FLD_RF_R_RX_NOACK_MAN        =  BIT(2),
    FLD_RF_R_RX_NOACK_MAN_EN     =  BIT(3),
    FLD_RF_R_TX_EN_MAN           =  BIT(4),
    FLD_RF_R_RX_EN_MAN           =  BIT(5),
    FLD_RF_R_TX_TRIQ_AUTO_EN     =  BIT(6),
    FLD_RF_R_S_TX_TIMEOUT_EN     =  BIT(7),
};


#define    reg_rf_ll_ctrl_1         REG_ADDR8(REG_BB_LL_BASE_ADDR+0x03)

enum{
    FLD_RF_FSM_TIMEOUT_EN       =   BIT(0),
    FLD_RF_RX_FIRST_TIMEOUT_EN  =   BIT(1),
    FLD_RF_RX_TIMEOUT_EN        =   BIT(2),
    FLD_RF_CRC_2_EN             =   BIT(3),

    //BLE mode
    FLD_RF_INIT_PRX_PID         =   BIT_RNG(4,5),
    FLD_RF_INIT_PTX_PID         =   BIT_RNG(6,7),
};

#define     FSM_TIMEOUT_ENABLE      ( reg_rf_ll_ctrl_1 |= FLD_RF_FSM_TIMEOUT_EN )
#define     FSM_TIMEOUT_DISABLE     ( reg_rf_ll_ctrl_1 &= (~FLD_RF_FSM_TIMEOUT_EN) )

#define    reg_rf_ll_tx_stl         REG_ADDR16(REG_BB_LL_BASE_ADDR+0x04)

#define    reg_rf_ll_tx_stl_l       REG_ADDR8(REG_BB_LL_BASE_ADDR+0x04)


#define    reg_rf_ll_tx_stl_h       REG_ADDR8(REG_BB_LL_BASE_ADDR+0x05)
enum{
    FLD_RF_R_T_TXSTL_H           =  BIT_RNG(0,3),
};


#define    reg_rf_ll_rxwait_l       REG_ADDR8(REG_BB_LL_BASE_ADDR+0x06)


#define    reg_rf_ll_rxwait_h       REG_ADDR8(REG_BB_LL_BASE_ADDR+0x07)
enum{
    FLD_RF_R_T_RXWAIT_H           = BIT_RNG(0,3),
};


#define    reg_rf_ll_rx_l           REG_ADDR8(REG_BB_LL_BASE_ADDR+0x0a)

#define    reg_rf_rx_timeout        REG_ADDR16(REG_BB_LL_BASE_ADDR+0x0a)

#define    reg_rf_ll_rx_h           REG_ADDR8(REG_BB_LL_BASE_ADDR+0x0b)
enum{
    FLD_RF_R_T_RX_H               = BIT_RNG(0,3),
};


#define    reg_rf_ll_rxstl_l        REG_ADDR8(REG_BB_LL_BASE_ADDR+0x0c)


#define    reg_rf_ll_rxstl_h        REG_ADDR8(REG_BB_LL_BASE_ADDR+0x0d)
enum{
    FLD_RF_R_T_RXSTL_H            = BIT_RNG(0,3),
};


#define    reg_rf_ll_txwait_l       REG_ADDR8(REG_BB_LL_BASE_ADDR+0x0e)


#define    reg_rf_ll_txwait_h       REG_ADDR8(REG_BB_LL_BASE_ADDR+0x0f)
enum{
    FLD_RF_R_T_TXWAIT_H           = BIT_RNG(0,3),
};


#define    reg_rf_ll_ard_l          REG_ADDR8(REG_BB_LL_BASE_ADDR+0x10)


#define    reg_rf_ll_ard_h          REG_ADDR8(REG_BB_LL_BASE_ADDR+0x11)
enum{
    FLD_RF_R_T_ARD_H              = BIT_RNG(0,3),
};


#define    reg_rf_t_coex_t1         REG_ADDR8(REG_BB_LL_BASE_ADDR+0x12)
#define    reg_rf_t_coex_t2         REG_ADDR8(REG_BB_LL_BASE_ADDR+0x13)
#define    reg_rf_ll_max_retry_cnt  REG_ADDR8(REG_BB_LL_BASE_ADDR+0x14)


#define    reg_rf_ll_ctrl2          REG_ADDR8(REG_BB_LL_BASE_ADDR+0x15)
enum{
    FLD_RF_R_TXCHN_MAN            = BIT_RNG(0,2),
    FLD_RF_R_NOACK_RETRY_CNT_EN       = BIT(3),
    FLD_RF_R_TXCHN_MAN_EN         = BIT(4),
    FLD_RF_R_NOACK_REV_EN         = BIT(5),
    FLD_RF_R_RXIRQ_REPORT_ALL     = BIT(6),
    FLD_RF_R_REP_SN_PID_EN        = BIT(7),
};


#define    reg_rf_ll_ctrl3          REG_ADDR8(REG_BB_LL_BASE_ADDR+0x16)
enum{
    FLD_RF_R_TX_EN_DLY_EN         = BIT(0),
    FLD_RF_R_PLL_RESET_EN         = BIT(1),
    FLD_RF_R_CMD_SCHEDULE_EN       =    BIT(2),
    FLD_RF_R_PLL_EN_MAN           = BIT(3),
    FLD_RF_R_T_TX_EN_DLY          = BIT_RNG(4,7),
};


#define    reg_rf_ll_pll_reset      REG_ADDR8(REG_BB_LL_BASE_ADDR+0x17)
#define    reg_rf_ll_cmd_schedule   REG_ADDR32(REG_BB_LL_BASE_ADDR+0x18)
#define    reg_rf_ll_cmd_schedule0  REG_ADDR8(REG_BB_LL_BASE_ADDR+0x18)
#define    reg_rf_ll_cmd_schedule1  REG_ADDR8(REG_BB_LL_BASE_ADDR+0x19)
#define    reg_rf_ll_cmd_schedule2  REG_ADDR8(REG_BB_LL_BASE_ADDR+0x1a)
#define    reg_rf_ll_cmd_schedule3  REG_ADDR8(REG_BB_LL_BASE_ADDR+0x1b)
#define    reg_rf_ll_irq_mask_l     REG_ADDR8(REG_BB_LL_BASE_ADDR+0x1c)
#define    reg_rf_ll_irq_mask_m     REG_ADDR8(REG_BB_LL_BASE_ADDR+0x1d)
#define    reg_rf_ll_irq_mask_h     REG_ADDR8(REG_BB_LL_BASE_ADDR+0x4e)
#define    reg_rf_ll_irq_mask_h1    REG_ADDR8(REG_BB_LL_BASE_ADDR+0x4f)
#define    reg_rf_irq_mask          REG_ADDR16(REG_BB_LL_BASE_ADDR+0x1c)
#define    reg_rf_irq_mask1         REG_ADDR16(REG_BB_LL_BASE_ADDR+0x4e)


#define    reg_rf_ll_tx_id          REG_ADDR8(REG_BB_LL_BASE_ADDR+0x1e)
enum{
    FLD_RF_R_TX_ID                = BIT_RNG(0,6),
    FLD_RF_R_LL_IRQ_MASK          = BIT(7),
};

#define    reg_rf_irq_status     REG_ADDR16(REG_BB_LL_BASE_ADDR+0x20)
#define    reg_rf_irq_status_h   REG_ADDR8(REG_BB_LL_BASE_ADDR+0x4d)
#define    reg_rf_irq_status_h1  REG_ADDR8(REG_BB_LL_BASE_ADDR+0x45)

typedef enum{
    FLD_RF_IRQ_RX                =  BIT(0),/*Auto/Manual: an interrupt is generated after each packet is received.*/
    FLD_RF_IRQ_TX                =  BIT(1),/*Auto/Manual: an interrupt is generated after each packet is sent*/
    FLD_RF_IRQ_RX_TIMEOUT        =  BIT(2),/*Auto: If the packet is still not received within the time set from trigger to timeout,
                                                   an interrupt is generated and the state machine returns to idle state.
                                                   The timeout time is set by void rf_set_rx_timeout(); and the timeout starts from the trigger RX.*/
    FLD_RF_IRQ_RX_FIFO_FULL      =  BIT(3),/*Auto/Manual:An interrupt is generated when the data size of the total received packets is exceeded to the allocated storage range of Rx SRAM
                                                         TL721X chip unavailable, it will not work after configuration.(add by chenxi.wang, confirmed by jingyong 202402029)*/
    FLD_RF_IRQ_RX_CRC_2          =  BIT(4),/*Auto:BTX,BRX,PTX,PRX will generate interrupt if CRC error is detected twice in a row during packet receiving (continuous packet receiving).*/
    FLD_RF_IRQ_CMD_DONE          =  BIT(5),/*Auto:After completing a normal packet receiving or sending operation,
                                                  the state machine returns to the IDLE state normally and generates an interrupt.*/
    FLD_RF_IRQ_FSM_TIMEOUT       =  BIT(6),/*Auto:It contains the state machine used for switching from receive to send, specifying the time of the entire state machine, and determining
                                                  whether the specified time is exceeded in the TX_WAIT state*/
    FLD_RF_IRQ_TX_RETRYCNT       =  BIT(7),/*Auto:Generate an interrupt when the number of ptx retry exceeds the set r_max_retry_cnt.*/
    FLD_RF_IRQ_TX_DS             =  BIT(8),/*Auto:Length of payload (sent by PTX, PRX) ! = 0, generate tx_ds interrupt.*/
    FLD_RF_IRQ_RX_DR             =  BIT(9),/*Auto:PRX, PTX, SRX received packets detect the packet payload length ! = 0, generate rx_dr interrupt*/
    FLD_RF_IRQ_FIRST_TIMEOUT     =  BIT(10),/*Auto:BRX, PRX, SRX, SRT first RX timeout, when the first packet receive timeout it will generate an interrupt*/
    FLD_RF_IRQ_INVALID_PID       =  BIT(11),/*Auto:PTX or PRX receives invalid pid and generates interrupt*/
    FLD_RF_IRQ_STX_TIMEOUT       =  BIT(12),/*Auto:The STX state does not wait for tx_done in the specified time, thus timeout, generating an interrupt.*/
    FLD_RF_IRQ_WIFI_DENY         =  BIT(13),/*Auto:After the Bluetooth chip receives the wifi_deny signal from the WiFi chip when it is connected to the WiFi chip,
                                                   the Bluetooth chip generates an interrupt*/
    FLD_RF_IRQ_SUPP_OF           =  BIT(14),/*Auto/Manual:The interrupt is mainly used for AOA and AOD. If iq sampling frequency is too high the hardware FIFO will overflow error,
                                                          generate interrupt iq sample and synchronize fifo overflow.*/
    FLD_RF_IRQ_RXDMA_OF          =  BIT(15),
    FLD_RF_IRQ_TR_TURNAROUND     =  BIT(16),
    FLD_RF_IRQ_TLK_RXCRYPT_ERR   =  BIT(17),/*Auto/Manual:Indicates real time decryption error, happens when CCM decryption is too slow compared to receiving bitstream by Packet Controller. */
    FLD_RF_IRQ_TLK_TXCRYPT_ERR   =  BIT(18),/*Auto/Manual:Indicates Real Time encryption error, happens when CCM encryption is too slow compared to transmitting requests by Packet Controller. */
    FLD_RF_IRQ_HIT_SYNC          =  BIT(19),/*Auto/Manual:An interrupt is generated when sync up of one packet is detected in Rx mode*/
    FLD_RF_IRQ_HEADER_DONE       =  BIT(20),/*Auto/Manual:Interrupt is generated when the Header Field of one packet has been received completely.*/
    FLD_RF_IRQ_PKT_UNMATCH       =  BIT(21),/*Auto/Manual:An interrupt is generated when the certain bits of the received packet are not matched to the predefined ones within the packet filter registers in Rx mode.*/
    FLD_RF_IRQ_PKT_MATCH         =  BIT(22),/*Auto/Manual:An interrupt is generated when the certain bits of the received packet are matched to the predefined ones within the packet filter registers in Rx mode. */
    FLD_RF_IRQ_RX_ZB_FREQ_HOP    =  BIT(23),/*Auto/Manual:When Zigbee performs dual-channel detection, it switches channels at preset intervals, and the interrupt generated at this time*/
    FLD_RF_IRQ_RX_FREQ_HOP_FIXED =  BIT(24),/*Auto/Manual:When Zigbee performs dual-channel detection, the interrupt is generated when it stays in one of the channels to successfully receive packets and fixes the interrupt when it continues to receive over-the-air packets in this channel.*/
    FLD_RF_IRQ_TRX_FCAL_DONE     =  BIT(25),/*After RF is turned on, the interrupt generated at the moment of fcal calibration is completed at tx or rx (including switching channels during dual-channel detection by zigbee).*/
    FLD_RF_IRQ_RADIO_DONE        =  BIT(26),/*TODO:TODO:This function interface is not available at this time, and will be updated in subsequent releases.(unverified)*/
    FLD_RF_IRQ_ALL               =  0x7ffffff,
}rf_irq_e;


#define    reg_rf_ll_pid_l         REG_ADDR8(REG_BB_LL_BASE_ADDR+0x22)
enum{
    FLD_RF_PID0                   = BIT_RNG(0,1),
    FLD_RF_PID1                   = BIT_RNG(2,3),
    FLD_RF_PID2                   = BIT_RNG(4,5),
    FLD_RF_PID3                   = BIT_RNG(6,7),
};


#define    reg_rf_ll_pid_h         REG_ADDR8(REG_BB_LL_BASE_ADDR+0x23)
enum{
    FLD_RF_PID4                   = BIT_RNG(0,1),
    FLD_RF_PID5                   = BIT_RNG(2,3),
    FLD_RF_NESN                   = BIT(4),
    FLD_RF_TURNAROUND_IRQ         = BIT(7),
};


#define    reg_rf_ll_2d_sclk       REG_ADDR8(REG_BB_LL_BASE_ADDR+0x24)
typedef enum {
    FLD_RF_STATE_MACHINE_IDLE = 0,          /**< idle */
    FLD_RF_STATE_MACHINE_ACTIVE,            /**< active */
    FLD_RF_STATE_MACHINE_TX_SETTLE,         /**< tx settle*/
    FLD_RF_STATE_MACHINE_TX,                /**< tx */
    FLD_RF_STATE_MACHINE_RX_WAIT,           /**< rx wait */
    FLD_RF_STATE_MACHINE_RX,                /**< rx */
    FLD_RF_STATE_MACHINE_TX_WAIT,           /**< tx wait */
} state_machine_status_e;


#define    reg_rf_ll_retry_cnt     REG_ADDR8(REG_BB_LL_BASE_ADDR+0x25)
enum{
    FLD_RF_LL_RETRY_CNT           = BIT_RNG(0,7),
};


#define    reg_rf_ll_cnt0          REG_ADDR8(REG_BB_LL_BASE_ADDR+0x26)
enum{
    FLD_RF_CRC_CNT                = BIT_RNG(0,3),
    FLD_RF_NAK_CNT                = BIT_RNG(4,7),
};


#define    reg_rf_ll_cnt1          REG_ADDR8(REG_BB_LL_BASE_ADDR+0x27)
enum{
    FLD_RF_OLD_CNT                = BIT_RNG(0,3),
    FLD_RF_ID_CNT                 = BIT_RNG(4,7),
};

#define    reg_rf_ll_rx_fst_timeout      REG_ADDR32(REG_BB_LL_BASE_ADDR+0x28)
#define    reg_rf_ll_rx_fst_l      REG_ADDR8(REG_BB_LL_BASE_ADDR+0x28)
#define    reg_rf_ll_rx_fst_m      REG_ADDR8(REG_BB_LL_BASE_ADDR+0x29)
#define    reg_rf_ll_rx_fst_h      REG_ADDR8(REG_BB_LL_BASE_ADDR+0x2a)

#define    reg_rf_ll_fsm_timeout   REG_ADDR32(REG_BB_LL_BASE_ADDR+0x2c)
#define    reg_rf_ll_fsm_timeout_l REG_ADDR8(REG_BB_LL_BASE_ADDR+0x2c)
#define    reg_rf_ll_fsm_timeout_m REG_ADDR8(REG_BB_LL_BASE_ADDR+0x2d)
#define    reg_rf_ll_fsm_timeout_h REG_ADDR8(REG_BB_LL_BASE_ADDR+0x2e)


#define    reg_rf_coex_enable      REG_ADDR8(REG_BB_LL_BASE_ADDR+0x30)
enum{
    FLD_RF_COEX_EN                = BIT(0),
    FLD_RF_COEX_WF_DN_POL         = BIT(1),
    FLD_RF_COEX_STATUS            = BIT(2),
    FLD_RF_COEX_TRX_POL           = BIT(3),
    FLD_RF_TRX_PRIO               = BIT(4),
    FLD_RF_TX_PRIO                = BIT(5),
    FLD_RF_RX_PRIO                = BIT(6),
};

#define    reg_rf_on_ccm_control     REG_ADDR8(REG_BB_LL_BASE_ADDR+0x31)
enum{
    FLD_RF_ON_MANUAL              = BIT(0),
    FLD_RF_ON                     = BIT(1),
    FLD_RF_DIS_TXLEN_FLG_UPD      = BIT(2),
    FLD_RF_DIS_TXCNT_UPDATE       = BIT(3),
    FLD_RF_DIS_RXCNT_UPDATE       = BIT(4),
};


#define   REG_TL_MODEM_BASE_ADDR                    0x170400
#define     reg_rf_modem_mode_cfg_rx1_0 REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x20)
enum
{
    FLD_RF_LR_MODE                    = BIT(0),
    FLD_RF_CONT_MODE                  = BIT(3),
    FLD_RF_RX_DATA_CLK_DBG            = BIT(5),
    FLD_RF_LR_TRIG_MODE               = BIT(6),
    FLD_RF_FDC_DBG_SEL                = BIT(7),

};
#define     reg_rf_modem_mode_cfg_rx1_1     REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x21)
enum
{
    FLD_RF_ZIGBEE_MODE_RX               = BIT(0),
    FLD_RF_LR_MODEM_SEL                 = BIT(2),
    FLD_RF_LR_VITERBI_SEL               = BIT(3),
    FLD_RF_HYBEE_SEL                    = BIT_RNG(4,6),
    FLD_RF_ZIGBEE_SEL                   = BIT(7),
};

#define     reg_rf_modem_mode_ctrl_tx1_0    REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x22)
enum
{
    FLD_RF_BLE_MODEM_TX           = BIT(0),
};

#define     reg_rf_modem_mode_ctrl_tx1_1    REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x23)
enum
{
    FLD_RF_ZIGBEE_MODEM_TX            = BIT(7),
};

#define     reg_rf_modem_mode_cfg_txrx_0    REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x26)
enum
{
    FLD_RF_X1_CQ_N                    = BIT(0),
    FLD_RF_SYNC_RST_SEL               = BIT(1),
    FLD_RF_GF_BT                      = BIT(2),
};

#define     reg_rf_modem_rxc_mi_flex_ble_0  REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x48)

#define     reg_rf_modem_rxc_mi_flex_ble_1  REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x49)

#define     reg_rf_modem_rx_ctrl_0      REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x4c)
enum
{
    FLD_RF_RX_ACC_LNE                   = BIT_RNG(0,2),
    FLD_RF_RX_CRC_EN                    = BIT(3),
    FLD_RF_SFD0_NUM                     = BIT_RNG(4,6),
};

#define     reg_rf_modem_rx_ctrl_1      REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x4d)
enum
{
    FLD_RF_RX_EN                  = BIT_RNG(0,5),
    FLD_RF_LR_MAN_EN              = BIT(6),
    FLD_RF_LR_MAN_SEL                 = BIT(7),
};

#define     reg_rf_modem_sync_thres_ble     REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x4e)
#define     reg_rf_modem_fdc_dbg_lat        REG_ADDR16(REG_TL_MODEM_BASE_ADDR+0x58)
#define     reg_rf_modem_gain_lat0          REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x5c)

#define     reg_rf_agc_rssi_lat     REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x5d)

#define     ref_rf_tim_align_1      REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x79)
#define     reg_rf_bypass_filt_1    REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x7b)

#define     reg_rf_tx_tl_ctrl           REG_ADDR8(REG_TL_MODEM_BASE_ADDR+0x9a)
enum
{
    FLD_RF_TX_TP_EN                 = BIT(0),
    FLD_RF_TX_IQ_EN                 = BIT(1),
    FLD_RF_TX_MPSK_EN                   = BIT(2),
    FLD_RF_TX_TP_ALIGN_BT                   = BIT(3),
    FLD_RF_TX_TP_ALIGN_BLE                  = BIT(4),
};

#define     reg_rf_mode_cfg_rx1_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x20)
enum
{
    FLD_RF_BW_CODE                    = BIT_RNG(1,3),
    FLD_RF_SC_CODE                    = BIT(4),
};

#define     reg_rf_mode_cfg_rx1_1       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x21)
enum
{
    FLD_RF_MODE_VANT_RX               = BIT(1),
    FLD_RF_FE_RTRIM_RX                = BIT_RNG(2,4),
    FLD_RF_IF_FREQ                    = BIT_RNG(5,6),

};

#define     reg_rf_mode_cfg_tx1_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x22)
enum
{
    FLD_RF_BLE_MODE_TX                = BIT(0),
    FLD_RF_VCO_TRIM_KVM               = BIT_RNG(1,3),
    FLD_RF_HPMC_EXP_DIFF_COUNT_L      = BIT_RNG(4,7),
};

#define     reg_rf_mode_cfg_tx1_1       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x23)
enum
{
    FLD_RF_HPMC_EXP_DIFF_COUNT_H      = BIT_RNG(0,4),
    FLD_RF_DAC_TRIM_CFBK              = BIT_RNG(5,6),

};

#define     reg_rf_mode_cfg_txrx_0      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x26)
enum
{
    FLD_RF_DIS_CLK_DIG_O            = BIT(0),
    FLD_RF_VANT_PULLDN              = BIT(1),
    FLD_RF_TX_IQ_MODE_EN_BT         = BIT(2),
    FLD_RF_LDO_ANT_TRIM             = BIT_RNG(3,5),
    FLD_RF_CBPF_TYPE                = BIT(6),
    FLD_RF_TX_PA_PWR_L              = BIT(7),
};


#define     reg_rf_mode_cfg_txrx_1      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x27)

enum
{
    FLD_RF_TX_PA_PWR_H              = BIT_RNG(0,4),
};

#define     reg_rf_burst_cfg_txrx_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x28)
enum
{
    FLD_RF_CHNL_NUM             = BIT_RNG(0,7),
};

#define     reg_rf_burst_cfg_txrx_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x29)
enum
{
    FLD_RF_CH_NUMLL_SEL             = BIT(0),
    FLD_RF_TX_EN_PIF                = BIT(1),
    FLD_RF_RX_EN_PIF                = BIT(2),
    FLD_RF_RX_TIM_SRQ_SEL_TESQ      = BIT(3),
    FLD_RF_TX_TIM_SRQ_SEL_TESQ      = BIT(4),
    FLD_RF_FE_CTRIM             = BIT_RNG(5,7),
};

#define     reg_rf_mode_cfg_tx3_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x3c)
enum
{
    FLD_RF_MODE_CFG_TX3             = BIT_RNG(0,5),
    FLD_RF_MODE_VANT_TX_BLE         = BIT(6),
    FLD_RF_TX_IQ_MODE_EN_BLE        = BIT(7),
};



#define     reg_rf_mode_cfg_tx3_1       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x3d)
enum
{
    FLD_RF_LDO_ANT_TRIM_BLE         = BIT_RNG(0,2),
    FLD_RF_BT_BLE_SEL_EN            = BIT(3),
    FLD_RF_TXC_PWR_SRL              = BIT(4),
    FLD_RF_BW_CODE_BLE              = BIT_RNG(5,7),

};
#define     reg_rf_radio_mode_cfg_rx2_0         REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x3e)
enum
{
    FLD_RF_TXC_MI_FLEX_L            = BIT_RNG(0,7),
};

#define     reg_rf_radio_mode_cfg_rx2_1         REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x3f)
enum
{
    FLD_RF_TXC_MI_FLEX_H                = BIT(0),
    FLD_RF_LOW_RATE_EN                  = BIT(1),
    FLD_RF_LOW_RATE_MODE                = BIT(2),
    FLD_RF_TX_DATA_RS                   = BIT_RNG(3,4),
    FLD_RF_NORM_PKT_FAST_STL_EN         = BIT(5),
    FLD_RF_NON_ID_FAST_STL_EN           = BIT(6),
    FLD_RF_FAST_STL_EN                  = BIT(7),
};

#define     reg_rf_radio_txrx_dbg1_0    REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x40)
enum
{
    FLD_RF_AGC_DISABLE                  = BIT(1),
    FLD_RF_RX_GAIN                      = BIT_RNG(2,4),
    FLD_RF_ADC_IQ_SWAP                  = BIT(5),
    FLD_RF_INVERT_CLK_DSM               = BIT(6),
    FLD_RF_DSM_FRAC_LSW_L               = BIT(7),
};

#define     reg_rf_radio_txrx_dbg1_1    REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x41)
enum
{
    FLD_RF_DSM_FRAC_LSW_H               = BIT_RNG(0,1),
    FLD_RF_SPARE_1                      = BIT(2),
    FLD_RF_AM_OVERRIDE_EN               = BIT(3),
    FLD_RF_AM_OVERRIDE_VAL              = BIT(4),
    FLD_RF_LOAD_PREV_GAIN               = BIT(5),
    FLD_RF_ADC_CLIP_EN                  = BIT(6),
};


#define     reg_rf_txrx_dbg3_0          REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x44)
enum
{
    FLD_RF_CHNL_FREQ_DIRECT         = BIT(0),
    FLD_RF_CHNL_FREQ_L              = BIT_RNG(1,7),
};

#define     reg_rf_txrx_dbg3_1          REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x45)
enum
{
    FLD_RF_CHNL_FREQ_H                  = BIT_RNG(0,5),
    FLD_RF_DSN_DITHER_DISABLE           = BIT(6),
    FLD_RF_DSM_INT_MODE             = BIT(7),
};


#define     reg_rf_cal_ow_ctrl_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x80)
enum
{
    FLD_RF_DSM_RUN_OW                 = BIT(0),
    FLD_RF_LDOT_TX_RUN_OW                 = BIT(1),
    FLD_RF_RCCAL_RUN_OW               = BIT(2),
    FLD_RF_FCAL_TX_RUN_OW                 = BIT(3),
    FLD_RF_RXDCOC_RUN_OW                  = BIT(4),
    FLD_RF_HPMC_RUN_OW                = BIT(5),
    FLD_RF_LOCD_RUN_OW                = BIT(6),
};

#define     reg_rf_cal_ow_ctrl_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x81)
enum
{
    FLD_RF_LDOT_RX_RUN_OW                 = BIT(0),
    FLD_RF_FCAL_RX_RUN_OW                 = BIT(1),
    FLD_RF_LDOT_DEBUG_RUN_OW              = BIT(2),
    FLD_RF_FCAL_DEBUG_RUN_OW              = BIT(3),
    FLD_RF_TIM_REC_REST_BLE               = BIT(6),
    FLD_RF_HOLD0                          = BIT(7),
};

#define     reg_rf_cal_ow_val_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x82)

#define     reg_rf_cal_ow_val_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x83)
enum
{
    FLD_RF_LDOT_RX_RUN                    = BIT(0),
    FLD_RF_FCAL_RX_RUN                    = BIT(1),
    FLD_RF_LDOT_DEBUG_RUN                 = BIT(2),
    FLD_RF_FCAL_DEBUG_RUN                 = BIT(3),
};

#define     reg_rf_txrx_cb_cal_ctrl     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x84)
enum
{
    FLD_RF_LDOT_TX_RUN_CB                 = BIT(0),
    FLD_RF_FCAL_TX_RUN_CB                 = BIT(1),
    FLD_RF_HPMC_RUN_CB                    = BIT(2),
    //
    FLD_RF_LDOT_RX_RUN_CB                 = BIT(4),
    FLD_RF_FCAL_RX_RUN_CB                 = BIT(5),
    FLD_RF_RCCAL_RUN_CB                   = BIT(6),
    FLD_RF_RXDCOC_RUN_CB                  = BIT(7),
};

#define     reg_rf_txrx_en_dbg_ow_ctrl0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x86)
enum
{
    FLD_RF_DCOC_GAIN_CFG_OW               = BIT(0),
    FLD_RF_RX_DIG_EN_OW                   = BIT(1),
    FLD_RF_TXC_TX_EN_OW                   = BIT(2),
    FLD_RF_TX_HPM_CAL_EN_OW               = BIT(3),
    FLD_RF_TX_MOD_EN_OW                   = BIT(4),
    FLD_RF_RESTART_TIM_SEL                = BIT_RNG(5,7),
};

#define     reg_rf_txrx_en_dbg_ow_ctrl1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x87)

#define     reg_rf_txrx_en_dbg_val      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x88)
enum
{
    FLD_RF_DCOC_GAIN_CFG                  = BIT(0),
    FLD_RF_RX_DIG_EN                      = BIT(1),
    FLD_RF_TXC_TX_EN                      = BIT(2),
    FLD_RF_TX_HPM_CAL_EN                  = BIT(3),
    FLD_RF_TX_MOD_EN                      = BIT(4),
    FLD_RF_EN_DCOC_ADC_DATA               = BIT(5),
};

#define     reg_rf_idle_txfsk_ss1_ss2_strt_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x8a)

#define     reg_rf_idle_txfsk_ss1_ss2_strt_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x8b)

#define     reg_rf_idle_txfsk_ss3_ss4_strt_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x8c)

#define     reg_rf_idle_txfsk_ss3_ss4_strt_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x8d)

#define     reg_rf_idle_txfsk_ss6_ss7_strt_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x8e)

#define     reg_rf_idle_txfsk_ss6_ss7_strt_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x8f)

#define     reg_rf_idle_rx_ss1_ss2_strt_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x90)

#define     reg_rf_idle_rx_ss1_ss2_strt_1       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x91)

#define     reg_rf_idle_rx_ss3_ss4_strt_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x92)

#define     reg_rf_idle_rx_ss3_ss4_strt_1       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x93)

#define     reg_rf_idle_rx_ss5_ss6_strt_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x94)

#define     reg_rf_idle_rx_ss5_ss6_strt_1       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x95)

#define     reg_rf_idle_txfsk_ss1_ss2_strt_cb_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x96)

#define     reg_rf_idle_txfsk_ss1_ss2_strt_cb_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x97)

#define     reg_rf_idle_txfsk_ss3_ss4_strt_cb0      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x98)

#define     reg_rf_idle_txfsk_ss3_ss4_strt_cb1      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x99)

#define     reg_rf_idle_txfsk_ss6_ss7_strt_cb_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x9a)

#define     reg_rf_idle_txfsk_ss6_ss7_strt_cb_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x9b)

#define     reg_rf_idle_rx_ss1_ss2_strt_cb_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x9c)

#define     reg_rf_idle_rx_ss1_ss2_strt_cb_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x9d)

#define     reg_rf_idle_rx_ss3_ss4_strt_cb_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x9e)

#define     reg_rf_idle_rx_ss3_ss4_strt_cb_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x9f)

#define     reg_rf_idle_rx_ss5_ss6_strt_cb_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xa0)

#define     reg_rf_idle_rx_ss5_ss6_strt_cb_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xa1)

#define     reg_rf_locd_trig        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xae)
enum
{
    FLD_RF_LOCD_TRIG                  = BIT(0),
};

#define     reg_rf_idle_txfsk_ss4_ss5_strt_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xbc)


#define     reg_rf_idle_txfsk_ss4_ss5_strt_cb_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xbd)


#define     reg_rf_fcal_rdbk_0      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc0)


#define     reg_rf_fcal_rdbk_1      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc1)
enum
{
    FLD_RF_FCAL_DEBUG_COUNT_H                 = BIT_RNG(0,4),
    FLD_RF_FCAL_PASS                  = BIT(5),
};

#define     reg_rf_fcal_dbg2_0      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc2)
enum
{
    FLD_RF_BYPASS                 = BIT(0),
    FLD_RF_DCAP_COARSE_OVERWRITE                  = BIT_RNG(1,6),
    FLD_RF_DCAP_FINE_OVERWRITE_L                  = BIT(7),
};

#define     reg_rf_fcal_dbg2_1      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc3)
enum
{
    FLD_RF_DCAP_FINE_OVERWRITE_H                  = BIT(0),
    FLD_RF_COUNT_POL                  = BIT(1),
    FLD_RF_DEBUG_CNT_SEL                  = BIT_RNG(2,5),
};

#define     reg_rf_txrx_cal_ctrl_norm0      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc4)

#define     reg_rf_txrx_cal_ctrl_norm1      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc5)
enum
{
    FLD_RF_BANDGAP_TX_RUN                 = BIT(0),
    FLD_RF_PDSTL_TX_RUN               = BIT(1),
    FLD_RF_BANDGAP_RX_RUN                 = BIT(2),
    FLD_RF_PDSTL_RX_RUN               = BIT(3),
};

#define     reg_rf_rccal_dbg1_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc6)
enum
{
    FLD_RF_RCCAL_OVERWRITE                = BIT_RNG(0,5),
    FLD_RF_CBPF_CCODE_BYPASS                  = BIT(6),
    FLD_RF_CBPF_CCODE_OVERWRITE_L                 = BIT(7),
};

#define     reg_rf_rccal_dbg1_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc7)
enum
{
    FLD_RF_CBPF_CCODE_OVERWRITE_H                 = BIT_RNG(0,5),
    FLD_RF_COMP_POL               = BIT(6),
    FLD_RF_BYPASS_RCCAL               = BIT(7),
};

#define     reg_rf_rccal_dbg2       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc8)
enum
{
    FLD_RF_RCCAL_CBPF_OFFSET                  = BIT_RNG(0,2),
    FLD_RF_RCCAL_ACCUM_INIT               = BIT_RNG(3,7),
};

#define     reg_rf_rccal_dbg3       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xc9)
enum
{
    FLD_RF_RCCAL_CBPF_CCODE_PRT               = BIT(0),
};

#define     reg_rf_rccal_rdbk_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xca)
enum
{
    FLD_RF_RCCAL_CODE                 = BIT_RNG(0,5),
    FLD_RF_CBPF_CCODE_L               = BIT_RNG(6,7),
};

#define     reg_rf_rccal_rdbk_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xcb)
enum
{
    FLD_RF_CBPF_CCODE_H               = BIT_RNG(0,4),
};

#define     reg_rf_dcoc_bypass_adc_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xce)
enum
{
    FLD_RF_DCOC_BYPASS_ADC                = BIT(0),
    FLD_RF_DCOC_IADC_OFFSET_OVERWRITE                 = BIT_RNG(1,7),
};

#define     reg_rf_dcoc_bypass_adc_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xcf)
enum
{
    FLD_RF_DCOC_QADC_OFFSET_OVERWRITE                 = BIT_RNG(0,6),
};

#define     reg_rf_dcoc_bypass_dac_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd0)
enum
{
    FLD_RF_DCOC_BYPASS_DAC                = BIT(0),
    FLD_RF_DCOC_IDAC_CODE_OVERWRITE               = BIT_RNG(1,6),
    FLD_RF_DCOC_QDAC_CODE_OVERWRITE_L                 = BIT(7),
};

#define     reg_rf_dcoc_bypass_dac_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd1)
enum
{
    FLD_RF_DCOC_QDAC_CODE_OVERWRITE_H                 = BIT_RNG(0,4),
};

#define     reg_rf_dcoc_dbg0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd2)
enum
{
    FLD_RF_DCOC_SFIQ                  = BIT(0),
    FLD_RF_DCOC_SFQI                  = BIT(1),
    FLD_RF_DCOC_IDAC_SRC_POL                  = BIT(2),
    FLD_RF_DCOC_QDAC_SRC_POL                  = BIT(3),
    FLD_RF_DCOC_SFIIP                 = BIT(4),
    FLD_RF_DCOC_SFQQP                 = BIT(5),
    FLD_RF_DCOC_SFII_L                = BIT_RNG(6,7),
};

#define     reg_rf_dcoc_dbg1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd3)
enum
{
    FLD_RF_DCOC_SFII_H                = BIT_RNG(0,1),
    FLD_RF_DCOC_SFQQ                  = BIT_RNG(2,5),
    FLD_RF_DCOC_DAC_ORDER                 = BIT(6),
};

#define     reg_rf_dcoc_lna_offset_0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd4)
enum
{
    FLD_RF_DCOC_ILNA_OFFSET               = BIT_RNG(0,5),
    FLD_RF_DCOC_QLNA_OFFSET_L                 = BIT_RNG(6,7),
};

#define     reg_rf_dcoc_lna_offset_1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd5)
enum
{
    FLD_RF_DCOC_QLNA_OFFSET_H                 = BIT_RNG(0,3),
};

#define     reg_rf_dcoc_cbpf_offset_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd6)
enum
{
    FLD_RF_DCOC_ICBPF_OFFSET                  = BIT_RNG(0,5),
    FLD_RF_DCOC_QCBPF_OFFSET_L                = BIT_RNG(6,7),
};

#define     reg_rf_dcoc_cbpf_offset_1       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd7)
enum
{
    FLD_RF_DCOC_QCBPF_OFFSET_H                = BIT_RNG(0,3),
};

#define     reg_rf_dcoc_rdbk1_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd8)
enum
{
    FLD_RF_DCOC_IDAC_CODE                 = BIT_RNG(0,5),
    FLD_RF_SPARE_3                = BIT_RNG(6,7),
};

#define     reg_rf_dcoc_rdbk1_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xd9)
enum
{
    FLD_RF_SPARE_4                = BIT_RNG(0,3),
};

#define     reg_rf_dcoc_rdbk2       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xda)
enum
{
    FLD_RF_DCOC_QDAC_CODE                 = BIT_RNG(0,5),
};

#define     reg_rf_dcoc_rdbk3_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xdc)
enum
{
    FLD_RF_DCOC_IADC_OFFSET               = BIT_RNG(0,6),
    FLD_RF_DCOC_QADC_OFFSET_L                 = BIT(7),
};

#define     reg_rf_dcoc_rdbk3_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xdd)
enum
{
    FLD_RF_DCOC_QADC_OFFSET_H                 = BIT_RNG(0,5),
};

#define     reg_rf_ldot_dbg1        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xe2)
enum
{
    FLD_RF_LDOT_LDO_CAL_BYPASS                = BIT(0),
    FLD_RF_LDOT_LDO_CAL_TRIM_OVERWRITE                = BIT_RNG(1,6),
};

#define     reg_rf_ldot_dbg2_0      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xe4)
enum
{
    FLD_RF_LDOT_LDO_RXTXHF_BYPASS                 = BIT(0),
    FLD_RF_LDOT_LDO_RXTXLF_BYPASS                 = BIT(1),
    FLD_RF_LDOT_LDO_RXTXHF_TRIM_OVERWRITE                 = BIT_RNG(2,7),
};

#define     reg_rf_ldot_dbg2_1      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xe5)
enum
{
    FLD_RF_LDOT_LDO_RXTXLF_TRIM_OVERWRITE                 = BIT_RNG(0,5),
};

#define     reg_rf_ldot_dbg3_0      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xe6)
enum
{
    FLD_RF_LDOT_LDO_PLL_BYPASS                = BIT(0),
    FLD_RF_LDOT_LDO_VCO_BYPASS                = BIT(1),
    FLD_RF_LDOT_LDO_PLL_TRIM_OVERWRITE                = BIT_RNG(2,7),
};

#define     reg_rf_ldot_dbg3_1      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xe7)
enum
{
    FLD_RF_LDOT_LDO_VCO_TRIM_OVERWRITE                = BIT_RNG(0,5),
};

#define     reg_rf_ldot_dbg4_0      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xe8)
enum
{
    FLD_RF_LDOT_INCCAL_DIS                = BIT(0),
    FLD_RF_LDOT_LDO_RXTXHF_OFFSET                 = BIT_RNG(1,2),
    FLD_RF_LDOT_LDO_RXTXLF_OFFSET                 = BIT_RNG(3,4),
    FLD_RF_LDOT_LDO_PLL_OFFSET                = BIT_RNG(5,6),
    FLD_RF_LDOT_LDO_VCO_OFFSET_L                  = BIT(7),
};

#define     reg_rf_ldot_dbg4_1      REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xe9)
enum
{
    FLD_RF_LDOT_LDO_VCO_OFFSET_H                  = BIT(0),
};

#define     reg_rf_ldot_rdbk1       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xea)
enum
{
    FLD_RF_LDOT_LDO_CAL_TRIM                  = BIT_RNG(0,5),
};

#define     reg_rf_ldot_rdbk2_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xec)
enum
{
    FLD_RF_LDOT_LDO_RXTXHF_TRIM               = BIT_RNG(0,5),
    FLD_RF_LDOT_LDO_RXTXLF_TRIM_L                 = BIT_RNG(6,7),
};

#define     reg_rf_ldot_rdbk2_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xed)
enum
{
    FLD_RF_LDOT_LDO_RXTXLF_TRIM_H                 = BIT_RNG(0,3),
};

#define     reg_rf_ldot_rdbk3_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xee)
enum
{
    FLD_RF_LDOT_LDO_PLL_TRIM                  = BIT_RNG(0,5),
    FLD_RF_LDOT_LDO_VCO_TRIM_L                = BIT_RNG(6,7),
};

#define     reg_rf_ldot_rdbk3_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xef)
enum
{
    FLD_RF_LDOT_LDO_VCO_TRIM_H                = BIT_RNG(0,3),
};

#define     reg_rf_hpmc_cfg     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xf4)
enum
{
    FLD_RF_HPMC_DAC_SWING                 = BIT(0),
    FLD_RF_HPMC_SLOW_MODE                 = BIT(1),
    FLD_RF_HPMC_COUNT_POL                 = BIT(2),
};

#define     reg_rf_hpmc_debug_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xf6)
enum
{
    FLD_RF_HPMC_BYPASS                = BIT(0),
    FLD_RF_HPMC_GAIN_OVERWRITE_L                  = BIT_RNG(1,7),
};

#define     reg_rf_hpmc_debug_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xf7)
enum
{
    FLD_RF_HPMC_GAIN_OVERWRITE_H                  = BIT_RNG(0,3),
};

#define     reg_rf_hpmc_rdbk1_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xf8)


#define     reg_rf_hpmc_rdbk1_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xf9)


#define     reg_rf_hpmc_rdbk2_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xfa)


#define     reg_rf_hpmc_rdbk2_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xfb)


#define     reg_rf_hpmc_rdbk3_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xfc)
enum
{
    FLD_RF_HPMC_COUNT1_LSW                = BIT_RNG(0,2),
    FLD_RF_HPMC_COUNT2_LSW                = BIT_RNG(3,5),
    FLD_RF_HPMC_MEAS_DIFF_COUNT_L                 = BIT_RNG(6,7),
};

#define     reg_rf_hpmc_rdbk3_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xfd)
enum
{
    FLD_RF_HPMC_MEAS_DIFF_COUNT_H                 = BIT_RNG(0,6),
    FLD_RF_HPMC_PASS                  = BIT(7),
};

#define     reg_rf_hpmc_rdbk4_0     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xfe)


#define     reg_rf_hpmc_rdbk4_1     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0xff)
enum
{
    FLD_RF_HPMC_GAIN_H                = BIT_RNG(0,2),
};

#define     reg_rf_locd_cfg_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x104)
enum
{
    FLD_RF_LOCD_DUR               = BIT_RNG(0,6),
    FLD_RF_LOCD_WAIT_L                = BIT(7),
};


#define     reg_rf_locd_cfg_0       REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x104)

#define     reg_rf_lnm_pa_ow_ctrl_val   REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x178)
enum
{
    FLD_RF_RX_LNA_PUP_OW            = BIT(0),
    FLD_RF_RX_LNA_HGAIN_OW          = BIT(1),
    FLD_RF_RX_LNA_LGAIN_OW          = BIT(2),
    FLD_RF_RX_LNA_ATTN_OW           = BIT(3),
    FLD_RF_RX_MIX_PUP_OW            = BIT(4),
    FLD_RF_TX_PA_PUP_OW             = BIT(5),
    FLD_RF_TX_PA_PWR_OW             = BIT(6),
    FLD_RF_PA_RAMP_TSEQ_OR_TX_ON    = BIT(7),
};

#define     reg_rf_pa_ow_val    REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x17c)
enum
{
    FLD_RF_TX_PA_PUP                = BIT(0),
    FLD_RF_TX_PA_PWR                = BIT_RNG(1,6),
};

#define     reg_rf_fcal_rdbk    REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x11e)
enum
{
    FLD_RF_FCAL_DCAP_COARSE               = BIT_RNG(0,5),
    FLD_RF_FCAL_DCAP_FINE                 = BIT_RNG(6,7),
};

#define     reg_rf_tx_frac_ctrl0        REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x198)
enum
{
    FLD_RF_TX_FRAC_TIME_MUX                    = BIT(0),
    FLD_RF_TXC_DSM_INT_OVERWRITE               = BIT(1),
    FLD_RF_FCAL_STL_DCAP_EN                    = BIT(2), //FCAL fast settle enable
    FLD_RF_FCAL_RVT_T1                         = BIT(3),
    FLD_RF_CH_FREQ_RVT_T1                      = BIT(4),
    FLD_RF_TX_MI_SWITCH_TONE_EN                = BIT(5),
    FLD_RF_TX_HAFM_RAMP_DOWN_EN                = BIT(6),
    FLD_RF_TX_RAMP_DOWN_TONE_EN                = BIT(7),
};

#define     reg_rf_fcal_ctrl_tx(i)   REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x1a0+i)//i range:0~7

#define     reg_rf_fcal_ctrl_rx(i)   REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x1a8+i)//i range:0~7

#define     reg_rf_fcal_chn_range_ctn(i)     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x1b0+i)//i range:0~7

#define     reg_rf_fcal_chn_range_ctf_low(i)     REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x1b8 + (i << 1))//i range:0~7

#define     reg_rf_fcal_chn_range_ctf_high(i)    REG_ADDR8(REG_TL_RADIO_BASE_ADDR+0x1b9 + (i << 1))//i range:0~7

#endif
