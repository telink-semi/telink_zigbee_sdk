/********************************************************************************************************
 * @file    i2c.h
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
/**	@page I2C
 *
 *	Header File: i2c.h
 *
 *	How to use this driver
 *	==============
 The i2c has two modules: i2c/i2c1_m
- I2C(It can be used as a master or slave)
 -# I2C Master
  -# Initialization and Configuration
     -# nodma/dma:
        - Initialize the sda/scl pin by i2c_set_pin() API;
        - Configure the speed by i2c_set_master_clk() API;
        - Enable master function by i2c_master_init() API;
     -# dma
        - dma initial configuration by i2c_set_rx_dma_config() / i2c_set_tx_dma_config() API;
  -# Interrupts Configuration and Response
     -# interrupt initial configuration:
        - nodma_write
           - polling write, it does not need to configure interrupt;
        - nodma_read
           - polling read, it does not need to configure interrupt;
        - dma_write/dma_read
           - i2c_set_irq_mask() : I2C_MASTER_NAK_MASK;
        - plic_interrupt_enable() / core_interrupt_enable() API;
     -# interrupt response(for details, see the enumeration i2c_irq_status_e):
        - dma_write/dma_read
          - i2c_get_irq_status() : I2C_MASTER_NAK_STATUS;
          - i2c_clr_irq_status() : I2C_MASTER_NAK_STATUS;
  -# TX and RX(note: during write/read, if nack is detected, send a stop signal,and abort the current write/read)
     -# nodma_write
        - polling write by i2c_master_write() API;
     -# nodma_read
        - polling send by i2c_master_read() API;
     -# dma_write
        - write data by i2c_master_write_dma() API,check whether write is complete by polling i2c_master_busy() API;
     -# dma_read
        - read data by i2c_master_read_dma() API,check whether read is complete by polling i2c_master_busy() API;
 -# I2C Slave(the slave can be divided into two cases, whether to combine the stretch function or not)
    -# no stretch
       -# Initialization and Configuration
          -# nodma/dma:
            - Initialize the sda/scl pin by i2c_set_pin() API;
            - Enable slave function and set id by i2c_slave_init() API;
          -# dma
            - dma initial configuration by i2c_set_rx_dma_config() / i2c_set_tx_dma_config() API;
       -# Interrupts Configuration and Response
          -# interrupts initial configuration:
            - nodma_write
              - no associated interrupt is used;
            - nodma_read
              - i2c_rx_irq_trig_cnt() / i2c_set_irq_mask() : I2C_RX_BUF_MASK|I2C_RX_END_MASK;
            - dma_write
              - no associated interrupt is used;
            - dma_read
              - dma_set_irq_mask() : TC_MASK;
            - plic_interrupt_enable() / core_interrupt_enable() API;
       -# interrupt response(for details, see the enumeration i2c_irq_status_e):
          - i2c_get_irq_status() ;
          - i2c_clr_irq_status() ;
          - nodma_read
            - I2C_RX_BUF_STATUS/I2C_RX_END_STATUS
          - dma_write/read
            - See the TC_MASK note for details;
       -# TX and RX
          - nodma_write
             - polling write by i2c_slave_write() API;
          - nodma_read
             - The data is read via I2C_RX_BUF_MASK and I2C_RX_END_MASK interrupt,the data is read in the interrupt by i2c_slave_read() API;
          - dma_write
             - write data by i2c_slave_set_tx_dma() API;
          - dma_read
             - read data by i2c_slave_set_rx_dma() API;

    -# enable stretch
       -# Initialization and Configuration
          -# nodma/dma:
            - Initialize the sda/scl pin by i2c_set_pin() API;
            - Enable slave function and set id by i2c_slave_init() API;
            - Enable stretch function by i2c_slave_stretch_en() API;
          -# dma
            - dma initial configuration by i2c_set_rx_dma_config() / i2c_set_tx_dma_config() API;
       -# Interrupts Configuration and Response
          -# interrupts initial configuration:
            - nodma_write
              - i2c_set_irq_mask() : I2C_SLAVE_WR_MASK;
            - nodma_read
              - i2c_rx_irq_trig_cnt() / i2c_set_irq_mask() : I2C_RX_BUF_MASK|I2C_RX_END_MASK;
            - dma_write
              - i2c_set_irq_mask() : I2C_SLAVE_WR_MASK;
            - dma_read
              - i2c_set_irq_mask() : I2C_RX_END_MASK;
            - plic_interrupt_enable() / core_interrupt_enable() API;
       -# interrupt response(for details, see the enumeration i2c_irq_status_e):
          - i2c_get_irq_status() ;
          - i2c_clr_irq_status() ;
          - nodma_write
            - I2C_SLAVE_WR_STATUS
          - nodma_read
            - I2C_RX_BUF_STATUS/I2C_RX_END_STATUS
          - dma_write
            - I2C_SLAVE_WR_STATUS
          - dma_read
            - I2C_RX_END_STATUS
       -# TX and RX
        -# nodma_write
         - polling write by i2c_slave_write() API;
        -# nodma_read
         - The data is read via I2C_RX_BUF_MASK and I2C_RX_END_MASK interrupt,the data is read in the interrupt by i2c_slave_read() API;
        -# dma_write
         - write data by i2c_slave_set_tx_dma() API;
        -# dma_read
         - read data by i2c_slave_set_rx_dma() API;
    -# the attention:
      - when the slave is used with the stretch function, Determine when the master reads the data and then use the fill data by interrupting I2C_SLAVE_WR_MASK (nodma:i2c_slave_write() dma:i2c_slave_set_tx_dma());
      - When the slave is not used with stretch function, if I2C_SLAVE_WR_MASK is used to interrupt to judge when the master reads and fills the data, the mcu cannot handle it. Therefore, it is necessary to fill the data in advance before the master read data (nodma:i2c_slave_write() dma:i2c_slave_set_tx_dma());
      - The i2c_master_send_stop() API is used to determine whether the master sends a stop signal after the sending or receiving is complete
- i2c1_m(Only as master,only nodma,)
     -# Initialization and Configuration
       - Initialize the sda/scl pin by i2c1_m_set_pin() API;
       - Configure the speed by i2c1_m_set_master_clk() API;
       - Enable master function by i2c1_m_master_init() API;
     -# TX and RX(For write and read, you can optionally add an address)
       - i2c1_m_master_write() API,if send the address, just put the address in the txbuff and send it together as data;
       - i2c1_m_master_write_read() API;
     -# the attention:
       - The i2c1_m_master_send_stop() API is used to determine whether the master sends a stop signal after the sending or receiving is complete;
- Timeout mechanism(i2c/i2c1_m)
     -# i2c_set_error_timeout()/i2c1_m_set_error_timeout():define global variable g_i2c_error_timeout_us/g_i2c1_m_error_timeout_us,
        the default value is a large value,can use the preceding interfaces to adjust the value based on actual applications.
     -# i2c_get_error_timeout_code()/i2c1_m_get_error_timeout_code(): when an error timeout abnormally, can use the above interface to read which error belongs to i2c_api_error_code_e/i2c1_m_api_error_code_e.
     -# i2c_timeout_handler()/i2c1_m_timeout_handler():when an error timeout exits abnormally,can do timeout processing at the application layer or the application layer redefines the interface.
 */
