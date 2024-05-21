/********************************************************************************************************
 * @file    usb_reg.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef USB_REG_H
#define USB_REG_H
#include "soc.h"
/*******************************      usb registers: 0x100800      ******************************/
#define 	REG_USB_BASE		   0x100800

#define reg_ctrl_ep_ptr			REG_ADDR8(REG_USB_BASE)
#define reg_ctrl_ep_dat			REG_ADDR8(REG_USB_BASE+0x01)
#define reg_ctrl_ep_ctrl		REG_ADDR8(REG_USB_BASE+0x02)
enum{
	FLD_EP_DAT_ACK  = 			BIT(0),
	FLD_EP_DAT_STALL =			BIT(1),
	FLD_EP_STA_ACK = 			BIT(2),
	FLD_EP_STA_STALL = 			BIT(3),
	FLD_CTRL_EP_IRQ_TRANS  = 	BIT_RNG(4,7),

};

#define reg_ctrl_ep_irq_sta		REG_ADDR8(REG_USB_BASE+0x03)
enum{
	FLD_USB_IRQ_RESET_STATUS    = BIT(0),
	FLD_USB_IRQ_250US_STATUS    = BIT(1),
	FLD_USB_IRQ_SUSPEND_STATUS  = BIT(2),
	 /*BIT(3) reserved  */
	FLD_CTRL_EP_IRQ_SETUP  		= BIT(4),
	FLD_CTRL_EP_IRQ_DATA 	 	= BIT(5),
	FLD_CTRL_EP_IRQ_STA  		= BIT(6),
	FLD_CTRL_EP_IRQ_INTF  		= BIT(7),
};

#define reg_ctrl_ep_irq_mode	REG_ADDR8(REG_USB_BASE+0x04)
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

#define reg_usb_ctrl			REG_ADDR8(REG_USB_BASE+0x05)
enum{
	FLD_USB_CTRL_AUTO_CLK = 	BIT(0),
	FLD_USB_CTRL_LOW_SPD = 		BIT(1),
	FLD_USB_CTRL_LOW_JITT =		BIT(2),
	FLD_USB_CTRL_TST_MODE = 	BIT(3),
};

#define reg_usb_cycl_cali		REG_ADDR8(REG_USB_BASE+0x06)
#define reg_usb_cych_cali		REG_ADDR8(REG_USB_BASE+0x07)
#define reg_usb_mdev			REG_ADDR8(REG_USB_BASE+0x0a)
enum{
	FLD_USB_MDEV_SELF_PWR = 	BIT(0),
	FLD_USB_MDEV_SUSP_STA = 	BIT(1),
	FLD_USB_MDEV_WAKE_FEA = 	BIT(2),
	FLD_USB_MDEV_VEND_CMD = 	BIT(3),
	FLD_USB_MDEV_VEND_DIS = 	BIT(4),
};

#define reg_usb_host_conn		REG_ADDR8(REG_USB_BASE+0x0b)
#define reg_usb_sups_cyc_cali	REG_ADDR8(REG_USB_BASE+0x0c)
#define reg_usb_intf_alt		REG_ADDR8(REG_USB_BASE+0x0d)
#define reg_usb_edp_en			REG_ADDR8(REG_USB_BASE+0x0e)
typedef enum{
	FLD_USB_EDP8_EN 		= 	BIT(0),	// printer
	FLD_USB_EDP1_EN 		= 	BIT(1),	// keyboard
	FLD_USB_EDP2_EN 		= 	BIT(2),	// mouse
	FLD_USB_EDP3_EN 		= 	BIT(3),
	FLD_USB_EDP4_EN 		= 	BIT(4),
	FLD_USB_EDP5_EN 		= 	BIT(5),	// printer
	FLD_USB_EDP6_EN 		= 	BIT(6),	// audio
	FLD_USB_EDP7_EN 		= 	BIT(7),	// audio
}usb_ep_en_e;

#define reg_usb_irq_mask		REG_ADDR8(REG_USB_BASE+0x0f)
enum{
	FLD_USB_IRQ_RESET_MASK   = 	BIT(0),
	FLD_USB_IRQ_250US_MASK 	 = 	BIT(1),
	FLD_USB_IRQ_SUSPEND_MASK = 	BIT(2),
	FLD_USB_IRQ_RESET_LVL	 = 	BIT(3),
	FLD_USB_IRQ_250US_LVL	 = 	BIT(4),
};


