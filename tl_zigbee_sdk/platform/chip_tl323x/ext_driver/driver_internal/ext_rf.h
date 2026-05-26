/********************************************************************************************************
 * @file    ext_rf.h
 *
 * @brief   This is the header file for BLE SDK
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
#ifndef DRIVERS_EXT_DRIVER_EXT_RF_H_
#define DRIVERS_EXT_DRIVER_EXT_RF_H_


#include "types.h"
#include "lib/include/rf/rf_common.h"



#define RF_RX_WAIT_MIN_VALUE            (0)

#ifndef RF_ACCESS_CODE_DEFAULT_THRESHOLD
#define RF_ACCESS_CODE_DEFAULT_THRESHOLD    (31)    //0x1e  . BQB may use 32. Coded PHY may use 0xF0
#endif


#ifndef FAST_SETTLE
#define FAST_SETTLE         0
#endif

#define TX_FAST_SETTLE_LEVEL  TX_SETTLE_TIME_15US
#define RX_FAST_SETTLE_LEVEL  RX_SETTLE_TIME_15US


#if  FAST_SETTLE
#define TX_FAST_SETTLE_TIME  15 //59   //Minimum support time for settle
#define RX_FAST_SETTLE_TIME  15 //37   //Minimum support time for settle
#define RF_RX_WAIT_MIN_VALUE  (125)
#else
//TODO
//TX_FAST_SETTLE_TIME Temporary use. Later, wait for it to be merged into "fast settle". Undergoing modification.  @ronglu
#define TX_FAST_SETTLE_TIME  92   //Minimum support time for settle
#define RX_FAST_SETTLE_TIME  85    //Minimum support time for settle
#define RF_RX_WAIT_MIN_VALUE  (0)
#endif

#ifndef BLE_S2_S8_NEW_PATH
#define BLE_S2_S8_NEW_PATH      0
#endif
enum{
    //BLE mode
    FLD_RF_BRX_SN_INIT          =   BIT(4),
    FLD_RF_BRX_NESN_INIT        =   BIT(5),
    FLD_RF_BTX_SN_INIT          =   BIT(6),
    FLD_RF_BTX_NESN_INIT        =   BIT(7),
};

//*********************Note: CRYPT*****************************/

//*********************Note: must write and read one word*****************************/

#define RF_CLEAR_ALL_IRQ_MASK               reg_rf_irq_mask = 0;reg_rf_irq_mask1=0
#define DMA_RFRX_LEN_HW_INFO                0   // 826x: 8
#define DMA_RFRX_OFFSET_HEADER              4   // 826x: 12
#define DMA_RFRX_OFFSET_RFLEN               5   // 826x: 13
#define DMA_RFRX_OFFSET_DATA                6   // 826x: 14

#define RF_TX_PACKET_DMA_LEN(rf_data_len)       (((rf_data_len)+3)/4)|(((rf_data_len) % 4)<<22)
#define DMA_RFRX_OFFSET_CRC24(p)            (p[DMA_RFRX_OFFSET_RFLEN]+6)  //data len:3
#define DMA_RFRX_OFFSET_TIME_STAMP(p)       (p[DMA_RFRX_OFFSET_RFLEN]+9)  //data len:4
#define DMA_RFRX_OFFSET_FREQ_OFFSET(p)      (p[DMA_RFRX_OFFSET_RFLEN]+13) //data len:2
#define DMA_RFRX_OFFSET_RSSI(p)             (p[DMA_RFRX_OFFSET_RFLEN]+15) //data len:1, signed
#define DMA_RFRX_OFFSET_STATUS(p)           (p[DMA_RFRX_OFFSET_RFLEN]+16)

#define RF_BLE_RF_PAYLOAD_LENGTH_OK(p)                  ( *((unsigned int*)p) == (unsigned int)(p[5]+13))               //dma_len must 4 byte aligned
#define RF_BLE_RF_PACKET_CRC_OK(p)                      ((p[(p[5]+5 + 11)] & 0x01) == 0x0)
#define RF_BLE_PACKET_VALIDITY_CHECK(p)                 (RF_BLE_RF_PAYLOAD_LENGTH_OK(p) && RF_BLE_RF_PACKET_CRC_OK(p))

#define RF_BLE_RF_PACKET_CRC_OK_HW_ECC(p)                       ((p[p[5]+5+11-4] & 0x01) == 0x0)

