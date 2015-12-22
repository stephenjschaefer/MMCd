/*
 *  $Id: panel.h,v 1.5 2003/06/16 13:17:10 dmitry Exp $
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

#ifndef __PANEL_H__
#define __PANEL_H__

#include <PalmTypes.h>
#include <Rect.h>
#include <Event.h>
#include "graph.h"

/*
 *
 */

typedef struct SensorType {
	UInt8 			addr;		// sensor id
	Char			slug[5];	// sensor name
	Int16 (*format)(UInt8, Char*);		// pointer to formatting function

	UInt8			exists   : 1;
	UInt8			capture  : 1;
	UInt8			graph    : 1;
	UInt8			sound	 : 4;
	UInt8			polarity : 1;
	UInt8			threshold;

	UInt8			color;

	Char value[8];
	const Char *desc;	// sensor name
	UInt8			trigger; // Added logtrigger toi struct - cb

} SensorType;

#define PREFS_UNIT_METRIC	0
#define PREFS_UNIT_ENGLISH	1
#define PREFS_UNIT_NUMERIC	2

typedef enum panelModeEnum {
	PANEL_MULTISELECT,
	PANEL_SINGLESELECT
} panelModeEnum;

typedef struct PanelType {
	RectangleType	bounds;
	UInt16		id;
	UInt16		rows;

	UInt32		capture;
	UInt32		graph;
	UInt8		selection;

	UInt8		sensorAt[32];
	SensorType	*sensor;

	WinHandle	_offscreen;
	GraphSample	_sample;
	panelModeEnum	_mode;

	UInt16		visible : 1;
} PanelType;

enum panelEventsEnum {
	pnlSelectEvent = firstUserEvent + 20
};


typedef struct PanelEventType {
	eventsEnum	eType;
	Boolean		penDown;
	UInt8		tapCount;
	Int16		screenX;
	Int16		screenY;
	union {
		struct _GenericEventType generic;

		struct pnlSelect {
			UInt16		panelID;
			PanelType	*pPanel;
			UInt32		capture;
			UInt32		graph;
			UInt8		selection;
		} pnlSelect;

	} data;
} PanelEventType;


/*
 *
 */

void	PnlCreatePanel(PanelType *panel, UInt16 id,
		SensorType *sensor, UInt8 *bank, UInt8 selection,
		Coord x, Coord y, Coord width, Coord height
	);

void	PnlDestroyPanel(PanelType *panel);

void	PnlHidePanel(PanelType *panel);

void	PnlShowPanel(PanelType *panel);

void	PnlUpdate(PanelType *panel, const GraphSample* sample);

void	PnlSetBank(PanelType *panel, UInt8 *bank);

Boolean	PnlHandleEvent(PanelType *panel, const EventType *event);

void	PnlSetMode(PanelType *panel, panelModeEnum mode);

void	PnlSetSelection(PanelType *panel, UInt8 selection);

extern SensorType _pnlSensor[32];
extern SensorType _pnlSensorDefault[32];

#define IDX_RPM		17
#define IDX_INJP	19
#define IDX_INJD	20
#define IDX_VSPD	0
#define IDX_MPG		23
#define IDX_COOL	4
#define IDX_AIRT	21

#endif //__PANEL_H__
