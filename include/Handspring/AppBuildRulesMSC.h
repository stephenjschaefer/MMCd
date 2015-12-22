/***************************************************************
 *
 *  Project:
 *	  Handspring Shared Includes 
 *
 *  Copyright info:
 *	  This is free software; you can redistribute it and/or modify
 *	  it as you like.
 *
 *	  This program is distributed in the hope that it will be useful,
 *	  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *
 *
 *
 *  FileName:
 *	  AppBuildRulesMSC.h
 * 
 *  Description:
 *	  This header gets included by <BuildRules.h> when it detects that
 *	we're using the Visual C Compiler (#ifdef _MSC_VER). 
 *
 *	  By default, <BuildRules.h> includes "AppBuildRules.h" but the
 *	Visual C compiler performs include path searching wrong - it starts
 *	the search from the directory of the current include file - not the
 *	directory of the source file. So, it would always end up using the
 *	"AppBuildRules.h" in the directory with all the other PalmOS equates
 *	and not the one in the applications local directory.
 *
 *	  We work around this buy making it use AppBuildRulesMSC.h instead when
 *	using the Visual C compiler and make sure there IS NOT one of these
 *	headers in the same directory as the distributed BuildRules.h. 
 * 
 *  History:
 *	29-Dec-98  RM - Created by Ron Marianetti
 *****************************************************************/

#ifndef CPU_TYPE
  #define	CPU_TYPE			CPU_68K
#endif
  
#undef	EMULATION_LEVEL
#define	EMULATION_LEVEL		EMULATION_WINDOWS


// Disable Warnings
#pragma warning( disable : 4103)	// used #pragma pack to change alignment
#pragma warning( disable : 4068)	// unknown pragma
#pragma warning( disable : 4761)	// integral size mismatch in argument
#pragma warning( disable : 4305)    // truncation from 'const int ' to 'short '
#pragma warning( disable : 4054)    // type cast from function to data pointer
#pragma warning( disable : 4214)    // bit field type other than int
#pragma warning( disable : 4244)	// possible loss of data
#pragma warning( disable : 4100)	// unreferenced parameter

/*
#pragma warning( disable : 4121)	// sensitive to packing
#pragma warning( disable : 4214)  
#pragma warning( disable : 4127)	// conditional expression is constant
#pragma warning( disable : 4611)	// interaction between setjmp and C++ non-portable
#pragma warning( disable : 4514)	// unused inline function has been removed
*/


// Allow compilation using Visual C compiler for browsing info
#ifndef __GNUC__
  #define asm(x)	  
#endif

// don't use traps in Visual C++
#undef	__PALMOS_TRAPS__
#define __PALMOS_TRAPS__	0	  

#undef	USE_TRAPS