#ifndef I2C_H
#define I2C_H
#include <stdbool.h>
#include "analog.h"
#include "dma.h"
#include "gpio.h"
#include "reg_include/i2c_reg.h"
#include "error_handler/error_handler.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
extern unsigned char i2c_slave_rx_index;

/**
 *  @brief  Define UART IRQ BIT MASK
 */
typedef enum{
    I2C_SLAVE_WR_MASK      =  BIT(0),/**<
	                                      <pre> the slave parsing master cmd interrupt,when received the master read or write cmd, will generate interrupt.
                                          Combined with the stretch function i2c_slave_stretch_en(),it is generally used to inform the i2c slave when to fill txfifo to write data.</pre>
	                                     */
	I2C_MASTER_NAK_MASK     =  BIT(1),/**< master detect to the nack, will generate interrupt */
	I2C_RX_BUF_MASK         =  BIT(2),/**< rxfifo_cnt >= FLD_I2C_RX_IRQ_TRIG_LEV generates an interrupt */
	I2C_TX_BUF_MASK         =  BIT(3),/**< txfifo_cnt <= FLD_I2C_TX_IRQ_TRIG_LEV, generate interrupt */
	I2C_RX_DONE_MASK 		=  BIT(4),/**< when the stop signal is detected, an interrupt occurs */
	I2C_TX_DONE_MASK        =  BIT(5),/**< when the stop signal is detected, an interrupt occurs */
	I2C_RX_END_MASK         =  BIT(6),/**< An interrupt is generated when one frame of data is received(the stop signal has been sent) */
	I2C_TX_END_MASK         =  BIT(7),/**< An interrupt is generated when one frame of data is sent(the stop signal has been sent) */
	I2C_STRETCH_IRQ         =  BIT(8),/**< Combined with the stretch function i2c_slave_stretch_en(),the slave stretch interrupt,when tx_fifo is empty or rx_fifo is full,will generate interrupt */
}i2c_irq_mask_e;

/**
 *  @brief  Define I2C IRQ bit status and explain what needs to be done in the interrupt.
 */
