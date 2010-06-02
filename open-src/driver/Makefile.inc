# -*- Makefile -*- rules common to all X.Org drivers
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

MODTYPE=driver

### Include common definitions
include $(TOP)/common/Makefile.init

# Determine from MODULE_NAME if this is a video or input driver
DRIVER_CLASS_V=$(MODULE_NAME:xf86-video-%=video)
DRIVER_CLASS=$(DRIVER_CLASS_V:xf86-input-%=input)
DRIVER_NAME=$(MODULE_NAME:xf86-$(DRIVER_CLASS)-%=%)

# Build 64-bit on SPARC, 32-bit & 64-bit on x86, to match Xorg server builds
BUILD_TYPES_SET=yes
BUILD_TYPES_sparc=64
BUILD_TYPES_i386=32 64
BUILD_TYPES=$(BUILD_TYPES_$(MACH))

# Additional command line options to GNU autoconf configure script
MODTYPE_CONFIG_OPTS = \
	--docdir='$(X11_DOC_DIR)' \
	--with-xorg-module-dir='$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)'

MODTYPE_BUILD_32_FLAGS=ARCHBINSUBDIR="$(SUBDIR32)"
MODTYPE_BUILD_64_FLAGS=ARCHBINSUBDIR="$(SUBDIR64)"

# Override module subdirs to be modules/xxx/amd64, not modules/amd64/xxx
DRIVER_DIR_input=$(X11_SERVERMODS_DIR)/input$(SERVERMOD_subdir)
DRIVER_DIR_video=$(X11_SERVERMODS_DIR)/drivers$(SERVERMOD_subdir)
DRIVER_DIR=$(DRIVER_DIR_$(DRIVER_CLASS))

MODTYPE_MAKEFLAGS= \
	inputdir='$(DRIVER_DIR_input)' \
	driversdir='$(DRIVER_DIR_video)' \
	$(DRIVER_NAME)_drv_ladir='$(DRIVER_DIR)'
MODTYPE_ADD_CONFIGURE_TARGETS = fix-moduledirs

# Set comments in ELF binaries to help identify versions/builds
MODTYPE_ADD_BUILD_TARGETS=set-elf-comments

MODTYPE_CPPFLAGS= \
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

MODTYPE_LDFLAGS_suncc=
MODTYPE_LDFLAGS_gcc=-R/usr/sfw/lib$(ARCHLIBSUBDIR)
MODTYPE_LDFLAGS=$(LDFLAGS) -Wl,-B,direct $(MODTYPE_LDFLAGS_$(MODULE_COMPILER))

MODTYPE_LD_OPTIONS=-z defs $(X11_SERVERLIBS_LDFLAGS) \
	-M $(TOP)/xserver/xorg/$(BUILD_DIR)/mapfile-Xorg-externs -lc

# Need to fix libtool interference with our linker flags
DELIBTOOLIZE=yes

### Include common rules
include $(TOP)/common/Makefile.inc
