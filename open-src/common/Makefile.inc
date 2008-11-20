# -*- Makefile -*- rules commonly shared among X consolidation open source dirs
#
# Copyright 2008 Sun Microsystems, Inc.  All Rights Reserved.
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
# @(#)Makefile.inc	1.95	08/11/18
#

### Default rules

# Which compiler to use to build module
MODTYPE_COMPILER_DEFAULT	= $(MODTYPE_COMPILER_SET:yes=$(POUND_SIGN))
$(MODTYPE_COMPILER_DEFAULT)	MODTYPE_COMPILER = $(DEFAULT_COMPILER)
MODULE_COMPILER_DEFAULT		= $(MODULE_COMPILER_SET:yes=$(POUND_SIGN))
$(MODULE_COMPILER_DEFAULT)	MODULE_COMPILER = $(MODTYPE_COMPILER)

# Which make to use to build module
MODTYPE_MAKE_DEFAULT		= $(MODTYPE_MAKE_SET:yes=$(POUND_SIGN))
$(MODTYPE_MAKE_DEFAULT)		MODTYPE_MAKE = $(MAKE)
MODULE_MAKE_DEFAULT		= $(MODULE_MAKE_SET:yes=$(POUND_SIGN))
$(MODULE_MAKE_DEFAULT)		MODULE_MAKE = $(MODTYPE_MAKE)

# Where to install files
MODTYPE_PREFIX_DEFAULT		= $(MODTYPE_PREFIX_SET:yes=$(POUND_SIGN))
$(MODULE_PREFIX_DEFAULT)	MODTYPE_PREFIX=$(X11_DIR)
MODULE_PREFIX_DEFAULT		= $(MODULE_PREFIX_SET:yes=$(POUND_SIGN))
$(MODULE_PREFIX_DEFAULT)	MODULE_PREFIX=$(MODTYPE_PREFIX)

# Directory created by unpacking source, if not set in module makefile
SOURCE_DIRNAME_DEFAULT		= $(SOURCE_DIRNAME_SET:yes=$(POUND_SIGN))
$(SOURCE_DIRNAME_DEFAULT)	SOURCE_DIRNAME=$(MODULE_NAME)-$(MODULE_VERSION)

SOURCE_DIR = $(BUILD_DIR)/$(SOURCE_DIRNAME)

# Set default tarball if not set in module makefile
SOURCE_TARBALL_NAME_DEFAULT	= $(SOURCE_TARBALL_NAME_SET:yes=$(POUND_SIGN))
$(SOURCE_TARBALL_NAME_DEFAULT)	SOURCE_TARBALL_NAME=$(SOURCE_DIRNAME).tar.bz2

# Tarballs for most modules built here are found in the X.Org archives/repos
# in a directory with the same name as what we use in our tree
SOURCE_TARBALL_DIR_DEFAULT	= $(SOURCE_TARBALL_DIR_SET:yes=$(POUND_SIGN))
$(SOURCE_TARBALL_DIR_DEFAULT)	SOURCE_TARBALL_DIR=$(MODTYPE)

# Set default download location if not set in module makefile
SOURCE_URL_DEFAULT		= $(SOURCE_URL_SET:yes=$(POUND_SIGN))
$(SOURCE_URL_DEFAULT)		SOURCE_URL=$(XORG_RELEASES_URL)/$(SOURCE_TARBALL_DIR)/$(SOURCE_TARBALL_NAME)

# Set default git repo location if not set in module makefile
GIT_URL_DEFAULT		= $(GIT_URL_SET:yes=$(POUND_SIGN))
$(GIT_URL_DEFAULT)	GIT_URL=$(XORG_GIT_URL)/$(SOURCE_TARBALL_DIR)/$(MODULE_NAME).git


BUILD_DIR_32 = build_32
BUILD_DIR_64 = build_64

# Set default build type to 32-bit if not set in module makefile
BUILD_TYPES_DEFAULT	= $(BUILD_TYPES_SET:yes=$(POUND_SIGN))
$(BUILD_TYPES_DEFAULT)	BUILD_TYPES=32

# Set default rules if not set in module/modtype Makefile
SOURCE_TARGETS_DEFAULT		= $(SOURCE_TARGETS_SET:yes=$(POUND_SIGN))
$(SOURCE_TARGETS_DEFAULT)	SOURCE_TARGETS=default_source

