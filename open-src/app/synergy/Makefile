###############################################################################
#
# synergy 1.x Makefile
#
# Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=synergy-plus

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=desktop/synergy
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=Applications/System Utilities
MODULE_PKGCLASS_SET=yes

# Package metadata
UPSTREAM		= Synergy
UPSTREAM_URL		= http://synergy-foss.org/
UPSTREAM_SET		= yes

MODULE_LICENSE		= GPLv2
MODULE_LICENSE_SET	= yes
MODULE_LICENSE_PREPEND	= $(GPL_CHOICE_FILE)

# Version number (used in path names)
MODULE_VERSION=1.3.4

# Checksums for upstream tarball
TARBALL_MD5  = 2c565afe5f920d363eef38dd97449b73
TARBALL_SHA1 = 17b2dc2611974189ad0d54367b6c96b7932d9e32

# Where to install synergy files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Source tarball
SOURCE_TARBALL_NAME=synergy-plus-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes
SOURCE_UNCOMPRESS=gzcat
SOURCE_UNCOMPRESS_SET=yes

# Binaries built in tree
SYNERGYS_BIN=$(SOURCE_DIR)/cmd/synergys/synergys
SYNERGYC_BIN=$(SOURCE_DIR)/cmd/synergyc/synergyc

#SYNERGY_DOC=$(SOURCE_DIR)/doc/obselete	- They moved it to a directory called "obselete"

INSTALL_TARGETS=install_synergys install_synergyc 
# install_doc (Obselete)
INSTALL_TARGETS_SET=yes

SOURCE_URL=http://synergy-plus.googlecode.com/files/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = CConfig.cpp.patch

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed
USE_DEFAULT_CONFIG_ENV=no

MODULE_CXXFLAGS = -library=no%Cstd -library=Crun
MODULE_CXXFLAGS += -m32 $(OPTIM) -s -xspace
MODULE_CXXFLAGS += -features=except,rtti,export,extensions,nestedaccess,tmplife,tmplrefstatic
MODULE_CXXFLAGS += -instances=global -template=geninlinefuncs -verbose=template
MODULE_CXXFLAGS += -xlang=c99 -xbuiltin=%none
MODULE_CXXFLAGS += -Qoption ccfe ++boolflag:sunwcch=false
MODULE_CXXFLAGS += -Qoption ccfe -features=gcc
MODULE_CXXFLAGS += -Qoption ccfe -features=zla
MODULE_CXXFLAGS += -mt -D_REENTRANT -D__EXTENSIONS__ -D_LARGEFILE_SOURCE
MODULE_CXXFLAGS += -D_FILE_OFFSET_BITS=64
MODULE_CXXFLAGS += -D_XPG5 -D_XOPEN_SOURCE=500
MODULE_CXXFLAGS += -D_POSIX_PTHREAD_SEMANTICS
MODULE_CXXFLAGS += -DPIC -KPIC -DSYSAPI_UNIX=1 -DWINAPI_XWINDOWS=1
MODULE_CPPFLAGS = -DSYSAPI_UNIX=1 \
		  -DWINAPI_XWINDOWS=1 \
		  -I/usr/include/stdcxx4/ansi \
		  -I/usr/include/stdcxx4/tr1 \
		  -I/usr/include/stdcxx4 \
		  -I$(PROTODIR)$(X11_INCLUDES_DIR) \
		  -I$(PROTODIR)/usr/include \
		  -I$(PROTODIR)/usr/include/freetype2
MODULE_LDFLAGS = -L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR) \
		 -L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR) \
		 -R$(X11_DIR)/lib$(ARCHLIBSUBDIR) \
		 -KPIC -Bdynamic -lc -lstdcxx4 -lCrun -lm \
		 -lsocket -lnsl -lgen -z combreloc -z ignore
INSTALL = /usr/bin/ginstall -c
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes
LD_OPTIONS = -lc -lstdcxx4 -lCrun -lm -lsocket -lnsl -lgen -z combreloc -z ignore

MODULE_ENV = \
	     SHELL="/bin/ksh93" \
	     CONFIG_SHELL="/bin/ksh93" \
	     CC="$(CC)" \
	     CXX="$(CXX)" \
	     CXXFLAGS="$(CXXFLAGS) $(MODULE_CXXFLAGS)" \
	     CPPFLAGS="$(MODULE_CPPFLAGS)" \
	     CPP="$(CC) -E $(MODULE_CPPFLAGS) $(CPPFLAGS)" \
	     CXXCPP="$(CXX) -E $(MODULE_CPPFLAGS) $(CPPFLAGS) $(MODULE_CXXFLAGS) $(CXXFLAGS)" \
	     LDFLAGS="$(MODULE_LDFLAGS)" \
	     LD="$(CXX) $(CXXFLAGS) $(LDFLAGS)" \
	     CXXLD="$(CXX) $(MODULE_CXXFLAGS) $(CXXFLAGS) $(MODULE_LDFLAGS) $(LDFLAGS)" \
	     INSTALL="$(INSTALL)" \
	     MAKE=$(GNUMAKE) \
	     GNUMAKE=$(GNUMAKE) \
	     LD_OPTIONS="$(LD_OPTIONS)"

MODULE_CONFIG_OPTS = --disable-debug

MODULE_CONFIG_ENV = $(MODULE_ENV)
MODULE_BUILD_ENV = $(CONFIG_ENV)

include ../Makefile.inc

install_gen:: $(INSTALL_TARGETS)

DESTDIR=$(PROTODIR)

install_synergys: $(SYNERGYS_BIN)
	mkdir -p $(DESTDIR)$(MODULE_PREFIX)/bin
	cp -pf $(SYNERGYS_BIN) $(DESTDIR)$(MODULE_PREFIX)/bin/

install_synergyc: $(SYNERGYC_BIN)
	mkdir -p $(DESTDIR)$(MODULE_PREFIX)/bin
	cp -pf $(SYNERGYC_BIN) $(DESTDIR)$(MODULE_PREFIX)/bin/

#DOC_AREA=$(DESTDIR)$(MODULE_PREFIX)/share/doc/synergy

# Doc all in html format
install_doc: $(INSTALL_DEPS)
	mkdir -p 				$(DOC_AREA)
	mkdir -p 				$(DOC_AREA)/images
	cp -pf $(SYNERGY_DOC)/about.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/authors.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/autostart.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/banner.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/border.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/compiling.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/configuration.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/contact.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/developer.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/faq.html		$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/history.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/home.html		$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/index.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/license.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/news.html		$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/roadmap.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/running.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/security.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/tips.html		$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/toc.html		$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/trouble.html	$(DOC_AREA)
	cp -pf $(SYNERGY_DOC)/images/logo.gif	$(DOC_AREA)/images
	cp -pf $(SYNERGY_DOC)/images/warp.gif	$(DOC_AREA)/images
