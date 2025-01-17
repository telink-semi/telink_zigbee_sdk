/********************************************************************************************************
 * @file    i2c_reg.h
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
#ifndef I2C_REG_H_
#define I2C_REG_H_
#include "soc.h"
/*******************************      i2c registers: 0x140280      ******************************/


#define REG_I2C_BASE           0x140280


#define reg_i2c_data_buf0_addr 0x80140288


/**
 *  i2c clock = pclk/(4*DivClock)
 */
#define reg_i2c_sp REG_ADDR8(REG_I2C_BASE)


/**
 * this register to configure I2C ID.
 * BIT[0] to set  write or read bit:0=0 for write, bit:0=1 for read.
 * BIT[7:1] for ID.
 */
#define reg_i2c_id REG_ADDR8(REG_I2C_BASE + 0x01)

enum
{
    FLD_I2C_WRITE_READ_BIT = BIT(0),
    FLD_I2C_ID             = BIT_RNG(1, 7),
};

/**
 * this register is to configure i2c master
 * BIT[0] means i2c bus whether busy.
 * BIT[1] means that a start signal is coming, it is 1, and an end signal is coming it will be 0.
 * BIT[2] master received status: 1 for nak; 0 for ack.
 * BIT_RNG[3:5] Indicate what state the i2c is in when it acts as master, BIT_RNG[3:5] default value is 0x06 it means master's state is IDLE.
 * BIT_RNG[6:7] Indicate what state the i2c is in when it acts as slave.
 */
#define reg_i2c_mst REG_ADDR8(REG_I2C_BASE + 0x02)

enum
{
    FLD_I2C_MST_BUSY = BIT(0),
    FLD_I2C_SCS_N    = BIT(1),
    FLD_I2C_ACK_IN   = BIT(2),
    FLD_I2C_MST_P    = BIT_RNG(3, 5),
    FLD_I2C_SS       = BIT_RNG(6, 7),
};

/**
 * This shows the status control register of i2c
 * BIT[0] i2c master enable.
 * BIT[1] master clk stretch enable.
 * BIT[2] rx interrupt enable.RX is related to rx_irq_trig_lev function,fifo_data_cnt> = rx_irq_trig_lev generates an interrupt.
 * BIT[3] tx interrupt enable.Related to tx_irq_trig_lev function,fifo_data_cnt <= tx_irq_trig_lev, generate interrupt.
 * BIT[4] tx_done.An interrupt is generated when one frame of data is sent.
 * BIT[5] rx_done.An interrupt is generated when one frame of data is received.
 * BIT[6] If the bit is set to 1, when the master reads, the hardware will automatically return ack / nak, no software processing is required.
 *        It is a new feature of eagle.Previously, software processed it through FLD_I2C_LS_ACK. Setting this bit to 0 will ack, and setting it to 1 will NAK.
 * BIT[7] means,before master ack, delay sda data bus.
 */
#define reg_i2c_sct0 REG_ADDR8(REG_I2C_BASE + 0x03)

typedef enum
{
    FLD_I2C_MASTER           = BIT(0),
    FLD_I2C_R_CLK_STRETCH_EN = BIT(1),
    FLD_I2C_RX_BUF_MASK      = BIT(2),
    FLD_I2C_TX_BUF_MASK      = BIT(3),
    FLD_I2C_TX_DONE_MASK     = BIT(4),
    FLD_I2C_RX_DONE_MASK     = BIT(5),
    FLD_I2C_RNCK_EN          = BIT(6),
    FLD_I2C_MANUAL_SDA_DELAY = BIT(7),
} i2c_mask_irq_type_e;

/**
 * This shows the status control register of i2c
 * BIT[0] launch i2c ID cycle.
 * BIT[1] launch i2c address cycle.
 * BIT[2] launch data write cycle.
 * BIT[3] launch data read cycle.
 * BIT[4] launch start cycle.
 * BIT[5] launch stop cycle.
 * BIT[6] enable if BIT[6]=1,Its role is to convert the SDA from output to input, so that the master can accept the data sent by the slave when it reads.
 * BIT[7] enable ACK in read command.When the master is reading, it needs to return to ack or nak.
 *        If it is in manual mode,when the master is reading, you need to configure this bit to determine whether it will ack.
 *        BIT[7] = 0,return ack. BIT[7] = 1,return nak.
 */
#define reg_i2c_sct1 REG_ADDR8(REG_I2C_BASE + 0x04)

enum
{
    FLD_I2C_LS_ID    = BIT(0),
    FLD_I2C_LS_ADDR  = BIT(1),
    FLD_I2C_LS_DATAW = BIT(2),
    FLD_I2C_LS_DATAR = BIT(3),
    FLD_I2C_LS_START = BIT(4),
    FLD_I2C_LS_STOP  = BIT(5),
    FLD_I2C_LS_ID_R  = BIT(6),
    FLD_I2C_LS_ACK   = BIT(7),
};

/**
 * This is the register that configures the i2c trigger interrupt
 * BIT_RNG[0,3] to configure the interrupt trigger level of rx_status, for example BIT_RNG[0:3]=0x04,when rx 4bytes,will trigger interrupt.
 * BIT_RNG[4,7] to configure the interrupt trigger level of tx_status, for example BIT_RNG[0:3]=0x04,when tx 4bytes,will trigger interrupt.
 */
#define reg_i2c_trig REG_ADDR8(REG_I2C_BASE + 0x05)

enum
{
    FLD_I2C_RX_IRQ_TRIG_LEV = BIT_RNG(0, 3),
    FLD_I2C_TX_IRQ_TRIG_LEV = BIT_RNG(4, 7),
};

