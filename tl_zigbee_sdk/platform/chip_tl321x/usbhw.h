/********************************************************************************************************
 * @file    usbhw.h
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
/** @page USBHW
 *
 *  Introduction
 *  ===============
 *  USB hardware.
 *
 *  API Reference
 *  ===============
 *  Header File: usbhw.h.
 */

#ifndef _USBHW_H_
#define _USBHW_H_

#include "lib/include/analog.h"
#include "gpio.h"
#include "lib/include/stimer.h"
#include "lib/include/clock.h"
#include "lib/include/pm/pm.h"
#include "reg_include/register.h"
#include <stdbool.h>

/* For compatibility, usb_set_pin_en() is equivalent to usb_set_pin(1), configure the usb pin and enable the dp_through_swire function.*/
#define usb_set_pin_en()    usb_set_pin(1)

typedef enum
{
    USB_EDP8_IN         = 8, // default buff len = 64
    USB_EDP1_IN         = 1, // default buff len = 8
    USB_EDP2_IN         = 2, // default buff len = 8
    USB_EDP3_IN         = 3, // default buff len = 16
    USB_EDP4_IN         = 4, // default buff len = 64
    USB_EDP5_OUT        = 5, // default buff len = 64
    USB_EDP6_OUT        = 6, // default buff len = 16
    USB_EDP7_IN         = 7, // default buff len = 16

    USB_EDP_PRINTER_IN  = USB_EDP8_IN,
    USB_EDP_KEYBOARD_IN = USB_EDP1_IN,
    USB_EDP_MOUSE       = USB_EDP2_IN,
    USB_EDP3_UNUSED_IN  = USB_EDP3_IN,
    USB_EDP_AUDIO_IN    = USB_EDP4_IN,
    USB_EDP_PRINTER_OUT = USB_EDP5_OUT,
    USB_EDP_SPEAKER     = USB_EDP6_OUT,
    USB_EDP_MIC         = USB_EDP7_IN,
    USB_EDP_MS_IN       = USB_EDP8_IN,
    USB_EDP_MS_OUT      = USB_EDP5_OUT,
    USB_EDP_SOMATIC_IN  = USB_EDP4_IN,
    USB_EDP_SOMATIC_OUT = USB_EDP5_OUT,
    USB_EDP_CDC_IN      = USB_EDP4_IN,
    USB_EDP_CDC_OUT     = USB_EDP5_OUT,
} usb_ep_index;

// #defined in the standard spec
enum
{
    USB_HID_AUDIO       = 2,
    USB_HID_MOUSE       = 1,
    USB_HID_KB_MEDIA    = 3, // media
    USB_HID_KB_SYS      = 4, // system : power,sleep,wakeup
    USB_HID_SOMATIC     = 5, // somatic sensor,  may have many report ids
};

typedef enum
{
    USB_IRQ_RESET_MASK   = BIT(0),
    USB_IRQ_250US_MASK   = BIT(1),
    USB_IRQ_SUSPEND_MASK = BIT(2),
    USB_IRQ_SOF_MASK     = BIT(3),
    USB_IRQ_SETUP_MASK   = BIT(4),
    USB_IRQ_DATA_MASK    = BIT(5),
    USB_IRQ_STATUS_MASK  = BIT(6),
    USB_IRQ_SETINF_MASK  = BIT(7),
} usb_irq_mask_e;

typedef enum
{
    USB_IRQ_RESET_STATUS    = BIT(0),
    USB_IRQ_250US_STATUS    = BIT(1),
    USB_IRQ_SUSPEND_STATUS  = BIT(2),
    USB_IRQ_SOF_STATUS      = BIT(3),
    USB_IRQ_EP_SETUP_STATUS = BIT(4),
    USB_IRQ_EP_DATA_STATUS  = BIT(5),
    USB_IRQ_EP_STA_STATUS   = BIT(6),
    USB_IRQ_EP_INTF_STATUS  = BIT(7),
} usb_irq_status_e;

typedef enum
{
    EP_MAP_DIS,
    EP_MAP_AUTO_EN,
} usb_ep_map_sel_e;

