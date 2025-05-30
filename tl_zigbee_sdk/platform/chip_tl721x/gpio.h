/********************************************************************************************************
 * @file    gpio.h
 *
 * @brief   This is the header file for TL721X
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
 *  @note   The GPIOB groups and PA6 of the A2 version chip does not support 10k pull-up wake-up.
 */
typedef enum
{
    GPIO_GROUPA = 0x000,
    GPIO_GROUPB = 0x100,
    GPIO_GROUPC = 0x200,
    GPIO_GROUPD = 0x300,
    GPIO_GROUPE = 0x400,
    GPIO_GROUPF = 0x500,
    GPIO_GROUPG = 0X600,
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
    GPIO_PF6  = GPIO_GROUPF | BIT(6),
    GPIO_PF7  = GPIO_GROUPF | BIT(7),
    GPIOF_ALL = GPIO_GROUPF | 0x00ff,

    GPIO_PG0  = GPIO_GROUPG | BIT(0),
    GPIO_PG1  = GPIO_GROUPG | BIT(1),
    GPIO_PG2  = GPIO_GROUPG | BIT(2),
    GPIO_PG3  = GPIO_GROUPG | BIT(3),
    GPIO_PG4  = GPIO_GROUPG | BIT(4),
    GPIO_PG5  = GPIO_GROUPG | BIT(5),
    GPIO_PG6  = GPIO_GROUPG | BIT(6),
    GPIO_PG7  = GPIO_GROUPG | BIT(7),
    GPIOG_ALL = GPIO_GROUPG | 0x00ff,

} gpio_pin_e;

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

    GPIO_FC_PG0 = GPIO_PG0,
    GPIO_FC_PG1 = GPIO_PG1,
    GPIO_FC_PG2 = GPIO_PG2,
    GPIO_FC_PG3 = GPIO_PG3,
    GPIO_FC_PG4 = GPIO_PG4,
    GPIO_FC_PG5 = GPIO_PG5,
    GPIO_FC_PG6 = GPIO_PG6,
    GPIO_FC_PG7 = GPIO_PG7,

    GPIO_NONE_PIN = 0x000,
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

    GSPI_CN0_IO  = 13,
    GSPI_CK_IO   = 14,
    GSPI_IO3_IO  = 15,
    GSPI_IO2_IO  = 16,
    GSPI_MISO_IO = 17,
    GSPI_MOSI_IO = 18,

    I2C_SCL_IO = 19,
    I2C_SDA_IO = 20,

    UART0_CTS_I  = 21,
    UART0_RTS    = 22,
    UART0_TX     = 23,
    UART0_RTX_IO = 24,

    UART1_CTS_I  = 25,
    UART1_RTS    = 26,
    UART1_TX     = 27,
    UART1_RTX_IO = 28,

    CLK_7816 = 29,

    I2S0_BCK_IO  = 30,
    I2S0_LR0_IO  = 31,
    I2S0_DAT0_IO = 32,
    I2S0_LR1_IO  = 33,
    I2S0_DAT1_IO = 34,
    I2S0_CLK     = 35,

    I2S1_BCK_IO  = 36,
    I2S1_LR0_IO  = 37,
    I2S1_DAT0_IO = 38,
    I2S1_LR1_IO  = 39,
    I2S1_DAT1_IO = 40,
    I2S1_CLK     = 41,
    DMIC0_CLK    = 42,
    DMIC0_DAT_I  = 43,

    MSPI_CN2      = 44,
    MSPI_CN3      = 45,
    DBG_PROBE_CLK = 46,
    TX_CYC2PA     = 47,
    WIFI_DENY_I   = 48,
    BT_ACTIVITY   = 49,
    BT_STATUS     = 50,

    ATSEL_0    = 52,
    ATSEL_1    = 53,
    ATSEL_2    = 54,
    ATSEL_3    = 55,
    RX_CYC2LNA = 56,

    I2C1_SDA_IO = 59,
    I2C1_SCL_IO = 60,
    GSPI_CN1    = 61,
    GSPI_CN2    = 62,
    GSPI_CN3    = 63,
    ATSEL_4     = 64,
    ATSEL_5     = 65,
    IR_LEARN_I  = 66,

    UART2_CTS_I  = 67,
    UART2_RTS    = 68,
    UART2_TX     = 69,
    UART2_RTX_IO = 70,
    SDM0_P       = 71,
    SDM0_N       = 72,
    SDM1_P       = 73,
    SDM1_N       = 74,

    I2S2_BCK_IO  = 75,
    I2S2_LR0_IO  = 76,
    I2S2_DAT0_IO = 77,
    I2S2_LR1_IO  = 78,
    I2S2_DAT1_IO = 79,
    I2S2_CLK     = 80,

    SSPI_CN_I  = 81,
    SSPI_CK_I  = 82,
    SSPI_SI_IO = 83,
    SSPI_SO_IO = 84,

    KEYS0_IO = 85,

    PWM_SYNC_I = 86,
    PWM6       = 87,
    PWM6_N     = 88,

    TMR0_CMP      = 89,
    TMR1_CMP      = 90,
    DBG_AUDIO_DAC = 91,
    LSPI_CN_IO    = 92,

    MSPI_CN1 = 96,
    RZ_TX    = 97,
    SWM_IO   = 98,
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
    FLD_GPIO_IRQ_CLR            = BIT(0),
    FLD_GPIO_IRQ_GPIO2RISC0_CLR = BIT(1),
    FLD_GPIO_IRQ_GPIO2RISC1_CLR = BIT(2),
} gpio_irq_status_e;