//As a master, you need to configure this length for both sending and receiving, and the hardware needs to know what the length is.
#define reg_i2c_len REG_ADDR8(REG_I2C_BASE + 0x06)


/**
 * This register is to configure the slave stretch function.
 * BIT[0] slave auto stretch clk enable,open this function, use slave to receive data,when data buffer is full, scl bus will be low to stop receive data.
 * BIT[1] slave manual stretch clk: suspend transmission by pulling SCL down to low level,and continue transmission after SCL is pulsed to high level.
 * BIT[2] clear the slave manual stretch.
 * BIT[6] in high speed mode,when open slave auto stretch clk function,Suddenly data came over, to meet the requirements of time setting.
 * BIT[7] in fast speed mode,when open slave auto stretch clk function,Suddenly data came over, to meet the requirements of time setting.
 */
#define reg_i2c_slave_stretch_en REG_ADDR8(REG_I2C_BASE + 0x07)

enum
{
    FLD_I2C_R_CLK_STRETCH_SEN  = BIT(0),
    FLD_I2C_R_MANUAL_STRETCH   = BIT(1),
    FLD_I2C_MANUAL_STRETCH_CLR = BIT(2),
    FLD_I2C_R_HS_MODE          = BIT(6),
    FLD_I2C_R_FAST_MODE        = BIT(7),
};

#define reg_i2c_data_buf(i) REG_ADDR8((REG_I2C_BASE + 0x08 + (i)))
/**
 * This register represents the data buffer of i2c.
 * BIT_RNG[0,7]  Buffer that stores one byte of data
 */
#define reg_i2c_data_buf0 REG_ADDR8(REG_I2C_BASE + 0x08)

enum
{
    FLD_I2C_BUF0 = BIT_RNG(0, 7),
};

/**
 * This register represents the data buffer of i2c.
 * BIT_RNG[0,7]  Buffer that stores one byte of data
 */
#define reg_i2c_data_buf1 REG_ADDR8(REG_I2C_BASE + 0x09)

enum
{
    FLD_I2C_BUF1 = BIT_RNG(0, 7),
};

/**
 * This register represents the data buffer of i2c.
 * BIT_RNG[0,7]  Buffer that stores one byte of data
 */
#define reg_i2c_data_buf2 REG_ADDR8(REG_I2C_BASE + 0x0a)

enum
{
    FLD_I2C_BUF2 = BIT_RNG(0, 7),
};

/**
 * This register represents the data buffer of i2c.
 * BIT_RNG[0,7]  Buffer that stores one byte of data
 */
#define reg_i2c_data_buf3 REG_ADDR8(REG_I2C_BASE + 0x0b)

enum
{
    FLD_I2C_BUF3 = BIT_RNG(0, 7),
};

/**
 * This register is used to configure the number of bytes in the i2c buffer
 * BIT_RNG[0,3]  rx_bufcnt is equivalent to a pointer to fifo, one in data plus one, one out data minus one.
 * BIT_RNG[4,7]  tx_bufcnt is equivalent to a pointer to fifo, one in data plus one, one out data minus one.
 */
#define reg_i2c_buf_cnt REG_ADDR8(REG_I2C_BASE + 0x0c)

enum
{
    FLD_I2C_RX_BUFCNT = BIT_RNG(0, 3),
    FLD_I2C_TX_BUFCNT = BIT_RNG(4, 7),
};

/**
 * This register used to configure the status of i2c.
 * BIT_RNG[0,2] rbcnt is the accumulation of this action read, fifo clear will clear.
 * BIT[3]       Indicates whether i2c is in an interrupted state.
 * BIT_RNG[4,6] if configure BIT[6]=1,will manual clean rx_fifo. BIT[5,4] Indicates the number of bytes of tx_buffer.
 * BIT[7]       if configure BIT[7]=1,will manual clean tx_fifo.
 */
#define reg_i2c_status REG_ADDR8(REG_I2C_BASE + 0x0d)

enum
{
    FLD_I2C_RBCNT  = BIT_RNG(0, 2),
    FLD_I2C_IRQ_O  = BIT(3),
    FLD_I2C_WBCNT  = BIT_RNG(4, 6),
    FLD_I2C_RX_CLR = BIT(6),
    FLD_I2C_TX_CLR = BIT(7),
};

/**
 * This register is used to indicate the interrupt status of i2c.
 * BIT[0] means Generate an interrupt after the transmission is completed.
 * BIT[1] If there is data in tx_buffer, an interrupt will be generated.
 * BIT[2] means Generate an interrupt after the receive is completed.
 * BIT[3] If there is data in rx_buffer, an interrupt will be generated.
 * BIT[4] tx_done manual clear.
 */
#define reg_i2c_irq_status REG_ADDR8(REG_I2C_BASE + 0x0e)

enum
{
    FLD_I2C_TXDONE     = BIT(0),
    FLD_I2C_TX_BUF_IRQ = BIT(1),
    FLD_I2C_RXDONE     = BIT(2),
    FLD_I2C_RX_BUF_IRQ = BIT(3),
    FLD_I2C_TX_EN      = BIT(4),
};

//reg_i2c_rx_fifo_len is the number actually entered in the hardware fifo, it is an accumulated value, and fifo clear will clear.
#define reg_i2c_rx_fifo_len REG_ADDR8(REG_I2C_BASE + 0x0f)

enum
{
    FLD_I2C_RX_FIFO_LEN = BIT_RNG(0, 7),
};


#endif
