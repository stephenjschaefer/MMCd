/*******************************************************************
 *
 * Project:
 *		USB Library
 *
 * Copyright info:
 *		 Copyright(c) 1998, Handspring Inc., All Rights Reserved
 *
 * FileName:
 *		UsbLib.h
 *
 * Description:
 *		This file contains functions for the USB Library 
 *    that replaces the Palm Serial Library.  This library is API
 *    conpatible with the Serial Library.
 *
 * History:
 *   	21-Dec-1998		Created by Art Lamb  (alamb@handspring.com)
 *
 *******************************************************************/

#ifndef __USB_MGR_H
#define __USB_MGR_H

#include <SerialMgrOld.h>	  // For SerSettingsType

// If building test verision, build without traps.
#ifdef BUILD_FOR_TESTING
	#define USB_SYS_TRAP(trapNum)
#else
	#define USB_SYS_TRAP SYS_TRAP
#endif

// Temporary place for custom control codes.  This support will be officially
// available in PalmOS 4.0
#define sdrvOpCodeCustomStart		0x8000
#define vdrvOpCodeCustomStart		0x8000

// ------------------------------------------------------------------------------
// Name of the USB library. Should be passed to SysLibFind()
// NOTE: The "UsbDispatch.c" jump table library name must be
//  manually updated if this is changed!!
// ------------------------------------------------------------------------------
#define	usbLibName	  "USB Library"


// These are the various driver ID's that we install for SrmOpen()
#define	usbLibSrmDriverIDConsole	hsFileCSerDrvrUSBConsole	  // USB for debugger console
#define	usbLibSrmDriverIDDesktop	hsFileCSerDrvrUSBDesktop	  // USB for HotSync
#define	usbLibSrmDriverIDRemFS		hsFileCSerDrvrUSBRemFS		  // USB for remote File system
#define	usbLibSrmDriverIDGeneric	hsFileCSerDrvrUSBGeneric	  // USB for generic, endpoint 1
#define	usbLibSrmDriverIDGeneric2	hsFileCSerDrvrUSBGeneric2	  // USB for generic, endpoint 2


// ------------------------------------------------------------------------------
// Equates for the USB ports
// ------------------------------------------------------------------------------
// A Usb port number is a bit-field, the low byte contains the endpoint used 
// by the port, and the high byte contains a function code.
#define usbLibEndpoint1				0x01
#define usbLibEndpoint2				0x02

#define usbLibFunctionGeneric		0x00
#define usbLibFunctionDebugger		0x01
#define usbLibFunctionHotsync		0x02
#define usbLibFunctionConsole		0x03
#define usbLibFunctionRemoteFileSys	0x04

#define usbLibEndpointMask			0x00FF
#define usbLibFunctionMask			0xFF00

#define usbLibPortDefault			0
#define usbLibPort1					1
#define usbLibPort2					2
#define usbLibPortHotsync			(usbLibEndpoint2 | (usbLibFunctionHotsync << 8) )
#define usbLibPortDebugger			(usbLibEndpoint1 | (usbLibFunctionDebugger << 8) )
#define usbLibPortConsole			(usbLibEndpoint1 | (usbLibFunctionConsole << 8) )
#define usbLibPortRemoteFileSys		(usbLibEndpoint2 | (usbLibFunctionRemoteFileSys << 8) )





// ------------------------------------------------------------------------------
// Control codes when using the new SerialHWMgr to access USB
//  Each of these accepts a UsbCtlBlockParamType structure
// ------------------------------------------------------------------------------
#define	usbOpCodeBlockReceive			  vdrvOpCodeCustomStart+0  // &UsbCtlBlockParamType
#define	usbOpCodeBlockReceiveProtected	  vdrvOpCodeCustomStart+1  // &UsbCtlBlockParamType
#define	usbOpCodeBlockSend				  vdrvOpCodeCustomStart+2  // &UsbCtlBlockParamType
#define	usbOpCodeBlockModeEnable		  vdrvOpCodeCustomStart+3  // &UInt8


// Structure passed in the 'valueP' parameter to SrmControl for each of the
//   usbOpCodeBlockSend/Receive control calls:
typedef struct
  {
	void*	  bufP;					// IN: buffer pointer usbOpCodeBlockReceive 
									//      and usbOpCodeBlockSend
									// IN: record pointer for BlockReceiveProtected()
	UInt32	  recordOffset;			// IN: offset within record for BlockReceiveProtected()
	UInt32	  bytes;				// IN:  # of bytes to read/write
									// OUT: # of bytes read or written 
	UInt16	  width;				// IN: width of destination buffer 
									//		1 : write a UInt8 at a time
									//		2 : write a UInt16 at a time
	UInt8	  fixedBuf;				// IN: if true, bufP is a hardware address that should
									//		not be incremented after each UInt8/UInt16
									//		read/write. 
	UInt8	  reserved;				// IN: must be 0
	Int32	  timeout;				// IN: timeout in ticks
  }
UsbCtlBlockParamType;





