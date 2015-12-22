/* 
 *  $Id: mmcd.c,v 1.16 2003/07/03 10:29:50 dmitry Exp $
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

#include <PalmOS.h>
#include <HsExt.h>
#include "mmcd.h"
#include "format.h"
#include "graph.h"
#include "panel.h"
#include "StringMgr.h"

//
UInt32 screenWidth, screenHeight, screenDepth;
Int16 screenDensity;
FontPtr bigFont;

// global state vars
Boolean colorMode = false;
UInt32 ticksPerSecond;

UInt16 portId;
UInt16 autoOff;
Boolean talkingECU = false;
Boolean portOpen = false;
Boolean newLog = false;  // Identify first time log file is  written
UInt8 currentSensor = 0;
UInt8 currentBank = 0;
UInt8 currentMode = 0;
UInt8 scanError = 0; 
Int8 lastSensorReturned = 0;

#define REVIEW_MODE	0
#define MONITOR_MODE	1

UInt32 graphSensors = 0;
UInt32 captureSensors = 0;
UInt8 selectedSensor = 0;
GraphSample currentSample;
FileHand stream;
//Char streamName[32];
Char streamName[24]; // cbhack Looking for bytes

UInt16 streamAttr;

// Char editFormTitle[32];
Char editFormTitle[16]; // cbhack Need a little more space...

Int32 alarmFreq[5] = { 0, 4000, 1000, 300, 50 };

// custom widgets
GraphType graph;
PanelType panel;
Coord graphX, graphY, graphW, graphH;

//#define DELAY_DEBUG// Code to debug serial processing delay

//#define ECU_DEBUG    // Enable debugging mode for simulation
					   // Consider enhancing to enable simulation when serial port not available
				
#ifndef ECU_DEBUG
	#define ECU_BAUDRATE 1920
	#define SIMULATE 0
#else
	#define ECU_BAUDRATE 9600
	#define SIMULATE 1
#endif

static const char egrtInput[6][12] = {
	"3.0 BAR",
	"2.5 BAR",
	"2.0 BAR",
	"Wideband O2",
	"EGRT",
	"0-5V" 	};
	
static const char egrtSlug[6][4] = {
	"MAP",
	"MAP",
	"MAP",
	"WBO2",
	"EGRT",
	"0-5V" 	};	
	

//typedef struct Prefs {  // This was declaring a static varible  Prefs of type Prefs
typedef struct {			// Just declare the struct.
	UInt32 exists;
	UInt32 capture;
	UInt32 graph;
	UInt8 currentBank;
	UInt8 currentMode;
	UInt8 addr[SENSOR_COUNT];
	UInt8 metricUnit;
	UInt8 mapScaling;
	UInt8 vehicleSelect;
	UInt32 baseTiming;
	UInt32 serialSpeed;
	UInt32 ecuSpeed;
	UInt8 overClock; //Insert SS 4/3/05 
	Char slug[SENSOR_COUNT][4];
	UInt16 topVisible;
	UInt8 selectedSensor;

	UInt32 polarity;
	UInt8 threshold[SENSOR_COUNT];
	UInt8 sound[SENSOR_COUNT];
	UInt8 trigger[SENSOR_COUNT]; // Added persistant storage for triggers

	Boolean hideUnused;
	Boolean numericMode;
	Boolean audibleAlarms;
	Boolean autologEnabled;
	Boolean o2NonLinEnabled;
	UInt8 bigNumSelect[8];
} PrefsType;  // Changed name to make it more obvious what was going on.

//PrefsType   *Prefs;  // I hate type == var declarations... this one was unintentional.. Prefs vcar not even used

UInt16 topVisible;
UInt32 serialSpeed;
UInt32 ecuSpeed;
UInt8 overClock; // Insert SS 4/3/05
UInt8 metricUnit;
UInt8 mapScaling;
UInt8 vehicleSelect;
UInt32 baseTiming;
UInt8 audibleAlarms;
UInt8 autologEnabled;  	// Why is this a UInt8 instead of bool ??
UInt8 o2NonLinEnabled;
UInt8 numericMode;		// show sensor numeric value in Monitor mode

Boolean		waitingAnswer; // Moved these declarations to top of source
UInt32		receiveTimeout = 0;

GraphSample		scratchSample;  // Original code
GraphSample		peakSample;  // Max value
GraphSample		minSample;	 // Min value
GraphSample		resetSample; // Simple way to clear struct to null

UInt32 lastGraphUpdate;		// Keeps track of last time low was written
UInt32 lastTimeTicks;		// Used to sync Seconds + Ticks
UInt32 lastTimeSeconds;		// to create mS resolution 

Boolean		peakEnabled; 	// 
Boolean 	logging; 		// Moved logging status to global 
UInt16 		postLog = 30; 	// Number of seconds in postlog - consider making user defined
UInt32 		autologTimer;   // Counts down autolog time remaining
Boolean 	userLogging; 	// User requesting logging (has priority over autolog)

extern	UInt8 bigNumSelect[8]; // Used in graph to select senors for display

			
/* Insert SP 06/01/03 - Hide Unused preference */
Boolean fHideUnused;
/* End Insert */

UInt8 sensorPosition[SENSOR_COUNT] = { 
//	17, 14, 19, 9, 18,  3, 10, 22,
//	 4, 11, 21, 13, 12,  23,  20, 0,
	17, 14, 20, 9, 18,  3, 10, 22,		// Swapped INJD / INJP
	 4, 11, 21, 13, 12,  23,  19, 0,
	 5, 15,  6, 16,  7,  1,  8,  2,
	24, 25, 26, 27, 28, 29, 30, 31,
};

/* Insert SP 06/01/03 - default sensor order */
const UInt8 defSensorPosition[SENSOR_COUNT] = {
//	17, 14, 19, 9, 18,  3, 10, 22,
//	 4, 11, 21, 13, 12,  23,  20, 0,
	17, 14, 20, 9, 18,  3, 10, 22,
	 4, 11, 21, 13, 12,  23,  19, 0,
	 5, 15,  6, 16,  7,  1,  8,  2,
	24, 25, 26, 27, 28, 29, 30, 31,
};
/* End Insert */



void AllocateMem()  // Called during Main init 
{
//	Prefs = MemPtrNew(sizeof(Prefs)); 	// Prefs var wasn't even being used
	
}


void ReleaseMem() // Called just before exiting Main
{
//	MemPtrFree(Prefs);

}
	

// UInt8 GetNextSensorOriginal(UInt8 index)
// {
//    	return (index+1) % 32; //Original logic
// }


UInt8 GetNextSensor(UInt8 currentsensor)
{

	UInt8	index = 0; // Find current sensor and return next in row/column order
	
	while( sensorPosition[index++] != currentsensor);
	index = index % 32;
	if (sensorPosition[index] == lastSensorReturned){ // Stuck at the end of the hide list
		index = 0; // Return first sensor in list
	}
	lastSensorReturned = sensorPosition[index];
   	return sensorPosition[index];
}


// calculate injector duty cycle from RPM and injector pulse width
void computeDerivatives(GraphSample* sample) {
	
Int32	v;

	if(sample->dataPresent & 1L << IDX_RPM &&   // Modified to include INJD as a qualifier for display.
    sample->dataPresent & 1L << IDX_INJP &&
    captureSensors & 1L << IDX_INJD) {			// Only update if INJD is enabled 
		v = sample->data[IDX_INJP] * sample->data[IDX_RPM] / 117L;
		sample->data[IDX_INJD] = MIN(v, 255);
		sample->dataPresent |= 1L << IDX_INJD;  // Mark data as being available for display
	}
	
	// Update vehicle speed 
	
	if(sample->dataPresent & 1L << IDX_VSPD) {
		if (sample->data[IDX_INJD] == 0)  // Not sure if runtime library will handle divide by zero case
			v = 255;
		else
			v = sample->data[IDX_VSPD] * 100L / ( 28L * sample->data[IDX_INJD] );  // MPH / GPH = MPG
	
		sample->data[IDX_MPG] = MIN(v, 255);
		sample->dataPresent |= 1L << IDX_MPG;
	}	
}

/* transpose function - row to column orientation */
#define XPOSE(i) ((i&0x18)|(i<<1&6)|(i>>2&1))

/* Insert SP 06/01/03 - hide/unhide unused sensors */
void HideUnused(void) {
	UInt8 i, j = 0;

	if(currentMode == REVIEW_MODE) {
		for(i = 0; i < SENSOR_COUNT; i++) {
			UInt8 xi = XPOSE(i);
			SensorType *sensor = &_pnlSensor[defSensorPosition[xi]];
			if(sensor->exists && (currentSample.dataPresent & (1L << defSensorPosition[xi]))) {
				sensorPosition[XPOSE(j)] = defSensorPosition[xi];
				j++;
			}
		}
	} else if(currentMode == MONITOR_MODE) {
		for(i = 0; i < SENSOR_COUNT; i++) {
			UInt8 xi = XPOSE(i);
			SensorType *sensor = &_pnlSensor[defSensorPosition[xi]];
			if(sensor->exists && (sensor->capture || sensor->graph)) {
				sensorPosition[XPOSE(j)] = defSensorPosition[xi];
				j++;
			}
		}
	} else {
		return;
	}

	/* fill remaining slots with blanks */
	for(; j < SENSOR_COUNT; j++) {
		sensorPosition[XPOSE(j)] = 0;
	}
}

void UnhideUnused(void) {
	UInt8 i;

	for(i = 0; i < SENSOR_COUNT; i++)
		sensorPosition[i] = defSensorPosition[i];
}

void RefreshPanel(void) {
	if(fHideUnused) HideUnused();
		PnlSetBank(&panel, &sensorPosition[currentBank * 8]);
}
/* End Insert */

/*
 *  return pointer to object in form by its identifier, if found
 *  NULL otherwise
 */
void *FrmGetObjectPtrById(FormType *form, UInt16 id)
{
	UInt16 index = FrmGetObjectIndex(form, id);
	return index == frmInvalidObjectId ? NULL : FrmGetObjectPtr(form, index);
}

/*
 *  graph-callback stream reader
 */

Int32 streamReader(UInt16 id, Int32 streamOffset, GraphSample *pBuffer, Int32 sampleCount)
{
	Int32 samplesRead = 0, i;
	Err err;

	// stream length request
	if(pBuffer == NULL) {
		FileTell(stream, &samplesRead, &err);
		return samplesRead / sizeof(GraphSample);
	}

	if(sampleCount == 0) return 0;

	FileSeek(stream, streamOffset * sizeof(GraphSample), fileOriginBeginning);
	samplesRead = FileRead(stream, pBuffer, sizeof(GraphSample), sampleCount, &err);
	if(samplesRead < sampleCount)
		MemSet(&pBuffer[samplesRead], (sampleCount - samplesRead) * sizeof(GraphSample), 0);

	// for compatibility with old logs
	for(i = 0; i < samplesRead; i++) {
		computeDerivatives(pBuffer + i);
	}
	return samplesRead;
}

