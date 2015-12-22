/***************************************************************
*
*  Project:
*	Handspring Common Includes
*
*  Copyright info:
*	Copyright 1998 Handspring, Inc. All Rights Reserved.   
*
*
*  FileName:
*	HsExt.h
* 
*  Description:
*	Public header file for the Handspring extensions to PalmOS
*
*  ToDo:
* 
*  History:
*	13-Jan-1999 RM - Created by Ron Marianetti
*	06-Apr-2000 BP - Modified to support new gcc compiler
*					 with multisegment support
*	13-Jul-2000 BP - Modified to support compiling apps with 
*					 the 3.1 sdk
****************************************************************/

#ifndef	  __HSEXT_H__
#define	  __HSEXT_H__


// Detect if we are using the pre-3.5 sdk, and if so, include the 
// new types used in the Handspring headers.  If not, then include 
// PalmTypes.h for legacy reasons. 
//
// Note:  to compile projects with 3.1 headers or earlier, you must 
// include Common.h or Pilot.h before including this header file.
// For best compatibility, all users should include Common.h (or 
// PalmTypes.h if using the 3.5 headers or later) before including 
// Handspring headers.  Also, include HsExt.h or HsNewTypes.h before
// other Handspring headers so that the 3.5 types are defined properly.
#if (defined __COMMON_H__) || (defined __PILOT_H__) 

	#include <HsNewTypes.h>  // for supporting 3.5-style types, like UInt8
	#include <Common.h>    // for backward compatibility with old 3.1 headers

	#define sysTrapOEMDispatch		0xA349  /* OEM trap in Palm OS 3.0 and later trap table (formerly sysTrapSysReserved4) */
	
#else
	#include <PalmTypes.h>
#endif

// Handspring Database creator IDs 
#include <HsCreators.h>
#include <HsErrorClasses.h>

//=============================================================================
// Handspring API Equates
//=============================================================================

// This is the Handspring feature id. Apps can tell if they're on
//  a handspring device if they get 0 err back from: 
//		err = FtrGet (hsFtrCreator, hsFtrIDVersion, &value)
#define	hsFtrCreator				 'hsEx'


// OLD METHOD: (bit 15 is 0)
// 0xMMmfsbbb, where MM is major version, m is minor version
// f is bug fix, s is stage: 3-release,2-beta,1-alpha,0-development,
// bbb is build number for non-releases 
// V1.12b3   would be: 0x01122003
// V2.00a2   would be: 0x02001002
// V1.01     would be: 0x01013000

// NEW METHOD: (bit 15 is 1)
// 0xMMmf %1xxxssbbbbbbbbbb, where MM is major version, m is minor version
// f is bug fix, xxx (3-bits) is extension rev, 
// ss (2-bits) is stage: 3-release,2-beta,1-alpha,0-development,
// bbbbbbbbbb (10-bits) is build number for non-releases 
// V1.12b3   would be: 0x01120803
// V2.00a2   would be: 0x02000402
// V1.01     would be: 0x01010C00
// V3.5.2H2  would be: 0x0352AC00

#define	hsFtrIDVersion				  0		 


// Modification date of Handspring extensions
#define	hsFtrIDModDate				  1		 

// Feature number indicating that the Launcher Database Mgr library is loaded.
//  The value of the feature is the refNum of the loaded library.
//  Call FtrGet (hsFtrCreator, hsFtrIDLdbMgrLibRefNum, ...) to get this feature.
#define	hsFtrIDLdbMgrLibRefNum		  2

// If this feature is present, then we won't use the optimization to grab the
// memory semaphore before doing multiple data manager calls.
#define hsFtrIDNoMemSemaphore         3

// This features contains boolean flags to let developers know that a bug
// present in older devices has been fixed.
#define hsFtrIDBugsFixed			  4

// This feature tells what type of hardware keys a device has.
#define hsFtrIDTypeOfKeyboard		  5

// The masks used to interpret the hardware keyboard feature
#define	hsFtrValKeyboardBasic		0x0001  // Basic keys (pwr/scroll/app)
#define hsFtrValKeyboardQwerty		0x0002  // Qwerty keys (A-Z, Period, Space, Backspace, Enter,
											//	Menu, Option, Shift)
#define hsFtrValKeyboardJog			0x0004	// Jog keys (jog-up, jog-down, jog-scan)
#define hsFtrValKeyboardCommCentric	0x0008	// Communicator centric buttons

// This feature describes the radio hardware available
#define hsFtrIDRadioAttributes		  6

// The masks used to interpret the radio attributes feature
#define	hsFtrValRadioAvailable		0x0001  // Radio is present and available for use
#define hsFtrValRadioBuiltin		0x0002  // Radio is part of the base hardware 
											// and not on a module

// This feature defines how the power putton behaves
#define hsFtrIDPowerKeyResponse		  7

#define hsFtrValPowerKeyDoubleClkBacklight	0x0001  // Double click for backlight
#define hsFtrValPowerKeyTripleClkInvert		0x0002  // Triple click for invers (requires DblClkBacklight)
#define hsFtrValPowerKeyHoldRadioPower		0x0004  // Hold for radio power

// This feature indicates the functionality of the status gadgets API
#define hsFtrIDStatusGadgetRev		  8

#define hsFtrValStatusGadgetRev1	  0x0001  // Revision 1 of API

// If this feature is set, the graffiti area is disabled
#define hsFtrIDGrfAreaDisabled		  9

// This feature tells which keyboard layout is being used
#define	hsFtrIDKeyboardLayout		  10

// The values used to interpret the keyboard layout feature
#define	hsFtrValKeyboardLayout_enUS		0x0001  // English
#define	hsFtrValKeyboardLayout_deDE		0x0002  // German
#define	hsFtrValKeyboardLayout_frFR		0x0003  // French

// This features contains the DB Id to open and
// play the resource with type 'smfr' with resource id 1
// when the lid opens
#define hsFtrIDToneOnLidOpen		  11


//=============================================================================
// Display Attribute Equates
//=============================================================================

// These are present on HsExtensions 3.5.2H5.0 and up.

#define hsDispFtrCreator					  'DisA'

// Display Attributes
// ------------------

// Attributes about the display / controller combination...
#define hsDispFtrIDDisplayAttributes		  0

// Values for the display attributes, which is a bitfield.
#define hsDispFtrValLCDControllerTypeMask	  0x000000FF
#define hsDispFtrValDragonballVZ			  0x00000000
#define hsDispFtrValSed1375					  0x00000001
#define hsDispFtrValSed1376					  0x00000002
#define hsDispFtrValSed13A3					  0x00000003

#define hsDispFtrValLCDDisplayTypeMask		  0x0000FF00
#define hsDispFtrValBwStn					  0x00000100	// The black and white STN used in black and
															//	 white Visors and Treos.
#define hsDispFtrValColorStn				  0x00000200	// New color STN
#define hsDispFtrValColorTft				  0x00000300	// TFT used in Prism.
#define hsDispFtrValColorOled				  0x00000400	// Organic LED

// Available Shades
// ----------------

// This feature contains the number of shades of gray, red, green, and blue
// available on this display.  Note that this will _never_ be bigger than
// the number of shades available on the controller, since the display is
// fundamentally limited by what you can set the controller to.  See masks
// below for accessing...
#define hsDispFtrIDDisplayShades			  1

// This feature contains the number of shades of gray, red, green, and blue
// available on this controller.  This can be larger than the number of shades
// that the display can show, since the controller can do hardware dithering
// to deal with the fact that the display can't show all the shades.
#define hsDispFtrIDControllerShades			  2


// You can use these bitmasks to get the number of shades
// ...note that the actual number of shades is the (value+1).  This is needed
// so in the future we can support displays/controllers that can have 256 shades.
#define hsDispFtrValGrayOffset				  24
#define hsDispFtrValRedOffset				  16
#define hsDispFtrValGreenOffset				  08
#define hsDispFtrValBlueOffset				  00

// Sample usage:
//
// UInt32 displayShades;
// UInt16 redShades;
// Err err = 0;
//
// err = FtrGet (hsDispFtrCreator, hsDispFtrIDDisplayShades, &displayShades);
// if (!err)
//   {
//     // Get the number of red shades - 1...
//     redShades = (UInt16) ((UInt8) (displayShades >> hsDispFtrValRedOffset));
//     
//     // Account for +1 after doing the conversion to 8-bit...
//     redShades++;
//   }

