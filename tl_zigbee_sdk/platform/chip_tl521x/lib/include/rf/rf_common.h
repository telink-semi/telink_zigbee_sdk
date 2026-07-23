
/********************************************************************************************************
 * @file    rf_common.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 /**
 * @page RF RF Module Configuration Guide
 * 
 * @brief This page provides detailed usage notes, power configuration instructions, and key precautions for the RF module.
 * 
 * @par Header File
 *      rf_common.h
 * 
 * @section rf_pm_precautions RF and PM Usage Precautions
 * When using RF and Power Management (PM) functions in combination, note the following requirements for different low-power modes:
 * 
 * - <b>Suspend mode</b>:
 *   RF-related digital registers will be lost. After waking up from suspend mode, re-invoke all RF-related function interfaces to restore functionality.
 *   <br>
 *   If you don't want to re-invoke RF-related function interfaces after waking up, you can use the <code>pm_set_suspend_power_cfg</code> function 
 *   to set <code>PM_POWER_BASEBAND</code> to maintain power during suspend sleep. 
 *   <b>Note:</b> This will increase power consumption during suspend.
 * 
 * - <b>Deep retention mode</b>:
 *   RF-related digital registers will be lost. After waking up from deep retention mode, re-invoke all RF-related function interfaces to restore functionality.
 * 
 * - <b>Deep mode</b>:
 *   RF-related digital registers will be lost. After waking up from deep mode, re-invoke all RF-related function interfaces to restore functionality.
 * 
 * @section rf_power_explanation RF Transmit Power Configuration
 * 
 * @subsection rf_power_supply Power Supply Modes of RF PA Module
 * The RF Power Amplifier (PA) module supports two power supply modes, with key characteristics as follows:
 * 
 * | Power Supply Mode | Power Source                          | Output Power Characteristics                                                                 | Advantage                                  |
 * |-------------------|---------------------------------------|------------------------------------------------------------------------------------------------|-------------------------------------------|
 * | VBAT mode         | Directly powered by VBAT              | Maximum output power varies with VBAT voltage (higher VBAT → higher available power)          | Simple power path, suitable for high-power scenarios |
 * | VANT mode         | Powered by embedded DCDC + LDO        | Output power is stable (independent of VBAT voltage)                                          | Lower power consumption at the same transmit power |
 * 
 * @subsection rf_power_table TX Power Table (Driver-Provided)
 * - The default <code>rf_power_level_e</code> enumeration is tested based on the largest package, providing typical reference values for both VBAT and VANT modes.
 * - Power levels exceeding the maximum VANT power are automatically mapped to VBAT mode (driver handles mode switching internally).
 * 
 * @subsection rf_power_adjust Power Level Adjustment Instructions
 * #### 3.1 Basic Configuration Rules
 * - Both VBAT and VANT modes support 64 configurable power levels (range: 0 ~ 63).
 * - The mapping relationship between level numbers and hardware registers is fixed (package-independent); only the actual transmit power differs by package.
 * 
 * #### 3.2 Configuration Methods (via <code>rf_set_power_level</code> Interface)
 * Two configuration approaches are supported:
 * a) Use predefined values from the <code>rf_power_level_e</code> enumeration (recommended for most scenarios).
 * b) Write custom level values (for scenario-specific power optimization), following mode-specific formatting rules.
 * 
 * #### 3.3 Mode-Specific Level Format
 * | Mode       | Level Format                          | Valid Range |
 * |------------|---------------------------------------|-------------|
 * | VBAT mode  | <code>level = power_level</code>      | 0 ~ 63      |
 * | VANT mode  | <code>level = BIT(7) \| power_level</code> | 0 ~ 63      |
 * 
 * @subsection rf_power_notes Supplementary Notes
 * 1. For package-specific detailed power data (e.g., actual power values corresponding to each level), refer to the <cite>[Chip Model] RF Test Report</cite>.
 * 2. Actual transmit power is affected by antenna matching, PCB layout, and hardware design. Mandatory hardware calibration is required for mass production or high-precision applications.
 */
#ifndef RF_COMMON_H
#define RF_COMMON_H

#include "lib/include/sys.h"
#include "lib/include/rf/rf_dma.h"
#include "lib/include/rf/rf_ble.h"
#include "lib/include/rf/rf_private.h"
#include "lib/include/rf/rf_zigbee.h"
#include "gpio.h"
#include "lib/include/stimer.h"
/**********************************************************************************************************************
 *                                         RF  global macro                                                           *
 *********************************************************************************************************************/

/**
 * @brief       This define for ble debug the effect of rx_dly.
 *              when this function turn on the time of rx_dly will shorten 6.3us,
 */
#define RF_RX_SHORT_MODE_EN 1 //In order to debug whether the problem is caused by rx_dly.
/**
 *  @brief This define serve to calculate the DMA length of packet.
 */
#define rf_tx_packet_dma_len(rf_data_len) (((rf_data_len) + 3) / 4) | (((rf_data_len) % 4) << 22)

/**
 *  @brief This macro provides an alternative name for the rf_get_latched_rssi() function to be compatible with older versions of code
 */
#define rf_get_rssi rf_get_latched_rssi
/**********************************************************************************************************************
 *                                       RF global data type                                                          *
 *********************************************************************************************************************/


/**
 *  @brief  RX fast settle time
 *  @note
 *  1. Call rf_fast_settle_config to configure timing during initialization.
 *  2. Call the enable function rf_rx_fast_settle_en when using the configured timing sequence.
 *     To close it, call rf_rx_fast_settle_dis.
 *  3. The deleted hardware calibration values are influenced by environmental temperature and require periodic recalibration.
 *     Calibration method: Call rf_rx_fast_settle_dis, then for different frequency points:
 *     stop RF-related states, enable RX, wait for packet transmission to end -> rf_rx_fast_settle_update_cal_val.
 */
typedef enum
{
    /**
     * @brief Reduce 77.5us of RX settle time
     *
     * Receive for a period of time and then do a normal calibration.
     *
     * @note
     *    Related to frequency points,Refer to the table below to determine the range of frequency points used.
     *    In addition to configuring the calibration values for the used frequency points,
     *    corresponding channel values in the respective intervals need to be configured.
     * @table
     * | Frequency (MHz) | 2400-2410 | 2410-2420 | 2420-2430 | 2430-2440 | 2440-2450 | 2450-2460 | 2460-2470 | 2470-2480 |
     * |-----------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
     * | Channel Value   |     4     |    14     |    24     |    34     |    44     |    54     |    64     |    74     |
     * @endtable
     */
    RX_SETTLE_TIME_15US = 0,

    /**
     * @brief Reduce 44.5us of RX settle time
     * Receive for a period of time and then do a normal calibration.
     */
    RX_SETTLE_TIME_37US = 1,

    /**
     * @brief Reduce 8.5us of RX settle time
     * Receive for a period of time and then do a normal calibration.
     */
    RX_SETTLE_TIME_77US = 2,

    RX_FAST_SETTLE_NONE = 3

} rf_rx_fast_settle_time_e;

/**
 *  @brief  TX fast settle time
 *  @note
 *  1. Call rf_fast_settle_config to configure timing during initialization.
 *  2. Call the enable function rf_tx_fast_settle_en when using the configured timing sequence.
 *     To close it, call rf_tx_fast_settle_dis.
 *  3. The deleted hardware calibration values are influenced by environmental temperature and require periodic recalibration.
 *     Calibration method: Call rf_tx_fast_settle_dis, then for different frequency points:
 *     stop RF-related states, enable TX, wait for packet transmission to end -> rf_tx_fast_settle_update_cal_val.
 */
