/********************************************************************************************************
 * @file     register_8258.h 
 *
 * @brief    This is the header file for TLSR8258
 *
 * @author	 public@telink-semi.com;
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#pragma once


#include "bsp.h"

/*******************************************************************************************************************************
 *
 *									Digital  Register Table
 *
 ******************************************************************************************************************************/

/****************************************************
  i2c regs struct: begin  addr : 0x00
 *****************************************************/
#define reg_i2c_set				REG_ADDR32(0x00)
#define reg_i2c_speed			REG_ADDR8(0x00)
#define reg_i2c_id				REG_ADDR8(0x01)
enum{
	FLD_I2C_WRITE_READ_BIT  =  BIT(0),
	FLD_I2C_ID              =  BIT_RNG(1,7),
};

#define reg_i2c_status			REG_ADDR8(0x02)
enum{
	FLD_I2C_CMD_BUSY		= 	BIT(0),
	FLD_I2C_BUS_BUSY		= 	BIT(1),
	FLD_I2C_NAK				= 	BIT(2),
};

#define reg_i2c_mode			REG_ADDR8(0x03)
enum{
	FLD_I2C_ADDR_AUTO 		=	BIT(0),
	FLD_I2C_MODE_MASTER		= 	BIT(1),		// 1: master, 0: slave
	FLD_I2C_MEM_MAP 		=	BIT(2), 	// write i2c data to predefined memory address which set by other register
	FLD_I2C_HOLD_MASTER     =   BIT(3),  //r_clk_stretch_en in kite register table, Congqing do not know its meaning, should ask Guangjun & Mingjian
};

#define reg_i2c_adr_dat			REG_ADDR16(0x04)
#define reg_i2c_dat_ctrl		REG_ADDR32(0x04)
#define reg_i2c_di_ctrl			REG_ADDR16(0x06)
#define reg_i2c_adr				REG_ADDR8(0x04)
#define reg_i2c_do				REG_ADDR8(0x05)
#define reg_i2c_di				REG_ADDR8(0x06)
#define reg_i2c_ctrl			REG_ADDR8(0x07)
enum{
	FLD_I2C_CMD_ID			= 		BIT(0),
	FLD_I2C_CMD_ADDR		= 		BIT(1),
	FLD_I2C_CMD_DO			= 		BIT(2),
	FLD_I2C_CMD_DI			= 		BIT(3),
	FLD_I2C_CMD_START		= 		BIT(4),
	FLD_I2C_CMD_STOP		= 		BIT(5),
	FLD_I2C_CMD_READ_ID		= 		BIT(6),
	FLD_I2C_CMD_ACK			= 		BIT(7),
};

/****************************************************
i2c map regs struct: begin  addr : 0xe0
 *****************************************************/
#define reg_i2c_map_hadr		 REG_ADDR8(0xe0)   //what meaning ?

#define reg_slave_map_addrl      REG_ADDR8(0xe1)
#define reg_slave_map_addrm      REG_ADDR8(0xe2)
#define reg_slave_map_addrh      REG_ADDR8(0xe3)

#define reg_i2c_map_host_status	 	REG_ADDR8(0xe4)
enum{
	FLD_HOST_CMD_IRQ = 		BIT(0),   //both host write & read trigger this status
	FLD_HOST_READ_IRQ = 	BIT(1),   //only host read trigger this status
};


//what meaning ?
#define reg_i2c_map_read0		 REG_ADDR8(0xe5)
#define reg_i2c_map_read1		 REG_ADDR8(0xe6)
#define reg_i2c_map_read2		 REG_ADDR8(0xe7)

/****************************************************
  secondary spi regs struct: begin  addr : 0x08
 *****************************************************/
#define reg_spi_data			REG_ADDR8(0x08)
#define reg_spi_ctrl			REG_ADDR8(0x09)
enum{
	FLD_SPI_CS 				= 	BIT(0),
	FLD_SPI_MASTER_MODE_EN 	= 	BIT(1),
	FLD_SPI_DATA_OUT_DIS 	= 	BIT(2),
	FLD_SPI_RD			    = 	BIT(3),
	FLD_SPI_ADDR_AUTO       =	BIT(4),
	FLD_SPI_SHARE_MODE      =   BIT(5),
	FLD_SPI_BUSY            = 	BIT(6),		// diff from doc,  bit 6 working
};
#define reg_spi_sp				REG_ADDR8(0x0a)
enum{
	FLD_MASTER_SPI_CLK = 		BIT_RNG(0,6),
	FLD_SPI_ENABLE =			BIT(7),
	FLD_SPI_PAD_SEL = 			BIT(7),
};

#define reg_spi_inv_clk			REG_ADDR8(0x0b)//spi supports four modes
enum {
    FLD_SPI_MODE_WORK_MODE = BIT_RNG(0,1),
};

/****************************************************
 master spi regs struct: begin  addr : 0x0c
 *****************************************************/
#define reg_master_spi_data		REG_ADDR8(0x0c)
#define reg_master_spi_ctrl		REG_ADDR8(0x0d)

enum{
	FLD_MASTER_SPI_CS = 		BIT(0),
	FLD_MASTER_SPI_SDO = 		BIT(1),
	FLD_MASTER_SPI_CONT = 		BIT(2),
	FLD_MASTER_SPI_RD = 		BIT(3),
	FLD_MASTER_SPI_BUSY = 		BIT(4),
};



#define reg_i2c_irq_status		REG_ADDR8(0x21)
#define reg_i2c_clr_status		REG_ADDR8(0x22)
enum{
	FLD_I2C_STATUS_WR = 		BIT(1),
	FLD_SPI_STATUS_WR =         BIT(1),
	FLD_I2C_STATUS_RD = 		BIT(2),
};

#define reg_spi_irq_status      reg_i2c_irq_status
#define reg_spi_clr_status      reg_i2c_clr_status

#define reg_slave_map_addrl      REG_ADDR8(0xe1)
#define reg_slave_map_addrm      REG_ADDR8(0xe2)
#define reg_slave_map_addrh      REG_ADDR8(0xe3)
#define reg_i2c_priority_ctrl    REG_ADDR8(0x5b7)

/****************************************************
 sys regs struct: begin  addr : 0x60
 *****************************************************/
#define reg_rst0				REG_ADDR8(0x60)
enum{
	FLD_RST0_SPI = 				BIT(0),
	FLD_RST0_I2C = 				BIT(1),
	FLD_RST0_RS232 = 			BIT(2),    //FLD_RST0_UART ?
	FLD_RST0_USB = 				BIT(3),
	FLD_RST0_PWM = 				BIT(4),
	FLD_RST0_QDEC =				BIT(5),
	FLD_RST0_SWIRE = 			BIT(7),
};

#define reg_rst1				REG_ADDR8(0x61)
enum{
	FLD_RST1_ZB = 				BIT(0),
	FLD_RST1_SYS_TIMER = 		BIT(1),
	FLD_RST1_DMA =				BIT(2),
	FLD_RST1_ALGM = 			BIT(3),
	FLD_RST1_AES = 				BIT(4),
	FLD_RST1_ADC = 				BIT(5),
	FLD_RST1_ALG =				BIT(6),
};

#define reg_rst2				REG_ADDR8(0x62)
enum{
	FLD_RST2_AIF =				BIT(0),
	FLD_RST2_AUD =				BIT(1),
	FLD_RST2_DFIFO =			BIT(2),
	FLD_RST2_RISC =				BIT(4),
	FLD_RST2_MCIC = 			BIT(5),
	FLD_RST2_RISC1 =			BIT(6),
	FLD_RST2_MCIC1 = 			BIT(7),
};



#define reg_clk_en0				REG_ADDR8(0x63)
enum{
	FLD_CLK_SPI_EN = 			BIT(0),
	FLD_CLK_I2C_EN = 			BIT(1),
	FLD_CLK_RS232_EN = 			BIT(2),
	FLD_CLK_USB_EN = 			BIT(3),
	FLD_CLK_PWM_EN = 			BIT(4),
	FLD_CLK_QDEC_EN = 			BIT(5),
	FLD_CLK_SWIRE_EN = 			BIT(7),
};

#define reg_clk_en1				REG_ADDR8(0x64)
enum{
	FLD_CLK1_ZB_EN = 			BIT(0),
	FLD_CLK1_SYS_TIMER_EN = 	BIT(1),
	FLD_CLK1_DMA_EN = 			BIT(2),
	FLD_CLK1_ALGM_EN = 			BIT(3),
	FLD_CLK1_AES_EN = 			BIT(4),

};


#define reg_clk_en2				REG_ADDR8(0x65)
enum{
	FLD_CLK2_AIF_EN = 			BIT(0),
	FLD_CLK2_AUD_EN = 			BIT(1),
	FLD_CLK2_DFIFO_EN = 		BIT(2),
	FLD_CLK2_MC_EN = 			BIT(4),
	FLD_CLK2_MCIC_EN = 			BIT(5),
};



#define reg_clk_sel				REG_ADDR8(0x66)
enum{
	FLD_SCLK_DIV = 				BIT_RNG(0,4),
	FLD_SCLK_SEL =				BIT_RNG(5,6),
	FLD_SCLK_HS_SEL =			BIT(7),
};

