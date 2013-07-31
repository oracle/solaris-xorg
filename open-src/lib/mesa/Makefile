###############################################################################
#
# Copyright (c) 2006, 2013, Oracle and/or its affiliates. All rights reserved.
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
ORACLE_TPNO		= 9926

# ARC cases that covered this module
# LSARC/2005/109 MESA, Open Source OpenGL clone
MODULE_ARC_CASES = LSARC/2005/109
# PSARC/2013/264 Mesa 9.0.3
MODULE_ARC_CASES += PSARC/2013/264

# Version number (used in path names)
MODULE_VERSION=9.0.3

# Checksums for upstream tarball
TARBALL_MD5  = d7515cc5116c72ac63d735655bd63689
TARBALL_SHA1 = c4d88d83d8dbe243e30dc07195888e19ddb1c402

# Source tarball
SOURCE_TARBALL_NAME=MesaLib-$(MODULE_VERSION).tar.bz2
SOURCE_TARBALL_NAME_SET=yes

# Download site for source
SOURCE_URL=ftp://ftp.freedesktop.org/pub/mesa/$(MODULE_VERSION)/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/mesa
GIT_URL_SET=yes

# Directory created by unpacking source
SOURCE_DIRNAME=Mesa-$(MODULE_VERSION)
SOURCE_DIRNAME_SET=yes
SOURCE_DIR = $(BUILD_DIR)/$(SOURCE_DIRNAME)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	CVE-2013-1993.patch,-p1 \
	install-pc-dir.patch \
	solaris-port.patch \
	mesa9-port.patch

# File created by running configure script
AUTOCONF_TARGET=$(SOURCE_DIR)/configs/autoconf
AUTOCONF_TARGET_SET=yes
MODULE_ADD_CONFIGURE_TARGETS=set_pc_dir
MODULE_INSTALL_DEPS=set_pc_dir

DRI_DRIVER_LIST_sparc=
DRI_DRIVER_LIST_i386=i915 i965 radeon r200
DRI_DRIVER_LIST= $(DRI_DRIVER_LIST_$(MACH)) swrast

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --with-driver=dri --disable-egl --disable-gallium \
	--with-dri-driverdir='$(X11_SERVERMODS_DIR)/dri$(ARCHLIBSUBDIR)' \
	--with-dri-drivers='$(DRI_DRIVER_LIST)' \
	--with-gallium-drivers=  --disable-gallium-llvm

# Build mesa with this define (CR 7032292)
MODULE_CFLAGS = -D__EXTENSIONS__

# Override LDFLAGS passed to configure to not include CFLAGS, since that
# breaks building libGLU with Sun Studio C++ compiler
#MODULE_CONFIG_ENV = LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(ARCH_FLAGS) $(MODTYPE_LDFLAGS) $(MODULE_CFLAGS) $(MODULE_LDFLAGS)"

# Add proto area to path to find makedepend
MODULE_BUILD_ENV = PATH="$(PROTODIR)/$(X11_BIN_DIR):$(PATH)"
MODULE_CONFIG_ENV += $(MODULE_BUILD_ENV)

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# To build with gcc, uncomment these lines and comment out the MODULE_CFLAGS
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Tell Sun compiler to not apply optimizations that increase binary sizes
# Without this flag, Studio 12 binaries are almost 2x bigger (CR 6808868)
#MODULE_CFLAGS = -xspace

# Add include paths and linker paths needed for DRM modules
MODULE_CPPFLAGS = -I$(PROTODIR)/usr/include/libdrm

