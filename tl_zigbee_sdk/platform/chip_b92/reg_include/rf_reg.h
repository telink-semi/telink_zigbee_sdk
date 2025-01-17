/********************************************************************************************************
 * @file    rf_reg.h
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
#ifndef RF_REG_H
#define RF_REG_H
#include "soc.h"

///*******************************      RF      ******************************/


#define CHNADDR                0x100400
#define REG_TL_RADIO_BASE_ADDR 0X170600


#define reg_rf_bb_auto_ctrl    REG_ADDR8(0x10050c)

enum
{
    FLD_RF_TX_MULTI_EN     = BIT(0),
    FLD_RF_RX_MULTI_EN     = BIT(1),
    FLD_RF_CH_0_RNUM_EN_BK = BIT(2),
    FLD_RF_CH_1_RNUM_EN_BK = BIT(3),
    FLD_RF_CH1_RX_ERR_EN   = BIT(4),
    FLD_RF_DMA_REQ_D1_EN   = BIT(5),
};

#define reg_rf_bb_tx_chn_dep  REG_ADDR8(0x1004f3)
#define reg_rf_bb_tx_size     REG_ADDR8(0x1004f0)
#define reg_rf_bb_tx_size_h   REG_ADDR8(0x1004f1)

#define reg_rf_dma_rx_wptr    REG_ADDR8(0x001004f4)
#define reg_rf_dma_rx_rptr    REG_ADDR8(0x001004f5)

#define reg_rf_dma_tx_rptr(i) REG_ADDR8(0x00100501 + (i << 1))
#define reg_rf_dma_tx_wptr(i) REG_ADDR8(0x00100500 + (i << 1))

#define reg_rf_bb_rx_size     REG_ADDR8(CHNADDR + 0xf6)
#define reg_rf_bb_rx_size_h   REG_ADDR8(CHNADDR + 0xf7)


#define reg_rf_rx_wptr_mask   REG_ADDR8(CHNADDR + 0x10d)


//******************************      RF_Register_list      ******************************/
#define REG_BASEBAND_BASE_ADDR 0x0170000


#define reg_rf_tx_mode1        REG_ADDR8(REG_BASEBAND_BASE_ADDR)

enum
{
    FLD_RF_DMA_EN       = BIT(0),
    FLD_RF_CRC_EN       = BIT(1),
    FLD_RF_TX_FMT       = BIT_RNG(2, 3),
    FLD_RF_TX_XOR_MODE  = BIT(4),
    FLD_RF_TX_TEST_DATA = BIT(5),
    FLD_RF_TX_TEST_EN   = BIT(6),
    FLD_RF_BB_DEC       = BIT(7),
};

#define reg_rf_tx_mode2 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x01)

enum
{
    FLD_RF_ZB_PN_EN      = BIT(0),
    FLD_RF_RESERVED0     = BIT_RNG(1, 2),
    FLD_RF_V_PN_EN       = BIT(3),
    FLD_RF_R_CUSTOM_MADE = BIT(4),
    FLD_RF_RESERVED1     = BIT(5),
    FLD_RF_TX_OUT_PLR    = BIT(6),
};

#define reg_rf_preamble_trail REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x02)

enum
{
    FLD_RF_PREAMBLE_LEN = BIT_RNG(0, 4),
    FLD_RF_TRAILER_LEN  = BIT_RNG(5, 7),
};

#define reg_rf_bbdbg REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x03)

enum
{
    FLD_RF_SLOW_EN     = BIT(0),
    FLD_RF_SLOW_SEL    = BIT(1),
    FLD_RF_XMODE_EN    = BIT(2),
    FLD_RF_REV_ORDER   = BIT(3),
    FLD_RF_TX_MODE     = BIT(4),
    FLD_RF_R_DBG_SEL0  = BIT(5),
    FLD_RF_MODEM1M_PAT = BIT(6),
    FLD_RF_R_DBG_SEL1  = BIT(7),
};

#define reg_rf_format REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x04)

enum
{
    FLD_RF_HEAD_MODE = BIT_RNG(0, 1),
    FLD_RF_CRC_MODE  = BIT_RNG(2, 3),
    FLD_RF_BLE_1M    = BIT(4),
    FLD_RF_BLE_WT    = BIT(5),
    FLD_RF_TX_NOACK  = BIT(6),
    FLD_RF_PN_AUTO   = BIT(7),
};

#define reg_rf_acc_len REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x05)

enum
{
    FLD_RF_ACC_LEN   = BIT_RNG(0, 2),
    FLD_RF_LR_TX_SEL = BIT(4),
    FLD_RF_BLE_LR    = BIT(5),
};

#define reg_rf_sblen REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x06)


#define reg_rf_rxchn REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x07)

enum
{
    FLD_RF_TX_ANT_SWITCH = BIT_RNG(0, 1),
    FLD_RF_RX_ANT_SWITCH = BIT(2),
    FLD_RF_TEST_TXON     = BIT(6),
    FLD_RF_TEST_TXON_EN  = BIT(7),
};

