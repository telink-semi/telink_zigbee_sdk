/********************************************************************************************************
 * @file    gpio.h
 *
 * @brief   This is the header file for TL321X
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
/** @page GPIO
 *
 *  Introduction
 *  ===============
 * -#To prevent power leakage, you need to make sure that all GPIOs are not FLOATING, suggested use process is as follows:
 *    -# gpio_shutdown(GPIO_ALL);---All GPIOs except MSPI as well as SWS will be set to high resistance state.(Prevent power leakage.)
 *    -# gpio_setup_up_down_resistor(GPIO_SWS, PM_PIN_PULLUP_1M);---Ensure SWS is a constant level.(There are two purposes: the first is to prevent leakage,
 *     the second is to prevent the SWS no fixed level, generating some interfering signals through the sws mistakenly written to the chip resulting in death.)
 *    -# If you want to use GPIO as another function, please configure it yourself.
 *    -# Must ensure that all GPIOs cannot be FLOATING status before going to sleep to prevent power leakage.
 *
 *  API Reference
 *  ===============
 *  Header File: gpio.h
 */
#ifndef DRIVERS_GPIO_H_
#define DRIVERS_GPIO_H_


#include "lib/include/plic.h"
#include "lib/include/analog.h"
#include "reg_include/gpio_reg.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 *  @brief  Define GPIO group types
 */
typedef enum
{
    GPIO_GROUP_A = 0,
    GPIO_GROUP_B = 1,
    GPIO_GROUP_C = 2,
    GPIO_GROUP_D = 3,
    GPIO_GROUP_E = 4,
    GPIO_GROUP_F = 5,
} gpio_group_e;

/**
 *  @brief  Define GPIO types
 */
typedef enum
{
    GPIO_GROUPA = 0x000,
    GPIO_GROUPB = 0x100,
    GPIO_GROUPC = 0x200,
    GPIO_GROUPD = 0x300,
    GPIO_GROUPE = 0x400,
    GPIO_GROUPF = 0x500,
    GPIO_ALL    = 0X700,

    GPIO_PA0  = GPIO_GROUPA | BIT(0),
    GPIO_PA1  = GPIO_GROUPA | BIT(1),
    /**
      PA2 is not recommended
      Because its use is limited, it is necessary to ensure that the PA2 input function is disabled before the sys_init /pm_sleep_wakeup/pm_set_dig_module_power_switch functions,
      otherwise RF may not work properly.For details, refer to the comments of the functions mentioned above(BUT-53)
    */
    GPIO_PA2  = GPIO_GROUPA | BIT(2),
    GPIO_PA3  = GPIO_GROUPA | BIT(3),
    GPIO_PA4  = GPIO_GROUPA | BIT(4),
    GPIO_PA5  = GPIO_GROUPA | BIT(5),
    GPIO_DM   = GPIO_PA5,
    GPIO_PA6  = GPIO_GROUPA | BIT(6),
    GPIO_DP   = GPIO_PA6,
    GPIO_PA7  = GPIO_GROUPA | BIT(7),
    GPIO_SWS  = GPIO_PA7,
    GPIOA_ALL = GPIO_GROUPA | 0x00ff,

    GPIO_PB0  = GPIO_GROUPB | BIT(0),
    GPIO_PB1  = GPIO_GROUPB | BIT(1),
    GPIO_PB2  = GPIO_GROUPB | BIT(2),
    GPIO_PB3  = GPIO_GROUPB | BIT(3),
    GPIO_PB4  = GPIO_GROUPB | BIT(4),
    GPIO_PB5  = GPIO_GROUPB | BIT(5),
    GPIO_PB6  = GPIO_GROUPB | BIT(6),
    GPIO_PB7  = GPIO_GROUPB | BIT(7),
    GPIOB_ALL = GPIO_GROUPB | 0x00ff,

    GPIO_PC0  = GPIO_GROUPC | BIT(0),
    GPIO_PC1  = GPIO_GROUPC | BIT(1),
    GPIO_PC2  = GPIO_GROUPC | BIT(2),
    GPIO_PC3  = GPIO_GROUPC | BIT(3),
    GPIO_PC4  = GPIO_GROUPC | BIT(4),
    GPIO_PC5  = GPIO_GROUPC | BIT(5),
    GPIO_PC6  = GPIO_GROUPC | BIT(6),
    GPIO_PC7  = GPIO_GROUPC | BIT(7),
    GPIOC_ALL = GPIO_GROUPC | 0x00ff,

    GPIO_PD0  = GPIO_GROUPD | BIT(0),
    GPIO_PD1  = GPIO_GROUPD | BIT(1),
    GPIO_PD2  = GPIO_GROUPD | BIT(2),
    GPIO_PD3  = GPIO_GROUPD | BIT(3),
    GPIO_PD4  = GPIO_GROUPD | BIT(4),
    GPIO_PD5  = GPIO_GROUPD | BIT(5),
    GPIO_PD6  = GPIO_GROUPD | BIT(6),
    GPIO_PD7  = GPIO_GROUPD | BIT(7),
    GPIOD_ALL = GPIO_GROUPD | 0x00ff,

    GPIO_PE0  = GPIO_GROUPE | BIT(0),
    GPIO_PE1  = GPIO_GROUPE | BIT(1),
    GPIO_PE2  = GPIO_GROUPE | BIT(2),
    GPIO_PE3  = GPIO_GROUPE | BIT(3),
    GPIO_PE4  = GPIO_GROUPE | BIT(4),
    GPIO_PE5  = GPIO_GROUPE | BIT(5),
    GPIO_PE6  = GPIO_GROUPE | BIT(6),
    GPIO_PE7  = GPIO_GROUPE | BIT(7),
    GPIOE_ALL = GPIO_GROUPE | 0x00ff,

    GPIO_PF0  = GPIO_GROUPF | BIT(0),
    GPIO_PF1  = GPIO_GROUPF | BIT(1),
    GPIO_PF2  = GPIO_GROUPF | BIT(2),
    GPIO_PF3  = GPIO_GROUPF | BIT(3),
    GPIO_PF4  = GPIO_GROUPF | BIT(4),
    GPIO_PF5  = GPIO_GROUPF | BIT(5),
    GPIO_PF6  = GPIO_GROUPF | BIT(6),
    GPIO_PF7  = GPIO_GROUPF | BIT(7),
    GPIOF_ALL = GPIO_GROUPF | 0x00ff,


} gpio_pin_e;

