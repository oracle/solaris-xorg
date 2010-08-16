###############################################################################
#
# xf86-input-vmmouse Makefile
#
# Copyright (c) 2007, 2010, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=xf86-input-vmmouse

# Version number (used in path names)
MODULE_VERSION=12.6.10

# Checksums for upstream tarball
TARBALL_MD5  = 49c6e77851e9f7bc5cb7d85f061992f8
TARBALL_SHA1 = 4c510d3f3ceaeb3bf826d440a406fe53fd85902c

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# vmmouse_client code won't build with Sun compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Need to use GNU Make to build due to Sun make bug 6474905
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Special handling for 64-bit installs, since GNU make doesn't
# pass through the *_drv_ladir override correctly from MODTYPE_MAKEFLAGS
INSTALL_TARGETS_64 = install-driver
INSTALL_TARGETS_SET_64 = yes
INSTALL_TARGETS = $(INSTALL_TARGETS_$(BUILD_TYPE))
INSTALL_TARGETS_SET = $(INSTALL_TARGETS_SET_$(BUILD_TYPE))

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - need to set install locations for HAL config file & probe util/script
MODULE_CONFIG_OPTS= \
	--with-hal-bin-dir=/usr/sbin \
	--with-hal-callouts-dir=/usr/lib/hal$(ARCHLIBSUBDIR) \
	--with-hal-fdi-dir=/etc/hal/fdi/policy/10osvendor$(ARCHLIBSUBDIR) \
	--bindir=$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

# Additional install steps to run
MODULE_ADD_INSTALL_TARGETS_64 = uninstall-hal-files
MODULE_ADD_INSTALL_TARGETS = $(MODULE_ADD_INSTALL_TARGETS_$(BUILD_TYPE))

include ../Makefile.inc

uninstall-hal-files: default_install
	(cd $(SOURCE_DIR)/tools ; \
	$(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS) \
	uninstall)
	(cd $(SOURCE_DIR)/fdi ; \
	$(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS) \
	uninstall)

install-driver: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)$(DRIVER_DIR)
	cp -pf $(SOURCE_DIR)/src/vmmouse_drv.so $(PROTODIR)$(DRIVER_DIR)/