#define reg_rf_access_code REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0x08)
#define reg_rf_access_0    REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x08)
#define reg_rf_access_1    REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x09)
#define reg_rf_access_2    REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x0a)
#define reg_rf_access_3    REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x0b)
#define reg_rf_access_4    REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x0c)


#define reg_rf_pn          REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x0d)

enum
{
    FLD_RF_PN_INIT = BIT_RNG(0, 5),
};

#define reg_rf_iq_samp_num            REG_ADDR16(REG_BASEBAND_BASE_ADDR + 0x0e)
#define reg_rf_iq_samp_num0           REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x0e)
#define reg_rf_iq_samp_num1           REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x0f)

#define reg_rf_access_5               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x10)
#define reg_rf_access_6               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x11)
#define reg_rf_access_7               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x12)
#define reg_rf_access_8               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x13)
#define reg_rf_access_9               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x14)
#define reg_rf_access_10              REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x18)
#define reg_rf_access_11              REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x19)
#define reg_rf_access_12              REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x1a)
#define reg_rf_access_13              REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x1b)
#define reg_rf_access_code_base_pipe0 0x170008
#define reg_rf_access_code_base_pipe2 0x170018

#define reg_rf_txfifo                 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x1c)

enum
{
    FLD_RF_TX_FIFO_FULL = BIT(0),
    FLD_RF_TX_ACT_2D    = BIT(1),
};

#define reg_rf_aux_hl_adv   REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x1d)

#define reg_rf_tx_hlen_mode REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x1e)


#define reg_rf_rxgth1       REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x21)

enum
{
    FLD_RF_R_PILOT_LEN_O = BIT_RNG(0, 3),
    FLD_RF_R_ZB_SFD_O    = BIT(4),
    FLD_RF_R_PKTLEN_O    = BIT(5),
    FLD_RF_R_SN_LEN_O    = BIT(6),
    FLD_RF_R_LEN_0_EN_O  = BIT(7),
};

#define reg_rf_rxsfd0_num REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x22)

enum
{
    FLD_RF_RXCHN_MAN_EN = BIT(3),
    FLD_RF_RXCHN_MAN    = BIT_RNG(4, 6),
};

#define reg_rf_crc_init0 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x24)
#define reg_rf_crc_init1 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x25)
#define reg_rf_crc_init2 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x26)


#define reg_rf_ctrl_0    REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x27)

enum
{
    FLD_RF_H1M_ODD       = BIT(0),
    FLD_RF_REV_ZB_SAMP   = BIT(1),
    FLD_RF_SFD_LAST_CHIP = BIT(2),
};

#define reg_rf_rxmode REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x28)

enum
{
    FLD_RF_RX_ENABLE = BIT(0),
};

#define reg_rf_rxclk_on REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x2a)

enum
{
    FLD_RF_CLKON_O = BIT_RNG(0, 1),
};

#define reg_rf_rxclk_auto REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x2b)

enum
{
    FLD_RF_CLK_AUTO = BIT_RNG(0, 1),
};

#define reg_rf_rxdma_hs REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x2c)

enum
{
    FLD_RF_RXDMA_HS = BIT(0),
};

#define reg_rf_rxtcrcpkt REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x30)

enum
{
    FLD_RF_FILTER_CRC_PKT = BIT(0),
    FLD_RF_EN_TS_RX       = BIT(2),
    FLD_RF_EN_TS_TX       = BIT(3),
    FLD_RF_LEN_SET_O      = BIT_RNG(4, 5),
};

#define reg_rf_rxtmaxlen0 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x31)
#define reg_rf_rxtmaxlen1 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x32)

enum
{
    FLD_RF_MAX_LEN    = BIT_RNG(0, 6),
    FLD_RF_RX_ISO_PDU = BIT(7),
};

#define reg_rf_rxlatf REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x33)

enum
{
    FLD_RF_R_LATF_CNT_O    = BIT_RNG(0, 3),
    FLD_RF_R_LATF_AT_END_O = BIT(4),
    FLD_RF_R_IQ_SAMP_MODE  = BIT(5),

};

#define reg_rf_burst_size REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x34)

enum
{
    FLD_RF_DMA_BURST_SIZE = BIT_RNG(0, 1),
    FLD_RF_RX_HLEN_MODE   = BIT_RNG(2, 3),
    FLD_RF_SEL_MAN        = BIT_RNG(4, 7),
};

#define reg_rf_ant_interval REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x35)

#define reg_rf_ant_msb      REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x36)

enum
{
    FLD_RF_ANT_INTERVAL_MSB  = BIT(0),
    FLD_RF_TX_ANT_OFFSET_MSB = BIT(1),
    FLD_RF_RX_ANT_OFFSET_MSB = BIT(2),
};

#define reg_rf_rxsupp REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x38)

enum
{
    FLD_RF_RX_SUPP_D_EN = BIT(0),
    FLD_RF_RX_SUPP_A_EN = BIT(1),
    FLD_RF_TX_SUPP_D_EN = BIT(2),
    FLD_RF_TX_SUPP_A_EN = BIT(3),
    FLD_RF_ANT_NUM      = BIT_RNG(4, 7),
};

