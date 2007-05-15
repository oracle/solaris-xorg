# -*- Makefile -*- rules commonly shared among X consolidation open source dirs
#
# Copyright 2007 Sun Microsystems, Inc.  All Rights Reserved.
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
# @(#)Makefile.inc	1.76	07/05/15
#

# Borrowed from Solaris OS/Net makefile.master:
# The declaration POUND_SIGN is always '#'. This is needed to get around the
# make feature that '#' is always a comment delimiter, even when escaped or
# quoted.  The only way of generating this is the :sh macro mechanism.  Note
# however that in general :sh macros should be avoided in makefiles that are
# widely included into other makefiles, as the resulting shell executions can
# cause a noticable slowdown in build times.
#
POUND_SIGN:sh= echo \\043

# BUILD_DATE:sh=date +'%e-%B-%Y'
# BUILD_ID = X11 for Solaris Nevada, built $(BUILD_DATE)

###  Machine architecture macros

# MACH will be either "sparc" or "i386"
MACH:sh=uname -p

# ARCH will be either "sun4" or "i386" - mainly used in $(PROTODIR)
ARCH=$(MACH:sparc=sun4)

MACH32_sparc	= sparc
MACH64_sparc 	= sparcv9
MACH32_i386	= i386
MACH64_i386 	= amd64

MACH32= $(MACH32_$(MACH))
MACH64= $(MACH64_$(MACH))

### Default compiler & options
CC     			= cc
GCC			= /usr/sfw/bin/gcc
CXX			= CC
DEFAULT_OPT_CFLAGS 	= -xO4 -xbuiltin -xlibmil -xprefetch \
				-xdepend -xstrconst
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
PROG_CFLAGS		= $(CFLAGS)
PROG_LDFLAGS		= $(LDFLAGS) $(MAPFILES_FOR_PROGS) -B direct

# Linker mapfiles to link with for better performance & security
# SPARC archicteture requires PLT section in .data be executable, so
# we can only make .bss, not all of .data no-exec on SPARC
MAPFILE_DIR 		= $(TOP)/common/mapfiles/
MAPFILE_NOEXBSS		= $(MAPFILE_DIR)/map.noexbss
MAPFILE_NOEXDATA_sparc	= $(MAPFILE_NOEXBSS)
MAPFILE_NOEXDATA_i386	= /usr/lib/ld/map.noexdata
MAPFILE_NOEXDATA	= $(MAPFILE_NOEXDATA_$(MACH))

MAPFILES_FOR_ALL   = -M /usr/lib/ld/map.pagealign -M $(MAPFILE_NOEXDATA)
MAPFILES_FOR_PROGS = -M /usr/lib/ld/map.bssalign -M /usr/lib/ld/map.noexstk \
			-M $(MAPFILE_NOEXBSS)

### Architecture specific flags
DEFAULT_ARCH32_FLAGS_sparc 	= -xarch=v8plus
DEFAULT_ARCH64_FLAGS_sparc	= -xarch=v9
DEFAULT_ARCH_LIB_FLAGS_sparc 	= -xregs=no%appl

# -xregs=no%frameptr is required on x86 when compiling at -xO4 or higher to 
# avoid losing stack frame pointers so you can't get stack traces or debug
DEFAULT_ARCH32_FLAGS_i386 	= -xpentium -xregs=no%frameptr
DEFAULT_ARCH64_FLAGS_i386 	= -xarch=generic64 -xregs=no%frameptr
DEFAULT_ARCH_LIB_FLAGS_i386 	=  

ARCH32_FLAGS = $(DEFAULT_ARCH32_FLAGS_$(MACH))
ARCH64_FLAGS = $(DEFAULT_ARCH64_FLAGS_$(MACH))
ARCH_LIB_FLAGS = $(DEFAULT_ARCH_LIB_FLAGS_$(MACH))

