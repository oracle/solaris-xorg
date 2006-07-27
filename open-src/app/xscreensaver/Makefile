###############################################################################
#
# Xscreensaver Makefile
#
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.65	06/07/27
#

PWD:sh=pwd
TOP=$(PWD)/../..
BUILD_TYPES=32

# Version number (used in path names)
XSCREENSAVER_VERS=4.05

# Source tarball
SOURCE_TARBALL_NAME=xscreensaver-$(XSCREENSAVER_VERS).tar.gz

# Download site for source - master site no longer carries 4.05 so we use
# a mirror site until we upgrade to a newer version
#SOURCE_URL=http://www.jwz.org/xscreensaver/$(SOURCE_TARBALL_NAME)
SOURCE_URL=http://slackware.osuosl.org/unsupported/source/gnome-1.4.1/xscreensaver/$(SOURCE_TARBALL_NAME)
SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	solaris-suncc-fixes.patch \
	s_isdir.patch \
	Sun.app-defaults.patch \
	GNOME-desktop.patch \
	solaris-paths.patch \
	dont-bug-jwz.patch \
	gtk-lock.patch \
	allow-root.patch \
	scf-smartcard.patch \
	passwdTimeout-pref.patch \
	xinput.patch \
	i18n.patch \
	debug-msgs.patch \
	tooltips.patch \
	dpms.patch \
	IPv6.patch \
	gl-error-capture.patch \
	blurb.patch \
	accessibility.patch \
	misc.patch \
	trusted.patch \
        pam_audit.patch \
        bug-6450019.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/xscreensaver-$(XSCREENSAVER_VERS)

# Merge in additional sources from sun-src directory
LNDIR=$(PROTODIR)/usr/X11/bin/lndir

default: all

source_gen:: $(LNDIR)
	mkdir -p $(SOURCE_DIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../sun-src)
	if [ -d closed-src ] ; then \
	    (cd $(SOURCE_DIR) && $(LNDIR) ../../closed-src) \
	fi

$(LNDIR):
	(cd ../../util/lndir && $(MAKE) $(MFLAGS) install)


include $(TOP)/common/Makefile.inc

# Where to find OpenGL headers/libraries
OPENGL_DIR_sun4=/usr/openwin
OPENGL_DIR_i86pc=/usr/X11
OPENGL_DIR:sh=arch | sed 's/^\(.*\)$/\$\(OPENGL_DIR_\1\)/'

# Command line options to GNU autoconf configure script
XS_CFG=--prefix=/usr/openwin/ --enable-maintainer-mode --enable-gtk-doc --with-shadow --with-dpms --enable-locking --disable-screengrab --enable-dpms --with-gtk2=/usr --with-gl=$(OPENGL_DIR) --with-pixbuf=/usr  --with-pam=/usr --without-motif --with-jpeg=/usr/sfw --mandir='$${prefix}/share/man' --libexecdir='$${prefix}/lib/xscreensaver/bin' --datadir='$${prefix}/share' --with-hackdir=lib/xscreensaver/hacks --with-configdir=lib/xscreensaver/config/control-center-2.0 --with-gnome --with-scf-smartcard

# BINARY built in tree
XS_BIN=$(SOURCE_DIR)/xscreensaver

# Messages for translation
XS_POT=$(SOURCE_DIR)/po/xscreensaver.pot
POT_DEST=$(PROTODIR)/usr/openwin/share/locale/C/LC_MESSAGES

# Man pages
XS_MAN=$(SOURCE_DIR)/driver/xscreensaver.man.orig

# Include Paths
INCLUDES=-I/usr/X11/include

# Additional optimization flags, to make the hacks show off the hardware
# better and we can get away with optimizations not allowed in the core X code
#
# Temporarily removed -fsimple=2 to avoid bug 6453103 in Studio 11 compilers
XS_CFLAGS=$(CFLAGS) -nofstore -xprefetch $(XS_ARCH_FLAGS) -xstrconst
XS_ARCH_FLAGS:sh=arch | sed 's/^\(.*\)$/\$\(XS_\1_ARCH_FLAGS\)/'
XS_sun4_ARCH_FLAGS=-xtarget=ultra2

# Configure/Make variables to override
# Set PERL to /usr/perl5/bin so it uses that path to perl for scripts
# as required by Sun rules for using the bundled version of perl
# /opt/sfw/bin must be ahead of /usr/bin to make sure we get GNU xgettext
# since intltool-update doesn't work with Solaris xgettext (see bugs
# 4812320 & 4826523)
XS_CNFG_ENV=CC=$(CC) CFLAGS="$(XS_CFLAGS) $(INCLUDES)" CPPFLAGS="$(INCLUDES)" \
	PATH=/usr/perl5/bin:/usr/sfw/bin:/opt/sfw/bin:$(PATH) \
	GNOME_DATADIR='$${prefix}/lib/xscreensaver/config' \
	GLADE_DATADIR='$${prefix}/lib/xscreensaver/config' \
	LDFLAGS="$(PROG_LDFLAGS) -L/usr/X11/lib -R/usr/X11/lib" \
	PERL=/usr/perl5/bin/perl
XS_MAKE_ENV=install_prefix=$(PROTODIR) 

build_gen:   $(XS_BIN) $(XS_MAN) $(XS_POT)

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x autogen.sh ; \
	 $(XS_CNFG_ENV) ./autogen.sh $(XS_CFG) )

$(XS_BIN): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; chmod a+x install-sh ; \
	 $(MAKE) $(MFLAGS) -e $(XS_MAKE_ENV))

$(XS_POT): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR)/po ; \
	 PATH=/opt/sfw/bin:$(PATH) $(MAKE) -e $(XS_MAKE_ENV) generate_potfiles_in ; \
	 PATH=/opt/sfw/bin:$(PATH) $(MAKE) -e $(XS_MAKE_ENV) POTFILES ; \
	 PATH=/opt/sfw/bin:$(PATH) $(MAKE) -e $(XS_MAKE_ENV) xscreensaver.pot )

# Add Sun attributes section to man pages
$(XS_MAN):
	for f in $(SOURCE_DIR)/driver/*.man ; do \
	  if [ ! -f $$f.orig ] ; then \
	    mv $$f $$f.orig ; \
	  fi ; \
	  cat	$(TOP)/common/table-prepend \
		$$f.orig \
		$(TOP)/common/sunman-stability \
	    | sed 's/__package__/SUNWxscreensaver/g' > $$f ; \
	done

install_gen: $(XS_BIN) $(XS_MAN) $(XS_POT)
	chmod +w $(SOURCE_DIR)/hacks/vidwhacker \
		$(SOURCE_DIR)/hacks/webcollage \
		$(SOURCE_DIR)/driver/xscreensaver-getimage-file \
		$(SOURCE_DIR)/driver/xscreensaver-getimage-video
	(cd $(SOURCE_DIR) ; chmod a+x install-sh intltool-*; \
	 $(MAKE) -e $(XS_MAKE_ENV) install )
	mkdir -p $(POT_DEST)
	cp -p $(XS_POT) $(POT_DEST)/xscreensaver.pot

