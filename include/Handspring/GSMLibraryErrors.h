/*******************************************************************************

FILE GSMLibraryErrors.h
 Copyright Option International 1999-2000 and Handspring Inc. 1999-2001
 All rights reserved.

SYNOPSIS
 Error codes returned by the GSM library.

*******************************************************************************/

#ifndef __GSMLIBRARYERRORS_H__
#define __GSMLIBRARYERRORS_H__

// GSM Library result codes
typedef enum GSMErrorCode {
	gsmErrorClass = 0x4000,
	gsmErrParam,
	gsmErrUnknownError,
	gsmErrNoResponse,
	gsmErrNotOpen,
	gsmErrStillOpen,
	gsmErrMemory,
	gsmErrUnknownID,
	gsmErrNoPower,
	gsmErrNoNetwork,
	gsmErrNoConnection,
	gsmErrNotAllowed,
	gsmErrIllegalFacility,
	gsmErrIllegalCondition,
	gsmErrIllegalStatus,
	gsmErrIllegalIndex,
	gsmErrIllegalChars,
	gsmErrIllegalMsg,
	gsmErrIllegalType,
	gsmErrIllegalNumber,
	gsmErrTimeout,
	gsmErrUnknownApp,
	gsmErrUnknownNumber,
	gsmErrBufferTooSmall,
	gsmErrPasswordRequired,
	gsmErrResponsePending,
	gsmErrCancelled,
	gsmErrNoRecipient,
		// Errors defined in the GSM recommendations
	gsmErrPhoneFailure,
	gsmErrPhoneNotConnected,
	gsmErrPhoneAdaptorLinkReserved,
	gsmErrNotSupported,
	gsmErrPhPINRequired,
	gsmErrPhFPINRequired,
	gsmErrPhFPUKRequired,
	gsmErrNoSIM,
	gsmErrPINRequired,
	gsmErrPUKRequired,
	gsmErrSIMFailure,
	gsmErrSIMBusy,
	gsmErrSIMWrong,
	gsmErrIncorrectPassword,
	gsmErrPIN2Required,
	gsmErrPUK2Required,
	gsmErrMemoryFull,
	gsmErrInvalidMemIndex,
	gsmErrNotFound,
	gsmErrMemFailure,
	gsmErrStringTooLong,
	gsmErrInvalidTextChars,
	gsmErrDialStringTooLong,
	gsmErrInvalidDialChars,
	gsmErrNoNetworkService,
	gsmErrNetworkTimeout,
	gsmErrNetworkNotAllowed,
	gsmErrNetPINRequired,
	gsmErrNetPUKRequired,
	gsmErrNetSubPINRequired,
	gsmErrNetSubPUKRequired,
	gsmErrSPPINRequired,
	gsmErrSPPUKRequired,
	gsmErrCorpPINRequired,
	gsmErrCorpPUKRequired,
	gsmErrIllegalMS,
	gsmErrIllegalME,
	gsmErrGPRSNotAllowed,
	gsmErrPLMNNotAllowed,
	gsmErrLocAreaNotAllowed,
	gsmErrRoamingNotAllowed,
	gsmErrOptionNotSupported,
	gsmErrReqOptionNotSubscribed,
	gsmErrOptionTempOutOfOrder,
	gsmErrUnspecifiedGPSRError,
	gsmErrAuthenticationFailure,
	gsmErrInvalidMobileClass,
	gsmErrUnassignedNumber,
	gsmErrOperDeterminedBarring,
	gsmErrCallBarred,
	gsmErrSMSXferRejected,
	gsmErrDestOutOfService,
	gsmErrUnidentifedSubscriber,
	gsmErrFacRejected,
	gsmErrUnknownSubscriber,
	gsmErrNetworkOutOfOrder,
	gsmErrTemporaryFailure,
	gsmErrCongestion,
	gsmErrResourcesUnavailable,
	gsmErrReqFacNotSubscribed,
	gsmErrReqFacNotImplemented,
	gsmErrInvalidSMSReference,
	gsmErrInvalidMsg,
	gsmErrInvalidMandInfo,
	gsmErrMsgTypeNonExistent,
	gsmErrMsgNoCompatible,
	gsmErrInfoElemNonExistent,
	gsmErrProtocolError,
	gsmErrInterworking,
	gsmErrTelematicIWNotSupported,
	gsmErrSMType0NotSupported,
	gsmErrCannotReplaceMsg,
	gsmErrUnspecifiedTPPIDError,
	gsmErrAlphabetNotSupported,
	gsmErrMsgClassNotSupported,
	gsmErrUnspecifiedTPDCSError,
	gsmErrCmdCannotBeActioned,
	gsmErrCmdUnsupported,
	gsmErrUnspecifiedTPCmdError,
	gsmErrTPDUNotSupported,
	gsmErrSCBusy,
	gsmErrNoSCSubscription,
	gsmErrSCSystemFailure,
	gsmErrInvalidSMEAddr,
	gsmErrDestSMEBarred,
	gsmErrSMRejectedDuplicate,
	gsmErrTPVPFNotSupported,
	gsmErrTPVPNotSupported,
	gsmErrSMSStorageFull,
	gsmErrNoSMSStorage,
	gsmErrErrorInMS,
	gsmErrSIMApplToolkitBusy,
	gsmErrMEFailure,
	gsmErrSMSServReserved,
	gsmErrInvalidParameter,
	gsmErrFiller,
	gsmErrFiller2,
	gsmErrFiller3,
	gsmErrMemoryFailure,
	gsmErrSCAddrUnknown,
	gsmErrNoCNMAAckExpected,
		// Errors returned by the firmware (NO CARRIER)
	gsmErrFDNMismatch,
	gsmErrEmergencyCallsOnly,
	gsmErrACMLimitExceeded,
	gsmErrHoldError,
	gsmErrNumberBlacklisted,
	gsmErrLidClosed,
		// Errors for SIM Application Toolkit
	gsmErrSATUnavailable,
	gsmErrSATInactive,
	gsmErrUNUSED,
		// Library loading errors
	gsmErrRadioNotAvailable,
		// Used internally
	gsmErrReserved_408b,
	gsmErrReserved_408c,
	gsmErrReserved_408d,
		// Firmware boot synchonization
	gsmErrFirmwareBootNotInprogress,// 0x408e
	gsmErrFirmwareBootInprogress,	// 0x408f
		// These error codes map directly to Wismo error
		// codes, but maybe could be used by other radios?
	gsmErrMMFailed,					// 0x4090
	gsmErrLowerLayer,				// 0x4091
	gsmErrCPError,					// 0x4092
	gsmErrCommandInProgress,		// 0x4093
	gsmErrSATNotSupported,			// 0x4094
	gsmErrSATNoInd,					// 0x4095
	gsmErrNeedResetModule,			// 0x4096
	gsmErrCOPSAbort					// 0x4097

	/***********************************************
	* WARNING:  If adding a new error code, you 
	* must update Errors.h, GSMLibraryErrors.h, 
	* and PhoneLibErrors.h.  
	************************************************/

} GSMErrorCode;

#endif
