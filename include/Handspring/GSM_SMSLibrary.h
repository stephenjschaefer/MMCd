/*******************************************************************************

FILE SMSLibrary.h
 Copyright Option International 1999-2000 and Handspring Inc. 1999-2001
 All rights reserved.

SYNOPSIS
 Public interface for the SMS library.


*******************************************************************************/

#ifndef __SMSLIBRARY_H__
#define __SMSLIBRARY_H__

#define SMSLibDBTypeID	'Msgs'	// Type for the message database

#define SMSLibDBName	"SMS Messages"		// passed to DmFindDatabase()

typedef MemHandle SMSMessageHandle;

// Enumeration types
enum SMSMessageType {
	kMTIncoming, kMTOutgoing
};
enum SMSMessageStatus {
	kNone,
	kReceiving, kReceived,
	kPending, kSending, kSent
};
enum SMSMessageFlags {
	kGreekSymbols = 1L << 0,
	kMissingPart =	1L << 1,
	kAutoDelete =	1L << 2,
	kNotification = 1L << 3,
	kDontEncode =	1L << 4,
	kSubstitution = 1L << 5,
	kFailed =		1L << 6,
	kStatusReport = 1L << 7,
	kFreeReply =	1L << 8,
	kInternetEMail =1L << 9,
	kTextSegments =	1L << 10,
  kSMSErrorType1 = 1L << 11,
  kSMSErrorType2 = 1L << 12,
  kSMSErrorType3 = 1L << 13,
		// application-level flags
	kRead = 1L << 16
};
#ifndef __cplusplus
typedef enum SMSMessageType SMSMessageType;
typedef enum SMSMessageStatus SMSMessageStatus;
typedef enum SMSMessageFlags SMSMessageFlags;
#endif

// Structured types
struct SMSSendOptions {
	Boolean freeReply;
	Boolean statusReport;
	unsigned char validity;
};
#ifndef _cplusplus
typedef struct SMSSendOptions SMSSendOptions;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// SMS library routines
extern DmOpenRef GSMLibGetDBRef(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapGetDBRef);
extern Err GSMLibReleaseDBRef(UInt16 refNum, DmOpenRef db)
				GSM_LIB_TRAP(GSMLibTrapReleaseDBRef);

extern GSMDatabaseID GSMLibNewMessage(UInt16 refNum, SMSMessageType type)
				GSM_LIB_TRAP(GSMLibTrapNewMessage);
extern Err GSMLibDeleteMessage(UInt16 refNum, GSMDatabaseID msgID, Boolean archive)
				GSM_LIB_TRAP(GSMLibTrapDeleteMessage);
extern Err GSMLibSendMessage(UInt16 refNum, GSMDatabaseID msgID, Boolean progress)
				GSM_LIB_TRAP(GSMLibTrapSendMessage);

extern Err GSMLibSetText(UInt16 refNum, GSMDatabaseID msgID, const char* data, int size)
				GSM_LIB_TRAP(GSMLibTrapSetText);
extern Err GSMLibSetDate(UInt16 refNum, GSMDatabaseID msgID, UInt32 date)
				GSM_LIB_TRAP(GSMLibTrapSetDate);
extern Err GSMLibSetOptions(UInt16 refNum, GSMDatabaseID msgID, const SMSSendOptions* options)
				GSM_LIB_TRAP(GSMLibTrapSetOptions);
extern Err GSMLibSetAddresses(UInt16 refNum, GSMDatabaseID msgID, const GSMAddressList list)
				GSM_LIB_TRAP(GSMLibTrapSetAddresses);
extern Err GSMLibSetStatus(UInt16 refNum, GSMDatabaseID msgID, SMSMessageStatus status)
				GSM_LIB_TRAP(GSMLibTrapSetStatus);
extern Err GSMLibSetFlags(UInt16 refNum, GSMDatabaseID msgID, UInt32 flags)
				GSM_LIB_TRAP(GSMLibTrapSetFlags);
extern Err GSMLibSetOwner(UInt16 refNum, GSMDatabaseID msgID, UInt32 owner)
				GSM_LIB_TRAP(GSMLibTrapSetOwner);

extern Err GSMLibGetText(UInt16 refNum, GSMDatabaseID msgID, MemHandle* data)
				GSM_LIB_TRAP(GSMLibTrapGetText);
extern Err GSMLibGetDate(UInt16 refNum, GSMDatabaseID msgID, UInt32* date)
				GSM_LIB_TRAP(GSMLibTrapGetDate);
extern Err GSMLibGetOptions(UInt16 refNum, GSMDatabaseID msgID, SMSSendOptions* options)
				GSM_LIB_TRAP(GSMLibTrapGetOptions);
extern Err GSMLibGetAddresses(UInt16 refNum, GSMDatabaseID msgID, GSMAddressList* list)
				GSM_LIB_TRAP(GSMLibTrapGetAddresses);
extern Err GSMLibGetStatus(UInt16 refNum, GSMDatabaseID msgID, SMSMessageStatus* status)
				GSM_LIB_TRAP(GSMLibTrapGetStatus);
extern Err GSMLibGetFlags(UInt16 refNum, GSMDatabaseID msgID, UInt32* flags)
				GSM_LIB_TRAP(GSMLibTrapGetFlags);
extern Err GSMLibGetOwner(UInt16 refNum, GSMDatabaseID msgID, UInt32* owner)
				GSM_LIB_TRAP(GSMLibTrapGetOwner);
extern Err GSMLibGetType(UInt16 refNum, GSMDatabaseID msgID, SMSMessageType* type)
				GSM_LIB_TRAP(GSMLibTrapGetType);

extern Boolean GSMLibIsLegalCharacter(UInt16 refNum, char c)
				GSM_LIB_TRAP(GSMLibTrapIsLegalCharacter);
extern char GSMLibMapCharacter(UInt16 refNum, char c)
				GSM_LIB_TRAP(GSMLibTrapMapCharacter);
extern Err GSMLibSetServiceCentreAddress(UInt16 refNum, const GSMAddressHandle address)
				GSM_LIB_TRAP(GSMLibTrapSetServiceCentreAddress);
extern Err GSMLibGetServiceCentreAddress(UInt16 refNum, GSMAddressHandle* address)
				GSM_LIB_TRAP(GSMLibTrapGetServiceCentreAddress);

extern int GSMLibLength(UInt16 refNum, const char* text, Boolean inMessages, Boolean substitution)
				GSM_LIB_TRAP(GSMLibTrapLength);
extern const char* GSMLibGetSubstitution(UInt16 refNum, char c)
				GSM_LIB_TRAP(GSMLibTrapGetSubstitution);

extern GSMAddressList GSMLibNewAddressList(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapNewAddressList);
extern Err GSMLibDisposeAddressList(UInt16 refNum, GSMAddressList list)
				GSM_LIB_TRAP(GSMLibTrapDisposeAddressList);
extern Err GSMLibAddAddress(UInt16 refNum, GSMAddressList list, const GSMAddressHandle address)
				GSM_LIB_TRAP(GSMLibTrapAddAddress);
extern Err GSMLibGetNth(UInt16 refNum, const GSMAddressList list, int index, GSMAddressHandle* address)
				GSM_LIB_TRAP(GSMLibTrapGetNth);
extern Err GSMLibSetNth(UInt16 refNum, GSMAddressList list, int index, const GSMAddressHandle address)
				GSM_LIB_TRAP(GSMLibTrapSetNth);
extern Err GSMLibCount(UInt16 refNum, GSMAddressList list, UInt16* count)
				GSM_LIB_TRAP(GSMLibTrapCount);

#ifdef __cplusplus 
}
#endif


#endif