MODULE_LDFLAGS= -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
		-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) -z ignore

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# pkg-config files that need to have -R added
FIX_PC_FILES=src/*/*.pc.in src/mesa/drivers/dri/dri.pc.in

### Include common rulesets
include ../Makefile.inc

# Mesa makefiles use a different variable name/method for pkgconfig path
set_pc_dir: $(AUTOCONF_TARGET)
	echo 'INSTALL_PC_DIR=$(PKGCONFIG_DIR)' >> $(AUTOCONF_TARGET)

# Move installed files to mesa subdirs and make links back
LIBRARIES_TO_LINK = libGL.so.1.2.0 libdricore9.0.3.so.1.0.0 libglapi.so.0.0.0
MESA_LIBDIR = $(PROTODIR)/usr/lib/mesa$(ARCHLIBSUBDIR)

HEADERS_TO_MOVE = gl.h gl_mangle.h glext.h \
	glx.h glx_mangle.h glxext.h \
	osmesa.h vms_x_fix.h wglext.h wmesa.h
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
	ln -s ../../mesa/internal/dri_interface.h \
		$(PROTODIR)/usr/include/GL/internal/
	-rm -f $(HEADERS_TO_DIRECT_LINK:%=$(PROTODIR)/usr/include/GL/%) \
		$(HEADERS_TO_RUNTIME_LINK:%=$(PROTODIR)/usr/include/GL/%)
	ln -s $(HEADERS_TO_DIRECT_LINK:%=../mesa/%) \
		$(HEADERS_TO_RUNTIME_LINK:%=../../../system/volatile/opengl/include/%) \
		$(PROTODIR)/usr/include/GL
	mkdir -p $(PROTODIR)/usr/lib/GL/$(SUBDIR64) \
		$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)
	-rm -f $(PROTODIR)/usr/lib/GL/64 $(PROTODIR)/usr/lib/mesa/64
	ln -s $(SUBDIR64) $(PROTODIR)/usr/lib/GL/64
	ln -s $(SUBDIR64) $(PROTODIR)/usr/lib/mesa/64
	-rm -f $(LIBRARIES_TO_LINK:%=$(PROTODIR)/usr/lib/mesa/%) \
		$(LIBRARIES_TO_LINK:%=$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/%)
	mv $(LIBRARIES_TO_LINK:%=$(PROTODIR)/usr/lib/%) \
		$(PROTODIR)/usr/lib/mesa/
	mv $(LIBRARIES_TO_LINK:%=$(PROTODIR)/usr/lib/$(SUBDIR64)/%) \
		$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/
	-rm -f $(PROTODIR)/usr/lib/mesa/libGL.so.1 
	-rm -f $(PROTODIR)/usr/lib/mesa/libGL.so 
	-rm -f $(PROTODIR)/usr/lib/mesa/libGLU.so.1
	-rm -f $(PROTODIR)/usr/lib/GL/libGL.so.1
	-rm -f $(PROTODIR)/usr/lib/GL/libGLU.so.1
	-rm -f $(PROTODIR)/usr/lib/GL/libGL.so 
	-rm -f $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGL.so.1 
	-rm -f $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/ibGLU.so.1
	-rm -f $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so.1
	-rm -f $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so
	-rm -f $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGLU.so.1
	ln -s libGL.so.1.2.0 $(PROTODIR)/usr/lib/mesa/libGL.so.1
	ln -s libGL.so.1 $(PROTODIR)/usr/lib/mesa/libGL.so
	ln -s libdricore9.0.3.so.1.0.0 $(PROTODIR)/usr/lib/mesa/libdricore9.0.3.so.1
	ln -s libdricore9.0.3.so.1 $(PROTODIR)/usr/lib/mesa/libdricore9.0.3.so
	ln -s libglapi.so.0.0.0 $(PROTODIR)/usr/lib/mesa/libglapi.so.0
	ln -s libglapi.so.0 $(PROTODIR)/usr/lib/mesa/libglapi.so
	ln -s libGL.so.1.2.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGL.so.1
	ln -s libGL.so.1 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGL.so
	ln -s libdricore9.0.3.so.1.0.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libdricore9.0.3.so.1
	ln -s libdricore9.0.3.so.1 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libdricore9.0.3.so
	ln -s libglapi.so.0.0.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libglapi.so.0
	ln -s libglapi.so.0 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libglapi.so
	rm -f $(PROTODIR)/usr/lib/*libglapi*
	rm -f $(PROTODIR)/usr/lib/*dricore9.0.3*
	rm -f $(PROTODIR)/usr/lib/*libGL.*
	rm -f $(PROTODIR)/usr/lib/$(SUBDIR64)/*libglapi*
	rm -f $(PROTODIR)/usr/lib/$(SUBDIR64)/*libdricore*
	rm -f $(PROTODIR)/usr/lib/$(SUBDIR64)/*libGL.*
	rm -f $(PROTODIR)/usr/lib/xorg/modules/dri/*.la
	rm -f $(PROTODIR)/usr/lib/xorg/modules/dri/$(SUBDIR64)/*.la
	(cd $(PROTODIR)/usr/lib/GL; ln -s $(LIBRARIES_TO_LINK:%=../mesa/%) .)
	ln -s libGL.so.1.2.0 $(PROTODIR)/usr/lib/GL/libGL.so.1
	ln -s libGL.so.1 $(PROTODIR)/usr/lib/GL/libGL.so
	ln -s libdricore9.0.3.so.1.0.0 $(PROTODIR)/usr/lib/GL/libdricore9.0.3.so.1
	ln -s libdricore9.0.3.so.1 $(PROTODIR)/usr/lib/GL/libdricore9.0.3.so
	ln -s libglapi.so.0.0.0 $(PROTODIR)/usr/lib/GL/libglapi.so.0
	ln -s libglapi.so.0 $(PROTODIR)/usr/lib/GL/libglapi.so
	(cd $(PROTODIR)/usr/lib/GL/$(SUBDIR64); ln -s $(LIBRARIES_TO_LINK:%=../../mesa/$(SUBDIR64)/%) .)
	ln -s libGL.so.1.2.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so.1
	ln -s libGL.so.1 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so
	ln -s libdricore9.0.3.so.1.0.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libdricore9.0.3.so.1
	ln -s libdricore9.0.3.so.1 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libdricore9.0.3.so
	ln -s libglapi.so.0.0.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libglapi.so.0
	ln -s libglapi.so.0 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libglapi.so
	(cd $(PROTODIR)/usr/lib; ln -s $(LIBRARIES_TO_LINK:%=GL/%) .)
	ln -s libGL.so.1.2.0 $(PROTODIR)/usr/lib/libGL.so.1
	ln -s libGL.so.1 $(PROTODIR)/usr/lib/libGL.so
	ln -s libdricore9.0.3.so.1.0.0 $(PROTODIR)/usr/lib/libdricore9.0.3.so.1
	ln -s libdricore9.0.3.so.1 $(PROTODIR)/usr/lib/libdricore9.0.3.so
	ln -s libglapi.so.0.0.0 $(PROTODIR)/usr/lib/libglapi.so.0
	ln -s libglapi.so.0 $(PROTODIR)/usr/lib/libglapi.so
	(cd $(PROTODIR)/usr/lib/$(SUBDIR64); ln -s $(LIBRARIES_TO_LINK:%=../GL/$(SUBDIR64)/%) .)
	ln -s libGL.so.1.2.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGL.so.1
	ln -s libGL.so.1 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGL.so
	ln -s libdricore9.0.3.so.1.0.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libdricore9.0.3.so.1
	ln -s libdricore9.0.3.so.1 $(PROTODIR)/usr/lib/$(SUBDIR64)/libdricore9.0.3.so
	ln -s libglapi.so.0.0.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libglapi.so.0
	ln -s libglapi.so.0 $(PROTODIR)/usr/lib/$(SUBDIR64)/libglapi.so
	-rm -f $(PROTODIR)/usr/lib/GL/libGL.so.1.2.0
	ln -s ../../../system/volatile/opengl/lib/libGL.so.1.2.0 \
		$(PROTODIR)/usr/lib/GL/libGL.so.1.2.0
	ln -s ../../../../usr/lib/mesa/libGL.so.1.2.0 $(PROTODIR)/system/volatile/opengl/lib/libGL.so.1.2.0
	-rm -f $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so.1.2.0
	ln -s ../../../../system/volatile/opengl/lib/$(SUBDIR64)/libGL.so.1.2.0 \
		$(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGL.so.1.2.0
	ln -s ../../../../../usr/lib/mesa/$(SUBDIR64)/libGL.so.1.2.0 $(PROTODIR)/system/volatile/opengl/lib/$(SUBDIR64)/libGL.so.1.2.0
	mkdir -p $(PROTODIR)/usr/X11/lib/$(SUBDIR64) \
		$(PROTODIR)/usr/X11/include
	-rm -f $(PROTODIR)/usr/X11/lib/GL $(PROTODIR)/usr/X11/lib/mesa
	ln -s ../../lib/GL $(PROTODIR)/usr/X11/lib/GL
	ln -s ../../lib/mesa $(PROTODIR)/usr/X11/lib/mesa
	-rm -f $(PROTODIR)/usr/X11/lib/64
	ln -s $(SUBDIR64) $(PROTODIR)/usr/X11/lib/64
	(cd $(PROTODIR)/usr/X11/lib; ln -s $(LIBRARIES_TO_LINK:%=GL/%) .)
	ln -s libGL.so.1.2.0 $(PROTODIR)/usr/X11/lib/libGL.so.1
	ln -s libGL.so.1 $(PROTODIR)/usr/X11/lib/libGL.so
	ln -s libdricore9.0.3.so.1.0.0 $(PROTODIR)/usr/X11/lib/libdricore9.0.3.so.1
	ln -s libdricore9.0.3.so.1 $(PROTODIR)/usr/X11/lib/libdricore9.0.3.so
	ln -s libglapi.so.0.0.0 $(PROTODIR)/usr/X11/lib/libglapi.so.0
	ln -s libglapi.so.0 $(PROTODIR)/usr/X11/lib/libglapi.so
	(cd $(PROTODIR)/usr/X11/lib/$(SUBDIR64); ln -s $(LIBRARIES_TO_LINK:%=../GL/$(SUBDIR64)/%) .)
	ln -s libGL.so.1.2.0 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libGL.so.1
	ln -s libGL.so.1 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libGL.so
	ln -s libdricore9.0.3.so.1.0.0 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libdricore9.0.3.so.1
	ln -s libdricore9.0.3.so.1 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libdricore9.0.3.so
	ln -s libglapi.so.0.0.0 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libglapi.so.0
	ln -s libglapi.so.0 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libglapi.so
	-rm -f $(PROTODIR)/usr/X11/include/GL $(PROTODIR)/usr/X11/include/mesa
	ln -s ../../include/GL ../../include/mesa $(PROTODIR)/usr/X11/include/
	$(TOP)/open-src/data/ogl-select/mesa_vendor_select root="$(PROTODIR)"
