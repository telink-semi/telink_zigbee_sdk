/********************************************************************************************************
 * @file    gpio.c
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
#include "gpio.h"
#include "analog.h"

/**
 * @brief      This function servers to set the GPIO's MUX function.
 * @param[in]  pin - the special pin.
 * @param[in]  func - the function of GPIO.
 * @return     none.
 */
static void gpio_set_mux(GPIO_PinTypeDef pin, GPIO_FuncTypeDef func)
{
    //config gpio mux
    unsigned char val = 0;
    unsigned char mask = 0xff;

    switch (pin) {
        case GPIO_PA0:
        {
            //0x5a8[1:0]
            //0. DMIC_DI
            //1. PWM0_N
            //2. UART_RX
            mask = (unsigned char)~(BIT(1) | BIT(0));
            if (func == AS_DMIC) {

            } else if (func == AS_PWM0_N) {
                val = BIT(0);
            } else if(func == AS_UART) {
                val = BIT(1);
            }
        }
        break;

        case GPIO_PA1:
        {
            //0x5a8[3:2]
            //0. DMIC_CLK
            //1. UART_7816_CLK
            //2. I2S_CLK
            mask = (unsigned char)~(BIT(3) | BIT(2));
            if (func == AS_DMIC) {

            } else if (func == AS_UART) {
                val = BIT(2);
            } else if (func == AS_I2S) {
                val = BIT(3);
            }
        }
        break;

        case GPIO_PA2:
        {
            //0x5a8[5:4]
            //0. DO
            //1. UART_TX
            //2. PWM0
            mask = (unsigned char)~(BIT(5) | BIT(4));
            if (func == AS_SPI) {

            } else if (func == AS_UART) {
                val = BIT(4);
            } else if (func == AS_PWM0) {
                val = BIT(5);
            }
        }
        break;

        case GPIO_PA3:
        {
            //0x5a8[7:6]
            //0. SPI_DI / I2C_SDA
            //1. UART_CTS
            //2. PWM1
            mask = (unsigned char)~(BIT(7) | BIT(6));
            if (func == AS_SPI || func == AS_I2C) {

            } else if (func == AS_UART) {
                val = BIT(6);
            } else if (func == AS_PWM1) {
                val = BIT(7);
            }
        }
        break;

        case GPIO_PA4:
        {
            //0x5a9[1:0]
            //0. SPI_DO / I2C_SCL
            //1. UART_RTS
            //2. PWM2
            mask = (unsigned char)~(BIT(1) | BIT(0));
            if (func == AS_SPI || func == AS_I2C) {

            } else if (func == AS_UART) {
                val = BIT(0);
            } else if (func == AS_PWM2) {
                val = BIT(1);
            }
        }
        break;

        case GPIO_PA5:
        case GPIO_PA6:
        {
            //only USB, no need set special func, but we can DM/DP input enable here
             gpio_set_input_en(pin, 1);
        }
        break;

        case GPIO_PA7:
        {
            //0x5a9[7:6]
            //0. SWS:
            //1. UART_RTS
            mask = (unsigned char)~(BIT(7) | BIT(6));
            if (func == AS_SWIRE) {

            } else if (func == AS_UART) {
                val = BIT(6);
            }
        }
        break;

        case GPIO_PB0:
        {
            //0x5aa[1:0]
            //0. PWM3
            //1. UART_RX
            //2. ATSEL1
            mask = (unsigned char)~(BIT(1) | BIT(0));
            if (func == AS_PWM3) {

            }else if (func == AS_UART) {
                val = BIT(0);
            } else if (func == AS_ATS) {
                val = BIT(1);
            }
        }
        break;

        case GPIO_PB1:
        {
            //0x5aa[3:2]
            //0. PWM4
            //1. UART_TX
            //2. ATSEL2
            mask = (unsigned char)~(BIT(3) | BIT(2));
            if (func == AS_PWM4) {

            } else if (func == AS_UART) {
                val = BIT(2);
            } else if(func == AS_ATS) {
                val = BIT(3);
            }
        }
        break;

        case GPIO_PB2:
        {
            //0x5aa[5:4]
            //0. PWM5
            //1. UART_CTS
            //2. RX_CYC2LNA
            mask = (unsigned char)~(BIT(5) | BIT(4));
            if (func == AS_PWM5) {

            } else if (func == AS_UART) {
                val = BIT(4);
            }
            //else if(func == AS_CYC){
            //    val = BIT(5);
            //}
            else if (func==AS_RX_CYC2LNA) {
                val = BIT(6);
            }
        }
        break;

        case GPIO_PB3:
        {
            //0x5aa[7:6]
            //0. PWM0_N
            //1. UART_RTS
            //2. TX_CYC2PA
            mask = (unsigned char)~(BIT(7) | BIT(6));
            if (func == AS_PWM0_N) {

            } else if (func == AS_UART) {
                val = BIT(6);
            } else if (func == AS_TX_CYC2PA) {
                val = BIT(7);
            }
        }
        break;

        case GPIO_PB4:
        {
            //0x5ab[1:0]
            //0. SDM_P0
            //1. PWM4
            //2. CMP_D
            mask = (unsigned char)~(BIT(1) | BIT(0));
            if (func == AS_SDM) {

            } else if (func == AS_PWM4) {
                val = BIT(0);
            } else if (func == AS_CMP) {
                val = BIT(1);
            }
        }
        break;

        case GPIO_PB5:
        {
            //0x5ab[3:2]
            //0. SDM_N0
            //1. PWM5
            //2. CMP_DF
            mask = (unsigned char)~(BIT(3) | BIT(2));
            if (func == AS_SDM) {

            } else if (func == AS_PWM5) {
                val = BIT(2);
            } else if (func == AS_CMP) {
                val = BIT(3);
            }
        }
        break;

        case GPIO_PB6:
        {
            //0x5ab[5:4]
            //0. SDM_P1
            //1. SPI_DI/I2C_SDA
            //2. UART_RTS
            mask = (unsigned char)~(BIT(5) | BIT(4));
            if (func == AS_SDM) {

            } else if (func == AS_SPI || func == AS_I2C) {
                val = BIT(4);
            } else if(func == AS_UART) {
                val = BIT(5);
            }
        }
        break;

        case GPIO_PB7:
        {
            //0x5ab[7:6]
            //0. SDM_N1
            //1. SPI_DO
            //2. UART_RX
            mask = (unsigned char)~(BIT(7) | BIT(6));
            if (func == AS_SDM) {

            } else if (func == AS_SPI) {
                val = BIT(6);
            } else if(func == AS_UART) {
                val = BIT(7);
            }
        }
        break;

        case GPIO_PC0:
        {
            //0x5ac[1:0]
            //0. I2C_SDA
            //1. PWM4_N
            //2. UART_RTS
            mask = (unsigned char)~(BIT(1) | BIT(0));
            if (func == AS_I2C) {

            } else if (func == AS_PWM4_N) {
                val = BIT(0);
            } else if (func == AS_UART) {
                val = BIT(1);
            }
        }
        break;

        case GPIO_PC1:
        {
            //0x5ac[3:2]
            //0. I2C_SCK
            //1. PWM1_N
            //2. PWM0
            mask = (unsigned char)~(BIT(3) | BIT(2));
            if (func == AS_I2C) {

            } else if (func == AS_PWM1_N) {
                val = BIT(2);
            } else if (func == AS_PWM0) {
                val = BIT(3);
            }
        }
        break;

        case GPIO_PC2:
        {
            //0x5ac[5:4]
            //0. PWM0
            //1. 7816_TRX
            //2. I2C_SDA
            mask = (unsigned char)~(BIT(5) | BIT(4));
            if (func == AS_PWM0) {

            } else if (func == AS_UART) {
                val = BIT(4);
            } else if (func == AS_I2C) {
                val = BIT(5);
            }
        }
        break;

        case GPIO_PC3:
        {
            //0x5ac[7:6]
            //0. PWM1
            //1. UART_RX
            //2. I2C_SCK
            mask = (unsigned char)~(BIT(7) | BIT(6));
            if (func == AS_PWM1) {

            } else if (func == AS_UART) {
                val = BIT(6);
            } else if (func == AS_I2C) {
                val = BIT(7);
            }
        }
        break;

        case GPIO_PC4:
        {
            //0x5ad[1:0]
            //0. PWM2
            //1. UART_CTS
            //2. PWM0_N
            mask = (unsigned char)~(BIT(1) | BIT(0));
            if (func == AS_PWM2) {

            } else if (func == AS_UART) {
                val = BIT(0);
            } else if (func == AS_PWM0_N) {
                val = BIT(1);
            }
        }
        break;

        case GPIO_PC5:
        {
            //0x5ad[3:2]
            //0. PWM3_N
            //1. UART_RX
            //2. ATSEL_0
            mask = (unsigned char)~(BIT(3) | BIT(2));
            if (func == AS_PWM3_N) {

            } else if (func == AS_UART) {
                val = BIT(2);
            } else if(func == AS_ATS) {
                val = BIT(3);
            }
        }
        break;

        case GPIO_PC6:
        {
            //0x5ad[5:4]
            //0. RX_CYC2LNA
            //1. ATSEL1
            //2. PWM4_N
            mask = (unsigned char)~(BIT(5)|BIT(4));

            if (func == AS_ATS) {
                val = BIT(4);
            } else if (func == AS_PWM4_N) {
                val = BIT(5);
            } else if (func == AS_RX_CYC2LNA) {
                val=0;
            }
        }
        break;

        case GPIO_PC7:
        {
            //0x5ad[7:6]
            //0. TX_CYC2PA
            //1. ATSEL2
            //2. PWM5_N
            mask = (unsigned char)~(BIT(7) | BIT(6));

            if (func == AS_ATS) {
                val = BIT(6);
            } else if (func == AS_PWM5_N) {
                val = BIT(7);
            } else if (func==AS_TX_CYC2PA) {
                val = 0;
            }
        }
        break;

        case GPIO_PD0:
        {
            //0x5ae[1:0]
            //0. RX_CYC2LNA
            //1. CMP_D
            //2. 7816_TRX
            mask = (unsigned char)~(BIT(1) | BIT(0));

            if (func == AS_CMP) {
                val = BIT(0);
            } else if(func == AS_UART) {
                val = BIT(1);
            } else if(func== AS_RX_CYC2LNA) {
                val = 0;
            }
        }
        break;

        case GPIO_PD1:
        {
            //0x5ae[3:2]
            //0. TX_CYC2PA
            //1. CMP_DF
            //2. UART_CTS
            mask = (unsigned char)~(BIT(3) | BIT(2));

            if (func == AS_CMP) {
                val = BIT(2);
            } else if (func == AS_UART) {
                val = BIT(3);
            } else if (func == AS_TX_CYC2PA) {
                val = 0;
            }
        }
        break;

        case GPIO_PD2:
        {
            //0x5ae[5:4]
            //0. SPI_CN
            //1. I2S_LR
            //2. PWM3
            mask = (unsigned char)~(BIT(5) | BIT(4));
            if (func == AS_SPI) {

            } else if (func == AS_I2S) {
                val = BIT(4);
            } else if (func == AS_PWM3) {
                val = BIT(5);
            }
        }
        break;

        case GPIO_PD3:
        {
            //0x5ae[7:6]
            //0. PWM1_N
            //1. I2S_SDI
            //2. UART_TX7816
            mask = (unsigned char)~(BIT(7) | BIT(6));
            if (func == AS_PWM1_N) {

            } else if (func == AS_I2S) {
                val = BIT(6);
            } else if (func == AS_UART) {
                val = BIT(7);
            }
        }
        break;

        case GPIO_PD4:
        {
            //0x5af[1:0]
            //0. SWM
            //1. I2S_SDO
            //2. PWM2_N
            mask = (unsigned char)~(BIT(1) | BIT(0));
            if (func == AS_SWIRE) {

            } else if (func == AS_I2S) {
                val = BIT(0);
            } else if (func == AS_PWM2_N) {
                val = BIT(1);
            }
        }
        break;

        case GPIO_PD5:
        {
            //0x5af[3:2]
            //0. PWM0
            //1. CMP_D
            //2. PWM0_N
            mask = (unsigned char)~(BIT(3) | BIT(2));
            if (func == AS_PWM0) {

            } else if(func == AS_CMP) {
                val = BIT(2);
            } else if(func == AS_PWM0_N) {
                val = BIT(3);
            }
        }
        break;

        case GPIO_PD6:
        {
            //0x5af[5:4]
            //0. CN
            //1. UART_RX
            //2. ATSEL0
            mask = (unsigned char)~(BIT(5) | BIT(4));
            if (func == AS_SPI) {

            } else if (func == AS_UART) {
                val = BIT(4);
            } else if (func == AS_ATS) {
                val = BIT(5);
            }
        }
        break;

        case GPIO_PD7:
        {
            //0x5af[7:6]
            //0. SPI_CK/I2C_SCL
            //1. I2S_BCK
            //2. 7816_TRX
            mask = (unsigned char)~(BIT(7) | BIT(6));
            if (func == AS_SPI || func == AS_I2C) {

            } else if (func == AS_I2S) {
                val = BIT(6);
            } else if (func == AS_UART) {
                val = BIT(7);
            }
        }
        break;

        default:
            break;
    }

    unsigned short reg = 0x5a8 + ((pin >> 8) << 1) + ((pin & 0x0f0) ? 1 : 0);
    write_reg8(reg, (read_reg8(reg) & mask) | val);
}