/*
 *  @brief define gpio irq mask types
 */
typedef enum
{
    GPIO_IRQ_MASK_GPIO       = BIT(2),
    GPIO_IRQ_MASK_GPIO2RISC0 = BIT(3),
    GPIO_IRQ_MASK_GPIO2RISC1 = BIT(4),
} gpio_irq_mask_e;

/*
 * @brief define gpio group irq types
 */
typedef enum
{
    FLD_GPIO_GROUP_IRQ0 = BIT(0),
    FLD_GPIO_GROUP_IRQ1 = BIT(1),
    FLD_GPIO_GROUP_IRQ2 = BIT(2),
    FLD_GPIO_GROUP_IRQ3 = BIT(3),
    FLD_GPIO_GROUP_IRQ4 = BIT(4),
    FLD_GPIO_GROUP_IRQ5 = BIT(5),
    FLD_GPIO_GROUP_IRQ6 = BIT(6),
    FLD_GPIO_GROUP_IRQ7 = BIT(7),
} gpio_group_irq_e;

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
    GPIO_PIN_PULLUP_10K    = 3, //PULLUP_10K of PB0 to PB7 and PA6 of the A2 version chip
                                //cannot be used as the wake up function; otherwise, the wake up may fail.
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
    PROBE_CLK_LSPI   = 8,
    PROBE_CLK_GSPI   = 9,
    PROBE_CLK_I2S0   = 10,
    PROBE_CLK_I2S1   = 11,
    PROBE_CLK_I2S2   = 12,
    PROBE_CLK_USBPHY = 13,
    PROBE_CODEC_MCLK = 14,
    PROBE_CLK_7816   = 15,
    PROBE_DSM_DEBUG  = 16,
    //RSVD
    //RSVD
    PROBE_DBG_CLK          = 19,
    PROBE_CODEC_ADC_CLK_6M = 20,
    PROBE_CODEC_ADC_CLK_1M = 21,
} probe_clk_sel_e;

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
 * @param[in]  pin - the pin needs to set the output function
 * @param[in]  value - enable or disable the pin's output function(1: enable,0: disable )
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
    reg_gpio_pg_ie = 0x3f;
}

/**
 * @brief       This function is used to disable the GPIO pin of mspi.
 * @param[in]   none.
 * @return      none.
 * @note        This interface is for internal use only.
 */
static _always_inline void gpio_set_mspi_pin_ie_dis(void)
{
    reg_gpio_pg_ie = 0x00;
}

/**
 * @brief      This function serves to enable gpio irq function.
 * @param[in]  pin  - the pin needs to enable its IRQ.
 * @return     none.
 */
static inline void gpio_irq_en(gpio_pin_e pin)
{
    BM_SET(reg_gpio_irq_en(pin), pin & 0xff);
}

/**
 * @brief      This function serves to disable gpio irq function.
 * @param[in]  pin  - the pin needs to disable its IRQ.
 * @return     none.
 */
static inline void gpio_irq_dis(gpio_pin_e pin)
{
    BM_CLR(reg_gpio_irq_en(pin), pin & 0xff);
}

/**
 * @brief      This function serves to enable gpio risc0 irq function.
 * @param[in]  pin  - the pin needs to enable its IRQ.
 * @return     none.
 */
static inline void gpio_gpio2risc0_irq_en(gpio_pin_e pin)
{
    BM_SET(reg_gpio_irq_risc0_en(pin), pin & 0xff);
}

/**
 * @brief      This function serves to disable gpio risc0 irq function.
 * @param[in]  pin  - the pin needs to disable its IRQ.
 * @return     none.
 */
static inline void gpio_gpio2risc0_irq_dis(gpio_pin_e pin)
{
    BM_CLR(reg_gpio_irq_risc0_en(pin), pin & 0xff);
}

/**
 * @brief      This function serves to enable gpio risc1 irq function.
 * @param[in]  pin  - the pin needs to enable its IRQ.
 * @return     none.
 */
static inline void gpio_gpio2risc1_irq_en(gpio_pin_e pin)
{
    BM_SET(reg_gpio_irq_risc1_en(pin), pin & 0xff);
}