typedef enum
{
    /**
     * @brief Reduce 87.5us of transmit settle time
     *
     * After frequency hopping, a normal calibration must be done.
     *
     * @note
     *    Related to frequency points,Refer to the following table to determine the range of frequency points used.
     *    Corresponding channel values in the respective intervals need to be configured.
     *
     * @table
     * | Frequency (MHz) | 2400-2410 | 2410-2420 | 2420-2430 | 2430-2440 | 2440-2450 | 2450-2460 | 2460-2470 | 2470-2480 |
     * |-----------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
     * | Channel Value   |     4     |    14     |    24     |    34     |    44     |    54     |    64     |    74     |
     * @endtable
     */
    TX_SETTLE_TIME_15US = 0,

    /**
     * @brief Reduce 61.5us of transmit settle time
     *
     * Related to frequency points. After frequency hopping, a normal calibration must be done.
     */
    TX_SETTLE_TIME_51US = 1,

    /**
     * @brief Reduce 8.5us of transmit settle time
     *
     * Perform a normal calibration at the beginning.
     */
    TX_SETTLE_TIME_104US = 2,

    TX_FAST_SETTLE_NONE = 3

} rf_tx_fast_settle_time_e;

/**
 *  @brief  LDO trim calibration value
 */
typedef struct
{
    unsigned char LDO_CAL_TRIM;
    unsigned char LDO_RXTXHF_TRIM;
    unsigned char LDO_RXTXLF_TRIM;
    unsigned char LDO_PLL_TRIM;
    unsigned char LDO_VCO_TRIM;
} rf_ldo_trim_t;

/**
 *  @brief  DCOC calibration value
 */
typedef struct
{
    unsigned char DCOC_IDAC;
    unsigned char DCOC_QDAC;
    unsigned char DCOC_IADC_OFFSET;
    unsigned char DCOC_QADC_OFFSET;
} rf_dcoc_cal_t;

/**
 *  @brief  RCCAL calibration value
 */
typedef struct
{
    unsigned char CBPF_CCODE_L;
    unsigned char CBPF_CCODE_H;
} rf_rccal_cal_t;

typedef struct
{
    unsigned short cal_tbl[81];
    rf_ldo_trim_t  ldo_trim;
    rf_dcoc_cal_t  dcoc_cal;
    rf_rccal_cal_t rccal_cal;
    unsigned char  tx_fcal[81];
    unsigned char  rx_fcal[81];
} rf_fast_settle_t;

/**
 * @brief       RF CRC config.
 * @note        init_value - The CRC init value to be set for RF.
 *              poly - The CRC polynomial to be set for RF.
 *              xor_out - XOR mask for CRC result (for no mask, should be 0)
 *              byte_order - CRC output byte order (1:MSByte first,0:LSByte first)
 *              start_cal_pos - CRC start byte position, 0 is the first byte
 *              len - The CRC length to be set for RF
 */
typedef struct
{
    unsigned int  init_value;
    unsigned int  poly;
    unsigned int  xor_out;
    unsigned char byte_order;
    unsigned char start_cal_pos;
    unsigned char len;
} rf_crc_config_t;

/**
 *  @brief  select status of rf.
 */
typedef enum
{
    RF_MODE_TX   = 0, /**<  Tx mode */
    RF_MODE_RX   = 1, /**<  Rx mode */
    RF_MODE_AUTO = 2, /**<  Auto mode */
    RF_MODE_OFF  = 3  /**<  TXRX OFF mode */
} rf_status_e;

/**
 *  @brief   Define power list of RF.
 *  @note    (1)The energy meter is averaged over 3 chips at room temperature and 3.3V supply voltage..
 *           (2)Transmit energy in VBAT mode decreases as the supply voltage drops.
 *           (3)There will be some differences in the energy values tested between different chips.
 */
typedef enum
{
    /*VBAT*/
    RF_POWER_P10p00dBm = 22, /**<  10.0 dbm */
    RF_POWER_P9p69dBm  = 20, /**<   9.7 dbm */
    RF_POWER_P9p43dBm  = 19, /**<   9.4 dbm */
    RF_POWER_P9p12dBm  = 18, /**<   9.1 dbm */
    RF_POWER_P8p79dBm  = 17, /**<   8.8 dbm */
    RF_POWER_P8p40dBm  = 16, /**<   8.4 dbm */
    RF_POWER_P7p91dBm  = 15, /**<   7.9 dbm */
    RF_POWER_P7p42dBm  = 14, /**<   7.4 dbm */
    RF_POWER_P6p88dBm  = 13, /**<   6.9 dbm */
    RF_POWER_P6p26dBm  = 12, /**<   6.3 dbm */
    RF_POWER_P5p59dBm  = 11, /**<   5.6 dbm */
    RF_POWER_P4p83dBm  = 10, /**<   4.8 dbm */

    /*VANT*/
    RF_POWER_P4p50dBm  = BIT(7) | 56,  /**<    4.5 dbm */
    RF_POWER_P4p00dBm  = BIT(7) | 49,  /**<    4.0 dbm */
    RF_POWER_P3p50dBm  = BIT(7) | 43,  /**<    3.5 dbm */
    RF_POWER_P3p00dBm  = BIT(7) | 39,  /**<    3.0 dbm */
    RF_POWER_P2p50dBm  = BIT(7) | 35,  /**<    2.5 dbm */
    RF_POWER_P2p00dBm  = BIT(7) | 32,  /**<    2.0 dbm */
    RF_POWER_P1p50dBm  = BIT(7) | 30,  /**<    1.5 dbm */
    RF_POWER_P1p00dBm  = BIT(7) | 28,  /**<    1.0 dbm */
    RF_POWER_P0p50dBm  = BIT(7) | 26,  /**<    0.5 dbm */
    RF_POWER_P0p00dBm  = BIT(7) | 24,  /**<    0.0 dbm */
    RF_POWER_N0p50dBm  = BIT(7) | 22,  /**<   -0.5 dbm */
    RF_POWER_N1p00dBm  = BIT(7) | 20,  /**<  -1.00 dbm */
    RF_POWER_N1p50dBm  = BIT(7) | 19,  /**<  -1.50 dbm */
    RF_POWER_N2p00dBm  = BIT(7) | 18,  /**<  -2.00 dbm */
    RF_POWER_N2p50dBm  = BIT(7) | 17,  /**<  -2.50 dbm */
    RF_POWER_N3p00dBm  = BIT(7) | 16,  /**<  -3.00 dbm */
    RF_POWER_N3p50dBm  = BIT(7) | 15,  /**<  -3.50 dbm */
    RF_POWER_N4p00dBm  = BIT(7) | 14,  /**<  -4.00 dbm */
    RF_POWER_N4p50dBm  = BIT(7) | 13,  /**<  -4.50 dbm */
    RF_POWER_N5p00dBm  = BIT(7) | 12,  /**<  -5.00 dbm */
    RF_POWER_N5p50dBm  = BIT(7) | 11,  /**<  -5.50 dbm */
    RF_POWER_N6p50dBm  = BIT(7) | 10,  /**<  -6.50 dbm */
    RF_POWER_N7p50dBm  = BIT(7) | 9,   /**<  -7.50 dbm */
    RF_POWER_N8p50dBm  = BIT(7) | 8,   /**<  -8.50 dbm */
    RF_POWER_N9p50dBm  = BIT(7) | 7,   /**<  -9.50 dbm */
    RF_POWER_N10p90dBm = BIT(7) | 6,  /**<  -10.90 dbm */
    RF_POWER_N12p30dBm = BIT(7) | 5,  /**<  -12.30 dbm */
    RF_POWER_N14p20dBm = BIT(7) | 4,  /**<  -14.20 dbm */
    RF_POWER_N16p50dBm = BIT(7) | 3,  /**<  -16.50 dbm */
    RF_POWER_N20p00dBm = BIT(7) | 2,  /**<  -20.00 dbm */
    RF_POWER_N25p30dBm = BIT(7) | 1,  /**<  -25.30 dbm */

} rf_power_level_e;