CONFIGURE_TARGETS_DEFAULT	= $(CONFIGURE_TARGETS_SET:yes=$(POUND_SIGN))
$(CONFIGURE_TARGETS_DEFAULT)	CONFIGURE_TARGETS=default_configure

BUILD_TARGETS_DEFAULT		= $(BUILD_TARGETS_SET:yes=$(POUND_SIGN))
$(BUILD_TARGETS_DEFAULT)	BUILD_TARGETS=default_build

INSTALL_TARGETS_DEFAULT		= $(INSTALL_TARGETS_SET:yes=$(POUND_SIGN))
$(INSTALL_TARGETS_DEFAULT)	INSTALL_TARGETS=default_install

# Additional flags to pass to make in 32-bit & 64-bit builds
BUILD_32_FLAGS= BUILD_DIR=$(BUILD_DIR_32) ARCH_FLAGS="$(ARCH32_FLAGS)" \
	$(MODTYPE_BUILD_32_FLAGS) $(MODULE_BUILD_32_FLAGS)

BUILD_64_FLAGS= BUILD_DIR=$(BUILD_DIR_64) ARCH_FLAGS="$(ARCH64_FLAGS)" \
	$(MODTYPE_BUILD_64_FLAGS) $(MODULE_BUILD_64_FLAGS)

all: $(BUILD_TYPES:%=source_%) $(BUILD_TYPES:%=build_%)
all_32: source_32 configure_32 build_32
all_64: source_64 configure_64 build_64

clean:: $(BUILD_TYPES:%=clean_%)

clean_32::
	-rm -rf $(BUILD_DIR_32)

clean_64::
	-rm -rf $(BUILD_DIR_64)

source: $(BUILD_TYPES:%=source_%)

source_gen:: $(SOURCE_TARGETS)

source_32: $(BUILD_DIR_32)/.unpack_done
$(BUILD_DIR_32)/.unpack_done:
	$(MAKE) $(MAKEFLAGS) BUILD_DIR=$(BUILD_DIR_32) source_gen

source_64: $(BUILD_DIR_64)/.unpack_done
$(BUILD_DIR_64)/.unpack_done:
	$(MAKE) $(MAKEFLAGS) BUILD_DIR=$(BUILD_DIR_64) source_gen

configure: $(BUILD_TYPES:%=configure_%)

configure_gen: $(CONFIGURE_TARGETS)

configure_32:: 
	$(MAKE) $(MAKEFLAGS) $(BUILD_32_FLAGS) configure_gen

configure_64::
	$(MAKE) $(MAKEFLAGS) LIBSUBDIR=/$(SUBDIR64) ARCHLIBSUBDIR=/$(SUBDIR64) $(BUILD_64_FLAGS) configure_gen

build: $(BUILD_TYPES:%=build_%)

build_gen: $(BUILD_TARGETS)

build_32::
	$(MAKE) $(MAKEFLAGS) $(BUILD_32_FLAGS) build_gen

build_64::
	$(MAKE) $(MAKEFLAGS) LIBSUBDIR=/$(SUBDIR64) ARCHLIBSUBDIR=/$(SUBDIR64) $(BUILD_64_FLAGS) build_gen

install: $(BUILD_TYPES:%=install_%)

install_gen:: $(INSTALL_TARGETS) install_license

install_32:: build_32
	$(MAKE) $(MAKEFLAGS) $(BUILD_32_FLAGS) install_gen

install_64:: build_64
	$(MAKE) $(MAKEFLAGS) LIBSUBDIR=/64 ARCHLIBSUBDIR=/$(SUBDIR64) $(BUILD_64_FLAGS) install_gen

DEBUG_BUILD_FLAGS = BUILD_DEBUG=yes

debug:
	$(MAKE) $(MAKEFLAGS) all $(DEBUG_BUILD_FLAGS)

debug_32:
	$(MAKE) $(MAKEFLAGS) all_32 $(DEBUG_BUILD_FLAGS) 

debug_64:
	$(MAKE) $(MAKEFLAGS) all_64 $(DEBUG_BUILD_FLAGS)

debug-install:
	$(MAKE) $(MAKEFLAGS) install $(DEBUG_BUILD_FLAGS)

