/********************************************************************************************************
 * @file    spi.h
 *
 * @brief   This is the header file for TL721X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef SPI_H
#define SPI_H

#include "reg_include/register.h"
#include "gpio.h"
#include "dma.h"
/** @page SPI
 *
 *  Introduction
 *  ===============
 *  TL721X supports two spi
 *
 *  API Reference
 *  ===============
 *  Header File: spi.h
 */

/** @page SPI
 *
 *  Introduction
 *  ===============
 *  TL721X supports two spi:gspi lspi
 *
 *  API Reference
 *  ===============
 *  Header File: spi.h
 *  How to use this driver
 *  ==============
  -# SPI Initialization and Configuration
     -# nodma/dma
        - Initialize the gspi/lspi pin by gspi_set_pin() or lspi_set_pin() API;
        - SPI as master:Configure clock idle and phase modes, whether cmd is used,
          how many bytes cmd occupies and the format of cmd, whether addr is used,
          how many bytes addr occupies and the format of addr,
          and the byte length of dummy via the spi_master_config_plus() API.
        - Configure rx_timeout by uart_set_rx_timeout();
        - SPI as master:Call the spi_master_init() API to initialize the spi clock.
        - SPI as slave:Clock edge and phase mode configuration of the slave via the spi_slave_init API.
        - SPI as slave:The slave's dummy byte value via the spi_set_dummy_cnt() API.
     -# dma
        - Initial dma burst size configuration via dma_set_burst_size() API.
        - Initial spi tx dma channel via spi_set_tx_dma_config()API.
        - SPI as master:Initial spi rx dma channel via spi_set_master_rx_dma_config() API.
        - SPI as slave:Initial spi rx dma channel via spi_set_slave_rx_dma_config() API.

  -# SPI Interrupts Configuration and Response
     -# SPI interrupts initial configuration:
        - nodma_tx
           - SPI as master:polling send, it does not need to configure interrupt,it is possible to use spi_master_write_plus() API;
           - SPI as slave:spi_write(),spi_set_irq_mask():SPI_SLV_CMD_EN,SPI_TXF_INT,SPI_TXFIFO_INT_EN,SPI_END_INT_EN;
        - nodma_rx
           - SPI as master:polling send, it does not need to configure interrupt,it is possible to use spi_master_read_plus() API;
           - SPI as slave:spi_read(),spi_set_irq_mask():SPI_SLV_CMD_EN,SPI_RXFIFO_INT_EN,SPI_END_INT_EN;
        - dma_tx
           - SPI as master:spi_set_irq_mask() : SPI_END_INT_EN ;
           - SPI as slave:spi_set_irq_mask():SPI_SLV_CMD_EN,SPI_END_INT_EN;
        - dma_rx
           - When the received length configured for rx_dma is less than 0xfffffc : spi_set_irq_mask() : SPI_END_INT ;
           - When the received length configured for rx_dma is 0xfffffc:dma_set_irq_mask() : TC_MASK ;
        - plic_interrupt_enable() / core_interrupt_enable() API
     -# SPI interrupt response(for details, see the enumeration spi_irq_status_e):
        - spi_get_irq_status() API;
        - spi_clr_irq_status() API;
        - nodma_rx
            - SPI_SLV_CMD_INT/SPI_RXFIFO_INT/SPI_END_INT;
        - dma_tx
            - SPI_SLV_CMD_INT/SPI_END_INT ;
        - dma_rx
           - when the received length configured for rx_dma is less than 0xfffffc :
               - SPI_SLV_CMD_INT,SPI_END_INT;
           - when the received length configured for rx_dma is 0xfffffc:
               - See the TC_MASK note for details;

  -# SPI io mode and irq status
      - spi mode
           - the spi's io mode supports single-wire, two-wire, three-wire, and four-wire modes.
      - interrupt status
           - before using spi interrupts, it is best to clear the corresponding interrupt bit via the spi_clr_irq_status() API to prevent false triggering.

  -# SPI_XIP
      - how to use
          - The SPI module clock is first initialized by calling the spi_master_init() API.
          - The second step initializes the pins of the spi by calling the gspi_set_xip_pin() or lspi_set_xip_pin() APIs.
          - The third step sets the end address of the SPI map by calling the gspi_xip_end_addr_set() API.
          - Step 4 Call the spi_xip_timeout_cnt() and spi_xip_timeout_mode_en() APIs to set the SPI XIP timeout and enable the timeout mode.
          - Step 5 Set the SPI XIP page size and enable page size mode by calling the spi_xip_page_size() and spi_xip_page_mode_en() APIs.
          - Finally, the SPI's XIP mode is enabled by calling the spi_xip_en() API.

  -# SPI_SLV
      - how to use
          - the pins are first initialized by calling the spi_slave_set_pin() API, and then SPI_SLV is enabled by calling the spi_slave_ready_en() API.
          - SPI_SLV If the spi master passes successfully the SPI_SLV module will respond to the master by sending one byte of data: 0x5a.

  -# SPI_END_INT_EN Recommended usage
      - spi_master_write_dma_plus()/spi_master_write_dma(): Check whether sending is complete.
          - If interrupt is enabled, spi_set_irq_mask() :SPI_END_INT_EN;
          - If interrupt is not enabled, the spi_is_busy() interface is queried.
      - spi_master_write_dma_plus()/spi_master_write_dma(): Check whether sending is complete.
          - spi_master_read_dma_plus()/spi_master_write_read_dma()/spi_master_write_read_dma_plus(): Determine whether it is complete:If interrupt is enabled, dma_set_irq_mask() : TC_MASK;
          - If interrupt is not enabled, the interface dma_get_tc_irq_status() is polled.
      - special note:
          - When spi_master_read_dma_plus()/spi_master_write_read_dma()/spi_master_write_read_dma_plus() is used, TC_MASK is used, because SPI_END_EN can only represent the completion of spi receiving,
            and can not represent whether the dma has completed the work, to prevent abnormal situations.
          - If spi_set_irq_mask() :SPI_END_INT_EN is enabled when master read is used, disable it before read. Enable the read function after it is complete.
 */

/**
 *  @brief  Define SPI IRQ mask.
 */
typedef enum{
    SPI_RXFIFO_OR_INT_EN        =BIT(0),
    SPI_TXFIFO_UR_INT_EN        =BIT(1),
    SPI_RXFIFO_INT_EN           =BIT(2),
    SPI_TXFIFO_INT_EN           =BIT(3),
    SPI_END_INT_EN              =BIT(4),
    SPI_SLV_CMD_EN              =BIT(5),
}spi_irq_mask;

/**
 *  @brief  Define SPI IRQ status.
 */
typedef enum{
    SPI_RXF_OR_INT      =BIT(0),
    SPI_TXF_UR_INT      =BIT(1),
    SPI_RXF_INT         =BIT(2),
    SPI_TXF_INT         =BIT(3),
    SPI_END_INT         =BIT(4),
    SPI_SLV_CMD_INT     =BIT(5),
}spi_irq_status_e;

/**
 *  @brief  Define SPI module.
 */
typedef enum{
    LSPI_MODULE = 0,
    GSPI_MODULE = 1 ,
}spi_sel_e;

/**
 * @brief  Define the work mode.
 */
typedef enum{
    SPI_MODE0 = 0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3,
}spi_mode_type_e;

/**
 * @brief  Define the mode for SPI io mode.
 */
typedef enum{
    SPI_SINGLE_MODE  = 0,
    SPI_DUAL_MODE    = 1,
    SPI_QUAD_MODE    = 2,
    SPI_3_LINE_MODE  = 3
}spi_io_mode_e;

/**
 * @brief  Define  SPI io mode only normal and 3line.
 */
typedef enum{
    SPI_NORMAL = 0,
    SPI_3LINE = 3,
}spi_normal_3line_mode_e;

/**
 *  @brief  Define token value.
 */
typedef enum{
    SPI_TOKEN_00 = 0,
    SPI_TOKEN_69 = 1,
}spi_token_val_e;

/**
 * @brief  Define the SPI translate mode.
 */
typedef enum{
    SPI_MODE_WRITE_AND_READ = 0,//write and read at the same.must enable CmdEn.
    SPI_MODE_WRITE_ONLY,//write.
    SPI_MODE_READ_ONLY,// read must enable CmdEn.
    SPI_MODE_WRITE_READ,//write_ read.
    SPI_MODE_READ_WRITE,//read_write.
    SPI_MODE_WRITE_DUMMY_READ,//write_dummy_read.
    SPI_MODE_READ_DUMMY_WRITE,//read_ dummy_write must enable CmdEn.
    SPI_MODE_NONE_DATA,//must enable CmdEn.
    SPI_MODE_DUMMY_WRITE,//dummy_write.
    SPI_MODE_DUMMY_READ,//dummy_read.
    SPI_MODE_DUMMY_WRITE_AND_READ,//dummy,write and read at the same.must enable CmdEn.
    SPI_MODE_RESERVED,
}spi_tans_mode_e;

/**
 *  @brief  Define SPI write transmission mode.
 */
typedef enum{
    SPI_MODE_WR_WRITE_ONLY  = 1,//write.
    SPI_MODE_WR_DUMMY_WRITE = 8,//dummy_write.
}spi_wr_tans_mode_e;

/**
 * @brief  Define the SPI read transmission mode.
 */
typedef enum{
    SPI_MODE_RD_READ_ONLY  = 2,//must enable CmdEn.
    SPI_MODE_RD_DUMMY_READ = 9,//dummy_read.
}spi_rd_tans_mode_e;

