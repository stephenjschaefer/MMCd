/*
 *  $Id: scroller.c,v 1.1.1.1 2003/01/28 16:01:29 dmitry Exp $
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
#include "scroller.h"

void ScrCreateScroller(ScrollerType *bar, UInt16 id,
		Coord x, Coord y, Coord width, Coord height)
{
	Err err;
	static UInt16 leftArrowBits[7]  = { 0x0800, 0x1800, 0x3800, 0x7800, 0x3800, 0x1800, 0x0800 };
	static UInt16 rightArrowBits[7] = { 0x4000, 0x6000, 0x7000, 0x7800, 0x7000, 0x6000, 0x4000 };

/*  cbhack Need more memory

	ErrFatalDisplayIf(bar == NULL, "ScrCreateScroller: bar == NULL");
	ErrFatalDisplayIf(width < 13, "ScrCreateScroller: width < 13");
	ErrFatalDisplayIf(height != 7, "ScrCreateScroller: height != 7");
*/

	bar->bounds.topLeft.x = x;
	bar->bounds.topLeft.y = y;
	bar->bounds.extent.x = width;
	bar->bounds.extent.y = height;
	bar->attr.usable = 1;
	bar->attr.visible = 1;
	bar->attr.hilighted = 0;
	bar->attr.shown = 0;
	bar->value = 0;
	bar->minValue = 0;
	bar->maxValue = 0;
	bar->pageSize = 1;

	bar->_leftArrow = BmpCreate(6, 7, 1, NULL, &err);
	if(err == errNone)
		MemMove(BmpGetBits(bar->_leftArrow), leftArrowBits, sizeof(leftArrowBits));
	else
		bar->_leftArrow = NULL;

	bar->_rightArrow = BmpCreate(6, 7, 1, NULL, &err);
	if(err == errNone)
		MemMove(BmpGetBits(bar->_rightArrow), rightArrowBits, sizeof(rightArrowBits));
	else
		bar->_rightArrow = NULL;
}

void ScrSetVisible(ScrollerType *bar, Boolean visible)
{
	ErrFatalDisplayIf(bar == NULL, "ScrCreateScroller: bar == NULL");

	if(visible && !bar->attr.visible) {
		bar->attr.visible = 1;
		ScrDrawScroller(bar);
	} else if(!visible && bar->attr.visible) {
		bar->attr.visible = 0;
		bar->attr.shown = 0;
		WinEraseRectangle(&bar->bounds, 0);
	}
}

void ScrDestroyScroller(ScrollerType *bar)
{
	ErrFatalDisplayIf(bar == NULL, "ScrDestroyScroller: bar == NULL");

	if(bar->_leftArrow) {
		BmpDelete(bar->_leftArrow);
		bar->_leftArrow = NULL;
	}
	if(bar->_rightArrow) {
		BmpDelete(bar->_rightArrow);
		bar->_rightArrow = NULL;
	}
	bar->attr.usable = 0;
}

void ScrGetScroller(const ScrollerType *bar,
	Int32 *value, Int32 *minValue, Int32 *maxValue, Int32 *pageSize)
{
	ErrFatalDisplayIf(bar == NULL, "ScrGetScroller: bar == NULL");

	if(value) *value = bar->value;
	if(minValue) *minValue = bar->minValue;
	if(maxValue) *maxValue = bar->maxValue;
	if(pageSize) *pageSize = bar->pageSize;
}

void ScrSetScroller(ScrollerType *bar, Int32 value, 
	Int32 minValue, Int32 maxValue, Int32 pageSize)
{

/* cbhack need more memory
	ErrFatalDisplayIf(bar == NULL, "ScrSetScroller: bar == NULL");
	ErrFatalDisplayIf(maxValue < minValue, "ScrSetScroller: maxValue < minValue");
	ErrFatalDisplayIf(value < minValue || value > maxValue, "ScrSetScroller: value < minValue || value > maxValue");
	ErrFatalDisplayIf(pageSize < 1, "ScrSetScroller: pageSize < 1");
*/

	bar->value = value;
	bar->minValue = minValue;
	bar->maxValue = maxValue;
	bar->pageSize = pageSize;

	if(bar->attr.usable && bar->attr.visible) ScrDrawScroller(bar);
}

