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
 *	  NetMasterLibTypes.h
 * 
 *  Description:
 *	  This file contains public data types exported by the NetMaster Library
 *
 *	Note:
 *
 *	History:
 *	  31-Jan-2002	vmk	  Created by Vitaly Kruglikov
 *
 ****************************************************************/

#ifndef _NET_MASTER_LIB_TYPES_H_
#define _NET_MASTER_LIB_TYPES_H_




// Current library API version number
//  Returned by NetMasterLibVersionGet()
#define netMasterLibVersionMajor	  0
#define	netMasterLibVersionMinor	  2
#define netMasterLibVersionBugFix	  0

// Change details:
//
// 0.1.0 -- original
// 0.2.0: implemented original VPN override notification




// Opaque structures
struct NetMasterContextTypeTag;




// TCP/IP Network Auto-login definitions used by NetMasterAutoLoginSettingGet()
//  and NetMasterAutoLoginSettingSet()
typedef UInt32	NetMasterAutoLoginType;
enum
  {
	// For CDMA 1xRTT: Automatically log in to the TCP/IP network when the
	//  wireless mode is turned on and network is acquired;
	// For GSM GPRS: Attempt to attach when the radio is powered on (ideally
	//  a combined attach)
	netMasterAutoLoginWhenWirelessOn	= 0x00000001UL,


	netMasterAutoLoginClearAll			= 0xFFFFFFFFUL
  };


// NetLib interface shutdown options for NetMasterNetInterfacesShutDown
typedef UInt32	NetMasterNetIFShutDownOptionsType;
enum
  {
	// If the Progress dialog is up, dismiss it even if it is
	//  in the error display state
	netMasterNetIFShutDownOptDismissPrgDialog	= 0x00000001UL,

	// If set, and the session being connected at the moment has
	//  a fallback, this will allow the fallback to be processed (for example,
	//  when aborting GPRS logon, but allowing the fallback, if any, to
	//  take place)
	netMasterNetIFShutDownOptCurrentOnly		= 0x00000002UL
  };

// NetMaster control codes used by NetMasterControl.  For internal, system use
//  only!
typedef UInt32	NetMasterCtlEnum;
enum
  {
	netMasterCtlUnknown				  = 0,

	// netMasterCtlAutoLoginInitiate:
	//
	// Initiates auto-login to the IP network if the current
	//  NetMasterAutoLoginType setting and the type of the current network
	//  profile warrant it.  Blocking.  MUST be called from the UI task only --
	//  UI will be displayed during connection establishment.  Called
	//  by Phone Library (or its associate) after the wireless mode is
	//  turned on.
	//
	// MUST be called from the context of the UI task!
	//
	// ctlDataP = NULL.
	netMasterCtlAutoLoginInitiate	  = 1,


	// netMasterCtlAutoLoginTerminate:
	//
	// Cleans up the auto-login state, if auto-login was active.  Blocking.
	// Called by Phone Library when ... ?.  This pretty much just calls
	//  NetLibClose if the preceding call to netMasterCtlAutoLoginInitiate
	//  made a successful call to NetLibClose.
	//
	// MUST be called from the context of the UI task!
	//  
	// ctlDataP = NULL.
	netMasterCtlAutoLoginTerminate	  = 2

  };


// ------------------------------------------------------------------
// Client Notification Events: (netMasterClientEventNotificationTypeID)
//
// Parameter block and supporting data structures sent to the currently
// registered NetMaster library client(s) via Palm OS Notification Manager.
// ------------------------------------------------------------------

// The application that wishes to receive these notifications registers
// via Palm OS Notification Manager's SysNotifyRegister() for the event type
// netMasterClientEventNotificationTypeID to receive the events listed in
// NetMasterClientEventEnum. See Palm OS Notification Manager
// documentation for registration and notification handling details.
//
// (Handspring registered 'HICe' with Palm Source on 08-Oct-2002 at 8:23pm PST)
#define netMasterClientEventNotificationTypeID	  'HnMe'


// NetMaster library client event selector
typedef UInt16	  NetMasterClientEventEnum;
enum
  {
	netMasterClientEventUnknown		= 0,	// reserve 0

	// Queries the VPN vendor to install its own connection profile name,
	//  if any; NetMaster library broadcasts this event to all registered
	//  clients from the context of the UI task when a client application
	//  calls NetLibOpen() or NetLibOpenConfig().
	// Parameter block:
	//  NetMasterClientEventInfoType:NetMasterClientEventVPNConnInstallType
	netMasterClientEventVPNConnInstall,




	netMasterClientEventLAST
  };