/**
 * @brief      This function serves to disable gpio risc1 irq function.
 * @param[in]  pin  - the pin needs to disable its IRQ.
 * @return     none.
 */
static inline void gpio_gpio2risc1_irq_dis(gpio_pin_e pin)
{
    BM_CLR(reg_gpio_irq_risc1_en(pin), pin & 0xff);
}

/**
 * @brief      This function serves to clr gpio irq status.
 * @param[in]  status  - the irq need to clear.
 * @return     none.
 */
static inline void gpio_clr_irq_status(gpio_irq_status_e status)
{
    reg_gpio_irq_clr = status;
}

/**
 * @brief      This function serves to clr gpio group irq status.
 * @param[in]  status  - the irq need to clear.
 * @return     none.
 */
static inline void gpio_clr_group_irq_status(gpio_group_irq_e status)
{
    reg_gpio_irq_src = status;
}

/**
 * @brief      This function serves to enable gpio irq mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_irq_mask(gpio_irq_mask_e mask)
{
    BM_SET(reg_gpio_irq_ctrl, mask);
}

/**
 * @brief      This function serves to enable gpio irq group mask function.
 * @param[in]  mask  - to select interrupt type.
 * @return     none.
 */
static inline void gpio_set_group_irq_mask(gpio_group_irq_e mask)
{
    BM_SET(reg_gpio_irq_src_mask, mask);
}

/**
 * @brief      This function serves to disable gpio irq mask function.
 *             if disable gpio interrupt,choose disable gpio mask , use interface gpio_clr_irq_mask instead of gpio_irq_dis/gpio_gpio2risc0_irq_dis/gpio_gpio2risc1_irq_dis.
 * @return     none.
 */
static inline void gpio_clr_irq_mask(gpio_irq_mask_e mask)
{
    BM_CLR(reg_gpio_irq_ctrl, mask);
}

/**
 * @brief      This function serves to disable gpio irq group mask function.
 *             if disable gpio src irq interrupt,can choose this function disable gpio group irq mask.
 * @return     none.
 */
static inline void gpio_clr_group_irq_mask(gpio_group_irq_e mask)
{
    BM_CLR(reg_gpio_irq_src_mask, mask);
}

/**
 * @brief      This function set the pin's driving strength at strong.
 * @param[in]  pin - the pin needs to set the driving strength.
 * @return     none.
 */
void gpio_ds_en(gpio_pin_e pin);


/**
 * @brief      This function set the pin's driving strength.
 * @param[in]  pin - the pin needs to set the driving strength at poor.
 * @return     none.
 */
void gpio_ds_dis(gpio_pin_e pin);


void gpio_set_irq(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);

/**
 * @brief     This function set a pin's IRQ_RISC0.
 * @param[in] pin           - the pin needs to enable its IRQ.
 * @param[in] trigger_type  - gpio interrupt type 0  rising edge 1 falling edge 2 high level 3 low level
 * @return    none.
 */
void gpio_set_gpio2risc0_irq(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);

/**
 * @brief     This function set a pin's IRQ_RISC1.
 * @param[in] pin           - the pin needs to enable its IRQ.
 * @param[in] trigger_type  - gpio interrupt type 0  rising edge 1 falling edge 2 high level 3 low level
 * @return    none.
 */
void gpio_set_gpio2risc1_irq(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);

/**
 * @brief     This function set a pin's IRQ.
 * @param[in] pin           - the pin needs to enable its IRQ.
 * @param[in] trigger_type  - gpio interrupt type.
 *                            0: rising edge.
 *                            1: falling edge.
 *                            2: high level.
 *                            3: low level
 * @note      if you want to use this irq,you should select irq_group first,which correspond to the function "gpio_set_src_irq_group()".
 * @return    none.
 */
void gpio_set_src_irq(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);
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
 * @brief     This function set pin's  pull-up register .
 * @param[in] pin - the pin needs to set its pull-up register .
 * @return    none.
 * @attention  This function sets the digital pull-up, it will not work after entering low power consumption.
 */
void gpio_set_digital_pulldown(gpio_pin_e pin);
/**
 * @brief     This function set pin's  pull-down register .
 * @param[in] pin - the pin needs to set its pull-down register .
 * @return    none.
 * @attention  This function sets the digital pull-down, it will not work after entering low power consumption.
 */
void gpio_set_digital_pullup(gpio_pin_e pin);

/**
 * @brief     This function select the irq group source.
 * @param[in] group - gpio irq group,include group A,B,C,D,E,F.
 * @note      after you choose the gpio_group,you should set the pin's irq one by one,which correspond to the function "gpio_set_src_irq()".
 * @return    none.
 */
static inline void gpio_set_src_irq_group(gpio_group_e group)
{
    reg_gpio_irq_sel &= (~group);
    reg_gpio_irq_sel |= group;
}

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
