###############################################################################
#
# xf86-video-ati Makefile
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
MODULE_NAME=xf86-video-ati

# Version number (used in path names)
MODULE_VERSION=6.12.4

# Checksums for upstream tarball
TARBALL_MD5  = e662348f6f957fcedf52818d668ab9f5
TARBALL_SHA1 = efd9b9392787251756bbce6afc9715c417dae68f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	mapfiles.patch \
	solaris-dri-drv-port.patch

# Need to regenerate configure after patching configure.ac in build-dri.patch
# and regenerate Makefile.in after patching Makefile.am in mapfiles.patch
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-graphics-ddx
MODULE_STABILITY=Volatile

# AtomBios code won't build with Sun compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Extra flags to pass to configure script
MODULE_CONFIG_OPTS=--enable-dri

# Need to override setting of CONFIG_SHELL from Makefile.init to work around
# ksh93 bug that causes config.status/config.h to be incorrectly generated
MODULE_CONFIG_ENV= CONFIG_SHELL=/usr/bin/bash

# Paths to find libraries/modules to link with - libraries themselves listed
# in src/Makefile.am since ATI has multiple submodules with different needs
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)/extensions \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)/linux \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-M $(PWD)/mapfile-bt829_drv-external \
	-M $(PWD)/mapfile-fi1236_drv-external \
	-M $(PWD)/mapfile-msp3430_drv-external \
	-M $(PWD)/mapfile-tda9885_drv-external \
	-M $(PWD)/mapfile-uda1380_drv-external \
	-R$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-R$(X11_SERVERMODS_DIR)/extensions$(SERVERMOD_subdir) \
	-R$(X11_SERVERMODS_DIR)/linux$(SERVERMOD_subdir) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

include ../Makefile.inc
