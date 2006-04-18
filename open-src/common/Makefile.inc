# -*- Makefile -*- rules commonly shared among X consolidation open source dirs
#
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
# @(#)Makefile.inc	1.60	06/04/11
#

BUILD_DATE:sh=date +'%e-%B-%Y'
BUILD_ID = X11 for Solaris Nevada, built $(BUILD_DATE)

### Default compiler & options
CC     			= cc
DEFAULT_OPT_CFLAGS 	= -xO4 -xbuiltin -xlibmil -xprefetch
OPT_CFLAGS 		= $(DEFAULT_OPT_CFLAGS)
CFLAGS 			=  -v -mc $(OPT_CFLAGS) $(ARCH_FLAGS)
LDFLAGS 		= -z combreloc -z lazyload $(MAPFILES_FOR_ALL)

### Flags for libraries to meet various Solaris requirements
#
# REQUIRED_LDFLAGS are forced on via LD_OPTIONS to override libtool when
# building libraries.
#
LIB_CFLAGS 		= $(CFLAGS) -D_REENTRANT $(ARCH_LIB_FLAGS)
LIB_REQUIRED_LDFLAGS	= -z text -z defs
LIB_LDFLAGS 		= $(LDFLAGS) $(LIB_REQUIRED_LDFLAGS)

# Flags for programs
PROG_LDFLAGS		= $(LDFLAGS) $(MAPFILES_FOR_PROGS)

# Linker mapfiles to link with for better performance & security
# SPARC archicteture requires PLT section in .data be executable, so
# we can only make .bss, not all of .data no-exec on SPARC
MAPFILE_DIR 		= $(TOP)/common/mapfiles/
MAPFILE_NOEXBSS		= $(MAPFILE_DIR)/map.noexbss
MAPFILE_NOEXDATA_sun4	= $(MAPFILE_NOEXBSS)
MAPFILE_NOEXDATA_i86pc	= /usr/lib/ld/map.noexdata
MAPFILE_NOEXDATA:sh=arch | sed 's/^\(.*\)$/\$\(MAPFILE_NOEXDATA_\1\)/'

MAPFILES_FOR_ALL   = -M /usr/lib/ld/map.pagealign -M $(MAPFILE_NOEXDATA)
MAPFILES_FOR_PROGS = -M /usr/lib/ld/map.bssalign -M /usr/lib/ld/map.noexstk \
			-M $(MAPFILE_NOEXBSS)

### Architecture specific flags
DEFAULT_sun4_ARCH32_FLAGS 	= -xarch=v8plus
DEFAULT_sun4_ARCH64_FLAGS 	= -xarch=v9
DEFAULT_sun4_ARCH_LIB_FLAGS 	= -xregs=no%appl

# -xregs=no%frameptr is required on x86 when compiling at -xO4 or higher to 
# avoid losing stack frame pointers so you can't get stack traces or debug
DEFAULT_i86pc_ARCH32_FLAGS 	= -xpentium -xregs=no%frameptr
DEFAULT_i86pc_ARCH64_FLAGS 	= -xarch=generic64 -xregs=no%frameptr
DEFAULT_i86pc_ARCH_LIB_FLAGS 	=  

ARCH:sh=arch | sed s/i86pc/i386/
ARCH32_FLAGS:sh=arch | sed 's/^\(.*\)$/\$\(DEFAULT_\1_ARCH32_FLAGS\)/'
ARCH64_FLAGS:sh=arch | sed 's/^\(.*\)$/\$\(DEFAULT_\1_ARCH64_FLAGS\)/'
ARCH_LIB_FLAGS:sh=arch | sed 's/^\(.*\)$/\$\(DEFAULT_\1_ARCH_LIB_FLAGS\)/'

SUBDIR64_sun4 	= sparcv9
SUBDIR64_i86pc 	= amd64
SUBDIR64:sh=arch | sed 's/^\(.*\)$/\$\(SUBDIR64_\1\)/'

