/********************************************************************************************************
 * @file	i2c.h
 *
 * @brief	This is the header file for B86
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
#include "gpio.h"

/**
 *  @brief  Define I2C pin port
 */
typedef enum {
    I2C_PIN_GPIOA = 0,
    I2C_PIN_GPIOB,
    I2C_PIN_GPIOC,
} I2C_GPIO_GroupTypeDef;

/**
 *  @brief when device as I2C slave
 *         select slave mode:1.DMA;2.MAPPING
 */
typedef enum {
	I2C_SLAVE_DMA = 0,
	I2C_SLAVE_MAP,
}I2C_SlaveMode;



/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  PinGrp - the pin port selected as I2C interface pin port.
 * @return     none
 */
extern void I2C_PinSelect(I2C_GPIO_GroupTypeDef PinGrp);

/**
 * @brief This function serves to set id of I2C module.
 * @param[in] SlaveID - this id is fixed id for slave device.For master device, this id is set to access different slave devices.
 * @return none
 */
extern void I2C_SetId(unsigned char SlaveID);

/**
 * @brief      This function set the id of slave device and the speed of I2C interface
 * @param[in]  DivClock - the division factor of I2C clock, 
 *             I2C clock = System clock / (4*DivClock);if the datasheet you look at is 2*,pls modify it.
 * @return     none
 */
extern void I2C_MasterInit(unsigned char DivClock);

/**
 *	@brief      the function config the ID of slave and mode of slave.
 *	@param[in]  device_ID - it contains write or read bit,the lsb is write or read bit.
 *			    ID|0x01 indicate read. ID&0xfe indicate write.
 *	@param[in]  mode - set slave mode. slave has two modes, one is DMA mode, the other is MAPPING mode.
 *	@param[in]  pBuf - if slave mode is MAPPING, set the first address of buffer master write or read slave.
 *	@return     none
 */
extern void I2C_SlaveInit(unsigned char device_ID,I2C_SlaveMode mode,unsigned char * pBuf);

/**
 * @brief      This function writes one byte to the slave device at the specified address
 * @param[in]  Addr - pointer to the address where the one byte data will be written
 * @param[in]  AddrLen - length in byte of the address, which makes this function is  
 *             compatible for slave device with both one-byte address and two-byte address
 * @param[in]  Data - the one byte data will be written via I2C interface
 * @return     none
 */
extern void I2C_WriteByte(unsigned int Addr, unsigned int AddrLen, unsigned char Data);

/**
 * @brief      This function reads one byte from the slave device at the specified address
 * @param[in]  Addr - pointer to the address where the one byte data will be read
 * @param[in]  AddrLen - length in byte of the address, which makes this function is  
 *             compatible for slave device with both one-byte address and two-byte address
 * @return     the one byte data read from the slave device via I2C interface
 */
extern unsigned char I2C_ReadByte(unsigned int Addr, unsigned int AddrLen);

/**
 *  @brief      This function writes a bulk of data to the slave device at the specified address
 *  @param[in]  Addr - the register that master write data to slave in. support one byte and two bytes. i.e param2 AddrLen may be 1 or 2.
 *  @param[in]  AddrLen - the length of register. enum 1 or 2. based on the spec of i2c slave.
 *  @param[in]  dataBuf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  dataLen - the length of data master write to slave.
 *  @return     none
 */
extern void I2C_WriteSeries(unsigned short Addr, int AddrLen, unsigned char * dataBuf, int dataLen);

/**
 * @brief      This function reads a bulk of data from the slave device at the specified address
 * @param[in]  Addr - the register master read data from slave in. support one byte and two bytes.
 * @param[in]  AddrLen - the length of register. two data: 1 or 2.
 * @param[in]  dataBuf - the first address of SRAM buffer master store data in.
 * @param[in]  dataLen - the length of data master read from slave.
 * @return     none.
 */
extern void I2C_ReadSeries(unsigned short Addr, int AddrLen, unsigned char * dataBuf, int dataLen);

/**
 *   @brief      master write data to slave that is mapping mode.
 *   @param[in]  pbuf - the first address of data master write to slave.
 *   @param[in]  bufLen - the length of data to write.
 *   @return     none
 */
extern void I2C_WriteDataSlaveMapping(unsigned char * pbuf,unsigned int bufLen);
/**
 *  @brief      read data from slave that is mapping mode.
 *  @param[in]  pbuf - the first address of SRAM buffer to store data.
 *  @param[in]  bufLen - the length of data read from slave(byte).
 *  @return     none
 */
extern void I2C_ReadDataSlaveMapping(unsigned char * pbuf,unsigned int bufLen);


#endif 
