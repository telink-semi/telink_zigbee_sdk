/********************************************************************************************************
 * @file    hci.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef _HCI_H_
#define _HCI_H_



#define			HCI_FLAG_EVENT_TLK_MODULE					(1<<24)
#define			HCI_FLAG_EVENT_BT_STD						(1<<25)

#if (MCU_CORE_TYPE == MCU_CORE_8258 || MCU_CORE_TYPE == MCU_CORE_827x || MCU_CORE_TYPE == MCU_CORE_TC321X)
typedef struct {
    u32 size;
    u16 num;
    u8 wptr;
    u8 rptr;
    u8 *p;
} my_fifo_t;

#define MYFIFO_INIT(name,size,n)        u8 name##_b[size * n]={0};my_fifo_t name = {size,n,0,0, name##_b}
#define MYFIFO_INIT_IRAM(name,size,n)   u8 name##_b[size * n]__attribute__((aligned(4)))/*={0}*/;my_fifo_t name = {size,n,0,0, name##_b}

void my_fifo_init(my_fifo_t *f, u32 s, u16 n, u8 *p);
u8 *my_fifo_wptr(my_fifo_t *f);
void my_fifo_next(my_fifo_t *f);
int my_fifo_push(my_fifo_t *f, u8 *p, int n);
void my_fifo_pop(my_fifo_t *f);
u8 *my_fifo_get(my_fifo_t *f);
#endif

/**
 * @brief	HCI RX handler callback declaration
 */
typedef int (*blc_hci_rx_handler_t) (void);


/**
 * @brief	HCI TX handler callback declaration
 */
typedef int (*blc_hci_tx_handler_t) (void);



/**
 * @brief	this function is used to register HCI Event handler Callback function
 */
typedef int (*hci_event_handler_t) (u32 h, u8 *para, int n);
extern hci_event_handler_t		blc_hci_event_handler;




/**
 * @brief      this function is used to get data by USB in RX mode for HCI Layer
 * @param[in]  none.
 * @return     0
 */
int blc_hci_rx_from_usb (void);

/**
 * @brief      this function is used to send data by USB in TX mode for HCI Layer
 * @param[in]  none.
 * @return     0
 */
int blc_hci_tx_to_usb (void);


/**
 * @brief      this function is used to process HCI data
 * @param[in]  *p - the pointer of HCI data
 * @param[in]  n - the length of HCI data
 * @return     0
 */
int blc_hci_handler (u8 *p, int n);



/**
 * @brief      this function is used to process HCI events
 * @param[in]  none.
 * @return     0
 */
int blc_hci_proc (void);






/**
 * @brief      this function is used to set HCI EVENT mask
 * @param[in]  evtMask  -  HCI EVENT mask
 * @return     0
 */
ble_sts_t	blc_hci_setEventMask_cmd(u32 evtMask);      //eventMask: BT/EDR

/**
 * @brief      this function is used to set HCI LE EVENT mask
 * @param[in]  evtMask  -  HCI LE EVENT mask(BIT<0-31>)
 * @return     0
 */
ble_sts_t	blc_hci_le_setEventMask_cmd(u32 evtMask);   //eventMask: LE event  0~31

/**
 * @brief      this function is used to set HCI LE EVENT mask
 * @param[in]  evtMask  -  HCI LE EVENT mask(BIT<32-63>)
 * @return     0
 */
ble_sts_t 	blc_hci_le_setEventMask_2_cmd(u32 evtMask_2);   //eventMask: LE event 32~63

/**
 * @brief      this function is used to register HCI event handler callback function
 * @param[in]  handler - hci_event_handler_t
 * @return     none.
 */
void 		blc_hci_registerControllerEventHandler (hci_event_handler_t  handler);

/**
 * @brief      this function is used to register HCI TX or RX handler callback function
 * @param[in]  prx - blc_hci_rx_handler
 * @param[in]  ptx - blc_hci_tx_handler
 * @return     none.
 */
void 		blc_register_hci_handler (blc_hci_rx_handler_t prx, blc_hci_tx_handler_t  ptx);

/**
 * @brief      this function is used to send ACL data to HOST
 * @param[in]  handle - connect handle
 * @param[in]  *p - the pointer of l2cap data
 * @return     0
 */
int 		blc_hci_sendACLData2Host (u16 handle, u8 *p);

/**
 * @brief      this function is used to send data
 * @param[in]  h - HCI Event type
 * @param[in]  *para - data pointer of event
 * @param[in]  n - data length of event
 * @return     0,-1
 */
int 		blc_hci_send_data (u32 h, u8 *para, int n);


#endif  /* _HCI_H_ */
