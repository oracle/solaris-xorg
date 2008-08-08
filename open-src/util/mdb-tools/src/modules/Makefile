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
#ident	"@(#)Makefile	1.3	08/08/08 SMI"

MODULES_plat=$(Xserver)_clients.so $(Xserver)_device_grabs.so
MODULES_32=$(MODULES_plat:%=$(PLATDIR_32)/%)
MODULES_64=$(MODULES_plat:%=$(PLATDIR_64)/%)
MODULES=$(MODULES_32) $(MODULES_64)

XC_TOP=$(TOP)/../xc

CFLAGS += -K pic -g
XSUN_CPPFLAGS = \
	-I $(PROTODIR)/usr/X11/include/X11 \
	-I /usr/include/X11 \
	-I $(PROTODIR)/usr/openwin/server/include \
	-I /usr/openwin/server/include \
	-I $(XC_TOP)/programs/Xserver/os
XORG_CPPFLAGS = \
	-I $(PROTODIR)/usr/X11/include/X11 \
	-I /usr/include/X11 \
        -I $(PROTODIR)/usr/X11/include/xorg \
        -I /usr/X11/include/xorg \
	-I $(TOP)/xserver/xorg/build_64/*/os
CPPFLAGS += $($(XSERVER)_CPPFLAGS) -D$(XSERVER)
LDFLAGS += -Wl,-z,text

OBJECTS = $(MODULES:%.so=%.o)
LINTFILES = $(MODULES:%.so=%.ln)

all: $(MODULES)

.KEEP_STATE:

.NO_PARALLEL:
.PARALLEL: $(OBJECTS) $(LINTFILES)

$(PLATDIR_32)/%.so: $(PLATDIR_32)/%.o
	$(CC) $(CFLAGS) $(ARCH32_FLAGS) -G -o $@ $< $(LDFLAGS) $(LIBS)

$(PLATDIR_64)/%.so: $(PLATDIR_64)/%.o
	$(CC) $(CFLAGS) $(ARCH64_FLAGS) -G -o $@ $< $(LDFLAGS) $(LIBS)

$(PLATDIR_32)/$(Xserver)_%.o: Xserver_%.c $(PLATDIR_32)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH32_FLAGS) -c $< -o $@

$(PLATDIR_64)/$(Xserver)_%.o: Xserver_%.c $(PLATDIR_64)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH64_FLAGS) -c $< -o $@

$(PLATDIR_32) $(PLATDIR_64):
	mkdir $@

clean:
	rm -f $(OBJECTS)

clean.lint:
	rm -f $(LINTFILES)

clobber: clean clean.lint
	rm -f $(MODULES)

%.ln: %.c
	$(LINT) $(LINTFLAGS) $(CPPFLAGS) -c $<

%.ln: ../common/%.c
	$(LINT) $(LINTFLAGS) $(CPPFLAGS) -c $<

lint: $(LINTFILES)
