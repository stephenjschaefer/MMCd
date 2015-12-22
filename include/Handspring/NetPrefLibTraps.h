/***************************************************************
 *
 *  Project:
 *	  NetPref Engine
 *
 * Copyright info:
 *
 *	  Copyright (c) Handspring 2001 -- All Rights Reserved
 *
 *
 *  FileName:
 *	  NetPrefLibTraps.h
 * 
 *  Description:
 *	  Trap definitions for the NetPref Library.
 *
 *	Note:
 *
 *	History:
 *	  12-Dec-2001	vmk	  Created by Vitaly Kruglikov
 *
 ****************************************************************/


#ifndef __NET_PREF_LIB_TRAPS_H__
#define __NET_PREF_LIB_TRAPS_H__


// Trap IDs for the NetPref Library's public functions. The order of the traps
// must be the same as the routines are listed in NetPrefLibDispatchTable.c.
#define netPrefLibTrapVersionGet						(sysLibTrapCustom + 0)
#define netPrefLibTrapRecCountGet						(sysLibTrapCustom + 1)
#define netPrefLibTrapRecIDGetByIndex					(sysLibTrapCustom + 2)
#define netPrefLibTrapRecNew							(sysLibTrapCustom + 3)
#define netPrefLibTrapRecLoad							(sysLibTrapCustom + 4)
#define netPrefLibTrapRecRelease						(sysLibTrapCustom + 5)
#define netPrefLibTrapRecSave							(sysLibTrapCustom + 6)
#define netPrefLibTrapRecIDGet							(sysLibTrapCustom + 7)
#define netPrefLibTrapRecIndexGet						(sysLibTrapCustom + 8)
#define netPrefLibTrapRecDelete							(sysLibTrapCustom + 9)
#define netPrefLibTrapRecFieldGet						(sysLibTrapCustom + 10)
#define netPrefLibTrapRecFieldSet						(sysLibTrapCustom + 11)
#define netPrefLibTrapDefaultTargetGet					(sysLibTrapCustom + 12)
#define netPrefLibTrapDefaultTargetSet					(sysLibTrapCustom + 13)
#define netPrefLibTrapRecObjCopy						(sysLibTrapCustom + 14)
#define netPrefLibTrapRecIsDirty						(sysLibTrapCustom + 15)
#define netPrefLibTrapRecIndexGetByID					(sysLibTrapCustom + 16)
#define netPrefLibTrapRecFieldViewSet					(sysLibTrapCustom + 17)
#define netPrefLibTrapRecFieldAttrsGet					(sysLibTrapCustom + 18)
#define netPrefLibTrapUpdateFromRadioNV					(sysLibTrapCustom + 19)

#define netPrefLibTrapRecFieldSetDefine					(sysLibTrapCustom + 20)
#define netPrefLibTrapRecFieldSetGet					(sysLibTrapCustom + 21)
#define netPrefLibTrapRecFieldSetDefineStd				(sysLibTrapCustom + 22)
#define netPrefLibTrapRecFieldAddToSet					(sysLibTrapCustom + 23)

#define netPrefLibTrapRecReadOnlyOverrideStart			(sysLibTrapCustom + 24)
#define netPrefLibTrapRecReadOnlyOverrideEnd			(sysLibTrapCustom + 25)
#define netPrefLibTrapRecDirtyFlagsReset				(sysLibTrapCustom + 26)
#define netPrefLibTrapRecMarkDirty						(sysLibTrapCustom + 27)
#define netPrefLibTrapRecObjAlloc						(sysLibTrapCustom + 28)
#define netPrefLibTrapRecIsAttached						(sysLibTrapCustom + 29)
#define netPrefLibTrapRecBindingErrorGet				(sysLibTrapCustom + 30)
#define netPrefLibTrapHandleHotSyncNotify				(sysLibTrapCustom + 31)




#endif /* __NET_PREF_LIB_TRAPS_H__ */