Int32 streamWriter(GraphSample *sample)
{
	Int32 samplesWritten = 0;
	Err err;

	FileSeek(stream, 0, fileOriginEnd);
	samplesWritten = FileWrite(stream, sample, sizeof(GraphSample), 1, &err);
	return samplesWritten;
}

void preLog(UInt8 store, GraphSample *sample)
{
	static GraphSample	 preLogSamples[21]; // Prelog sample storage;
	static UInt8 head,tail;
	static UInt8 maxPreLog=20;  // Var so it can be changed programatically
	
	if (store){ 						// Save data to pre-log circular queue
		head = ++head % maxPreLog; 		// wrap around to beginning of queue
		preLogSamples[head]=*sample; 	// write sample to prelog
		if (tail == head)
			tail = ++tail % maxPreLog;
		
	}
	else{ // copy data from pre-log queue to log

//		streamWriter(&preLogSamples[head]); // For testing just write one most recent record

		while (head != tail){
			streamWriter(&preLogSamples[tail]);
			tail = ++tail % maxPreLog;
		}
	}
}		


/*
 *  open serial port, leaving handle in portId global var
 */


Err openPort(UInt32 baudRate, Boolean enableFlowControl)
{
	Err err;

	if(!portOpen) {
		err = SrmOpen(serPortCradleRS232Port, baudRate, &portId);
		if(err != errNone)
			err = SrmOpen(serPortCradlePort, baudRate, &portId);
		if(err != errNone) {
			ErrAlert(err);
			return err;
		}

		if(!enableFlowControl) {
			UInt16 paramSize;
			UInt32 flags =	srmSettingsFlagBitsPerChar8 |
					srmSettingsFlagStopBits1;
			paramSize = sizeof(flags);
			err = SrmControl(portId, srmCtlSetFlags, &flags, &paramSize);
		}
		portOpen = true;

		// disable auto-sleep when talking to ECU
		autoOff = SysSetAutoOffTime(0);

		currentSensor = 0;
		waitingAnswer = false;
		scratchSample.dataPresent = 0;  // Reset flags before starting scan
	}

	return 0;
}

Err closePort()
{
//	Err err;

	if(portOpen) {
		portOpen = false;
		SrmClose(portId);
	}

	if(autoOff) {
		SysSetAutoOffTime(autoOff);
		autoOff = 0;
	}

	return 0;
}

Boolean lastSensor()
{
	return currentSensor == (SENSOR_COUNT - 1) || captureSensors < (1L << (currentSensor + 1));
}





Boolean nextSensor()
{
	UInt32	scanSensorList;  // CB - New scan list for INJD

	if(captureSensors == 0) {
		currentSensor = 0;
		scratchSample.dataPresent = 0;
		return false;
	}

	for(;;) {
		currentSensor = (currentSensor + 1) % SENSOR_COUNT;
//		if (currentSensor == IDX_INJD) currentSensor++; // Skip INJD sensor
														// Disabled in 1.6p - was causing scan to be disabled
														// when INJD was last sensor in scan list
//		if (currentSensor == IDX_INJD)
//			currentSensor = (currentSensor + 1) % SENSOR_COUNT; // Added back in V1.7d with new logic in derived calc
																// Dummy scan was tossing bogus value in MAX

// 		if(captureSensors & (1L << currentSensor)) break;  // Original code


		if (captureSensors & (1L << IDX_INJD)) 		// INJD Enabled so force INJP / RPM
			scanSensorList = captureSensors | (1L << IDX_INJP) | (1L << IDX_RPM);
		else
			scanSensorList = captureSensors;	 // Scan RPM / INJP
			
		if(scanSensorList & (1L << currentSensor)) break; // Use the new list


	}
	return true;
}

Boolean switchMode(UInt8 newMode); 	//Resolve forward references - should be in headers
void haltScanning(); 

void WriteGraphGap(void) { // Add gap in log file 
	UInt8	blanks;
	for (blanks=0; blanks<10 ; blanks++) 
		streamWriter(&resetSample);
}


Boolean queryECU(GraphSample *sample)
{

	Err err;
	Boolean sampleComplete = false;
	UInt16 index;
	//static UInt8	pendingSensor;	// Initialized to get rid of warning
	//UInt8 	tempSensor;			// Temp storage for sensor processing
	
	FormType *form = FrmGetActiveForm();
	if(!portOpen) return false;

	ErrTry {
		if(waitingAnswer) {
			UInt32 bytesAvailable;
			UInt8 buf[2];

			// any data available?
			err = SrmReceiveCheck(portId, &bytesAvailable);
			if(err != errNone) ErrThrow(0);

			// got the answer?
			// Assumes half duplex operation with xmit command byte in rec buffer buf[0]
			
			if(bytesAvailable >= 2) {
				Int32 bytesReceived = SrmReceive(portId, &buf, 2, 1, &err);
				if(err != errNone || bytesReceived != 2) ErrThrow(0);

//				ASSERT(buf[0] == _pnlSensor[currentSensor].addr);
				scratchSample.dataPresent |= 1L << currentSensor; // Mark data available
				scratchSample.data[currentSensor] = buf[1];       // Xfer data to scratch sample
			
				switch (currentSensor){   // Calculate peak/min V1.7e					
					case IDX_COOL:			// Peak logic reversed for coolant
					case IDX_AIRT:			// and Air Temp
						if (buf[1] < peakSample.data[currentSensor])
							peakSample.data[currentSensor]= buf[1];
						if (buf[1] > minSample.data[currentSensor])
							minSample.data[currentSensor]= buf[1];	
						break;	

					case IDX_INJD:  // Do nothing for now
						scratchSample.data[currentSensor]=0; // cb Hack - Might solve the peak problem
						break;	
										
					default:				// Normal logic for other sensors
						if (buf[1] > peakSample.data[currentSensor])
							peakSample.data[currentSensor]= buf[1];
						if (buf[1] < minSample.data[currentSensor])
							minSample.data[currentSensor]= buf[1];						
						break;
				}
				
				waitingAnswer = false;  // Got a valid response
				scanError = 0;

			
			} // not yet - half-second timeout expired?
			else if(TimGetTicks() - receiveTimeout > ticksPerSecond / 2) {

//				if (scanError++ > 10) //pause to allow powerdown
				if (scanError++ > 5) // No need to wait so long V1.7d			
				{				  
					//switchMode(REVIEW_MODE);
					haltScanning();
					peakSample=resetSample; // cbnewcode Reset peak values corrupted when ECU is off 
					scanError = 0;
				}				
				ErrThrow(0);
				
			} 			
			else if ((SIMULATE) || (serialSpeed==1200)) {
				// pretend that we got an anwer
				scratchSample.dataPresent |= 1L << currentSensor;
				scratchSample.data[currentSensor] = SysRandom(0);
				// Update peak values for synthesized values
				
//				if (buf[1] > peakSample.data[currentSensor])
				if (scratchSample.data[currentSensor] > peakSample.data[currentSensor]) // Fixed V1.7e
					peakSample.data[currentSensor]= scratchSample.data[currentSensor];
					
//				if (buf[1] < minSample.data[currentSensor])
				if (scratchSample.data[currentSensor] < minSample.data[currentSensor]) // Fixed V1.7e
					minSample.data[currentSensor]= scratchSample.data[currentSensor];	
					
				waitingAnswer = false;									
			} 
			
			else 	
				return false; 	// Still waiting for data receipt pending so just return
	
			}  //  End of conditional block receive processing logic

			
			// Got data so send request for next data while processing results
			// CB - Moved this code from end of loop to improve efficiency

/*
			tempSensor=currentSensor; // Save a copy of the currentSensor

			if(nextSensor()) {
				SrmSend(portId, &_pnlSensor[currentSensor].addr, 1, &err);
				receiveTimeout = TimGetTicks();
				waitingAnswer = true;
			}		
			
			pendingSensor = currentSensor; 	// Save pendingSensor - Swap after processing 
			currentSensor = tempSensor; 	// Replace currentSensor value for alarm processing
*/
		
			// Process alarms

			if( (_pnlSensor[currentSensor].trigger || _pnlSensor[currentSensor].sound) 
 		 		&& (((currentSensor == 10 || currentSensor == 22)&& (scratchSample.data[14] > 200))
			    || (currentSensor != 10 && currentSensor !=  22)))

			{
				_pnlSensor[IDX_COOL].polarity = ! _pnlSensor[IDX_COOL].polarity; // Toggle for alarm processing
				_pnlSensor[IDX_AIRT].polarity = ! _pnlSensor[IDX_AIRT].polarity; // because the raw data is inverted
				
				if((_pnlSensor[currentSensor].polarity == 0 && 	
					scratchSample.data[currentSensor] > _pnlSensor	[currentSensor].threshold) ||
					(_pnlSensor[currentSensor].polarity == 1 && 
					scratchSample.data[currentSensor] < _pnlSensor	[currentSensor].threshold))
				{ 
				
					if (audibleAlarms){   // Process audible alarm
						SndCommandType sndCmd = { sndCmdFrqOn, 0, alarmFreq[_pnlSensor[currentSensor].sound], 100, sndMaxAmp };
						SndDoCmd(NULL, &sndCmd, true);					
					}
					if (autologEnabled && _pnlSensor[currentSensor].trigger){ // Start autolog
						autologTimer = TimGetSeconds()+ postLog; // Any alarm with trigger starts autologging
						logging = true; 	
						index = FrmGetObjectIndex(form, logCheckId);
						FrmSetControlValue(form, index, 1);
					}
				}		
				_pnlSensor[IDX_COOL].polarity = ! _pnlSensor[IDX_COOL].polarity; // Toggle back to original
				_pnlSensor[IDX_AIRT].polarity = ! _pnlSensor[IDX_AIRT].polarity; 

			}

//		}
	} ErrCatch(exception __attribute__ ((unused))) {
		SrmClearErr(portId);
		SrmReceiveFlush(portId, 0);
		waitingAnswer = false;
	} ErrEndCatch

//	ASSERT(waitingAnswer == false);

	if(lastSensor() && scratchSample.dataPresent) {
		// before starting over return collected sample to the caller

		*sample = scratchSample;
		sample->time = TimGetSeconds(); // Add timestamp to log record
		
		// CBB - Hack to synthesize mS resolution
		// Consider creating new function to return elapsed time + mS
		//
		if (sample->time != lastTimeSeconds){ // Seconds have rolled over so reset mS
			lastTimeSeconds = sample->time;
			lastTimeTicks = TimGetTicks(); // Reset starting timer for centiseconds
			sample->data[23] = 0 ; // Write 00 centiseconds to unused log entry CBB
		} else
			sample->data[23] = (100 * (TimGetTicks() - lastTimeTicks)) / ticksPerSecond; // Centiseconds
							
		sampleComplete = true;
															 // CB - Deleted - done after calculating derivatives
//		peakSample.dataPresent = scratchSample.dataPresent;  // Update data available in peakSample
//		minSample.dataPresent = scratchSample.dataPresent;   // Update data available in peakSample
		scratchSample.dataPresent = 0;	// This is being reset before returning?????
		
	}

	if(nextSensor()) {
		SrmSend(portId, &_pnlSensor[currentSensor].addr, 1, &err);
		receiveTimeout = TimGetTicks();
		waitingAnswer = true;
	}

//	currentSensor = pendingSensor;	// Update currentSensor to point to sensor that is already pending

	return sampleComplete;
}


