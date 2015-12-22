/*******************************************************************************

FILE GSMLibrary.h
 Copyright Option International 1999-2000 and Handspring Inc. 1999-2001
 All rights reserved.

SYNOPSIS
 Public interface for the GSM library.

*******************************************************************************/

#ifndef __GSMLIBRARY_H__
#define __GSMLIBRARY_H__

// If we're actually compiling the library code, then we need to
// eliminate the trap glue that would otherwise be generated from
// this header file in order to prevent compiler errors in CW Pro 2.
#if defined(BUILDING_GSM_LIBRARY) || defined(USING_STATIC_LIBRARY)
	#define GSM_LIB_TRAP(trapNum)
#else
	#define GSM_LIB_TRAP(trapNum) SYS_TRAP(trapNum)
#endif

// PalmPilot common definitions
#ifndef __PALMOS_H__
#include <PalmOS.h>
#endif

#include "GSMLibraryErrors.h"	// error codes
#include "GSMLibraryTraps.h"	// traps for the libraryÕs routines

// Type and creator of GSM Library database
#define GSMLibCreatorID	'GSM!'	// GSM Library database creator
#define GSMLibTypeID	'libr'	// Standard library database type

// Internal library name which can be passed to SysLibFind()
#define GSMLibName "GSMLibrary.lib"
#define GSMLibDBName	"GSM Library"	// name used for DmFindDatabase

// This is a prebuilt database.  So if this name changes, need to make
// sure that the following references to the Ring Tone database name
// are updated as well:
// 1. In the Viewer/Prebuilt directory, update the actual name
//	  of the pdb file
// 2. In the GSMLibrary Makefile, update ringTonePdbName
// 3. In GSMLibraryCommon.rcp, update the DATA 'dflt' 1 resource
// 4. In PhoneLib.h
#define GSMLibRingsDbName			"System Ring Tones"

// Creator of SystemRingTone database
#define GSMLibRingsDBCreatorID 'GSMr'  // registered with Palm

// Notification Types
#define phnNotifySubscriber 'CLIP'
#define phnNBSEvent         'Hnbs'
#define phnNotifyEquipMode	'Heqp'

typedef Int16 GSMConnectionID;
typedef UInt32 GSMDatabaseID;
typedef UInt32 GSMOperatorID;
typedef MemHandle GSMAddressHandle;
typedef MemHandle GSMAddressList;

#define GSMkUnknownID 0xFF000000
#define GSMkUnknownCID 0

// Phone slider switch
#define GSMkSliderPositions 2
#define GSMSliderLow			0
#define GSMSliderHigh			1

#define GSMMaximumID 7
#define GSMConferenceID 8

// Maximum length of a ringer name (string resource)
#define GSMMaxRingName		16

#define GSMLibUnknownID		0xff000000

#define GSMVolumeMin		0
#define GSMVolumeMax		7

// Phone Call Status flags
#define gsmVoiceCall1Active	  0x0001 // There is a voice call active on line1
#define gsmVoiceCall2Active	  0x0002 // There is a voice call active on line2
#define gsmCSDCallActive	  0x0004 // There is a data call currently active
										 // Note: Actually the virtual modem has
										 //	control but does not neccessarily
										 //	have an active data call
#define gsmGPRSCallActive	  0x0008 // There is a GPRS session active

// Phone Connection Type
typedef enum {
  voiceConnection	= 1,
  csdConnection		= 2,
  gprsConnection    = 3
} GSMConnectionEnum;

// Volume of phone ringer
typedef enum { 
	GSMRingerLoud	= 7, 
	GSMRingerSoft	= 1, 
	GSMRingerOff	= 0
} PhnRingerVolumeType;

// Enumeration types
enum GSMServiceKind {
	kVoice = 1, kData = 2, kTelefax = 4
};
enum GSMActionCode {
	gsmAppLaunchCmdEvent = 0x2BAD, // handle an event
	gsmAppLaunchCmdRegister // ask application for registration
};

// Phone "equipment modes"
typedef enum {
	gsmHandsetMode 		= 0,
	gsmSpeakerPhoneMode	= 1,
	gsmCarKitMode		= 2,
	gsmHeadsetMode		= 3,
	gsmHandsetLidCloseMode 	= 4
} GSMEquipmentMode;