typedef enum
{
    SIZE_8_BYTE,
    SIZE_16_BYTE,
    SIZE_32_BYTE,
    SIZE_64_BYTE,
} usb_ctrl_ep_sel_e;

/**
 * @brief     This function servers to enable endpoint 8 fifo mode.
 * @return    none.
 */
static inline void usbhw_set_ep8_fifo_mode(void)
{
    BM_SET(reg_usb_ep8_fifo_mode, FLD_USB_ENP8_FIFO_MODE);
}

/**
 * @brief     This function servers to disable endpoint 8 fifo mode.
 * @return    none.
 */
static inline void usbhw_ep8_fifo_mode_dis(void)
{
    BM_CLR(reg_usb_ep8_fifo_mode, FLD_USB_ENP8_FIFO_MODE);
}

/**
 * @brief     This function servers to reset the pointer of control endpoint.
 * @return    none.
 */
static inline void usbhw_reset_ctrl_ep_ptr(void)
{
    reg_ctrl_ep_ptr = 0;
}

/**
 * @brief     This function servers to set the size of control endpoint.
 * @param[in] size - the size of control endpoint.
 * @return    none.
 */
static inline void usbhw_set_ctrl_ep_size(usb_ctrl_ep_sel_e size)
{
    reg_usb_edp0_size = ((reg_usb_edp0_size & ~(FLD_USB_CTR_EP_SIZE)) | size);
}

/**
 * @brief     This function servers to set the value of control endpoint.
 * @param[in] data - the value of control endpoint.
 * @return    none.
 */
static inline void usbhw_write_ctrl_ep_ctrl(unsigned char data)
{
    reg_ctrl_ep_ctrl = data;
}

/**
 * @brief     This function servers to read the data of control endpoint.
 * @return    The value of control endpoint data.
 */
static inline unsigned char usbhw_read_ctrl_ep_data(void)
{
    return reg_ctrl_ep_dat;
}

/**
 * @brief     This function servers to write the data of control endpoint.
 * @param[in] data - the data of control endpoint to write.
 * @return    none.
 */
static inline void usbhw_write_ctrl_ep_data(unsigned char data)
{
    reg_ctrl_ep_dat = data;
}

/**
 * @brief     This function servers to determine whether control endpoint is busy.
 * @return    1: busy; 0: not busy.
 */
static inline int usbhw_is_ctrl_ep_busy(void)
{
    return reg_ctrl_ep_ctrl & FLD_EP_DAT_BUSY;
}

/**
 * @brief     This function servers to reset the pointer of data endpoint.
 * @param[in] ep - select the data endpoint.
 * @return    none.
 */
static inline void usbhw_reset_ep_ptr(usb_ep_index ep)
{
    reg_usb_ep_ptr(ep) = 0;
    reg_usb_ep_ptrh(ep) = 0;
}

/**
 * @brief     This function servers to get the irq status of control endpoint.
 * @return    The irq status of control endpoint.
 */
static inline unsigned int usbhw_get_ctrl_ep_irq(void)
{
    return reg_ctrl_ep_irq_sta;
}

/**
 * @brief     This function servers to clear the irq status of control endpoint.
 * @param[in] ep - the irq status of control endpoint.
 * @return    none.
 */
static inline void usbhw_clr_ctrl_ep_irq(int ep)
{
    reg_ctrl_ep_irq_sta = ep;
}
/**
 * @brief     This function servers to get the pointer of data endpoint.
 * @param[in] ep - select the data endpoint.
 * @return    The selected endpoint pointer address.
 */
static inline unsigned short usbhw_get_ep_ptr(usb_ep_index ep)
{
    return (reg_usb_ep_ptrh(ep) << 8) | reg_usb_ep_ptr(ep);
}

/**
 * @brief     This function servers to set the buffer address of data endpoint.
 * @param[in] ep - select the data endpoint.
 * @param[in] addr - data endpoint buffer address.
 * @return    none.
 */
static inline void usbhw_set_ep_addr(usb_ep_index ep, unsigned short addr)
{
    reg_usb_ep_buf_addr(ep) = addr & 0xff;
    reg_usb_ep_buf_addrh(ep) = (addr >> 8) & 0xff;
}

