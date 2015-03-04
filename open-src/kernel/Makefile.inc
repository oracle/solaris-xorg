#
# Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.
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
###############################################################################
#
#  Makefile.inc - rules common to the kernel directory
#
###############################################################################

PWD:sh=pwd
TOP:sh=hg root

MODTYPE=driver

# Hierarchical parent for pkg names in this area
# Note that each driver needs to set the MODULE_PKGTAIL
# value so that it can be appropriately installed in that package
# e.g. i915/drm define this as "MODULE_PKGTAIL = drm"
MODTYPE_PKGHIER		= driver/graphics
MODTYPE_PKGHIER_SET	= yes

# PKGCLASS - Drivers/Display for framebuffers.
MODULE_PKGCLASS		= Drivers/Display
MODULE_PKGCLASS_SET	= yes

# The package that this belongs in - must match the name in pkg/manifests
# This is something that should be a bit more dynamic.
MODULE_PKG		= driver-graphics-drm

# Default module description for packages in this area
MODTYPE_DESC	 = the $(MODULE_NAME) driver

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

#
# Default build types, driver is only 64 bit, and this one is x86 only.
#
BUILD_TYPES_DEFAULT	= $(BUILD_TYPES_SET:yes=$(POUND_SIGN))
# debug isn't working quite right yet
# $(BUILD_TYPES_DEFAULT)	BUILD_TYPES=64 debug
$(BUILD_TYPES_DEFAULT)	BUILD_TYPES=64
# $(BUILD_TYPES_DEFAULT)	BUILD_TYPES=64
$(BUILD_TYPES_DEFAULT)	BUILD_TYPES_SET=yes


# Set comments in ELF binaries to help identify versions/builds
# XXXX - We probably need something here, but not sure what.
# MODTYPE_ADD_BUILD_TARGETS=set-elf-comments

# Define common LDFLAGS
MODTYPE_LDFLAGS	= -zstrip-class=comment  -zstrip-class=exclude

#
# Defines that are specific to kernel drivers
#
# Default build options
USE_DEFAULT_CONFIG_LDFLAGS	= no
USE_DEFAULT_CONFIG_CPPFLAGS	= no
USE_DEFAULT_CONFIG_ENV		= no

#
# Much of the below is the redefinition of defines in the common
# Makefiles.  It was changed here as the environment is sufficiently
# different for kernel modules that it introduced a fair amount of
# changed code.  So it was initially decided to separate the work
# and figure later how to move this into common code (which can be
# accomplished in smaller increments).

#
# Due to the common Makefile's use of DEFAULT_LD_OPTIONS, we clear it here.
#
DEFAULT_LD_OPTIONS	=

#
# Define CONFIG_ENV for kernel modules here.
# Note that _anything_ needed to build these modules must be listed here
# as they are not retained from the common Makefiles.
#
CONFIG_ENV	= CC="$(CC)" CXX="$(CXX)" AR="$(AR)"
CONFIG_ENV	+= LD="$(LD)"
CONFIG_ENV	+= CFLAGS="$(MODTYPE_CFLAGS) $(MODULE_CFLAGS)"
CONFIG_ENV	+= CXXFLAGS="$(MODTYPE_CXXFLAGS) $(MODULE_CXXFLAGS)"
CONFIG_ENV	+= CPPFLAGS="$(DEFAULT_CONFIG_CPPFLAGS) $(MODTYPE_CPPFLAGS) $(MODULE_CPPFLAGS)"
CONFIG_ENV	+= LDFLAGS="$(MODULE_LDFLAGS) $(MODTYPE_LDFLAGS)"
CONFIG_ENV	+= PKG_CONFIG_PATH="$(DEFAULT_PKG_CONFIG_PATH)"
CONFIG_ENV	+= INSTALL="$(INSTALL_SCRIPT)" CPPROG="cp -p"
CONFIG_ENV	+= MAKE="$(MODULE_MAKE)" CONFIG_SHELL="$(CONFIG_SHELL)"
CONFIG_ENV	+= NM="/usr/gnu/bin/nm"
CONFIG_ENV	+= POUND_SIGN="$(POUND_SIGN)"
CONFIG_ENV	+= PROTOMETA="$(PROTOMETA)/$(MODULE_PKG)"
CONFIG_ENV	+= X11_MAN_DIR="$(X11_MAN_DIR)"

# Where to find the common include files
DRM_INCL	= -I$(TOP)/open-src/kernel/sys/drm -I$(TOP)/open-src/kernel/sys

#
# CFLAGS for modules in open-src/kernel
#
# Compiler flags for kernel modules
KFLAGS		= -m64 -U$(MACH) -U__$(MACH) -xmodel=kernel -xvector=none \
		-Wu,-save_args -v -xildoff -D_KERNEL -D_ELF64  -D_DDI_STRICT \
		-Dsun -D__sun -D__SVR4 -D_SYSCALL32 -D_SYSCALL32_IMPL \
		-D_ASM_INLINES -Xa -xspace -xdebugformat=stabs -errtags=yes \
		-errwarn=%all -W0,-xglobalstatic

# Architecture specific flags
# New flags for sunstudio12.4
KFLAGS_i386	+= -W2,-xregs=no%float
KFLAGS_i386	+= -Wu,-xregs=no%float
KFLAGS_i386	+= -W0,-y-xregs=no%float

# To make sure ctfconvert has CTF data
CTF_FLAGS	= -g

INC_PATH	= -I. $(DRM_INCL) -I/usr/include

# Debug cflags - added in the "debug" path of the target makefile.
MODTYPE_CFLAGS_DEBUG = -DDEBUG

MODTYPE_CPPFLAGS = $(INC_PATH)
MODTYPE_CFLAGS	= $(INC_PATH) $(KFLAGS) $(KFLAGS_$(MACH)) $(CTF_FLAGS) $(PROG_FLAGS)

#
# Define the MODTYPE environment
#
MODTYPE_BUILD_ENV	= CFLAGS='$(MODTYPE_CFLAGS)'
MODTYPE_BUILD_ENV	+= CC='$(CC)' LD='$(LD)' LINT='$(LINT)'

### Include common file
include $(TOP)/open-src/common/Makefile.inc