/**
 *  @brief  Define GPIO function pin types.
 */
typedef enum
{

    GPIO_FC_PA0 = GPIO_PA0,
    GPIO_FC_PA1 = GPIO_PA1,
    /**
      PA2 is not recommended
      Because its use is limited, it is necessary to ensure that the PA2 input function is disabled before the sys_init /pm_sleep_wakeup/pm_set_dig_module_power_switch functions,
      otherwise RF may not work properly.For details, refer to the comments of the functions mentioned above(BUT-53)
    */
    GPIO_FC_PA2 = GPIO_PA2,
    GPIO_FC_PA3 = GPIO_PA3,
    GPIO_FC_PA4 = GPIO_PA4,

    GPIO_FC_PB0 = GPIO_PB0,
    GPIO_FC_PB1 = GPIO_PB1,
    GPIO_FC_PB2 = GPIO_PB2,
    GPIO_FC_PB3 = GPIO_PB3,
    GPIO_FC_PB4 = GPIO_PB4,
    GPIO_FC_PB5 = GPIO_PB5,
    GPIO_FC_PB6 = GPIO_PB6,
    GPIO_FC_PB7 = GPIO_PB7,

    GPIO_FC_PC0 = GPIO_PC0,
    GPIO_FC_PC1 = GPIO_PC1,
    GPIO_FC_PC2 = GPIO_PC2,
    GPIO_FC_PC3 = GPIO_PC3,
    GPIO_FC_PC4 = GPIO_PC4,
    GPIO_FC_PC5 = GPIO_PC5,
    GPIO_FC_PC6 = GPIO_PC6,
    GPIO_FC_PC7 = GPIO_PC7,

    GPIO_FC_PD0 = GPIO_PD0,
    GPIO_FC_PD1 = GPIO_PD1,
    GPIO_FC_PD2 = GPIO_PD2,
    GPIO_FC_PD3 = GPIO_PD3,
    GPIO_FC_PD4 = GPIO_PD4,
    GPIO_FC_PD5 = GPIO_PD5,
    GPIO_FC_PD6 = GPIO_PD6,
    GPIO_FC_PD7 = GPIO_PD7,

    GPIO_FC_PE0 = GPIO_PE0,
    GPIO_FC_PE1 = GPIO_PE1,
    GPIO_FC_PE2 = GPIO_PE2,
    GPIO_FC_PE3 = GPIO_PE3,
    GPIO_FC_PE4 = GPIO_PE4,
    GPIO_FC_PE5 = GPIO_PE5,
    GPIO_FC_PE6 = GPIO_PE6,
    GPIO_FC_PE7 = GPIO_PE7,

    GPIO_FC_PF0 = GPIO_PF0,
    GPIO_FC_PF1 = GPIO_PF1,
    GPIO_FC_PF2 = GPIO_PF2,
    GPIO_FC_PF3 = GPIO_PF3,
    GPIO_FC_PF4 = GPIO_PF4,
    GPIO_FC_PF5 = GPIO_PF5,
    GPIO_FC_PF6 = GPIO_PF6,
    GPIO_FC_PF7 = GPIO_PF7,


    GPIO_NONE_PIN = 0x00,
} gpio_func_pin_e;