#define rf_set_tx_packet_address(addr)      rf_dma_set_src_address(RF_TX_DMA,(unsigned int)(addr));




//Current RF RX DMA buffer point for BLE
extern  unsigned char *ble_curr_rx_dma_buff;
_attribute_ram_code_sec_noinline_ void ble_rf_set_rx_dma(unsigned char *buff, unsigned char size_div_16);

_attribute_ram_code_sec_noinline_ void ble_rf_set_tx_dma(unsigned char fifo_dep, unsigned char size_div_16);

_attribute_ram_code_sec_noinline_ void ble_rx_dma_config(void);

// rf_get_internal_cap used read register reference is contained in rf_update_internal_cap......
#define  rf_get_internal_cap()   analog_read_reg8(0x10a)

/**
 * @brief   This function serves to settle adjust for RF Tx.This function for adjust the differ time
 *          when rx_dly enable.
 * @param   txstl_us - adjust TX settle time.
 * @return  none
 */
static inline void  rf_ble_set_tx_settle(unsigned short txstl_us)
{
    reg_rf_ll_tx_stl = txstl_us;
}

/* attention that not bigger than 4095 */
static inline void rf_ble_set_rx_settle( unsigned short rx_stl_us )
{
//     write_reg16(0x17020c, rx_stl_us);
    reg_rf_ll_rxstl_l = rx_stl_us & 0xFF;
    reg_rf_ll_rxstl_h = (rx_stl_us >> 8) & 0xFF;
}

static inline void  rf_ble_set_rx_wait(unsigned short rx_wait_us)
{
//    write_reg8(0x170206, rx_wait_us);
    reg_rf_ll_rxwait = rx_wait_us;
}

static inline void  rf_ble_set_tx_wait(unsigned short tx_wait_us)
{
    tx_wait_us = (tx_wait_us>0x0fff ? 0x0fff : tx_wait_us);
    reg_rf_ll_txwait = tx_wait_us;
}


/**
 * @brief     This function performs to set RF Access Code Threshold.// use for BQB
 * @param[in] threshold   cans be 0-32bits
 * @return    none.
 */
#define rf_ble_set_access_code_threshold(threshold)    (reg_rf_modem_sync_thres_ble = threshold)

/**
 * @brief   This function serves to set RF access code value.
 * @param[in]   ac - the address value.
 * @return  none
 */
static inline void rf_set_ble_access_code_value (unsigned int ac)
{
    write_reg32 (reg_rf_access_code_base_pipe0, ac);
}

/**
 * @brief   This function serves to set RF access code.
 * @param[in]   p - the address to access.
 * @return  none
 */
static inline void rf_set_ble_access_code (unsigned char *p)
{
    write_reg32 (reg_rf_access_code_base_pipe0, p[3] | (p[2]<<8) | (p[1]<<16) | (p[0]<<24));
}


/**
 * @brief   This function serves to set RF access code advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_ble_access_code_adv (void)
{
    write_reg32 (reg_rf_access_code_base_pipe0, 0xd6be898e);
}


/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 *******************need driver change
 */
static inline void reset_sn_nesn(void)
{
    reg_rf_ll_rest_pid  =  0x01;
}




/**
 * @brief   This function serves to trigger accesscode in coded Phy mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_trigger_codedPhy_accesscode(void)
{
    write_reg8(0x170425,read_reg8(0x170425)|0x01);
}

/**
 * @brief     This function performs to enable RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_on (void)
{
    reg_rf_ll_ctrl0 = 0x45; // reset tx/rx state machine.
    reg_rf_ll_ctrl0 |= FLD_RF_R_TX_EN_MAN;

}

/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_done (void)
{
    reg_rf_ll_ctrl0 = 0x45;                          // reset tx/rx state machine.
}

/**
 * @brief   This function serves to set RX first timeout value.
 * @param[in]   tm - timeout, unit: uS.
 * @return  none.
 */
static inline void rf_set_1st_rx_timeout(unsigned int tm)
{
    reg_rf_ll_rx_fst_timeout = tm;
}


/**
 * @brief   This function serves to set RX timeout value.
 * @param[in]   tm - timeout, unit: uS, range: 0 ~ 0xfff
 * @return  none.
 */
static inline void rf_ble_set_rx_timeout(u16 tm)
{
    reg_rf_rx_timeout = tm;
}




