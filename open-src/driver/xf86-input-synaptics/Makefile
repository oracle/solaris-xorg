###############################################################################
#
# xf86-input-synaptics Makefile
#
# Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-input-synaptics

# Version number (used in path names)
MODULE_VERSION=1.2.1

# Checksums for upstream tarball
TARBALL_MD5  = 29a6f9da5123149e4abc8ff83880ed5c
TARBALL_SHA1 = ffdad38384f3dcbf650d1db7e3d75ed46935a2c6

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - need to set install locations for HAL config file & probe util/script
MODULE_CONFIG_OPTS= \
	--with-hal-bin-dir=/usr/sbin \
	--with-hal-callouts-dir=/usr/lib/hal \
	--with-hal-fdi-dir=/etc/hal/fdi/policy/10osvendor \
	--bindir=$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

# Libraries/modules to link with
MODULE_LD_OPTIONS= -lm

# Extra install rules
MODULE_ADD_INSTALL_TARGETS = install_fdi

# Fix path for installing headers in proto area
MODULE_INSTALL_MAKEFLAGS = sdkdir='$(XORG_SDK_INCLUDES_DIR)'

include ../Makefile.inc

FDI_DIR=$(PROTODIR)/etc/hal/fdi/policy/10osvendor

install_fdi: $(SOURCE_TARGETS)
	mkdir -p $(FDI_DIR)
	$(INSTALL_SCRIPT) -m 0444 $(SOURCE_DIR)/fdi/11-x11-synaptics.fdi \
		$(FDI_DIR)/
