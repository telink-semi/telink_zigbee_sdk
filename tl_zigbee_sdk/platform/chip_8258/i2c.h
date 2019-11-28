/********************************************************************************************************
 * @file     i2c.h
 *
 * @brief    I2C configuration interface for tlsr8258
 *
 * @author   jian.zhang@telink-semi.com
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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
 *
 *******************************************************************************************************/
#pragma once
#ifndef I2C_H
#define I2C_H
#include "gpio.h"


// SDA   SCL
//  C0    C1
//  C2    C3
//  A3    A4
//  B6    D7
typedef enum {
	I2C_GPIO_GROUP_A3A4,
	I2C_GPIO_GROUP_B6D7,
	I2C_GPIO_GROUP_C0C1,
	I2C_GPIO_GROUP_C2C3,
}I2C_GPIO_GroupTypeDef;







/**
 *  @brief when device as I2C slave
 *   select i2c slave mode:1.DMA;2.MAPPING
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


//i2c slave : this id is slave device id, slave will initialize it in   i2c_slave_init func, no need change afterward.
//i2c master: this id is for finding slave device in read/write operation, so i2c master could change it to access different devices
static inline void i2c_set_id(unsigned char SlaveID)
{
    reg_i2c_id	  = SlaveID; //slave address
}





static inline void i2c_slave_mapping_mode_data_buffer_config(unsigned char * pMapBuf)
{
	 reg_slave_map_addrl  = (unsigned char)(((unsigned int)pMapBuf & 0xff)); //
	 reg_slave_map_addrm = (unsigned char)(((unsigned int)pMapBuf>>8)&0xff);
	 reg_slave_map_addrh = 0x04;
}

/**
 * @brief      This function set the id of slave device and the speed of I2C interface
 *             note: the param ID contain the bit of writting or reading.
 *             eg:the parameter 0x5C. the reading will be 0x5D and writting 0x5C.
 * @param[in]  SlaveID - the id of slave device.it contains write or read bit,the lsb is write or read bit.
 *                       ID|0x01 indicate read. ID&0xfe indicate write.
 * @param[in]  DivClock - the division factor of I2C clock,
 *             I2C clock = System clock / (4*DivClock);if the datasheet you look at is 2*,pls modify it.
 * @return     none
 */
void i2c_master_init(unsigned char SlaveID, unsigned char DivClock);

/**
 *	@brief      the function config the ID of slave and mode of slave.
 *	@param[in]  device_ID - it contains write or read bit,the lsb is write or read bit.
 *			    ID|0x01 indicate read. ID&0xfe indicate write.
 *	@param[in]  mode - set slave mode. slave has two modes, one is DMA mode, the other is MAPPING mode.
 *	@param[in]  pMapBuf - if slave mode is MAPPING, set the first address of buffer master write or read slave.
 *	@return     none
 */
void i2c_slave_init(unsigned char device_ID,I2C_SlaveMode mode,unsigned char * pMapBuf);



/**
 * @brief      This function writes one byte to the slave device at the specified address
 * @param[in]  Addr - i2c slave address where the one byte data will be written
 * @param[in]  AddrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @param[in]  Data - the one byte data will be written via I2C interface
 * @return     none
 */
void i2c_write_byte(unsigned int Addr, unsigned int AddrLen, unsigned char Data);

/**
 * @brief      This function reads one byte from the slave device at the specified address
 * @param[in]  Addr - i2c slave address where the one byte data will be read
 * @param[in]  AddrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @return     the one byte data read from the slave device via I2C interface
 */
unsigned char i2c_read_byte(unsigned int Addr, unsigned int AddrLen);



/**
 *  @brief      write continous data to slave
 *  @param[in]  Addr - the register that master write data to slave in. support one byte and two bytes. i.e param2 AddrLen may be 1 or 2.
 *  @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3. based on the spec of i2c slave.
 *  @param[in]  dataBuf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  dataLen - the length of data master write to slave.
 *  @return     none
 */
void  i2c_write_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);
/**
 * @brief      read continous data from slave
 * @param[in]  Addr - the register master read data from slave in. support one byte and two bytes.
 * @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3 based on the spec of i2c slave.
 * @param[in]  dataBuf - the first address of SRAM buffer master store data in.
 * @param[in]  dataLen - the length of data master read from slave.
 * @return     none.
 */
void   i2c_read_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);

/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  PinGrp - the pin port selected as I2C interface pin port.
 * @return     none
 */
void i2c_gpio_set(I2C_GPIO_GroupTypeDef i2c_pin_group);

#endif





