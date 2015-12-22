/* 
 *  $Id: test.c,v 1.1 2003/06/30 08:41:05 dmitry Exp $
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
#include "mmcd.h"
#include "panel.h"

UInt16 activeFaults;
UInt16 storedFaults;

void testSendCommand(UInt8 cmd, UInt32 timeout) {
	UInt8 buf[2] = { cmd, 0x00 };
	Err err;

	openPort(serialSpeed, false);
	ErrTry {
		SrmSend(portId, &buf[0], 1, &err);
		if(err != errNone) ErrThrow(err);
		SrmReceive(portId, &buf[0], 1, ticksPerSecond / 10, &err);
		if(err != errNone || buf[0] != cmd) ErrThrow(err);
		SrmReceive(portId, &buf[1], 1, ticksPerSecond * timeout, &err);
		if(err != errNone) ErrThrow(err);
	} ErrCatch(exception __attribute__ ((unused))) {
		SrmClearErr(portId);
		SrmReceiveFlush(portId, 0);
	} ErrEndCatch
	closePort();
}

UInt16 testReadFaults(Boolean active)
{
	UInt8 req[2][2] = { 
		{ ECU_ACTIVE_DTC_LOW, ECU_ACTIVE_DTC_HIGH },
		{ ECU_STORED_DTC_LOW, ECU_STORED_DTC_HIGH },
	};	// fault code ecu locations
	UInt8 buf[4] = { 0x00, 0x00, 0x00, 0x00 };

	UInt16 fault = 0, i, y = active ? 32 : 53;
	static Char str[36];
	Err err = errNone;
	RectangleType r = { { 42, y }, { 118, 18 } };

	WinEraseRectangle(&r, 0);
	WinDrawChars("Reading...", 10, 42, y);
	
	openPort(serialSpeed, false);
	ErrTry {
		SrmSend(portId, &req[!active][0], 1, &err);
		if(err != errNone) ErrThrow(err);
		SrmReceive(portId, &buf[0], 2, ticksPerSecond / 4, &err);
		if(err != errNone) ErrThrow(err);

		SrmSend(portId, &req[!active][1], 1, &err);
		if(err != errNone) ErrThrow(err);
		SrmReceive(portId, &buf[2], 2, ticksPerSecond / 4, &err);
		if(err != errNone) ErrThrow(err);

		fault = buf[1] + ((UInt16)buf[3] << 8);
	} ErrCatch(exception __attribute__ ((unused))) {
		SrmClearErr(portId);
		SrmReceiveFlush(portId, 0);
	} ErrEndCatch
	closePort();

	WinEraseRectangle(&r, 0);

	if(err != errNone) {
		// 17 "Serial comm error" if nothing was received
		// 18 "ECU comm error" if only command byte was echoed back by the adapter
		SysStringByIndex(faultStringsId, buf[0] != req[!active][0] ? 17 : 18, str, 35);
		WinDrawChars(str, StrLen(str), 42, y);
		return 0;
	}

	if(buf[0] != req[!active][0] || buf[2] != req[!active][1]) {
		SysStringByIndex(faultStringsId, 18, str, 35);	// ECU comm error
		WinDrawChars(str, StrLen(str), 42, y);
		return 0;
	}

	// no faults
	if(fault == 0) {
		SysStringByIndex(faultStringsId, 16, str, 35);
		WinDrawChars(str, StrLen(str), 42, y);
		return 0;
	}
	
	for(i = 0; i < 16; i++) {
		if(fault & (1<<i)) {
			SysStringByIndex(faultStringsId, i, str, 35);
			WinDrawChars(str + 1, 2, 42 + (i&7) * 15, y + (i >> 3) * 9);
		}
	}
	
	return fault;
}

Boolean testFormHandleEvent(EventType *e)
{
	Err err;
	static Char str[36];
	FormType *form = FrmGetActiveForm();

	if(SysHandleEvent(e) || MenuHandleEvent(NULL, e, &err)) return true;

	switch(e->eType) {

	case frmOpenEvent:
		{
			FrmDrawForm(form);
		}
		return true;

	case frmCloseEvent:
		return false;

	case penDownEvent:
		// ugly, i know...
		if(e->screenX >= 42 && e->screenX <= 42 + 8 * 15 &&
		   e->screenY >= 32 && e->screenY <= 32 + 2 * 9) {
		   	UInt16 n = (e->screenY - 32) / 9 * 8 + (e->screenX - 42) / 15;
		   	
		   	if(activeFaults & (1 << n)) {
			   	SysStringByIndex(faultStringsId, n, str, 36);
				FrmCopyLabel(form, descrLabelId, str);
		   	}
		} else if(e->screenX >= 42 && e->screenX <= 42 + 8 * 15 &&
			  e->screenY >= 53 && e->screenY <= 53 + 2 * 9) {
		   	UInt16 n = (e->screenY - 53) / 9 * 8 + (e->screenX - 42) / 15;
		   	
		   	if(storedFaults & (1 << n)) {
			   	SysStringByIndex(faultStringsId, n, str, 36);
				FrmCopyLabel(form, descrLabelId, str);
		   	}
		}
		break;
	
	case ctlSelectEvent:
		switch(e->data.ctlSelect.controlID) {

		case doneButtonId:
			FrmGotoForm(mainFormId);
			return true;

		case readFaultsButtonId:
			activeFaults = testReadFaults(true);
			storedFaults = testReadFaults(false);
			return true;

		case eraseFaultsButtonId:
			if(FrmAlert(eraseFaultsAlertId) == 0) {
				testSendCommand(ECU_ERASE_DTC, 1);
				activeFaults = testReadFaults(true);
				storedFaults = testReadFaults(false);
			}
			return true;

		case pumpButtonId:
			FrmSetControlValue(form, FrmGetObjectIndex(form, pumpButtonId), 1);
			testSendCommand(ECU_FUEL_PUMP, ECU_CMD_TIMEOUT);
			FrmSetControlValue(form, FrmGetObjectIndex(form, pumpButtonId), 0);
			return true;

		case pressureButtonId:
			FrmSetControlValue(form, FrmGetObjectIndex(form, pressureButtonId), 1);
			testSendCommand(ECU_PRESSURE_SOL, ECU_CMD_TIMEOUT);
			FrmSetControlValue(form, FrmGetObjectIndex(form, pressureButtonId), 0);
			return true;
			
		case mvicButtonId:
			FrmSetControlValue(form, FrmGetObjectIndex(form, mvicButtonId), 1);
			testSendCommand(ECU_MVIC_MOTOR, ECU_CMD_TIMEOUT);
			FrmSetControlValue(form, FrmGetObjectIndex(form, mvicButtonId), 0);
			return true;
			
		case purgeButtonId:
			FrmSetControlValue(form, FrmGetObjectIndex(form, purgeButtonId), 1);
			testSendCommand(ECU_PURGE_SOL, ECU_CMD_TIMEOUT);
			FrmSetControlValue(form, FrmGetObjectIndex(form, purgeButtonId), 0);
			return true;
			
		case egrButtonId:
			FrmSetControlValue(form, FrmGetObjectIndex(form, egrButtonId), 1);
			testSendCommand(ECU_EGR_SOL, ECU_CMD_TIMEOUT);
			FrmSetControlValue(form, FrmGetObjectIndex(form, egrButtonId), 0);
			return true;
			
		case boostButtonId:
			FrmSetControlValue(form, FrmGetObjectIndex(form, boostButtonId), 1);
			testSendCommand(ECU_BOOST_SOL, ECU_CMD_TIMEOUT);
			FrmSetControlValue(form, FrmGetObjectIndex(form, boostButtonId), 0);
			return true;

		case inj1ButtonId:
		case inj2ButtonId:
		case inj3ButtonId:
		case inj4ButtonId:
		case inj5ButtonId:
		case inj6ButtonId:
			FrmSetControlValue(form, FrmGetObjectIndex(form, e->data.ctlSelect.controlID), 1);
			testSendCommand(ECU_INJ_1 + inj1ButtonId - e->data.ctlSelect.controlID, ECU_CMD_TIMEOUT);
			FrmSetControlValue(form, FrmGetObjectIndex(form, e->data.ctlSelect.controlID), 0);
			return true;
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
			
		case customMenuId:
			doCustomDialog();
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

