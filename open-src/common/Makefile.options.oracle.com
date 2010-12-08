# X build options for Oracle Solaris
#
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
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

# Settings for building X with Oracle Solaris branding and access to
# sites behind the Oracle firewall.   If you're not at Oracle, you 
# probably want to either use the generic Makefile.options.opensolaris
# or make a customized options file for your site/distro/build.

# Oracle internal mirror/archive of tarballs used to build
TARBALL_ARCHIVE = http://xserver.us.oracle.com/tarballs
TARBALL_ARCHIVE_SET=yes

# OS/Distro name for use in documentation
DISTRO_NAME = Oracle Solaris

# When packages contain only code to which Oracle owns the rights, Oracle 
# doesn't need to publish the open source license notice, and can just let
# the distro's top-level license notice apply.
LICENSE_CHOICE = Oracle
LICENSE_CHOICE_SET = yes

# When packages contain code covered by GPL/LGPL, Oracle prepends to the GPL
# a notice specifying the version Oracle is publishing the software under.
GPL_CHOICE_FILE = $(PKG_SRC_DIR)/license_files/gpl_choice_Oracle

# Xserver configuration options for vendor name & support URL
# The vendor name needs to include "X.Org Foundation" for software
# like cairo that does strstr on VendorName to detect servers built
# from the X.Org source tree for bug workarounds/compatibility tweaks.
VENDOR_NAME = Oracle Corporation, based on X.Org Foundation sources
VENDOR_SUPPORT_URL = http://support.oracle.com/

# Additional pkg transforms to set Oracle Solaris branding
PKG_BRANDING_TRANSFORMS = branding_Oracle