typedef enum{
	I2C_SLAVE_WR_STATUS      =  BIT(0),   /**<
	                                            <pre>get interrupt status:i2c_get_irq_status(), clr interrupt status:i2c_clr_irq_status().
	                                            in nodma mode,i2c slave writes data by i2c_slave_write() API;
	                                            in dma mode,i2c slave writes data by i2c_slave_set_tx_dma() API;</pre>
	                                         */
	I2C_MASTER_NAK_STATUS    =  BIT(1),  /**<
	                                           <pre>
	                                           get interrupt status:i2c_get_irq_status(), clr interrupt status:i2c_clr_irq_status().
                                               in nodma mode,nodma does not need this interrupt;
                                               for master,in dma mode:
                                                  When write data, reg_i2c_sct1 = (FLD_I2C_LS_STOP)-> while(i2c_master_busy())-> dma_chn_dis(I2C_TX_DMA_CHN)-> i2c_clr_irq_status(I2C_TX_BUF_STATUS);
                                                  when read data,reg_i2c_sct1 = (FLD_I2C_LS_STOP)-> while(i2c_master_busy())-> dma_chn_dis(I2C_TX_DMA_CHN);
                                               </pre>
                                         */

	I2C_RX_BUF_STATUS        =  BIT(2),  /**<
	                                            <pre>get interrupt status:i2c_get_irq_status(), clr interrupt status: automatically cleared.
	                                            if using the i2c_clr_irq_status(), will clear the clear RX FIFO pointer.
	                                            in nodma mode,i2c slave read data by i2c_get_rx_buf_cnt() / i2c_slave_read().
	                                            in dma mode,dma does not need this interrupt.</pre>
	                                         */
	I2C_TX_BUF_STATUS        =  BIT(3), /**<  <pre>in general, this interrupt is not required</pre> */
	I2C_RX_DONE_STATUS 		 =  BIT(4), /**<  <pre>usually don't use this interrupt,use I2C_RX_END_STATUS instead</pre> */
	I2C_TX_DONE_STATUS       =  BIT(5), /**<  <pre>usually don't use this interrupt,use I2C_TX_END_STATUS instead</pre> */
	I2C_RX_END_STATUS        =  BIT(6), /**<
                                           <pre>
                                              get interrupt status:i2c_get_irq_status(), clr interrupt status: i2c_clr_irq_status().
                                              Generally speaking, it is used for i2c slaves.
                                             in no dma,i2c slave read data by i2c_get_rx_buf_cnt() / i2c_slave_read().
                                            </pre>
                                        */

	I2C_TX_END_STATUS        =  BIT(7),/**<  <pre>get interrupt status:i2c_get_irq_status(), clr interrupt status: i2c_clr_irq_status()</pre>  */
	I2C_STRETCH_STATUS       =  BIT(8),/**<  <pre>get interrupt status:i2c_get_irq_status(), clr interrupt status: i2c_clr_irq_status()</pre>  */
}i2c_irq_status_e;



/**
 * this register is use to get slave relevant status
 * BIT[1] slave stretch indicate.
 * BIT[2] judge slave tx_fifo is empty.
 * BIT[3] judge slave rx_fifo is full.
 */
typedef enum{
	I2C_SLAVER_STRETCH_INDICATE         = BIT(1),
	I2C_SLAVER_TX_EMPTY                 = BIT(2),
	I2C_SLAVE_RX_FULL                   = BIT(3),
}i2c_slave_status1_e;


/**
 *  judge the read and write status of slave.
 */
typedef enum{
    I2C_SLAVE_WRITE   = 1,
	I2C_SLAVE_READ    = 0,
}i2c_slave_wr_e;

/**
 * judge the read and write status of master.
 */
typedef enum{
	I2C_MASTER_WRITE =0,
	I2C_MASTER_READ =1,
}i2c_master_wr_e;


// i2c api error code
typedef enum {
	I2C_API_ERROR_TIMEOUT_NONE              = 0x00,
	I2C_API_ERROR_TIMEOUT_ID			    = 0x01,
	I2C_API_ERROR_TIMEOUT_WRITE_DATA		= 0x02,
	I2C_API_ERROR_TIMEOUT_READ_DATA         = 0x03,
	I2C_API_ERROR_TIMEOUT_STOP		        = 0x04,
} i2c_api_error_timeout_code_e;

/**
* record i2c error code, can obtain the value through the i2c_get_error_timeout_code interface.
*/
extern volatile i2c_api_error_timeout_code_e g_i2c_error_timeout_code;

/**
 * i2c error timeout(us),a large value is set by default,can set it by i2c_set_error_timeout().
 */
extern unsigned int g_i2c_error_timeout_us;

