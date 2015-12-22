/* 
 *  $Id: format.c,v 1.7 2003/06/30 08:24:45 dmitry Exp $
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

#include <StringMgr.h>
#include "format.h"
#include "panel.h"

#define DEG "\260"	// degree symbol

extern UInt8 metricUnit;
extern UInt8 mapScaling; // Scaling selection for MAP sensor
extern UInt8 vehicleSelect;
extern UInt32 baseTiming;
extern UInt8 overClock;

/*

MMC Galant E3x, MMC Eclipse 1G, ...

*/

char snumeric[10];

char *numOnly(char *soriginal){ // cb Function toi return only numeric portion of acaled 
    UInt8 index=0;
    while(index<9){
	  snumeric[index]=soriginal[index];
	  index++;
 	  if ( (soriginal[index]< 43) ||  (soriginal[index]> 57 ) ) // Only retuien numbers and - sign
 	  	break;
 	}
 	snumeric[index]=0;  // Terminate string with null			  	
	return snumeric;	// Return pointer to snumeric string
}

// just decimal value 0..255
Int16 f_DEC(UInt8 d, Char *s) {
	return StrPrintF(s, "%u", (UInt16)d);
}

// hexadecimal value
Int16 f_HEX(UInt8 d, Char *s) {
	return StrPrintF(s, "%x", d);
}


// Changed FLG0 / FLG2 tags here and in Panel.C 
// to make source code more readable - cb

// Flag 02 status bits 
Int16 f_FLG2(UInt8 d, Char *s) {
	return StrPrintF(s, "%s%s%s", //Modified SS 11/22/04 Removed trailing %
		d & 0x20 ? "-" : "A",  // Air conditioning relay???
		d & 0x10 ? "F" : "-",  // Fuel pump relay 
		d & 0x04 ? "B" : "-"   // Boost solenoid
	);
}

// Flag 00 status bits
Int16 f_FLG0(UInt8 d, Char *s) {
	return StrPrintF(s, "%s%s%s%s%s", 
		d & 0x04 ? "-" : "T", // TDC 
		d & 0x08 ? "S" : "-", // Power Steering Pressure SW
		d & 0x10 ? "-" : "A", // AC switch
		d & 0x20 ? "-" : "N", // Neutral
		d & 0x80 ? "I" : "-"  // Idle switch
	);
}


// Air Temperature, degrees C
// interp - 60
Int16 f_AIRT(UInt8 d, Char *s) {
	static const UInt8 interp[17] = {
		0xF4, 0xB0, 0x91, 0x80, 0x74, 0x6A, 0x62, 0x5A,
		0x53, 0x4C, 0x45, 0x3E, 0x35, 0x2B, 0x1D, 0x01,
		0x01
	};

	UInt8 v1 = interp[d / 16], v2 = interp[d / 16 + 1];

	if(metricUnit == PREFS_UNIT_METRIC) {
		Int32 v = v1 * 65536L - (d % 16) * (v1 - v2) * 4096L - 60 * 65536L;
		return StrPrintF(s, "%ld.%ld" DEG "C", v >> 16, (v & 0xffff) * 10 >> 16);
//		return StrPrintF(s, "%ld" DEG "C", v >> 16);
	} else if(metricUnit == PREFS_UNIT_ENGLISH) {
		Int32 v = v1 * 65536L * 9/5 - (d % 16) * (v1 - v2) * 4096L  * 9/5 - 60 * 65536L * 9/5 + 32 * 65536L;
		//return StrPrintF(s, "%ld.%ld" DEG "F", v >> 16, (v & 0xffff) * 10 >> 16);
		return StrPrintF(s, "%ld" DEG "F", v >> 16);
	} else {
		return f_DEC(d, s);
	}
}

