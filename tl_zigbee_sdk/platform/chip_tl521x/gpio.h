/********************************************************************************************************
 * @file    gpio.h
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
#include "pem.h"

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
    GPIOF_ALL = GPIO_GROUPF | 0x00ff,
} gpio_pin_e;

typedef enum
{
    PA0_INPUT  = 0x0000,
    PA1_INPUT,
    PA2_INPUT,
    PA3_INPUT,
    PA4_INPUT,
    PA5_INPUT,
    PA6_INPUT,
    PA7_INPUT,

    PB0_INPUT  = 0x0100,
    PB1_INPUT,
    PB2_INPUT,
    PB3_INPUT,
    PB4_INPUT,
    PB5_INPUT,
    PB6_INPUT,
    PB7_INPUT,

    PC0_INPUT  = 0x0200,
    PC1_INPUT,
    PC2_INPUT,
    PC3_INPUT,
    PC4_INPUT,
    PC5_INPUT,
    PC6_INPUT,
    PC7_INPUT,

    PD0_INPUT  = 0x0300,
    PD1_INPUT,
    PD2_INPUT,
    PD3_INPUT,
    PD4_INPUT,
    PD5_INPUT,
    PD6_INPUT,
    PD7_INPUT,

    PE0_INPUT  = 0x0400,
    PE1_INPUT,
    PE2_INPUT,
    PE3_INPUT,
    PE4_INPUT,
    PE5_INPUT,
    PE6_INPUT,
    PE7_INPUT,

    PF0_INPUT  = 0x0500,
    PF1_INPUT,
    PF2_INPUT,
    PF3_INPUT,
    PF4_INPUT,
    PF5_INPUT,

    GPIO_IRQ  = 0x0600,
    GPIO2RISC0,
    GPIO2RISC1,

    GPIO_IRQ_GROUP0  = 0x0700,
    GPIO_IRQ_GROUP1,
    GPIO_IRQ_GROUP2,
    GPIO_IRQ_GROUP3,
    GPIO_IRQ_GROUP4,
    GPIO_IRQ_GROUP5,
    GPIO_IRQ_GROUP6,
    GPIO_IRQ_GROUP7,

} gpio_event_e;

typedef enum
{
    PA0_TOGGLE  = 0x0000,
    PA1_TOGGLE,
    PA2_TOGGLE,
    PA3_TOGGLE,
    PA4_TOGGLE,
    PA5_TOGGLE,
    PA6_TOGGLE,
    PA7_TOGGLE,

    PB0_TOGGLE  = 0x0100,
    PB1_TOGGLE,
    PB2_TOGGLE,
    PB3_TOGGLE,
    PB4_TOGGLE,
    PB5_TOGGLE,
    PB6_TOGGLE,
    PB7_TOGGLE,

    PC0_TOGGLE  = 0x0200,
    PC1_TOGGLE,
    PC2_TOGGLE,
    PC3_TOGGLE,
    PC4_TOGGLE,
    PC5_TOGGLE,
    PC6_TOGGLE,
    PC7_TOGGLE,

    PD0_TOGGLE  = 0x0300,
    PD1_TOGGLE,
    PD2_TOGGLE,
    PD3_TOGGLE,
    PD4_TOGGLE,
    PD5_TOGGLE,
    PD6_TOGGLE,
    PD7_TOGGLE,

    PE0_TOGGLE  = 0x0400,
    PE1_TOGGLE,
    PE2_TOGGLE,
    PE3_TOGGLE,
    PE4_TOGGLE,
    PE5_TOGGLE,
    PE6_TOGGLE,
    PE7_TOGGLE,

    PF0_TOGGLE  = 0x0500,
    PF1_TOGGLE,
    PF2_TOGGLE,
    PF3_TOGGLE,
    PF4_TOGGLE,
    PF5_TOGGLE,

} gpio_task_e;

/**
 *  @brief  Define GPIO function pin types.
 */
typedef enum
{

    GPIO_FC_PA0 = GPIO_PA0,
    GPIO_FC_PA1 = GPIO_PA1,
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

    GPIO_NONE_PIN = 0x00,
} gpio_func_pin_e;

