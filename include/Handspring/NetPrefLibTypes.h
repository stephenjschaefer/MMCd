/***************************************************************
 *
 *  Project:
 *	  NetPref Library
 *
 * Copyright info:
 *
 *	  Copyright (c) Handspring 2001 -- All Rights Reserved
 *
 *
 *  FileName:
 *	  NetPrefLibTypes.h
 * 
 *  Description:
 *	  This file contains public data types exported by the NetPref Library
 *
 *	Note:
 *
 *	History:
 *	  12-Dec-2001	vmk	  Created by Vitaly Kruglikov
 *
 ****************************************************************/

#ifndef _NET_PREF_LIB_TYPES_H_
#define _NET_PREF_LIB_TYPES_H_




// Current library API version number
//  Returned by NetPrefLibVersionGet()
#define netPrefLibVersionMajor	  0
#define	netPrefLibVersionMinor	  1
#define netPrefLibVersionBugFix	  0


// Record field identifiers
typedef UInt16 NetPrefRecFieldEnum;

// Define the required macros and then include NetPrefLibFieldInfoTable.h in
//  order to define the record field indentifier values (see
//  NetPrefLibFieldInfoTable.h for usage notes and currently defined fieldIDs)
#define NetPrefRecFieldInfoPreamble()

#define NetPrefRecFieldInfoFIRST(fieldID, idValue)		fieldID = idValue,

#define NetPrefRecFieldInfoGrpStart(fieldID, idValue)	fieldID = idValue,

#define NetPrefRecFieldInfoGrpEnd(fieldID)				fieldID,

#define NetPrefRecFieldInfoExtended(fieldID, idValue, type, maxSize, flags, tagText, tagSize) \
														fieldID = idValue,

#define NetPrefRecFieldInfoLegacy(fieldID, idValue, type, maxSize, flags, tagText, tagSize) \
														fieldID = idValue,

#define NetPrefRecFieldInfoLAST(fieldID)				fieldID,

#define NetPrefRecFieldInfoPostamble()

enum
  {
	// See NetPrefLibFieldInfoTable.h for usage notes
	#include "NetPrefLibFieldInfoTable.h"
  };


// Field type identifiers
typedef UInt16	NetPrefFieldTypeEnum;
enum
  {
	netPrefFieldTypeStr		= 1,  // ascii string -- size must include zero-
								  //  terminator
	netPrefFieldTypeUInt8	= 2,
	netPrefFieldTypeUInt16	= 3,
	netPrefFieldTypeUInt32	= 4,
	netPrefFieldTypeBin		= 5,  // binary
	netPrefFieldTypeZStrList= 6,  // string list: list of zero-terminated strings
								  //  terminated by empty string. Size must
								  //  include zero-terminator

  };

// Service Medium identifiers (used with netPrefRecFieldMedium)
typedef UInt16 NetPrefSvcMediumEnum;
enum
  {
	netPrefSvcMediumUnknown = 0,	// reserve 0

	netPrefSvcMediumDialUp	= 1,	// basic dial-up -
									//  communication driver selection
									//  determines service attributes [DEFAULT]
	
	netPrefSvcMediumDirect	= 2,	// basic, non-modem type of connection

	netPrefSvcMedium1xRTT	= 3,	// CDMA 3G PDP service
	netPrefSvcMediumGPRS	= 4,	// GSM 2.5G PDP service
	netPrefSvcMediumQNC		= 5,	// CDMA Quick Net Connect
	netPrefSvcMediumGISDN	= 6,	// GSM ISDN over CSD service
	netPrefSvcMediumGCSD	= 7,	// GSM circuit switched data service (AKA
									//  wireless dial-up)
	netPrefSvcMediumCCSD	= 8,	// CDMA circuit switched data service (AKA
									//  wireless dial-up)


	netPrefSvcMediumLAST			// ALWAYS KEEP THIS ENTRY AT THE END!!!
  };


// Service Binding identifiers (use with netPrefRecFieldBinding);
typedef UInt16 NetPrefSvcBindingEnum;
enum
  {
	netPrefSvcBindingUnknown  = 0,	// reserve 0

	netPrefSvcBindingIOTA	  = 1,	// Provisioned via IOTA

	netPrefSvcBindingCCSM	  = 2,	// Default Wireless profile based on the
									//  CCSM tables
	netPrefSvcBindingCustom	  = 3,	// Created by user or "user agent" on
									//  behalf of the user [DEFAULT]


	netPrefSvcBindingLAST			// ALWAYS KEEP THIS ENTRY AT THE END!!!
  };


