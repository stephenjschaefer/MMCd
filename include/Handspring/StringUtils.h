/***************************************************************
 *
 * Project:
 *    ViewerUtils 
 *
 * Copyright info:
 *    Copyright Handspring, Inc. 1999.
 *
 * FileName:
 *    StringUtils.h
 * 
 * Description:
 *    This file contains utility functions dealing with strings
 *    and string resources. 
 *
 * ToDo:
 *
 * History:
 *    11-aug-1999 dia  Created by Douglas Anderson (danderson@handspring.com)
 *    17-dec-1999 dia  Made OS 3.5 header friendly (no functional changes)
 *	  31-mar-2000 dia  Added far-call versions of functions.
 *	  26-jun-2001 jbs added const qualifier where appropriate
 *
 ****************************************************************/

#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H


#include <FarCall.h>


/***************************************************************
 *  Function:    GetStringResource 
 *
 *  Summary:      
 *    Gets a string out of a resource.  If the given buffer is not
 *    NULL, copies the string into there.  If the given buffer is
 *    NULl, allocates just enough space for the string in the heap.
 *
 *  Parameters:
 *    id          IN    The ID of the string resource.
 *    buffer      OUT   The place to put the string--if NULL, allocate
 *                      memory on the heap.
 *    bufferSize  IN    The size of the buffer in bytes.
 *    
 *  Returns:
 *    A pointer to the string--if buffer was not NULL, this will be
 *    equal to buffer; otherwise it will point to heap memory.
 *    ...returns NULL if the resource doesn't exist.  
 *  
 *  History:
 *    26-Jul-1999 dia Created
 *    03-sep-1999 dia Fixed major bugs with passing NULL buffer (oops)
 ****************************************************************/

typedef Char* GetStringResourceFnType (UInt16 id, Char* buffer, UInt16 bufferSize);
FAR_DECLARE (GetStringResource);


/***************************************************************
 *  Function:    StrVariableSubstitute 
 *
 *  Summary:      
 *    Does variable substitution on the given string, putting the
 *    output in the given buffer.  The variables should be of the
 *    form "$(var)".  Variables are specified using as arguments
 *    (alternate variable name and definition).  If buffer space
 *    runs out, the output string is simply truncated.
 *
 *  Sample call:
 *    char buffer[64];
 *    StrVariableSubstitute (buffer, 64, "Welcome $(user)@$(host)",
 *                           2, "user", "doug", "host", "dilbert");
 *
 *  Parameters:
 *    buffer      OUT   The buffer to "print" to.
 *    bufferSize  IN    The size of the buffer.
 *    format      IN    The format string to replace variable in.
 *    numVars     IN    The number of variables specified in arguments.
 *    ...         IN    Alternating strings of variable and definition.
 *    
 *  Returns:
 *    The final length of the string.
 *
 *  Notes:
 *    * The format string is allowed to have any number of references
 *      to a variable, including 0.
 *    * The format string may refer to variables not passed in--these
 *      will be replaced by the empty string.
 *    * There is currently no way to specify "$(" as a literal.
 *    * Do not use "$$" in strings--it may be used later to quote $.
 *    * You cannot have recursive variables (variables that refer to
 *      themselves).
 *  
 *  History:
 *    26-Jul-1999 dia Created
 *    03-sep-1999 dia Made return length; made sure there are no
 *                    reads past the end of the format string.
 ****************************************************************/

#ifdef STRVARIABLESUBSTITUTE_SECTION
UInt16 StrVariableSubstitute (Char* buffer, UInt16 bufferSize, 
							  const Char* format, UInt16 numVars, ...) 
  __attribute__ ((section (STRVARIABLESUBSTITUTE_SECTION)));
#else // not STRVARIABLESUBSTITUTE_SECTION
typedef UInt16 StrVariableSubstituteFnType (Char* buffer, UInt16 bufferSize, 
											const Char* format, UInt16 numVars, ...);
FAR_DECLARE (StrVariableSubstitute);
#endif // not STRVARIABLESUBSTITUTE_SECTION



/***************************************************************
 *  Function:    StrSpn 
 *
 *  Summary:      
 *    Implementation of ANSI strspn() function, which
 *    returns the length of the prefix in str consisting
 *    of characters in set.
 *
 *  Parameters:
 *    str         IN    The string to search through.
 *    set         IN    The set of characters allowable in the
 *                      prefix.
 *    
 *  Returns:
 *    The length of the prefix.
 *  
 *  Notes:
 *    * As far as I know, this function should be multi-byte clean
 *      (since the functions it is based on are)
 *
 *  History:
 *    07-sep-1999 dia Created
 *	  16-jun-2001 jbs changed argument types to const Char*
 ****************************************************************/

typedef UInt16 StrSpnFnType (const Char* str, const Char* set);
FAR_DECLARE (StrSpn);


/***************************************************************
 *  Function:    StrCSpn 
 *
 *  Summary:      
 *    Implementation of ANSI strcspn() function, which
 *    returns the length of the prefix in str consisting
 *    of characters NOT in set.
 *
 *  Parameters:
 *    str         IN    The string to search through.
 *    set         IN    The set of characters NOT allowable
 *                      in the prefix.
 *    
 *  Returns:
 *    The length of the prefix.
 *  
 *  Notes:
 *    * As far as I know, this function should be multi-byte clean
 *      (since the functions it is based on are)
 *
 *  History:
 *    07-sep-1999 dia Created
 *	  16-jun-2001 jbs changed argument types to const Char*
 ****************************************************************/