#define reg_rf_tx_antoffset REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x39)
#define reg_rf_rx_antoffset REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x3a)
#define reg_rf_samp_offset  REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x3b)


#define reg_rf_man_ant_slot REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x3c)

enum
{
    //  FLD_RF_ANT_SEL_MAN          =   BIT_RNG(0,2),
    FLD_RF_ANT_SEL_MAN_EN  = BIT(3),
    FLD_RF_SLOT_1US_MAN_EN = BIT(4),
    FLD_RF_SLOT_1US_MAN    = BIT(5),
    FLD_RF_ANT_PAT         = BIT_RNG(6, 7),
};

#define reg_rf_sof_offset REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x3d)

enum
{
    FLD_RF_ANT_SOF_OFFSET  = BIT_RNG(0, 1),
    FLD_RF_SAMP_SOF_OFFSET = BIT_RNG(2, 3),
    FLD_RF_SUPP_MODE       = BIT_RNG(4, 6),
};

#define reg_rf_mode_ctrl0 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x3e)

enum
{
    FLD_RF_INTV_MODE        = BIT_RNG(0, 1),
    FLD_RF_IQ_SAMP_EN       = BIT(2),
    FLD_RF_INFO_EXTENSION   = BIT(3),
    FLD_RF_IQ_SAMP_INTERVAL = BIT_RNG(4, 7),
};

#define reg_rf_iq_samp_start REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x3f)


#define reg_rf_dec_err       REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x40)

enum
{
    FLD_RF_SS          = BIT_RNG(0, 2),
    FLD_RF_TLK_MIC_ERR = BIT(3),
    FLD_RF_PKT_DEC_ERR = BIT_RNG(4, 7),
};

#define reg_rf_timestamp REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0x50)
#define reg_rf_tstamp0   REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x50)

enum
{
    FLD_RF_R_TSTAMP0 = BIT_RNG(0, 7),
};

#define reg_rf_tstamp1 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x51)

enum
{
    FLD_RF_R_TSTAMP1 = BIT_RNG(0, 7),
};

#define reg_rf_tstamp2 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x52)

enum
{
    FLD_RF_R_TSTAMP2 = BIT_RNG(0, 7),
};

#define reg_rf_tstamp3 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x53)

enum
{
    FLD_RF_R_TSTAMP3 = BIT_RNG(0, 7),
};

#define reg_rf_iqstart_tstamp  REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0x54)
#define reg_rf_iqstart_tstamp0 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x54)

enum
{
    FLD_RF_IQSTART_TSTAMP0 = BIT_RNG(0, 7),
};

#define reg_rf_iqstart_tstamp1 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x55)

enum
{
    FLD_RF_IQSTART_TSTAMP1 = BIT_RNG(0, 7),
};

#define reg_rf_iqstart_tstamp2 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x56)

enum
{
    FLD_RF_IQSTART_TSTAMP2 = BIT_RNG(0, 7),
};

#define reg_rf_iqstart_tstamp3 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x57)

enum
{
    FLD_RF_IQSTART_TSTAMP3 = BIT_RNG(0, 7),
};

#define reg_rf_max_match0 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x58)

enum
{
    FLD_RF_MATCH_REG_SYNC0 = BIT_RNG(0, 7),
};

#define reg_rf_max_match1 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x59)

enum
{
    FLD_RF_MATCH_REG_SYNC1 = BIT(0),
    FLD_RF_FILT_AGC_GAIN   = BIT_RNG(4, 6),
};

#define reg_rf_ant_lut_0 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x68)

enum
{
    FLD_RF_ANT_LUT0 = BIT_RNG(0, 3),
    FLD_RF_ANT_LUT1 = BIT_RNG(4, 7),
};

#if 0
    #define reg_rf_ant_lut_1 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x69)
enum{
    FLD_RF_ANT_LUT0              =  BIT_RNG(0,3),
    FLD_RF_ANT_LUT1              =  BIT_RNG(4,7),
};


    #define reg_rf_ant_lut_2 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x6a)
enum{
    FLD_RF_ANT_LUT0              =  BIT_RNG(0,3),
    FLD_RF_ANT_LUT1              =  BIT_RNG(4,7),
};


    #define reg_rf_ant_lut_3 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x6b)
enum{
    FLD_RF_ANT_LUT0              =  BIT_RNG(0,3),
    FLD_RF_ANT_LUT1              =  BIT_RNG(4,7),
};


    #define reg_rf_ant_lut_4 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x6c)
enum{
    FLD_RF_ANT_LUT0              =  BIT_RNG(0,3),
    FLD_RF_ANT_LUT1              =  BIT_RNG(4,7),
};


    #define reg_rf_ant_lut_5 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x6d)
enum{
    FLD_RF_ANT_LUT0              =  BIT_RNG(0,3),
    FLD_RF_ANT_LUT1              =  BIT_RNG(4,7),
};


    #define reg_rf_ant_lut_6 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x6e)
enum{
    FLD_RF_ANT_LUT0              =  BIT_RNG(0,3),
    FLD_RF_ANT_LUT1              =  BIT_RNG(4,7),
};


    #define reg_rf_ant_lut_7 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x6f)