/*
 *  review mode-specific event handler
 */
  

Int32 scrollerScale = 1;
void _setScroller(Int32 value) {
	ScrollBarType *bar = FrmGetObjectPtrById(FrmGetActiveForm(), scrollBarId);
	scrollerScale = graph.length / 32000 + 1;
	SclSetScrollBar(bar, value / scrollerScale, 0, 
		graph.length > graph.width
		? (graph.length - graph.width) / scrollerScale
		: 0,
		graph.width / scrollerScale
	);
}

void updateSampleTime()
{
	static Char s[32];
	DateTimeType date;

	TimSecondsToDateTime(currentSample.time, &date);
//	StrPrintF(s, "%02d:%02d:%02d", date.hour, date.minute, date.second); 

	// Add month/day/year to review display
	StrPrintF(s, "%02d/%02d/%02d    %02d:%02d:%02d.%03d", date.month, date.day, date.year, date.hour, date.minute, date.second,currentSample.data[23] * 10);
	FrmCopyLabel(FrmGetActiveForm(), timeLabelId, s);
}

Boolean reviewHandleEvent(EventType* e)
{
	/* Insert SP 06/03/03 */
	FormType *form = FrmGetActiveForm();
	/* End Insert */

	if(GrfHandleEvent(&graph, e)) return true;

	switch(e->eType) {

	case ctlSelectEvent:
		switch(e->data.ctlSelect.controlID) {
		case hideUnusedButtonId:
			fHideUnused = !fHideUnused;
			if (fHideUnused) {
				/* highlight "H" button */
				FrmSetControlValue(form, FrmGetObjectIndex(form, hideUnusedButtonId), 1);
//				fHideUnused = true;  // fix hide mode in Review
			} else {
				/* deselect "H" button */
				FrmSetControlValue(FrmGetActiveForm(), FrmGetObjectIndex(form, hideUnusedButtonId), 0);
//				fHideUnused = false; // fix hide mode in Review
				UnhideUnused();
			}
			RefreshPanel();
			return true;
		}
		break;

	case sclRepeatEvent:
		// user moved scroller - update graph
		if(e->data.sclRepeat.newValue != e->data.sclRepeat.value)
			GrfSetGraph(&graph, graph.length, e->data.sclRepeat.newValue * scrollerScale, graph.cursor, graph.dataMask);
		return false;

	case grfChangeEvent:
		// graph has been changed by user interaction
		{
			GraphEventType *event = (GraphEventType *)e;

			// position changed? update scroller, then
			if(event->data.grfChange.positionChanged || event->data.grfChange.lengthChanged) {
				_setScroller(event->data.grfChange.position);
			}

			// cursor moved? update values and currectSample
			if(event->data.grfChange.cursorChanged) {
				currentSample = graph._buffer[graph.cursor - graph.position];
				computeDerivatives(&currentSample);
				/* Insert SP 06/01/03 - update hidden sensors */
				RefreshPanel();
				/* End Insert */
				PnlUpdate(&panel, &currentSample);
				updateSampleTime();
			}
		}
		return true;

	/*
	 * removed, pageUp and pageDown now scroll thru banks
	 */
	
	case keyDownEvent:

		// move cursor 1 sample left if not at the beginning of graph
		if(e->data.keyDown.chr == hard1Chr && graph.cursor > 0) {
			Int32 newPosition = graph.position;
			Int32 newCursor = graph.cursor - 1;

			SndPlaySystemSound(sndClick);

			// update graph and scroller positions
			// if moved out of visible area
			if(newCursor < newPosition) {
				_setScroller(newPosition);
				newPosition = newCursor;
			}

			// update the graph
			GrfSetGraph(&graph, graph.length, newPosition, newCursor, graph.dataMask);

			// update values
			currentSample = graph._buffer[graph.cursor - graph.position];
			computeDerivatives(&currentSample);
			// Insert SP 06/01/03 - update hidden sensors
			RefreshPanel();
			// End Insert
			PnlUpdate(&panel, &currentSample);
			updateSampleTime();
			return true;
		}

		// move cursor 1 sample right if not at the end of graph
		if(e->data.keyDown.chr == hard2Chr && graph.cursor < graph.length - 1) {
			Int32 newPosition = graph.position;
			Int32 newCursor = graph.cursor + 1;

			SndPlaySystemSound(sndClick);

			// update graph and scroller positions
			// if moved out of visible area
			if(newCursor >= newPosition + graph.width) {
				newPosition = newCursor - graph.width + 1;
				_setScroller(newPosition);
			}

			// update the graph
			GrfSetGraph(&graph, graph.length, newPosition, newCursor, graph.dataMask);

			// update values
			currentSample = graph._buffer[graph.cursor - graph.position];
			computeDerivatives(&currentSample);
			// Insert SP 06/01/03 - update hidden sensors
			RefreshPanel();
			// End Insert
			PnlUpdate(&panel, &currentSample);
			updateSampleTime();
			return true;
		}
		break;

	default:
		break;
	}
	return false;
}

/*
 *  monitor mode-specific event handler
 */



void haltScanning()
{
	FormType *form = FrmGetActiveForm();

	SndPlaySystemSound(sndClick);
	{
		UInt16 index = FrmGetObjectIndex(form, pauseCheckId);
		Int16 newValue = FrmGetControlValue(form, index) ? 0 : 1;
		FrmSetControlValue(form, index, newValue);
				
		if(newValue) 
		{
			RectangleType r = { { 1, 147 }, { 50, 13 } };
			WinEraseRectangle(&r, 0);
			FrmShowObject(form, FrmGetObjectIndex(form, reviewButtonId));
			FrmShowObject(form, FrmGetObjectIndex(form, doneButtonId));
			FrmHideObject(form, FrmGetObjectIndex(form, peakButtonId));
			closePort();
			talkingECU = false;
			GrfAppendSample(&graph, NULL);
		}
	}
}


