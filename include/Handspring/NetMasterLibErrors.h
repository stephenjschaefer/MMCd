/***************************************************************
 *
 *  Project:
 *	  NetMaster Library
 *
 * Copyright info:
 *
 *	  Copyright (c) Handspring 2001 -- All Rights Reserved
 *
 *
 *  FileName:
 *	  NetMasterLibErrors.h
 * 
 *  Description:
 *	  This is the error declarations file for the NetMaster Library.
 *
 *	Note:
 *
 *	History:
 *	  31-Jan-2002	vmk	  Created by Vitaly Kruglikov
 *
 ****************************************************************/

#ifndef _NET_MASTER_LIB_ERRORS_H_
#define _NET_MASTER_LIB_ERRORS_H_

#include <HsErrorClasses.h>

//#include "OSServices.h"	  // for osSvcErrClassApp


// hsNetMasterErrorClass is defined in HsErrorClasses.h
#define netMasterErrClass	  (hsNetMasterErrorClass) // 0x7100


// WARNING:
//	  DO NOT MOVE OR REASSIGN ERROR CODE VALUES TO EXISTING CONSTANTS BECAUSE
//	  THEY ARE HARD-CODED IN OTHER MODULES, SUCH AS NETLIB PATCHES, SYSTEM
//	  PATCHES, AND ERROR STRING LISTS IN THE SYSTEM RESOURCE DATABASE.

#define netMasterErrMemory			(netMasterErrClass + 0x01)  // runtime memory error
#define netMasterErrStorage			(netMasterErrClass + 0x02)  // storage memory error
#define netMasterErrBadArg			(netMasterErrClass + 0x03)  // invalid argument, unknown ID
#define netMasterErrNetNotInstalled	(netMasterErrClass + 0x04)  // NetLib has not been installed
#define netMasterErrDispatchTable	(netMasterErrClass + 0x05)  // Unrecognized dipatch table format
#define netMasterErrNoNetPrefLib	(netMasterErrClass + 0x06)  // NetPref library not found
#define netMasterErrConfigNotFound	(netMasterErrClass + 0x07)  // NetPref config not found
#define netMasterErrConfigFailed	(netMasterErrClass + 0x08)  // Attempt to configure network settings
																//  failed
#define netMasterErrNo1xService		(netMasterErrClass + 0x09)	// 1xRTT is not available
#define netMasterErrMIPFailed		(netMasterErrClass + 0x0A)	// Generic Mobile-IP failure
#define netMasterErrBadPort			(netMasterErrClass + 0x0B)	// Invalid port number -- we
																//  remap this from serErrBadPort.

