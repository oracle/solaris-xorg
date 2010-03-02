###############################################################################
#
# libowconfig Makefile
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
#

SHLIBLDFLAGS = -G -z defs $(LDFLAGS)
REQUIREDLIBS = -lc

HEADERS = \
	Sunowconfig.h

SRCS = \
	OWconfig.c

OBJS = \
	OWconfig.o

# Inspired by automake 1.11 silent rules - hides details by default
# Run make V=1 to show details
DEFAULT_V  = 0
V_CC       = $(V_CC_$(V))
V_CC_      = $(V_CC_$(DEFAULT_V))
V_CC_0     = @echo "  CC    " $@;

all: libowconfig.so.0

libowconfig.so.0: $(OBJS)
	$(V_CC)$(LD) $(CFLAGS) -o $@ $(SHLIBLDFLAGS) -h libowconfig.so.0 \
		$(OBJS) $(REQUIREDLIBS)

.c.o:
	$(V_CC)$(CC) -Kpic $(CFLAGS) $(CPPFLAGS) -c $<

INCDIR = $(X11_INCLUDES_DIR)/X11

install: libowconfig.so.0
	mkdir -p $(DESTDIR)$(libdir) $(DESTDIR)$(INCDIR)
	$(INSTALL) libowconfig.so.0 $(DESTDIR)$(libdir)/libowconfig.so.0
	$(INSTALL) Sunowconfig.h $(DESTDIR)$(INCDIR)/Sunowconfig.h
