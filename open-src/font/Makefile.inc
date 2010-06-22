# -*- Makefile -*- rules common to most X.Org font modules
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

### Makefile settings/rules common to all X.Org font modules

PWD:sh=pwd
TOP=$(PWD)/../../..

MODTYPE=font

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

# Most font modules have no man pages
MODTYPE_SUNTOUCH_MAN_FLAGS= 

# No common dependencies
MODTYPE_CONFIGURE_DEPS=

# Set default installation path for fonts
MODTYPE_CONFIG_OPTS = --with-fontrootdir="$(X11_FONT_DIR)"
MODTYPE_CONFIG_OPTS += $(MODULE_FONTDIR:%=--with-fontdir="%")

# Disable compression of pcf fonts
MODTYPE_CONFIG_OPTS += --without-compression

# Most font modules have nothing to build, for those that do, use lib flags
MODTYPE_CFLAGS=$(LIB_CFLAGS)
MODTYPE_LDFLAGS=$(LIB_LDFLAGS)
MODTYPE_LD_OPTIONS=

# Paths to find mkfontscale, fc-cache & the libraries they need in build area
FONT_UTILS_PATH=$(PROTODIR)$(X11_DIR)/bin:$(PROTODIR)/usr/bin
FONT_LIBS_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR):$(PROTODIR)$(X11_DIR)/lib:$(PROTODIR)/usr/lib

# Additional environment variable options to GNU autoconf configure script
MODTYPE_CONFIG_ENV = PATH="$(FONT_UTILS_PATH):$$PATH" \
	LD_LIBRARY_PATH="$(FONT_LIBS_PATH)"

# Environment variable options needed when building
MODTYPE_BUILD_ENV = PATH="$(FONT_UTILS_PATH):$$PATH" \
	LD_LIBRARY_PATH="$(FONT_LIBS_PATH)"

# Need to install links in fontpath.d directory
MODTYPE_ADD_INSTALL_TARGETS = install_fontpath_links

### Include common rulesets
include $(TOP)/open-src/common/Makefile.inc

# Can't use Makefile substitutions or dependencies on the links themselves
#  due to the : characters & shell math expansion in the link names
# Ignores errors since /usr/bin/ln -sf can fail due to race conditions
#  when parallel makes try making the same symlink at the same time
install_fontpath_links: $(PROTODIR)$(X11_FONT_CATALOGUE)
	@for l in $(FONTPATH_LINKS) ; do \
	    if [[ ! -L $(PROTODIR)$(X11_FONT_CATALOGUE)/$$l ]] ; then \
		( $(START_CMD_ECHO) ; ln -sf ../../..$(MODULE_FONTDIR) \
		  $(PROTODIR)$(X11_FONT_CATALOGUE)/$$l || true ); \
	    fi \
	done

$(PROTODIR)$(X11_FONT_CATALOGUE):
	mkdir -p $@
