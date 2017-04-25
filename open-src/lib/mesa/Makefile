############################################################################### 
#
# Copyright (c) 2006, 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=mesa

# Package metadata
UPSTREAM	= The Mesa 3-D Graphics Library
UPSTREAM_URL	= http://mesa3d.org/
UPSTREAM_SET	= yes

MODULE_LICENSE          = Mesa License (Mixed: MIT, LGPLv2, others)
MODULE_LICENSE_SET      = yes
MODULE_LICENSE_PREPEND	= $(GPL_CHOICE_FILE)
MODULE_DESC		= the Mesa 3-D Graphics Libraries
ORACLE_TPNO		= 31745

# ARC cases that covered this module
# LSARC/2005/109 MESA, Open Source OpenGL clone
MODULE_ARC_CASES = LSARC/2005/109
# PSARC/2013/264 Mesa 9.0.3
MODULE_ARC_CASES += PSARC/2013/264
# PSARC/2015/251 Xorg server 1.17
MODULE_ARC_CASES += PSARC/2015/251
# PSARC/2016/560 Mesa 12
MODULE_ARC_CASES += PSARC/2016/560
# PSARC/2017/085 Mesa EGL/GLES
MODULE_ARC_CASES += PSARC/2017/085

# Version number (used in path names)
MODULE_VERSION=12.0.3

# Checksums for upstream tarball
TARBALL_MD5  = 60c5f9897ddc38b46f8144c7366e84ad
TARBALL_SHA1 = 3661e2f6b3ff71b7498fa787848959059517e92a
TARBALL_SHA256 = 79abcfab3de30dbd416d1582a3cf6b1be308466231488775f1b7bb43be353602 

# Source tarball
SOURCE_TARBALL_NAME=mesa-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Download site for source
SOURCE_URL=ftp://ftp.freedesktop.org/pub/mesa/$(MODULE_VERSION)/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/mesa
GIT_URL_SET=yes

# Directory created by unpacking source
SOURCE_DIRNAME=Mesa-$(MODULE_VERSION)
SOURCE_DIRNAME_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += 0001-Can-t-use-restrict-in-C.patch 0002-libglapi.patch 
CLONE_SRC = yes

# Regenerate configure after patching configure.ac in mesa9-port.patch
AUTORECONF=yes

# delibtoolize does not get along with Mesa 9.0.3
DELIBTOOLIZE = no

DRI_DRIVER_LIST_sparc=
DRI_DRIVER_LIST_i386=i915 i965 radeon r200
DRI_DRIVER_LIST= $(DRI_DRIVER_LIST_$(MACH)) swrast

MODULE_CXXFLAGS += -std=c++11

# Command line options to GNU autoconf configure script

MODULE_CONFIG_OPTS_32 = --disable-gallium-llvm
MODULE_CONFIG_OPTS_64 =  --enable-gallium-llvm --disable-llvm-shared-libs

MODULE_CONFIG_OPTS = --enable-shared --with-dri-drivers='$(DRI_DRIVER_LIST)'
MODULE_CONFIG_OPTS += --with-gallium-drivers=swrast 
MODULE_CONFIG_OPTS +=  --with-dri-driverdir='$(X11_SERVERMODS_DIR)/dri$(ARCHLIBSUBDIR)' 
MODULE_CONFIG_OPTS += $(MODULE_CONFIG_OPTS_$(BUILD_TYPE))

# Build mesa with this define (Bug 15706016)
MODULE_CFLAGS = -D__EXTENSIONS__

MODULE_CPPFLAGS = -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
# Add proto area to path to find makedepend
MODULE_BUILD_ENV = PATH="$(PROTODIR)/$(X11_BIN_DIR):/usr/gnu/bin:$(PATH)"
MODULE_BUILD_ENV += LLVM_CONFIG=$(PWD)/llvm-config
MODULE_CONFIG_ENV += $(MODULE_BUILD_ENV)

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes
MODULE_MAKEFLAGS = TOP="$(PWD)/$(BUILD_DIR)"

# Need to build with gcc
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes
USE_PARFAIT=no

MODULE_LD_OPTIONS += -M $(PWD)/mapfile.externs 
MODULE_LD_SHARED_OPTIONS_64 = -z nodefs $(ZDISCARD_UNUSED_DEP) -lLLVM -lssp
MODULE_LD_SHARED_OPTIONS = $(MODULE_LD_SHARED_OPTIONS_$(BUILD_TYPE))

MODULE_LDFLAGS += -Wl,-z,rescan-now 