/**
 * @brief     This function servers to set the irq mask of data endpoint.
 * @param[in]  mask -the irq mask of usb.
 * @return    none.
 */
static inline void usbhw_set_eps_irq_mask(usb_ep_irq_e mask)
{
    reg_usb_ep_irq_mask |= mask;
}

/**
 * @brief     This function servers to clr the irq mask of data endpoint.
 * @param[in]  mask -the irq mask of usb.
 * @return    none.
 */
static inline void usbhw_clr_eps_irq_mask(usb_ep_irq_e mask)
{
    reg_usb_ep_irq_mask &= (~mask);
}

/**
 * @brief     This function servers to get the irq status of data endpoint.
 * @return    The irq status of data endpoint.
 */
static inline unsigned int usbhw_get_eps_irq(void)
{
    return reg_usb_ep_irq_status;
}

/**
 * @brief     This function servers to clear the irq status of data endpoint.
 * @param[in] ep - the irq of data endpoint
 * @return    none.
 */
static inline void usbhw_clr_eps_irq(int ep)
{
    reg_usb_ep_irq_status = ep;
}

/**
 * @brief     This function servers to set usb irq mask.
 * @param[in]  mask -the irq mask of usb.
 * @return    none.
 */
static inline void usbhw_set_irq_mask(usb_irq_mask_e mask)
{
    reg_usb_irq_mask |= mask;
    if (mask == USB_IRQ_SETINF_MASK)
    {
        reg_usb_edp0_size |= FLD_USB_IRQ_SETADDR_MASK;
    }
}

/**
 * @brief     This function servers to clr usb irq mask.
 * @param[in]  mask -the irq mask of usb.
 * @return    none.
 */
static inline void usbhw_clr_irq_mask(usb_irq_mask_e mask)
{
    reg_usb_irq_mask &= (~mask);
    if (mask == USB_IRQ_SETINF_MASK)
    {
        reg_usb_edp0_size &= (~FLD_USB_IRQ_SETADDR_MASK);
    }
}

/**
 * @brief     This function servers to get usb irq status.
 * @param[in]  status -the irq status of usb.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned char usbhw_get_irq_status(usb_irq_status_e status)
{
    return reg_ctrl_ep_irq_sta & status;
}

/**
 * @brief     This function servers to clr usb irq status.
 * @param[in]  status -the irq status of usb.
 * @return    none.
 */
static inline void usbhw_clr_irq_status(usb_irq_status_e status)
{
    reg_ctrl_ep_irq_sta = status;
}

/**
 * @brief     This function servers to get usb set address irq status.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned char usbhw_get_set_addr_irq_status(void)
{
    return reg_ctrl_ep_ctrl & FLD_EP_IRQ_SETADDR;
}

/**
 * @brief     This function servers to clr usb set address irq status.
 * @return    none.
 */
static inline void usbhw_clr_set_addr_irq_status(void)
{
    reg_ctrl_ep_ctrl |= FLD_EP_IRQ_SETADDR;
}

/**
 * @brief     This function servers to get usb set address error status.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned char usbhw_get_set_addr_error_status(void)
{
    return reg_usb_mdev & BIT(1);
}

/**
 * @brief     This function servers to enable data endpoint.
 * @param[in] ep - select the data endpoint.
 * @return    none.
 * @note
 *            - when usb map en, reg_usb_edps_logic_en reg enable data endpoint.
 *            - when usb map dis, reg_usb_edp_en enable data endpoint.
 */
static inline void usbhw_set_eps_en(usb_ep_en_e ep)
{
    if (reg_usb_min1 & FLD_USB_EDPS_SM_MAP_EN)
    {
        reg_usb_edps_logic_en = ep;
    }
    else
    {
        reg_usb_edp_en = ep;
    }
}

/**
 * @brief     This function servers to disable data endpoint.
 * @param[in] ep - select the data endpoint mask.
 * @return    none.
 */
static inline void usbhw_set_eps_dis(usb_ep_en_e ep)
{
    reg_usb_edp_en &= (~ep);
}
/**
 * @brief     This function servers to enable mapping logical data endpoint.
 * @param[in] ep - select the data endpoint mask.
 * @return    none.
 */
