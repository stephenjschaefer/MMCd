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
 *  FileName:
 *	  CallbackFix.h
 *
 *  Description:	
 *      Macros for fixing up A4 (GNU GCC-style) globals in
 *	  GCC-compiled callback functions that are called from
 *	  non-GCC code.
 *	  
 *	    Note:  these routines are now obsolete because gcc
 *	   2.95 switched to use A5 for globals instead of A4. 
 *	  
 * From Ian Goldberg (reformatted):
 *   Routines compiled by gcc, but called by routines _not_ compiled by gcc
 * (most commonly, your event handlers) have their globals sometimes broken,
 * unless you use a workaround.
 * 
 * At the top of the callback (right after the var. declarations), put
 * 
 * CALLBACK_PROLOGUE();
 * 
 * At the bottom (just before the return), put
 * 
 * CALLBACK_EPILOGUE();
 * 
 * Make _sure_ the event handler can _only_ exit by going through the
 * CALLBACK_EPILOGUE.
 * 
 * This is a workaround for a bug in the older version of gcc:
 * gcc assumes that no one will touch %a4 after it is set up in crt0.o.
 * This isn't true if a function is called as a callback by something
 * that wasn't compiled by gcc (like FrmCloseAllForms()).  It may also
 * not be true if it is used as a callback by something in a different
 * shared library.
 * We really want a function attribute "callback" which will insert this
 * progloue and epilogoue automatically.
 * - Ian 
 *
 * ALSO: the "-mvolatile-gp" GCC compiler flag may be used to force the compiler
 *		  to generate this code automatically.
 *
 *
 *  History:
 *	19-Apr-1999  vmk  Initial version
 *	08-Oct-1999  BP   Converted to shared include format
 *	14-Apr-2000  BP   Updated for latest gcc (no longer needed)
 *
 **********************************************************************/

#ifndef __CALLBACK_FIX_H__
#define __CALLBACK_FIX_H__

	#if (defined __GNUC__)

	  #if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)

		  #define CALLBACK_PROLOGUE()
		  #define CALLBACK_EPILOGUE()

	  #else		    // not GCC >= 2.95

		  register void *reg_a4 asm("%a4");

		  #define CALLBACK_PROLOGUE() \
			 void *save_a4 = reg_a4; asm("move.l %%a5,%%a4; sub.l #edata,%%a4" : :);
		  #define CALLBACK_EPILOGUE() reg_a4 = save_a4;

	  #endif		// not GCC >= 2.95

	#else		// not GCC

		#define CALLBACK_PROLOGUE()
		#define CALLBACK_EPILOGUE()

	#endif		// not GCC

#endif	// __CALLBACK_FIX_H__
