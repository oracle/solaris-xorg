# -*- Makefile -*- rules common to most X.Org doc modules
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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

PWD:sh=pwd
TOP=$(PWD)/../../..

MODTYPE=doc

# Hierarchical parent for pkg names in this area
MODTYPE_PKGHIER=x11/documentation
MODTYPE_PKGHIER_SET=yes

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

# No common man flags to add
MODTYPE_SUNTOUCH_MAN_FLAGS= 

# No common dependencies
MODTYPE_CONFIGURE_DEPS=

# Use prog flags for building doc tools
MODTYPE_CFLAGS=$(PROG_CFLAGS)
MODTYPE_CPPFLAGS=$(PROG_CPPFLAGS)
MODTYPE_LDFLAGS=$(PROG_LDFLAGS)
MODTYPE_LD_OPTIONS=

DOCTOOLS_PATH = \
    PATH="$(PROTODIR)$(X11_DIR)/bin:$(DOCBOOKUTIL_BINDIR):$$PATH"

# Additional environment variable options to GNU autoconf configure script
MODTYPE_CONFIG_ENV = $(DOCTOOLS_PATH)

# Additional environment variable options needed when building
MODTYPE_BUILD_ENV = $(DOCTOOLS_PATH) \
	LD_LIBRARY_PATH="$(PROTODIR)$(X11_SERVERLIBS_DIR)"

### Include common rulesets
include $(TOP)/open-src/common/Makefile.inc



