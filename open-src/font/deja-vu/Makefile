###############################################################################
#
# Deja Vu Fonts Makefile
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
# ident	"@(#)Makefile	1.13	09/12/05 SMI"
#

# Package name used in tarballs
MODULE_NAME=dejavu-fonts-ttf

# Version number (used in path names)
MODULE_VERSION=2.30

# Download site for source
SOURCE_URL=$(SF_DOWNLOADS_URL)/dejavu/$(SOURCE_TARBALL_NAME)
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


