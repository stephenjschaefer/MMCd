/* 
 *  $Id: resource.h,v 1.1 2003/06/30 08:41:22 dmitry Exp $
 *
 *  Copyright (c) 2003, Dmitry Yurtaev <dm1try@umail.ru>
 *
 *  This is free software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation; either version 2, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 */


#define bigFontId		128	/* should be 128! */
 
/*
 * main form
 */

#define mainFormId			1000
#define mainTableId			1001
#define newButtonId			1002
#define tempLabelId			1003
#define mainScrollbarId		1004
#define aboutDialogId		1006

/*
 *  menu
 */

#define alarmsMenuId		102
#define prefsMenuId			103
#define aboutMenuId			104
#define convertLogMenuId	105
#define newLogMenuId		106
#define renameLogMenuId		107
#define deleteLogMenuId		108
#define truncateLogMenuId	109
#define testMenuId			110
#define debugMenuId			111

/*
 *  edit form
 */
 
#define editFormId		2000
#define doneButtonId		2001
#define timeLabelId		2003
#define peakButtonId		2004

#define scrollBarId		2012
#define scrollLeftButtonId	2013
#define	scrollRightButtonId	2014
#define scrollerId		2015
#define graphId			2016
#define pauseCheckId		2017
#define logCheckId		2018
#define graphCheckId		2019
#define peakCheckId		2020

#define newLogAlertId		2100
#define renameLogFormId		2101
#define deleteLogAlertId	2102
#define truncateLogAlertId	2103
#define eraseFaultsAlertId	2105
#define versionAlertId		2106

#define newNameFieldId		2110
#define backupLogCheckId	2111

#define panelId			2200

#define bank1ButtonId		2400
#define bank2ButtonId		2401
#define bank3ButtonId		2402
#define bank4ButtonId		2403
#define bank5ButtonId		2404 // CD - Added additional bank buttons
#define bank6ButtonId		2405

#define bankGroupId		2
#define reviewButtonId		2410
#define monitorButtonId		2411
#define testButtonId		2412
#define debugButtonId		2413
#define modeGroupId		3
#define hideUnusedButtonId	2420

#define debugFormId		2500
#define debugFieldId		2501

/*
 *  test form
 */
 
#define testFormId		5000

#define testFirstId		5300
#define injLabelId		5300
#define inj1ButtonId		5301
#define inj2ButtonId		5302
#define inj3ButtonId		5303
#define inj4ButtonId		5304
#define inj5ButtonId		5305
#define inj6ButtonId		5306
#define pumpButtonId    	5307
#define boostButtonId   	5308
#define pressureButtonId	5309
#define mvicButtonId		5310
#define purgeButtonId   	5311
#define egrButtonId     	5312
#define fuelLabelId     	5313
#define solLabelId	     	5314
#define faultsLabelId	     	5315
#define readFaultsButtonId     	5316
#define eraseFaultsButtonId    	5317
#define currentFaultsLabelId	5318
#define storedFaultsLabelId	5319
#define meaningLabelId		5320
#define descrLabelId		5321
#define testLastId		5321


/*
 *  preferences
 */

#define prefsDialogId		3000
#define prefsTableId		3001
#define prefsScrollbarId	3002
#define okButtonId			3003
#define cancelButtonId		3004
#define speedTriggerId		3005
#define speedListId			3006
#define unitTriggerId		3007
#define unitListId			3008
#define defaultButtonId		3009
#define alarmsCheckId		3010
#define egtinputTriggerId	3011
#define egtinputListId		3012
#define autologCheckId		3013  // Added autlog function 5/04
#define clockTriggerId      3014
#define clockListId         3015
#define customSensorId		3020	/* 8 * 3 */
#define o2NonLinCheckId		3021

/*
 *  alarms
 */

#define alarmsFormId		4000
#define sound0ButtonId		4001
#define sound1ButtonId		4002
#define sound2ButtonId		4003
#define sound3ButtonId		4004
#define sound4ButtonId		4005
#define soundGroupId		5
#define threshThumbBitmapId	4006
#define threshBackBitmapId	4007
#define threshSignButtonId	4012
#define threshFieldId		4013
#define threshSliderId		4014
#define alarmLabelId		4015  

#define alarmTriggerId		4016
#define alarmListId		    4017
#define triggerCheckId		4018


/*
 * System Configuration
 */
 
#define	autoid	            2600
#define	systemFormId	    2601
#define	vehicleTriggerId	2602
#define	vehicleListId	    2603
#define	baseTriggerId	    2604
#define	baseListId	        2605
#define	userPrelogId	    2606
#define	userPostlogId	    2607
#define	userLograteId	    2608
#define	userDisplayrateId	2609
#define	systemMenuId	    2610
#define	smartscanCheckId	2611

/*
 * Sensor Setup
 */
 
#define	sensorMenuId	    2612
#define	sensorFormId	    2613
#define	userPidId	        2614
#define	userTagId	        2615
#define	userUnitId	        2616
#define	userScaleId	        2617
#define	userOffsetId	    2618
#define	userGRScaleId	    2619
#define	userGROffsetId	    2620
#define	userColorId	        2621
#define	audibleCheckId	    2623
#define	visualCheckId	    2624
#define	autoCheckId	        2625
#define	sensorTagId	        2626
#define	fuelCutCheckId    	2627


/*
 * Custom Sensor
 */
 
#define	customMenuId	    2627
#define	customFormId	    2628

#define faultStringsId		8000
