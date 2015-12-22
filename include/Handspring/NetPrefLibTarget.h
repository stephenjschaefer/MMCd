/***************************************************************
 *
 *  Project:
 *	  NetPref Library
 *
 * Copyright info:
 *
 *	  Copyright (c) Handspring 2001 -- All Rights Reserved
 *
 *
 *  FileName:
 *	  NetPrefLibTarget.h
 * 
 *  Description:
 *	  This is the target parameters file for the NetPref Library. We
 *	  break out these definitions so that other portable modules
 *	  could access them without PalmOS-specific definishions
 *
 *	Note:
 *
 *	History:
 *	  12-Dec-2001	vmk	  Created by Vitaly Kruglikov
 *
 ****************************************************************/

#ifndef _NET_PREF_LIB_TARAGET_H_
#define _NET_PREF_LIB_TARAGET_H_



// 'HsNP' was registered on 12/12/2001 @ 4:56pm
#define netPrefLibCreatorID	  'HsNP'	// NetPref Library database creator

#define netPrefLibTypeID	  'libr'	// Standard library database type

#define netPrefLibName		  "HsNetPrefLibrary.lib"  // Internal library name
										//  which can be passed to SysLibFind()




#endif // _NET_PREF_LIB_TARAGET_H_
