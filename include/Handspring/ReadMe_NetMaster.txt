====================================
Handspring DTS Team
http://www.handspring.com/developers
====================================
***If you want to submit a bug report send an email to DevBugs@handspring.com
   Note that there will be no feedback from this email as it is 
   only for bug reports and not for submitting technical questions.
   For technical support, see:
   http://www.handspring.com/developers/tech_support.jhtml


------------------------------------------
Handspring NetMaster Library Header Files
------------------------------------------
November 22, 2002

1. Compiling
	* These are the header files required for developing programs  
          specific to Handspring Treo 180/180g/270 that have been upgraded
          using the GPRS Software Upgrade or the Treo 300 using the 1xRTT
          Data Network.

	* The header files should be placed under /PalmDev/include/Handspring
	* You should also include the NetPref Library header file.

2. Revision History
	* Version 1.00 (August 9, 2002)
		- Initial Beta Release 
	* Version 2.00 (November 22, 2002)
		- Fix version number for production
		- Remove extra commas in NetMasterLibTypes.h that gave
		  errors in CodeWarriors
		- Added new API(s) and notification to support VPN clients
		  see netMasterClientEventNotificationTypeID in 
		  NetMasterLibTypes.h. 
		- Now include a stand-alone library (under utils) to "replace" 
		  the built-in NetMaster Library that does not support VPN clients.
		  YOU ONLY NEED THIS LIBRARY IF YOU WERE USING A VIRTUAL 
		  SERIAL DRIVER. OTHERWISE, DO NOT INCLUDE IN YOUR 
		  PRODUCT/DEVELOPMENT.
		   