/**
 * @brief  Define the SPI write and read transmission mode.
 */
typedef enum{
    SPI_MODE_WR_RD       = 3,//must enable CmdEn.
    SPI_MODE_WR_DUMMY_RD = 5,//write_dummy_read.
}spi_wr_rd_tans_mode_e;

/**
 * @brief  Define the SPI slave write command.
 */
typedef enum{
    SPI_SLAVE_WRITE_DATA_CMD = 0x00,
    SPI_SLAVE_WRITE_DATA_DUAL_CMD = FLD_SPI_CMD_DATA_DUAL,
    SPI_SLAVE_WRITE_ADDR_DUAL_CMD = FLD_SPI_CMD_ADDR_DUAL,

    SPI_SLAVE_WRITE_DATA_DUAL_4CYC_CMD = FLD_SPI_CMD_DATA_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
    SPI_SLAVE_WRITE_ADDR_DUAL_4CYC_CMD = FLD_SPI_CMD_ADDR_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
    SPI_SLAVE_WRITE_DATA_AND_ADDR_DUL_4CYC_CMD = FLD_SPI_CMD_ADDR_DUAL | FLD_SPI_CMD_DATA_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
}spi_slave_write_cmd_e;

/**
 * @brief  Define the SPI slave read command.
 */
typedef enum{
    SPI_SLAVE_READ_DATA_CMD = FLD_SPI_CMD_RD_EN,
    SPI_SLAVE_READ_DATA_DUAL_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_DATA_DUAL,
    SPI_SLAVE_READ_ADDR_DUAL_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_ADDR_DUAL,

    SPI_SLAVE_READ_DATA_DUAL_4CYC_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_DATA_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
    SPI_SLAVE_READ_ADDR_DUAL_4CYC_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_ADDR_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,

    SPI_SLAVE_READ_DATA_AND_ADDR_DUL_4CYC_CMD = FLD_SPI_CMD_RD_EN | FLD_SPI_CMD_ADDR_DUAL | FLD_SPI_CMD_DATA_DUAL | FLD_SPI_CMD_RD_DUMMY_4CYCLE,
}spi_slave_read_cmd_e;

/**
 * @brief  Define the SPI command for Telink RISC-V MCU gspi/lspi slave.
 */
typedef enum{
    SPI_READ_STATUS_SINGLE_CMD  = 0x05,
    SPI_READ_STATUS_DUAL_CMD    = 0x15,
    SPI_READ_STATUS_QUAD_CMD    = 0x25,
    SPI_READ_DATA_SINGLE_CMD    = 0x0B,
    SPI_READ_DATA_DUAL_CMD      = 0x0C,
    SPI_READ_DATA_QUAD_CMD      = 0x0E,
    SPI_WRITE_DATA_SINGLE_CMD   = 0x51,
    SPI_WRITE_DATA_DUAL_CMD     = 0x52,
    SPI_WRITE_DATA_QUAD_CMD     = 0x54,
}spi_cmd_e;

/**
 * @brief  Define the SPI write read configuration struct.
 */
typedef struct{
    spi_io_mode_e  spi_io_mode;//set spi io mode.
    unsigned short spi_dummy_cnt;//set dummy cnt if tans_mode have dummy.
    unsigned char spi_cmd_en;//enable cmd phase.
    unsigned char spi_addr_en;//enable address phase.
    unsigned char spi_addr_len;//enable address phase.
    unsigned char spi_cmd_fmt_en;//if cmd_en enable cmd fmt will follow the interface (dual/quad).
    unsigned char spi_addr_fmt_en;//if addr_en enable addr fmt will follow the interface (dual/quad).
    unsigned char spi_token_val_sel;//select token val.
    unsigned char spi_token_en;//enable token mode.
}spi_wr_rd_config_t;

/**
 * @brief  Define the lspi pin.
 */
typedef enum{
    LSPI_CSN_PA0_PIN        = GPIO_PA0,
    LSPI_CSN_PA1_PIN        = GPIO_PA1,
    LSPI_CSN_PA2_PIN        = GPIO_PA2,
    LSPI_CSN_PA3_PIN        = GPIO_PA3,
    LSPI_CSN_PA4_PIN        = GPIO_PA4,
    LSPI_CSN_PB0_PIN        = GPIO_PB0,
    LSPI_CSN_PB1_PIN        = GPIO_PB1,
    LSPI_CSN_PB2_PIN        = GPIO_PB2,
    LSPI_CSN_PB3_PIN        = GPIO_PB3,
    LSPI_CSN_PB4_PIN        = GPIO_PB4,
    LSPI_CSN_PB5_PIN        = GPIO_PB5,
    LSPI_CSN_PB6_PIN        = GPIO_PB6,
    LSPI_CSN_PB7_PIN        = GPIO_PB7,
    LSPI_CSN_PC0_PIN        = GPIO_PC0,
    LSPI_CSN_PC1_PIN        = GPIO_PC1,
    LSPI_CSN_PC2_PIN        = GPIO_PC2,
    LSPI_CSN_PC3_PIN        = GPIO_PC3,
    LSPI_CSN_PC4_PIN        = GPIO_PC4,
    LSPI_CSN_PC5_PIN        = GPIO_PC5,
    LSPI_CSN_PC6_PIN        = GPIO_PC6,
    LSPI_CSN_PC7_PIN        = GPIO_PC7,
    LSPI_CSN_PD0_PIN        = GPIO_PD0,
    LSPI_CSN_PD1_PIN        = GPIO_PD1,
    LSPI_CSN_PD2_PIN        = GPIO_PD2,
    LSPI_CSN_PD3_PIN        = GPIO_PD3,
    LSPI_CSN_PD4_PIN        = GPIO_PD4,
    LSPI_CSN_PD5_PIN        = GPIO_PD5,
    LSPI_CSN_PD6_PIN        = GPIO_PD6,
    LSPI_CSN_PD7_PIN        = GPIO_PD7,
    LSPI_CSN_PE0_PIN        = GPIO_PE0,
    LSPI_CLK_PE1_PIN        = GPIO_PE1,
    LSPI_MOSI_IO0_PE2_PIN   = GPIO_PE2,
    LSPI_MISO_IO1_PE3_PIN   = GPIO_PE3,
    LSPI_IO2_PE4_PIN        = GPIO_PE4,
    LSPI_IO3_PE5_PIN        = GPIO_PE5,
    LSPI_CSN_PE6_PIN        = GPIO_PE6,
    LSPI_CSN_PE7_PIN        = GPIO_PE7,
    LSPI_CSN_PF0_PIN        = GPIO_PF0,
    LSPI_CSN_PF1_PIN        = GPIO_PF1,
    LSPI_CSN_PF2_PIN        = GPIO_PF2,
    LSPI_CSN_PF3_PIN        = GPIO_PF3,
    LSPI_CSN_PF4_PIN        = GPIO_PF4,
    LSPI_CSN_PF5_PIN        = GPIO_PF5,
    LSPI_CSN_PF6_PIN        = GPIO_PF6,
    LSPI_CSN_PF7_PIN        = GPIO_PF7,
}lspi_pin_def_e;

/**
 * @brief  Define the GSPI pin definition structure.
 */
typedef struct{
    gpio_func_pin_e         spi_clk_pin;
    gpio_func_pin_e         spi_csn_pin;
    gpio_func_pin_e     spi_mosi_io0_pin;
    gpio_func_pin_e     spi_miso_io1_pin;
    gpio_func_pin_e         spi_io2_pin;
    gpio_func_pin_e         spi_io3_pin;
}gspi_pin_config_t;

/**
 * @brief  Define the LSPI pin definition structure.
 */
typedef struct{
    lspi_pin_def_e         spi_clk_pin;
    lspi_pin_def_e         spi_csn_pin;
    lspi_pin_def_e     spi_mosi_io0_pin;
    lspi_pin_def_e     spi_miso_io1_pin;
    lspi_pin_def_e         spi_io2_pin;
    lspi_pin_def_e         spi_io3_pin;
}lspi_pin_config_t;

/**
 * @brief  Define the GSPI XIP pin definition structure.
 */
typedef struct{
    gpio_func_pin_e      gspi_clk_pin;
    gpio_func_pin_e        gspi_csn0_pin;
    gpio_func_pin_e        gspi_csn1_pin;
    gpio_func_pin_e        gspi_csn2_pin;
    gpio_func_pin_e        gspi_csn3_pin;
    gpio_func_pin_e    gspi_mosi_io0_pin;
    gpio_func_pin_e    gspi_miso_io1_pin;
    gpio_func_pin_e         gspi_io2_pin;
    gpio_func_pin_e         gspi_io3_pin;
}gspi_xip_pin_config_t;

/**
 * @brief  Define the SSPI pin definition structure.
 */
typedef struct{
    gpio_func_pin_e         spi_clk_pin;
    gpio_func_pin_e         spi_csn_pin;
    gpio_func_pin_e     spi_mosi_io0_pin;
    gpio_func_pin_e     spi_miso_io1_pin;
}sspi_pin_config_t;

/**
 * @brief  Define the gspi xip enum.
 */
typedef enum{
    GSPI_XIP0 = 0,
    GSPI_XIP1 = 1,
    GSPI_XIP2 = 2,
    GSPI_XIP3 = 3,
}gspi_xip_e;

/**
 * @brief  Define spi bit sequence.
 */
typedef enum{
    SPI_MSB = 0,
    SPI_LSB = 1,
}spi_bit_seq_e;

/**
 * @brief  Define the gspi xip configuration struct.
 */
