###############################################################################
#
# Copyright (c) 2009, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-openchrome

# Package metadata
UPSTREAM        = The OpenChrome project
UPSTREAM_URL    = http://www.openchrome.org/
UPSTREAM_SET    = yes
ORACLE_TPNO	= 18321
ORACLE_TPNO_SET	= yes

# Download site for source
SOURCE_URL	= $(SOURCE_URL_xorg)
SOURCE_URL_SET	= yes
GIT_URL		= $(FDO_GIT_URL)/openchrome/xf86-video-openchrome
GIT_URL_SET	= yes

# Version number (used in path names)
MODULE_VERSION=0.4.0

# Checksums for upstream tarball
TARBALL_MD5  = eff2ca08a530c67144531466500af8cb
TARBALL_SHA1 = 8889aa051acfd2007c4bbd8d5efbaf6f3d39db51
TARBALL_SHA256 = 73e7966d01d0d644d3b9204e8bfc106cb99069c58e981626e7352f2693e3e927

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += ldflags.patch,-p1
SOURCE_PATCHES += disable-xvmc.patch,-p1

# Extra rules to run when unpacking upstream source
CLEAN_LINEFEED_TARGET = $(SOURCE_DIR)/.source.clean.done
MODULE_ADD_SOURCE_TARGETS = $(CLEAN_LINEFEED_TARGET)

# disable-xvmc.patch edits configure.ac, so we need to autoreconf
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - Disable DRI since the kernel module isn't ported
MODULE_CONFIG_OPTS=--disable-dri --disable-kms
# --libdir is needed to set path for XvMC libraries
MODULE_CONFIG_OPTS += --libdir='$(MODULE_FULLARCH_LIB_DIR)'

# Instead of allowing the default MODTYPE_LD_SHARED_OPTIONS to link all
# the libraries against the Xorg server binary, ldflags.patch adds
# $(openchrome_drv_la_LDFLAGS_add) to the link of just openchrome_drv.so,
# but not to the client side XvMC libs.
XORG_EXTERNS_IN_LD_SHARED_OPTIONS=no
openchrome_drv_la_LDFLAGS_add = \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
        -L$(PROTODIR)$(X11_LIB_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	$(XORG_EXTERNS_FLAG) -M $(PWD)/mapfile.externs \
	-lfb -lint10 -lpciaccess -lpixman-1 -lvbe \
	-lexa -lvgahw -lm -ldrm -lshadow
MODULE_BUILD_ENV = \
	openchrome_drv_la_LDFLAGS_add='$(openchrome_drv_la_LDFLAGS_add)'

include ../Makefile.inc

# via_regs.h has ^M's at linefeeds which break Sun compilers
clean_linefeeds: $(CLEAN_LINEFEED_TARGET)
$(SOURCE_ADD_TARGET): $(CLEAN_LINEFEED_TARGET)
$(PATCHED_TARGET): $(CLEAN_LINEFEED_TARGET)

$(CLEAN_LINEFEED_TARGET): $(SOURCE_INIT_TARGET)
	find $(BUILD_DIR) -type f -name '*.[ch]' | \
		xargs $(PERL) -i -p -e 's{\r\n}{\n}g'
	touch $(CLEAN_LINEFEED_TARGET)