#define reg_i2s_step			REG_ADDR8(0x67)
enum{
	FLD_I2S_STEP = 				BIT_RNG(0,6),
	FLD_I2S_CLK_EN =			BIT(7),
};

#define reg_i2s_mod				REG_ADDR8(0x68)




#define reg_wakeup_en			REG_ADDR8(0x6e)
enum{
	FLD_WAKEUP_SRC_I2C = 		BIT(0),
	FLD_WAKEUP_SRC_SPI =		BIT(1),
	FLD_WAKEUP_SRC_USB =		BIT(2),
	FLD_WAKEUP_SRC_GPIO =		BIT(3),
	FLD_WAKEUP_SRC_I2C_SYN =	BIT(4),
	FLD_WAKEUP_SRC_GPIO_RM =	BIT(5),
	FLD_WAKEUP_SRC_USB_RESM =	BIT(6),
	FLD_WAKEUP_SRC_RST_SYS =	BIT(7),
};






#define reg_mcu_wakeup_mask		REG_ADDR32(0x78)


/****************************************************
 uart regs struct: begin  0x90
 *****************************************************/
#define reg_uart_data_buf0		REG_ADDR8(0x90)
#define reg_uart_data_buf1		REG_ADDR8(0x91)
#define reg_uart_data_buf2		REG_ADDR8(0x92)
#define reg_uart_data_buf3		REG_ADDR8(0x93)

#define reg_uart_data_buf(i)    REG_ADDR8(0x90 + (i))  //i = 0~3


#define reg_uart_clk_div		REG_ADDR16(0x94)
enum{
	FLD_UART_CLK_DIV = 			BIT_RNG(0,14),
	FLD_UART_CLK_DIV_EN = 		BIT(15)
};

#define reg_uart_ctrl0			REG_ADDR8(0x96)
enum{
	FLD_UART_BPWC = 			BIT_RNG(0,3),
	FLD_UART_RX_DMA_EN = 		BIT(4),
	FLD_UART_TX_DMA_EN =		BIT(5),
	FLD_UART_RX_IRQ_EN = 		BIT(6),
	FLD_UART_TX_IRQ_EN =		BIT(7),
};

#define reg_uart_ctrl1         		REG_ADDR8(0x97)
enum {
    FLD_UART_CTRL1_CTS_SELECT	   = BIT(0),
    FLD_UART_CTRL1_CTS_EN 		   = BIT(1),
    FLD_UART_CTRL1_PARITY_EN 	   = BIT(2),
    FLD_UART_CTRL1_PARITY_POLARITY = BIT(3),   //1:odd parity   0:even parity
    FLD_UART_CTRL1_STOP_BIT 	   = BIT_RNG(4,5),
    FLD_UART_CTRL1_TTL 			   = BIT(6),
    FLD_UART_CTRL1_LOOPBACK 	   = BIT(7),
};

#define reg_uart_ctrl2			REG_ADDR16(0x98)
enum {
    FLD_UART_CTRL2_RTS_TRIG_LVL   	 = BIT_RNG(0,3),
    FLD_UART_CTRL2_RTS_PARITY 		 = BIT(4),
    FLD_UART_CTRL2_RTS_MANUAL_VAL 	 = BIT(5),
    FLD_UART_CTRL2_RTS_MANUAL_EN 	 = BIT(6),
    FLD_UART_CTRL2_RTS_EN 			 = BIT(7),
	FLD_UART_CTRL3_RX_IRQ_TRIG_LEVEL = BIT_RNG(8,11),
	FLD_UART_CTRL3_TX_IRQ_TRIG_LEVEL = BIT_RNG(12,15),
};


#define reg_uart_ctrl3        	REG_ADDR8(0x99)
enum {
	FLD_UART_RX_IRQ_TRIG_LEV = BIT_RNG(0,3),
	FLD_UART_TX_IRQ_TRIG_LEV = BIT_RNG(4,7),
};



#if 0
#define reg_uart_rx_timeout		REG_ADDR16(0x9a)
enum{
	FLD_UART_TIMEOUT		 = 	BIT_RNG(0,9),		//  timeout bit width
	FLD_UART_P7816_EN	 	 =  BIT(13),
	FLD_UART_MASK_TXDONE_IRQ =  BIT(14),
	FLD_UART_MASK_ERR_IRQ 	 =  BIT(15),
};
#else

#define reg_uart_rx_timeout0	REG_ADDR8(0x9a)
enum{
	FLD_UART_TIMEOUT_BW		 = 	BIT_RNG(0,7),		//  timeout bit width
};

#define reg_uart_rx_timeout1    REG_ADDR8(0x9b)
enum{
	FLD_UART_TIMEOUT_MUL	 = 	BIT_RNG(0,1),		//  timeout bit width
	FLD_UART_P7816_EN	 	 =  BIT(5),
	FLD_UART_MASK_TXDONE_IRQ =  BIT(6),
	FLD_UART_MASK_ERR_IRQ 	 =  BIT(7),
};

#endif


#define reg_uart_buf_cnt       REG_ADDR8(0x9c)
enum{
	FLD_UART_RX_BUF_CNT		=  BIT_RNG(0,3),
	FLD_UART_TX_BUF_CNT		=  BIT_RNG(4,7),
};

#define reg_uart_status0       REG_ADDR8(0x9d)
enum{
	FLD_UART_RBCNT 	     =  BIT_RNG(0,2),
	FLD_UART_IRQ_FLAG    =  BIT(3),
	FLD_UART_WBCNT 	     =  BIT_RNG(4,6),
	FLD_UART_RX_ERR_FLAG =  BIT(7),
};

#define reg_uart_status1       REG_ADDR8(0x9e)
enum{
	FLD_UART_TX_DONE   	  =  BIT(0),
	FLD_UART_TX_BUF_IRQ   =  BIT(1),
	FLD_UART_RX_DONE   	  =  BIT(2),
	FLD_UART_RX_BUF_IRQ   =  BIT(3),
};


#define reg_uart_state       REG_ADDR8(0x9f)
enum{
	FLD_UART_TSTATE_I 	     =  BIT_RNG(0,2),
	FLD_UART_RSTATE_I	     =  BIT_RNG(4,7),
};


/****************************************************
 swire regs struct: begin  addr : 0xb0
 *****************************************************/
#define reg_swire_data			REG_ADDR8(0xb0)
#define reg_swire_ctrl1			REG_ADDR8(0xb1)
enum{
	FLD_SWIRE_WR  = 			BIT(0),
	FLD_SWIRE_RD  = 			BIT(1),
	FLD_SWIRE_CMD =				BIT(2),
	FLD_SWIRE_USB_DET =			BIT(6),
	FLD_SWIRE_USB_EN =			BIT(7),
};

#define reg_swire_clk_div		REG_ADDR8(0xb2)

//////  analog controls 0xb8 ///////
#define reg_ana_ctrl32			REG_ADDR32(0xb8)	// for performance, set addr and data at a time
#define reg_ana_addr_data		REG_ADDR16(0xb8)	// for performance, set addr and data at a time
#define reg_ana_addr			REG_ADDR8(0xb8)
#define reg_ana_data			REG_ADDR8(0xb9)
#define reg_ana_ctrl			REG_ADDR8(0xba)

enum{
	FLD_ANA_BUSY  = 			BIT(0),
	FLD_ANA_RSV	=				BIT(4),
	FLD_ANA_RW  = 				BIT(5),
	FLD_ANA_START  = 			BIT(6),
	FLD_ANA_CYC  = 				BIT(7),
};
/****************************************************
 audio regs struct: begin  addr : 0x100
 *****************************************************/
#define reg_ctrl_ep_ptr			REG_ADDR8(0x100)
#define reg_ctrl_ep_dat			REG_ADDR8(0x101)
#define reg_ctrl_ep_ctrl		REG_ADDR8(0x102)

// same for all endpoints
enum{
	FLD_EP_DAT_ACK  = 			BIT(0),
	FLD_EP_DAT_STALL =			BIT(1),
	FLD_EP_STA_ACK = 			BIT(2),
	FLD_EP_STA_STALL = 			BIT(3),
};

#define reg_ctrl_ep_irq_sta		REG_ADDR8(0x103)
enum{
	FLD_CTRL_EP_IRQ_TRANS  = 	BIT_RNG(0,3),
	FLD_CTRL_EP_IRQ_SETUP  =	BIT(4),
	FLD_CTRL_EP_IRQ_DATA  =		BIT(5),
	FLD_CTRL_EP_IRQ_STA  = 		BIT(6),
	FLD_CTRL_EP_IRQ_INTF  = 	BIT(7),
};

#define reg_ctrl_ep_irq_mode	REG_ADDR8(0x104)
enum{
	FLD_CTRL_EP_AUTO_ADDR = 	BIT(0),
	FLD_CTRL_EP_AUTO_CFG =		BIT(1),
	FLD_CTRL_EP_AUTO_INTF =		BIT(2),
	FLD_CTRL_EP_AUTO_STA =		BIT(3),
	FLD_CTRL_EP_AUTO_SYN =		BIT(4),
	FLD_CTRL_EP_AUTO_DESC =		BIT(5),
	FLD_CTRL_EP_AUTO_FEAT =		BIT(6),
	FLD_CTRL_EP_AUTO_STD =		BIT(7),
};