void _ScrCalculateCarBounds(const ScrollerType *bar, RectangleType *carBounds)
{
	RectangleType stripeBounds = { 
		{ bar->bounds.topLeft.x + 6, bar->bounds.topLeft.y + 2 },
		{ bar->bounds.extent.x - 12, bar->bounds.extent.y - 4 },
	};

	if(bar->maxValue > bar->minValue) {
		Int32 tmp = bar->pageSize * stripeBounds.extent.x
			/ (bar->maxValue - bar->minValue + bar->pageSize);
		Int32 carSize = tmp > 7 ? tmp : 7; // don't let it be less than 7 px
		Int32 carPos = (bar->value - bar->minValue)
			* (stripeBounds.extent.x - carSize)
			/ (bar->maxValue - bar->minValue);
		carBounds->topLeft.x = stripeBounds.topLeft.x + carPos;
		carBounds->topLeft.y = stripeBounds.topLeft.y;
		carBounds->extent.x = carSize;
		carBounds->extent.y = stripeBounds.extent.y;
	} else {
		*carBounds = stripeBounds;
	}
}

void ScrDrawScroller(ScrollerType *bar)
{
	RectangleType stripeBounds = { 
		{ bar->bounds.topLeft.x + 6, bar->bounds.topLeft.y + 2 },
		{ bar->bounds.extent.x - 12, bar->bounds.extent.y - 4 },
	}, carBounds;

	if(!bar->attr.usable || !bar->attr.visible) return;

	_ScrCalculateCarBounds(bar, &carBounds);

	WinPushDrawState();

	WinSetForeColor(UIColorGetTableEntryIndex(UIObjectForeground));
	WinSetBackColor(UIColorGetTableEntryIndex(UIObjectFill));

	// background should be clear for scroller to look correctly
	if(!bar->attr.shown) WinEraseRectangle(&bar->bounds, 0);
	// was left arrow bitmap created in constructor?
	if(bar->_leftArrow) {
		// if we have a pen down on it, paint it reverse
		WinSetDrawMode(bar->attr.hilighted && bar->attr.activeRegion == scrLeftArrow ? winPaintInverse : winPaint);
		WinPaintBitmap(bar->_leftArrow, bar->bounds.topLeft.x, bar->bounds.topLeft.y);
	}
	// right arrow
	if(bar->_rightArrow) {
		WinSetDrawMode(bar->attr.hilighted && bar->attr.activeRegion == scrRightArrow ? winPaintInverse : winPaint);
		WinPaintBitmap(bar->_rightArrow, bar->bounds.topLeft.x + bar->bounds.extent.x - 6, bar->bounds.topLeft.y);
	}
	WinSetDrawMode(winPaint);
	WinSetPatternType(blackPattern);
	// car
	WinPaintRectangle(&carBounds, 0);
	WinSetPatternType(grayPattern);
	// paint stripe left half
	// if car is not at far left...
	if(carBounds.topLeft.x > stripeBounds.topLeft.x) {
		// car is at least 2 pixels away from a left end
		if(carBounds.topLeft.x > stripeBounds.topLeft.x + 1) {
			RectangleType leftStripe = { 
				stripeBounds.topLeft,
				{ carBounds.topLeft.x - stripeBounds.topLeft.x - 1, stripeBounds.extent.y }
			};
			WinPaintRectangle(&leftStripe, 0);
		}
		// erase a line at the left of a car
		WinEraseLine(carBounds.topLeft.x - 1, carBounds.topLeft.y, carBounds.topLeft.x - 1, carBounds.topLeft.y + carBounds.extent.y - 1);
	}
	// right half
	if(carBounds.topLeft.x + carBounds.extent.x < stripeBounds.topLeft.x + stripeBounds.extent.x) {
		if(carBounds.topLeft.x + carBounds.extent.x + 1 < stripeBounds.topLeft.x + stripeBounds.extent.x) {
			RectangleType rightStripe = { 
				{ carBounds.topLeft.x + carBounds.extent.x + 1, stripeBounds.topLeft.y },
				{ stripeBounds.topLeft.x + stripeBounds.extent.x - carBounds.topLeft.x - carBounds.extent.x - 1, stripeBounds.extent.y }
			};
			WinPaintRectangle(&rightStripe, 0);
		}
		WinEraseLine(carBounds.topLeft.x + carBounds.extent.x, carBounds.topLeft.y, carBounds.topLeft.x + carBounds.extent.x, carBounds.topLeft.y + carBounds.extent.y - 1);
	}
	WinPopDrawState();
	bar->attr.shown = 1;
}

