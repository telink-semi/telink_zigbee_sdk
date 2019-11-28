/********************************************************************************************************
 * @file     drv_i2c.h
 *
 * @brief	 i2c driver interface file
 *
 * @author
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

#include "platform_includes.h"
#include "../common/types.h"
#include "../common/compiler.h"


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
void drv_i2c_master_init(unsigned char SlaveID, unsigned char DivClock);

/**
 *	@brief      the function config the ID of slave and mode of slave.
 *	@param[in]  device_ID - it contains write or read bit,the lsb is write or read bit.
 *			    ID|0x01 indicate read. ID&0xfe indicate write.
 *	@param[in]  mode - set slave mode. slave has two modes, one is DMA mode, the other is MAPPING mode.
 *	@param[in]  pMapBuf - if slave mode is MAPPING, set the first address of buffer master write or read slave.
 *	@return     none
 */
void drv_i2c_slave_init(unsigned char device_ID, I2C_SlaveMode mode,unsigned char * pMapBuf);



/**
 * @brief      This function writes one byte to the slave device at the specified address
 * @param[in]  Addr - i2c slave address where the one byte data will be written
 * @param[in]  AddrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @param[in]  Data - the one byte data will be written via I2C interface
 * @return     none
 */
void drv_i2c_write_byte(unsigned int Addr, unsigned int AddrLen, unsigned char Data);

/**
 * @brief      This function reads one byte from the slave device at the specified address
 * @param[in]  Addr - i2c slave address where the one byte data will be read
 * @param[in]  AddrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @return     the one byte data read from the slave device via I2C interface
 */
unsigned char drv_i2c_read_byte(unsigned int Addr, unsigned int AddrLen);

/**
 *  @brief      write continous data to slave
 *  @param[in]  Addr - the register that master write data to slave in. support one byte and two bytes. i.e param2 AddrLen may be 1 or 2.
 *  @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3. based on the spec of i2c slave.
 *  @param[in]  dataBuf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  dataLen - the length of data master write to slave.
 *  @return     none
 */
void  drv_i2c_write_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);
/**
 * @brief      read continous data from slave
 * @param[in]  Addr - the register master read data from slave in. support one byte and two bytes.
 * @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3 based on the spec of i2c slave.
 * @param[in]  dataBuf - the first address of SRAM buffer master store data in.
 * @param[in]  dataLen - the length of data master read from slave.
 * @return     none.
 */
void  drv_i2c_read_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);

/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  PinGrp - the pin port selected as I2C interface pin port.
 * @return     none
 */
void drv_i2c_gpio_set(I2C_GPIO_GroupTypeDef i2c_pin_group);
