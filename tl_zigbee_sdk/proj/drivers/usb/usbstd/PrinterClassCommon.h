/********************************************************************************************************
 * @file     PrinterClassCommon.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma once

/* Includes: */
#include "tl_common.h"
	
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

