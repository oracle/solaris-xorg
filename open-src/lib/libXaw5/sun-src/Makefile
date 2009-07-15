# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, provided that the above
# copyright notice(s) and this permission notice appear in all copies of
# the Software and that both the above copyright notice(s) and this
# permission notice appear in supporting documentation.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
# OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
# INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# Except as contained in this notice, the name of a copyright holder
# shall not be used in advertising or otherwise to promote the sale, use
# or other dealings in this Software without prior written authorization
# of the copyright holder.
#
# @(#)Makefile	1.1 09/07/14

SRCS = 	AllWidgets.c AsciiSink.c AsciiText.c Box.c \
	Command.c Dialog.c Form.c Grip.c Label.c List.c MenuButton.c \
	MultiSink.c Paned.c Panner.c Porthole.c \
	Repeater.c Scrollbar.c Simple.c SimpleMenu.c Sme.c SmeBSB.c \
	SmeLine.c StripChart.c TextSrc.c \
	TextAction.c TextPop.c TextTr.c Toggle.c Tree.c Vendor.c \
	XawIm.c XawInit.c XawI18n.c sharedlib.c \
	Text.c TextSink.c Viewport.c AsciiSrc.c MultiSrc.c
OBJS = $(SRCS:.c=.o)

REQUIREDLIBS = -lX11 -lc -lXext -lXmu -lXt -lXaw

XLIB_CFLAGS:sh = pkg-config --cflags x11

libXaw.so.5: $(OBJS) 
	$(CC) -G -o $@ -h $@ $(CFLAGS) $(LDFLAGS) \
	$(OBJS) $(REQUIREDLIBS)

.c.o:
	$(CC) -Kpic $(CFLAGS) $(XLIB_CFLAGS) $(CPPFLAGS) -c $<

install: libXaw.so.5
	$(INSTALL) libXaw.so.5 $(DESTDIR)$(libdir)/libXaw.so.5
