/********************************************************************************************************
 * @file	USBController.h
 *
 * @brief	This is the header file for USBController
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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/* Defines: */
/** \name Endpoint Direction Masks */
//@{
/** Endpoint direction mask, for masking against endpoint addresses to retrieve the endpoint's
 *	direction for comparing with the \c ENDPOINT_DIR_* masks.
 */
#define ENDPOINT_DIR_MASK				   0x80

/** Endpoint address direction mask for an OUT direction (Host to Device) endpoint. This may be ORed with
 *	the index of the address within a device to obtain the full endpoint address.
 */
#define ENDPOINT_DIR_OUT				   0x00

/** Endpoint address direction mask for an IN direction (Device to Host) endpoint. This may be ORed with
 *	the index of the address within a device to obtain the full endpoint address.
 */
#define ENDPOINT_DIR_IN 				   0x80
//@}

/** \name Endpoint/Pipe Type Masks */
//@{
/** Mask for determining the type of an endpoint from an endpoint descriptor. This should then be compared
 *	with the \c EP_TYPE_* masks to determine the exact type of the endpoint.
 */
#define EP_TYPE_MASK					   0x03

/** Mask for a CONTROL type endpoint or pipe.
 *
 *	\note See \ref Group_EndpointManagement and \ref Group_PipeManagement for endpoint/pipe functions.
 */
#define EP_TYPE_CONTROL 				   0x00

/** Mask for an ISOCHRONOUS type endpoint or pipe.
 *
 *	\note See \ref Group_EndpointManagement and \ref Group_PipeManagement for endpoint/pipe functions.
 */
#define EP_TYPE_ISOCHRONOUS 			   0x01

/** Mask for a BULK type endpoint or pipe.
 *
 *	\note See \ref Group_EndpointManagement and \ref Group_PipeManagement for endpoint/pipe functions.
 */
#define EP_TYPE_BULK					   0x02

/** Mask for an INTERRUPT type endpoint or pipe.
 *
 *	\note See \ref Group_EndpointManagement and \ref Group_PipeManagement for endpoint/pipe functions.
 */
#define EP_TYPE_INTERRUPT				   0x03

/**If Isochronous endpoint,
Bits 3..2 = Synchronisation Type (Iso Mode)
00 = No Synchonisation
01 = Asynchronous
10 = Adaptive
11 = Synchronous
Bits 5..4 = Usage Type (Iso Mode)
00 = Data Endpoint
01 = Feedback Endpoint
10 = Explicit Feedback Data Endpoint
11 = Reserved
*/
#define EP_SYNC_TYPE_NO_SYNC                0
#define EP_SYNC_TYPE_ASYN                   1
#define EP_SYNC_TYPE_ADAPTIVE               2
#define EP_SYNC_TYPE_SYNC                   3

#define EP_USAGE_TYPE_DATA                  0
#define EP_USAGE_TYPE_FEEDBACK              1
#define EP_USAGE_TYPE_FEEDBACK_DATA         2
#define EP_USAGE_TYPE_RSV                   3

//@}

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