#define reg_usb_ctrl			REG_ADDR8(0x105)
enum{
	FLD_USB_CTRL_AUTO_CLK = 	BIT(0),
	FLD_USB_CTRL_LOW_SPD = 		BIT(1),
	FLD_USB_CTRL_LOW_JITT =		BIT(2),
	FLD_USB_CTRL_TST_MODE = 	BIT(3),
};

#define reg_usb_cyc_cali		REG_ADDR16(0x106)
#define reg_usb_mdev			REG_ADDR8(0x10a)
#define reg_usb_host_conn		REG_ADDR8(0x10b)
enum{
	FLD_USB_MDEV_SELF_PWR = 	BIT(0),
	FLD_USB_MDEV_SUSP_STA = 	BIT(1),
};

#define reg_usb_sups_cyc_cali	REG_ADDR8(0x10c)
#define reg_usb_intf_alt		REG_ADDR8(0x10d)

#define reg_usb_ep8123_ptr		REG_ADDR32(0x110)
#define reg_usb_ep8_ptr			REG_ADDR8(0x110)
#define reg_usb_ep1_ptr			REG_ADDR8(0x111)
#define reg_usb_ep2_ptr			REG_ADDR8(0x112)
#define reg_usb_ep3_ptr			REG_ADDR8(0x113)
#define reg_usb_ep4567_ptr		REG_ADDR32(0x114)
#define reg_usb_ep4_ptr			REG_ADDR8(0x114)
#define reg_usb_ep5_ptr			REG_ADDR8(0x115)
#define reg_usb_ep6_ptr			REG_ADDR8(0x116)
#define reg_usb_ep7_ptr			REG_ADDR8(0x117)
#define reg_usb_ep_ptr(i)		REG_ADDR8(0x110+((i) & 0x07))

#define reg_usb_ep8123_dat		REG_ADDR32(0x118)
#define reg_usb_ep8_dat			REG_ADDR8(0x118)
#define reg_usb_ep1_dat			REG_ADDR8(0x119)
#define reg_usb_ep2_dat			REG_ADDR8(0x11a)
#define reg_usb_ep3_dat			REG_ADDR8(0x11b)
#define reg_usb_ep4567_dat		REG_ADDR32(0x11c)
#define reg_usb_ep4_dat			REG_ADDR8(0x11c)
#define reg_usb_ep5_dat			REG_ADDR8(0x11d)
#define reg_usb_ep6_dat			REG_ADDR8(0x11e)
#define reg_usb_ep7_dat			REG_ADDR8(0x11f)
#define reg_usb_ep_dat(i)		REG_ADDR8(0x118+((i) & 0x07))

#define reg_usb_ep8_ctrl		REG_ADDR8(0x120)
#define reg_usb_ep1_ctrl		REG_ADDR8(0x121)
#define reg_usb_ep2_ctrl		REG_ADDR8(0x122)
#define reg_usb_ep3_ctrl		REG_ADDR8(0x123)
#define reg_usb_ep4_ctrl		REG_ADDR8(0x124)
#define reg_usb_ep5_ctrl		REG_ADDR8(0x125)
#define reg_usb_ep6_ctrl		REG_ADDR8(0x126)
#define reg_usb_ep7_ctrl		REG_ADDR8(0x127)
#define reg_usb_ep_ctrl(i)		REG_ADDR8(0x120+((i) & 0x07))

enum{
	FLD_USB_EP_BUSY = 			BIT(0),
	FLD_USB_EP_STALL =			BIT(1),
	FLD_USB_EP_DAT0 =			BIT(2),
	FLD_USB_EP_DAT1 =			BIT(3),
	FLD_USB_EP_MONO =			BIT(6),
	FLD_USB_EP_EOF_ISO =		BIT(7),
};

#define reg_usb_ep8123_buf_addr	REG_ADDR32(0x128)
#define reg_usb_ep8_buf_addr	REG_ADDR8(0x128)
#define reg_usb_ep1_buf_addr	REG_ADDR8(0x129)
#define reg_usb_ep2_buf_addr	REG_ADDR8(0x12a)
#define reg_usb_ep3_buf_addr	REG_ADDR8(0x12b)
#define reg_usb_ep4567_buf_addr	REG_ADDR32(0x12c)
#define reg_usb_ep4_buf_addr	REG_ADDR8(0x12c)
#define reg_usb_ep5_buf_addr	REG_ADDR8(0x12d)
#define reg_usb_ep6_buf_addr	REG_ADDR8(0x12e)
#define reg_usb_ep7_buf_addr	REG_ADDR8(0x12f)
#define reg_usb_ep_buf_addr(i)	REG_ADDR8(0x128+((i) & 0x07))

#define reg_usb_ram_ctrl		REG_ADDR8(0x130)
enum{
	FLD_USB_CEN_PWR_DN =		BIT(0),
	FLD_USB_CLK_PWR_DN =		BIT(1),
	FLD_USB_WEN_PWR_DN =		BIT(3),
	FLD_USB_CEN_FUNC =			BIT(4),
};

#define reg_usb_iso_mode		REG_ADDR8(0x138)
#define reg_usb_irq				REG_ADDR8(0x139)
#define reg_usb_mask			REG_ADDR8(0x13a)
#define reg_usb_ep8_send_max	REG_ADDR8(0x13b)
#define reg_usb_ep8_send_thre	REG_ADDR8(0x13c)
#define reg_usb_ep8_fifo_mode	REG_ADDR8(0x13d)
#define reg_usb_ep_max_size		REG_ADDR8(0x13e)

enum{
	FLD_USB_ENP8_FIFO_MODE =	BIT(0),
	FLD_USB_ENP8_FULL_FLAG =	BIT(1),
};
/****************************************************
	RF : begin  addr : 0x4e8
 *****************************************************/
#define reg_rf_tx_mode1			REG_ADDR8(0x400)
#define reg_rf_tx_mode			REG_ADDR16(0x400)
enum{
	FLD_RF_TX_DMA_EN =			BIT(0),
	FLD_RF_TX_CRC_EN =			BIT(1),
	FLD_RF_TX_BANDWIDTH =		BIT_RNG(2,3),
	FLD_RF_TX_OUTPUT = 			BIT(4),
	FLD_RF_TX_TST_OUT =			BIT(5),
	FLD_RF_TX_TST_EN =			BIT(6),
	FLD_RF_TX_TST_MODE =		BIT(7),
	FLD_RF_TX_ZB_PN_EN =		BIT(8),
	FLD_RF_TX_ZB_FEC_EN =		BIT(9),
	FLD_RF_TX_ZB_INTL_EN =		BIT(10),	// interleaving
	FLD_RF_TX_1M2M_PN_EN =		BIT(11),
	FLD_RF_TX_1M2M_FEC_EN =		BIT(12),
	FLD_RF_TX_1M2M_INTL_EN =	BIT(13), 	// interleaving
};
#define reg_rf_tx_buf_sta		REG_ADDR32(0x41c)

#define reg_rf_rx_sense_thr		REG_ADDR8(0x422)
#define reg_rf_rx_auto			REG_ADDR8(0x426)
enum{
	FLD_RF_RX_IRR_GAIN =		BIT(0),
	FLD_RF_RX_IRR_PHASE =		BIT(1),
	FLD_RF_RX_DAC_I =			BIT(2),
	FLD_RF_RX_DAC_Q =			BIT(3),
	FLD_RF_RX_LNA_GAIN =		BIT(4),
	FLD_RF_RX_MIX2_GAIN =		BIT(5),
	FLD_RF_RX_PGA_GAIN =		BIT(6),
	FLD_RF_RX_CAL_EN =			BIT(7),
};
#define reg_rf_rx_sync			REG_ADDR8(0x427)
enum{
	FLD_RF_FREQ_COMP_EN =		BIT(0),
	FLD_RF_ADC_SYNC =			BIT(1),
	FLD_RF_ADC_INP_SIGNED =		BIT(2),
	FLD_RF_SWAP_ADC_IQ =		BIT(3),
	FLD_RF_NOTCH_FREQ_SEL =		BIT(4),
	FLD_RF_NOTCH_BAND_SEL = 	BIT(5),
	FLD_RF_NOTCH_EN = 			BIT(6),
	FLD_RF_DN_CONV_FREQ_SEL =	BIT(7),
};

#define reg_rf_rx_mode			REG_ADDR8(0x428)
enum{
	FLD_RF_RX_EN =				BIT(0),
	FLD_RF_RX_MODE_1M =			BIT(1),
	FLD_RF_RX_MODE_2M =			BIT(2),
	FLD_RF_RX_LOW_IF =			BIT(3),
	FLD_RF_RX_BYPASS_DCOC =		BIT(4),
	FLD_RF_RX_MAN_FINE_TUNE = 	BIT(5),
	FLD_RF_RX_SINGLE_CAL =		BIT(6),
	FLD_RF_RX_LOW_PASS_FILTER =	BIT(7),
};

