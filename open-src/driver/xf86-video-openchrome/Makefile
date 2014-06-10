###############################################################################
#
# Copyright (c) 2009, 2014, Oracle and/or its affiliates. All rights reserved.
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
ORACLE_TPNO	= 18004
ORACLE_TPNO_SET	= yes

# Version number (used in path names)
MODULE_VERSION=0.3.2

# Checksums for upstream tarball
TARBALL_MD5  = 172509a5a7ab5c89ff09501d733cf5f6
TARBALL_SHA1 = e5be9a8200410b3e3e66349bab4c8acd1ead4402

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = manpage-sections.patch,-p1
SOURCE_PATCHES += CVE-2013-1994.patch,-p1
SOURCE_PATCHES += ldflags.patch,-p1

# Extra rules to run when unpacking upstream source
CLEAN_LINEFEED_TARGET = $(SOURCE_DIR)/.source.clean.done
MODULE_ADD_SOURCE_TARGETS = $(CLEAN_LINEFEED_TARGET)

# manpage-sections.patch edits configure.ac, so we need to autoreconf
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - Disable DRI since the kernel module isn't ported
MODULE_CONFIG_OPTS=--disable-dri --disable-kms

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