static inline void usbhw_set_eps_map_en(usb_ep_en_e ep)
{
    reg_usb_edps_map_en = ep;
}

/**
 * @brief     This function servers to disable mapping logical endpoint.
 * @param[in] ep - select the data endpoint mask.
 * @return    none.
 */
static inline void usbhw_set_eps_map_dis(usb_ep_en_e ep)
{
    reg_usb_edps_map_en &= (~ep);
}

/**
 * @brief     This function servers to disable mapping logical endpoint.
 * @param[in] ep - select the data endpoint mask.
 * @return    none.
 */
static inline void usbhw_set_eps_log_dis(usb_ep_en_e ep)
{
    reg_usb_edps_logic_en &= (~ep);
}

/**
 * @brief     This function servers to set in endpoint buffer max size(except 7 aiso mode).
 * @param[in] max_size - in endpoint max size.
 * @return    none.
 */
static inline void usbhw_set_eps_max_size(unsigned int max_size)
{
    reg_usb_ep_max_size = max_size >> 3;
}

/**
 * @brief     This function servers to set data endpoint 8 buffer max size.
 * @param[in] max_size - max size.
 * @return    none.
 */
static inline void usbhw_set_ep8_max_size(unsigned int max_size)
{
    reg_usb_ep8_send_max = max_size >> 3;
}

/**
 * @brief     This function servers to read the data of data endpoint.
 * @param[in] ep - selected the endpoint.
 * @return    the value of endpoint.
 */
static inline unsigned char usbhw_read_ep_data(unsigned int ep)
{
    return reg_usb_ep_dat(ep);
}

/**
 * @brief     This function servers to write the data of data endpoint.
 * @param[in] ep   - selected the endpoint.
 * @param[in] data - the value of endpoint.
 * @return    none.
 */
static inline void usbhw_write_ep_data(unsigned int ep, unsigned char data)
{
    reg_usb_ep_dat(ep) = data;
}

/**
 * @brief     This function servers to determine whether data endpoint is busy.
 * @param[in] ep - selected the endpoint.
 * @return    1: busy; 0: not busy.
 */
static inline unsigned int usbhw_is_ep_busy(unsigned int ep)
{
    return reg_usb_ep_ctrl(ep) & FLD_USB_EP_BUSY;
}

/**
 * @brief     This function servers to set the specified data endpoint to ack.
 * @param[in] ep - select the data endpoint.
 * @return    none.
 */
static inline void usbhw_data_ep_ack(unsigned int ep)
{
    reg_usb_ep_ctrl(ep) = FLD_USB_EP_ACK;
}

/**
 * @brief     This function servers to set the specified data endpoint to stall.
 * @param[in] ep - select the data endpoint.
 * @return    none.
 */
static inline void usbhw_data_ep_stall(unsigned int ep)
{
    reg_usb_ep_ctrl(ep) = FLD_USB_EP_STALL;
}

/**
 * @brief     This function servers to set the threshold of printer.
 * @param[in] th - set the threshold for printer.
 * @return    none.
 */
static inline void usbhw_set_printer_threshold(unsigned char th)
{
    reg_usb_ep8_send_thres = th;
}

/**
 * @brief     This function servers to set in endpoint size(wMaxPacketSize) for returning a zero-length packet.
 * @param[in] size - in endpoint size.
 * @return    none.
 */
static inline void usbhw_set_in_ep_size(unsigned short size)
{
    reg_usb_edp_eptrl =size & 0xff;
    reg_usb_edp_eptrh = (size >> 8) & 0xff;
}

/**
 * @brief     This function servers to determine whether endpoint is busy.
 * @param[in] ep - selected the endpoint.
 * @return    1: busy; 0: not busy.
 */
static inline unsigned char usbhw_is_in_ep_full_thrd(unsigned int ep)
{
    return reg_usb_edps_full_thrd & BIT(ep & 0x07);
}

/**
 * @brief     This function servers to get time stamp.
 * @return    the time stamp.
 */
static inline unsigned int usbhw_get_timer_stamp(void)
{
    return (reg_usb_tstamp0 | (reg_usb_tstamp1 << 8) | (reg_usb_tstamp2 << 16) | (reg_usb_tstamp3 << 24));
}