#define spi_xip_addr_bytes(n) ((n)-1)
#define spi_xip_dummy_cnt(n) ((n)-1)
typedef struct{
    //Byte 0~3.
    unsigned char spi_xip_rd_io_mode:2;//set spi interface mode.
    unsigned char spi_xip_rd_addr_len:2;//address byte length.
    unsigned char spi_xip_rd_addr_fmt_en:1;//if addr_en enable addr fmt will follow the interface (dual/quad).
    unsigned char spi_xip_rd_addr_en:1;//enable address phase.
    unsigned char spi_xip_rd_cmd_fmt_en:1;//if cmd_en enable cmd fmt will follow the interface (dual/quad).
    unsigned char spi_xip_rd_cmd_en:1;//enable cmd phase.
    unsigned char spi_xip_rd_dummy_cnt:4;//set dummy cnt if tans_mode have dummy.
    unsigned char spi_xip_rd_transmode:4;//set read transmode.
    unsigned char spi_xip_rd_cmd1_en:1;//enable cmd1 phase.
    unsigned char spi_xip_rd_token_val_sel:1;//select token val.
    unsigned char spi_xip_rd_token_en:1;//enable token mode.
    unsigned char spi_xip_rd_dummy_cnt_add:1;//set dummy_cnt_add.
    unsigned char :3;
    unsigned char spi_xip_rd_cmd;//set read cmd.
    //Byte 4~7;
    unsigned char spi_xip_wr_io_mode:2;//set spi interface mode.
    unsigned char spi_xip_wr_addr_len:2;//address byte length.
    unsigned char spi_xip_wr_addr_fmt_en:1;//if addr_en enable addr fmt will follow the interface (dual/quad).
    unsigned char spi_xip_wr_addr_en:1;//enable address phase.
    unsigned char spi_xip_wr_cmd_fmt_en:1;//if cmd_en enable cmd fmt will follow the interface (dual/quad).
    unsigned char spi_xip_wr_cmd_en:1;//enable cmd phase.
    unsigned char spi_xip_wr_dummy_cnt:4;//set dummy cnt if tans_mode have dummy.
    unsigned char spi_xip_wr_transmode:4;//set write transmode.
    unsigned char spi_xip_wr_cmd1_en:1;//enable cmd1 phase.
    unsigned char :7;
    unsigned char spi_xip_wr_cmd;//set write cmd.
    //Byte 8~10;
    unsigned char spi_3_line_en:1;//enable 3-line mode,must set spi_xip_rd_io_mode and spi_xip_wr_io_mode as SPI_SINGLE_MODE.
    unsigned char :7;
    unsigned char spi_xip_rd_cmd1;//set read second cmd, only support MXIC flash.
    unsigned char spi_xip_wr_cmd1;//set write second cmd, only support MXIC flash.
}spi_xip_config_t;

/**
 * @brief  Define the gspi xip end address enum.
 */
typedef enum{
    GSPI_XIP_16M = 0,
    GSPI_XIP_32M = 1,
    GSPI_XIP_48M = 2,
    GSPI_XIP_64M = 3,
}gspi_xip_addr_e;

/**
 *  @brief  Define lspi lcd RGB mode.
 */
typedef enum{
    LSPI_LCD_RGB_RESERVED  = 0x00,
    LSPI_LCD_RGB565        = 0x01,
    LSPI_LCD_RGB666        = 0x02,
    LSPI_LCD_RGB888        = 0x03,
}lspi_lcd_rgb_mode_e;

/**
 * @brief  Define spi api error code.
 */
typedef enum {
    SPI_API_ERROR_TIMEOUT_NONE                         = 0,
    SPI_API_ERROR_TIMEOUT_TXFIFO_NUM_WORD              = 1,
    SPI_API_ERROR_TIMEOUT_TXFIFO_FULL                  = 2,
    SPI_API_ERROR_TIMEOUT_RXFIFO_NUM_WORD              = 3,
    SPI_API_ERROR_TIMEOUT_RXFIFO_EMPTY                 = 4,
    SPI_API_ERROR_TIMEOUT_BUS_BUSY                     = 5,
} spi_api_error_timeout_code_e;

typedef struct {
    unsigned int g_spi_error_timeout_us;                             //spi error timeout(us)(lspi/gspi),a large value is set by default,can set it by spi_set_error_timeout();
    timeout_handler_fp spi_timeout_handler;                        //spi_timeout_handler(lspi/gspi);
    volatile spi_api_error_timeout_code_e g_spi_error_timeout_code;//record spi error timeout code(lspi/gspi), can obtain the value through the spi_get_error_timeout_code() interface;
}spi_timeout_error_t;

extern spi_timeout_error_t g_spi_timeout_error[2];

#define SPI_WAIT(condition,spi_sel,g_spi_error_timeout_us,spi_timeout_handler,spi_api_error_code)                               wait_condition_fails_or_timeout_with_param(condition,(unsigned int)spi_sel,g_spi_error_timeout_us,spi_timeout_handler,(unsigned int)spi_api_error_code)

#define lspi_reset()        spi_hw_fsm_reset(LSPI_MODULE)
#define gspi_reset()        spi_hw_fsm_reset(GSPI_MODULE)
/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/*!
 * @name   External interface
 */
/**
 * @brief       This function set gspi xip config.
 * @param[in]   gspi_xip_n  - the gspi xip select.
 * @param[in]   config      - configuration struct of gspi xip.
 * @return      none.
 */
static inline void gspi_set_xip_config(gspi_xip_e gspi_xip_n ,spi_xip_config_t *config)
{
    reg_gspi_xip_rd_cmd1(gspi_xip_n) = config->spi_xip_rd_cmd1;
    reg_gspi_xip_wr_cmd1(gspi_xip_n) = config->spi_xip_wr_cmd1;
    if(config->spi_3_line_en == 1)
    {
        /*The io mode must set to single mode*/
        BM_SET(reg_spi_ctrl3(1), FLD_SPI_3LINE);
    }
    else
    {
        BM_CLR(reg_spi_ctrl3(1), FLD_SPI_3LINE);
    }
    reg_gspi_rd_config(gspi_xip_n) =  (*(unsigned int*)config);
    reg_gspi_wr_config(gspi_xip_n) =  (*(((unsigned int*)config)+1));
}

/**
 * @brief       This function set lspi xip config.
 * @param[in]   config  - configuration of lspi xip.
 * @return      none.
 */
static inline void lspi_set_xip_config(spi_xip_config_t *config)
{
    reg_lspi_xip_rd_cmd1 = config->spi_xip_rd_cmd1;
    reg_lspi_xip_wr_cmd1 = config->spi_xip_wr_cmd1;
    if(config->spi_3_line_en == 1)
    {
        /*The io mode must set to single mode*/
        BM_SET(reg_spi_ctrl3(1), FLD_SPI_3LINE);
    }
    else
    {
        BM_CLR(reg_spi_ctrl3(1), FLD_SPI_3LINE);
    }
    reg_lspi_rd_config =  (*(unsigned int*)config);
    reg_lspi_wr_config = (*(((unsigned int*)config)+1));
}

/**
 * @brief       This function set gspi xip end address.
 * @param[in]   gspi_xip_n  - the gspi xip select.
 * @param[in]   addr        - the gspi xip end address.
 * @return      none.
 *  gspi xip base address : 0x88000000
 *  xip0 space  = {0x88000000, 0x88ffff00}.
 *  xip1 space  = {0x89000000, 0x89ffff00}.
 *  xip2 space  = {0x8a000000, 0x8affff00}.
 *  xip3 space  = {0x8b000000, 0x8bffff00}.
 */
static inline void gspi_xip_end_addr_set(void)
{
    reg_gspi_xip_size = ((GSPI_XIP_64M<<6)&FLD_GSPI_XIP3_END_ADDR) | ((GSPI_XIP_48M<<4)&FLD_GSPI_XIP2_END_ADDR) | ((GSPI_XIP_32M<<2)&FLD_GSPI_XIP1_END_ADDR) | ((GSPI_XIP_16M<<0)&FLD_GSPI_XIP0_END_ADDR);
}

/**
 * @brief     This function serves to spi finite state machine reset(the configuration register is still there and does not need to be reconfigured).
 * @param[in] spi_sel - the spi module.
 * @return    none.
 */
static inline void spi_hw_fsm_reset(spi_sel_e spi_sel)
{

    if(spi_sel == LSPI_MODULE)
    {
        reg_rst0 &= (~FLD_RST0_LSPI);
        reg_rst0 |= FLD_RST0_LSPI;
    }
    else if(spi_sel == GSPI_MODULE)
    {
        reg_rst1 &= (~FLD_RST1_GSPI);
        reg_rst1 |= FLD_RST1_GSPI;
    }
    g_spi_timeout_error[spi_sel].g_spi_error_timeout_code = SPI_API_ERROR_TIMEOUT_NONE;
}

/**
 * @brief       This function get tx_fifo number.
 * @param[in]   spi_sel     - the spi module.
 * @return      tx_fifo     - byte number in tx_fifo.
 */
static inline  unsigned char spi_get_txfifo_num(spi_sel_e spi_sel)
{
   return (reg_spi_txfifo_status(spi_sel) & FLD_SPI_TXF_ENTRIES);
}

/**
 * @brief       This function get rx_fifo number.
 * @param[in]   spi_sel     - the spi module.
 * @return      rx_fifo     - byte number in rx_fifo.
 */
static inline unsigned char spi_get_rxfifo_num(spi_sel_e spi_sel)
{
   return reg_spi_rxfifo_status(spi_sel) & FLD_SPI_RXF_ENTRIES;
}

