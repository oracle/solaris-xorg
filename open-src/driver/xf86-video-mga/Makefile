###############################################################################
#
# xf86-video-mga Makefile
#
# Copyright (c) 2006, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-mga

# Version number (used in path names)
MODULE_VERSION=1.6.4

# Checksums for upstream tarball
TARBALL_MD5   = cd3db8370caa3e607614ea4e74d4c350
TARBALL_SHA1  = 2f8cecdb6fe3f0addb47a3a50928ff0705a31352
TARBALL_SHA256= 48c6690b6751c76f53de64f8dbeaa9d6c62dbcfe890c768fd87167951247d44f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
		libraries.patch,-p1 \
		g200se_pilot3_offscreen.patch,-p1 \
		sparc.patch,-p1 \
		rom_map.patch,-p1

# Regenerate Makefile after libraries.patch patches src/Makefile.am
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - Disable DRI since the kernel module isn't ported
MODULE_CONFIG_OPTS=--disable-dri

# Extra flags to link mga_drv.so, but not util/stormdwg binary with
DRIVER_LD_FLAGS = \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR)
MODULE_CONFIG_OPTS += XORG_LIBS="$(DRIVER_LD_FLAGS)"

include ../Makefile.inc


