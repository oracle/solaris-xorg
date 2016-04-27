###############################################################################
#
# intel-gpu-tools - Intel graphics driver debug tools
#
# Copyright (c) 2013, 2016, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=intel-gpu-tools

# Package metadata
MODULE_DESC = Intel graphics driver debug tools

# Dates to show in Oracle copyright notice in pkg license file
# Earliest & latest of the copyrights in the Oracle files in this pkg
ORACLE_COPYRIGHT_YEARS = 2013
ORACLE_TPNO = 13224
ORACLE_TPNO_SET = yes

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/diagnostic/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.3

# Checksums for upstream tarball
TARBALL_MD5   = 67facd6241e26e2c68614728e3a932e9
TARBALL_SHA1  = 35e4907c4dbc48ad37921e44d73f0fe15a989aa2
TARBALL_SHA256= 51d22fdb3d415a1b3b7d0a172c1bb24dec6f16116e80a9ce49873f44527f20a0

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --disable-nouveau

MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Cos of 23185711 - disable parfait temporarily
USE_PARFAIT=no

# Different programs need different libraries, but upstream links them all in
MODULE_LD_OPTIONS = $(ZDISCARD_UNUSED_DEP)

# Override default config environment - if PYTHON is set to a specific version
# then configure will complain it's version is not >= 3, but if it's unset it's
# fine with not finding a python version >= 3.
MODULE_CONFIG_ENV = PYTHON=""

# ARC cases that covered this module
# PSARC/2013/087
MODULE_ARC_CASES = PSARC/2013/087

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

include ../Makefile.inc