enum GSMEvent {
	kCardInsertion,
	kRegistration,
	kError,
	kButton,
	kPower,
	kPassword,
	kProgress,
	kIndication,
	kConnectInd,
	kConnectConf,
	kSubscriber,
	kDisconnectInd,
	kDisconnectConf,
	kBusy,
	kUpdate,
	kConference,
	kVoiceMail,
		// Events used by the SMS library (see [R&P 1])
	kMessageInd,
	kSegmentInd,
	kMessageStat,
	kMessageDel,
	kMessageMoved,
	    // Events used by the SIM Application Toolkit 
	kSATNotification,
		// Events added after GSMLibrary went public
	kUSSDInd,
	kPhoneEquipmentMode
};
enum GSMEventClass {
	kECPhone      =  1,
	kECSMS        =  2,
	kECTelefax    =  4,
	kECData       =  8,
	kECVoiceMail  = 16,
	kECSIMToolkit = 32,
	kECAllClasses = kECPhone | kECSMS | kECTelefax | kECData | kECVoiceMail | kECSIMToolkit
};
enum GSMConnectionState {
	kActive, kHeld, kDialing, kAlerting, kIncoming, kWaiting,
		// Additional states
	kActiveHeld, kHeldHeld
};
enum GSMBarFacility {
	kBarNoOutgoing,
	kBarAllOutgoing, kBarInternational, kBarInternationalExceptHome,
	kBarNoIncoming,
	kBarAllIncoming, kBarWhenRoaming
};

// Phone call forwarding reason
enum GSMForwardingCondition {
	kForwardUnconditional,
	kForwardOnBusy,
	kForwardOnNoReply,
	kForwardOnNotReachable,
	kForwardTelefax,
	kForwardData,
	kForwardCancelAll,
	kForwardConditionalAll,
	kForwardLast
};

// Phone call forwarding mode
enum GSMForwardingMode {
	kForwardModeDisable,
	kForwardModeEnable,
	kForwardModeInterrogate,
	kForwardModeRegistration,
	kForwardModeErasure
};

enum GSMPasswordType {
	pwUnknown,				// FAULT or none of the strings below
	// See GSM 07.07, section 8.3 for a list of passwords. The following
	// list is based on on version 6.2.0 of the recommendation.
	pwNone,					// READY
	pwSIMPIN,				// SIM PIN
	pwSIMPUK,				// SIM PUK
	pwPhSIMPIN,				// PH-SIM PIN
	pwPh1SIMPIN,			// PH-FSIM PIN
	pwPh1SIMPUK,			// PH-FSIM PUK
	pwSIMPIN2,				// SIM PIN2
	pwSIMPUK2,				// SIM PUK2
		// It is assumed that all PINs and PUKs below are network-based
		// PINS and PUKs. Function Change() uses a different timeout for
		// such PINs and/or PUKs.
	pwNetworkPIN,			// PH-NET PIN
	pwNetworkPUK,			// PH-NET PUK
	pwNetworkSubsetPIN,	// PH-NETSUB PIN
	pwNetworkSubsetPUK,	// PH-NETSUB PUK
	pwServiceProviderPIN,// PH-SP PIN
	pwServiceProviderPUK,// PH-SP PUK
	pwCorporatePIN,		// PH-CORP PIN
	pwCorporatePUK,		// PH-CORP PUK
	// The passwords below are supported by the +CPWD command. See GSM 07.07
	// section 7.4 and 7.5 for a list of facility-specific passwords.
	pwBarrAO,				// all outgoing call
	pwBarrOI,				// outgoing intÕl calls
	pwBarrOX,				// outgoing intÕl calls except to home country
	pwBarrAI,				// all incoming calls
	pwBarrIR,				// incoming calls when roaming outside home country
	pwBarrAB,				// all barring services
	pwBarrAG,				// all outgoing barring services
	pwBarrAC					// all incoming barring services
};
enum GSMOperatorStatus {
	kUnknown, kAvailable, kCurrent, kForbidden
};
enum GSMAddressField {
	kAFPhone, kAFFirstName, kAFLastName
};
enum GSMModuleButton {
	kButtonPhoneApp,
	kButtonDataApp,
	kButtonHeadset
};
enum GSMBoxType {
	kBoxVoice, kBoxTelefax, kBoxEMail, kBoxOther, kBoxData
	// other values reserved for future expansion
};
enum GSMPowerType {
	kPowerOff, kPowerOn, kPowerStartCharging, kPowerStopCharging, kPowerLow
};
enum GSMProgressKind {
	kOpenDialog, kCloseDialog, kSetText, kSetRecipient, kShowSegment
};
enum GSMOpenDialogKind {
	kDialogSetBarring, kDialogGetBarring,
	kDialogSetForwarding, kDialogGetForwarding,
	kDialogSetCallWaiting, kDialogGetCallWaiting,
	kDialogGetOperatorList, kDialogSetOperator,
	kDialogOperatorSelection,
	kDialogImmediateSend, kDialogDeferredSend,
	kDialogSendUSSD
};
enum GSMDebugAction {
	dbgDumpRegistration, dbgSIMReady
};
enum GSMRegistrationStatus {
	registrationNone, registrationHome, registrationSearch, registrationDenied,
	registrationUnknown, registrationRoaming
};
enum GSMIndicationKind {
	indicationSIMReady, indicationSIMMessages, indicationNetworkSearch,
	indicationPasswordAccepted
};
enum GSMSIMMessagesDialogKind {
	gsmMessagesConfirmMove, gsmMessagesCantReceive
};
enum GSMSIMStatus {
	simMissing, simFailure, simWrong, simNotReady, simReady
};
enum GSMRegistrationMode {
	gsmRegModeAutomatic, gsmRegModeManual, gsmRegModeDeregister, gsmRegModeFormat,
	gsmRegModeManualAutomatic
};

