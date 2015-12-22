/* 
 *  $Id: format.h,v 1.2 2003/03/28 17:28:58 dmitry Exp $
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

#include <PalmTypes.h>

/*

MMC Galant E3x, MMC Eclipse 1G, ...

*/

Int16 f_DEC(UInt8 d, Char *s);
Int16 f_HEX(UInt8 d, Char *s);
Int16 f_FLG0(UInt8 d, Char *s);
Int16 f_FLG2(UInt8 d, Char *s);
Int16 f_AIRT(UInt8 d, Char *s);
Int16 f_COOL(UInt8 d, Char *s);
Int16 f_EGRT(UInt8 d, Char *s);
Int16 f_MAP(UInt8 d, Char *s);
Int16 f_BATT(UInt8 d, Char *s);
Int16 f_ERPM(UInt8 d, Char *s);
Int16 f_INJP(UInt8 d, Char *s);
Int16 f_INJD(UInt8 d, Char *s);
Int16 f_BARO(UInt8 d, Char *s);
Int16 f_AIRF(UInt8 d, Char *s);
Int16 f_THRL(UInt8 d, Char *s);
Int16 f_FTxx(UInt8 d, Char *s);
Int16 f_OXYG(UInt8 d, Char *s);
Int16 f_TIMA(UInt8 d, Char *s);
Int16 f_PRES(UInt8 d, Char *s);  // CD -Added for fuel/oil pressure sensors
