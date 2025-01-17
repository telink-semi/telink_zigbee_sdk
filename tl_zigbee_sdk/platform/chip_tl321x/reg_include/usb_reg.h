/********************************************************************************************************
 * @file    usb_reg.h
 *
 * @brief   This is the header file for TL321X
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
#ifndef USB_REG_H
#define USB_REG_H
#include "soc.h"
/*******************************      usb registers: 0x118000      ******************************/
#define REG_USB_BASE     0x118000

#define reg_ctrl_ep_ptr  REG_ADDR8(REG_USB_BASE)
#define reg_ctrl_ep_dat  REG_ADDR8(REG_USB_BASE + 0x01)

#define reg_ctrl_ep_ctrl REG_ADDR8(REG_USB_BASE + 0x02)

enum
{
    FLD_EP_DAT_ACK     = BIT(0),
    FLD_EP_DAT_BUSY    = BIT(0),
    FLD_EP_DAT_STALL   = BIT(1),
    FLD_EP_STA_ACK     = BIT(2),
    FLD_EP_STA_STALL   = BIT(3),
    FLD_EP_IRQ_SETADDR = BIT(6),
};

#define reg_ctrl_ep_irq_sta REG_ADDR8(REG_USB_BASE + 0x03)

enum
{
    FLD_USB_IRQ_RESET_STATUS   = BIT(0),
    FLD_USB_IRQ_250US_STATUS   = BIT(1),
    FLD_USB_IRQ_SUSPEND_STATUS = BIT(2),
    FLD_USB_IRQ_SOF_STATUS     = BIT(3),
    FLD_CTRL_EP_IRQ_SETUP      = BIT(4),
    FLD_CTRL_EP_IRQ_DATA       = BIT(5),
    FLD_CTRL_EP_IRQ_STA        = BIT(6),
    FLD_CTRL_EP_IRQ_INTF       = BIT(7),
};

#define reg_ctrl_ep_irq_mode REG_ADDR8(REG_USB_BASE + 0x04)

enum
{
    FLD_CTRL_EP_AUTO_ADDR = BIT(0),
    FLD_CTRL_EP_AUTO_CFG  = BIT(1),
    FLD_CTRL_EP_AUTO_INTF = BIT(2),
    FLD_CTRL_EP_AUTO_STA  = BIT(3),
    FLD_CTRL_EP_AUTO_SYN  = BIT(4),
    FLD_CTRL_EP_AUTO_DESC = BIT(5),
    FLD_CTRL_EP_AUTO_FEAT = BIT(6),
    FLD_CTRL_EP_AUTO_STD  = BIT(7),
};

#define reg_usb_ctrl REG_ADDR8(REG_USB_BASE + 0x05)

typedef enum
{
    FLD_USB_AUTO_HALT_CLR    = BIT(0), /* enable auto clear halt feature(STALL) after either a SetConfiguration() or SetInterface() */
    FLD_USB_AUTO_HALT_STALL  = BIT(1), /* enable auto decoding the halt feature(STALL) by hardware */
    FLD_USB_AUTO_HALT_TOGGLE = BIT(2), /* enable auto decoding the data toggle */
    FLD_USB_AUTO_WKUP_FEA    = BIT(3), /* enable auto decoding the wakeup feature command */
} usb_hw_feature_e;

#define reg_usb_cycl_cali    REG_ADDR8(REG_USB_BASE + 0x06)
#define reg_usb_cych_cali    REG_ADDR8(REG_USB_BASE + 0x07)
#define reg_usb_edp0_udc_cnt REG_ADDR8(REG_USB_BASE + 0x08)

#define reg_usb_edp0_size    REG_ADDR8(REG_USB_BASE + 0x09)

enum
{
    FLD_USB_CTR_EP_SIZE      = BIT_RNG(0, 1), /* 0-8bytes,1-16bytes,2-32bytes,3-64bytes, */
    FLD_USB_CTR_EP_LENS      = BIT_RNG(2, 3),
    FLD_USB_IRQ_RESET_LVL    = BIT(4),
    FLD_USB_IRQ_250US_LVL    = BIT(5),
    FLD_USB_IRQ_SETADDR_MASK = BIT(6),
    FLD_USB_IRQ_SOF_LVL      = BIT(7),
};

#define reg_usb_mdev REG_ADDR8(REG_USB_BASE + 0x0a)

enum
{
    FLD_USB_MDEV_SELF_PWR     = BIT(0),
    FLD_USB_MDEV_SET_WAKE_FEA = BIT(1),
    FLD_USB_MDEV_WAKE_FEA     = BIT(2),
    FLD_USB_MDEV_VEND_CMD     = BIT(3),
    FLD_USB_MDEV_VEND_DIS     = BIT(4),
    FLD_USB_MDEV_MODEL_SEL    = BIT_RNG(5, 6),
    FLD_USB_CONFIG_NOW        = BIT(7),
};

