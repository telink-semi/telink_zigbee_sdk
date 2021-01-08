/********************************************************************************************************
 * @file	CDCClassDevice.h
 *
 * @brief	This is the header file for CDCClassDevice
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
#include "CDCClassCommon.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
    extern "C" {
#endif

/* Public Interface - May be used in end-application: */
/* Type Defines: */
/** \brief CDC Class Device Mode Configuration and State Structure.
 *
 *  Class state structure. An instance of this structure should be made for each CDC interface
 *  within the user application, and passed to each of the CDC class driver functions as the
 *  CDCInterfaceInfo parameter. This stores each CDC interface's configuration and state information.
 */
typedef struct
{
	struct
	{
		u8       ControlInterfaceNumber; /**< Interface number of the CDC control interface within the device. */

		u8       DataINEndpointNumber; /**< Endpoint number of the CDC interface's IN data endpoint. */
		u16      DataINEndpointSize; /**< Size in bytes of the CDC interface's IN data endpoint. */
		bool     DataINEndpointDoubleBank; /**< Indicates if the CDC interface's IN data endpoint should use double banking. */

		u8       DataOUTEndpointNumber; /**< Endpoint number of the CDC interface's OUT data endpoint. */
		u16      DataOUTEndpointSize;  /**< Size in bytes of the CDC interface's OUT data endpoint. */
		bool     DataOUTEndpointDoubleBank; /**< Indicates if the CDC interface's OUT data endpoint should use double banking. */

		u8       NotificationEndpointNumber; /**< Endpoint number of the CDC interface's IN notification endpoint, if used. */
		u16      NotificationEndpointSize;  /**< Size in bytes of the CDC interface's IN notification endpoint, if used. */
		bool     NotificationEndpointDoubleBank; /**< Indicates if the CDC interface's notification endpoint should use double banking. */
	} Config; /**< Config data for the USB class interface within the device. All elements in this section
			   *   <b>must</b> be set or the interface will fail to enumerate and operate correctly.
			   */
	struct
	{
		struct
		{
			u16      HostToDevice; /**< Control line states from the host to device, as a set of \c CDC_CONTROL_LINE_OUT_*
									*   masks. This value is updated each time \ref CDC_Device_USBTask() is called.
									*/
			u16      DeviceToHost; /**< Control line states from the device to host, as a set of \c CDC_CONTROL_LINE_IN_*
									*   masks - to notify the host of changes to these values, call the
									*   \ref CDC_Device_SendControlLineStateChange() function.
									*/
		} ControlLineStates; /**< Current states of the virtual serial port's control lines between the device and host. */

		CDC_LineEncoding_t LineEncoding; /** Line encoding used in the virtual serial port, for the device's information.
										  *  This is generally only used if the virtual serial port data is to be
										  *  reconstructed on a physical UART.
										  */
	} State; /**< State data for the USB class interface within the device. All elements in this section
			  *   are reset to their defaults when the interface is enumerated.
			  */
} USB_ClassInfo_CDC_Device_t;


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	}
#endif


