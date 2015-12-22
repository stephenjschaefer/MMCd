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
*	HsKeys.h
* 
*  Description:
*	Public header file for the Handspring extensions to the Key
*	Manager.
*
*  ToDo:
* 
*  History:
*	27-Feb-2001 DOC - Created by Debbie Chyi
****************************************************************/



/********************************************************************
 * Definition of the 1st extension bit field returned in the bit
 *  field array passed into KeyCurrentStateEx.  The following bit
 *	names are based on the American keyboard.
 ********************************************************************/
#define	keyBitExt1Q		  0x00000001	// Q key
#define keyBitNumExt1Q			   0
#define	keyBitExt1W		  0x00000002	// W key
#define keyBitNumExt1W			   1
#define	keyBitExt1E		  0x00000004	// E key
#define keyBitNumExt1E			   2
#define	keyBitExt1R		  0x00000008	// R key
#define keyBitNumExt1R			   3
#define	keyBitExt1T		  0x00000010	// T key
#define keyBitNumExt1T			   4
#define	keyBitExt1Y		  0x00000020	// Y key
#define keyBitNumExt1Y			   5
#define	keyBitExt1U		  0x00000040	// U key
#define keyBitNumExt1U			   6
#define	keyBitExt1I		  0x00000080	// I key
#define keyBitNumExt1I			   7
#define	keyBitExt1O		  0x00000100	// O key
#define keyBitNumExt1O			   8
#define	keyBitExt1P		  0x00000200	// P key
#define keyBitNumExt1P			   9
#define	keyBitExt1A		  0x00000400	// A key
#define keyBitNumExt1A			  10
#define	keyBitExt1S	  	  0x00000800	// S key
#define keyBitNumExt1S			  11
#define	keyBitExt1D		  0x00001000	// D key
#define keyBitNumExt1D			  12
#define	keyBitExt1F		  0x00002000	// F key
#define keyBitNumExt1F			  13
#define	keyBitExt1G		  0x00004000	// G key
#define keyBitNumExt1G			  14
#define	keyBitExt1H		  0x00008000	// H key
#define keyBitNumExt1H			  15
#define	keyBitExt1J		  0x00010000	// J key
#define keyBitNumExt1J			  16
#define	keyBitExt1K		  0x00020000	// K key
#define keyBitNumExt1K			  17
#define	keyBitExt1L		  0x00040000	// L key
#define keyBitNumExt1L			  18
#define keyBitExt1Bksp	  0x00080000	// Backspace key
#define keyBitNumExt1Bksp		  19
#define	keyBitExt1Opt	  0x00100000	// Option key
#define keyBitNumExt1Opt		  20
#define	keyBitExt1Z		  0x00200000	// Z key
#define keyBitNumExt1Z			  21
#define	keyBitExt1X		  0x00400000	// X key
#define keyBitNumExt1X			  22
#define	keyBitExt1C		  0x00800000	// C key
#define keyBitNumExt1C			  23
#define	keyBitExt1V		  0x01000000	// V key
#define keyBitNumExt1V			  24
#define	keyBitExt1B		  0x02000000	// B key
#define keyBitNumExt1B			  25
#define	keyBitExt1N		  0x04000000	// N key
#define keyBitNumExt1N			  26
#define	keyBitExt1M		  0x08000000	// M key
#define keyBitNumExt1M			  27
#define	keyBitExt1Period  0x10000000	// Period key
#define keyBitNumExt1Period		  28   
#define keyBitExt1Enter	  0x20000000	// Enter key
#define keyBitNumExt1Enter		  29   
#define	keyBitExt1Shift	  0x40000000	// Shift key
#define keyBitNumExt1Shift		  30
#define keyBitExt1Space	  0x80000000	// Space key
#define keyBitNumExt1Space		  31

#define	keyBitsAllExt1	  0xFFFFFFFF	// All keys


/********************************************************************
 * Definition of the 2nd extension bit field returned in the bit
 *  field array passed into KeyCurrentStateEx.  The following bit
 *	names are based on the American keyboard.
 ********************************************************************/
#define	keyBitExt2Symbol  0x00000001	// Symbol key
#define keyBitNumExt2Symbol		   0
#define	keyBitExt2CmdMenu 0x00000002	// Cmd/menu key
#define keyBitNumExt2CmdMenu	   1
#define	keyBitExt2JogUp	  0x00000004	// Jog-up key
#define keyBitNumExt2JogUp		   2
#define keyBitExt2JogDown 0x00000008	// Jog-down key
#define keyBitNumExt2JogDown	   3
#define	keyBitExt2JogScan 0x00000010	// Jog-scan key
#define keyBitNumExt2JogScan	   4

#define	keyBitsAllExt2	  0xFFFFFFFF	// All keys


/********************************************************************
 *Which keys can wake-up the device (set of bits for when keys
 *	pressed alone and a set for when keys pressed with option)
 ********************************************************************/
#define keyBitsWakeUp		( keyBitPower | keyBitCradle |  \
							  keyBitHard1 | keyBitHard2  |  \
							  keyBitHard3 | keyBitHard4  )
#define keyBitsWakeUpExt1	0x00000000
#define keyBitsWakeUpExt2	( keyBitExt2JogScan )

#define keyBitsOptWakeUp	  0x00000078
#define keyBitsOptWakeUpExt1  0x00000000  // Option key is bit 0x00100000
										  //  in the Ext1 field.  It should
										  //  never be set.
#define keyBitsOptWakeUpExt2  0x00000002


/********************************************************************
 *Extended shift state values for supporting an option and option lock state
 ********************************************************************/
#define keyGrfTempShiftOption		  (grfTempShiftLower+1) // grfTempShiftLower defined in Graffiti.h
#define keyGlfOptLock				  (glfNumLock << 1)	//	glfNumLock defined in GraffitiShift.h