/**
 * @brief       This function set spi rx_cnt.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cnt         - rx amount of data.
 * @return      none.
 */
static inline void spi_rx_cnt(spi_sel_e spi_sel, unsigned int cnt)
{
    reg_spi_rx_cnt(spi_sel) = ((cnt - 1) & 0xffffff);
}

/**
 * @brief       This function set spi tx_cnt.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cnt         - tx amount of data.
 * @return      none.
 */
static inline void spi_tx_cnt(spi_sel_e spi_sel, unsigned int cnt)
{
    reg_spi_tx_cnt(spi_sel) = ((cnt - 1) & 0xffffff);
}

/**
 * @brief       This function clear tx_fifo.
 * @param[in]   spi_sel - the spi module.
 * @return      none.
 */
static inline void spi_tx_fifo_clr(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_status(spi_sel), FLD_SPI_TXF_CLR_LEVEL);
    BM_CLR(reg_spi_ctrl3(spi_sel), FLD_SPI_DMATX_SOF_CLRTXF_EN);
    while(FLD_SPI_TXF_CLR_LEVEL == (reg_spi_status(spi_sel) & FLD_SPI_TXF_CLR_LEVEL));
}

/**
 * @brief       This function clear rx_fifo.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_rx_fifo_clr(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_status(spi_sel), FLD_SPI_RXF_CLR_LEVEL);
    BM_CLR(reg_spi_ctrl3(spi_sel), FLD_SPI_DMARX_EOF_CLRRXF_EN);
    while(FLD_SPI_RXF_CLR_LEVEL == (reg_spi_status(spi_sel) & FLD_SPI_RXF_CLR_LEVEL));
}

/**
 * @brief       This function set spi command content and enable translate process.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - command content.
 * @return      none.
 */
static inline void spi_set_cmd(spi_sel_e spi_sel, unsigned char cmd)
{
    reg_spi_cmd(spi_sel) = cmd;
}

/**
 * @brief       This function set spi command1 content and enable translate process.
 *              The cmd1 must be configured first.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - command content.
 * @return      none.
 */
static inline void spi_set_cmd1(spi_sel_e spi_sel, unsigned char cmd)
{
    reg_spi_cmd1(spi_sel) = cmd;
}

/**
 * @brief       This function servers to enable dma trig spi.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_dma_trig_spi_en(spi_sel_e spi_sel)
{
    reg_spi_ctrl4(spi_sel) |= FLD_SPI_DMA_TRIG_SPI_EN;
}

/**
 * @brief       This function servers to disable dma trig spi.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_dma_trig_spi_dis(spi_sel_e spi_sel)
{
    reg_spi_ctrl4(spi_sel) &= ~FLD_SPI_DMA_TRIG_SPI_EN;
}

/**
 * @brief       This function servers to enable cmd which will sent a byte cmd.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_cmd_en(spi_sel_e spi_sel)
{
    BM_SET( reg_spi_ctrl1(spi_sel), FLD_SPI_CMD_EN);
}

/**
 * @brief       This function servers to disable cmd which will not sent cmd.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_cmd_dis(spi_sel_e spi_sel)
{

    BM_CLR(reg_spi_ctrl1(spi_sel), FLD_SPI_CMD_EN);
}

/**
 * @brief       This function servers to enable second cmd.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_cmd1_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_reg_ctrl0(spi_sel), FLD_SPI_CMD1_EN);
}

/**
 * @brief       This function servers to disable second cmd.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_cmd1_dis(spi_sel_e spi_sel)
{

    BM_CLR(reg_spi_reg_ctrl0(spi_sel), FLD_SPI_CMD1_EN);
}

/**
 * @brief     This function servers enable cmd format,the format of cmd phase is the same as the data phase(Dual/Quad).
 * @param[in] spi_sel   - the spi module.
 * @return    none.
 */
static inline void spi_cmd_fmt_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_ctrl1(spi_sel), FLD_SPI_CMD_FMT);
}

/**
 * @brief   This function servers disable cmd format,the format of cmd phase is single.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_cmd_fmt_dis(spi_sel_e spi_sel)
{
    BM_CLR(reg_spi_ctrl1(spi_sel), FLD_SPI_CMD_FMT);
}

/**
 * @brief   This function servers enable token mode.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_token_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_reg_ctrl0(spi_sel), FLD_SPI_TOKEN_EN);
}

/**
 * @brief   This function servers disable token mode.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_token_dis(spi_sel_e spi_sel)
{
    BM_CLR(reg_spi_reg_ctrl0(spi_sel), FLD_SPI_TOKEN_EN);
}

/**
 * @brief   This function servers setting token value.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   token_val   - the token value.
 * @return  none.
 */
static inline void spi_set_token_val(spi_sel_e spi_sel, spi_token_val_e token_val)
{
    reg_spi_reg_ctrl0(spi_sel) = ((reg_spi_reg_ctrl0(spi_sel) & (~FLD_SPI_TOKEN_VAL_SEL)) | ((token_val & 0x1) << 1));
}

/**
 * @brief       This function servers to reply master slave is ready .
 * When slave_ready is set to 1, after the master sends a read status command to the slave,
 * the slave will automatically return to 0x5a by the hardware.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_slave_ready_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_status(spi_sel), FLD_SPI_SET_SLAVE_READY);
}

/**
 * @brief       This function servers to reply master slave is not ready.
 * When slave_ready is set to 0, after the master sends a read status command to the slave,
 * the slave will automatically return to 0x00 by the hardware.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_slave_ready_dis(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_status(spi_sel), FLD_SPI_CLR_SLAVE_READY);
}

/**
 * @brief       This function servers to read cmd transferred from master for slave.
 * @param[in]   spi_sel     - the spi module.
 * @return      cmd transferred from by master.
 */
static inline unsigned char spi_slave_get_cmd(spi_sel_e spi_sel)
{
    return reg_spi_cmd(spi_sel);
}

/**
 * @brief       This function servers to enable spi 3line mode.
 * @param[in]   spi_sel - the spi module.
 * @return      none.
 */
static inline void spi_3line_mode_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_ctrl3(spi_sel), FLD_SPI_3LINE);
}

/**
 * @brief       This function servers to disable spi 3line mode.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_3line_mode_dis(spi_sel_e spi_sel)
{
    BM_CLR(reg_spi_ctrl3(spi_sel), FLD_SPI_3LINE);
}

/**
 * @brief   This function to enable address format.the format of address phase is the same as the data phase(Dual/Quad).
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_addr_fmt_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_ctrl1(spi_sel), FLD_SPI_ADDR_FMT);

}

/**
 * @brief   This function to disable address format.the format of address phase is single.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_addr_fmt_dis(spi_sel_e spi_sel)
{
    BM_CLR(reg_spi_ctrl1(spi_sel), FLD_SPI_ADDR_FMT);

}

/**
 * @brief       This function servers to set slave address.
 * @param[in]   addr - address of slave.
 * @return      none.
 */
static inline void spi_set_address(spi_sel_e spi_sel,unsigned int addr)
{
    reg_spi_addr_word(spi_sel) = addr;
}

/**
 * @brief   This function enable address phase.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_addr_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_ctrl1(spi_sel), FLD_SPI_ADDR_EN);
}

/**
 * @brief   This function disable address phase.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_addr_dis(spi_sel_e spi_sel)
{
    BM_CLR(reg_spi_ctrl1(spi_sel), FLD_SPI_ADDR_EN);
}

/**
 * @brief       This function servers to set spi address length.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   len         - byte number of address.
 * @return      none.
 */
static inline void spi_set_addr_len(spi_sel_e spi_sel, unsigned char len)
{
    (reg_spi_ctrl1(spi_sel)) = ((reg_spi_ctrl1(spi_sel) & (~FLD_SPI_ADDR_LEN)) | (((len - 1) & 0x3) << 2));
}

/**
 * @brief       This function servers to determine whether the bus is busy.
 * @param[in]   spi_sel     - the spi module.
 * @return      1:Indicates that the bus is busy. 0:Indicates that the bus is free.
 */
static inline bool spi_is_busy(spi_sel_e spi_sel)
{
    return  reg_spi_status(spi_sel) & FLD_SPI_BUSY;

}

/**
 * @brief       This function servers to enable spi tx dma function.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_tx_dma_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_ctrl0(spi_sel), FLD_SPI_TX_DMA_EN);
}

/**
 * @brief       This function servers to disable spi tx dma function.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_tx_dma_dis(spi_sel_e spi_sel)
{
    BM_CLR(reg_spi_ctrl0(spi_sel), FLD_SPI_TX_DMA_EN);
}

/**
 * @brief       This function servers to enable spi rx dma function.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_rx_dma_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_ctrl0(spi_sel), FLD_SPI_RX_DMA_EN);
}

/**
 * @brief       This function servers to disable spi rx dma function.
 * @param[in]   spi_sel     - the spi module.
 * @return      none.
 */
static inline void spi_rx_dma_dis(spi_sel_e spi_sel)
{
    BM_CLR(reg_spi_ctrl0(spi_sel), FLD_SPI_RX_DMA_EN);
}

/**
 * @brief       This function servers to set xip address offset.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   addr_offset - the offset of data.
 * @return      none.
 */
static inline void spi_xip_addr_offset(spi_sel_e spi_sel, unsigned int addr_offset)
{
    reg_spi_xip_addr_offset(spi_sel) = (addr_offset >> 24) & 0xff;
}

