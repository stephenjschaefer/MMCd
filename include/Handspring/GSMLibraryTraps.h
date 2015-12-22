/*******************************************************************************

FILE GSMLibraryTraps.h
 Copyright Option International 1999-2000 and Handspring Inc. 1999-2001
 All rights reserved.

SYNOPSIS
 Public interface for the GSM library.

*******************************************************************************/

#ifndef __GSMLIBRARYTRAPS_H__
#define __GSMLIBRARYTRAPS_H__

// Trap IDs for the GSM library public functions. The order of the traps
// must be the same as the routines are listed in GSMLibDispatch.

#define GSMLibTrapGetLibAPIVersion          sysLibTrapCustom
#define GSMReserved1                        (sysLibTrapCustom + 1)
#define GSMLibTrapRegister                  (sysLibTrapCustom + 2)
#define GSMReserved3                        (sysLibTrapCustom + 3)
#define GSMLibTrapNewAddress                (sysLibTrapCustom + 4)
#define GSMLibTrapGetField                  (sysLibTrapCustom + 5)
#define GSMLibTrapGetID                     (sysLibTrapCustom + 6)
#define GSMLibTrapSetField                  (sysLibTrapCustom + 7)
#define GSMLibTrapSetNumber                 (sysLibTrapCustom + 8)
#define GSMLibTrapSetID                     (sysLibTrapCustom + 9)
#define GSMLibTrapAddressToText             (sysLibTrapCustom + 10)
#define GSMLibTrapHasName                   (sysLibTrapCustom + 11)
#define GSMLibTrapEqual                     (sysLibTrapCustom + 12)
#define GSMReserved13                       (sysLibTrapCustom + 13)
#define GSMReserved14                       (sysLibTrapCustom + 14)
#define GSMReserved15                       (sysLibTrapCustom + 15)
#define GSMReserved16                       (sysLibTrapCustom + 16)
#define GSMReserved17                       (sysLibTrapCustom + 17)
#define GSMReserved18                       (sysLibTrapCustom + 18)
#define GSMReserved19                       (sysLibTrapCustom + 19)
#define GSMReserved20                       (sysLibTrapCustom + 20)
#define GSMReserved21                       (sysLibTrapCustom + 21)
#define GSMReserved22                       (sysLibTrapCustom + 22)
#define GSMReserved23                       (sysLibTrapCustom + 23)
#define GSMReserved24                       (sysLibTrapCustom + 24)
#define GSMReserved25                       (sysLibTrapCustom + 25)
#define GSMReserved26                       (sysLibTrapCustom + 26)
#define GSMReserved27                       (sysLibTrapCustom + 27)
#define GSMReserved28                       (sysLibTrapCustom + 28)
#define GSMReserved29                       (sysLibTrapCustom + 29)
#define GSMReserved30                       (sysLibTrapCustom + 30)
#define GSMReserved31                       (sysLibTrapCustom + 31)
#define GSMReserved32                       (sysLibTrapCustom + 32)
#define GSMReserved33                       (sysLibTrapCustom + 33)
#define GSMReserved34                       (sysLibTrapCustom + 34)
#define GSMReserved35                       (sysLibTrapCustom + 35)
#define GSMReserved36                       (sysLibTrapCustom + 36)
#define GSMReserved37                       (sysLibTrapCustom + 37)
#define GSMReserved38                       (sysLibTrapCustom + 38)
#define GSMReserved39                       (sysLibTrapCustom + 39)
#define GSMLibTrapGetRingingInfo            (sysLibTrapCustom + 40)
#define GSMLibTrapSetRingingInfo            (sysLibTrapCustom + 41)
#define GSMLibTrapGetToneIDs                (sysLibTrapCustom + 42)
#define GSMLibTrapGetToneName               (sysLibTrapCustom + 43)
#define GSMLibTrapPlayTone                  (sysLibTrapCustom + 44)
#define GSMLibTrapStopTone                  (sysLibTrapCustom + 45)
#define GSMReserved46                       (sysLibTrapCustom + 46)
#define GSMReserved47                       (sysLibTrapCustom + 47)
#define GSMReserved48                       (sysLibTrapCustom + 48)
#define GSMReserved49                       (sysLibTrapCustom + 49)
#define GSMReserved50                       (sysLibTrapCustom + 50)
#define GSMReserved51                       (sysLibTrapCustom + 51)
#define GSMLibTrapRegistered                (sysLibTrapCustom + 52)
#define GSMLibTrapRoaming                   (sysLibTrapCustom + 53)
#define GSMReserved54                       (sysLibTrapCustom + 54)
#define GSMReserved55                       (sysLibTrapCustom + 55)
#define GSMReserved56                       (sysLibTrapCustom + 56)
#define GSMReserved57                       (sysLibTrapCustom + 57)
#define GSMReserved58                       (sysLibTrapCustom + 58)
#define GSMLibTrapGetSIMStatus              (sysLibTrapCustom + 59)
#define GSMReserved60                       (sysLibTrapCustom + 60)
#define GSMLibTrapGetErrorText              (sysLibTrapCustom + 61)
#define GSMLibTrapGetDBRef                  (sysLibTrapCustom + 62)
#define GSMLibTrapReleaseDBRef              (sysLibTrapCustom + 63)
#define GSMLibTrapNewMessage                (sysLibTrapCustom + 64)
#define GSMLibTrapDeleteMessage             (sysLibTrapCustom + 65)
#define GSMLibTrapSendMessage               (sysLibTrapCustom + 66)
#define GSMLibTrapSetText                   (sysLibTrapCustom + 67)
#define GSMLibTrapSetDate                   (sysLibTrapCustom + 68)
#define GSMLibTrapSetOptions                (sysLibTrapCustom + 69)
#define GSMLibTrapSetAddresses              (sysLibTrapCustom + 70)
#define GSMLibTrapSetStatus                 (sysLibTrapCustom + 71)
#define GSMLibTrapSetFlags                  (sysLibTrapCustom + 72)
#define GSMLibTrapSetOwner                  (sysLibTrapCustom + 73)
#define GSMLibTrapGetText                   (sysLibTrapCustom + 74)
#define GSMLibTrapGetDate                   (sysLibTrapCustom + 75)
#define GSMLibTrapGetOptions                (sysLibTrapCustom + 76)
#define GSMLibTrapGetAddresses              (sysLibTrapCustom + 77)
#define GSMLibTrapGetStatus                 (sysLibTrapCustom + 78)
#define GSMLibTrapGetFlags                  (sysLibTrapCustom + 79)
#define GSMLibTrapGetOwner                  (sysLibTrapCustom + 80)
#define GSMLibTrapGetType                   (sysLibTrapCustom + 81)
#define GSMLibTrapIsLegalCharacter          (sysLibTrapCustom + 82)
#define GSMLibTrapMapCharacter              (sysLibTrapCustom + 83)
#define GSMLibTrapSetServiceCentreAddress   (sysLibTrapCustom + 84)
#define GSMLibTrapGetServiceCentreAddress   (sysLibTrapCustom + 85)
#define GSMLibTrapLength                    (sysLibTrapCustom + 86)
#define GSMLibTrapGetSubstitution           (sysLibTrapCustom + 87)
#define GSMLibTrapNewAddressList            (sysLibTrapCustom + 88)
#define GSMLibTrapDisposeAddressList        (sysLibTrapCustom + 89)
#define GSMLibTrapAddAddress                (sysLibTrapCustom + 90)
#define GSMLibTrapGetNth                    (sysLibTrapCustom + 91)
#define GSMLibTrapSetNth                    (sysLibTrapCustom + 92)
#define GSMLibTrapCount                     (sysLibTrapCustom + 93)
#define GSMLibTrapSetModulePower            (sysLibTrapCustom + 94)
#define GSMLibTrapModulePowered             (sysLibTrapCustom + 95)
#define GSMLibTrapModuleButtonDown          (sysLibTrapCustom + 96)
#define GSMReserved97                       (sysLibTrapCustom + 97)
#define GSMReserved98                       (sysLibTrapCustom + 98)
#define GSMLibTrapGetDataApplication        (sysLibTrapCustom + 99)
#define GSMLibTrapSetDataApplication        (sysLibTrapCustom + 100)
#define GSMReserved101                      (sysLibTrapCustom + 101)

