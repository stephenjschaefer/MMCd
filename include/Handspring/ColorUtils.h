/***************************************************************
 *
 * Project:
 *    ViewerUtils 
 *
 * Copyright info:
 *    Copyright Handspring, Inc. 1999.
 *
 * FileName:
 *    ColorUtils.h
 * 
 * Description:
 *    This file contains utility functions dealing with color.
 *
 * ToDo:
 *
 * History:
 *    23-may-2000 dia  Created by Douglas Anderson (dianders@handspring.com)
 *
 ****************************************************************/

#ifndef _COLOR_UTILS_H
#define _COLOR_UTILS_H


#include <FarCall.h>


//=============================================================================
// Types
//=============================================================================


typedef struct
{
  UInt32 fromDepths;    // Bitfield of depths to map from...
  UInt16 toDepth;       // Depth to map to (if supported)...
} 
HsUtilDepthMappingType;


// This is not registered or anything, but it should be safe to use 
// since it's only internal to your application...
#define hsUtilDepthMapRscType 'dMap'


//=============================================================================
// Functions
//=============================================================================

/***************************************************************
 *  Function:    HsUtilSetScreenDepth
 *
 *  Summary:
 *    Sets the screen depth using the table passed in.  
 *    Processes the table starting at element 0 until it finds
 *    an entry where the current depth is in the set of
 *    fromDepths and the toDepth is supported.
 *
 *  Parameters:
 *    numMappings       IN  Num entries in depthMappingsTbl.
 *    depthMappingsTbl  IN  A table of mappings from sets of
 *                          start depths to final depths.
 *    
 *  Returns:
 *    The final depth after the function has run.
 *  
 *  Called By: 
 *    client
 *
 *  History:
 *    23-may-2000 dia Created.
 *
 ****************************************************************/

typedef UInt16 HsUtilSetScreenDepthFnType (UInt16 numMappings, 
                                           const HsUtilDepthMappingType* depthMappingsTbl);
FAR_DECLARE (HsUtilSetScreenDepth);


/***************************************************************
 *  Function:    HsUtilSetScreenDepthFromRes
 *
 *  Summary:
 *    Calls HsUtilSetScreenDepth(), getting the depth mappings
 *	  from a resource of type 'dMap' with the given resource
 *	  ID.
 *
 *  Parameters:
 *	  resID	  IN  ID of a 'dMap' resource with the depth map
 *				  to use.
 *    
 *  Returns:
 *    The final depth after the function has run.
 *  
 *  Called By: 
 *    client
 *
 *  History:
 *    28-aug-2000 dia Created.
 *
 ****************************************************************/

typedef UInt16 HsUtilSetScreenDepthFromResFnType (UInt16 resID);
FAR_DECLARE (HsUtilSetScreenDepthFromRes);


#endif // _COLOR_UTILS_H