/**
 * @brief   This function servers to enable xip.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_xip_en(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_ctrl4(spi_sel), FLD_SPI_XIP_ENABLE);
}

/**
 * @brief   This function servers to disable xip.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_xip_dis(spi_sel_e spi_sel)
{
    BM_CLR(reg_spi_ctrl4(spi_sel), FLD_SPI_XIP_ENABLE);
}

/**
 * @brief   This function servers to stop xip.
 * @param[in]   spi_sel     - the spi module.
 * @return  none.
 */
static inline void spi_xip_stop(spi_sel_e spi_sel)
{
    BM_SET(reg_spi_ctrl4(spi_sel), FLD_SPI_XIP_STOP);
}

/**
 * @brief       This function servers to set xip timeout cnt,when two data frame intervals exceed hclk_period*timeout_cnt,csn will set high level.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cnt         - xip timeout cnt.
 * @return      none.
 */
static inline void spi_xip_timeout_cnt(spi_sel_e spi_sel, unsigned char cnt)
{
    reg_spi_xip_timeout_cnt(spi_sel) = cnt;
}

/**
 * @brief       This function servers to set page_size.
 * @param[in]   spi_sel         - the spi module.
 * @param[in]   page_size_i     - page boundary 2^page_size_i bytes.
 * @return      none.
 */
static inline void spi_xip_page_size(spi_sel_e spi_sel, unsigned char page_size_i)
{
    reg_spi_xip_page_size(spi_sel) = page_size_i;
}

/**
 * @brief       This function servers to set the number of bytes to triggered the receive interrupt.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   cnt     - the interrupt trigger level.
 * @return      none.
 */
static inline void spi_rx_irq_trig_cnt(spi_sel_e spi_sel, unsigned char cnt)
{
    reg_spi_rxfifo_thres(spi_sel) = ((reg_spi_rxfifo_thres(spi_sel) & (~FLD_SPI_RXF_THRES)) | (cnt & FLD_SPI_RXF_THRES));
}

/**
 * @brief       This function servers to set the number of bytes to triggered the transmit interrupt.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   cnt     - the interrupt trigger level.
 * @return      none.
 */
static inline void spi_tx_irq_trig_cnt(spi_sel_e spi_sel, unsigned char cnt)
{
    reg_spi_txfifo_thres(spi_sel) = ((reg_spi_txfifo_thres(spi_sel) & (~FLD_SPI_TXF_THRES)) | (cnt & FLD_SPI_TXF_THRES));
}

/**
 * @brief       This function servers to get irq status.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   status  - the irq status.
 * @return    - the value of status is be set.
 */
static inline unsigned char spi_get_irq_status(spi_sel_e spi_sel,spi_irq_status_e status)
{
    return reg_spi_int_status0(spi_sel)&status;
}

/**
 * @brief       This function servers to clear irq status.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   status  - the irq status.
 * @return      none.
 */
static inline void spi_clr_irq_status(spi_sel_e spi_sel, spi_irq_status_e status)
{
    reg_spi_int_status0(spi_sel) = status;
}

/**
 * @brief       This function servers to set irq mask.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mask    - the irq mask.
 * @return      none.
 */
static inline void spi_set_irq_mask(spi_sel_e spi_sel, spi_irq_mask mask)
{
   BM_SET(reg_spi_ctrl0(spi_sel), mask);
}

/**
 * @brief       This function servers to get irq mask.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mask    - the irq mask.
 * @return      irq mask.
 */
static inline unsigned char spi_get_irq_mask(spi_sel_e spi_sel, spi_irq_mask mask)
{
    return reg_spi_ctrl0(spi_sel)&mask;
}

/**
 * @brief       This function servers to clear irq mask.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mask    - the irq mask.
 * @return      none.
 */
static inline void spi_clr_irq_mask(spi_sel_e spi_sel, spi_irq_mask mask)
{
    BM_CLR(reg_spi_ctrl0(spi_sel), mask);
}

/**
 * @brief       This function servers to enable single color mode, lspi would send the color in lut1 repeatedly.
 * @return      none.
 */
static inline void lspi_lcd_single_color_mode_en(void)
{
    BM_SET(reg_lspi_lcd_ctrl2, FLD_LSPI_LCD_SINGLE_COLOR_MODE);
}

/**
 * @brief       This function servers to disable single color mode.
 * @return      none.
 */
static inline void lspi_lcd_single_color_mode_dis(void)
{
    BM_CLR(reg_lspi_lcd_ctrl2, FLD_LSPI_LCD_SINGLE_COLOR_MODE);
}

/**
 * @brief       This function servers to set color send method as little endian mode, default is big little mode.
 * @return      none.
 */
static inline void lspi_lcd_rgb_little_endian_mode(void)
{
    BM_CLR(reg_lspi_lcd_ctrl2, FLD_LSPI_LCD_RGB_BIG_ENDIAN_MODE);
}

/**
 * @brief       This function servers to set color send method as big endian mode, default is little mode, only ramless lcd scan mode support big endian mode.
 * @return      none.
 */
static inline void lspi_lcd_rgb_big_endian_mode(void)
{
    BM_SET(reg_lspi_lcd_ctrl2, FLD_LSPI_LCD_RGB_BIG_ENDIAN_MODE);
}

/**
 * @brief       This function servers to enable ram 4bit mode, lspi map 4bit to lut table, and send the color in lut table.
 * @return      none.
 */
static inline void lspi_lcd_ram_4bit_mode_en(void)
{
    BM_SET(reg_lspi_lcd_ctrl2, FLD_LSPI_LCD_RAM_4BIT_MODE);
}

/**
 * @brief       This function servers to disable ram 4bit mode.
 * @return      none.
 */
static inline void lspi_lcd_ram_4bit_mode_dis(void)
{
    BM_CLR(reg_lspi_lcd_ctrl2, FLD_LSPI_LCD_RAM_4BIT_MODE);
}

/**
 * @brief       This function servers to set lspi lcd interrupt mask.
 * @param[in]   mask    - the lcd irq mask.
 * @return      none.
 */
static inline void lspi_lcd_set_irq_mask(lspi_lcd_irq_mask lcd_irq_mask)
{
    BM_SET(reg_lspi_lcd_ctrl2, lcd_irq_mask);
}

/**
 * @brief       This function servers to get lspi lcd interrupt mask.
 * @param[in]   mask    - the lcd irq mask.
 * @return      none.
 */
static inline unsigned char lspi_lcd_get_irq_mask(lspi_lcd_irq_mask lcd_irq_mask)
{
    return reg_lspi_lcd_ctrl2&lcd_irq_mask;
}

/**
 * @brief       This function servers to get lspi lcd interrupt status.
 * @param[in]   status - the lspi lcd irq status.
 * @return      none.
 */
static inline unsigned char lspi_lcd_get_irq_status(lspi_lcd_irq_status_e status)
{
    return reg_lspi_int_status1&status;
}

/**
 * @brief       This function servers to clear lspi lcd interrupt status.
 * @param[in]   status - the lspi lcd irq status.
 * @return      none.
 */
static inline void lspi_lcd_clr_irq_status(lspi_lcd_irq_status_e status)
{
    reg_lspi_int_status1 = status;
}

/**
 * @brief       This function servers to set lspi lcd 3line (9bit)data first bit(dcx) as cmd(0).
 * @return      none.
 */
static inline void lspi_set_lcd_dcx_cmd(void)
{
    BM_CLR(reg_lspi_lcd_ctrl, FLD_LSPI_LCD_DCX);
}

/**
 * @brief       This function servers to set lspi lcd 3line (9bit)data first bit(dcx) as data(1).
 * @return      none.
 */
static inline void lspi_set_lcd_dcx_data(void)
{
    BM_SET(reg_lspi_lcd_ctrl, FLD_LSPI_LCD_DCX);
}

/**
 * @brief       This function servers to enable lspi lcd 3line dcx mode.
 * @return      none.
 */
static inline void lspi_lcd_dcx_en(void)
{
    BM_SET(reg_lspi_lcd_ctrl, FLD_LSPI_LCD_3LINE_DCX_EN);
}

/**
 * @brief       This function servers to disable lspi lcd 3line dcx mode.
 * @return      none.
 */
static inline void lspi_lcd_dcx_dis(void)
{
    BM_CLR(reg_lspi_lcd_ctrl, FLD_LSPI_LCD_3LINE_DCX_EN);
}

/**
 * @brief       This function servers to enable lspi lcd 2lane mode.
 * @return      none.
 */
static inline void lspi_lcd_2lane_en(void)
{
    BM_SET(reg_lspi_lcd_ctrl, FLD_LSPI_LCD_2LANE_EN);
}

/**
 * @brief       This function servers to disable lspi lcd 2lane mode.
 * @return      none.
 */
static inline void lspi_lcd_2lane_dis(void)
{
    BM_CLR(reg_lspi_lcd_ctrl, FLD_LSPI_LCD_2LANE_EN);
}

/**
 * @brief       This function servers to set lspi lcd RGB mode.
 * @param[in]   rgb_mode    - lcd rgb mode.
 * @return      none.
 */
static inline void lspi_lcd_rgb_mode(lspi_lcd_rgb_mode_e rgb_mode)
{
    reg_lspi_lcd_ctrl = ((reg_lspi_lcd_ctrl & (~FLD_LSPI_LCD_RGB_MODE)) | ((rgb_mode & 0x3) << 1));
}

/**
 * @brief       This function servers to enable lspi lcd scan. Hardware automatic repeat scan lcd.
 * @return      none.
 */
static inline void lspi_lcd_scan_en(void)
{
    BM_SET(reg_lspi_lcd_ctrl, FLD_LSPI_LCD_SCAN_EN);
}

