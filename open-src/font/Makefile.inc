# -*- Makefile -*- rules common to most X.Org font modules
#
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
#

### Makefile settings/rules common to all X.Org font modules

PWD:sh=pwd
TOP=$(PWD)/../..

MODTYPE=font

### Include common definitions
include $(TOP)/common/Makefile.init

# Most font modules have no man pages
MODTYPE_SUNTOUCH_MAN_FLAGS= 

# No common dependencies
MODTYPE_CONFIGURE_DEPS=

# Set default installation path for fonts
MODTYPE_CONFIG_OPTS = --with-fontrootdir="$(X11_FONT_DIR)"

# Disable compression of pcf fonts
MODTYPE_CONFIG_OPTS += --without-compression

# Most font modules have nothing to build, for those that do, use lib flags
MODTYPE_CFLAGS=$(LIB_CFLAGS)
MODTYPE_LDFLAGS=$(LIB_LDFLAGS)
MODTYPE_LD_OPTIONS=

# Paths to find mkfontscale, fc-cache & the libraries they need in build area
FONT_UTILS_PATH=$(PROTODIR)$(X11_DIR)/bin:$(PROTODIR)/usr/bin
FONT_LIBS_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR):$(PROTODIR)$(X11_DIR)/lib:$(PROTODIR)/usr/lib

# Additional environment variable options to GNU autoconf configure script
MODTYPE_CONFIG_ENV = PATH="$(FONT_UTILS_PATH):$$PATH" \
	LD_LIBRARY_PATH="$(FONT_LIBS_PATH)"

# Environment variable options needed when building
MODTYPE_BUILD_ENV = PATH="$(FONT_UTILS_PATH):$$PATH" \
	LD_LIBRARY_PATH="$(FONT_LIBS_PATH)"

### Include common rulesets
include $(TOP)/common/Makefile.inc

