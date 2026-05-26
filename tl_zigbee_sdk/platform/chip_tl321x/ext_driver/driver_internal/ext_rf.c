/********************************************************************************************************
 * @file    ext_rf.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "ext_lib.h"
//#include "stack/ble/ble_multi/controller/ble_controller.h"

volatile unsigned int TXADDR = 0xc0013000;

extern unsigned char        s_dcoc_software_cal_en;
extern unsigned short       g_rf_dcoc_iq_code;
extern void rf_rx_dcoc_cali_by_sw(unsigned char call);
extern void rf_set_dcoc_iq_code(unsigned short iq_code);

#define   BLE_TXDMA_DATA        (0x170000 + 0x84)      //0x170084
#define   BLE_RXDMA_DATA        (0x170000 + 0x80)      //0x170080

//extern dma_config_t rf_tx_dma_config;
//extern dma_config_t rf_rx_dma_config;

_attribute_data_retention_sec_ signed char ble_txPowerLevel = 0; /* <<TX Power Level>>: -127 to +127 dBm */

//RF BLE Minimum TX Power LVL (unit: 1dBm)
const char  ble_rf_min_tx_pwr   = -23; /* -23dBm */
//RF BLE Maximum TX Power LVL (unit: 1dBm)
const char  ble_rf_max_tx_pwr   = 9;   /*  +9dBm */
//RF BLE Current TX Path Compensation (s16: -1280 ~ 1280, unit: 0.1 dB)
_attribute_data_retention_  signed short ble_rf_tx_path_comp = 0;
//RF BLE Current RX Path Compensation (s16: -1280 ~ 1280, unit: 0.1 dB)
_attribute_data_retention_  signed short ble_rf_rx_path_comp = 0;

//Current RF RX DMA buffer point for BLE
_attribute_data_retention_ _attribute_aligned_(4) unsigned char *ble_curr_rx_dma_buff = NULL;

_attribute_data_retention_sec_ ext_rf_t blt_extRF;

void ble_rf_set_tx_dma(unsigned char fifo_dep,unsigned char size_div_16)
{
    unsigned short fifo_byte_size = size_div_16<<4;
//
//  reg_rf_bb_auto_ctrl |= (FLD_RF_TX_MULTI_EN | FLD_RF_CH_0_RNUM_EN_BK);
//  reg_rf_bb_tx_chn_dep = fifo_dep;
//
//  reg_rf_bb_tx_size   = fifo_byte_size&0xff;
//  reg_rf_bb_tx_size_h = fifo_byte_size>>8;
//
//  dma_config(DMA0,&rf_tx_dma_config);//solve dma_chn_dis(DMA0) cause read_num_en reset to 0
//  dma_set_address(DMA0, TXADDR, BLE_TXDMA_DATA);   // TXADDR=0xc0013000;
    rf_set_tx_dma_config();
    rf_set_tx_dma_fifo_num(fifo_dep);
    rf_set_tx_dma_fifo_size(fifo_byte_size);
}

_attribute_ram_code_
void ble_rf_set_rx_dma(unsigned char *buff, unsigned char size_div_16)
{

//  unsigned short fifo_byte_size = size_div_16<<4;
//  ble_curr_rx_dma_buff = buff;
//  buff +=4;
//
//  reg_rf_bb_auto_ctrl |= (FLD_RF_RX_MULTI_EN | FLD_RF_CH_0_RNUM_EN_BK);//ch0_rnum_en_bk,tx_multi_en,rx_multi_en
//
//  //TODO: check with Qiangkai
//  reg_rf_rx_wptr_mask = 0; //rx_wptr_real=rx_wptr & mask:After receiving 4 packets,the address returns to original address.mask value must in (0x01,0x03,0x07,0x0f)
//  reg_rf_bb_rx_size = fifo_byte_size&0xff;//rx_idx_addr = {rx_wptr*bb_rx_size,4'h0}// in this setting the max data in one dma buffer is 0x20<<4.
//  reg_rf_bb_rx_size_h = fifo_byte_size>>8;
//  dma_set_address(DMA1, BLE_RXDMA_DATA, (u32)convert_ram_addr_cpu2bus(buff));
//  reg_dma_size(DMA1)=0xffffffff;

    unsigned short fifo_byte_size = size_div_16<<4;
    rf_set_rx_dma_config();
    ble_curr_rx_dma_buff = buff;
    rf_set_rx_buffer(buff);
    reg_rf_rx_wptr_mask = 0;
    rf_set_rx_dma_fifo_size(fifo_byte_size);
}

