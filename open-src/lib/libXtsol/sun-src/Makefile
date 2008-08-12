# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use subject to license terms.
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
# @(#)Makefile	1.1	08/08/12

HEADERSPublic = Xtsol.h Xtsolproto.h
HEADERSPublic_dir = $(PREFIX)/include/X11/extensions

SRCS = Xtsol.c auditwrite.c
OBJS = $(SRCS:.c=.o)

REQUIREDLIBS = -lX11 -lbsm -ltsol -lc

XLIB_CFLAGS:sh = pkg-config --cflags x11

libXtsol.so.1: $(OBJS) mapfile-vers
	$(CC) -G -o $@ -h $@ $(CFLAGS) $(LDFLAGS) -M mapfile-vers \
	  $(OBJS) $(REQUIREDLIBS)

.c.o:
	$(CC) -Kpic $(XLIB_CFLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

install: libXtsol.so.1
	mkdir -p $(DESTDIR)$(HEADERSPublic_dir)
	for i in $(HEADERSPublic) ; do \
		$(INSTALL) $$i $(DESTDIR)$(HEADERSPublic_dir) ; \
	done
	mkdir -p $(DESTDIR)$(libdir)
	-rm -f $(DESTDIR)$(libdir)/libXtsol.so \
	       $(DESTDIR)$(libdir)/libXtsol.so.1
	$(INSTALL) libXtsol.so.1 $(DESTDIR)$(libdir)/libXtsol.so.1
	ln -s libXtsol.so.1 $(DESTDIR)$(libdir)/libXtsol.so