/**
 *  @brief  Define GPIO function mux types
 */
typedef enum
{
    PWM0                = 1,
    PWM1                = 2,
    PWM2                = 3,
    PWM3                = 4,
    PWM4                = 5,
    PWM5                = 6,
    PWM0_N              = 7,
    PWM1_N              = 8,
    PWM2_N              = 9,
    PWM3_N              = 10,
    PWM4_N              = 11,
    PWM5_N              = 12,
    MSPI_CN2            = 13,
    MSPI_CN3            = 14,
    MSPI_CN1            = 15,
    GSPI_CN1            = 16,
    GSPI_CN2            = 17,
    GSPI_CN3            = 18,
    GSPI_CN0_IO         = 19,
    GSPI_CK_IO          = 20,
    GSPI_IO3_IO         = 21,
    GSPI_IO2_IO         = 22,
    GSPI_MISO_IO        = 23,
    GSPI_MOSI_IO        = 24,
    I2C_SCL_IO          = 25,
    I2C_SDA_IO          = 26,
    I2C1_SDA_IO         = 27,
    I2C1_SCL_IO         = 28,
    UART0_CTS_I         = 29,
    UART0_RTS           = 30,
    UART0_TX            = 31,
    UART0_RTX_IO        = 32,
    UART1_CTS_I         = 33,
    UART1_RTS           = 34,
    UART1_TX            = 35,
    UART1_RTX_IO        = 36,
    UART2_CTS_I         = 37,
    UART2_RTS           = 38,
    UART2_TX            = 39,
    UART2_RTX_IO        = 40,
    CLK_7816            = 41,
    SDM_CLK             = 42,
    I2S_BCK_IO          = 43,
    I2S_LR0_IO          = 44,
    I2S_DAT0_IO         = 45,
    I2S_LR1_IO          = 46,
    I2S_DAT1_IO         = 47,
    I2S_CLK             = 48,
    DMIC0_CLK           = 49,
    DMIC0_DAT_I         = 50,
    SDM0_P              = 51,
    SDM0_N              = 52,
    SDM1_P              = 53,
    SDM1_N              = 54,
    IR_LEARN_I          = 55,
    SSPI_CN_I           = 56,
    SSPI_CK_I           = 57,
    SSPI_SI_IO          = 58,
    SSPI_SO_IO          = 59,
    KEYS0_IO            = 60,
    TMR0_CMP            = 61,
    TMR1_CMP            = 62,
    RZ_TX               = 63,
    SWM_IO              = 64,
    TX_CYC2PA           = 65,
    WIFI_DENY_I         = 66,
    BT_ACTIVITY         = 67,
    BT_STATUS           = 68,
    ATSEL_0             = 69,
    ATSEL_1             = 70,
    ATSEL_2             = 71,
    ATSEL_3             = 72,
    ATSEL_4             = 73,
    ATSEL_5             = 74,
    RX_CYC2LNA          = 75,
    DBG_PROBE_CLK       = 76,
    DBG_BB0             = 77, //DBG_BB0~DBG_BB4, DBG_BB27~DBG_BB31
    DBG_BB5_IO          = 77, //DBG_BB5_IO~DBG_BB26_IO
    DBG_RX_CLK          = 77,
    DBG_TX_EN_I         = 77,
    DBG_TX_ON_I         = 77,
    DBG_TX_DATA_VALID_I = 77,
    DBG_TX_CLK_I        = 77,
    DBG_ADC_I_DAT0      = 78, //DBG_ADC_I_DAT0~DBG_ADC_I_DAT9
    DBG_ADC_Q_DAT0      = 78, //DBG_ADC_Q_DAT0~DBG_ADC_Q_DAT9
    DBG_RATE0_I         = 78, //DBG_RATE0_I~DBG_RATE2_I
    DBG_RX_EN_I         = 78,
    DBG_FREQ_CHN_I      = 78,
    DBG_TRNG0           = 79, //DBG_TRNG0~DBG_TRNG12
    DBG_IR_ANA2DIG      = 79,
    DBG_TX_DAT1_I       = 79, //DBG_TX_DAT1_I~DBG_TX_DAT2_I
    DBG_TX_PWR_I        = 79,
    DBG_TX_DAT0_I       = 79,
    DBG_AUDIO_DAC       = 79,
} gpio_func_e;

