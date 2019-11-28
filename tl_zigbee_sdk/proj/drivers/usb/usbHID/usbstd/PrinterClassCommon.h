#pragma once
	/* Includes: */
#include "../../../../common/types.h"
	
	/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	extern "C" {
#endif
	

/* Macros: */
	/** \name Virtual Printer Status Line Masks */
	//@{
	/** Port status mask for a printer device, indicating that an error has *not* occurred. */
	#define PRNT_PORTSTATUS_NOTERROR	(1 << 3)

	/** Port status mask for a printer device, indicating that the device is currently selected. */
	#define PRNT_PORTSTATUS_SELECT		(1 << 4)

	/** Port status mask for a printer device, indicating that the device is currently out of paper. */
	#define PRNT_PORTSTATUS_PAPEREMPTY	(1 << 5)
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