// 1xRTT Tunneling identifiers (use with netPrefRecField1xRTTTunneling)
typedef UInt16 NetPref1xRTTTunnelingEnum;
enum
  {
	netPref1xRTTTunnelingUnknown = 0,	// reserve 0

	netPref1xRTTTunnelingReverse = 1,	// reverse tunneling
	netPref1xRTTTunnelingForward = 2,	// forward tunneling


	netPref1xRTTTunnelingLAST			// ALWAYS KEEP THIS ENTRY AT THE END!!!
  };

// Default 1xRTT Tunneling value
#define netPref1xRTTTunnelingDefault  netPref1xRTTTunnelingReverse


// Service Target identifiers use with NetPrefDefaultTargetGet
//  and NetPrefDefaultTargetSet
typedef UInt16 NetPrefSvcTargetEnum;
enum
  {
	netPrefSvcTargetUnknown	  = 0,	// reserve 0

	netPrefSvcTargetInternet  = 1,		// provides access to the Internet
	netPrefSvcTargetWAP		  = 2,		// provides access to WAP


	netPrefSvcTargetLAST			// ALWAYS KEEP THIS ENTRY AT THE END!!!
  };

// Record field view attributes bitwise-or'ed together
typedef UInt32	NetPrefRecFieldViewType;
#define netPrefRecFieldViewRead		0x00000001UL  // read allowed by application
#define netPrefRecFieldViewWrite	0x00000002UL  // write allowed by application
#define netPrefRecFieldViewVisible	0x00000004UL  // field value is user-visible
#define netPrefRecFieldViewEditable	0x00000008UL  // field value is user-editable
#define netPrefRecFieldViewExternal	0x00000010UL  // field value is stored
												  //  exernally -- such as in the
												  //  CCSM table, radio module's
												  //  flash memory, etc., i.e.
												  //  not in the NetPref database
#define netPrefRecFieldViewCache	0x00000020UL  //  the external field value
												  //  is cached locally
#define netPrefRecFieldViewPrivate	0x00000040UL  // PRESENTLY NOT RESPECTED;
												  //  If set, do not display the
												  //  actual value to the user

// WARNING: THIS CONSTANT IS FOR INTERNAL USE ONLY -- IT WILL CHANGE
//  IN FUTURE VERSIONS!!! (used for error-checking only).
#define netPrefRecFieldViewCurrentlyValidFlags		\
					(	netPrefRecFieldViewRead		\
					  |	netPrefRecFieldViewWrite	\
					  |	netPrefRecFieldViewVisible	\
					  | netPrefRecFieldViewEditable	\
					  | netPrefRecFieldViewExternal	\
					  | netPrefRecFieldViewCache	\
					  | netPrefRecFieldViewPrivate)

// Special value to pass in the flagsToClear parameter to
//  NetPrefRecFieldViewSet in order to clear all field view flags
#define netPrefRecFieldViewClearAll	0xFFFFFFFFUL



// Record field attributes (retrieved via NetPrefRecFieldAttrsGet)
typedef UInt32	NetPrefRecFieldAttrType;
#define netPrefRecFieldAttrInSet	0x10000000UL  // R/O: set if the field's
								//  data or flags are part of the record's field
								//  set.

#define netPrefRecFieldAttrDirty	0x20000000UL  // R/O: set if the field's
								//  data or flags have been modified in the current
								//  instance of the record object



// Special values used with netPrefRecFieldCloseWaitTime
typedef UInt32 NetPrefNetTimeoutEnum;
enum
  {
	netPrefNetTimeoutOnPowerOff		= 0xffffffffUL,
	netPrefNetTimeoutOnWirelessOff	= 0xfffffffeUL,
	netPrefNetTimeoutCarrierDefault	= 0xfffffffdUL,
  };



