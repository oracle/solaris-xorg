# -*- Makefile -*- rules common to most X.Org protocol modules
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
TOP=$(PWD)/../../..

MODTYPE=proto

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

# Default package name for X11 protocol header packages
MODTYPE_PKGHIER=x11/header
MODTYPE_PKGHIER_SET=yes
MODULE_PKGTAIL_DEFAULT		= $(MODULE_PKGTAIL_SET:yes=$(POUND_SIGN))
$(MODULE_PKGTAIL_DEFAULT)	MODULE_PKGTAIL=x11-protocols
$(MODULE_PKGTAIL_DEFAULT)	MODULE_PKGTAIL_SET=yes


# Package classification - must be a valid value recognized by pkg gui
MODULE_PKGCLASS_DEFAULT		= $(MODULE_PKGCLASS_SET:yes=$(POUND_SIGN))
$(MODULE_PKGCLASS_DEFAULT)	MODULE_PKGCLASS=Development/X11
$(MODULE_PKGCLASS_DEFAULT)	MODULE_PKGCLASS_SET=yes

# Most proto modules have no man pages
MODTYPE_SUNTOUCH_MAN_FLAGS= 

# No common dependencies
MODTYPE_CONFIGURE_DEPS=

# Most proto modules have nothing to build, for those that do, use lib flags
MODTYPE_CFLAGS=$(LIB_CFLAGS)
MODTYPE_LDFLAGS=$(LIB_LDFLAGS)
MODTYPE_LD_OPTIONS=

# pkg-config files that need to have -I paths added to -uninstalled versions
FIX_PC_FILES_DEFAULT		= $(FIX_PC_FILES_SET:yes=$(POUND_SIGN))
$(FIX_PC_FILES_DEFAULT)		FIX_PC_FILES=*.pc.in

# proto pkg-config files are architecture independent, so install in share
MODTYPE_PKGCONFIG_DIR=$(PKGCONFIG_DIR_SHARE)
MODTYPE_PKGCONFIG_DIR_SET=yes

### Include common rulesets
include $(TOP)/open-src/common/Makefile.inc


