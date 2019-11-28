/********************************************************************************************************
 * @file     my_usb.c
 *
 * @brief    This is the header file for TLSR8258
 *
 * @author	 junwei.lu@telink-semi.com;yafei.tu@telink-semi.com
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
#include "my_usb.h"
#include "usbhw_i.h"
#include "usbhw.h"
#include "my_usbdesc.h"


static USB_Request_Header_t control_request;
static u8 *g_response = 0;
static u16 g_response_len = 0;
static u8 my_g_stall = 0;
static u8 my_g_rate = 0;
static u8 my_g_protocol = 0;	//< 1: report protocol ;0: start protocol.
u8 my_g_config = 0;

void my_usb_init(void) {
	/*  EDP0MODE(reg addr:0x0104):set EP0 mode
		[0]: enable auto decoding set_address command
		[1]: enable auto decoding set_config command
		[2]: enable auto decoding set_interface command
		[3]: enable auto decoding get_status command
		[4]: enable auto decoding sync_frame command
		[5]: enable auto decoding get_descriptor command
		[6]: enable auto decoding set_feature command
		[7]: enable auto decoding standard command
	*/
	usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD	/*disable auto decoding standard command */ | \
			                      FLD_CTRL_EP_AUTO_DESC	/*disable auto decoding get_descriptor command */ | \
			                      FLD_CTRL_EP_AUTO_CFG	/*disable auto decoding set_config command */);
}

