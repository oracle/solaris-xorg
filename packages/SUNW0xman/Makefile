#
# Copyright (c) 2006, 2009, Oracle and/or its affiliates. All rights reserved.
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
#

SOURCEDIR=../../packages

PROTO_SOURCES = \
	$(SOURCEDIR)/SUNWxwman/prototype_com			\
	$(SOURCEDIR)/SUNWfontconfig-docs/prototype 		\
	$(SOURCEDIR)/SUNWpciaccess/prototype	 		\
	$(SOURCEDIR)/SUNWvncviewer/prototype	 		\
	$(SOURCEDIR)/SUNWxvnc/prototype_com	 		\
	$(SOURCEDIR)/SUNWxorg-graphics-ddx/prototype_com	\
	$(SOURCEDIR)/SUNWxorg-server/prototype_com		\
	$(SOURCEDIR)/SUNWxorg-client-docs/prototype		

# Don't include platform-specific man pages for now, since it will
# break builds on the other platform:
#	$(SOURCEDIR)/SUNWxorg-graphics-ddx/prototype_sparc	\
#	$(SOURCEDIR)/SUNWxorg-graphics-ddx/prototype_i386	\
#	$(SOURCEDIR)/SUNWxorg-server/prototype_i386		\

all: prototype

prototype: $(PROTO_SOURCES) $(SOURCEDIR)/SUNW0xman/make_l10n_man_proto.pl
	-rm -f $@
	perl $(SOURCEDIR)/SUNW0xman/make_l10n_man_proto.pl $(PROTO_SOURCES) > $@