/**
 *  @brief   Define power list of RF.
 *  @note    (1)The energy meter is averaged over 3 chips at room temperature and 3.3V supply voltage..
 *           (2)Transmit energy in VBAT mode decreases as the supply voltage drops.
 *           (3)There will be some differences in the energy values tested between different chips.
 */
typedef enum
{
    /*VBAT*/
    RF_POWER_INDEX_P10p00dBm, /**<  10.0 dbm */
    RF_POWER_INDEX_P9p69dBm,  /**<   9.7 dbm */
    RF_POWER_INDEX_P9p43dBm,  /**<   9.4 dbm */
    RF_POWER_INDEX_P9p12dBm,  /**<   9.1 dbm */
    RF_POWER_INDEX_P8p79dBm,  /**<   8.8 dbm */
    RF_POWER_INDEX_P8p40dBm,  /**<   8.4 dbm */
    RF_POWER_INDEX_P7p91dBm,  /**<   7.9 dbm */
    RF_POWER_INDEX_P7p42dBm,  /**<   7.4 dbm */
    RF_POWER_INDEX_P6p88dBm,  /**<   6.9 dbm */
    RF_POWER_INDEX_P6p26dBm,  /**<   6.3 dbm */
    RF_POWER_INDEX_P5p59dBm,  /**<   5.6 dbm */
    RF_POWER_INDEX_P4p83dBm,  /**<   4.8 dbm */

    /*VANT*/
    RF_POWER_INDEX_P4p50dBm,  /**<    4.5 dbm */
    RF_POWER_INDEX_P4p00dBm,  /**<    4.0 dbm */
    RF_POWER_INDEX_P3p50dBm,  /**<    3.5 dbm */
    RF_POWER_INDEX_P3p00dBm,  /**<    3.0 dbm */
    RF_POWER_INDEX_P2p50dBm,  /**<    2.5 dbm */
    RF_POWER_INDEX_P2p00dBm,  /**<    2.0 dbm */
    RF_POWER_INDEX_P1p50dBm,  /**<    1.5 dbm */
    RF_POWER_INDEX_P1p00dBm,  /**<    1.0 dbm */
    RF_POWER_INDEX_P0p50dBm,  /**<    0.5 dbm */
    RF_POWER_INDEX_P0p00dBm,  /**<    0.0 dbm */
    RF_POWER_INDEX_N0p50dBm,  /**<   -0.5 dbm */
    RF_POWER_INDEX_N1p00dBm,  /**<  -1.00 dbm */
    RF_POWER_INDEX_N1p50dBm,  /**<  -1.50 dbm */
    RF_POWER_INDEX_N2p00dBm,  /**<  -2.00 dbm */
    RF_POWER_INDEX_N2p50dBm,  /**<  -2.50 dbm */
    RF_POWER_INDEX_N3p00dBm,  /**<  -3.00 dbm */
    RF_POWER_INDEX_N3p50dBm,  /**<  -3.50 dbm */
    RF_POWER_INDEX_N4p00dBm,  /**<  -4.00 dbm */
    RF_POWER_INDEX_N4p50dBm,  /**<  -4.50 dbm */
    RF_POWER_INDEX_N5p00dBm,  /**<  -5.00 dbm */
    RF_POWER_INDEX_N5p50dBm,  /**<  -5.50 dbm */
    RF_POWER_INDEX_N6p50dBm,  /**<  -6.50 dbm */
    RF_POWER_INDEX_N7p50dBm,  /**<  -7.50 dbm */
    RF_POWER_INDEX_N8p50dBm,  /**<  -8.50 dbm */
    RF_POWER_INDEX_N9p50dBm,  /**<  -9.50 dbm */
    RF_POWER_INDEX_N10p90dBm, /**<  -10.90 dbm */
    RF_POWER_INDEX_N12p30dBm, /**<  -12.30 dbm */
    RF_POWER_INDEX_N14p20dBm, /**<  -14.20 dbm */
    RF_POWER_INDEX_N16p50dBm, /**<  -16.50 dbm */
    RF_POWER_INDEX_N20p00dBm, /**<  -20.00 dbm */
    RF_POWER_INDEX_N25p30dBm, /**<  -25.30 dbm */

} rf_power_level_index_e;

/**
 *  @brief  Define RF mode.
 */
typedef enum
{
    RF_MODE_BLE_2M,                   /**< ble 2m mode */
    RF_MODE_BLE_1M,                   /**< ble 1M mode */
    RF_MODE_BLE_1M_NO_PN,             /**< ble 1M close pn mode */
    RF_MODE_ZIGBEE_250K,              /**< zigbee 250K mode */
    RF_MODE_LR_S2_500K,               /**< ble 500K mode */
    RF_MODE_LR_S8_125K,               /**< ble 125K mode */
    RF_MODE_PRIVATE_250K,             /**< private 250K mode */
    RF_MODE_PRIVATE_500K,             /**< private 500K mode */
    RF_MODE_PRIVATE_1M,               /**< private 1M mode */
    RF_MODE_PRIVATE_2M,               /**< private 2M mode */
    RF_MODE_ANT,                      /**< ant mode */
    RF_MODE_BLE_2M_NO_PN,             /**< ble 2M close pn mode */
    RF_MODE_HYBEE_1M,                 /**< hybee 1M mode */
    RF_MODE_HYBEE_2M,                 /**< hybee 2M mode */
    RF_MODE_HYBEE_500K,               /**< hybee 500K mode */
    RF_MODE_PRI_GENERIC_250K,         /**< private generic 250K mode */
    RF_MODE_PRI_GENERIC_500K,         /**< private generic 500K mode */
    RF_MODE_PRI_GENERIC_1M,           /**< private generic 1M mode */
    RF_MODE_PRI_GENERIC_2M,           /**< private generic 2M mode */
    RF_MODE_BLE_4M_NO_PN,             /**< ble 4M close pn mode */
    RF_MODE_BLE_6M_NO_PN,             /**< ble 6M close pn mode */
    RF_MODE_BLE_4M,                   /**< ble 4M mode */
    RF_MODE_BLE_6M,                   /**< ble 6M mode */
    RF_MODE_PRIVATE_4M,               /**< private 4M mode */
    RF_MODE_PRIVATE_6M,               /**< private 6M mode */
    RF_MODE_PRI_GENERIC_4M,           /**< private generic 4M mode */
    RF_MODE_PRI_GENERIC_6M,           /**< private generic 6M mode */
    RF_MODE_HYBEE_1M_OLD,             /**< hybee 1M mode old*/
    RF_MODE_HYBEE_2M_OLD,             /**< hybee 2M mode old*/
    RF_MODE_HYBEE_500K_NEW,           /**< hybee 500k mode new*/
    RF_MODE_HYBEE_1M_NEW,             /**< hybee 1M mode new*/
    RF_MODE_HYBEE_2M_NEW,             /**< hybee 2M mode new*/
    RF_MODE_HYBEE_500K_2BYTE_SFD,     /**< hybee 500K 2byte sfd mode*/
    RF_MODE_HYBEE_1M_2BYTE_SFD,       /**< hybee 1M 2byte sfd mode*/
    RF_MODE_HYBEE_2M_2BYTE_SFD,       /**< hybee 2M 2byte sfd mode*/
    RF_MODE_HYBEE_2M_2BYTE_SFD_NEW,   /**< hybee 2M 2byte sfd mode new*/
    RF_MODE_HYBEE_1M_2BYTE_SFD_NEW,   /**< hybee 1M 2byte sfd mode new*/
    RF_MODE_HYBEE_500K_2BYTE_SFD_NEW, /**< hybee 500K 2byte sfd mode new*/
    RF_MODE_HR_2M,                    /**< hr 2M mode*/

} rf_mode_e;

