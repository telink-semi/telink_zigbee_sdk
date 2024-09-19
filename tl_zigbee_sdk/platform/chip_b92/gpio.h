/********************************************************************************************************
 * @file    gpio.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/**	@page GPIO
 *
 *	Introduction
 *	===============
 * -#To prevent power leakage, you need to make sure that all GPIOs are not FLOATING, suggested use process is as follows:
 *    -# gpio_shutdown(GPIO_ALL);---All GPIOs except MSPI as well as SWS will be set to high resistance state.(Prevent power leakage.)
 *    -# gpio_setup_up_down_resistor(GPIO_SWS, PM_PIN_PULLUP_1M);---Ensure SWS is a constant level.(There are two purposes: the first is to prevent leakage,
 *     the second is to prevent the SWS no fixed level, generating some interfering signals through the sws mistakenly written to the chip resulting in death.)
 *    -# If you want to use GPIO as another function, please configure it yourself.
 *    -# Must ensure that all GPIOs cannot be FLOATING status before going to sleep to prevent power leakage.
 *
 *	API Reference
 *	===============
 *	Header File: gpio.h
 */
#ifndef DRIVERS_GPIO_H_
#define DRIVERS_GPIO_H_

#include <stdbool.h>
#include "lib/include/plic.h"
#include "analog.h"
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
typedef enum{
		GPIO_GROUP_A    = 0,
		GPIO_GROUP_B    = 1,
		GPIO_GROUP_C    = 2,
		GPIO_GROUP_D    = 3,
		GPIO_GROUP_E    = 4,
		GPIO_GROUP_F    = 5,
}gpio_group_e;
/**
 *  @brief  Define GPIO types
 *  @note	the following two points need to noticed when using PC5 GPIO port:
 *  		1. This pin is not recommend to use as wake-up source;
 *  		2. Since this pin is output function by default, even if it is configured with pull-up/pull-down retention,
 *  		when deep/deep Retention is invoked, it can't maintain high/low level and an abnormal level will occur.
 *  		Therefore, this pin can't be used in applications where a certain level state needs to be maintained all the time.
 *  		The PG group can only be used as an mspi pin and cannot be used as a wake-up source.
 */