/**
 * @brief      This function serves to enable USB power and clock.
 * @return     none.
 */
void usbhw_init(void);

/**
 * @brief      This function serves to disable the manual interrupt(endpoint8 is the alias of endpoint0).
 * @param[in]  m - the irq mode needs to set.
 * @return     none.
 */
void usbhw_disable_manual_interrupt(int m);

/**
 * @brief      This function serves to enable the manual interrupt.
 * @param[in]  m - the irq mode needs to set.
 * @return     none.
 */
void usbhw_enable_manual_interrupt(int m);

/**
 * @brief      This function serves to send a bulk of data to host via the specified endpoint.
 * @param[in]  ep - the number of the endpoint.
 * @param[in]  data - pointer to the data need to send.
 * @param[in]  len - length in byte of the data need to send.
 * @return     none.
 */
void usbhw_write_ep(unsigned int ep, unsigned char *data, int len);

/**
 * @brief      This function serves to send two bytes data to host via the control endpoint.
 * @param[in]  v - the two bytes data need to send.
 * @return     none.
 */
void usbhw_write_ctrl_ep_u16(unsigned short v);

/**
 * @brief   This function serves to read two bytes data from host via the control endpoint.
 * @return  The two bytes data read from the control endpoint.
 */
unsigned short usbhw_read_ctrl_ep_u16(void);

/**
 * @brief   This function serves to set data endpoint mapping.
 * @param   source_ep - The source endpoint of the mapping.
 * @param   target_ep - The target endpoint of the mapping.
 * @return    none.
 */
void usbhw_set_ep_map(usb_ep_index source_ep, usb_ep_index target_ep);

/**
 * @brief   This function serves to set data endpoint mapping.
 * @param   map_en - mapping enable or disable.
 * @return    none.
 */
void usbhw_ep_map_en(usb_ep_map_sel_e map_en);

/**
 * @brief      This function serves to enable or disable the internal pull-up resistor of DP pin of USB interface.
 * @param[in]  en - enables or disables the internal pull-up resistor(1: enable 0: disable).
 * @return     none.
 */
static inline void usb_dp_pullup_en(int en)
{
    unsigned char dat = analog_read_reg8(0x0b);
    if (en)
    {
        dat = dat & (~BIT(7));
    }
    else
    {
        dat = dat | BIT(7);
    }

    analog_write_reg8(0x0b, dat);
}

/**
 * @brief      This function serves to power on or down USB module.
 * @param[in]  en - 1: power on 0: power down.
 * @return     none.
 */
static inline void usb_power_on(unsigned char en)
{
    if (en)
    {
        analog_write_reg8(0x7d, analog_read_reg8(0x7d) & 0xfd);
    }
    else
    {
        analog_write_reg8(0x7d, analog_read_reg8(0x7d) | 0x02);
    }
}

/**
 * @brief      This function serves to set dp_through_swire function.
 * @param[in]  dp_through_swire - 1: swire_usb_en 0: swire_usb_dis
 * @return     none.
 */
void dp_through_swire_en(bool dp_through_swire);

/**
 * @brief      This function serves to set GPIO MUX function as DP and DM pin of USB.
 * @param[in]  dp_through_swire - 1: swire_usb_en 0: swire_usb_dis
 * @return     none.
 * @note       1. Configure usb_set_pin(0) , there are some risks, please refer to the startup.S file about DP_THROUGH_SWIRE_DIS
 *                for detailed description (by default dp_through_swire is disabled). Configure usb_set_pin(1) to enable dp_through_swire again.
 *             2. When dp_through_swire is enabled, Swire and USB applications do not affect each other.
 */
void usb_set_pin(bool dp_through_swire);

/**
 * @brief     This function servers to enable hardware feature.
 * @return    none.
 */
static inline void usbhw_enable_hw_feature(usb_hw_feature_e feature)
{
    reg_usb_ctrl |= feature;
}

/**
 * @brief     This function servers to disable hardware feature.
 * @return    none.
 */
static inline void usbhw_disable_hw_feature(usb_hw_feature_e feature)
{
    reg_usb_ctrl &= (~feature);
}
#endif /* _USBHW_H_ */
