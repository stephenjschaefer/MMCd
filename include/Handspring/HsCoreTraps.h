/***************************************************************
*
*  Project:
*	Handspring Core Traps
*
*  Copyright info:
*	Copyright 2001 Handspring, Inc. All Rights Reserved.   
*
*
*  FileName:
*	HsCoreTraps.h
* 
*  Description:
*	Public header file for the Handspring extension traps to the PalmOS
*
*  ToDo:
* 
*  History:
*	10-Apr-2001 RR - Created by Rajan Ranga
****************************************************************/

#ifndef	  __HSCORETRAPS_H__
#define	  __HSCORETRAPS_H__


//=============================================================================
// Traps
//=============================================================================
// These are added by Handspring as new OS calls
#define	sysTrapWinSetForeColorRGB					0xA3E8
#define	sysTrapWinSetBackColorRGB					0xA3E9
#define	sysTrapWinSetTextColorRGB					0xA3EA
#define	sysTrapWinGetPixelRGB						  0xA3EB


//=============================================================================
// Prototypes
//=============================================================================
void WinSetForeColorRGB (const RGBColorType* newRgbP, RGBColorType* prevRgbP)
      SYS_TRAP(sysTrapWinSetForeColorRGB);

void WinSetBackColorRGB (const RGBColorType* newRgbP, RGBColorType* prevRgbP)
      SYS_TRAP(sysTrapWinSetBackColorRGB);

void WinSetTextColorRGB (const RGBColorType* newRgbP, RGBColorType* prevRgbP)
      SYS_TRAP(sysTrapWinSetTextColorRGB);

void WinGetPixelRGB (Coord x, Coord y, RGBColorType* rgbP)
      SYS_TRAP(sysTrapWinGetPixelRGB);


#endif __HSCORETRAPS_H__