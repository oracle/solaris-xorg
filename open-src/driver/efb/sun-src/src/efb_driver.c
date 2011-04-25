/*
 * Copyright (c) 2008, 2009, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Copyright 2000 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <stdio.h>

#include "radeon.h"
#include "radeon_probe.h"
#include "radeon_version.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "efb.h"
#include "fb.h"

#ifdef XSERVER_LIBPCIACCESS
#include "pciaccess.h"
#endif


/* Module loader interface for subsidiary driver module */

static XF86ModuleVersionInfo EFBVersionRec =
{
    RADEON_DRIVER_NAME,
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    RADEON_VERSION_MAJOR, RADEON_VERSION_MINOR, RADEON_VERSION_PATCH,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    {0, 0, 0, 0}
};

/*
 * EFBSetup --
 *
 * This function is called every time the module is loaded.
 */
static pointer
EFBSetup
(
    pointer Module,
    pointer Options,
    int     *ErrorMajor,
    int     *ErrorMinor
)
{
    static Bool Inited = FALSE;

    if (!Inited) {
        Inited = TRUE;
        xf86AddDriver(&RADEON, Module, HaveDriverFuncs);
    }

    return (pointer)TRUE;
}

/* The following record must be called efbModuleData */
_X_EXPORT XF86ModuleData efbModuleData =
{
    &EFBVersionRec,
    EFBSetup,
    NULL
};


#define EFB_REG_SIZE       256*1024
#define EFB_REG_SIZE_LOG2  18

#ifndef XSERVER_LIBPCIACCESS
pciVideoPtr EFBGetPciInfo(RADEONInfoPtr info)
{
    int status;
    pciVideoPtr pciInfo = NULL;
    int i;
    int cmd = GFX_IOCTL_GET_PCI_CONFIG;
    struct gfx_pci_cfg  pciCfg;
    int bar;

    if ((status = ioctl(info->fd, GFX_IOCTL_GET_PCI_CONFIG, &pciCfg))
		!= -1) {
        pciInfo = malloc(sizeof(pciVideoRec));

	pciInfo->vendor = pciCfg.VendorID;
	pciInfo->chipType = pciCfg.DeviceID;
	pciInfo->chipRev = pciCfg.RevisionID;
	pciInfo->subsysVendor = pciCfg.SubVendorID;
	pciInfo->subsysCard = pciCfg.SubSystemID;

	// TODO: need to fill in the complete structure
	//
	for (i = 0; i < 6; i++) {
	    bar = pciCfg.bar[i];
	    if (bar != 0) {
		if (bar & PCI_MAP_IO) {
		    pciInfo->ioBase[i] = (memType)PCIGETIO(bar);
		    pciInfo->type[i] = bar & PCI_MAP_IO_ATTR_MASK;

		    // TODO: size?
		} else {
		    pciInfo->type[i] = bar & PCI_MAP_MEMORY_ATTR_MASK;
		    pciInfo->memBase[i] = (memType)PCIGETMEMORY(bar);
		    if (PCI_MAP_IS64BITMEM(bar)) {
                        if (i == 5) {
                            pciInfo->memBase[i] = 0;
                        } else {
                            int  bar_hi = pciCfg.bar[i+1];
                            /* 64 bit architecture */
                            pciInfo->memBase[i] |=
                                        (memType)bar_hi << 32;
                            ++i;    /* Step over the next BAR */
                        }
		    }
		    pciInfo->size[i] = EFB_REG_SIZE_LOG2;
		}
	    }
	}
    }

    return pciInfo;
}
#else

/*
 * These defines are from the xorg 1.3 xf86pci.h
 */
#define PCI_MAP_MEMORY                  0x00000000
#define PCI_MAP_IO                      0x00000001

#define PCI_MAP_MEMORY_TYPE             0x00000007
#define PCI_MAP_MEMORY_TYPE_64BIT       0x00000004
#define PCI_MAP_IO_TYPE                 0x00000003

#define PCI_MAP_MEMORY_ADDRESS_MASK     0xfffffff0
#define PCI_MAP_IO_ADDRESS_MASK         0xfffffffc

#define PCI_MAP_IS_IO(b)        ((b) & PCI_MAP_IO)

#define PCI_MAP_IS64BITMEM(b)   \
        (((b) & PCI_MAP_MEMORY_TYPE) == PCI_MAP_MEMORY_TYPE_64BIT)

