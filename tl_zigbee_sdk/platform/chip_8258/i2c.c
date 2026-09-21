/********************************************************************************************************
 * @file    i2c.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "i2c.h"
#include "gpio.h"

/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  i2c_pin_group - the pin port selected as I2C interface pin port.
 * @return     none
 * 	A3:5b7[0] set 1 as spi input,set 0 not as spi input ;5b7[4] set 1 as i2c input ,set 0 not as i2c input
 *	A4:5b7[1] set 1 as spi input,set 0 not as spi input ;5b7[5] set 1 as i2c input ,set 0 not as i2c input
 *	B6:5b7[2] set 1 as spi input,set 0 not as spi input ;5b7[6] set 1 as i2c input ,set 0 not as i2c input
 *	D7:5b7[3] set 1 as spi input,set 0 not as spi input ;5b7[7] set 1 as i2c input ,set 0 not as i2c input
 */
void i2c_gpio_set(I2C_GPIO_GroupTypeDef i2c_pin_group)
{
    GPIO_PinTypeDef sda, scl;

    if (i2c_pin_group == I2C_GPIO_GROUP_A3A4) {
        sda = GPIO_PA3;
        scl = GPIO_PA4;
        reg_pin_i2c_spi_en |= (FLD_PIN_PA3_I2C_EN | FLD_PIN_PA4_I2C_EN);
        reg_pin_i2c_spi_en &= ~(FLD_PIN_PA3_SPI_EN | FLD_PIN_PA4_SPI_EN);
    } else if (i2c_pin_group == I2C_GPIO_GROUP_B6D7) {
        sda = GPIO_PB6;
        scl = GPIO_PD7;
        reg_pin_i2c_spi_en |= (FLD_PIN_PB6_I2C_EN | FLD_PIN_PD7_I2C_EN);
        reg_pin_i2c_spi_en &= ~(FLD_PIN_PB6_SPI_EN | FLD_PIN_PD7_SPI_EN);
    } else if (i2c_pin_group == I2C_GPIO_GROUP_C0C1) {
        sda = GPIO_PC0;
        scl = GPIO_PC1;
    } else if (i2c_pin_group == I2C_GPIO_GROUP_C2C3) {
        sda = GPIO_PC2;
        scl = GPIO_PC3;
    } else{ //ERR
        sda = 0;
        scl = 0;
    }
     //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will may misread the short low-level timing.confirmed by minghai.20210709.
    gpio_set_input_en(sda, 1);//enable sda input
    gpio_set_input_en(scl, 1);//enable scl input
    gpio_setup_up_down_resistor(sda, PM_PIN_PULLUP_10K);
    gpio_setup_up_down_resistor(scl, PM_PIN_PULLUP_10K);
    gpio_set_func(sda, AS_I2C);
    gpio_set_func(scl, AS_I2C);
}

/**
 * @brief      This function serves to set the id of slave device and the speed of I2C interface
 *             note: the param ID contain the bit of writing or reading.
 *             eg:the parameter 0x5C. the reading will be 0x5D and writing 0x5C.
 * @param[in]  DivClock - the division factor of I2C clock,
 *             I2C clock = System clock / (4*DivClock);if the datasheet you look at is 2*,pls modify it.
 * @return     none
 */
void i2c_master_init(unsigned char DivClock)
{
    reg_i2c_speed = DivClock; //i2c clock = system_clock/(4*DivClock)
    //reg_i2c_id	  = SlaveID; //slave address
    reg_i2c_mode |= FLD_I2C_MASTER_EN; //enable master mode

    reg_clk_en0 |= FLD_CLK0_I2C_EN;    //enable i2c clock
    reg_spi_sp  &= ~FLD_SPI_ENABLE;   //force PADs act as I2C; i2c and spi share the hardware of IC
}

/**
 *  @brief      This function serves to set the ID and mode of slave device.
 *  @param[in]  device_ID - it contains write or read bit,the lsb is write or read bit.
 *              ID|0x01 indicate read. ID&0xfe indicate write.
 *  @param[in]  mode - set slave mode. slave has two modes, one is DMA mode, the other is MAPPING mode.
 *  @param[in]  pMapBuf - if slave mode is MAPPING, set the first address of buffer master write or read slave.
 *  @return     none
 */
