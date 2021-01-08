/********************************************************************************************************
 * @file	StdRequestType.h
 *
 * @brief	This is the header file for StdRequestType
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
#pragma once

/* Includes: */
#include "types.h"

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

#define FEATURE_SELFPOWERED_ENABLED     (1 << 0)
#define FEATURE_REMOTE_WAKEUP_ENABLED   (1 << 1)
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
	u8  bmRequestType; /**< Type of the request. */
	u8  bRequest; /**< Request command code. */
	u16 wValue; /**< wValue parameter of the request. */
	u16 wIndex; /**< wIndex parameter of the request. */
	u16 wLength; /**< Length of the data to transfer in bytes. */
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

	
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	}
#endif



