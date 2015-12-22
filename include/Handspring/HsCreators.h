/***************************************************************
*
*  Project:
*	Handspring Common Includes
*
*  Copyright info:
*	Copyright 1999 Handspring, Inc. All Rights Reserved.   
*
*
*  FileName:
*	HsExt.h
* 
*  Description:
*	Public header file for the creator IDs used by Handspring
*
*  ToDo:
* 
*  History:
*	4-May-1999 VMK - Created by Vitaly Kruglikov (vkruglikov@handspring.com)
*	8-Jan-2000 DOC - Added resource type and id for character lookup table
*					  used by Manhattan's keyboard driver
****************************************************************/

#ifndef	  __HS_CREATORS_H__
#define	  __HS_CREATORS_H__



//-----------------------------------------------------------
// Note from Palm Computing's SystemMgr.rh:
//................................................................
// File types and creators
//
//	Each database shall have a creator ID and a type.
//
//	The creator ID shall establish which application, patch, or extension
//	a particular database is associated with.  The creator ID should identify
//	the application/patch/extension, NOT who created it.
//
//	The type will determine which part of an application,
//	patch, or extension a particular database is.
//
//	There can be only one database per application, patch, or extension
//	which has type 'application', 'patch', or 'extension'.
//
//	Creators:
//
//	ROM-based applications created by Palm Computing have all-lower case
//	creator ID's.  Third-party applications have creator ID's which
//	are either all caps, or mixed case.  The same requirements go for
//	system patches and extensions.
//
//	All applications, system patches and extensions shall have unique creator
//	ID's.
//
//	Types:
//
//	'Application', 'Extension', and 'Patch' file/database types for ROM-based
//	apps shall be all-lower case.  Other file/database types must be mixed-case,
//	or all caps.  These other types are internal to the applications, and
//	therefore the system is unconcerned with their exact values.
//................................................................

// All Creators with "Yes" in the Reg column have been registered into
//  Palm's Creator database using the web page at 
//  http://www.palm.com/devzone/crid/cridsub.html


// ------------------------------------------------------------------
// Apps											  // Reg : Comment
// ------------------------------------------------------------------
#define hsFileCAdvCalculator			'HsPr'	  // Yes : Advanced Calculator application
#define hsFileCCardSetup				'HsCd'	  // Yes : ALL card setup utilities have
												  //	  this creator

#define	hsFileCSampleCardWelcome		'CWel'	  // Yes : Sample card welcome app
#define	hsFileCHandTerminal				'HTrm'	  // Yes : HandTerminal application sample code

#define hsFileCFileMover				'FlMv'	  // Yes : FileMover application (for flash cards)

#define hsFileCFileInstaller			'FlIn'	  // Yes : FileInstaller application (for flash cards)

#define hsFileCDateBook3H				'HsDB'	  // Yes : Handspring's version of Datebk3.

#define hsFileCBackup					'HsBk'	  // Yes : Serial Flash Backup application
#define hsFileCBackupWelcome			'HsBw'	  // Yes : Not used

#define hsFileCPhone					'HsPh'	  // Yes : Phone application
#define hsFileCPhoneSecurity	        'HsSE'    // Yes : Phone Security application
#define	hsFileCPhoneWizard				'HsPw'	  // Yes : Phone wizard application
#define hsFileCPhoneRingerPanel			'HsRN'    // Yes  : Ringer Panel
//#define hsFileCSMS					'SMSa'	  // No  : Phone SMS app (Short Message Service)
#define hsFileCSMS						'SMSh'	  // Yes  : Phone SMS app (Short Message Service)
#define hsFileCSimPhoneBook				'HsPB'	  // Yes : Phone - Sim PhoneBook app
#define hsFileCSimServices				'HsSS'	  // Yes : Phone - Sim Services app

#define hsFileCImagine					'HsIm'    // Yes : Imagine (imaging app)

#define hsFileCAddrBookExtensions       'HsAX'    // Yes : Handspring address book extensions
#define hsFileCDialingPanel             'HsDi'    // Yes : Dialing Panel...
#define hsFileCAddrBookOneFingerMode	'HsAl'	  // Yes : Address book modifications - one finger mode

