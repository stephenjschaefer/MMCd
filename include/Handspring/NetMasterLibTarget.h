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
 *	  NetMasterLibTarget.h
 * 
 *  Description:
 *	  This is the target parameters file for the NetMaster Library. We
 *	  break out these definitions so that other portable modules
 *	  could access them without PalmOS-specific definishions
 *
 *	Note:
 *
 *	History:
 *	  31-Jan-2002	vmk	  Created by Vitaly Kruglikov
 *
 ****************************************************************/

#ifndef _NET_MASTER_LIB_TARAGET_H_
#define _NET_MASTER_LIB_TARAGET_H_



// 'HsNM' was registered with Palm on February 01, 2002 at 3:59pm
#define netMasterLibCreatorID	'HsNM'	// NetMaster Library database creator

#define netMasterLibTypeID		'libr'	// Standard library database type

#define netMasterLibName		"HsNetMasterLibrary.lib"  // Internal library name
										//  which can be passed to SysLibFind()




#endif // _NET_MASTER_LIB_TARAGET_H_
