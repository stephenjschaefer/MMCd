/***************************************************************
 *
 *  Project:
 *	  NetPref library
 *
 * Copyright info:
 *
 *	  Copyright (c) Handspring 2002 -- All Rights Reserved
 *
 *
 *  FileName:
 *	  NetPrefUtils.h
 * 
 *  Description:
 *
 *	  NetPref library's public utilities -- this file may be
 *	  linked with your application or library to provide access
 *	  to a set of convenient helper functions.
 *
 *	Note:
 *
 *	History:
 *	  7-Feb-2002	vmk	  Created by Vitaly Kruglikov 
 *
 ****************************************************************/

#ifndef _NET_PREF_LIB_UTILS_H_
#define _NET_PREF_LIB_UTILS_H_

#include <FarCall.h>

#include "NetPrefLibrary.h"



#define netPrefMaxPhoneStringSize 80	// max size of a phone number


#define netPrefUtilNetPrefLinkSignature	0xFEEDDADAUL

typedef struct
  {
	UInt16	refNum;
	struct NetPrefContextTypeTag* cxtP;

	UInt32	linkSignature;	// for debugging -- set by NetPrefUtilNetPrefLibLink
							//  and checked by NetPrefUtilNetPrefLibUnlink
  }
NetPrefUtilNetPrefLibType;



// ------------------------------------------------------------------
// Platform IDs
// ------------------------------------------------------------------

typedef enum
  {
	netPrefUtilPlatformUnknown	  = 0,

	netPrefUtilPlatformGSM		  = 1,
	netPrefUtilPlatformCDMA	  = 2,

	netPrefUtilPlatformLAST
  }
NetPrefUtilPlatformEnum;



// ------------------------------------------------------------------
// Phone Info structure -- for convenience
// ------------------------------------------------------------------

typedef struct
  {
	struct
	  {
		UInt8          dialPrefixEnabled;	// dial prefix string enabled
		UInt8          callWaitingEnabled;	// call waiting string enabled
		UInt8          callingCardEnabled;	// calling card string enabled
		UInt8          pulseDial;			// if true, pulse dial, otherwise touch-tone
	  } flags;
	char*			phoneP;		// phone number
	char*			prefixP;	// dial prefix
	char*			callWaitingP;	// call waiting string
	char*			callingCardP;	// calling card number string

	// The following fields are used to detect if we need
	// to update the NetLib settings due to a change in this
	// structure
	Boolean         dirty;		// True if prefs have changed
  }
NetPrefUtilPhoneInfoType;




#ifdef __cplusplus				// Functions are made to be accessible via "C"
extern          "C"
{
#endif


extern Boolean
NetPrefUtilNetPrefLibLink (NetPrefUtilNetPrefLibType* netPrefLibP);

extern Boolean
FarNetPrefUtilNetPrefLibLink (NetPrefUtilNetPrefLibType* netPrefLibP)
					  FAR_CALL(NetPrefUtilNetPrefLibLink);

extern void
NetPrefUtilNetPrefLibUnlink (NetPrefUtilNetPrefLibType* netPrefLibP);

extern void
FarNetPrefUtilNetPrefLibUnlink (NetPrefUtilNetPrefLibType* netPrefLibP)
					  FAR_CALL(NetPrefUtilNetPrefLibUnlink);


// Given an open NetPref library refNum and context pointer, initialize
//  a NetPref library Instance structure that may be used with other NetPrefUtil
//  API's
extern void
NetPrefUtilNetPrefInstanceConstruct (UInt16 netPrefLibRefNum,
									 struct NetPrefContextTypeTag* netPrefLibCxtP,
									 NetPrefUtilNetPrefLibType* netPrefLibP);
extern void
FarNetPrefUtilNetPrefInstanceConstruct (UInt16 netPrefLibRefNum,
									 struct NetPrefContextTypeTag* netPrefLibCxtP,
									 NetPrefUtilNetPrefLibType* netPrefLibP)
					  FAR_CALL(NetPrefUtilNetPrefInstanceConstruct);


extern Boolean
NetPrefUtilNetPrefLongGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						  struct NetPrefRecordTypeTag* recP,
						  NetPrefRecFieldEnum recFieldID,
						  UInt32* resultP,
						  NetPrefRecFieldViewType* viewFlagsP);
extern Boolean
FarNetPrefUtilNetPrefLongGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						  struct NetPrefRecordTypeTag* recP,
						  NetPrefRecFieldEnum recFieldID,
						  UInt32* resultP,
						  NetPrefRecFieldViewType* viewFlagsP)
					  FAR_CALL(NetPrefUtilNetPrefLongGet);

