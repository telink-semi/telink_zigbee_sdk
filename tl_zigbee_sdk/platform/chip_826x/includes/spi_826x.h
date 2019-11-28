/********************************************************************************************************
 * @file     spi_826x.h
 *
 * @brief    This file provides set of functions to manage the spi interface for tlsr826x
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
#ifndef SPI_H
#define SPI_H

#include "gpio_826x.h"

/**
 *  @brief  Define the mode for SPI interface
 */
typedef enum {
    SPI_MODE0 = 0,
    SPI_MODE2,
    SPI_MODE1,
    SPI_MODE3,
} SPI_ModeTypeDef;

/**
 *  @brief  Define the pin port for SPI interface 
 */
typedef enum {
    SPI_PIN_GPIOA = 0,
    SPI_PIN_GPIOB,
} SPI_PinTypeDef;

/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */
extern void SPI_MasterPinSelect(SPI_PinTypeDef PinGrp);

/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */
extern void SPI_MasterCSPinSelect(GPIO_PinTypeDef CSPin);

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module
 * @return    none
 */
extern void SPI_MasterInit(unsigned char DivClock, SPI_ModeTypeDef Mode);

/**
 * @brief      This function serves to write a bulk of data to the SPI slave
 *             device specified by the CS pin
 * @param[in]  Cmd - pointer to the command bytes needed written into the 
 *             slave device first before the writing operation of actual data
 * @param[in]  CmdLen - length in byte of the command bytes 
 * @param[in]  Data - pointer to the data need to write 
 * @param[in]  DataLen - length in byte of the data need to write
 * @param[in]  CSPin - the CS pin specifing the slave device
 * @return     none
 */
extern void SPI_Write(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin);

/**
 * @brief      This function serves to read a bulk of data from the SPI slave
 *             device specified by the CS pin
 * @param[in]  Cmd - pointer to the command bytes needed written into the 
 *             slave device first before the reading operation of actual data
 * @param[in]  CmdLen - length in byte of the command bytes 
 * @param[out] Data - pointer to the buffer that will cache the reading out data 
 * @param[in]  DataLen - length in byte of the data need to read
 * @param[in]  CSPin - the CS pin specifing the slave device
 * @return     none
 */
extern void SPI_Read(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin);

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module
 * @return    none
 */
extern void SPI_SlaveInit(unsigned char DivClock, SPI_ModeTypeDef Mode);

/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */
extern void SPI_SlavePinSelect(SPI_PinTypeDef PinGrp);

#endif

