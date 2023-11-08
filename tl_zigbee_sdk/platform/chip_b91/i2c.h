/********************************************************************************************************
 * @file    i2c.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/**	@page I2C
 *
 *	Introduction
 *	===============
 *	i2c support master mode or slave mode.
 *
 *	API Reference
 *	===============
 *	Header File: i2c.h
 */

#ifndef I2C_H
#define I2C_H
#include "gpio.h"

#include "analog.h"
#include "reg_include/i2c_reg.h"
#include "dma.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
extern unsigned char i2c_slave_rx_index;

/**
 *  @brief  select pin as SDA and SCL of i2c
 */
typedef enum{
	I2C_GPIO_SDA_B3		= GPIO_PB3,
	I2C_GPIO_SDA_C2		= GPIO_PC2,
	I2C_GPIO_SDA_E2		= GPIO_PE2,
	I2C_GPIO_SDA_E3		= GPIO_PE3,
}i2c_sda_pin_e;


typedef enum{
	I2C_GPIO_SCL_B2		= GPIO_PB2,
	I2C_GPIO_SCL_C1		= GPIO_PC1,
	I2C_GPIO_SCL_E0		= GPIO_PE0,
	I2C_GPIO_SCL_E1		= GPIO_PE1,
}i2c_scl_pin_e;



typedef enum{
	I2C_RX_BUF_MASK         =  BIT(2),
	I2C_TX_BUF_MASK         =  BIT(3),
	I2C_TX_DONE_MASK		=  BIT(4),
	I2C_RX_DONE_MASK        =  BIT(5),
}i2c_irq_mask_e;



typedef enum{
	I2C_RX_BUFF_CLR  		= BIT(6),
	I2C_TX_BUFF_CLR         = BIT(7),
}i2c_buff_clr_e;

/**
 *  @brief  Define UART IRQ BIT STATUS
 *  -# I2C_TXDONE_STATUS:when the stop signal is detected, an interrupt occurs.
 *  -# I2C_TX_BUF_STATUS:rxfifo_cnt <= tx_irq_trig_lev, generate interrupt.
 *  -# I2C_RXDONE_STATUS:when the stop signal is detected, an interrupt occurs.
 *  -# I2C_RX_BUF_STATUS:rxfifo cnt> = i2c_rx_irq_trig_cnt generates an interrupt.
 */
typedef enum{

	I2C_TXDONE_STATUS          = BIT(0),
	I2C_TX_BUF_STATUS          = BIT(1),
	I2C_RXDONE_STATUS          = BIT(2),
	I2C_RX_BUF_STATUS          = BIT(3),

}i2c_irq_status_e;


typedef enum{
I2C_TX_DONE_CLR  		= BIT(4),
}i2c_irq_clr_e;

/**
 * @brief      The function of this API is to determine whether the bus is busy.
 * @return     1:Indicates that the bus is busy. 0:Indicates that the bus is free
 */
static inline _Bool i2c_master_busy(void)
{
    return reg_i2c_mst & FLD_I2C_MST_BUSY;
}

/**
 * @brief      This function serves to get the txfifo cnt,FLD_I2C_TX_BUFCNT is decremented when there are outgoing sent data in the txfifo,
 *             when there is write data in the tx_fifo, this register is increased.
 * @return     The actual number of bytes in txfifo.
 */
static inline unsigned char i2c_get_tx_buf_cnt(void)
{
   return (reg_i2c_buf_cnt & FLD_I2C_TX_BUFCNT)>>4;
}


/**
 * @brief      This function serves to get the rxfifo cnt,FLD_I2C_RX_BUFCNT is increased when there are incoming received data rxfifo,
 *             when there is read data in the rxfifo, FLD_I2C_RX_BUFCNT is decremented.
 * @return     The actual number of bytes in rxfifo.
 */
static inline unsigned char i2c_get_rx_buf_cnt(void)
{
   return (reg_i2c_buf_cnt & FLD_I2C_RX_BUFCNT);
}


/**
 * @brief      The function of this API is to set the number of bytes to triggered the receive interrupt.
 *             Its default value is 4. We recommend setting it to 1 or 4.
 * @param[in]  cnt - the interrupt trigger level,the range is less than 8.
 * @return     none
 */