typedef UInt16 StrCSpnFnType (const Char* str, const Char* set);
FAR_DECLARE (StrCSpn);


/***************************************************************
 *  Function:    StrDup 
 *
 *  Summary:      
 *    Implementation of ANSI strdup() function, which
 *    allocates memory in the heap and the copies the given
 *    string into it.
 *
 *  Parameters:
 *    src         IN    The string to copy.
 *    
 *  Returns:
 *    The copy, allocated with MemPtrNew
 *  
 *  Notes:
 *    * As far as I know, this function should be multi-byte clean.
 *
 *  History:
 *    07-sep-1999 dia Created
 *	  16-jun-2001 jbs changed argument type to const Char*
 ****************************************************************/

typedef Char* StrDupFnType (const Char* src);
FAR_DECLARE (StrDup);


/***************************************************************
 *  Function:    StrNCopyZ
 *
 *  Summary:      
 *    StrNCopy() the way it should be: it always terminates
 *    the string for you.  That is, it copies at most n-1
 *    characters and _always_ terminates.  The string passed in
 *    as the destination should be n bytes big.
 *
 *  Parameters:
 *    dstP        OUT   The buffer to copy into.
 *    srcP        IN    The string to copy.
 *    n           IN    The size of the destination buffer.
 *    
 *  Returns:
 *    dstP
 *  
 *  Notes:
 *    * Multi-byte clean assuming that terminating null is
 *      one byte big.
 *
 *  History:
 *    07-sep-1999 dia Created
 ****************************************************************/

typedef Char* StrNCopyZFnType (Char* dstP, const Char* srcP, UInt16 n);
FAR_DECLARE (StrNCopyZ);


/***************************************************************
 *  Function:    StrTruncateToWidth 
 *
 *  Summary:      
 *    Truncates the given string to the given number of pixels,
 *    adding an ellipsis to the end of it if truncated.
 *
 *  Parameters:
 *    str         IN/OUT  The string to truncate.
 *    width       IN      The number of pixels to truncate to.
 *    font        IN      The font to use in the calculation;
 *                        ...will not change the currently set font.
 *    
 *  Returns:
 *    The copy, allocated with MemPtrNew
 *  
 *  Notes:
 *    * As far as I know, this function should be multi-byte clean.
 *    * Requires OS 3.1, since it uses the new ellipsis characer.
 *    * If the width won't even handle the ellipsis, just makes
 *      str the empty string.
 *    * Assumes that there is space in the given string buffer
 *      to do ellipse truncation.  This should be fine _unless_
 *      the last character in the string is one byte and the ellipse
 *      character is two bytes, which isn't the case anywhere as far
 *      as I know.
 *
 *  DOLATER:
 *    * Make an alternate version of the function that 
 *      takes/returns the length.  Could make this function 
 *      use that one.
 *
 *  History:
 *    07-sep-1999 dia Created
 *    15-oct-1999 dia Made more multi-byte friendly (should be
 *                    completely multi-byte friendly now).
 *    15-oct-1999 dia Returns the width of the final string...
 *        
 ****************************************************************/
#ifdef STRVARIABLESUBSTITUTE_SECTION
Int16 StrTruncateToWidth (Char* str, Int16 fieldWidth, 
										FontID font)
  __attribute__ ((section (STRVARIABLESUBSTITUTE_SECTION)));
#else // not STRVARIABLESUBSTITUTE_SECTION

typedef Int16 StrTruncateToWidthFnType (Char* str, Int16 fieldWidth, 
										FontID font);
FAR_DECLARE (StrTruncateToWidth);

#endif // not STRVARIABLESUBSTITUTE_SECTION

/***************************************************************
 *  Function:    HsUtilWinDrawTruncCharsWithJust 
 *
 *  Summary:      
 *    A version of WinDrawTruncChars that adds justification.
 *    See WinDrawTruncChars for a full description.
 *
 *  Parameters:
 *    pChars    IN  The string to draw.
 *    length    IN  The length of pChars (in bytes).
 *    x         IN  The left coord to draw at.  Note that depending
 *                  on the width of the string, the maxWidth, and
 *                  the justification, drawing may start to the
 *                  right of this location.
 *    y         IN  The top coord to draw at.
 *    maxWidth  IN  The width of the "field".  The string will be
 *                  justified in this field.
 *    just      IN  The justification (justLeft, justRight, or
 *                  justCenter).
 *    
 *  Returns:
 *    nothing
 *  
 *  Notes:
 *    * As far as I know, this function should be multi-byte clean.
 *    * Requires OS 3.1, since it uses the new ellipsis characer.
 *    * If the width won't even handle the ellipsis, draws nothing.
 *
 *  History:
 *    15-oct-1999 dia Created based on DrawStringWithEllipsis from
 *                    Vitaly's FileMover (except that this one is
 *                    multibyte clean).
 *        
 ****************************************************************/

typedef enum
  {
	justLeft,
	justCenter,
	justRight
  } JustificationEnum;

typedef void HsUtilWinDrawTruncCharsWithJustFnType 
									 (const Char* pChars, UInt16 length, 
                                      Int16 x, Int16 y, Int16 maxWidth, 
                                      JustificationEnum just);
FAR_DECLARE (HsUtilWinDrawTruncCharsWithJust);

#endif // _STRING_UTILS_H