/**
 * @brief      The function of this API is to determine whether the bus is busy.
 * @return     1:Indicates that the bus is busy. 0:Indicates that the bus is free
 * @note       If upper-layer application calls the interface, the interface may be stuck due to an exception on both sides of the communication.
 *             If the upper-layer uses the timeout processing mechanism, see set_i2c_error_timeout_us (time setting requirement).
 */
static inline bool i2c_master_busy(void)
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
 * @brief      This function serves to enable i2c interrupt function.
 * @param[in]  mask - to select interrupt type.
 * @return     none
 */
static inline void i2c_set_irq_mask(i2c_irq_mask_e mask)
{
	if(mask & I2C_STRETCH_IRQ)
	{
		reg_i2c_ctrl2  |=  FLD_I2C_MASK_STRETCH;
	}

	 reg_i2c_mask0  |=  mask;
}

/**
 * @brief      This function serves to disable i2c interrupt function.
 * @param[in]  mask - to select Interrupt type.
 * @return     none
 */
static inline void i2c_clr_irq_mask(i2c_irq_mask_e mask)
{
	if(mask & I2C_STRETCH_IRQ)
	{
		reg_i2c_ctrl2  &= (~ FLD_I2C_MASK_STRETCH);
	}

	    reg_i2c_mask0  &=  (~mask);
}

/**
 * @brief      This function serves to get i2c interrupt status.
 * @param[in]  status     - to select Interrupt status type.
 * @retval     non-zero       - the interrupt occurred.
 * @retval     zero   - the interrupt did not occur.
 *
 */
static inline unsigned char i2c_get_irq_status(i2c_irq_status_e status)
{
	if(status  & I2C_STRETCH_IRQ)
	{
		return i2c_slave_status1&FLD_I2C_SS_SCL_IRQ;
	}

	    return reg_i2c_status0&status;

}

/**
 * @brief     This function is used to set the 'i2c_slave_rx_index' to 0,
 *            'i2c_slave_rx_index' is used to synchronize the rxfifo hardware pointer in no_dma mode.
 * @return    none.
 * @note      note the following:
 *            -# After calling the i2c reset interface, i2c_slave_clr_rx_index must be called to clear the read pointer,
 *               after the i2c reset interface is invoked, the hardware read pointers are cleared to zero.
 *               Therefore, the software write pointers are cleared to ensure logical correctness.
 *               (the above processing is done in i2c_hw_fsm_reset)
 *            -# After suspend wakes up, you must call i2c_slave_clr_rx_index to clear read pointers,
 *               because after suspend wakes up, the chip is equivalent to performing a i2c_reset,
 *               so the software read pointer also needs to be cleared to zero.
 */
static inline void i2c_slave_clr_rx_index(void)
{
	i2c_slave_rx_index=0;
}

/**
 * @brief     This function serves to i2c finite state machine reset(the configuration register is still there and does not need to be reconfigured).
 * @return    none.
 */
static inline void i2c_hw_fsm_reset(void)
{
	reg_rst0 &= (~FLD_RST0_I2C);
	reg_rst0 |= FLD_RST0_I2C;
	i2c_slave_clr_rx_index();
	g_i2c_error_timeout_code = I2C_API_ERROR_TIMEOUT_NONE;
}

/**
 * @brief      This function serves to clear i2c irq status.
 * @param[in]  status - to select interrupt status type.
 * @return     none
 */
static inline void  i2c_clr_irq_status(i2c_irq_status_e status)
{
	/**
      [0]:ss_read    read only
      [1]:ss_scl     read only
      [2]:tx_empty   read only
      [3]:rx_full    read only
      [6]:ss_scl_irq	Write 1 to clear zero
      (add by xianren.yang, confirmed by xuqiang.zhang 20231017)
	 */
	if(status  & I2C_STRETCH_STATUS)
	{
		i2c_slave_status1 = FLD_I2C_SS_SCL_IRQ;
	}
   if(status &  I2C_RX_BUF_STATUS){
	   i2c_slave_clr_rx_index();
   }
	 reg_i2c_status0=status;
}

/**
 * @brief      This function serves to enable slave mode.
 * @param[in]  id - the id of slave device.it contains write or read bit,the laster bit is write or read bit.
 *                  ID|0x01 indicate read. ID&0xfe indicate write.
 * @return     none
 */
void i2c_slave_init(unsigned char id);



/**
 * @brief      The function of this API is to ensure that the data can be successfully sent out.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received in id or data phase, during exception handling, a stop signal will be sent.
 *             1.in the id phase,detect nack,stop sending.
 *             2.in the data phase,detect nack,stop sending.
 * @param[in]  id   - to set the slave ID.
 * @param[in]  data - The data to be sent.
 * @param[in]  len  - This length is the total length, including both the length of the slave RAM address and the length of the data to be sent.
 * @return     0: received nack in id or data phase,and then stop
 *               - check whether the id is correct;
 *               - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *               - check whether the slave is abnormal;
 *             1: write successfully;
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c_set_error_timeout);
 */
