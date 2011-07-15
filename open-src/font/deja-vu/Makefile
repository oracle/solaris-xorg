###############################################################################
#
# Deja Vu Fonts Makefile
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=dejavu-fonts-ttf

# Package metadata
UPSTREAM	= DejaVu fonts
UPSTREAM_URL	= http://dejavu-fonts.org/
UPSTREAM_SET	= yes

MODULE_LICENSE          = Bitstream Vera Fonts License
MODULE_LICENSE_SET      = yes

# Version number (used in path names)
MODULE_VERSION=2.33

# Checksums for upstream tarball
TARBALL_MD5  = 8b601e91725b6d69141b0fcf527948c0
TARBALL_SHA1 = 4459d6adc3d5cfeb08e5fbe7a4310da96a58f752

# Download site for source
SOURCE_URL=$(SF_DOWNLOADS_URL)/dejavu/dejavu/$(MODULE_VERSION)/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=		# None

# What to install
MODULE_ADD_INSTALL_TARGETS=install_fonts_conf 

# Directory to install to
TTF_SUBDIR=dejavu
TTF_SUBDIR_SET=yes
FONTCONF_DEST_DIR=$(PROTODIR)/etc/fonts

# License file
LICENSE_FILE=LICENSE
LICENSE_FILE_SET=yes

TTF_FONT_FILES= \
	README \
	$(LICENSE_FILE) \
	ttf/*.ttf

# Subdir of $(SOURCE_DIR) containing *.ttf files
TTF_SOURCE_SUBDIR=ttf/

### Include common rulesets
include ../Makefile.ttf

### Rules specific to this directory:

install_fonts_conf: $(SOURCE_TARGETS)
	mkdir -p $(FONTCONF_DEST_DIR)/conf.avail \
		$(FONTCONF_DEST_DIR)/conf.d
	@( cd $(SOURCE_DIR)/fontconfig ; $(START_CMD_ECHO) ; \
	  for f in *.conf ; do \
		rm -f $(FONTCONF_DEST_DIR)/conf.avail/$$f \
			$(FONTCONF_DEST_DIR)/conf.d/$$f ; \
		cp -p $$f $(FONTCONF_DEST_DIR)/conf.avail/ ; \
		ln -s ../conf.avail/$$f $(FONTCONF_DEST_DIR)/conf.d/ ; \
	  done )