/**
 * @brief       This function servers to disable lspi lcd scan.
 * @return      none.
 */
static inline void lspi_lcd_scan_dis(void)
{
    BM_CLR(reg_lspi_lcd_ctrl, FLD_LSPI_LCD_SCAN_EN);
}

/**
 * @brief       This function servers to set lspi lcd back porch line number.
 * @param[in]   vbp_line_cnt    - vbp line cnt.
 * @return      none.
 */
static inline void lspi_lcd_vbp_line_cnt(unsigned char vbp_line_cnt)
{
    reg_lspi_lcd_vbp_line_cnt = vbp_line_cnt;
}

/**
 * @brief       This function servers to set lspi lcd front porch line number.
 * @param[in]   vfp_line_cnt    - vfp line cnt.
 * @return      none.
 */
static inline void lspi_lcd_vfp_line_cnt(unsigned char vfp_line_cnt)
{
    reg_lspi_lcd_vfp_line_cnt = vfp_line_cnt;
}

/**
 * @brief       This function servers to set lspi lcd line interrupt trigger level.
 * @param[in]   lcd_line_trig_lvl   - lcd line trigger level.
 * @return      none.
 */
static inline void lspi_lcd_line_trig_lvl(unsigned short lcd_line_trig_lvl)
{
    reg_lspi_lcd_line_trig_lvl = lcd_line_trig_lvl>>2;
}

/**
 * @brief       This function servers to set lspi lcd background image address.
 * @param[in]   bimage_start_addr   - background start address.
 * @return      none.
 */
static inline void lspi_lcd_bimage_start_addr(unsigned int bimage_start_addr)
{
    reg_lspi_lcd_bimage_start_addr32 = bimage_start_addr;
}

/**
 * @brief       This function servers to set lspi lcd front ground image address.
 * @param[in]   fimage_start_addr   - front ground image start address.
 * @return      none.
 */
static inline void lspi_lcd_fimage_start_addr(unsigned int fimage_start_addr)
{
    reg_lspi_lcd_fimage_start_addr32 = fimage_start_addr;
}

/**
 * @brief       This function servers to set lspi lcd display porch min line time, the unit is spi clk.
 * @param[in]   display_line_time   - lcd display porch min line time.
 * @return      none.
 */
static inline void lspi_lcd_display_min_line_time(unsigned short display_line_time)
{
    reg_lspi_lcd_display_line_time = display_line_time;
}

/**
 * @brief       This function servers to set lspi lcd back \ front porch min line time, the unit is spi clk.
 * @param[in]   porch_line_time     - lcd back \ front porch min line time.
 * @return      none.
 */
static inline void lspi_lcd_porch_min_line_time(unsigned short porch_line_time)
{
    reg_lspi_lcd_porch_line_time = porch_line_time;
}

/**
 * @brief       This function servers to set lspi lcd resolution.
 * @param[in]   line_res    - line size in resolution.
 * @param[in]   row_res     - lcd back\front porch min line time.
 * @return      none.
 */
static inline void lspi_lcd_set_res(unsigned short line_res, unsigned short row_res)
{
    reg_lspi_lcd_frame_res = (FLD_LSPI_LCD_LINE_PER_FRAME & ((unsigned int) (line_res-1))<<10) | (FLD_LSPI_LCD_PIXEL_PER_LINE & (row_res - 1));
}

/**
 * @brief       This function servers to set lspi lcd 4bit color look up table. 4bit in ram is mapped to totally 16 lookup tables.
 * @param[in]   addr    - lut address.
 * @param[in]   color   - the color of the setting.
 * @return      none.
 */
static inline void lspi_lcd_set_lut(unsigned char addr, unsigned int color)
{
    reg_lspi_lcd_lut_data32(addr) = color&0xffffff;
}

/**
 * @brief       This function servers to set lspi lcd display line transmode.
 * @param[in]   mode    - lcd display line transmode.
 * @return      none.
 */
static inline void lspi_lcd_set_display_transmode(spi_tans_mode_e mode)
{
    reg_lspi_lcd_display_transmode = ((reg_lspi_lcd_display_transmode & (~FLD_LSPI_LCD_DISPLAY_TRANSMODE)) | ((mode & 0xf) << 4));
}

/**
 * @brief       This function servers to set lspi lcd porch line transmode.
 * @param[in]   mode    - lcd porch line transmode.
 * @return      none.
 */
static inline void lspi_lcd_set_porch_transmode(spi_tans_mode_e mode)
{
    reg_lspi_lcd_porch_transmode = ((reg_lspi_lcd_porch_transmode & (~FLD_LSPI_LCD_PORCH_TRANSMODE)) | ((mode & 0xf) << 4));
}

/**
 * @brief     This function set DMA spi tx llp sof mode.
 * @param[in] chn       - dma channel.
 * @param[in] sof_mode  - dma llp sof mode.
 * @return    none.
 */
static inline void spi_set_llp_sof_mode(dma_chn_e chn,unsigned char sof_mode)
{
    if(sof_mode){
        reg_dma_ctrl_llp |= BIT(chn);
    }
    else{
        reg_dma_ctrl_llp &= ~BIT(chn);
    }

}

/**
 * @brief       This function servers to set lspi lcd send cmd.
 * @return      none.
 */
static inline void lspi_lcd_set_cmd(unsigned char cmd)
{
    reg_lspi_lcd_cmd = cmd;
}

/**
 * @brief       This function servers to set the minimum time between the edge of SPI_CS and  the edges of SPI_CLK
 * @param[in]   spi_sel - the spi module.
 * @param[in]   cs2sclk -  the actual duration is (SPI_CLK period*(cs2sclk+1)),cs2sclk default value is 0x01.
 * @return      none.
 */
static inline void spi_set_cs2sclk_time(spi_sel_e spi_sel,unsigned char cs2sclk){
    reg_spi_timing(spi_sel) =  (reg_spi_timing(spi_sel) & ~FLD_SPI_CS2SCLK)|(cs2sclk);
}

/**
 * @brief       This function servers to set the minimum time that spi cs should stay high,
 *              the minimum time needs to be met from the cs pulling up after the current packet of data is sent to the cs pulling down after the next packet of data is sent,
 *              if pull down the cs before the minimum time is reached, the hardware will wait until the time meet to pull down the cs for the next data operation.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   csht -  the actual duration is (SPI_CLK period*(csht+1)),csht default value is 0x01.
 * @return      none.
 * @note        this interface is currently used for lspi lcd function (there is a minimum limit for cs to maintain high level),
 *              normal spi peripheral communication does not need to call this interface.
 */
static inline void spi_set_cs_high_time(spi_sel_e spi_sel,unsigned char csht){
    reg_spi_timing(spi_sel) =  (reg_spi_timing(spi_sel) & ~FLD_SPI_CSHT)|(csht<<3);
}

/**
 * @brief       This function servers to set bit sequence
 * @param[in]   spi_sel - the spi module.
 * @param[in]   bit_seq - bit sequence(lsb or msb).
 * @return      none.
 */
static inline void spi_set_bit_sequence(spi_sel_e spi_sel,spi_bit_seq_e bit_seq){
    reg_spi_ctrl3(spi_sel) =  reg_spi_ctrl3(spi_sel)|MASK_VAL(FLD_SPI_LSB,bit_seq);
}

/**
 * @brief     This function serves to record the api status.
 * @param[in] spi_error_timeout_code - spi_api_error_timeout_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario.
 */
__attribute__((weak)) void lspi_timeout_handler(unsigned int spi_error_timeout_code);
__attribute__((weak)) void gspi_timeout_handler(unsigned int spi_error_timeout_code);

/**
 * @brief     This function serves to set the spi timeout(us).
 * @param[in] spi_sel    - the spi module.
 * @param[in] timeout_us - the timeout(us).
 * @return    none.
 * @note      The default timeout (g_spi_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
 *            g_spi_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
 *            The minimum time for g_spi_error_timeout_us is related to the following factors:
 *            1.spi clock rate;
 *            2.cclk clock rate;
 *            3.spi's data line(SPI_SINGLE_MODE, SPI_DUAL_MODE, SPI_QUAD_MODE);
 *            4.maximum interrupt processing time;
 *            5.spi write/read data length (especially SPI_WAIT_BUSY);
 *            Solution in case of timeout exit:
 *            when timeout exits, solution:
 *            reset SPI(as master or slave) module,corresponding api:spi_hw_fsm_reset;
 */
void spi_set_error_timeout(spi_sel_e spi_sel,unsigned int timeout_us);

/**
 * @brief     This function serves to return the spi api error timeout code.
 * @param[in] spi_sel    - the spi module.
 * @return    none.
 */
spi_api_error_timeout_code_e spi_get_error_timeout_code(spi_sel_e spi_sel);

/**
 * @brief      This function selects  pin  for gspi master or slave mode.
 * @param[in]  pin       - the selected pin.
 * @param[in]  function  - pin-multiplexing function.
 * @return     none.
 */
void gspi_set_pin_mux(gpio_func_pin_e pin,gpio_func_e function);

/**
 * @brief       This function enable gspi csn pin.
 * @param[in]   pin - the csn pin.
 * @return      none.
 */
void gspi_cs_pin_en(gpio_pin_e pin);

/**
 * @brief       This function disable gspi csn pin.
 * @param[in]   pin - the csn pin.
 * @return      none.
 */
void gspi_cs_pin_dis(gpio_pin_e pin);

/**
 * @brief       This function change gspi csn pin.
 * @param[in]   current_csn_pin - the current csn pin.
 * @param[in]   next_csn_pin - the next csn pin.
 * @return      none.
 */