typedef enum{
    FSM_BTX     = 0x81,
    FSM_BRX     = 0x82,
    FSM_PTX     = 0x83,
    FSM_PRX     = 0x84,
    FSM_STX     = 0x85,
    FSM_SRX     = 0x86,
    FSM_TX2RX   = 0x87,
    FSM_RX2TX   = 0x88,
}fsm_mode_e;


/**
 * @brief       This function serves to RF trigger RF state machine.
 * @param[in]   mode  - FSM mode.
 * @param[in]   tx_addr  - DMA TX buffer, if not TX, must be "NULL"
 * @param[in]   tick  - FAM Trigger tick.
 * @return      none.
 */
void rf_start_fsm(fsm_mode_e mode, void* tx_addr, unsigned int tick);

void ext_ble_rf_init_rx_sensitivity_optimize(void);

/**
 * @brief       This function serves to set RF baseband channel.This function is suitable for ble open PN mode.
 * @param[in]   chn_num  - Bluetooth channel set according to Bluetooth protocol standard.
 * @return      none.
 */
extern void ext_ble_rf_state_channel_rx_sensitivity_optimize(signed char freq);
#define rf_set_ble_channel(num)   do{ \
                                  ext_ble_rf_state_channel_rx_sensitivity_optimize(num);\
                                  rf_set_ble_chn(num); \
                                  }while(0);

enum{
    FLD_RF_SN                     = BIT(0),
};

//the register reg_rf_ll_2d_sclk  BIT(7) indicate NESN. This is different from tercel.
enum{
    FLD_RF_NESN                   = BIT(7),
};

/**
 * @brief    This function serves to enable zb_rt interrupt source.
 * @return  none
 */
static inline void zb_rt_irq_enable(void)
{
    plic_interrupt_enable(IRQ_ZB_RT);
}


/*
 * SiHui & QingHua & SunWei sync with Xuqiang.Zhang & Zhiwei.Wang & Kaixin.Chen & Shujuan.chu
 * B91/B92
 * TX settle recommend value by digital team: 108.5uS without fast settle;   108.5-58.5=50 with fast settle
 * we BLE use 110 without fast settle; 110-57=53 with fast settle, here 53 = real settle 45uS + extra 1 preamble 8uS(1M for example)
 *
 * RX settle recommend value by digital team: 108.5uS without fast settle;   85-40=45 with fast settle
 */


#define RX_SETTLE_US                    RX_FAST_SETTLE_TIME

#if  FAST_SETTLE
#define RXSET_OPTM_ANTI_INTRF               20   //RX settle value for optimize anti-interference
#else
#define RXSET_OPTM_ANTI_INTRF               100  //RX settle value for optimize anti-interference
#endif

#define PRMBL_LENGTH_1M                     2   //preamble length for 1M PHY
#define PRMBL_LENGTH_2M                     4   //preamble length for 2M PHY
#define PRMBL_LENGTH_Coded                  10  //preamble length for Coded PHY, never change this value !!!

#define PRMBL_EXTRA_1M                      (PRMBL_LENGTH_1M - 1)   // 1 byte for 1M PHY
#define PRMBL_EXTRA_2M                      (PRMBL_LENGTH_2M - 2)   // 2 byte for 2M
#define PRMBL_EXTRA_Coded                   (PRMBL_LENGTH_Coded - 10)                       // 10byte for Coded, 80uS, no extra byte


