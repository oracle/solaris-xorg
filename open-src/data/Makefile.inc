# -*- Makefile -*- rules common to most X.Org data modules
#
# Copyright (c) 2006, 2008, Oracle and/or its affiliates. All rights reserved.
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
TOP=$(PWD)/../..

MODTYPE=data

### Include common definitions
include $(TOP)/common/Makefile.init

# Most data modules have no man pages
MODTYPE_SUNTOUCH_MAN_FLAGS= 

# No common dependencies
MODTYPE_CONFIGURE_DEPS=

# Most data modules have nothing to build, for those that do, use lib flags
MODTYPE_CFLAGS=$(LIB_CFLAGS)
MODTYPE_LDFLAGS=$(LIB_LDFLAGS)
MODTYPE_LD_OPTIONS=

### Include common rulesets
include $(TOP)/common/Makefile.inc