static inline void i2c_rx_irq_trig_cnt(unsigned char cnt)
{
   /*
	  in the i2c_rx_irq_trig_cnt interface,originally first set i2c_rc_irq_trig_cnt to 0 and then assign,
      if the rx_buff mask is opened first, when set i2c_rc_irq_trig_cnt to 0,rx_fifo is empty, an interrupt will be triggered by mistake.
   */
	reg_i2c_trig=(((reg_i2c_trig)&(~FLD_I2C_RX_IRQ_TRIG_LEV))|(cnt& 0x0f));
}

/**
 * @brief      The function of this interface is equivalent to that after the user finishes calling the write or read interface, the stop signal is not sent,
 * 			   and then the write or read command is executed again. The driver defaults that every write or read API will send a stop command at the end
 * @param[in]  en - Input parameters.Decide whether to disable the stop function after each write or read interface
 * @return     none
 */
void i2c_master_send_stop(unsigned char en);

/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  sda_pin - the pin port selected as I2C sda pin port.
 * @param[in]  scl_pin - the pin port selected as I2C scl pin port.
 * @return     none
 */
void i2c_set_pin(i2c_sda_pin_e sda_pin,i2c_scl_pin_e scl_pin);


/**
 * @brief      This function serves to enable i2c master function.
 * @return     none.
 */
void i2c_master_init(void);


/**
 * @brief      This function serves to enable i2c RX/TX/MASK_RX/MASK_TX  interrupt function.
 * @param[in]  mask - to select Interrupt type.
 * @return     none
 */
static inline void i2c_set_irq_mask(i2c_irq_mask_e mask)
{
	reg_i2c_sct0  |=  mask;
}

/**
 * @brief      This function serves to disable i2c RX/TX/MASK_RX/MASK_TX  interrupt function.
 * @param[in]  mask - to select Interrupt type.
 * @return     none
 */
static inline void i2c_clr_irq_mask(i2c_irq_mask_e mask)
{
	reg_i2c_sct0  &=  (~mask);
}


/**
 * @brief      This function serves to get i2c interrupt status.
 * @return     i2c interrupt status.
 *
 */
static inline unsigned char i2c_get_irq_status(i2c_irq_status_e status)
{
	return reg_i2c_irq_status&status;
}

/**
 * @brief     This function is used to set the 'i2c_slave_rx_index' to 0,
 *            'i2c_slave_rx_index' is used to synchronize the rxfifo hardware pointer in no_dma mode.
 * @return    none.
 * @note      note the following:
 *            -# After calling the i2c reset interface, i2c_slave_clr_rx_index must be called to clear the read pointer,
 *               after the i2c reset interface is invoked, the hardware read pointers are cleared to zero.
 *               Therefore, the software write pointers are cleared to ensure logical correctness.
 *            -# After suspend wakes up, you must call i2c_slave_clr_rx_index to clear read pointers,
 *               because after suspend wakes up, the chip is equivalent to performing a i2c_reset,
 *               so the software read pointer also needs to be cleared to zero.
 */
static inline void i2c_slave_clr_rx_index()
{
	i2c_slave_rx_index=0;
}

/**
 * @brief      This function serves to clear i2c rx/tx fifo.
 * @param[in]  clr - to select Interrupt type.
 * @return     none
 */
static inline void i2c_clr_fifo(i2c_buff_clr_e clr)
{
	 reg_i2c_status = clr;
	 if(I2C_RX_BUFF_CLR == clr)
	{
		i2c_slave_clr_rx_index();
	}
}

/**
 * @brief      This function serves to clear i2c irq status.
 * @return     none
 */
static inline void  i2c_clr_irq_status(i2c_irq_clr_e status)
{
	    reg_i2c_irq_status=status;
}

/**
 * @brief   this function set reset i2c,i2c logic  will be reset.
 * @return    none
 */
static inline void i2c_reset(void)
{
	reg_rst0 &= ~(FLD_RST0_I2C);
	reg_rst0 |= FLD_RST0_I2C;
}

