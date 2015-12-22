====================================
Handspring DTS Team
http://www.handspring.com/developers
====================================

***If you want to submit a bug report send an email to DevBugs@handspring.com
   Note that there will be no feedback from this email as it is 
   only for bug reports and not for submitting technical questions.
   For technical support, see:
   http://www.handspring.com/developers/tech_support.jhtml

---------------------------------
Handspring Extension Header Files
---------------------------------
December 10, 2002

1. Compiling
	* These are the header files required for developing programs specific
	  to Handspring products.
	* The header files should be placed under /PalmDev/include/Handspring
	* Please take a look at the file, ReadMe_MissingFiles.txt for information
	  about certain missing header files.

2. Revision History
	* Version 3.55 (December 10, 2002)
		- HsExt.h now supports the Treo 300
		- Added Support for the new NetMaster and NetPref Lib 
		  on Treo 300 and GPRS Software Upgrade. 
		  See NetMaster/NetPref API doc at:
		  http://www.handspring.com/developers/app_dev_ctr.jhtml
		  
		- There is a new Phone Library SDK available to replace
		  the GSM/SMS Library header files that are still included
		  in the SDK. You should start thinking about converting
		  your software to these SDK as we will not support the 
		  GSM/SMS Library header files in the future. Note that
		  SMSlibrary.h was rename GSM_SMSLibrary.h to reflect that
		  it uses the "old" GSM library. The new SMSLibrary.h is
		  using the PhoneLib type and error files.

	* Version 3.53 (May 1, 2002)
		- Added support for Treo 270

	* Version 3.52 (March 1, 2002)
		- Added GSM/SMS Library header files
		- Added APIs for Ring Tone management
		- Updated HsExt.h to final header for Treo

	* Version 3.51 (October 15, 2001)
		- Added support for Treo.

	* Version 3.5
		- Initial Release of the Palm OS v3.5 headers


