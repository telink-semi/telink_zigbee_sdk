/********************************************************************************************************
 * @file     random.h 
 *
 * @brief    This is the header file for TLSR8258
 *
 * @author	 junyuan.zhang@telink-semi.com;
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

/**
 * @brief     This function performs to preparatory initials random generator.
 * @param[in] none.
 * @return    none.
 */
void rng_init(void);

/**
 * @brief     This function performs to initialize the rand time in flash/sram.
 *            (example: system clock:16M, code in flash 23us, code in sram 4us)
 * @param[in] none.
 * @return    the value of time.
 */
unsigned int rand(void);

/*********************************************************************
 * @fn          generateRandomNum
 *
 * @brief       generate random number
 *
 * @param       len - len
 *
 * @param       data -  buffer
 *
 * @return      None
 */
void generateRandomNum(int len, unsigned char *data);

/*********************************************************************
 * @fn          rng_get_result_buff
 *
 * @brief       generate random number
 *
 * @param       len - len
 *
 * @param       data -  buffer
 *
 * @return      None
 */
void rng_get_result_buff(int len, unsigned char *data);

/** \defgroup GP10  Random Usage
 * 	This DEMO uses the random number function of the ADC to generate pseudo-random numbers. \n
 * 	@{
 */

//-----------------------------------------------------------1-10
/*! \page random Random Usage
Next, we will introduce how to use ADC RNG of API interface: \n
__1.RNG__ \n

This DEMO implements RNG generation by setting the operating mode of the ADC and related registers.\n

API Interface Function : \n

~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
  void random_generator_pre_init(void)
  void random_generator_post_init(void)
  _attribute_ram_code_ unsigned int rand(void)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

case 1:  This DEMO is mainly pre-initialization and initialization of ADC RNG, and then you can directly read pseudo-random numbers. \n

~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
int main (void) {

unsigned short random;

	cpu_wakeup_init();


	random_generator_pre_init();

	random_generator_post_init();


	while (1) {
		random = rand();
	}
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~


\n
*/
 /** @}*/ //end of GP10
