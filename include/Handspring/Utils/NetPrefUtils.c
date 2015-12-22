/***************************************************************
 *
 *  Project:
 *	  NetPref library
 *
 * Copyright info:
 *
 *	  Copyright (c) Handspring 2002 -- All Rights Reserved
 *
 *
 *  FileName:
 *	  NetPrefUtils.c
 * 
 *  Description:
 *
 *	  NetPref library's public utilities -- this file may be
 *	  linked with your application or library to provide access
 *	  to a set of convenient helper functions.
 *
 *	Note:
 *
 *	History:
 *	  7-Feb-2002	vmk	  Created by Vitaly Kruglikov 
 *
 ****************************************************************/

#include <PalmOS.h>
#include <PalmTypes.h>

#include <UIResources.h>
#include <Event.h>
#include <CharAttr.h>

//#include "NetServiceAPI.h"
#include <ConnectionMgr.h>

#include <HsExt.h>

//#include <HsCreators.h>	  // for hsFileCVirtualModemSerLib and friends

#include "NetPrefLibrary.h"
#include "NetPrefUtils.h"




#pragma mark ------NetPrefUtils-------


/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetPrefLibLink
 *
 * DESCRIPTION:  "Link" with the NetPref library
 *
 * PARAMETERS:   	netPrefLibP OUT linked NetPref lib on success, undefined
 *					 on failure.
 *
 *	CALLED BY:		
 *
 * RETURNED:     true if successful, false if not
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/3/2002	created
 *
 ***********************************************************************/
extern Boolean
NetPrefUtilNetPrefLibLink (NetPrefUtilNetPrefLibType* netPrefLibP)
{
  Boolean	isSuccessful = false;
  Err		err = 0;
  UInt16	refNum = 0;
  struct NetPrefContextTypeTag* cxtP = NULL;

  ErrNonFatalDisplayIf (!netPrefLibP, "null arg");
  ErrNonFatalDisplayIf (netPrefLibP->linkSignature
						  == netPrefUtilNetPrefLinkSignature,
						"NetPref lib already linked");


  err = SysLibLoad (netPrefLibTypeID, netPrefLibCreatorID,
					&refNum);
  if (err)
	{
	  ErrNonFatalDisplay ("failed to load NetPrefLib");
	  goto Exit;
	}

  err = NetPrefLibOpen (refNum, &cxtP);
  if (err)
	{
	  ErrNonFatalDisplay ("failed to open NetPrefLib");
	  goto Exit;
	}

  // "Construct" the NetPref lib "instance"
  NetPrefUtilNetPrefInstanceConstruct (refNum, cxtP, netPrefLibP);
  isSuccessful = true;

Exit:
  if (err)
	{
	  if (refNum != 0)
		SysLibRemove (refNum);

	  MemSet (netPrefLibP, sizeof(*netPrefLibP), 0);

	}

  return (isSuccessful);

} // NetPrefUtilNetPrefLibLink




/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetPrefLibUnlink
 *
 * DESCRIPTION:  "Unlink" from the NetPref library
 *
 * PARAMETERS:   	netPrefLibP - successfully linked NetPref lib
 *
 *	CALLED BY:		
 *
 * RETURNED:     nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/3/2002	created
 *
 ***********************************************************************/
extern void
NetPrefUtilNetPrefLibUnlink (NetPrefUtilNetPrefLibType* netPrefLibP)
{
  ErrNonFatalDisplayIf (!netPrefLibP, "null arg");

  ErrNonFatalDisplayIf (netPrefLibP->refNum == 0, "netPrefLib not loaded");
  ErrNonFatalDisplayIf (netPrefLibP->cxtP == NULL, "netPrefLib not opened");

  if (netPrefLibP->linkSignature != netPrefUtilNetPrefLinkSignature)
	{
	  ErrNonFatalDisplay ("NetPref lib wasn't linked");
	  goto Exit;
	}

  if (netPrefLibP->refNum)
	{
	  if (netPrefLibP->cxtP)
		NetPrefLibClose (netPrefLibP->refNum, netPrefLibP->cxtP);

	  SysLibRemove (netPrefLibP->refNum);
	}

  MemSet (netPrefLibP, sizeof(*netPrefLibP), 0);

Exit:
  return;
} // NetPrefUtilNetPrefLibUnlink


/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetPrefInstanceConstruct
 *
 * DESCRIPTION:  
 *	  Given an open NetPref library refNum and context pointer, initialize
 *	  a NetPref library Instance structure that may be used with other
 *	  NetPrefUtil API's
 *
 * PARAMETERS:   	
 *	  netPrefLibRefNum	IN	refNum of an already opened NetPref library
 *	  netPrefLibCxtP	IN	cxtP of an already opened NetPref library
 *	  netPrefLibP		OUT linked NetPref lib on success, undefined
 *							 on failure.
 *
 *	CALLED BY:		
 *
 * RETURNED:     nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		3/21/2002	created
 *
 ***********************************************************************/
