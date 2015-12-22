/***********************************************************************
*  Project:
*	Handspring Common Includes
*
*  FileName:
*	NewTypes.h
* 
*  Description:
*	Public header file for the Handspring extensions to PalmOS
*
*  ToDo:
* 
*  History:
*	13-Jul-2000 BP - Created by Bob Petersen from NewTypes.h from John
*	Marshall at Palm.  
*
*  From John Marshall: 
*     With Palm OS SDKs prior to the 3.5 SDK, programmers were encouraged
*  to use typedefs such as `DWord' and `VoidHand' rather than the built-in
*  types.  In the 3.5 SDK, the names of these typedefs have been changed to
*  the somewhat more mnemonic `UInt32' (etc) and the rather more
*  functional `MemHandle'.  (Previously there were two "handle" datatypes,
*  `VoidHand' and `Handle', with no real reason for a distinction between
*  them, and programmers were continually casting between them.
*  `MemHandle' corrects this historical accident.)
*  
*     People wanting to write library header files capable of interacting
*  with any SDK have a dilemma:  which to use, old- or new-style type
*  names?
*  
*     You can use old-style names by instructing users to include
*  `PalmCompatibility.h' before including your header file if they are
*  using the 3.5 SDK.  But this is not very satisfactory:  the onus
*  shouldn't be on users to get this right, and you might prefer to use
*  new-style names.
*  
*  You can ensure new-style names are available simply by including
*  `NewTypes.h' at the top of your header file, and instructing users to
*  include at least `Common.h'/`PalmTypes.h' before your header.  (They
*  are probably including `Pilot.h'/`PalmOS.h' anyway.)
*  
*  Including `NewTypes.h' provides the following:
*  
*  * The identifiers `UInt8', `Int8', `UInt16', `Int16', `UInt32',
*  `Int32', `MemPtr', and `MemHandle' are all typedeffed
*  appropriately.
*  
*  * The macro `SDK_VERSION' is defined to either 10, 20, 30, 31, or 35,
*  indicating the SDK version which is in use.
*  
***********************************************************************/

#ifndef NEWTYPES_H
#define NEWTYPES_H

#ifndef SDK_VERSION

/* 3.5 uses <PalmTypes.h> instead of <Common.h>.  */
#if defined(__PALMTYPES_H__)
#define SDK_VERSION  35

/* Otherwise, all versions of Common.h unconditionally #include
   <BuildRules.h>.  #defines corresponding to new functionality in
   each SDK were added to this file.  */

/* 3.1 added Japanese support.  */
#elif defined(LANGUAGE_JAPANESE)
#define SDK_VERSION  31

/* 3.0 introduced the v2 memory manager.  */
#elif defined(MEMORY_VERSION_2)
#define SDK_VERSION  30

/* 2.0 added Italian support.  */
#elif defined(LANGUAGE_ITALIAN)
#define SDK_VERSION  20

#else
#define SDK_VERSION  10
#endif

#endif


#if SDK_VERSION < 35
typedef Byte UInt8;
typedef SByte Int8;

/* The 3.0 and 3.1 SDKs already typedef the 16 and 32 bit types.  */
#if SDK_VERSION < 30
typedef Word UInt16;
typedef SWord Int16;
typedef DWord UInt32;
typedef SDWord Int32;
#endif

typedef VoidPtr MemPtr;
typedef VoidHand MemHandle;
#endif

#endif
