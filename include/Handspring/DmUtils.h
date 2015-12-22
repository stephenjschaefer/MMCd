/***************************************************************
 *
 * Project:
 *    ViewerUtils 
 *
 * Copyright info:
 *    Copyright Handspring, Inc. 1999.
 *
 * FileName:
 *    DmUtils.h
 * 
 * Description:
 *    Utilities for database manager stuff.
 *
 * ToDo:
 *
 * History:
 *    11-oct-1999 - Created by Douglas Anderson (danderson@handspring.com)
 *
 ****************************************************************/

#ifndef _DM_UTILS_H
#define _DM_UTILS_H

/***************************************************************
 *  Function:    HsUtilDmFindDatabaseMulticard 
 * 
 *  Summary:     
 *    A version of DmFindDatabase that is multicard.  Finds the
 *    database with the given name that has the greatest version,
 *    searching all cards.  If there's a tie, the lower numbered
 *    card wins.
 * 
 *  Parameters:
 *    nameP   IN  The name of the database to search for.
 *    cardNoP OUT The card number that the database was found on.
 *                Untouched upon err.
 *    dbIdP   OUT The local id of the found database.  Untouched
 *                upon err.
 *     
 *  Returns: 
 *    dmErrCantFind if no matches were found; 0 upon success.
 *  
 *  Called By:  
 *    client
 * 
 *  History: 
 *    08-oct-99 dia Created...
 ****************************************************************/

Err HsUtilDmFindDatabaseMulticard (CharPtr nameP, UInt16* cardNoP, LocalID* dbIdP);

#endif // _DM_UTILS_H