enum{
    FLD_RF_ANT_LUT0              =  BIT_RNG(0,3),
    FLD_RF_ANT_LUT1              =  BIT_RNG(4,7),
};
#endif

#define reg_rf_rxdma_adr   0x170080
#define reg_rf_rxdma_fifo0 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x80)

enum
{
    FLD_RF_RXDMA_FIFO0 = BIT_RNG(0, 7),
};

#define reg_rf_rxdma_fifo1 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x81)

enum
{
    FLD_RF_RXDMA_FIFO1 = BIT_RNG(0, 7),
};

#define reg_rf_rxdma_fifo2 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x82)

enum
{
    FLD_RF_RXDMA_FIFO2 = BIT_RNG(0, 7),
};

#define reg_rf_rxdma_fifo3 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x83)

enum
{
    FLD_RF_RXDMA_FIFO3 = BIT_RNG(0, 7),
};

#define reg_rf_txdma_adr   0x170084
#define reg_rf_txdma_fifo0 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x84)

enum
{
    FLD_RF_TXDMA_FIFO0 = BIT_RNG(0, 7),
};

#define reg_rf_txdma_fifo1 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x85)

enum
{
    FLD_RF_TXDMA_FIFO1 = BIT_RNG(0, 7),
};

#define reg_rf_txdma_fifo2 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x86)

enum
{
    FLD_RF_TXDMA_FIFO2 = BIT_RNG(0, 7),
};

#define reg_rf_txdma_fifo3 REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x87)

enum
{
    FLD_RF_TXDMA_FIFO3 = BIT_RNG(0, 7),
};

#define reg_rf_hpm_pos_time_addr           0x170088
#define reg_rf_hpm_pos_time0               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x88)
#define reg_rf_hpm_pos_time1               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x89)
#define reg_rf_hpm_pos_time2               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x8a)
#define reg_rf_hpm_pos_time3               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x8b)

#define reg_rf_hpm_neg_time_addr           0x17008c
#define reg_rf_hpm_neg_time0               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x8c)
#define reg_rf_hpm_neg_time1               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x8d)
#define reg_rf_hpm_neg_time2               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x8e)
#define reg_rf_hpm_neg_time3               REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x8f)

#define reg_rf_tr_turnaround_pos_time_addr 0x170090
#define reg_rf_tr_turnaround_pos_time      REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0x90)
#define reg_rf_tr_turnaround_pos_time0     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x90)
#define reg_rf_tr_turnaround_pos_time1     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x91)
#define reg_rf_tr_turnaround_pos_time2     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x92)
#define reg_rf_tr_turnaround_pos_time3     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x93)

#define reg_rf_tr_turnaround_neg_time_addr 0x170094
#define reg_rf_tr_turnaround_neg_time      REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0x94)
#define reg_rf_tr_turnaround_neg_time0     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x94)
#define reg_rf_tr_turnaround_neg_time1     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x95)
#define reg_rf_tr_turnaround_neg_time2     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x96)
#define reg_rf_tr_turnaround_neg_time3     REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x97)

#define reg_rf_pa_pup_pos_time_addr        0x170098
#define reg_rf_pa_pup_pos_time0            REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x98)
#define reg_rf_pa_pup_pos_time1            REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x99)
#define reg_rf_pa_pup_pos_time2            REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x9a)
#define reg_rf_pa_pup_pos_time3            REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x9b)

#define reg_rf_pa_pup_neg_time_addr        0x17009c
#define reg_rf_pa_pup_neg_time0            REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x9c)
#define reg_rf_pa_pup_neg_time1            REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x9d)
#define reg_rf_pa_pup_neg_time2            REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x9e)
#define reg_rf_pa_pup_neg_time3            REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0x9f)

#define reg_rf_en_freq_comp_time_addr      0x1700a0
#define reg_rf_en_freq_comp_time0          REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0xa0)
#define reg_rf_en_freq_comp_time1          REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0xa1)
#define reg_rf_en_freq_comp_time2          REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0xa2)
#define reg_rf_en_freq_comp_time3          REG_ADDR8(REG_BASEBAND_BASE_ADDR + 0xa3)


/**
 * BB_LL
 */
#define REG_BB_LL_BASE_ADDR 0x0170200


#define reg_rf_ll_cmd       REG_ADDR8(REG_BB_LL_BASE_ADDR)

enum
{
    FLD_RF_R_CMD      = BIT_RNG(0, 3),
    FLD_RF_R_STOP     = 0,
    FLD_RF_R_BTX      = 1,
    FLD_RF_R_BRX      = 2,
    FLD_RF_R_PTX      = 3,
    FLD_RF_R_PRX      = 4,
    FLD_RF_R_STX      = 5,
    FLD_RF_R_SRX      = 6,
    FLD_RF_R_STR      = 7,
    FLD_RF_R_SRT      = 8,
    FLD_RF_R_CMD_TRIG = BIT(7),
};

#define reg_rf_ll_rest_pid REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x01)