//data_request: MY_USB_IRQ_SETUP_REQ  or  MY_USB_IRQ_DATA_REQ!
//SETUP REQ handle
//DATA REQ handle
void my_usb_handle_request(u8 data_request) {
	u8 bmRequestType = control_request.bmRequestType;
	u8 bRequest = control_request.bRequest;
	u8 value_l = (control_request.wValue) & 0xff;
    u8 value_h = (control_request.wValue >> 8) & 0xff;
    u8 index_l = (control_request.wIndex) & 0xff;
    //Step1: reset EDP0 rx buffer pointer
	usbhw_reset_ctrl_ep_ptr();

	//Step2: if request type is input request. and then
	if((bmRequestType&0x80)==0x80){							//bmRequestType<7> = 1

		// bmRequestType<6:5>: sub type of input request
		// 0b00: standard request
		// 0b01: class request
		// 0b10: vendor request
		switch(bmRequestType&0x60){
			case (REQTYPE_STANDARD): 						//##1.standard request
			{
 	 	 	 	if(MY_USB_IRQ_SETUP_REQ == data_request)
 	 	 	 	{
 	 	 	 		//printf("##1.USB Standard Input Request:\r\n");
 	 	 	 	}

				switch(bRequest){
					case (REQ_GetConfiguration):			//1-1.get configuration
					{
                        if(MY_USB_IRQ_SETUP_REQ == data_request)
                        	//printf("--1-1.get configuration \r\n");
						break;
					}
				    case (REQ_GetDescriptor):				//1-2.get descriptor and return descriptor info. to host
					{
		            	if(MY_USB_IRQ_SETUP_REQ == data_request) {
							//printf("--1-2.get descriptor \r\n");
							g_response = 0;
							g_response_len = 0;

							switch(value_h)
							{
								case DTYPE_Device: 			//1-2-1. get device descriptor
								{
									//printf("++1-2-1.device descriptor \r\n");
									g_response = my_usbdesc_get_device();
									g_response_len = sizeof(USB_Descriptor_Device_t);
									break;
								}
								case DTYPE_Configuration:	//1-2-2. get configuration descriptor
								{
									//printf("++1-2-2.configuration descriptor \r\n");
									g_response = my_usbdesc_get_configuration();
									g_response_len = sizeof(MY_USB_Descriptor_Configuration_t);
									break;
								}
								case DTYPE_String:			//1-2-3. get string descriptor
								{
									//printf("++1-2-3.string descriptor \r\n");

									switch(value_l)
									{
										case USB_STRING_LANGUAGE:  //1-2-3-1. get language string
										{
											//printf("**1-2-3-1. Language string \r\n");
											g_response = my_usbdesc_get_language();
											g_response_len = sizeof(LANGUAGE_ID_ENG);
											break;
										}
										case USB_STRING_VENDOR:  //1-2-3-2. get vendor string
										{
											//printf("**1-2-3-2. Vendor String \r\n");
											g_response = my_usbdesc_get_vendor();
											g_response_len = sizeof(MY_STRING_VENDOR);
											break;
										}
										case USB_STRING_PRODUCT:  //1-2-3-2. get product string
										{
											//printf("**1-2-3-3. Product String \r\n");
											g_response = my_usbdesc_get_product();
											g_response_len = sizeof(MY_STRING_PRODUCT);
											break;
										}
										case USB_STRING_SERIAL:  //1-2-3-2. get product serial number string
										{
											//printf("**1-2-3-4. Product Serial Number String \r\n");
											g_response = my_usbdesc_get_serial();
											g_response_len = sizeof(MY_STRING_SERIAL);
											break;
										}
										default :
										{
											//printf("**1-2-3-4. Unknown String \r\n");
											//return a packet with 0 length
											my_g_stall = 1;
											break;
										}
									}
									break;
								}
								//For HID device(Mouse/Keyboard/joystick )
								case HID_DTYPE_HID:			//1-2-4. get HID Descriptor
								{
									//printf("++1-2-4.HID descriptor \r\n");
									if (index_l == USB_INTF_KEYBOARD_ID) 				//index_l is the interface number
									{
										//printf("**1-2-4-1.HID descriptor: KEYBOARD \r\n");	//mouse
										g_response = my_usbdesc_get_keyboard();
										g_response_len = sizeof(USB_HID_Descriptor_HID_Keyboard_t);
									}
									else if (index_l == USB_INTF_KEYS_ID) 				//index_l is the interface number
									{
										//printf("**1-2-4-1.HID descriptor: KEYS \r\n");	//mouse
										g_response = my_usbdesc_get_keys();
										g_response_len = sizeof(USB_HID_Descriptor_HID_Keyboard_t);
									}
									else if (index_l == USB_INTF_VOICE_ID) 				//index_l is the interface number
									{
										//printf("**1-2-4-1.HID descriptor: VOICE \r\n");	//mouse
										g_response = my_usbdesc_get_voice();
										g_response_len = sizeof(USB_HID_Descriptor_HID_Audio_t);
									}
									break;
								}
								case HID_DTYPE_Report:		//1-2-5. get Report Descriptor
								{
									//printf("++1-2-5.Report descriptor \r\n");
									if (index_l == USB_INTF_KEYBOARD_ID) {
										//printf("**1-2-5-1.Report descriptor: KEYBOARD \r\n");//mouse
										g_response = (u8*) my_usbkeyboard_get_report_desc();
										g_response_len = my_usbkeyboard_get_report_desc_size();
									}
									else if (index_l == USB_INTF_KEYS_ID) {
										//printf("**1-2-5-1.Report descriptor: KEYS \r\n");//mouse
										g_response = (u8*) my_usbkeys_get_report_desc();
										g_response_len = my_usbkeys_get_report_desc_size();
									}
									else if (index_l == USB_INTF_VOICE_ID) {
										//printf("**1-2-5-1.Report descriptor: VOICE \r\n");//mouse
										g_response = (u8*) my_usbvoice_get_report_desc();
										g_response_len = my_usbvoice_get_report_desc_size();
									}
									else{
										my_g_stall = 1;
									}
									break;
								}
								case 0x23:					//1-2-6. physical descriptor (optional)
								{
									//printf("++1-2-6.Physical descriptor \r\n");
									//printf("**1-2-6-1.TODO \r\n");
									break;
								}
								default:
								{
									//printf("++1-2-7.Other descriptors,code: \r\n");
									//printHEX((control_request.wValue>>8)&0xFF);
									//printf(" \r\n");
									my_g_stall = 1;
									break;
								}
							}

							if(control_request.wLength < g_response_len) {
								g_response_len = control_request.wLength;
								//printf("g_response_len = %d\n",g_response_len);
							}
		            	}
		            	// return descriptor info. to host device

		            	my_usb_send_response();

						//printf("--1-2.return descriptor to host \r\n");
				    	break;
					}
					case (REQ_GetStatus):					//1-3. get status
                  		//printf("--1-3.get status \r\n");
						break;

					case (REQ_GetInterface):				//1-4. get interface
						//printf("--1-4.get interface \r\n");
						break;

					case (REQ_SynchFrame):					//1-5. get synchFrame
						//printf("--1-5.get sycnFrame \r\n");
						break;

					default:								//1-6. undefined request
						//printf("--1-6.undefined request \r\n");
						my_g_stall = 1;
						break;
				}
			    break;
			}
			case (REQTYPE_CLASS): 						//##2.class request
				if(MY_USB_IRQ_SETUP_REQ == data_request)
					//printf("##2.USB Class Input Request \r\n");

				switch (bRequest){
					//HID special input request: 3 input request and 3 output request
					case HID_REQ_GetReport:		//-- 2-1. get report request
						//This request is mandatory and must be supported by all devices.

						if(MY_USB_IRQ_SETUP_REQ == data_request){
							//printf("-- 2-1.get report request \r\n");
							switch(value_h){
							//value_h:Report Type
							//value_l:Report ID(value_l = 0, if not define Report ID)
								case 1://input
									//printf("-- 2-1-1.report type: input(todo) \r\n");
									break;
								case 2://output
									//printf("-- 2-1-2.report type: output(todo) \r\n");
									break;
								case 3://feature
									//printf("-- 2-1-3.report type: feature(todo) \r\n");
									break;
								default:
									my_g_stall = 1;
									break;
							}
						}
						//send report to USB host
						//printf("-- 2-1.return report request(todo) \r\n");
						break;
					case HID_REQ_GetIdle:		//-- 2-2. get idle time request
						if(MY_USB_IRQ_SETUP_REQ == data_request){
							//printf("-- 2-2.return idle time request \r\n");
							usbhw_write_ctrl_ep_data(my_g_rate);//send idle time to USB host(data period)
						}
						break;
					case HID_REQ_GetProtocol:  //-- 2-3. get protocol request
						//This request is required only for boot devices.
						if(MY_USB_IRQ_SETUP_REQ == data_request){
							//printf("-- 2-3.return protocol request \r\n");
							//my_g_protocol:0 start protocol
							//my_g_protocol:1 report protocol
							usbhw_write_ctrl_ep_data(my_g_protocol);//send data to USB host(data period)
						}
						break;
					default:
						my_g_stall = 1;
						break;
				}
		        break;

			case (REQTYPE_VENDOR): //##3.vendor request
				//printf("##3.USB Vendor Input Request \r\n");
			    break;

			default:
				//printf("##4.USB Undefined Input Request \r\n");
				my_g_stall = 1;
			    break;
		}
	}
	//Step2: otherwise, request type is output request. and then
	else{
		// bmRequestType<6:5>: sub type of output request
		// 0b00: standard request
		// 0b01: class request
		// 0b10: vendor request
		switch(bmRequestType&0x60)
		{
			case REQTYPE_STANDARD:  //##1.standard output request

				//printf("##1.USB Standard Output Request:\r\n");

				switch(bRequest)
				{
					case REQ_ClearFeature: 					//--1-1. Clear Feature
						//printf("--1-1. Clear Feature \r\n");

					    break;
					//our USB device will automatically process request,Not need to process REQ_SetAddress
#if 0
					case REQ_SetAddress:   					//--1-2. set address
						//printf("--1-2. set address :");
						//printHEX(control_request.wValue&0xFF);
						//printf("\r\n");
						break;
#endif
					case REQ_SetConfiguration: 				//--1-3. set configuration
						//printf("--1-3. set configuration \r\n");
						//enable uEDP0,after configuring uEDP0
						if(control_request.wValue&0xff){//configuring for uEDP0 if wValue != 0
							my_g_config = 1;
						}
						break;

					case REQ_SetDescriptor: 				//--1-4. set descriptor
						//printf("--1-4. set descriptor \r\n");
						break;

					case REQ_SetFeature: 					//--1-5. set feature
						//printf("--1-5. set feature \r\n");
						break;

					case REQ_SetInterface: 					//--1-6. set interface
						//printf("--1-6. set interface \r\n");
						break;

					default:
						//printf("--1-7. undefined output request \r\n");
						my_g_stall = 1;
						break;
				}
				break;

			case REQTYPE_CLASS:  //##2.class output request
				if(MY_USB_IRQ_SETUP_REQ == data_request)
				{
					//printf("##2.USB Class Output Request");
				}
				switch(bRequest)
				{
					//HID special output request: 3 input request and 3 output request
				    case HID_REQ_SetReport:
				    	if(MY_USB_IRQ_SETUP_REQ == data_request){
							//printf("-- 2-1.Set Report \r\n");
							switch(value_h){
							//value_h:Report Type
							//value_l:Report ID(value_l = 0, if not define Report ID)
								case 1://input
									//printf("-- 2-1-1.report type: input(todo) \r\n");
									break;
								case 2://output
									//printf("-- 2-1-2.report type: output(todo) \r\n");
									break;
								case 3://feature
									//printf("-- 2-1-3.report type: feature(todo) \r\n");
									break;
								default:
									my_g_stall = 1;
									break;
							}
				    	}
				    	//USB host send report to device(DATA PERIOD: Report)
				    	//todo
				    	break;

					case HID_REQ_SetIdle:
						if(MY_USB_IRQ_SETUP_REQ == data_request){
							//printf("-- 2-2.Set Idle Time \r\n");
					        //value_h: Idle time 4ms ~ 1020ms
							//value_l: Report ID
							my_g_rate = value_h;
						}
						break;

					case HID_REQ_SetProtocol://This request is required only for boot devices.
						if (MY_USB_IRQ_SETUP_REQ == data_request){

							//printf("-- 2-3.Set Protocol \r\n");
							//my_g_protocol:0  start Protocol
							//my_g_protocol:1  report Protocol
							my_g_protocol = value_l;
						}
					    break;

					default:
						//printf("-- 2-4.unknown request \r\n");
						my_g_stall = 1;
						break;
				}
				break;

			case REQTYPE_VENDOR:  //##3.vendor output request
				//printf("##3.USB Vendor Output Request \r\n");
				break;

			default:
				//printf("##4.USB Undefined Output Request\r\n");
				my_g_stall = 1;
				break;
		}
	}

	return;
}