// Record protection settings -- used with netPrefRecFieldRecProtection
typedef UInt32 NetPrefRecProtectionType;
enum
  {
	// Record deletion options
	netPrefRecProtectionDeletionMask				  = 0x00000007UL,
	netPrefRecProtectionDeletionAfterWarning		  = 0x0UL,
	netPrefRecProtectionDeletionNotAllowed			  = 0x1UL,


	// Duplication options
	netPrefRecProtectionDuplicationMask				  = (0x7UL << 3),
	netPrefRecProtectionDuplicationAllowed			  = (0x0UL << 3),
	netPrefRecProtectionDuplicationNotAllowed		  = (0x1UL << 3),


	// Editing options
	netPrefRecProtectionEditingMask					  = (0x7UL << 6),
	netPrefRecProtectionEditingAllowed				  = (0x0UL << 6),
	netPrefRecProtectionEditingNotAllowed			  = (0x1UL << 6),
	netPrefRecProtectionEditingAfterWarning			  = (0x2UL << 6),
  };



// Record Service gateway type -- used with netPrefRecFieldSvcGatewayType;
//  1 or more values can be bitwise or'ed together to mark
//  a record as supporting those gateway types; absense of
//  any service type falgs implies Internet.
typedef UInt32 NetPrefSvcGatewayType;
enum
  {
	netPrefSvcGatewayUnknown		= 0UL,			// reserved
	netPrefSvcGatewayInternet		= 0x00000001UL, // Generic Internet "gateway"
	netPrefSvcGatewayWAP			= 0x00000002UL, // Generic WAP gateway
	netPrefSvcGatewayPrivate		= 0x00000004UL, // Application-specific "gateway"
  };


// NetPrefGPRSQOSType -- GPRS Quality Of Service (QOS) info
typedef struct
  {
	// IMPORTANT: maintain this structure at fixed size; remove
	//  space for new fields from the reserved fields, making sure
	//  to adjust the reserved field size so the overall structure
	//  size is preserved exactly.  Use reserved space wisely :-)

	// NOTE: "ccsmCS_QOS..." in the following comments refers to Hanspring-
	//  internal data structures.  Developer support will need to provide
	//  clarification for the use of these fields.

	Int8	  qosReqPrecedenceClass;  // see ccsmCS_QOSReqPrecClassField
	Int8	  qosReqDelayClass;		  // see ccsmCS_QOSReqDelayClassField
	Int8	  qosReqReliabilityClass; // see ccsmCS_QOSReqRelClassField
	Int8	  qosReqPeakThruClass;	  // see ccsmCS_QOSReqPeakThruClassField
	Int8	  qosReqMeanThruClass;	  // see ccsmCS_QOSReqMeanThruClassField
	Int8	  qosMinPrecedenceClass;  // see ccsmCS_QOSMinPrecClassField
	Int8	  qosMinDelayClass;		  // see ccsmCS_QOSMinDelayClassField
	Int8	  qosMinReliabilityClass; // see ccsmCS_QOSMinRelClassField
	Int8	  qosMinPeakThruClass;	  // see ccsmCS_QOSMinPeakThruClassField
	Int8	  qosMinMeanThruClass;	  // see ccsmCS_QOSMinMeanThruClassField
	Int8	  pdpDataCompression;	  // see ccsmCS_PDPDataComprField
	Int8	  pdpHeaderCompression;	  // see ccsmCS_PDPHeaderComprField

	UInt32	  reserved[4];			  // Reserved by Handspring -- init to zero
  }
NetPrefGPRSQOSType;


// Options for NetPrefRecFieldSetDefineStd and NetPrefRecFieldSetDefine
typedef UInt32	  NetPrefFieldSetDefineOptionsType;
enum
  {
	// Pass this value for "no options"
	netPrefFieldSetDefineOptionNone = 0,

	// If set, NetPrefRecFieldSetDefineStd and NetPrefRecFieldSetDefine
	//  will temporarily keep around the data of out-of-set fields; however,
	//  it will not save such data when saving the field; this feature
	//  is a convenience for the Network panel that allows the user to
	//  experiment with different connection mediuma (dial-up, direct, 3G, etc.)
	//  without losing the data that is in one service medium's field set,
	//  but not in another.
	netPrefFieldSetDefineOptionKeepData = 0x00000001UL,
  };

// Opaque structures
struct NetPrefContextTypeTag;
struct NetPrefRecordTypeTag;


							
#endif // _NET_PREF_LIB_TYPES_H_