/**
 *  @brief  Define GPIO function mux types
 */
typedef enum
{
    PWM0 = 1,
    PWM1 = 2,
    PWM2 = 3,
    PWM3 = 4,
    PWM4 = 5,
    PWM5 = 6,

    PWM0_N = 7,
    PWM1_N = 8,
    PWM2_N = 9,
    PWM3_N = 10,
    PWM4_N = 11,
    PWM5_N = 12,

    I2C_SCL_IO  = 13,
    I2C_SDA_IO  = 14,
    DMIC0_CLK = 15,
    DMIC0_DAT_I = 16,

    SDM0_P        = 17,
    SDM0_N          = 18,
    SDM1_P           = 19,
    SDM1_N       = 20,

    UART0_CTS_I        = 21,
    UART0_RTS          = 22,
    UART0_TX           = 23,
    UART0_RTX_IO       = 24,

    UART1_CTS_I        = 25,
    UART1_RTS          = 26,
    UART1_TX           = 27,
    UART1_RTX_IO       = 28,

    UART2_CTS_I        = 29,
    UART2_RTS          = 30,
    UART2_TX           = 31,
    UART2_RTX_IO       = 32,

    UART3_CTS_I        = 33,
    UART3_RTS          = 34,
    UART3_TX           = 35,
    UART3_RTX_IO       = 36,


    UART4_CTS_I        = 37,
    UART4_RTS          = 38,
    UART4_TX           = 39,
    UART4_RTX_IO       = 40,



    I2S2_BCK_IO        = 41,
    I2S2_LR0_IO        = 42,
    I2S2_DAT0_IO       = 43,
    I2S2_LR1_IO        = 44,
    I2S2_DAT1_IO       = 45,
    I2S2_CLK           = 46,

    IR_LEARN_I         = 47,
    KEYS_IO            = 48,


    CLK_7816       = 49,
    TDI_I          = 50,
    TDO_IO         = 50,
    TMS_IO         = 50,
    TCK_I          = 50,

    SSPI_CN_I          = 51,
    SSPI_CK_I          = 52,
    SSPI_SI_IO         = 53,
    SSPI_SO_IO         = 54,

    RZ_TX              = 55,
    SWM_IO             = 56,
    TX_CYC2PA          = 57,
    WIFI_DENY_I        = 58,
    BT_ACTIVITY        = 59,
    BT_STATUS          = 60,
    ATSEL_0            = 61,
    ATSEL_1            = 62,
    ATSEL_2            = 63,
    ATSEL_3            = 64,
    ATSEL_4            = 65,
    ATSEL_5            = 66,
    RX_CYC2LNA         = 67,
    DBG_PROBE_CLK      = 68,
    DBG_BB0            = 69,
    CAN0_RX_I          = 72,
    CAN0_TX            = 73,
    I3C0_SDA_PULLUP_EN = 74,
    I3C0_SDA_IO        = 75,
    I3C0_SCL_IO        = 76,


    GSPI_CN_IO         = 77,GSPI_CN0_IO    = 77,
    GSPI_IO3_IO        = 78,
    GSPI_IO2_IO        = 79,
    GSPI_MISO_IO       = 80,
    GSPI_MOSI_IO       = 81,
    GSPI_CK_IO         = 82,

    GSPI1_CN_IO         = 83,
    GSPI1_IO3_IO        = 84,
    GSPI1_IO2_IO        = 85,
    GSPI1_MISO_IO       = 86,
    GSPI1_MOSI_IO       = 87,
    GSPI1_CK_IO         = 88,

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
    GPIO_PIN_PULLUP_20K    = 3,
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
    PROBE_CLK_RRAM   = 7,
    PROBE_CLK_MSPI   = 8,
    PROBE_CLK_LSPI   = 9,
    PROBE_CLK_GSPI   = 10,
    PROBE_CLK_GSPI1  = 11,
    PROBE_CLK_GSPI2  = 12,
    PROBE_CLK_GSPI3  = 13,
    PROBE_CLK_GSPI4  = 14,
    PROBE_CLK_I3C0   = 15,
    PROBE_CLK_I3C1   = 16,
    PROBE_CLK_STIMER   = 17,
    PROBE_CLK_SARDIG   = 18,
    PROBE_CLK_I2S0      = 19,
    PROBE_CLK_I2S2      = 20,
    PROBE_CLK_DMIC      = 21,
    PROBE_CLK_AUDIO     = 22,
    PROBE_CODEC_ADC_CLK_6M = 23,
    PROBE_CODEC_ADC_CLK_1M = 24,
    PROBE_CLK_7816         = 25,
    PROBE_DBG_CLK          = 26,
    PROBE_CLK_USBPHY0      = 27,
    PROBE_CLK_USBPHY1      = 28,
    PROBE_CLK_USBPHY_CLK   = 29,
    PROBE_CLK_USB_UTMI     = 30,
    PROBE_CLK_USB_PLL      = 31,
    PROBE_CLK_USB_PLL2     = 32,
    PROBE_CLK_USB_PLL3 = 33,
    PROBE_CLK_USB_PLL4 = 34,
    PROBE_CLK_SARDIG1   = 35,
    PROBE_CLK_KS = 36,

} probe_clk_sel_e;

