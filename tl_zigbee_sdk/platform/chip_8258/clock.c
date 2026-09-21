/********************************************************************************************************
 * @file    clock.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
#include "clock.h"
#include "register.h"
#include "bsp.h"
#include "analog.h"
#include "timer.h"
#include "lib/include/pm.h"
#include "compiler.h"

_attribute_data_retention_ unsigned char system_clk_type;
_attribute_data_retention_ unsigned char system_clk_mHz;

/**
 * @brief       This function to set RC for the system clock.
 * @param[in]   SYS_CLK - the clock source of the system clock.
 * @return      none
 */
void clock_rc_set(SYS_CLK_TypeDef SYS_CLK)
{
    unsigned char temp = analog_read(0x04) & 0xfc;
    if (SYS_CLK == SYS_CLK_24M_RC) {
        analog_write(0x04, temp | 0x00);
        rc_24m_cal();
    } else if(SYS_CLK == SYS_CLK_32M_RC) {
        analog_write(0x04, temp | 0x01);
        //rc_32m_cal();
    } else if(SYS_CLK == SYS_CLK_48M_RC) {
        analog_write(0x04, temp | 0x03);
        rc_48m_cal();
    }
}

/**
 * @brief       This function to select the system clock source.
 * @param[in]   SYS_CLK - the clock source of the system clock.
 * @return      none
 * @note		1. Do not switch the clock during the DMA sending and receiving process because during the clock switching process, 
 * 					the system clock will be suspended for a period of time, which may cause data loss.
 * 				2. When this function called after power on or deep sleep wakeup, it will perform 24m rc calibration. 
 * 					The usage rules of 24m rc, please refer to the rc_24m_cal() for details.
 * 					If do not want this logic, you can check the usage and precautions of clock_init_calib_24m_rc_cfg().
 */
void clock_init(SYS_CLK_TypeDef SYS_CLK)
{
    reg_clk_sel = (unsigned char)SYS_CLK;
    system_clk_type = (unsigned char)SYS_CLK;

    switch (SYS_CLK) {
        case SYS_CLK_12M_Crystal:
            system_clk_mHz = 12;
            break;
        case SYS_CLK_16M_Crystal:
            system_clk_mHz = 16;
            break;
        case SYS_CLK_24M_Crystal:
        case SYS_CLK_24M_RC:
            system_clk_mHz = 24;
            break;
        case SYS_CLK_32M_Crystal:
        case SYS_CLK_32M_RC:
            system_clk_mHz = 32;
            break;
        case SYS_CLK_48M_Crystal:
        case SYS_CLK_48M_RC:
            system_clk_mHz = 48;
            break;
        default:
            return;
    }

#if (SYSCLK_RC_CLOCK_EN)
    if(SYS_CLK == SYS_CLK_48M_Crystal || SYS_CLK == SYS_CLK_48M_RC)
#else
    if(SYS_CLK == SYS_CLK_48M_Crystal)
#endif
    {
        /*default c4: dcdc 1.8V  -> GD flash 48M clock may error need higher DCDC voltage
                  c6: dcdc 1.9V
        */
        analog_write(0x0c, 0xc6);
    }

#if (SYSCLK_RC_CLOCK_EN)
    if (SYS_CLK < SYS_CLK_RC_THRES) {
        clock_rc_set(SYS_CLK);
    }
#endif
}

/**
 * @brief   This function serves to set 32k clock source.
 * @param[in]   variable of 32k type.
 * @return  none.
 */
void clock_32k_init(CLK_32K_TypeDef src)
{
    unsigned char sel_32k   = analog_read(0x2d) & 0x7f;
    unsigned char power_32k = analog_read(0x05) & 0xfc;
    analog_write(0x2d, sel_32k | (src << 7));
    if (src) {
        analog_write(0x05, power_32k | 0x1);//32k xtal
        //2.set pc3 as pwm output
        unsigned char sys_clk = read_reg8(0x66);
        write_reg8(0x66, 0x43);
        unsigned char reg_596 = read_reg8(0x596);
        write_reg8(0x596, reg_596 & 0xf7);
        unsigned short reg_798 = read_reg16(0x798);
        write_reg16(0x798, 0x01);
        unsigned short reg_79a = read_reg16(0x79a);
        write_reg16(0x79a, 0x02);
        unsigned char reg_780 = read_reg8(0x780);
        write_reg8(0x780, 0x02);
        write_reg8(0x782, 0xf3);

        //3.wait for PWM wake up Xtal
        sleep_ms(5);

        //4.Xtal 32k output
        analog_write(0x03, 0x4f); //<7:6>current select

        //5.Recover PC3 as Xtal pin
        write_reg8(0x66, sys_clk);
        write_reg8(0x596, reg_596);
        write_reg16(0x798, reg_798);
        write_reg16(0x79a, reg_79a);
        write_reg8(0x780, reg_780);
    } else {
        analog_write(0x05, power_32k | 0x2);//32k rc
    }
}