enum
{
    FLD_RF_R_RESET_PID_0 = BIT(0),
    FLD_RF_R_RESET_PID_1 = BIT(1),
    FLD_RF_R_RESET_PID_2 = BIT(2),
    FLD_RF_R_RESET_PID_3 = BIT(3),
    FLD_RF_R_RESET_PID_4 = BIT(4),
    FLD_RF_R_RESET_PID_5 = BIT(5),
};

#define reg_rf_ll_ctrl0 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x02)

enum
{
    FLD_RF_R_MD_EN           = BIT(0),
    FLD_RF_R_ID_EN           = BIT(1),
    FLD_RF_R_RX_NOACK_MAN    = BIT(2),
    FLD_RF_R_RX_NOACK_MAN_EN = BIT(3),
    FLD_RF_R_TX_EN_MAN       = BIT(4),
    FLD_RF_R_RX_EN_MAN       = BIT(5),
    FLD_RF_R_TX_TRIQ_AUTO_EN = BIT(6),
    FLD_RF_R_S_TX_TIMEOUT_EN = BIT(7),
};

#define reg_rf_ll_ctrl_1 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x03)

enum
{
    FLD_RF_FSM_TIMEOUT_EN      = BIT(0),
    FLD_RF_RX_FIRST_TIMEOUT_EN = BIT(1),
    FLD_RF_RX_TIMEOUT_EN       = BIT(2),
    FLD_RF_CRC_2_EN            = BIT(3),

    //BLE mode
    FLD_RF_INIT_PRX_PID = BIT_RNG(4, 5),
    FLD_RF_INIT_PTX_PID = BIT_RNG(6, 7),
};

#define FSM_TIMEOUT_ENABLE  (reg_rf_ll_ctrl_1 |= FLD_RF_FSM_TIMEOUT_EN)
#define FSM_TIMEOUT_DISABLE (reg_rf_ll_ctrl_1 &= (~FLD_RF_FSM_TIMEOUT_EN))

#define reg_rf_ll_tx_stl    REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x04)

#define reg_rf_ll_tx_stl_l  REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x04)


#define reg_rf_ll_tx_stl_h  REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x05)

enum
{
    FLD_RF_R_T_TXSTL_H = BIT_RNG(0, 3),
};

#define reg_rf_ll_rxwait   REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x06)
#define reg_rf_ll_rxwait_l REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x06)


#define reg_rf_ll_rxwait_h REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x07)

enum
{
    FLD_RF_R_T_RXWAIT_H = BIT_RNG(0, 3),
};

#define reg_rf_ll_rx_l    REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x0a)

#define reg_rf_rx_timeout REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x0a)

#define reg_rf_ll_rx_h    REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x0b)

enum
{
    FLD_RF_R_T_RX_H = BIT_RNG(0, 3),
};

#define reg_rf_ll_rxstl_l REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x0c)


#define reg_rf_ll_rxstl_h REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x0d)

enum
{
    FLD_RF_R_T_RXSTL_H = BIT_RNG(0, 3),
};

#define reg_rf_ll_txwait   REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x0e)
#define reg_rf_ll_txwait_l REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x0e)


#define reg_rf_ll_txwait_h REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x0f)

enum
{
    FLD_RF_R_T_TXWAIT_H = BIT_RNG(0, 3),
};

#define reg_rf_ll_ard_l REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x10)


#define reg_rf_ll_ard_h REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x11)

enum
{
    FLD_RF_R_T_ARD_H = BIT_RNG(0, 3),
};

#define reg_rf_t_coex_t1        REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x12)
#define reg_rf_t_coex_t2        REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x13)
#define reg_rf_ll_max_retry_cnt REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x14)


#define reg_rf_ll_ctrl2         REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x15)

enum
{
    FLD_RF_R_TXCHN_MAN          = BIT_RNG(0, 2),
    FLD_RF_R_NOACK_RETRY_CNT_EN = BIT(3),
    FLD_RF_R_TXCHN_MAN_EN       = BIT(4),
    FLD_RF_R_NOACK_REV_EN       = BIT(5),
    FLD_RF_R_RXIRQ_REPORT_ALL   = BIT(6),
    FLD_RF_R_REP_SN_PID_EN      = BIT(7),
};

#define reg_rf_ll_ctrl3 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x16)

enum
{
    FLD_RF_R_TX_EN_DLY_EN    = BIT(0),
    FLD_RF_R_PLL_RESET_EN    = BIT(1),
    FLD_RF_R_CMD_SCHEDULE_EN = BIT(2),
    FLD_RF_R_PLL_EN_MAN      = BIT(3),
    FLD_RF_R_T_TX_EN_DLY     = BIT_RNG(4, 7),
};

#define reg_rf_ll_pll_reset     REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x17)
#define reg_rf_ll_cmd_schedule  REG_ADDR32(REG_BB_LL_BASE_ADDR + 0x18)
#define reg_rf_ll_cmd_schedule0 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x18)
#define reg_rf_ll_cmd_schedule1 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x19)
#define reg_rf_ll_cmd_schedule2 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x1a)
#define reg_rf_ll_cmd_schedule3 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x1b)
#define reg_rf_ll_irq_mask_l    REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x1c)
#define reg_rf_ll_irq_mask_h    REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x1d)
#define reg_rf_irq_mask         REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x1c)