typedef enum{
		GPIO_GROUPA    = 0x000,
		GPIO_GROUPB    = 0x100,
		GPIO_GROUPC    = 0x200,
		GPIO_GROUPD    = 0x300,
		GPIO_GROUPE    = 0x400,
		GPIO_GROUPF    = 0x500,
		GPIO_GROUPG    = 0X600,
		GPIO_ALL       = 0x700,
	    GPIO_PA0 = GPIO_GROUPA | BIT(0),
		GPIO_PA1 = GPIO_GROUPA | BIT(1),
		GPIO_PA2 = GPIO_GROUPA | BIT(2),
		GPIO_PA3 = GPIO_GROUPA | BIT(3),
		GPIO_PA4 = GPIO_GROUPA | BIT(4),
		GPIO_PA5 = GPIO_GROUPA | BIT(5),GPIO_DM=GPIO_PA5, // only support  DM
		GPIO_PA6 = GPIO_GROUPA | BIT(6),GPIO_DP=GPIO_PA6, // only support  DP
		GPIO_PA7 = GPIO_GROUPA | BIT(7),GPIO_SWS=GPIO_PA7,// only support  SWS(default)
		GPIOA_ALL = GPIO_GROUPA | 0x00ff,

		GPIO_PB0 = GPIO_GROUPB | BIT(0),
		GPIO_PB1 = GPIO_GROUPB | BIT(1),
		GPIO_PB2 = GPIO_GROUPB | BIT(2),
		GPIO_PB3 = GPIO_GROUPB | BIT(3),
		GPIO_PB4 = GPIO_GROUPB | BIT(4),
		GPIO_PB5 = GPIO_GROUPB | BIT(5),
		GPIO_PB6 = GPIO_GROUPB | BIT(6),
		GPIO_PB7 = GPIO_GROUPB | BIT(7),

		GPIO_PC0 = GPIO_GROUPC | BIT(0),//default: SSPI_CN
		GPIO_PC1 = GPIO_GROUPC | BIT(1),//default: SSPI_CK
		GPIO_PC2 = GPIO_GROUPC | BIT(2),//default: SSPI_SI
		GPIO_PC3 = GPIO_GROUPC | BIT(3),//default: SSPI_SO
		GPIO_PC4 = GPIO_GROUPC | BIT(4),//default: TDI
		GPIO_PC5 = GPIO_GROUPC | BIT(5),//default: TDO
		GPIO_PC6 = GPIO_GROUPC | BIT(6),//default: TMS
		GPIO_PC7 = GPIO_GROUPC | BIT(7),//default: TCK
		GPIOC_ALL = GPIO_GROUPC | 0x00ff,

		GPIO_PD0 = GPIO_GROUPD | BIT(0),
		GPIO_PD1 = GPIO_GROUPD | BIT(1),
		GPIO_PD2 = GPIO_GROUPD | BIT(2),
		GPIO_PD3 = GPIO_GROUPD | BIT(3),
		GPIO_PD4 = GPIO_GROUPD | BIT(4),
		GPIO_PD5 = GPIO_GROUPD | BIT(5),
		GPIO_PD6 = GPIO_GROUPD | BIT(6),
		GPIO_PD7 = GPIO_GROUPD | BIT(7),

		GPIO_PE0 = GPIO_GROUPE | BIT(0), // only support  LSPI_CN
		GPIO_PE1 = GPIO_GROUPE | BIT(1), // only support  LSPI_CK
		GPIO_PE2 = GPIO_GROUPE | BIT(2), // only support  LSPI_MOSI
		GPIO_PE3 = GPIO_GROUPE | BIT(3), // only support  LSPI_MISO
		GPIO_PE4 = GPIO_GROUPE | BIT(4), // only support  LSPI_IO2
		GPIO_PE5 = GPIO_GROUPE | BIT(5), // only support  LSPI_IO3
		GPIO_PE6 = GPIO_GROUPE | BIT(6),
		GPIO_PE7 = GPIO_GROUPE | BIT(7),
		GPIOE_ALL = GPIO_GROUPE | 0x00ff,

		GPIO_PF0 = GPIO_GROUPF | BIT(0),
		GPIO_PF1 = GPIO_GROUPF | BIT(1),
		GPIO_PF2 = GPIO_GROUPF | BIT(2),
		GPIO_PF3 = GPIO_GROUPF | BIT(3),
		GPIO_PF4 = GPIO_GROUPF | BIT(4),
		GPIO_PF5 = GPIO_GROUPF | BIT(5),
		GPIO_PF6 = GPIO_GROUPF | BIT(6), //Not support GSPI_CSN3,GSPI_CSN2,GSPI_CSN1,I2C1_SCL_IO,I2C1_SDA_IO in enum "gpio_func_e"
		GPIO_PF7 = GPIO_GROUPF | BIT(7), //Not support GSPI_CSN3,GSPI_CSN2,GSPI_CSN1,I2C1_SCL_IO,I2C1_SDA_IO in enum "gpio_func_e"

		GPIO_PG0 = GPIO_GROUPG | BIT(0), // only support  MSPI_MOSI(default)
		GPIO_PG1 = GPIO_GROUPG | BIT(1), // only support  MSPI_CK(default)
		GPIO_PG2 = GPIO_GROUPG | BIT(2), // only support  MSPI_IO3(default)
		GPIO_PG3 = GPIO_GROUPG | BIT(3), // only support  MSPI_CN(default)
		GPIO_PG4 = GPIO_GROUPG | BIT(4), // only support  MSPI_MISO(default)
		GPIO_PG5 = GPIO_GROUPG | BIT(5), // only support  MSPI_IO2(default)

}gpio_pin_e;
/**
 *  @brief  Define GPIO function pin types.
 */