/**
 * @brief      This function servers to set the GPIO's function.
 * @param[in]  pin - the special pin.
 * @param[in]  func - the function of GPIO.
 * @return     none.
 */
/**Steps to set GPIO as a multiplexing function is as follows.
 * Step 1: Set GPIO as a multiplexing function.
 * Step 2: Disable GPIO function.
 * NOTE: Failure to follow the above steps may result in risks.
 */
void gpio_set_func(GPIO_PinTypeDef pin, GPIO_FuncTypeDef func)
{
    unsigned char bit = pin & 0xff;
    if (func > AS_GPIO) {
        gpio_set_mux(pin, func);
    }
    if (func == AS_GPIO) {
        BM_SET(reg_gpio_func(pin), bit);
        return;
    } else {
        BM_CLR(reg_gpio_func(pin), bit);
    }
}

/**
 * @brief      This function set the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function
 * @param[in]  value - enable or disable the pin's input function(0: disable, 1: enable)
 * @return     none
 */
void gpio_set_input_en(GPIO_PinTypeDef pin, unsigned int value)
{
    unsigned char bit = pin & 0xff;
    unsigned short group = pin & 0xf00;

    if (group == GPIO_GROUPA || group == GPIO_GROUPD || group == GPIO_GROUPE) {
        if (value) {
            BM_SET(reg_gpio_ie(pin), bit);
        } else {
            BM_CLR(reg_gpio_ie(pin), bit);
        }
    } else if (group == GPIO_GROUPB) {
    	if (value) {
            analog_write(areg_gpio_pb_ie, analog_read(areg_gpio_pb_ie) | bit);
    	} else {
            analog_write(areg_gpio_pb_ie, analog_read(areg_gpio_pb_ie) & (~bit));
    	}
    } else if (group == GPIO_GROUPC) {
    	if (value) {
            analog_write(areg_gpio_pc_ie, analog_read(areg_gpio_pc_ie) | bit);
    	} else {
            analog_write(areg_gpio_pc_ie, analog_read(areg_gpio_pc_ie) & (~bit));
    	}
    }
}

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 */
void gpio_setup_up_down_resistor(GPIO_PinTypeDef gpio, GPIO_PullTypeDef up_down)
{
    unsigned char r_val = up_down & 0x03;

    unsigned char base_ana_reg = 0x0e + ((gpio >> 8) << 1) + ( (gpio & 0xf0) ? 1 : 0);  //group = gpio>>8;
    unsigned char shift_num, mask_not;

    if (gpio & 0x11) {
        shift_num = 0;
        mask_not = 0xfc;
    } else if(gpio & 0x22) {
        shift_num = 2;
        mask_not = 0xf3;
    } else if(gpio & 0x44) {
        shift_num = 4;
        mask_not = 0xcf;
    } else if(gpio & 0x88) {
        shift_num = 6;
        mask_not = 0x3f;
    } else {
        return;
    }

    if (GPIO_DP == gpio) {
        //usb_dp_pullup_en(0);
    }

    analog_write(base_ana_reg, (analog_read(base_ana_reg) & mask_not) | (r_val << shift_num));
}

