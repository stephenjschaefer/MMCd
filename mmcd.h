/* 
 *  $Id: mmcd.h,v 1.11 2003/06/30 08:26:03 dmitry Exp $
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

#ifndef __MMCD_H__
#define __MMCD_H__

#include "resource.h"

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#define ECU_ACTIVE_DTC_LOW		0x38
#define ECU_ACTIVE_DTC_HIGH		0x39
#define ECU_STORED_DTC_LOW		0x3b
#define ECU_STORED_DTC_HIGH		0x3c
#define ECU_ERASE_DTC			0xca

#define ECU_CMD_TIMEOUT			10

#define ECU_INJ_1			0xfc
#define ECU_INJ_2			0xfb
#define ECU_INJ_3			0xfa
#define ECU_INJ_4			0xf9
#define ECU_INJ_5			0xf8
#define ECU_INJ_6			0xf7

#define ECU_FUEL_PUMP			0xf6
#define ECU_PURGE_SOL			0xf5
#define ECU_PRESSURE_SOL		0xf4	/* ? */
#define ECU_EGR_SOL			0xf3
#define ECU_MVIC_MOTOR			0xf2
#define ECU_BOOST_SOL			0xf1	/* ? */

#define SENSOR_COUNT 32

Err openPort(UInt32 baudRate, Boolean enableFlowControl);
Err closePort();
void doPrefsDialog();
void doAboutDialog();
void doCustomDialog();  //Insert SS 1/6/05 Avoids implicit declaration warning

Boolean debugFormHandleEvent(EventType *e);
Boolean testFormHandleEvent(EventType *e);

extern UInt32 serialSpeed;
extern UInt16 portId;
extern Boolean portOpen;
extern UInt32 ticksPerSecond;
extern Int16 screenDensity;

/* #define ERROR_CHECK_LEVEL 2 */

#endif	// __MMCD_H__