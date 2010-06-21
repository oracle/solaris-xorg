# -*- Makefile -*- rules common to most X.Org application modules
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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

MODTYPE=app

### Include common definitions
include $(TOP)/common/Makefile.init

# Changes to apply to man pages
MODTYPE_SUNTOUCH_MAN_FLAGS= -p $(MODULE_PREFIX)/bin/

# Flags to build & link applications with
MODTYPE_CFLAGS=$(PROG_CFLAGS)
MODTYPE_CXXFLAGS=$(PROG_CXXFLAGS)
MODTYPE_LDFLAGS=$(PROG_LDFLAGS)

# Set comments in ELF binaries to help identify versions/builds
MODTYPE_ADD_BUILD_TARGETS=set-elf-comments

### Include common rulesets
include $(TOP)/common/Makefile.inc