void i2c_slave_init(unsigned char device_ID, I2C_SlaveMode mode, unsigned char *pMapBuf)
{
    reg_i2c_id = device_ID; //slave address

    reg_clk_en0 |= FLD_CLK0_I2C_EN;    //enable i2c clock
    reg_spi_sp  &= ~FLD_SPI_ENABLE;   //force PADs act as I2C; i2c and spi share the hardware of IC

    reg_i2c_mode &= (~FLD_I2C_MASTER_EN); //enable slave mode
     //notice that: both dma and mapping mode need this to trigger data address auto increase(confirmed by congqing and sihui)
    reg_i2c_mode |= FLD_I2C_ADDR_AUTO_ADD;

    if (mode == I2C_SLAVE_MAP) {
        reg_i2c_mode |= FLD_I2C_SLAVE_MAPPING;
        reg_i2c_slave_map_addrl  = (unsigned char)(((unsigned int)pMapBuf & 0xff));
        reg_i2c_slave_map_addrm = (unsigned char)(((unsigned int)pMapBuf >> 8) & 0xff);
        reg_i2c_slave_map_addrh = 0x04;
    } else {
        reg_i2c_mode &= ~FLD_I2C_SLAVE_MAPPING;
    }
}

/**
 * @brief      This function serves to write one byte to the slave device at the specified address
 * @param[in]  Addr - i2c slave address where the one byte data will be written
 * @param[in]  AddrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @param[in]  Data - the one byte data will be written via I2C interface
 * @return     none
 */
void i2c_write_byte(unsigned int Addr, unsigned int AddrLen, unsigned char Data)
{
    reg_i2c_id &= (~FLD_I2C_WRITE_READ_BIT); 	//SlaveID & 0xfe,.i.e write data. R:High  W:Low

    //start + id(Write) + address
    if (AddrLen == 0) {  //telink 82xx slave mapping mode no need send any address
        //launch start /id    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_START);// when addr=0,other (not telink) iic device no need start signal
    } else if (AddrLen == 1) {
    	reg_i2c_adr = (unsigned char)Addr; //address
        //launch start /id/04    start
    	reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_START);
    } else if (AddrLen == 2) {
    	reg_i2c_adr = (unsigned char)(Addr >> 8); //address high
    	reg_i2c_do  = (unsigned char)Addr; //address low
        //launch start /id/04/05    start
    	reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_DO | FLD_I2C_CMD_START);
    } else if (AddrLen == 3) {
        reg_i2c_adr = (unsigned char)(Addr >> 16); //address high
        reg_i2c_do  = (unsigned char)(Addr >> 8); //address middle
        reg_i2c_di  = (unsigned char)(Addr);    //address low
        //launch start /id/04/05/06    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_DO | FLD_I2C_CMD_DI | FLD_I2C_CMD_START);
    }
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);

    //write data
    reg_i2c_di = Data;
    reg_i2c_ctrl = FLD_I2C_CMD_DI; //launch data read cycle
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);
    //stop
    reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);
}

/**
 * @brief      This function serves to read one byte from the slave device at the specified address
 * @param[in]  Addr - i2c slave address where the one byte data will be read
 * @param[in]  AddrLen - length in byte of the address, which makes this function is
 *             compatible for slave device with both one-byte address and two-byte address
 * @return     the one byte data read from the slave device via I2C interface
 */
unsigned char i2c_read_byte(unsigned int Addr, unsigned int AddrLen)
{
    unsigned char ret = 0;

    reg_i2c_id &= (~FLD_I2C_WRITE_READ_BIT); //SlaveID & 0xfe,.i.e write data. R:High  W:Low

    //start + id(Write) + address
    if (AddrLen == 0) {  //telink 82xx slave mapping mode no need send any address
        //launch start /id    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_START);// when addr=0,other (not telink) iic device no need start signal
    } else if (AddrLen == 1) {
        reg_i2c_adr = (unsigned char)Addr; //address
        //launch start /id/04    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_START);
    } else if (AddrLen == 2) {
        reg_i2c_adr = (unsigned char)(Addr >> 8); //address high
        reg_i2c_do  = (unsigned char)Addr; //address low
        //launch start /id/04/05    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_DO | FLD_I2C_CMD_START);
    } else if (AddrLen == 3) {
        reg_i2c_adr = (unsigned char)(Addr >> 16); //address high
        reg_i2c_do  = (unsigned char)(Addr >> 8); //address middle
        reg_i2c_di  = (unsigned char)(Addr);    //address low
        //launch start /id/04/05/06    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_DO | FLD_I2C_CMD_DI | FLD_I2C_CMD_START);
    }
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);

    //start + id(Read)
    reg_i2c_id |= FLD_I2C_WRITE_READ_BIT;  //SlaveID & 0xfe,.i.e write data. Read:High  Write:Low
    reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_START);
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);

    //read data
    reg_i2c_ctrl = (FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID | FLD_I2C_CMD_ACK);
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);
    ret = reg_i2c_di;

    //stop
    reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);

    return ret;
}