#define reg_usb_ep8_ptr			REG_ADDR8(REG_USB_BASE+0x10)
#define reg_usb_ep1_ptr			REG_ADDR8(REG_USB_BASE+0x11)
#define reg_usb_ep2_ptr			REG_ADDR8(REG_USB_BASE+0x12)
#define reg_usb_ep3_ptr			REG_ADDR8(REG_USB_BASE+0x13)
#define reg_usb_ep4_ptr			REG_ADDR8(REG_USB_BASE+0x14)
#define reg_usb_ep5_ptr			REG_ADDR8(REG_USB_BASE+0x15)
#define reg_usb_ep6_ptr			REG_ADDR8(REG_USB_BASE+0x16)
#define reg_usb_ep7_ptr			REG_ADDR8(REG_USB_BASE+0x17)
#define reg_usb_ep_ptr(i)		REG_ADDR8(REG_USB_BASE+0x10+((i) & 0x07))

#define reg_usb_ep8_ptrh		REG_ADDR8(REG_USB_BASE+0x18)
#define reg_usb_ep1_ptrh		REG_ADDR8(REG_USB_BASE+0x19)
#define reg_usb_ep2_ptrh		REG_ADDR8(REG_USB_BASE+0x1a)
#define reg_usb_ep3_ptrh		REG_ADDR8(REG_USB_BASE+0x1b)
#define reg_usb_ep4_ptrh		REG_ADDR8(REG_USB_BASE+0x1c)
#define reg_usb_ep5_ptrh		REG_ADDR8(REG_USB_BASE+0x1d)
#define reg_usb_ep6_ptrh		REG_ADDR8(REG_USB_BASE+0x1e)
#define reg_usb_ep7_ptrh		REG_ADDR8(REG_USB_BASE+0x1f)
#define reg_usb_ep_ptrh(i)		REG_ADDR8(REG_USB_BASE+0x18+((i) & 0x07))


#define reg_usb_ep8_dat			REG_ADDR8(REG_USB_BASE+0x20)
#define reg_usb_ep1_dat			REG_ADDR8(REG_USB_BASE+0x21)
#define reg_usb_ep2_dat			REG_ADDR8(REG_USB_BASE+0x22)
#define reg_usb_ep3_dat			REG_ADDR8(REG_USB_BASE+0x23)
#define reg_usb_ep4_dat			REG_ADDR8(REG_USB_BASE+0x24)
#define reg_usb_ep5_dat			REG_ADDR8(REG_USB_BASE+0x25)
#define reg_usb_ep6_dat			REG_ADDR8(REG_USB_BASE+0x26)
#define reg_usb_ep7_dat			REG_ADDR8(REG_USB_BASE+0x27)
#define reg_usb_ep_dat(i)		REG_ADDR8(REG_USB_BASE+0x20+((i) & 0x07))

#define reg_usb_ep8_ctrl		REG_ADDR8(REG_USB_BASE+0x28)
#define reg_usb_ep1_ctrl		REG_ADDR8(REG_USB_BASE+0x29)
#define reg_usb_ep2_ctrl		REG_ADDR8(REG_USB_BASE+0x2a)
#define reg_usb_ep3_ctrl		REG_ADDR8(REG_USB_BASE+0x2b)
#define reg_usb_ep4_ctrl		REG_ADDR8(REG_USB_BASE+0x2c)
#define reg_usb_ep5_ctrl		REG_ADDR8(REG_USB_BASE+0x2d)
#define reg_usb_ep6_ctrl		REG_ADDR8(REG_USB_BASE+0x2e)
#define reg_usb_ep7_ctrl		REG_ADDR8(REG_USB_BASE+0x2f)
#define reg_usb_ep_ctrl(i)		REG_ADDR8(REG_USB_BASE+0x28+((i) & 0x07))

enum{
	FLD_USB_EP_BUSY = 			BIT(0),
	FLD_USB_EP_STALL =			BIT(1),
	FLD_USB_EP_DAT0 =			BIT(2),
	FLD_USB_EP_DAT1 =			BIT(3),
	FLD_USB_EP_MONO =			BIT(6),
	FLD_USB_EP_EOF_ISO =		BIT(7),
};