### Flags for gcc
DEFAULT_GCC_CFLAGS		= -O3 -fno-omit-frame-pointer -Wall
DEFAULT_ARCH32_GCC_FLAGS_sparc 	= -mcpu=ultrasparc -m32
DEFAULT_ARCH64_GCC_FLAGS_sparc 	= -mcpu=ultrasparc -m64
DEFAULT_ARCH32_GCC_FLAGS_i386 	= -march=i686 -m32
DEFAULT_ARCH64_GCC_FLAGS_i386 	= -march=opteron -m64
ARCH32_GCC_FLAGS=$(DEFAULT_ARCH32_GCC_FLAGS_$(MACH))
ARCH64_GCC_FLAGS=$(DEFAULT_ARCH64_GCC_FLAGS_$(MACH))

# Need to use -Wl,.. to pass linker flags through gcc
GCC_LDFLAGS_CMD=echo $(LDFLAGS) | \
	sed -e 's/-M /-Wl,-M,/g' -e 's/-B /-Wl,-B,/g'
GCC_LDFLAGS=$(GCC_LDFLAGS_CMD:sh)
GCC_LIB_LDFLAGS_CMD=echo $(LIB_LDFLAGS) | \
	sed -e 's/-M /-Wl,-M,/g' -e 's/-B /-Wl,-B,/g'
GCC_LIB_LDFLAGS=$(GCC_LIB_LDFLAGS_CMD:sh)
GCC_PROG_LDFLAGS_CMD=echo $(PROG_LDFLAGS) | \
	sed -e 's/-M /-Wl,-M,/g' -e 's/-B /-Wl,-B,/g'
GCC_PROG_LDFLAGS=$(GCC_PROG_LDFLAGS_CMD:sh)


SUBDIR32_sparc	= sparcv8
SUBDIR32_i386	= i386
SUBDIR64_sparc 	= sparcv9
SUBDIR64_i386 	= amd64
SUBDIR32 = $(SUBDIR32_$(MACH))
SUBDIR64 = $(SUBDIR64_$(MACH))


### Proto directory to install into for packaging
PROTODIR=$(TOP)/../proto-$(ARCH)-svr4

# Common subdirs
OW_DIR=/usr/openwin
OW_X11LIB_DIR=$(OW_DIR)/lib/X11

FONT_DIR=$(OW_X11LIB_DIR)/fonts
TRUETYPE_DIR=$(FONT_DIR)/TrueType

X11_DIR=/usr/X11
X11_INCLUDES_DIR=$(X11_DIR)/include
X11_DOC_DIR=$(X11_DIR)/share/doc
X11_LOCALE_DIR=$(X11_DIR)/share/locale
X11_LC_MESSAGES_DIR=$(X11_LOCALE_DIR)/C/LC_MESSAGES
# Subdirectory of /usr/X11/lib for X server private libraries
X11_SERVERLIBS_SUBDIR=/X11/xserver
X11_SERVERLIBS_DIR=$(X11_DIR)/lib$(X11_SERVERLIBS_SUBDIR)

# Subdirectory of /usr/X11/lib for X server loadable modules
X11_SERVERMODS_SUBDIR=/modules
X11_SERVERMODS_DIR=$(X11_DIR)/lib$(X11_SERVERMODS_SUBDIR)

MODULE_subdir_sparc=
MODULE_subdir_i386=$(ARCHLIBSUBDIR)
MODULE_subdir=$(MODULE_subdir_$(MACH))

# Directory used for installing tools used during build but not
# delivered in packages
TOOLS_DIR=/tools

### Default rules

BUILD_DIR_32 = build_32
BUILD_DIR_64 = build_64

# Set default build type to 32-bit if not set in module makefile
BUILD_TYPES_DEFAULT	= $(BUILD_TYPES_SET:yes=$(POUND_SIGN))
$(BUILD_TYPES_DEFAULT)	BUILD_TYPES=32

all: $(BUILD_TYPES:%=source_%) $(BUILD_TYPES:%=build_%)
install: $(BUILD_TYPES:%=install_%)