Boolean monitorHandleEvent(EventType* e)  // Main loop for scanning 
{
	FormType *form = FrmGetActiveForm();
	static UInt32 startTicks;
	static UInt32 cycles;
	static UInt32 lastUpdateTime;
	static Boolean needsUpdate;
	static GraphSample sample;
//	static GraphSample scaledGraphSample;
	
//	static Boolean logging;
//	UInt8 blanks;	
	Boolean sampleComplete;
	UInt32 keepScanning;
//	ControlType *newtest; // Trying to figure out how to get a ptr to graph control
		
	GrfHandleEvent(&graph, e);

	if(e->eType == keyDownEvent && !(e->data.keyDown.modifiers & autoRepeatKeyMask)) {
		switch(e->data.keyDown.chr) {
		case hard1Chr:
			// change sensor for numeric display
			SndPlaySystemSound(sndClick);
			PnlSetSelection(&panel, selectedSensor = GrfSetSensor(&graph, 0xff));
			return true;

		case hard2Chr:
		
			// toggle Graph checkbox
			SndPlaySystemSound(sndClick);

//			newtest = FrmGetObjectPtr(form, graphCheckId);
//			CtlHitControl(newtest);
			
//			CtlHitControl(FrmGetObjectPtr(form, graphCheckId)); // CB - Get rid of dedundant code
//*
		
 			{
				UInt16 index = FrmGetObjectIndex(form, graphCheckId);
				Int16 newValue = FrmGetControlValue(form, index) ? 0 : 1;
				FrmSetControlValue(form, index, newValue);
				GrfSetMode(&graph, numericMode = !newValue);
				GrfAppendSample(&graph, NULL);
//				if (numericMode)            // cbTest panel hide/unhide
//					PnlHidePanel(&panel);
//				else
//					PnlShowPanel(&panel);
	
				PnlSetMode(&panel, numericMode ? PANEL_SINGLESELECT : PANEL_MULTISELECT);
			}
//*/
						return true;

		case hard3Chr:
			// toggle Log checkbox
			SndPlaySystemSound(sndClick);
			{
				UInt16 index = FrmGetObjectIndex(form, logCheckId);
				Int16 newValue = FrmGetControlValue(form, index) ? 0 : 1;
				FrmSetControlValue(form, index, newValue);
				logging = newValue;
				if (!logging){ // Disable logging
					autologTimer =0; // Stop autologging - ready for next autolog trigger
					userLogging=false; // Reset flag for user controlled logging
				} else{		
					userLogging=true; // User logging overrides autologging
				}	
			}
			return true;

		case hard4Chr:
			// toggle Pause checkbox
			SndPlaySystemSound(sndClick);
			{
				UInt16 index = FrmGetObjectIndex(form, pauseCheckId);
				Int16 newValue = FrmGetControlValue(form, index) ? 0 : 1;
				FrmSetControlValue(form, index, newValue);
				
				if(newValue) {
					RectangleType r = { { 1, 147 }, { 50, 13 } };

					WinEraseRectangle(&r, 0);
					FrmShowObject(form, FrmGetObjectIndex(form, reviewButtonId));
					FrmShowObject(form, FrmGetObjectIndex(form, doneButtonId));
					FrmHideObject(form, FrmGetObjectIndex(form, peakButtonId));
					closePort();
					talkingECU = false;
					GrfAppendSample(&graph, NULL);
				} else {
					FrmHideObject(form, FrmGetObjectIndex(form, reviewButtonId));
					FrmHideObject(form, FrmGetObjectIndex(form, doneButtonId));
				
					// update current mode pushbutton as its frame
					// gets erased by neighbour hiding buttons
					FrmShowObject(form, FrmGetObjectIndex(form, monitorButtonId));
					FrmShowObject(form, FrmGetObjectIndex(form, peakButtonId));
//					FrmShowObject(form, FrmGetObjectIndex(form, minmaxButtonId)); // New button not implemented yet
					openPort(serialSpeed, false);
					talkingECU = true;
					startTicks = TimGetTicks();
					cycles = 0;
				}
			}
			return true;
		}
	}
	
	/* Modified SP 06/01/03 - change conditional structure */
	if (e->eType == ctlSelectEvent) {
		switch (e->data.ctlSelect.controlID) {
		case pauseCheckId:
			// disable mode-switching when serial communication
			// is in progress
			if(e->data.ctlSelect.on) {
				RectangleType r = { { 1, 147 }, { 50, 13 } };

				WinEraseRectangle(&r, 0);
				FrmShowObject(form, FrmGetObjectIndex(form, reviewButtonId));
				FrmShowObject(form, FrmGetObjectIndex(form, doneButtonId));
				FrmHideObject(form, FrmGetObjectIndex(form, peakButtonId));
//				FrmShowObject(form, FrmGetObjectIndex(form, peakButtonId));
				closePort();
				talkingECU = false;
				GrfAppendSample(&graph, NULL);
			} else {
				FrmHideObject(form, FrmGetObjectIndex(form, reviewButtonId));
				FrmHideObject(form, FrmGetObjectIndex(form, doneButtonId));
				// update current mode pushbutton as its frame
				// gets erased by neighbour hiding buttons
				FrmShowObject(form, FrmGetObjectIndex(form, monitorButtonId));
				FrmShowObject(form, FrmGetObjectIndex(form, peakButtonId));
				openPort(serialSpeed, false);
				talkingECU = true;
				startTicks = TimGetTicks();
				cycles = 0;
			}
			break;

		case logCheckId:

			logging = e->data.ctlSelect.on;
			if (!logging){
				autologTimer =0; // Stop logging - ready for next autolog trigger
				userLogging=false;
			} else {
				userLogging=true; // Flag for manual logging
//				WriteGraphGap();
			}
			break;
              
		case peakCheckId:
			peakEnabled = e->data.ctlSelect.on;
			break;

		case graphCheckId:
			GrfSetMode(&graph, numericMode = !e->data.ctlSelect.on);
			GrfAppendSample(&graph, NULL);
			PnlSetMode(&panel, numericMode ? PANEL_SINGLESELECT : PANEL_MULTISELECT);
			break;
             
		/* Insert SP 06/01/03 - handle Hide Unused button */
		case hideUnusedButtonId:
			if (e->data.ctlSelect.on) {
				/* Hide */
				fHideUnused = true;
			} else {
				/* Do not hide */
				UnhideUnused();
				fHideUnused = false;
			}
			RefreshPanel();
			break;
		/* End Insert - handle Hide Unused button */
		}
	}
        /* End Modified - change conditional structure */
        
//		if(queryECU(&sample)) { 

        // Modified to read multiple samples per event main loo

        sampleComplete=false;
        keepScanning = TimGetTicks()+4; // Originall set to 2
        while (TimGetTicks() < keepScanning){
	        if (queryECU(&sample)){
				sampleComplete=true;
				break;
			}
		}	
		if (sampleComplete) {

		computeDerivatives(&sample);
		
		// Update fields in peak structures				
		peakSample.dataPresent = sample.dataPresent;
		minSample.dataPresent = sample.dataPresent;

		// Update min/max values for INJD
		if (sample.data[IDX_INJD] > peakSample.data[IDX_INJD]) 
			peakSample.data[IDX_INJD] = sample.data[IDX_INJD]; 
		
		if (sample.data[IDX_INJD] < minSample.data[IDX_INJD])
			minSample.data[IDX_INJD] = sample.data[IDX_INJD];

		// show it			 
		GrfAppendSample(&graph, &sample);
		needsUpdate = true;
		cycles++;

		// if log checkbox is checked - write the sample to log file
		if ((logging) ){		
 			if ((sample.time-lastGraphUpdate) > 2){
 				if (!newLog)
 					WriteGraphGap();  	// Insert blank gap in log file
 				else
 					newLog = false; 	// Don't put gap at start of log file
 			preLog(0,&sample );			// Copy prelog data to log 
 										// Consider passing argument of seconds to preLog()
// 			streamWriter(&resetSample); // Insert blank record for testing
 				 				
 			}						
			streamWriter(&sample); // Write the sample to the log
			

			// Check if autolog has timed out
			if (!userLogging && (autologTimer < TimGetSeconds())){
				UInt16 index = FrmGetObjectIndex(form, logCheckId);
				FrmSetControlValue(form, index, 0);
				logging = false;
			}	

			lastGraphUpdate = sample.time;

		} 
		else
			preLog(1,&sample); // Write sample to prelog buffer
		
	}
	
	if(TimGetTicks() > lastUpdateTime + ticksPerSecond / 6) {
		lastUpdateTime = TimGetTicks();
		
		if (peakEnabled)
			PnlUpdate(&panel, &peakSample);
		else
			PnlUpdate(&panel, &sample);

		// show sample rate and current sensor
		if(portOpen) {
			static Char str[32];
			Int16 strLen, labelWidth;
			RectangleType r;

//			strLen = StrPrintF(str, "%ldHz %s", cycles * ticksPerSecond / (lastUpdateTime - startTicks + 1), _pnlSensor[currentSensor].slug);
			strLen = StrPrintF(str, "%ldHz %s", cycles * ticksPerSecond / (lastUpdateTime - startTicks + 1), _pnlSensor[currentSensor].slug);
		
			labelWidth = FntCharsWidth(str, strLen);
			r.topLeft.x = 1 + labelWidth;
			r.topLeft.y = 147;
//			r.extent.x = 50  - labelWidth; 
//			r.extent.x = 40 - labelWidth; // Make narrower to allow for reset button
			r.extent.x = 46 - labelWidth; // Make wider to clean up for long tags
			
			r.extent.y = 13;
			WinDrawChars(str, strLen, 1, 147);
			WinEraseRectangle(&r, 0);
		}
	}

	return false;
}

void doAboutDialog()
{
	FormType *form = FrmInitForm(aboutDialogId);
	FrmDoDialog(form);
	FrmDeleteForm(form);

}

/*
 *  p r e f s
 */

void FldInsertText(FieldType *field, const Char *source)
{
	MemHandle textHandle;
	Char *text;

	textHandle = FldGetTextHandle(field);
	if(textHandle != NULL) {
		FldSetTextHandle(field, NULL);
		MemHandleFree(textHandle);
	}

	textHandle = MemHandleNew(5);
	text = MemHandleLock(textHandle);
	StrNCopy(text, source, 5);
	MemHandleUnlock(textHandle);

	FldSetTextHandle (field, textHandle);
}


void setDefaults()
{
	UInt16 i;
	
	graphSensors = 0;
	captureSensors = 0; 
	currentBank = 0;
	currentMode = 1; // cb - Start off in Monitor Mode
	topVisible = 0;
	metricUnit = PREFS_UNIT_METRIC;
	serialSpeed = ECU_BAUDRATE;
	ecuSpeed = 100;
	overClock = 0; //Insert SS 4/3/05
	mapScaling = 0;
	vehicleSelect = 0;
	baseTiming = 5; //Modified SS 4/3/05 - changed default to 5 degrees base timing
	numericMode = 0;
	selectedSensor = 0;
	fHideUnused = false;
	audibleAlarms = true;	// Default to on
	autologEnabled = true; 	// Default to on
	o2NonLinEnabled = true; // Default to on
	
	for(i = 0; i < SENSOR_COUNT; i++) {
	  if (i>16 && i<=20) {   // cb 1/4/05 Turn on several sensor for scanning/graphing by default
        _pnlSensor[i].capture=1; 
        if (i!=18)
          _pnlSensor[i].graph=1;  // Make one sensor not selected for graphing 
      }
      _pnlSensor[19].capture=0;  // Disable INJP
      _pnlSensor[19].graph=0;  // Disable INJP graphing 
      _pnlSensor[18].graph=0;  // Make one sensor not selected for graphing 
	  if(_pnlSensor[i].capture) captureSensors |= (1L << i);
	  if(_pnlSensor[i].graph) graphSensors |= (1L << i);
	}
}