/**
 *  @brief  Define RF channel.
 */
typedef enum
{
    RF_CHANNEL_0    = BIT(0), /**< RF channel 0 */
    RF_CHANNEL_1    = BIT(1), /**< RF channel 1 */
    RF_CHANNEL_2    = BIT(2), /**< RF channel 2 */
    RF_CHANNEL_3    = BIT(3), /**< RF channel 3 */
    RF_CHANNEL_4    = BIT(4), /**< RF channel 4 */
    RF_CHANNEL_5    = BIT(5), /**< RF channel 5 */
    RF_CHANNEL_6    = BIT(6), /**< RF channel 6 */
    RF_CHANNEL_7    = BIT(7), /**< RF channel 7 */
    RF_CHANNEL_NONE = 0x00,   /**< none RF channel*/
    RF_CHANNEL_ALL  = 0xff,   /**< all RF channel */
} rf_channel_e;

/**
 * @brief define rf bb timer clock tick per us/ms/s.
 */
enum
{
    BB_TIMER_TICK_1US = 8,
    BB_TIMER_TICK_1MS = 8000,
    BB_TIMER_TICK_1S  = 8000000,
};

/**
 * @brief Define RF modem rate, RF_24M_MODEM_RATE and RF_48M_MODEM_RATE
 */
typedef enum
{
    RF_24M_MODEM_RATE = 0,
    RF_48M_MODEM_RATE = 1,
} rf_modem_rate_e;

typedef enum {
    ASYM_1M_TO_2M = 0,
    ASYM_2M_TO_1M = 1,
} rf_phy_switch_dir_e;

/**********************************************************************************************************************
 *                                         RF global constants                                                        *
 *********************************************************************************************************************/
extern _attribute_data_retention_ volatile rf_power_level_e rf_power_Level_list[60];
extern rf_mode_e              g_rfmode;
extern rf_crc_config_t        rf_crc_config[3];

/**********************************************************************************************************************
 *                                         RF function declaration                                                    *
 *********************************************************************************************************************/

/**
 * @brief       This function serves to judge the statue of  RF receive.
 * @return      -#0:idle
 *              -#1:rx_busy
 */
static inline unsigned char rf_receiving_flag(void)
{
    //if the value of [2:0] of the reg_0x170040 isn't 0 , it means that the RF is in the receiving packet phase.(confirmed by junwen).
    return ((read_reg8(0x170040) & 0x07) > 1);
}

/**
 * @brief       This function serves to set the which irq enable.
 * @param[in]   mask    - Options that need to be enabled.
 * @return      Yes: 1, NO: 0.
 */
static inline void rf_set_irq_mask(rf_irq_e mask)
{
    BM_SET(reg_rf_irq_mask, mask);
    BM_SET(reg_rf_ll_irq_mask_h, (mask & 0xff0000) >> 16);
    BM_SET(reg_rf_ll_cmd, (mask & 0x5000000) >> 20);
    BM_SET(reg_rf_ll_irq_mask_h1, (mask & 0x2000000) >> 24);
}

/**
 * @brief       This function serves to clear the TX/RX irq mask.
 * @param[in]   mask    - RX/TX irq value.
 * @return      none.
 */
static inline void rf_clr_irq_mask(rf_irq_e mask)
{
    BM_CLR(reg_rf_irq_mask, mask);
    BM_CLR(reg_rf_ll_irq_mask_h, (mask & 0xff0000) >> 16);
    BM_CLR(reg_rf_ll_cmd, (mask & 0x5000000) >> 20);
    BM_CLR(reg_rf_ll_irq_mask_h1, (mask & 0x2000000) >> 24);
}

/**
 *  @brief      This function serves to judge whether it is in a certain state.
 *  @param[in]  mask      - RX/TX irq status.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned int rf_get_irq_status(rf_irq_e status)
{
    return ((unsigned int)(BM_IS_SET(reg_rf_irq_status, status) | BM_IS_SET((reg_rf_irq_status_h << 16), status) | BM_IS_SET((reg_rf_irq_status_h1 << 24), status)));
}

/**
 *@brief    This function serves to clear the Tx/Rx finish flag bit.
 *          After all packet data are sent, corresponding Tx finish flag bit
 *          will be set as 1.By reading this flag bit, it can check whether
 *          packet transmission is finished. After the check, it is needed to
 *          manually clear this flag bit so as to avoid misjudgment.
 *@return   none.
 */
static inline void rf_clr_irq_status(rf_irq_e status)
{
    reg_rf_irq_status    = status;
    reg_rf_irq_status_h  = (status & 0xff0000) >> 16;
    reg_rf_irq_status_h1 = (status & 0x7000000) >> 24;
}

/**
 * @brief       This function serves to set RF access code.
 * @param[in]   acc   - the value of access code.
 * @return      none.
 */
static inline void rf_access_code_comm(unsigned int acc)
{
    reg_rf_access_code = acc;
    reg_rf_access_4 = (reg_rf_access_3 & 0x01) ? 0x55 : 0xaa;
    //The following two lines of code are for trigger access code in S2,S8 mode.It has no effect on other modes.
    reg_rf_modem_mode_cfg_rx1_0 &= ~FLD_RF_LR_TRIG_MODE;
    write_reg8(0x170425, read_reg8(0x170425) | 0x01);
}

/**
 * @brief       this function is to enable/disable each access_code channel for
 *              RF Rx terminal.
 * @param[in]   pipe    - Bit0~bit7 correspond to channel 0~7, respectively.
 *                      - #0:Disable.
 *                      - #1:Enable.
 *                        If "enable" is set as 0xff (i.e. 11111111),
 *                        all access_code channels (0~7) are enabled.
 * @return      none
 */
static inline void rf_rx_acc_code_pipe_en(rf_channel_e pipe)
{
    reg_rf_modem_rx_ctrl_1 = pipe; //rx_access_code_chn_en
}

/**
 * @brief       this function is to select access_code channel for RF tx terminal.
 * @param[in]   pipe    - tx access code channel,The pipe range is from 0 to 7
 *                        And only 1 channel can be selected every time.
 *                        If "enable" is set as 0x7 (i.e. 0111),the access_code channel (7) is enabled.
 * @return      none
 */
static inline void rf_tx_acc_code_pipe_en(unsigned char pipe)
{
    write_reg8(0x170215, ((read_reg8(0x170215) & 0xf8) | pipe) | BIT(4)); //Tx_Channel_man[2:0]
}

/**
 * @brief     This function serves to reset RF Tx/Rx mode.
 * @return    none.
 */
static inline void rf_set_tx_rx_off(void)
{
    write_reg8(0x80170216, 0x29);
    write_reg8(0x80170028, 0x80); // rx disable
    write_reg8(0x80170202, 0x45); // reset tx/rx state machine
}

/**
 * @brief    This function serves to turn off RF auto mode.
 * @return   none.
 * @note     Attention: When forcibly stopping the state machine through this interface, it must be ensured
 *           that rx is not in the process of receiving packets.Otherwise, an error may be caused.To determine
 *           whether the packet is being received, you can use the function rf_receiving_flag.
 */
static inline void rf_set_tx_rx_off_auto_mode(void)
{
    reg_rf_ll_cmd = FLD_RF_R_CMD_TRIG;
}

/**
 * @brief       This function serve to rx dma fifo size.
 * @param[in]   fifo_byte_size - The length of one dma fifo,the range is 1~0xffff(the corresponding number of fifo bytes is fifo_byte_size).
 * @return      none
 */
static inline void rf_set_rx_dma_fifo_size(unsigned short fifo_byte_size)
{
    reg_rf_bb_rx_size   = fifo_byte_size & 0xff;
    reg_rf_bb_rx_size_h = fifo_byte_size >> 8;
}

