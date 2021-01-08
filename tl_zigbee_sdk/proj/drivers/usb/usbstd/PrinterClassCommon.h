/********************************************************************************************************
 * @file	PrinterClassCommon.h
 *
 * @brief	This is the header file for PrinterClassCommon
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
#include "bit.h"
	
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	extern "C" {
#endif
	

/* Macros: */
/** \name Virtual Printer Status Line Masks */
//@{
/** Port status mask for a printer device, indicating that an error has *not* occurred. */
#define PRNT_PORTSTATUS_NOTERROR	BIT(3)

/** Port status mask for a printer device, indicating that the device is currently selected. */
#define PRNT_PORTSTATUS_SELECT		BIT(4)

/** Port status mask for a printer device, indicating that the device is currently out of paper. */
#define PRNT_PORTSTATUS_PAPEREMPTY	BIT(5)
//@}

/* Enums: */
/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors relating to the Printer
 *	device class.
 */
enum PRNT_Descriptor_ClassSubclassProtocol_t
{
	PRNT_CSCP_PrinterClass			= 0x07, /**< Descriptor Class value indicating that the device or interface
											 *	 belongs to the Printer class.
											 */
	PRNT_CSCP_PrinterSubclass		= 0x01, /**< Descriptor Subclass value indicating that the device or interface
											 *	 belongs to the Printer subclass.
											 */
	PRNT_CSCP_BidirectionalProtocol = 0x02, /**< Descriptor Protocol value indicating that the device or interface
											 *	 belongs to the Bidirectional proto of the Printer class.
											 */
};

/** Enum for the Printer class specific control requests that can be issued by the USB bus host. */
enum PRNT_ClassRequests_t
{
	PRNT_REQ_GetDeviceID			= 0x00, /**< Printer class-specific request to retrieve the Unicode ID
											 *	 string of the device, containing the device's name, manufacturer
											 *	 and supported printer languages.
											 */
	PRNT_REQ_GetPortStatus			= 0x01, /**< Printer class-specific request to get the current status of the
											 *	 virtual printer port, for device selection and ready states.
											 */
	PRNT_REQ_SoftReset				= 0x02, /**< Printer class-specific request to reset the device, ready for new
											 *	 printer commands.
											 */
};

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	}
#endif