extern void
NetPrefUtilNetPrefInstanceConstruct (UInt16 netPrefLibRefNum,
									 struct NetPrefContextTypeTag* netPrefLibCxtP,
									 NetPrefUtilNetPrefLibType* netPrefLibP)
{
  ErrNonFatalDisplayIf (!netPrefLibRefNum || !netPrefLibCxtP
						|| !netPrefLibP, "null arg");


  // Set up netPrefLib instance so we may use NetPrefUtil API
  MemSet (netPrefLibP, sizeof(*netPrefLibP), 0);
  netPrefLibP->refNum = netPrefLibRefNum;
  netPrefLibP->cxtP = netPrefLibCxtP;
  netPrefLibP->linkSignature = netPrefUtilNetPrefLinkSignature;

  return;

} // NetPrefUtilNetPrefInstanceConstruct



/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetPrefFixedWidthGet
 *
 * DESCRIPTION:  Get a fixed width field value from a record
 *
 * PARAMETERS:   	netPrefLibP - 
 *					recP - 
 *					recFieldID	-
 *					numBytes - width of requested data in # of bytes	
 *					resultP	- output value, set to zero if not available
 *					viewFlagsP - OUT field's view flags -- OPTIONAL - set to
 *								  NULL to ignore
 *
 *	CALLED BY:		
 *
 * RETURNED:     true if available (set), false if not
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/3/2002	created
 *
 ***********************************************************************/
static Boolean
NetPrefUtilNetPrefFixedWidthGet (NetPrefUtilNetPrefLibType* netPrefLibP,
								struct NetPrefRecordTypeTag* recP,
								NetPrefRecFieldEnum recFieldID,
								UInt32 numBytes, void* resultP,
								NetPrefRecFieldViewType* viewFlagsP)
{
  void*		dataP = NULL;
  UInt32	fieldLen;
  NetPrefRecFieldViewType viewFlags;
  Err		err = 0;
  Boolean	gotIt = false;

  ErrNonFatalDisplayIf (!resultP, "null arg");

  // Init result buffer to 0
  MemSet (resultP, numBytes, 0);

  if (viewFlagsP)
	*viewFlagsP = 0;

  // Get the field from the record object
  err = NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP,
							recP, recFieldID, &dataP, &fieldLen,
							&viewFlags);
  if (err)
	goto Exit;

  if (viewFlagsP)
	*viewFlagsP = viewFlags;

  if (!dataP)
	goto Exit;

  ErrNonFatalDisplayIf (fieldLen != numBytes, "unexp fld len");

  // Set result from the record field
  MemMove (resultP, dataP, numBytes);

  gotIt = true;

Exit:
  return (gotIt);
} // NetPrefUtilNetPrefFixedWidthGet



/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetPrefLongGet
 *
 * DESCRIPTION:  Get a UInt32 field value from a record
 *
 * PARAMETERS:   	netPrefLibP - 
 *					recP - 
 *					recFieldID	-	
 *					resultP	- value, set to zero if not available
 *					viewFlagsP - OUT field's view flags -- OPTIONAL - set to
 *								  NULL to ignore
 *
 *	CALLED BY:		
 *
 * RETURNED:     true if available (set), false if not
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/3/2002	created
 *
 ***********************************************************************/
extern Boolean
NetPrefUtilNetPrefLongGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						  struct NetPrefRecordTypeTag* recP,
						  NetPrefRecFieldEnum recFieldID,
						  UInt32* resultP,
						  NetPrefRecFieldViewType* viewFlagsP)
{
  Boolean	gotIt;

  gotIt = NetPrefUtilNetPrefFixedWidthGet (netPrefLibP, recP, recFieldID,
										  sizeof(*resultP), resultP,
										  viewFlagsP);
  return (gotIt);
} // NetPrefUtilNetPrefLongGet


/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetPrefShortGet
 *
 * DESCRIPTION:  Get a UInt16 field value from a record
 *
 * PARAMETERS:   	netPrefLibP - 
 *					recP - 
 *					recFieldID	-	
 *					resultP	- value, set to zero if not available
 *
 *	CALLED BY:		
 *
 * RETURNED:     true if available (set), false if not
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/4/2002	created
 *
 ***********************************************************************/
extern Boolean
NetPrefUtilNetPrefShortGet (NetPrefUtilNetPrefLibType* netPrefLibP,
							struct NetPrefRecordTypeTag* recP,
							NetPrefRecFieldEnum recFieldID,
							UInt16* resultP,
							NetPrefRecFieldViewType* viewFlagsP)
{
  Boolean	gotIt;

  gotIt = NetPrefUtilNetPrefFixedWidthGet (netPrefLibP, recP, recFieldID,
										  sizeof(*resultP), resultP,
										  viewFlagsP);
  return (gotIt);
} // NetPrefUtilNetPrefShortGet


/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetPrefByteGet
 *
 * DESCRIPTION:  Get a UInt8 field value from a record
 *
 * PARAMETERS:   	netPrefLibP - 
 *					recP - 
 *					recFieldID	-	
 *					resultP	- value, set to zero if not available
 *
 *	CALLED BY:		
 *
 * RETURNED:     true if available (set), false if not
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/4/2002	created
 *
 ***********************************************************************/