/**
 * @brief       This function serve to set rx dma wptr.
 * @param[in]   fifo_num    -This parameter is used to set the mask value for the number of enabled FIFOs. The value of the mask must (0x00,0x01,0x03,0x07,0x0f,0x1f).
 *                           The number of FIFOs enabled is the value of wptr_mask plus 1.(0x01,0x02,0x04,0x08,0x10,0x20)
 * @return      none
 */
static inline void rf_set_rx_dma_fifo_num(unsigned char fifo_num)
{
    reg_rf_rx_wptr_mask = fifo_num;
}

/**
 * @brief       This function serves to DMA rxFIFO address
 *              The function apply to the configuration of one rxFiFO when receiving packets,
 *              In this case,the rxFiFo address can be changed every time a packet is received
 *              Before setting, call the function "rf_set_rx_dma" to clear DMA fifo mask value(set 0)
 * @param[in]   rx_addr   - The address store receive packet.
 * @return      none
 * @note        rx_addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
static inline void rf_set_rx_buffer(unsigned char *rx_addr)
{
    rx_addr += 4;
    rf_dma_set_dst_address(RF_RX_DMA, (unsigned int)(rx_addr));
}

/**
 * @brief       This function serve to set the number of tx dma fifo.
 * @param[in]   fifo_dep - the number of dma fifo is 2 to the power of fifo_dep.
 * @return      none
 */
static inline void rf_set_tx_dma_fifo_num(unsigned char fifo_num)
{
    reg_rf_bb_tx_chn_dep = fifo_num; //tx_chn_dep = 2^2 =4 (have 4 fifo)
}

/**
 * @brief       This function serve to set the number of tx dma fifo.
 * @param[in]   fifo_byte_size - the size of each dma fifo.
 * @return      none
 */
static inline void rf_set_tx_dma_fifo_size(unsigned short fifo_byte_size)
{
    reg_rf_bb_tx_size   = fifo_byte_size & 0xff; //tx_idx_addr = {tx_chn_adr*bb_tx_size,4'b0}// in this setting the max data in one dma buffer is 0x20<<4.And the The product of fifo_dep and bytesize cannot exceed 0xfff.
    reg_rf_bb_tx_size_h = fifo_byte_size >> 8;
}

/**
 * @brief   This function serves to set RF tx settle time.
 * @param[in]  tx_stl_us  tx settle time,the unit is us.The max value of this param is 0xfff;The default settling time value is 150us.
 *             The typical value is 113us (tx_settle time should not be less than this value).
 * @return  none.
 * @note       Attention:It is not necessary to call this function to adjust the settling time in the normal sending state.
 */
static inline void rf_set_tx_settle_time(unsigned short tx_stl_us)
{
    tx_stl_us &= 0x0fff;
    write_reg16(0x170204, (read_reg16(0x170204) & 0xf000) | (tx_stl_us - 1));
}

/**
 * @brief   This function serves to set RF tx settle time and rx settle time.
 * @param[in]  rx_stl_us  rx settle time,the unit is us.The max value of this param is 0xfff;The default settling time value is 150us.
 *             The typical value is 85us (rx_settle time should not be less than this value).
 * @return  none.
 * @note       Attention:It is not necessary to call this function to adjust the settling time in the normal packet receiving state.
 */
static inline void rf_set_rx_settle_time(unsigned short rx_stl_us)
{
    rx_stl_us &= 0x0fff;
    write_reg16(0x17020c, (read_reg16(0x17020c) & 0xf000) | (rx_stl_us - 1));
}

/**
 * @brief       This function serves to set CRC length for RF.
 * @param[in]   len - The CRC length to be set for RF,the range is 0~4byte.
 * @return      none.
 * @note        Attention:when CRC length is 0,TX need to disable CRC.
 */
static inline void rf_set_crc_len(unsigned char len)
{
    reg_rf_crc_config2 = ((reg_rf_crc_config2 & (~FLD_RF_CRC_LENGTH)) | len);
}

/**
 * @brief       This function serves to set CRC value for RF.
 * @param[in]   init_value - The CRC init value to be set for RF.
 * @return      none.
 */
static inline void rf_set_crc_init_value(unsigned int init_value)
{
    reg_rf_crc_init = init_value;
}

/**
 * @brief       This function serves to set CRC polynomial for RF.
 * @param[in]   poly - The CRC polynomial to be set for RF.
 *              For example, to set CRC polynomial(BLE CRC-24):
 *              crc_poly = x^24 + x^10 + x^9 + x^6 + x^4 + x^3 + x + 1
 *              the crc_poly value would be 0x65b.
 * @return      none.
 */
static inline void rf_set_crc_poly(unsigned int poly)
{
    reg_rf_crc_poly = poly;
}

/**
 * @brief       This function serves to set CRC start byte for RF.
 * @param[in]   byte_pos - CRC start byte position, 0 is the first byte
 *              ble:The starting position of the PDU header.
 *              private:The starting position of the data packet
 *              zigbee:The starting position is the first byte after SHR.
 * @return      none.
 */
static inline void rf_set_crc_start_cal_byte_pos(unsigned char byte_pos)
{
    reg_rf_crc_config1 = (reg_rf_crc_config1 & (~FLD_RF_CRC_START_BYTE)) | byte_pos;
}

/**
 * @brief       This function serves to set the XOR value of the CRC result for RF.
 * @param[in]   xor_value - XOR mask for CRC result (for no mask, should be 0)
 * @return      none.
 */
static inline void rf_set_crc_xor_out(unsigned int xor_value)
{
    reg_rf_crc_xorout = xor_value;
}

/**
 * @brief       This function serves to set the CRC output byte order for RF.
 * @param[in]   order - CRC output byte order (1:MSByte first,0:LSByte first)
 * @return      none.
 */
static inline void rf_set_crc_byte_order(unsigned char order)
{
    reg_rf_crc_config2 = (reg_rf_crc_config2 & 0xf7) | (order << 3);
}

/**
 * @brief       This function serves to set crc for RF.
 * @param[in]   config - crc config address pointer
 * @return      none.
 */
static inline void rf_set_crc_config(const rf_crc_config_t *config)
{
    rf_set_crc_init_value(config->init_value);
    rf_set_crc_poly(config->poly);
    rf_set_crc_xor_out(config->xor_out);
    rf_set_crc_byte_order(config->byte_order);
    rf_set_crc_start_cal_byte_pos(config->start_cal_pos);
    rf_set_crc_len(config->len);
}

/**
 * @brief   This function serve to get tx wptr.
 * @param[in]   pipe_id -   The number of tx fifo.0<= pipe_id <=5.
 * @return      The write pointer of the tx.
 */
static inline unsigned char rf_get_tx_wptr(unsigned char pipe_id)
{
    if ((pipe_id > 5) && (pipe_id < 8)) {
        return reg_rf_dma_tx_wptr1((pipe_id - 6));
    } else {
        return reg_rf_dma_tx_wptr(pipe_id);
    }
}

/**
 * @brief   This function serve to update the wptr of tx terminal.
 * @param[in]   pipe_id -   The number of pipe which need to update wptr.
 * @param[in]   wptr    -   The pointer of write in tx terminal.
 * @return      none
 */
static inline void rf_set_tx_wptr(unsigned char pipe_id, unsigned char wptr)
{
    if ((pipe_id > 5) && (pipe_id < 8)) {
        reg_rf_dma_tx_wptr1(0) = wptr;
    } else {
        reg_rf_dma_tx_wptr(pipe_id) = wptr;
    }
}

/**
 * @brief   This function serve to clear the writer pointer of tx terminal.
 * @param[in]   pipe_id -   The number of tx DMA.0<= pipe_id <=5.
 * @return  none.
 */
static inline void rf_clr_tx_wptr(unsigned char pipe_id)
{
    if ((pipe_id > 5) && (pipe_id < 8)) {
        reg_rf_dma_tx_wptr1((pipe_id - 6)) = 0;
    } else {
        reg_rf_dma_tx_wptr(pipe_id) = 0;
    }
}