void my_usb_handle_ctl_ep_setup() {
	usbhw_reset_ctrl_ep_ptr();										//Step1: reset EDP0 rx buffer pointer
	control_request.bmRequestType 	= usbhw_read_ctrl_ep_data();	//Step2: get u8 data from EDP0
	control_request.bRequest 		= usbhw_read_ctrl_ep_data();
	control_request.wValue 			= usbhw_read_ctrl_ep_u16();		//Step3: get u16 data from EDP0
	control_request.wIndex 			= usbhw_read_ctrl_ep_u16();
	control_request.wLength 		= usbhw_read_ctrl_ep_u16();
	/*printHEX(control_request.bmRequestType);
	printHEX(control_request.bRequest);
	printHEX(control_request.wValue&0xFF);
	printHEX((control_request.wValue>>8)&0xFF);
	printHEX(control_request.wIndex&0xFF);
	printHEX((control_request.wIndex>>8)&0xFF);
	printHEX(control_request.wLength&0xFF);
	printHEX((control_request.wLength>>8)&0xFF);
	printf(" \r\n");*/

	my_g_stall = 0;

	my_usb_handle_request(MY_USB_IRQ_SETUP_REQ);

	if(my_g_stall)
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
		//printf("STALL! \r\n");
	}
	else
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
		//printf("ACK! \r\n");
	}
}

