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
*	HsErrorClasses.h
* 
*  Description:
*	Public header file for the error classes used by Handspring
*	viewer modules
*
*  ToDo:
* 
*  History:
*	17-Jun-1999 VMK - Created by Vitaly Kruglikov (vkruglikov@handspring.com)
****************************************************************/

#ifndef	  __HS_ERROR_CLASSES_H__
#define	  __HS_ERROR_CLASSES_H__


#include <SystemMgr.h>	  // for appErrorClass


/************************************************************
 * Error Classes for each manager
 *************************************************************/

#define hsAppErrorClass	    appErrorClass				// For top-level applications
#define	hsFfsErrorClass	    (appErrorClass | 0x0100)	// Flash file system manager
#define	hsFlmErrorClass	    (appErrorClass | 0x0200)	// Flash manager
#define	hsFlashErrorClass   (appErrorClass | 0x0300)    // Flash driver
#define	hsFttErrorClass	    (appErrorClass | 0x0400)    // File Transfer Transport driver
													    //  (used by the File Installer app)
#define	hsFtcErrorClass	    (appErrorClass | 0x0500)    // File Transfer Command driver
													    //  (used by the File Installer app)

#define hsTasLibErrorClass  (appErrorClass | 0x0600)    // Task manager.

#define hsPsaErrorClass		(appErrorClass | 0x0700)	// Phone Serial Arbitrator

#define hsHwmErrorClass		(appErrorClass | 0x0800)	// Phone HAL

#define hsVmErrorClass		(appErrorClass | 0x0900)	// Virtual Modem
#define hsSrmExtErrorClass	(appErrorClass | 0x0A00)	// Srm Extensions


/************************************************************
 * OEM Error Classes 
 *************************************************************/
#define hsOEMErrorClass			oemErrorClass

#define hsOEMRadioErrorClass	(hsOEMErrorClass)		// Radio Errors

// NetMaster library error class
#define hsNetMasterErrorClass		(hsOEMErrorClass + 0x0100)	// 0x7100

// NetPref library error class
#define hsNetPrefLibErrorClass		(hsOEMErrorClass + 0x0200)	// 0x7200



#endif	  // __HS_ERROR_CLASSES_H__ -- include once




