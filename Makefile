#  Copyright (c) 2003, Dmitry Yurtaev <dm1try@umail.ru>
#
#  This is free software; you can redistribute it and/or modify it under the
#  terms of the GNU General Public License as published by the Free Software
#  Foundation; either version 2, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#  for more details.
#

CRID		= MMCd
POSTOBJS	= mmcd mmcd.ro
CFLAGS		= -DCRID="'$(CRID)'" -g -O2 -Wall
CC		= m68k-palmos-gcc
PILRCFLAGS	= -q -allowEditID
AVOIDS		= -D__CHARSHIFTJIS_H__ -D_HOSTCONTROL_H_ -D__SONYCHARS_H__ -D__SERIALMGROLD_H_
FONTS           = font/font.txt
BITMAPS		= icons/mmcd1bpp.bmp icons/mmcd4bpp.bmp icons/mmcd8bpp.bmp icons/mmcd8bpp-h.bmp \
		  icons/smmcd1bpp.bmp icons/smmcd4bpp.bmp icons/smmcd8bpp.bmp icons/smmcd8bpp-h.bmp \
		  icons/slider-back.bmp icons/slider-thumb.bmp
OBJS		= mmcd.o test.o debug.o graph.o panel.o format.o
INSTALLCODE	= use EmRPC; use HostControl; use EmFunctions; \
		  OpenConnection(6420, "localhost"); \
		  EvtEnqueuePenPoint({x => 248, y => 86}); \
		  EvtEnqueuePenPoint({x => -1, y => -1}); \
		  HostImportFile("$(PRC)", 0) && printf("failed!\n"); \
		  EvtEnqueuePenPoint({x => 101, y => 251}); \
		  EvtEnqueuePenPoint({x => -1, y => -1}); \
		  EvtEnqueuePenPoint({x => 101, y => 251}); \
		  EvtEnqueuePenPoint({x => -1, y => -1}); \
		  CloseConnection();


all: mmcd.prc

mmcd.prc: $(POSTOBJS)
	build-prc -n MMCd -c $(CRID) $(POSTOBJS)


mmcd: $(OBJS)
	m68k-palmos-gcc -g -o mmcd $(OBJS)

mmcd.o: mmcd.c mmcd.h format.h graph.h panel.h format.h
test.o: test.c mmcd.h
debug.o: debug.c mmcd.h panel.h
graph.o: graph.c mmcd.h graph.h format.h panel.h
panel.o: panel.c panel.h format.h
format.o: format.c format.h

mmcd.ro: mmcd.rcp $(FONTS) resource.h $(BITMAPS)

.SUFFIXES: .ro .rcp

.rcp.ro:
	pilrc >/dev/null $(PILRCFLAGS) -ro -o $@ $<

clean:
	-rm -f *.ro *.o mmcd *.prc

install: mmcd.prc
	@echo installing...
	@perl -e '$(INSTALLCODE)'