ARCH_MAPFILE:sh=uname -p | sed 's/^\(.*\)$/mapfile_pga_\1/'
MAPFILE_FLAGS = -M $(TOP)/common/mapfiles/$(ARCH_MAPFILE)

### Proto directory to install into for packaging
PROTODIR=$(TOP)/../proto-$(ARCH)-svr4

# Common subdirs
OW_DIR=/usr/openwin
OW_X11LIB_DIR=$(OW_DIR)/lib/X11

FONT_DIR=$(OW_X11LIB_DIR)/fonts
TRUETYPE_DIR=$(FONT_DIR)/TrueType

X11_DIR=/usr/X11
X11_INCLUDES_DIR=$(X11_DIR)/include

### Default rules

BUILD_DIR_32 = build_32
BUILD_DIR_64 = build_64

all: $(BUILD_TYPES:%=source_%) $(BUILD_TYPES:%=build_%)
install: $(BUILD_TYPES:%=install_%)

clean: $(BUILD_TYPES:%=clean_%)

clean_32::
	-rm -rf $(BUILD_DIR_32)

clean_64::
	-rm -rf $(BUILD_DIR_64)

build_gen: $(BUILD_TARGETS)

build_32::
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_32) ARCH_FLAGS="$(ARCH32_FLAGS)" build_gen

build_64::
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_64) ARCH_FLAGS="$(ARCH64_FLAGS)" LIBSUBDIR=/$(SUBDIR64) build_gen

install_32::
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_32) ARCH_FLAGS="$(ARCH32_FLAGS)" install_gen

install_64::
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_64) ARCH_FLAGS="$(ARCH64_FLAGS)" LIBSUBDIR=/64 ARCHLIBSUBDIR=/$(SUBDIR64) install_gen

debug:
	$(MAKE) $(MFLAGS) all OPT_FLAGS="-g"

### Unpack source and apply patches

source: $(BUILD_TYPES:%=source_%)
source_32: $(BUILD_DIR_32)/.unpack_done
source_64: $(BUILD_DIR_64)/.unpack_done

UNPACK_TARGET = $(BUILD_DIR)/.unpack_done

$(BUILD_DIR_32)/.unpack_done:
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_32) source_gen

$(BUILD_DIR_64)/.unpack_done:
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_64) source_gen

source_gen::
	mkdir -p $(BUILD_DIR)
	@if [ "x$(SOURCE_UNCOMPRESS)" = "x" ] ; then \
	    echo "bzcat $(SOURCE_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )" ; \
	    ( bzcat $(SOURCE_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )) ; \
	else \
	    echo "$(SOURCE_UNCOMPRESS) $(SOURCE_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )" ; \
	    ( $(SOURCE_UNCOMPRESS) $(SOURCE_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )) ; \
	fi
	@if [ "x$(SOURCE_PATCHES)" != "x" ] ; then \
	    for p in $(SOURCE_PATCHES) x ; do \
	        if [ "$$p" != "x" ] ; then \
		    echo "gpatch -d $(SOURCE_DIR) -p0 < $$p" ; \
		    gpatch -d $(SOURCE_DIR) -p0 < $$p ; \
		fi ; \
	    done ; \
	fi
	touch $(BUILD_DIR)/.unpack_done


### Source tarballs & downloading if necessary

TARBALLDIR=$(TOP)/tarballs
WGET=/usr/sfw/bin/wget
SOURCE_TARBALL=$(TARBALLDIR)/$(SOURCE_TARBALL_NAME)

XORG_RELEASES_URL=http://xorg.freedesktop.org/releases/individual

download:
	if [ ! -f $(SOURCE_TARBALL) ] ; then \
		$(WGET) -O $(SOURCE_TARBALL) $(SOURCE_URL) ;\
	fi

### Other tools needed to build

# Some modules (such as FreeType 2) require GNU make
GNUMAKE = gmake

$(GNUMAKE):
	@echo "Checking for GNU make:"
	$(GNUMAKE) --version