// ------------------------------------------------------------------------------
// Error codes
// ------------------------------------------------------------------------------
#define	hsErrorClass					  (appErrorClass+0x0100)
#define	hsErrNotSupported				  (hsErrorClass | 0x01)
#define	hsErrInvalidCardNum				  (hsErrorClass | 0x02)
#define	hsErrReadOnly					  (hsErrorClass | 0x03)
#define	hsErrInvalidParam				  (hsErrorClass | 0x04)
#define	hsErrBufferTooSmall				  (hsErrorClass | 0x05)
#define	hsErrInvalidCardHdr				  (hsErrorClass | 0x06)
#define	hsErrCardPatchAlreadyInstalled	  (hsErrorClass | 0x07)
#define	hsErrCardPatchNotInstalled		  (hsErrorClass | 0x08)
#define	hsErrNotEnoughPower				  (hsErrorClass | 0x09)
#define	hsErrCardNotInstalled			  (hsErrorClass | 0x0A)
#define hsErrInvalidPeriod				  (hsErrorClass | 0x0B)
#define hsErrPeriodicNotInstalled		  (hsErrorClass | 0x0C)
#define hsErrFixedStorageRequired		  (hsErrorClass | 0x0D)
#define hsErrPeriodicAlreadyInstalled	  (hsErrorClass | 0x0E)
#define hsErrInUse						  (hsErrorClass | 0x0F)
#define hsErrIndicatorInvalidSequence	  (hsErrorClass | 0x10)
#define hsErrIndicatorInvalidCap		  (hsErrorClass | 0x11)
#define hsErrIndicatorDisabled			  (hsErrorClass | 0x12)
#define hsErrUnderflow					  (hsErrorClass | 0x13)

// ------------------------------------------------------------------------------
// OEM Error codes
// ------------------------------------------------------------------------------

#define hsErrRadioOff					  (hsOEMRadioErrorClass | 0x01)
#define	hsErrRadioNotRegistered			  (hsOEMRadioErrorClass | 0x02)
#define hsErrVoiceCallActive			  (hsOEMRadioErrorClass | 0x03)
#define hsErrSimNotReady				  (hsOEMRadioErrorClass | 0x04)
#define hsErrPhoneLocked				  (hsOEMRadioErrorClass | 0x05)
#define hsErrPhoneNotActivated			  (hsOEMRadioErrorClass | 0x06)

// ------------------------------------------------------------------------------
//  Bug Fix flags
// ------------------------------------------------------------------------------

#define hsBugFixSoundsFromCardEventsWork  (0x00000001)		// Check for bug in HSX >= 0x03523020
#define hsBugFixFlickerAdjustmentsInHal	  (0x00000002)		// Check for flick fix in HAL
#define hsBugFixPrefCallsAreThreadSafe	  (0x00000004)		// Pref manager calls can be called from
															// a background task.

// Old names (for backward compatibility)
#define hsFlickerFixAdjustmentsInHal	  hsBugFixFlickerAdjustmentInHal

// ------------------------------------------------------------------------------
//  Key codes and events
// ------------------------------------------------------------------------------

// Max card user event number that can be passed to HsCardEventPost()
#define	hsMaxCardEvent				  0x07

// Max app user event number that can be passed to HsAppEventPost()
#define	hsMaxAppEvent				  0x03

// Keycode range assigned to us from Palm: vchrSlinkyMin to vchrSlinkyMax
#define	hsChrRangeMin				  0x1600
#define	hsChrRangeMax				  0x16FF


// New key codes we generate for the "dot" soft icons
#define hsChrMidLeftDot				  hsChrRangeMin
#define hsChrMidRightDot			  (hsChrRangeMin+0x01)
#define hsChrBotLeftDot				  (hsChrRangeMin+0x02)
#define hsChrBotRightDot			  (hsChrRangeMin+0x03)

// The virtual cradle 2 character i
#define	hsChrCradle2OnChr			  (hsChrRangeMin+0x04)	  // dock input level asserted
#define	hsChrCradle2OffChr			  (hsChrRangeMin+0x05)	  // dock input level de-asserted

// card removed or inserted
#define	hsChrCardStatusChg			  (hsChrRangeMin+0x06)	  

// no-op key event that is used to interrupt apps that are
// interrupted by key events...
#define hsChrNoop					  (hsChrRangeMin+0x07)

// jog-in key
#define hsChrJogScan				  (hsChrRangeMin+0x08)

// key that invokes post processing mechanism
#define hsChrPostProcess			  (hsChrRangeMin+0x09)

// answer incoming call
#define hsChrAnswer					  (hsChrRangeMin+0x0A)

// the lid was either opened or closed (to determine which,
//   call HsAttrGet(hsAttrLidOpen) to check the current state)
#define hsChrLidStatusChg			  (hsChrRangeMin+0x0B)

// toggle the radio power
#define hsChrRadioPower				  (hsChrRangeMin+0x0C)

// update the graffiti state indicator
#define hsChrUpdateGrfIndicator		  (hsChrRangeMin+0x0D)

// shift + page-up
#define	hsChrShiftPageUp			  (hsChrRangeMin+0x0E)

// shift + page-down
#define hsChrShiftPageDown			  (hsChrRangeMin+0x0F)

// please shut down netlib
#define hsChrShutdownNetlib			  (hsChrRangeMin+0x10)

// mute switch on or off
#define hsChrMuteOn					  (hsChrRangeMin+0x11)
#define hsChrMuteOff				  (hsChrRangeMin+0x12)


// These keys have been defined by Palm at our request
#define  vchrHard5               0x0214         // Alternative vchrHard1 action
#define  vchrHard6               0x0215         // Alternative vchrHard2 action
#define  vchrHard7               0x0216         // Alternative vchrHard3 action
#define  vchrHard8               0x0217         // Alternative vchrHard4 action
#define  vchrHard9               0x0218         // Alternative vchrHardPower or other action
#define  vchrHard10              0x0219         // Alternative vchrHardCradle or other action

// opt + app
#define hsChrOptHard1			  vchrHard5
#define hsChrOptHard2			  vchrHard6
#define hsChrOptHard3			  vchrHard7
#define hsChrOptHard4			  vchrHard8


// Range of key events available to the HsCardEventPost() call
#define	hsChrCardUserFirst			  (hsChrRangeMin+0x80)
#define	hsChrCardUserLast			  (hsChrCardUserFirst+hsMaxCardEvent)


// Range of key events available to the HsAppEventPost() call
#define	hsChrAppUserFirst			  (hsChrCardUserLast+1)
#define	hsChrAppUserLast			  (hsChrAppUserFirst+hsMaxAppEvent)




// ------------------------------------------------------------------------------
// Special Databases that can be present on a card
// ------------------------------------------------------------------------------

// Handspring defined selector codes for the "CardSetup" application on
//  a card
#define	hsSysAppLaunchCmdInstall	  (sysAppLaunchCmdCustomBase+0)
#define	hsSysAppLaunchCmdRemove		  (sysAppLaunchCmdCustomBase+1)


// The cmdPBP parmeter to the setup app's PilotMain() will point to
//  this structure when the hsSysAppLaunchCmdInstall action code is sent
typedef struct 
  {
	UInt16		cardNo;					// card # of removable card
	UInt8		isReset;				// true if being called due to a soft or
										//  hard reset. The setup app must be "tolerant"
										//  of being sent an install action code
										//  during reset even if it was already
										//  installed before the reset. 
	UInt8		reserved;

	// Database info of the launched setup app, sent for convenience
	UInt16		setupCardNo;			// card # of setup app 
	LocalID		setupDbID;				// database ID of setup app

  } HsSysAppLaunchCmdInstallType;

// The cmdPBP parmeter to the setup app's PilotMain() will point to
//  this structure when the hsSysAppLaunchCmdRemove action code is sent
typedef struct 
  {
	UInt16		cardNo;					// card # of removable card
  } HsSysAppLaunchCmdRemoveType;


// If a card has a database with this type and creator, it will be automatically
//  copied to card 0 when inserted and sent an action code of 
// hsSysAppLaunchCodeInstall. Likewise, when the card is removed,
//  this app will be sent an action code of hsSysAppLaunchCodeRemove and
//  then it will be deleted.
// 
// hsFileCCardSetup			<= defined in HsCreators.h
// hsFileTCardSetup			<= defined in HsCreators.h


// If a card has an application database with this name, it will be
//  automatically launched when the card is inserted
#define	hsWelcomeAppName			  "CardWelcome"