#define reg_rf_ll_tx_id         REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x1e)

enum
{
    FLD_RF_R_TX_ID       = BIT_RNG(0, 6),
    FLD_RF_R_LL_IRQ_MASK = BIT(7),
};

#define reg_rf_ll_tx_committed REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x1f)

enum
{
    FLD_RF_R_TX_COMMITTED = BIT_RNG(0, 5),
};

#define reg_rf_irq_status REG_ADDR16(REG_BB_LL_BASE_ADDR + 0x20)

typedef enum
{
    FLD_RF_IRQ_RX            = BIT(0),
    FLD_RF_IRQ_TX            = BIT(1),
    FLD_RF_IRQ_RX_TIMEOUT    = BIT(2),
    FLD_RF_IRQ_RX_FIFO_FULL  = BIT(3),
    FLD_RF_IRQ_RX_CRC_2      = BIT(4),
    FLD_RF_IRQ_CMD_DONE      = BIT(5),
    FLD_RF_IRQ_FSM_TIMEOUT   = BIT(6),
    FLD_RF_IRQ_TX_RETRYCNT   = BIT(7),
    FLD_RF_IRQ_TX_DS         = BIT(8),
    FLD_RF_IRQ_RX_DR         = BIT(9),
    FLD_RF_IRQ_FIRST_TIMEOUT = BIT(10),
    FLD_RF_IRQ_INVALID_PID   = BIT(11),
    FLD_RF_IRQ_STX_TIMEOUT   = BIT(12),
    FLD_RF_IRQ_WIFI_DENY     = BIT(13),
    FLD_RF_IRQ_SUPP_OF       = BIT(14),
    FLD_RF_IRQ_RXDMA_OF      = BIT(15),
    FLD_RF_IRQ_ALL           = 0XFFFF,
} rf_irq_e;

#define reg_rf_ll_pid_l REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x22)

enum
{
    FLD_RF_PID0 = BIT_RNG(0, 1),
    FLD_RF_PID1 = BIT_RNG(2, 3),
    FLD_RF_PID2 = BIT_RNG(4, 5),
    FLD_RF_PID3 = BIT_RNG(6, 7),
};

#define reg_rf_ll_pid_h REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x23)

enum
{
    FLD_RF_PID4           = BIT_RNG(0, 1),
    FLD_RF_PID5           = BIT_RNG(2, 3),
    FLD_RF_NESN           = BIT(4),
    FLD_RF_TURNAROUND_IRQ = BIT(7),
};

#define reg_rf_ll_2d_sclk REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x24)

typedef enum
{
    FLD_RF_STATE_MACHINE_IDLE = 0,  /**< idle */
    FLD_RF_STATE_MACHINE_ACTIVE,    /**< active */
    FLD_RF_STATE_MACHINE_TX_SETTLE, /**< tx settle*/
    FLD_RF_STATE_MACHINE_TX,        /**< tx */
    FLD_RF_STATE_MACHINE_RX_WAIT,   /**< rx wait */
    FLD_RF_STATE_MACHINE_RX,        /**< rx */
    FLD_RF_STATE_MACHINE_TX_WAIT,   /**< tx wait */
} state_machine_status_e;

#define reg_rf_ll_retry_cnt REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x25)

enum
{
    FLD_RF_LL_RETRY_CNT = BIT_RNG(0, 7),
};

#define reg_rf_ll_cnt0 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x26)

enum
{
    FLD_RF_CRC_CNT = BIT_RNG(0, 3),
    FLD_RF_NAK_CNT = BIT_RNG(4, 7),
};

#define reg_rf_ll_cnt1 REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x27)

enum
{
    FLD_RF_OLD_CNT = BIT_RNG(0, 3),
    FLD_RF_ID_CNT  = BIT_RNG(4, 7),
};

#define reg_rf_ll_rx_fst_timeout REG_ADDR32(REG_BB_LL_BASE_ADDR + 0x28)
#define reg_rf_ll_rx_fst_l       REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x28)
#define reg_rf_ll_rx_fst_m       REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x29)
#define reg_rf_ll_rx_fst_h       REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x2a)

#define reg_rf_ll_fsm_timeout    REG_ADDR32(REG_BB_LL_BASE_ADDR + 0x2c)
#define reg_rf_ll_fsm_timeout_l  REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x2c)
#define reg_rf_ll_fsm_timeout_m  REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x2d)
#define reg_rf_ll_fsm_timeout_h  REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x2e)

#define reg_rf_fsm_timeout       REG_ADDR32(0x80140a2c)

#define reg_rf_coex_enable       REG_ADDR8(REG_BB_LL_BASE_ADDR + 0x30)

enum
{
    FLD_RF_COEX_EN        = BIT(0),
    FLD_RF_COEX_WF_DN_POL = BIT(1),
    FLD_RF_COEX_STATUS    = BIT(2),
    FLD_RF_COEX_TRX_POL   = BIT(3),
    FLD_RF_TRX_PRIO       = BIT(4),
    FLD_RF_TX_PRIO        = BIT(5),
    FLD_RF_RX_PRIO        = BIT(6),
};