_attribute_ram_code_ void ble_rx_dma_config(void){
//  dma_config(DMA1,&rf_rx_dma_config);
    rf_set_rx_dma_config();
}

void rf_drv_dcoc_cali_sw(void)
{
#if (RF_RX_DCOC_SOFTWARE_CAL_EN)
    if (s_dcoc_software_cal_en == 1) {
        //Solve the problem of unstable rx sensitivity test of some chips by software dcoc calibration scheme. If the calibration value is
        //not lost after a calibration is completed, it can be used directly without recalibration. Since the _attribute_data_retention_sec_ type
        //variable is not lost in suspend and deep retention modes, it can be used to record the calibration value to avoid having to perform
        //software calibration again after returning from suspend and deep retention modes.(Modified by zhiwei,confirmed by xuqiang and yuya at 20230921.)
        if (g_rf_dcoc_iq_code == 0) //After calibration is completed, it is impossible for the value of g_rf_dcoc_iq_code to be 0.
        {
            rf_rx_dcoc_cali_by_sw(1);
        } else {
            rf_set_dcoc_iq_code(g_rf_dcoc_iq_code);
        }
    }
#endif
}

void rf_drv_tx_hpmc_compensate(void)
{
//used by zigbee-ble dual-mode
#if(RF_TX_HPMC_COMPENSATE_EN == 1)
	extern rf_fast_settle_t g_hpmc_val;
	extern void rf_tx_hpmc_compensation(void);
	if(g_hpmc_val.cal_tbl[4] == 0)
	{
		rf_tx_hpmc_compensation();
	}
	else
	{
		reg_rf_hpmc_debug_0 |= FLD_RF_HPMC_BYPASS;//hpmc bypass enable, a temporary solution, which should be solved later
	}
#endif
}

