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
#include "drv_spi.h"


/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module,mode 0~mode 3
 * @return    none
 */
void drv_spi_master_init(unsigned char DivClock, drv_spi_mode_type_def Mode)
{
#if	defined (MCU_CORE_826x)
	SPI_MasterInit(DivClock, Mode);
#elif defined(MCU_CORE_HAWK)

#else
	spi_master_init(DivClock, Mode);
#endif
}

/**
 * @brief     This function configures the clock and working mode for SPI interface
 * @param[in] DivClock - the division factor for SPI module
 *            SPI clock = System clock / ((DivClock+1)*2)
 * @param[in] Mode - the selected working mode of SPI module,mode 0~mode 3
 * @return    none
 */
void drv_spi_slave_init(unsigned char DivClock, drv_spi_mode_type_def Mode)
{
#if	defined (MCU_CORE_826x)
	SPI_SlaveInit(DivClock, Mode);
#elif defined(MCU_CORE_HAWK)

#else
	spi_slave_init(DivClock, Mode);
#endif
}

/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] PinGrp - the selected pin group port
 * @return    none
 */
void drv_spi_master_pin_select(drv_spi_pin_group PinGrp)
{
#if	defined (MCU_CORE_826x)
	SPI_MasterPinSelect(PinGrp);
#elif defined(MCU_CORE_HAWK)

#else
	spi_master_gpio_set(PinGrp);
#endif
}

/**
 * @brief     This function selects a GPIO pin as CS of SPI function.
 * @param[in] CSPin - the selected CS pin
 * @return    none
 */
void drv_spi_master_cspin_select(GPIO_PinTypeDef CSPin)
{
#if	defined (MCU_CORE_826x)
	SPI_MasterCSPinSelect(CSPin);
#elif defined(MCU_CORE_HAWK)

#else
	spi_masterCSpin_select(CSPin);
#endif
}

/**
 * @brief     This function selects a pin port for the SPI interface
 * @param[in] PinGrp - the selected pin port
 * @return    none
 */
void drv_spi_slave_pin_select(drv_spi_pin_group PinGrp)
{
#if	defined (MCU_CORE_826x)
	SPI_SlavePinSelect(PinGrp);
#elif defined(MCU_CORE_HAWK)

#else
	spi_slave_gpio_set(PinGrp);
#endif
}

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
void drv_spi_write(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin)
{
#if	defined (MCU_CORE_826x)
	SPI_Write(Cmd,CmdLen,Data,DataLen,CSPin);
#elif defined(MCU_CORE_HAWK)

#else
	spi_write(Cmd,CmdLen,Data,DataLen,CSPin);
#endif
}

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
void drv_spi_read(unsigned char *Cmd, int CmdLen, unsigned char *Data, int DataLen, GPIO_PinTypeDef CSPin)
{
#if	defined (MCU_CORE_826x)
	SPI_Read(Cmd,CmdLen,Data,DataLen,CSPin);
#elif defined(MCU_CORE_HAWK)

#else
	spi_read(Cmd,CmdLen,Data,DataLen,CSPin);
#endif
}
