/********************************************************************************************************
 * @file     mdec.h
 *
 * @brief    This is the MDEC driver header file for TLSR8278
 *
 * @author	 Driver Group
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#pragma once
#include "bsp.h"
#include "analog.h"
#include "register.h"
#include "gpio_8278.h"
#include "clock.h"


typedef  enum{
	GPIO_PA0_MDEC,
	GPIO_PB7_MDEC,
	GPIO_PC4_MDEC,
	GPIO_PD0_MDEC
}MDEC_PinTypeDef;

/**
 * @brief     This function servers to reset the MDEC module.When the system is wakeup by MDEC, you should
 * 			  to reset the MDEC module to clear the flag bit of MDEC wakeup.
 * @param[in] none
 * @return    none
 */
static inline void mdec_reset(void)
{
	analog_write(mdec_rst_addr,analog_read(mdec_rst_addr) | FLD_RST_MDEC);
	analog_write(mdec_rst_addr,analog_read(mdec_rst_addr) & (~FLD_RST_MDEC));
}

/**
 * @brief This function is used to initialize the MDEC module,include clock setting and input IO select.
 * @param[in]    pin - MDEC_PinTypeDef
 * @return none
 */
void mdec_init(MDEC_PinTypeDef pin);

/**
 * @brief This function is used to read the receive data of MDEC module's IO.
 * @param[out]   dat - The array to store date.
 * @return       1--decode success,  0--decode failure.
 */
unsigned char mdec_read_dat(unsigned char *dat);