extern Boolean
NetPrefUtilNetPrefShortGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						  struct NetPrefRecordTypeTag* recP,
						  NetPrefRecFieldEnum recFieldID,
						  UInt16* resultP,
						  NetPrefRecFieldViewType* viewFlagsP);

extern Boolean
FarNetPrefUtilNetPrefShortGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						  struct NetPrefRecordTypeTag* recP,
						  NetPrefRecFieldEnum recFieldID,
						  UInt16* resultP,
						  NetPrefRecFieldViewType* viewFlagsP)
					  FAR_CALL(NetPrefUtilNetPrefShortGet);


extern Boolean
NetPrefUtilNetPrefByteGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						  struct NetPrefRecordTypeTag* recP,
						  NetPrefRecFieldEnum recFieldID,
						  UInt8* resultP,
						  NetPrefRecFieldViewType* viewFlagsP);

extern Boolean
FarNetPrefUtilNetPrefByteGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						  struct NetPrefRecordTypeTag* recP,
						  NetPrefRecFieldEnum recFieldID,
						  UInt8* resultP,
						  NetPrefRecFieldViewType* viewFlagsP)
					  FAR_CALL(NetPrefUtilNetPrefByteGet);


extern NetPrefRecFieldViewType
NetPrefUtilFieldViewGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						 struct NetPrefRecordTypeTag* recP,
						 NetPrefRecFieldEnum recFieldID);

extern NetPrefRecFieldViewType
FarNetPrefUtilFieldViewGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						 struct NetPrefRecordTypeTag* recP,
						 NetPrefRecFieldEnum recFieldID)
					  FAR_CALL(NetPrefUtilFieldViewGet);


// Set a 32-bit field data value as flags. The clear operation is
//  performed before the set operation.  To skip either operation,
//  set the corresponding parameter to 0.
extern Err
NetPrefUtilLongDataSetAsFlags (NetPrefUtilNetPrefLibType* netPrefLibP,
							   struct NetPrefRecordTypeTag* recP,
							   NetPrefRecFieldEnum recFieldID,
							   UInt32 flagsToClear, UInt32 flagsToSet);

extern Err
FarNetPrefUtilLongDataSetAsFlags (NetPrefUtilNetPrefLibType* netPrefLibP,
							   struct NetPrefRecordTypeTag* recP,
							   NetPrefRecFieldEnum recFieldID,
							   UInt32 flagsToClear, UInt32 flagsToSet)
					  FAR_CALL(NetPrefUtilLongDataSetAsFlags);


extern Err
NetPrefUtilNetPrefRecBinHandleSet (NetPrefUtilNetPrefLibType* netPrefLibP,
								struct NetPrefRecordTypeTag* recP,
								NetPrefRecFieldEnum recFieldID,
								MemHandle dataH);

extern Err
FarNetPrefUtilNetPrefRecBinHandleSet (NetPrefUtilNetPrefLibType* netPrefLibP,
								struct NetPrefRecordTypeTag* recP,
								NetPrefRecFieldEnum recFieldID,
								MemHandle dataH)
					  FAR_CALL(NetPrefUtilNetPrefRecBinHandleSet);


extern Err
NetPrefUtilNetPrefRecStrHandleSet (NetPrefUtilNetPrefLibType* netPrefLibP,
								struct NetPrefRecordTypeTag* recP,
								NetPrefRecFieldEnum recFieldID,
								MemHandle dataH);

extern Err
FarNetPrefUtilNetPrefRecStrHandleSet (NetPrefUtilNetPrefLibType* netPrefLibP,
								struct NetPrefRecordTypeTag* recP,
								NetPrefRecFieldEnum recFieldID,
								MemHandle dataH)
					  FAR_CALL(NetPrefUtilNetPrefRecStrHandleSet);


extern Boolean
NetPrefUtilConnectionInfoGet (NetPrefUtilNetPrefLibType* netPrefLibP,
							 struct NetPrefRecordTypeTag* recP,
							 UInt32* portCreatorIDP, Boolean* isModemP);

extern Boolean
FarNetPrefUtilConnectionInfoGet (NetPrefUtilNetPrefLibType* netPrefLibP,
							 struct NetPrefRecordTypeTag* recP,
							 UInt32* portCreatorIDP, Boolean* isModemP)
					  FAR_CALL(NetPrefUtilConnectionInfoGet);


extern NetPrefSvcMediumEnum
NetPrefUtilRecMediumGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						struct NetPrefRecordTypeTag* recP,
						UInt32 wirelessDriverID);

extern NetPrefSvcMediumEnum
FarNetPrefUtilRecMediumGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						struct NetPrefRecordTypeTag* recP,
						UInt32 wirelessDriverID)
					  FAR_CALL(NetPrefUtilRecMediumGet);


