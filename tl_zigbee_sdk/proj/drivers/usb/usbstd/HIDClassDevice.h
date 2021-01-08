/********************************************************************************************************
 * @file	HIDClassDevice.h
 *
 * @brief	This is the header file for HIDClassDevice
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
/* Type Defines: */
/** \brief HID Class Device Mode Configuration and State Structure.
 *
 *  Class state structure. An instance of this structure should be made for each HID interface
 *  within the user application, and passed to each of the HID class driver functions as the
 *  \c HIDInterfaceInfo parameter. This stores each HID interface's configuration and state information.
 *
 *  \note Due to technical limitations, the HID device class driver does not utilize a separate OUT
 *        endpoint for host->device communications. Instead, the host->device data (if any) is sent to
 *        the device via the control endpoint.
 */
typedef struct usbhid_config_t
{
	u8  InterfaceNumber; /**< Interface number of the HID interface within the device. */

	u8  ReportINEndpointNumber; /**< Endpoint number of the HID interface's IN report endpoint. */
	u16 ReportINEndpointSize; /**< Size in bytes of the HID interface's IN report endpoint. */
	bool ReportINEndpointDoubleBank; /**< Indicates if the HID interface's IN report endpoint should use double banking. */

	void* PrevReportINBuffer; /**< Pointer to a buffer where the previously created HID input report can be
	                              *  stored by the driver, for comparison purposes to detect report changes that
	                              *  must be sent immediately to the host. This should point to a buffer big enough
	                              *  to hold the largest HID input report sent from the HID interface. If this is set
								  *  to \c NULL, it is up to the user to force transfers when needed in the
								  *  \ref CALLBACK_HID_Device_CreateHIDReport() callback function.
								  *
								  *  \note Due to the single buffer, the internal driver can only correctly compare
								  *        subsequent reports with identical report IDs. In multiple report devices,
								  *        this buffer should be set to \c NULL and the decision to send reports made
								  *        by the user application instead.
	                              */
	u8  PrevReportINBufferSize; /**< Size in bytes of the given input report buffer. This is used to create a
	                                  *  second buffer of the same size within the driver so that subsequent reports
	                                  *  can be compared. If the user app is to determine when reports are to be sent
	                                  *  exclusively (i.e. \ref PrevReportINBuffer is \c NULL) this value must still be
									  *  set to the size of the largest report the device can issue to the host.
	                                  */
} usbhid_config_t; /**< Config data for the USB class interface within the device. All elements in this section
           *   <b>must</b> be set or the interface will fail to enumerate and operate correctly.
           */
typedef struct usbhid_state_t
{
	bool UsingReportProtocol; /**< Indicates if the HID interface is set to Boot or Report proto mode. */
	u16 PrevFrameNum; /**< Frame number of the previous HID report packet opportunity. */
	u16 IdleCount; /**< Report idle period, in milliseconds, set by the host. */
	u16 IdleMSRemaining; /**< Total number of milliseconds remaining before the idle period elapsed - this
							   *   should be decremented by the user application if non-zero each millisecond. */
} usbhid_state_t; /**< State data for the USB class interface within the device. All elements in this section
          *   are reset to their defaults when the interface is enumerated.
          */
typedef struct
{
	
} USB_ClassInfo_HID_Device_t;


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	}
#endif