// ------------------------------------------------------------------------------
// Library alias names that SysLibFind maps to a real library name
//  according to the appropriate hsPrefSerialLibXXX setting.
// 
// By Convention, library alias names start with a '*'. The exception
//  is the "Serial Library" name which is mapped in order to 
//  be compatible with pre-existing applications that already use it. 
// ------------------------------------------------------------------------------
#define	hsLibAliasDefault			  "Serial Library" //hsPrefSerialLibDef
#define	hsLibAliasHotSyncLocal		  "*HsLoc  SerLib" //hsPrefSerialLibHotSyncLocal
#define	hsLibAliasHotSyncModem		  "*HsMdm  SerLib" //hsPrefSerialLibHotSyncModem
#define	hsLibAliasIrda				  "*Irda   SerLib" //hsPrefSerialLibIrda
#define	hsLibAliasConsole			  "*Cons   SerLib" //hsPrefSerialLibConsole


// Actual library name of the Dragonball's built-in serial library.
// This is the default value of the hsPrefDefSerialLib pref setting which
//  SysLibFind uses to map an incoming library name to an actual library
//  name. 
#define	hsLibNameBuiltInSerial		  "BuiltIn SerLib"



// <chg 19-oct-2000 dia> Fake Srm ports moved to private header--clients
// shouldn't rely on these staying constant through multiple versions of
// HsExtensions...

// ------------------------------------------------------------------------------
// Flags for the HsDatabaseCopy routine
// ------------------------------------------------------------------------------
#define	hsDbCopyFlagPreserveCrDate	  0x0001	  // preserve creation date
#define	hsDbCopyFlagPreserveModDate	  0x0002	  // preserve modification date
#define	hsDbCopyFlagPreserveModNum	  0x0004	  // preserve modification number
#define	hsDbCopyFlagPreserveBckUpDate 0x0008	  // preserve backup  date
#define	hsDbCopyFlagOKToOverwrite	  0x0010	  // if true, it's OK to overwrite
												  //  existing database. 
#define	hsDbCopyFlagDeleteFirst		  0x0020	  // delete dest DB first, if it
												  //  exists. Use this if space
												  //  is limited on dest card.
												  //  Implies hsDbCopyFlagOKToOverwrite
#define	hsDbCopyFlagPreserveUniqueIDSeed 0x0040	  // preserve database unique ID seed


// ------------------------------------------------------------------------------
// Flags for the HsDmGetNextDBByTypeCreator() call
// ------------------------------------------------------------------------------
#define	hsDmGetNextDBFlagOneCard	  0x00001	  // Only search 1 card


// ------------------------------------------------------------------------------
// Reason codes for the card power handler
// ------------------------------------------------------------------------------
typedef enum
  {
	hsCardPwrDownNormal = 0,					  // normal power down
	hsCardPwrDownLowVoltage = 1					  // low voltage		-- never made in HSX 3.5.2 v. 3.0
  } HsCardPwrDownEnum;


// ------------------------------------------------------------------------------
// Result codes for HsCardBatteryStatusCallback
// ------------------------------------------------------------------------------

// Charge states that a rechargable battery can be in.  Not all of these
// states are actually represented w/ the current LED.
typedef enum
  {
	hsCardBatteryStatusNoBattery = 0,		// No battery (currently) in module...
	hsCardBatteryStatusFastCharge,			// Battery is in fast-charge mode; for
											//  batteries that don't distinguish fast vs.
											//  slow, just use this result...
	hsCardBatteryStatusSlowCharge,			// Battery is in slow-charge mode.
	hsCardBatteryStatusFullyCharged			// Battery is fully charged.
  } HsCardBatteryStatusEnum;

// ------------------------------------------------------------------------------
// Equates for the HsCardAttrGet/Set calls
// ------------------------------------------------------------------------------

// Prototypes for the various handlers that can be installed for a card
typedef void	(*HsCardIntHandlerPtr) (UInt32 cardParam, Boolean* sysAwakeP);

typedef void	(*HsCardPwrHandlerPtr) (UInt32 cardParam, Boolean sleep,
			  							UInt16 /*HsCardPwrDownEnum*/ reason);

typedef Boolean (*HsCardEvtHandlerPtr) (UInt32 cardParam, UInt16 evtNum,
										UInt16 evtParam);

typedef HsCardBatteryStatusEnum (*HsCardChargeStatusCallbackPtr) (void);

// Bits for the hsCardAttrSlotInfo attribute
#define hsCardAttrSlotSpringboard		0x00000001

// The attributes
typedef enum
  {								// Type     : RW : Description
								// ---------------------------------
	hsCardAttrRemovable,		// UInt8	: R  : true if card is removable
	hsCardAttrHwInstalled,		// UInt8	: R  : true if card hardware is installed
	hsCardAttrSwInstalled,		// UInt8	: R  : true if OS knows about card

	hsCardAttrCsBase,			// UInt32	: R  : address of first chip select
	hsCardAttrCsSize,			// UInt32	: RW : size of chip selects
	hsCardAttrAccessTime,		// UInt32	: RW : required access time

	hsCardAttrReset,			// UInt8	: RW : if true, assert reset to card
	hsCardAttrIntEnable,		// UInt8	: RW : if true, enable card interrupt

	hsCardAttrCardParam,		// UInt32	: RW : parameter passed to int handler,
								//					power handler, and event handler

	hsCardAttrIntHandler,		// HsCardIntHandlerPtr  
								//			: RW : card interrupt handler

	hsCardAttrPwrHandler,		// HsCardPwrHandlerPtr
								//			: RW : card power handler

	hsCardAttrEvtHandler,		// HsCardEvtHandlerPtr
								//			: RW : card event handler

	hsCardAttrLogicalBase,		// UInt32	: R  : logical base address of card
	hsCardAttrLogicalSize,		// UInt32	: R  : total reserved address space for card
	hsCardAttrHdrOffset,		// UInt32	: R  : offset from halCardAttrLogicalBase to
								//					card header

	hsCardAttrSetupInstalled,	// UInt8	: R  : true if card setup app is installed


	// HsExtensions 3.6+ only
	hsCardAttrBatteryStatusCallback,  // HsCardChargeStatusCallbackPtr
									  //	: RW : card battery status callback; se
									  //			to use the the builtin-charge LED
									  //			(if present) to indicate the status
									  //			of module battery while charging.

	// TODO: What version??
	// HsExtensions x.x+ only
	hsCardAttrSlotInfo,			// UInt32   : R  : Bitfield indicating what expansion
								//                  is available (e.g. Springboard)

	// Leave this one at end!!!
	hsCardAttrLast

  } HsCardAttrEnum;




// ------------------------------------------------------------------------------
// Equates for the HsPrefGet/Set calls
// ------------------------------------------------------------------------------

// The prefs
typedef enum
  {								  // Type : Description
								  // ---------------------------------

	// The following are used by SysLibFind() to resolve a virtual library
	//  name to an actual one.  They are also used directly by SrmOpen()
	//	to figure out whether to override a new-style driver w/ an old-style
	//	driver.
	hsPrefSerialLibDef,			  // Char[] : Name of serial library
								  //    to substitute for hsLibAliasDefault
	hsPrefSerialLibHotSyncLocal,  // Char[] : Name of serial library
								  //    to substitute for hsLibAliasHotSyncLocal
	hsPrefSerialLibHotSyncModem,  // Char[] : Name of serial library
								  //    to substitute for hsLibAliasHotSyncModem
	hsPrefSerialLibIrda,		  // Char[] : Name of serial library
								  //    to substitute for hsLibAliasIrda
	hsPrefSerialLibConsole,		  // Char[] : Name of serial library
								  //    to substitute for hsLibAliasConsole

	// The following are used in SysHandleEvent() to launch apps in response
	//  to associated virtual characters.
	hsPrefJogScanCharAppCreator,	  // UInt32 : Creator type of app to launch
									  //		  in response to hsChrJogScan
	hsPrefLidStatusChgCharAppCreator, // UInt32 : Creator type of app to launch
									  //		  in response to hsChrLidStatusChg
    hsPrefOptHard1CharAppCreator,	  // UInt32 : Creator type of app to launch
									  //		  in response to hsChrOptHard1
    hsPrefOptHard2CharAppCreator,	  // UInt32 : Creator type of app to launch
									  //		  in response to hsChrOptHard2
    hsPrefOptHard3CharAppCreator,	  // UInt32 : Creator type of app to launch
									  //		  in response to hsChrOptHard3
    hsPrefOptHard4CharAppCreator,	  // UInt32 : Creator type of app to launch
									  //		  in response to hsChrOptHard4

	// NOTE: If you add more app mapping prefs, make sure they don't have the
	// same enum value as the app mappings in the SystemPreferencesChoice enum,
	// otherwise we'll have to fix HsPrvFindAppFromIDs() to deal with it correctly.

	// The following are used to set Key Manager preferences
	hsPrefTempShiftTimeOut,			  // UInt32 : Number of ticks until a temporary
									  //	shift state (upper shift or option shift) 
									  //	times out.

	hsPrefLidClosedAutoOffSeconds,	  // UInt16 : Number of seconds for auto-off when
									  //	the lid is closed

	hsPrefLCDRefreshRate,			  // UInt16 : Type of refresh rate for lighting
									  //	see HsLCDRefreshRateEnum

	// Leave this one at end!!!
	hsPrefLast

  } HsPrefEnum;


