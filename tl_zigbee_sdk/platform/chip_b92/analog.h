/********************************************************************************************************
 * @file    analog.h
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
/*******************************      analog control registers: 0xb8      ******************************/
/**	@page ANALOG
 *
 *	Introduction
 *	===============
 *  analog support dma and normal mode, in each mode, support byte/halfword/word/buffer write and read.
 *
 *	API Reference
 *	===============
 *	Header File: analog.h
 */
#pragma once


#include "reg_include/register.h"
#include "dma.h"
#include "compiler.h"
#include "core.h"
#include "error_handler/error_handler.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * When reading and writing analog registers in DMA mode, exit interface after configuration.
 * But the actual operation of the analog register is not finished, and the DMA is still moving the data.
 * An interrupt may be opened at this time, and if there is an operation on the analog register,
 * it will interrupt the previous DMA reading and writing the analog register, creating an unknown risk.
 * Therefore, it is not recommended to use DMA to read and write analog registers.
 */
#define ANALOG_DMA		0

/**
 * @brief      This function serves to analog register read by byte.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_optimize_o2_ unsigned char analog_read_reg8(unsigned char addr);

/**
 * @brief      This function serves to analog register write by byte.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_optimize_o2_ void analog_write_reg8(unsigned char addr, unsigned char data);

/**
 * @brief      This function serves to analog register read by halfword.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_optimize_o2_ unsigned short analog_read_reg16(unsigned char addr);

/**
 * @brief      This function serves to analog register write by halfword.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_optimize_o2_ void analog_write_reg16(unsigned char addr, unsigned short data);

/**
 * @brief      This function serves to analog register read by word.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_optimize_o2_ unsigned int analog_read_reg32(unsigned char addr);

 /**
  * @brief      This function serves to analog register write by word.
  * @param[in]  addr - address need to be write.
  * @param[in]  data - the value need to be write.
  * @return     none.
  */
_attribute_ram_code_sec_optimize_o2_ void analog_write_reg32(unsigned char addr, unsigned int data);
/**
 * @brief      This function serves to analog register read.
 * @param[in]  addr  - address need to be read.
 * @param[out] buff  - the ptr of buffer to store the read data.
 * @param[in]  len   - the length of read value.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_read_buff(unsigned char addr, unsigned char *buff, unsigned char len);

/**
 * @brief      This function serves to analog register write.
 * @param[in]  addr  - address need to be write.
 * @param[in]  buff  - the ptr of value need to be write.
 * @param[in]  len   - the length of write value.(The data length cannot be greater than 8)
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_buff(unsigned char addr, unsigned char *buff, unsigned char len);

#if (ANALOG_DMA == 1)
/**
 * @brief      This function serves to analog register write by word using dma.
 * @param[in]  chn  - the dma channel.
 * @param[in]  addr - address need to be write.
 * @param[in]  pdat - the ptr of data need to be write.
 * @return     none.
 */
void analog_write_reg32_dma(dma_chn_e chn, unsigned char addr, void *pdat);

/**
 * @brief      This function serves to analog register write by word using dma.
 * @param[in]  chn  - the dma channel.
 * @param[in]  addr - address need to be read.
 * @param[out] pdat - the buffer ptr to store read data.
 * @return     none.
 */
void analog_read_reg32_dma(dma_chn_e chn, unsigned char addr,void *pdat);

/**
 * @brief      This function write buffer to analog register by dma channel.
 * @param[in]  chn  - the dma channel.
 * @param[in]  addr - address need to be write.
 * @param[in]  pdat - the buffer ptr need to be write.
 * @param[in]  len  - the length of buffer.
 * @return     none.
 */
void analog_write_buff_dma(dma_chn_e chn, unsigned char addr, unsigned char *pdat, unsigned int len);

/**
 * @brief      This function write buffer to analog register by dma channel.
 * @param[in]  chn  - the dma channel.
 * @param[in]  addr - address need to be read from.
 * @param[out] pdat - the buffer ptr to store read data.
 * 			   note: The size of array pdat must be a multiple of 4.
 * 			  	 	For example, if you just need read 5 byte by dma, you should
 * 			  	 	define the size of array pdat to be greater than 8 other than 5.
 * 			  	 	Because the dma would return 4 byte data every time, 5 byte is
 * 			  	 	not enough to store them.
 * @param[in]  len  - the length of read data.
 * @return     none.
 */
void analog_read_buff_dma(dma_chn_e chn, unsigned char addr, unsigned char *pdat, unsigned int len);

/**
 * @brief      This function write buffer to analog register by dma channel.
 * @param[in]  chn  - the dma channel.
 * @param[in]  pdat - the buffer(addr & data) ptr need to be write,
 * 			   note: The array pdat should look like this,
 * 			   |  pdat     |            |        |
 * 			   |  :------  | :----------|  :---- |
 * 			   |  pdat[0]  |   address  |  0x3a  |
 * 			   |  pdat[1]  |    data    |  0x11  |
 * 			   |  pdat[2]  |   address  |  0x3b  |
 *			   |  pdat[3]  |    data    |  0x22  |
 *			   |  ......   |            |        |
 * 				It means write data 0x11 to address 0x3a,
 * 						 write data 0x22 to address 0x3b,
 * 						 ......
 * @param[in]  len - the length of read data.
 * @return     none.
 */
void analog_write_addr_data_dma(dma_chn_e chn, void *pdat, int len);
#endif


/********************************************************************************************************
 *											internal
 *******************************************************************************************************/

/********************************************************************************************************
 * 				this is only internal interface, customers do not need to care.
 *******************************************************************************************************/
/**
 * @brief      This function serves to judge whether analog Tx buffer is empty.
 * @return     0:not empty      1: empty
 */
_attribute_ram_code_sec_optimize_o2_  bool analog_txbuf_no_empty(void);

/**
 * @brief      This function serves to judge whether analog is busy.
 * @return     0: not busy  1:busy
 */
_attribute_ram_code_sec_optimize_o2_  bool analog_busy(void);

/**
 * @brief      This function serves to judge whether analog write/read is busy .
 * @return     none.
 */
#define analog_wait()                  wait_condition_fails_or_timeout(analog_busy,g_drv_api_error_timeout_us,drv_timeout_handler,(unsigned int)DRV_API_ERROR_TIMEOUT_ANALOG_WAIT)

/**
 * @brief      This function serves to judge whether analog Tx buffer is empty.
 * @return     none.
 */
#define analog_wait_txbuf_no_empty()  wait_condition_fails_or_timeout(analog_txbuf_no_empty,g_drv_api_error_timeout_us,drv_timeout_handler,(unsigned int)DRV_API_ERROR_TIMEOUT_ANA_TX_BUFCNT)



