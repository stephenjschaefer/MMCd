/***************************************************************
*
*  Project:
*	Handspring AppLaunchCmd
*
*  Copyright info:
*	Copyright 1999 Handspring, Inc. All Rights Reserved.   
*
*
*  FileName:
*	HsExt.h
* 
*  Description:
*	Public header file akin to Palm's AppLaunchCmd.h
*
*  ToDo:
* 
*  History:
*	12-oct-1999 dia - Created by Douglas Anderson
*   20-apr-2000 dia - New APIs for dialing.
*
****************************************************************/

#ifndef	  __HSAPPLAUNCHCMD_H__
#define	  __HSAPPLAUNCHCMD_H__

//#include <CoreCompatibility.h>
#include <AppLaunchCmd.h>


//=============================================================================
// Useful macros...
//=============================================================================

// <chg 07-mar-00 dia>
// The LaunchWithCommand() macro was added in the OS 3.2 headers, so we
// only define this macro if we're compiling with old headers.

#ifndef LaunchWithCommand
  #define LaunchWithCommand(type, creator, command, commandParams) \
  { \
	UInt16				cardNo; \
	LocalID				dbID; \
	DmSearchStateType	searchState; \
	Err 					err; \
	DmGetNextDatabaseByTypeCreator(true, &searchState, type, \
		creator, true,	&cardNo, &dbID); \
	ErrNonFatalDisplayIf(!dbID, "Could not find db"); \
	if (dbID) \
      { \
		err = SysUIAppSwitch(cardNo, dbID, command, (Ptr) commandParams); \
		ErrNonFatalDisplayIf(err, "Could not launch db"); \
	  } \
  }
#endif // def(LaunchWithCommand)


// <chg 07-mar-00 dia>
// Unfortunately, CallWithCmmand() was never defined (I should have known 
// better).  Still, just in case Palm ever does define it, I'll couch
// it in ifdefs.  Hopefully, they follow the other macro and it will look
// just like this.

#ifndef CallWithCommand
  #define CallWithCommand(type, creator, command, commandParams) \
  { \
	UInt16				cardNo; \
	LocalID				dbID; \
	DmSearchStateType	searchState; \
	UInt32 result; \
	Err err; \
	DmGetNextDatabaseByTypeCreator(true, &searchState, type, \
		creator, true,	&cardNo, &dbID); \
	ErrNonFatalDisplayIf(!dbID, "Could not find db"); \
	if (dbID) \
      { \
		err = SysAppLaunch(cardNo, dbID, 0, command, (Ptr) commandParams, &result); \
		ErrNonFatalDisplayIf(err, "Could not launch db"); \
	  } \
  }
#endif // def(CallWithCommand)


//=============================================================================
// Handspring-modified address book
//=============================================================================

//-------------------------------------------------------------------
// Extra launch codes
//-------------------------------------------------------------------

// ...start at customBase + 0x2300 so that hopefully we won't conflict with
// any launch codes that Palm or others add to the address book.
typedef enum 
  {
	addrAppLaunchCmdHsLookup = sysAppLaunchCmdCustomBase + 0x2300,
    addrAppLaunchCmdCallerID
  } 
AddrCustomActionCodes;

/*
 Switch to using notifications rather than launch codes to avoid potential
  conflicts if palm modifies the address book app.
 */
typedef enum
  {
		// NOTE: 
		// Shouldn't be using these values!  Notification types are
		// supposed to be 4-character creator IDs that are registered in Palm's
		// Creator database!  Guess it's a little late now, since these codes are
		// out in the field.
		addrAppNotificationHsLookup = sysAppLaunchCmdCustomBase + 0x2300,
		addrAppNotificationCmdCallerID,
		addrAppNotificationPhonebookApp,

		// DON'T ADD ANY MORE based on 2300:  See above NOTE

		addrAppNotificationListLookupTypeGet = 'HsLL'		// Registered 2001-09-12 w/ Creator DB
  }
AddrCustomNotificationCodes;

//-------------------------------------------------------------------
// Support for addrAppLaunchCmdHsLookup
//-------------------------------------------------------------------

// Parameter block is the same as OS 3.1's block for sysAppLaunchCmdLookup
typedef AddrLookupParamsType AddrHsLookupParamsType;
typedef AddrHsLookupParamsType * AddrHsLookupParamsPtr;

// We support all of the lookup fields that defined in the AddressLookupFields
// enum for the OS 3.1 headers, plus these two.  The string versions of these
// (for the formatstring paramter) are "voicephone" and "anyphone".
// ...start at 0x70 so hopefully we get no conflicts.

typedef enum {
    addrLookupVoicePhones=0x70,   // All phone fields except fax and email.
    addrLookupAnyPhones ,          // All phone fields (including email)
	addrLookupSMSPhones		  // Mobile and email fields
} AddressHsLookupFields;


//-------------------------------------------------------------------
// Support for addrAppLaunchCmdCallerID
//-------------------------------------------------------------------

// Parameter block...

typedef struct 
  {
    Char* lookupString;   // Must be only digits and include country code.
                          // Example: 14085551212
    Char* formatString;   // format of the output string like 
                          // AddrLookupParamsPtr->formatString
                          // Example ^name ( ^phonetypelabel ) might result
                          // in "David ( work )"

	Char* resultString; //  returned heap allocated string  resultStringH;
	Boolean (*callback)(void*); //call back; Returns true to stop execution
    void* ref;  // reference that gets passed to callbackFunc
	UInt32 uniqueID; // Unique ID of the matching record 0 if no match

  }