#define reg_rf_rx_pilot			REG_ADDR8(0x42b)
enum{
	FLD_RF_PILOT_LEN =			BIT_RNG(0,3),
	FLD_RF_ZB_SFD_CHK =			BIT(4),
	FLD_RF_1M_SFD_CHK =			BIT(5),
	FLD_RF_2M_SFD_CHK = 		BIT(6),
	FLD_RF_ZB_OR_AUTO = 		BIT(7),
};

#define reg_rf_rx_chn_dc		REG_ADDR32(0x42c)
#define reg_rf_rx_q_chn_cal		REG_ADDR8(0x42f)
enum{
	FLD_RF_RX_DCQ_HIGH =		BIT_RNG(0,6),
	FLD_RF_RX_DCQ_CAL_START =	BIT(7),
};
#define reg_rf_rx_pel			REG_ADDR16(0x434)
#define reg_rf_rx_pel_gain		REG_ADDR32(0x434)
#define reg_rf_rx_rssi_offset	REG_ADDR8(0x439)

#define reg_rf_rx_hdx			REG_ADDR8(0x43b)
enum{
	FLD_RX_HEADER_LEN =			BIT_RNG(0,3),
	FLD_RT_TICK_LO_SEL = 		BIT(4),
	FLD_RT_TICK_HI_SEL = 		BIT(5),
	FLD_RT_TICK_FRAME = 		BIT(6),
	FLD_PKT_LEN_OUTP_EN = 		BIT(7),
};

#define reg_rf_rx_gctl			REG_ADDR8(0x43c)
enum{
	FLD_RX_GCTL_CIC_SAT_LO_EN =	BIT(0),
	FLD_RX_GCTL_CIC_SAT_HI_EN = BIT(1),
	FLD_RX_GCTL_AUTO_PWR =		BIT(2),
	FLD_RX_GCTL_ADC_RST_VAL =	BIT(4),
	FLD_RX_GCTL_ADC_RST_EN =	BIT(5),
	FLD_RX_GCTL_PWR_CHG_DET_S =	BIT(6),
	FLD_RX_GCTL_PWR_CHG_DET_N = BIT(7),
};
#define reg_rf_rx_peak			REG_ADDR8(0x43d)
enum{
	FLD_RX_PEAK_DET_SRC_EN =	BIT_RNG(0,2),
	FLD_TX_PEAK_DET_EN =		BIT(3),
	FLD_PEAK_DET_NUM =			BIT_RNG(4,5),
	FLD_PEAK_MAX_CNT_PRD =		BIT_RNG(6,7),
};

#define reg_rf_rx_status		REG_ADDR8(0x443)
enum{
	FLD_RF_RX_STATE =			BIT_RNG(0,3),
	FLD_RF_RX_STA_RSV = 		BIT_RNG(4,5),
	FLD_RF_RX_INTR = 			BIT(6),
	FLD_RF_TX_INTR =			BIT(7),
};



#define reg_rf_ll_ctrl_0		REG_ADDR8(0xf02)
#define reg_rf_ll_ctrl_1		REG_ADDR8(0xf03)
#define reg_rf_ll_ctrl_2		REG_ADDR8(0xf15)

#define reg_rf_ll_ctrl_3		REG_ADDR8(0xf16)
enum{
	FLD_RF_TX_EN_DLY_EN = 		BIT(0),
	FLD_RF_PLL_RST_EN	=       BIT(1),
	FLD_RF_CMD_SCHEDULE_EN	=   BIT(2),
	FLD_RF_R_TX_EN_DLY =		BIT_RNG(4,7),
};


#define reg_rf_irq_mask			REG_ADDR16(0xf1c)
#define reg_rf_irq_status		REG_ADDR16(0xf20)
#define reg_rf_fsm_timeout		REG_ADDR32(0xf2c)

#define		CLEAR_ALL_RFIRQ_STATUS   ( reg_rf_irq_status = 0xffff )

enum{
	FLD_RF_IRQ_RX = 			BIT(0),
	FLD_RF_IRQ_TX =				BIT(1),
	FLD_RF_IRQ_RX_TIMEOUT =		BIT(2),
	FLD_RF_IRQ_CRC =			BIT(4),
	FLD_RF_IRQ_CMD_DONE  =		BIT(5),
	FLD_RF_IRQ_FSM_TIMEOUT  =	BIT(6),
	FLD_RF_IRQ_RETRY_HIT =		BIT(7),
	FLD_RF_IRQ_TX_DS =          BIT(8),
    FLD_RF_IRQ_RX_DR =          BIT(9),
	FLD_RF_IRQ_FIRST_TIMEOUT =	BIT(10),
	FLD_RF_IRQ_INVALID_PID =    BIT(11),
	FLD_RF_IRQ_STX_TIMEOUT =    BIT(12),
	FLD_RF_IRQ_ALL =            0X1FFF,
};


enum{
	FLD_RF_IRX_RX_TIMEOUT =		BIT(2),
	FLD_RF_IRX_CMD_DONE  =		BIT(5),
	FLD_RF_IRX_RETRY_HIT =		BIT(7),
};

// The value for FLD_RF_RX_STATE
enum{
	RF_RX_STA_IDLE = 0,
	RF_RX_STA_SET_GAIN = 1,
	RF_RX_STA_CIC_SETTLE = 2,
	RF_RX_STA_LPF_SETTLE = 3,
	RF_RX_STA_PE = 4,
	RF_RX_STA_SYN_START = 5,
	RF_RX_STA_GLOB_SYN = 6,
	RF_RX_STA_GLOB_LOCK = 7,
	RF_RX_STA_LOCAL_SYN = 8,
	RF_RX_STA_LOCAL_LOCK = 9,
	RF_RX_STA_ALIGN = 10,
	RF_RX_STA_ADJUST = 11,
	RF_RX_STA_DEMOD = 12,		// de modulation
	RF_RX_STA_FOOTER = 13,
};

#define reg_rx_rnd_mode			REG_ADDR8(0x447)
enum{
	FLD_RX_RND_SRC =			BIT(0),
	FLD_RX_RND_MANU_MODE =		BIT(1),
	FLD_RX_RND_AUTO_RD =		BIT(2),
	FLD_RX_RND_FREE_MODE =		BIT(3),
	FLD_RX_RND_CLK_DIV =		BIT_RNG(4,7),
};
#define reg_rnd_number			REG_ADDR16(0x448)

#define reg_bb_max_tick			REG_ADDR16(0x44c)
#define reg_rf_rtt				REG_ADDR32(0x454)
enum{
	FLD_RTT_CAL =				BIT_RNG(0,7),
	FLD_RTT_CYC1 =				BIT_RNG(8,15),
	FLD_RTT_LOCK =				BIT_RNG(16,23),
	FLD_RT_SD_DLY_40M =			BIT_RNG(24,27),
	FLD_RT_SD_DLY_BYPASS = 		BIT(28),
};

#define reg_rf_chn_rssi			REG_ADDR8(0x458)

#define reg_rf_rx_gain_agc(i)	REG_ADDR32(0x480+((i)<<2))

#define reg_rf_rx_dci			REG_ADDR8(0x4cb)	//  different from the document, why
#define reg_rf_rx_dcq			REG_ADDR8(0x4cf)	//  different from the document, why

#define reg_pll_rx_coarse_tune	REG_ADDR16(0x4d0)
#define reg_pll_rx_coarse_div	REG_ADDR8(0x4d2)
#define reg_pll_rx_fine_tune	REG_ADDR16(0x4d4)
#define reg_pll_rx_fine_div		REG_ADDR8(0x4d6)
#define reg_pll_tx_coarse_tune	REG_ADDR16(0x4d8)
#define reg_pll_tx_coarse_div	REG_ADDR8(0x4da)
#define reg_pll_tx_fine_tune	REG_ADDR16(0x4dc)
#define reg_pll_tx_fine_div		REG_ADDR8(0x4de)

#define reg_pll_rx_frac			REG_ADDR32(0x4e0)
#define reg_pll_tx_frac			REG_ADDR32(0x4e4)

