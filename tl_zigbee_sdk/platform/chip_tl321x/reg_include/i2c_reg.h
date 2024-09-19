/********************************************************************************************************
 * @file    i2c_reg.h
 *
 * @brief   This is the header file for TL321X
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
#ifndef I2C_REG_H_
#define I2C_REG_H_
#include "soc.h"
/*******************************      i2c registers: 0x140280      ******************************/


#define     REG_I2C_BASE            0x140280


#define reg_i2c_data_buf0_addr      0x80140288


/**
 *  i2c clock = pclk/(4*DivClock)
 */
#define reg_i2c_sp                  REG_ADDR8(REG_I2C_BASE)


/**
 * this register to configure I2C ID.
 * BIT[0] to set  write or read bit:0=0 for write, bit:0=1 for read.
 * BIT[7:1] for ID.
 */
#define reg_i2c_id                  REG_ADDR8(REG_I2C_BASE+0x01)
enum{
    FLD_I2C_WRITE_READ_BIT  =  BIT(0),
    FLD_I2C_ID              =  BIT_RNG(1,7),
};


/**
 * this register is to configure i2c master
 * BIT[0] means i2c bus whether busy.
 * BIT[1] means that a start signal is coming, it is 1, and an end signal is coming it will be 0.
 * BIT[2] master received status: 1 for nak; 0 for ack.
 * BIT_RNG[3:5] Indicate what state the i2c is in when it acts as master, BIT_RNG[3:5] default value is 0x06 it means master's state is IDLE.
 * BIT_RNG[6:7] Indicate what state the i2c is in when it acts as slave.
 */
#define reg_i2c_mst                 REG_ADDR8(REG_I2C_BASE+0x02)
enum{
    FLD_I2C_MST_BUSY        =  BIT(0),
    FLD_I2C_SCS_N           =  BIT(1),
    FLD_I2C_ACK_IN          =  BIT(2),
    FLD_I2C_MST_P           =  BIT_RNG(3,5),
    FLD_I2C_SS              =  BIT_RNG(6,7),
};


/**
 * This shows the interrupt mask register of i2c
 * BIT[0] mask the slave parsing master cmd interrupt,when received the master read or write cmd, will generate interrupt.
 * BIT[1] when slave is wrong, reply nack to master,master detect to the nack, will generate interrupt.
 * BIT[2] rx interrupt enable.RX is related to rx_irq_trig_lev function.
 *        fifo_data_cnt> = rx_irq_trig_lev generates an interrupt.
 * BIT[3] tx interrupt enable.Related to tx_irq_trig_lev function.
 *        fifo_data_cnt <= tx_irq_trig_lev, generate interrupt.
 * BIT[4] rx_done.An interrupt is generated when one frame of data is received.
 * BIT[5] tx_done.An interrupt is generated when one frame of data is sent.
 * BIT[6] rx_end.An interrupt is generated when i2c agreement is received.
 * BIT[7] tx_end.An interrupt is generated when i2c agreement is sent.
 */
#define reg_i2c_mask0               REG_ADDR8(REG_I2C_BASE+0x03)

/*
 *  This register is used to indicate the interrupt status of i2c.
 */
#define reg_i2c_status0             REG_ADDR8(REG_I2C_BASE+0x0f)
enum{
    FLD_I2C_SLAVE_WR_IRQ      =  BIT(0),
    FLD_I2C_MASTER_NAK_IRQ     =  BIT(1),
    FLD_I2C_RX_BUF_IRQ         =  BIT(2),
    FLD_I2C_TX_BUF_IRQ         =  BIT(3),
    FLD_I2C_RX_DONE_IRQ        =  BIT(4),
    FLD_I2C_TX_DONE_IRQ        =  BIT(5),
    FLD_I2C_RX_END_IRQ         =  BIT(6),
    FLD_I2C_TX_END_IRQ         =  BIT(7),
};

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
#define reg_i2c_sct1                REG_ADDR8(REG_I2C_BASE+0x04)
enum{
    FLD_I2C_LS_ID                   =  BIT(0),
    FLD_I2C_LS_ADDR                 =  BIT(1),
    FLD_I2C_LS_DATAW                =  BIT(2),
    FLD_I2C_LS_DATAR                =  BIT(3),
    FLD_I2C_LS_START                =  BIT(4),
    FLD_I2C_LS_STOP                 =  BIT(5),
    FLD_I2C_LS_ID_R                 =  BIT(6),
    FLD_I2C_LS_ACK                  =  BIT(7),
};



/**
 * This is the register that configures the i2c trigger interrupt
 * BIT_RNG[0,3] to configure the interrupt trigger level of rx_status, for example BIT_RNG[0:3]=0x04,when rx 4bytes,will trigger interrupt.
 * BIT_RNG[4,7] to configure the interrupt trigger level of tx_status, for example BIT_RNG[0:3]=0x04,when tx 4bytes,will trigger interrupt.
 */