extern Boolean
NetPrefUtilNetPrefByteGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						  struct NetPrefRecordTypeTag* recP,
						  NetPrefRecFieldEnum recFieldID,
						  UInt8* resultP,
						  NetPrefRecFieldViewType* viewFlagsP)
{
  Boolean	gotIt;

  gotIt = NetPrefUtilNetPrefFixedWidthGet (netPrefLibP, recP, recFieldID,
										  sizeof(*resultP), resultP,
										  viewFlagsP);
  return (gotIt);
} // NetPrefUtilNetPrefByteGet



/**************************************************************
 *	Function:	  NetPrefUtilFieldViewGet
 *
 *	Summary:	  
 *	  Returns field view flags for a given field of the given record.
 *
 *	Parameters:
 *	  netPrefLibP		  IN  NetPref library instance
 *	  recP				  IN  NetPref record object
 *	  recFieldID		  IN  NetPref record field ID
 *
 *	Returns:
 *	  NetPrefRecFieldViewType
 *	
 *	Called By: 
 *	  application
 *	
 *	History:
 *	  03-Apr-2002	vmk	  Created
 *
 ****************************************************************/
extern NetPrefRecFieldViewType
NetPrefUtilFieldViewGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						 struct NetPrefRecordTypeTag* recP,
						 NetPrefRecFieldEnum recFieldID)
{
  NetPrefRecFieldViewType	fieldViewFlags = 0;
  void*						fieldValueP;
  UInt32	fieldLen;


  NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
					  recFieldID, &fieldValueP, &fieldLen,
					  &fieldViewFlags);


  return (fieldViewFlags);
} // NetPrefUtilFieldViewGet



/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilLongDataSetAsFlags
 *
 * DESCRIPTION:  Set a 32-bit field data value as flags. The clear
 *				 operation is performed before the set operation.  To
 *				 skip either operation, set the corresponding parameter to 0.
 *
 * PARAMETERS:   	netPrefLibP - 
 *					recP - 
 *					recFieldID	-	
 *					resultP	- value, set to zero if not available
 *					flagsToClear  IN flags to clear, or 0 to skip
 *					flagsToSet	  IN flags to set, or 0 to skip
 *
 *	CALLED BY:		
 *
 * RETURNED:     true if available (set), false if not
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/4/2002	created
 *
 ***********************************************************************/
extern Err
NetPrefUtilLongDataSetAsFlags (NetPrefUtilNetPrefLibType* netPrefLibP,
							   struct NetPrefRecordTypeTag* recP,
							   NetPrefRecFieldEnum recFieldID,
							   UInt32 flagsToClear, UInt32 flagsToSet)
{
  Err		err = 0;
  UInt32	currentValue;


  // Get the current value
  NetPrefUtilNetPrefLongGet (netPrefLibP, recP, recFieldID, &currentValue,
							 NULL);

  // First clear
  currentValue &= ~flagsToClear;


  // Then set
  currentValue |= flagsToSet;
  

  // Save the new value to the record
  err = NetPrefRecFieldSet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
							recFieldID, &currentValue, sizeof(currentValue));

  return (err);
} // NetPrefUtilLongDataSetAsFlags


/***************************************************************
 *	Function:	  NetPrefUtilNetPrefRecBinHandleSet
 *
 *	Summary:	  Set a record object binary field from a PalmOS
 *				  memory handle.
 *
 *	Parameters:
 *	  netPrefLibP	  IN  Open NetPref library context 
 *	  recP			  IN/OUT  record object
 *	  recFieldID	  IN	  field ID -- NetPrefRecFieldEnum
 *	  dataH			  IN	  data handle
 *
 *	Returns:
 *	  0 on success
 *	
 *	Called By: 
 *	  Internal
 *	
 *	Notes:
 *	
 *	History:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		2/11/2002	created
 *
 ****************************************************************/
extern Err
NetPrefUtilNetPrefRecBinHandleSet (NetPrefUtilNetPrefLibType* netPrefLibP,
								struct NetPrefRecordTypeTag* recP,
								NetPrefRecFieldEnum recFieldID,
								MemHandle dataH)
{
  Err	  err = 0;
  void*	  dataP = NULL;
  UInt32  dataSize = 0;

  ErrNonFatalDisplayIf (!netPrefLibP, "null arg");

  if (dataH)
	{
	  dataP = MemHandleLock (dataH);
	  dataSize = MemPtrSize(dataP);
	}

  err = NetPrefRecFieldSet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
							recFieldID, dataP,
							dataSize);
  if (dataP)
	MemPtrUnlock (dataP);

  return (err);
} // NetPrefUtilNetPrefRecBinHandleSet


