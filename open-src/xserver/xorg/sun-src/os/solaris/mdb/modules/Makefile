# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
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

MODULES=Xserver.so
MDB_MODULE_DIR=/usr/lib/mdb/proc/$(ARCHLIBSUBDIR)

CFLAGS += -K pic -g
CPPFLAGS += -I ../../../../include -I ../../../.. -I ../../../../Xext
LDFLAGS += -Wl,-z,text

HEADERS = Xserver_mdb.h
SOURCES = Xserver_clients.c Xserver_device_grabs.c mdb_module.c
OBJECTS = $(SOURCES:%.c=%.o)
LINTFILES = $(SOURCES:%.so=%.ln)

all: $(MODULES)

.KEEP_STATE:

.NO_PARALLEL:
.PARALLEL: $(OBJECTS) $(LINTFILES)

Xserver.so: $(OBJECTS)
	$(CC) $(CFLAGS) -G -o Xserver.so $(OBJECTS) $(LDFLAGS) $(LIBS)

install:
	mkdir $(DESTDIR)$(MDB_MODULE_DIR)

clean:
	rm -f $(OBJECTS)

clean.lint:
	rm -f $(LINTFILES)

clobber: clean clean.lint
	rm -f $(MODULES)

%.ln: %.c
	$(LINT) $(LINTFLAGS) $(CPPFLAGS) -c $<

lint: $(LINTFILES)
