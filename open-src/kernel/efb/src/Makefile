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
#  This Makefile actually builds the efb framebuffer driver
#
###############################################################################

TOP:sh	= hg root

C_SRCS = ati_pcigart.c \
	drm_agpsupport.c \
	drm_auth.c \
	drm_bufs.c \
	drm_context.c \
	drm_dma.c \
	drm_drawable.c \
	drm_drv.c \
	drm_fops.c \
	drm_ioctl.c \
	drm_irq.c \
	drm_kstat.c \
	drm_lock.c \
	drm_memory.c \
	drm_msg.c \
	drm_pci.c \
	drm_scatter.c \
	drm_sunmod.c \
	efb_drv.c \
	efb_edid.c \
	efb_i2c.c \
	efb_map.c \
	efb_vis.c \
	r300_cmdbuf.c \
	radeon_cp.c \
	radeon_drv.c \
	radeon_irq.c \
	radeon_mem.c \
	radeon_state.c

MANPAGES	= efb.7d
MANSECTION	= man7d
MANSOURCE	= ../manpages

# Include kernel common rules
include $(TOP)/open-src/kernel/Makefile.rules

MODULE		= efb
OBJECTS		= $(C_SRCS:%.c=%.o)
LINTS		= $(OBJECTS:%.o=%.ln)

#
#	Define targets
#
ALL_TARGET	= $(BINARY)
LINT_TARGET	= $(MODULE).lint
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE)

#
# Lint errors that are not clean
#
LINTTAGS	+= -erroff=E_STATIC_UNUSED
LINTFLAGS	+= -erroff=E_BAD_PTR_CAST_ALIGN


#
#	Default build targets.
#
.KEEP_STATE_FILE: .make.state.$(MACH)
# .SUFFIXES:
# .DEFAULT:	$(MODULE)

all:		$(MODULE)

$(OBJECTS):
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
		$(CTFCONVERT) $(CTFCVTFLAGS) $@

$(MODULE):	$(OBJECTS)
		$(LD) -o $@ $(LDFLAGS) $(OBJECTS)
		$(CTFMERGE) $(CTFMERGE_LABEL) -o $@ $(OBJECTS)

def:		$(DEF_DEPS)

clean:		$(CLEAN_DEPS)
		$(RM) $(OBJECTS)

clobber:	clean
		$(RM) *.bc
		$(RM) $(MODULE)

lint:		$(LINT_DEPS)

modlintlib:	$(MODLINTLIB_DEPS)

clean.lint:	$(CLEAN_LINT_DEPS)

install:	$(INSTALL_DEPS)
		@print Installing $(MODULE) in $(PROTODIR)/$(PREFIX)
		@mkdir -p $(PROTODIR)/$(PREFIX)
		@$(INSTALL) -m 555 $(MODULE) $(PROTODIR)/$(PREFIX)
		@print Installing $(MANSOURCE)/$(MANPAGES) in $(PROTODIR)$(MANTARGET)
		@mkdir -p $(PROTODIR)$(ROOT_MAN_DIR)/$(MANSECTION)
		@$(INSTALL) -m 644 $(MANSOURCE)/$(MANPAGES) $(PROTODIR)$(MANTARGET)
