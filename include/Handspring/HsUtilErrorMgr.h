/***************************************************************
 *
 * Project:
 *    ViewerUtils 
 *
 * Copyright info:
 *    Copyright Handspring, Inc. 2001
 *
 * FileName:
 *    HsUtilErrorMgr.h
 * 
 * Description:
 *	  This file is like Palm's ErrorMgr.h, but contains
 *	  additional macros that are useful.
 *
 * ToDo:
 *
 * History:
 *	  2001-07-17  dia	Creaded by Doug Anderson
 *
 ****************************************************************/

#ifndef _HS_UTIL_ERROR_MGR_H
#define _HS_UTIL_ERROR_MGR_H


// ------------------------------------------------------------------------------------
// Verify that Palm's error check constants are defined
// ------------------------------------------------------------------------------------

// Verify that these are defined.  Note that ANSI C says that 
// constants evaluate to 0 in the preprocessor if they're not
// defined, so this is very important!
#if ((!defined (ERROR_CHECK_LEVEL))	  || \
	 (!defined (ERROR_CHECK_FULL))	  || \
	 (!defined (ERROR_CHECK_PARTIAL)) || \
	 (!defined (ERROR_CHECK_NONE))	      )
  #error "Error check levels not defined"
#endif


// ------------------------------------------------------------------------------------
// Macro for internal use
// ------------------------------------------------------------------------------------

// This macro is used by the definitions below.  It shouldn't be used directly!
#define _HsUtilErrBreakIf(condition) \
  do \
	{ \
	  if (condition) { DbgBreak(); } \
	} \
  while (0)


// ------------------------------------------------------------------------------------
// HsUtilErrFatalBreakIf	- DbgBreak() if the given condition is true except in
//							  case of no error checking (which we currently don't use)
// HsUtilErrNonFatalBreakIf - DbgBreak() if the given condition is true, but only in
//							  debug ROMs
// HsUtilErrFatalBreak	  	- DbgBreak() except in case of no error checking (which we 
//							  currently don't use).  Not much difference between this
//							  and just using DbgBreak() directly.
// HsUtilErrNonFatalBreakIf - DbgBreak(), but only in debug ROMs
// ------------------------------------------------------------------------------------

#if (ERROR_CHECK_LEVEL == ERROR_CHECK_FULL)
  // This is the level of error checking of Debug (dev/test) ROMs

  #define HsUtilErrFatalBreakIf					_HsUtilErrBreakIf
  #define HsUtilErrNonFatalBreakIf				_HsUtilErrBreakIf
  #define HsUtilErrFatalBreak()					_HsUtilErrBreakIf(true)
  #define HsUtilErrNonFatalBreak()				_HsUtilErrBreakIf(true)

#elif (ERROR_CHECK_LEVEL == ERROR_CHECK_PARTIAL)
  // This is the level of error checking of Release (rel) ROMs

  #define HsUtilErrFatalBreakIf					_HsUtilErrBreakIf
  #define HsUtilErrNonFatalBreakIf(condition)	
  #define HsUtilErrFatalBreak()					_HsUtilErrBreakIf(true)
  #define HsUtilErrNonFatalBreak()				

#else
  // This level of error checking is never used currently

  #define HsUtilErrFatalBreakIf(condition)
  #define HsUtilErrNonFatalBreakIf(condition)
  #define HsUtilErrFatalBreak()
  #define HsUtilErrNonFatalBreak()

#endif



#endif // _HS_UTIL_ERROR_MGR_H