// ------------------------------------------------------------------------------
// Equates for the HsCardPower call
// ------------------------------------------------------------------------------

// The commands
typedef enum
  {									  // IN/OUT : Description
									  // ---------------------------------
									  
	hsCardPowerCmdBatteryCapacity,	  // OUT : Capacity of battery in milliwatts
	hsCardPowerCmdBatteryPercent,	  // OUT : Current percent level of battery 
									  //		(0 -> 100)
	hsCardPowerCmdBatteryVoltage,	  // OUT : Battery voltage in millivolts
	hsCardPowerCmdTotalLoad,		  // OUT : Total load on batteries in milliwatts
	hsCardPowerCmdCardLoad,			  // OUT : load on batteries from Springboard
	hsCardPowerCmdAvailablePower,	  // OUT : Additional available milliwatts for 
									  //		Springboard
	hsCardPowerCmdAddLoad,			  // IN  : Add *valueP milliwatts of load to 
									  //		Springboard
	hsCardPowerCmdRemoveLoad,		  // IN  : Remove *valueP milliwatts of load 
									  //		from Springboard
	hsCardPowerCmdCurrentMaxVcc,	  // OUT : max sustained current from Vcc 
									  //		in milliamps
	hsCardPowerCmdCurrentMaxVBat,	  // OUT : max sustained current from VBat 
									  //		in milliamps
	hsCardPowerCmdVccMin,			  // OUT : min Vcc voltage in millivolts
	hsCardPowerCmdVccMax,			  // OUT : max Vcc voltage in millivolts
	hsCardPowerCmdVccNominal,		  // OUT : nominal Vcc voltage in millivolts
	hsCardPowerCmdVBatMin,			  // OUT : min VBat voltage in millivolts
	hsCardPowerCmdVBatMax,			  // OUT : max VBat voltage in millivolts
	hsCardPowerCmdVBatNominal,		  // OUT : nominal VBat voltage in millivolts


	// Leave this one at end!!!
	hsCardPowerCmdLast

  } HsCardPowerCmdEnum;

// -------------------------------------------------------------------------------
// Equates for the hsPrefLCDRefreshRate preference
// -------------------------------------------------------------------------------

typedef enum 
  {
	hsLCDRefreshFor60Hz,	  //optimal refresh rate for 60Hz lighting
	hsLCDRefreshFor50Hz		  //optimal refresh rate for 50Hz lighting
  } HsLCDRefreshRateEnum;


// ------------------------------------------------------------------------------
// Prototype of the App Event Handler that can be setup using
//  HsAppEventHandlerSet() and triggered using HsAppEventPost(). The
//  evtRefCon is a copy of evtRefCon passed to HsAppEventHandlerSet()
//
// This routine should not rely on globals since it may be called
//  while in the context of another app's action code. 
// ------------------------------------------------------------------------------
typedef Boolean (*HsAppEvtHandlerPtr) (UInt32 evtRefcon, UInt16 evtNum, 
					UInt16 evtParam);



// ------------------------------------------------------------------------------
// Notification Manager equates and structures. 
// ------------------------------------------------------------------------------

// This Notification is sent out using SysNotifyBroadcast() whenever a 
//  card is inserted or removed. The notifyDetailsP field of the event
//  contains a pointer to a HsNotifyCardChangeType that indicates
//  if the card was inserted or removed. 
// The inserted event is sent AFTER the card setup app runs but before we
//  launch the welcome app
// The removed event is sent out BEFORE the card setup app runs
#define	  hsNotifyCardChangeEvent	hsFileCHandspringExt

typedef struct
  {
	UInt8	inserted;				// true if card  inserted, false if card removed
	UInt8	reserved1;				// reserved, set to 0
	UInt16	cardNo;					// card number of card that was inserted/removed
	UInt32	reserved2;				// reserved, set to 0
  }
HsNotifyCardChangeType;
	


// hsNotifyBackgroundNetCloseEvent:
//
// The Handspring Extensions sends this broadcast when it is necessary to get
// background tasks that may be using NetLib to close NetLib via NetLibClose.
// Only apps that use the PalmOS Network Library from a background task should
// register for this notification.  This broadcast will be repeated periodically
// until the closePending field of HsNotifyNetCloseType remains at zero (false)
// upon completion of the broadcast, or an internal timeout occurs.
//
// The broadcaster initializes the closePending field to zero at
// the beginning of each broadcast.  The handler should set this field to true
// if it has initiated the action that will cause its background task to call
// NetLibClose shortly, but it hasn't completed yet.  Otherwise, the handler
// shold NOT modify this field.
//
// ***IMPORTANT*** Because Notification Manager reserves the Memory Manager semaphore
//  before delivering the broadcast, the handler of this broadcast MUST NOT block
//  as that will cause a deadlock to occur with the Network Library's background task
//  or some other task.  Instead, the handler should set an "abort" flag that the
//  app's background task checks frequently, and return immediately from the
//  notification.  When the background task detects that this flag has been set, it should
//  immediately perform whatever cleanup is necessary and close Network Library (if it
//  had it open), and set an "NetLib closed" flag, that will inform the handler during
//  a subsequent invocation that it should not modify the closePending field.
//
//  For testing purposes, one of the instances when this broadcast may be invoked,
//  is when there is a live data connection and the user switches to the phone app
//  and/or attempts to make a phone call.
//
#define	  hsNotifyBackgroundNetCloseEvent	'HsNc'	  // registered on 11/07/2001

typedef struct
  {
	UInt8	closePending;			// the broadcaster initializes this to zero at
									//  the beginning of each broadcast.
									//  the handler should set this field to true
									//  if it has initiated the action that will cause
									//  its background task to call NetLibClose shortly,
									//  but it hasn't completed yet.  Otherwise, the handler
									//  shold NOT modify this field.  The broadcaster will
									//  periodicaly repeat this broadcast until closePending
									//  remains at zero upon completion of the broadcast.
	UInt8	reserved1;				// reserved, the broadcaster will set to 0
	UInt32	reserved2;				// reserved, the broadcaster will set to 0
  }
HsNotifyNetCloseType;


// Extended reason codes for sysNotifySleepRequestEvent
//  (queried with HsAttrGet(hsAttrSysSleepReason))
#define hsSysSleepLid	0xE000	  // Extension of sysSleepPowerButton 
								  //  meaning that the user closed the lid

// For reference, Palm's values for the sleep-reason for the sysNotifySleepEvent:
//#define sysSleepPowerButton		0
//#define sysSleepAutoOff			1
//#define sysSleepResumed			2
//#define sysSleepUnknown			3


// ------------------------------------------------------------------------------
// Equates for HsPeriodic calls
// ------------------------------------------------------------------------------

// Called periodically as a result of registering w/ HsPeriodicRegister()
//   param:     Param that was passed in during registration
//   sysAwakeP: Behaves like sysAwakeP on card interrupts
typedef void	(*HsPeriodicHandlerPtr) (UInt32 param, Boolean* sysAwakeP);



// ------------------------------------------------------------------------------
// Equates for HsAttr calls
// ------------------------------------------------------------------------------
typedef enum 
  {						  // Type    : Description
	hsAttrLidOpen,		  // UInt16  : false=closed, true=open
	hsAttrSysSleepReason, // UInt16  : Same as the SleepEventParamType.reason
						  //		   from the last sysNotifySleepRequestEvent
						  //		   unless the real reason is different
						  //		   (e.g. hsSysSleepLid)
	hsAttrDisplayOn,	  // UInt16	 : true= lid open, false=lid closed
						  //		   (returns hsErrNotSupported if no lid)
	hsAttrRingSwitch,	  // UInt16	 : State of ring switch
	hsAttrPostProcessRsc, // UInt32	 : Pointer to resource containing the post-
						  //		   processing lists
	hsAttrSysActive,	  // UInt16	 : Bits representing various system activity
	hsAttrActiveCalls,	  // UInt16  : Nunber of active voice calls
	hsAttrPhoneType,      // UInt32   : CDMA or GSM phone
	hsAttrE911Mode		  // Boolean  : true = phone is in 911 mode	
	
  } HsAttrEnum;