//#if BLC_PM_EN
_attribute_ram_code_
//#endif
void rf_drv_ble_init(void){

#if (PRMBL_LENGTH_1M < 1 || PRMBL_LENGTH_1M > 15)
    #error "1M PHY pream_ble error!!!"
#endif


#if (PRMBL_LENGTH_2M < 2 || PRMBL_LENGTH_1M > 15)
    #error "2M PHY pream_ble error!!!"
#endif

#if 0//here is driver code
    rf_mode_init();
    rf_ble_set_1m_phy();
#else
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   2. merge from driver function "rf_mode_init"
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    rf_mode_init();
    //    pm_set_dig_module_power_switch(FLD_PD_ZB_EN, PM_POWER_UP);

    reg_rst4 |= FLD_RST4_ZB;
    reg_clk_en4 |= FLD_CLK4_ZB_EN;

    reg_n22_rst     = 0xffc;                         //reset dma_bb,zb,rst_modem,rstl_bb(bb:baseband)
    reg_n22_clk_en0 = 0xff;                          //enable dma_bb,zb_hclk
    reg_rf_tstimp_ctrl |= FLD_RF_R_STIMER_REVERT_EN; //Switching RF clock to stimer.

    //one_time_setup
    write_reg8(0x1706d2, 0x9b); //DCOC_SFIIP:bit<4> DCOC_SFQQP:bit<5> DCOC_SFII_L:bit<6-7>
    write_reg8(0x1706d3, 0x19); //DCOC_SFII_H:bit<0-1> DCOC_SFQQ:bit<2-5>
#if RF_RX_SHORT_MODE_EN
    write_reg8(0x17047b, 0x0e); //BLANK_WINDOW
    write_reg8(0x170479, 0x38); //BIT[3] RX_DIS_PDET_BLANK.BIT_RNG[4,5]SHORT MODE all mode open pdet blank to fix
                                //per floor issue.modified by zhiwei,confirmed by qiangkai and xuqiang.20221205
#else
    write_reg8(0x17047b, 0xfe); //BLANK_WINDOW
    write_reg8(0x170479, 0x08); //RX_DIS_PDET_BLANK.BIT_RNG[4,5]SHORT MODE all mode open pdet blank to fix per floor
                                //issue.modified by zhiwei,confirmed by qiangkai and xuqiang.20221205
#endif

    //To set AGC thresholds
    write_reg8(0x17064a, 0x0e); //POW_000_001:bit<0-6> POW_001_010_L:bit<7>
    write_reg8(0x17064b, 0x09); //POW_001_010_H:bit<0-5>
    write_reg8(0x17064e, 0x09); //POW_100_101:bit<0-6> POW_101_100_L:bit<7>
    write_reg8(0x17064f, 0x0f); //POW_101_100_H:bit<0-5>
    write_reg8(0x170654, 0x0e); //POW_000_001:bit<0-6> POW_001_010_L:bit<7>
    write_reg8(0x170655, 0x09); //POW_001_010_H:bit<0-5>
    write_reg8(0x170656, 0x0c); //POW_010_011:bit<0-6> POW_011_100_L:bit<7>
    write_reg8(0x170657, 0x08); //POW_011_100_H:bit<0-5>
    write_reg8(0x170658, 0x09); //POW_100_101:bit<0-6> POW_101_100_L:bit<7>
    write_reg8(0x170659, 0x0f); //POW_101_100_H:bit<0-5>

    //For optimum preamble detection
    write_reg8(0x170476, 0x50);      //RX_PE_DET_MIN_LO_THRESH
    write_reg8(0x170477, 0x73);      //RX_PE_DET_MIN_HI_THRESH

    rf_clr_irq_mask(FLD_RF_IRQ_ALL); //The default interrupt mask in RF is open.
    //Close the interrupt mask in the initialization code and reopen it when in use

    reg_rf_ll_ctrl3 &= ~(FLD_RF_R_TX_EN_DLY_EN); //Turn off the extension tx_en function

    /*
    *         bit                        default    value                note
    *                                                             note
    * ---------------------------------------------------------------------------
    * <1:0>:lna_itrim           default:0,->3(4.4u->6.2u)    Increase lna_itrim current to boost RX performance.
    * <5:4>:pa_vbias           default:1,->0(515mv->535mv)    Raise the pa_vbais voltage to boost transmit power level.
    * This setting is used for A0 to improve performance, pending A1 hardware to fix performance issues; this setting
    * will be canceled out to reduce power consumption. modified by zhiwei.wang,confirmed by wenfeng.lou 24020531.
    */
    // write_reg8(0x17074c,0x03);
    /*
    *         bit                        default    value                note
    *                                                             note
    * ---------------------------------------------------------------------------
    * <1:0>:cbpf_trim_i                default:0,->3(5.00u->8.75u)    Increasing the I-way trim current of cbpf to improve rx performance.
    * <3:2>:cbpf_trim_q                   default:0,->3(5.00u->8.75u)    Increasing the Q-way trim current of cbpf to improve rx performance.
    * <6:5>:cbpf_trim_short_dcbias    default:2,->1(460mv->490mv)    Increase cbpf_trim_short_dcbias voltage to boost RX performance.
    * <7:7>:cbpf_vcm_trim_l            default:1,->0(490mv->520mv) Increase cbpf_vcm_trim_l voltage to boost RX performance.
    * This setting is used for A0 to improve performance, pending A1 hardware to fix performance issues; this setting will be canceled
    * out to reduce power consumption. modified by zhiwei.wang,confirmed by wenfeng.lou 24020531.
    */
    write_reg8(0x17074e, 0x4f);

    /*
    *         bit                        default    value                note
    *                                                             note
    * ---------------------------------------------------------------------------
    * <2:0>:VCO_TRIM_KVT                default:0x7  Adjustment of Kv of vctrl path depending upon reference frequency. Default should change depending upon if the reference frequency is 24MHz or 32MHz
    * <3>  :VCO_EN_PKDET                default:0    Enable peak detector operation
    * <5:4>:LDOTRIM_TRIM_VREF           default:2,->0->3->0(0.946V->0.901V->0.879V->0.901V) Bump bits for the 900 mV LDOTRIM reference voltage.
    * This setting is used to optimize RX performance by changing the LDO trimming reference voltage from 0.901 to 0.879 and then LDO output to 1.0v.
    * modified by chenxi.wang,confirmed by wenfeng.lou 20241223.
    * Set the LDOTRIM_TRIM_VREF configuration to 0 (0.901V) to resolve RF transmit/receive frequency lock loss issues in certain non-conforming chips.
    * This modification will increase RF power consumption.
    * modified by chenxi.wang,confirmed by wenfeng.lou 20250910.(CCN-378)
    */
    write_reg8(0x170754, 0x07);
    /*
    *         bit                        default    value                note
    *                                                             note
    * ---------------------------------------------------------------------------
    * <0>:LDO_PLL_BOOST           default:0 To boost the LDO output voltage to close to 1.5V with trim code being maximum
    * <1>:LDO_VCO_BYPASS          default:0 Bypass the LDO output to Vline
    * <2>:LDO_VCO_BOOST           default:0 To boost the LDO output voltage to close to 1.5V with trim code being maximum
    * <3>:LDO_CAL_BYPASS          default:0 Bypass the LDO output to Vline
    * <4>:LDO_CAL_BOOST           default:0 To boost the LDO output voltage to close to 1.5V with trim code being maximum
    * <5>:LDO_ANT_BYPASS          default:0,->1 Bypass the LDO output to Vline
    * This setting is used for A1 to obtain higher output power under vant mode.modified by chenxi.wang,confirmed by wenfeng.lou 20240820.
    */
    write_reg8(0x170741, 0x20);

    /*
    *         bit                        default    value                note
    *                                                             note
    * ---------------------------------------------------------------------------
    * <0>:LNA_HP_DIG                 default:1 LNA high power high performance control: 0:normal mode;1:high power low noise mode
    * <3:1>:TX_ATTN_TRIM_DIG         default:4 Tx lo attenuation mode trim
    * <4>:TX_ATTN_SEL_DIG            default:0 Tx lo attenuation enable:0(default): tx lo normal mode; 1:tx lo attenuation mode
    * <7:5>:TX_BUF_TRIM_DIG          default:4,->0 Tx lo buffer vbias trim to adjust the duty cycle
    * This setting is used for A1 to optimise power consumption.modified by chenxi.wang,confirmed by wenfeng.lou 20240820.
    * <7:5>:TX_BUF_TRIM_DIG          default:4,->0,->1. This setting is to fix the HPMC gain value jitter issue. modified by kun.he,confirmed by siming.leng 20250409.
    */
    write_reg8(0x170638, 0x29);
    /*
     *         bit                        default    value                note
     * ---------------------------------------------------------------------------
     * <1:0>:PA_RAMP_MODE           default:01,->03 Increment PA slices to programmed value from 0 using delay of 24M between each step.
     *                              (1 - 2 - 4 - 8 -16 - 32 - 48-63)
     * <4:2>:EXT_PA_EN_ASSERT_DLY   default:02 delay of ext_pa_en signal going high.0 to 3.5us in steps of 0.5us.
     * <7:5>:EXT_PA_EN_DEASSERT_DLY default:02 delay of ext_pa_en signal going low.0 to 3.5us in steps of 0.5us.
     * This setting is set to 0x4b to improve the bandedge characteristics.modified by chenxi.wang,confirmed by wenfeng.lou 20250110.
     * (0x4b->0x4a)This setting is set to 0x4a to fix the issue of exceeding the initial frequency offset in S2/S8/BLE2M mode. modified by kun.he,confirmed by wenfeng.lou 20250414.
     */
    write_reg8(0x170624, 0x4a);

    /*
    * This configuration is used for A1 to improve the performance of rf rx sensitivity.
    * Defaults to RX_LOW_POWER for A1.
    * If you need higher performance, you need to call rf_rx_performance_mode() after rf_mode_init;
    * Select the RX_HIGH_PERFORMANCE mode, in which the RX sensitivity is increased by 1dBm, but the receiving power consumption will increase
    * (modified by chenxi.wang,confirmed by wenfeng.lou 20240826.)
    */
    rf_rx_performance_mode(RF_RX_LOW_POWER);

    #if (RF_RX_DCOC_SOFTWARE_CAL_EN)
        if (s_dcoc_software_cal_en == 1) {
            //Solve the problem of unstable rx sensitivity test of some chips by software dcoc calibration scheme. If the calibration value is
            //not lost after a calibration is completed, it can be used directly without recalibration. Since the _attribute_data_retention_sec_ type
            //variable is not lost in suspend and deep retention modes, it can be used to record the calibration value to avoid having to perform
            //software calibration again after returning from suspend and deep retention modes.(Modified by zhiwei,confirmed by xuqiang and yuya at 20230921.)
            if (g_rf_dcoc_iq_code == 0) //After calibration is completed, it is impossible for the value of g_rf_dcoc_iq_code to be 0.
            {
            	/*For zb_ble dual-mode mode, RF to BLE mode will be switched in STIMER interrupt, so we need to put all this function
            	 * into the RAM code. And DCOC will be implemented at initialization, no need here. */
//                rf_rx_dcoc_cali_by_sw(1);
            } else {
                rf_set_dcoc_iq_code(g_rf_dcoc_iq_code);
            }
        }
    #endif
	/*
	 *         bit                        default    value                note
	 * ---------------------------------------------------------------------------
	 * <7>:PA_RAMP_TSEQ_OR_TX_ON_SEL      default:0,->1 bit to select between tx on or pa ramp from timing sequence
	 * (1)This setting advances the PA ramp start time to the end of the timing sequence,
	 *    and after configuration, tx performs PA ramp up before preamble carrier.
	 * (2)Due to the PA ramp up performed by tx before preamble transmission, the settling time of tx will increase by 8us.
	 *    To adapt to this TX method, the preamble length will be reduced
	 *  Modified by chenxi.wang,confirmed by xuqiang.zhang 20250114.
	 */
	 reg_rf_lnm_pa_ow_ctrl_val |=FLD_RF_PA_RAMP_TSEQ_OR_TX_ON;

#if(RF_TX_HPMC_COMPENSATE_EN == 1)
    extern rf_fast_settle_t g_hpmc_val;
    if(g_hpmc_val.cal_tbl[4] == 0)
    {
//    	extern void rf_tx_hpmc_compensation(void);
//    	rf_tx_hpmc_compensation();
    }
    else
    {
        reg_rf_hpmc_debug_0 |= FLD_RF_HPMC_BYPASS;//hpmc bypass enable, a temporary solution, which should be solved later
    }
#endif


//    rf_set_ble_1M_mode();
    //aura_1m
    write_reg8(0x17063d, 0x61); //ble:bw_code.
    write_reg8(0x170620, 0x10); //sc_code.
    /*
    *         bit                        default    value                note
    *                                                             note
    * ---------------------------------------------------------------------------
    * <1>:MODE_VANT_RX           default:1           defines if RX is in vbat or vant mode. Default is LDO_ANT mode
    * <4:2>:FE_RTRIM_RX          default:0x02->0x03  Front end matching resistor adjustment for RX. (Configured by the rf_rx_performance_mode interface)
    * <6:5>:IF_FREQ              default:0x00(IF:1MHz,BW:1MHz) Intermediate Frequency Selection.
    * This setting is used to set the RF different modes Intermediate Frequency.
    */
    reg_rf_mode_cfg_rx1_1 = (reg_rf_mode_cfg_rx1_1 & (~FLD_RF_IF_FREQ)) | FLD_RF_MODE_VANT_RX;

    write_reg8(0x170622, 0x20); //RADIO BLE_MODE_TX,1MBPS:bit<0>;VCO_TRIM_KV:bit<1-3>;HPMC_EXP_DIFF_COUNT_L:bit<4-7>.
    write_reg8(0x170623, 0x23); //HPMC_EXP_DIFF_COUNT_H.
    write_reg8(0x170422, 0x00); //modem:BLE_MODE_TX,1MBPS.
    write_reg8(0x17044e, 0x1e); //ble sync threshold:To modem.


    write_reg8(0x17063f, 0x00); //250k modulation index:telink add rx for 250k/500k.

    //rx_cont_mode
    write_reg8(0x170420, 0xc8); // script cc. rx continue mode on:bit<3>


    write_reg8(0x17044d, 0x01); //r_rxchn_en_i:To modem.
    write_reg8(0x170421, 0x00); //modem:ZIGBEE_MODE:01
    write_reg8(0x170423, 0x00); //modem:ZIGBEE_MODE_TX.
    write_reg8(0x170426, 0x00); //modem:sync rst sel,for zigbee access code sync.
    write_reg8(0x17042a, 0x10); //modem:disable MSK.
    write_reg8(0x17043d, 0x00); //modem:zb_sfd_frm_ll.
    write_reg8(0x17042c, 0x38); //modem:zb_dis_rst_pdet_isfd.
    write_reg8(0x170436, 0xb7); //LR_NUM_GEAR_L.
    write_reg8(0x170437, 0x0e); //LR_NUM_GEAR_H.
    write_reg8(0x170438, 0xb6); //LR_TIM_EDGE_DEV.0xc4->0xb6
    write_reg8(0x170439, 0x71); //LR_TIM_REC_CFG_1.
    write_reg8(0x170473, 0x01); //TOT_DEV_RST.
    write_reg8(0x17049a, 0x00); //tx_tp_align.

    //agc_table_1m
    if ((g_chip_version == CHIP_VERSION_A0) || (g_chip_version == CHIP_VERSION_A1))
    {
        write_reg8(0x1704c2, 0x3e); //grx_0.
        write_reg8(0x1704c3, 0x4b); //grx_1.
        write_reg8(0x1704c4, 0x56); //grx_2.
        write_reg8(0x1704c5, 0x63); //grx_3.
        write_reg8(0x1704c6, 0x6e); //grx_4.
        write_reg8(0x1704c7, 0x7a); //grx_5.
    }
    else
    {
        if (RF_RX_LOW_POWER == g_rf_rx_high_performance)
        {
            write_reg8(0x1704c2, 0x3d); //grx_0.
            write_reg8(0x1704c3, 0x47); //grx_1.
            write_reg8(0x1704c4, 0x54); //grx_2.
            write_reg8(0x1704c5, 0x61); //grx_3.
            write_reg8(0x1704c6, 0x6d); //grx_4.
            write_reg8(0x1704c7, 0x79); //grx_5.
        }
        else
        {
            write_reg8(0x1704c2, 0x3d); //grx_0.
            write_reg8(0x1704c3, 0x48); //grx_1.
            write_reg8(0x1704c4, 0x57); //grx_2.
            write_reg8(0x1704c5, 0x66); //grx_3.
            write_reg8(0x1704c6, 0x71); //grx_4.
            write_reg8(0x1704c7, 0x7b); //grx_5.
        }
    }
    write_reg8(0x1704c8, 0x39); //default:0x00->0x39 Gain offset to compensate system error
    //ble1m_setup
    write_reg8(0x170000, 0x0f); //tx_mode.
    write_reg8(0x170001, 0x08); //PN.
    /*
    *       bit                 default value               note
    * ---------------------------------------------------------------------------
    * <4: 0>:preamble length     default:2
    * At present, TX adopts the method of pa_ramp starting first and preamble sending later, so the preamble adopts this length setting
    * modified by chenxi.wang,confirmed by wenfeng.lou 20250114.
    */
    write_reg8(0x170002, 0x42); //preamble len.
    write_reg8(0x170003, 0x54); //bit<0:1>private mode control.
    write_reg8(0x170004, 0xf1); //bit<4>mode:1->1m;bit<0:3>:ble head.
    write_reg8(0x170005, 0x04); //lr mode bit<4:5>

    write_reg8(0x170021, 0xa1); //rx packet len 0 enable.
                                //bit<5>:write packet length filed into sram

    write_reg8(0x170022, 0x00); //rxchn_man_en.
    write_reg8(0x17044c, 0x0c); //RX:acc_len modem.0x4c->0x0c
    write_reg8(0x1704bb, 0x00); //disable 2 stage filter
    write_reg8(0x17043e, 0x81); //BIT<7>:0 new ,1 old  pm2fm suppress more than pi/4

    //The following register configurations are configured in zigbee/hybee mode, which maintains register defaults
    write_reg8(0x170014, 0x7a); //access code for hybee 500K.
    write_reg8(0x170015, 0x35); //access code for hybee 500K.
    write_reg8(0x17043b, 0x1c); //ZB_NUM_GEAR_H
    write_reg8(0x170132, 0x01); //zigbee PHR field enable 1: phr field length embedded in data stream; 0: phr field length from reg ctrl as like private SB packet

    //The following registers are configured in BLE 125K and BLE 500K mode, which maintains the register defaults
    write_reg8(0x1704f0, 0x1c); //defaults 0x1c. lr_s8_pdet synv_success threshold 0~32
    //<7:6> to optimize zigbee performance from 00->03; other modes restore defaults.modified by zhiwei.wang,confirmed by yuya.hao 20250322.
    write_reg8(0x170450, 0x3f); //dciq edr  auto
    //<0> to optimize zigbee performance from 00->01; other modes restore defaults.modified by zhiwei.wang,confirmed by yuya.hao 20250322.
    write_reg8(0x170451, 0x0e); //edr dcoc auto

//    rf_set_crc_config(&rf_crc_config[0]);
//
//    g_rfmode = RF_MODE_BLE_1M;


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   3. setting for BLE by BLE_Team
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //access code, can save
    write_reg32(0x170008,0x00000000);   //default:0xf8118ac9;

    write_reg8(0x170030, 0x36);         //default:0x3c;disable tx timestamp en, add by LiBiao

    write_reg8(0x80170206, 0x00);       //LL_RXWAIT, default 0x0009
    write_reg8(0x8017020c, 0x50);       //LL_RXSTL   default 0x0095
    write_reg8(0x8017020e, 0x00);       //LL_TXWAIT, default 0x0009
    write_reg8(0x80170210, 0x00);       //LL_ARD,    default 0x0063

    reg_rf_modem_mode_cfg_rx1_0 &= ~FLD_RF_LR_TRIG_MODE;        //coded phy accesscode trigger mode: manual mode
#endif
}