Boolean	ScrHandleEvent(ScrollerType *bar, const EventType *event)
{
	ScrollerEventType *e = (ScrollerEventType *)event, newEvent;
	Int32 oldValue = bar->value;
	Int32 newValue = bar->value;
	RectangleType carBounds;
	Boolean repaint = false;

	ErrFatalDisplayIf(bar == NULL, "ScrHandleEvent: bar == NULL");

	if(!bar->attr.usable || !bar->attr.visible) return false;

	switch(e->eType) {

	case penDownEvent:
		if(RctPtInRectangle(e->screenX, e->screenY, &bar->bounds)) {
			EvtCopyEvent((EventType*)e, (EventType*)&newEvent);
			newEvent.eType = scrEnterEvent;
			newEvent.data.scrEnter.scrollerID = bar->id;
			newEvent.data.scrEnter.pScroller = bar;
			EvtAddEventToQueue((EventType*)&newEvent);
			return true;
		}
		break;

	case scrEnterEvent:
		bar->attr.hilighted = 1;

		if(e->screenX < bar->bounds.topLeft.x + 6) {
			bar->attr.activeRegion = scrLeftArrow;
			repaint = true;
		} else if(e->screenX >= bar->bounds.topLeft.x + bar->bounds.extent.x - 6) {
			bar->attr.activeRegion = scrRightArrow;
			repaint = true;
		} else {
			RectangleType carBounds;
			_ScrCalculateCarBounds(bar, &carBounds);

			if(e->screenX < carBounds.topLeft.x) {
				bar->attr.activeRegion = scrLeftPage;
			} else if(e->screenX >= carBounds.topLeft.x + carBounds.extent.x) {
				bar->attr.activeRegion = scrRightPage;
			} else {
				bar->attr.activeRegion = scrCar;
				bar->penPosInCar = e->screenX;
				bar->savePos = bar->value;
			}
		}
		bar->_ticks = TimGetTicks();
		goto processRegion;

	case penUpEvent:
		if(bar->attr.hilighted) {
			bar->attr.hilighted = 0;
			ScrDrawScroller(bar);
			EvtCopyEvent((EventType*)e, (EventType*)&newEvent);
			newEvent.eType = scrExitEvent;
			newEvent.data.scrExit.scrollerID = bar->id;
			newEvent.data.scrExit.pScroller = bar;
			newEvent.data.scrExit.value = bar->value;	// TODO
			newEvent.data.scrExit.newValue = bar->value;
			EvtAddEventToQueue((EventType*)&newEvent);
			return true;
		}
		break;

	case penMoveEvent:
		if(bar->attr.hilighted && bar->attr.activeRegion == scrCar && bar->maxValue > bar->minValue) {
			_ScrCalculateCarBounds(bar, &carBounds);
			newValue = bar->savePos + ((Int32)e->screenX - bar->penPosInCar) * (bar->maxValue - bar->minValue) / (bar->bounds.extent.x - 12 - carBounds.extent.x);
			if(newValue < bar->minValue) newValue = bar->minValue;
			if(newValue > bar->maxValue) newValue = bar->maxValue;
			if(newValue == oldValue) return true;
			goto sendRepeatEvent;
		}

		break;

	case scrRepeatEvent:
		break;

	default:
		if(bar->attr.hilighted && TimGetTicks() > bar->_ticks + SysTicksPerSecond() / 5) {
processRegion:
//			bar->_ticks = TimGetTicks();
			switch(bar->attr.activeRegion) {
			case scrLeftArrow:
				newValue -= 4;
				if(newValue < bar->minValue) newValue = bar->minValue;
				goto sendRepeatEvent;
			case scrRightArrow:
				newValue += 4;
				if(newValue > bar->maxValue) newValue = bar->maxValue;
				goto sendRepeatEvent;
			case scrLeftPage:
				_ScrCalculateCarBounds(bar, &carBounds);
				if(carBounds.topLeft.x + carBounds.extent.x / 2 > e->screenX) {
					newValue -= bar->pageSize;
					if(newValue < bar->minValue) newValue = bar->minValue;
					goto sendRepeatEvent;
				}
				break;
			case scrRightPage:
				_ScrCalculateCarBounds(bar, &carBounds);
				if(carBounds.topLeft.x + carBounds.extent.x / 2 < e->screenX) {
					newValue += bar->pageSize;
					if(newValue > bar->maxValue) newValue = bar->maxValue;
					goto sendRepeatEvent;
				}
				break;
			}
		}
		break;
	}

	return false;

sendRepeatEvent:
	if(newValue != oldValue || repaint) {
       		ScrSetScroller(bar, newValue, bar->minValue, bar->maxValue, bar->pageSize);
		EvtCopyEvent((EventType*)e, (EventType*)&newEvent);
		newEvent.eType = scrRepeatEvent;
		newEvent.data.scrRepeat.scrollerID = bar->id;
		newEvent.data.scrRepeat.pScroller = bar;
		newEvent.data.scrRepeat.value = oldValue;
		newEvent.data.scrRepeat.newValue = newValue;
		EvtAddUniqueEventToQueue((EventType*)&newEvent, bar->id, false);
	}

	return false;
}