void my_usb_handle_ctl_ep_data(void) {
	usbhw_reset_ctrl_ep_ptr();
	my_g_stall = 0;
	my_usb_handle_request(MY_USB_IRQ_DATA_REQ);

	if(my_g_stall)
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
		//printf("STALL! \r\n");
	}
	else
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
		//printf("ACK! \r\n");
	}
}

void my_usb_handle_ctl_ep_status() {
	if(my_g_stall)
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_STALL);
		//printf("STALL! \r\n");
	}
	else
	{
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_ACK);
		//printf("ACK! \r\n");
	}
}


void my_usb_handle_irq(void) {
	u32 irq = usbhw_get_ctrl_ep_irq();							//Step1: get value of irq status register

	if(irq & FLD_CTRL_EP_IRQ_SETUP) {							//Step2-1: if rx packet is SETUP packet
		//printf("--------------------------------------------- \r\n");
		//printf("USB SETUP IRQ!  \r\n");
		usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_SETUP);			//clear irq status of SETUP
		my_usb_handle_ctl_ep_setup();							//SETUP packet process function
	}

	if(irq & FLD_CTRL_EP_IRQ_DATA) {							//Step2-2: if rx packet is DATA packet
		//printf("--------------------------------------------- \r\n");
		//printf("USB DATA IRQ! \r\n");
		usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_DATA);
		my_usb_handle_ctl_ep_data();							//DATA packet process function
	}

	if(irq & FLD_CTRL_EP_IRQ_STA) {								//Step2-3: if rx packet is DATA packet
		//printf("--------------------------------------------- \r\n");
		//printf("USB STATUS IRQ! \r\n");
		usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_STA);
		my_usb_handle_ctl_ep_status();							//STATUS packet process function
	}


	irq = usbhw_get_eps_irq();

	//Keyboard endpoint1 IN IRQ
	if(irq & BIT(EDP_ID_KEYBOARD_IN & 0x07)){
		//printf("Endpoint %01x input irq!\n",EDP_ID_KEYBOARD_IN);
		usbhw_clr_eps_irq(BIT(EDP_ID_KEYBOARD_IN));
		usbhw_reset_ep_ptr(EDP_ID_KEYBOARD_IN);
	}

	if(irq & BIT(EDP_ID_KEYS_IN & 0x07)){
		//printf("Endpoint %01x input irq!\n",EDP_ID_KEYS_IN);
		usbhw_clr_eps_irq(BIT(EDP_ID_KEYS_IN));
		usbhw_reset_ep_ptr(EDP_ID_KEYS_IN);
	}

	//Keyboard endpoint1 Output IRQ
	if(irq & BIT(EDP_ID_KEYBOARD_OUT & 0x07)){
		//printf("Endpoint %01x output irq!\n",EDP_ID_KEYBOARD_OUT);
		usbhw_clr_eps_irq(BIT(EDP_ID_KEYBOARD_OUT));

		usbhw_reset_ep_ptr(EDP_ID_KEYBOARD_OUT);
		my_g_stall = 0;

		//u8 led_state = usbhw_read_ep_data(EDP_ID_KEYBOARD_OUT);
		//printf("read 1 byte:data = 0x%01X", led_state);

		//Num Lock   :bit0
		//Caps Lock  :bit1
		//Scroll Lock:bit2
		//Compose    :bit3
		//Kana       :bit4
//		led_onoff(GREEN_LED, led_state&BIT(1));

		if(my_g_stall){
			usbhw_data_ep_stall(EDP_ID_KEYBOARD_OUT);
			//printf("STALL!\n");}
		}else{
			usbhw_data_ep_ack(EDP_ID_KEYBOARD_OUT);
			//printf("ACK!\n");}
		}
	}

	if(irq & BIT(EDP_ID_KEYS_OUT & 0x07)){
		//printf("Endpoint %01x output irq!\n",EDP_ID_KEYS_OUT);
		usbhw_clr_eps_irq(BIT(EDP_ID_KEYS_OUT));

		usbhw_reset_ep_ptr(EDP_ID_KEYS_OUT);
		my_g_stall = 0;

		//u8 led_state = usbhw_read_ep_data(EDP_ID_KEYS_OUT);
		//printf("read 1 byte:data = 0x%01X", led_state);

		//Num Lock   :bit0
		//Caps Lock  :bit1
		//Scroll Lock:bit2
		//Compose    :bit3
		//Kana       :bit4
//		led_onoff(GREEN_LED, led_state&BIT(1));

		if(my_g_stall){
			usbhw_data_ep_stall(EDP_ID_KEYS_OUT);
			//printf("STALL!\n");}
		}else{
			usbhw_data_ep_ack(EDP_ID_KEYS_OUT);
			//printf("ACK!\n");}
		}
	}


	if(reg_irq_src & FLD_IRQ_USB_RST_EN) {						//Step2-4: if rx packet is USB reset
		my_g_protocol = 1;										//1: report protocol;//0: start protocol
		//printf("--------------------------------------------- \r\n");
		//printf("USB RESET! \r\n");
		reg_irq_src3 = BIT(1);									//Clear USB reset flag
	}

	my_g_stall = 0;