typedef enum{

	    GPIO_FC_PA0 =GPIO_PA0 ,
		GPIO_FC_PA1 =GPIO_PA1,
		GPIO_FC_PA2 =GPIO_PA2,
		GPIO_FC_PA3 =GPIO_PA3,
		GPIO_FC_PA4 =GPIO_PA4,

		GPIO_FC_PB0 =GPIO_PB0,
		GPIO_FC_PB1 =GPIO_PB1,
		GPIO_FC_PB2 =GPIO_PB2,
		GPIO_FC_PB3 =GPIO_PB3,
		GPIO_FC_PB4 =GPIO_PB4,
		GPIO_FC_PB5 =GPIO_PB5,
		GPIO_FC_PB6 =GPIO_PB6,
		GPIO_FC_PB7 =GPIO_PB7,

		GPIO_FC_PC0 =GPIO_PC0,
		GPIO_FC_PC1 =GPIO_PC1,
		GPIO_FC_PC2 =GPIO_PC2,
		GPIO_FC_PC3 =GPIO_PC3,
		GPIO_FC_PC4 =GPIO_PC4,
		GPIO_FC_PC5 =GPIO_PC5,
		GPIO_FC_PC6 =GPIO_PC6,
		GPIO_FC_PC7 =GPIO_PC7,

		GPIO_FC_PD0 =GPIO_PD0,
		GPIO_FC_PD1 =GPIO_PD1,
		GPIO_FC_PD2 =GPIO_PD2,
		GPIO_FC_PD3 =GPIO_PD3,
		GPIO_FC_PD4 =GPIO_PD4,
		GPIO_FC_PD5 =GPIO_PD5,
		GPIO_FC_PD6 =GPIO_PD6,
		GPIO_FC_PD7 =GPIO_PD7,

		GPIO_FC_PE6 =GPIO_PE6,
		GPIO_FC_PE7 =GPIO_PE7,

		GPIO_FC_PF0 =GPIO_PF0,
		GPIO_FC_PF1 =GPIO_PF1,
		GPIO_FC_PF2 =GPIO_PF2,
		GPIO_FC_PF3 =GPIO_PF3,
		GPIO_FC_PF4 =GPIO_PF4,
		GPIO_FC_PF5 =GPIO_PF5,
		GPIO_FC_PF6 =GPIO_PF6, //Not support GSPI_CSN3,GSPI_CSN2,GSPI_CSN1,I2C1_SCL_IO,I2C1_SDA_IO in enum "gpio_func_e"
		GPIO_FC_PF7 =GPIO_PF7, //Not support GSPI_CSN3,GSPI_CSN2,GSPI_CSN1,I2C1_SCL_IO,I2C1_SDA_IO in enum "gpio_func_e"
		GPIO_NONE_PIN =0x00,
}gpio_func_pin_e;

/**
 *  @brief  Define GPIO function mux types
 */
typedef enum{

	    PWM0           =1,
		PWM1           =2,
		PWM2           =3,
		PWM3           =4,
		PWM4           =5,
		PWM5           =6,
		PWM0_N         =7,
		PWM1_N         =8,
		PWM2_N         =9,
		PWM3_N         =10,
		PWM4_N         =11,
		PWM5_N         =12,

		GSPI_CSN0_IO    =13,
		GSPI_CLK_IO     =14,
		GSPI_IO3_IO    =15,
		GSPI_IO2_IO    =16,
		GSPI_MISO_IO   =17,
		GSPI_MOSI_IO   =18,

		I2C_SCL_IO     =19,
		I2C_SDA_IO     =20,

		UART0_CTS_I    =21,
		UART0_RTS      =22,
		UART0_TX       =23,
		UART0_RTX_IO   =24,
		UART1_CTS_I    =25,
		UART1_RTS      =26,
		UART1_TX       =27,
		UART1_RTX      =28,

		CLK_7816       =29,

		I2S0_BCK_IO    =30,
		I2S0_LR_OUT_IO =31,
		I2S0_DAT_OUT   =32,
		I2S0_LR_IN_IO  =33,
		I2S0_DAT_IN_I  =34,
		I2S0_CLK       =35,
		I2S1_BCK_IO    =36,
		I2S1_LR_OUT_IO =37,
		I2S1_DAT_OUT   =38,
		I2S1_LR_IN_IO  =39,
		I2S1_DAT_IN_I  =40,
		I2S1_CLK       =41,

		DMIC0_CLK      =42,
		DMIC0_DAT_I    =43,
		DMIC1_CLK      =44,
		DMIC1_DAT_I    =45,

		DBG_PROBE_CLK  =46,
		TX_CYC2PA      =47,
		WIFI_DENY_I    =48,
		BT_ACTIVITY    =49,
		BT_STATUS      =50,
		BT_INBAND      =51,
		ATSEL_0        =52,
		ATSEL_1        =53,
		ATSEL_2        =54,
		ATSEL_3        =55,
		RX_CYC2LNA     =56,

		I2C1_SDA_IO    =59, //Not support PF6,PF7 in enum "gpio_func_pin_e"
		I2C1_SCL_IO    =60, //Not support PF6,PF7 in enum "gpio_func_pin_e"
		GSPI_CSN1      =61, //Not support PF6,PF7 in enum "gpio_func_pin_e"
		GSPI_CSN2      =62, //Not support PF6,PF7 in enum "gpio_func_pin_e"
		GSPI_CSN3      =63, //Not support PF6,PF7 in enum "gpio_func_pin_e"
}gpio_func_e;