clean: $(BUILD_TYPES:%=clean_%)

clean_32::
	-rm -rf $(BUILD_DIR_32)

clean_64::
	-rm -rf $(BUILD_DIR_64)

build_gen: $(BUILD_TARGETS)

build_32::
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_32) ARCH_FLAGS="$(ARCH32_FLAGS)" $(BUILD_32_FLAGS) build_gen

build_64::
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_64) ARCH_FLAGS="$(ARCH64_FLAGS)" LIBSUBDIR=/$(SUBDIR64) ARCHLIBSUBDIR=/$(SUBDIR64) $(BUILD_64_FLAGS) build_gen

install_32:: build_32
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_32) ARCH_FLAGS="$(ARCH32_FLAGS)" $(BUILD_32_FLAGS) install_gen

install_64:: build_64
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_64) ARCH_FLAGS="$(ARCH64_FLAGS)" LIBSUBDIR=/64 ARCHLIBSUBDIR=/$(SUBDIR64) $(BUILD_64_FLAGS) install_gen

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

FIX_PC_OPS= \
	-e 's|-L\$${libdir}|-L\$${libdir} -R\$${libdir}|;' \
	-e 's|Requires.private: .*$$||;'

#	-e 's|^prefix=@prefix@|prefix=\$$\{top_builddir\}@prefix@|'

source_gen::
	mkdir -p $(BUILD_DIR)
	@if [ "x$(SOURCE_TARBALL_NAME)" != "xNONE" ] ; then \
	    if [ "x$(SOURCE_UNCOMPRESS)" = "x" ] ; then \
	        echo "bzcat $(SOURCE_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )" ; \
	        ( bzcat $(SOURCE_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )) ; \
	    else \
	        echo "$(SOURCE_UNCOMPRESS) $(SOURCE_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )" ; \
	        ( $(SOURCE_UNCOMPRESS) $(SOURCE_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )) ; \
	    fi ; \
	fi
	@if [ "x$(SOURCE_PATCHES)" != "x" ] ; then \
	    for p in $(SOURCE_PATCHES) x ; do \
	        if [ "$$p" != "x" ] ; then \
		    PATCH_CMD=`echo $$p | awk -F, '{if (NF > 1) FLAGS = $$2; else FLAGS="-p0" ; printf "gpatch -d $(SOURCE_DIR) %s < %s\n", FLAGS, $$1}'` ; \
		    echo "$${PATCH_CMD}" ; \
		    eval $${PATCH_CMD} ; \
		fi ; \
	    done ; \
	fi
	@if [ "x$(SUNTOUCHED_MANPAGES)" != "x" ] ; then \
	    echo "/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
		$(SUNTOUCH_MAN_FLAGS) $(SUNTOUCHED_MANPAGES)" ; \
	    (cd $(SOURCE_DIR) && \
		/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
		$(SUNTOUCH_MAN_FLAGS) $(SUNTOUCHED_MANPAGES) ) ; \
	fi
	@if [ "x$(FIX_PC_FILES)"  != "x" ] ; then \
	    echo "/usr/perl5/bin/perl -i.orig -p \
			$(FIX_PC_OPS) $(FIX_PC_FILES)" ; \
	    (cd $(SOURCE_DIR) && \
		/usr/perl5/bin/perl -i.orig -p \
			$(FIX_PC_OPS) $(FIX_PC_FILES) ); \
	fi
	touch $(BUILD_DIR)/.unpack_done


### Source tarballs & downloading if necessary

TARBALLDIR=$(TOP)/tarballs
WGET=/usr/sfw/bin/wget
SOURCE_TARBALL=$(TARBALLDIR)/$(SOURCE_TARBALL_NAME)

XORG_RELEASES_URL=http://xorg.freedesktop.org/releases/individual
SF_DOWNLOADS_URL=http://easynews.dl.sourceforge.net/sourceforge