#define reg_i2c_trig                REG_ADDR8(REG_I2C_BASE+0x05)
enum{
    FLD_I2C_RX_IRQ_TRIG_LEV         =  BIT_RNG(0,3),
    FLD_I2C_TX_IRQ_TRIG_LEV         =  BIT_RNG(4,7),
};



/*
 * this register is to configure i2c master
 * BIT[0]: i2c master enable.
 * BIT[1]: open master stretch function.
 * BIT[2]: can stop at any time regardless of tx.
 * BIT[3]: can stop at any time regardless of rx.
 * BIT[4]: open master nack function.
 * BIT[5]: 0:id stretch,only when in the id phase,tx_fifo is empty, FLD_I2C_SS_SCL_IRQ set 1,
 *         1:id or data stretch,when in the id or data phase, the slave tx_fifo is empty,FLD_I2C_SS_SCL_IRQ set 1.
 * BIT[6]: mask the slave stretch interrupt,when tx_fifo is empty or rx_fifo is full,will generate interrupt.
 */

#define reg_i2c_ctrl2               REG_ADDR8(REG_I2C_BASE+0x06)
typedef enum{
    FLD_I2C_MASTER                   =  BIT(0),
    FLD_I2C_MASTER_STRETCH_EN         =  BIT(1),
    FLD_MANUAL_TX_STOP_HIT          =   BIT(2),
    FLD_MANUAL_RX_STOP_HIT          =   BIT(3),
    FLD_I2C_MASTER_NAK_STOP_EN       =  BIT(4),
    FLD_I2C_TX_STRETCH_SEL           =  BIT(5),
    FLD_I2C_MASK_STRETCH              =  BIT(6),
}i2c_master_e;


/*
 * BIT[0] to enable slave stretch function.
 * BIT[2] to master,the last byte data read is automatically returned to nack.
 * BIT[3] there is some delay before going back to ACK in the (id,address,dataw) phase
 * BIT[4] If enable rx_done interrupt, it needs to be set to 1 under no_dma,
 *        but not under dma because there is a mechanism for setting it to 1 under DMA.
 * BIT[5] auto_clear function.
 */
#define reg_i2c_ctrl3            REG_ADDR8(REG_I2C_BASE+0x07)
enum{
    FLD_I2C_SLAVE_STRETCH_EN     = BIT(0),
    FLD_I2C_RNCK_EN              = BIT(2),
    FLD_MANUAL_SDA_DELAY         = BIT(3),
    FLD_NDMA_RXDONE_EN           = BIT(4),
    FLD_AUTO_RXCLR_EN            = BIT(5),
};

#define reg_i2c_data_buf(i)             REG_ADDR8(( REG_I2C_BASE+0x08 +(i) ))
/**
 * This register represents the data buffer of i2c.
 * BIT_RNG[0,7]  Buffer that stores one byte of data
 */
#define reg_i2c_data_buf0           REG_ADDR8(REG_I2C_BASE+0x08)
enum{
    FLD_I2C_BUF0                    = BIT_RNG(0,7),
};


/**
 * This register represents the data buffer of i2c.
 * BIT_RNG[0,7]  Buffer that stores one byte of data
 */
#define reg_i2c_data_buf1           REG_ADDR8(REG_I2C_BASE+0x09)
enum{
    FLD_I2C_BUF1                    = BIT_RNG(0,7),
};


/**
 * This register represents the data buffer of i2c.
 * BIT_RNG[0,7]  Buffer that stores one byte of data
 */
#define reg_i2c_data_buf2           REG_ADDR8(REG_I2C_BASE+0x0a)
enum{
    FLD_I2C_BUF2                    = BIT_RNG(0,7),
};


/**
 * This register represents the data buffer of i2c.
 * BIT_RNG[0,7]  Buffer that stores one byte of data
 */
#define reg_i2c_data_buf3           REG_ADDR8(REG_I2C_BASE+0x0b)
enum{
    FLD_I2C_BUF3                    = BIT_RNG(0,7),
};


/**
 * This register is used to configure the number of bytes in the i2c buffer
 * BIT_RNG[0,3]  rx_bufcnt is equivalent to a pointer to fifo, one in data plus one, one out data minus one.
 * BIT_RNG[4,7]  tx_bufcnt is equivalent to a pointer to fifo, one in data plus one, one out data minus one.
 */
#define reg_i2c_buf_cnt             REG_ADDR8(REG_I2C_BASE+0x0c)
enum{
    FLD_I2C_RX_BUFCNT               = BIT_RNG(0,3),
    FLD_I2C_TX_BUFCNT               = BIT_RNG(4,7),
};