/**
 *  @brief  Define GPIO mux func
 */
typedef enum{
	    AS_GPIO,
		AS_MSPI,

		AS_SWS,
		AS_SWM,

		AS_USB_DP,
		AS_USB_DM,

		AS_TDI,
		AS_TDO,
		AS_TMS,
		AS_TCK,
}gpio_fuc_e;

/*
 * @brief define gpio irq status types
 */
typedef enum{
	FLD_GPIO_IRQ_CLR                      = BIT(0),
	FLD_GPIO_IRQ_GPIO2RISC0_CLR           = BIT(1),
	FLD_GPIO_IRQ_GPIO2RISC1_CLR           = BIT(2),
}gpio_irq_status_e;


/*
 *  @brief define gpio irq mask types
 */
typedef enum{
	GPIO_IRQ_MASK_GPIO       = 			BIT(2),
	GPIO_IRQ_MASK_GPIO2RISC0 = 			BIT(3),
	GPIO_IRQ_MASK_GPIO2RISC1 = 			BIT(4),
}gpio_irq_mask_e;

/*
 * @brief define gpio group irq types
 */
typedef enum{
	FLD_GPIO_GROUP_IRQ0                     = BIT(0),
	FLD_GPIO_GROUP_IRQ1                     = BIT(1),
	FLD_GPIO_GROUP_IRQ2                     = BIT(2),
	FLD_GPIO_GROUP_IRQ3                     = BIT(3),
	FLD_GPIO_GROUP_IRQ4                     = BIT(4),
	FLD_GPIO_GROUP_IRQ5                     = BIT(5),
	FLD_GPIO_GROUP_IRQ6                     = BIT(6),
	FLD_GPIO_GROUP_IRQ7                     = BIT(7),
}gpio_group_irq_e;

/**
 *  @brief  Define rising/falling types
 */
typedef enum{
	POL_RISING   = 0,
	POL_FALLING  = 1,
}gpio_pol_e;

/**
 *  @brief  Define interrupt types
 */
typedef enum{
	 INTR_RISING_EDGE=0,
	 INTR_FALLING_EDGE ,
	 INTR_HIGH_LEVEL,
	 INTR_LOW_LEVEL,
} gpio_irq_trigger_type_e;

/**
 *  @brief  Define pull up or down types
 *  @note   The GPIO pull-up/pull-down resistance is a simulation result by the internal
 *          MOSFET and affected by the IO voltage VDDO3. The lower the IO voltage of GPIO,
 *          the higher the pull-up/pull-down resistance of GPIO.
 */
typedef enum {
	GPIO_PIN_UP_DOWN_FLOAT      = 0,
	GPIO_PIN_PULLUP_1M        	= 1,
	GPIO_PIN_PULLDOWN_100K  	= 2,
	GPIO_PIN_PULLUP_10K 		= 3,
}gpio_pull_type_e;

typedef enum{
	PROBE_CCLK 				= 0,
	PROBE_HCLK 				= 1,
	PROBE_PCLK 				= 2,
	PROBE_CLKPLL 			= 3,
	PROBE_XTL24M 			= 4,
	PROBE_RC24M 			= 5,
	PROBE_CLK32K 			= 6,
	PROBE_CLK_STIMER 		= 7,
	PROBE_CLK_MSPI 			= 8,
	PROBE_CLK_GSPI 			= 9,
	PROBE_CLK_LSPI 			= 10,
	PROBE_CLK_USBPHY 		= 11,
	PROBE_CLK_ZB_MST		= 12,
	PROBE_CLK_ZB_32K		= 13,
	PROBE_CODEC_MCLK	    = 14,
	PROBE_CLK_EOC			= 15,
	PROBE_CLK_EOC_SRC		= 16,
	PROBE_CODEC_ADC_CLK_6M	= 17,
	PROBE_CODEC_ADC_CLK_1M	= 18,
	PROBE_CODEC_DAC_CLK		= 19,
	PROBE_ANA_PA_CLK_6M		= 20,
}probe_clk_sel_e;
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
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_func(pin), bit);
}