/**
 *  @brief  Define GPIO mux func
 */
typedef enum
{
    AS_GPIO,

    AS_SWS,

    AS_TDI,
    AS_TDO,
    AS_TMS,
    AS_TCK,

    AS_MSPI,
} gpio_fuc_e;

/*
 * @brief define gpio irq status types
 */
typedef enum
{
    GPIO_IRQ_IRQ0 = BIT(0),
    GPIO_IRQ_IRQ1 = BIT(1),
    GPIO_IRQ_IRQ2 = BIT(2),
    GPIO_IRQ_IRQ3 = BIT(3),
    GPIO_IRQ_IRQ4 = BIT(4),
    GPIO_IRQ_IRQ5 = BIT(5),
    GPIO_IRQ_IRQ6 = BIT(6),
    GPIO_IRQ_IRQ7 = BIT(7),
} gpio_irq_e;

/**
 *  @brief  Define rising/falling types
 */
typedef enum
{
    POL_RISING  = 0,
    POL_FALLING = 1,
} gpio_pol_e;

/**
 *  @brief  Define interrupt types
 */
typedef enum
{
    INTR_RISING_EDGE = 0,
    INTR_FALLING_EDGE,
    INTR_HIGH_LEVEL,
    INTR_LOW_LEVEL,
} gpio_irq_trigger_type_e;

/**
 *  @brief  Define IRQ types
 */
typedef enum
{
    GPIO_IRQ0 = 0,
    GPIO_IRQ1 = 1,
    GPIO_IRQ2 = 2,
    GPIO_IRQ3 = 3,
    GPIO_IRQ4 = 4,
    GPIO_IRQ5 = 5,
    GPIO_IRQ6 = 6,
    GPIO_IRQ7 = 7,
} gpio_irq_num_e;

/**
 *  @brief  Define pull up or down types
 *  @note   The GPIO pull-up/pull-down resistance is a simulation result by the internal
 *          MOSFET and affected by the IO voltage VDDO3. The lower the IO voltage of GPIO,
 *          the higher the pull-up/pull-down resistance of GPIO.
 */
typedef enum
{
    GPIO_PIN_UP_DOWN_FLOAT = 0,
    GPIO_PIN_PULLUP_1M     = 1,
    GPIO_PIN_PULLDOWN_100K = 2,
    GPIO_PIN_PULLUP_10K    = 3,
} gpio_pull_type_e;