unsigned char i2c_master_write(unsigned char id, unsigned char *data, unsigned int len);


/**
 * @brief      This function serves to read a packet of data from the specified address of slave device.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received in id phase, during exception handling, a stop signal will be sent.
 *             only in id phase, detect nack,stop receiving.
 * @param[in]  id   - to set the slave ID.
 * @param[in]  data - Store the read data
 * @param[in]  len  - The total length of the data read back.
 * @return     0: the master receive NACK after sending out the id and then send stop;
 *               - check whether the id is correct;
 *               - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *               - check whether the slave is abnormal;
 *             1: the master receive the data successfully;
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c_set_error_timeout);
 */
unsigned char  i2c_master_read(unsigned char id, unsigned char *data, unsigned int len);

/**
 * @brief      This function serves to write data and read data,
 *             can choose whether to send stop or not,If i2c stop is not configured,the master between writing and reading, it will be a restart signal,
 *             and after reading and writing, a stop signal is sent,but the stop configure,stays in its original state,
 *             when a nack exception signal is received, during exception handling, a stop signal will be sent.
 * @param[in]  id - to set the slave ID.
 * @param[in]  wr_data - The data to be sent.
 * @param[in]  wr_len -  This length is the total length, including both the length of the slave RAM address and the length of the data to be sent.
 * @param[in]  rd_data - Store the read data
 * @param[in]  rd_len -  The total length of the data read back.
 * @return     0: the master detect nack in the id or data when the master write,stop sending,and return.
 *                or the master detect nack in the id when the master read,stop receiving,and return.
 *                - check whether the id is correct;
 *                - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *                - check whether the slave is abnormal;
 *             1: the master receive the data successfully.
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c_set_error_timeout);
 */
unsigned char i2c_master_write_read(unsigned char id, unsigned char *wr_data, unsigned int wr_len, unsigned char *rd_data, unsigned int rd_len);



/**
 * @brief      The function of this API is just to write data to the i2c tx_fifo by DMA.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received in id or data phase, during exception handling, a stop signal will be sent.
 * @param[in]  id   - to set the slave ID.
 * @param[in]  data - The data to be sent,must be aligned by word (4 bytes).
 * @param[in]  len  - This length is the total length, including both the length of the slave RAM address and the length of the data to be sent,
 *                    the maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return     none.
 * @note       After the DMA transfer is complete, the interface needs to be re-invoked to write the next batch of data.
 */
void i2c_master_write_dma(unsigned char id, unsigned char *data, unsigned int len);


/**
 * @brief      This function serves to read a packet of data from the specified address of slave device by dma.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received in id phase, during exception handling, a stop signal will be sent.
 * @param[in]  id      - to set the slave ID.
 * @param[in]  rx_data - Store the read data,must be aligned by word (4 bytes).
 * @param[in]  len     - The total length of the data read back,the maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return     none
 * @note       After the DMA transfer is complete, the interface needs to be re-invoked to read the next batch of data.
 */
void i2c_master_read_dma(unsigned char id, unsigned char *rx_data, unsigned int len);


/**
 * @brief      Send an amount of data in DMA mode
 * @param[in]  data -  Pointer to data buffer, it must be 4-bytes aligned address
 * @param[in]  len  -  Amount of data to be sent in bytes, range from 1 to 0xFFFFFC
 * @return     none.
 * @note       After the DMA transfer is complete, the interface needs to be re-invoked to write the next batch of data.
 */
void i2c_slave_set_tx_dma( unsigned char *data, unsigned int len);



/**
 * @brief      Receive an amount of data in DMA mode
 * @param[in]  data - Pointer to data buffer, it must be 4-bytes aligned address
 * @param[in]  len  - Length of DMA in bytes, It must be set to 0xFFFFFC.
 * @return     none.
 * @attention  The first four bytes in the buffer of the received data are the length of the received data.
 *             The actual buffer size that the user needs to set needs to be noted on two points:
 *			   -# you need to leave 4bytes of space for the length information.
 *			   -# dma is transmitted in accordance with 4bytes, so the length of the buffer needs to be a multiple of 4. Otherwise, there may be an out-of-bounds problem
 *			   For example, the actual received data length is 5bytes, the minimum value of the actual buffer size that the user needs to set is 12bytes, and the calculation of 12bytes is explained as follows:
 *			   4bytes (length information) + 5bytes (data) + 3bytes (the number of additional bytes to prevent out-of-bounds)
 *			   -# After the DMA transfer is complete, the interface needs to be re-invoked to read the next batch of data.
 *
 */