/**
 * This register used to configure the status of i2c.
 * BIT(0): W1C,clear the slave manual stretch.
 * BIT(1): W1C,slave manual stretch clk.
 * BIT(2): W1C,clear FLD_I2C_ACK_IN.
 * BIT_RNG[0,2] rbcnt is the accumulation of this action read, fifo clear will clear.
 * BIT[3]       Indicates whether i2c is in an interrupted state.
 * BIT_RNG[4,6] wbcnt is the accumulation of this action write.
 * BIT[7]       the rx_done interrupt,consistent with eagle's rx_done functionality
 */
#define reg_i2c_status              REG_ADDR8(REG_I2C_BASE+0x0d)
enum{
    FLD_I2C_MANUAL_STRETCH_CLR      = BIT(0),
    FLD_I2C_R_MANUAL_STRETCH        = BIT(1),
    FLD_I2C_MANUAL_CLR_MASTER_ACK    = BIT(2),
    FLD_I2C_RBCNT                   = BIT_RNG(0,2),
    FLD_I2C_IRQ_O                   = BIT(3),
    FLD_I2C_WBCNT                   = BIT_RNG(4,6),
    FLD_I2C_RX_DONE                 = BIT(7),
};



/*
 * this register is use to get slave relevant status
 * BIT[0] Judge whether slave is to read or write.
 * BIT[1] slave stretch indicate.
 * BIT[2] judge slave tx_fifo is empty.
 * BIT[3] judge slave rx_fifo is full.
 * BIT[3] the stretch interrupt status.
 */
#define i2c_slave_status1               REG_ADDR8(REG_I2C_BASE+0x0e)
enum{
    FLD_I2C_SLAVE_READ                     = BIT(0),
    FLD_I2C_SLAVE_STRETCH_INDICATE         = BIT(1),
    FLD_I2C_SLAVE_TX_EMPTY                 = BIT(2),
    FLD_I2C_SLAVE_RX_FULL                   = BIT(3),
    FLD_I2C_SS_SCL_IRQ                      = BIT(6),

};




//As a master, you need to configure this length for both sending and receiving, and the hardware needs to know what the length is.
#define reg_i2c_len(i)                  REG_ADDR8(REG_I2C_BASE+0x10+(i))


/**
 *  This register is used to indicate the status of i2c master.
 *  BIT[0] When the master is in the id phase, the flag is 1 until the next start.
 *  BIT[1] When the master is in the address phase, the flag is 1 until the next start.
 *  BIT[2] When the master is in the dataw phase, the flag is 1 until the next start.
 *  BIT[3] When the master is in the datar phase, the flag is 1 until the next start.
 * */
#define reg_i2c_mst_status              REG_ADDR8(REG_I2C_BASE+0x13)
enum{
    FLD_ID_BUSY    =  BIT(0),
    FLD_ADDR_BUSY  =  BIT(1),
    FLD_DATAW_BUSY = BIT(2),
    FLD_DATAR_BUSY = BIT(3),

};

/*******************************      i2c1_m registers: 0x140480     ******************************/


#define     REG_I2C1_M_BASE         0x140480

#define reg_i2c1_m_sp               REG_ADDR8(REG_I2C1_M_BASE+0x00)
#define reg_i2c1_m_id               REG_ADDR8(REG_I2C1_M_BASE+0x01)
enum{
    FLD_I2C1_M_WRITE_READ_BIT  =  BIT(0),
    FLD_I2C1_M_ID              =  BIT_RNG(1,7),
};

#define reg_i2c1_m_mst          REG_ADDR8(REG_I2C1_M_BASE+0x02)
enum{
    FLD_I2C1_M_MST_BUSY             =   BIT(0),
    FLD_I2C1_M_MST_SCS_N            =   BIT(1),
    FLD_I2C1_M_MAT_ACK_IN           =   BIT(2),
};

#define reg_i2c1_m_sct0         REG_ADDR8(REG_I2C1_M_BASE+0x03)
enum{
    FLD_I2C1_M_MASTER                   =   BIT(1),      // 1: master, 0: slave
    FLD_I2C1_M_R_CLK_STRETCH_EN         =   BIT(3),
};

#define reg_i2c1_m_ad           REG_ADDR8(REG_I2C1_M_BASE+0x04)
#define reg_i2c1_m_dw           REG_ADDR8(REG_I2C1_M_BASE+0x05)
#define reg_i2c1_m_dr           REG_ADDR8(REG_I2C1_M_BASE+0x06)




#define reg_i2c1_m_ctrl         REG_ADDR8(REG_I2C1_M_BASE+0x07)

enum{
    FLD_I2C1_M_LS_ID                   =  BIT(0),
    FLD_I2C1_M_LS_ADDR                 =  BIT(1),
    FLD_I2C1_M_LS_DATAW                =  BIT(2),
    FLD_I2C1_M_LS_DATAR                =  BIT(3),
    FLD_I2C1_M_LS_START                =  BIT(4),
    FLD_I2C1_M_LS_STOP                 =  BIT(5),
    FLD_I2C1_M_LS_ID_R                 =  BIT(6),
    FLD_I2C1_M_LS_ACK                  =  BIT(7),
};

#endif