#if RF_RX_SHORT_MODE_EN//open rx dly


    // TODO: zhihan, -2 is used for scanrsp TIFS, which is 147us before.
    // after fast settle merged, this issue will be solved.
    #define         TX_STL_LEGADV_SCANRSP_REAL                      (TX_FAST_SETTLE_TIME + PRMBL_EXTRA_1M*8 - 2)  //can change, consider TX packet quality
    #define         TX_STL_LEGADV_SCANRSP_SET                       TX_FAST_SETTLE_TIME


    #define         TX_STL_TIFS_REAL_COMMON                         (TX_FAST_SETTLE_TIME)

    #define         TX_STL_TIFS_REAL_1M                             (TX_FAST_SETTLE_TIME + PRMBL_EXTRA_1M*8)
    #define         TX_STL_TIFS_SET_1M                              TX_FAST_SETTLE_TIME

    #define         TX_STL_TIFS_REAL_2M                             (TX_FAST_SETTLE_TIME + PRMBL_EXTRA_2M*4)
    #define         TX_STL_TIFS_SET_2M                              TX_FAST_SETTLE_TIME

    #define         TX_STL_TIFS_REAL_CODED                          TX_FAST_SETTLE_TIME
    #define         TX_STL_TIFS_SET_CODED                           TX_STL_TIFS_REAL_CODED
    #if(PRMBL_EXTRA_Coded)
    #error "TX_STL_TIFS_SET_CODED needs to be configured"
    #endif

    #define         TX_STL_ADV_REAL_COMMON                          TX_FAST_SETTLE_TIME + PRMBL_EXTRA_1M*8

    #define         TX_STL_ADV_REAL_1M                              (TX_FAST_SETTLE_TIME + PRMBL_EXTRA_1M*8)
    #define         TX_STL_ADV_SET_1M                               TX_FAST_SETTLE_TIME

    #define         TX_STL_ADV_REAL_2M                              (TX_FAST_SETTLE_TIME + PRMBL_EXTRA_2M*4)
    #define         TX_STL_ADV_SET_2M                               TX_FAST_SETTLE_TIME

    #define         TX_STL_ADV_REAL_CODED                           TX_STL_ADV_REAL_COMMON
    #define         TX_STL_ADV_SET_CODED                            TX_STL_ADV_REAL_CODED  //can not change !!!


    #define         TX_STL_AUTO_MODE_1M                             (127 - PRMBL_EXTRA_1M * 8 - 11)
    #define         TX_STL_AUTO_MODE_2M                             (129 - PRMBL_EXTRA_2M * 4)
#if(BLE_S2_S8_NEW_PATH)
    #define         TX_STL_AUTO_MODE_CODED_S2                       115
    #define         TX_STL_AUTO_MODE_CODED_S8                       119
#else
    #define         TX_STL_AUTO_MODE_CODED                          127
#endif


    #if (FAST_SETTLE)
        #define     TX_STL_BTX_1ST_PKT_REAL                         TX_FAST_SETTLE_TIME //110 - 57 = 53
    #else
        /* normal mode(no fast settle): for ACL Central, tx settle real 110uS or 107uS or even 105uS, not big difference,
         * but for CIS Central, timing is very urgent considering T_MSS between two sub_event, so SiHui use 107, we keep this set
         * fast settle mode:  */
        #define     TX_STL_BTX_1ST_PKT_REAL                         (110 - 3) //3 is total switch delay time
    #endif

    #define         TX_STL_BTX_1ST_PKT_SET_1M                       (TX_STL_BTX_1ST_PKT_REAL - PRMBL_EXTRA_1M * 8)
    #define         TX_STL_BTX_1ST_PKT_SET_2M                       (TX_STL_BTX_1ST_PKT_REAL - PRMBL_EXTRA_2M * 4)
    #define         TX_STL_BTX_1ST_PKT_SET_CODED                    TX_STL_BTX_1ST_PKT_REAL
#else
    #error "add code here, TX settle time"
#endif


#if(1) //(LL_FEATURE_ENABLE_FRAME_SPACE_UPDATE)
    #define         RX_PATH_DLY_EXTRA_PREAMBLE_1M                   (150 - TX_STL_AUTO_MODE_1M)
    #define         RX_PATH_DLY_EXTRA_PREAMBLE_2M                   (150 - TX_STL_AUTO_MODE_2M)

    #if(BLE_S2_S8_NEW_PATH)
        #define         RX_PATH_DLY_EXTRA_PREAMBLE_S2               (150-TX_STL_AUTO_MODE_CODED_S2)
        #define         RX_PATH_DLY_EXTRA_PREAMBLE_S8               (150-TX_STL_AUTO_MODE_CODED_S8)
    #else
        #define         RX_PATH_DLY_EXTRA_PREAMBLE_CODED            (150- TX_STL_AUTO_MODE_CODED)
    #endif
#endif



/* AD convert delay : timing cost on RF analog to digital convert signal process:
 *                  Eagle   1M: 20uS       2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 *                  Jaguar  1M: 20uS       2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 */
#define AD_CONVERT_DLY_1M                                           19  //before:20. Jaguar T_IFS need 32M + AD_Convert=19, tested by kai.jia at 2022-11-17
#define AD_CONVERT_DLY_2M                                           10
#define AD_CONVERT_DLY_CODED                                        14

#define OTHER_SWITCH_DELAY_1M                                       0
#define OTHER_SWITCH_DELAY_2M                                       2
#if(BLE_S2_S8_NEW_PATH)
#define OTHER_SWITCH_DELAY_CODED                                    9
#else
#define OTHER_SWITCH_DELAY_CODED                                    2
#endif


