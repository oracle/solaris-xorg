/*
 * Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
 */

/*
 * Common misc module interfaces of DRM under Solaris
 */

/*
 * I915 DRM Driver for Solaris
 *
 * This driver provides the hardware 3D acceleration support for Intel
 * integrated video devices (e.g. i8xx/i915/i945 series chipsets), under the
 * DRI (Direct Rendering Infrastructure). DRM (Direct Rendering Manager) here
 * means the kernel device driver in DRI.
 *
 * I915 driver is a device dependent driver only, it depends on a misc module
 * named drm for generic DRM operations.
 *
 * This driver also calls into gfx and agpmaster misc modules respectively for
 * generic graphics operations and AGP master device support.
 */

#ifndef	_SYS_DRM_SUNMOD_H_
#define	_SYS_DRM_SUNMOD_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/conf.h>
#include <sys/kmem.h>
#include <sys/visual_io.h>
#include <sys/fbio.h>
#include <sys/ddi.h>
#include <sys/sunddi.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/open.h>
#include <sys/modctl.h>
#ifdef __x86
#include <sys/font.h>
#include <sys/vgareg.h>
#include <sys/vgasubr.h>
#include <sys/kd.h>
#endif /* x86 */
#include <sys/pci.h>
#include <sys/ddi_impldefs.h>
#include <sys/sunldi.h>
#include <sys/mkdev.h>
#ifdef __x86
#include <gfx_private.h>
#include <sys/agpgart.h>
#include <sys/agp/agpdefs.h>
#include <sys/agp/agpmaster_io.h>
#endif /* x86 */
#include "drmP.h"
#include <sys/modctl.h>

/*
 * dev_t of this driver looks consists of:
 *
 * major number with NBITSMAJOR bits
 * instance node number with NBITSINST bits
 * minor node number with NBITSMINOR - NBITSINST bits
 *
 * Each instance has at most 2^(NBITSMINOR - NBITSINST) minor nodes, the first
 * three are:
 * 0: gfx<instance number>, graphics common node
 * 1: agpmaster<instance number>, agpmaster node
 * 2: drm<instance number>, drm node
 */
#define	GFX_MINOR		0
#define	AGPMASTER_MINOR		1
#define	DRM_MINOR		2
#define	DRM_MIN_CLONEMINOR	3

/*
 * Number of bits occupied by instance number in dev_t, currently maximum 8
 * instances are supported.
 */
#define	NBITSINST		3

/* Number of bits occupied in dev_t by minor node */
#define	NBITSMNODE		(18 - NBITSINST)

/*
 * DRM use a "cloning" minor node mechanism to release lock on every close(2),
 * thus there will be a minor node for every open(2) operation. Here we give
 * the maximum DRM cloning minor node number.
 */
#define	MAX_CLONE_MINOR		(1 << (NBITSMNODE) - 1)
#define	DEV2MINOR(dev)		(getminor(dev) & ((1 << (NBITSMNODE)) - 1))
#define	DEV2INST(dev)		(getminor(dev) >> NBITSMNODE)
#define	INST2NODE0(inst)	((inst) << NBITSMNODE)
#define	INST2NODE1(inst)	(((inst) << NBITSMNODE) + AGPMASTER_MINOR)
#define	INST2NODE2(inst)	(((inst) << NBITSMNODE) + DRM_MINOR)

/* graphics name for the common graphics minor node */
#define	GFX_NAME		"gfx"


/*
 * softstate for DRM module
 */
typedef struct drm_instance_state {
	kmutex_t		mis_lock;
	kmutex_t		dis_ctxlock;
	major_t			mis_major;
	dev_info_t		*mis_dip;
	drm_device_t		*mis_devp;
	ddi_acc_handle_t	mis_cfg_hdl;
#ifdef __x86
	agp_master_softc_t	*mis_agpm;	/* agpmaster softstate ptr */
	gfxp_vgatext_softc_ptr_t mis_gfxp;	/* gfx softstate */
#else
	void			*mis_agpm;	/* agpmaster softstate ptr */
	void			*mis_gfxp;	/* gfx softstate */
#endif /* x86 */
} drm_inst_state_t;


struct drm_inst_state_list {
	drm_inst_state_t	disl_state;
	struct drm_inst_state_list *disl_next;

};
typedef struct drm_inst_state_list drm_inst_list_t;

extern drm_inst_state_t *drm_sup_devt_to_state(dev_t);
extern drm_cminor_t *drm_find_minordev(drm_inst_state_t *mstate, dev_t dev);


#define	PCI_CONF_CAP_MASK		0x10
#define	PCI_CONF_CAPID_MASK		0xff
#define	PCI_CONF_NCAPID_MASK		0xff00

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_DRM_SUNMOD_H_ */
