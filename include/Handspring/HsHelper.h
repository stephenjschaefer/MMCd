/***************************************************************
*
*  Project:
*	Handspring Common Includes
*
*  Copyright info:
*	Copyright 2001 Handspring, Inc. All Rights Reserved.   
*
*
*  FileName:
*	HsHelper.h
* 
*  Description:
*	Public header file for the Handspring helper functions
*
*  ToDo:
* 
*  History:
*
****************************************************************/

#ifndef HSHELPER_H
#define HSHELPER_H

#include <Helper.h>
#define sysNotifyHelperEvent   'hlpr'

// MACRO: HelperEnumerateEnqueueEntry
//
// Enqueues a node of type HelperEvtEnumerateNodeType. Each node
//  must be individually allocated from the heap by the helper.
//
// PROTOTYPE:
//	  void HelperEnumNodeEnqueue(HelperNotifyEnumerateType* enumEvtDataP,
//								 HelperNotifyEnumerateListType* entryP);
#define HelperEnumerateEnqueueEntry(enumEvtDataP, entryP)	  \
  do												  \
	{												  \
	(entryP)->nextP = (enumEvtDataP) 	;	  \
	(enumEvtDataP) = (entryP);				  \
	}												  \
  while (0)

#endif