/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   kick_ms - duration of kick.
 * @return  	none.
 */
_attribute_no_inline_ static void pwm_kick_32k_pad(unsigned int kick_ms)
{
    //2.set pc3 as pwm output
    write_reg8(0x66,  0x43); //sys clk use 16M crystal
    write_reg8(0x596, 0xf7); //pc3 as pwm func
    write_reg32(0x798, 0x20001); //pwm1's high time or low time && pwm1's cycle time
    write_reg8(0x780, 0x02); //enable pwm1
    write_reg8(0x782, 0xf3); //pwm clk div

    //3.wait for PWM wake up Xtal

    sleep_us(kick_ms * 1000);

    //4.Recover PC3 as Xtal pin
    write_reg8(0x66,   0x06); //default value
    write_reg8(0x596,  0xff); //default value
    write_reg32(0x798, 0x00); //default value
    write_reg8(0x780,  0x00); //default value
    write_reg8(0x782,  0x00); //default value
}

/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   times - the frequency of being kicked by pwm.
 * @return  	none.
 */
_attribute_no_inline_ void pwm_kick_32k_pad_times(unsigned int times)
{
    if (times) {
        //1. select 32k xtal
        analog_write(0x2d, 0x95);//32k select:[7]:0 sel 32k rc,1:32k XTAL

        #if 0 //must close, reason is as follows:
            /*
             * This problem occurs with suspend and deep and deep retention. When the power supply voltage is low, suspend/deep/deep retention cannot
             * be reset within 12ms, otherwise softstart will work. However, there is not enough delay to wait for softstart to complete. This delay
             * will be postponed to the code execution area and it will not be able to handle larger code, otherwise it will be dropped by 1.8v, load
             * error, and finally stuck.(Root: DCDC dly depends on the 32k rc clock, so the 32k rc power supply can't be turned off here.)
             */
            analog_write(0x05, 0x01);//Power down 32KHz RC,  Power up [32KHz crystal, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]
        #else
            analog_write(0x05, 0x00);//Power up 32KHz RC,  Power up 32KHz crystal
        #endif

        //analog_write(0x03,0x4f); //<7:6>current select(default value, not need)

        int last_32k_tick;
        int curr_32k_tick;
        int i = 0;
        for (i = 0; i< times; i++) {
            //After 10ms, the external 32k crystal clk is considered stable(when using PWM to accelerate the oscillation process)
            pwm_kick_32k_pad(10);//PWM kick external 32k pad (duration 10ms)

            //Check if 32k pad vibration and basically works stably
            last_32k_tick = pm_get_32k_tick();

            sleep_us(305);//for 32k tick accumulator, tick period: 30.5us, dly 10 ticks

            curr_32k_tick = pm_get_32k_tick();

            if (last_32k_tick != curr_32k_tick) { //pwm kick 32k pad success
                break;
            }
        }
    }
}

/**
 * @brief     This function performs to select 48M RC as the system clock source.
 * @param[in] none.
 * @return    none.
 */
void rc_48m_cal(void)
{
    analog_write(0x33, 0x80);
    analog_write(0x30, 0x20);
    analog_write(0xc7, 0x0e);
    sleep_us(1000);
    analog_write(0xc7, 0x0f);
    while((analog_read(0xcf) & 0x80) == 0);

    volatile unsigned int cal_cnt = analog_read(0xcf) & 0x07;
    cal_cnt = (cal_cnt << 8) + analog_read(0xce);
    unsigned int f = 64;
    unsigned int temp_v = 0;
    unsigned int temp_d = 0;
    unsigned int temp_d2 = 100;
    unsigned char temp_cap = 0;
    unsigned int i = 0;
    while (f >= 1) {
        temp_v = analog_read(0x33);
    	if (cal_cnt > 250) {
            temp_d = cal_cnt - 250;
    	} else {
            temp_d = 250 - cal_cnt;
    	}
    	if (cal_cnt > 250) {
            analog_write(0x33, temp_v - f);
    	} else {
            analog_write(0x33, temp_v+f);
    	}
    	f = f / 2;
        analog_write(0xc7, 0x0e);
        analog_write(0xc7, 0x0f);
        while((analog_read(0xcf) & 0x80) == 0);
        cal_cnt = analog_read(0xcf) & 0x07;
        cal_cnt = (cal_cnt << 8) + analog_read(0xce);
        i++;
    	if (temp_d2 > temp_d) {
            temp_d2 = temp_d;
            temp_cap = temp_v;
    	}
    }
    analog_write(0x33, temp_cap);
}