void i2c_slave_set_rx_dma(unsigned char *data, unsigned int len);


/**
 * @brief     This function serves to receive data .
 * @param[in]  data - the data need read.
 * @param[in]  len - The total length of the data
 * @return    none
 */
void i2c_slave_read(unsigned char* data , unsigned int len );


/**
 * @brief     This function serves to receive i2c data by byte with not DMA method.
 * @param[in]  data - the data need send.
 * @param[in]  len - The total length of the data.
 * @return    none
 */
void i2c_slave_write(unsigned char* data , unsigned int len);


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
 */
void i2c_set_tx_dma_config(dma_chn_e chn);

/**
 * @brief     This function serves to set i2c rx_dma channel and config dma rx default.
 * @param[in] chn: dma channel.
 * @return    none
 */
void i2c_set_rx_dma_config(dma_chn_e chn);


/**
 *@brief     This function serves to enable i2c slave stretch function,conjunction with stretch function of master,
 *           this stretch function is usually used in combination with I2C_SLAVER_WR_IRQ/I2C_STRETCH_STATUS interrupt,
 *           when TX_FIFO of slave terminal is empty or RX_FIFO of slave terminal is full, the interrupt state is up and the clock line is pulled down.
 *@return    none.
 */
static inline void i2c_slave_stretch_en(void){

	reg_i2c_ctrl3 |= FLD_I2C_SLAVE_STRETCH_EN;
}

/**
 *@brief     This function serves to disable i2c slave stretch function.
 *@return    none.
 */
static inline void i2c_slave_stretch_dis(void){

	reg_i2c_ctrl3 &= ~FLD_I2C_SLAVE_STRETCH_EN;
}

/**
 *@brief     This function serves to i2c slave manual stretch function,conjunction with stretch function of master,
 *           When this interface is called, clk will be pulled, it should be noted that this interface can only be called when the master is in the idle state.
 *@return    none.
 */
static inline void i2c_slave_manual_stretch_en(void){
	reg_i2c_status|=FLD_I2C_R_MANUAL_STRETCH;
}

/**
 *@brief     This function serves to clear i2c slave manual stretch function,When the interface is called, clk recovers.
 *@return    none.
 */
static inline void i2c_slave_manual_stretch_clr(void){
	reg_i2c_status |= FLD_I2C_MANUAL_STRETCH_CLR;
}
/**
 *@brief     This function serves to enable i2c master stretch function,
 *           If stretch is enabled on the slave, it needs to be enabled,by default, it is enabled on the interface i2c_master_init.
 *@return    none.
 */
static inline void i2c_master_stretch_en(void){

	reg_i2c_ctrl2 |=FLD_I2C_MASTER_STRETCH_EN;
}

/**
 *@brief     This function serves to disable i2c master stretch function.
 *@return    none.
 */
static inline void i2c_master_stretch_dis(void){

	reg_i2c_ctrl2 &=~(FLD_I2C_MASTER_STRETCH_EN);
}


/**
 * @brief      this function serves to:
 *             -# when the nack detect function is used on the master, need to enable;
 *             -# during the sending and receiving of data, if the master detects nack,turn on the function that sends the stop signal,
 *                otherwise, the stop signal cannot be sent, resulting in the busy state of the master,this interface is enabled by default on the i2c_master_init interface.
 * @return     none.
 */
static inline void i2c_master_detect_nack_en(void)
{
	reg_i2c_ctrl2 |= FLD_I2C_MASTER_NAK_STOP_EN;
}

/**
 * @brief      This function serves to disable i2c master nack stop function.
 * @return     none.
 */
static inline void i2c_master_detect_nack_dis(void)
{
	reg_i2c_ctrl2 &= ~(FLD_I2C_MASTER_NAK_STOP_EN);
}
/**
 * @brief     This function serves to configure the master i2c send and receive byte length,the hardware needs to know what the length is.
 * @param[in] len - the maximum transmission length of i2c is 0xffffff bytes, so dont'n over this length.
 * @return    none
 */
void i2c_master_set_len(unsigned int len);

/**
 *@brief     in slave ,judge whether the master is sending a read cmd or a write cmd.
 *@return    1:if return 1,it means that the slave is to get master read cmd.
 *           0:if return 0,it means that the slave is to get master write cmd.
 */
i2c_slave_wr_e i2c_slave_get_cmd(void);

/**
 * @brief     in master,judge whether master is to read or write
 * @return    1:if return 1,it means that the state of the master is read status.
 *            0:if return 0,it means that the state of the master is not read status.
 */
