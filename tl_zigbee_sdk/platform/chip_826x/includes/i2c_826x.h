/********************************************************************************************************
 * @file     i2c_826x.h
 *
 * @brief    This file provides set of functions to manage the i2c interface
 *
 * @author   qiuwei.chen@telink-semi.com
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
#ifndef I2C_H
#define I2C_H

#include "gpio_826x.h"
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
 * @brief      This function set the id of slave device and the speed of I2C interface
 *             note: the param ID contain the bit of writting or reading.
 *             eg:the parameter 0x5C. the reading will be 0x5D and writting 0x5C.
 * @param[in]  SlaveID - the id of slave device.it contains write or read bit,the lsb is write or read bit.
 *                       ID|0x01 indicate read. ID&0xfe indicate write.
 * @param[in]  DivClock - the division factor of I2C clock, 
 *             I2C clock = System clock / (4*DivClock);if the datasheet you look at is 2*,pls modify it.
 * @return     none
 */
extern void I2C_MasterInit(unsigned char SlaveID, unsigned char DivClock);

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
 * @brief      This function writes a bulk of data to the slave device at the specified address
 * @param[in]  Addr - pointer to the address where the data will be written
 * @param[in]  AddrLen - length in byte of the address, which makes this function is  
 *             compatible for slave device with both one-byte address and two-byte address
 * @param[in]  Buf - pointer to the data will be written via I2C interface
 * @param[in]  Len - length in byte of the data will be written via I2C interface
 * @return     none
 */
extern void I2C_Write(unsigned int Addr, unsigned int AddrLen, unsigned char * pbuf, int len);

/**
 * @brief      This function reads a bulk of data from the slave device at the specified address
 * @param[in]  Addr - pointer to the address where the data will be read
 * @param[in]  AddrLen - length in byte of the address, which makes this function is  
 *             compatible for slave device with both one-byte address and two-byte address
 * @param[out] Buf - pointer to the buffer will cache the data read via I2C interface
 * @param[in]  Len - length in byte of the data will be read via I2C interface
 * @return     none
 */
extern void I2C_Read(unsigned int Addr, unsigned int AddrLen, unsigned char * pbuf, int len);

/**
 *  @brief      write data to slave that is DMA mode.refer to datasheet for DMA mode spec.
 *  @param[in]  Addr - the register that master write data to slave in. support one byte and two bytes. i.e param2 AddrLen may be 1 or 2.
 *  @param[in]  AddrLen - the length of register. enum 1 or 2. based on the spec of i2c slave.
 *  @param[in]  dataBuf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  dataLen - the length of data master write to slave.
 *  @return     none
 */
extern void I2C_WriteDataSlaveDMA(unsigned short Addr, int AddrLen, unsigned char * dataBuf, int dataLen);
/**
 * @brief      read data from slave that is DMA mode.Refer to datasheet for DMA spec.
 * @param[in]  Addr - the register master read data from slave in. support one byte and two bytes.
 * @param[in]  AddrLen - the length of register. two data: 1 or 2.
 * @param[in]  dataBuf - the first address of SRAM buffer master store data in.
 * @param[in]  dataLen - the length of data master read from slave.
 * @return     none.
 */
extern void I2C_ReadDataSlaveDMA(unsigned short Addr, int AddrLen, unsigned char * dataBuf, int dataLen);
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