debug-install_32:
	$(MAKE) $(MAKEFLAGS) install_32 $(DEBUG_BUILD_FLAGS)

debug-install_64:
	$(MAKE) $(MAKEFLAGS) install $(DEBUG_BUILD_FLAGS)


### Source tarballs & downloading if necessary

SOURCE_TARBALL=$(TARBALLDIR)/$(SOURCE_TARBALL_NAME)

GIT_CLONE_DIR=$(MODULE_NAME)-$(MODULE_VERSION)

# Download if SOURCE_TARBALL_NAME is not set to NONE and the file either
# doesn't exist or is empty
# If MODULE_VERSION is xgit, checkout source from git repo instead of
# a tarball
download:
	@case '$(MAKEFLAGS)' in *[ik]*) set +e;; esac; set -x ; \
	if [[ "$(MODULE_VERSION)" = "git" ]] ; then \
	    if [[ ! -d $(GIT_CLONE_DIR) ]] ; then \
		$(GIT) clone $(GIT_URL) $(GIT_CLONE_DIR) ; \
		if [[ ! -z "$(GIT_BRANCH)" ]] ; then \
		    cd $(GIT_CLONE_DIR) && $(GIT) checkout $(GIT_BRANCH) ; \
		fi ; \
	    fi ; \
	else \
	    if [[ "$(SOURCE_TARBALL_NAME)" != "NONE" ]] ; then \
		if [[ ! -s $(SOURCE_TARBALL) ]] ; then \
		    $(WGET) -O $(SOURCE_TARBALL) $(SOURCE_URL) ; \
		fi ; \
	    fi ; \
	fi

# Update a git cloned repo
git-update:
	if [[ "$(MODULE_VERSION)" = "git" ]] ; then \
	    if [[ -d $(GIT_CLONE_DIR) ]] ; then \
		cd $(GIT_CLONE_DIR) ; \
		if [[ ! -z "$(GIT_BRANCH)" ]] ; then \
		    $(GIT) checkout master && \
		    $(GIT) fetch && \
		    $(GIT) checkout $(GIT_BRANCH) ; \
		else \
		    $(GIT) fetch ; \
		fi ; \
	    fi ; \
	fi


### Unpack source and apply patches

FIX_PC_OPS= \
	-e 's|-L\$${libdir}|-L\$${libdir} -R\$${libdir}|;' \
	-e 's|Requires.private: .*$$||;'

#	-e 's|(Requires: .*)\bx11\b|$$1|;' \
#	-e 's|^prefix=@prefix@|prefix=\$$\{top_builddir\}@prefix@|'

SUNTOUCH_MAN_FLAGS = \
	-a '{Availability, $(SUN_PACKAGE)}' \
	-a '{Interface Stability, $(MODULE_STABILITY)}' \
	$(MODTYPE_SUNTOUCH_MAN_FLAGS) \
	$(MODULE_SUNTOUCH_MAN_FLAGS)