i2c_master_wr_e i2c_get_master_wr_status(void);

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
void i2c_set_pin(gpio_func_pin_e sda_pin,gpio_func_pin_e scl_pin);


/**
 * @brief      This function serves to enable i2c master function.
 * @return     none.
 */
void i2c_master_init(void);


/**
 * @brief     This function serves to record the api status.
 * @param[in] i2c_error_timeout_code - i2c_api_error_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario,can by g_i2c_error_timeout_code to obtain details about the timeout reason,
 *            for the solution, refer to the i2c_set_error_timeout note.
 */
__attribute__((weak)) void i2c_timeout_handler(unsigned int i2c_error_timeout_code);


/**
 * @brief     This function serves to set the i2c timeout(us).
 * @param[in] timeout_us - the timeout(us).
 * @return    none.
 * @note      the default timeout (g_i2c_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
 *            g_i2c_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
 *            g_i2c_error_timeout_us the minimum time must meet the following conditions:
 *            1. eight i2c data;
 *            2. maximum interrupt processing time;
 *            3. maximum normal stretch time of the slave;(the stretch description: if the slave end cannot process in time, the clk will be stretch,the master will hold,
 *               when the slave is processed, the clk will be released and the master will continue working.)
 *            when timeout exits, solution:
 *            1.reset master,reset slave(i2c_hw_fsm_reset);
 *            2.ensure that the clk/data is high(gpio_get_level);
 */
void i2c_set_error_timeout(unsigned int timeout_us);

/**
 * @brief     This function serves to return the i2c api error code.
 * @return    none.
 */
i2c_api_error_timeout_code_e i2c_get_error_timeout_code(void);

/********************************************************************************************
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 *****|								i2c1_m.h 						                   |*****
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 ********************************************************************************************/
// i2c1_m api error code
typedef enum {
	I2C1_M_API_ERROR_TIMEOUT_NONE             = 0x00,
	I2C1_M_API_ERROR_TIMEOUT_ID			      = 0x01,
	I2C1_M_API_ERROR_TIMEOUT_WRITE_DATA		  = 0x02,
	I2C1_M_API_ERROR_TIMEOUT_READ_DATA        = 0x03,
	I2C1_M_API_ERROR_TIMEOUT_STOP		      = 0x04,
} i2c1_m_api_error_timeout_code_e;

/**
 * i2c1_m error timeout(us),a large value is set by default,can set it by i2c1_m_set_error_timeout().
 */
extern unsigned int g_i2c1_m_error_timeout_us;

/**
 * record i2c1_m error code, can obtain the value through the i2c1_m_get_error_timeout_code() interface.
 */
extern volatile i2c1_m_api_error_timeout_code_e g_i2c1_m_error_timeout_code;

/**
 * @brief      The function of this API is to determine whether the bus is busy.
 * @return     1:Indicates that the bus is busy. 0:Indicates that the bus is free
 * @note       If upper-layer application calls the interface, the interface may be stuck due to an exception on both sides of the communication.
 *             If the upper-layer uses the timeout processing mechanism, see set_i2c1_m_error_timeout_us (time setting requirement).
 */
static inline bool i2c1_m_master_busy(void)
{
    return reg_i2c1_m_mst & FLD_I2C1_M_MST_BUSY;
}

/**
 *@brief     This function serves to enable i2c1_m master stretch function,
 *           If stretch is enabled on the slave, it needs to be enabled,by default, it is enabled on the interface i2c1_m_master_init.
 *@return    none.
 */
static inline void i2c1_m_master_stretch_en(void){

	reg_i2c1_m_sct0 |=FLD_I2C1_M_R_CLK_STRETCH_EN;
}

/**
 *@brief     This function serves to disable i2c1_m master stretch function.
 *@return    none.
 */
static inline void i2c1_m_master_stretch_dis(void){

	reg_i2c1_m_sct0 &=~(FLD_I2C1_M_R_CLK_STRETCH_EN);
}

/**
 * @brief     This function serves to i2c1_m finite state machine reset(the configuration register is still there and does not need to be reconfigured).
 * @return    none.
 */
static inline void i2c1_m_hw_fsm_reset(void){
	reg_rst2 &= (~FLD_RST2_I2C1);
	reg_rst2 |= FLD_RST2_I2C1;
	g_i2c1_m_error_timeout_code=I2C1_M_API_ERROR_TIMEOUT_NONE;
}

/**
 * @brief      The function of this interface is equivalent to that after the user finishes calling the write or read interface, the stop signal is not sent,
 * 			   and then the write or read command is executed again. The driver defaults that every write or read API will send a stop command at the end
 * @param[in]  en - Input parameters.Decide whether to disable the stop function after each write or read interface
 * @return     none
 */