typedef enum
{
    PROBE_CLK32K     = 0,
    PROBE_RC24M      = 1,
    PROBE_PLL0       = 2,
    PROBE_XTL24M     = 3,
    PROBE_CCLK       = 4,
    PROBE_HCLK       = 5,
    PROBE_PCLK       = 6,
    PROBE_CLK_MSPI   = 7,
    PROBE_CLK_GSPI   = 9,
    PROBE_CLK_SDM    = 10,
    PROBE_CLK_I2S2   = 12,
    PROBE_CLK_USBPHY = 13,
    PROBE_CLK_DMIC   = 14,
    PROBE_CLK_7816   = 15,
    PROBE_DSM_DEBUG  = 16,
    //RSVD
    //RSVD
    PROBE_DBG_CLK          = 19,
    PROBE_CODEC_ADC_CLK_6M = 20,
    PROBE_CODEC_ADC_CLK_1M = 21,
} probe_clk_sel_e;

/**
 *  @brief  Define pem task signal types
 */
typedef enum
{
    TASK_SIGNAL_SEL0 = 0,
    TASK_SIGNAL_SEL1 = 1,
    TASK_SIGNAL_SEL2 = 2,
    TASK_SIGNAL_SEL3 = 3,
    TASK_SIGNAL_SEL4 = 4,
    TASK_SIGNAL_SEL5 = 5,
    TASK_SIGNAL_SEL6 = 6,
    TASK_SIGNAL_SEL7 = 7,
} pem_task_signal_type_e;

/**
 *  @brief  Define pem gpio group task types, choose which gpio group  as task 
 */
typedef enum
{
    TASK_GROUP_GPIO_PA = 0,
    TASK_GROUP_GPIO_PB = 1,
    TASK_GROUP_GPIO_PC = 2,
    TASK_GROUP_GPIO_PD = 3,
    TASK_GROUP_GPIO_PE = 4,
    TASK_GROUP_GPIO_PF = 5,
} pem_gpio_group_task_type_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief      This function servers to enable gpio function.
 * @param[in]  pin - the selected pin.
 * @return     none.
 */
static inline void gpio_function_en(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    BM_SET(reg_gpio_func(pin), bit);
}

/**
 * @brief      This function servers to disable gpio function.
 * @param[in]  pin - the selected pin.
 * @return     none.
 */
static inline void gpio_function_dis(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    BM_CLR(reg_gpio_func(pin), bit);
}

/**
 * @brief     This function set the pin's output high level.
 * @param[in] pin - the pin needs to set its output level.
 * @return    none.
 */
static _always_inline void gpio_set_high_level(gpio_pin_e pin)
{
    unsigned char bit     = pin & 0xff;
    reg_gpio_out_set(pin) = bit;
}

/**
 * @brief     This function set the pin's output low level.
 * @param[in] pin - the pin needs to set its output level.
 * @return    none.
 */
static _always_inline void gpio_set_low_level(gpio_pin_e pin)
{
    unsigned char bit       = pin & 0xff;
    reg_gpio_out_clear(pin) = bit;
}

/**
 * @brief     This function set the pin's output level.
 * @param[in] pin - the pin needs to set its output level
 * @param[in] value - value of the output level(1: high 0: low)
 * @return    none
 */
static _always_inline void gpio_set_level(gpio_pin_e pin, unsigned char value)
{
    if (value) {
        gpio_set_high_level(pin);
    } else {
        gpio_set_low_level(pin);
    }
}

/**
 * @brief     This function read the pin's input level.
 * @param[in] pin - the pin needs to read its input level.
 * @return    1: the pin's input level is high.
 *            0: the pin's input level is low.
 */
static inline _Bool gpio_get_level(gpio_pin_e pin)
{
    return BM_IS_SET(reg_gpio_in(pin), pin & 0xff);
}

/**
 * @brief      This function read all the pins' input level.
 * @param[out] p - the buffer used to store all the pins' input level
 * @return     none
 */