// Coolant temperature, degrees C
// interp - 80
Int16 f_COOL(UInt8 d, Char *s) {
	static const UInt8 interp[17] = {
		0xEE, 0xBE, 0xA0, 0x90, 0x84, 0x7B, 0x73, 0x6C,
		0x65, 0x5F, 0x58, 0x51, 0x49, 0x40, 0x33, 0x15,
		0x15
	};

	UInt8 v1 = interp[d / 16], v2 = interp[d / 16 + 1];
	
        if(metricUnit == PREFS_UNIT_METRIC) {
		Int32 v = v1 * 65536L - (d % 16) * (v1 - v2) * 4096L - 80 * 65536L;
		return StrPrintF(s, "%ld.%ld" DEG "C", v >> 16, (v & 0xffff) * 10 >> 16);
//		return StrPrintF(s, "%ld" DEG "C", v >> 16);
	} else if(metricUnit == PREFS_UNIT_ENGLISH) {
		Int32 v = v1 * 65536L * 9/5 - (d % 16) * (v1 - v2) * 4096L  * 9/5 - 80 * 65536L * 9/5 + 32 * 65536L;
		//return StrPrintF(s, "%ld.%ld" DEG "F", v >> 16, (v & 0xffff) * 10 >> 16);
		return StrPrintF(s, "%ld" DEG "F", v >> 16);
	} else {
		return f_DEC(d, s);
	}
}


// EGRT Temperature, degrees C
// -1.5x + 314.2(7)
// F: -2.7x + 597.7
Int16 f_EGRT(UInt8 d, Char *s) {
	if(metricUnit == PREFS_UNIT_METRIC) {
		Int32 v = -98304L * d + 20596508L;
		return StrPrintF(s, "%ld.%ld" DEG "C", v >> 16, (v & 0xffff) * 10 >> 16);
	} else if(metricUnit == PREFS_UNIT_ENGLISH) {
		Int32 v = -176947L * d + 39170867L;
		return StrPrintF(s, "%ld.%ld" DEG "F", v >> 16, (v & 0xffff) * 10 >> 16);
	} else {
		return f_DEC(d, s);
	}
}


/*
 Original Motorolla 2.5 BAR MPX4250 Testing (
 BAR = x * 0.010989 
 PSI = x * 0.1593736 - 14.503

 INT32 Match scaler  = 65536

 Updated calibration from datasheets

 3 BAR	
 BAR = (ADC * 0.0117647) + .1
 PSI = (ADC * 0.170632588) - 13.0534 

 2.5 BAR	
 BAR = (ADC * 0.0098039) +.1
 PSI = (ADC * 0.142193511) - 13.0534 

 2.0 BAR	
 BAR = (ADC * 0.0078431) +.1
 PSI = (ADC * 0.113755059) - 13.0534 

 Wideband (0-255 counts = 10-20 AF)
 AF = (ADC * .03921569) + 10

 Updated Wideband (0-255 counts = 8-22 AF)
 AF = (ADC * .054902) + 8
 
*/