#define	hsFileCPinballGame				'HsPg'	  // Yes: Pinball color game
#define hsFileCSetISP					'HsJS'	  // Yes: SetISP application -Sitti 2001-05-17 for Hydra project.

#define hsFileCBlazer					'BLZ1'	  // Yes : Bluelark Blazer browser

#define hsFileCCityTime					'CiAa'	  // No  : CityTime app - creator belongs
												  //		to original author, Darren Beck

#define hsFileCKeyboardDemo				'kDem'	  // Yes : Keyboard Demo

#define hsFileCFirmwareUpdater			'HsFU'	  // Yes : Wismo Firmware updater
#define hsFileCRomUpdater				'hROM'	  // Yes : ROM updater

// Testing apps...
#define hsFileCLangSpoofer				'HsLS'    // Yes : Language spoofer
#define hsFileCAddrTester				'HsAT'    // Yes : Address Tester
#define hsFileCMailWrapper              'HsMW'    // Yes : Mail Wrapper...
#define hsFileCUsbTester                'HsU2'    // No  : Usb tester...
#define hsFileCPhoneWrapper				'HsPW'	  // Yes : Phone Wrapper
#define hsFileCSetMem					'Hssm'	  // Yes : Handspring Set Mem App
#define hsFileCSerialCoupler			'Hssc'	  // Yes : Handspring serial coupler app
#define hsFileCUartCoupler				'Hsuc'	  // Yes : Handspring uart coupler app
#define hsFileCSpringWriter				'Hss1'	  // Yes : Not used (feel free to take)
#define hsFileCUsbResetTest				'Hsur'	  // Yes : USB reset tester.
#define hsFileCIsolateMemoryChip		'Isom'	  // Yes : Isolate a failing memory chip

#define hsFileCUseSerial				'HSUS'	  // Yes : App to signal FlashPrep for serial use

// ------- Types
#define	hsFileTCardSetup				'HsSU'	  // N/A : ALL card setup utilities have this
												  //		type
#define	hsFileTCardSetupShell			'HApp'	  // N/A : Card Setup Shell application type

// Creators for things other than apps...
#define hsFileCAutomaticHelper          'Auto'    // Yes : Apps that handle the "automatic"
                                                  //       choice for dialing.

// ------------------------------------------------------------------
// Panels											  // Reg : Comment
// ------------------------------------------------------------------

#define hsFileCPhoneServicesPanel		'hsPS'	  // No: Phone preference panel

#define hsFileCLCDAdjustPanel			'HsLA'	  // Yes: LCD Adjustment preference panel

#define hsFileCMultiChannelRingToneDB   'MCnl'    // Yes: Database that has multi-channel ring tones

// ------------------------------------------------------------------
// Patches
// ------------------------------------------------------------------
//#define hsFileC....

// ------- Others
#define hsFileCHandspringPalmOSUpdate	'Hspt'	  // Yes : Creator of PalmOS 3.5 patches
												  //	   sanctioned by Handspring.  Type
												  //	   of these files should be 'ptch'.

// ------- Types
#define	hsFileTHandspringHal			'HwAl'	  // N/A : File type of HAL.prc
#define	hsFileTHandspringPatch			'HsPt'	  // N/A  : File type of HsExtensions.prc

#define	hsFileTHandspringUIObj			'HsUi'	  // N/A  : File type of HsExtUIRsc.prc




// ------------------------------------------------------------------
// Extensions
// ------------------------------------------------------------------
#define hsFileCHandspringExt			'HsEx'	  // Yes : Handspring extensions




// ------------------------------------------------------------------
// Libraries
// ------------------------------------------------------------------
#define hsFileCSampleSerLib				'HsCs'	  // Yes : Sample Serial 16650 library

#define hsFileCTaskLib                  'HsTS'    // Yes: Task library

#define hsFileCFfsCSegLib				'FfMg'	  // Yes : Flash File System code segment
												  //		library

#define hsFileCLdbMgr					'HsLm'	  // Yes : Launcher Database Manager library


#define hsFileCModemSerLib				'HsMs'	  // Yes : Modem Module Serial library