#define PCIGETMEMORY(b)         ((b) & PCI_MAP_MEMORY_ADDRESS_MASK)
#define PCIGETMEMORY64HIGH(b)   (*((CARD32*)&(b) + 1))
#define PCIGETMEMORY64(b)       \
        (PCIGETMEMORY(b) | ((CARD64)PCIGETMEMORY64HIGH(b) << 32))

#define PCIGETIO(b)             ((b) & PCI_MAP_IO_ADDRESS_MASK)


struct pci_device * EFBGetPciInfo(RADEONInfoPtr info)
{
    int status;
    struct pci_device *pciInfo = NULL;
    int i;
    int cmd = GFX_IOCTL_GET_PCI_CONFIG;
    struct gfx_pci_cfg  pciCfg;
    int bar;

    if ((status = ioctl(info->fd, GFX_IOCTL_GET_PCI_CONFIG, &pciCfg))
		!= -1) {
        pciInfo = malloc(sizeof(struct pci_device));

	pciInfo->vendor_id = pciCfg.VendorID;
	pciInfo->device_id = pciCfg.DeviceID;
	pciInfo->revision = pciCfg.RevisionID;
	pciInfo->subvendor_id = pciCfg.SubVendorID;
	pciInfo->subvendor_id = pciCfg.SubSystemID;

	// TODO: need to fill in the complete structure
	//
	for (i = 0; i < 6; i++) {
	    bar = pciCfg.bar[i];
	    if (bar != 0) {
		if (PCI_MAP_IS_IO(bar)) {
		    pciInfo->regions[i].base_addr = (pciaddr_t)PCIGETIO(bar);
		} else {
		    pciInfo->regions[i].size = EFB_REG_SIZE;
		    if (PCI_MAP_IS64BITMEM(bar)) {
		        pciInfo->regions[i].base_addr = (pciaddr_t)PCIGETMEMORY64(bar);
                        ++i;    /* Step over the next BAR */
		    } else {
		        pciInfo->regions[i].base_addr = (pciaddr_t)PCIGETMEMORY(bar);
		    }
		}
	    }
	}
    }

    return pciInfo;
}
#endif


pointer
EFBMapVidMem(ScrnInfoPtr pScrn, unsigned int flags, PCITAG picTag, 
			unsigned long base, unsigned long size)
{
    int BUS_BASE = 0;
    pointer memBase;
    int fdd;
    int mapflags = MAP_SHARED;
    int prot;
    memType realBase, alignOff;
    unsigned long realSize;
    int pageSize;
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    fdd = info->fd;
    realBase = base & ~(getpagesize() - 1);
    alignOff = base - realBase;

    pageSize = getpagesize();
    realSize = size + (pageSize - 1) & (~(pageSize - 1));

    printf("base: %lx, realBase: %lx, alignOff: %lx \n",
                base, realBase, alignOff);

    if (flags & VIDMEM_READONLY) {
        prot = PROT_READ;
    } else {
        prot = PROT_READ | PROT_WRITE;
    }

    memBase = mmap((caddr_t)0, realSize + alignOff, prot, mapflags, fdd,
                (off_t)(off_t)realBase + BUS_BASE);

    if (memBase == MAP_FAILED) {
        printf("RADEONMapVidMem: Could not map framebuffer\n");
        return NULL;
    }

    return ((char *)memBase + alignOff);
}

void
EFBUnmapVidMem(ScrnInfoPtr pScrn, pointer base, unsigned long size)
{
    memType alignOff = (memType)base - ((memType)base & ~(getpagesize() - 1));

    printf("alignment offset: %lx\n",alignOff);
    munmap((caddr_t)((memType)base - alignOff), (size + alignOff));

    return;
}


void
EFBNotifyModeChanged(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    int status;

    struct gfx_video_mode mode;

    if (pScrn->currentMode->name) {
	strlcpy(mode.mode_name, pScrn->currentMode->name, GFX_MAX_VMODE_LEN);
    } else {
	strlcpy(mode.mode_name, " ", GFX_MAX_VMODE_LEN);
    }
    
    mode.vRefresh = pScrn->currentMode->VRefresh;

    status = ioctl(info->fd, GFX_IOCTL_SET_VIDEO_MODE, &mode);
}

