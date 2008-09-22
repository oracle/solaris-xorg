###############################################################################
#
# xlock 1.x Makefile
#
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
# @(#)Makefile.plain	35.1 94/09/01 XLOCK
# Makefile for xlock. 

CFLAGS=${DEFINES} -DSYSV -DUSE_PAM
C_SRCS=xlock.c hsbramp.c usleep.c resource.c \
	hopalong.c qix.c life.c image.c blank.c swarm.c rotor.c pyro.c flame.c
HDRS=xlock.h
IMAGES=lifeicon.bit sunlogo.bit
SRCS=${C_SRCS} ${HDRS} ${IMAGES} xlock.man XLock.ad
OBJS=${C_SRCS:.c=.o}
LIBDIR=/usr/X11/lib

xlock: ${OBJS}
	cc -o $@ ${OBJS} $(CFLAGS) $(LDFLAGS) -lX11 -lm -lpam -lbsm -lsocket -lnsl

resource.o:=DEFINES=-DDEF_FILESEARCHPATH=\"$(LIBDIR)/%T/%N%S\"
pyro.o:=DEFINES=-DSTARSIZE=2

install:
	$(INSTALL) -m 4755 xlock $(DESTDIR)/usr/X11/bin/xlock
	$(INSTALL) -m 644 xlock.1 \
		$(DESTDIR)/usr/X11/share/man/man1/xlock.1
	$(INSTALL) -m 644 XLock.ad $(DESTDIR)/usr/X11/lib/X11/app-defaults/XLock