void i2c1_m_master_send_stop(unsigned char en);


/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  sda_pin - the pin port selected as I2C sda pin port.
 * @param[in]  scl_pin - the pin port selected as I2C scl pin port.
 * @return     none
 */
void i2c1_m_set_pin(gpio_func_pin_e sda_pin,gpio_func_pin_e scl_pin);

/**
 * @brief      This function serves to enable i2c1_m master function.
 * @return     none.
 */
void i2c1_m_master_init(void);


/**
 * @brief      This function serves to set the i2c1_m clock frequency.The i2c1_m clock is consistent with the pclk.
 * @param[in]  clock - the division factor of i2c1_m clock,
 *             i2c1_m frequency = pclk / (4*clock).
 * @return     none
 */
void i2c1_m_set_master_clk(unsigned char clock);


/**
 *  @brief      This function serves to write a packet of data to the specified address of slave device
 *  @param[in]  id      - to set the slave ID.
 *  @param[in]  data_buf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  data_len - the length of data master write to slave.
 *  @return     0:During sending id or data, the master receives the nack returned by the slave, and stops sending.
 *                - check whether the id is correct;
 *                - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *                - check whether the slave is abnormal;
 *              1:Master sent data successfully.
 *              DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c1_m_set_error_timeout);
 */
unsigned char i2c1_m_master_write(unsigned char id, unsigned char * data_buf, unsigned int data_len);



/**
 * @brief      This function serves to read a packet of data.
 * @param[in]  id      - to set the slave ID.
 * @param[in]  data_buf - the first address of SRAM buffer master store data in.
 * @param[in]  data_len - the length of data master read from slave.
 * @return     0:During sending id, the master receives the nack returned by the slave, and stops sending.
 *             - check whether the id is correct;
 *             - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *             - check whether the slave is abnormal;
 *             1:Master receive data successfully.
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c1_m_set_error_timeout);
 */
unsigned char i2c1_m_master_read(unsigned char id, unsigned char * data_buf, unsigned int data_len);



/**
 * @brief      This function serves to read a packet of data from the specified address of slave device.
 *             the master between writing and reading, it will be a restart signal,and after reading and writing, a stop signal is sent,
 *             but the stop configure,stays in its original state,when a nack exception signal is received, during exception handling, a stop signal will be sent.
 * @param[in]  id      - to set the slave ID.
 * @param[in]  wr_data - specifies the SRAM address of the slave to send data.
 * @param[in]  wr_len - the length of register. enum 0 or 1 or 2 or 3 based on the demand of i2c slave.
 * @param[in]  rd_data - the first address of SRAM buffer master store data in.
 * @param[in]  rd_len - the length of data master read from slave.
 * @return     0:During sending id+address, the master receives the nack returned by the slave, and stops sending.
 *             - check whether the id is correct;
 *             - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *             - check whether the slave is abnormal;
 *             1:Master receive data successfully.
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c1_m_set_error_timeout);
 */
unsigned char i2c1_m_master_write_read(unsigned char id, unsigned char *wr_data, unsigned int wr_len, unsigned char *rd_data, unsigned int rd_len);

/**
 * @brief     This function serves to record the api status.
 * @param[in] i2c1_m_error_timeout_code - i2c1_m_api_error_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario,can by g_i2c1_m_error_timeout_code to obtain details about the timeout reason,
 *            for the solution, refer to the i2c1_m_set_error_timeout note.
 */
__attribute__((weak)) void i2c1_m_timeout_handler(unsigned int i2c1_m_error_timeout_code);

/**
 * @brief     This function serves to set the i2c1_m timeout(us).
 * @param[in] timeout_us - the timeout(us).
 * @return    none.
 * @note      i2c1_m_master_init add i2c1_m_master_stretch_en(), the master will be stuck if the slave pulls the Master abnormally,
 *            the default timeout (g_i2c1_m_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
 *            g_i2c1_m_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
 *		      g_i2c1_m_error_timeout_us the minimum time must meet the following conditions:
 *		      1. two i2c data;
 *		      2. maximum interrupt processing time;
 *		      3. maximum normal stretch time of the slave;(the stretch description: if the slave end cannot process in time, the clk will be stretch,the master will hold,
 *		         when the slave is processed, the clk will be released and the master will continue working.)
 *		      when timeout exits, solution:
 *		      1.reset master,reset slave(i2c1_m_hw_fsm_reset);
 *		      2.ensure that the clk/data is high(gpio_get_level);
 */
void i2c1_m_set_error_timeout(unsigned int timeout_us);

/**
 * @brief     This function serves to return the i2c1_m api error code.
 * @return    none.
 */
i2c1_m_api_error_timeout_code_e i2c1_m_get_error_timeout_code(void);

#endif










