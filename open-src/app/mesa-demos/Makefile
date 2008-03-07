###############################################################################
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.8	08/03/07
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Version number (used in path names)
MESA_VERS=6.5.2

# Source tarball
SOURCE_TARBALL_NAME=MesaDemos-$(MESA_VERS).tar.bz2

# Download site for source
SOURCE_URL=$(SF_DOWNLOADS_URL)/mesa3d/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	null-display.patch 		\
	Xorg-6.9-changes.patch 

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/Mesa-$(MESA_VERS)

# Binaries built in tree
GLXGEARS_BIN=$(SOURCE_DIR)/progs/xdemos/glxgears
GLXINFO_BIN=$(SOURCE_DIR)/progs/xdemos/glxinfo

# Where to install files
MESA_prefix=$(PROTODIR)/usr/X11

# What to build
BUILD_TARGETS_sparc=$(GLXGEARS_BIN)
BUILD_TARGETS_i386=$(GLXGEARS_BIN) $(GLXINFO_BIN)
BUILD_TARGETS= $(BUILD_TARGETS_$(MACH))

# What to install
INSTALL_TARGETS_sparc=install_glxgears install_man
INSTALL_TARGETS_i386=install_glxgears install_glxinfo install_man
INSTALL_TARGETS= $(INSTALL_TARGETS_$(MACH))

### Include common rulesets
include $(TOP)/common/Makefile.inc

### Rules specific to this directory:

# Merge in additional sources from Mesa library sun-src directory
source_gen:: $(LNDIR)
	mkdir -p $(SOURCE_DIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../../../lib/mesa/sun-src)
	(cd $(SOURCE_DIR)/configs && touch default)

# Only care about building X demos
DEMOS_DIR=$(SOURCE_DIR)/progs/xdemos

# Mesa config file names
MESA_CFG_32_sparc=solaris-sparcv8-cc
MESA_CFG_64_sparc=sunos5-v9
MESA_CFG_32_i386=solaris-x86-cc-dri
MESA_CFG_64_i386=solaris-x64-cc-dri

MESA_CFG_32= $(MESA_CFG_32_$(MACH))
MESA_CFG_64= $(MESA_CFG_64_$(MACH))

BUILD_32_FLAGS= MESA_CFG=$(MESA_CFG_32)
BUILD_64_FLAGS= MESA_CFG=$(MESA_CFG_64)

# Build Mesa demos
$(GLXGEARS_BIN): $(UNPACK_TARGET)
	(cd $(SOURCE_DIR)/configs && rm -f current && ln -s $(MESA_CFG) current)
	(cd $(DEMOS_DIR) && $(MAKE) $(MFLAGS) LDFLAGS="$(PROG_LDFLAGS)" glxgears )

$(GLXINFO_BIN): $(UNPACK_TARGET)
	(cd $(SOURCE_DIR)/configs && rm -f current && ln -s $(MESA_CFG) current)
	(cd $(DEMOS_DIR) && $(MAKE) $(MFLAGS) LDFLAGS="$(PROG_LDFLAGS)" glxinfo )

# For testing - we don't ship the rest of these
all-xdemos_gen: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR)/configs && rm -f current && ln -s $(MESA_CFG) current)
	(cd $(DEMOS_DIR) && $(MAKE) $(MFLAGS) LDFLAGS="$(PROG_LDFLAGS)")

all-xdemos:
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_32) ARCH_FLAGS="$(ARCH32_FLAGS)" $(BUILD_32_FLAGS) all-xdemos_gen

install_gen:: $(INSTALL_TARGETS)

install_glxgears: $(GLXGEARS_BIN)
	mkdir -p $(MESA_prefix)/bin
	cp -pf $(GLXGEARS_BIN) $(MESA_prefix)/bin

install_glxinfo: $(GLXINFO_BIN)
	mkdir -p $(MESA_prefix)/bin
	cp -pf $(GLXINFO_BIN) $(MESA_prefix)/bin

install_man: 
	mkdir -p $(MESA_prefix)/share/man/man1
	cp -pf glxgears.1 glxinfo.1 $(MESA_prefix)/share/man/man1


