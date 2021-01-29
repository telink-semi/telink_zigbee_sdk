/********************************************************************************************************
 * @file	drv_i2c.h
 *
 * @brief	This is the header file for drv_i2c
 *
 * @author	Zigbee Group
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
#pragma once


#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define I2C_CLOCK_SOURCE			CLOCK_SYS_CLOCK_HZ
#elif defined(MCU_CORE_B91)
	/* PCLK provides clock source for I2C module. */
	#define I2C_CLOCK_SOURCE			(sys_clk.pclk * 1000 * 1000)
#endif


/**
 * @brief      This function set the id of slave device and the speed of I2C interface
 * @param[in]  i2cClock - I2C clock,
 *             I2C clock = System clock / (4*DivClock);if the datasheet you look at is 2*,pls modify it.
 * @return     none
 */
void drv_i2c_master_init(u32 i2cClock);

/**
 *	@brief      the function config the ID of slave and mode of slave.
 *	@param[in]  deviceID - it contains write or read bit,the lsb is write or read bit.
 *			    ID|0x01 indicate read. ID&0xfe indicate write.
 *	@return     none
 */
void drv_i2c_slave_init(u8 deviceID);

/**
 * @brief      This function writes one byte to the slave device at the specified address
 * @param[in]  slaveID - id of the slave device
 * @param[in]  addr - i2c slave address where the one byte data will be written
 * @param[in]  addrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @param[in]  data - the one byte data will be written via I2C interface
 * @return     none
 */
void drv_i2c_write_byte(u8 slaveID, u32 addr, u32 addrLen, u8 data);

/**
 * @brief      This function reads one byte from the slave device at the specified address
 * @param[in]  slaveID - id of the slave device
 * @param[in]  addr - i2c slave address where the one byte data will be read
 * @param[in]  addrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @return     the one byte data read from the slave device via I2C interface
 */
u8 drv_i2c_read_byte(u8 slaveID, u32 addr, u32 addrLen);

/**
 *  @brief      write continuous data to slave
 *  @param[in]  slaveID - id of the slave device
 *  @param[in]  addr - the register that master write data to slave in. support one byte and two bytes. i.e param2 AddrLen may be 1 or 2.
 *  @param[in]  addrLen - the length of register. enum 0 or 1 or 2 or 3. based on the spec of i2c slave.
 *  @param[in]  dataBuf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  dataLen - the length of data master write to slave.
 *  @return     none
 */
void drv_i2c_write_series(u8 slaveID, u32 addr, u32 addrLen, u8 *dataBuf, int dataLen);

/**
 * @brief      read continuous data from slave
 * @param[in]  slaveID - id of the slave device
 * @param[in]  addr - the register master read data from slave in. support one byte and two bytes.
 * @param[in]  addrLen - the length of register. enum 0 or 1 or 2 or 3 based on the spec of i2c slave.
 * @param[out] dataBuf - the first address of SRAM buffer master store data in.
 * @param[in]  dataLen - the length of data master read from slave.
 * @return     none.
 */
void drv_i2c_read_series(u8 slaveID, u32 addr, u32 addrLen, u8 *dataBuf, int dataLen);

/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  Pin Group or Pins
 * @return     none
 */
#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
void drv_i2c_gpio_set(I2C_GPIO_GroupTypeDef i2c_pin_group);
#elif defined(MCU_CORE_8278)
void drv_i2c_gpio_set(I2C_GPIO_SdaTypeDef sda_pin, I2C_GPIO_SclTypeDef scl_pin);
#elif defined(MCU_CORE_B91)
void drv_i2c_gpio_set(i2c_sda_pin_e sda_pin, i2c_scl_pin_e scl_pin);
#endif
