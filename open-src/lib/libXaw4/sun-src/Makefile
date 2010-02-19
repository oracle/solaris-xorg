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

SRCS = 	Xaw3_1AsciiSink.c Xaw3_1AsciiSrc.c Xaw3_1AsciiText.c Xaw3_1Box.c\
	Xaw3_1Clock.c Xaw3_1Command.c Xaw3_1Dialog.c Xaw3_1Form.c \
	Xaw3_1Grip.c Xaw3_1Label.c Xaw3_1List.c Xaw3_1Logo.c \
	Xaw3_1Mailbox.c Xaw3_1MenuButton.c Xaw3_1Paned.c Xaw3_1Scrollbar.c \
	Xaw3_1Simple.c Xaw3_1SimpleMenu.c Xaw3_1Sme.c Xaw3_1SmeBSB.c \
	Xaw3_1SmeLine.c Xaw3_1StripChart.c Xaw3_1Template.c Xaw3_1Text.c \
	Xaw3_1TextAction.c Xaw3_1TextPop.c Xaw3_1TextSink.c Xaw3_1TextSrc.c \
	Xaw3_1TextTr.c Xaw3_1Toggle.c Xaw3_1Vendor.c Xaw3_1Viewport.c \
	Xaw3_1XawInit.c
OBJS = $(SRCS:.c=.o)

REQUIREDLIBS = -lX11 -lc -lXext -lXmu -lm -lXt

XLIB_CFLAGS:sh = pkg-config --cflags x11

libXaw.so.4: $(OBJS) 
	$(CC) -G -o $@ -h $@ $(CFLAGS) $(LDFLAGS) $(OBJS) $(REQUIREDLIBS)

.c.o:
	$(CC) -Kpic $(CFLAGS) $(XLIB_CFLAGS) $(CPPFLAGS) -c $<

install: libXaw.so.4
	$(INSTALL) libXaw.so.4 $(DESTDIR)$(libdir)/libXaw.so.4
