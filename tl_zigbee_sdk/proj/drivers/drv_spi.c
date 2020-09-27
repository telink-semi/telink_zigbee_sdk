/********************************************************************************************************
 * @file     drv_spi.c
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

#include "../tl_common.h"


/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] divClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] mode - the selected working mode of SPI module,mode 0~mode 3
 * @return    none
 */
void drv_spi_master_init(u8 divClock, drv_spi_mode_type_def mode)
{
#if	defined(MCU_CORE_826x)
	SPI_MasterInit(divClock, mode);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	spi_master_init(divClock, mode);
#elif defined(MCU_CORE_B91)

#endif
}

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] divClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] mode - the selected working mode of SPI module,mode 0~mode 3
 * @return    none
 */
void drv_spi_slave_init(u8 divClock, drv_spi_mode_type_def mode)
{
#if	defined(MCU_CORE_826x)
	SPI_SlaveInit(divClock, mode);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	spi_slave_init(divClock, mode);
#elif defined(MCU_CORE_B91)

#endif
}

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] pinGpio - the selected pin
 * @return    none
 */
void drv_spi_master_pin_select(drv_spi_pin_group pinGrp)
{
#if	defined(MCU_CORE_826x)
	SPI_MasterPinSelect(pinGrp);
#else
	spi_master_gpio_set(pinGrp);
#endif
}
#elif defined(MCU_CORE_8278)
void drv_spi_master_pin_select(SPI_GPIO_SclkTypeDef sclk_pin, SPI_GPIO_CsTypeDef cs_pin, SPI_GPIO_SdoTypeDef sdo_pin, SPI_GPIO_SdiTypeDef sdi_pin)
{
	spi_master_gpio_set(sclk_pin, cs_pin, sdo_pin, sdi_pin);
}
#elif defined(MCU_CORE_B91)

#endif

#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */
void drv_spi_master_cspin_select(GPIO_PinTypeDef CSPin)
{
#if	defined(MCU_CORE_826x)
	SPI_MasterCSPinSelect(CSPin);
#else
	spi_masterCSpin_select(CSPin);
#endif
}
#elif defined(MCU_CORE_B91)

#endif


#if	defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] pinGpio - the selected pin
 * @return    none
 */
void drv_spi_slave_pin_select(drv_spi_pin_group pinGrp)
{
#if	defined(MCU_CORE_826x)
	SPI_SlavePinSelect(pinGrp);
#else
	spi_slave_gpio_set(pinGrp);
#endif
}
#elif defined(MCU_CORE_8278)
void drv_spi_slave_pin_select(SPI_GPIO_SclkTypeDef sclk_pin, SPI_GPIO_CsTypeDef cs_pin, SPI_GPIO_SdoTypeDef sdo_pin, SPI_GPIO_SdiTypeDef sdi_pin)
{
	spi_slave_gpio_set(sclk_pin, cs_pin, sdo_pin, sdi_pin);
}
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
 * @param[in]  csPin - the CS pin specifing the slave device
 * @return     none
 */
void drv_spi_write(u8 *cmd, int cmdLen, u8 *data, int dataLen, GPIO_PinTypeDef csPin)
{
#if	defined(MCU_CORE_826x)
	SPI_Write(cmd, cmdLen, data, dataLen, csPin);
#else
	spi_write(cmd, cmdLen, data, dataLen, csPin);
#endif
}
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
void drv_spi_read(u8 *cmd, int cmdLen, u8 *data, int dataLen, GPIO_PinTypeDef csPin)
{
#if	defined(MCU_CORE_826x)
	SPI_Read(cmd, cmdLen, data, dataLen, csPin);
#else
	spi_read(cmd, cmdLen, data, dataLen, csPin);
#endif
}
#elif defined(MCU_CORE_B91)

#endif