/***************************************************************
 *	Function:	  NetPrefUtilNetPrefRecStrHandleSet
 *
 *	Summary:	  Set a record object string field from a PalmOS memory handle.
 *
 *	Parameters:
 *	  netPrefLibP	  IN  Open NetPref library context 
 *	  recP			  IN/OUT  record object
 *	  recFieldID	  IN	  field ID -- NetPrefRecFieldEnum
 *	  dataH			  IN	  data handle
 *
 *	Returns:
 *	  0 on success
 *	
 *	Called By: 
 *	  Internal
 *	
 *	Notes:
 *	
 *	History:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/7/2002	created
 *
 ****************************************************************/
extern Err
NetPrefUtilNetPrefRecStrHandleSet (NetPrefUtilNetPrefLibType* netPrefLibP,
								struct NetPrefRecordTypeTag* recP,
								NetPrefRecFieldEnum recFieldID,
								MemHandle dataH)
{
  Err	  err = 0;
  char*	  dataP = NULL;
  UInt32  dataSize = 0;

  ErrNonFatalDisplayIf (!netPrefLibP, "null arg");

  if (dataH)
	{
	  dataP = MemHandleLock (dataH);
	  dataSize = StrLen(dataP) + 1;
	}

  err = NetPrefRecFieldSet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
							recFieldID, dataP,
							dataSize);
  if (dataP)
	MemPtrUnlock (dataP);

  return (err);
} // NetPrefUtilNetPrefRecStrHandleSet



/***************************************************************
 *	Function:	  NetPrefUtilConnectionInfoGet
 *
 *	Summary:	  Get the port ID and modem status of the connection
 *				  referenced by the record object.
 *
 *	Parameters:
 *	  netPrefLibP	  IN  Open NetPref library context 
 *	  recP			  IN  record object
 *
 *	Returns:
 *	  true if got information, false if connection is not in the Connection
 *	  Manager database
 *	
 *	Called By: 
 *	  Internal
 *	
 *	Notes:
 *	
 *	History:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/28/2002	created
 *
 ****************************************************************/
extern Boolean
NetPrefUtilConnectionInfoGet (NetPrefUtilNetPrefLibType* netPrefLibP,
							  struct NetPrefRecordTypeTag* recP,
							  UInt32* portCreatorIDP, Boolean* isModemP)
{
  Boolean	  gotIt = false;
  Err		  palmOSErr;
  UInt32	  fieldLen;
  NetPrefRecFieldViewType fieldViewFlags;
  Char*		  connNameP = NULL;
  UInt32	  port = 0;
  Boolean	  isModem = true;
  NetPrefSvcMediumEnum	  svcMediumAlias = netPrefSvcMediumUnknown;

  ErrNonFatalDisplayIf (!netPrefLibP || !portCreatorIDP || !isModemP, "null arg");

  *portCreatorIDP = 0;
  *isModemP = false;


  // Check the service medium alias first
  // Check if this entry uses a Service Medium Alias
  NetPrefUtilNetPrefShortGet (netPrefLibP, recP,
							  netPrefRecFieldSvcMediumAlias,
							  &svcMediumAlias, NULL);

  switch (svcMediumAlias)
	{
	  case netPrefSvcMedium1xRTT:
	  case netPrefSvcMediumGPRS:
	  case netPrefSvcMediumQNC:
	  case netPrefSvcMediumGISDN:
	  case netPrefSvcMediumGCSD:
	  case netPrefSvcMediumCCSD:
		{
		  UInt32  builtInWirelessPortID;
		  NetPrefUtilPlatformEnum	platform;

		  platform = NetPrefUtilPlatformGet ();
		  builtInWirelessPortID = NetPrefUtilDefWirelessDriverIDGet (platform);

		  if (builtInWirelessPortID)
			{
			  *portCreatorIDP = builtInWirelessPortID;
			  *isModemP = true;
			  gotIt = true;
			}
		  
		}
		break;

	  default:
		break;
	}

  if (gotIt)
	goto Exit;


  // Get the connection name
  NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP,
					  recP, netPrefRecFieldConnection,
					  (void**)&connNameP, &fieldLen, &fieldViewFlags);

  if (!connNameP)
	{
	  // NOTE... if connName isn't set, we may want to default it to one

	  goto Exit;
	}


  // Query the serial driver port id and capability (modem/serial);
  palmOSErr = CncGetProfileInfo (connNameP, &port, NULL, NULL, NULL, NULL,
								 NULL, &isModem, NULL);
  if (palmOSErr)
	{
	  // NOTE... consider fixing up the connection name if CncGetProfileInfo
	  //  fails -- it may fail most likely when the user deletes a connection
	  //  that this service was referencing.

	  goto Exit;
	}

  *portCreatorIDP = port;
  *isModemP = isModem;

  gotIt = true;

Exit:
  return (gotIt);
} // NetPrefUtilConnectionInfoGet