/**
 *  @brief  Define pull up or down types
 *  @note
 *  The names of the enumeration members are named according to the driving capability under 3.3V power supply.
 *  PIN2 = 0, PIN1 = 0, Driving strength: 2.2mA(1.8v), 3.6mA(2.5v), 4.7mA(3.3v);
    PIN2 = 0, PIN1 = 1, Driving strength: 4.4mA(1.8v), 7.2mA(2.5v), 9.4A(3.3v);
    PIN2 = 1, PIN1 = 0, Driving strength: 6.6mA(1.8v), 10.8mA(2.5v), 14.1mA(3.3v);
    PIN2 = 1, PIN1 = 1, Driving strength: 8.8mA(1.8v), 14.4mA(2.5v), 18.8mA(3.3v);
 */
typedef enum
{
    DRV_STRENGTH_4P7MA = 0,
    DRV_STRENGTH_9P4MA = 1,
    DRV_STRENGTH_14P1MA = 2,
    DRV_STRENGTH_18P8MA = 3,
} gpio_drv_strength_e;



/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief      This function servers to enable gpio function.
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
    BM_CLR(reg_gpio_irq_src_mask, mask);
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
 * @brief      This function serves to configure the GPIO PEM event.
 * @param[in]  chn - to select the PEM channel.
 * @param[in]  pin - the GPIO event signal selection.
 * @param[in]  pol - the GPIO event signal edge selection
 * @return     none.
 */
void gpio_set_pem_event(pem_chn_e chn, gpio_event_e pin, pem_event_pol_e pol);

/**
 * @brief      This function serves to configure the GPIO PEM task.
 * @param[in]  chn - to select the PEM channel.
 * @param[in]  pin - the GPIO task signal selection.
 * @return     none.
 */
void gpio_set_pem_task(pem_chn_e chn, gpio_task_e pin);

/**
 * @brief     This function serves to set jtag(4 wires) pin . where, PD[4]; PD[5]; PD[6]; PD[7] correspond to TDI; TDO; TMS; TCK functions mux respectively.
 * @param[in] none
 * @return    none.
 * @note      Power-on or hardware reset will detect the level of PB0 (reboot will not detect it), detecting a low level is configured as jtag,
               detecting a high level is configured as sdp.  the level of PB0 can not be configured internally by the software, and can only be input externally.
 */
void jtag_set_pin_en(void);

/**
 * @brief     This function serves to set sdp(2 wires) pin . where, PD[6]; PD[7] correspond to TMS and TCK functions mux respectively.
 * @param[in] none
 * @return    none.
 * @note      Power-on or hardware reset will detect the level of PB0 (reboot will not detect it), detecting a low level is configured as jtag,
               detecting a high level is configured as sdp.  the level of PB0 can not be configured internally by the software, and can only be input externally.
 */
void sdp_set_pin_en(void);
#endif