static inline void gpio_get_level_all(unsigned char *p)
{
    p[0] = reg_gpio_pa_in;
    p[1] = reg_gpio_pb_in;
    p[2] = reg_gpio_pc_in;
    p[3] = reg_gpio_pd_in;
    p[4] = reg_gpio_pe_in;
    p[5] = reg_gpio_pf_in;
    //  p[6] = reg_gpio_pg_in;
    //p[7] = reg_gpio_ph_in;
}

/**
 * @brief     This function set the pin toggle.
 * @param[in] pin - the pin needs to toggle.
 * @return    none.
 */
static _always_inline void gpio_toggle(gpio_pin_e pin)
{
    unsigned char bit        = pin & 0xff;
    reg_gpio_out_toggle(pin) = bit;
}

/**
 * @brief      This function enable the output function of a pin.
 * @param[in]  pin - the pin needs to set the output function.
 * @return     none.
 */
static inline void gpio_output_en(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    BM_CLR(reg_gpio_oen(pin), bit);
}

/**
 * @brief      This function disable the output function of a pin.
 * @param[in]  pin - the pin needs to set the output function.
 * @return     none.
 */
static inline void gpio_output_dis(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    BM_SET(reg_gpio_oen(pin), bit);
}

/**
 * @brief      This function enable set output function of a pin.
 * @param[in]  pin - the pin needs to set the output function (1: enable,0: disable)
 * @return     none
 */
static inline void gpio_set_output(gpio_pin_e pin, unsigned char value)
{
    if (value) {
        gpio_output_en(pin);
    } else {
        gpio_output_dis(pin);
    }
}

/**
 * @brief      This function determines whether the output function of a pin is enabled.
 * @param[in]  pin - the pin needs to determine whether its output function is enabled.
 * @return     1: the pin's output function is enabled.
 *             0: the pin's output function is disabled.
 */
static inline _Bool gpio_is_output_en(gpio_pin_e pin)
{
    return !BM_IS_SET(reg_gpio_oen(pin), pin & 0xff);
}

/**
 * @brief     This function determines whether the input function of a pin is enabled.
 * @param[in] pin - the pin needs to determine whether its input function is enabled(not include group_pc).
 * @return    1: the pin's input function is enabled.
 *            0: the pin's input function is disabled.
 */
static inline _Bool gpio_is_input_en(gpio_pin_e pin)
{
    return BM_IS_SET(reg_gpio_ie(pin), pin & 0xff);
}

/**
 * @brief       This function is used to enable the GPIO pin of mspi.
 * @param[in]   none.
 * @return      none.
 * @note        This interface is for internal use only.
 */
static _always_inline void gpio_set_mspi_pin_ie_en(void)
{
    reg_gpio_pf_ie = 0x3f;
}

/**
 * @brief       This function is used to disable the GPIO pin of mspi.
 * @param[in]   none.
 * @return      none.
 * @note        This interface is for internal use only.
 */
static _always_inline void gpio_set_mspi_pin_ie_dis(void)
{
    reg_gpio_pf_ie = 0x00;
}

/**
 * @brief      This function serves to enable gpio irq0~7 function.
 * @param[in]  pin  - the pin needs to enable its IRQ.
 * @param[in]  irq  - there are 8 types of irq to choose.(irq0/irq1/irq2/irq3/irq4/irq5/irq6/irq7)
 * @return     none.
 */
static inline void gpio_irq_en(gpio_pin_e pin, gpio_irq_num_e irq)
{
    BM_SET(reg_gpio_irq_en(pin, irq), pin & 0xff);
}

/**
 * @brief      This function serves to disable gpio irq0 function.
 * @param[in]  pin  - the pin needs to disable its IRQ.
 * @param[in]  irq  - there are 8 types of irq to choose.(irq0/irq1/irq2/irq3/irq4/irq5/irq6/irq7)
 * @return     none.
 */
static inline void gpio_irq_dis(gpio_pin_e pin, gpio_irq_num_e irq)
{
    BM_CLR(reg_gpio_irq_en(pin, irq), pin & 0xff);
}

