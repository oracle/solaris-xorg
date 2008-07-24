###############################################################################
#
# winsysck 1.x Makefile
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
# @(#)Makefile	1.1 08/04/11
#

# Package name used in tarballs
APP_NAME=winsysck

# Version number (used in path names)
APP_VERS=1.0.1

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

#AUTORECONF=true

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=*.man
#SUNTOUCH_MAN_FLAGS= \
# -a '{Availability, SUNWxwdem} {Interface Stability, Committed}'

include ../Makefile.inc

LIB_ADD_LD_OPTIONS += -lX11

SOURCE_DIR=$(BUILD_DIR)/winsysck-$(APP_VERS)

source_gen:: $(LNDIR)
	mkdir -p $(SOURCE_DIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../sun-src)

APP_BIN=$(SOURCE_DIR)

build_gen: $(APP_BIN)

$(APP_BIN): $(SOURCE_DIR)/Makefile $(LIB_ADD_BUILD_DEPS)
	(cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS) )

install_gen:: $(APP_BIN)
	(cd $(SOURCE_DIR); $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) install)