#define reg_pll_tx_ctrl			REG_ADDR8(0x4e8)
#define reg_pll_ctrl16			REG_ADDR16(0x4e8)
#define reg_pll_ctrl			REG_ADDR32(0x4e8)
enum{
	FLD_PLL_TX_CYC0 =			BIT(0),
	FLD_PLL_TX_SOF =			BIT(1),
	FLD_PLL_TX_CYC1 =			BIT(2),
	FLD_PLL_TX_PRE_EN =			BIT(3),
	FLD_PLL_TX_VCO_EN =			BIT(4),
	FLD_PLL_TX_PWDN_DIV =		BIT(5),
	FLD_PLL_TX_MOD_EN =			BIT(6),
	FLD_PLL_TX_MOD_TRAN_EN =	BIT(7),
	FLD_PLL_RX_CYC0 =			BIT(8),
	FLD_PLL_RX_SOF = 			BIT(9),
	FLD_PLL_RX_CYC1 =			BIT(10),
	FLD_PLL_RX_PRES_EN = 		BIT(11),
	FLD_PLL_RX_VCO_EN =			BIT(12),
	FLD_PLL_RX_PWDN_DIV =		BIT(13),
	FLD_PLL_RX_PEAK_EN =		BIT(14),
	FLD_PLL_RX_TP_CYC = 		BIT(15),
	FLD_PLL_SD_RSTB =			BIT(16),
	FLD_PLL_SD_INTG_EN =		BIT(17),
	FLD_PLL_CP_TRI = 			BIT(18),
	FLD_PLL_PWDN_INTG1 = 		BIT(19),
	FLD_PLL_PWDN_INTG2 =		BIT(20),
	FLD_PLL_PWDN_INTG_DIV =		BIT(21),
	FLD_PLL_PEAK_DET_EN =		BIT(22),
	FLD_PLL_OPEN_LOOP_EN =		BIT(23),
	FLD_PLL_RX_TICK_EN =		BIT(24),
	FLD_PLL_TX_TICK_EN =		BIT(25),
	FLD_PLL_RX_ALWAYS_ON =		BIT(26),
	FLD_PLL_TX_ALWAYS_ON =		BIT(27),
	FLD_PLL_MANUAL_MODE_EN =	BIT(28),
	FLD_PLL_CAL_DONE_EN =		BIT(29),
	FLD_PLL_LOCK_EN =			BIT(30),
};
#define reg_pll_rx_ctrl			REG_ADDR8(0x4e9)
enum{
	FLD_PLL_RX2_CYC0 =			BIT(0),
	FLD_PLL_RX2_SOF = 			BIT(1),
	FLD_PLL_RX2_CYC1 =			BIT(2),
	FLD_PLL_RX2_PRES_EN = 		BIT(3),
	FLD_PLL_RX2_VCO_EN =		BIT(4),
	FLD_PLL_RX2_PD_DIV =		BIT(5),
	FLD_PLL_RX2_PEAK_EN =		BIT(6),
	FLD_PLL_RX2_TP_CYC = 		BIT(7),
};

#define reg_pll_ctrl_a			REG_ADDR8(0x4eb)
enum{
	FLD_PLL_A_RX_TICK_EN =		BIT(0),
	FLD_PLL_A_TX_TICK_EN =		BIT(1),
	FLD_PLL_A_RX_ALWAYS_ON =	BIT(2),
	FLD_PLL_A_TX_ALWAYS_ON =	BIT(3),
	FLD_PLL_A_MANUAL_MODE_EN =	BIT(4),
	FLD_PLL_A_CAL_DONE_EN =		BIT(5),
	FLD_PLL_A_LOCK_EN =			BIT(6),
};
// pll polarity
#define reg_pll_pol_ctrl		REG_ADDR16(0x4ec)
enum{
	FLD_PLL_POL_TX_PRE_EN =		BIT(0),
	FLD_PLL_POL_TX_VCO_EN =		BIT(1),
	FLD_PLL_POL_TX_PD_DIV =		BIT(2),
	FLD_PLL_POL_MOD_EN =		BIT(3),
	FLD_PLL_POL_MOD_TRAN_EN =	BIT(4),
	FLD_PLL_POL_RX_PRE_EN =		BIT(5),
	FLD_PLL_POL_RX_VCO_EN =		BIT(6),
	FLD_PLL_POL_RX_PD_DIV =		BIT(7),
	FLD_PLL_POL_SD_RSTB =		BIT(8),
	FLD_PLL_POL_SD_INTG_EN =	BIT(9),
	FLD_PLL_POL_CP_TRI =		BIT(10),
	FLD_PLL_POL_TX_SOF =		BIT(11),
	FLD_PLL_POL_RX_SOF =		BIT(12),
};

#define reg_rf_rx_cap			REG_ADDR16(0x4f0)
#define reg_rf_tx_cap			REG_ADDR16(0x4f0)


/****************************************************
 dma mac regs struct: begin  addr : 0xC00
 *****************************************************/
//uart rx
#define reg_dma0_addr			REG_ADDR16(0xc00)
#define reg_dma0_size			REG_ADDR8(0xc02)
#define reg_dma0_mode			REG_ADDR8(0xc03)
enum{
	FLD_DMA_WR_MEM =			BIT(0),
	FLD_DMA_PINGPONG_EN =		BIT(1),
	FLD_DMA_FIFO_EN =			BIT(2),
	FLD_DMA_AUTO_MODE =			BIT(3),
	FLD_DMA_BYTE_MODE =			BIT(4),
	FLD_DMA_FIFO8 	=			(BIT(7) | BIT(6) | FLD_DMA_WR_MEM | FLD_DMA_PINGPONG_EN),
};


//uart tx
#define reg_dma1_addr			REG_ADDR16(0xc04)
#define reg_dma1_size			REG_ADDR8(0xc06)
#define reg_dma1_mode			REG_ADDR8(0xc07)

//rf rx dma
#define reg_dma2_addr			REG_ADDR16(0xc08)
#define reg_dma2_size			REG_ADDR8(0xc0a)
#define reg_dma2_mode			REG_ADDR8(0xc0b)

//rf tx dma
#define reg_dma3_addr			REG_ADDR16(0xc0c)
#define reg_dma3_size			REG_ADDR8(0xc0e)
#define reg_dma3_mode			REG_ADDR8(0xc0f)

#define reg_dma4_addr			REG_ADDR16(0xc10)
#define reg_dma4_size			REG_ADDR8(0xc12)
#define reg_dma4_mode			REG_ADDR8(0xc13)

#define reg_dma5_addr			REG_ADDR16(0xc14)
#define reg_dma5_size			REG_ADDR8(0xc16)
#define reg_dma5_mode			REG_ADDR8(0xc17)

//pwm tx dma
#define reg_dma7_addr			REG_ADDR16(0xc18)
#define reg_dma7_size			REG_ADDR8(0xc1a)
#define reg_dma7_mode			REG_ADDR8(0xc1b)


#define reg_dma_t_addr			REG_ADDR16(0xc1c)
#define reg_dma_t_size			REG_ADDR8(0xc1e)



//  The default channel assignment
#define reg_dma_uart_rx_addr	reg_dma0_addr
#define reg_dma_uart_rx_size	reg_dma0_size
#define reg_dma_uart_rx_mode	reg_dma0_mode

#define reg_dma_uart_tx_addr	reg_dma1_addr
#define reg_dma_uart_tx_size	reg_dma1_size
#define reg_dma_uart_tx_mode	reg_dma1_mode

#define reg_dma_rf_rx_addr		reg_dma2_addr
#define reg_dma_rf_rx_size		reg_dma2_size
#define reg_dma_rf_rx_mode		reg_dma2_mode

#define reg_dma_rf_tx_addr		reg_dma3_addr
#define reg_dma_rf_tx_size		reg_dma3_size
#define reg_dma_rf_tx_mode		reg_dma3_mode

#define reg_dma_pwm_addr		reg_dma7_addr
#define reg_dma_pwm_size		reg_dma7_size
#define reg_dma_pwm_mode		reg_dma7_mode



#define reg_dma_chn_en			REG_ADDR8(0xc20)
#define reg_dma_chn_irq_msk		REG_ADDR8(0xc21)
#define reg_dma_tx_rdy0			REG_ADDR8(0xc24)
#define reg_dma_rx_rdy0			REG_ADDR8(0xc26)
#define reg_dma_rx_rdy1			REG_ADDR8(0xc27)
#define reg_dma_irq_status		reg_dma_rx_rdy0
enum{
	FLD_DMA_CHN0 =	BIT(0),		FLD_DMA_CHN_UART_RX =	BIT(0),
	FLD_DMA_CHN1 =	BIT(1),		FLD_DMA_CHN_UART_TX =	BIT(1),
	FLD_DMA_CHN2 =	BIT(2),		FLD_DMA_CHN_RF_RX =		BIT(2),
	FLD_DMA_CHN3 =	BIT(3),		FLD_DMA_CHN_RF_TX =		BIT(3),
	FLD_DMA_CHN4 =	BIT(4),		FLD_DMA_CHN_AES_DECO =  BIT(4),
	FLD_DMA_CHN5 =	BIT(5),     FLD_DMA_CHN_AES_CODE =  BIT(5),
	FLD_DMA_CHN7 =	BIT(7),		FLD_DMA_CHN_PWM  	 =	BIT(7),
};

typedef enum {
    FLD_DMA_IRQ_UART_RX  = BIT(0),
    FLD_DMA_IRQ_UART_TX  = BIT(1),
    FLD_DMA_IRQ_RF_RX    = BIT(2),
    FLD_DMA_IRQ_RF_TX    = BIT(3),
    FLD_DMA_IRQ_AES_DECO = BIT(4),
    FLD_DMA_IRQ_AES_CODE = BIT(5),
    FLD_DMA_IRQ_PWM		 = BIT(7),
    FLD_DMA_IRQ_ALL      = 0xff,
} IRQ_DMAIrqTypeDef;


#define reg_dma_rx_rptr			REG_ADDR8(0xc28)
#define reg_dma_rx_wptr			REG_ADDR8(0xc29)

