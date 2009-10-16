###############################################################################
#
# Droid Fonts Makefile
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
# ident	"@(#)Makefile	1.2	09/10/13 SMI"
#

# Package name used in tarballs
MODULE_NAME=google-droid-fonts

# Version number (used in path names)
MODULE_VERSION=1.00

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Directory created when unpacking tarball
SOURCE_DIRNAME=base
SOURCE_DIRNAME_SET=yes

# Download site for source
SOURCE_URL='http://android.git.kernel.org/?p=platform/frameworks/base.git;a=snapshot;h=b2fdf4096d6f8fc4aaca49bd804f480ba7892856;sf=tgz'
SOURCE_URL_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=		# None

# No configure script to run
CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes

# What to build
BUILD_TARGETS=$(SOURCE_DIR)/fonts.scale
BUILD_TARGETS_SET=yes

# What to install
INSTALL_TARGETS=install_droid
INSTALL_TARGETS_SET=yes

# License file
LICENSE_FILE=NOTICE
LICENSE_FILE_SET=yes

### Include common rulesets
include ../Makefile.inc

### Rules specific to this directory:

DROID_FONT_FILES= \
    DroidSans.ttf		\
    DroidSans-Bold.ttf		\
    DroidSerif-Regular.ttf	\
    DroidSerif-Bold.ttf		\
    DroidSerif-Italic.ttf	\
    DroidSerif-BoldItalic.ttf	\
    DroidSansMono.ttf		\
    DroidSansFallback.ttf

$(SOURCE_DIR)/fonts.scale: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) && $(MKFONTSCALE) . )

FONT_DEST_DIR=$(PROTODIR)$(X11_FONT_DIR)/TrueType/google-droid

install_droid:: $(BUILD_TARGETS)
	-rm -rf $(FONT_DEST_DIR)
	mkdir -p $(FONT_DEST_DIR)
	( cd $(SOURCE_DIR) ; cp -p $(DROID_FONT_FILES) $(FONT_DEST_DIR)/ )
	$(MKFONTSCALE) $(FONT_DEST_DIR)
	$(MKFONTDIR) $(FONT_DEST_DIR)