// Data block for netMasterClientEventVPNConnInstall
typedef struct
  {
	// --------------------------------------------------------------
	// Inputs
	// --------------------------------------------------------------

	// Name of the connection that NetLib will use unless the VPN
	// client overrides it by setting vpnConnProfileOverrideName
	// to a non-empty connection name string.  Watch out for the case
	// where the user inadvertently selected the VPN connection name
	// itself as the connection name for the network profile. VPN vendors
	// that choose to override this connection name, will likely wish
	// to save this connection name so that their destination connection.
	Char					defaultConnProfileName[cncProfileNameSize];

	UInt32					inReserved1;  // set to zero by this lib
	UInt32					inReserved2;  // set to zero by this lib

	
	// --------------------------------------------------------------
	// Outputs -- modify only if you wish to override the default
	//  connection name!
	// --------------------------------------------------------------

	// The VPN vendor may override the default connection name that NetLib
	// will use by setting this field to a non-empty string -- this string
	// MUST be a valid connection name that presently exists in the connection
	// panel!
	//
	// If the VPN vendor presently does *not* wish to override the default
	// connection (based on user settings in the VPN vendor's configuration
	// UI), the VPN vendor should leave this field alone (don't modify
	// it in any way in this case to avoid conflicts with other VPN vendor
	// that may be currently installed on the same device).
	Char					vpnConnProfileOverrideName[cncProfileNameSize];
  }
NetMasterClientEventVPNConnInstallType;


// Increment this version # when adding fields to NetMasterClientEventInfoType,
//  except when just adding a new event data pointer to eventData
#define netMasterClientCurrentEventVersion		1

// NetMasterClientEventInfoType: This is passed as the 'detailsP' of the
// notification.
typedef struct
  {
	// --------------------------------------------------------------
	// Inputs
	// --------------------------------------------------------------

	// Version of this structure -- all future versions MUST define
	// backward compatible Event structures.
	// NetMaster Library sets this to netMasterClientCurrentEventVersion
	// at the time the library was built.
	UInt16						version;

	// Event ID
	NetMasterClientEventEnum	eventID;

	// Data corresponding to the event ID
	union
	  {
		// genericEventDataP is for the convenience of the internal
		// code only.
		void*									genericEventDataP;

		// For netMasterClientEventVPNConnInstall
		NetMasterClientEventVPNConnInstallType*	vpnConnInstallP;

	  }
	eventData;

	// Reserved values; Set to 0 by NetMaster library
	UInt32							inReserved1;
	UInt32							inReserved2;
	UInt32							inReserved3;
	UInt32							inReserved4;


	// --------------------------------------------------------------
	// Outputs - none for now
	// --------------------------------------------------------------

  }
NetMasterClientEventInfoType;




// ----------------------------------------------------------------
// Provisioning structures (initially used by the IOTA logic only)
// ----------------------------------------------------------------

// The special "NetLibOpenConfig" backdoor function that the provisioning
//  app MUST use to logon -- it MUST NOT call NetLibOpen or NetLibOpenConfig
//  directly because this hook function is usually called from NetMaster's
//  patch of NetLibOpen/Config.  netLibRefNum, openFlags, and netIFErrP
//  are same as for NetLibOpenConfig.
typedef Err
NetMasterNetLibOpenConfigBackdoorFnType (void* netMasterGP,
										 void* oldNetLibOpenConfigFnP,
										 UInt16 netLibRefNum,
										 UInt32 openFlags,
										 UInt16* netIFErrP);

// NetMaster library will pass this structure to the Provisioning Hook
//  app.  This is usually done from NetMaster's patch of the NetLib
//  function NetMasterConfig before servicing the request.  The
//  Provisioning Hook app is responsible for performing *quick* tests
//  first to determine whether provisioning is necessary (to avoid
//  slowing down the normal network logons), and perform provisioning
//  only if necessary.
typedef struct NetMasterProvisioningHookTypeTag
  {
	UInt16		structVersion;	// starts at 0

	// ----------------------------------------------------------------
	// Inputs -- filled in by NetMaster library
	// ----------------------------------------------------------------

	// The special "NetLibOpenConfig" backdoor function that the provisioning
	//  app MUST use to logon -- it MUST NOT call NetLibOpen or NetLibOpenConfig
	//  directly because this hook function is usually called from NetMaster's
	//  patch of NetLibOpen/Config.
	NetMasterNetLibOpenConfigBackdoorFnType*  openConfigFnP;
	void*		netMasterGP;			  // parameter for openConfigFnP
	void*		oldNetLibOpenConfigFnP;	  // parameter for openConfigFnP
	UInt16		netLibRefNum;			  // parameter for openConfigFnP

	// NetMaster library API refNum
	UInt16		netMasterRefNum;

	UInt32		reservedInput1;			  // reserved -- set to zero
	UInt32		reservedInput2;			  // reserved -- set to zero


	// ----------------------------------------------------------------
	// Outputs -- initialized to zero by NetMaster library before calling
	//  the provisioning hook; overwritten by the provisioning hook as
	//  necessary.
	// ----------------------------------------------------------------

	// Error code from the provisioning session.  If abortLogon is set to
	//  true, this field MUST be set to a non-zero error code.
	Err			provisionErrCode;

	// If set to true, instructs NetMaster to abort the current
	//  NetLibOpen/Config call and return the value in provisionErrCode
	//  as the return value of the current call to NetLibOpen/Config.
	// If this is set to true, provisionErrCode must contain a non-zero
	//  error code.  The expectation is that the Provisioning Hook is
	//  responsible for displaying the appropriate error message to
	//  the user in this case.
	Boolean		abortLogon;			

	UInt32		reservedOutput1;		  // reserved -- set to zero
	UInt32		reservedOutput2;		  // reserved -- set to zero
  }
NetMasterProvisioningHookType;




#endif // _NET_MASTER_LIB_TYPES_H_