default_source: $(MODULE_SOURCE_DEPS) $(MODTYPE_SOURCE_DEPS)
	mkdir -p $(BUILD_DIR)
	@ if [[ "$(SOURCE_TARBALL_NAME)" != "NONE" ]] ; then \
	  if [[ "$(MODULE_VERSION)" = "git" ]] ; then \
	    SOURCE_CMD='gtar --exclude=.git* -cf - $(GIT_CLONE_DIR)' ; \
	  else \
	    UNCOMP_CMD="$(SOURCE_UNCOMPRESS)" ; \
	    SOURCE_CMD="$${UNCOMP_CMD:-bzcat} $(SOURCE_TARBALL)" ; \
	  fi ; \
	  print "$${SOURCE_CMD} | (cd $(BUILD_DIR) ; gtar -xf - )" ; \
	  $${SOURCE_CMD} | (cd $(BUILD_DIR) ; gtar -xf - ) ; \
	else \
	  print "mkdir -p $(SOURCE_DIR)" ; \
	  mkdir -p $(SOURCE_DIR) ; \
	fi ; \
	if [[ "$(ADDITIONAL_SOURCE_DIR)" != "" ]] ; then \
	  if [[ ! -x $(LNDIR) ]] ; then \
	    (cd $(TOP)/util/lndir && $(MAKE) $(MAKEFLAGS) install) ; \
	  fi ; \
	  print "cd $(SOURCE_DIR) && $(LNDIR) ../../$(ADDITIONAL_SOURCE_DIR)" ; \
	  (cd $(SOURCE_DIR) && $(LNDIR) ../../$(ADDITIONAL_SOURCE_DIR) ) ; \
	fi ; \
	if [[ "$(SOURCE_PATCHES)" != "" ]] ; then \
	    for p in $(SOURCE_PATCHES) x ; do \
	        if [[ "$$p" != "x" ]] ; then \
		    PATCH_CMD="$$(print $$p | awk -F, '{if (NF > 1) FLAGS = $$2; else FLAGS="-p0" ; printf "gpatch -d $(SOURCE_DIR) %s < %s\n", FLAGS, $$1}')" ; \
		    print "$${PATCH_CMD}" ; \
		    eval $${PATCH_CMD} ; \
		fi ; \
	    done ; \
	fi ; \
	if [[ "$(SUNTOUCHED_MANPAGES)" != "" ]] ; then \
	    print "/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
		$(SUNTOUCH_MAN_FLAGS) $(SUNTOUCHED_MANPAGES)" ; \
	    (cd $(SOURCE_DIR) && \
		/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
		$(SUNTOUCH_MAN_FLAGS) $(SUNTOUCHED_MANPAGES) ) ; \
	fi ; \
	if [[ "$(FIX_PC_FILES)" != "" ]] ; then \
	    print "/usr/perl5/bin/perl -i.orig -p \
			$(FIX_PC_OPS) $(FIX_PC_FILES)" ; \
	    (cd $(SOURCE_DIR) && \
		/usr/perl5/bin/perl -i.orig -p \
			$(FIX_PC_OPS) $(FIX_PC_FILES) ); \
	fi
	touch $(BUILD_DIR)/$(UNPACK_FILE)

### Run configure scripts

# Include Xorg autoconf macros when rebuilding configure scripts
ACLOCAL_PATH=aclocal -I $(PROTODIR)$(X11_ACLOCAL_DIR)

# Dependencies for running configure to make a Makefile
CONFIGURE_DEPS = $(UNPACK_TARGET) \
	$(MODTYPE_CONFIGURE_DEPS) $(MODULE_CONFIGURE_DEPS)

# Command line options to GNU autoconf configure script
DEFAULT_CONFIG_OPTS = --prefix=$(MODULE_PREFIX) \
	--mandir='$${prefix}/share/man' --localstatedir=/var \
	--disable-libtool-lock

CONFIG_OPTS = $(DEFAULT_CONFIG_OPTS) \
	$(MODTYPE_CONFIG_OPTS) $(MODULE_CONFIG_OPTS)

# Environment variable options to GNU autoconf configure script
$(USE_DEFAULT_CONFIG_CPPFLAGS:no=$(POUND_SIGN)) DEFAULT_CONFIG_CPPFLAGS= \
	-I$(PROTODIR)$(X11_INCLUDES_DIR) \
	-I$(PROTODIR)/usr/include -I$(PROTODIR)/usr/include/freetype2
$(USE_DEFAULT_CONFIG_LDFLAGS:no=$(POUND_SIGN)) DEFAULT_CONFIG_LDFLAGS= \
	-L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR) \
	-L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR)

DEFAULT_PKG_CONFIG_PATH=$(TOP)/common/pc-files/$(BUILD_DIR):$(PROTODIR)/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig:/usr/lib$(ARCHLIBSUBDIR)/pkgconfig

# Set USE_DEFAULT_CONFIG_ENV=no in a Makefile to not use this default
# autoconfig environment
$(USE_DEFAULT_CONFIG_ENV:no=$(POUND_SIGN))	DEFAULT_CONFIG_ENV = \
    CC="$(CC)" CXX="$(CXX)" \
    LD="$(CC) $(LDFLAGS)" \
    CFLAGS="$(MODTYPE_CFLAGS) $(MODULE_CFLAGS)" \
    CXXFLAGS="$(MODTYPE_CXXFLAGS) $(MODULE_CXXFLAGS)" \
    CPPFLAGS="$(DEFAULT_CONFIG_CPPFLAGS) $(MODTYPE_CPPFLAGS) $(MODULE_CPPFLAGS)" \
    LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(MODTYPE_CFLAGS) $(MODTYPE_LDFLAGS) $(MODULE_CFLAGS) $(MODULE_LDFLAGS)" \
    PKG_CONFIG_PATH="$(DEFAULT_PKG_CONFIG_PATH)" \
    INSTALL="$(INSTALL_SCRIPT)" CPPROG="cp -p" \
    MAKE="$(MODULE_MAKE)" CONFIG_SHELL="$(SHELL)"