#if RF_THREE_CHANNEL_CALIBRATION
_attribute_data_retention_ unsigned char rf_channel_power[40];
_attribute_data_retention_ unsigned char channel_power_calibration_enable = 0;

_attribute_ram_code_ //must be RamCode
void ble_rf_set_chn_power(signed char  chn_num)
{
    unsigned char value = (unsigned char)(rf_channel_power[chn_num] & 0x3F);
    reg_rf_mode_cfg_txrx_0 = ((reg_rf_mode_cfg_txrx_0 & 0x7f) | ((value&0x01)<<7));
    reg_rf_mode_cfg_txrx_1 = ((reg_rf_mode_cfg_txrx_1 & 0xe0) | ((value>>1)&0x1f));
}

/**
 *  @brief      this function serve to set the TX power calibration.
 *  @param[in]  channel_power: channel power calibration of 40 channel.
 *  @return     none
*/
void rf_set_channel_power_calibration(unsigned char *channel_power)
{
    memcpy(rf_channel_power,channel_power,40);
}

/**
 *  @brief      this function serve to enable the rx timing sequence adjusted.
 *  @param[in]  enable: channel power calibration enable or disable.
 *  @return     none
*/
void rf_set_channel_power_enable(unsigned char enable)
{
    channel_power_calibration_enable = enable;
}
#endif