// hsAttrRingSwitch
//  For now there are only two values, but it's left open to have a control
//  that works more like a volume control and has a range of settings. The
//  value of the "loud" setting was chosen so that it can be decided later
//  to make this a bit field or a scalar value.
#define hsAttrRingSwitchMute		  0
#define hsAttrRingSwitchLoud		  0x8000

// hsAttrSysActive
#define hsAttrSysActiveHotSync	  0x0001  // HotSync is in progress
#define hsAttrSysActiveProgress	  0x0002  // Progress dialog is up (e.g. beaming)

#define hsAttrPhoneTypeCDMA       0x0001 // CDMA Phone
#define hsAttrPhoneTypeGSM        0x0002 // GSM Phone

// ------------------------------------------------------------------------------
// Word correction equates
// ------------------------------------------------------------------------------
#define hsWordCorrectMaxWordLen		25  // Max length (in bytes) of a word 
										//	(corrected or or not) in the
										//	word correction dictionary.  With
										//	null, word can be 26 bytes.


// ------------------------------------------------------------------
// Equates for supporting system handled status gadgets
// ------------------------------------------------------------------
typedef enum
  {
	hsStatusGadgetBattery = 1,
	hsStatusGadgetSignal = 2			// Status gadget is a battery meter
  } HsStatusGadgetTypeEnum;				// Status gadget is a signal strength indicator


// ------------------------------------------------------------------
// Equates for determining if a key event has a meta-meaning
// ------------------------------------------------------------------
typedef enum
  {
	hsEvtPageKeyIsJog = 1
  } HsEvtPageKeyEnum;


// --------------------------------------------------------------	
// Indicator control (LEDs, Vibrator, etc.)
// --------------------------------------------------------------

// These are the commands passed to HsIndicator
typedef enum
  {								  // (param type)
	// Commands echoed in HsExtensions
	hsIndicatorCmdSetSequence,	  // (HalIndicatorSequence[]) : Must be locked and remain locked
								  //  for the duration the indicator is enabled. Also must have
								  //  halIndicatorCapLast in the final element
	hsIndicatorCmdEnable,		  // (UInt32) : How many times to run the sequence 
								  //  0 : disable, 1-254 : count, >=255 : infinite
	hsIndicatorCmdGetCaps,		  // (HsIndicatorCapType**) : Array of capabilities
	hsIndicatorCmdTestSequence,	  // (HalIndicatorCmdParamTestSequence*)
	hsIndicatorCmdGetSequence,	  // (const HalIndicatorSequence**) : Retrieve the currently set
								  //  sequence.
	hsIndicatorCmdGetRepeats	  // (UInt32*) : Retrieve how many repeats are left until
								  //  the sequence stops. Includes the current repeat, so if
								  //  the result is 0 it means the sequence is not running.

  }
HsIndicatorCmdEnum;

// To ensure all compilers use the same size for the enum...
typedef UInt16 HsIndicatorCmdType;

// These are the possibile indicator capabilities
typedef enum
  {
	hsIndicatorCapOff,
	hsIndicatorCapVibrator,
	hsIndicatorCapLEDRed,
	hsIndicatorCapLEDGreen,

	hsIndicatorCapCount,
	hsIndicatorCapLast = 0xFFFF
  }
HsIndicatorCapEnum;

// To ensure all compilers use the same size for the enum...
typedef UInt16 HsIndicatorCapType;

// This is one element in the array which composes a sequence
typedef struct
  {
	HsIndicatorCapType	cap;	  // HalIndicatorCapEnum
	UInt16				delay;	  // mibiseconds (1/1024ths of a second)
  }
HsIndicatorSequence;

#define hsIndicatorNone			0xFFFF	// indicator index of "no indicator"

#define hsIndicatorRepeatForever	0xFFFF	// repeat count of "forever"

#define hsIndicatorDelayOneSecond	0x400
#define hsIndicatorDelayOneMinute	(60*hsIndicatorDelayOneSecond)  // 0xF000
#define hsIndicatorDelayForever		0

// This is type pointed to as the parameter for hsIndicatorCmdTestSequence
typedef struct
  {
	const HsIndicatorSequence*  seq;	// IN :	Sequence to test
	UInt16						index;	// OUT:	Index of indicator which can 
										//		play the sequence (search starts
										//		with index parameter)
  }
HsIndicatorCmdParamTestSequence;

	
// --------------------------------------------------------------	
// Light Manager
// --------------------------------------------------------------

// These are different modes that can be used to affect the light manager,
// which is in charge of temporary changes in the lighting situation.
typedef enum
  {
	// Preset modes
	// ------------
	// These modes have saved values associated with them...
	hsLightModeNormal,		// 0000. Light mode is at normal presets
	hsLightModeNight,		// 0001. Light mode is at night-mode presets


	hsLightModeNumPresets,	// Num presets.  SUBJECT TO CHANGE.


							// Reserved mode to force compiler to use 16-bit for enum...
	hsLightModeReserved = 0x7FFF	
  }
HsLightModeEnum;


// These circumstances modify the mode and allow us to handle 
// different logical circumstances on devices with different hardware.
//
// - These are applied in the following order:
//   1. Alert woke device
//	 2. Quick typing
//	 3. User not looking
typedef enum
  {
	hsLightCircumstanceAlertWokeDevice, // 0000. An important alert woke the
										//	     device up from sleep.
	hsLightCircumstanceQuickTyping,		// 0001. We want the user to be able
										//	     to access the keyboard easily
	hsLightCircumstanceUserNotLooking,	// 0002. We suspect the user isn't
										//	     looking at the device if they're
										//		 not actively using the device, so 
										//		 we'll dim the screen (but we don't
										//	     actually want to turn it off).

	hsLightCircumstanceNumCircumstances,// Num circumstances.  SUBJECT TO CHANGE.


							// Reserved circumstance to force compiler to use 16-bit for enum...
	hsLightCircumstanceReserved = 0x7FFF
  }
HsLightCircumstanceEnum;

#define hsLightCircumstanceFirst hsLightCircumstanceAlertWokeDevice

// ------------------------------------------------------------------
// Prototype for the callback of our extended pop-up list function
//	(HsLstPopupListExt).  The listP param is actually a ListType*.
//
// HsPopupListCallback (const EventType* eventP, const ListType* listP,
//	Boolean* dimissP)
//
// PARAMETERS:
//	eventP	  IN  ptr to event to be handled
//	listP	  IN  ptr to popup list
//	dismissP  OUT whether the list should be dismissed after the
//				  callback
//
// RETURNED:
//	Whether the event was handled or not.
//
// IMPORTANT:
//	The callback function should only use the ListMgr's API to
//	manipulate the lists (i.e., LstSetSelection, LstGetSelection,
//	LstScrollList).  The structures pointed to by eventP and listP
//	CANNOT be changed.  To have the popup list dismissed after the
//	callback, set *dismissP to true inside the callback.  If the list
//	should be dismissed without selecting an item, be sure to make the
//	following call:
//
//	  LstSetSelection (listP, noListSelection);
//
//-------------------------------------------------------------------
typedef Boolean	(*HsPopupListCallbackPtr) (EventType* eventP, 
										   void* listP,
										   Boolean* dismissP);


//=============================================================================
// Handspring selectors for the Handspring system trap 
// 
// NOTE: If you add traps here, you must:
//
//	  1.) Add a prototype to the Prototypes section of this header or
//		  to the prototype section of HsExtPrv.h if it's  private call
// 
//	  2.) Modify the _PrvHsSelector() routine in HsExtensions.c to recognize
//		  and dispatch to the new call
//
// IMPORTANT: If you change any of these trap numbers, be sure to
//  update any *Patches.txt patch files that use the trap number to patch
//  the object code. 
// 
//=============================================================================
// This is the trap number we use for the Hs trap calls
// IMPORTANT: If this changes, you must manually update the any object code
//  patches files. PalmVSystemboot10001Patches.txt is at least one that
//  patches this trap in .
#define	sysTrapHsSelector			  sysTrapOEMDispatch

#define	hsSelectorBase	  0

#define hsSelInfo 						  0x0

#define hsSelPrvInit 					  0x1
#define hsSelPrvCallSafely 				  0x2
#define hsSelPrvCallSafelyNewStack 		  0x3

#define hsSelDatabaseCopy 				  0x4
#define hsSelExtKeyboardEnable 			  0x5
#define hsSelCardAttrGet 				  0x6
#define hsSelCardAttrSet 				  0x7
#define hsSelCardEventPost 				  0x8
#define hsSelPrvErrCatchListP 			  0x9

#define hsSelPrefGet 					  0xA
#define hsSelPrefSet 					  0xB