# pkg-config files that need to have -R added
FIX_PC_FILES=src/*/*.pc.in src/mesa/drivers/dri/dri.pc.in

### Include common rulesets
include ../Makefile.inc

# Move installed files to mesa subdirs and make links back
LIBRARIES_TO_LINK = libGL.so.1.2.0 libglapi.so.0.0.0

# Creating these separately here so that we avoid creating /usr/X11/*
# links for these since they are newly delivered.
LIBRARIES_TO_LINK_NEW += libEGL.so.1.0.0 libGLESv1_CM.so.1.1.0 libGLESv2.so.2.0.0
MESA_LIBDIR = $(PROTODIR)/usr/lib/mesa$(ARCHLIBSUBDIR)

HEADERS_TO_MOVE = gl.h gl_mangle.h glext.h \
	glx.h glx_mangle.h glxext.h \
	osmesa.h wglext.h 

HEADERS_TO_DIRECT_LINK = osmesa.h
HEADERS_TO_RUNTIME_LINK = gl.h glext.h glx.h glxext.h

# Run just once after all other install steps are done for both 32/64-bit
install: install_ogl_layout

install_ogl_layout:
	-rm -rf $(PROTODIR)/usr/include/mesa
	mkdir -p $(PROTODIR)/usr/include/mesa/internal
	mkdir -p $(PROTODIR)/system/volatile/opengl/include
	mkdir -p $(PROTODIR)/system/volatile/opengl/lib/$(SUBDIR64)
	mv $(HEADERS_TO_MOVE:%=$(PROTODIR)/usr/include/GL/%) \
		$(PROTODIR)/usr/include/mesa
	mv $(PROTODIR)/usr/include/GL/internal/dri_interface.h \
		$(PROTODIR)/usr/include/mesa/internal
	ln -sf ../../mesa/internal/dri_interface.h \
		$(PROTODIR)/usr/include/GL/internal/
	-rm -f $(HEADERS_TO_DIRECT_LINK:%=$(PROTODIR)/usr/include/GL/%) \
		$(HEADERS_TO_RUNTIME_LINK:%=$(PROTODIR)/usr/include/GL/%)
	ln -sf $(HEADERS_TO_DIRECT_LINK:%=../mesa/%) \
		$(HEADERS_TO_RUNTIME_LINK:%=../../../system/volatile/opengl/include/%) \
		$(PROTODIR)/usr/include/GL
	mkdir -p $(PROTODIR)/usr/lib/GL/$(SUBDIR64) \
		$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)
	-rm -f $(PROTODIR)/usr/lib/GL/64 $(PROTODIR)/usr/lib/mesa/64
	ln -sf $(SUBDIR64) $(PROTODIR)/usr/lib/GL/64
	ln -sf $(SUBDIR64) $(PROTODIR)/usr/lib/mesa/64
	-rm -f $(LIBRARIES_TO_LINK:%=$(PROTODIR)/usr/lib/mesa/%) \
		$(LIBRARIES_TO_LINK:%=$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/%)
	-rm -f $(LIBRARIES_TO_LINK_NEW:%=$(PROTODIR)/usr/lib/mesa/%) \
		$(LIBRARIES_TO_LINK_NEW:%=$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/%)
	mv $(LIBRARIES_TO_LINK:%=$(PROTODIR)/usr/lib/%) \
		$(PROTODIR)/usr/lib/mesa/
	mv $(LIBRARIES_TO_LINK:%=$(PROTODIR)/usr/lib/$(SUBDIR64)/%) \
		$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/
	mv $(LIBRARIES_TO_LINK_NEW:%=$(PROTODIR)/usr/lib/%) \
		$(PROTODIR)/usr/lib/mesa/
	mv $(LIBRARIES_TO_LINK_NEW:%=$(PROTODIR)/usr/lib/$(SUBDIR64)/%) \
		$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/
	-rm -f $(PROTODIR)/usr/lib/mesa/libGL.so.1 
	-rm -f $(PROTODIR)/usr/lib/mesa/libGL.so 
	-rm -f $(PROTODIR)/usr/lib/GL/libGL.so.1
	-rm -f $(PROTODIR)/usr/lib/GL/libGL.so 
	-rm -f $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGL.so.1 
	-rm -f $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGL.so 
	-rm -f $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so.1
	-rm -f $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so
	ln -sf libGL.so.1.2.0 $(PROTODIR)/usr/lib/mesa/libGL.so.1
	ln -sf libGL.so.1 $(PROTODIR)/usr/lib/mesa/libGL.so
	ln -sf libglapi.so.0 $(PROTODIR)/usr/lib/mesa/libglapi.so
	ln -sf libglapi.so.0.0.0 $(PROTODIR)/usr/lib/mesa/libglapi.so.0
	ln -sf libEGL.so.1 $(PROTODIR)/usr/lib/mesa/libEGL.so
	ln -sf libEGL.so.1.0.0 $(PROTODIR)/usr/lib/mesa/libEGL.so.1
	ln -sf libGLESv1_CM.so.1 $(PROTODIR)/usr/lib/mesa/libGLESv1_CM.so
	ln -sf libGLESv1_CM.so.1.1.0 $(PROTODIR)/usr/lib/mesa/libGLESv1_CM.so.1
	ln -sf libGLESv2.so.2 $(PROTODIR)/usr/lib/mesa/libGLESv2.so
	ln -sf libGLESv2.so.2.0.0 $(PROTODIR)/usr/lib/mesa/libGLESv2.so.2
	ln -sf libGL.so.1.2.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGL.so.1
	ln -sf libGL.so.1 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGL.so
	ln -sf libglapi.so.0.0.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libglapi.so.0
	ln -sf libglapi.so.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libglapi.so
	ln -sf libEGL.so.1.0.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libEGL.so.1
	ln -sf libEGL.so.1 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libEGL.so
	ln -sf libGLESv1_CM.so.1.1.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGLESv1_CM.so.1
	ln -sf libGLESv1_CM.so.1 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGLESv1_CM.so
	ln -sf libGLESv2.so.2.0.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGLESv2.so.2
	ln -sf libGLESv2.so.2 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGLESv2.so
	rm -f $(PROTODIR)/usr/lib/*libGL.*
	rm -f $(PROTODIR)/usr/lib/$(SUBDIR64)/*libGL.*
	rm -f $(PROTODIR)/usr/lib/xorg/modules/dri/*.la
	rm -f $(PROTODIR)/usr/lib/xorg/modules/dri/$(SUBDIR64)/*.la
	(cd $(PROTODIR)/usr/lib/GL; ln -sf $(LIBRARIES_TO_LINK:%=../mesa/%) .)
	rm -f $(PROTODIR)/usr/lib/GL/libGL.so.1.2.0
	ln -sf libGL.so.1 $(PROTODIR)/usr/lib/GL/libGL.so
	ln -sf libglapi.so.0.0.0 $(PROTODIR)/usr/lib/GL/libglapi.so.0
	ln -sf libglapi.so.0 $(PROTODIR)/usr/lib/GL/libglapi.so
	(cd $(PROTODIR)/usr/lib/GL; ln -sf $(LIBRARIES_TO_LINK_NEW:%=../mesa/%) .)
	ln -sf libEGL.so.1.0.0 $(PROTODIR)/usr/lib/GL/libEGL.so.1
	ln -sf libEGL.so.1 $(PROTODIR)/usr/lib/GL/libEGL.so
	ln -sf libGLESv1_CM.so.1.1.0 $(PROTODIR)/usr/lib/GL/libGLESv1_CM.so.1
	ln -sf libGLESv1_CM.so.1 $(PROTODIR)/usr/lib/GL/libGLESv1_CM.so
	ln -sf libGLESv2.so.2.0.0 $(PROTODIR)/usr/lib/GL/libGLESv2.so.2
	ln -sf libGLESv2.so.2 $(PROTODIR)/usr/lib/GL/libGLESv2.so
	(cd $(PROTODIR)/usr/lib/GL/$(SUBDIR64); ln -sf $(LIBRARIES_TO_LINK:%=../../mesa/$(SUBDIR64)/%) .)
	rm -f $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so.1.2.0
	ln -sf libGL.so.1 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so
	ln -sf libglapi.so.0.0.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libglapi.so.0
	ln -sf libglapi.so.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libglapi.so
	(cd $(PROTODIR)/usr/lib/GL/$(SUBDIR64); ln -sf $(LIBRARIES_TO_LINK_NEW:%=../../mesa/$(SUBDIR64)/%) .)
	ln -sf libEGL.so.1.0.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libEGL.so.1
	ln -sf libEGL.so.1 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libEGL.so
	ln -sf libGLESv2.so.2.0.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGLESv2.so.2
	ln -sf libGLESv2.so.2 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGLESv2.so
	ln -sf libGLESv1_CM.so.1.1.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGLESv1_CM.so.1
	ln -sf libGLESv1_CM.so.1 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGLESv1_CM.so
	(cd $(PROTODIR)/usr/lib; ln -sf $(LIBRARIES_TO_LINK:%=GL/%) .)
	rm -f $(PROTODIR)/usr/lib/libGL.so.1.2.0
	ln -sf GL/libGL.so.1 $(PROTODIR)/usr/lib/libGL.so.1
	ln -sf libGL.so.1 $(PROTODIR)/usr/lib/libGL.so
	ln -sf libglapi.so.0.0.0 $(PROTODIR)/usr/lib/libglapi.so.0
	ln -sf libglapi.so.0 $(PROTODIR)/usr/lib/libglapi.so
	(cd $(PROTODIR)/usr/lib; ln -sf $(LIBRARIES_TO_LINK_NEW:%=GL/%) .)
	ln -sf libEGL.so.1.0.0 $(PROTODIR)/usr/lib/libEGL.so.1
	ln -sf libEGL.so.1 $(PROTODIR)/usr/lib/libEGL.so
	ln -sf libGLESv2.so.2.0.0 $(PROTODIR)/usr/lib/libGLESv2.so.2
	ln -sf libGLESv2.so.2 $(PROTODIR)/usr/lib/libGLESv2.so
	ln -sf libGLESv1_CM.so.1.1.0 $(PROTODIR)/usr/lib/libGLESv1_CM.so.1
	ln -sf libGLESv1_CM.so.1 $(PROTODIR)/usr/lib/libGLESv1_CM.so
	(cd $(PROTODIR)/usr/lib/$(SUBDIR64); ln -sf $(LIBRARIES_TO_LINK:%=../GL/$(SUBDIR64)/%) .)
	rm -f $(PROTODIR)/usr/lib/$(SUBDIR64)/libGL.so.1.2.0
	ln -sf ../GL/$(SUBDIR64)/libGL.so.1 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGL.so.1
	ln -sf libGL.so.1 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGL.so
	ln -sf libglapi.so.0.0.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libglapi.so.0
	ln -sf libglapi.so.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libglapi.so
	(cd $(PROTODIR)/usr/lib/$(SUBDIR64); ln -sf $(LIBRARIES_TO_LINK_NEW:%=../GL/$(SUBDIR64)/%) .)
	ln -sf libEGL.so.1.0.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libEGL.so.1
	ln -sf libEGL.so.1 $(PROTODIR)/usr/lib/$(SUBDIR64)/libEGL.so
	ln -sf libGLESv2.so.2.0.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGLESv2.so.2
	ln -sf libGLESv2.so.2 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGLESv2.so
	ln -sf libGLESv1_CM.so.1.1.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGLESv1_CM.so.1
	ln -sf libGLESv1_CM.so.1 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGLESv1_CM.so
	ln -sf ../../../system/volatile/opengl/lib/libGL.so.1 \
		$(PROTODIR)/usr/lib/GL/libGL.so.1
	ln -sf ../../../../system/volatile/opengl/lib/$(SUBDIR64)/libGL.so.1 \
		$(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so.1
	mkdir -p $(PROTODIR)/usr/X11/lib/$(SUBDIR64) \
		$(PROTODIR)/usr/X11/include
	-rm -f $(PROTODIR)/usr/X11/lib/GL $(PROTODIR)/usr/X11/lib/mesa
	ln -sf ../../lib/GL $(PROTODIR)/usr/X11/lib/GL
	ln -sf ../../lib/mesa $(PROTODIR)/usr/X11/lib/mesa
	-rm -f $(PROTODIR)/usr/X11/lib/64
	ln -sf $(SUBDIR64) $(PROTODIR)/usr/X11/lib/64
	(cd $(PROTODIR)/usr/X11/lib; ln -sf $(LIBRARIES_TO_LINK:%=GL/%) .)
	rm -f $(PROTODIR)/usr/X11/lib/libGL.so.1.2.0
	ln -sf ../../lib/GL/libGL.so.1 $(PROTODIR)/usr/X11/lib/libGL.so.1
	ln -sf libGL.so.1 $(PROTODIR)/usr/X11/lib/libGL.so
	ln -sf libglapi.so.0.0.0 $(PROTODIR)/usr/X11/lib/libglapi.so.0
	ln -sf libglapi.so.0 $(PROTODIR)/usr/X11/lib/libglapi.so
	(cd $(PROTODIR)/usr/X11/lib/$(SUBDIR64); ln -sf $(LIBRARIES_TO_LINK:%=../GL/$(SUBDIR64)/%) .)
	rm -f $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libGL.so.1.2.0
	ln -sf ../../../lib/GL/$(SUBDIR64)/libGL.so.1 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libGL.so.1
	ln -sf libGL.so.1 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libGL.so
	ln -sf libglapi.so.0.0.0 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libglapi.so.0
	ln -sf libglapi.so.0 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libglapi.so
	-rm -f $(PROTODIR)/usr/X11/include/GL $(PROTODIR)/usr/X11/include/mesa
	ln -sf ../../include/GL ../../include/mesa $(PROTODIR)/usr/X11/include/
	$(TOP)/open-src/data/ogl-select/mesa_vendor_select root="$(PROTODIR)"