AddrCallerIDParamsType, * AddrCallerIDParamsPtr;


//-------------------------------------------------------------------
// Support for addrAppNotificationListLookupTypeGet
//-------------------------------------------------------------------

enum
  {
	addrListLookupTypeLastName,			  // phone lookup will be by last name...
	addrListLookupTypeFirstName,		  // phone lookup will be by first name...
	addrListLookupTypeCompany,			  // phone lookup will be by company (currently unused)...
	addrListLookupTypeIntelligent		  // phone lookup will be intelligent (either name or
										  //   some other matching) (currently unused)... 
  };
typedef UInt16 AddrListLookupTypeEnum;

typedef struct
  {
	UInt16					version;	  // IN/OUT:  should always be 0...
	AddrListLookupTypeEnum	type;		  // OUT:     the type of phone lookup that
										  //		  will occur if you do a lookup by
										  //		  listName.
  }
AddrListLookupTypeGetParamsType;


//-------------------------------------------------------------------
// Support for addrAppLaunchCmdHsPhonebook
//-------------------------------------------------------------------
typedef struct
  {
	UInt16	  firstChar;  // The first character that should be
						  // preloaded in the lookup field
	Boolean	  activeCall; // Whether the active call icon should be
						  // usable or not... could change in a sublaunch
						  // when a call is dropped
	Boolean   lookup;     // true if this notification is for a lookup
						  // false if it is for a new contact
	Char *    newPhoneNum;// Phone number of the new contact.
  }
AddrPhonebookAppParamsType, * AddrPhonebookAppParamsPtr;


//-------------------------------------------------------------------
// Support for dialPnlLaunchCmd... commands
//-------------------------------------------------------------------

// Boolean IsDialingAvailable (UInt32 serviceFlags)
// ------------------------------------------------

typedef struct 
  {
    Boolean result;             // OUT: Space for result...
    UInt8 _reserved0;           // IN:  Reserved byte--MUST BE 0.
    
    const UInt32* serviceIDsP;  // IN:  Array of services to check...
    UInt16 numServices;         // IN:  Num elements in service IDs array.
  }
DialPnlIsDialingAvailableParamsType;


// void DispatchDial (UInt16 service, void* paramP)
// ------------------------------------------------

typedef struct
  {
    UInt32	serviceID;		// IN:  The service type to dial...
    void*	dataP;          // IN:  Basic Data to send to the helper...
    Char*	descriptionStr; // IN:  Description of data to dial...
	UInt8	handled;		// OUT: non-zero if handled
    Err		err;			// OUT: Err code returned...
  }
DialPnlDispatchDialParamsType;


//=============================================================================
// Handspring phone application
//=============================================================================

// DOLATER: Some of these may be obsolete now...

//------------------------------------------------------------------------
// Phone application's custom launch codes
//------------------------------------------------------------------------

// Launch the phone application and dial the passed number
#define phoneAppLaunchCmdDial			(sysAppLaunchCmdCustomBase)

// Launch the phone application and dial the last number dialed.
#define phoneAppLaunchCmdRedial			(sysAppLaunchCmdCustomBase+1)

// Launch the phone application and display the active calls view.
#define phoneAppLaunchCmdViewCalls		(sysAppLaunchCmdCustomBase+2)

// Launch the phone application and prompt to power on the phone
#define phoneAppLaunchCmdPowerOn		(sysAppLaunchCmdCustomBase+3)

// Launch the phone application and dial a number
#define phoneAppLaunchCmdDialVoiceMail	(sysAppLaunchCmdCustomBase+4)

// Launch the phone application and display the call history view.
#define phoneAppLaunchCmdViewHistory	(sysAppLaunchCmdCustomBase+5)

// Launch the phone application and display the speed dial view.
#define phoneAppLaunchCmdViewSpeed		(sysAppLaunchCmdCustomBase+6)

// Launch the phone application and display the kaypad view.
#define phoneAppLaunchCmdViewKeypad		(sysAppLaunchCmdCustomBase+7)

// Launch the ## Application
#define phoneAppLaunchCmdActivation     (sysAppLaunchCmdCustomBase+10)

//------------------------------------------------------------------------
// Data send this request to dial a number (phoneAppLaunchCmdDial).
//------------------------------------------------------------------------

typedef enum
  {
	PhoneAppDialMethodNormal = 0,	  // Normal PhnLibDial()
	PhoneAppDialMethodSATSetupCall,	  // Use SIM Toolkit and respond "affirmative"
									  //   to the SIM's request to set up a call.
  }
PhoneAppDialMethodEnum;

typedef struct 
	{
	UInt8					version;			// version number, set to one
	Boolean					confirm;			// confirm before dialing.
	Char*					name;				// optional
	Char*					number;				// optional
	Char*					extraDigits;		// optional
	// Version 1
	UInt32					failLaunchCreator;	// app to relaunch if dialling fails
	UInt16					failLaunchCode;		// launch code to use when relaunching
	void*					failLaunchParams;	// params to use when relaunching
	UInt16					dialMethod;			// Method to use to dial the phone number
												//	 See PhoneAppDialMethodEnum.
	}
PhoneAppLaunchCmdDialType, * PhoneAppLaunchCmdDialPtr;	




#endif // __HSAPPLAUNCHCMD_H__