/**
 * @brief      This function serves to enable gpio irq mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_irq_mask(gpio_irq_e mask)
{
    BM_SET(reg_gpio_irq_src_mask, mask);
}

/**
 * @brief      This function serves to clr gpio irq status.
 * @param[in]  status  - the irq need to clear.
 * @return     none.
 */
static inline void gpio_clr_irq_status(gpio_irq_e status)
{
    reg_gpio_irq_clr = status;
}

/**
 * @brief      This function serves to disable gpio irq mask function.
 *             if disable gpio interrupt,choose disable gpio mask , use interface gpio_clr_irq_mask instead of gpio_irq_dis/gpio_gpio2risc0_irq_dis/gpio_gpio2risc1_irq_dis.
 * @return     none.
 */
static inline void gpio_clr_irq_mask(gpio_irq_e mask)
{
    BM_CLR(reg_gpio_irq_ctrl, mask);
}

/**
 * @brief     This function set a pin's IRQ , here you can choose from 8 interrupts for flexible configuration, each interrupt is independent and equal to each other.
 * @param[in] irq           - there are 8 types of irq to choose.(irq0/irq1/irq2/irq3/irq4/irq5/irq6/irq7)
 * @param[in] pin           - the pin needs to enable its IRQ.
 * @param[in] trigger_type  - gpio interrupt type.
 *                            0: rising edge.
 *                            1: falling edge.
 *                            2: high level.
 *                            3: low level.
 * @return    none.
 */
void gpio_set_irq(gpio_irq_num_e irq, gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);

/**
 * @brief      This function serves to set the gpio-mux function.
 * @param[in]  pin      - the pin needs to set.
 * @param[in]  function - the function need to set.
 * @return     none.
 */
void gpio_set_mux_function(gpio_func_pin_e pin, gpio_func_e function);

/**
 * @brief      This function set the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function.
 * @return     none.
 */
void gpio_input_en(gpio_pin_e pin);

/**
 * @brief      This function disable the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function.
 * @return     none.
 */
void gpio_input_dis(gpio_pin_e pin);

/**
 * @brief      This function set the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function
 * @param[in]  value - enable or disable the pin's input function(1: enable,0: disable )
 * @return     none
 */
void gpio_set_input(gpio_pin_e pin, unsigned char value);
/**
 * @brief      This function servers to set the specified GPIO as high resistor.
 *             To prevent power leakage, you need to call gpio_shutdown(GPIO_ALL) (set all gpio to high resistance, except SWS and MSPI.)
 *             as front as possible in the program, and then initialize the corresponding GPIO according to the actual using situation.
 * @param[in]  pin  - select the specified GPIO.
 * @return     none.
 */
void gpio_shutdown(gpio_pin_e pin);

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] pin - the pin needs to set its pull-up/down resistor.
 * @param[in] up_down_res - the type of the pull-up/down resistor.
 * @return    none.
 */
void gpio_set_up_down_res(gpio_pin_e pin, gpio_pull_type_e up_down_res);

/**
 * @brief     This function set probe clk output.
 * @param[in] pin
 * @param[in] sel_clk
 * @return    none.
 */
void gpio_set_probe_clk_function(gpio_func_pin_e pin, probe_clk_sel_e sel_clk);

/**
 * @brief     This function serves to set jtag(4 wires) pin , Where, PC[4]; PC[5]; PC[6]; PC[7] correspond to TDI; TDO; TMS; TCK functions mux respectively.
 * @param[in] none
 * @return    none.
 * @note      Power-on or hardware reset will detect the level of PB0 (reboot will not detect it), detecting a low level is configured as jtag,
               detecting a high level is configured as sdp.  the level of PB0 can not be configured internally by the software, and can only be input externally.
 */
void jtag_set_pin_en(void);

/**
 * @brief     This function serves to set sdp(2 wires) pin ,where, PC[6]; PC[7] correspond to TMS and TCK functions mux respectively.
 * @param[in] none
 * @return    none.
 * @note      Power-on or hardware reset will detect the level of PB0 (reboot will not detect it), detecting a low level is configured as jtag,
               detecting a high level is configured as sdp.  the level of PB0 can not be configured internally by the software, and can only be input externally.
 */
void sdp_set_pin_en(void);
#endif
