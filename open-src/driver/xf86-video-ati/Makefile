###############################################################################
#
# xf86-video-ati Makefile
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)/extensions$(SERVERMOD_subdir) \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-M $(PWD)/mapfile-bt829_drv-external \
	-M $(PWD)/mapfile-fi1236_drv-external \
	-M $(PWD)/mapfile-msp3430_drv-external \
	-M $(PWD)/mapfile-tda9885_drv-external \
	-M $(PWD)/mapfile-uda1380_drv-external \
	-R$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-R$(X11_SERVERMODS_DIR)/extensions$(SERVERMOD_subdir) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

# Need to override install path for all the modules, not just ati_drv.so
MULTIMEDIA_MODDIR = $(X11_SERVERMODS_DIR)/multimedia$(SERVERMOD_subdir)
MODULE_MAKEFLAGS = radeon_drv_ladir='$(DRIVER_DIR)' \
	theatre_detect_drv_ladir='$(MULTIMEDIA_MODDIR)' \
	theatre_drv_ladir='$(MULTIMEDIA_MODDIR)' \
	theatre200_drv_ladir='$(MULTIMEDIA_MODDIR)'

include ../Makefile.inc