#define GSMReserved102            			(sysLibTrapCustom + 102)
#define GSMReserved103            			(sysLibTrapCustom + 103)
#define GSMReserved104		           		(sysLibTrapCustom + 104)
#define GSMReserved105		           		(sysLibTrapCustom + 105)

#define GSMReserved106            			(sysLibTrapCustom + 106)

#define GSMReserved107		              	(sysLibTrapCustom + 107)
#define GSMReserved108		              	(sysLibTrapCustom + 108)

#define GSMReserved109              		(sysLibTrapCustom + 109)
#define GSMReserved110              		(sysLibTrapCustom + 110)

#define GSMLibTrapGetSMSRingInfo			(sysLibTrapCustom + 111)
#define GSMLibTrapSetSMSRingInfo			(sysLibTrapCustom + 112)

#define GSMLibTrapPlayDTMF                  (sysLibTrapCustom + 113)

#define GSMLibTrapStartVibrate              (sysLibTrapCustom + 114)
#define GSMLibTrapStopVibrate               (sysLibTrapCustom + 115)

#define GSMLibTrapSendUSSD					(sysLibTrapCustom + 116)

#define GSMLibTrapSATGetMainMenu			(sysLibTrapCustom + 117)
#define GSMLibTrapSATSendRequest			(sysLibTrapCustom + 118)
#define GSMLibTrapSATEndSession				(sysLibTrapCustom + 119)

#define GSMReserved120						(sysLibTrapCustom + 120)
#define GSMReserved121						(sysLibTrapCustom + 121)
#define GSMReserved122						(sysLibTrapCustom + 122)
#define GSMReserved123						(sysLibTrapCustom + 123)
#define GSMLibTrapGetPhoneCallStatus		(sysLibTrapCustom + 124)
#define GSMReserved125						(sysLibTrapCustom + 125)

#define GSMReserved126						(sysLibTrapCustom + 126)
#define GSMReserved127						(sysLibTrapCustom + 127)
#define GSMLibTrapGetSMSGateway				(sysLibTrapCustom + 128)

#define GSMLibTrapLast                      (sysLibTrapCustom + 129)

#endif