#define reg_usb_ep8_buf_addr	REG_ADDR8(REG_USB_BASE+0x30)
#define reg_usb_ep1_buf_addr	REG_ADDR8(REG_USB_BASE+0x31)
#define reg_usb_ep2_buf_addr	REG_ADDR8(REG_USB_BASE+0x32)
#define reg_usb_ep3_buf_addr	REG_ADDR8(REG_USB_BASE+0x33)
#define reg_usb_ep4_buf_addr	REG_ADDR8(REG_USB_BASE+0x34)
#define reg_usb_ep5_buf_addr	REG_ADDR8(REG_USB_BASE+0x35)
#define reg_usb_ep6_buf_addr	REG_ADDR8(REG_USB_BASE+0x36)
#define reg_usb_ep7_buf_addr	REG_ADDR8(REG_USB_BASE+0x37)
#define reg_usb_ep_buf_addr(i)	REG_ADDR8(REG_USB_BASE+0x30+((i) & 0x07))

#define reg_usb_ep8_buf_addrh	REG_ADDR8(REG_USB_BASE+0x38)
#define reg_usb_ep1_buf_addrh	REG_ADDR8(REG_USB_BASE+0x39)
#define reg_usb_ep2_buf_addrh	REG_ADDR8(REG_USB_BASE+0x3a)
#define reg_usb_ep3_buf_addrh	REG_ADDR8(REG_USB_BASE+0x3b)
#define reg_usb_ep4_buf_addrh	REG_ADDR8(REG_USB_BASE+0x3c)
#define reg_usb_ep5_buf_addrh	REG_ADDR8(REG_USB_BASE+0x3d)
#define reg_usb_ep6_buf_addrh	REG_ADDR8(REG_USB_BASE+0x3e)
#define reg_usb_ep7_buf_addrh	REG_ADDR8(REG_USB_BASE+0x3f)
#define reg_usb_ep_buf_addrh(i)	REG_ADDR8(REG_USB_BASE+0x38+((i) & 0x07))


#define reg_usb_iso_mode		REG_ADDR8(REG_USB_BASE+0x40)

#define reg_usb_ep_irq_status	REG_ADDR8(REG_USB_BASE+0x41)
#define reg_usb_ep_irq_mask		REG_ADDR8(REG_USB_BASE+0x42)
typedef enum{
	FLD_USB_EDP8_IRQ 		= 	BIT(0),
	FLD_USB_EDP1_IRQ 		= 	BIT(1),
	FLD_USB_EDP2_IRQ 		= 	BIT(2),
	FLD_USB_EDP3_IRQ 		= 	BIT(3),
	FLD_USB_EDP4_IRQ 		= 	BIT(4),
	FLD_USB_EDP5_IRQ 		= 	BIT(5),
	FLD_USB_EDP6_IRQ 		= 	BIT(6),
	FLD_USB_EDP7_IRQ 		= 	BIT(7),
}usb_ep_irq_e ;


#define reg_usb_ep8_send_max	REG_ADDR8(REG_USB_BASE+0x43)
#define reg_usb_ep8_send_thres	REG_ADDR8(REG_USB_BASE+0x44)
#define reg_usb_ep8_fifo_mode	REG_ADDR8(REG_USB_BASE+0x45)
enum{
	FLD_USB_ENP8_FIFO_MODE =	BIT(0),
	FLD_USB_ENP8_FULL_FLAG =	BIT(1),
};

#define reg_usb_ep_max_size		REG_ADDR8(REG_USB_BASE+0x46)
#define reg_usb_ep_tick			REG_ADDR8(REG_USB_BASE+0x47)
#define reg_usb_ram_ctrl		REG_ADDR8(REG_USB_BASE+0x48)

enum{
	FLD_USB_CEN_PWR_DN =		BIT(0),
	FLD_USB_CLK_PWR_DN =		BIT(1),
	FLD_USB_WEN_PWR_DN =		BIT(3),
	FLD_USB_CEN_FUNC =			BIT(4),
};



#define reg_usb_mic_dat0		REG_ADDR16(0x1800)
#define reg_usb_mic_dat1		REG_ADDR16(0x1802)




#endif
