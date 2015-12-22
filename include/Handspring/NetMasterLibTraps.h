/***************************************************************
 *
 *  Project:
 *	  NetMaster Library
 *
 * Copyright info:
 *
 *	  Copyright (c) Handspring 2002 -- All Rights Reserved
 *
 *
 *  FileName:
 *	  NetMasterLibTraps.h
 * 
 *  Description:
 *	  Trap definitions for the NetMaster Library.
 *
 *	Note:
 *
 *	History:
 *	  31-Jan-2002	vmk	  Created by Vitaly Kruglikov
 *
 ****************************************************************/


#ifndef __NET_MASTER_LIB_TRAPS_H__
#define __NET_MASTER_LIB_TRAPS_H__


// Trap IDs for the NetMaster Library's public functions. The order of the traps
// must be the same as the routines are listed in NetMasterLibDispatchTable.c.
#define netMasterLibTrapLibVersionGet					(sysLibTrapCustom + 0)
#define netMasterLibTrapNetAttach						(sysLibTrapCustom + 1)
#define netMasterLibTrapNetLibPatchInstall				(sysLibTrapCustom + 2)
#define netMasterLibTrapAutoLoginSettingGet				(sysLibTrapCustom + 3)
#define netMasterLibTrapAutoLoginSettingSet				(sysLibTrapCustom + 4)
#define netMasterLibTrapServiceSetUp					(sysLibTrapCustom + 5)
#define netMasterLibTrapNetLibOpenIfFullyUp				(sysLibTrapCustom + 6)
#define netMasterLibTrapNetLibIsFullyUp					(sysLibTrapCustom + 7)
#define netMasterLibTrapPhoneServiceClassesGet			(sysLibTrapCustom + 8)
#define netMasterLibTrapHandlePhoneEvent				(sysLibTrapCustom + 9)
#define netMasterLibTrapControl							(sysLibTrapCustom + 10)
#define netMasterLibTrapNetInterfacesShutDown			(sysLibTrapCustom + 11)
#define netMasterLibTrapNetLibIsLoggingIn				(sysLibTrapCustom + 12)



#endif /* __NET_MASTER_LIB_TRAPS_H__ */