// Sprint's Mobile-IP (MIP) error codes
#define netMasterErrMIPNoSimBindings			(netMasterErrClass + 0x0C)	// 
#define netMasterErrMIPReasonUnspecified64		(netMasterErrClass + 0x0D)	// 
#define netMasterErrMIPAdminProhibited65		(netMasterErrClass + 0x0E)	// 
#define netMasterErrMIPInsufficientResources66	(netMasterErrClass + 0x0F)	// 
#define netMasterErrMIPMobileNodeAuth67			(netMasterErrClass + 0x10)	// 
#define netMasterErrMIPHomeAgentAuth68			(netMasterErrClass + 0x11)	// 
#define netMasterErrMIPReqLifetimeTooLong69		(netMasterErrClass + 0x12)	// 
#define netMasterErrMIPPoorlyFormedReq70		(netMasterErrClass + 0x13)	// 
#define netMasterErrMIPPoorlyFormedReply71		(netMasterErrClass + 0x14)	// 
#define netMasterErrMIPReqedEncapUnavail72		(netMasterErrClass + 0x15)	// 
#define netMasterErrMIPReservedNUnavail73		(netMasterErrClass + 0x16)	// 
#define netMasterErrMIPCantRevTun74				(netMasterErrClass + 0x17)	// 
#define netMasterErrMIPMustRevTun75				(netMasterErrClass + 0x18)	// 
#define netMasterErrMIPBadTtl76					(netMasterErrClass + 0x19)	// 
#define netMasterErrMIPInvalidCareOfAddr77		(netMasterErrClass + 0x1A)	// 
#define netMasterErrMIPRegistration_Timeout78	(netMasterErrClass + 0x1B)	// 
#define netMasterErrMIPDelivStyleNotSupported79	(netMasterErrClass + 0x1C)	// 
#define netMasterErrMIPHome_Network_Unreachable80 (netMasterErrClass + 0x1D)	// 
#define netMasterErrMIPHa_Host_Unreachable81	(netMasterErrClass + 0x1E)	// 
#define netMasterErrMIPHa_Port_Unreachable82	(netMasterErrClass + 0x1F)	// 
#define netMasterErrMIPHa_Unreachable88			(netMasterErrClass + 0x20)	// 
#define netMasterErrMIPNonZeroHaRequested96		(netMasterErrClass + 0x21)	// 
#define netMasterErrMIPMissingNai97				(netMasterErrClass + 0x22)	// 
#define netMasterErrMIPForeign_Agent98			(netMasterErrClass + 0x23)	// 
#define netMasterErrMIPMissingHa99				(netMasterErrClass + 0x24)	// 
#define netMasterErrMIPErrorFa1_100				(netMasterErrClass + 0x25)	// 
#define netMasterErrMIPErrorFa2_101				(netMasterErrClass + 0x26)	// 
#define netMasterErrMIPUnknown_Challenge104		(netMasterErrClass + 0x27)	// 
#define netMasterErrMIPMissing_Challenge105		(netMasterErrClass + 0x28)	// 
#define netMasterErrMIPStale_Challenge106		(netMasterErrClass + 0x29)	// 
#define netMasterErrMIPReasonUnspecified128		(netMasterErrClass + 0x2A)	// 
#define netMasterErrMIPAdminProhibited129		(netMasterErrClass + 0x2B)	// 
#define netMasterErrMIPInsufficientResources130	(netMasterErrClass + 0x2C)	// 
#define netMasterErrMIPMobileNodeAuth131		(netMasterErrClass + 0x2D)	// 
#define netMasterErrMIPForeignAgentAuth132		(netMasterErrClass + 0x2E)	// 
#define netMasterErrMIPRegIdMismatch133			(netMasterErrClass + 0x2F)	// 
#define netMasterErrMIPPoorlyFormedReq134		(netMasterErrClass + 0x30)	// 
#define netMasterErrMIPTooManySimMobBindings135	(netMasterErrClass + 0x31)	// 
#define netMasterErrMIPUnknownHaAddr136			(netMasterErrClass + 0x32)	// 
#define netMasterErrMIPCantRevTun137			(netMasterErrClass + 0x33)	// 
#define netMasterErrMIPMustRevTun138			(netMasterErrClass + 0x34)	// 
#define netMasterErrMIPReqEncapNotAvail139		(netMasterErrClass + 0x35)	// 
#define netMasterErrMIPErrorHa1_140				(netMasterErrClass + 0x36)	// 
#define netMasterErrMIPErrorHa2_141				(netMasterErrClass + 0x37)	// 
//801 Username suspension due to repeated incorrect passwords being sent
#define netMasterErrMIPUsernameSuspension801	(netMasterErrClass + 0x38)	// 
#define netMasterErrMIPReserved2				(netMasterErrClass + 0x39)	// 
#define netMasterErrMIPReserved3				(netMasterErrClass + 0x3A)	// 
#define netMasterErrMIPReserved4				(netMasterErrClass + 0x3B)	// 

#define netMasterErrAutoLoginNotPossible		(netMasterErrClass + 0x3C)	// auto-login did not take place
#define netMasterErrCarrierNetworkBusy			(netMasterErrClass + 0x3D)	// "network busy" event was received
																			//  from the carrier's network.



#endif // _NET_MASTER_LIB_ERRORS_H_
