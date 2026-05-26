/********************************************************************************************************
 * @file    ext_lib.c
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
#include <string.h>
#include "../../gpio.h"
#include "../../lib/include/pm/pm.h"
#include "lib/include/rf/rf_common.h"
#include "ext_lib.h"
#include "../ext_misc.h"


 /******************************* debug_start ***************************************************************/
/**
 * @brief      This function writes a byte data to analog register
 * @param[in]  addr - the address of the analog register needs to write
 * @param[in]  value  - the data will be written to the analog register
 * @param[in]  e - the end address of value
 * @param[in]  s - the start address of the value
 * @return     none
 */
void sub_wr_ana(unsigned int addr, unsigned char value, unsigned char e, unsigned char s)
{
    unsigned char v, mask, tmp1, target, tmp2;

    v = analog_read(addr);
    mask = BIT_MASK_LEN(e - s + 1);
    tmp1 = value & mask;

    tmp2 = v & (~BIT_RNG(s,e));

    target = (tmp1 << s) | tmp2;
    analog_write(addr, target);
}

/**
 * @brief      This function writes a byte data to a specified analog register
 * @param[in]  addr - the address of the analog register needs to write
 * @param[in]  value  - the data will be written to the analog register
 * @param[in]  e - the end address of value
 * @param[in]  s - the start address of the value
 * @return     none
 */
void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s)
{
    unsigned char v, mask, tmp1, target, tmp2;

    v = read_reg8(addr);
    mask = BIT_MASK_LEN(e - s + 1);
    tmp1 = value & mask;

    tmp2 = v & (~BIT_RNG(s,e));

    target = (tmp1 << s) | tmp2;
    write_reg8(addr, target);
}

 /******************************* debug_end ****************************************************************/

/******************************* dbgport start ******************************************************************/
void dbg_bb_set_pin(gpio_pin_e pin)
{
    unsigned char mask = 0x3F;//bit5~bit0
    unsigned char val = 0x4d;//function select DBG_BB
    //note:  setting pad the function  must before  setting no_gpio function, cause it will lead to uart transmit extra one byte data at begin.(confirmed by minghai&sunpeng)
    reg_gpio_func_mux(pin) = (reg_gpio_func_mux(pin) & mask) | val;
    gpio_function_dis(pin);
}
void rf_enable_bb_debug(void)
{
    REG_ADDR8(0x170003) |= BIT(5);//default 0x0
    REG_ADDR8(0x140ca1) &= 0x0F;        // 140379[7:4] = 0
    REG_ADDR8(0x140ca1) |= BIT(0);        // 140379[0] = 1; if not set, B2/B3 not work.
    REG_ADDR8(0x140ca0) |= BIT(1);        // 140378[1] = 1
    REG_ADDR8(0x140ca0) |= BIT(2);        // 140378[2] = 1
    REG_ADDR8(0x140ca0) &= ~BIT(5);    // 140378[5] = 0

    dbg_bb_set_pin(GPIO_PA0); //tx_en
    dbg_bb_set_pin(GPIO_PA1); //tx_on
    dbg_bb_set_pin(GPIO_PA2); //rx_en
    dbg_bb_set_pin(GPIO_PA3); //clk_bb
    dbg_bb_set_pin(GPIO_PA4); //hit_sync

    dbg_bb_set_pin(GPIO_PB0); //sclk
    dbg_bb_set_pin(GPIO_PB1); //tx_data
    dbg_bb_set_pin(GPIO_PB2); //rx_data_vld
    dbg_bb_set_pin(GPIO_PB3); //rx_data
}
/******************************* dbgport end ********************************************************************/

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 */
void gpio_setup_up_down_resistor(gpio_pin_e gpio, gpio_pull_type up_down)
{
    ///////////////////////////////////////////////////////////
    //        PA[3:0]           PA[7:4]         PB[3:0]         PB[7:4]     PC[3:0]         PC[7:4]
    // sel: ana_0x17<7:0>    ana_0x18<7:0>  ana_0x19<7:0>  ana_0x1a<7:0>  ana_0x1b<7:0>  ana_0x1c<7:0>
    //        PD[3:0]           PD[7:4]         PE[3:0]         PE[7:4]     PF[3:0]         PF[7:4]
    // sel: ana_0x1d<7:0>    ana_0x1e<7:0>  ana_0x1f<7:0>  ana_0x20<7:0>  ana_0x21<7:0>  ana_0x22<7:0>
    unsigned char r_val = up_down & 0x03;

    unsigned char base_ana_reg = 0;
    if((gpio>>8)<6)//A-E
    {
         base_ana_reg = 0x17 + ((gpio >> 8) << 1) + ((gpio & 0xf0) ? 1 : 0 );
    }
    else{
        return;
    }
    unsigned char shift_num, mask_not;

    if(gpio & 0x11){
        shift_num = 0;
        mask_not = 0xfc;
    }
    else if(gpio & 0x22){
        shift_num = 2;
        mask_not = 0xf3;
    }
    else if(gpio & 0x44){
        shift_num = 4;
        mask_not = 0xcf;
    }
    else if(gpio & 0x88){
        shift_num = 6;
        mask_not = 0x3f;
    }
    else{
        return;
    }
    analog_write_reg8(base_ana_reg, (analog_read_reg8(base_ana_reg) & mask_not) | (r_val << shift_num));
}