void
EFBScreenInit(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (xf86ReturnOptValBool(info->Options, OPTION_DISABLE_RANDR, FALSE)) {

   	xf86DrvMsg (pScrn->scrnIndex, X_INFO, "RANDR is disabled\n");

        //
        // disable RANDR
        // 
        EnableDisableExtension("RANDR", FALSE);
    }

    EFBNotifyModeChanged(pScrn);
}

void
EFBCloseScreen(ScrnInfoPtr pScrn)
{
    EFBNotifyModeChanged(pScrn);
}

DisplayModePtr
EFBOutputGetEDIDModes(xf86OutputPtr output)
{
    xf86MonPtr  edid_mon = output->MonInfo;

    if (!strcmp(edid_mon->vendor.name, "SUN")) {
	if (edid_mon->ver.version == 1 &&
		edid_mon->ver.revision <= 2) {

	    //
	    // force detail timing to be the preferred one
	    //
	    edid_mon->features.msc |= 0x2;
	}
    }

    return (xf86OutputGetEDIDModes(output));
}

DisplayModePtr
efb_get_modes(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    DisplayModePtr pModes;

    pModes = RADEONProbeOutputModes(output);

    //
    // display modes have precedence
    //
    if (pScrn->display->modes[0] != NULL) {
        int mm;
        char *modes;
        DisplayModePtr dModes, preferredMode = NULL;
        int found = 0, done = 0;
        for (mm = 0; pScrn->display->modes[mm] != NULL && !found; mm++) {
            modes = pScrn->display->modes[mm];
            if (!strcasecmp(modes, "Auto") || !strcasecmp(modes, "None")) {
		done = found = 1;
            } else {
                done = 0;
                dModes = pModes;
                while (dModes && !done) {
                    if (!strcmp(modes, dModes->name)) {
                        done = found = 1;
			dModes->type |= M_T_PREFERRED;
			preferredMode = dModes;
                    } else {
                        dModes = dModes->next;
                    }
                }
            }
        }

        if (found && preferredMode) {
	    dModes = pModes;
	    while (dModes) {
		if ((dModes->type & M_T_PREFERRED) &&
		        (dModes != preferredMode)) {
		    dModes->type &= ~M_T_PREFERRED;
		}
		dModes = dModes->next;
	    }
	}
    }

    return(pModes);
}

char *efb_default_mode = "1024x768";

float ratio[] = { 10/16,	/* 16:10 AR */
		  3/4,		/*  4:3  AR */
		  4/5,		/*  5:4  AR */
		  9/16		/* 16:9  AR */
		};

char *efb_get_edid_preferred_mode(ScrnInfoPtr pScrn, int head)
{
    RADEONInfoPtr      info = RADEONPTR(pScrn);
    int                i, status;
    unsigned int       length;
    gfx_edid_t         edid_buf;
    unsigned char      *data;
    unsigned char      *mode = "1280x1024";
    unsigned char      pmode[10];
    unsigned char      *vblk, *pblk;
    unsigned int       width = 0, height = 0;

    edid_buf.head = head;
    edid_buf.version = GFX_EDID_VERSION;
    status = ioctl(info->fd, GFX_IOCTL_GET_EDID_LENGTH, &edid_buf);
    if (status == -1) {
	return strdup(efb_default_mode);
    }

    data = edid_buf.data = (char *)malloc(edid_buf.length);
    status = ioctl(info->fd, GFX_IOCTL_GET_EDID, &edid_buf);
    if (status == -1) {
	return strdup(efb_default_mode);
    }

    vblk = data + 8 + 10; 			/* skip header & product info */

    pblk = vblk + 2 + 5 + 10 + 3 + 16;	/* skip to preferred timing block */

    for (i = 0; i < 4 && width == 0; i++, pblk+=18) {
	if (!((pblk[0] == 0) && (pblk[0] == 0))) {
            width = pblk[2] | ((pblk[4] & 0xf0) << 4);
            height = pblk[5] | ((pblk[7] & 0xf0) << 4);
	}
    }

    sprintf(pmode, "%dx%d", width, height);
    
    return strdup(pmode);
}

