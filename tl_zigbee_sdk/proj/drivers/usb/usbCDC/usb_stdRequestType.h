/********************************************************************************************************
 * @file     usb_stdRequestType.h
 *
 * @brief    This file provides definitions of requests for standard USB protocol
 *
 * @author   xiaodong.zong@telink-semi.com; jian.zhang@telink-semi.com
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
#pragma once
/* Includes: */
//#include "../common/types.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	extern "C" {
#endif

/* Public Interface - May be used in end-application: */
/* Macros: */
	/** Mask for the request type parameter, to indicate the direction of the request data (Host to Device
	 *  or Device to Host). The result of this mask should then be compared to the request direction masks.
	 *
	 *  \see \c REQDIR_* macros for masks indicating the request data direction.
	 */
	#define CONTROL_REQTYPE_DIRECTION  0x80

	/** Mask for the request type parameter, to indicate the type of request (Device, Class or Vendor
	 *  Specific). The result of this mask should then be compared to the request type masks.
	 *
	 *  \see \c REQTYPE_* macros for masks indicating the request type.
	 */
	#define CONTROL_REQTYPE_TYPE       0x60

	/** Mask for the request type parameter, to indicate the recipient of the request (Device, Interface
	 *  Endpoint or Other). The result of this mask should then be compared to the request recipient
	 *  masks.
	 *
	 *  \see \c REQREC_* macros for masks indicating the request recipient.
	 */
	#define CONTROL_REQTYPE_RECIPIENT  0x1F

	/** \name Control Request Data Direction Masks */
	//@{
	/** Request data direction mask, indicating that the request data will flow from host to device.
	 *
	 *  \see \ref CONTROL_REQTYPE_DIRECTION macro.
	 */
	#define REQDIR_HOSTTODEVICE        (0 << 7)

	/** Request data direction mask, indicating that the request data will flow from device to host.
	 *
	 *  \see \ref CONTROL_REQTYPE_DIRECTION macro.
	 */
	#define REQDIR_DEVICETOHOST        (1 << 7)
	//@}

	/** \name Control Request Type Masks */
	//@{
	/** Request type mask, indicating that the request is a standard request.
	 *
	 *  \see \ref CONTROL_REQTYPE_TYPE macro.
	 */
	#define REQTYPE_STANDARD           (0 << 5)

	/** Request type mask, indicating that the request is a class-specific request.
	 *
	 *  \see \ref CONTROL_REQTYPE_TYPE macro.
	 */
	#define REQTYPE_CLASS              (1 << 5)

	/** Request type mask, indicating that the request is a vendor specific request.
	 *
	 *  \see \ref CONTROL_REQTYPE_TYPE macro.
	 */
	#define REQTYPE_VENDOR             (2 << 5)
	//@}

	/** \name Control Request Recipient Masks */
	//@{
	/** Request recipient mask, indicating that the request is to be issued to the device as a whole.
	 *
	 *  \see \ref CONTROL_REQTYPE_RECIPIENT macro.
	 */
	#define REQREC_DEVICE              (0 << 0)

	/** Request recipient mask, indicating that the request is to be issued to an interface in the
	 *  currently selected configuration.
	 *
	 *  \see \ref CONTROL_REQTYPE_RECIPIENT macro.
	 */
	#define REQREC_INTERFACE           (1 << 0)

	/** Request recipient mask, indicating that the request is to be issued to an endpoint in the
	 *  currently selected configuration.
	 *
	 *  \see \ref CONTROL_REQTYPE_RECIPIENT macro.
	 */
	#define REQREC_ENDPOINT            (2 << 0)

	/** Request recipient mask, indicating that the request is to be issued to an unspecified element
	 *  in the currently selected configuration.
	 *
	 *  \see \ref CONTROL_REQTYPE_RECIPIENT macro.
	 */
	#define REQREC_OTHER               (3 << 0)
	//@}

/* Type Defines: */
	/** \brief Standard USB Control Request
	 *
	 *  Type define for a standard USB control request.
	 *
	 *  \see The USB 2.0 specification for more information on standard control requests.
	 */
	typedef struct
	{
		unsigned char  bmRequestType; /**< Type of the request. */
		unsigned char  bRequest; /**< Request command code. */
		unsigned short wValue; /**< wValue parameter of the request. */
		unsigned short wIndex; /**< wIndex parameter of the request. */
		unsigned short wLength; /**< Length of the data to transfer in bytes. */
	}USB_Request_Header_t;

/* Enums: */
	/** Enumeration for the various standard request commands. These commands are applicable when the
	 *  request type is \ref REQTYPE_STANDARD (with the exception of \ref REQ_GetDescriptor, which is always
	 *  handled regardless of the request type value).
	 *
	 *  \see Chapter 9 of the USB 2.0 Specification.
	 */
	enum USB_Control_Request_t
	{
		REQ_GetStatus           = 0, /**< Implemented in the library for device and endpoint recipients. Passed
		                              *   to the user application for other recipients via the
		                              *   \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_ClearFeature        = 1, /**< Implemented in the library for device and endpoint recipients. Passed
		                              *   to the user application for other recipients via the
		                              *   \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_SetFeature          = 3, /**< Implemented in the library for device and endpoint recipients. Passed
		                              *   to the user application for other recipients via the
		                              *   \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_SetAddress          = 5, /**< Implemented in the library for the device recipient. Passed
		                              *   to the user application for other recipients via the
		                              *   \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_GetDescriptor       = 6, /**< Implemented in the library for device and interface recipients. Passed to the
		                              *   user application for other recipients via the
		                              *   \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_SetDescriptor       = 7, /**< Not implemented in the library, passed to the user application
		                              *   via the \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_GetConfiguration    = 8, /**< Implemented in the library for the device recipient. Passed
		                              *   to the user application for other recipients via the
		                              *   \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_SetConfiguration    = 9, /**< Implemented in the library for the device recipient. Passed
		                              *   to the user application for other recipients via the
		                              *   \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_GetInterface        = 10, /**< Not implemented in the library, passed to the user application
		                              *   via the \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_SetInterface        = 11, /**< Not implemented in the library, passed to the user application
		                              *   via the \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
		REQ_SynchFrame          = 12, /**< Not implemented in the library, passed to the user application
		                              *   via the \ref EVENT_USB_Device_ControlRequest() event when received in
		                              *   device mode. */
	};

	/** Feature Selector values for Set Feature and Clear Feature standard control requests directed to the device, interface
	 *  and endpoint recipients.
	 */
	enum USB_Feature_Selectors_t
	{
		FEATURE_SEL_EndpointHalt       = 0x00, /**< Feature selector for Clear Feature or Set Feature commands. When
		                                        *   used in a Set Feature or Clear Feature request this indicates that an
		                                        *   endpoint (whose address is given elsewhere in the request) should have
		                                        *   its stall condition changed.
		                                        */
		FEATURE_SEL_DeviceRemoteWakeup = 0x01, /**< Feature selector for Device level Remote Wakeup enable set or clear.
	                                            *   This feature can be controlled by the host on devices which indicate
	                                            *   remote wakeup support in their descriptors to selectively disable or
	                                            *   enable remote wakeup.
	                                            */
		FEATURE_SEL_TestMode           = 0x02, /**< Feature selector for Test Mode features, used to test the USB controller
	                                            *   to check for incorrect operation.
	                                            */
	};

	#define FEATURE_SELFPOWERED_ENABLED     (1 << 0)
	#define FEATURE_REMOTE_WAKEUP_ENABLED   (1 << 1)


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	}
#endif

/** @} */