/**
 * @brief   This function serve to get ptx rptr.
 * @param[in]   pipe_id -The number of tx pipe.0<= pipe_id <=5.
 * @return      The read pointer of the tx.
 */
static inline unsigned char rf_get_tx_rptr(unsigned char pipe_id)
{
    if ((pipe_id > 5) && (pipe_id < 8)) {
        return reg_rf_dma_tx_rptr1((pipe_id - 6));
    } else {
        return reg_rf_dma_tx_rptr(pipe_id);
    }
}

/**
 * @brief   This function serve to clear read pointer of tx terminal.
 * @param[in]   pipe_id -   The number of tx DMA.0<= pipe_id <=5.
 * @return  none.
 */
static inline void rf_clr_tx_rptr(unsigned char pipe_id)
{
    if ((pipe_id > 5) && (pipe_id < 8)) {
        reg_rf_dma_tx_rptr1((pipe_id - 6)) = 0x80;
    } else {
        reg_rf_dma_tx_rptr(pipe_id) = 0x80;
    }
}

/**
 * @brief   This function serve to get the pointer of read in rx terminal.
 * @return  wptr    -   The pointer of rx_rptr.
 */
static inline unsigned char rf_get_rx_rptr(void)
{
    return reg_rf_dma_rx_rptr;
}

/**
 * @brief   This function serve to clear read pointer of rx terminal.
 * @return  none.
 */
static inline void rf_clr_rx_rptr(void)
{
    write_reg8(0x1708f5, 0x80); //clear rptr
}

/**
 * @brief   This function serve to get the pointer of write in rx terminal.
 * @return  wptr    -   The pointer of rx_wptr.
 */
static inline unsigned char rf_get_rx_wptr(void)
{
    return reg_rf_dma_rx_wptr;
}

/**
 * @brief     This function is used to select the RF modulation and demodulation rate.
 * @param[in] modem_rate - RF modulation and demodulation rate.
 * @return    none.
 * @note      There are two rate modes here, 24M is the default rate and 48M is the high rate mode.
 */
void rf_modem_rate_mode(rf_modem_rate_e modem_rate);

/**
 * @brief    This function is used to initialize the baseband related digital modules for n22
 * @return   none.
 * @note     Attention:
 *           (1)This interface should be called before rf_mode_init.
 *           (2)This interface should be called after sys_n22_init.
 */
void rf_n22_dig_init(void);

/**
 * @brief      This function serves to initiate information of RF.
 * @return     none.
 */
void rf_mode_init(void);

/**
 * @brief     This function serves to set RF tx DMA setting.
 * @param[in] none
 * @return    none.
 */
void rf_set_tx_dma_config(void);

/**
 * @brief     This function serves to set RF tx DMA setting.
 * @param[in] fifo_depth        - tx chn deep,fifo_depth range: 0~5,Number of fifo=2^fifo_depth.
 * @param[in] fifo_byte_size    - The length of one dma fifo,the range is 1~0xffff(the corresponding number of fifo bytes is fifo_byte_size).
 * @return    none.
 */
void rf_set_tx_dma(unsigned char fifo_depth, unsigned short fifo_byte_size);

/**
 * @brief      This function serves to rx dma setting.
 * @param[in]  buff - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 * @attention  The first four bytes in the buffer of the received data are the length of the received data.
 *             The actual buffer size that the user needs to set needs to be noted on two points:
 *             -# you need to leave 4bytes of space for the length information.
 *             -# dma is transmitted in accordance with 4bytes, so the length of the buffer needs to be a multiple of 4. Otherwise, there may be an out-of-bounds problem
 *             For example, the actual received data length is 5bytes, the minimum value of the actual buffer size that the user needs to set is 12bytes, and the calculation of 12bytes is explained as follows::
 *             4bytes (length information) + 5bytes (data) + 3bytes (the number of additional bytes to prevent out-of-bounds)
 * @param[in]  wptr_mask       - This parameter is used to set the mask value for the number of enabled FIFOs. The value of the mask must (0x00,0x01,0x03,0x07,0x0f,0x1f).
 *                               The number of FIFOs enabled is the value of wptr_mask plus 1.(0x01,0x02,0x04,0x08,0x10,0x20)
 * @param[in]  fifo_byte_size  - The length of one dma fifo,the range is 1~0xffff(the corresponding number of fifo bytes is fifo_byte_size).
 * @return     none.
 */
void rf_set_rx_dma(unsigned char *buff, unsigned char wptr_mask, unsigned short fifo_byte_size);

/**
 * @brief       This function serve to rx dma config
 * @param[in]   none
 * @return      none
 */
void rf_set_rx_dma_config(void);

/**
 * @brief     This function serves to trigger srx on.
 * @param[in] tick  - Trigger rx receive packet after tick delay.
 * @return    none.
 */
void rf_start_srx(unsigned int tick);


/**
 * @brief       This function serves to get latched rssi.
 * @return      rssi value.
 */
signed char rf_get_latched_rssi(void);

/**
 * @brief       This function serves to get the real time rssi.
 * @return      rssi value.
 */
signed char rf_get_real_time_rssi(void);

/**
 * @brief       This function serves to set RF Tx mode.
 * @return      none.
 */
void rf_set_txmode(void);


/**
 * @brief       This function serves to set RF Tx packet address to DMA src_addr.
 * @param[in]   addr   - The packet address which to send.
 * @return      none.
 * @note        addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
_attribute_ram_code_sec_ void rf_tx_pkt(void *addr);


/**
 * @brief       This function serves to set rf channel for all mode.The actual channel set by this function is 2400+chn.
 * @param[in]   chn   - That you want to set the channel as 2400+chn.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void rf_set_chn(signed char chn);

/**
 * @brief       This function serves to get the right fifo packet.
 * @param[in]   fifo_num   - The number of fifo set in dma.
 * @param[in]   fifo_dep   - deepth of each fifo set in dma.
 * @param[in]   addr       - address of rx packet.
 * @return      the next rx_packet address.
 * @note        addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
unsigned char *rf_get_rx_packet_addr(int fifo_num, int fifo_dep, void *addr);


/**
 * @brief       This function serves to set RF power level.
 * @param[in]   level    - The power level to set.
 * @return      none.
 */
void rf_set_power_level(rf_power_level_e level);


/**
 * @brief       This function serves to set RF power through select the level index.
 * @param[in]   idx      - The index of power level which you want to set.
 * @return      none.
 */
void rf_set_power_level_index(rf_power_level_index_e idx);

/**
 * @brief       This function serves to get RF status.
 * @return      RF Rx/Tx status.
 */
rf_status_e rf_get_trx_state(void);

/**
 * @brief       This function serves to judge RF Tx/Rx state.
 * @param[in]   rf_status   - Tx/Rx status.
 * @param[in]   rf_channel  - This param serve to set frequency channel(2400+rf_channel) .
 * @return      Whether the setting is successful(-1:failed;else success).
 */
int rf_set_trx_state(rf_status_e rf_status, signed char rf_channel);

/**
 * @brief       This function serves to update the value of internal cap.
 * @param[in]   value   - The value of internal cap which you want to set.
 * @return      none.
 */
void rf_update_internal_cap(unsigned char value);

/**
 * @brief       This function serves to close internal cap;
 * @return      none.
 */
void rf_turn_off_internal_cap(void);

/**
 * @brief       This function serve to change the length of preamble.
 * @param[in]   len     -The value of preamble length.Set the register bit<0>~bit<4>.
 * @return      none
 */
void rf_set_preamble_len(unsigned char len);

/**
 * @brief       This function serve to set the length of access code.
 * @param[in]   byte_len    -   The value of access code length,the range is 3~5byte.
 * @return      none
 */
