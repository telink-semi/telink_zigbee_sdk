/********************************************************************************************************
 * @file    software_uart.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "software_uart.h"
//#include "vendor/common/app_common.h"


#if (SOFT_UART_ENABLE)

volatile soft_uart_rece_t soft_uart_rece;
typedef int (*soft_uart_rx_handler_t)(void);
soft_uart_rx_handler_t soft_uart_RxHandler = 0; //rx

typedef int (*soft_uart_sdk_adv_handler_t)(void);
soft_uart_sdk_adv_handler_t soft_uart_sdk_advhandler = 0;

typedef void (*soft_uart_SoftUartisRfState_handler_t)(int);
soft_uart_SoftUartisRfState_handler_t soft_uart_SoftUartisRfStatehandler = 0;

/**
 * @brief      software serial port interrupt handler function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void soft_uart_irq_handler(void)
{

	if (SOFT_UART_DIS & soft_uart_rece.state)
		return;
	soft_uart_rece.mutex_flag = 1;//lock
	//IO irq
	if ((reg_irq_src & FLD_IRQ_GPIO_EN) == FLD_IRQ_GPIO_EN) {
		reg_irq_src |= FLD_IRQ_GPIO_EN; // clear the relevant irq
		if ((gpio_read(SOFT_UART_RX_IO) == 0)&&(SOFT_UART_WAIT & soft_uart_rece.state)) {
				BM_CLR(reg_gpio_irq_wakeup_en(SOFT_UART_RX_IO), SOFT_UART_RX_IO & 0xff); // close GPIO irq
				soft_uart_rece.bit_num = 0x00;
				soft_uart_rece.temp_byte = 0x00;
				soft_uart_rece.state &= ~SOFT_UART_DONE_CHECK;
				soft_uart_rece.state &= ~SOFT_UART_WAIT;
				soft_uart_rece.state |= SOFT_UART_WORK;
				soft_uart_rece.done_count = 0;
				timer0_set_mode(TIMER_MODE_SYSCLK, 0, soft_uart_rece.time_interval);
				timer_start(TIMER0);
		}
	}

	//time irq
	if (timer_get_interrupt_status(FLD_TMR_STA_TMR0)) {
		timer_clear_interrupt_status(FLD_TMR_STA_TMR0); //clear irq status

		if (soft_uart_rece.state & SOFT_UART_WORK) {
			if ( gpio_read(SOFT_UART_RX_IO)) { //
				soft_uart_rece.temp_byte |= BIT(soft_uart_rece.bit_num);
			}
			soft_uart_rece.bit_num++;
			if (8 == soft_uart_rece.bit_num) {
				soft_uart_rece.bit_num = 0x00;
				soft_uart_rece.state |= SOFT_UART_STOP_CHECK; //change state
				soft_uart_rece.state &= ~SOFT_UART_WORK;
			}

		} else if (soft_uart_rece.state & SOFT_UART_STOP_CHECK) {
			soft_uart_rece.state &= ~SOFT_UART_STOP_CHECK;
			if ( gpio_read(SOFT_UART_RX_IO)) { //
				soft_uart_rece.data[soft_uart_rece.data_count + 4] = soft_uart_rece.temp_byte; //len + buf
				soft_uart_rece.temp_byte = 0x00;
				soft_uart_rece.data_count++;
				if (soft_uart_rece.data_count >= soft_uart_rece.data_size) { //over flow
					soft_uart_rece.data[0] = soft_uart_rece.data_count;
					if (soft_uart_RxHandler)
						soft_uart_RxHandler();
				}
			}
			soft_uart_rece.state |= SOFT_UART_DONE_CHECK;
			soft_uart_rece.state |= SOFT_UART_WAIT;
			reg_irq_src |= FLD_IRQ_GPIO_EN; // clear the relevant irq
			BM_SET(reg_gpio_irq_wakeup_en(SOFT_UART_RX_IO), SOFT_UART_RX_IO & 0xff); //start io irq

		} else if (soft_uart_rece.state & SOFT_UART_DONE_CHECK) {
			soft_uart_rece.done_count++;
			if (UART_RECE_DONE_NUM <= soft_uart_rece.done_count) {
				timer_stop(TIMER0);
				if(soft_uart_rece.data_count > 0){
					soft_uart_rece.data[0] = soft_uart_rece.data_count;
					if (soft_uart_RxHandler)
						soft_uart_RxHandler();
				}
				soft_uart_rece.state &= ~SOFT_UART_DONE_CHECK;
			}

		}
	}
	//irq end
	soft_uart_rece.mutex_flag = 0;//unlock


}

/**
 * @brief
 * @param[in]	none
 * @return      none
 */
void soft_uart_dis(void)
{

	while(soft_uart_rece.mutex_flag == 1);
	soft_uart_rece.state = SOFT_UART_DIS;
	timer_clear_interrupt_status(FLD_TMR_STA_TMR0); //clear irq status
	reg_irq_src |= FLD_IRQ_GPIO_EN; // clear the relevant irq
	timer_stop(TIMER0);
	BM_CLR(reg_gpio_irq_wakeup_en(SOFT_UART_RX_IO), SOFT_UART_RX_IO & 0xff); // close GPIO irq

	soft_uart_rece.bit_num = 0x00;
	soft_uart_rece.temp_byte = 0x00;

	soft_uart_rece.stop_count = 0;
	soft_uart_rece.done_count = 0;

}