enum GSMDialCLIRStatus {
	gsmDialCLIRNotProvisioned,
	gsmDialCLIRProvisioned,
	gsmDialCLIRUnknown,
	gsmDialCLIRTemporaryRestricted,
	gsmDialCLIRTemporaryAllowed
};

enum GSMDialCLIRMode {
	gsmDialCLIRDefault, gsmDialCLIRTemporaryInvocation, gsmDialCLIRTemporarySuppression
};

enum GSMAddressFormat {
	gsmFmtIgnoreKind = 0,
	gsmFmtMatchKind = 8,
	gsmFmtExceptKind = 16,
	gsmFmtAllKinds = 24,
	gsmFmtLastNameFirst = 32,
	gsmFmtShortLabel = 64
};
enum GSMButtonModifier {
	gsmBtnCodeMask = 0x00FF,
	gsmBtnPowerOnMask = 0x8000,
	gsmBtnUpMask = 0x4000
};
#ifndef __cplusplus
typedef enum GSMServiceKind GSMServiceKind;
typedef enum GSMActionCode GSMActionCode;
typedef enum GSMEvent GSMEvent;
typedef enum GSMEventClass GSMEventClass;
typedef enum GSMConnectionState GSMConnectionState;
typedef enum GSMBarFacility GSMBarFacility;
typedef enum GSMForwardingCondition GSMForwardingCondition;
typedef enum GSMForwardingMode GSMForwardingMode;
typedef enum GSMPasswordType GSMPasswordType;
typedef enum GSMOperatorStatus GSMOperatorStatus;
typedef enum GSMAddressField GSMAddressField;
typedef enum GSMModuleButton GSMModuleButton;
typedef enum GSMBoxType GSMBoxType;
typedef enum GSMPowerType GSMPowerType;
typedef enum GSMProgressKind GSMProgressKind;
typedef enum GSMOpenDialogKind GSMOpenDialogKind;
typedef enum GSMDebugAction GSMDebugAction;
typedef enum GSMRegistrationStatus GSMRegistrationStatus;
typedef enum GSMIndicationKind GSMIndicationKind;
typedef enum GSMSIMMessagesDialogKind GSMSIMMessagesDialogKind;
typedef enum GSMSIMStatus GSMSIMStatus;
typedef enum GSMRegistrationMode GSMRegistrationMode;
typedef enum GSMDialCLIRMode GSMDialCLIRMode;
typedef enum GSMDialCLIRStatus GSMDialCLIRStatus;
typedef enum GSMAddressFormat GSMAddressFormat;
typedef enum GSMButtonModifier GSMButtonModifier;
#endif