/**
 *@brief     This function serves to enable i2c slave stretch function,conjunction with stretch function of master,
 *           slave auto stretch clk enable,open this function, use slave to receive data,when data buffer is full, scl bus will be low to stop receive data.
 *@return    none.
 */
static inline void i2c_slave_stretch_en(void){

	reg_i2c_slave_stretch_en |= FLD_I2C_R_CLK_STRETCH_SEN;
}

/**
 *@brief     This function serves to disable i2c slave stretch function.
 *@return    none.
 */
static inline void i2c_slave_stretch_dis(void){

	reg_i2c_slave_stretch_en &= ~FLD_I2C_R_CLK_STRETCH_SEN;
}

/**
 *@brief     This function serves to i2c slave manual stretch function,conjunction with stretch function of master,
 *           When this interface is called, clk will be pulled, it should be noted that this interface can only be called when the master is in the idle state.
 *@return    none.
 */
static inline void i2c_slave_manual_stretch_en(){
	reg_i2c_slave_stretch_en|=FLD_I2C_R_MANUAL_STRETCH;
}

/**
 *@brief     This function serves to clear i2c slave manual stretch function,When the interface is called, clk recovers.
 *@return    none.
 */
static inline void i2c_slave_manual_stretch_clr(){
	reg_i2c_slave_stretch_en |= FLD_I2C_MANUAL_STRETCH_CLR;
}
/**
 *@brief     This function serves to enable i2c master stretch function,
 *           If stretch is enabled on the slave, the master needs to be enabled,by default, it is enabled on the interface i2c_master_init.
 *@return    none.
 */
static inline void i2c_master_stretch_en(void){

	reg_i2c_sct0 |=FLD_I2C_R_CLK_STRETCH_EN;
}

/**
 *@brief     This function serves to disable i2c master stretch function.
 *@return    none.
 */
static inline void i2c_master_stretch_dis(void){

	reg_i2c_sct0 &=~(FLD_I2C_R_CLK_STRETCH_EN);
}


/**
 * @brief      This function serves to enable slave mode.
 * @param[in]  id - the id of slave device.it contains write or read bit,the laster bit is write or read bit.
 *                       ID|0x01 indicate read. ID&0xfe indicate write.
 * @return     none
 */
void i2c_slave_init(unsigned char id);


/**
 *  @brief      The function of this API is to ensure that the data can be successfully sent out.
 *              can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *              but if a nack exception is received after sending out the id, during exception handling, a stop signal will be sent.
 *  @param[in]  id - to set the slave ID.
 *  @param[in]  data - The data to be sent, The first three bytes can be set as the RAM address of the slave.
 *  @param[in]  len - This length is the total length, including both the length of the slave RAM address and the length of the data to be sent.
 *  @return     0 : the master receive NACK after sending out the id and then send stop.  1: the master sent the data successfully,(master does not detect NACK in data phase)
 */

unsigned char i2c_master_write(unsigned char id, unsigned char *data, unsigned char len);


/**
 * @brief      This function serves to read a packet of data from the specified address of slave device.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received after sending out the id, during exception handling, a stop signal will be sent.
 * @param[in]  id - to set the slave ID.
 * @param[in]  data - Store the read data
 * @param[in]  len - The total length of the data read back.
 * @return     0 : the master receive NACK after sending out the id and then send stop.  1: the master receive the data successfully.
 */

unsigned char i2c_master_read(unsigned char id, unsigned char *data, unsigned char len);


/**
 * @brief      This function serves to write data and restart read data.
 *             can choose whether to send stop or not,If i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception signal is received after sending out the id, during exception handling, a stop signal will be sent.
 * @param[in]  id - to set the slave ID.
 * @param[in]  wr_data - The data to be sent, The first three bytes can be set as the RAM address of the slave.
 * @param[in]  wr_len -  This length is the total length, including both the length of the slave RAM address and the length of the data to be sent.
 * @param[in]  rd_data - Store the read data
 * @param[in]  rd_len -  The total length of the data read back.
 * @return     0 : the master receive NACK after sending out the id and then send stop.  1: the master receive the data successfully.
 */
unsigned char i2c_master_write_read(unsigned char id, unsigned char *wr_data, unsigned char wr_len, unsigned char *rd_data, unsigned char rd_len);