Int16 f_MAP(UInt8 d, Char *s) {

	static const Int32 metricScale[5]= {
	0.0117647 * 65535L,  // 3 Bar
	0.0098039 * 65535L,  // 2.5 Bar
	0.0078431 * 65535L,  // 2.0 Bar
	0.054902 * 65535L,   // AFR 8-22
	-98304L        };    // EGRT

	static const Int32 metricOffset[5]= {
	0.1 * 65535L,
	0.1 * 65535L,
	0.1 * 65535L,
	8.0 * 65535L,
	20596508L     };

	static const Int32 englishScale[5]= {
	0.17063 * 65535L,
	0.14219 * 65535L,
	0.11375 * 65535L,
	0.054902 * 65535L,
	-176947L      };

	static const Int32 englishOffset[5]= {
	-13.0534 * 65535L,
	-13.0534 * 65535L,
	-13.0534 * 65535L,
 	8.0 * 65535L,
	391708L      };

	
	switch(mapScaling) {
	Int32 	v;
		
	case 0:
	case 1:
	case 2:
		if(metricUnit == PREFS_UNIT_METRIC) {
			v = metricScale[mapScaling]* d +  metricOffset[mapScaling];
			return StrPrintF(s, "%ld.%02ld bar", v >> 16, (v & 0xffff) * 100 >> 16);
		} else if(metricUnit == PREFS_UNIT_ENGLISH) {
			v = englishScale[mapScaling]* d +  englishOffset[mapScaling];
			if (v > 0 )
				return StrPrintF(s, "%ld.%01ld psi", v >> 16, (v & 0xffff) * 10 >> 16);
		 	else if (v < -65535) 
				return StrPrintF(s, "%ld.%01ld psi", v >> 16, ((v & 0xffff) ^ 0xffff ) * 10 >> 16);
			else 
				return StrPrintF(s, "-0.%01ld psi", ((v & 0xffff) ^ 0xffff ) * 10 >> 16);
		} else 
			return f_DEC(d, s);
		
	case 3:{     // cb - Had to add brace to eliminate compiler error
		v = metricScale[mapScaling]* d +  metricOffset[mapScaling];
		return StrPrintF(s, "%ld.%01ld afr", v >> 16, (v & 0xffff) * 10 >> 16);
		}

	case 4:
		if(metricUnit == PREFS_UNIT_METRIC) {
			v = metricScale[mapScaling]* d +  metricOffset[mapScaling];
			return StrPrintF(s, "%ld.%ld" DEG "C", v >> 16, (v & 0xffff) * 10 >> 16);
		} else if(metricUnit == PREFS_UNIT_ENGLISH) {
			v = englishScale[mapScaling]* d +  englishOffset[mapScaling];
			return StrPrintF(s, "%ld.%ld" DEG "F", v >> 16, (v & 0xffff) * 10 >> 16);
		} else 
			return f_DEC(d, s);

	case 5:{
		v = 1280L * d;
		return StrPrintF(s, "%ld.%02ld V", v >> 16, (v & 0xffff) * 100 >> 16);
		}
	}
	return 0; // Default case to supress compiler warning
}

/*
Fuel and oil Pressure using Honeywell Pressure Sensor

.500V = 0 PSI
4.5V  = 100 PSI
 
PSI = (ADC * 0.48828) - 12.207
ATM = (ADC * .0336669) - .84167

BAR = PSI / 14.50326 -
  
*/

Int16 f_PRES(UInt8 d, Char *s) {
	if(metricUnit == PREFS_UNIT_METRIC) {
		Int32 v = 2206L * d - 55160;
		return StrPrintF(s, "%ld.%02ld atm", v >> 16, (v & 0xffff) * 100 >> 16);
	} else if(metricUnit == PREFS_UNIT_ENGLISH) {
		Int32 v = 32000L * d - 791020L;
		return StrPrintF(s, "%ld.%01ld psi", v >> 16, (v & 0xffff) * 10 >> 16);
	} else {
		return f_DEC(d, s);
	}
}


// Battery voltage, 0.0..18.7 volts
// Also used for voltage divider input for fuel pump voltage
// 0.0733 * x
Int16 f_BATT(UInt8 d, Char *s) {
	Int32 v = 4804L * d;
	return StrPrintF(s, "%ld.%ld V", v >> 16, (v & 0xffff) * 10 >> 16);
}

// Engine speed, 0..8000 rpm
// 31.25 * x
Int16 f_ERPM(UInt8 d, Char *s) {

	static const Int32 rpmScale[4]= {
	31.25 * 65536L, //Stock ECU
	1.10 * 31.25 * 65535L, //10% overclock
	1.125 * 31.25 * 65535L, //12.5% overclock
	1.15 * 31.25 * 65535L };  //15% overclock
	
Int32 v;
v = rpmScale[overClock ] * d;
//Int32 v = 2048000L * d;
return StrPrintF(s, "%ld", v >> 16);
}