void gspi_change_csn_pin(gpio_pin_e current_csn_pin,gpio_pin_e next_csn_pin);

/**
 * @brief      This function selects  pin  for lspi master or slave mode.
 * @param[in]  pin       - the selected pin.
 * @param[in]  function  - pin-multiplexing function.
 * @return     none.
 */
void lspi_set_pin_mux(gpio_func_pin_e pin,gpio_func_e function);

/**
 * @brief       This function servers to set gspi pin.
 * @param[in]   gspi_pin_config - the pointer of pin config struct.
 * @return      none.
 */
void gspi_set_pin(gspi_pin_config_t *spi_pin_config);

/**
 * @brief   This function servers to set sspi pin.
 * @param[in]   spi_pin_config - the pointer of pin config struct.
 * @return  none.
 */
void spi_slave_set_pin(sspi_pin_config_t *spi_pin_config);

/**
 * @brief       This function configures the clock and working mode for SPI interface.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   div_clock   - the division factor for SPI module.
 *              spi_clock_out = pll_clk / div_clock
 * @param[in]    mode       - the selected working mode of SPI module.
 *               bit2: CPHA-SPI_CLK Phase,bit3: CPOL-SPI_CLK Polarity
 *              MODE0:  CPOL = 0 , CPHA = 0;
 *              MODE1:  CPOL = 0 , CPHA = 1;
 *              MODE2:  CPOL = 1 , CPHA = 0;
 *              MODE3:  CPOL = 1 , CPHA = 1;
 * @return      none.
 * @note        When using the spi master module, this interface must be configured first, otherwise the following interfaces will not take effect.
 */
void spi_master_init(spi_sel_e spi_sel,unsigned short div_clock, spi_mode_type_e mode);

/**
 * @brief       This function configures the clock and working mode for SPI interface.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mode    - the selected working mode of SPI module.
 *               bit2: CPHA-SPI_CLK Phase,bit3: CPOL-SPI_CLK Polarity
 *              MODE0:  CPOL = 0 , CPHA = 0;
 *              MODE1:  CPOL = 0 , CPHA = 1;
 *              MODE2:  CPOL = 1 , CPHA = 0;
 *              MODE3:  CPOL = 1 , CPHA = 1;
 * @return      none.
 * @note       - When using the spi slave module, this interface must be configured first, otherwise the following interfaces will not take effect.
 *             - spi_clock_in(output from master clk_pin )  <= slave clk/4
 */
void spi_slave_init(spi_sel_e spi_sel,spi_mode_type_e mode);

/**
 * @brief       This function servers to set dummy cycle cnt.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   dummy_cnt   - the cnt of dummy clock.
 * @return      none.
 */
void spi_set_dummy_cnt(spi_sel_e spi_sel, unsigned char dummy_cnt);

/**
 * @brief       This function servers to set spi transfer mode.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mode    - transfer mode.
 * @return      none.
 */
void spi_set_transmode(spi_sel_e spi_sel, spi_tans_mode_e mode);

/**
 * @brief       This function servers to set spi io  mode.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mode    - single/dual/quad/3line.
 * @return      none.
  */
void spi_set_io_mode(spi_sel_e spi_sel, spi_io_mode_e mode);

/**
 * @brief       This function servers to config normal mode.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mode    - nomal ,mode 3line.
 * @return      none.
 */
void spi_master_config(spi_sel_e spi_sel, spi_normal_3line_mode_e mode);

/**
 * @brief       This function servers to config spi special mode.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   config  - the pointer of pin special config struct.
 * @return      none.
 */
void spi_master_config_plus(spi_sel_e spi_sel,spi_wr_rd_config_t *config);

/**
 * @brief       This function servers to set gspi pin for XIP.
 * @param[in]   gspi_xip_pin_config - the pointer of pin config struct.
 * @return      none.
 */
void gspi_set_xip_pin(gspi_xip_pin_config_t *gspi_xip_pin_config);

/**
 * @brief       This function servers to set lspi pin.
 * @param[in]   spi_pin_config - the pointer of pin config struct.
 * @return      none.
 */
void lspi_set_pin(lspi_pin_config_t *spi_pin_config);

/**
 * @brief      This function selects  pin  for sspi master or slave mode.
 * @param[in]  pin       - the selected pin.
 * @param[in]  function  - pin-multiplexing function.
 * @return     none.
 */
void sspi_set_pin_mux(gpio_func_pin_e pin,gpio_func_e function);

/**
 * @brief       This function servers to send command by spi.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   cmd - command.
 * @return      DRV_API_SUCCESS:command sent successfully. others:fail to send command.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_send_cmd(spi_sel_e spi_sel, unsigned char cmd);

/**
 * @brief       This function servers to write spi fifo.
 * tx_fifo_depth are fixed sizes.
 * lspi with txfifo deepth = 20 bytes.
 * gspi with txfifo deepth = 8 bytes.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   data    - the pointer to the data for write.
 * @param[in]   len     - write length.
 * @return      DRV_API_SUCCESS:data write to txfifo successfully. others:data write to txfifo failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 *              Master timeout will only be affected by their own factors, slave timeout will not only be affected by their own factors,
                will also be affected by the master side of the influence.
 */
drv_api_status_e spi_write(spi_sel_e spi_sel, unsigned char *data, unsigned int len);

/**
 * @brief       This function servers to read spi fifo.
 * rx_fifo_depth are fixed sizes.
 * lspi with rxfifo deepth = 12 bytes.
 * gspi with rxfifo deepth = 8 bytes.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   data    - the pointer to the data for read.
 * @param[in]   len     - write length.
 * @return      DRV_API_SUCCESS:data fetched from rxfifo successfully. others:data fetched from rxfifo failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 *              Master timeout will only be affected by their own factors, slave timeout will not only be affected by their own factors,
                will also be affected by the master side of the influence.
 */
drv_api_status_e spi_read(spi_sel_e spi_sel, unsigned char *data, unsigned int len);

/**
 * @brief       This function serves to normal write data in normal.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   data    - the pointer to the data for write.
 * @param[in]   len     - write length.
 * @return      DRV_API_SUCCESS:master write data successfully. others:master write data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write(spi_sel_e spi_sel, unsigned char *data, unsigned int len);

/**
 * @brief       This function serves to normal write and read data.
 * This interface cannot be used for full duplex.
 * rd_len shouldn't set to 0. Must write first, then read.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   wr_data - the pointer to the data for write.
 * @param[in]   wr_len  - write length.
 * @param[in]   rd_data - the pointer to the data for read.
 * @param[in]   rd_len  - read length.
 * @return      DRV_API_SUCCESS:master write read data successfully. others:master write read data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write_read(spi_sel_e spi_sel, unsigned char *wr_data, unsigned int wr_len, unsigned char *rd_data, unsigned int rd_len);

/**
 * @brief       This function serves to single/dual/quad write to the SPI slave.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   data        -  pointer to the data need to write.
 * @param[in]   data_len    - length in byte of the data need to write.
 * @param[in]   wr_mode     - write mode.dummy or not dummy.
 * @return      DRV_API_SUCCESS:master write data successfully. others:master write data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode);

/**
 * @brief       This function serves to normal write data repeatedly.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   data        - the pointer to the data for write.
 * @param[in]   len         - write length.
 * @param[in]   repeat_time - number of times to write data repeatedly.
 * @return      DRV_API_SUCCESS:master write data successfully. others:master write data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write_repeat(spi_sel_e spi_sel, unsigned char *data, unsigned int len, unsigned int repeat_time);

/**
 * @brief       This function serves to single/dual/quad write data to the SPI slave repeatedly.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   data        - pointer to the data need to write.
 * @param[in]   data_len    - length in byte of the data need to write.
 * @param[in]   wr_mode     - write mode.dummy or not dummy.
 * @param[in]   repeat_time - number of times to write data repeatedly.
 * @return      DRV_API_SUCCESS:master write data successfully. others:master write data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 * @attention   Only data would be written repeatedly. the typical sending order is cmd + address + data * repeat_time,
 *              cmd and address would not be repeated.
 */
drv_api_status_e spi_master_write_repeat_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, unsigned int repeat_time, spi_wr_tans_mode_e wr_mode);

/**
 * @brief       This function serves to single/dual/quad  read from the SPI slave.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   data        - pointer to the data need to read.
 * @param[in]   data_len    - the length of data.
 * @param[in]   rd_mode     - read mode.dummy or not dummy.
 * @return      DRV_API_SUCCESS:master read data successfully. others:master read data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_read_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_rd_tans_mode_e rd_mode);

/**
 * @brief       This function serves to write address, then read data from the SPI slave.
 * This interface cannot be used for full duplex.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addrs       - pointer to the address of slave.
 * @param[in]   addr_len    - the length of address.
 * @param[in]   data        - the pointer to the data for read.
 * @param[in]   data_len    - read length.
 * @param[in]   wr_mode     - write mode.dummy or not dummy.
 * @return      DRV_API_SUCCESS:master write read data successfully, others:master write read data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write_read_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned char *addrs, unsigned int addr_len, unsigned char *data, unsigned int data_len, spi_rd_tans_mode_e wr_mode);

/**
 * @brief       This function serves to set tx_dma channel and config dma tx default.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   chn     - dma channel.
 * @return      none.
 */
void spi_set_tx_dma_config(spi_sel_e spi_sel, dma_chn_e chn);

/**
 * @brief       This function serves to set tx_dma channel and config dma tx default.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   chn     - dma channel.
 * @return      none.
 */