/**
 * @brief      This function servers to disable gpio function.
 * @param[in]  pin - the selected pin.
 * @return     none.
 */
static inline void gpio_function_dis(gpio_pin_e pin)
{
	unsigned char	bit = pin & 0xff;
	BM_CLR(reg_gpio_func(pin), bit);
}

/**
 * @brief     This function set the pin's output high level.
 * @param[in] pin - the pin needs to set its output level.
 * @return    none.
 */
static _always_inline void gpio_set_high_level(gpio_pin_e pin)
{
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_out(pin), bit);

}

/**
 * @brief     This function set the pin's output low level.
 * @param[in] pin - the pin needs to set its output level.
 * @return    none.
 */
static _always_inline void gpio_set_low_level(gpio_pin_e pin)
{
	unsigned char	bit = pin & 0xff;
	BM_CLR(reg_gpio_out(pin), bit);

}

/**
 * @brief     This function set the pin's output level.
 * @param[in] pin - the pin needs to set its output level
 * @param[in] value - value of the output level(1: high 0: low)
 * @return    none
 */
static _always_inline void gpio_set_level(gpio_pin_e pin, unsigned char value)
{
	if(value)
	{
		gpio_set_high_level(pin);
	}
	else
	{
		gpio_set_low_level(pin);
	}
}

/**
 * @brief     This function read the pin's input level.
 * @param[in] pin - the pin needs to read its input level.
 * @return    1: the pin's input level is high.
 * 			  0: the pin's input level is low.
 */
static inline bool gpio_get_level(gpio_pin_e pin)
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
	p[6] = reg_gpio_pg_in;
}

/**
 * @brief     This function set the pin toggle.
 * @param[in] pin - the pin needs to toggle.
 * @return    none.
 */
static _always_inline void gpio_toggle(gpio_pin_e pin)
{
	reg_gpio_out(pin) ^= (pin & 0xFF);
}

/**
 * @brief      This function enable the output function of a pin.
 * @param[in]  pin - the pin needs to set the output function.
 * @return     none.
 */
static inline void gpio_output_en(gpio_pin_e pin)
{
	unsigned char	bit = pin & 0xff;
	BM_CLR(reg_gpio_oen(pin), bit);
}

/**
 * @brief      This function disable the output function of a pin.
 * @param[in]  pin - the pin needs to set the output function.
 * @return     none.
 */
static inline void gpio_output_dis(gpio_pin_e pin)
{
	unsigned char	bit = pin & 0xff;
	BM_SET(reg_gpio_oen(pin), bit);
}

/**
 * @brief      This function enable set output function of a pin.
 * @param[in]  pin - the pin needs to set the output function (1: enable,0: disable)
 * @return     none
 */
static inline void gpio_set_output(gpio_pin_e pin, unsigned char value)
{
	if(value)
	{
		gpio_output_en(pin);
	}
	else
	{
		gpio_output_dis(pin);
	}

}

/**
 * @brief      This function determines whether the output function of a pin is enabled.
 * @param[in]  pin - the pin needs to determine whether its output function is enabled.
 * @return     1: the pin's output function is enabled.
 *             0: the pin's output function is disabled.
 */
static inline bool  gpio_is_output_en(gpio_pin_e pin)
{
	return !BM_IS_SET(reg_gpio_oen(pin), pin & 0xff);
}

/**
 * @brief     This function determines whether the input function of a pin is enabled.
 * @param[in] pin - the pin needs to determine whether its input function is enabled(not include group_pc).
 * @return    1: the pin's input function is enabled.
 *            0: the pin's input function is disabled.
 */
