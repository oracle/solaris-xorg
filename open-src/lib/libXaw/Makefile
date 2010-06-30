###############################################################################
#
# Xaw Makefile
#
# Copyright (c) 2007, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXaw

# Version number (used in path names)
MODULE_VERSION=1.0.7

# Checksums for upstream tarball
TARBALL_MD5  = 815e74de989ccda684e2baf8d12cf519
TARBALL_SHA1 = 0e5effe33c4c44d960132e3dd50370134a28f8b2

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=

# Library name
LIBNAME=Xaw

# Library man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=3xaw

# Link with version mapfile
# MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=xaw7.pc.in

# Additional arguments to pass to configure
# We only ship libXaw.so.7, since we don't need to maintain binary
# compatibility with the libXaw.so.6 version we never shipped, and never
# started shipping the now-dropped-by-X.Org libXaw.so.8 (Xaw + Xprint)
MODULE_CONFIG_OPTS= --disable-xaw6

include ../Makefile.inc

# Remove existing Xaw headers from proto area before build to avoid conflicts
clean: clean_old_headers

clean_old_headers:
	rm -rf $(PROTODIR)$(X11_INCLUDES_DIR)/X11/Xaw