/**
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength
 * @param[in]  value - the level of driving strength(1: strong 0: poor)
 * @return     none
 */
void gpio_set_data_strength(GPIO_PinTypeDef pin, unsigned int value)
{
    unsigned char bit = pin & 0xff;
    unsigned short group = pin & 0xf00;
    if ((group == GPIO_GROUPA) || (group == GPIO_GROUPD) || (group == GPIO_GROUPE)) {
    	if (value) {
            BM_SET(reg_gpio_ds(pin), bit);
    	} else {
            BM_CLR(reg_gpio_ds(pin), bit);
    	}
    } else if (group == GPIO_GROUPB) {
    	if (value) {
            analog_write(areg_gpio_pb_ds, analog_read(areg_gpio_pb_ds) | bit);
    	} else {
            analog_write(areg_gpio_pb_ds, analog_read(areg_gpio_pb_ds) & (~bit));
    	}
    } else if (group == GPIO_GROUPC) {
    	if (value) {
            analog_write(areg_gpio_pc_ds, analog_read(areg_gpio_pc_ds) | bit);
    	} else {
            analog_write(areg_gpio_pc_ds, analog_read(areg_gpio_pc_ds) & (~bit));
    	}
    }
}

/**
 * @brief      This function servers to set the specified GPIO as high resistor.
 * @param[in]  pin  - select the specified GPIO, GPIOE group is not included in GPIO_ALL
 * @return     none.
 */
