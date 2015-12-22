/* 
 *  $Id: debug.c,v 1.1 2003/06/30 08:40:48 dmitry Exp $
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

Err openPort(UInt32 baudRate, Boolean enableFlowControl);

/*
 *  d e b u g
 */

void debugDrawByteMatrix(UInt8 bank)
{
	RectangleType r = { { 0, 15 }, { 160, 130 } };
	UInt16 x, y, labelWidth, i;
	Char str[16];
	FontID oldFontId = FntSetFont(stdFont);

	WinPushDrawState();
	for(y = 0; y < 8; y++) {
		for(x = 0; x < 8; x++) {
			UInt16 byte = bank * 64 + y * 8 + x;

			for(i = 0; i < SENSOR_COUNT; i++) {
				if(byte == _pnlSensor[i].addr) break;
			}

			FntSetFont(i < SENSOR_COUNT ? 1 : 0);

			RctSetRectangle(&r, 5 + x * 19, 19 + y * 10, 18, 9);
			WinEraseRectangle(&r, 0);
			StrIToH(str, byte);
			labelWidth = FntCharsWidth(str + 6, 2);
			WinDrawChars(str + 6, 2, 14 + x * 19 - labelWidth / 2, 18 + y * 10);
		}
	}

	WinSetBackColor(UIColorGetTableEntryIndex(UIObjectFill));
	WinSetForeColor(UIColorGetTableEntryIndex(UIObjectForeground));

	for(y = 0; y < 9; y++) {
		WinDrawLine(4, 18 + y * 10, 4 + 8 * 19, 18 + y * 10);
		WinDrawLine(4 + y * 19, 18, 4 + y * 19, 18 + 8 * 10);
	}
	WinPopDrawState();
	FntSetFont(oldFontId);
}

Boolean debugFormHandleEvent(EventType *e)
{
	Err err;
	RectangleType r;
	static UInt16 x, y;
	static Boolean penDown = false, hilited = false;
	static UInt8 page;
	FormType *form = FrmGetActiveForm();
	FieldType *field = FrmGetObjectPtr(form, FrmGetObjectIndex(form, debugFieldId));

	if(SysHandleEvent(e) || MenuHandleEvent(NULL, e, &err)) return true;

	if(portOpen) {
	ErrTry {
		Err err;
		UInt8 buf[32], *p = buf;
		Int32 bytesReceived = SrmReceive(portId, &buf, sizeof(buf), 1, &err);

//		if(err != errNone) ErrThrow(0);

		// got the answer?
		if(bytesReceived > 0) {
			Char str[16];

			while(bytesReceived-- > 0) {
				StrPrintF(str, "%x ", *p++);
				FldInsert(field, str + 2, 3);
			}
		}
	} ErrCatch(exception) {
		exception = 0;
		SrmClearErr(portId);
		SrmReceiveFlush(portId, 0);
	} ErrEndCatch
	}

	switch(e->eType) {

	case frmOpenEvent:
		FrmDrawForm(form);
		// hilite current bank
		FrmSetControlGroupSelection(form, bankGroupId, bank1ButtonId + page);
		debugDrawByteMatrix(page);
		openPort(serialSpeed, false);
		return true;

	case frmCloseEvent:
		closePort();
		return false;

	case keyDownEvent:
		if(e->data.keyDown.modifiers & autoRepeatKeyMask) break;

		// decrement current bank
		if(e->data.keyDown.chr == pageUpChr) {
//			SndPlaySystemSound(sndClick);
			page = (page - 1) & 3;
			FrmSetControlGroupSelection(form, bankGroupId, bank1ButtonId + page);
			debugDrawByteMatrix(page);
			return true;
		}
		// cincrement current bank
		if(e->data.keyDown.chr == pageDownChr) {
//			SndPlaySystemSound(sndClick);
			page = (page + 1) & 3;
			FrmSetControlGroupSelection(form, bankGroupId, bank1ButtonId + page);
			debugDrawByteMatrix(page);
			return true;
		}
		break;

	case penDownEvent:
		if(e->screenX >= 4 && e->screenX <= 4 + 8 * 19 &&
		   e->screenY >= 18 && e->screenY <= 18 + 80) {
			x = (e->screenX - 4) / 19;
			y = (e->screenY - 18) / 10;
			RctSetRectangle(&r, 5 + x * 19, 19 + y * 10, 18, 9);
			WinInvertRectangle(&r, 0);
			hilited = penDown = true;
			
			return true;
		}
		break;

	case penMoveEvent:
		if(penDown) {
			RctSetRectangle(&r, 5 + x * 19, 19 + y * 10, 18, 9);
			if(RctPtInRectangle(e->screenX, e->screenY, &r)) {
				if(!hilited) {
					WinInvertRectangle(&r, 0);
					hilited = true;
				}
			} else {
				if(hilited) {
					WinInvertRectangle(&r, 0);
					hilited = false;
				}
			}
			return true;
		}
		break;

	case penUpEvent:
		if(penDown) {
			if(hilited) {
				UInt8 byte = page * 64 + y * 8 + x;
				Err err;

				RctSetRectangle(&r, 5 + x * 19, 19 + y * 10, 18, 9);
				WinInvertRectangle(&r, 0);

				if(portOpen) {
					SndPlaySystemSound(sndClick);
					SrmSend(portId, &byte, 1, &err);
				}
			}
			penDown = false;
			return true;
		}
		break;
		
	case sclRepeatEvent:
		{
			Int32 linesToScroll = e->data.sclRepeat.newValue - e->data.sclRepeat.value;
			
			if(linesToScroll < 0)
				FldScrollField(field, -linesToScroll, winUp);
			else
				FldScrollField(field, linesToScroll, winDown);
      
			// if there are blank lines at the end and we scroll up, FldScrollField
			// makes the blank lines disappear. Therefore, we've got to update
			// the scrollbar
			if(FldGetNumberOfBlankLines(field) && linesToScroll >= 0)
				break;
		}
		/* FALL THRU */
			
	case fldChangedEvent:
		{
			UInt16 scrollPos, textHeight, fieldHeight, maxValue;
			ScrollBarType *bar = FrmGetObjectPtr(form, FrmGetObjectIndex(form, scrollBarId));
			
			FldGetScrollValues(field, &scrollPos, &textHeight, &fieldHeight);
			if(textHeight > fieldHeight)
				maxValue = textHeight - fieldHeight;
			else if(scrollPos)
				maxValue = scrollPos;
			else
				maxValue = 0;
				
			SclSetScrollBar(bar, scrollPos, 0, maxValue, fieldHeight - 1);
		}
		break;


	case ctlSelectEvent:
		switch(e->data.ctlSelect.controlID) {

		case doneButtonId:
			FrmGotoForm(mainFormId);
			return true;

		case bank1ButtonId:
		case bank2ButtonId:
		case bank3ButtonId:
		case bank4ButtonId:
			debugDrawByteMatrix(e->data.ctlSelect.controlID - bank1ButtonId);
			break;

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


