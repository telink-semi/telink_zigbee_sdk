/********************************************************************************************************
 * @file    software_uart.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    01,2022
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef BLT_SOFTWARE_UART_H_
#define BLT_SOFTWARE_UART_H_

#include "driver_8258.h"
#include "types.h"


#ifndef SOFT_UART_ENABLE
#define SOFT_UART_ENABLE                 				0
#endif


#if (SOFT_UART_ENABLE)


#ifndef SOFT_UART_BAUD_RATE
#define SOFT_UART_BAUD_RATE               9600
#endif

#ifndef SOFT_UART_TX_IO
#define SOFT_UART_TX_IO                   GPIO_PD0
#endif

#ifndef SOFT_UART_RX_IO
#define SOFT_UART_RX_IO                   GPIO_PD1
#endif

#ifndef	SOFT_UART_INTERVAL
#define	SOFT_UART_INTERVAL				 (1000*1000/SOFT_UART_BAUD_RATE)
#endif

#ifndef SOFT_UART_STOP_BIT_SIZE
#define	SOFT_UART_STOP_BIT_SIZE			1
#endif

#ifndef SOFT_UART_STOP_BIT_INTERVAL
#define SOFT_UART_STOP_BIT_INTERVAL		(SOFT_UART_INTERVAL*SOFT_UART_STOP_BIT_SIZE)
#endif

#ifndef	SOFT_UART_OFFSET
#define	SOFT_UART_OFFSET				 1
#endif


#define SOFT_UART_SEND_ONE_BYTE          (SOFT_UART_INTERVAL*20)



#define UART_RECE_DONE_NUM           	3    /*3 * 104us*/

/**
 *  @brief  Define state bits
 */
#define	    SOFT_UART_CLR		  	   	0x0000
#define	    SOFT_UART_WAIT		  	   	0x0001
#define	 	SOFT_UART_WORK	 		  	0x0002
#define	 	SOFT_UART_STOP_CHECK  	  	0x0004
#define	 	SOFT_UART_DONE_CHECK 	  	0x0008
#define	 	SOFT_UART_OVERFLOW 	   	  	0x0010
#define	 	SOFT_UART_ERR 	  	  	 	0x0020
#define     SOFT_UART_DIS            	0x8000


#define     mutex_lock(a)           	{a=1;}
#define     mutex_unlock(a)             {a=0;}
#define     mutex_take(a)            	(a)

/**
 *  @brief  define uart recevice struct
 */
typedef struct{
	unsigned short  time_interval;					//Time interval calculated by baud rate
	unsigned char   stop_count;    					//Reserve
	unsigned char   done_count;    	 				//time out done (UART_RECE_DONE_NUM)
	volatile unsigned short  state;         		//operating state

	unsigned char   temp_byte;     					//Temporarily store a byte
	unsigned char 	bit_num; 	   					//A bit to receive

	unsigned int 	data_size; 	   					//Maximum receive cache
	unsigned char 	data_count;	   					//Receive cache counter

	volatile unsigned char   mutex_flag;            //lock is 1 ,unlock is 0
	unsigned char rsdv[2];



	unsigned char 	*data;        	    			//Receive cache address
}soft_uart_rece_t;



typedef void (*soft_uart_irq_handler_t)(void);

/**
 * @brief     init
 * @param[in] none
 * @return    none
 */
void soft_uart_init(void);



/**
 * @brief      software serial port interrupt handler function
 * @param[in]	none
 * @return      none
 */
void soft_uart_irq_handler(void);

/**
 * @brief     data receive buffer initiate function. ,
 *            uart packet length needs to be no larger than (recBuffLen - 4).
 * @param[in] pRxbuf - pointer to the receiving buffer
 * @param[in] size - length in byte of the receiving buffer
 * @return    none
 */
void soft_uart_RxSetFifo(unsigned char *pRxbuf, int size);

/**
 * @brief       Disabling the software serial port
 * @param[in]	none
 * @return      none
 */
void soft_uart_dis(void);

/**
 * @brief       Open the software serial port
 * @param[in]	none
 * @return      none
 */
void soft_uart_en(void);

/**
 * @brief       The interrupt receives the callback function
 * @param[in]	callback
 * @return      none
 */
void soft_uart_rx_handler(void *prx);

void soft_uart_sdk_adv_handler(void *prx);

void soft_uart_SoftUartisRfState_handler(void *prx);

/**
 * @brief       Serial port sending data
 * @param[in]	pointer to the buffer containing data need to send
 * @param[in]   buffer len
 * @return      none
 */
void soft_uart_send(unsigned char * buf, unsigned char len);



#endif
#endif /* BLT_SOFTWARE_UART_H_ */