#define reg_dma_tx_rptr			REG_ADDR8(0xc2a)
enum{
	FLD_DMA_RPTR_CLR =			BIT(4),
	FLD_DMA_RPTR_NEXT =			BIT(5),
	FLD_DMA_RPTR_SET =			BIT(6),
};
#define reg_dma_tx_wptr			REG_ADDR8(0xc2b)
#define reg_dma_tx_fifo			REG_ADDR16(0xc2c)



#define reg_dma0_addrHi			REG_ADDR8(0xc40)
#define reg_dma1_addrHi			REG_ADDR8(0xc41)
#define reg_dma2_addrHi			REG_ADDR8(0xc42)
#define reg_dma3_addrHi			REG_ADDR8(0xc43)
#define reg_dma4_addrHi			REG_ADDR8(0xc44)
#define reg_dma5_addrHi			REG_ADDR8(0xc45)
#define reg_dma_ta_addrHi		REG_ADDR8(0xc46)
#define reg_dma_a3_addrHi		REG_ADDR8(0xc47)
#define reg_dma7_addrHi			REG_ADDR8(0xc48)

/****************************************************
 aes regs struct: begin  0x540
 *****************************************************/
#define reg_aes_ctrl            REG_ADDR8(0x540)
#define reg_aes_data            REG_ADDR32(0x548)
#define reg_aes_key(key_id)     reg_aes_key##key_id
#define reg_aes_key0            REG_ADDR8(0x550)
#define reg_aes_key1            REG_ADDR8(0x551)
#define reg_aes_key2            REG_ADDR8(0x552)
#define reg_aes_key3            REG_ADDR8(0x553)
#define reg_aes_key4            REG_ADDR8(0x554)
#define reg_aes_key5            REG_ADDR8(0x555)
#define reg_aes_key6            REG_ADDR8(0x556)
#define reg_aes_key7            REG_ADDR8(0x557)
#define reg_aes_key8            REG_ADDR8(0x558)
#define reg_aes_key9            REG_ADDR8(0x559)
#define reg_aes_key10            REG_ADDR8(0x55a)
#define reg_aes_key11            REG_ADDR8(0x55b)
#define reg_aes_key12            REG_ADDR8(0x55c)
#define reg_aes_key13            REG_ADDR8(0x55d)
#define reg_aes_key14            REG_ADDR8(0x55e)
#define reg_aes_key15            REG_ADDR8(0x55f)


/****************************************************
 gpio regs struct: begin  0x580
 *****************************************************/
#define reg_gpio_pa_in			REG_ADDR8(0x580)
#define reg_gpio_pa_ie			REG_ADDR8(0x581)
#define reg_gpio_pa_oen			REG_ADDR8(0x582)
#define reg_gpio_pa_out			REG_ADDR8(0x583)
#define reg_gpio_pa_pol			REG_ADDR8(0x584)
#define reg_gpio_pa_ds			REG_ADDR8(0x585)
#define reg_gpio_pa_gpio		REG_ADDR8(0x586)
#define reg_gpio_pa_irq_en		REG_ADDR8(0x587)

#define reg_gpio_pb_in			REG_ADDR8(0x588)
#define anaReg_gpioPB_ie		0xBD
#define reg_gpio_pb_oen			REG_ADDR8(0x58a)
#define reg_gpio_pb_out			REG_ADDR8(0x58b)
#define reg_gpio_pb_pol			REG_ADDR8(0x58c)
#define anaReg_gpioPB_ds		0xBF
#define reg_gpio_pb_gpio		REG_ADDR8(0x58e)
#define reg_gpio_pb_irq_en		REG_ADDR8(0x58f)

#define reg_gpio_pc_in			REG_ADDR8(0x590)
#define anaReg_gpioPC_ie		0xC0
#define reg_gpio_pc_oen			REG_ADDR8(0x592)
#define reg_gpio_pc_out			REG_ADDR8(0x593)
#define reg_gpio_pc_pol			REG_ADDR8(0x594)
#define anaReg_gpioPC_ds		0xC2
#define reg_gpio_pc_gpio		REG_ADDR8(0x596)
#define reg_gpio_pc_irq_en		REG_ADDR8(0x597)

#define reg_gpio_pd_in			REG_ADDR8(0x598)
#define reg_gpio_pd_ie			REG_ADDR8(0x599)
#define reg_gpio_pd_oen			REG_ADDR8(0x59a)
#define reg_gpio_pd_out			REG_ADDR8(0x59b)
#define reg_gpio_pd_pol			REG_ADDR8(0x59c)
#define reg_gpio_pd_ds			REG_ADDR8(0x59d)
#define reg_gpio_pd_gpio		REG_ADDR8(0x59e)
#define reg_gpio_pd_irq_en		REG_ADDR8(0x59f)

#define reg_gpio_pe_in			REG_ADDR8(0x5a0)
#define reg_gpio_pe_ie			REG_ADDR8(0x5a1)
#define reg_gpio_pe_oen			REG_ADDR8(0x5a2)
#define reg_gpio_pe_out			REG_ADDR8(0x5a3)
#define reg_gpio_pe_pol			REG_ADDR8(0x5a4)
#define reg_gpio_pe_ds			REG_ADDR8(0x5a5)
#define reg_gpio_pe_gpio		REG_ADDR8(0x5a6)

#define reg_gpio_pa_setting1	REG_ADDR32(0x580)
#define reg_gpio_pa_setting2	REG_ADDR32(0x584)
#define reg_gpio_pd_setting1	REG_ADDR32(0x598)
#define reg_gpio_pd_setting2	REG_ADDR32(0x59c)





#define reg_gpio_in(i)				REG_ADDR8(0x580+((i>>8)<<3))
#define reg_gpio_ie(i)				REG_ADDR8(0x581+((i>>8)<<3))
#define reg_gpio_oen(i)				REG_ADDR8(0x582+((i>>8)<<3))
#define reg_gpio_out(i)				REG_ADDR8(0x583+((i>>8)<<3))
#define reg_gpio_pol(i)				REG_ADDR8(0x584+((i>>8)<<3))
#define reg_gpio_ds(i)				REG_ADDR8(0x585+((i>>8)<<3))

#define reg_gpio_irq_src(i)			REG_ADDR8(0x5e0+(i>>8))


#define reg_gpio_gpio_func(i)		REG_ADDR8(0x586+((i>>8)<<3))

#define reg_gpio_irq_wakeup_en(i)	REG_ADDR8(0x587+((i>>8)<<3))  // reg_irq_mask: FLD_IRQ_GPIO_EN

#define reg_gpio_irq_risc0_en(i)  	REG_ADDR8(0x5b8 + (i >> 8))	  // reg_irq_mask: FLD_IRQ_GPIO_RISC0_EN
#define reg_gpio_irq_risc1_en(i)  	REG_ADDR8(0x5c0 + (i >> 8))	  // reg_irq_mask: FLD_IRQ_GPIO_RISC1_EN


#define    PAD_FUNC_MUX_1PA      	REG_ADDR8(0x5a8)
#define    PAD_FUNC_MUX_2PA      	REG_ADDR8(0x5a9)
#define    PAD_FUNC_MUX_1PB     	REG_ADDR8(0x5aa)
#define    PAD_FUNC_MUX_2PB     	REG_ADDR8(0x5ab)
#define    PAD_FUNC_MUX_1PC     	REG_ADDR8(0x5ac)
#define    PAD_FUNC_MUX_2PC     	REG_ADDR8(0x5ad)
#define    PAD_FUNC_MUX_1PD     	REG_ADDR8(0x5ae)
#define    PAD_FUNC_MUX_2PD     	REG_ADDR8(0x5af)


#define reg_pin_i2c_spi_out_en	    REG_ADDR8(0x5b6)
enum{
	FLD_PIN_PAGROUP_SPI_EN =	BIT_RNG(4,5),
	FLD_PIN_PBGROUP_SPI_EN =	BIT(6),
	FLD_PIN_PDGROUP_SPI_EN =	BIT(7),
};


#define reg_pin_i2c_spi_en			REG_ADDR8(0x5b7)  //poweron default  0xff
enum{
	FLD_PIN_PA3_SPI_EN =	BIT(0),
	FLD_PIN_PA4_SPI_EN =	BIT(1),
	FLD_PIN_PB6_SPI_EN =	BIT(2),
	FLD_PIN_PD7_SPI_EN =	BIT(3),
	FLD_PIN_PA3_I2C_EN =	BIT(4),
	FLD_PIN_PA4_I2C_EN =	BIT(5),
	FLD_PIN_PB6_I2C_EN =	BIT(6),
	FLD_PIN_PD7_I2C_EN =	BIT(7),
};


/****************************************************
 timer regs struct: begin  0x620
 *****************************************************/
