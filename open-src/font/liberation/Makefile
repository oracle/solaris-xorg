###############################################################################
#
# Liberation Fonts Makefile
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=liberation-fonts-ttf

# Package metadata
UPSTREAM	= Fedora Fonts SIG
UPSTREAM_URL	= http://fedorahosted.org/liberation-fonts/
UPSTREAM_SET	= yes

MODULE_LICENSE          = Liberation Font Software License Agreement
MODULE_LICENSE_SET      = yes
MODULE_LICENSE_PREPEND	= $(GPL_CHOICE_FILE)

# Version number (used in path names)
MODULE_VERSION=1.07.2

# Checksums for upstream tarball
TARBALL_MD5   = 274ba7088e294d5dd27616aabbd88d34
TARBALL_SHA1  = bf6314386793d99d4f86987ff86fd274e9a679e6
TARBALL_SHA256= b1bbf2df80f388ca1748a41068fafebc8efdfdee7612afb78a7a7cd31004d50f

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Download site for source
SOURCE_URL=http://fedorahosted.org/releases/l/i/liberation-fonts/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=		# None

# License file
LICENSE_FILE=License.txt
LICENSE_FILE_SET=yes

TTF_FONT_FILES= \
	$(LICENSE_FILE) \
	LiberationMono-Bold.ttf \
	LiberationMono-BoldItalic.ttf \
	LiberationMono-Italic.ttf \
	LiberationMono-Regular.ttf \
	LiberationSans-Bold.ttf \
	LiberationSans-BoldItalic.ttf \
	LiberationSans-Italic.ttf \
	LiberationSans-Regular.ttf \
	LiberationSansNarrow-Bold.ttf \
	LiberationSansNarrow-BoldItalic.ttf \
	LiberationSansNarrow-Italic.ttf \
	LiberationSansNarrow-Regular.ttf \
	LiberationSerif-Bold.ttf \
	LiberationSerif-BoldItalic.ttf \
	LiberationSerif-Italic.ttf \
	LiberationSerif-Regular.ttf

# Directory to install to
TTF_SUBDIR=liberation
TTF_SUBDIR_SET=yes

# Since normal package version includes a number starting with 0, which
# pkg rules disallow, add a human-version to display that
MODULE_ATTRDATA_COMMANDS = \
	print 'set name=pkg.human-version value="$(MODULE_VERSION)"' >> $@

### Include common rulesets
include ../Makefile.ttf

# Additional license file
install_license: $(MODULE_PKG_METADATA_DIR)/license_GPL

$(MODULE_PKG_METADATA_DIR)/license_GPL: $(SOURCE_DATA_DIR)/COPYING
	rm -f $@
	cp -pf $(SOURCE_DATA_DIR)/COPYING $@
	print 'license license_GPL license="GPLv2"' > $(LICENSE_ATTR_FILE)