#define hsSelDmGetNextDBByTypeCreator	  0xC
#define hsSelDmGetNextDBInit 			  0xD

#define hsSelCardHdrUpdate 				  0xE

#define hsSelAppEventHandlerSet 		  0xF
#define hsSelAppEventPost 				  0x10

#define hsSelUsbCommStatePtr 			  0x11

#define hsSelCardPatchInstall 			  0x12
#define hsSelCardPatchRemove 			  0x13

#define hsSelEvtResetAutoOffTimer 		  0x14

#define hsSelDmDatabaseUniqueIDSeed		  0x15

#define hsSelAboutHandspringApp 		  0x16

#define hsSelDmDatabaseIsOpen 			  0x17
#define hsSelDmDatabaseIsProtected 		  0x18

#define hsSelDlkForceSlowSync 			  0x19

#define hsSelPrvHandleCardChangeEvent	  0x1A

// 0x03523000

#define hsSelCardPower 					  0x1B

#define	hsSelDmDatabaseDeleted			  0x1C

#define	hsSelDmLockFileSystem			  0x1D

#define	hsSelPrvLaunchCompareFunc		  0x1E

// 0x03523020

#define hsSelPrvLEDCommand				  0x1F

#define hsInstallSerialWrapper			  0x20

#define hsUninstallSerialWrapper		  0x21

#define hsPeriodicRegister				  0x22
#define hsPeriodicUnregister			  0x23
#define hsPeriodicPeriodsAvailable		  0x24

// 0x03523030

// No HsExtension calls added

// 0x03523040

// NOTE: Be sure to update 68000Instrs.h if these change
#define hsSelPrvSetButtonDefault		  0x25
#define hsSelPrvFindAppFromIDs			  0x26

#define	hsSelAttrGet					  0x27
#define	hsSelAttrSet					  0x28
#define hsSelKeyCurrentStateExt			  0x29
#define hsSelKeySetMaskExt				  0x2A
#define hsSelGrfSetStateExt				  0x2B
#define hsSelGrfGetStateExt				  0x2C
#define hsSelKeyChrToOptChr				  0x2D
#define hsSelKeyChrToRegChr				  0x2E
#define hsSelPrvKeyHandleEvent			  0x2F
#define hsSelWordCorrectInvoke			  0x30
#define hsSelWordCorrectUndo			  0x31
#define hsSelPrvUpdateBatteryGadget		  0x32
#define hsSelPrvUpdateSignalGadget		  0x33
#define hsSelStatusSetGadgetType		  0x34
#define hsSelSysKeyboardReferenceDialog	  0x35
#define hsSelPutObjectAfterTitle		  0x36
#define hsSelEvtMetaEvent				  0x37
#define hsSelStatusUpdateGadgets		  0x38
#define hsSelLstPopupListExt			  0x39
#define hsSelDmCreateDatabasesFromImages  0x3A	  // DO NOT CHANGE used in object patch!
#define hsSelPrvLaunchDemoApp			  0x3B	  // DO NOT CHANGE used in object patch!
#define hsSelPrefGetAppKeyCreator		  0x3C
#define hsSelUtilFrmDoDialogWithCallback  0x3D

#define hsSelHsGetTrapAddress			  0x3E
#define hsSelHsSetTrapAddress			  0x3F
#define hsSelUnimplemented				  0x40

#define hsSelIndicator					  0x41

#define hsSelUtilSclUpdateScrollBar		  0x42
#define hsSelUtilSclScroll				  0x43
#define hsSelUtilSclScrollPage			  0x44

#define hsSelPrvInitPostProcess			  0x45
#define hsSelPostProcPopupList			  0x46

#define hsSelPeriodicUnregisterFromSelf	  0x47
#define hsSelNetworkDropConnection		  0x48

// 0x03523050

#define hsSelPrvSetupGoToCustomForm			  0x49	  // DO NOT CHANGE used in object patch!
#define hsSelPrvSetupInstallFormEventHandler  0x4A	  // DO NOT CHANGE used in object patch!

#define hsSelUtilSliHandleKeyDown		  0x4B
#define hsSelUtilWinGetMaxDepth			  0x4C

#define hsSelLightMode					  0x4D
#define hsSelLightCircumstance			  0x4E

#define hsSelUtilBbutInstallFromResource  0x4F
#define hsSelUtilBbutHitBigButton		  0x50
#define hsSelUtilFrmDoDialog			  0x51
#define hsSelUtilStrCSpn				  0x52

// 0x03523055

#define hsSelUtilFrmDoTimedDialog         0x53

#define hsSelPreventRadioPowerOff         0x54
#define hsSelPrvDisplaySecurityAboutBox   0x55  // DO NOT CHANGE used in object patch!


// NOTE: Please use up reserved spaces during
// 3.5H5 development; this will reduce merge
// issues...

#define hsSelReserved352H5_54			  0x54
#define hsSelReserved352H5_55			  0x55
#define hsSelReserved352H5_56			  0x56
#define hsSelReserved352H5_57			  0x57
#define hsSelReserved352H5_58			  0x58
#define hsSelReserved352H5_59			  0x59
#define hsSelReserved352H5_5A			  0x5A
#define hsSelReserved352H5_5B			  0x5B
#define hsSelReserved352H5_5C			  0x5C
#define hsSelReserved352H5_5D			  0x5D

// 0x03523060

#define hsSelSelectCurrentTime			  0x5E  // DO NOT CHANGE used in object patch!
#define hsSelIsNetworkTimePrefSelected    0x5F

// NOTE: Please use up reserved spaces during
// 3.5H6 development; this will reduce merge
// issues...

#define hsSelReserved352H6_60			  0x60
#define hsSelReserved352H6_61			  0x61
#define hsSelReserved352H6_62			  0x62
#define hsSelReserved352H6_63			  0x63
#define hsSelReserved352H6_64			  0x64
#define hsSelReserved352H6_65			  0x65
#define hsSelReserved352H6_66			  0x66
#define hsSelReserved352H6_67			  0x67
#define hsSelReserved352H6_68			  0x68
#define hsSelReserved352H6_69			  0x69
#define hsSelReserved352H6_6A			  0x6A

// WARNING!  Leave this one at the end! 
// When adding new traps, renumber it
// to one greater than the last trap.
#define hsSelLast  						  0x6B

#define	hsNumSels	 (hsSelLast - hsSelectorBase)



// <chg 06-Apr-2000 BP> added support for gcc 2.95 and new callseq
#if (!(defined NO_HSEXT_TRAPS)) && (defined __GNUC__) && (EMULATION_LEVEL == EMULATION_NONE)
  #if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)

	#ifndef _Str
	#define _Str(X)  #X
	#endif

	#define _OS_CALL_WITH_UNPOPPED_16BIT_SELECTOR(table, vector, selector)	\
		__attribute__ ((__callseq__ (										\
			"move.w #" _Str(selector) ",-(%%sp); "							\
			"trap #" _Str(table) "; dc.w " _Str(vector))))
	
	#define SYS_SEL_TRAP(trapNum, selector) \
		_OS_CALL_WITH_UNPOPPED_16BIT_SELECTOR(sysDispatchTrapNum, trapNum, selector)	
	    

  #else // GNUC < 2.95

	#define SYS_SEL_TRAP(trapNum, selector) \
	    __attribute__ ((inline (0x3f3c, selector, m68kTrapInstr+sysDispatchTrapNum,trapNum)))

  #endif // GNUC < 2.95

#elif (!(defined NO_HSEXT_TRAPS)) && (defined (__MWERKS__))

	#define SYS_SEL_TRAP(trapNum, selector) \
		= {0x3f3c, selector, m68kTrapInstr+sysDispatchTrapNum,trapNum}

#else
  	#define SYS_SEL_TRAP(trapNum, selector) 


#endif	




//=============================================================================
// HsCardErrTry / Catch / support
//
// ---------------------------------------------------------------------
// 
//  Typical Use:
//		volatile UInt16	  x;
//      x = 0;
//		HsCardErrTry 
//        {
//          // access card in some manner that may fail
//          value = cardBaseP[0]
//  
//          // do other stuff too
//			x = 1;						// Set local variable in Try
// 
//          // access card again
//          value = cardBaseP[1]  
//
//		  }
//
//		HsCardErrCatch 
//        {
//          // Recover or cleanup after a failure in above Try block
//          // The code in this block does NOT execute if the above
//          //  try block completes without a card removal
//			if (x > 1) 
//			  SysBeep(1);
// 
//		  } HsCardErrEnd
//   
//
// ***IMPORTANT***
//
//	DO NOT, under any circumstances, "return" or "goto" out of
//  the HsCardErrTry or HsCardErrCatch blocks.  If your code leaves
//  the Try block prematurely, it will corrupt the try/catch
//  exception list, and very bad things will happen (the problem
//  will typically manifest itself at a later time, making it
//  very difficult to debug).
//
//  See also comments about volatile variables next to the definition
//  of HsCardErrCatch.
//
// ***IMPORTANT***
//   
//=============================================================================
	
