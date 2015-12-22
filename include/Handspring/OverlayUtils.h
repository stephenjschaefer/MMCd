/***************************************************************
 *
 * Project:
 *    ViewerUtils 
 *
 * Copyright info:
 *    Copyright Handspring, Inc. 1999.
 *
 * FileName:
 *    OverlayUtils.h
 * 
 * Description:
 *    Utilities for doing pseudo-overlays until OS 3.5 becomes
 *    commonplace and we can use its facilities.
 *
 * ToDo:
 *
 * History:
 *    11-oct-1999   Created
 *    31-mar-2000   Added far-call versions of functions.
 *
 ****************************************************************/

#ifndef _OVERLAY_UTILS_H
#define _OVERLAY_UTILS_H


#include <FarCall.h>


//=============================================================================
// Equates
//=============================================================================

// These are #defines that are useful for overriding the language and/or
// country of the overlay functions.

// The creator code for the 'override' features.
// ...this code is actually registered as the creator of the "language spoofer"
// test program.
#define hsFtrCreatorOverlayOverrides        'HsLS'

// The feature id to override the country.
#define hsFtrIDCountryOverride              0

// The feature id to override the language.
#define hsFtrIDLanguageOverride             1


//=============================================================================
// Functions
//=============================================================================


/***************************************************************
 *  Function:    HsUtilOverlayInitialize
 * 
 *  Summary:
 *    Tries to open an overlay resource based on the language 
 *    and country that the ROM was compiled for.  If no overlay
 *    exists, doesn't open anything and just uses built-in
 *    resources.
 *
 *    The format of the overlay name is 
 *    $(nameP)_$(country)_$(language) where country and 
 *    language are numeric values.  See preferences.h (for 
 *    country) and buildrules.h (for language)
 *
 *    ...looks on all cards for the newest version, starting
 *    in RAM.
 * 
 *    ...if one is not found, it keys off of the language and
 *    tries to look for an overlay for the default country
 *    for that language (the logic specifying the default country
 *    is internal to this function).  
 *
 *    ...if no overlay is found, no overlay is opened.
 *  
 *  Parameters:
 *    baseStrP  IN  The base name to use when looking for
 *                  overlays.
 *     
 *  Returns: 
 *    The open ref of the overlay opened, or NULL.
 *  
 *  Called By:  
 *    client
 * 
 *  History: 
 *    08-oct-99 dia Created...
 ****************************************************************/


typedef DmOpenRef HsUtilOverlayInitializeFnType (Char* baseStrP);
FAR_DECLARE (HsUtilOverlayInitialize);


/***************************************************************
 *  Function:    HsUtilOverlayCleanup
 * 
 *  Summary:
 *    Closes an overlay opened with HsUtilOverlayInitialize()
 * 
 *  Parameters:
 *    openRefP  IN  The DmOpenRef returned from 
 *                  HsUtilOverlayInitialize().  Ok to call with NULL.
 *     
 *  Returns: 
 *    nothing
 *  
 *  Called By:  
 *    client
 * 
 *  History: 
 *    08-oct-99 dia Created...
 ****************************************************************/

typedef void HsUtilOverlayCleanupFnType (DmOpenRef openRefP);
FAR_DECLARE (HsUtilOverlayCleanup);


#endif // _OVERLAY_UTILS_H