void efb_set_position(xf86CrtcConfigPtr config, BOOL swap, 
			int pos1X, int pos1Y, int pos2X, int pos2Y)
{
    int o;
    xf86OutputPtr output;
    char *value[2];
    char str[20];
    OptionInfoRec *p;

    int first = 0;
    int second = 1;

    if (swap) {
	first = 1;
	second = 0;
    }

    sprintf(str,"%d %d", pos1X, pos1Y);
    value[first] = strdup(str);
    sprintf(str,"%d %d", pos2X, pos2Y);
    value[second] = strdup(str);

    for (o = 0; o < 2; o++) {

        output = config->output[o];

	for (p = output->options; p->token >= 0; p++) {

	    if (strcmp(p->name, "Position") == 0) {
		if (!p->found) {
		    p->value.str = value[o];
		    p->found = TRUE;
		}
	    }
	} 
    }
}

void EFBPreInitOutputConfiguration(ScrnInfoPtr pScrn, xf86CrtcConfigPtr config)
{
    RADEONInfoPtr      info = RADEONPTR(pScrn);

    Bool doubleWide = FALSE;
    Bool doubleHigh = FALSE;
    Bool swap = FALSE;
    int  offset = 0, position = 0;
    int  head0 = GFX_EDID_HEAD_ONE;
    int  head1 = GFX_EDID_HEAD_TWO;
    char *str;

    str = xf86GetOptValString(info->Options, OPTION_DUAL_DISPLAY);
    if ((str != NULL) && (strcasecmp(str, "Enable") == 0)) {
	doubleWide = TRUE;
    }
    str = xf86GetOptValString(info->Options, OPTION_DUAL_DISPLAY_VERTICAL);
    if ((str != NULL) && (strcasecmp(str, "Enable") == 0)) {
	doubleHigh = TRUE;
    }

    // do the output configuration here only if dual display is enabled

    if ((doubleWide == FALSE) && (doubleHigh == FALSE)) {
	return;
    }

    if ((doubleWide == TRUE) || (doubleHigh == TRUE)) {
	char *outputOrder;

        outputOrder = xf86GetOptValString(info->Options, OPTION_OUTPUTS);
        if ((outputOrder != NULL) && (strcasecmp(outputOrder, "Swapped") == 0)) {
	    swap = TRUE;
	    head0 = GFX_EDID_HEAD_TWO;
            head1 = GFX_EDID_HEAD_ONE;
        }
    }

    if (pScrn->display->modes[0] != NULL) {
        int mm;
        char *modes;
        int found = 0, done = 0;
        for (mm = 0; pScrn->display->modes[mm] != NULL && !found; mm++) {
            modes = pScrn->display->modes[mm];
            if (!strcasecmp(modes, "Auto") || !strcasecmp(modes, "None")) {
		free(pScrn->display->modes[mm]);
		pScrn->display->modes[mm] = efb_get_edid_preferred_mode(pScrn, head0);
		found = 1;
	    }
	}
    } else {
	pScrn->display->modes = malloc(2*sizeof(char *));
        pScrn->display->modes[0] = efb_get_edid_preferred_mode(pScrn, head0);
	pScrn->display->modes[1] = NULL;
    }

    if ((doubleWide == TRUE) || (doubleHigh == TRUE)) {

	char *mode, *token;
	int  width, height, position;
        char value[10];
	xf86OutputPtr output;

	mode = strdup(pScrn->display->modes[0]);
	token = strtok(mode, "x");
	width = atoi(token);
	token = strtok(NULL, "x");
	height = atoi(token);

	free(mode);

	//
	// check for stream offset
	//
	if (doubleWide == TRUE) {
            xf86GetOptValInteger(info->Options, OPTION_STREAM_XOFFSET, &offset);

	    position = width;

	    if (offset != 0) {
		position += offset;
	    }

	    efb_set_position(config, swap, 0, 0, position, 0);

	} else {
            xf86GetOptValInteger(info->Options, OPTION_STREAM_YOFFSET, &offset);

	    position = height;

	    if (offset != 0) {
		position += offset;
	    }

	    efb_set_position(config, swap, 0, 0, 0, position);
	}

	//
	// if display virtual dimension is specified in xorg.conf, leave it alone
	//
        if (!pScrn->display->virtualX || !pScrn->display->virtualY) {
	    if (doubleWide) {
	    	pScrn->display->virtualX = width * 2 + offset;
		pScrn->display->virtualY = height;
	    } else if (doubleHigh) {
	    	pScrn->display->virtualX = width;
		pScrn->display->virtualY = height * 2 + offset;
	    }
	}

    }
}