CONFIG_ENV = $(DEFAULT_CONFIG_ENV) $(MODTYPE_CONFIG_ENV) $(MODULE_CONFIG_ENV)

# File created by running configure - defaults to the top-level Makefile
# in a source directory unless AUTOCONF_TARGET_SET=yes is set
AUTOCONF_TARGET_DEFAULT = $(AUTOCONF_TARGET_SET:yes=$(POUND_SIGN))
$(AUTOCONF_TARGET_DEFAULT)	AUTOCONF_TARGET=$(SOURCE_DIR)/Makefile

$(AUTOCONF_TARGET): $(CONFIGURE_DEPS)
	cd $(SOURCE_DIR) ; \
	if [[ "$(AUTORECONF)" = "yes" ]] ; then \
	     ACLOCAL="$(ACLOCAL_PATH)" autoreconf -v --install --force ; \
	fi ; \
	chmod a+x configure ; \
	$(CONFIG_ENV) $(SHELL) ./configure $(CONFIG_OPTS) ; \
	if [[ "$(DELIBTOOLIZE)" = "yes" ]] ; then \
	    $(TOP)/common/delibtoolize.pl $(DELIBTOOLIZE_FLAGS) . ; \
	fi

default_configure:: $(AUTOCONF_TARGET)


### Remove libtool from build process when needed

delibtoolize:
	$(TOP)/common/delibtoolize.pl $(DELIBTOOLIZE_FLAGS) $(SOURCE_DIR)


### Generate mapfiles to limit/version exported symbols in libraries

SPEC2MAP=       /usr/lib/abi/spec2map

SPECMAP=	mapfile.vers.$(LIBNAME)
SPECFILE=	spec/$(LIBNAME).spec 
VERSFILE=	spec/versions

$(BUILD_DIR_32)/$(SPECMAP): $(SPECFILE) $(VERSFILE)
	$(MAKE) $(MAKEFLAGS) BUILD_DIR=$(BUILD_DIR_32) MACH=$(MACH32) spec_gen

$(BUILD_DIR_64)/$(SPECMAP): $(SPECFILE) $(VERSFILE)
	$(MAKE) $(MAKEFLAGS) BUILD_DIR=$(BUILD_DIR_64) MACH=$(MACH64) spec_gen

spec_gen:
	$(SPEC2MAP) -a $(MACH) -v $(VERSFILE) -p -l $(LIBNAME) \
	 -o $(BUILD_DIR)/$(SPECMAP) $(SPECFILE)

### Default rules for common build pattern

LD_OPTIONS = -L$(PROTODIR)$(X11_DIR)/lib$(LIBSUBDIR) \
	$(MODTYPE_LD_OPTIONS) $(MODULE_LD_OPTIONS)

BUILD_ENV = LD_OPTIONS='$(LD_OPTIONS)' \
	$(MODTYPE_BUILD_ENV) $(MODULE_BUILD_ENV)

DEFAULT_BUILD_MAKEFLAGS=$(MODTYPE_MAKEFLAGS) $(MODULE_MAKEFLAGS) \
	$(MODTYPE_BUILD_MAKEFLAGS) $(MODULE_BUILD_MAKEFLAGS)

default_build:: $(UNPACK_TARGET) $(CONFIGURE_TARGETS) $(MODTYPE_BUILD_DEPS) $(MODULE_BUILD_DEPS) $(MODULE_MAKE)
	(cd $(SOURCE_DIR) ; \
	 $(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS))

print_default_build_flags:
	@print "BUILD_ENV='$(BUILD_ENV)'" ; \
	print "MAKE='$(MODULE_MAKE)'" ; \
	print "MAKE_FLAGS='$(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS)'"

build-in-subdir:
	(cd $(subdir) ; \
	 $(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS) \
	 $(subdir_cmd) )