#define HW_DELAY_1M                                                 (AD_CONVERT_DLY_1M + OTHER_SWITCH_DELAY_1M)
#define HW_DELAY_2M                                                 (AD_CONVERT_DLY_2M + OTHER_SWITCH_DELAY_2M)
#define HW_DELAY_CODED                                              (AD_CONVERT_DLY_CODED + OTHER_SWITCH_DELAY_CODED)

#define rf_ble_set_1m_phy()        rf_set_ble_1M_mode()

#define rf_ble_set_2m_phy()        rf_set_ble_2M_mode()

#define rf_ble_set_coded_phy_common()   ((void) 0)

#define rf_ble_set_coded_phy_s2()  rf_set_ble_500K_mode()

#define rf_ble_set_coded_phy_s8()  rf_set_ble_125K_mode()



#if 1
//RF BLE Minimum TX Power LVL (unit: 1dBm)
extern const char  ble_rf_min_tx_pwr;
//RF BLE Maximum TX Power LVL (unit: 1dBm)
extern const char  ble_rf_max_tx_pwr;
//RF BLE Current TX Path Compensation
extern  signed short ble_rf_tx_path_comp;
//RF BLE Current RX Path Compensation
extern  signed short ble_rf_rx_path_comp;
#define  ble_txPowerLevel   0
static inline u8 rf_ble_get_tx_pwr_idx(s8 rfTxPower)
{
    rf_power_level_index_e rfPwrLvlIdx;

    /*VBAT*/
    if      (rfTxPower >=   9)  {  rfPwrLvlIdx = RF_POWER_INDEX_P9p00dBm;  }
    else if (rfTxPower >=   8)  {  rfPwrLvlIdx = RF_POWER_INDEX_P8p00dBm;  }
    else if (rfTxPower >=   7)  {  rfPwrLvlIdx = RF_POWER_INDEX_P7p00dBm;  }
    else if (rfTxPower >=   6)  {  rfPwrLvlIdx = RF_POWER_INDEX_P6p00dBm;  }
    /*VANT*/
    else if (rfTxPower >=   5)  {  rfPwrLvlIdx = RF_POWER_INDEX_P5p00dBm;  }
    else if (rfTxPower >=   4)  {  rfPwrLvlIdx = RF_POWER_INDEX_P4p00dBm;  }
    else if (rfTxPower >=   3)  {  rfPwrLvlIdx = RF_POWER_INDEX_P3p00dBm;  }

    return rfPwrLvlIdx;
}

static inline s8 rf_ble_get_tx_pwr_level(rf_power_level_index_e rfPwrLvlIdx)
{
    s8 rfTxPower;

    /*VBAT*/
    if      (rfPwrLvlIdx <= RF_POWER_INDEX_P9p00dBm)  {  rfTxPower =   9;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P8p00dBm)  {  rfTxPower =   8;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P7p00dBm)  {  rfTxPower =   7;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P6p00dBm)  {  rfTxPower =   6;  }
    /*VANT*/
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P5p00dBm)  {  rfTxPower =   5;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P4p00dBm)  {  rfTxPower =   4;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P3p00dBm)  {  rfTxPower =   3;  }
    else                                              {  rfTxPower = -23;  }

    return rfTxPower;
}
#endif

#if FAST_SETTLE
    typedef struct
    {
        unsigned short cal_tbl[81];
        rf_ldo_trim_t   ldo_trim;
        rf_dcoc_cal_t   dcoc_cal;
        rf_rccal_cal_t  rccal_cal;
        unsigned char  tx_fcal[81];
        unsigned char  rx_fcal[81];
        unsigned char   fcal[81];
        unsigned char   tx_fast_en;
        unsigned char   rx_fast_en;
    }Fast_Settle;
    extern Fast_Settle fast_settle_1M;
    extern Fast_Settle fast_settle_2M;
    extern Fast_Settle fast_settle_S2;
    extern Fast_Settle fast_settle_S8;


#endif

typedef struct {
    unsigned char  rfMode_init_flag;
    unsigned char  txPower_index;
    unsigned char  txPower_level;   /*!< added to be compatible with driver api */
}ext_rf_t;
extern ext_rf_t blt_extRF;




#endif /* DRIVERS_EXT_DRIVER_EXT_RF_H_ */