#define hsFileCFlMgr					'FLMG'	  // Yes : Flash Manager  

#define hsFileCHwmCSegLib				'Hhwm'	  // Yes : Phone HAL

#define hsFileCCDMAPhoneLib             'HsCL'    // Yes : CDMA Phone Library

#define hsFileCCDMAMsgLib               'HsMg'    // Yes : CDMA Message Library

// Driver database creator and driver creator for the built-in Serial driver
// IMPORTANT: There is a "feature" in the Connection Panel that makes it
//  always pick the first serial driver as the default method when creating
//  a new connection and assuming the first method supports modems. 
// So... we make sure the creator of the built-in serial comes first in
//  the directory listing by giving it a creator ID that sorts before
//  all the other serial driver creator IDs (hsFileCUsbLib, hsFileCSerDrvrAuto, 
//	sysFileCVirtIrComm). 
#define	hsFileCSerLib					'A8xZ'	  // Yes  : SerialHWMgr driver ID

#define	hsFileCModuleSerLib				'HsSs'	  // Yes  : Serial lib for UART2 (goes to springboard)
												  //        ...only present on some devices like Belle...

#define hsFileCVirtualModemSerLib		'FakM'	  // Yes  : Virtual Modem

#define hsFileCHsSerWrapperLib			'Hssw'	  // Yes  : Old-style library used to wrap new drivers.

// Driver database creator for the USB drivers
#define	hsFileCUsbLib					'HsUs'	  // Yes : USB Library database creator
  // These driver ID's are installed by the USB database
  #define	hsFileCSerDrvrUSBConsole	'HsUc'	  // Yes : SerialHWMgr driver ID
  #define	hsFileCSerDrvrUSBDesktop	'HsUd'	  // Yes : SerialHWMgr driver ID
  #define	hsFileCSerDrvrUSBRemFS		'HsUr'	  // Yes : SerialHWMgr driver ID
  #define	hsFileCSerDrvrUSBGeneric	'HsUg'	  // Yes : SerialHWMgr driver ID
  #define	hsFileCSerDrvrUSBGeneric2	'HsUh'	  // Yes : SerialHWMgr driver ID


// Driver database creator and driver creator for the auto-select virtual driver
// ...this one is able to be set by springboard modules; by default it maps to 
// the cradle...
#define	hsFileCSerDrvrAuto				'HsSA'	  // Yes  : SerialHWMgr driver ID

#define	hsFileCSerDrvrCradle			'HsCS'	  // Yes  : SerialHWMgr driver ID




// ------- Types
#define hsFileTCodeSeg					'CSeg'	  // N/A  : Code segment library


// ------------------------------------------------------------------
// ROM tokens
// ------------------------------------------------------------------
#define hsFileCFFSInfoTok1				'Ffs1'	  // Yes : Flash file system info
												  //	    ROM Token string; see
												  //	    FfsMgrPrv.h for format
												  //	    details (FOR HANDSPRING, INC.
												  //	    USE ONLY!)

#define hsFileCFfsCrType				'Ffs2'	  // Yes : ascii string of colon-separated
				// creator and type of the module's Flash File System Manager. This value
				// MUST be unique for each Flash File System Manager implementation. This
				// token provides a level of indirction that allows applications to locate
				// the provider of FfsMgr services on any module that supports this
				// architecture and is fully compatible with the Handspring, Inc.'s FfsMgr
				// API. The format is "CCCC:TTTT", where CCCC is the unique 4-character
				// creator ID of your FfsMgr registred with Palm Computing, and TTTT is the
				// 4-character type id of your FfsMgr (must be mixed case ascii). (for example,
				// the reserved value used by Handspring, Inc. implementation is "FfMg:CSeg")


#define	hsFileCCardAccessTime			'HsAT'	  // Yes : ascii decimal string of
												  //	    access time in nano-seconds

#define	hsFileCCardWelcomeDuringReset	'HsWR'	  // Yes : actual value ignored. Presence
												  //		means launch welcome app during reset

