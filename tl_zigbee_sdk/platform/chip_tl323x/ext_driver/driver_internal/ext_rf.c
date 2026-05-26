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
//#include "drivers.h"
#include "ext_lib.h"
//#include "stack/ble/ble_multi/controller/ble_controller.h"

volatile unsigned int TXADDR = 0xc0013000;


#define   BLE_TXDMA_DATA        (0x170000 + 0x84)      //0x170084
#define   BLE_RXDMA_DATA        (0x170000 + 0x80)      //0x170080

//extern dma_config_t rf_tx_dma_config;
//extern dma_config_t rf_rx_dma_config;


//RF BLE Minimum TX Power LVL (unit: 1dBm)
const char  ble_rf_min_tx_pwr   = -23; /* -23dBm */
//RF BLE Maximum TX Power LVL (unit: 1dBm)
const char  ble_rf_max_tx_pwr   = 9;   /*  +9dBm */
//RF BLE Current TX Path Compensation (s16: -1280 ~ 1280, unit: 0.1 dB)
_attribute_data_retention_sec_  signed short ble_rf_tx_path_comp = 0;
//RF BLE Current RX Path Compensation (s16: -1280 ~ 1280, unit: 0.1 dB)
_attribute_data_retention_sec_  signed short ble_rf_rx_path_comp = 0;

//Current RF RX DMA buffer point for BLE
_attribute_data_retention_sec_ _attribute_aligned_(4) unsigned char *ble_curr_rx_dma_buff = NULL;

_attribute_data_retention_sec_ ext_rf_t blt_extRF;

_attribute_ram_code_sec_noinline_  //BLE SDK use
void ble_rf_set_tx_dma(unsigned char fifo_dep,unsigned char size_div_16)
{
    unsigned short fifo_byte_size = size_div_16<<4;
    rf_set_tx_dma_config();
    rf_set_tx_dma_fifo_num(fifo_dep);
    rf_set_tx_dma_fifo_size(fifo_byte_size);
}

_attribute_ram_code_sec_noinline_
void ble_rf_set_rx_dma(unsigned char *buff, unsigned char size_div_16)
{
    unsigned short fifo_byte_size = size_div_16<<4;
    rf_set_rx_dma_config();
    ble_curr_rx_dma_buff = buff;
    rf_set_rx_buffer(buff);
    reg_rf_rx_wptr_mask = 0;
    rf_set_rx_dma_fifo_size(fifo_byte_size);
}

_attribute_ram_code_sec_noinline_
void ble_rx_dma_config(void){
//  dma_config(DMA1,&rf_rx_dma_config);
    rf_set_rx_dma_config();
}

// BLE parameter retention: no loss on power-off
_attribute_ble_data_retention_ static unsigned char rxpara_flag = 1;
void ext_ble_rf_init_rx_sensitivity_optimize(void);

_attribute_ram_code_ void rf_drv_ble_init(void){

#if (PRMBL_LENGTH_1M < 1 || PRMBL_LENGTH_1M > 15)
    #error "1M PHY pream_ble error!!!"
#endif


#if (PRMBL_LENGTH_2M < 2 || PRMBL_LENGTH_1M > 15)
    #error "2M PHY pream_ble error!!!"
#endif

#if 1 //here is driver code
    rf_mode_init();
//    rf_rx_performance_mode(RF_RX_LOW_POWER); // used high performance by default
    rf_set_ble_1M_mode();

//    rf_modem_hp_path(1); //Customers may decide for themselves whether to use HP mode.

    rf_set_crc_config(&rf_crc_config[0]);
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

    if(rxpara_flag)
    {
//      ext_ble_rf_init_rx_sensitivity_optimize();
    }
#endif
}

#if FAST_SETTLE

_attribute_data_retention_ Fast_Settle fast_settle_1M;
_attribute_data_retention_ Fast_Settle fast_settle_2M;
_attribute_data_retention_ Fast_Settle fast_settle_S2;
_attribute_data_retention_ Fast_Settle fast_settle_S8;


#endif


#if RF_THREE_CHANNEL_CALIBRATION
_attribute_data_retention_sec_ unsigned char rf_channel_power[40];
_attribute_data_retention_sec_ unsigned char channel_power_calibration_enable = 0;

_attribute_ram_code_sec_noinline_ //must be RamCode
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