// Save an IP address to the record object.  If the IP address is 0,
//  saves a value of 0 length to optimize memory access performance
extern Err
NetPrefUtilNetPrefIPAddrSet (NetPrefUtilNetPrefLibType* netPrefLibP,
							struct NetPrefRecordTypeTag* recP,
							NetPrefRecFieldEnum recFieldID, UInt32 ipAddr);

extern Err
FarNetPrefUtilNetPrefIPAddrSet (NetPrefUtilNetPrefLibType* netPrefLibP,
							struct NetPrefRecordTypeTag* recP,
							NetPrefRecFieldEnum recFieldID, UInt32 ipAddr)
					  FAR_CALL(NetPrefUtilNetPrefIPAddrSet);


// Return the length of a zero-terminated string list.  A
//  zero-terminated string list consists of zero-terminated
//  strings and ends with an empty string.  The size includes
//  all of the zero-terminators, including the list terminator.
extern UInt32
NetPrefUtilZStringListSize (char* zStrListP);

extern UInt32
FarNetPrefUtilZStringListSize (char* zStrListP)
					  FAR_CALL(NetPrefUtilZStringListSize);

#define NetPrefUtilLoginScriptLen(scriptP)	FarNetPrefUtilZStringListSize (scriptP)


//  This is a little utility function for setting network 
// options that are saved as strings; handles netIFSettingLoginScript
// as a special case.
extern Err
NetPrefUtilNetConfigStrSet (char* strP, UInt16 netSettingID,
							UInt16 netLibRefnum, UInt32 netInterface);

extern Err
FarNetPrefUtilNetConfigStrSet (char* strP, UInt16 netSettingID,
							UInt16 netLibRefnum, UInt32 netInterface)
					  FAR_CALL(NetPrefUtilNetConfigStrSet);


// Utility function for getting a string setting from a
//  record object and using it to set a NetLib option. 
//  Also handles netIFSettingLoginScript as a special case.
extern void
NetPrefUtilNetConfigStrSetFromRec (NetPrefUtilNetPrefLibType* netPrefLibP,
								   struct NetPrefRecordTypeTag* recP,
								   NetPrefRecFieldEnum recFieldID,
								   UInt16 netSettingID, UInt16 netLibRefnum,
								   UInt32 netInterface, Boolean force);

extern void
FarNetPrefUtilNetConfigStrSetFromRec (NetPrefUtilNetPrefLibType* netPrefLibP,
								   struct NetPrefRecordTypeTag* recP,
								   NetPrefRecFieldEnum recFieldID,
								   UInt16 netSettingID, UInt16 netLibRefnum,
								   UInt32 netInterface, Boolean force)
					  FAR_CALL(NetPrefUtilNetConfigStrSetFromRec);


extern void
NetPrefUtilPhoneStringCompose (Char* destP, UInt16 maxLen,
						   NetPrefUtilNetPrefLibType* netPrefLibP,
						   struct NetPrefRecordTypeTag* recP);

extern void
FarNetPrefUtilPhoneStringCompose (Char* destP, UInt16 maxLen,
						   NetPrefUtilNetPrefLibType* netPrefLibP,
						   struct NetPrefRecordTypeTag* recP)
					  FAR_CALL(NetPrefUtilPhoneStringCompose);


// Determine on which platform we are running -- CDMA or GSM
extern NetPrefUtilPlatformEnum
NetPrefUtilPlatformGet (void);

extern NetPrefUtilPlatformEnum
FarNetPrefUtilPlatformGet (void)
					  FAR_CALL(NetPrefUtilPlatformGet);


// Get the default wireless driver ID for the given platform
extern UInt32
NetPrefUtilDefWirelessDriverIDGet (NetPrefUtilPlatformEnum platform);

extern UInt32
FarNetPrefUtilDefWirelessDriverIDGet (NetPrefUtilPlatformEnum platform)
					  FAR_CALL(NetPrefUtilDefWirelessDriverIDGet);

// Initialize the phone info structure from a record
//  object.  The pointers to strings will NOT be duplicates,
//  but will point directly to the corresponding fields
//  within the object.
extern void
NetPrefUtilPhoneInfoGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						 struct NetPrefRecordTypeTag* recP,
						 NetPrefUtilPhoneInfoType* phoneInfoP);

extern void
FarNetPrefUtilPhoneInfoGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						 struct NetPrefRecordTypeTag* recP,
						 NetPrefUtilPhoneInfoType* phoneInfoP)
					  FAR_CALL(NetPrefUtilPhoneInfoGet);


#ifdef __cplusplus
}
#endif


#endif // _NET_PREF_LIB_UTILS_H_