void spi_set_master_rx_dma_config(spi_sel_e spi_sel,dma_chn_e chn);

/**
 * @brief       This function serves to set rx_dma channel and config dma rx default for spi slave.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   chn     - dma channel.
 * @return      none.
 */
void spi_set_slave_rx_dma_config(spi_sel_e spi_sel,dma_chn_e chn);


/**
 * @brief       this  function set spi tx dma channel.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   src_addr    - the address of source.
 * @param[in]   len         - the length of data.
 * @return      none.
 * @note        src_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception
 *
 */
void spi_set_tx_dma(spi_sel_e spi_sel, unsigned char* src_addr,unsigned int len);

/**
 * @brief       this  function set spi rx dma channel.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   dst_addr    - the address of destination.
 * @param[in]   len         - the length of data.
 * @return      none.
 * @note
               -# If write_num is enabled, the length of data received by DMA will be written into the first four bytes of the address.
               -# A length greater than XX_len itself and a multiple of 4 is denoted as CEILING(XX_len,4). For example: XX_len=3 ,CEILING(XX_len,4)=4; XX_len=21 ,CEILING(Tx_length, 4)=24.
                  The actual length sent by master  is denoted as Tx_len, The length (param[in]-len) of the interface configuration is denoted as Rx_len.
                  when CEILING(Tx_len,4) > CEILING(Rx_len,4), When the length of the DMA carry reaches Rx_len, the DMA will not stop working and the excess data will not be carried into the buff.
                  for example:Tx_len=21,Rx_len=20,When the DMA stops working the buff is written with a length of 21 and only 20 bytes of data are stored.It is recommended to configure the appropriate Rx_len to avoid this situation.
               -# After DMA transfer completion, the interface needs to be invoked again to read the next batch of data.
 */
void spi_set_rx_dma(spi_sel_e spi_sel, unsigned char* dst_addr,unsigned int len);

/**
 * @brief       This function serves to normal write data by dma.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   src_addr    - the pointer to the data for write.
 * @param[in]   len         - write length.
 * @return      none.
 * @note        src_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_write_dma(spi_sel_e spi_sel, unsigned char *src_addr, unsigned int len);

/**
 * @brief       This function serves to normal write cmd and address, then read data by dma.
 * The interface does not support full-duplex communication.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   addr        - the address of slave.
 * @param[in]   addr_len    - write length.
 * @param[in]   data        - the pointer to the data for read.
 * @param[in]   data_len    - read length.
 * @return      none.
 * @note        addr/data : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_write_read_dma(spi_sel_e spi_sel, unsigned char *addr, unsigned int addr_len, unsigned char *data, unsigned int data_len);

/**
 * @brief       This function serves to single/dual/quad  write to the SPI slave by dma.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   data        - pointer to the data need to write.
 * @param[in]   data_len    - length in byte of the data need to write.
 * @param[in]   wr_mode     - write mode.dummy or not dummy.
 * @return      none.
 * @note        data : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_write_dma_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode);

/**
 * @brief       This function serves to single/dual/quad  read from the SPI slave by dma.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   dst_addr    - pointer to the buffer that will cache the reading out data.
 * @param[in]   data_len    - length in byte of the data need to read.
 * @param[in]   rd_mode     - read mode.dummy or not dummy.
 * @return      none.
 * @note        dst_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_read_dma_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *dst_addr, unsigned int data_len, spi_rd_tans_mode_e rd_mode);

/**
 * @brief       This function serves to single/dual/quad write address and read from the SPI slave by dma.
 * This interface could be used for full duplex.
 * When this interface is used for full-duplex communication, it can only be used on the master side.
 * 1.the spi_master_config() interface must be called first to disable hardware cmd and hardware address
 * 2.must cmd is 0,addr_len is equal to rd_len,rd_mode is SPI_MODE_WRITE_AND_READ.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   addr_len    - the length of address.
 * @param[in]   rd_data     - pointer to the buffer that will cache the reading out data.
 * @param[in]   rd_len      - length in byte of the data need to read.
 * @param[in]   rd_mode     - read mode.dummy or not dummy.
 * @return      none.
 * @note        addr/rd_data : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_write_read_dma_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned char *addr, unsigned int addr_len, unsigned char *rd_data, unsigned int rd_len, spi_rd_tans_mode_e rd_mode);

/**
 * @brief       This function serves to write and read data simultaneously.
 * This interface can only be used by the master.
 * When initializing the master, call the spi_master_config() interface to disable the hardware cmd and hardware address,
 * and then start sending and receiving data.
 * The default chunk size sent and received by this interface is 8byte.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   write_data  - write data pointer.
 * @param[in]   read_data   - read data pointer.
 * @param[in]   len         - write/read length.
 * @return      none.
 */
void spi_master_write_read_full_duplex(spi_sel_e spi_sel,unsigned char *write_data, unsigned char *read_data, unsigned int len);

/**
 * @brief       This function serves to read data in normal.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   data        - read data pointer.
 * @param[in]   len         - read length.
 * @return      none.
 */
void spi_master_read(spi_sel_e spi_sel, unsigned char *data, unsigned int len);

/**
 * @brief       This function serves to set master rx dma burst size
 * @param[in]   spi_sel      - the spi module.
 * @param[in]   burst_size   - dma burst size.
 * @return      none.
 * @note        - lspi tx dma support burst4/burst2/burst1,gpsi tx dma only support burst1.
 *              - If the set burst size is larger than burst1 (burst2/burst4), the length of the dma transfer must be a multiple of the corresponding burst size,e.g., burst size= burst2,  dma transfer length must be a multiple of 8 bytes.
 *              - Must be configured after spi_set_tx_dma_config().
 */
void spi_set_dma_tx_burst(spi_sel_e spi_sel, dma_burst_size_e  burst_size);

/**
 * @brief       This function serves to set master rx dma burst size
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   burst_size   - dma burst size
 * @return      none.
 * @note        - lspi rx dma support burst2/burst1,gpsi rx dma only support burst1.
 *              - If the set burst size is larger than burst1 (burst2), the length of the dma transfer must be a multiple of the corresponding burst size,e.g., burst size= burst2,  dma transfer length must be a multiple of 8 bytes.
 *              - Must be configured after spi_set_master_rx_dma_config().
 */
void spi_set_dma_rx_burst(spi_sel_e spi_sel, dma_burst_size_e  burst_size);

/**
  * @brief     This function serves to config slave rx_dma channel llp.
  * @param[in] spi_sel  - the spi module.
  * @param[in] chn          - dma channel.
  * @param[in] dst_addr     - the dma address of destination.
  * @param[in] head_of_list - the head address of dma llp.
  * @return    none.
  */
void spi_set_slave_rx_dma_chain_llp(spi_sel_e spi_sel, dma_chn_e chn,unsigned char * dst_addr, unsigned int data_len,dma_chain_config_t *head_of_list);

/**
 * @brief     This function serves to set rx dma chain transfer
 * @param[in] spi_sel   - the spi module.
 * @param[in] chn          - dma channel.
 * @param[in] config_addr  - the head of list of llp_pointer.
 * @param[in] llpointer    - the next element of llp_pointer.
 * @param[in] dst_addr     -the dma address of destination.
 * @param[in] data_len     -the length of dma size by byte.
 * @return    none.
 */
void spi_rx_dma_add_list_element(spi_sel_e spi_sel,dma_chn_e chn,dma_chain_config_t *config_addr,dma_chain_config_t *llpointer ,unsigned char *dst_addr,unsigned int data_len);

/**
 * @brief      This function serves to set spi slave rx dma chain transfer.
 * @param[in] spi_sel   - the spi module.
 * @param[in] chn       - dma channel.
 * @param[in] dst_addr  - the dma address of destination.
 * @param[in] data_len  - the length of dma size by byte.
 * @return     none.
 */
void spi_slave_rx_dma_chain_init(spi_sel_e spi_sel, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len);

 /**
  * @brief     This function serves to config slave rx_dma channel llp.
  * @param[in] spi_sel      - the spi module.
  * @param[in] chn          - dma channel.
  * @param[in] src_addr     - the dma address of source.
  * @param[in] data_len     - the length of dma rx size by byte.
  * @param[in] head_of_list - the head address of dma llp.
  * @return    none.
  */
void spi_set_slave_tx_dma_chain_llp(spi_sel_e spi_sel, dma_chn_e chn,unsigned char *src_addr,unsigned int data_len,dma_chain_config_t *head_of_list);

/**
 * @brief     This function serves to set tx dma chain transfer
 * @param[in] spi_sel       - the spi module.
 * @param[in] chn            - dma channel.
 * @param[in] config_addr    - the head of list of llp_pointer.
 * @param[in] llpointer      - the next element of llp_pointer.
 * @param[in] src_addr       - the dma address of source.
 * @param[in] data_len       - the length of dma size by byte.
 * @return    none.
 */
void spi_tx_dma_add_list_element(spi_sel_e spi_sel,dma_chn_e chn,dma_chain_config_t *config_addr,dma_chain_config_t *llpointer ,unsigned char *src_addr,unsigned int data_len);

/**
 * @brief      This function serves to set spi slave tx dma chain transfer.
 * @param[in] spi_sel   - the spi module.
 * @param[in] chn       - dma channel.
 * @param[in] dst_addr  - the dma address of destination.
 * @param[in] data_len  - the length of dma size by byte.
 * @return     none.
 */
void spi_slave_tx_dma_chain_init(spi_sel_e spi_sel, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len);
/** @} */
#endif