// Structured types
struct GSMRegistrationInfo {
	GSMEventClass classes;
};
struct GSMSubscriberAddrInfo {
	GSMAddressHandle caller;
	GSMServiceKind service;
};
struct GSMConferenceDesc {
	GSMConnectionID call1;
	GSMConnectionID call2;
	GSMConnectionID id;
};
struct GSMError {
	Err code;
	UInt32 id;
};
struct GSMOperatorDesc { // all fields should be considered read-only
	GSMOperatorStatus status;
	GSMOperatorID uniqueID;
	char* longname;
	char* shortname;
};
#ifndef __cplusplus
typedef struct GSMOperatorDesc GSMOperatorDesc;
#endif
struct GSMOperatorList {
	Int16 count;	// length of following array
	GSMOperatorDesc oper[1];
	// followed immediately by strings for operatorsÕ names
};
struct GSMSMSParams {
	GSMDatabaseID id;
	char oldStatus;
	char newStatus;
};
struct GSMButtonParams {
	GSMModuleButton name;
	UInt16 modifiers;
};
struct GSMRegistrationParams {
	GSMRegistrationStatus status;
};
struct GSMBoxData {
	Boolean indicatorOn;
	GSMBoxType type;
	Int16 messageCount;
	Int16 lineNumber;
};
struct GSMPowerParams {
	GSMPowerType state;
};
typedef char GSMPassword[10]; 
struct GSMPasswordParams {
	GSMPasswordType type;
	GSMPasswordType prevType;
	Err error;
	GSMPassword pin;
	GSMPassword puk;
};
struct GSMProgressParams {
	GSMProgressKind progress;
	GSMOpenDialogKind dialog;
		// only for SMS progress:
	UInt32 data;
};
struct GSMIndicationParams {
	GSMIndicationKind kind;
	char filler;
	union { // union is for further expansion
		struct {
			Boolean state;
		} simReady;
		struct {
			GSMSIMMessagesDialogKind dialog;
			Boolean moveMessages;
		} simMessages;
		struct {
			GSMPasswordType type;
		} passwordAccepted;
	} data;
};
struct GSMMovedMsgDesc {
	GSMDatabaseID msgID;
	UInt32 msgOwner;
	Err error;
	GSMEvent event;
};
#ifndef __cplusplus
typedef struct GSMRegistrationInfo GSMRegistrationInfo;
typedef struct GSMSubscriberAddrInfo GSMSubscriberAddrInfo;
typedef struct GSMConferenceDesc GSMConferenceDesc;
typedef struct GSMError GSMError;
typedef struct GSMOperatorList GSMOperatorList;
typedef struct GSMSMSParams GSMSMSParams;
typedef struct GSMButtonParams GSMButtonParams;
typedef struct GSMRegistrationParams GSMRegistrationParams;
typedef struct GSMBoxData GSMBoxData;
typedef struct GSMPowerParams GSMPowerParams;
typedef struct GSMPasswordParams GSMPasswordParams;
typedef struct GSMProgressParams GSMProgressParams;
typedef struct GSMIndicationParams GSMIndicationParams;
typedef struct GSMMovedMsgDesc GSMMovedMsgDesc;
typedef struct GSMMovedMsgsParams GSMMovedMsgsParams;
typedef struct GSMUSSDParams GSMUSSDParams;
typedef struct GSMSATParams GSMSATParams;
typedef struct GSMPhoneBookInfo GSMPhoneBookInfo;
typedef struct GSMPhoneEquipmentMode GSMPhoneEquipmentMode;
#endif

struct GSMMovedMsgsParams {
	UInt16 count;
	GSMMovedMsgDesc* list;
};
struct GSMUSSDParams {
	long		result;
	const char*	string;
};


struct GSMSATParams {
	MemHandle	dataH;
	UInt32		notificationType;
};

struct GSMPhoneBookInfo {
	UInt16	firstEntry;
	UInt16	lastEntry;
	UInt16	maxNameLength;
	UInt16	maxNumberLength;
};

struct GSMPhoneEquipmentMode {
	long	mode;
};