// Try & Catch macros
#define	HsCardErrTry											\
	{															\
	  ErrExceptionType	_tryObject;								\
	  MemPtr*	volatile _listP;								\
	  _listP = HsPrvErrCatchListP();							\
	  _tryObject.err = 0;										\
	  _tryObject.nextP = (ErrExceptionPtr)*_listP;				\
	  *_listP = (MemPtr)&_tryObject;							\
	  if (ErrSetJump(_tryObject.state) == 0)                    \
	    {

		
// NOTE: All variables referenced in and after the ErrCatch must 
// be declared volatile.  Here's how for variables and pointers:
//	volatile UInt16			  oldMode;
//	ShlDBHdrTablePtr volatile hdrTabP = nil;
// 
// If you have many local variables after the ErrCatch you may
// opt to put the ErrTry and ErrCatch in a separate enclosing function.
#define	HsCardErrCatch											\
		  *_listP = (MemPtr)_tryObject.nextP;					\
		} 														\
	  else														\
	    {														\
		  *_listP = (MemPtr)_tryObject.nextP;
			
			
#define	HsCardErrEnd											\
		}														\
	}




//=============================================================================
// Prototypes
//=============================================================================
UInt32		HsInfo (UInt16 item, UInt16 paramSize,  void* paramP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelInfo);


Err			HsDatabaseCopy (UInt16 srcCardNo, LocalID srcDbID, UInt16 dstCardNo,
							char* dstNameP, UInt32 hsDbCopyFlags, char* tmpNameP,
							LocalID* dstDbIDP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDatabaseCopy);

Err			HsExtKeyboardEnable (Boolean enable)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelExtKeyboardEnable);


Err			HsCardAttrGet (UInt16 cardNo, UInt16 /*HsCardAttrEnum*/ attr,
						   void* valueP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardAttrGet);

Err			HsCardAttrSet (UInt16 cardNo, UInt16 /*HsCardAttrEnum*/ attr,
						   void* valueP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardAttrSet);

Err			HsCardEventPost (UInt16 cardNo, UInt16 evtNum, UInt16 evtParam)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardEventPost);


MemPtr*		HsPrvErrCatchListP (void)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelPrvErrCatchListP);


Err			HsPrefGet (UInt16 /*HsPrefEnum*/ pref, void* bufP,
					UInt32* prefSizeP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelPrefGet);

Err			HsPrefSet (UInt16 /*HsPrefEnum*/ pref, void* bufP,
					UInt32 prefSize)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelPrefSet);


			// This call behaves the same as the PalmOS DmGetNextDatabaseByTypeCreator
			//   but can also be used in conjunction with the HsGetNextDBInit()
			//   call to start the search at a particular card number or limit
			//   the search to one card. 
Err			HsDmGetNextDBByTypeCreator (Boolean newSearch, 
					DmSearchStatePtr stateInfoP, UInt32	type, UInt32 creator, 
					Boolean onlyLatestVers, UInt16* cardNoP, LocalID* dbIDP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmGetNextDBByTypeCreator);


			// Can be used to init the stateInfo for HsDmGetNextDBByTypeCreator()
			//  so that it starts at a particular card number or limits the
			//  search to one card. The flags are 1 or more of hsDmGetNextDBFlagXXX
Err			HsDmGetNextDBInit (DmSearchStatePtr stateInfoP, UInt32 flags,
					UInt16 cardNo)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmGetNextDBInit);



			// This is an advanced call for use by card flash utilities that
			// change which card header to use AFTER the card is installed.
Err			HsCardHdrUpdate (UInt16 cardNo, void* newCardHdrP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardHdrUpdate);



			// Register an app event handler that can be triggered using
			//  HsAppEventPost(). The evtRefCon will be passed to the
			//  event handler when it is called. 
			// The event Handler should not rely on global variables since it may 
			//  be called while in the context of another app's action code. 
			//  Instead, pass in a pointer in evtRefCon to globals. 
Err			HsAppEventHandlerSet (HsAppEvtHandlerPtr procP, UInt32 evtRefCon)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelAppEventHandlerSet);


			// Post an event to be processed by the AppEventHandler procedure
			//  registered with HsAppEventHandlerSet(). The evtNum param
			//  can be from 0 to hsMaxAppEvent. 
Err			HsAppEventPost (UInt16 evtNum, UInt16 evtParam)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelAppEventPost);



			// Returns a pointer to a 4 byte area of global
			//	  memory that can be shared between the debugger stub's 
			//	  communication support and run-time communication support.
			// This can be used by USB for example to store the enumeration state.
			// This is an exported stub into the HAL layer routine which actually
			//    does the real work. 
UInt32*		HsUsbCommStatePtr (void)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelUsbCommStatePtr);


			// Patch a system trap for a card. This call should be used instead of
			//  SysSetTrapAddress() by card setup utilities because it will 
			//  insure compatibility with HackMaster and any HackMaster 
			//  hacks that are installed. 
			// The implementation of the patch should use HsCardPatchPrevProc() 
			//   to get the address of the old routine to chain to:
			//		HsCardPatchPrevProc (&oldProcP);
			//		(*oldProcP)(); 
			// IMPORTANT: Setup utilities are only allowed to install *ONE*
			//  patch for each trapNum!
Err			HsCardPatchInstall (UInt16 trapNum, void* procP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardPatchInstall);

			// Remove a patch of a system trap installed using HsSysPatchInstall().
			// The 'creator' and 'id' must the same as passed to HsSysPatchInstall().
Err			HsCardPatchRemove (UInt16 trapNum)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardPatchRemove);

			// Macro to get the old routine address of a trap patched using
			//  HsCardPatchInstall()
#define		HsCardPatchPrevProc(trapNum,oldProcPP)					  \
			  FtrGet (hsFileCCardSetup, trapNum, (UInt32*)oldProcPP)


			// Called from interrupt routines to reset the auto-off timer
			// and tell the system whether or not the user hardware (i.e. LCD)
			// needs to be woken up or not. Note: The granularity of the
			// stayAwakeTicks is only about 5*sysTicksPerSecond. 
Err			HsEvtResetAutoOffTimer (Int32 stayAwakeTicks, Boolean userOn)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelEvtResetAutoOffTimer);



			// Call to get or set the Unique ID seed field of a database
Err			HsDmDatabaseUniqueIDSeed (UInt16 cardNo, LocalID dbID, 
				  Boolean set, UInt32* uniqueIDSeed)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmDatabaseUniqueIDSeed);

			// <chg 29-Jun-99 dia> Added extra credits string parameter.
void		HsAboutHandspringApp (UInt16 appCardNo, LocalID appDbId,
				  Char* copyrightYearStrP, Char* extraCreditsStrP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelAboutHandspringApp);

// <chg 30-Jun-99 dia> Defined macros to make about box easier to call.
#define HsAboutHandspringAppWithYearId(yearId)								\
	do																		\
	  {																		\
		UInt16	appCardNo;													\
		LocalID	appDbId;													\
		MemHandle	yearStrH;												\
		Char* 	yearStrP;													\
																			\
		yearStrH = DmGetResource (strRsc, yearId);							\
		yearStrP = MemHandleLock (yearStrH);								\
		SysCurAppDatabase(&appCardNo, &appDbId);							\
		HsAboutHandspringApp (appCardNo, appDbId, yearStrP, NULL);			\
		MemPtrUnlock (yearStrP);											\
		DmReleaseResource (yearStrH);										\
	  }																		\
	while (0)
		
#define HsAboutHandspringAppWithYearCredId(yearId, creditsId)				\
	do																		\
	  {																		\
		UInt16	appCardNo;													\
		LocalID	appDbId;													\
		MemHandle	yearStrH, extraStrH;									\
		Char* 	yearStrP;													\
		Char*	extraStrP;													\
																			\
		yearStrH = DmGetResource (strRsc, yearId);							\
		yearStrP = MemHandleLock (yearStrH);								\
		extraStrH = DmGetResource (strRsc, creditsId);						\
		extraStrP = MemHandleLock (extraStrH);								\
		SysCurAppDatabase(&appCardNo, &appDbId);							\
		HsAboutHandspringApp (appCardNo, appDbId, yearStrP, extraStrP);		\
		MemPtrUnlock (extraStrP);											\
		DmReleaseResource (extraStrH);										\
		MemPtrUnlock (yearStrP);											\
		DmReleaseResource (yearStrH);										\
	  }																		\
	while (0)