/**
 * @brief
 * @param[in]	none
 * @return      none
 */
void soft_uart_en(void)
{
	soft_uart_rece.bit_num = 0x00;
	soft_uart_rece.temp_byte = 0x00;

	soft_uart_rece.stop_count = 0;
	soft_uart_rece.done_count = 0;
	soft_uart_rece.state = SOFT_UART_WAIT;
	timer_clear_interrupt_status(FLD_TMR_STA_TMR0); //clear irq status
	reg_irq_src |= FLD_IRQ_GPIO_EN; // clear the relevant irq
	BM_SET(reg_gpio_irq_wakeup_en(SOFT_UART_RX_IO), SOFT_UART_RX_IO & 0xff); //start io irq
}

/**
 * @brief
 * @param[in]	none
 * @return      none
 */
void soft_uart_init(void)
{

	// set software rx io
	gpio_set_func(SOFT_UART_RX_IO, AS_GPIO);
	gpio_set_output_en(SOFT_UART_RX_IO, 0);
	gpio_set_input_en(SOFT_UART_RX_IO, 1);
	gpio_setup_up_down_resistor(SOFT_UART_RX_IO, PM_PIN_PULLUP_10K);
	gpio_set_interrupt(SOFT_UART_RX_IO, POL_FALLING);

	//set software tx io
	gpio_set_func(SOFT_UART_TX_IO , AS_GPIO);
	gpio_setup_up_down_resistor(SOFT_UART_TX_IO, PM_PIN_PULLUP_1M);
	gpio_set_output_en(SOFT_UART_TX_IO,1);//Enable output
	gpio_write(SOFT_UART_TX_IO, 1);// Add this code to fix the problem that the first byte will be error.

	soft_uart_rece.bit_num = 0x00;
	soft_uart_rece.temp_byte = 0x00;

	soft_uart_rece.stop_count = 0;
	soft_uart_rece.done_count = 0;

	soft_uart_rece.state = SOFT_UART_WAIT;

	soft_uart_rece.mutex_flag = 0;

	soft_uart_rece.time_interval = (1000000 / SOFT_UART_BAUD_RATE) * CLOCK_SYS_CLOCK_1US + SOFT_UART_OFFSET;
	//SET TIME
	timer0_set_mode(TIMER_MODE_SYSCLK, 0, SOFT_UART_INTERVAL * CLOCK_SYS_CLOCK_1US);
	timer_stop(TIMER0);
}


/**
 * @brief     data receive buffer initiate function. ,
 *            uart packet length needs to be no larger than (recBuffLen - 4).
 * @param[in] pRxbuf - pointer to the receiving buffer
 * @param[in] size - length in byte of the receiving buffer, should in range of 5 ~ 260
 * @return    none
 */
_attribute_ram_code_ void soft_uart_RxSetFifo(unsigned char *pRxbuf, int size)
{
	soft_uart_rece.data = pRxbuf;
	soft_uart_rece.data_size = size-4;		//The first four bytes of the array are the received length
	soft_uart_rece.data_count = 0;
}


/**
 * @brief       The interrupt receives the callback function
 * @param[in]	callback
 * @return      none
 */
void soft_uart_rx_handler(void *prx)
{
	soft_uart_RxHandler = prx;
}

void soft_uart_sdk_adv_handler(void *prx)
{
	soft_uart_sdk_advhandler = prx;
}

void soft_uart_SoftUartisRfState_handler(void *prx)
{
	soft_uart_SoftUartisRfStatehandler = prx;
}

/**
 * @brief      This function serves to format string.
 * @param[in]  byte  -  a byte need to print
 * @return     none.
 */

_attribute_ram_code_sec_noinline_ void soft_uart_putchar(unsigned char byte) {

	gpio_write(SOFT_UART_TX_IO, 0);		//uart start bit
	sleep_us(SOFT_UART_INTERVAL);
	for (unsigned char j = 0; j < 8; j++) {
		gpio_write(SOFT_UART_TX_IO, byte&0x01);
		byte >>= 1;
		sleep_us(SOFT_UART_INTERVAL); //send data bit
	}

	gpio_write(SOFT_UART_TX_IO, 1);		//uart stop bit
	sleep_us(SOFT_UART_STOP_BIT_INTERVAL);
}

void soft_uart_send(unsigned char * buf, unsigned char len) {

	unsigned char i;
	for (i = 0; i < len; i++) {

		unsigned char s;
		extern u8 blc_ll_getCurrentState(void);
		s = blc_ll_getCurrentState();
		/*
		 *
		 #define			BLS_LINK_STATE_IDLE								0
		 #define			BLS_LINK_STATE_ADV								BIT(0)
		 #define			BLS_LINK_STATE_SCAN								BIT(1)
		 #define			BLS_LINK_STATE_INIT								BIT(2)
		 #define 		    BLS_LINK_STATE_CONN								BIT(3)
		 * */
		if (s == BIT(3)) {
			if(soft_uart_SoftUartisRfStatehandler)
				soft_uart_SoftUartisRfStatehandler(SOFT_UART_SEND_ONE_BYTE);
		} else if (s == BIT(0)) {
			if(soft_uart_sdk_advhandler)
				soft_uart_sdk_advhandler();
		}
		soft_uart_putchar(buf[i]);
	}

}

#endif

