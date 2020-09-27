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
    SPI_PIN_GPIO1 = 0,	//826x:SPI_PIN_GPIOA 8258/8278:SPI_GPIO_GROUP_A2A3A4D6
    SPI_PIN_GPIO2,		//826x:SPI_PIN_GPIOB 8258/8278:SPI_GPIO_GROUP_B6B7D2D7
} drv_spi_pin_group;

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] divClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] mode - the selected working mode of SPI module,mode 0~mode 3
 * @return    none
 */
void drv_spi_master_init(u8 divClock, drv_spi_mode_type_def mode);

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] PinGrp - the selected pin group port
 * @return    none
 */
void drv_spi_master_pin_select(drv_spi_pin_group pinGrp);
#elif defined(MCU_CORE_8278)
/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in]
 * @return    none
 */
void drv_spi_master_pin_select(SPI_GPIO_SclkTypeDef sclk_pin,SPI_GPIO_CsTypeDef cs_pin,SPI_GPIO_SdoTypeDef sdo_pin, SPI_GPIO_SdiTypeDef sdi_pin);
#elif defined(MCU_CORE_B91)

#endif

/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */
#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
void drv_spi_master_cspin_select(GPIO_PinTypeDef CSPin);
#elif defined(MCU_CORE_B91)

#endif

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] divClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] mode - the selected working mode of SPI module,mode 0~mode 3
 * @return    none
 */
void drv_spi_slave_init(u8 divClock, drv_spi_mode_type_def mode);

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] pinGrp - the selected pin port
 * @return    none
 */
void drv_spi_slave_pin_select(drv_spi_pin_group pinGrp);
#elif defined(MCU_CORE_8278)
void drv_spi_slave_pin_select(SPI_GPIO_SclkTypeDef sclk_pin, SPI_GPIO_CsTypeDef cs_pin, SPI_GPIO_SdoTypeDef sdo_pin, SPI_GPIO_SdiTypeDef sdi_pin);
#elif defined(MCU_CORE_B91)

#endif

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
/**
 * @brief      This function serves to write a bulk of data to the SPI slave
 *             device specified by the CS pin
 * @param[in]  cmd - pointer to the command bytes needed written into the
 *             slave device first before the writing operation of actual data
 * @param[in]  cmdLen - length in byte of the command bytes
 * @param[in]  data - pointer to the data need to write
 * @param[in]  dataLen - length in byte of the data need to write
 * @param[in]  csPin - the CS pin specific the slave device
 * @return     none
 */
void drv_spi_write(u8 *cmd, int cmdLen, u8 *data, int dataLen, GPIO_PinTypeDef csPin);
#elif defined(MCU_CORE_B91)

#endif

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
/**
 * @brief      This function serves to read a bulk of data from the SPI slave
 *             device specified by the CS pin
 * @param[in]  cmd - pointer to the command bytes needed written into the
 *             slave device first before the reading operation of actual data
 * @param[in]  cmdLen - length in byte of the command bytes
 * @param[out] data - pointer to the buffer that will cache the reading out data
 * @param[in]  dataLen - length in byte of the data need to read
 * @param[in]  csPin - the CS pin specifing the slave device
 * @return     none
 */
void drv_spi_read(u8 *cmd, int cmdLen, u8 *data, int dataLen, GPIO_PinTypeDef csPin);
#elif defined(MCU_CORE_B91)

#endif