#define CLEAR_ALL_RFIRQ_STATUS (reg_rf_irq_status = 0xffff)
//#define     REG_TL_MODEM_BASE_ADDR                0x140c00//140c00//170200
#define REG_TL_MODEM_BASE_ADDR      0x170400
#define reg_rf_modem_mode_cfg_rx1_0 REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x20)

enum
{
    FLD_RF_LR_MODE          = BIT(0),
    FLD_RF_BT_BLE_SEL_AUTO  = BIT(1),
    FLD_RF_BT_BLE_SEL_EN_RX = BIT(2),
    FLD_RF_CONT_MODE        = BIT(3),
    FLD_RF_NTL_CV           = BIT(4),
    FLD_RF_RX_DATA_CLK_DBG  = BIT(5),
    FLD_RF_LR_TRIG_MODE     = BIT(6),
    FLD_RF_FDC_DBG_SEL      = BIT(7),

};

#define reg_rf_modem_mode_cfg_rx1_1 REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x21)

enum
{
    FLD_RF_ZIGBEE_MODE_RX = BIT(0),
    FLD_RF_BT_MASK_EN     = BIT(1),
};

#define reg_rf_modem_mode_ctrl_tx1_0 REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x22)

enum
{
    FLD_RF_BLE_MODEM_TX = BIT(0),
};

#define reg_rf_modem_mode_ctrl_tx1_1 REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x23)

enum
{
    FLD_RF_ZIGBEE_MODEM_TX = BIT(7),
};

#define reg_rf_modem_mode_cfg_txrx_0 REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x26)

enum
{
    FLD_RF_X1_CQ_N      = BIT(0),
    FLD_RF_SYNC_RST_SEL = BIT(1),
    FLD_RF_GF_BT        = BIT(2),
};

#define reg_rf_modem_rxc_mi_flex_ble_0 REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x48)

#define reg_rf_modem_rxc_mi_flex_ble_1 REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x49)

#define reg_rf_modem_rx_ctrl_0         REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x4c)

enum
{
    FLD_RF_RX_ACC_LNE = BIT_RNG(0, 2),
    FLD_RF_RX_CRC_EN  = BIT(3),
    FLD_RF_SFD0_NUM   = BIT_RNG(4, 6),
};

#define reg_rf_modem_rx_ctrl_1 REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x4d)

enum
{
    FLD_RF_RX_EN      = BIT_RNG(0, 5),
    FLD_RF_LR_MAN_EN  = BIT(6),
    FLD_RF_LR_MAN_SEL = BIT(7),
};

#define reg_rf_modem_sync_thres_ble REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x4e)
#define reg_rf_modem_fdc_dbg_lat    REG_ADDR16(REG_TL_MODEM_BASE_ADDR + 0x58)
#define reg_rf_modem_gain_lat0      REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x5c)

#define reg_rf_agc_rssi_lat         REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x5d)

#define ref_rf_tim_align_1          REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x79)
#define reg_rf_bypass_filt_1        REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x7b)

#define reg_rf_tx_tl_ctrl           REG_ADDR8(REG_TL_MODEM_BASE_ADDR + 0x9a)

enum
{
    FLD_RF_TX_TP_EN        = BIT(0),
    FLD_RF_TX_IQ_EN        = BIT(1),
    FLD_RF_TX_MPSK_EN      = BIT(2),
    FLD_RF_TX_TP_ALIGN_BT  = BIT(3),
    FLD_RF_TX_TP_ALIGN_BLE = BIT(4),
};

#define reg_rf_mode_cfg_rx1_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x20)

enum
{
    FLD_RF_BW_CODE = BIT_RNG(1, 3),
    FLD_RF_SC_CODE = BIT(4),
};

#define reg_rf_mode_cfg_rx1_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x21)

enum
{
    FLD_RF_MODE_VANT_RX = BIT(1),
    FLD_RF_FE_RTRIM_RX  = BIT_RNG(2, 4),
    FLD_RF_IF_FREQ      = BIT_RNG(5, 6),

};

#define reg_rf_mode_cfg_tx1_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x22)

enum
{
    FLD_RF_BLE_MODE_TX           = BIT(0),
    FLD_RF_VCO_TRIM_KVM          = BIT_RNG(1, 3),
    FLD_RF_HPMC_EXP_DIFF_COUNT_L = BIT_RNG(4, 7),
};

#define reg_rf_mode_cfg_tx1_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x23)

enum
{
    FLD_RF_HPMC_EXP_DIFF_COUNT_H = BIT_RNG(0, 4),
    FLD_RF_DAC_TRIM_CFBK         = BIT_RNG(5, 6),

};

#define reg_rf_mode_cfg_txrx_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x26)

enum
{
    FLD_RF_DIS_CLK_DIG_O    = BIT(0),
    FLD_RF_VANT_PULLDN      = BIT(1),
    FLD_RF_TX_IQ_MODE_EN_BT = BIT(2),
    FLD_RF_LDO_ANT_TRIM     = BIT_RNG(3, 5),
    FLD_RF_CBPF_TYPE        = BIT(6),
    FLD_RF_TX_PA_PWR_L      = BIT(7),
};