void rf_set_access_code_len(unsigned char byte_len);

/**
 * @brief       This function serves to set RF rx timeout.
 * @param[in]   timeout_us  -   rx_timeout after timeout_us us,The maximum of this param is 0xfff.
 * @return      none.
 */
static inline void rf_set_rx_timeout(unsigned short timeout_us)
{
    reg_rf_rx_timeout = timeout_us - 1;
}

/**
 * @brief   This function serves to disable RF rx timeout.
 * @return  none.
 */
static inline void rf_dis_rx_timeout(void)
{
    reg_rf_ll_ctrl_1 &= ~(FLD_RF_RX_TIMEOUT_EN);
}

/**
 * @brief       This function serves to RF trigger stx
 * @param[in]   addr    - DMA tx buffer.
 * @param[in]   tick    - Send after tick delay.
 * @return      none.
 * @note        addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
_attribute_ram_code_sec_noinline_ void rf_start_stx(void *addr, unsigned int tick);


/**
 * @brief       This function serves to RF trigger stx2rx.
 * @param[in]   addr  - DMA tx buffer.
 * @param[in]   tick  - Trigger tx send packet after tick delay.
 * @return      none.
 * @note        addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
_attribute_ram_code_sec_noinline_ void rf_start_stx2rx(void *addr, unsigned int tick);

/**
 * @brief       This function serves to set RF Rx manual on.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void rf_set_rxmode(void);

/**
 * @brief       This function serves to RF trigger srx2rx.
 * @param[in]   addr  - DMA tx buffer.
 * @param[in]   tick  - Trigger rx receive packet after tick delay.
 * @return      none.
 * @note        addr:must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
_attribute_ram_code_sec_noinline_ void rf_start_srx2tx(void *addr, unsigned int tick);

/**
 * @brief       This function is used to judge whether there is a CRC error in the received packet through hardware.
 *              For the same packet, the value of this bit is consistent with the CRC flag bit in the packet.
 * @param[in]   none.
 * @return      none.
 */
unsigned char rf_get_crc_err(void);

/**
 * @brief      This function serves to set the max length of rx packet.Use byte_len to limit what DMA
 *             moves out will not exceed the buffer size we define.And old chip do this through dma size.
 * @param[in]  byte_len  - The longest of rx packet.
 * @return     none.
 */
static inline void rf_set_rx_maxlen(unsigned int byte_len)
{
    reg_rf_rxtmaxlen0 = byte_len & 0xff;
    reg_rf_rxtmaxlen1 = (byte_len >> 8) & 0xff;
}

/**
 * @brief       This function serves to disable pn of rf mode.
 * @return      none.
 */
void rf_pn_disable(void);

/**********************************************************************************************************************
 *  Fast settle related interfaces
 *  Attention:
 *  (1)This part of the function is only for the internal use of the driver, not open to customers to use,
 *  we will rewrite this part, and provide demo
 *  (2)When using TL321X fast settle, it should be noted that different settle times correspond to different calibration modules being turned off,
 *     so you need to manually configure the calibration values of these calibration modules before using fast settle.
 *  (3)Calibration modules that require manual setting of calibration values:
 *     RX_SETTLE_TIME_15US: ldo trim; rx_fcal; dcoc;
 *     RX_SETTLE_TIME_37US: ldo trim; dcoc;
 *     RX_SETTLE_TIME_77US: ldo trim;
 *
 *     TX_SETTLE_TIME_15US: ldo trim; tx_fcal; hpmc;
 *     TX_SETTLE_TIME_51US: ldo trim; hpmc;
 *     TX_SETTLE_TIME_104US: ldo trim;
 *
 *********************************************************************************************************************/

/**
 * @brief      This function serve to adjust tx/rx settle timing sequence.
 * @param[in]  tx_settle_us    After adjusting the timing sequence, the time required for tx to settle.
 * @param[in]  rx_settle_us    After adjusting the timing sequence, the time required for rx to settle.
 * @return     none
 */
void rf_fast_settle_config(rf_tx_fast_settle_time_e tx_settle_us, rf_rx_fast_settle_time_e rx_settle_us);


/**
 *  @brief      This function serve to enable the tx timing sequence adjusted.
 *  @param[in]  none
 *  @return     none
*/
void rf_tx_fast_settle_en(void);

/**
 *  @brief      This function serve to disable the tx timing sequence adjusted.
 *  @param[in]  none
 *  @return     none
*/
void rf_tx_fast_settle_dis(void);

/**
 *  @brief      This function serve to enable the rx timing sequence adjusted.
 *  @param[in]  none
 *  @return     none
*/
void rf_rx_fast_settle_en(void);

/**
 *  @brief      This function serve to disable the rx timing sequence adjusted.
 *  @param[in]  none
 *  @return     none
 *  @note        This interface needs to be called after rf mode initialization (e.g. after rf_set_ble_1M_NO_PN_mode)
*/
void rf_rx_fast_settle_dis(void);

/**
 *  @brief      This function is mainly used to get rccal Calibration-related values.
 *  @param[in]  rccal_cal  - rccal calibration value address pointer
 *  @return     none
 */
void rf_get_rccal_cal_val(rf_rccal_cal_t *rccal_cal);

/**
 *  @brief      This function is mainly used to set rccal Calibration-related values.
 *  @param[in]  rccal_cal    - rccal Calibration-related values.
 *  @return     none
 */
void rf_set_rccal_cal_val(rf_rccal_cal_t rccal_cal);
/**
 *  @brief      This function is used to set the tx fast_settle calibration value.
 *  @param[in]  tx_settle_us    After adjusting the timing sequence, the time required for tx to settle.
 *  @param[in]  chn             Calibrates the frequency (2400 + chn). Range: 0 to 80. Applies to TX_SETTLE_TIME_15US and TX_SETTLE_TIME_59US, other parameters are invalid.
 *                              (When tx_settle_us is 15us or 59us, the modules to be calibrated are frequency-dependent, so all used frequency points need to be calibrated.)
 *  @return     none
*/
void rf_tx_fast_settle_update_cal_val(rf_tx_fast_settle_time_e tx_settle_time, unsigned char chn);

/**
 *  @brief      This function is used to set the rx fast_settle calibration value.
 *  @param[in]  rx_settle_us    After adjusting the timing sequence, the time required for rx to settle.
 *  @param[in]  chn             Calibrates the frequency (2400 + chn). Range: 0 to 80. Applies to RX_SETTLE_TIME_15US, other parameters are invalid.
 *                              (When rx_settle_us is 15us, the modules to be calibrated are frequency-dependent, so all used frequency points need to be calibrated.)
 *  @return     none
*/
void rf_rx_fast_settle_update_cal_val(rf_rx_fast_settle_time_e rx_settle_time, unsigned char chn);

/**
 *  @brief      This function is used to get the tx fast_settle calibration value.
 *  @param[in]  tx_settle_us    After adjusting the timing sequence, the time required for tx to settle.
 *  @param[in]  chn             Calibrates the frequency (2400 + chn). Range: 0 to 80. Applies to TX_SETTLE_TIME_15US and TX_SETTLE_TIME_59US, other parameters are invalid.
 *                              (When tx_settle_us is 15us or 59us, the modules to be calibrated are frequency-dependent, so all used frequency points need to be calibrated.)
 *  @param[in]  fs_cv           Fast settle calibration value address pointer.
 *  @return     none
*/
void rf_tx_fast_settle_get_cal_val(rf_tx_fast_settle_time_e tx_settle_time, unsigned char chn, rf_fast_settle_t *fs_cv);

