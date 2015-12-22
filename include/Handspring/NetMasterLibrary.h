/***************************************************************
 *
 *  Project:
 *	  NetMaster Library
 *
 * Copyright info:
 *
 *	  Copyright (c) Handspring 2002 -- All Rights Reserved
 *
 *
 *  FileName:
 *	  NetMasterLibrary.h
 * 
 *  Description:
 *	  This is the main header file for the NetMaster Library.
 *
 *	Note:
 *
 *	History:
 *	  31-Jan-2002	vmk	  Created by Vitaly Kruglikov
 *
 ****************************************************************/

#ifndef _NET_MASTER_LIBRARY_H_
#define _NET_MASTER_LIBRARY_H_


//#include "OSServices.h"
#include "NetMasterLibTarget.h"
#include "NetMasterLibTraps.h"
#include "NetMasterLibErrors.h"
#include "NetMasterLibTypes.h"


// ==================================================================
// Usage Model
// ==================================================================
// The NetMaster library is preloaded by the system software before
//  the system sends sysAppLaunchCmdSystemReset.  Clients of NetMaster
//  library need to call SysLibFind (netMasterLibName,...) to get
//  a library refNum of NetMaster.  If SysLibFind returns a non-zero
//  error, clients *MUST* assume that NetMaster was not loaded for a
//  good reason (such as when the user performed a "safe" reset), or is
//  simply not present on the system and fail gracefully.  Clients
//  *MUST NOT* load NetMaster themselves (such as via SysLibLoad or
//  SysLibInstall) -- keep in mind that if the system didn't load it,
//  there was a good reason for it!


#ifdef BUILDING_NET_MASTER_LIB_DISPATCH_TABLE	// defined in the dispatch table
									  //  module when building the NetMaster
									  //  library itself
  #define NETMASTER_LIB_TRAP(trapNum)

#else
  #define NETMASTER_LIB_TRAP(trapNum) SYS_TRAP(trapNum)
#endif