/**
 * @brief      The function of this API is just to write data to the i2c tx_fifo by DMA.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but in B91,in dma mode,there is no way to detect the abnormal phenomenon,so there is no corresponding exception handling method in case of abnormal phenomenon.
 * @param[in]  id - to set the slave ID.
 * @param[in]  data - The data to be sent, The first three bytes represent the RAM address of the slave.
 * @param[in]  len - This length is the total length, including both the length of the slave RAM address and the length of the data to be sent,
 *                   the maximum transmission length of i2c is 0xff bytes, so dont'n over this length.
 * @return     none.
 * @note       data: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void i2c_master_write_dma(unsigned char id, unsigned char *data, unsigned char len);



/**
 * @brief      This function serves to read a packet of data from the specified address of slave device.
 * @param[in]  id - to set the slave ID.
 * @param[in]  rx_data - Store the read data.
 * @param[in]  len - The total length of the data read back,the maximum transmission length of i2c is 0xFF bytes, so dont'n over this length.
 * @return     none.
 * @note       data: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void i2c_master_read_dma(unsigned char id, unsigned char *rx_data, unsigned char len);




/**
 * @brief      This function serves to send a packet of data to master device.It will trigger after the master sends the read sequence.
 * @param[in]  data - the pointer of tx_buff.
 * @param[in]  len - The total length of the data,the maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return     none.
 * @note       data: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void i2c_slave_set_tx_dma( unsigned char *data, unsigned char len);



/**
 * @brief      This function serves to receive a packet of data from master device,It will trigger after the master sends the write sequence.
 * @param[in]  data - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 *                    and the actual buffer size defined by the user needs to be not smaller than the len, otherwise there may be an out-of-bounds problem.
 * @param[in]  len  - This parameter is used to set the size of the received dma and must be set to a multiple of 4. The maximum value that can be set is 0xFFFFFC.
 * @return 	   none
 */
void i2c_slave_set_rx_dma(unsigned char *data, unsigned char len);


/**
 * @brief     This function serves to receive data .
 * @param[in]  data - the data need read.
 * @param[in]  len - The total length of the data
 * @return    none
 */
void i2c_slave_read(unsigned char* data , unsigned char len );


/**
 * @brief     This function serves to receive i2c data by byte with not DMA method.
 * @param[in]  data - the data need send.
 * @param[in]  len - The total length of the data.
 * @return    none
 */
void i2c_slave_write(unsigned char* data , unsigned char len);


/**
 * @brief      This function serves to set the i2c clock frequency.The i2c clock is consistent with the pclk.
 * @param[in]  clock - the division factor of I2C clock,the i2c frequency can meet 1M, and the maximum limit is not confirmed,
 *             I2C frequency = pclk/ (4*DivClock).
 * @return     none
 */
void i2c_set_master_clk(unsigned char clock);

/**
 * @brief     This function serves to set i2c tx_dma channel and config dma tx default.
 * @param[in] chn: dma channel.
 * @return    none
 * @note      In the case that the DMA transfer is not completed(bit 0 of reg_dma_ctr0(chn): 1-the transmission has not been completed,0-the transmission is completed), re-calling the DMA-related functions may cause problems.
 *            If you must do this, you must perform the following sequence:
 *            1. dma_chn_dis(i2c_dma_tx_chn) 2.i2c_reset() 3.i2c_master_write_dma()/i2c_slave_set_tx_dma()
 */
void i2c_set_tx_dma_config(dma_chn_e chn);

/**
 * @brief     This function serves to set i2c rx_dma channel and config dma rx default.
 * @param[in] chn: dma channel.
 * @return    none
 * @note      In the case that the DMA transfer is not completed(bit 0 of reg_dma_ctr0(chn): 1-the transmission has not been completed,0-the transmission is completed), re-calling the DMA-related functions may cause problems.
 *            If you must do this, you must perform the following sequence:
 *            1. dma_chn_dis(i2c_dma_tx_chn) 2.i2c_reset() 3.i2c_master_read_dma()/i2c_slave_set_rx_dma()
 */
void i2c_set_rx_dma_config(dma_chn_e chn);


#endif