/**
 * @brief       This function serves to set RF baseband channel.This function is suitable for ble open PN mode.
 * @param[in]   chn_num  - Bluetooth channel set according to Bluetooth protocol standard.
 * @return      none.
 */
_attribute_ram_code_ //must be RamCode
void rf_set_ble_channel (signed char chn_num)
{
    signed char ble_chn_num = 0;
    write_reg8 (0x170020, chn_num);

    if (chn_num < 11)
        ble_chn_num = chn_num + 2;

    else if (chn_num < 37)
        ble_chn_num = chn_num + 3;

    else if (chn_num == 37)
        ble_chn_num = 1;

    else if (chn_num == 38)
        ble_chn_num = 13;

    else if (chn_num == 39)
        ble_chn_num = 40;
#if RF_THREE_CHANNEL_CALIBRATION
    if(channel_power_calibration_enable)
    {
        ble_rf_set_chn_power(ble_chn_num - 1);
    }
#endif
    ble_chn_num = ble_chn_num << 1;
    rf_set_chn(ble_chn_num);
}

#if (SCHEDULE_USE_BB_TIMER)
_attribute_ram_code_
void rf_start_fsm (fsm_mode_e mode, void* tx_addr, unsigned int tick)
{
    unsigned int r = core_interrupt_disable(); //prevent user IRQ priority 3 task destroying FSM setting

    unsigned int cur_stimer_tick = stimer_get_tick();
    unsigned int cur_bbtimer_tick = bb_clock_time();

    unsigned int diff_tick = tick - cur_stimer_tick;
    if((unsigned int)(diff_tick - 2*SYSTEM_TIMER_TICK_1US) < BIT(30)){
        cur_bbtimer_tick = (cur_bbtimer_tick + (diff_tick/3));
    }

    reg_rf_ll_cmd_schedule = cur_bbtimer_tick;
    reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHEDULE_EN;    // Enable cmd_schedule mode.
    reg_rf_ll_cmd = mode;

    if(tx_addr){
        rf_dma_set_src_address(RF_TX_DMA,(unsigned int)(tx_addr));
    }

    core_restore_interrupt(r);
}
#else
_attribute_ram_code_
void rf_start_fsm (fsm_mode_e mode, void* tx_addr, unsigned int tick)
{
    unsigned int r = core_interrupt_disable();
//  write_reg32(0x80170218, tick);
    reg_rf_ll_cmd_schedule = tick;
    reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHEDULE_EN;    // Enable cmd_schedule mode.
    reg_rf_ll_cmd = mode;

    if(tx_addr){
        rf_dma_set_src_address(RF_TX_DMA,(unsigned int)tx_addr);
    }

//  if(mode != FSM_SRX){ //BLE SDK never use PRX mode
//      rf_fsm_tx_trigger_flag = 1;
//  }


    core_restore_interrupt(r);
}
#endif
// todo here need to optimize_Bool
_attribute_ram_code_
_Bool ll_resolvPrivateAddr(u8 *irk, u8 *addr, u8 irk_num)
{
    reg_cv_llbt_hash_status = FLD_CV_RPASE_STATUS_CLR; //clear flag
    reg_cv_llbt_rpase_cntl =(FLD_CV_PRASE_ENABLE) ; //disable RPA

    //set irk
    reg_cv_llbt_irk_ptr = (unsigned int)irk;


    //set prand and irk num = 1
    reg_cv_llbt_rpase_cntl |= (addr[3] | (addr[4]<<8) | (addr[5]<<16) | ((irk_num-1)<<24));

    reg_cv_llbt_hash_status |= (addr[0] | (addr[1]<<8) | (addr[2]<<16));


    // DBG_CHN4_HIGH;
    reg_cv_llbt_rpase_cntl |= FLD_CV_RPASE_START;

    while(!((reg_cv_llbt_hash_status&0x60000000) == 0x40000000));

    // DBG_CHN4_LOW;
    return (reg_cv_llbt_hash_status&FLD_CV_HASH_MATCH? 1:0);
}