// Injector pulse duty period, 0.0..65.0 ms
// 0.256 * x
Int16 f_INJP(UInt8 d, Char *s) {
	Int32 v = 16777L * d;
//	return StrPrintF(s, "%ld.%02ld ms", v >> 16, (v & 0xffff) * 100 >> 16);
	return StrPrintF(s, "%ld.%01ld ms", v >> 16, (v & 0xffff) * 10 >> 16);
}

// Barometric pressure, 0.00..1.24 bar
// 0.00486 * x
// 1 bar = 14.50326 psi
Int16 f_BARO(UInt8 d, Char *s) {
	if(metricUnit == PREFS_UNIT_METRIC) {
		Int32 v = 319L * d;
		return StrPrintF(s, "%ld.%02ld bar", v >> 16, (v & 0xffff) * 100 >> 16);
//		return StrPrintF(s, "%ld.%01ld bar", v >> 16, (v & 0xffff) * 10 >> 16);
	} else if(metricUnit == PREFS_UNIT_ENGLISH) {
		Int32 v = 4619L * d;
//		return StrPrintF(s, "%ld.%02ld psi", v >> 16, (v & 0xffff) * 100 >> 16);
		return StrPrintF(s, "%ld.%01ld psi", v >> 16, (v & 0xffff) * 10 >> 16);
	} else {
		return f_DEC(d, s);
	}
}

// Mass air flow sensor, 0.0..1600.0 Hz
// 6,29 * x

//  0-3200hz for DSM's who've changed their MAF hz logging
// 12.58 * x 

Int16 f_AIRF(UInt8 d, Char *s) {
 static const Int32 mafScale[6]= {
 6.27451 * 65535L,     // Normal CPU 1600 HZ 412221L
 1.100 * 6.27451 * 65535L,  // 10% Overclock
 1.125 * 6.27451 * 65535L,   // 12.5% Overclock
 1.150 * 6.27451 * 65535L,   // 15% Overclock
 1.200 * 6.27451 * 65535L,   // 20% Overclock
 2.000 * 6.27451 * 65535L }; // 3200 MAF for DSM Guys
 
 Int32 v;
 
 v = mafScale[overClock ] * d; 
 
// return StrPrintF(s, "%ld.%ld Hz", v >> 16, (v & 0xffff) * 10 >> 16);  // xxxx.x format
 return StrPrintF(s, "%ld Hz", v >> 16); // xxxx format
}




// Throttle position, 0..100%
Int16 f_THRL(UInt8 d, Char *s) {
	Int32 v = 25600L * d;
	return StrPrintF(s, "%ld.%ld%%", v >> 16, (v & 0xffff) * 10 >> 16);
}

// Fuel trim, 0..200% - Also used for INJD display scaling
// f: 0.7843 * x
Int16 f_FTxx(UInt8 d, Char *s) {
	Int32 v = 51200L * d;
	if (d <128)
		return StrPrintF(s, "%ld.%ld%%", v >> 16, (v & 0xffff) * 10 >> 16);
	else
		return StrPrintF(s, "%ld%%", v >> 16);
}

// Oxygen sensor voltage, 0.00..5.00 volts
// f: 0.0196 * x
Int16 f_OXYG(UInt8 d, Char *s) {
	Int32 v = 1280L * d;
	return StrPrintF(s, "%ld.%02ld V", v >> 16, (v & 0xffff) * 100 >> 16);
}

// Timing advance, -10 .. 245 deg
// f: x - 10
Int16 f_TIMA(UInt8 d, Char *s) {
    if(vehicleSelect == 0) {
	   return StrPrintF(s, "%ld" DEG, (Int32)d - 20 - 5 + baseTiming); //Modified SS 4/3/05 change in logic for addition of base timing
	} else {
	   return StrPrintF(s, "%ld" DEG, (Int32)d - 10 - 5 + baseTiming);
	}
}