void gpio_shutdown(GPIO_PinTypeDef pin)
{
    unsigned short group = pin & 0xf00;
    unsigned char bit = pin & 0xff;
    switch (group) {
        case GPIO_GROUPA:
            reg_gpio_pa_out &= (~bit);
            reg_gpio_pa_oen |= bit;
            reg_gpio_pa_gpio |= (bit & 0x7f);
            reg_gpio_pa_ie &= ((~bit) | 0x80);
            break;
        case GPIO_GROUPB:
            reg_gpio_pb_out &= (~bit);
            reg_gpio_pb_oen |= bit;
            reg_gpio_pb_gpio |= bit;
            analog_write(areg_gpio_pb_ie, analog_read(areg_gpio_pb_ie) & (~bit));
            break;
        case GPIO_GROUPC:
            reg_gpio_pc_out &= (~bit);
            reg_gpio_pc_oen |= bit;
            reg_gpio_pc_gpio |= bit;
            analog_write(areg_gpio_pc_ie, analog_read(areg_gpio_pc_ie) & (~bit));
            break;
        case GPIO_GROUPD:
            reg_gpio_pd_out &= (~bit);
            reg_gpio_pd_oen |= bit;
            reg_gpio_pd_gpio |= bit;
            reg_gpio_pd_ie &= (~bit);
            break;
        case GPIO_GROUPE:
            reg_gpio_pe_out &= (~bit);
            reg_gpio_pe_oen |= bit;
            reg_gpio_pe_gpio |= bit;
            reg_gpio_pe_ie &= (~bit);
            break;
        case GPIO_ALL:
            {
                //set low level
                reg_gpio_pa_out = 0x00;
                reg_gpio_pb_out = 0x00;
                reg_gpio_pc_out = 0x00;
                reg_gpio_pd_out = 0x00;

                //output disable
                reg_gpio_pa_oen = 0xff;
                reg_gpio_pb_oen = 0xff;
                reg_gpio_pc_oen = 0xff;
                reg_gpio_pd_oen = 0xff;

                //dataO = 0
                reg_gpio_pa_gpio = 0x7f;
                reg_gpio_pb_gpio = 0xff;
                reg_gpio_pc_gpio = 0xff;
                reg_gpio_pd_gpio = 0xff;

                //ie = 0
                reg_gpio_pa_ie = 0x80;					//SWS   581<7>
                analog_write(areg_gpio_pb_ie, 0);
                analog_write(areg_gpio_pc_ie, 0);
                reg_gpio_pd_ie = 0x00;
            }
            break;
        default:
            break;
    }
}
