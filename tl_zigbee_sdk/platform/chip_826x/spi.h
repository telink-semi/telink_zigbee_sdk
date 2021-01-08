/********************************************************************************************************
 * @file	spi.h
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
#ifndef SPI_H
#define SPI_H

#include "gpio.h"

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