#define	hsFileCCardWelcomeAppTypeCreator 'HsWT'	  // Yes : type & creator of card welcome app
												  // ONLY AVAILABLE IN VERSION 3.5 AND LATER
												  //       Ex: 	-tokStr "HsWT" "applMYAP"  

#define hsFileCSmartSmallromToken		'HsSR'	  // Yes: defines that the given smallrom is
												  //      smart about running from RAM--it will
												  //      auto-drop into the appropriate debugger.

#define hsFileCBuiltinHsExtToken		'HsHX'	  // Yes: defines what version of HSExtension is
												  //	  builtin to the ROM.
												  // ONLY AVAILABLE IN HSEXT 3.5 VERSION 1.2 AND LATER


//................................................................
// Miscellaneous database types
//................................................................

#define hsFileTModemSetupPatch			'HsMp'	  // N/A  : The Patch for the Modem Module's
												  //	     CardSetup app has this type and
												  //	     the same creator as the Modem
												  //	     Module's serial library (hsFileCModemSerLib)


#define hsCompressedDbCreatorId			'Lz7C'	  // Yes: Creator and type id's of the compressed
#define hsCompressedDbTypeId			'Lz7C'	  //	  databases

#define hsKbdLayoutDbTypeCreator		'KbLy'	  // Yes: Type and creator of database containing
												  //	  keyboard layout resources


//................................................................
// System Resource types and IDs
//................................................................
#define	hsRscTKbdChrLookupData			'HsKb'	  // Yes: type of a HEX resource that is a table
												  //      used by the keyboard driver for looking up the
												  //	  code and modifier of a character.

#define hsRscKbdChrDataID				1000	  // ID of resource containing regular character lookup
												  //  table

#define hsRscKbdChrOptDataID			1001	  // ID of resource containing option character lookup
												  //  table

#define	hsRscTOptLockKeys				'HsLk'	  // Yes: type of a HEX resource that specifies which
												  //  keys are affected by option lock state

#define hsRscOptLockKeysID				1000	  // ID of resource that specifies which keys respect
												  //  option lock

#define hsRscOptLockKeysExt1ID			1001	  // ID of resource that specifies which keys of 1st
												  //  extension group respect option lock

#define hsRscOptLockKeysExt2ID			1002	  // ID of resource that specifies which keys of 2nd
												  //  extension group respect option lock

#define hsRscTPostProcessLists			'HsPp'	  // Yes: type of a HEX resource that defines lists
												  //	  used by a post-processing mechanism to
												  //	  convert an entered character into a related
												  //	  character or string.

#define hsRscPostProcessList			19000	  // Using a Palm reserved ID (an ID > 10000) to prevent
												  //  resource conflicts.  Since we've registered
												  //  the HsPp id with Palm, we should not have resource
												  //  conflicts with Palm.

#define hsRscTWordCorrectionData		'HsWc'	  // Yes: type of a HEX resource that defines the words
												  //	  used by the word correction feature.

#define hsRscWordCorrectionDataID		19000	  // Using a Palm reserved ID (an ID > 10000) to prevent
												  //  resource conflicts.  Since we've registered
												  //  the HsWc id with Palm, we should not have resource
												  //  conflicts with Palm.

#define hsRscTButtonMappingData			'HsBm'	  // Yes: type of a HEX resource that defines the strings
												  //  used to map keyboard buttons to UI buttons


// Note: The following must match the types/IDs in the HAL makefile

#define hsRscTHalDriver					'HwDv'	  // N/A: type of code resource containing a Hal driver

#define hsRscFirstHalDriverID			19001	  // The ID at which HAL starts loading sequentially
												  //  numbered drivers

#define hsRscHalKbdDriverID				19001	  // Using a Palm reserved ID (an ID > 10000) which
												  //  sequentially follows the main HAL boot resource.

#define hsRscTHsExPatchModule			'HsPt'	  // Yes: type of code resource containing HsExtensions 
												  //  patch. Deliberately the same as hsFileTHandspringPatch

#define hsRscFirstHsExModuleID			19000	  // Using a Palm reserved ID (an ID > 10000) which
												  //  sequentially follows the main HAL boot resource.


#endif	  // __HS_CREATORS_H__ -- include once