/***************************************************************
 *	Function:	  NetPrefUtilRecMediumGet
 *
 *	Summary:	  Determine the record's "medium" (NetPrefSvcMediumEnum).
 *				  Determine based on Connection name and/or
 *				  the value of the netPrefRecFieldMedium.
 *				  The precedence goes to netPrefRecFieldMedium.
 *
 *	Parameters:
 *	  netPrefLibP	  IN  Open NetPref library context 
 *	  recP			  IN  record object
 *
 *	Returns:
 *	  NetPrefSvcMediumEnum
 *	
 *	Called By: 
 *	  Internal
 *	
 *	Notes:
 *	
 *	History:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/22/2002	created
 *
 ****************************************************************/
extern NetPrefSvcMediumEnum
NetPrefUtilRecMediumGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						 struct NetPrefRecordTypeTag* recP,
						 UInt32 wirelessDriverID)
{
  NetPrefSvcMediumEnum	svcMedium = netPrefSvcMediumUnknown;
  Boolean	  gotIt;
  UInt32	  fieldLen;
  NetPrefRecFieldViewType fieldViewFlags;
  Char*		  connNameP = NULL;
  UInt32	  port = 0;
  Boolean	  isModem = true;
  NetPrefUtilPlatformEnum	platform;

  ErrNonFatalDisplayIf (!netPrefLibP, "null arg");


  // Check if netPrefRecFieldMedium field has been set
  gotIt = NetPrefUtilNetPrefShortGet (netPrefLibP, recP,
									  netPrefRecFieldSvcMediumAlias,
									  &svcMedium, NULL);
  if (gotIt)
	goto Exit;


  // Determine medium based on connection

  // Get the platform
  platform = NetPrefUtilPlatformGet ();

  // Get the connection name
  NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP,
						recP, netPrefRecFieldConnection,
						(void**)&connNameP, &fieldLen, &fieldViewFlags);

  if (!connNameP)
	{
	  // NOTE... if connName isn't set, we may want to choose a valid default

	  svcMedium = netPrefSvcMediumDialUp; // assume dial-up if no name
										  // (should never happen)
	  goto Exit;
	}


  // Get the serial/virtual driver port ID and modem capability
  gotIt = NetPrefUtilConnectionInfoGet (netPrefLibP, recP, &port, &isModem);
  if (!gotIt)
	{
	  svcMedium = netPrefSvcMediumDialUp; // assume dial-up if failed
										  // (should never happen)
	  goto Exit;
	}


  // If the driver is the Handspring Virtual Modem, try to identify
  //  "well-known" substrings in the name
  // NOTE... this may be a temporary hack until a better method is devised.
  //  (this is useful for initial simulation of these services)
  if (port == wirelessDriverID)
	{
	  if (StrStr (connNameP, "GPRS"))
		{
		  svcMedium = netPrefSvcMediumGPRS;
		}
	  else if (StrStr (connNameP, "CSD"))
		{
		  if (platform == netPrefUtilPlatformGSM)
			svcMedium = netPrefSvcMediumGCSD;
		  else
		  if (platform == netPrefUtilPlatformCDMA)
			svcMedium = netPrefSvcMediumCCSD;
		  else
			{
			  ErrNonFatalDisplay ("unexp pltfm");
			}
		}
	  else if (StrStr (connNameP, "UDI"))
		{
		  svcMedium = netPrefSvcMediumGISDN;
		}
	  else if (StrStr (connNameP, "1xRTT"))
		{
		  svcMedium = netPrefSvcMedium1xRTT;
		}
	  else if (StrStr (connNameP, "QNC"))
		{
		  svcMedium = netPrefSvcMediumQNC;
		}
	  else
		{
		  svcMedium = netPrefSvcMediumDialUp;
		}

	  goto Exit;
	}


  // If some other driver, base the decision on whether it communicates
  //  with a modem or not
  if (isModem)
	svcMedium = netPrefSvcMediumDialUp;
  else
	svcMedium = netPrefSvcMediumDirect;


Exit:

  return (svcMedium);
} // NetPrefUtilRecMediumGet


/***********************************************************************
 *
 * FUNCTION:	NetPrefUtilNetPrefIPAddrSet
 *
 * DESCRIPTION:	Set the value of an IP address field in the record object.
 *				If the IP address is 0, saves a value of 0 length to
 *				optimize memory access performance
 *
 * PARAMETERS:		netPrefLibP - NetPref lib instance
 *					recP  - record object
 *					recFieldID - record object field ID to set
 *					ipAddr - IP address
 *
 * RETURNED:	nothing.
 *
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/24/02		Created
 *
 ***********************************************************************/
extern Err
NetPrefUtilNetPrefIPAddrSet (NetPrefUtilNetPrefLibType* netPrefLibP,
							struct NetPrefRecordTypeTag* recP,
							NetPrefRecFieldEnum recFieldID, UInt32 ipAddr)
{
  Err		  err = 0;
  UInt32*	  ipAddrP;
  UInt32	  fieldLen;

  if (ipAddr != 0)
	{
	  ipAddrP = &ipAddr; fieldLen = sizeof(ipAddr);
	}
  else
	{
	  ipAddrP = NULL; fieldLen = 0;
	}

  err = NetPrefRecFieldSet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
							recFieldID, ipAddrP, fieldLen);

  return (err);
} // NetPrefUtilNetPrefIPAddrSet