void doPrefsDialog()
{
	FormType *form = FrmInitForm(prefsDialogId);
	UInt16 i, index;
	Char *s, str[16],str2[8], str3[8];
	ControlType* ctl;

	ctl = FrmGetObjectPtrById(form, speedTriggerId); // serial speed
	StrPrintF(str, "%ld bps", serialSpeed);
	if (serialSpeed==1200)
		CtlSetLabel(ctl, "Simulate");
	else
		CtlSetLabel(ctl, str);
		

	ctl = FrmGetObjectPtrById(form, unitTriggerId); // unit
	switch(metricUnit) {
		case PREFS_UNIT_ENGLISH: CtlSetLabel(ctl, "English"); break;
		case PREFS_UNIT_NUMERIC: CtlSetLabel(ctl, "Numeric"); break;
		default:		 CtlSetLabel(ctl, "Metric"); break;
	}

	ctl = FrmGetObjectPtrById(form, egtinputTriggerId); // EGT input - cb Modified to use egrtInput const
	CtlSetLabel(ctl, egrtInput[mapScaling]);
	StrNCopy(_pnlSensor[9].slug, egrtSlug[mapScaling], 4); // Works but does not change slugs immediately	
	
/*

	switch(mapScaling) { 
	
	case 0: 
		CtlSetLabel(ctl, "3.0 BAR");
//	    StrNCopy(_pnlSensor[9].slug, "MAP ", 4); // Works but does not change slugs immediately
		break;
	case 1: 
		CtlSetLabel(ctl, "2.5 BAR"); 
//	    StrNCopy(_pnlSensor[9].slug, "MAP ", 4);	
		break;
	case 2: 
		CtlSetLabel(ctl, "2.0 BAR"); 
//	    StrNCopy(_pnlSensor[9].slug, "MAP ", 4);	
		break;
	case 3: 
		CtlSetLabel(ctl, "Wideband O2"); 
//	    StrNCopy(_pnlSensor[9].slug, "WB02", 4);	
		break;
	case 4: 
		CtlSetLabel(ctl, "EGRT"); 
//	    StrNCopy(_pnlSensor[9].slug, "EGRT", 4);	
		break;
	case 5: 
		CtlSetLabel(ctl, "0-5V"); 
//	    StrNCopy(_pnlSensor[9].slug, "0-5v ", 4);	
		break;
	}
	
*/
	
		
	ctl = FrmGetObjectPtrById(form, vehicleTriggerId); // Vehicle Select
	switch(vehicleSelect) {
	case 0:
		CtlSetLabel(ctl, "3/S");
		break;
	case 1:
		CtlSetLabel(ctl, "DSM");
		break;
	}
	
	ctl = FrmGetObjectPtrById(form, baseTriggerId); // Base Timing
	StrPrintF(str2, "%ld°" , baseTiming); // CB Much simpler version
	CtlSetLabel(ctl, str2);

	ctl = FrmGetObjectPtrById(form, clockTriggerId); // ECU Speed
	//StrPrintF(str3, "%ld\%", ecuSpeed);  //Escape char causing crash?
	StrPrintF(str3, "%ld%%", ecuSpeed);
    CtlSetLabel(ctl, str3);
	
	switch (ecuSpeed) {  //Overclock Airflow and RPM scaling functionality established here
	case 100:
		overClock = 0;
		break;
	case 110:
		overClock = 1;
		break;
	case 112:
		overClock = 2;
		break;
	case 115:
		overClock = 3;
		break;
	default:
		overClock = 0;
	}
	
	index = FrmGetObjectIndex(form, alarmsCheckId);
	FrmSetControlValue(form, index, audibleAlarms);
	
	index = FrmGetObjectIndex(form, o2NonLinCheckId);
	FrmSetControlValue(form, index, o2NonLinEnabled);

	index = FrmGetObjectIndex(form, autologCheckId);
	FrmSetControlValue(form, index, autologEnabled);

	i = FrmDoDialog(form);
	if(i == defaultButtonId) {
		MemMove(_pnlSensor, _pnlSensorDefault, sizeof(_pnlSensor));
		setDefaults();
		PnlUpdate(&panel, &currentSample);  
											
	} else if(i == okButtonId) {
		
		ctl = FrmGetObjectPtrById(form, speedTriggerId); // serial speed
		serialSpeed = StrAToI(CtlGetLabel(ctl));
		if (serialSpeed==0)
			serialSpeed = 1200; // Use 1200 baud as flag for simulation
			
		ctl = FrmGetObjectPtrById(form, unitTriggerId); 
		s = (Char*)CtlGetLabel(ctl);
		
		if(0 == StrCompare(s, "English")) {
			metricUnit = PREFS_UNIT_ENGLISH;
		} else if(0 == StrCompare(s, "Numeric")) {
			metricUnit = PREFS_UNIT_NUMERIC;
		} else {
			metricUnit = PREFS_UNIT_METRIC;
		}

		//ctl = FrmGetObjectPtrById(form, egtinputTriggerId); // EGT Input
		//mapScaling = CtlGetValue(ctl);

		ctl = FrmGetObjectPtrById(form, egtinputTriggerId); // EGT Input
		s = (Char*)CtlGetLabel(ctl);
	
		for (mapScaling=0; mapScaling < 6; mapScaling++) // cd - refactored to use egrtInput constants 
			if (0==StrCompare(s,egrtInput[mapScaling]))  // If it matches we're
				break;
			
/*														// cd - refactored to use egrtInput constants 
	
		if(0 == StrCompare(s, "3.0 BAR")) {
			mapScaling = 0;
		} else if(0 == StrCompare(s, "2.5 BAR")) {
			mapScaling = 1;

		} else if(0 == StrCompare(s, "2.0 BAR")) {
			mapScaling = 2;		

		} else if(0 == StrCompare(s, "Wideband O2")) {
			mapScaling = 3;	

		} else if(0 == StrCompare(s, "EGRT")) {
			mapScaling = 4;	

		} else {
			mapScaling = 5;  // 0-5V back to EGT Scaling
		}

*/

		ctl = FrmGetObjectPtrById(form, vehicleTriggerId); // Insert SS 1/3/05 Vehicle Select
		s = (Char*)CtlGetLabel(ctl);
		if(0 == StrCompare(s, "3/S")) {
			vehicleSelect = 0;
		} else if(0 == StrCompare(s, "DSM")) {
			vehicleSelect = 1;
		}
		
		ctl = FrmGetObjectPtrById(form, baseTriggerId); // Insert SS 1/3/05 Base Timing
		baseTiming = StrAToI(CtlGetLabel(ctl));
	
		ctl = FrmGetObjectPtrById(form, clockTriggerId); // Insert SS 4/4/05 ECU Speed
		ecuSpeed = StrAToI(CtlGetLabel(ctl));
		
		index = FrmGetObjectIndex(form, alarmsCheckId);
		audibleAlarms = FrmGetControlValue(form, index) ? true : false;
		
		index = FrmGetObjectIndex(form, o2NonLinCheckId);
		o2NonLinEnabled = FrmGetControlValue(form, index) ? true : false;	

		index = FrmGetObjectIndex(form, autologCheckId);
		autologEnabled = FrmGetControlValue(form, index) ? true : false;
	}

	FrmDeleteForm(form);
}
	

/*
 *  a l a r m s
 */

static Char* alarmListSlugs[32];
static Char alarmListStrings[32*6];
static Char alarmListSensor[32];

void updateAlarmThreshold(FormType* form, UInt16 si)
{
	static Char str[16] = "\0";

	_pnlSensor[si].format(_pnlSensor[si].threshold, str);
	FrmCopyLabel(form, threshFieldId, str);
}

void updateAlarmForm(FormType* form, UInt16 si)
{
	UInt16 index;
	ControlType* control;
	UInt16 v[4] = { 0, 255, 1, _pnlSensor[si].threshold };

	index = FrmGetObjectIndex(form, alarmTriggerId);
	control = FrmGetObjectPtr(form, index);
	CtlSetLabel(control, _pnlSensor[si].slug);

	FrmSetControlGroupSelection(form, soundGroupId, sound0ButtonId + _pnlSensor[si].sound);
	index = FrmGetObjectIndex(form, threshSliderId);
	control = FrmGetObjectPtr(form, index);
	CtlSetSliderValues(control, &v[0], &v[1], &v[2], &v[3]);

	index = FrmGetObjectIndex(form, threshSignButtonId);
	control = FrmGetObjectPtr(form, index);
	CtlSetLabel(control, _pnlSensor[si].polarity ? "<" : ">");
	
	index = FrmGetObjectIndex(form, triggerCheckId);
	FrmSetControlValue(form, index,  _pnlSensor[si].trigger);
	
	updateAlarmThreshold(form, si);
}

void newupdateAlarmForm(FormType* form, UInt16 si)
{
	UInt16 index;
	ControlType* control;
	UInt16 v[4] = { 0, 255, 1, _pnlSensor[si].threshold };
	
	index = FrmGetObjectIndex(form, alarmTriggerId);
	control = FrmGetObjectPtr(form, index);
	CtlSetLabel(control, _pnlSensor[si].slug);
	
	//FrmCopyLabel(form, sensorTagId, alarmTriggerId);
	
	//index = FrmGetObjectIndex(form, sensorTagId);
	//control = FrmGetObjectPtr(form, index);
	//CtlSetLabel(control, _pnlSensor[si].slug);

	FrmSetControlGroupSelection(form, soundGroupId, sound0ButtonId + _pnlSensor[si].sound);
	index = FrmGetObjectIndex(form, threshSliderId);
	control = FrmGetObjectPtr(form, index);
	CtlSetSliderValues(control, &v[0], &v[1], &v[2], &v[3]);

	index = FrmGetObjectIndex(form, threshSignButtonId);
	control = FrmGetObjectPtr(form, index);
	CtlSetLabel(control, _pnlSensor[si].polarity ? "<" : ">");
	
	updateAlarmThreshold(form, si);
}

void updateAlarmList(FormType* form)
{
	Int16 i, last = 0;
	ListType* list;
/*
	for(i = 0; i < SENSOR_COUNT; i++) {
		if(_pnlSensor[sensorPosition[i]].exists) {
			alarmListSensor[last] = sensorPosition[i];
			StrNCopy(alarmListSlugs[last], _pnlSensor[sensorPosition[i]].slug, 5);
			if(_pnlSensor[sensorPosition[i]].sound)
				StrNCat(alarmListSlugs[last], "#", 6);
			last++;
		}
	}
*/
	for(i = 0; i < SENSOR_COUNT; i++) {
		if(_pnlSensor[i].exists) {
			alarmListSensor[last] = i;
			StrNCopy(alarmListSlugs[last], _pnlSensor[i].slug, 5);
			if(_pnlSensor[i].sound || _pnlSensor[i].trigger)
				StrNCat(alarmListSlugs[last], "#", 6);
			last++;
		}
	}

	list = FrmGetObjectPtr(form, FrmGetObjectIndex(form, alarmListId));
	LstSetListChoices(list, alarmListSlugs, last);
}

Boolean alarmsFormHandleEvent(EventType *e)
{
	FormType *form = FrmGetActiveForm();
	SndCommandType sndCmd = { sndCmdFrqOn, 0, 1000, 200, sndMaxAmp };
	static int si;
	Int32 i;
	Err err;

	if(SysHandleEvent(e) || MenuHandleEvent(NULL, e, &err)) return true;
	
	switch(e->eType) {

	case frmOpenEvent:

		for(i = 0; i < SENSOR_COUNT; i++)
			alarmListSlugs[i] = alarmListStrings + i * 6;

		si = selectedSensor & 0x1f;
		for(i = 0; i < SENSOR_COUNT; i++) {
			if(!_pnlSensor[si].exists)
				si = (si + 1) % SENSOR_COUNT;
			else
				break;
		}

		FrmDrawForm(form);
		updateAlarmList(form);
		updateAlarmForm(form, si);

		return true;

	case popSelectEvent:
		si = alarmListSensor[e->data.popSelect.selection];
		updateAlarmForm(form, si);
		return true;


	case frmCloseEvent:
		// if user switcher application when this form is open
		// post frmCloseEvent to caller form as it will get it
		// a chance to free resources
		FrmReturnToForm(0);
		return true;

	case ctlRepeatEvent:
		switch(e->data.ctlRepeat.controlID) {
		case threshSliderId:
			_pnlSensor[si].threshold = e->data.ctlRepeat.value;
			updateAlarmThreshold(form, si);
			return false;
		}
		break;

	case ctlSelectEvent:
		switch(e->data.ctlSelect.controlID) {
		case okButtonId:
			FrmReturnToForm(0);	
			return true;

		case sound0ButtonId:
		case sound1ButtonId:
		case sound2ButtonId:
		case sound3ButtonId:
		case sound4ButtonId:
			_pnlSensor[si].sound = e->data.ctlSelect.controlID - sound0ButtonId;
			updateAlarmList(form);
			if((sndCmd.param1 = alarmFreq[_pnlSensor[si].sound]))
				SndDoCmd(NULL, &sndCmd, true);
			return true;

		case threshSignButtonId:
			{
				UInt16 index = FrmGetObjectIndex(form, threshSignButtonId);
				ControlType* control = FrmGetObjectPtr(form, index);
				const Char *label = CtlGetLabel(control);
				_pnlSensor[si].polarity = *label == '<' ? 0 : 1;
				CtlSetLabel(control, *label == '<' ? ">" : "<");
			}
			return true;
		case triggerCheckId:
			{
				UInt16 index = FrmGetObjectIndex(form,triggerCheckId);
				_pnlSensor[si].trigger=FrmGetControlValue(form, index);
//				FrmSetControlValue(form, index, newValue);
			
			}
			return true;
		}
		return false;

	default:
		break;
	}

	return false;
}

/*
 * System Configuration
 */