UInt8		HsDmDatabaseIsOpen (UInt16 cardNo, LocalID dbID)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmDatabaseIsOpen);

UInt8		HsDmDatabaseIsProtected (UInt16 cardNo, LocalID dbID)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmDatabaseIsProtected);


// =====================================================================
// The following API calls are only available for versions 0x02000000
//   and greater
// =====================================================================
Err			HsDlkForceSlowSync (Boolean clearLastPCIID, Boolean clearAllSyncPrefs, 
							Boolean* didAnythingP, UInt32* lastSyncDateP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDlkForceSlowSync);



// =====================================================================
// The following API calls are only available for versions 0x03500000
//   and greater
// =====================================================================

// Power Management information and Springboard load registration
Err			HsCardPower (UInt16 cardNo, UInt16 /*HsCardPowerCmdEnum*/ cmd, 
						  UInt32* valueP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardPower);

// Notify OS that a database on a read-only (i.e. Flash memory) card has been deleted
// This will clean up all Alarms, Notifies, etc. associated with the database
Err			HsDmDatabaseDeleted (UInt16 cardNo, LocalID dbID)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmDatabaseDeleted);


// Lock/Unlock the file system for reading or writing
Err			HsDmLockFileSystem (Boolean lock, Boolean writeAccess)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmLockFileSystem);



// =====================================================================
// The following API calls are only available for versions 0x03523020
//   and greater
// =====================================================================

// This function installs an old-style serial library as a wrapper for
// the given new-style serial driver.  The library can be accessed
// using SysLibFind() on the name returned from the function.  Note:
// only SysLibFind() of the name is guaranteed to work--an actual
// old-style serial library may or may not be created on the device.
Err			HsInstallSerialWrapper (UInt32 driverPortID, Char* outLibraryNameP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsInstallSerialWrapper);

// This function uninstalls the wrapper for the given portID.
// DOLATER: what is the behavior if the library is currently open?
// ...should this function return an error in the case?
Err			HsUninstallSerialWrapper (UInt32 driverPortID)
				SYS_SEL_TRAP (sysTrapHsSelector, hsUninstallSerialWrapper);


// Register a handler for periodic events...
Err			HsPeriodicRegister (HsPeriodicHandlerPtr procP, UInt32 param,
								Int32 periodNSecs)
				SYS_SEL_TRAP (sysTrapHsSelector, hsPeriodicRegister);

// Unregister a handler for periodic events...
Err			HsPeriodicUnregister (HsPeriodicHandlerPtr procP, Int32 periodNSecs)
				SYS_SEL_TRAP (sysTrapHsSelector, hsPeriodicUnregister);

// Get list of periods available for HalPeriodicRegister
Err			HsPeriodicPeriodsAvailable (UInt16* numPeriodsP, const Int32** periodsArrayP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsPeriodicPeriodsAvailable);

// =====================================================================
// The following API calls are only available for versions 0x03523040
//   and greater
// =====================================================================

Err			HsAttrGet (UInt16 /*HsAttrEnum*/ attr, UInt32 param, void* valueP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelAttrGet);

Err			HsAttrSet (UInt16 /*HsAttrEnum*/ attr, UInt32 param, void* valueP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelAttrSet);


// Extended version of KeyCurrentState() to allow detection of many more keys
//	being pressed (i.e., keys on Manhattan keyboard).
void		HsKeyCurrentStateExt (UInt32 keys[3])
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelKeyCurrentStateExt);

// Extended version of KeySetMask() to allow masking of many more keys
//	(i.e., keys on Manhattan keyboard).
void		HsKeySetMaskExt (UInt32 keyMaskNew[3], UInt32 keyMaskOld[3])
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelKeySetMaskExt);

// Extended version of GrfSetState() to allow setting of an option and option
//	lock state.
Err			HsGrfSetStateExt (Boolean capsLock, Boolean numLock, Boolean optLock, 
							  Boolean upperShift, Boolean optShift, Boolean autoShift)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelGrfSetStateExt);

// Extended version of GrfGetState() to allow an option and option lock state to
//	be represented.
Err			HsGrfGetStateExt (Boolean* capsLockP, Boolean* numLockP, Boolean* optLockP, 
							  UInt16* tempShiftP, Boolean* autoShiftedP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelGrfGetStateExt);

// Converts the code of a character into it's corresponding option character and
//  modifier.
Err		  HsKeyChrToOptChr (UInt16 chr, UInt16* optChrP, UInt16* optModifiersP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelKeyChrToOptChr);

// Converts the code of a character into it's corresponding regular character
//  and modifier.
Err		  HsKeyChrToRegChr (UInt16 chr, UInt16* regChrP, UInt16* regModifiersP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelKeyChrToRegChr);

// Displays a post processing list based on (and on top of) the character just
//	before the insertion point.  A selected item replaces the character.
Int16		HsPostProcPopupList (void)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelPostProcPopupList);

// TODO: reconsider why you made the 2 word correct functions public...
// If in a PalmOS field, invokes the word correction mechanism on the word
//	preceding the insertion point
void		HsWordCorrectInvoke (Char** origWordP, UInt8* spacesIntroducedP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelWordCorrectInvoke);

// If in a PalmOS field and a word correction was just performed, reverses the
//	correction.
void		HsWordCorrectUndo (Char* origWord, UInt8 spacesIntroduced)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelWordCorrectUndo);

// Tells the system that a gadget on the form is a certain type of status
//	gadget so that the system can properly draw and update it
Err			HsStatusSetGadgetType (void* frmP, UInt16 gadgetID, 
								   UInt16 /*HsStatusGadgetTypeEnum*/ type)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelStatusSetGadgetType);

// Pops up a help dialog for the keyboard.  Should be used in place of Graffiti
//	Help in the Edit menus of Manhattan apps.
void		HsSysKeyboardReferenceDialog (void)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelSysKeyboardReferenceDialog);


// Places a UI object to the right of a form's title
Boolean 
HsPutObjectAfterTitle (void* voidFrmP, UInt16 objID, UInt8 titleOffset)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelPutObjectAfterTitle);


// Determines if an event has a meta-meaning (i.e., a page-up key event was
//	generated by a jog-up key)
UInt16
HsEvtMetaEvent (EventPtr eventP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelEvtMetaEvent);


// Updates all the status gadgets on the current form
void
HsStatusUpdateGadgets (void)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelStatusUpdateGadgets);


// Allows a popup list to have added functionality (through a callback fcn)
Int16 HsLstPopupListExt (void* listP, 
						 HsPopupListCallbackPtr listCallbackP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelLstPopupListExt);

// Creates databases from resources of 'resType' in the resource database 'dbP'.
// The firstID is the ID of the first resource to start from.  Function will
// then iterate chronologically from that ID, and create databases from each
// resource.  Function will stop when it can't find a resource.
UInt16
HsDmCreateDatabasesFromImages (DmOpenRef dbP, DmResType resType, DmResID firstID)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmCreateDatabasesFromImages);

// Retrieve the creator ID of the application associated with a hard key.
Err
HsPrefGetAppKeyCreator (WChar chr, UInt16 modifiers, UInt32* creatorP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelPrefGetAppKeyCreator);


// Get the address of the given Handspring trap...
void *HsGetTrapAddress (UInt16 trapNum)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelHsGetTrapAddress);

// Patch the given Handspring trap...
Err HsSetTrapAddress (UInt16 trapNum, void *procP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelHsSetTrapAddress);

// Unimplemented trap...
void HsUnimplemented (void)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelUnimplemented);

// LED and vibrator control
Err HsIndicator(HsIndicatorCmdType cmd, UInt16 index, void* param)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelIndicator);

// Unregister a handler for periodic events from the event handler itself...
Err			HsPeriodicUnregisterFromSelf (HsPeriodicHandlerPtr procP, Int32 periodNSecs)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelPeriodicUnregisterFromSelf);

// Disconnects all open NetLib connections.  ***IMPORTANT*** This function MUST
//  be called only from the context of the UI task!
Err			HsNetworkDropConnection (void)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelNetworkDropConnection);

// Switch mode function
Err HsLightMode (Boolean set, HsLightModeEnum* modeP)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelLightMode);
// Set light circumstances
Err HsLightCircumstance (Boolean add, HsLightCircumstanceEnum circumstance)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelLightCircumstance);

void HsPreventRadioPowerOff (Boolean enable)
                SYS_SEL_TRAP (sysTrapHsSelector, hsSelPreventRadioPowerOff);

#endif




