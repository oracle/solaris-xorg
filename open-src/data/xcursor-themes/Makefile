###############################################################################
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use subject to license terms.
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

# Package name used in tarballs
MODULE_NAME=xcursor-themes

# Version number (used in path names)
MODULE_VERSION=1.0.2

# Checksums for upstream tarball
TARBALL_MD5  = e8929a1bb4d98d1a3991fd340d0b5f09
TARBALL_SHA1 = 12b865c17c65e70a6bc20300dfa660fe8faaa0a4

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Additional environment variable options to GNU autoconf configure script
MODULE_CONFIG_ENV= PATH="$(PROTODIR)$(X11_DIR)/bin:$(PATH)"

# Additional environment variable options needed when building
MODULE_BUILD_ENV = LD_LIBRARY_PATH="$(PROTODIR)$(X11_DIR)/lib" \
	PATH="$(PROTODIR)$(X11_DIR)/bin:$(PATH)"

### Include common rulesets
include ../Makefile.inc