/**
 * @brief     This function performs to select 24M as the system clock source.
 * @param[in] none.
 * @return    none.
 * @note	  During the first power-on, after the xtal is stable (cpu_wakeup_init()), it is necessary to calibrate the 24m rc as soon as possible 
 * 				to prevent some unknown problems caused by a large frequency deviation of the RC clock.
 *            1. If the sleep function is not used and the accuracy of 24m rc is not high, then there is no need for regular calibration.
 *            2. If the sleep wake-up function is required, it is necessary to calibrate the 24m rc before the first sleep, otherwise it may cause the 
 * 					oscillator to fail to start after waking up.The recommended interval for regular calibration is 10 seconds. 
 *            3. If the 24m rc is more accurate, the oscillator will start up faster after waking up. If it is not accurate, the oscillator may not start
 * 					up after waking up.Therefore, regular calibration is needed to prevent the impact of temperature changes.
 */
void rc_24m_cal(void)
{
    analog_write(0xc8, 0x80);

    //sub_wr_ana(0x30, 1, 7, 7);
    analog_write(0x30, analog_read(0x30) | BIT(7));

    analog_write(0xc7, 0x0e);
    analog_write(0xc7, 0x0f);
    while((analog_read(0xcf) & 0x80) == 0);
    unsigned char cap = analog_read(0xcb);
    analog_write(0x33, cap);		//write 24m cap into manual register

    //sub_wr_ana(0x30, 0, 7, 7);	//manual on
    analog_write(0x30, analog_read(0x30) & (~BIT(7)));

    analog_write(0xc7, 0x0e);
    tl_24mrc_cal = analog_read(0x33);
}

/**
 * @brief     This function performs to select 32K as the system clock source.
 * @param[in] none.
 * @return    none.
 * @note	  1. If a more accurate 32K RC timing is required, then to prevent temperature effects, calibration can be performed regularly.
 * 			  2. If it is to ensure accurate sleep time, then the 32K RC calibration is not necessary. Although sleep time is measured by 32K RC, 
 * 				    sleep time is obtained through tracking way and will not affected by 32K RC deviation. So in this scenario, it is necessary to 
 * 				    calibrate once when power-on (to prevent significant frequency deviation caused by 32K RC), and regular calibration is not necessary.
 */
void rc_32k_cal(void)
{
    analog_write(0x30, 0x60);
    analog_write(0xc6, 0xf6);
    analog_write(0xc6, 0xf7);
    while((analog_read(0xcf) & BIT(6)) == 0);
    unsigned char res1 = analog_read(0xc9);	//read 32k res[13:6]
    analog_write(0x32, res1);		//write 32k res[13:6] into manual register
    unsigned char res2 = analog_read(0xca);	//read 32k res[5:0]
    analog_write(0x31, res2);		//write 32k res[5:0] into manual register
    analog_write(0xc6, 0xf6);
    analog_write(0x30, 0x20);//manual on
}

/**
 * @brief     This function performs to select 32K as source of DMIC.
 * @param[in] source clock to provide DMIC.
 * @return    none.
 */
void dmic_prob_32k(unsigned char src)
{
    write_reg8(0x73, read_reg8(0x73) | 0x02);			// sel dmic src
    write_reg8(0x73, (read_reg8(0x73) & 0xfe) | src);    // sel 32k src
    write_reg8(0x586, read_reg8(0x586) & 0xfd);
    write_reg8(0x5a8, read_reg8(0x5a8) & 0xf3);
}

/**
 * @brief     This function performs to calibration the source clock.
 * @param[in] none.
 * @return    none.
 * @note	  You cannot call the calibration function when using a 48M clock.
 * 			  For example, it cannot be calibrated when the system clock is 48M/32M, otherwise it may crash.
 * 			  USB clock is 48M. Do not calibrate during USB packet sending and receiving; otherwise, the communication may be abnormal.
 */

void doubler_calibration(void)
{
     analog_write(0x86, 0xbb);//power on duty cycle cal module
     analog_write(0x82, analog_read(0x82) & 0x7f);
     analog_write(0x87, (analog_read(0x87) & 0xfc) | 0x02);
     analog_write(0x87, analog_read(0x87) | 0x04);
     unsigned char tmp = 0;
     unsigned char dcal = 0;
     tmp = analog_read(0x88);
     dcal= tmp & 0x1f;
     analog_write(0x87, (analog_read(0x87) & 0x07) | (dcal << 3));
     analog_write(0x82, analog_read(0x82) | 0x80);
     analog_write(0x87, analog_read(0x87) & 0xfd);
     analog_write(0x86, 0xfb);
}

/**
 * @brief     This function performs to select 24M/2 RC as source of DMIC.
 * @param[in] source clock to provide DMIC.
 * @return    none.
 */
void dmic_prob_24M_rc(void)
{
    //probe 24M/2 RC
    //PD[5] select dmic clk
    write_reg8(0x586, 0x1f);
    write_reg8(0x794, 0x01);
    write_reg8(0x796, 0x02);
    write_reg8(0x781, 0x01);
    write_reg8(0x59e, 0x5b);
}