// .
/***********************************************************************
 *
 * FUNCTION:	  NetPrefUtilZStringListSize
 *
 * DESCRIPTION:	  Return the length of a zero-terminated string list.  A
 *				  zero-terminated string list consists of zero-terminated
 *				  strings and ends with an empty string.  The size includes.
 *				  all of the zero-terminators, including the list terminator.
 *
 * PARAMETERS:	  zStrListP - pointer to the zero-terminated sting list
 *
 * RETURNED:     errors
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		2/11/02		Created based on Palm's SetPrefStr from
 *								Network Panel.
 ***********************************************************************/
extern UInt32
NetPrefUtilZStringListSize (char* zStrListP)
{
  UInt32          len;


  len = 0;

  ErrNonFatalDisplayIf (!zStrListP, "null arg");
  while (*zStrListP)
	{
	  len += StrLen (zStrListP) + 1;
	  zStrListP += StrLen (zStrListP) + 1;
	}
  return len + 1;
} // NetPrefUtilZStringListSize




/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetConfigStrSet
 *
 * DESCRIPTION:  This is a little utility function for setting network 
 *				  options that are saved as strings.  Also handles
 *				  netIFSettingLoginScript as a special case
 *
 * PARAMETERS:	  strP - handle to the string to store
 *				  id -  of the net setting
 *				  netLibRefnum - network library refNum
 *				  interface - interface id ('ppp-' or 'slip' - 0 for generic)
 *
 * RETURNED:     errors
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		2/11/02		Created based on Palm's SetPrefStr from
 *								Network Panel.
 ***********************************************************************/
extern Err
NetPrefUtilNetConfigStrSet (char* strP, UInt16 netSettingID,
							UInt16 netLibRefnum, UInt32 netInterface)
{
  Err		  err = 0;
  UInt16	  len = 0;

  if (strP)
	{
	  // Special case for login script
	  if (netInterface && netSettingID == netIFSettingLoginScript)
		len = NetPrefUtilLoginScriptLen (strP);

	  // Normal strings:
	  else
		{
		  if (*strP)
			len = StrLen (strP) + 1;
		}
	}

  if (netInterface)
	err = NetLibIFSettingSet (netLibRefnum, netInterface, 0,
								netSettingID, strP, len);
  else
	err = NetLibSettingSet (netLibRefnum, netSettingID, strP, len);


  return (err);
} // NetPrefUtilNetConfigStrSet


/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilNetConfigStrSetFromRec
 *
 * DESCRIPTION: Utility function for getting a string setting from a
 *				record object and using it to set a NetLib option. 
 *				Also handles netIFSettingLoginScript as a special case.
 *
 * PARAMETERS:   	netPrefLibP - 
 *					recP - 
 *					recFieldID	-
 *					netSettingID - 	
 *					netLibRefnum - 	
 *					netInterface - interface id ('ppp-' or 'slip' - 0 for
 *									generic)
 *					force - if false, will not alter the option if the field
 *					 is NULL; if true, will set it regardless
 *
 * RETURNED:     nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		2/11/02		Created.
 *
 *		
 *
 ***********************************************************************/
extern void
NetPrefUtilNetConfigStrSetFromRec (NetPrefUtilNetPrefLibType* netPrefLibP,
								   struct NetPrefRecordTypeTag* recP,
								   NetPrefRecFieldEnum recFieldID,
								   UInt16 netSettingID, UInt16 netLibRefnum,
								   UInt32 netInterface, Boolean force)
{
  char* strP = NULL;
  UInt32	fieldLen;
  NetPrefRecFieldViewType viewFlags;

  NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP,
					  recP, recFieldID, (void**)&strP,
					  &fieldLen, &viewFlags);
  if (force || strP)
	NetPrefUtilNetConfigStrSet (strP, netSettingID, netLibRefnum,
								netInterface);

  return;
} // NetPrefUtilNetConfigStrSetFromRec


/***********************************************************************
 *
 * FUNCTION:     NetPrefUtilPhoneStringCompose
 *
 * DESCRIPTION:	Compose a phone string from the cached Sync preferences
 *						structure;  contcatenate the dial prefix, disable
 *						call waiting, calling card number, and the phone number
 *
 * PARAMETERS:	destP, a string buffer to hold the complete number
 *					maxLen. the allocated length of destP (-1)
 *					phonePref - the phone preferences structure
 *
 * RETURNED:	nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		2/11/02		Created based on Palm's ComposePhoneString
 *								from Network Panel.
 *
 *		
 *
 ***********************************************************************/
extern void
NetPrefUtilPhoneStringCompose (Char* destP, UInt16 maxLen,
						   NetPrefUtilNetPrefLibType* netPrefLibP,
						   struct NetPrefRecordTypeTag* recP)
{
  Char*           str;
  NetPrefUtilPhoneInfoType	  phoneInfo;

  *destP = 0;
 
  // Load phone info from record (for convenience)
  NetPrefUtilPhoneInfoGet (netPrefLibP, recP, &phoneInfo);

