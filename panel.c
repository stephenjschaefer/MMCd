/*
 *  $Id: panel.c,v 1.8 2003/06/30 08:43:32 dmitry Exp $
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
#include <Font.h>
#include "mmcd.h"
#include "panel.h"
#include "format.h"

#define NEW_SENSORS	// Directive to include new sensor support tables

#define W1 31
#define W2 47

SensorType _pnlSensor[32];
SensorType _pnlSensorDefault[32] = {
//					E - exists, C - capture, G - graph
//		  addr   slug   format  E  C  G  Sn P Thr  clr  value	       descr

/*  0	---- */	{ 0xff, "",     f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },  // Null entry used to blank out sensor display
//	/*  0	---- */	{ 0xEF, "VSPD",     f_DEC,  1, 0, 0, 0, 0, 0, 0, "?\0      ", "Vehicle Speed",	 		 },

/*  1	---X */	{ 0x00, "FLG0", f_FLG0, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Flags 0", 		 }, // Confusing FLAG0 / FLAG2 

/*  2	---X */	{ 0x02, "FLG2", f_FLG2, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Flags 2", 		 },

/*  3	X--- */	{ 0x06, "TIMA", f_TIMA, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Timing advance",	 },
                                                                            
/*  4>	-X-- */	{ 0x07, "COOL", f_COOL, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Coolant temp",	 },

/*  5	--X- */	{ 0x0c, "FTRL", f_FTxx, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Fuel trim low",	 }, // 0..200%

/*  6	--X- */	{ 0x0d, "FTRM", f_FTxx, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Fuel trim middle",	 }, //

/*  7	--X- */	{ 0x0e, "FTRH", f_FTxx, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Fuel trim high",	 }, //
                                                                            
/*  8	--X- */	{ 0x0f, "FTO2", f_FTxx, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "O2 feedback trim",	 }, //

/*  9	-X-- */	{ 0x12, "MAP", f_MAP, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "MAP - Boost",		 },

/* 10>	X--- */	{ 0x13, "O2-R", f_OXYG, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "O2 sensor",	 },

/* 11	-X-- */	{ 0x14, "BATT", f_BATT, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Battery",		 },
                                                                            
/* 12	-X-- */	{ 0x15, "BARO", f_BARO, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Barometer",	 },

/* 13 -X-- */	{ 0x16, "ISC",  f_DEC, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "ISC position",	 },

/* 14>	X--- */	{ 0x17, "TPS",  f_THRL, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Throttle position", },

/* 15	--X- */	{ 0x1a, "MAFS", f_AIRF, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Mass air flow",	 },
                                                               
/* 16	--X- */	{ 0x1d, "ACLE", f_THRL, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Accel enrichment",	 }, // 0..100%

/* 17>	X--- */	{ 0x21, "RPM",  f_ERPM, 1, 0, 0, 0, 0, 0, 119, "?\0      ", "Engine speed",	 },

/* 18	X--- */	{ 0x26, "KNCK", f_DEC,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Knock sum",	 },

/* 19>	X--- */	{ 0x29, "INJP", f_INJP, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Inj pulse width",	 },
                                                                            
/* 20	X--- */	{ 0x00, "INJD", f_FTxx, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Inj duty cycle",	 },

/* 21>	-X-- */	{ 0x3a, "AIRT", f_AIRT, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "Air temp",		 },
                                                               
/* 22>	X--- */	{ 0x3E, "O2-F", f_OXYG, 1, 0, 0, 0, 0, 0,   0, "?\0      ", "O2 sensor (rear)",	 },

// /* 23	---- */	{ 0xEE, "MPG",     f_DEC,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Fuel consumption",	 		 },
/* 23	---- */	{ 0xEF, "VSPD",     f_DEC,  1, 0, 0, 0, 0, 0, 0, "?\0      ", "Vehicle Speed",	 		 },

#ifdef 	NEW_SENSORS
                                                                      
/* 24	---- */	{ 0xE0, "MAP",   f_MAP,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Boost pressure",	 		},
/* 25	---- */	{ 0xE1, "FUEL",   f_PRES,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Fuel pressure",	 		},
/* 26	---- */	{ 0xE2, "OILP",   f_PRES,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Oil pressure",	 		},
/* 27	---- */	{ 0xE3, "WBO2",   f_DEC,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "WideBand O2",	 		},
/* 28	---- */	{ 0xE4, "EGT",   f_DEC,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Exhaust temperature",	 	},
/* 29	---- */	{ 0xE5, "PLEN",   f_DEC,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Plenum temperature",	 	},
/* 30	---- */	{ 0xE6, "OILT",   f_DEC,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Oil temperature",	 	},
/* 31	---- */	{ 0xE7, "PUMP",   f_BATT,  1, 0, 0, 0, 0, 0,   0, "?\0      ", "Fuel pump voltage",	 	},
#else

/* 24	---- */	{ 0x00, "00",   f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },
/* 25	---- */	{ 0x00, "00",   f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },
/* 26	---- */	{ 0x00, "00",   f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },
/* 27	---- */	{ 0x00, "00",   f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },
/* 28	---- */	{ 0x00, "00",   f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },
/* 29	---- */	{ 0x00, "00",   f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },
/* 30	---- */	{ 0x00, "00",   f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },
/* 31	---- */	{ 0x00, "00",   f_DEC,  0, 0, 0, 0, 0, 0,   0, "?\0      ", "",	 		 },
#endif

};

extern Boolean colorMode;
extern UInt8 graphColor[32];


void _PnlDrawButton(PanelType *panel, UInt16 row, UInt16 column, UInt16 field, Boolean blit)
{
	RectangleType rect = { { column * 81 + field * (W1 + 1), row * 10 }, { field ? W2 : W1, 9 } };
	RectangleType fullRect = { { 0, 0 }, panel->bounds.extent };
	WinHandle oldWin;
	FontID oldFontId;
	UInt16 labelWidth, labelLen;
	UInt16 sensorIndex = panel->sensorAt[row * 2 + column];

	// paint to offscreen
	oldWin = WinSetDrawWindow(panel->_offscreen);
	WinPushDrawState();

	// if sensor doesn't exist just erase its button
	if(sensorIndex > 31 || !panel->sensor[sensorIndex].exists) {
		WinSetBackColor(UIColorGetTableEntryIndex(UIObjectFill));
		WinEraseRectangle(&rect, 0);
	} else {
		Boolean active;
		const Char *label;

		if(field == 0) {	//SW
			active = panel->sensor[sensorIndex].capture;
			label = panel->sensor[sensorIndex].slug;
		} else {
			if(panel->_mode == PANEL_MULTISELECT) {
				active = panel->sensor[sensorIndex].graph;
			} else {
				active = (sensorIndex == panel->selection);
			}
			label = panel->sensor[sensorIndex].value;
		}

		if(active) {
			WinSetBackColor(
				colorMode && field == 1 && panel->_mode == PANEL_MULTISELECT
				? graphColor[sensorIndex & 31] 
				: UIColorGetTableEntryIndex(UIObjectSelectedFill)
			);
			WinSetForeColor(UIColorGetTableEntryIndex(UIObjectSelectedForeground));
			WinSetTextColor(UIColorGetTableEntryIndex(UIObjectSelectedForeground));
		} else {
			WinSetBackColor(UIColorGetTableEntryIndex(UIObjectFill));
			WinSetForeColor(UIColorGetTableEntryIndex(UIObjectForeground));
			WinSetTextColor(UIColorGetTableEntryIndex(UIObjectForeground));
		}


		WinEraseRectangle(&rect, 0);

		if((labelLen = StrLen(label))) {
				
			labelWidth = FntCharsWidth(label, labelLen) - 2;

			if(field == 0) 
				oldFontId = FntSetFont(stdFont); // Standard font for label
			else
				oldFontId = FntSetFont(boldFont); // Bold font for data
					
			WinSetClip(&rect);
			WinDrawChars(label, labelLen,
				rect.topLeft.x + (rect.extent.x - labelWidth) / 2,
				rect.topLeft.y + ((Int16)rect.extent.y - FntCharHeight()) / 2
			);
			WinSetClip(&fullRect);
			FntSetFont(oldFontId);
		}
	}

	WinPopDrawState();
	WinSetDrawWindow(oldWin);

	if(blit && panel->visible)
		WinCopyRectangle(panel->_offscreen, oldWin, &rect, panel->bounds.topLeft.x + rect.topLeft.x, panel->bounds.topLeft.y + rect.topLeft.y, winPaint);
}

void _PnlDrawPanel(PanelType *panel)
{
	UInt16 row, column, field;
	RectangleType rect = { { 0, 0 }, panel->bounds.extent };
	

	for(row = 0; row < panel->rows; row++)
		for(column = 0; column < 2; column++)
			for(field = 0; field < 2; field++)
				_PnlDrawButton(panel, row, column, field, false);

	if(panel->visible)
		WinCopyRectangle(panel->_offscreen, WinGetActiveWindow(), &rect, panel->bounds.topLeft.x, panel->bounds.topLeft.y, winPaint);
}

void PnlUpdate(PanelType *panel, const GraphSample* sample)
{
	UInt16 i, sensorIndex, updated = 0;

	if(sample == NULL) {
		for(i = 0; i < panel->rows * 2; i++) {
			sensorIndex = panel->sensorAt[i];
			if(panel->_sample.dataPresent & 1L << sensorIndex) {
				panel->sensor[sensorIndex].format(panel->_sample.data[sensorIndex], panel->sensor[sensorIndex].value);
			} else {
				panel->sensor[sensorIndex].value[0] = '-';
				panel->sensor[sensorIndex].value[1] = 0;
			}
			updated |= 1L << i;
		}
	} else {
		for(i = 0; i < panel->rows * 2; i++) {
			sensorIndex = panel->sensorAt[i];
			if(	(panel->_sample.dataPresent & 1L << sensorIndex) != (sample->dataPresent & 1L << sensorIndex)
			     || (panel->_sample.data[sensorIndex]) != (sample->data[sensorIndex])
			) {
				if(sample->dataPresent & 1L << sensorIndex) {
					panel->sensor[sensorIndex].format(sample->data[sensorIndex], panel->sensor[sensorIndex].value);
				} else {
					panel->sensor[sensorIndex].value[0] = '-';
					panel->sensor[sensorIndex].value[1] = 0;
				}
				updated |= 1L << i;
			}
		}
		panel->_sample = *sample;

		if(updated) {
			for(i = 0; i < panel->rows * 2; i++) {
				if(updated & (1L << i))
					_PnlDrawButton(panel, i / 2, i & 1, 1, false);
			}

			if(panel->visible) {
				RectangleType r0 = { { W1 + 1, 0 }, { W2, panel->bounds.extent.y } };
				RectangleType r1 = { { panel->bounds.extent.x - W2, 0 }, { W2, panel->bounds.extent.y } };

				WinCopyRectangle(panel->_offscreen, WinGetActiveWindow(), &r0, r0.topLeft.x + panel->bounds.topLeft.x, r0.topLeft.y + panel->bounds.topLeft.y, winPaint);
				WinCopyRectangle(panel->_offscreen, WinGetActiveWindow(), &r1, r1.topLeft.x + panel->bounds.topLeft.x, r1.topLeft.y + panel->bounds.topLeft.y, winPaint);
			}
		}
	}
}

void PnlSetBank(PanelType *panel, UInt8 *bank)
{
	UInt16 i;

	for(i = 0; i < panel->rows * 2; i++)
		panel->sensorAt[i] = bank[i];

	PnlUpdate(panel, NULL);
	_PnlDrawPanel(panel);
}

void PnlCreatePanel(PanelType *panel, UInt16 id, SensorType *sensor, UInt8 *bank, UInt8 selection, Coord x, Coord y, Coord width, Coord height)
{
	Err err;
	UInt16 i;
	WinHandle oldWin;

	sensor = _pnlSensor;	// TODO

	panel->bounds.topLeft.x = x;
	panel->bounds.topLeft.y = y;
	panel->bounds.extent.x = width;
	panel->bounds.extent.y = height;

	panel->rows = height / 10;
	panel->visible = 1;

	panel->graph = 0;
	panel->capture = 0;

	panel->_mode = PANEL_MULTISELECT;
	panel->selection = selection;

	for(i = 0; i < 32; i++) {
		panel->sensorAt[i] = i;
		panel->graph |= (UInt32)sensor[i].graph << i;
		panel->capture |= (UInt32)sensor[i].capture << i;
	}
	panel->_sample.dataPresent = 0;

	panel->sensor = sensor;
	panel->_offscreen = WinCreateOffscreenWindow(width, height, nativeFormat, &err);

	oldWin = WinSetDrawWindow(panel->_offscreen);
	WinPushDrawState();
	WinSetBackColor(UIColorGetTableEntryIndex(UIObjectFill));
	WinEraseWindow();
	WinPopDrawState();
	WinSetDrawWindow(oldWin);

	PnlSetBank(panel, bank);
}

void PnlHidePanel(PanelType *panel)
{
	if(!panel->visible) return;

	panel->visible = 0;
	WinPushDrawState();
	WinSetBackColor(UIColorGetTableEntryIndex(UIObjectFill));
	WinEraseRectangle(&panel->bounds, 0);
	WinPopDrawState();
}

void PnlShowPanel(PanelType *panel)
{
	if(panel->visible) return; // If already visible do nothing.

	panel->visible = 1;
	PnlUpdate(panel, NULL);
	_PnlDrawPanel(panel);
}

void PnlDestroyPanel(PanelType *panel)
{
	PnlHidePanel(panel);
	WinDeleteWindow(panel->_offscreen, false);
}

Boolean	PnlHandleEvent(PanelType *panel, const EventType *e)
{
	PanelEventType newEvent;

	switch(e->eType) {

	case penDownEvent:
		if(panel->visible && RctPtInRectangle(e->screenX, e->screenY, &panel->bounds)) {
			UInt16 x, y, row, column, field, index, sensorIndex;

			x = e->screenX - panel->bounds.topLeft.x;
			y = e->screenY - panel->bounds.topLeft.y;

			// is it an intercell space?
			if(    (y % 10) == 9
			     || x == 79 || x == 80
			     || x == 29 || x == 109) return true;

			row = y / 10;
			column = x / 81;
			field = (x % 81) > 29;
			index = row * 2 + column;

			sensorIndex = panel->sensorAt[row * 2 + column];

			if(panel->sensor[sensorIndex].exists) {
				UInt16 i;
				UInt8 oldSelection = panel->selection;

				if(field == 1) {	//SW
					if(panel->_mode == PANEL_MULTISELECT) {
						panel->sensor[sensorIndex].graph ^= 1;
						panel->graph ^= 1L << sensorIndex;
					} else {
						panel->selection = sensorIndex;
					}
				} else if(panel->sensor[sensorIndex].addr != 0xff) {
					panel->sensor[sensorIndex].capture ^= 1;
					panel->capture ^= 1L << sensorIndex;
				} else {
					return true;
				}

				for(i = 0; i < panel->rows * 2; i++)
					if(sensorIndex == panel->sensorAt[i] || (panel->_mode == PANEL_SINGLESELECT && oldSelection == panel->sensorAt[i]))
						_PnlDrawButton(panel, i / 2, i & 1, field, true);

				EvtCopyEvent((EventType*)e, (EventType*)&newEvent);
				newEvent.eType = pnlSelectEvent;
				newEvent.data.pnlSelect.panelID = panel->id;
				newEvent.data.pnlSelect.pPanel = panel;
				newEvent.data.pnlSelect.capture = panel->capture;
				newEvent.data.pnlSelect.graph = panel->graph;
				newEvent.data.pnlSelect.selection = panel->selection;
				EvtAddEventToQueue((EventType*)&newEvent);

				SndPlaySystemSound(sndClick);
			}

			return true;
		}
		break;

	default:
		break;
	}
	return false;
}

/*
 *  Set panel mode, either PANEL_SINGLESELECT or PANEL_MULTISELECT
 */

void PnlSetMode(PanelType *panel, panelModeEnum numeric)
{
	panel->_mode = numeric;
//	PnlUpdate(panel, NULL);
	_PnlDrawPanel(panel);
}


void PnlSetSelection(PanelType *panel, UInt8 selection)
{
	panel->selection = selection;
	if(panel->_mode == PANEL_SINGLESELECT) {
		// refresh if in single select mode
		_PnlDrawPanel(panel);
	}
}