Boolean systemFormHandleEvent(EventType *e) {
FormType *form = FrmGetActiveForm();
Err err;

	if(SysHandleEvent(e) || MenuHandleEvent(NULL, e, &err)) return true;
	
	switch(e->eType) {

	case frmOpenEvent:
		FrmDrawForm(form);
		return true;

	case ctlSelectEvent:
		switch(e->data.ctlSelect.controlID) {
	
	       case doneButtonId:
		      FrmReturnToForm(0);
		      return true;
		}
		return false;

	default:
		break;
	}

	return false;
}

/*
 * Sensor Setup
 */

Boolean sensorFormHandleEvent(EventType *e) {
FormType *form = FrmGetActiveForm();
	SndCommandType sndCmd = { sndCmdFrqOn, 0, 1000, 200, sndMaxAmp };
	static int si;
	Int32 i;
	Err err;

	if(SysHandleEvent(e) || MenuHandleEvent(NULL, e, &err)) return true;
	
	switch(e->eType) {

	case frmOpenEvent:

		for(i = 0; i < SENSOR_COUNT; i++)
			alarmListSlugs[i] = alarmListStrings + i * 6;

		si = selectedSensor & 0x1f;
		for(i = 0; i < SENSOR_COUNT; i++) {
			if(!_pnlSensor[si].exists)
				si = (si + 1) % SENSOR_COUNT;
			else
				break;
		}

		FrmDrawForm(form);
		updateAlarmList(form);
		newupdateAlarmForm(form, si);

		return true;

	case popSelectEvent:
		si = alarmListSensor[e->data.popSelect.selection];
		newupdateAlarmForm(form, si);
		return true;


	case frmCloseEvent:
		// if user switcher application when this form is open
		// post frmCloseEvent to caller form as it will get it
		// a chance to free resources
		FrmReturnToForm(0);
		return true;

	case ctlRepeatEvent:
		switch(e->data.ctlRepeat.controlID) {
		case threshSliderId:
			_pnlSensor[si].threshold = e->data.ctlRepeat.value;
			updateAlarmThreshold(form, si);
			return false;
		}
		break;

	case ctlSelectEvent:
		switch(e->data.ctlSelect.controlID) {
		case doneButtonId:
			FrmReturnToForm(0);
//			RefreshPanel();  //bug
			return true;

		case sound0ButtonId:
		case sound1ButtonId:
		case sound2ButtonId:
		case sound3ButtonId:
		case sound4ButtonId:
			_pnlSensor[si].sound = e->data.ctlSelect.controlID - sound0ButtonId;
			updateAlarmList(form);
			if((sndCmd.param1 = alarmFreq[_pnlSensor[si].sound]))
				SndDoCmd(NULL, &sndCmd, true);
			return true;

		case threshSignButtonId:
			{
				UInt16 index = FrmGetObjectIndex(form, threshSignButtonId);
				ControlType* control = FrmGetObjectPtr(form, index);
				const Char *label = CtlGetLabel(control);
				_pnlSensor[si].polarity = *label == '<' ? 0 : 1;
				CtlSetLabel(control, *label == '<' ? ">" : "<");
			}
			return true;
		}
		return false;

	default:
		break;
	}

	return false;
}

/*
 * Custom Sensor
 */
 
void doCustomDialog()
{
	FormType *form = FrmInitForm(customFormId);
	UInt16 i, index;
	FieldType *field;
	Char *s, str[16];

	for(i = 0; i < 8; i++) {
//		SensorType *sensor = &_pnlSensor[sensorPosition[currentBank * 8 + i]];
		SensorType *sensor = &_pnlSensor[sensorPosition[3 * 8 + i]];

		index = FrmGetObjectIndex(form, customSensorId+i*3);	// checkbox
		FrmSetControlValue(form, index, sensor->exists);

		index = FrmGetObjectIndex(form, customSensorId+i*3+1);	// addr
		field = FrmGetObjectPtr(form, index);
		StrIToH(str, sensor->addr);
		FldInsertText(field, str + 6);

		index = FrmGetObjectIndex(form, customSensorId+i*3+2);	// slug
		field = FrmGetObjectPtr(form, index);
		s = sensor->slug;
		FldInsertText(field, s);
	}

	i = FrmDoDialog(form);
	if(i == doneButtonId) {
		for(i = 0; i < 8; i++) {
			SensorType *sensor = &_pnlSensor[sensorPosition[3 * 8 + i]];

			index = FrmGetObjectIndex(form, customSensorId+i*3);	// checkbox
			sensor->exists = FrmGetControlValue(form, index) ? 1 : 0;

			index = FrmGetObjectIndex(form, customSensorId+i*3+1);	// addr
			field = FrmGetObjectPtr(form, index);
			s = FldGetTextPtr(field);
			while(s && *s) {
				index <<= 4;
				if(*s >= '0' && *s <= '9')
					index += *s - '0';
				else if(*s >= 'a' && *s <= 'f')
					index += *s - 'a' + 10;
				else if(*s >= 'A' && *s <= 'F')
					index += *s - 'A' + 10;
				s++;
			}
			sensor->addr = index;

			index = FrmGetObjectIndex(form, customSensorId+i*3+2);	// slug
			field = FrmGetObjectPtr(form, index);
			s = FldGetTextPtr(field);
			StrNCopy(sensor->slug, s ? s : "", 5);
		}
	}

	FrmDeleteForm (form);
}

/*
 *  e d i t
 */

/*
 *  update form according to current mode
 */

Boolean switchMode(UInt8 newMode)
{
	FormType *form = FrmGetActiveForm();
	
	UInt16 pauseCheckIndex = FrmGetObjectIndex(form, pauseCheckId);
	UInt16 logCheckIndex = FrmGetObjectIndex(form, logCheckId);
	UInt16 graphCheckIndex = FrmGetObjectIndex(form, graphCheckId);
	UInt16 peakCheckIndex = FrmGetObjectIndex(form, peakCheckId);
	UInt16 timeLabelIndex = FrmGetObjectIndex(form, timeLabelId);
	UInt16 scrollBarIndex = FrmGetObjectIndex(form, scrollBarId);

	if(newMode == currentMode) return false;

	closePort();
	{	// erase graph
		RectangleType r = { { 0, graphY - 14}, { 160, 1 } };
		WinEraseRectangle(&r, 0);
		r.topLeft.y = graphY - 12;
		r.extent.y = 11;
		WinEraseRectangle(&r, 0);
	}

	RefreshPanel();
	PnlUpdate(&panel, &currentSample);

	switch(newMode) {

	case REVIEW_MODE:
		currentMode = REVIEW_MODE;

		PnlSetMode(&panel, PANEL_MULTISELECT);
		PnlShowPanel(&panel);

		// hide Graph, Log and Pause checkboxen
		FrmHideObject(form, pauseCheckIndex);
		FrmHideObject(form, logCheckIndex);
		FrmHideObject(form, graphCheckIndex);
		FrmHideObject(form, peakCheckIndex);
		FrmShowObject(form, timeLabelIndex);

		// show graph
		GrfSetMode(&graph, 0);
		GrfShowCursor(&graph, 1);
		GrfUpdateGraph(&graph);

		// show graph scroller
		_setScroller(graph.position);
		FrmShowObject(form, scrollBarIndex);

		break;

	case MONITOR_MODE:
		currentMode = MONITOR_MODE;

		PnlSetMode(&panel, numericMode ? PANEL_SINGLESELECT : PANEL_MULTISELECT);
		PnlShowPanel(&panel);

		FrmHideObject(form, timeLabelIndex);
		FrmHideObject(form, scrollBarIndex);

		// set and show Pause checkbox
		FrmSetControlValue(form, pauseCheckIndex, 1);
		FrmSetControlValue(form, graphCheckIndex, !numericMode);
		FrmShowObject(form, pauseCheckIndex);
		FrmShowObject(form, graphCheckIndex);
		FrmShowObject(form, logCheckIndex);
		FrmShowObject(form, peakCheckIndex);

		// hide graph
		GrfSetMode(&graph, !FrmGetControlValue(form, graphCheckIndex));
		GrfEraseGraph(&graph);
		GrfShowCursor(&graph, 0);
		GrfAppendSample(&graph, 0);
	
		break;
	}

	return true;
}