# Set default download location if not set in module makefile
SOURCE_URL_DEFAULT	= $(SOURCE_URL_SET:yes=$(POUND_SIGN))
$(SOURCE_URL_DEFAULT)	SOURCE_URL=$(XORG_RELEASES_URL)/$(SOURCE_TARBALL_DIR)/$(SOURCE_TARBALL_NAME)

download:
	if [ ! -f $(SOURCE_TARBALL) ] ; then \
		$(WGET) -O $(SOURCE_TARBALL) $(SOURCE_URL) ;\
	fi

### Generate mapfiles to limit/version exported symbols in libraries

SPEC2MAP=       /usr/lib/abi/spec2map

SPECMAP=	mapfile.vers.$(LIBNAME)
SPECFILE=	spec/$(LIBNAME).spec 
VERSFILE=	spec/versions

$(BUILD_DIR_32)/$(SPECMAP): $(SPECFILE) $(VERSFILE)
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_32) MACH=$(MACH32) spec_gen

$(BUILD_DIR_64)/$(SPECMAP): $(SPECFILE) $(VERSFILE)
	$(MAKE) $(MFLAGS) BUILD_DIR=$(BUILD_DIR_64) MACH=$(MACH64) spec_gen

spec_gen:
	$(SPEC2MAP) -a $(MACH) -v $(VERSFILE) -p -l $(LIBNAME) \
	 -o $(BUILD_DIR)/$(SPECMAP) $(SPECFILE)

### Other tools needed to build

# Some modules (such as FreeType 2) require GNU make
GNUMAKE = gmake

$(GNUMAKE):
	@echo "Checking for GNU make:"
	$(GNUMAKE) --version

# Some modules use lndir to merge in sources from Sun specific directories
LNDIR=$(PROTODIR)/usr/X11/bin/lndir

$(LNDIR):
	(cd $(TOP)/util/lndir && $(MAKE) $(MFLAGS) install)

DOCBOOKUTIL_BINDIR=$(PROTODIR)$(TOOLS_DIR)/bin
DOCBOOK2HTML=$(DOCBOOKUTIL_BINDIR)/dockbook2html

$(DOCBOOK2HTML):
	(cd $(TOP)/doc/docbook-utils && $(MAKE) $(MFLAGS) install)

### Dependencies

# Almost everything depends on the headers in xproto
XPROTO_DEP=$(PROTODIR)/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig/xproto.pc

$(XPROTO_DEP):
	@echo "Installing dependency proto/X11"
	(cd $(TOP)/proto/X11 ; make install)

### Useful rules for maintaining patch sets

# regenerate patches against new baseline to reduce messages about hunks
# having fuzz or offsets when applying patches - new patches will be in new/

regen-patches: 
	CUR_LIST=""
	rm -rf tmp-regen
	make SOURCE_PATCHES="$(CUR_LIST)" BUILD_DIR=tmp-regen source_gen
	mkdir -p new
	for f in $(SOURCE_PATCHES) ; do \
	    p=`echo $$f | sed 's/,.*$$//'` ; \
	    echo '=================' $$p ; \
	    rm -rf prev ; \
	    mv tmp-regen prev ; \
	    $(MAKE) SOURCE_PATCHES="$${CUR_LIST} $$f" BUILD_DIR=tmp-regen source_gen ; \
	    awk '/^--- /	{exit} \
		 /^diff -u/	{exit} \
			 	{print}' $$p > new/$$p ; \
	    gdiff -urp -x '*~' -x '*.orig' ./prev ./tmp-regen \
		| sed -e 's% \./prev$(SOURCE_DIR)/% %' -e 's% \./tmp-regen$(SOURCE_DIR)/% %' \
		| grep -v '^Common subdirectories:' \
		| grep -v '^Only in ' \
		>> new/$$p ; \
	    CUR_LIST="$${CUR_LIST} new/$$p" ; \
	done
	rm -rf tmp-regen