#define reg_usb_addr REG_ADDR8(REG_USB_BASE + 0x0b)

enum
{
    FLD_USB_ADDRESS      = BIT_RNG(0, 6),
    FLD_USB_SET_ADDR_ERR = BIT(7),
};

#define reg_usb_sups_cyc_cali REG_ADDR8(REG_USB_BASE + 0x0c)

enum
{
    FLD_USB_SUSPEND_CNT  = BIT_RNG(0, 4),
    FLD_USB_CTR_EP_STALL = BIT(5),
    FLD_USB_HOLD0        = BIT_RNG(6, 7)
};

#define reg_usb_intf_alt REG_ADDR8(REG_USB_BASE + 0x0d)

#define reg_usb_edp_en   REG_ADDR8(REG_USB_BASE + 0x0e)

typedef enum
{
    FLD_USB_EDP8_EN = BIT(0), // printer
    FLD_USB_EDP1_EN = BIT(1), // keyboard
    FLD_USB_EDP2_EN = BIT(2), // mouse
    FLD_USB_EDP3_EN = BIT(3),
    FLD_USB_EDP4_EN = BIT(4),
    FLD_USB_EDP5_EN = BIT(5), // printer
    FLD_USB_EDP6_EN = BIT(6), // audio
    FLD_USB_EDP7_EN = BIT(7), // audio
} usb_ep_en_e;

#define reg_usb_irq_mask REG_ADDR8(REG_USB_BASE + 0x0f)

enum
{
    FLD_USB_IRQ_RESET_MASK   = BIT(0),
    FLD_USB_IRQ_250US_MASK   = BIT(1),
    FLD_USB_IRQ_SUSPEND_MASK = BIT(2),
    FLD_USB_IRQ_SOF_MASK     = BIT(3),
    FLD_USB_IRQ_SETUP_MASK   = BIT(4),
    FLD_USB_IRQ_DATA_MASK    = BIT(5),
    FLD_USB_IRQ_STATUS_MASK  = BIT(6),
    FLD_USB_IRQ_SETINF_MASK  = BIT(7),
};

#define reg_usb_ep8_ptr    REG_ADDR8(REG_USB_BASE + 0x10)
#define reg_usb_ep1_ptr    REG_ADDR8(REG_USB_BASE + 0x11)
#define reg_usb_ep2_ptr    REG_ADDR8(REG_USB_BASE + 0x12)
#define reg_usb_ep3_ptr    REG_ADDR8(REG_USB_BASE + 0x13)
#define reg_usb_ep4_ptr    REG_ADDR8(REG_USB_BASE + 0x14)
#define reg_usb_ep5_ptr    REG_ADDR8(REG_USB_BASE + 0x15)
#define reg_usb_ep6_ptr    REG_ADDR8(REG_USB_BASE + 0x16)
#define reg_usb_ep7_ptr    REG_ADDR8(REG_USB_BASE + 0x17)
#define reg_usb_ep_ptr(i)  REG_ADDR8(REG_USB_BASE + 0x10 + ((i) & 0x07))

#define reg_usb_ep8_ptrh   REG_ADDR8(REG_USB_BASE + 0x18)
#define reg_usb_ep1_ptrh   REG_ADDR8(REG_USB_BASE + 0x19)
#define reg_usb_ep2_ptrh   REG_ADDR8(REG_USB_BASE + 0x1a)
#define reg_usb_ep3_ptrh   REG_ADDR8(REG_USB_BASE + 0x1b)
#define reg_usb_ep4_ptrh   REG_ADDR8(REG_USB_BASE + 0x1c)
#define reg_usb_ep5_ptrh   REG_ADDR8(REG_USB_BASE + 0x1d)
#define reg_usb_ep6_ptrh   REG_ADDR8(REG_USB_BASE + 0x1e)
#define reg_usb_ep7_ptrh   REG_ADDR8(REG_USB_BASE + 0x1f)
#define reg_usb_ep_ptrh(i) REG_ADDR8(REG_USB_BASE + 0x18 + ((i) & 0x07))