  if (phoneInfo.flags.dialPrefixEnabled && phoneInfo.prefixP)
	{
	  str = phoneInfo.prefixP;
	  StrNCat (destP, str, maxLen);
	}
  if (phoneInfo.flags.callWaitingEnabled && phoneInfo.callWaitingP)
	{
	  str = phoneInfo.callWaitingP;
	  StrNCat (destP, str, maxLen);
	}
  if (phoneInfo.phoneP)
	{
	  str = phoneInfo.phoneP;
	  StrNCat (destP, str, maxLen);
	}
  if (phoneInfo.flags.callingCardEnabled && phoneInfo.callingCardP)
	{
	  str = phoneInfo.callingCardP;
	  StrNCat (destP, str, maxLen);
	}


  return;
} // NetPrefUtilPhoneStringCompose







/***********************************************************************
 *
 * Function:	NetPrefUtilPlatformGet
 *
 * Summary:		Determine on which platform we are running -- CDMA or GSM
 *
 * Parameters:
 *	  nothing
 *
 * Returns: 
 *    NetPrefUtilPlatformEnum or netPrefUtilPlatformUnknown if could not
 *	  identify
 *  
 * Called By:  
 *
 * History: 
 *	  07-Feb-2002	vmk	  Created
 *	  08-Apr-2002	vmk	  Fixed bug #12884: Use HsAttrGet for PhoneType.
 *	  08-Apr-2002	vmk	  POSE bug workaround: on POSE, check the
 *						   phone library instead of hsAttrPhoneType.
 *	  23-May-2002	vmk	  Removed old hack that used library names and
 *						   creators to determine platform ID.
 *
 ***********************************************************************/
extern NetPrefUtilPlatformEnum
NetPrefUtilPlatformGet (void)
{
  Err	  palmOSErr = 0;
  UInt32  platformType = 0;
  NetPrefUtilPlatformEnum	  platform = netPrefUtilPlatformUnknown;

#if 0
  UInt16  refNum;
  char*	  cdmaPhoneLibNameP = "PhoneLib.prc";	// NOTE... use constants
  UInt32  cdmaPhoneLibDbCreator = /*'APaf' CHANGED!*/;
  char*	  gsmPhoneLibNameP = "GSMLibrary.lib";	//  from lib headers
  UInt32  gsmPhoneLibDbCreator = 'GSM!';
#endif




  // Get the platform ID from Handspring Extensions
  palmOSErr = HsAttrGet (hsAttrPhoneType, 0/*param*/, &platformType);
  ErrNonFatalDisplayIf (palmOSErr, "HsAttrGet(hsAttrPhoneType) failed");
  if (!palmOSErr)
	{
	  if (platformType == hsAttrPhoneTypeCDMA)
		platform = netPrefUtilPlatformCDMA;
	  else
	  if (platformType == hsAttrPhoneTypeGSM)
		platform = netPrefUtilPlatformGSM;
	  else
		{
		  ErrNonFatalDisplay ("unexp pltfmType");
		}
	}


#if 0 // CASTRATION -- not needed any more, or so we think

  // The old, pre-London POSE incorrectly reports hsAttrPhoneType
  //  as netPrefUtilPlatformGSM.  So, we need to
  //  use our own hueristics on POSE until this is fixed. Here, we
  //  validate the above platform determination when running on POSE.


  if (HostGetHostID() != hostIDPalmOS)
	{
	  // Hack method -- check for presense of the currently platform-specific
	  //  phone library

	  // Check if CDMA phone library is installed in the system
	  palmOSErr = SysLibFind (cdmaPhoneLibNameP, &refNum);
	  if (!palmOSErr)
		{
		  ErrFatalDisplayIf (platform != netPrefUtilPlatformCDMA,
							 "Switch to CDMA (London) POSE");
		  platform = netPrefUtilPlatformCDMA;
		  goto Exit;
		}
  
	  // Check if GSM phone library is installed in the system
	  palmOSErr = SysLibFind (gsmPhoneLibNameP, &refNum);
	  if (!palmOSErr)
		{
		  ErrFatalDisplayIf (platform != netPrefUtilPlatformGSM,
							 "Switch to GSM (Treo/Atlanta) POSE");
		  platform = netPrefUtilPlatformGSM;
		  goto Exit;
		}

	  // If the phone libraries are not loaded yet, try another hack by
	  //  looking for the appropriate library database
	  if (platform == netPrefUtilPlatformUnknown)
		{
		  DmSearchStateType		searchState;
		  UInt16	cardNo;
		  LocalID	dbID;

		  // Look for the CDMA phone library database
		  palmOSErr = DmGetNextDatabaseByTypeCreator (true/*newSearch*/,
													  &searchState,
													  'libr',
													  cdmaPhoneLibDbCreator,
													  false/*onlyLatestVers*/,
													  &cardNo, &dbID);
		  if (!palmOSErr)
			{
			  ErrFatalDisplayIf (platform != netPrefUtilPlatformCDMA,
								 "Switch to CDMA (London) POSE");
			  platform = netPrefUtilPlatformCDMA;
			  goto Exit;
			}



		  // Look for the GSM phone library database
		  palmOSErr = DmGetNextDatabaseByTypeCreator (true/*newSearch*/,
													  &searchState,
													  'libr',
													  gsmPhoneLibDbCreator,
													  false/*onlyLatestVers*/,
													  &cardNo, &dbID);
		  if (!palmOSErr)
			{
			  ErrFatalDisplayIf (platform != netPrefUtilPlatformGSM,
								 "Switch to GSM (Treo/Atlanta) POSE");
			  platform = netPrefUtilPlatformGSM;
			  goto Exit;
			}
		}
	}

Exit:
#endif // CASTRATION

  


  return (platform);
} // NetPrefUtilPlatformGet


