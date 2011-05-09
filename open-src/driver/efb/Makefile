###############################################################################
#
# efb Makefile
#
# Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-ati

# Version number (used in path names)
MODULE_VERSION=6.8.0

# Checksums for upstream tarball
TARBALL_MD5  = 3c97c7925ebf4162eeb3463e23adc0e3
TARBALL_SHA1 = b6974237b6ae9b45b32fc37f31a3fee7358230d3

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	RAC-API-removal.patch,-p1 \
	no-modes.patch,-p1 \
	sparc-efb.patch,-p1

# Need to regenerate configure after patching Makefile.am in sparc-efb.patch
AUTORECONF=yes

# pkg(5) name we deliver the files in (override default)
MODULE_PKGTAIL=xorg-efb
MODULE_PKGTAIL_SET=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Use -KPIC instead of -Kpic because we have too many symbols on SPARC
DELIBTOOLIZE_FLAGS = -P

# Extra flags to pass to configure script
MODULE_CONFIG_OPTS=--disable-dri

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# Extra rules to run when unpacking upstream source
CLEAN_LINEFEED_TARGET = $(SOURCE_DIR)/.source.clean.done
MODULE_ADD_SOURCE_TARGETS = $(CLEAN_LINEFEED_TARGET)

XF86_VIDEO_ATI_SRC = $(PWD:efb=xf86-video-ati)

# Libraries/modules to link with
MODULE_LD_OPTIONS= \
        -L$(PROTODIR)$(X11_SERVERMODS_DIR) \
        -L$(PROTODIR)$(X11_SERVERMODS_DIR)/extensions \
        -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
        -L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR) \
        -R$(X11_SERVERMODS_DIR) \
        -R$(X11_SERVERMODS_DIR)/extensions \
        -R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
        -R$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR) \
        -lexa -lfb -lint10 -lvbe -lvgahw -lxaa -lshadow \
        -lpciaccess -lpixman-1 -ldrm -ldri -lglx -lm \
        -M $(XF86_VIDEO_ATI_SRC)/mapfile-bt829_drv-external \
        -M $(XF86_VIDEO_ATI_SRC)/mapfile-fi1236_drv-external \
        -M $(XF86_VIDEO_ATI_SRC)/mapfile-msp3430_drv-external \
        -M $(XF86_VIDEO_ATI_SRC)/mapfile-tda9885_drv-external \
        -M $(XF86_VIDEO_ATI_SRC)/mapfile-uda1380_drv-external \
	-M drv.mapfile

include ../Makefile.inc

# ATI's atombios code has ^M's at linefeeds which break Sun compilers
clean_linefeeds: $(CLEAN_LINEFEED_TARGET)
$(SOURCE_ADD_TARGET): $(CLEAN_LINEFEED_TARGET)

$(CLEAN_LINEFEED_TARGET): $(SOURCE_INIT_TARGET)
	find $(BUILD_DIR) -type f -name '*.[ch]' | \
		xargs perl -i -p -e 's{\r\n}{\n}g'
	touch $(CLEAN_LINEFEED_TARGET)