//	if(IRQ_USB_PWDN_ENABLE && (reg_irq_src & FLD_IRQ_USB_PWDN_EN))
//	{
//		//printf("USB POWER DOWN! \r\n");
//		reg_irq_src = FLD_IRQ_USB_PWDN_EN;
//	}
//	else {
//
//	}
}

void my_usb_send_response(void) {
	u16 n;
	if(g_response_len < 8) {
		n = g_response_len;
	}
	else {
		n = 8;
	}

	g_response_len -= n;
	usbhw_reset_ctrl_ep_ptr();

	while(n-- > 0) {
		usbhw_write_ctrl_ep_data(*g_response);
		//printHEX(*g_response);
		++g_response;
	}
	//printf(" \r\n");
}

int MouseHidReport(unsigned char * p)
{
	//if(reg_usb_ep_ctrl(EDP_ID_MOUSE) & FLD_USB_EP_BUSY)
	if(usbhw_is_ep_busy(EDP_ID_MOUSE))
		return 0;

	signed short x = (signed char)p[1] ;//& BIT7 ? -(p[1] & 0x7f) : p[1];
	signed short y = (signed char)p[2] ;//& BIT7 ? -(p[2] & 0x7f) : p[2];

	reg_usb_ep_ptr(EDP_ID_MOUSE) = 0;

	if(my_g_protocol == 0){
		reg_usb_ep_dat(EDP_ID_MOUSE) = x & 0xff;
		reg_usb_ep_dat(EDP_ID_MOUSE) = y & 0xff;
	}
	else {
		reg_usb_ep_dat(EDP_ID_MOUSE) = p[0];
		reg_usb_ep_dat(EDP_ID_MOUSE) = x;
		reg_usb_ep_dat(EDP_ID_MOUSE) = y;
		reg_usb_ep_dat(EDP_ID_MOUSE) = p[3];
	}
	reg_usb_ep_ctrl(EDP_ID_MOUSE) = FLD_EP_DAT_ACK;
	return 1;
}