/**
 *  @brief      This function serves to write a packet of data to the specified address of slave device
 *  @param[in]  Addr - the register that master write data to slave in. support one byte and two bytes. i.e param2 AddrLen may be 1 or 2.
 *  @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3. based on the spec of i2c slave.
 *  @param[in]  dataBuf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  dataLen - the length of data master write to slave.
 *  @return     none
 */
void i2c_write_series(unsigned int Addr, unsigned int AddrLen, unsigned char *dataBuf, int dataLen)
{
    reg_i2c_id &= (~FLD_I2C_WRITE_READ_BIT); //SlaveID & 0xfe,.i.e write data. R:High  W:Low

    //start + id(Write) + address
    if (AddrLen == 0) {  //telink 82xx slave mapping mode no need send any address
        //launch start /id    start
    	reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_START);// when addr=0,other (not telink) iic device no need start signal
    } else if (AddrLen == 1) {
    	reg_i2c_adr = (unsigned char)Addr; //address
        //launch start /id/04    start
    	reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_START);
    } else if (AddrLen == 2) {
    	reg_i2c_adr = (unsigned char)(Addr >> 8); //address high
    	reg_i2c_do  = (unsigned char)Addr; //address low
        //launch start /id/04/05    start
    	reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_DO | FLD_I2C_CMD_START);
    } else if (AddrLen == 3) {
        reg_i2c_adr = (unsigned char)(Addr >> 16); //address high
        reg_i2c_do  = (unsigned char)(Addr >> 8); //address middle
        reg_i2c_di  = (unsigned char)(Addr);    //address low
        //launch start /id/04/05/06    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_DO | FLD_I2C_CMD_DI | FLD_I2C_CMD_START);
    }
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);

    //write data
    unsigned int buff_index = 0;
    for (buff_index = 0; buff_index < dataLen; buff_index++) {
        reg_i2c_di = dataBuf[buff_index];
        reg_i2c_ctrl = FLD_I2C_CMD_DI; //launch data read cycle
        while(reg_i2c_status & FLD_I2C_CMD_BUSY);
    }
    //stop
    reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);
}

/**
 * @brief      This function serves to read a packet of data from the specified address of slave device
 * @param[in]  Addr - the register master read data from slave in. support one byte and two bytes.
 * @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3 based on the spec of i2c slave.
 * @param[in]  dataBuf - the first address of SRAM buffer master store data in.
 * @param[in]  dataLen - the length of data master read from slave.
 * @return     none.
 */
void i2c_read_series(unsigned int Addr, unsigned int AddrLen, unsigned char *dataBuf, int dataLen)
{
    reg_i2c_id &= (~FLD_I2C_WRITE_READ_BIT); //SlaveID & 0xfe,.i.e write data. R:High  W:Low

    //start + id(Write) + address
    if (AddrLen == 0) {  //telink 82xx slave mapping mode no need send any address
        //launch start /id    start
    	reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_START);
    }else if (AddrLen == 1) {
        reg_i2c_adr = (unsigned char)Addr; //address
        //launch start /id/04    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_START);
    } else if (AddrLen == 2) {
        reg_i2c_adr = (unsigned char)(Addr >> 8); //address high
        reg_i2c_do  = (unsigned char)Addr; //address low
        //launch start /id/04/05    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_DO | FLD_I2C_CMD_START);
    } else if (AddrLen == 3) {
        reg_i2c_adr = (unsigned char)(Addr >> 16); //address high
        reg_i2c_do  = (unsigned char)(Addr >> 8); //address middle
        reg_i2c_di  = (unsigned char)(Addr);    //address low
        //launch start /id/04/05/06    start
        reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_DO | FLD_I2C_CMD_DI | FLD_I2C_CMD_START);
    }
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);

    //start + id(Read)
    reg_i2c_id |= FLD_I2C_WRITE_READ_BIT;  //SlaveID & 0xfe,.i.e write data. Read:High  Write:Low
    reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_START);
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);

    //read data
    unsigned int bufIndex = 0;

    dataLen--;    //the length of reading data must larger than 0
    //if not the last byte master read slave, master wACK to slave
    while (dataLen) {
        reg_i2c_ctrl = (FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID);
        while(reg_i2c_status & FLD_I2C_CMD_BUSY);
        dataBuf[bufIndex] = reg_i2c_di;
        bufIndex++;
        dataLen--;
    }
    //when the last byte, master will ACK to slave
    reg_i2c_ctrl = (FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID | FLD_I2C_CMD_ACK);
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);
    dataBuf[bufIndex] = reg_i2c_di;

    //terminate
    reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
    while(reg_i2c_status & FLD_I2C_CMD_BUSY);
}