#if (SCHEDULE_USE_BB_TIMER)
_attribute_ram_code_sec_noinline_
void rf_start_fsm (fsm_mode_e mode, void* tx_addr, unsigned int tick)
{
    unsigned int r = core_interrupt_disable(); //prevent user IRQ priority 3 task destroying FSM setting

    unsigned int cur_stimer_tick = clock_time();
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
_attribute_ram_code_sec_noinline_
void rf_start_fsm (fsm_mode_e mode, void* tx_addr, unsigned int tick)
{
    unsigned int r = core_interrupt_disable();
//  write_reg32(0x80170218, tick);
    reg_rf_ll_cmd_schedule = tick;
    delay_us(1);
    reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHEDULE_EN;    // Enable cmd_schedule mode.
    reg_rf_ll_cmd = mode;

    if(tx_addr){
        rf_dma_set_src_address(RF_TX_DMA,(unsigned int)tx_addr);
    }

    core_restore_interrupt(r);
}
#endif

/**
* ----------------------------------------------------
 * | Array Index (i) | Col 0: BLE Channel Freq (Key) | Col 1: Calibration Value (Result) |
 * ----------------------------------------------------
 * | channel_calibration[0] | 10 (e.g., Adv Ch 37) | 0 (Placeholder, will be updated)  |
 * | channel_calibration[1] | 21 (e.g., Data Ch 21) | 0 (Placeholder, will be updated)  |
 * | channel_calibration[2] | 33 (e.g., Data Ch 39) | 0 (Placeholder, will be updated)  |
 * ----------------------------------------------------
 */
_attribute_ble_data_retention_     unsigned char reg_calibration = 0XFF;
// Initialize once on first RX entry
_attribute_no_inline_
void ext_ble_rf_init_rx_sensitivity_optimize(void)
{
    // Buffer for RF DMA (likely for BQB testing/measurement)
    unsigned char bqbtest_buffer[256];


    // Set fixed TX Modulation Index
    rf_set_tx_modulation_index(525);

    // Initial DMA setup (can be done once outside the loop)
    rf_set_rx_dma(bqbtest_buffer, 0, 272);


        // Stop RF operations and clear state
//        STOP_RF_STATE_MACHINE;
        reg_rf_ll_cmd = 0x80;
        rf_set_tx_rx_off();
        CLEAR_ALL_RFIRQ_STATUS;
        rf_set_tx_rx_off_auto_mode(); // Set to idle mode

        // --- MCU Core Specific Mode Configuration (TL323X) ---
#if defined(MCU_CORE_TL323X)
        // Check if the current channel is BLE Channel 10 or 33
        if(0) {
            // Use 48M modem rate for specific channels (likely 1M PHY)
            reg_rf_mode_cfg_txrx_0 |= FLD_RF_DIS_CLK_DIG_O;
            rf_modem_rate_mode(RF_48M_MODEM_RATE);
            reg_rf_mode_cfg_txrx_0 &= (~FLD_RF_DIS_CLK_DIG_O);
        } else {
            // Use 24M modem rate for other channels (likely 2M PHY or advertising channels)
            reg_rf_mode_cfg_txrx_0 |= FLD_RF_DIS_CLK_DIG_O;
            rf_modem_rate_mode(RF_24M_MODEM_RATE);
            reg_rf_mode_cfg_txrx_0 &= (~FLD_RF_DIS_CLK_DIG_O);
        }
#endif

        // Set the BLE channel using the stored frequency value (10, 21, or 33)
        rf_set_ble_chn(21);
        // Set RF to RX mode to perform measurement/calibration
//        rf_set_rxmode();

        rf_set_rx_dma(bqbtest_buffer, 0, 272);

        rf_start_srx(stimer_get_tick());

        delay_us(30);

        // --- Read Calibration Value ---
        // rf_set_rxpara
        reg_calibration = read_reg8(0x1706ea);

        // Apply a correction/offset to the measured value
        if (reg_calibration > 5) {
            reg_calibration -= 5;
        }
        // Stop RF state machine
        rf_set_tx_rx_off_auto_mode();
        CLEAR_ALL_RFIRQ_STATUS;
        delay_us(10);
        CLEAR_ALL_RFIRQ_STATUS;

        /* process all potential TX DMA conflict */
        rf_dma_reset();

        rf_clr_dig_logic_state();

    // Flag to indicate that the initial calibration has been completed
    rxpara_flag = 0;
}
/**
 * BLE RF channel sensitivity optimization(xiaomi vela OS project)
 * @param chn_num Channel number to configure
 * @note Ref: http://192.168.48.112:8080/browse/CPTL321X-71
 */
_attribute_ram_code_
void ext_ble_rf_state_channel_rx_sensitivity_optimize(signed char freq)
{


    // Return if initial calibration is not yet finished (rxpara_flag is likely 1 initially)
    if (rxpara_flag == 1) {
        return;
    }

    // --- MCU Core Specific Mode Configuration (TL323X) ---
#if defined(MCU_CORE_TL323X)
    // Dynamic modem rate selection based on the current BLE channel (freq)
   // if(freq == 10 || freq == 33)
      if(0)
      {
        reg_rf_mode_cfg_txrx_0 |= FLD_RF_DIS_CLK_DIG_O;
        rf_modem_rate_mode(RF_48M_MODEM_RATE);
        reg_rf_mode_cfg_txrx_0 &= (~FLD_RF_DIS_CLK_DIG_O);
    } else {
        reg_rf_mode_cfg_txrx_0 |= FLD_RF_DIS_CLK_DIG_O;
        rf_modem_rate_mode(RF_24M_MODEM_RATE);
        reg_rf_mode_cfg_txrx_0 &= (~FLD_RF_DIS_CLK_DIG_O);
    }
#endif


    // --- Apply Calibration ---
        // Write the retrieved calibration value to the hardware register (0x1706e5)
    write_reg8(0x1706e5, (read_reg8(0x1706e5) & 0xc0) | reg_calibration);


    // --- LDO Bypass Configuration ---
    // Apply LDO bypass only for the three specified calibrated channels
    if (freq == 10 || freq == 21 || freq == 33) {
        rf_ldot_ldo_rxtxlf_bypass_en();
    } else {
        rf_ldot_ldo_rxtxlf_bypass_dis();
    }
}


