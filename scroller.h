/*
 *  $Id: scroller.h,v 1.1.1.1 2003/01/28 16:01:29 dmitry Exp $
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

#ifndef __SCROLLER_H__
#define __SCROLLER_H__

#include <PalmTypes.h>
#include <Rect.h>
#include <Event.h>

/*
 *  i tried hard to keep all structures, functions and behaviour close
 *  to corresponding Scrollbar equivalents
 */

typedef enum {
	scrLeftArrow,
	scrRightArrow,
	scrLeftPage,
	scrRightPage,
	scrCar
} ScrollerRegionType;


typedef struct ScrollerAttrTag {
	UInt16 usable		:1;		// Set if part of ui 
	UInt16 visible		:1;		// Set if drawn, used internally
	UInt16 hilighted	:1;		// Set if region is hilighted
	UInt16 shown		:1;		// Set if drawn and maxValue > minValue
	UInt16 activeRegion	:4;		// ScrollerRegionType
	UInt16 reserved		:8;		// Reserved for future use
} ScrollerAttrType;


typedef struct ScrollerType {
	RectangleType		bounds;
	UInt16			id;
	ScrollerAttrType	attr;
	Int32			value;
	Int32			minValue;
	Int32			maxValue;
	Int32			pageSize;
	UInt16			penPosInCar;
	Int32			savePos;

	// private
	BitmapType		*_leftArrow;
	BitmapType		*_rightArrow;
	UInt32			_ticks;

} ScrollerType;

typedef ScrollerType *ScrollerPtr;


enum scrollerEventsEnum {
	scrEnterEvent = firstUserEvent,
	scrRepeatEvent,
	scrExitEvent
};


typedef struct ScrollerEventType {
	eventsEnum	eType;
	Boolean		penDown;
	UInt8		tapCount;
	Int16		screenX;
	Int16		screenY;
	union {
		struct _GenericEventType generic;

		struct scrEnter {
			UInt16		scrollerID;
			ScrollerType	*pScroller;
		} scrEnter;

		struct scrExit {
			UInt16		scrollerID;
			ScrollerType	*pScroller;
			Int32		value;
			Int32		newValue;
		} scrExit;

		struct scrRepeat {
			UInt16		scrollerID;
			ScrollerType	*pScroller;
			Int32		value;
			Int32		newValue;
		} scrRepeat;
	} data;
} ScrollerEventType;


/*
 *
 */

void	ScrCreateScroller(ScrollerType *bar, UInt16 id,
		Coord x, Coord y, Coord width, Coord height);

void	ScrDestroyScroller(ScrollerType *bar);

void 	ScrGetScroller(const ScrollerType *bar, Int32 *value, 
		Int32 *minValue, Int32 *maxValue, Int32 *pageSize);

void	ScrSetScroller(ScrollerType *bar, Int32 value, 
		Int32 minValue, Int32 maxValue, Int32 pageSize);

void	ScrDrawScroller(ScrollerType *bar);

void	ScrSetVisible(ScrollerType *bar, Boolean visible);

Boolean	ScrHandleEvent(ScrollerType *bar, const EventType *event);

#endif //__SCROLLER_H__