Boolean editFormHandleEvent(EventType *e)
{
	Err err;
	FormType *form = FrmGetActiveForm();

	// give a chance to mode-specific handlers first
	if(currentMode == REVIEW_MODE && reviewHandleEvent(e)) return true;
	if(currentMode == MONITOR_MODE && monitorHandleEvent(e)) return true;
	// if serial comm in progress - block all system events
	// otherwise just block hardware buttons 1..4
	if(!talkingECU && !(
		e->eType == keyDownEvent && 
			(
			e->data.keyDown.chr == hard1Chr ||
			e->data.keyDown.chr == hard2Chr ||
			e->data.keyDown.chr == hard3Chr ||
			e->data.keyDown.chr == hard4Chr
			)
		) && (SysHandleEvent(e) || MenuHandleEvent(NULL, e, &err))) return true;

	// should be the last as it interfere with MenuHandleEvent
	if(PnlHandleEvent(&panel, e)) return true;

	switch(e->eType) {

	case frmOpenEvent:
		{
			FrmSetTitle(form, editFormTitle);
			FrmDrawForm(form);

			// TODO: move to resource some day
			graphX = 0;
			graphY = 70;
			graphW = 160;
			graphH = 64;

			// streamName was filled by main form
			stream = FileOpen(0, streamName, 0, CRID, fileModeUpdate, NULL);

			/* Insert SP 06/01/03 - default Hide Unused setting if off */
			FrmSetControlValue(form, FrmGetObjectIndex(form, hideUnusedButtonId), fHideUnused);
			/* End Insert */

			// panel
			PnlCreatePanel(&panel, panelId, NULL, &sensorPosition[currentBank * 8], selectedSensor, 0, 16, 160, 10 * 4);

			// create and paint the graph
			GrfCreateGraph(&graph, graphId, graphX, graphY, graphW, graphH, streamReader);
			// TODO: set graph without painting it
			GrfSetGraph(&graph, 0, 0, 0, graphSensors);
			GrfSetNumericMask(&graph, captureSensors);
			GrfSetSensor(&graph, selectedSensor);

			// hilite current bank
			FrmSetControlGroupSelection(form, bankGroupId, bank1ButtonId + currentBank);

			// hilite current mode
			FrmSetControlGroupSelection(form, modeGroupId, reviewButtonId + currentMode);

			WinDrawGrayLine(graphX, graphY - 13, graphX + graphW - 1, graphY - 13);
			WinDrawGrayLine(graphX, graphY - 1, graphX + graphW - 1, graphY - 1);
			WinDrawGrayLine(graphX, graphY + graphH, graphX + graphW - 1, graphY + graphH);
			
			// paint the form according to current mode
			switchMode(currentMode++);
		}
		return true;

	case frmCloseEvent:
		numericMode = graph._numericMode;
		GrfDestroyGraph(&graph);
		PnlDestroyPanel(&panel);
		{
			Int32 streamSize;
			Err err;

			FileTell(stream, &streamSize, &err);
			FileClose(stream);
			// delete log if empty
			if(streamSize == 0) FileDelete(0, streamName);
		}
		
		return false;

	case pnlSelectEvent:
		{
			PanelEventType *event = (PanelEventType *)e;

			if(graphSensors != event->data.pnlSelect.graph)
				GrfSetMask(&graph, event->data.pnlSelect.graph);

			if(captureSensors != event->data.pnlSelect.capture)
				GrfSetNumericMask(&graph, event->data.pnlSelect.capture);

			if(selectedSensor != event->data.pnlSelect.selection);
				GrfSetSensor(&graph, event->data.pnlSelect.selection);

			graphSensors = event->data.pnlSelect.graph;
			captureSensors = event->data.pnlSelect.capture;
			selectedSensor = event->data.pnlSelect.selection;
		}
		return true;

	case keyDownEvent:
		if(e->data.keyDown.modifiers & autoRepeatKeyMask) break;

		// decrement current bank
		if(e->data.keyDown.chr == pageUpChr) {
//			SndPlaySystemSound(sndClick);
			currentBank = (currentBank - 1) & 3;
			FrmSetControlGroupSelection(form, bankGroupId, bank1ButtonId + currentBank);
			RefreshPanel();
			return true;
		}
		// cincrement current bank
		if(e->data.keyDown.chr == pageDownChr) {
//			SndPlaySystemSound(sndClick);
			currentBank = (currentBank + 1) & 3;
			FrmSetControlGroupSelection(form, bankGroupId, bank1ButtonId + currentBank);
			RefreshPanel();
			return true;
		}
		break;

	case ctlSelectEvent:
		switch(e->data.ctlSelect.controlID) {

		case doneButtonId:
			FrmGotoForm(mainFormId);
			return true;

		case peakButtonId:
//			peakSample=resetSample;
			peakSample=scratchSample; // Solves problem with Coolant sensor where MAX T = 0 count
			minSample = scratchSample;
			return true;

		case bank1ButtonId:
		case bank2ButtonId:
		case bank3ButtonId:
		case bank4ButtonId:
			currentBank = (FrmGetControlGroupSelection(form, bankGroupId) - 1) & 3;
			/* Insert SP 06/01/03 */
			RefreshPanel();
			/* End Insert */
			return true;

		case reviewButtonId:
			switchMode(REVIEW_MODE);
			return true;

		case monitorButtonId:
			switchMode(MONITOR_MODE);
			return true;

		}
    		break;

	case menuEvent:
		switch (e->data.menu.itemID) {

		case alarmsMenuId:
			FrmPopupForm(alarmsFormId);
			return true;

		case prefsMenuId:
			doPrefsDialog();
			PnlSetBank(&panel, &sensorPosition[currentBank * 8]);
			panel.graph = graphSensors;
			panel.capture = captureSensors;
			GrfSetMask(&graph, panel.graph);
			GrfSetNumericMask(&graph, panel.capture);
			return true;

		case customMenuId:
			doCustomDialog();
			return true;
			
		case aboutMenuId:
			doAboutDialog();
			return true;
			
		case systemMenuId:
			FrmPopupForm(systemFormId);
			return true;
			
		case sensorMenuId:
			FrmPopupForm(sensorFormId);
			return true;

		case renameLogMenuId:
			{
				FormType *dialog = FrmInitForm(renameLogFormId);
				UInt16 fieldIndex = FrmGetObjectIndex(dialog, newNameFieldId);
				Int16 checkIndex = FrmGetObjectIndex(dialog, backupLogCheckId);
				FieldType *field = FrmGetObjectPtr(dialog, fieldIndex);

				// paste current log name into the field
				FldInsert(field, streamName, StrLen(streamName));
				// select field contents, so it would be deleted if user starts typing
				FldSetSelection(field, 0, StrLen(streamName));
				FrmSetFocus(dialog, fieldIndex);
				FrmSetControlValue(dialog, checkIndex, (streamAttr & dmHdrAttrBackup) ? 1 : 0);

				// show dialog
				if(FrmDoDialog(dialog) == okButtonId) {
					const Char *newName = FldGetTextPtr(field);
					Boolean streamBackup = FrmGetControlValue(dialog, checkIndex);

					// file can't have no name - don't even try
					if(StrLen(newName) <= 0)
						newName = streamName;

					{
						DmOpenRef dbRef = NULL;
						LocalID dbId = 0;
						Int32 size = sizeof(DmOpenRef);
						UInt16 newAttr = dmHdrAttrStream | (streamBackup ? dmHdrAttrBackup : 0);

						(void)(0
						|| FileControl(fileOpGetOpenDbRef, stream, &dbRef, &size)
						|| DmOpenDatabaseInfo(dbRef, &dbId, NULL, NULL, NULL, NULL)
						|| DmSetDatabaseInfo(0, dbId, StrCompare(streamName, newName) ? newName : NULL, &newAttr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
						|| StrCopy(streamName, newName)
						|| (streamAttr = newAttr)
						);
					}
				}

				FrmDeleteForm(dialog);
			}
			return true;

		case newLogMenuId:
			{
				if(FrmAlert(newLogAlertId) == 0) {
				}
			}
			return true;

		case deleteLogMenuId:
			if(FrmAlert(deleteLogAlertId) == 0) {
				// just truncate the log and switch to main
				// form. frmCloseEvent handler will delete
				// the file...
				FileSeek(stream, 0, fileOriginBeginning);
				FileTruncate(stream, 0);
				FrmGotoForm(mainFormId);
			}
			return true;

		case truncateLogMenuId:
			if(FrmAlert(truncateLogAlertId) == 0) {
				FileSeek(stream, 0, fileOriginBeginning);
				FileTruncate(stream, 0);
				GrfUpdateGraph(&graph);
				_setScroller(graph.position);
			}
			return true;

		default:
			/* let the system handle it */
			break;
		}
		break;

	default:
		break;
	}

	return false;
}

/*
 *  m a i n
 */

struct DirEntry {
	Char name[32];
	UInt16 attr;
};

struct DirEntry *dir;
Int16 dirSize;

#define ITEM_HEIGHT 11
#define ITEM_COUNT 11

Int16 compare(void *p1, void *p2, Int32 length)
{
	Char* s1 = ((struct DirEntry *)p1)->name;
	Char* s2 = ((struct DirEntry *)p2)->name;
	Int16 r;

	while(*s2) if((r = *s1++ - *s2++)) return r;
	return *s1;

	// couldn't believe it but PalmOS prior to 4.0 have no
	// plain strcmp! StrCompare is doing something obscure...

	/* return StrCompare((Char *)p1, (Char *)p2); */
}

void loadDirectory()
{
	Boolean newSearch = true;
	DmSearchStateType state;
	UInt16 cardNo;
	LocalID dbID;
	Int16 i;

	dirSize = 16;
	dir = MemPtrNew(dirSize * sizeof(struct DirEntry));

	i = 0;
	while(errNone == DmGetNextDatabaseByTypeCreator(newSearch, &state, sysFileTFileStream, CRID, false, &cardNo, &dbID)) {
		if(errNone == DmDatabaseInfo(cardNo, dbID,
			dir[i].name, &dir[i].attr /* attributesP */,
			NULL /* versionP */, NULL /* cdDateP */,
			NULL /* modDateP */, NULL /*bckUpDateP */,
			NULL /* modNumP */, NULL /* appInfoIDP */,
			NULL /* sortInfoIDP */,
			NULL /* *typeP */, NULL /* creatorP */))
		{
			if(++i >= dirSize) {
				void *p = MemPtrNew((dirSize + 16) * sizeof(struct DirEntry));
				MemMove(p, dir, dirSize * sizeof(struct DirEntry));
				MemPtrFree(dir);
				dir = p;
				dirSize += 16;
			}
		}

		newSearch = false;
	}

	dirSize = i;
	SysInsertionSort(dir, dirSize, sizeof(struct DirEntry), compare, 32);
}

void mainDrawItem(UInt16 i, Boolean highlight)
{
	if(i < topVisible + ITEM_HEIGHT) {
		UInt16 strWidth, strLen;
		Char str[32];
		RectangleType clip, rect = { { 0, 18 + i * ITEM_HEIGHT }, { 151, 11 } };

		WinPushDrawState();
		
		if(highlight) {
			WinSetBackColor(UIColorGetTableEntryIndex(UIObjectSelectedFill));
			WinSetTextColor(UIColorGetTableEntryIndex(UIObjectSelectedForeground));
		} else {
			WinSetBackColor(UIColorGetTableEntryIndex(UIObjectFill));
			WinSetTextColor(UIColorGetTableEntryIndex(UIObjectForeground));
		}

		WinEraseRectangle(&rect, 0);
		WinGetClip(&clip);
		WinSetClip(&rect);

		strLen = StrPrintF(str, "%d.", topVisible + i + 1);
		strWidth = FntCharsWidth(str, strLen);
		WinDrawChars(str, strLen, 14 - strWidth, 18 + i * ITEM_HEIGHT);

		if(dir[topVisible + i].attr & dmHdrAttrBackup)
			WinDrawChars("\225", 1, 146, 18 + i * ITEM_HEIGHT);

		rect.extent.x -= 6;
		WinSetClip(&rect);

		WinDrawChars(dir[topVisible + i].name, StrLen(dir[topVisible + i].name), 18, 18 + i * ITEM_HEIGHT);

		WinSetClip(&clip);
		WinPopDrawState();
	}
}

Boolean mainFormHandleEvent(EventType *e)
{
	FormType *form = FrmGetActiveForm();
	static Int16 hilitedItem;
	Int16 i;
	Err err;
	ScrollBarType* bar = FrmGetObjectPtr(form, FrmGetObjectIndex(form, mainScrollbarId));

	if(SysHandleEvent(e) || MenuHandleEvent(NULL, e, &err)) return true;
	
	switch(e->eType) {

	case frmOpenEvent:

		closePort();
		FrmDrawForm(form);
		hilitedItem = -1;

		loadDirectory();
		if(topVisible + ITEM_COUNT > dirSize) topVisible = MAX(0, dirSize - ITEM_COUNT); 

		for(i = 0; i < MIN(dirSize, ITEM_COUNT); i++)
			mainDrawItem(i, false);
		
		SclSetScrollBar(bar, topVisible, 0, MAX(0, dirSize - ITEM_COUNT), ITEM_COUNT);

		return true;

	case frmCloseEvent:
		MemPtrFree(dir);
		return false;

	case sclRepeatEvent:
		topVisible = e->data.sclRepeat.newValue;
		
		for(i = 0; i < MIN(dirSize, ITEM_COUNT); i++)
			mainDrawItem(i, false);

		return false;
	
	case penDownEvent:
		if(e->screenX < 150 && e->screenY >= 18 && e->screenY <= 18 + ITEM_COUNT * ITEM_HEIGHT) {
			i = (e->screenY - 18) / ITEM_HEIGHT;
			if(i >= 0 && i < dirSize) {
				mainDrawItem(i, true);
				hilitedItem = i;
	                	return true;
			}
		}
		return false;

	case penMoveEvent:
		i = (e->screenY - 18) / ITEM_HEIGHT;
		if(hilitedItem >= 0 && i != hilitedItem) {
			mainDrawItem(hilitedItem, false);
			hilitedItem = -1;
                	return true;
		}
		return false;

	case penUpEvent:
		i = (e->screenY - 18) / ITEM_HEIGHT;
		if(hilitedItem >= 0 && i == hilitedItem) {
			StrCopy(streamName, dir[topVisible + i].name);
			streamAttr = dir[topVisible + i].attr;
//			StrPrintF(editFormTitle, "MMCd Log %d of %d", i + 1 + topVisible, dirSize);
			StrPrintF(editFormTitle, "MMCd  Log %d", i + 1 + topVisible);
			
			FrmGotoForm(editFormId);
			return true;
		} else if(hilitedItem >= 0) {
			mainDrawItem(hilitedItem, false);
			hilitedItem = -1;
		}
		
		return false;

	case ctlSelectEvent:
		if(e->data.ctlSelect.controlID == newButtonId) {
			DateTimeType date;

			TimSecondsToDateTime(TimGetSeconds(), &date);
			StrPrintF(streamName, "%04d-%02d-%02d %02d:%02d:%02d", date.year, date.month, date.day, date.hour, date.minute, date.second);
			streamAttr = dmHdrAttrStream;
			StrPrintF(editFormTitle, "MMCd Log"); 
			FrmGotoForm(editFormId);
			newLog=true;  // Set when creating new log file
			return true;
		}
		return false;

	case menuEvent:
		switch (e->data.menu.itemID) {

		case alarmsMenuId:
			FrmPopupForm(alarmsFormId);
			return true;

		case aboutMenuId:
			doAboutDialog();
			return true;

		case testMenuId:
			FrmGotoForm(testFormId);
			return true;

		case debugMenuId:
			FrmGotoForm(debugFormId);
			return true;

		case prefsMenuId:
			doPrefsDialog();
//			PnlSetBank(&panel, &sensorPosition[currentBank * 8]); // Test to see if this updates panel slugs
			return true;
			
		case customMenuId:
			doCustomDialog();
			return true;
			
		case systemMenuId:
			FrmPopupForm(systemFormId);
			return true;
			
		case sensorMenuId:
			FrmPopupForm(sensorFormId);
			return true;

		default:
			break;

		}
	default:
		break;
	}

	return false;
}


Boolean appHandleEvent(EventType *e)
{
	if(e->eType == frmLoadEvent) {
		// Load the form resource.
		UInt16 formID = e->data.frmLoad.formID;
		FormPtr form = FrmInitForm(formID);
		FrmSetActiveForm(form);

		switch(formID) {

		case mainFormId:
			FrmSetEventHandler(form, mainFormHandleEvent);
			break;

		case editFormId:
			FrmSetEventHandler(form, editFormHandleEvent);
			break;

		case testFormId:
			FrmSetEventHandler(form, testFormHandleEvent);
			break;

		case debugFormId:
			FrmSetEventHandler(form, debugFormHandleEvent);
			break;

		case alarmsFormId:
			FrmSetEventHandler(form, alarmsFormHandleEvent);
			break;

		case systemFormId:
			FrmSetEventHandler (form, systemFormHandleEvent);
			break;
			
		case sensorFormId:
			FrmSetEventHandler (form, sensorFormHandleEvent);
			break;
			
		default:
			ErrFatalDisplayIf(1, "Unknown form!");
			break;
		}
	}
	return false;
}

void loadPrefs()
{
//	Prefs prefs = { 0, 0, 0, 0 };
	PrefsType prefs = { 0, 0, 0, 0 };
	
	UInt16 size = sizeof(PrefsType);
	UInt16 chksize = 0;
	UInt32 i;
	Err err = !errNone;

	// initialize sensors
	MemMove(_pnlSensor, _pnlSensorDefault, sizeof(_pnlSensor));

	// check size of preferences before reading 
	PrefGetAppPreferences(CRID, 0, NULL, &chksize, false);
	if (chksize == size) err = PrefGetAppPreferences(CRID, 0, &prefs, &size, false);

	if (err != errNone) {  // Prefs struct has changed size so initialize 
		PrefSetAppPreferences(CRID, 0, 0, NULL, 0, false);
		setDefaults(); 	
	} else {
		for(i = 0; i < SENSOR_COUNT; i++) {
			_pnlSensor[i].capture = !!(prefs.capture & (1L << i));
			_pnlSensor[i].graph = !!(prefs.graph & (1L << i));
			_pnlSensor[i].exists = !!(prefs.exists & (1L << i));
			_pnlSensor[i].polarity = !!(prefs.polarity & (1L << i));
			_pnlSensor[i].addr = prefs.addr[i];
			_pnlSensor[i].threshold = prefs.threshold[i];
			_pnlSensor[i].trigger = prefs.trigger[i];
			_pnlSensor[i].sound = prefs.sound[i];
			StrNCopy(_pnlSensor[i].slug, prefs.slug[i], 4);
		}
		graphSensors = prefs.graph;
		captureSensors = prefs.capture;
		currentBank = prefs.currentBank;
		currentMode = prefs.currentMode;
		topVisible = prefs.topVisible;
		serialSpeed = prefs.serialSpeed;
		ecuSpeed = prefs.ecuSpeed;
		overClock = prefs.overClock; //Insert SS 4/3/05
		metricUnit = prefs.metricUnit;
		mapScaling = prefs.mapScaling;
		vehicleSelect = prefs.vehicleSelect;
		baseTiming = prefs.baseTiming;
		audibleAlarms = prefs.audibleAlarms;
		selectedSensor = prefs.selectedSensor;
		fHideUnused = prefs.hideUnused;
		numericMode = prefs.numericMode;
		audibleAlarms = prefs.audibleAlarms;
		autologEnabled = prefs.autologEnabled;
		o2NonLinEnabled = prefs.o2NonLinEnabled;
		
		MemMove(bigNumSelect, prefs.bigNumSelect, sizeof(bigNumSelect)); // V1.7d

	}
}

void savePrefs()
{
	UInt16 i;
//	Prefs prefs;
	PrefsType prefs;

	prefs.capture = captureSensors;
	prefs.graph = graphSensors;
	prefs.currentBank = currentBank;
	prefs.currentMode = currentMode;
	prefs.topVisible = topVisible;
	prefs.serialSpeed = serialSpeed;
	prefs.ecuSpeed = ecuSpeed;
	prefs.overClock = overClock; // Insert SS 4/3/05
	prefs.metricUnit = metricUnit;
	prefs.mapScaling = mapScaling;
	prefs.vehicleSelect = vehicleSelect;
	prefs.baseTiming = baseTiming;
	prefs.selectedSensor = selectedSensor;
	prefs.hideUnused = fHideUnused;
	prefs.numericMode = numericMode;
	prefs.audibleAlarms = audibleAlarms;
	prefs.autologEnabled = autologEnabled;
	prefs.o2NonLinEnabled = o2NonLinEnabled;
	MemMove(prefs.bigNumSelect, bigNumSelect, sizeof(bigNumSelect)); // V1.7d

	prefs.exists = 0;
	prefs.polarity = 0;

	for(i = 0; i < SENSOR_COUNT; i++) {
		if(_pnlSensor[i].exists) prefs.exists |= (1L << i);
		prefs.addr[i] = _pnlSensor[i].addr;
		MemMove(prefs.slug[i], _pnlSensor[i].slug, 4);
		prefs.threshold[i] = _pnlSensor[i].threshold;
		prefs.trigger[i] = _pnlSensor[i].trigger;
		if(_pnlSensor[i].polarity) prefs.polarity |= (1L << i);
		prefs.sound[i] = _pnlSensor[i].sound;
	}

	PrefSetAppPreferences(CRID, 0, 0, &prefs, sizeof(prefs), false);
}

UInt32 PilotMain(UInt16 cmd, void *pbp, UInt16 flags __attribute__ ((unused)))
{
	switch(cmd) {
	case sysAppLaunchCmdNormalLaunch:
		{
			EventType event;
			Boolean screenEnableColor;
			UInt32 version;

			// check if PalmOS version is 3.5 or newer
			if(errNone != FtrGet(sysFtrCreator, sysFtrNumROMVersion, &version) || version < 0x03503000) {
				FrmAlert(versionAlertId);
				return 0;
			}

			colorMode = false;
			screenDensity = 1;

			if(errNone == WinScreenMode(winScreenModeGet,
			   &screenWidth, &screenHeight, &screenDepth,
			   &screenEnableColor)) {
				if(screenDepth >= 8) colorMode = true;
			}

			if(errNone == FtrGet(sysFtrCreator, sysFtrNumWinVersion, &version) && version >= 4) {
				UInt32 attr;
				WinScreenGetAttribute(winScreenDensity, &attr);

				switch(attr) {
				case kDensityDouble:
					screenDensity = 2;
					break;
				case kDensityTriple:
					screenDensity = 3;
					break;
				case kDensityQuadruple:
					screenDensity = 4;
					break;
				}
			}

			// If on Handspring device, disable the keyboard
			// thread before opening the serial library.
			if(!FtrGet('hsEx', 0, &version))
				HsExtKeyboardEnable(false);

			AllocateMem();

			ticksPerSecond = SysTicksPerSecond();

			loadPrefs();
			FrmGotoForm(mainFormId);

			bigFont = MemHandleLock(DmGetResource('NFNT', bigFontId));
			FntDefineFont(fntAppFontCustomBase, bigFont);

			talkingECU = false;
			portOpen = false;

			do {
				/* Modified SP 06/03/03 - Wait up to 1 second for events */
				/* reduced timeout to 1/50 sec, because null events are
				   used by graph widget to scroll diplay when pen
				   is dragged to the edge of the screen -/dmitry */
				EvtGetEvent(&event, talkingECU ? 0 : ticksPerSecond / 50);
				/* End Modified */
				(void) (
				        appHandleEvent(&event)
				     || FrmDispatchEvent(&event)
				);
			} while(event.eType != appStopEvent);

			MemHandleUnlock(DmGetResource('NFNT', bigFontId)); //Added SS 11/23/04 to avoid chunk over-locked error
			FrmCloseAllForms();
			closePort();	// just in case
			savePrefs();
			ReleaseMem();
			
		}
		break;

	case sysAppLaunchCmdSystemReset: 
		break;

	case sysAppLaunchCmdAlarmTriggered:
		{
			SysAlarmTriggeredParamType *param = pbp;
			param->purgeAlarm = true;
		}
		break;
	}

	return 0;
}