// todo here need to optimize
u8 ll_getRpaAddr(u8 *irk, u8 prand[3], u8 rpa[6])
{
    reg_cv_llbt_hash_status |= FLD_CV_RPASE_STATUS_CLR; //clear flag
    reg_cv_llbt_rpase_cntl =(FLD_CV_PRASE_ENABLE | FLD_CV_GEN_RES ) ; //disable RPA

    //set irk
    reg_cv_llbt_irk_ptr = (unsigned int)irk;

    //set prand and irk num
    prand[2] = (prand[2] & 0x3F) | 0x40;
    reg_cv_llbt_rpase_cntl |= prand[0] | (prand[1]<<8) | (prand[2]<<16);

    //set RPA_GEN, enable and start RPA
    reg_cv_llbt_rpase_cntl |= FLD_CV_RPASE_START;


    while(!((reg_cv_llbt_hash_status&0x60000000) == 0x40000000));

    u32 hash = reg_cv_llbt_hash_status;

    rpa[0] = hash & 0xff;
    rpa[1] = (hash>>8)&0xff;
    rpa[2] = (hash>>16)&0xff;

    memcpy(&rpa[3], prand, 3);

    return 1;
}

/**
 * this function is same as rf_clr_dig_logic_state. just because rf_clr_dig_logic_state is driver code and is flash code.
 * so here rewrite the API and define the API as ram code.
 */
_attribute_ram_code_
void rf_ble_clr_dig_logic_state(void)
{
    reg_n22_rst &= ~((FLD_RST0_ZB)|((FLD_RST1_RSTL_BB|FLD_RST1_RST_MDM)<<8));
    reg_n22_rst |= ((FLD_RST0_ZB)|((FLD_RST1_RSTL_BB|FLD_RST1_RST_MDM)<<8));
}

#if FAST_SETTLE

_attribute_data_retention_ Fast_Settle fast_settle_1M;
_attribute_data_retention_ Fast_Settle fast_settle_2M;
_attribute_data_retention_ Fast_Settle fast_settle_S2;
_attribute_data_retention_ Fast_Settle fast_settle_S8;

#endif