#define reg_tmr_ctrl			REG_ADDR32(0x620)
#define reg_tmr_ctrl16			REG_ADDR16(0x620)
#define reg_tmr_ctrl8			REG_ADDR8(0x620)
enum{
	FLD_TMR0_EN =				BIT(0),
	FLD_TMR0_MODE =				BIT_RNG(1,2),
	FLD_TMR1_EN = 				BIT(3),
	FLD_TMR1_MODE =				BIT_RNG(4,5),
	FLD_TMR2_EN =				BIT(6),
	FLD_TMR2_MODE = 			BIT_RNG(7,8),
	FLD_TMR_WD_CAPT = 			BIT_RNG(9,22),
	FLD_TMR_WD_EN =				BIT(23),
	FLD_TMR0_STA =				BIT(24),
	FLD_TMR1_STA =				BIT(25),
	FLD_TMR2_STA =				BIT(26),
	FLD_CLR_WD =				BIT(27),
};
#define WATCHDOG_TIMEOUT_COEFF	18		//  check register definiton, 0x622

#define WATCHDOG_DISABLE	( reg_tmr_ctrl &= ~FLD_TMR_WD_EN )

#define reg_tmr_sta				REG_ADDR8(0x623)
enum{
	FLD_TMR_STA_TMR0 =			BIT(0),
	FLD_TMR_STA_TMR1 =			BIT(1),
	FLD_TMR_STA_TMR2 =			BIT(2),
	FLD_TMR_STA_WD =			BIT(3),
};

#define reg_tmr0_capt			REG_ADDR32(0x624)
#define reg_tmr1_capt			REG_ADDR32(0x628)
#define reg_tmr2_capt			REG_ADDR32(0x62c)
#define reg_tmr_capt(i)			REG_ADDR32(0x624 + ((i) << 2))
#define reg_tmr0_tick			REG_ADDR32(0x630)
#define reg_tmr1_tick			REG_ADDR32(0x634)
#define reg_tmr2_tick			REG_ADDR32(0x638)
#define reg_tmr_tick(i)			REG_ADDR32(0x630 + ((i) << 2))

/****************************************************
 interrupt regs struct: begin  0x640
 *****************************************************/
#define reg_irq_mask			REG_ADDR32(0x640)
#define reg_irq_pri				REG_ADDR32(0x644)
#define reg_irq_src				REG_ADDR32(0x648)
#define reg_irq_src3			REG_ADDR8(0x64a)
enum{
	FLD_IRQ_TMR0_EN =			BIT(0),
	FLD_IRQ_TMR1_EN =			BIT(1),
	FLD_IRQ_TMR2_EN =			BIT(2),
	FLD_IRQ_USB_PWDN_EN =		BIT(3),
	FLD_IRQ_DMA_EN =			BIT(4),
	FLD_IRQ_DAM_FIFO_EN =		BIT(5),
	FLD_IRQ_UART_EN 	=		BIT(6),
	FLD_IRQ_HOST_CMD_EN =		BIT(7),

	FLD_IRQ_EP0_SETUP_EN =		BIT(8),
	FLD_IRQ_EP0_DAT_EN =		BIT(9),
	FLD_IRQ_EP0_STA_EN =		BIT(10),
	FLD_IRQ_SET_INTF_EN =		BIT(11),
	FLD_IRQ_EP_DATA_EN =		BIT(12),
	FLD_IRQ_IRQ4_EN =			BIT(12),
	FLD_IRQ_ZB_RT_EN =			BIT(13),
	FLD_IRQ_SW_PWM_EN =			BIT(14),  //irq_software | irq_pwm
	FLD_IRQ_AN_EN =				BIT(15),

	FLD_IRQ_USB_250US_EN =		BIT(16),
	FLD_IRQ_USB_RST_EN =		BIT(17),
	FLD_IRQ_GPIO_EN =			BIT(18),
	FLD_IRQ_PM_EN =				BIT(19),
	FLD_IRQ_SYSTEM_TIMER =		BIT(20),
	FLD_IRQ_GPIO_RISC0_EN =		BIT(21),
	FLD_IRQ_GPIO_RISC1_EN =		BIT(22),
	FLD_IRQ_CMP_EN		  = 	BIT(23),

	FLD_IRQ_EN =				BIT_RNG(24,31),
	FLD_IRQ_ALL           =     0XFFFFFFFF,
};
#define reg_irq_en				REG_ADDR8(0x643)




/****************************************************
SYSTEM_TIMER register table (base address 0x0740)
 *****************************************************/
#define reg_system_tick			REG_ADDR32(0x740)
#define reg_system_tick_irq		REG_ADDR32(0x744)
#define reg_system_wakeup_tick	REG_ADDR32(0x748)
#define reg_system_tick_mode	REG_ADDR8(0x74c)
#define reg_system_tick_ctrl	REG_ADDR8(0x74f)

enum {
	FLD_SYSTEM_TICK_START	=		BIT(0),
	FLD_SYSTEM_TICK_STOP	=		BIT(1),
	FLD_SYSTEM_TICK_RUNNING	=		BIT(1),

	FLD_SYSTEM_TICK_IRQ_EN  = 		BIT(1),
};



/****************************************************
 PWM regs define:  begin  0x780
 *****************************************************/
#define reg_pwm_enable			REG_ADDR8(0x780)
#define reg_pwm0_enable			REG_ADDR8(0x781)
#define reg_pwm_clk				REG_ADDR8(0x782)

#define reg_pwm0_mode			REG_ADDR8(0x783)


#define reg_pwm_invert			REG_ADDR8(0x784)
#define reg_pwm_n_invert		REG_ADDR8(0x785)
#define reg_pwm_pol				REG_ADDR8(0x786)

#define reg_pwm_phase(i)		REG_ADDR16(0x788 + (i << 1))
#define reg_pwm_cycle(i)		REG_ADDR32(0x794 + (i << 2))   //<15:0>: TCMP 0~5  <31:16>: TMAX 0~5
#define reg_pwm_cmp(i)			REG_ADDR16(0x794 + (i << 2))   //TCMP 0~5
#define reg_pwm_max(i)			REG_ADDR16(0x796 + (i << 2))   //TMAX 0~5
enum{
	FLD_PWM_CMP  = 				BIT_RNG(0,15),
	FLD_PWM_MAX  = 				BIT_RNG(16,31),
};

#define reg_pwm_pulse_num		REG_ADDR16(0x7ac)

#define reg_pwm_irq_mask		REG_ADDR8(0x7b0)
#define reg_pwm_irq_sta			REG_ADDR8(0x7b1)
enum{
	FLD_IRQ_PWM0_PNUM =					BIT(0),
	FLD_IRQ_PWM0_IR_DMA_FIFO_DONE  =	BIT(1),
	FLD_IRQ_PWM0_FRAME =				BIT(2),
	FLD_IRQ_PWM1_FRAME =				BIT(3),
	FLD_IRQ_PWM2_FRAME =				BIT(4),
	FLD_IRQ_PWM3_FRAME =				BIT(5),
	FLD_IRQ_PWM4_FRAME 	=				BIT(6),
	FLD_IRQ_PWM5_FRAME =				BIT(7),
};



#define reg_pwm0_fifo_mode_irq_mask		REG_ADDR8(0x7b2)
#define reg_pwm0_fifo_mode_irq_sta		REG_ADDR8(0x7b3)
enum{
	FLD_IRQ_PWM0_FIFO_MODE_STOP =		BIT(0),
	FLD_IRQ_PWM0_FIFO_MODE_CNT  =		BIT(1),
};



#define reg_pwm_tcmp0_shadow		REG_ADDR16(0x7c4)   //<15:0>: TCMP 0~5  <31:16>: TMAX 0~5
#define reg_pwm_tmax0_shadow		REG_ADDR16(0x7c6)   //TCMP 0~5

//////////////////////////////////////////////////////////////
// DFIFO
//////////////////////////////////////////////////////////////

#define reg_fifo0_data			REG_ADDR32(0x800)
#define reg_fifo1_data			REG_ADDR32(0x900)
#define reg_fifo2_data			REG_ADDR32(0xa00)

/****************************************************
 dfifo regs define:  begin  0xb00
 *****************************************************/
#define reg_dfifo0_addr			REG_ADDR16(0xb00)
#define reg_dfifo0_size			REG_ADDR8(0xb02)
#define reg_dfifo0_addHi		REG_ADDR8(0xb03)  //default 0x04, no need set

#define reg_dfifo1_addr			REG_ADDR16(0xb04)
#define reg_dfifo1_size			REG_ADDR8(0xb06)
#define reg_dfifo1_addHi		REG_ADDR8(0xb07)  //default 0x04, no need set

//misc channel only use dfifo2
#define reg_dfifo2_addr			REG_ADDR16(0xb08)
#define reg_dfifo2_size			REG_ADDR8(0xb0a)
#define reg_dfifo2_addHi		REG_ADDR8(0xb0b)  //default 0x04, no need set

#define reg_dfifo_audio_addr		reg_dfifo0_addr
#define reg_dfifo_audio_size		reg_dfifo0_size

#define reg_dfifo_misc_chn_addr		reg_dfifo2_addr
#define reg_dfifo_misc_chn_size		reg_dfifo2_size


