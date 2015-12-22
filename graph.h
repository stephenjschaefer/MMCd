/*
 *  $Id: graph.h,v 1.3 2003/06/30 08:25:17 dmitry Exp $
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

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <PalmTypes.h>
#include <Rect.h>
#include <Event.h>

/*
 *
 */
 

typedef enum {
	graphNone,
	graphLeft,
	graphRight
} GraphRegionType;


typedef struct GraphAttrTag {
	UInt16	usable		:1;
	UInt16	visible		:1;
	UInt16	hilighted	:1;
	UInt16	shown		:1;
	UInt16	activeRegion	:4;
	UInt16	needsData	:1;
	UInt16	cursorVisible	:1;
	UInt16	reserved	:7;
} GraphAttrType;


typedef struct GraphSample {
	UInt32	time;
	UInt32	dataPresent;
	UInt8	data[32];
} GraphSample;

// callback function graph calls when it needs new data
typedef Int32 (*GraphDataReader)(UInt16 id, Int32 streamOffset,
	GraphSample *pBuffer, Int32 sampleCount);


typedef struct GraphType {
	RectangleType		bounds;
	UInt16			id;
	GraphAttrType		attr;

	Int32			cursor;		// cursor position in stream
	Int32			position;	// stream offset
	Int32			length;		// stream length in samples
	UInt32			dataMask;
	UInt32			numericMask;
	UInt16			width;		// == bounds.extent.x (in 160x160)

	// private
	GraphSample		*_buffer;
	WinHandle		_offscreen;
	UInt8			*_offscreenBits;
	GraphDataReader		_dataReader;
	Boolean			_numericMode;
	UInt8			_numericSensorIdx;
	void (*_offscreenDrawFunction)(const struct GraphType*, UInt16, UInt16, UInt8);
} GraphType;

typedef GraphType *GraphPtr;


enum graphEventsEnum {
	grfChangeEvent = firstUserEvent + 10
};


typedef struct GraphEventType {
	eventsEnum	eType;
	Boolean		penDown;
	UInt8		tapCount;
	Int16		screenX;
	Int16		screenY;
	union {
		struct _GenericEventType generic;

		struct grfChange {
			UInt16		graphID;
			Int32		position;
			Int32		cursor;
			Int32		length;
			UInt16		positionChanged	:1;
			UInt16		cursorChanged	:1;
			UInt16		lengthChanged	:1;
		} grfChange;
	} data;
} GraphEventType;


/*
 *
 */

void	GrfCreateGraph(GraphType *graph, UInt16 id,
		Coord x, Coord y, Coord width, Coord height,
		GraphDataReader dataReader);

void	GrfDestroyGraph(GraphType *graph);

void 	GrfGetGraph(const GraphType *graph,
		Int32 *length, Int32 *position, Int32 *cursor, UInt32 *mask);

void	GrfSetGraph(GraphType *graph,
		Int32 length, Int32 position, Int32 cursor, UInt32 mask);

void	GrfSetGraphPosition(GraphType *graph, Int32 position);

void	GrfDrawGraph(const GraphType *graph, UInt16 from, UInt16 to);

void	GrfEraseGraph(GraphType *graph);

void	GrfSetMask(GraphType *graph, UInt32 mask);

Boolean	GrfHandleEvent(GraphType *graph, const EventType *event);

void	GrfAppendSample(GraphType *graph, GraphSample *newSample);

void	GrfShowCursor(GraphType *graph, Boolean visible);

void	GrfUpdateGraph(GraphType *graph);

void	GrfSetMode(GraphType *graph, Boolean numeric);

UInt8	GrfSetSensor(GraphType *graph, UInt8 sensor);

void	GrfSetNumericMask(GraphType *graph, UInt32 mask);

#endif //__GRAPH_H__