// ------------------------------------------------------------------------------
// Library routines
// ------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif


Err UsbLibOpen(UInt16 refNum, UInt16 port, UInt32 baud)
				USB_SYS_TRAP(sysLibTrapOpen);

Err UsbLibDbgAssureOpen (UInt16 refNum, UInt16 port, UInt32 baud);


Err UsbLibClose(UInt16 refNum)
				USB_SYS_TRAP(sysLibTrapClose);

Err UsbLibSleep(UInt16 refNum)
				USB_SYS_TRAP(sysLibTrapSleep);

Err UsbLibWake(UInt16 refNum)
				USB_SYS_TRAP(sysLibTrapWake);

Err UsbLibGetSettings(UInt16 refNum, SerSettingsPtr settingsP)
				USB_SYS_TRAP(sysLibTrapCustom);

Err UsbLibSetSettings(UInt16 refNum, SerSettingsPtr settingsP)
				USB_SYS_TRAP(sysLibTrapCustom+1);

UInt16 UsbLibGetStatus(UInt16 refNum, Boolean* ctsOnP, Boolean* dsrOnP)
				USB_SYS_TRAP(sysLibTrapCustom+2);

Err UsbLibClearErr(UInt16 refNum)
				USB_SYS_TRAP(sysLibTrapCustom+3);



Err UsbLibSend10(UInt16 refNum, MemPtr bufP, UInt32 size)
				USB_SYS_TRAP(sysLibTrapCustom+4);

Err UsbLibSendWait(UInt16 refNum, Int32 timeout)
				USB_SYS_TRAP(sysLibTrapCustom+5);

Err UsbLibSendCheck(UInt16 refNum, UInt32* numBytesP)
				USB_SYS_TRAP(sysLibTrapCustom+6);

Err UsbLibSendFlush(UInt16 refNum)
				USB_SYS_TRAP(sysLibTrapCustom+7);



Err UsbLibReceive10(UInt16 refNum, MemPtr bufP, UInt32 bytes, Int32 timeout)
				USB_SYS_TRAP(sysLibTrapCustom+8);

Err UsbLibReceiveWait(UInt16 refNum, UInt32 bytes, Int32 timeout)
				USB_SYS_TRAP(sysLibTrapCustom+9);

Err UsbLibReceiveCheck(UInt16 refNum, UInt32* numBytesP)
				USB_SYS_TRAP(sysLibTrapCustom+10);

void UsbLibReceiveFlush(UInt16 refNum, Int32 timeout)
				USB_SYS_TRAP(sysLibTrapCustom+11);

Err UsbLibSetReceiveBuffer(UInt16 refNum, MemPtr bufP, UInt16 bufSize)
				USB_SYS_TRAP(sysLibTrapCustom+12);



Err UsbLibReceiveWindowOpen(UInt16 refNum, UInt8** bufPP, UInt32* sizeP)
				USB_SYS_TRAP(sysLibTrapCustom+13);

Err UsbLibReceiveWindowClose(UInt16 refNum, UInt32 bytesPulled)
				USB_SYS_TRAP(sysLibTrapCustom+14);


Err UsbLibSetWakeupHandler(UInt16 refNum, SerWakeupHandler procP, UInt32 refCon)
				USB_SYS_TRAP(sysLibTrapCustom+15);

Err UsbLibPrimeWakeupHandler(UInt16 refNum, UInt16 minBytes)
				USB_SYS_TRAP(sysLibTrapCustom+16);

Err UsbLibControl(UInt16 refNum, UInt16 op, MemPtr valueP, UInt16* valueLenP)
				USB_SYS_TRAP(sysLibTrapCustom+17);


UInt32 UsbLibSend(UInt16 refNum, MemPtr bufP, UInt32 count, Err* errP)
				USB_SYS_TRAP(sysLibTrapCustom+18);

UInt32 UsbLibReceive(UInt16 refNum, MemPtr bufP, UInt32 count, Int32 timeout, Err* errP)
				USB_SYS_TRAP(sysLibTrapCustom+19);



UInt32 UsbLibBlockReceive (UInt16 refNum, MemPtr dest, UInt32 bytesToRead, Boolean fixedDest, 
				Boolean destWidth, Int32 timeout)
				USB_SYS_TRAP(sysLibTrapCustom+20);

UInt32 UsbLibBlockReceiveProtected (UInt16 refNum, UInt8* recordP, UInt16 offset, UInt32 bytesToRead, 
				Int32 timeout)
				USB_SYS_TRAP(sysLibTrapCustom+21);

UInt32 UsbLibBlockSend (UInt16 refNum, MemPtr src, UInt32 bytesToSend, Boolean fixedSrc, 
				UInt8 scrWidth, Int32 timeout)
				USB_SYS_TRAP(sysLibTrapCustom+22);

void UsbLibBlockModeEnabled (UInt16 refNum, Boolean enabled)
				USB_SYS_TRAP(sysLibTrapCustom+23);


#ifdef __cplusplus
}
#endif

#endif	//__USB_MGR_H