DEFAULT_INSTALL_MAKEFLAGS= -e DESTDIR=$(PROTODIR) \
	pkgconfigdir=/usr/lib$(ARCHLIBSUBDIR)/pkgconfig \
	$(MODTYPE_MAKEFLAGS) $(MODULE_MAKEFLAGS) \
	$(MODTYPE_INSTALL_MAKEFLAGS) $(MODULE_INSTALL_MAKEFLAGS) \

default_install:: $(BUILD_TARGETS)
	(cd $(SOURCE_DIR) ; \
	 $(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS) \
	 install)

print_default_install_flags:
	@print "BUILD_ENV='$(BUILD_ENV)'" ; \
	print "MAKE='$(MODULE_MAKE)'" ; \
	print "MAKE_FLAGS='$(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS)'"

install-in-subdir: 
	(cd $(subdir) ; \
	 $(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS) \
	 $(subdir_cmd))


### Other tools/dependencies needed to build

# Some modules (such as FreeType 2) require GNU make
$(GNUMAKE):
	@print -n "Building with GNU make from $$(whence gmake): "
	@$(GNUMAKE) --version

$(MAKE):
	@print "Building with make from $$(whence $(MAKE))"

# Some modules use lndir to merge in sources from Sun specific directories
$(LNDIR):
	(cd $(TOP)/util/lndir && $(MAKE) $(MAKEFLAGS) install)

# Some modules use these tools to convert DocBook docs to text or html
$(DOCBOOK2HTML) $(DOCBOOK2TEXT):
	cd $(TOP)/doc/docbook-utils && $(MAKE) $(MAKEFLAGS) install

$(XORG_DEFS_ENT):
	cd $(TOP)/doc/xorg-sgml-doctools && $(MAKE) $(MAKEFLAGS) install

# Almost everything depends on the headers in xproto
$(XPROTO_DEP):
	@print "$(XPROTO_DEP) not found..."
	@print "Installing dependency proto/x11proto"
	(cd $(TOP)/proto/x11proto ; make install)

### Useful rules for maintaining patch sets

# regenerate patches against new baseline to reduce messages about hunks
# having fuzz or offsets when applying patches - new patches will be in new/

regen-patches: 
	CUR_LIST=""
	rm -rf tmp-regen
	make SOURCE_PATCHES="$(CUR_LIST)" BUILD_DIR=tmp-regen source_gen
	mkdir -p new
	for f in $(SOURCE_PATCHES) ; do \
	    p=$$(print $$f | sed 's/,.*$$//') ; \
	    print '=================' $$p ; \
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

### Licensing files

# Default license file name is COPYING (X.Org & GNU autoconf default)
# For other license file names, set LICENSE_FILE to the file name
# and set LICENSE_FILE_SET=yes
# A file in the same directory as the Makefile takes precedence
# over one that is delivered in the package.
# For additional license files, set EXTRA_LICENSE_FILES to a list of
# licenses to copy
LICENSE_FILE_DEFAULT	= $(LICENSE_FILE_SET:yes=$(POUND_SIGN))
$(LICENSE_FILE_DEFAULT)	LICENSE_FILE=COPYING

install_license:
	@ THIS_MODULE="$$(echo $(PWD) | sed 's|.*/open-src/||')" ; \
	PS4="" ; set -x ; \
	if [[ -f $(LICENSE_FILE) ]] ; then \
	    LICENSE_TO_COPY="$(LICENSE_FILE)" ; \
	elif [[ -f $(SOURCE_DIR)/$(LICENSE_FILE) ]] ; then \
	    LICENSE_TO_COPY="$(SOURCE_DIR)/$(LICENSE_FILE)" ; \
	fi ; \
	if [[ "$${LICENSE_TO_COPY}" != "" ]] ; then \
	    mkdir -p $(LICENSE_DEST)/$${THIS_MODULE}/ ; \
	    cp -pf $${LICENSE_TO_COPY} \
		$(LICENSE_DEST)/$${THIS_MODULE}/COPYING ; \
	fi ; \
	if [[ "$(EXTRA_LICENSES)" != "" ]] ; then \
	    mkdir -p $(LICENSE_DEST)/$${THIS_MODULE}/ ; \
	    cp -pf $(EXTRA_LICENSES) $(LICENSE_DEST)/$${THIS_MODULE} ; \
	fi
