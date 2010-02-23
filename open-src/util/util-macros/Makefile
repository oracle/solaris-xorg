###############################################################################
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
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
#

# Package name used in tarballs
MODULE_NAME=util-macros

# Version number (used in path names)
MODULE_VERSION=1.3.0

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_libtool_fix

### Include common rulesets
include ../Makefile.inc

# Libtool versions before 2.2.2 fail to work with LD=cc in 64 bit builds, 
# so install fixed libtool m4 macros here as well, so that modules that
# run autoreconf get the fixed sources
install_libtool_fix:
	mkdir -p $(PROTODIR)$(X11_ACLOCAL_DIR)/
	-rm -f $(PROTODIR)$(X11_ACLOCAL_DIR)/libtool.m4
	SYSTEM_ACLOCAL_DIR=$$($(ACLOCAL_PATH) --print-ac-dir) ; \
	cp $${SYSTEM_ACLOCAL_DIR}/libtool.m4 \
		$(PROTODIR)$(X11_ACLOCAL_DIR)/libtool.m4
	gpatch -d $(PROTODIR)$(X11_ACLOCAL_DIR) < libtool-64.patch