struct GSMEventRecord {
	GSMEvent event;
	Boolean acknowledge;
	GSMConnectionID id;
	UInt16 launchCode;
	MemPtr launchParams;
	union Data {
		GSMSubscriberAddrInfo info;
		GSMConferenceDesc conference;
		GSMError error;
		GSMSMSParams params;
		GSMButtonParams button;
		GSMRegistrationParams registration;
		GSMBoxData box;
		GSMPowerParams power;
		GSMPasswordParams password;
		GSMProgressParams progress;
		GSMIndicationParams indication;
		GSMMovedMsgsParams moved;
		GSMUSSDParams ussd;
		GSMSATParams sat;
		GSMPhoneEquipmentMode phoneEquipmentMode;
	} data;
};
struct GSMConnectionDesc {
	GSMConnectionID id;
	GSMConnectionState state;
	GSMServiceKind service;
	Boolean incoming;
	Boolean multiparty;
	GSMAddressHandle address;
	UInt32 owner;
};
struct GSMRingingProfile {
	UInt32 tone;			  // uniqueRecID of the MIDI sound
	Int16 volume;
	Boolean vibrate;
};
#ifndef _cplusplus
typedef struct GSMRingingProfile GSMRingingProfile;
#endif
struct GSMRingingInfo {
	GSMRingingProfile profile[GSMkSliderPositions];
};
#ifndef _cplusplus
typedef struct GSMEventRecord EventRecordType; //move this from GSMEvents.h
typedef struct GSMEventRecord GSMEventRecord;
typedef struct GSMConnectionDesc GSMConnectionDesc;
typedef struct GSMRingingInfo GSMRingingInfo;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// SIM Application Toolkit's data types
// type of request/respond we are sending
typedef enum {
	kSATSelectMainMenuItem,
	kSATClearText,
	kSATSetInkey,
	kSATSetInput,
	kSATMakeCall,
	kSATSelectItem = 6,
	kSATCancel	   = 95,
	kSATUnknownNotification,
	kSATApplicationBusy,
	kSATUserNotResponse,
	kSATEndSession
} SATRequestType;

// decision we are sending to SIM
typedef enum {
	kSATSessionAborted,
	kSATItemSelected,
	kSATHelpRequested,
	kSATNavigateBack
} SATDecisionType;

// type of notification sent by SAT
typedef enum {
	kSATMainMenuAvailable,
	kSATDisplayText,
	kSATGetInkey,
	kSATGetInput,
	kSATSetupCall,
	kSATPlayTone,
	kSATDisplaySubmenus,
	kSATRefresh,			// application do not get this notification
	kSATSendSS,
	kSATSendSMS,
	kSATSendUSSD,
	kSATTimeout		= 98,
	kSATSessionEnd	= 99
} SATNotificationType;

// meaning of feature bits in a GSMSATEventMenuRec
enum GSMSATEventMenuFlag {
	gsmSATEventMenuFlagHelp		= 1,// help is available
	gsmSATEventMenuFlagNextAct	= 2	// nextAction is valid 
};

#ifndef __cplusplus
typedef struct GSMSATEventHeader		GSMSATEventHeader;
typedef struct GSMSATEventDisplayText	GSMSATEventDisplayText;
typedef struct GSMSATEventGetInkey		GSMSATEventGetInkey;
typedef struct GSMSATEventGetInput		GSMSATEventGetInput;
typedef struct GSMSATEventSetupCall		GSMSATEventSetupCall;
typedef struct GSMSATEventPlayTone		GSMSATEventPlayTone;
typedef struct GSMSATEventSendSMS		GSMSATEventSendSMS;

typedef struct GSMSATEventMenuRec		GSMSATEventMenuRec;
typedef struct GSMSATEventMenu			GSMSATEventMenu;
typedef struct GSMNBSNotificationEventType GSMNBSNotificationEventType;
#endif

// header for all SATEvents
struct GSMSATEventHeader {
	UInt32	signature;
	UInt32	notificationType;
};

// for notificationType = kSATDisplayText
struct GSMSATEventDisplayText {
	UInt32	signature;
	UInt32	notificationType;

	UInt32	textOffset;
	UInt16	priority;
	UInt16	clearMode;
};

// for notificationType = kSATGetInkey
struct GSMSATEventGetInkey {
	UInt32	signature;
	UInt32	notificationType;

	UInt32	textOffset;
	UInt16	format;
	UInt16	helpInfo;
};

// for notificationType = kSATGetInput
struct GSMSATEventGetInput {
	UInt32	signature;
	UInt32	notificationType;

	UInt32	textOffset;
	UInt16	format;
	UInt16	helpInfo;
	UInt16	echoMode;
	UInt16	minSize;
	UInt16	maxSize;
};

// for notificationType = kSATSetupCall
struct GSMSATEventSetupCall {
	UInt32	signature;
	UInt32	notificationType;