#define reg_usb_ep8_dat    REG_ADDR8(REG_USB_BASE + 0x20)
#define reg_usb_ep1_dat    REG_ADDR8(REG_USB_BASE + 0x21)
#define reg_usb_ep2_dat    REG_ADDR8(REG_USB_BASE + 0x22)
#define reg_usb_ep3_dat    REG_ADDR8(REG_USB_BASE + 0x23)
#define reg_usb_ep4_dat    REG_ADDR8(REG_USB_BASE + 0x24)
#define reg_usb_ep5_dat    REG_ADDR8(REG_USB_BASE + 0x25)
#define reg_usb_ep6_dat    REG_ADDR8(REG_USB_BASE + 0x26)
#define reg_usb_ep7_dat    REG_ADDR8(REG_USB_BASE + 0x27)
#define reg_usb_ep_dat(i)  REG_ADDR8(REG_USB_BASE + 0x20 + ((i) & 0x07))

#define reg_usb_ep8_ctrl   REG_ADDR8(REG_USB_BASE + 0x28)
#define reg_usb_ep1_ctrl   REG_ADDR8(REG_USB_BASE + 0x29)
#define reg_usb_ep2_ctrl   REG_ADDR8(REG_USB_BASE + 0x2a)
#define reg_usb_ep3_ctrl   REG_ADDR8(REG_USB_BASE + 0x2b)
#define reg_usb_ep4_ctrl   REG_ADDR8(REG_USB_BASE + 0x2c)
#define reg_usb_ep5_ctrl   REG_ADDR8(REG_USB_BASE + 0x2d)
#define reg_usb_ep6_ctrl   REG_ADDR8(REG_USB_BASE + 0x2e)
#define reg_usb_ep7_ctrl   REG_ADDR8(REG_USB_BASE + 0x2f)

#define reg_usb_ep_ctrl(i) REG_ADDR8(REG_USB_BASE + 0x28 + ((i) & 0x07))

enum
{
    FLD_USB_EP_ACK     = BIT(0),
    FLD_USB_EP_BUSY    = BIT(0), // read only
    FLD_USB_EP_STALL   = BIT(1),
    FLD_USB_EP_DAT0    = BIT(2),
    FLD_USB_EP_DAT1    = BIT(3),
    FLD_USB_EP_MONO    = BIT(6),
    FLD_USB_EP_EOF_ISO = BIT(7),
};

#define reg_usb_ep8_buf_addr    REG_ADDR8(REG_USB_BASE + 0x30)
#define reg_usb_ep1_buf_addr    REG_ADDR8(REG_USB_BASE + 0x31)
#define reg_usb_ep2_buf_addr    REG_ADDR8(REG_USB_BASE + 0x32)
#define reg_usb_ep3_buf_addr    REG_ADDR8(REG_USB_BASE + 0x33)
#define reg_usb_ep4_buf_addr    REG_ADDR8(REG_USB_BASE + 0x34)
#define reg_usb_ep5_buf_addr    REG_ADDR8(REG_USB_BASE + 0x35)
#define reg_usb_ep6_buf_addr    REG_ADDR8(REG_USB_BASE + 0x36)
#define reg_usb_ep7_buf_addr    REG_ADDR8(REG_USB_BASE + 0x37)
#define reg_usb_ep_buf_addr(i)  REG_ADDR8(REG_USB_BASE + 0x30 + ((i) & 0x07))

#define reg_usb_ep8_buf_addrh   REG_ADDR8(REG_USB_BASE + 0x38)
#define reg_usb_ep1_buf_addrh   REG_ADDR8(REG_USB_BASE + 0x39)
#define reg_usb_ep2_buf_addrh   REG_ADDR8(REG_USB_BASE + 0x3a)
#define reg_usb_ep3_buf_addrh   REG_ADDR8(REG_USB_BASE + 0x3b)
#define reg_usb_ep4_buf_addrh   REG_ADDR8(REG_USB_BASE + 0x3c)
#define reg_usb_ep5_buf_addrh   REG_ADDR8(REG_USB_BASE + 0x3d)
#define reg_usb_ep6_buf_addrh   REG_ADDR8(REG_USB_BASE + 0x3e)
#define reg_usb_ep7_buf_addrh   REG_ADDR8(REG_USB_BASE + 0x3f)
#define reg_usb_ep_buf_addrh(i) REG_ADDR8(REG_USB_BASE + 0x38 + ((i) & 0x07))

#define reg_usb_iso_mode        REG_ADDR8(REG_USB_BASE + 0x40)
#define reg_usb_ep_irq_status   REG_ADDR8(REG_USB_BASE + 0x41)

#define reg_usb_ep_irq_mask     REG_ADDR8(REG_USB_BASE + 0x42)