/**
 *  @brief      This function is used to set the tx fast_settle calibration value.
 *  @param[in]  tx_settle_us    After adjusting the timing sequence, the time required for tx to settle.
 *  @param[in]  chn             Calibrates the frequency (2400 + chn). Range: 0 to 80. Applies to TX_SETTLE_TIME_15US and TX_SETTLE_TIME_59US, other parameters are invalid.
 *                              (When tx_settle_us is 15us or 59us, the modules to be calibrated are frequency-dependent, so all used frequency points need to be calibrated.)
 *  @param[in]  fs_cv           Fast settle calibration value address pointer.
 *  @return     none
*/
void rf_tx_fast_settle_set_cal_val(rf_tx_fast_settle_time_e tx_settle_time, unsigned char chn, rf_fast_settle_t *fs_cv);

/**
 *  @brief      This function is used to get the rx fast_settle calibration value.
 *  @param[in]  rx_settle_us    After adjusting the timing sequence, the time required for rx to settle.
 *  @param[in]  chn             Calibrates the frequency (2400 + chn). Range: 0 to 80. Applies to RX_SETTLE_TIME_15US, other parameters are invalid.
 *                              (When rx_settle_us is 15us, the modules to be calibrated are frequency-dependent, so all used frequency points need to be calibrated.)
 *  @param[in]  fs_cv           Fast settle calibration value address pointer.
 *  @return     none
*/
void rf_rx_fast_settle_get_cal_val(rf_rx_fast_settle_time_e rx_settle_time, unsigned char chn, rf_fast_settle_t *fs_cv);

/**
 *  @brief      This function is used to set the rx fast_settle calibration value.
 *  @param[in]  rx_settle_us    After adjusting the timing sequence, the time required for rx to settle.
 *  @param[in]  chn             Calibrates the frequency (2400 + chn). Range: 0 to 80. Applies to RX_SETTLE_TIME_15US, other parameters are invalid.
 *                              (When rx_settle_us is 15us, the modules to be calibrated are frequency-dependent, so all used frequency points need to be calibrated.)
 *  @param[in]  fs_cv           Fast settle calibration value address pointer.
 *  @return     none
*/
void rf_rx_fast_settle_set_cal_val(rf_rx_fast_settle_time_e rx_settle_time, unsigned char chn, rf_fast_settle_t *fs_cv);

/**
 * @brief      This function serves to set the tx wait time during the rx2tx process
 * @param[in]  tx_wait_us  tx wait time,the unit is us.The max value of this param is 0xfff; The default wait time value is 10us.
 * @return     none.
 * @note       Attention:It is not necessary to call this function to adjust the wait time in the rx2tx process.
 */
static inline void rf_set_tx_wait_time(unsigned short tx_wait_us)
{
    if (tx_wait_us > 0x0fff) {
        tx_wait_us = 0x0fff;
    }
    reg_rf_ll_txwait = (reg_rf_ll_txwait & 0xf000) | (tx_wait_us - 1);
}

/**
 * @brief      This function serves to set the rx wait time during the tx2rx process
 * @param[in]  rx_wait_us  rx wait time,the unit is us.The max value of this param is 0xfff; The default wait time value is 10us.
 * @return     none.
 * @note       Attention:It is not necessary to call this function to adjust the wait time in the tx2rx process.
 */
static inline void rf_set_rx_wait_time(unsigned short rx_wait_us)
{
    if (rx_wait_us > 0x0fff) {
        rx_wait_us = 0x0fff;
    }
    reg_rf_ll_rxwait = (reg_rf_ll_rxwait & 0xf000) | (rx_wait_us - 1);
}

/**
 * @brief       This function serve to set the length of access code.
 * @param[in]   bit_len -   The value of access code match threshold ,the range is 0~40bit.
 * @return      none
 */
void rf_set_acc_match_threshold(unsigned char bit_len);

/**
 * @brief      This function serves to reset RF digital logic states.
 * @return     none
 * @note       This function requires setting reset zb, rstl_bb, and rst_mdm.
 *             It is used to clear RF related state machines, IRQ states, and digital internal logic states.
 */
_attribute_ram_code_sec_noinline_ void rf_clr_dig_logic_state(void);

/**
 * @brief      This function is used to restore the rf related registers to their default values.
 * @return     none
 * @note       (1)After calling this interface, all configured interfaces of rf need to be called again.
 *             (2)After calling this interface, the tick of bb timer will be reset to zero.
 *             (3)After calling this interface, RF DMA configurations need to be reconfigured.
 */
_attribute_ram_code_sec_noinline_ void rf_reset_register_value(void);

/**
 * @brief          This function is mainly used to set the energy when sending a single carrier.
 * @param[in]    level        - The slice corresponding to the energy value.
 * @return         none.
 */
void rf_set_power_level_singletone(rf_power_level_e level);

/**
 * @brief       This function serves to set rf fpga channel for all mode.The actual channel set by this function is 2400+chn.
 * @param[in]   chn
 * @return      none.
 */
void rf_fpga_set_chn(signed char chn);

void rf_set_fcal_value(unsigned char fcal_value);

/**
 * @brief       This function is used to enable the ldo rxtxlf bypass function, and the calibration value
 *              written by the software will take effect after enabling.
 * @param[in]   none.
 * @return      none.
 */
void rf_ldot_ldo_rxtxlf_bypass_en(void);

/**
 * @brief       This function is used to close the ldo rxtxlf bypass function, and the hardware will
 *              automatically perform the calibration function after closing.
 * @param[in]   none.
 * @return      none.
 * @note        Since this function distinguishes between RF modes, the function call must be made after the RF mode configuration function.
 */
void rf_ldot_ldo_rxtxlf_bypass_dis(void);

/**
 * @brief      This function serves to optimize RF performance
 *             This function must be called every time rx is turned on,
 *             and is called by an internal function.
 *             If there are other requirements that need to be called,
 *             turn off rx first, then call it again to make sure the Settings take effect
 * @param[in]  none
 * @return     none
 * @note       1.Call this function after turning on rx 30us, and the calibration value set by the function
 *                will take effect after calling rf_ldot_ldo_rxtxlf_bypass_en;if automatic calibration is
 *                required, you can use rf_ldot_ldo_rxtxlf_bypass_dis to turn off the bypass function; how to
 *                use it can refer to bqb.c file or rf_emi_rx in emi.c
 *             2. After using rf_ldot_ldo_rxtxlf_bypass_dis to turn off the bypass function and enter tx/rx
 *                automatic calibration, to use this function again, you need to call the rf_set_rxpara function
 *                again after entering rx 30us.
 *
 */
void rf_set_rxpara(void);

/**
 * @brief       Asymmetric PHY rate switching
 * @param[in]   dir     Direction to switch
 *                      - PHY_SWITCH_1M_TO_2M: Upgrade TX to 2Mbps
 *                      - PHY_SWITCH_2M_TO_1M: Downgrade TX to 1Mbps
 * @param[in]   enable  Asymmetric mode enable
 *                      - true:  Asymmetric mode (only TX changes, RX unchanged)
 *                      - false: Symmetric mode (TX and RX change together)
 * @return      None
 *
 * @note        - RF transceiver is disabled during the switch
 *              - Allow 80μs settling time after switching
 *              - Ensure no ongoing packet transfer before calling
 *
 * @code
 * // Enable asymmetric: TX=2M, RX=1M
 * rf_phy_switch_asymmetric(ASYM_1M_TO_2M, true);
 * @endcode
 */
_attribute_ram_code_sec_noinline_ void rf_phy_switch_asymmetric(rf_phy_switch_dir_e dir, _Bool enable);

/**
 * @brief      This interface is used to configure the BLE debug port IO.
 * @return     none.
 * @note       This function is only used for BLE mode debugging and testing.
 *              PA1  - ble_tx_en
 *              PA2  - ble_tx_on
 *              PA3  - ble_rx_en
 *              PA5  - ble_rx_hit_sync
 *              PA8  - ble_rx_data_vld
 */
void rf_set_ble_bb_debugport(void);

#endif