	UInt32	numberOffset;
	UInt32	subAddressOffset;
	UInt16	type;
	UInt16	callType;
};

// for notificationType = kSATPlayTone
struct GSMSATEventPlayTone {
	UInt32	signature;
	UInt32	notificationType;

	UInt32	textOffset;
	UInt16	toneType;
	UInt16	timeUnit;	// valid only if timeInterval > 0
	UInt16	timeInterval;
};

// contain info for one menu item
struct GSMSATEventMenuRec {
	UInt32	labelOffset;
	UInt32	features;
	UInt16	id;
	UInt16	nextAction;
};

// for notificationType = kSATDisplaySubmenus
struct GSMSATEventMenu {
	UInt32	signature;
	UInt32	notificationType;

	UInt32	titleOffset;
	UInt32	defaultItem;	// only valid in submenu
	UInt32	recordCount;
	GSMSATEventMenuRec	record[1];
};


// for notificationType = kSATSendSS, kSATSendSMS, and kSATSendUSSD
struct GSMSATEventSendSMS {
	UInt32	signature;
	UInt32	notificationType;

	UInt32	textOffset;
};


/* Structure passed to the callbacks registered for incoming NBS notifications */
struct GSMNBSNotificationEventType
{
	UInt16 version;  /* version number to provide future backwards compatibility */

	/* helper fields */
	Boolean NBSdatagram;  /* flag if it is an NBS datagram */
	Boolean binary;       /* true if binary data */

	void *headerP;    /* pointer to raw header */
	UInt8 headerLen;  /* length of headerP */
	void *dataP;      /* pointer to data body */
	UInt8 dataLen;    /* length of dataP */

	/* NBS datagram fields */
	UInt8 refNum;    /* NBS reference number */
	UInt8 maxNum;    /* max segment number 1-255 */
	UInt8 seqNum;    /* sequence number    1-255, no more than maxNum */
	Int8  reserved1; /* padding */

	UInt32 srcPort;  /* source port */
	UInt32 dstPort;  /* destination port */

	/* SMS related fields */
	UInt32 msgID; /* ID into the SMS database to reference this
                  * message this ID is not gauranteed to be
                  * valid once the notification callback
                  * returns.  Users should make a copy of the
                  * msg if they want to work on it after the
                  * callback returns.
                  */

	char   *senderP;   /* sender number - null terminated */
	UInt32 datetime;   /* date/time stamp */
	Int32  reserved2;  /* reserved*/
	Int32  reserved3;  /* reserved*/
};



// GSM library routines
extern Err GSMLibGetLibAPIVersion(UInt16 refNum, UInt32* dwVerP)
				GSM_LIB_TRAP(GSMLibTrapGetLibAPIVersion);

extern Err GSMLibRegister(UInt16 refNum, UInt32 creator, UInt16 services)
				GSM_LIB_TRAP(GSMLibTrapRegister);

extern GSMAddressHandle GSMLibNewAddress(UInt16 refNum, const char* number, GSMDatabaseID id)
				GSM_LIB_TRAP(GSMLibTrapNewAddress);
extern char* GSMLibGetField(UInt16 refNum, GSMAddressHandle address, GSMAddressField field)
				GSM_LIB_TRAP(GSMLibTrapGetField);
extern GSMDatabaseID GSMLibGetID(UInt16 refNum, GSMAddressHandle address)
				GSM_LIB_TRAP(GSMLibTrapGetID);

extern Err GSMLibGetPhoneCallStatus(UInt16 refNum, UInt32* phnFlags)
				GSM_LIB_TRAP(GSMLibTrapGetPhoneCallStatus);

extern Err GSMLibSetField(UInt16, GSMAddressHandle address, GSMAddressField field, const char* data)
				GSM_LIB_TRAP(GSMLibTrapSetField);
extern Err GSMLibSetNumber(UInt16 refNum, GSMAddressHandle address, const char* number)
				GSM_LIB_TRAP(GSMLibTrapSetNumber);
extern Err GSMLibSetID(UInt16 refNum, GSMAddressHandle address, GSMDatabaseID id)
				GSM_LIB_TRAP(GSMLibTrapSetID);
extern char* GSMLibAddressToText(UInt16 refNum, GSMAddressHandle address, GSMAddressFormat format)
				GSM_LIB_TRAP(GSMLibTrapAddressToText);
