# -*- Makefile -*- rules common to all X.Org drivers
#
# Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
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

MODTYPE=driver

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

# Determine from MODULE_NAME if this is a video or input driver
DRIVER_CLASS_V=$(MODULE_NAME:xf86-video-%=video)
DRIVER_CLASS=$(DRIVER_CLASS_V:xf86-input-%=input)
DRIVER_NAME=$(MODULE_NAME:xf86-$(DRIVER_CLASS)-%=%)

# Hierarchical parent for pkg names in this area
MODTYPE_PKGHIER=x11/server/xorg/driver
MODTYPE_PKGHIER_SET=yes

# Default pkg names for drivers - map xf86- to xorg- in names
MODULE_PKGTAIL_DEFAULT		= $(MODULE_PKGTAIL_SET:yes=$(POUND_SIGN))
$(MODULE_PKGTAIL_DEFAULT)	MODULE_PKGTAIL=$(MODULE_NAME:xf86-%=xorg-%)
$(MODULE_PKGTAIL_DEFAULT)	MODULE_PKGTAIL_SET=yes

# Package classification - must be a valid value recognized by pkg gui
MODULE_PKGCLASS_input		= Drivers/Other Peripherals
MODULE_PKGCLASS_video		= Drivers/Display
MODULE_PKGCLASS_DEFAULT		= $(MODULE_PKGCLASS_SET:yes=$(POUND_SIGN))
$(MODULE_PKGCLASS_DEFAULT)	MODULE_PKGCLASS=$(MODULE_PKGCLASS_$(DRIVER_CLASS))
$(MODULE_PKGCLASS_DEFAULT)	MODULE_PKGCLASS_SET=yes

# Additional command line options to GNU autoconf configure script
MODTYPE_CONFIG_OPTS = \
	--docdir='$(X11_DOC_DIR)' \
	--with-xorg-module-dir='$(X11_SERVERMODS_DIR)'

# Set comments in ELF binaries to help identify versions/builds
MODTYPE_ADD_BUILD_TARGETS=set-elf-comments

MODTYPE_CPPFLAGS= $(PROG_CPPFLAGS) \
	-I$(PROTODIR)/usr/include		\
	-I$(PROTODIR)/usr/include/pixman-1	\
	-I$(PROTODIR)$(XORG_SDK_INCLUDES_DIR)	\
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/drm	\
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/X11/dri

MODTYPE_CFLAGS=$(PROG_CFLAGS)

ASM_IL_DIR = $(PROTODIR)$(XORG_SDK_INCLUDES_DIR)
MODTYPE_CFLAGS_suncc_i386_32 = $(ASM_IL_DIR)/solaris-ia32.il
MODTYPE_CFLAGS_suncc_i386_64 = $(ASM_IL_DIR)/solaris-amd64.il
MODTYPE_CFLAGS_suncc_sparc_32 = $(ASM_IL_DIR)/solaris-sparcv8plus.il
MODTYPE_CFLAGS_suncc_sparc_64 = $(ASM_IL_DIR)/solaris-sparcv8plus.il
MODTYPE_CFLAGS += $(MODTYPE_CFLAGS_$(MODULE_COMPILER)_$(MACH)_$(BUILD_TYPE))

# Replaces @symbol_visibilty@ in xorg-server.pc, since that fails when
# some drivers are compiled with a different compiler than the server
MODTYPE_CFLAGS_suncc_common = -xldscope=hidden
MODTYPE_CFLAGS_gcc_common = # -fvisibility=hidden - doesn't work with gcc 3.4.3
MODTYPE_CFLAGS += $(MODTYPE_CFLAGS_$(MODULE_COMPILER)_common)

MODTYPE_LDFLAGS=$(LDFLAGS) -R/lib$(ARCHLIBSUBDIR)

XORG_EXTERNS_FLAG = -M $(TOP)/open-src/xserver/xorg/$(XORG_EXTERNS_FILE)
MODTYPE_LD_OPTIONS=-z defs $(XORG_EXTERNS_FLAG) -z ignore -lc

$(BUILD_DEBUG:yes=)			MODTYPE_DEBUG_LD_OPTIONS = -lpixman-1
$(BUILD_DEBUG:yes=$(POUND_SIGN))	MODTYPE_DEBUG_LD_OPTIONS =
MODTYPE_LD_OPTIONS += $(MODTYPE_DEBUG_LD_OPTIONS)

# Need to fix libtool interference with our linker flags
DELIBTOOLIZE=yes

# Mark packages as only compatible with the current Xorg minor release series,
# since Xorg tends to break ABI's in each minor release train (1.8, 1.9, etc.)
MODTYPE_ATTRDATA_COMMANDS = @$(START_CMD_ECHO) ; \
	print "depend fmri=pkg:/x11/server/xorg@$(MIN_XSERVER_VERS) type=optional" >> $@ ; \
	print "depend fmri=pkg:/x11/server/xorg@$(NEXT_XSERVER_VERS) type=exclude" >> $@

### Include common rules
include $(TOP)/open-src/common/Makefile.inc
