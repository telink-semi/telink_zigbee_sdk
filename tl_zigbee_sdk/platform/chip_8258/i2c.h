/********************************************************************************************************
 * @file	i2c.h
 *
 * @brief	This is the header file for B85
 *
 * @author	Driver & Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef I2C_H
#define I2C_H

#include "register.h"


/**
 *  @brief  select pin as SDA and SCL of i2c
 */
typedef enum {
	I2C_GPIO_GROUP_A3A4,
	I2C_GPIO_GROUP_B6D7,
	I2C_GPIO_GROUP_C0C1,
	I2C_GPIO_GROUP_C2C3,
}I2C_GPIO_GroupTypeDef;


/**
 *  @brief  select i2c slave mode: DMA and MAPPING
 *
 *   dma mode:
 *   		   all sram(64k area form 0x40000~0x4ffff, address 3 byte) could be used for reading or writing data buffer,
 *             i2c master just need send sram address(3 byte) on i2c bus in i2c reading or writing, reading/writing
 *             sram address could be anywhere and could be different.
 *			   i2c master timing is:
 *              	1.master write:  start + device_id(W) + address2+ address1 + address0 + data(1,2...n byte)  +  stop
 *                  2.master read:   start + device_id(W) + address2+ address1 + address0 + start + device_id(R) + data(1,2...n byte) + stop
 *
 *   mapping mode:
 *  			i2c slave data buffer sram could be configured by function: void i2c_slave_mapping_mode_data_buffer_config(unsigned char * pMapBuf),
 *              and it is also configured in function: void i2c_slave_init(unsigned char device_ID,I2C_SlaveMode mode,unsigned char * pMapBuf)
 *   			reading/writing data buffer configured by pMapBuf addressed in core_e1/e2/e3,
 *   		    for i2c master: writing data buffer is pMapBuf,and reading data buffer is (pMapBuf+64) (this offset 64 is managed by MCU hardware, user can not change it)
 *
 *              i2c master no need send any address when reading or writing data, which means i2c master timing is:
 *              	1.master write:  start + device_id(W) + data(1,2...n byte)  +  stop
 *                  2.master read:   start + device_id(W) + start + device_id(R) + data(1,2...n byte) + stop
 *
 *
 */
typedef enum {
	I2C_SLAVE_DMA = 0,
	I2C_SLAVE_MAP,
}I2C_SlaveMode;


/**
 * @brief This function reset I2C module.
 * @param[in] none
 * @return none
 */
static inline void reset_i2c_moudle(void)
{
	reg_rst0 |= FLD_RST0_I2C;
	reg_rst0 &= (~FLD_RST0_I2C);
}

/**
 * @brief This function serves to set id of I2C module.
 * @param[in] SlaveID - this id is fixed id for slave device.For master device, this id is set to access different slave devices.
 * @return none
 */
static inline void i2c_set_id(unsigned char SlaveID)
{
    reg_i2c_id = SlaveID; //slave address
}

/**
 * @brief This function servers to config i2c data buffer in slave mode.
 * @param[in] pMapBuf - to store the buffer data.
 * @return none
 */
static inline void i2c_slave_mapping_mode_data_buffer_config(unsigned char * pMapBuf)
{
	 reg_i2c_slave_map_addrl  = (unsigned char)(((unsigned int)pMapBuf & 0xff)); //
	 reg_i2c_slave_map_addrm = (unsigned char)(((unsigned int)pMapBuf>>8)&0xff);
	 reg_i2c_slave_map_addrh = 0x04;
}

/**
 * @brief      This function serves to select a pin port for I2C interface.
 * @param[in]  i2c_pin_group - the pin port selected as I2C interface pin port.
 * @return     none
 */
void i2c_gpio_set(I2C_GPIO_GroupTypeDef i2c_pin_group);

/**
 * @brief      This function set the id of slave device and the speed of I2C interface
 * @param[in]  DivClock - the division factor of I2C clock,
 *             I2C clock = System clock / (4*DivClock);if the datasheet you look at is 2*,pls modify it.
 * @return     none
 */
void i2c_master_init(unsigned char DivClock);

/**
 *  @brief      This function serves to set the ID and mode of slave device.
 *  @param[in]  device_ID - it contains write or read bit,the lsb is write or read bit.
 *              ID|0x01 indicate read. ID&0xfe indicate write.
 *  @param[in]  mode - set slave mode. slave has two modes, one is DMA mode, the other is MAPPING mode.
 *  @param[in]  pMapBuf - if slave mode is MAPPING, set the first address of buffer master write or read slave.
 *  @return     none
 */
void i2c_slave_init(unsigned char device_ID,I2C_SlaveMode mode,unsigned char * pMapBuf);


/**
 * @brief      This function serves to write one byte to the slave device at the specified address
 * @param[in]  Addr - i2c slave address where the one byte data will be written
 * @param[in]  AddrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @param[in]  Data - the one byte data will be written via I2C interface
 * @return     none
 */
void i2c_write_byte(unsigned int Addr, unsigned int AddrLen, unsigned char Data);

/**
 * @brief      This function serves to read one byte from the slave device at the specified address
 * @param[in]  Addr - i2c slave address where the one byte data will be read
 * @param[in]  AddrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @return     the one byte data read from the slave device via I2C interface
 */
unsigned char i2c_read_byte(unsigned int Addr, unsigned int AddrLen);

/**
 *  @brief      This function serves to write a packet of data to the specified address of slave device
 *  @param[in]  Addr - the register that master write data to slave in. support one byte and two bytes. i.e param2 AddrLen may be 1 or 2.
 *  @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3. based on the spec of i2c slave.
 *  @param[in]  dataBuf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  dataLen - the length of data master write to slave.
 *  @return     none
 */
void i2c_write_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);

/**
 * @brief      This function serves to read a packet of data from the specified address of slave device
 * @param[in]  Addr - the register master read data from slave in. support one byte and two bytes.
 * @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3 based on the spec of i2c slave.
 * @param[in]  dataBuf - the first address of SRAM buffer master store data in.
 * @param[in]  dataLen - the length of data master read from slave.
 * @return     none.
 */
void i2c_read_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);
#endif