static inline bool gpio_is_input_en(gpio_pin_e pin)
{
	return BM_IS_SET(reg_gpio_ie(pin), pin & 0xff);
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

/**
 * @brief     This function set a pin's IRQ.
 * @param[in] pin 			- the pin needs to enable its IRQ.
 * @param[in] trigger_type  - gpio interrupt type.
 * 							  0: rising edge.
 * 							  1: falling edge.
 * 							  2: high level.
 * 							  3: low level.
 * @return    none.
 */
void gpio_set_irq(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);

/**
 * @brief     This function set a pin's IRQ_RISC0.
 * @param[in] pin 			- the pin needs to enable its IRQ.
 * @param[in] trigger_type  - gpio interrupt type 0:rising edge 1:falling edge 2:high level 3:low level.
 * @return    none.
 */
void gpio_set_gpio2risc0_irq(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);

/**
 * @brief     This function set a pin's IRQ_RISC1.
 * @param[in] pin 			- the pin needs to enable its IRQ.
 * @param[in] trigger_type  - gpio interrupt type 0:rising edge 1:falling edge 2:high level 3:low level.
 * @return    none.
 */
void gpio_set_gpio2risc1_irq(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);

/**
 * @brief     This function is used to set the gpio interrupt
 * @param[in] pin 			 - gpio pin that needs to enable irq
 *							  <p> This parameter can only be set to the pin in GPIO_GROUP selected using the function "gpio_set_src_irq_group()"
 *							  <p> For example, if you call the function gpio_set_src_irq_group(GPIO_GROUP_A) to select GPIO_GROUP_A,
 *							  <p> the pin parameter of function gpio_set_src_irq() can only select the following gpio:GPIO_PA0/GPIO_PA1/GPIO_PA2/GPIO_PA3/GPIO_PA4/GPIO_PA5/GPIO_PA6/GPIO_PA7
 * @param[in] trigger_type  - gpio interrupt type.
 * 							  0: rising edge.
 * 							  1: falling edge.
 * 							  2: high level.
 * 							  3: low level
 * @attention <p> GPIO_PX0 (GPIO_PA0/GPIO_PB0/... /GPIO_PF0) corresponds to the interrupt source IRQ_GPIO_SRC0
 *			  <p> GPIO_PX1 (GPIO_PA1/GPIO_PB1/... /GPIO_PF1) corresponds to the interrupt source IRQ_GPIO_SRC1
 *			  <p> ...
 *			  <p> GPIO_PX7 (GPIO_PA7/GPIO_PB7/... /GPIO_PF7) corresponds to the interrupt source IRQ_GPIO_SRC7
 * @return    none.
 */
void gpio_set_src_irq(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type);

/**
 * @brief      This function serves to set the gpio-mux function.
 * @param[in]  pin      - the pin needs to set.
 * @param[in]  function - the function need to set.
 * @return     none.
 */
void gpio_set_mux_function(gpio_func_pin_e pin,gpio_func_e function);

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
 * @param[in] up_down_res - the type of the pull-up/down resistor,0:FLOAT 1:PULLUP_1M 2:PULLDOWN_100K 3:PULLUP_10K.
 * @return    none.
 */
void gpio_set_up_down_res(gpio_pin_e pin, gpio_pull_type_e up_down_res);

/**
 * @brief     This function set pin's 30k pull-up register.
 * @param[in] pin - the pin needs to set its pull-up register,not include PF[5:0] and PG[5:0] which are not available.
 * @return    none.
 */
void gpio_set_pullup_res_30k(gpio_pin_e pin);

/**
 * @brief     This function set probe clk output.
 * @param[in] pin
 * @param[in] sel_clk
 * @return    none.
 */
void  gpio_set_probe_clk_function(gpio_func_pin_e pin,probe_clk_sel_e sel_clk);

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

/**
 * @brief     This function select the irq group source.
 * @param[in] group - enum gpio_group_e, to specify which GPIO_GROUP_x to enable
 * @note      To enable the gpio group interrupt, you would need to do the following:
 *			  gpio_set_src_irq_group()   - specify the group of gpio to enable
 *            gpio_set_src_irq()		 - select the pin from the group to enable the interrupt.
 *
 *            you could only set the pin in the GPIO_GROUP that has been selected
 * @return    none.
 */
static inline void gpio_set_src_irq_group(gpio_group_e group)
{
	reg_gpio_irq_sel &= (~group);
	reg_gpio_irq_sel |= group;
}

#endif




