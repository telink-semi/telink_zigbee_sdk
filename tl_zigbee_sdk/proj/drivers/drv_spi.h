/********************************************************************************************************
 * @file     drv_spi.h
 *
 * @brief	 spi driver interface file
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

/**
 *  @brief  Define the mode for SPI interface
 */
typedef enum {
    SPI_MODE_0 = 0,
    SPI_MODE_2,
    SPI_MODE_1,
    SPI_MODE_3,
}drv_spi_mode_type_def;

/**
 *  @brief  Define the pin port for SPI interface
 */
typedef enum {
    SPI_PIN_GPIO1 = 0,	//826x:SPI_PIN_GPIOA 8258:SPI_GPIO_GROUP_A2A3A4D6
    SPI_PIN_GPIO2,		//826x:SPI_PIN_GPIOB 8258:SPI_GPIO_GROUP_B6B7D2D7
} drv_spi_pin_group;

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module,mode 0~mode 3
 * @return    none
 */
extern void drv_spi_master_init(unsigned char DivClock, drv_spi_mode_type_def Mode);

/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] PinGrp - the selected pin group port
 * @return    none
 */
extern void drv_spi_master_pin_select(drv_spi_pin_group PinGrp);

/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */
extern void drv_spi_master_cspin_select(GPIO_PinTypeDef CSPin);

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module,mode 0~mode 3
 * @return    none
 */
extern void drv_spi_slave_init(unsigned char DivClock, drv_spi_mode_type_def Mode);

/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */
extern void drv_spi_slave_pin_select(drv_spi_pin_group PinGrp);

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
extern void drv_spi_write(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin);

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
extern void drv_spi_read(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin);