typedef enum
{
    FLD_USB_EDP8_IRQ = BIT(0),
    FLD_USB_EDP1_IRQ = BIT(1),
    FLD_USB_EDP2_IRQ = BIT(2),
    FLD_USB_EDP3_IRQ = BIT(3),
    FLD_USB_EDP4_IRQ = BIT(4),
    FLD_USB_EDP5_IRQ = BIT(5),
    FLD_USB_EDP6_IRQ = BIT(6),
    FLD_USB_EDP7_IRQ = BIT(7),
} usb_ep_irq_e;

#define reg_usb_ep8_send_max   REG_ADDR8(REG_USB_BASE + 0x43)
#define reg_usb_ep8_send_thres REG_ADDR8(REG_USB_BASE + 0x44)

#define reg_usb_ep8_fifo_mode  REG_ADDR8(REG_USB_BASE + 0x45)

enum
{
    FLD_USB_ENP8_FIFO_MODE = BIT(0),
    FLD_USB_ENP8_FULL_FLAG = BIT(1),
    FLD_USB_RMODE00        = BIT(2),
    FLD_USB_ENP8_EOF       = BIT(3),
    FLD_USB_ENP8_DMA_EOF   = BIT_RNG(4, 6),
};

#define reg_usb_ep_max_size REG_ADDR8(REG_USB_BASE + 0x46)
#define reg_usb_ep_tick     REG_ADDR8(REG_USB_BASE + 0x47)

#define reg_usb_ram_ctrl    REG_ADDR8(REG_USB_BASE + 0x48)

enum
{
    FLD_USB_CEN_PWR_DN = BIT(0),
    FLD_USB_CLK_PWR_DN = BIT(1),
    FLD_USB_WEN_PWR_DN = BIT(3),
    FLD_USB_CEN_FUNC   = BIT(4),
};

#define reg_usb_min1 REG_ADDR8(REG_USB_BASE + 0x49)

enum
{
    FLD_CTRL_EP8_SEND_THRES_H = BIT_RNG(0, 2),
    FLD_USB_EDP_MAP_MANUAL_EN = BIT(3),
    FLD_USB_EDP_MAP_AUTO_EN   = BIT(4),
    FLD_USB_EDPS_SM_MAP_EN    = BIT(5),
    FLD_USB_EDPS_MAP_TGL_EN   = BIT(6),
    FLD_USB_GET_STA_MAP_EN    = BIT(7),
};

#define reg_usb_rdps_map(i)    REG_ADDR8(REG_USB_BASE + 0x4a + (((i) & 0x07) >> 1))

#define reg_usb_sof_frame0     REG_ADDR8(REG_USB_BASE + 0x4e)
#define reg_usb_sof_frame1     REG_ADDR8(REG_USB_BASE + 0x4f)

#define reg_usb_edps_maxh0     REG_ADDR8(REG_USB_BASE + 0x50)

#define reg_usb_edp            REG_ADDR8(REG_USB_BASE + 0x51)
#define reg_usb_pidl           REG_ADDR8(REG_USB_BASE + 0x52)
#define reg_usb_pidh           REG_ADDR8(REG_USB_BASE + 0x53)

#define reg_usb_edp_eptrl      REG_ADDR8(REG_USB_BASE + 0x54)
#define reg_usb_edp_eptrh      REG_ADDR8(REG_USB_BASE + 0x55)

#define reg_usb_edp_udc_ptrl   REG_ADDR8(REG_USB_BASE + 0x56)
#define reg_usb_edp_udc_ptrh   REG_ADDR8(REG_USB_BASE + 0x57)

#define reg_usb_edp_s_ptrl     REG_ADDR8(REG_USB_BASE + 0x58)
#define reg_usb_edp_s_ptrh     REG_ADDR8(REG_USB_BASE + 0x59)

#define reg_usb_edps_map_en    REG_ADDR8(REG_USB_BASE + 0x5a)
#define reg_usb_edps_logic_en  REG_ADDR8(REG_USB_BASE + 0x5b)
#define reg_usb_edps_full_thrd REG_ADDR8(REG_USB_BASE + 0x5c)

#define reg_usb_pem_ctrl       REG_ADDR8(REG_USB_BASE + 0x5d)

enum
{
    FLD_USB_PEM_EVENT_EN  = BIT(0),
    FLD_USB_PEM_EVENT_SEL = BIT(1),
};

#define reg_usb_pem_ctrl1 REG_ADDR8(REG_USB_BASE + 0x5e)

#define reg_usb_tstamp0   REG_ADDR8(REG_USB_BASE + 0x60)
#define reg_usb_tstamp1   REG_ADDR8(REG_USB_BASE + 0x61)
#define reg_usb_tstamp2   REG_ADDR8(REG_USB_BASE + 0x62)
#define reg_usb_tstamp3   REG_ADDR8(REG_USB_BASE + 0x63)

#endif /* USB_REG_H */