#define reg_rf_mode_cfg_txrx_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x27)

enum
{
    FLD_RF_TX_PA_PWR_H = BIT_RNG(0, 4),
};

#define reg_rf_burst_cfg_txrx_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x28)

enum
{
    FLD_RF_CHNL_NUM = BIT_RNG(0, 7),
};

#define reg_rf_burst_cfg_txrx_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x29)

enum
{
    FLD_RF_CH_NUMLL_SEL        = BIT(0),
    FLD_RF_TX_EN_PIF           = BIT(1),
    FLD_RF_RX_EN_PIF           = BIT(2),
    FLD_RF_RX_TIM_SRQ_SEL_TESQ = BIT(3),
    FLD_RF_TX_TIM_SRQ_SEL_TESQ = BIT(4),
    FLD_RF_FE_CTRIM            = BIT_RNG(5, 7),
};

#define reg_rf_mode_cfg_tx3_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x3c)

enum
{
    FLD_RF_MODE_CFG_TX3      = BIT_RNG(0, 5),
    FLD_RF_MODE_VANT_TX_BLE  = BIT(6),
    FLD_RF_TX_IQ_MODE_EN_BLE = BIT(7),
};

#define reg_rf_mode_cfg_tx3_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x3d)

enum
{
    FLD_RF_LDO_ANT_TRIM_BLE = BIT_RNG(0, 2),
    FLD_RF_BT_BLE_SEL_EN    = BIT(3),
    FLD_RF_TXC_PWR_SRL      = BIT(4),
    FLD_RF_BW_CODE_BLE      = BIT_RNG(5, 7),

};

#define reg_rf_radio_mode_cfg_rx2_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x3e)

enum
{
    FLD_RF_TXC_MI_FLEX_L = BIT_RNG(0, 7),
};

#define reg_rf_radio_mode_cfg_rx2_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x3f)

enum
{
    FLD_RF_TXC_MI_FLEX_H        = BIT(0),
    FLD_RF_LOW_RATE_EN          = BIT(1),
    FLD_RF_LOW_RATE_MODE        = BIT(2),
    FLD_RF_TX_DATA_RS           = BIT_RNG(3, 4),
    FLD_RF_NORM_PKT_FAST_STL_EN = BIT(5),
    FLD_RF_NON_ID_FAST_STL_EN   = BIT(6),
    FLD_RF_FAST_STL_EN          = BIT(7),
};

#define reg_rf_radio_txrx_dbg1_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x40)

enum
{
    FLD_RF_AGC_DISABLE    = BIT(1),
    FLD_RF_RX_GAIN        = BIT_RNG(2, 4),
    FLD_RF_ADC_IQ_SWAP    = BIT(5),
    FLD_RF_INVERT_CLK_DSM = BIT(6),
    FLD_RF_DSM_FRAC_LSW_L = BIT(7),
};

#define reg_rf_radio_txrx_dbg1_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x41)

enum
{
    FLD_RF_DSM_FRAC_LSW_H  = BIT_RNG(0, 1),
    FLD_RF_SPARE_1         = BIT(2),
    FLD_RF_AM_OVERRIDE_EN  = BIT(3),
    FLD_RF_AM_OVERRIDE_VAL = BIT(4),
    FLD_RF_LOAD_PREV_GAIN  = BIT(5),
    FLD_RF_ADC_CLIP_EN     = BIT(6),
};

#define reg_rf_txrx_dbg3_0 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x44)

enum
{
    FLD_RF_CHNL_FREQ_DIRECT = BIT(0),
    FLD_RF_CHNL_FREQ_L      = BIT_RNG(1, 7),
};

#define reg_rf_txrx_dbg3_1 REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x45)

enum
{
    FLD_RF_CHNL_FREQ_H        = BIT_RNG(0, 5),
    FLD_RF_DSN_DITHER_DISABLE = BIT(6),
    FLD_RF_DSM_INT_MODE       = BIT(7),
};

#define reg_rf_lnm_pa_ow_ctrl_val REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x178)

enum
{
    FLD_RF_RX_LNA_PUP_OW         = BIT(0),
    FLD_RF_RX_LNA_HGAIN_OW       = BIT(1),
    FLD_RF_RX_LNA_LGAIN_OW       = BIT(2),
    FLD_RF_RX_LNA_ATTN_OW        = BIT(3),
    FLD_RF_RX_MIX_PUP_OW         = BIT(4),
    FLD_RF_TX_PA_PUP_OW          = BIT(5),
    FLD_RF_TX_PA_PWR_OW          = BIT(6),
    FLD_RF_PA_RAMP_TSEQ_OR_TX_ON = BIT(7),
};

#define reg_rf_pa_ow_val REG_ADDR8(REG_TL_RADIO_BASE_ADDR + 0x17c)

enum
{
    FLD_RF_TX_PA_PUP = BIT(0),
    FLD_RF_TX_PA_PWR = BIT_RNG(1, 6),
};

#endif