extern Boolean GSMLibHasName(UInt16 refNum, GSMAddressHandle address)
				GSM_LIB_TRAP(GSMLibTrapHasName);
extern Boolean GSMLibEqual(UInt16 refNum, GSMAddressHandle a, GSMAddressHandle b)
				GSM_LIB_TRAP(GSMLibTrapEqual);

extern Err GSMLibGetRingingInfo(UInt16 refNum, GSMRingingInfo* info)
				GSM_LIB_TRAP(GSMLibTrapGetRingingInfo);
extern Err GSMLibSetRingingInfo(UInt16 refNum, const GSMRingingInfo* info)
				GSM_LIB_TRAP(GSMLibTrapSetRingingInfo);
extern Err GSMLibGetToneIDs(UInt16 refNum, UInt32** list, int* listLength)
				GSM_LIB_TRAP(GSMLibTrapGetToneIDs);
extern Err GSMLibGetToneName(UInt16 refNum, UInt16 toneIndex, char* name, short maxLength)
				GSM_LIB_TRAP(GSMLibTrapGetToneName);
extern Err GSMLibPlayTone(UInt16 refNum, UInt32 tone, int volume)
				GSM_LIB_TRAP(GSMLibTrapPlayTone);
extern Err GSMLibStopTone(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapStopTone);

extern Err GSMLibGetSMSRingInfo(UInt16 refNum, GSMRingingInfo* info)
				GSM_LIB_TRAP(GSMLibTrapGetSMSRingInfo);
extern Err GSMLibSetSMSRingInfo(UInt16 refNum, const GSMRingingInfo* info)
				GSM_LIB_TRAP(GSMLibTrapSetSMSRingInfo);
extern Err GSMLibGetSMSGateway(UInt16 refNum, char** smsGateway)
				GSM_LIB_TRAP(GSMLibTrapGetSMSGateway);

extern Err GSMLibPlayDTMF(UInt16 refNum, const char* digits)
				GSM_LIB_TRAP(GSMLibTrapPlayDTMF);

extern Err GSMLibSendUSSD(UInt16 refNum, const char* command)
				GSM_LIB_TRAP(GSMLibTrapSendUSSD);

extern Err GSMLibStartVibrate(UInt16 refNum, Boolean pulse, Boolean repeat)
				GSM_LIB_TRAP(GSMLibTrapStartVibrate);
extern Err GSMLibStopVibrate(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapStopVibrate);

extern Boolean GSMLibRegistered(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapRegistered);
extern Boolean GSMLibRoaming(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapRoaming);

extern Err GSMLibSetModulePower(UInt16 refNum, Boolean On)
				GSM_LIB_TRAP(GSMLibTrapSetModulePower);
extern Boolean GSMLibModulePowered(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapModulePowered);

extern UInt32 GSMLibGetDataApplication(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapGetDataApplication);
extern void GSMLibSetDataApplication(UInt16 refNum, UInt32 creator)
				GSM_LIB_TRAP(GSMLibTrapSetDataApplication);

extern GSMSIMStatus GSMLibGetSIMStatus(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapGetSIMStatus);
extern void GSMLibGetErrorText(UInt16 refNum, Err error, char* s, UInt16 sSize)
				GSM_LIB_TRAP(GSMLibTrapGetErrorText);

// SIM Application Toolkit
extern Err GSMLibSATGetMainMenu(UInt16 refNum, MemHandle* menuHandlePtr)
				GSM_LIB_TRAP(GSMLibTrapSATGetMainMenu);

extern Err GSMLibSATSendRequest(UInt16 refNum, SATRequestType request, SATDecisionType decision, UInt32 data, UInt32 length)
				GSM_LIB_TRAP(GSMLibTrapSATSendRequest);

extern Err GSMLibSATEndSession(UInt16 refNum)
				GSM_LIB_TRAP(GSMLibTrapSATEndSession);

#ifdef __cplusplus
// Global variables used within emulator for library.
namespace GSMLibrary {
	extern UInt16 Reference;
	extern SysLibTblEntryType TableEntry;

	// Callback for registration.
	extern void (*RegisterProc)(GSMRegistrationInfo* info);
}
#endif

#ifdef __cplusplus 
}
#endif


#endif	__GSMLIBRARY_H__