#ifdef __cplusplus
extern "C" {
#endif

/** Standard library open routine.
 *  
 *  @param refnum          IN  Library reference number
 *  @retval Err                Library error code
 */
extern Err NetMasterLibOpen (UInt16 refNum, struct NetMasterContextTypeTag** cxtPP)
	NETMASTER_LIB_TRAP (sysLibTrapOpen);

/** Standard library close routine.
 *  
 *  @param refnum          IN  Library reference number
 *  @retval Err                Library error code
 */
extern Err NetMasterLibClose (UInt16 refNum, struct NetMasterContextTypeTag* cxtP)
	NETMASTER_LIB_TRAP (sysLibTrapClose);

/** Standard library sleep routine.
 *  
 *  @param refnum          IN  Library reference number
 *  @retval Err                Library error code
 */
extern Err NetMasterLibSleep (UInt16 refNum)
	NETMASTER_LIB_TRAP (sysLibTrapSleep);

/** Standard library wake routine.
 *  
 *  @param refnum          IN  Library reference number
 *  @retval Err                Library error code
 */
extern Err NetMasterLibWake (UInt16 refNum)
	NETMASTER_LIB_TRAP (sysLibTrapWake);

/** Get Library API Version number.
 *
 *  @param refnum          IN  Library reference number
 *  @param majorVerP       OUT Major version number
 *  @param minorVerP       OUT Minor version number
 *  @param bugFixVerP      OUT Bug fix version number
 *  @retval Err          NetMaster Library error code
 */
extern Err
NetMasterLibVersionGet (UInt16 refNum, UInt32* majorVerP, UInt32* minorVerP,
						UInt32* bugFixVerP)
	NETMASTER_LIB_TRAP (netMasterLibTrapLibVersionGet);


// Install the NetLib patches for the fallback engine, etc.
extern Err
NetMasterNetLibPatchInstall (UInt16 refNum)
	NETMASTER_LIB_TRAP (netMasterLibTrapNetLibPatchInstall);


// NOTE: NOT IMPLEMENTED!
// Attach to the network using the specified service; this will call
//  NetLibOpen and execute any required fallback services, as necessary
//  on the caller's behalf.
//
// IMPORTANT: This call is semantically equivalent to calling NetLibOpen;
//  if this call succeeds (returns 0
//  error code), the caller is responsible for calling NetLibClose
//  when done with NetLib; if this call fails (non-zero error code
//  returned), the caller must *NOT* make the matching call to NetLibClose.
extern Err
NetMasterNetAttach (UInt16 refNum, struct NetMasterContextTypeTag* cxtP,
					UInt32 recID)
	NETMASTER_LIB_TRAP (netMasterLibTrapNetAttach);



// Get the "auto-login" settings
extern Err
NetMasterAutoLoginSettingGet (UInt16 refNum,
							  NetMasterAutoLoginType* settingsP)
	NETMASTER_LIB_TRAP (netMasterLibTrapAutoLoginSettingGet);


// NetMasterAutoLoginSettingSet:
//
// set the "auto-login" settings
//
//	  flagsToClear	  IN  Flags to clear -- any of the NetMasterAutoLoginType
//						   constants bitwise-or'ed together or
//						   netMasterAutoLoginClearAll to clear all flags; the
//						   flag clearing operation is performed before the
//						   flag setting operation. Pass 0 to skip the clearing
//						   operation.
//	  flagsToSet	  IN  Flags to set -- any of the NetMasterAutoLoginType
//						   constants bitwise-or'ed together. Pass 0 to skip the
//						   setting operation.
extern Err
NetMasterAutoLoginSettingSet (UInt16 refNum,
							  NetMasterAutoLoginType flagsToClear,
							  NetMasterAutoLoginType flagsToSet)
	NETMASTER_LIB_TRAP (netMasterLibTrapAutoLoginSettingSet);


// Function:	NetMasterServiceSetUp -- OBSOLETED, DON'T USE THIS FUNCTION
//
// Summary:		Configure NetLib to use a given NetPref service
//
// Parameters:
//	  refNum		  IN  NetMasterLib reference number (from SysLibFind
//						   or SysLibLoad)
//	  netPrefRecID	  IN  Unique record ID of a given NetPref service record
//
// Returns: 
//    0 on success
extern Err
NetMasterServiceSetUp (UInt16 refNum, UInt32 netPrefRecID)
	NETMASTER_LIB_TRAP (netMasterLibTrapServiceSetUp);


// Function:	NetMasterNetLibOpenIfFullyUp
//
// Summary:		Open NetLib only if all of its current interfaces are already
//				fully up.  If this call succeeds (returns true), it is
//				semantically equivalent to a successful NetLibOpen call, which
//				implies that the caller is responsible for calling NetLibClose
//				to match this call when done with NetLib.  If
//				NetMasterNetLibOpenIfFullyUp fails (returns false),
//				the caller *MUST NOT* make a matching call to NetLibClose.
//				
//
// Parameters:
//	  refNum		  IN  NetMasterLib reference number (from SysLibFind
//						   or SysLibLoad)
//	  netLibRefNumP	  OUT NetLib refnum on success, undefined on failure
//
// Returns: 
//    true if opened, false if not (failure)
extern Boolean
NetMasterNetLibOpenIfFullyUp (UInt16 refNum, UInt16* netLibRefNumP)
	NETMASTER_LIB_TRAP (netMasterLibTrapNetLibOpenIfFullyUp);


// Function:	NetMasterNetLibIsFullyUp
//
// Summary:		Checks if NetLib is fully up -- i.e. all of its interfaces
//				are up.
//				
//
// Parameters:
//	  refNum		  IN  NetMasterLib reference number (from SysLibFind
//						   or SysLibLoad)
//
// Returns: 
//    true if NetLib is fully up, false if at least one interface is down.
extern Boolean
NetMasterNetLibIsFullyUp (UInt16 refNum)
	NETMASTER_LIB_TRAP (netMasterLibTrapNetLibIsFullyUp);


// Function:	NetMasterPhoneServiceClassesGet
//
// Summary:		Returns the phone service classes of interest to NetMaster
//				library.  Since the phone library presently does not
//				support delivering phone events to libraries, a proxy
//				application (Network panel) will call this function
//				to get the Phone Event classes that are of interest
//				to NetMaster, and then register for those events on
//				its behalf.  Subsequently, the proxy application
//				will call NetMasterHandlePhoneEvent to allow NetMaster
//				to process phone events.
//				
//
// Parameters:
//	  refNum		  IN  NetMasterLib reference number (from SysLibFind
//						   or SysLibLoad)
//
// Returns: 
//    0 or more constants from PhoneServiceClassType bitwise-or'ed together.
extern UInt32
NetMasterPhoneServiceClassesGet (UInt16 refNum)
	NETMASTER_LIB_TRAP (netMasterLibTrapPhoneServiceClassesGet);


// Function:	NetMasterHandlePhoneEvent
//
// Summary:		Process phone events of interest.  See
//				NetMasterPhoneServiceClassesGet for more info.
//				
//
// Parameters:
//	  refNum		  IN  NetMasterLib reference number (from SysLibFind
//						   or SysLibLoad)
//	  cmd			  IN  launch code (phnLibLaunchCmdEvent, for example)
//	  cmdPBP		  IN  parameter block corresponding to the cmd.
//
// Returns: 
//    0 on success (it is anticipated that, in most cases, this error
//	   code will be ignored by the caller)
extern Err
NetMasterHandlePhoneEvent (UInt16 refNum, UInt16 cmd, void* cmdPBP)
	NETMASTER_LIB_TRAP (netMasterLibTrapHandlePhoneEvent);


// Internal, low-level function for use by system software only!
extern Err
NetMasterControl (UInt16 refNum, NetMasterCtlEnum ctlCode, void* ctlDataP)
	NETMASTER_LIB_TRAP (netMasterLibTrapControl);


// Shut down NetLib's interfaces, if they are presently up; does nothing
//  if they are presently down; blocking;  If NetLib is in the process
//  of logging on to the IP network (Connection progress dialog is up),
//  will simulate the press of the Cancel key and return without waiting
//  for interfaces to shut down.
// WARNING: will dead-lock if called from the context of a Notification
//  Manager notification.  This is because Notification Manager erroneously
//  grabs the Memory Manager semaphore before sending a notification, and
//  this causes a dead-lock when the TCP/IP stack's background task calls
//  into Memory Manager or Data Manager API as part of the shut-down.
extern Err
NetMasterNetInterfacesShutDown (UInt16 refNum, Err reasonErrCode,
							NetMasterNetIFShutDownOptionsType shutDownOptions)
	NETMASTER_LIB_TRAP (netMasterLibTrapNetInterfacesShutDown);


// Test if NetLib is in the Logging On state.
//  Returns true if some client is presently inside NetLibOpen or
//  NetLibOpenConfig.
//  	  isLogonActiveP
//  			  OUT [OPTIONAL] true if NetLib is in the middle of
//  				   logging in and an error has not yet been encountered.
//  				   This parameter is OPTIONAL -- pass NULL to ignore.
//  	  isPrgDialogUpP
//  			  OUT [OPTIONAL] if NetLib's Service Connection Prorgress
//  				   dialog is up, NetMasterIsLoggingIn will set
//  				   *isPrgDialogUpP to true; false otherwise.
//  				   This parameter is OPTIONAL -- pass NULL to ignore.
//  	  waitingInErrorStateP
//  			  OUT [OPTIONAL] if NetLib's Service Connection Prorgress
//  				   dialog is in the error acknowledegement state
//  				   (displaying an error message to the user,
//  				   waiting for OK), NetMasterIsLoggingIn will set
//  				   *waitingInErrorStateP to true; false otherwise.
//  				   This parameter is OPTIONAL -- pass NULL to ignore.
extern Boolean
NetMasterNetLibIsLoggingIn (UInt16 refNum, Boolean* isLogonActiveP,
							Boolean* isPrgDialogUpP,
							Boolean* waitingInErrorStateP)
	NETMASTER_LIB_TRAP (netMasterLibTrapNetLibIsLoggingIn);




#ifdef __cplusplus
}
#endif


#endif // _NET_MASTER_LIBRARY_H_
