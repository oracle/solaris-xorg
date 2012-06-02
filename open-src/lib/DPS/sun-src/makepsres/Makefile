###############################################################################
#
# Copyright (c) 2009, 2011, Oracle and/or its affiliates. All rights reserved.
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

# Manual conversion of X11R6.9 xc/programs/makepsres/Imakefile

      DEFINES = -DXENVIRONMENT
    CPPFLAGS += $(DEFINES)

SRCS = makepsres.c

OBJS = makepsres.o

# Inspired by automake 1.11 silent rules - hides details by default
# Run make V=1 to show details
DEFAULT_V  = 0
V_CC       = $(V_CC_$(V))
V_CC_      = $(V_CC_$(DEFAULT_V))
V_CC_0     = @echo "  CC    " $@;

all: makepsres

makepsres: $(OBJS)
	$(V_CC)$(CC) -o $@ $(LDFLAGS) $(PROG_LDFLAGS) $(OBJS) $(EXTRA_LIBRARIES)

.c.o:
	$(V_CC)$(CC) $(CFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<

install: makepsres
	$(INSTALL) makepsres $(DESTDIR)$(bindir)/makepsres

clean:
	${RM} *.o makepsres