/***********************************************************************
 *
 * Function:	NetPrefUtilDefWirelessDriverIDGet
 *
 * Summary:		Get the default wireless driver ID for the given platform
 *
 * Parameters:
 *	  platform				IN	platform ID
 *
 * Returns: 
 *    Wireless driver ID (creator) or 0 if platform is not wireless
 *  
 * Called By:  
 *
 * History: 
 *	  07-Feb-2002	vmk	  Created
 *
 ***********************************************************************/
extern UInt32
NetPrefUtilDefWirelessDriverIDGet (NetPrefUtilPlatformEnum platform)
{
  UInt32	driverID = 0;


  switch (platform)
	{
	  case netPrefUtilPlatformCDMA:
		driverID = hsFileCModuleSerLib;
		break;

	  case netPrefUtilPlatformGSM:
		driverID = hsFileCVirtualModemSerLib;
		break;

	  default:
		driverID = 0;
		break;
	}


  return (driverID);
} // NetPrefUtilDefWirelessDriverIDGet


/***************************************************************
 *	Function:	  NetPrefUtilPhoneInfoGet
 *
 *	Summary:	  Initialize the phone info structure from a record
 *				   object.  The pointers to strings will NOT be duplicates,
 *				   but will point directly to the corresponding fields
 *				   within the object.
 *
 *	Parameters:
 *	  netPrefLibP	  IN  NetPrefLib reference number (from SysLibFind
 *						   or SysLibLoad)
 *	  recP			  IN  record object
 *	  phoneInfoP	  OUT legacy phone prefs structure
 *
 *	Returns:
 *	  nothing
 *	
 *	Called By: 
 *	  Internal
 *	
 *	Notes:
 *	
 *	History:
 *	  4-Jan-2002	vmk	  Created
 *
 ****************************************************************/
extern void
NetPrefUtilPhoneInfoGet (NetPrefUtilNetPrefLibType* netPrefLibP,
						 struct NetPrefRecordTypeTag* recP,
						 NetPrefUtilPhoneInfoType* phoneInfoP)
{
  char*		strP;
  UInt32	valueLen;
  NetPrefRecFieldViewType viewFlags;


  ErrNonFatalDisplayIf (!(netPrefLibP && recP && phoneInfoP),
						"null arg");


  MemSet (phoneInfoP, sizeof(*phoneInfoP), 0);


  // "dial prefix enabled"
  NetPrefUtilNetPrefByteGet (netPrefLibP, recP,
							netPrefRecFieldDialPrefixEnable,
							&phoneInfoP->flags.dialPrefixEnabled,
							NULL);

  // "call waiting enabled"
  NetPrefUtilNetPrefByteGet (netPrefLibP, recP,
							netPrefRecFieldCallWaitingEnable,
							&phoneInfoP->flags.callWaitingEnabled,
							NULL);


  // "calling card enabled"
  NetPrefUtilNetPrefByteGet (netPrefLibP, recP,
							netPrefRecFieldCallingCardEnable,
							&phoneInfoP->flags.callingCardEnabled,
							NULL);


  // "pulse dial"
  NetPrefUtilNetPrefByteGet (netPrefLibP, recP,
							netPrefRecFieldPulseDialEnable,
							&phoneInfoP->flags.pulseDial, NULL);

  // Phone Number string
  NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
					  netPrefRecFieldPhoneNumber,
					  (void**)&strP, &valueLen, &viewFlags);
  phoneInfoP->phoneP = strP;

  // Dial Prefix string
  NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
					  netPrefRecFieldDialPrefix,
					  (void**)&strP, &valueLen, &viewFlags);
  phoneInfoP->prefixP = strP;

  // Call Waiting string
  NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
					  netPrefRecFieldCallWaitingString,
					  (void**)&strP, &valueLen, &viewFlags);
  phoneInfoP->callWaitingP = strP;

  // Calling Card Number string
  NetPrefRecFieldGet (netPrefLibP->refNum, netPrefLibP->cxtP, recP,
					  netPrefRecFieldCallingCardNumber,
					  (void**)&strP, &valueLen, &viewFlags);
  phoneInfoP->callingCardP = strP;


  return;

} // NetPrefUtilPhoneInfoGet