#define reg_dfifo0_l_level		REG_ADDR8(0xb0c)  //dfifo0  low int threshold(wptr - rptr)
#define reg_dfifo0_h_level		REG_ADDR8(0xb0d)  //dfifo0 high int threshold(wptr - rptr)
#define reg_dfifo1_h_level		REG_ADDR8(0xb0e)  //dfifo1 high int threshold(wptr - rptr)
#define reg_dfifo2_h_level		REG_ADDR8(0xb0f)  //dfifo2 high int threshold(wptr - rptr)


#define	reg_dfifo_mode			REG_ADDR8(0xb10)
enum{
	FLD_AUD_DFIFO0_IN 		= BIT(0),
	FLD_AUD_DFIFO1_IN 		= BIT(1),
	FLD_AUD_DFIFO2_IN 		= BIT(2),
	FLD_AUD_DFIFO0_OUT 		= BIT(3),
	FLD_AUD_DFIFO0_L_INT	= BIT(4),
	FLD_AUD_DFIFO0_H_INT	= BIT(5),
	FLD_AUD_DFIFO1_H_INT	= BIT(6),
	FLD_AUD_DFIFO2_H_INT	= BIT(7),
};



#define	reg_dfifo_ain			REG_ADDR8(0xb11)
enum{
	FLD_AUD_DMIC0_DATA_IN_RISING_EDGE = BIT(0),
	FLD_AUD_DMIC1_DATA_IN_RISING_EDGE = BIT(1),
	FLD_AUD_INPUT_SELECT     		  = BIT_RNG(2,3),
	FLD_AUD_INPUT_MONO_MODE	   		  = BIT(4),
	FLD_AUD_DECIMATION_FILTER_BYPASS  = BIT(5),
	FLD_AUD_DMIC_RISING_EDGE_BYPASS   = BIT(6),
	FLD_AUD_DMIC_FALLING_EDGE_BYPASS  = BIT(7),

};

enum{  //core_b11<0> <1>  audio dmic_n  rising/falling edge
	AUDIO_DMIC_DATA_IN_RISING_EDGE 		= 0,
	AUDIO_DMIC_DATA_IN_FALLING_EDGE 	= 1,
};

enum{  //core_b11<3:2>  audio input select
	AUDIO_INPUT_USB  = 0x00,
	AUDIO_INPUT_I2S  = 0x01,
	AUDIO_INPUT_AMIC = 0x02,
	AUDIO_INPUT_DMIC = 0x03,
};



#define reg_dfifo_dec_ratio		REG_ADDR8(0xb12)

#define reg_dfifo_irq_status	REG_ADDR8(0xb13)

#define reg_dfifo0_rptr			REG_ADDR16(0xb14)
#define reg_dfifo0_wptr			REG_ADDR16(0xb16)

#define reg_dfifo1_rptr			REG_ADDR16(0xb18)
#define reg_dfifo1_wptr			REG_ADDR16(0xb1a)

#define reg_dfifo2_rptr			REG_ADDR16(0xb1c)
#define reg_dfifo2_wptr			REG_ADDR16(0xb1e)


#define reg_audio_wptr			reg_dfifo0_wptr
static inline unsigned short get_mic_wr_ptr (void)
{
	return reg_audio_wptr >>1;
}

#define reg_dfifo0_num			REG_ADDR16(0xb20)
#define reg_dfifo1_num			REG_ADDR16(0xb24)
#define reg_dfifo2_num			REG_ADDR16(0xb28)

#define reg_dfifo0_manual		REG_ADDR8(0xb2c)
enum{
	FLD_DFIFO_MANUAL_MODE_EN	= BIT(0),
};

#define reg_dfifo0_man_dat		REG_ADDR32(0xb30)



#define	reg_alc_sft				REG_ADDR8(0xb34)

#define	reg_audio_dec_mode		REG_ADDR8(0xb35)
enum{
	FLD_AUD_LNR_VALID_SEL	= BIT(0),
	FLD_AUD_CIC_MODE  		= BIT(3)
};


#define	reg_adc_mul				REG_ADDR8(0xb36)
#define	reg_adc_bias			REG_ADDR8(0xb37)

#define	reg_aud_alc_hpf_lpf_ctrl	 REG_ADDR8(0xb40)
enum {
	FLD_AUD_IN_HPF_SFT		=	BIT_RNG(0,3),
	FLD_AUD_IN_HPF_BYPASS	=	BIT(4),
	FLD_AUD_IN_ALC_BYPASS	=	BIT(5),
	FLD_AUD_IN_LPF_BYPASS   =   BIT(6),
	FLD_DOUBLE_DOWN_SAMPLING_ON =   BIT(7)
};



#define reg_aud_alc_vol_l_chn		REG_ADDR8(0xb41)  //default 0x00, will be 0x20 after ana_34 set to 0x80
#define reg_aud_alc_vol_r_chn		REG_ADDR8(0xb42)  //default 0x00, will be 0x20 after ana_34 set to 0x80
enum{
	FLD_AUD_ALC_MIN_VOLUME_IN_DIGITAL_MODE 	= BIT_RNG(0,5),
	FLD_AUD_ALC_MIN_PGA_IN_ANALOG_MODE	    = BIT_RNG(0,6),
	//alc digital mode left/right channel regulate mode select: 1 for auto regulate; 0 for manual regulate
	FLD_AUD_ALC_DIGITAL_MODE_AUTO_REGULATE_EN	= BIT(7),
};


#define reg_alc_vol_h				REG_ADDR8(0xb43)
enum{
	FLD_AUD_ALC_MAX_VOLUME_IN_DIGITAL_MODE 	= BIT_RNG(0,5),
	FLD_AUD_ALC_MAX_PGA_IN_ANALOG_MODE	    = BIT_RNG(0,6),
};


#define reg_alc_vol_th_h			REG_ADDR16(0xb44)
#define reg_alc_vol_th_l			REG_ADDR16(0xb46)
#define reg_alc_vol_thn				REG_ADDR16(0xb48)
#define reg_alc_vad_thn				REG_ADDR16(0xb4a)

#define reg_alc_vol_step			REG_ADDR8(0xb4c)

#define reg_alc_vol_l				REG_ADDR8(0xb4d)
#define reg_alc_vol_r				REG_ADDR8(0xb4e)

#define reg_alc_peak_tick			REG_ADDR16(0xb50)

#define reg_alc_dec_tick			REG_ADDR8(0xb52)
#define reg_alc_noi_tick			REG_ADDR8(0xb53)


#define reg_aud_alc_cfg				REG_ADDR8(0xb54)   //default 0x00, will be 0x02 after ana_34 set to 0x80
enum{
	FLD_AUD_ALC_ANALOG_MODE_EN 		= BIT(0),    //alc mode select:   1 for analog mode;  0 for digital mode
	FLD_AUD_ALC_NOISE_EN 			= BIT(1),
};

#define reg_alc_coef_iir			REG_ADDR8(0xb55)
#define reg_alc_dat_mask			REG_ADDR8(0xb56)
#define reg_alc_inc_spd				REG_ADDR8(0xb57)
#define reg_alc_inc_max				REG_ADDR8(0xb58)
#define reg_alc_dec_spd				REG_ADDR8(0xb59)
#define reg_alc_dec_max				REG_ADDR8(0xb5a)
#define reg_alc_noi_spd				REG_ADDR8(0xb5b)
#define reg_alc_noi_max				REG_ADDR8(0xb5c)

#define reg_pga_gain_init			REG_ADDR8(0xb5d)
#define reg_pga_gain_l				REG_ADDR8(0xb5e)   //used to check current left  channel gain in analog mode auto regulate
#define reg_pga_gain_r				REG_ADDR8(0xb5f)   //used to check current right channel gain in analog mode auto regulate
#define reg_pga_man_speed			REG_ADDR8(0xb60)

#define reg_pga_man_target_l		REG_ADDR8(0xb61)
#define reg_pga_value_l				REG_ADDR8(0xb62)
#define reg_pga_fix_value			REG_ADDR8(0xb63)
#define reg_pga_r_l					REG_ADDR8(0xb64)
#define reg_pga_man_target_r		REG_ADDR8(0xb65)
#define reg_pga_value_r				REG_ADDR8(0xb66)










/*******************************************************************************************************************************
 *
 *									Analog  Register Table
 *
 ******************************************************************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////
//   analog register table 3.3 V
/////////////////////////////////////////////////////////////////////////////////////



//afe_0x06
//    BIT(4):    Power down Bandgap in PLL
//    	1: Power down;     0: Power up
#define anareg_06					0x06
enum{
	FLD_PLL_BG_POWER_DOWN = BIT(4),
};



/////////////////////////////////////////////////////////////////////////////////////
//   analog register table 1.8 V
/////////////////////////////////////////////////////////////////////////////////////



#define anareg_82					0x82
enum{
	FLD_DCCC_DOUBLER_POWER_DOWN	  = BIT(3),
	FLD_CLK_48M_TO_RX_EN 		  = BIT(4),
	FLD_CLK_48M_TO_DIG_EN 		  = BIT(5),
	FLD_CLK_24M_TO_SAR_EN 		  = BIT(6),
	FLD_CLK_48M_TO_CAL_DIG_MAN_EN = BIT(7),
};




#if defined(__cplusplus)
}
#endif




