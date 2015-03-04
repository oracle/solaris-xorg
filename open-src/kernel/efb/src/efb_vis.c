/*
 * Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
 */

#include <sys/visual_io.h>

#if VIS_CONS_REV > 2

#include "efb.h"

/*
 * Internal functions
 */
static int	efb_map_dfb(drm_device_t *);
static void	efb_termemu_display(efb_private_t *,
		    struct efb_vis_draw_data *);
static void	efb_termemu_copy(efb_private_t *, struct vis_conscopy *);
static void	efb_termemu_cursor(efb_private_t *, struct vis_conscursor *);
static int	efb_chk_disp_params(efb_private_t *, struct vis_consdisplay *);
static int	efb_chk_copy_params(efb_private_t *, struct vis_conscopy *);
static int	efb_chk_cursor_params(efb_private_t *, struct vis_conscursor *);
static void	efb_polled_conscursor(struct vis_polledio_arg *,
		    struct vis_conscursor *);
static void	efb_polled_consdisplay(struct vis_polledio_arg *,
		    struct vis_consdisplay *);
static void	efb_polled_conscopy(struct vis_polledio_arg *,
		    struct vis_conscopy *);
static int	efb_invalidate_userctx(efb_private_t *);
static void	efb_restore_kcmap(efb_private_t *);
static void	efb_setup_cmap32(efb_private_t *);
static void	efb_polled_check_power(efb_private_t *);

/*
 * All functions called in polled I/O mode should not have lock
 * ASSERT, for lock is not available in polled I/O mode. Each
 * time polled routines (e.g. efb_polled_consdisplay) are called,
 * efb_in_polledio is set and then PASSERT will just simply succeed.
 */
int	volatile efb_in_polledio = 0;
static	void	 efb_polledio_enter(void);
static	void	 efb_polledio_exit(void);

#define	DFB32ADR(efb_priv, row, col) \
	((((uint32_t *)efb_priv->consinfo.dfb +	\
	efb_priv->consinfo.offset)) +		\
	((row) * efb_priv->consinfo.pitch + (col)))

#define	DFB8ADR(efb_priv, row, col) \
	(((uint8_t *)efb_priv->consinfo.dfb) + \
	efb_priv->consinfo.offset + \
	((row) * efb_priv->consinfo.pitch + (col)))


int
efb_map_dfb(drm_device_t *statep)
{
	volatile caddr_t registers;
	uint32_t offset;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	static	struct ddi_device_acc_attr fbMem = {
		DDI_DEVICE_ATTR_V0,
		DDI_STRUCTURE_BE_ACC,
		DDI_STRICTORDER_ACC
	};

	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);
	registers = efb_priv->registers;

	if (efb_priv->consinfo.stream == 0)
		offset	 = regr(CRTC_OFFSET);
	else
		offset	 = regr(CRTC2_OFFSET);

	efb_priv->consinfo.offset = offset;

	if (ddi_regs_map_setup(statep->dip, 1,
	    (caddr_t *)&efb_priv->consinfo.dfb,
	    0, 0x8000000, &fbMem,
	    (ddi_acc_handle_t *)&efb_priv->consinfo.dfb_handle)) {
		cmn_err(CE_WARN, "efb_vis_devinit:  efb Unable to map dfb");
		return (DDI_FAILURE);
	}

	return (DDI_SUCCESS);
}

extern int
efb_vis_devinit(drm_device_t *statep, caddr_t data, int flag)
{
	struct vis_devinit devinit = { 0 };
	volatile caddr_t registers;
	int i;
	drm_device_t *dev = statep;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	if (!(flag & FKIOCTL)) {
		return (EPERM);
	}

	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);
	registers = efb_priv->registers;

	/*
	 * Read the terminal emulator's change mode callback
	 * address out of the incoming structure
	 */
	if (ddi_copyin(data, &devinit, sizeof (struct vis_devinit), flag)) {
		return (EFAULT);
	}


	DRM_LOCK();

	/*
	 * Allocate memory needed by the the kernel console
	 */

	if (efb_priv->consinfo.bufp == NULL) {
		efb_priv->consinfo.bufsize = DEFCHAR_SIZE * EFB_MAX_PIXBYTES;
		efb_priv->consinfo.bufp =
		    kmem_zalloc(efb_priv->consinfo.bufsize, KM_SLEEP);
	}

	if (efb_priv->consinfo.polledio == NULL) {
		efb_priv->consinfo.polledio =
		    kmem_zalloc(sizeof (struct vis_polledio), KM_SLEEP);

		efb_priv->consinfo.polledio->arg =
		    (struct vis_polledio_arg *)efb_priv;
	}

	if (efb_priv->consinfo.csrp == NULL)
		efb_priv->consinfo.csrp =
		    kmem_alloc(CSRMAX * sizeof (uint32_t), KM_SLEEP);

	/*
	 * Determine which video stream the console will render on.
	 */
	switch (efb_priv->primary_stream) {
	case 0: /* PROM version 1.10 or earlier uses 0 to mean stream 1 */
	case 1:
		efb_priv->consinfo.stream = 0;
		break;
	case 2:
		efb_priv->consinfo.stream = 1;
		break;
	}


	/*
	 * Extract the terminal emulator's video mode change notification
	 * callback information from the incoming struct
	 */

	efb_priv->consinfo.te_modechg_cb = devinit.modechg_cb;
	efb_priv->consinfo.te_ctx  = devinit.modechg_arg;

	efb_getsize(efb_priv);

	/*
	 * Describe this driver's configuration for the caller.
	 */

	devinit.version		= VIS_CONS_REV;
	devinit.width		= efb_priv->w[efb_priv->consinfo.stream];
	devinit.height		= efb_priv->h[efb_priv->consinfo.stream];
	devinit.linebytes	= devinit.width *
	    efb_priv->depth[efb_priv->consinfo.stream] / 8;
	devinit.depth		= efb_priv->depth[efb_priv->consinfo.stream];
	/* color_map ? */
	devinit.mode		= VIS_PIXEL;
	devinit.polledio	= efb_priv->consinfo.polledio;


	/*
	 * Setup the standalone access (polled mode) entry points
	 * which are also passed back to the terminal emulator.
	 */

	efb_priv->consinfo.polledio->display = efb_polled_consdisplay;
	efb_priv->consinfo.polledio->copy    = efb_polled_conscopy;
	efb_priv->consinfo.polledio->cursor  = efb_polled_conscursor;

	/*
	 * Get our view of the console as scribbling pad of memory.
	 * (dumb framebuffer)
	 */
	if (efb_map_dfb(statep) != DDI_SUCCESS) {
		DRM_UNLOCK();
		return (ENOMEM);
	}

	/*
	 * Calculate the FULL length of a horizontal line of pixel
	 * memory including both the visible portion and the portion
	 * that extends past the visible boundary on the right.
	 * (The invisible section is where any rounding excess
	 * goes to accomodate to the 64 or 256 byte rounding
	 * requirements of certain device functional block
	 * components).
	 */

	if (efb_priv->consinfo.stream == 0)
		efb_priv->consinfo.pitch = regr(CRTC_PITCH) * 8;
	else
		efb_priv->consinfo.pitch = regr(CRTC2_PITCH) * 8;

	/*
	 * Clear colormap update flags so we can do lazy loading
	 */
	for (i = 0; i < EFB_CMAP_ENTRIES; i++) {
		efb_priv->cmap_flags[0][i] = 0;
		efb_priv->cmap_flags[1][i] = 0;
	}

	/*
	 * TrueColor 24 _requires_ prepping the DAC properly, although it
	 * isn't documented in the Radeon PDF files (there is a reference
	 * in the Mach64 documents, however).
	 */

	if (devinit.depth == 32)
		efb_setup_cmap32(efb_priv);


	DRM_UNLOCK();

	/*
	 * Send framebuffer kernel console rendering parameters back to the
	 * terminal emulator
	 */

	if (ddi_copyout(&devinit, data, sizeof (struct vis_devinit), flag))
		return (EFAULT);

	return (DDI_SUCCESS);
}

/*
 * This calls back into the terminal emulator to inform it of
 * a driver mode change.
 */
extern void
efb_termemu_callback(drm_device_t *statep)
{
	struct vis_devinit devinit = { 0 };
	volatile caddr_t registers;
	int npixels = 8;
	int surface_cntl;

	int validated = 0;
	int ntry = 0;

	drm_device_t *dev = statep;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);

	registers = efb_priv->registers;

	DRM_LOCK();

	efb_getsize(efb_priv);

	/* make sure we have the right stream */
	while (!validated && ntry < 2) {

		if (efb_priv->consinfo.stream == 0) {
			efb_priv->consinfo.pitch =
			    (regr(CRTC_PITCH) & CRTC_PITCH__CRTC_PITCH_MASK)
			    * npixels;
			efb_priv->consinfo.offset = regr(CRTC_OFFSET);

		} else {
			efb_priv->consinfo.pitch =
			    (regr(CRTC2_PITCH) & CRTC_PITCH__CRTC_PITCH_MASK)
			    * npixels;
			efb_priv->consinfo.offset = regr(CRTC2_OFFSET);
		}

		if (efb_priv->consinfo.pitch == 0) {
			efb_priv->consinfo.stream =
			    (~efb_priv->consinfo.stream) & 0x1;
			ntry++;
		} else {
			validated = 1;
		}
	}

	switch (efb_priv->depth[efb_priv->consinfo.stream]) {
	case 8:
		efb_priv->consinfo.bgcolor = 0;
		break;
	case 32:
		efb_setup_cmap32(efb_priv);
		efb_priv->consinfo.bgcolor = 0xffffffff;
		break;
	}

	efb_priv->setting_videomode = 0;

	surface_cntl = regr(RADEON_SURFACE_CNTL);
	if (surface_cntl &
	    (RADEON_NONSURF_AP0_SWP_BIG32 | RADEON_NONSURF_AP1_SWP_BIG32)) {

		/* 00rrggbb */

		efb_priv->consinfo.rshift = 16;
		efb_priv->consinfo.gshift = 8;
		efb_priv->consinfo.bshift = 0;
	} else {

		/* bbggrr00 */

		efb_priv->consinfo.rshift = 8;
		efb_priv->consinfo.gshift = 16;
		efb_priv->consinfo.bshift = 24;
	}

	DRM_UNLOCK();

	if (efb_priv->consinfo.te_modechg_cb != NULL) {

		DRM_LOCK();

		devinit.version	 = VIS_CONS_REV;
		devinit.mode	 = VIS_PIXEL;
		devinit.polledio = efb_priv->consinfo.polledio;
		devinit.width	 = efb_priv->w[efb_priv->consinfo.stream];
		devinit.height	 = efb_priv->h[efb_priv->consinfo.stream];
		devinit.depth	 = efb_priv->depth[efb_priv->consinfo.stream];
		devinit.linebytes = devinit.width *
		    efb_priv->depth[efb_priv->consinfo.stream] / 8;

		DRM_UNLOCK();

		efb_priv->consinfo.te_modechg_cb(efb_priv->consinfo.te_ctx,
		    &devinit);
	}
}

extern int
efb_vis_devfini(drm_device_t *statep, caddr_t data, int flag)
{
	_NOTE(ARGUNUSED(data))

	drm_device_t *dev = statep;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	if (!(flag & FKIOCTL))
		return (EPERM);

	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);

	DRM_LOCK();

	efb_priv->consinfo.polledio->display = NULL;
	efb_priv->consinfo.polledio->copy    = NULL;
	efb_priv->consinfo.polledio->cursor  = NULL;

	efb_priv->consinfo.te_modechg_cb = NULL;
	efb_priv->consinfo.te_ctx  = NULL;

	if (efb_priv->consinfo.polledio != NULL) {
		kmem_free(efb_priv->consinfo.polledio,
		    sizeof (struct vis_polledio));

		efb_priv->consinfo.polledio = NULL;
	}

	if (efb_priv->consinfo.bufp != NULL) {
		kmem_free(efb_priv->consinfo.bufp,
		    efb_priv->consinfo.bufsize);

		efb_priv->consinfo.bufsize = 0;
		efb_priv->consinfo.bufp = NULL;
	}

	if (efb_priv->consinfo.csrp != NULL) {
		kmem_free(efb_priv->consinfo.csrp,
		    CSRMAX * sizeof (uint32_t));
		efb_priv->consinfo.csrp = NULL;
	}

	efb_priv->consinfo.kcmap_max = 0;

	DRM_UNLOCK();

	return (DDI_SUCCESS);
}


/*
 * colormap manipulation entry points -------------------------------------
 *
 * For 8-bit a'lazy loading' scheme is used so that only restore cmap entries
 * that have been flagged as in use by the terminal emulator.  The highest
 * ordered cmap entry that has been accessed since console's VIS_DEVINIT
 * ioctl was invoked is tracked to optimize colormap restoration for speed.
 *
 * For 32-bit depth the DAC must be initialized for linear TrueColor 24 before
 * the colors will work properly.
 */


/*
 * Initialize the cmap for the current fb depth
 */
static void
efb_setup_cmap32(efb_private_t *efb_priv)
{
	int stream = efb_priv->consinfo.stream;
	int i;

	for (i = 0; i < EFB_CMAP_ENTRIES; i++) {
		efb_priv->colormap[stream][0][i] =
		    efb_priv->colormap[stream][1][i] =
		    efb_priv->colormap[stream][2][i] = (i << 2) | (i >> 6);
		efb_priv->cmap_flags[stream][i] = 1;
	}
	efb_cmap_write(efb_priv, stream);
}


/*
 * This function is called to save a copy of the kernel terminal emulator's c
 * color map when a VIS_PUTCMAP ioctl is issued to the driver.
 */
extern int
efb_vis_putcmap(drm_device_t *statep, caddr_t data, int flag)
{
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	struct vis_cmap *cmap = (struct vis_cmap *)data;
	int stream;
	int i;

	if (!(flag & FKIOCTL))
		return (EPERM);

	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);

	stream = efb_priv->consinfo.stream;

	for (i = 0; i < cmap->count; ++i) {

		efb_priv->consinfo.kcmap[stream][0][cmap->index + i] =
		    (cmap->red[i] << 2) | (cmap->red[i] >> 6);

		efb_priv->consinfo.kcmap[stream][1][cmap->index + i] =
		    (cmap->green[i] << 2) | (cmap->green[i] >> 6);

		efb_priv->consinfo.kcmap[stream][2][cmap->index + i] =
		    (cmap->blue[i] << 2) | (cmap->blue[i] >> 6);
	}

	return (DDI_SUCCESS);
}

/*
 * Re-establish the kernel's 8-bit color map.
 */
static void
efb_restore_kcmap(efb_private_t *efb_priv)
{
	int stream, save;
	int i;

	stream = efb_priv->consinfo.stream;
	save = stream + 2;

	switch (efb_priv->depth[stream]) {
	case 8:
		for (i = 0; i <= efb_priv->consinfo.kcmap_max; i++) {

			if (efb_priv->consinfo.kcmap_flags[stream][i]) {

				efb_priv->colormap[save][0][i] =
				    efb_priv->colormap[stream][0][i];

				efb_priv->colormap[save][1][i] =
				    efb_priv->colormap[stream][1][i];

				efb_priv->colormap[save][2][i] =
				    efb_priv->colormap[stream][2][i];


				efb_priv->colormap[stream][0][i] =
				    efb_priv->consinfo.kcmap[stream][0][i];

				efb_priv->colormap[stream][1][i] =
				    efb_priv->consinfo.kcmap[stream][1][i];

				efb_priv->colormap[stream][2][i] =
				    efb_priv->consinfo.kcmap[stream][2][i];

				efb_priv->cmap_flags[stream][i] = 1;
			}
		}

		efb_cmap_write(efb_priv, stream);
		break;

	case 32:
	/*
	 * This is a quick chance to see if we've set up the colormap
	 * properly.  It is executed for each character so we want to
	 * keep it optimized.  It seems to work.  Worst case, some
	 * sort of sparse but more thorough checking could be done.
	 * This seems to work for the simple tradeoff between the
	 * X server's colormaps and the kernel terminal emulator's.
	 */
		if (efb_priv->colormap[stream][0][0] != 0 ||
		    efb_priv->colormap[stream][0][255] != 0x3ff)
			efb_setup_cmap32(efb_priv);

		break;
	}
}

/*
 * ioctl entry points -------------------------------------------------
 *
 * The ioctl interface tactics differ from the polled interface's.
 * The console ioctls need to do context management (ie. unload user mappings,
 * save user context, load kernel's context).  However since the kernel's
 * context isn't associated with memory mappings, the DDI won't prod us to
 * re-establish the kernel's context if userland interrupts us.	 Therefore,
 * we prevent interruption for the duration of any given rendering operation
 * (ie. displaying character, scrolling one line, or displaying cursor),
 * by holding the context lock.
 *
 * Each rendering operation depends on a particular static setup of the mach64
 * draw engine state.  Besides that, anything the kernel console rendering
 * functions change in the draw engine state can be discarded, because they
 * are re-initialized at each rendering operation.  Therefore the static
 * state is saved once when the kernel context is created, and then loaded,
 * but not saved thereafter.
 *
 */
extern int
efb_vis_consdisplay(drm_device_t *statep, caddr_t data, int flag)
{
	struct vis_consdisplay	 efb_cd;
	struct efb_vis_draw_data efb_draw;
	uint32_t image_size;

	drm_device_t *dev = statep;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);

	if (!(flag & FKIOCTL))
		return (EPERM);

	if (ddi_copyin(data, &efb_cd, sizeof (struct vis_consdisplay), flag))
		return (EFAULT);

	image_size = efb_cd.width * efb_cd.height * EFB_MAX_PIXBYTES;

	if (image_size > efb_priv->consinfo.bufsize) {
		void *tmp = efb_priv->consinfo.bufp;
		if (!(efb_priv->consinfo.bufp =
		    kmem_alloc(image_size, KM_SLEEP))) {
			efb_priv->consinfo.bufp = tmp;
			return (ENOMEM);
		}
		if (tmp)
			kmem_free(tmp, efb_priv->consinfo.bufsize);

		efb_priv->consinfo.bufsize = image_size;
	}

	if (ddi_copyin(efb_cd.data, efb_priv->consinfo.bufp,
	    image_size, flag))
		return (EFAULT);

	if (efb_chk_disp_params(efb_priv, &efb_cd) != DDI_SUCCESS) {
		return (EINVAL);
	}

	DRM_LOCK();

	/* Just return if videomode change is on-going */
	if (efb_priv->setting_videomode) {
		DRM_UNLOCK();
		return (DDI_SUCCESS);
	}

	efb_draw.image_row	= efb_cd.row;
	efb_draw.image_col	= efb_cd.col;
	efb_draw.image_width	= efb_cd.width;
	efb_draw.image_height	= efb_cd.height;
	efb_draw.image		= efb_priv->consinfo.bufp;

	if (efb_invalidate_userctx(efb_priv) == DDI_SUCCESS) {
		efb_restore_kcmap(efb_priv);
		efb_termemu_display(efb_priv, &efb_draw);
	}

	DRM_UNLOCK();

	return (DDI_SUCCESS);
}

extern int
efb_vis_conscopy(drm_device_t *statep, caddr_t data, int flag)
{
	struct vis_conscopy efb_cpydata;

	drm_device_t *dev = statep;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);

	if (!(flag & FKIOCTL))
		return (EPERM);

	if (ddi_copyin(data, &efb_cpydata,
	    sizeof (struct vis_conscopy), flag))
		return (EFAULT);

	if (efb_chk_copy_params(efb_priv, &efb_cpydata) != DDI_SUCCESS) {
		return (EINVAL);
	}

	DRM_LOCK();

	/* Just return if videomode change is on-going */
	if (efb_priv->setting_videomode) {
		DRM_UNLOCK();
		return (DDI_SUCCESS);
	}

	if (efb_invalidate_userctx(efb_priv) == DDI_SUCCESS)
		efb_termemu_copy(efb_priv, &efb_cpydata);

	DRM_UNLOCK();

	return (DDI_SUCCESS);
}

extern int
efb_vis_conscursor(drm_device_t *statep, caddr_t data, int flag)
{
	static struct vis_conscursor efb_cc;

	drm_device_t *dev = statep;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);

	if (!(flag & FKIOCTL))
		return (EPERM);

	if (ddi_copyin(data, &efb_cc,
	    sizeof (struct vis_conscursor), flag))
		return (EFAULT);

	if (efb_chk_cursor_params(efb_priv, &efb_cc) != DDI_SUCCESS) {
		return (EINVAL);
	}

	DRM_LOCK();

	/* Just return if videomode change is on-going */
	if (efb_priv->setting_videomode) {
		DRM_UNLOCK();
		return (DDI_SUCCESS);
	}

	if (efb_invalidate_userctx(efb_priv) == DDI_SUCCESS)
		efb_termemu_cursor(efb_priv, &efb_cc);

	DRM_UNLOCK();

	return (DDI_SUCCESS);
}

/*
 * Polled I/O Entry Points. -----------------------------------------
 *
 * The tactics in these routines are based on the fact that we are
 * -only- called in standalone mode.  Therefore time is frozen
 * for us.  We sneak in restore the kernel's colormap, establish
 * the kernel's draw engine context, render, and then replace the
 * previous context -- no one the wiser for it.
 *
 * In polled I/O mode (also called standalone mode), the kernel isn't
 * running, Only one CPU is enabled, system services are not running,
 * and all access is single-threaded.  The limitations of standalone
 * mode are:  (1) The driver cannot wait for interrupts, (2) The driver
 * cannot use mutexes, (3) The driver cannot allocate memory.
 *
 * The advantage of polled I/O mode is, that because we don't have to
 * worry about concurrent access to device state, we don't need to
 * unload mappings and can perform a lighter form of graphics context
 * switching, which doesn't require the use of mutexes.
 *
 */


/*
 * Setup for DFB rectangle BLIT on a "quiesced" system
 */
static void
efb_polled_consdisplay(struct vis_polledio_arg *arg,
			struct vis_consdisplay *efb_cd)
{
	efb_private_t *efb_priv = (efb_private_t *)arg;
	struct efb_vis_draw_data efb_draw;

	efb_draw.image_row	= efb_cd->row;
	efb_draw.image_col	= efb_cd->col;
	efb_draw.image_width	= efb_cd->width;
	efb_draw.image_height	= efb_cd->height;
	efb_draw.image		= efb_cd->data;

	efb_polledio_enter();
	efb_polled_check_power(efb_priv);
	efb_restore_kcmap(efb_priv);
	efb_termemu_display(efb_priv, &efb_draw);
	efb_polledio_exit();
}

/*
 * Setup for DFB rectangle copy (vertical scroll) on
 * a "quiesced: system.
 */
static void
efb_polled_conscopy(struct vis_polledio_arg *arg,
			struct vis_conscopy *efb_cpydata)
{
	efb_private_t *efb_priv = (efb_private_t *)arg;

	efb_polledio_enter();
	efb_polled_check_power(efb_priv);
	efb_restore_kcmap(efb_priv);
	efb_termemu_copy(efb_priv, efb_cpydata);
	efb_polledio_exit();
}


/*
 * Setup for DFB inverting rectangle BLIT (cursor)
 * on a "quiesced" system.
 *
 */
static void
efb_polled_conscursor(struct vis_polledio_arg *arg,
			struct vis_conscursor *efb_cc)
{
	efb_private_t *efb_priv = (efb_private_t *)arg;

	efb_polledio_enter();
	efb_polled_check_power(efb_priv);
	efb_restore_kcmap(efb_priv);
	efb_termemu_cursor(efb_priv, efb_cc);
	efb_polledio_exit();
}

/* ----------------------------------------------------------- */

/*
 * Copy to DFB a rectangular image whose size, coordinates and pixels
 * are defined in the draw struct.   The proper pixel conversions
 * are made based on the video mode depth.  This operation is implemented
 * in terms of memory copies.
 */

static void
efb_termemu_display(efb_private_t *efb_priv, struct efb_vis_draw_data *efb_draw)
{
	uint8_t	 b, *disp, red, grn, blu;
	void	 *pixp;
	int	 r, c, y, x, h, w;
	int	 stream = efb_priv->consinfo.stream;
	uint32_t pix;
	int	 rshift, gshift, bshift;

	r = efb_draw->image_row;
	c = efb_draw->image_col;
	h = efb_draw->image_height;
	w = efb_draw->image_width;

	disp = efb_draw->image;

	rshift = efb_priv->consinfo.rshift;
	gshift = efb_priv->consinfo.gshift;
	bshift = efb_priv->consinfo.bshift;

	for (y = 0; y < h; y++) {

		for (x = 0; x < w; x++) {

			switch (efb_priv->depth[stream]) {

			case 32:
				/* disp is 00rrggbb */
				disp++;
				red = *disp++;
				grn = *disp++;
				blu = *disp++;

				/* Transform into DFB byte order */
				pix = (red << rshift) | (grn << gshift) |
				    (blu << bshift);
				pixp = DFB32ADR(efb_priv, r + y, c + x);
				*(uint32_t *)pixp = pix;
				break;

			case 8:	 /* 8-bit color map index */
				pixp = DFB8ADR(efb_priv, r + y, c + x);

				b = *disp++;
				if (b > efb_priv->consinfo.kcmap_max)
					efb_priv->consinfo.kcmap_max = b;

				*(uint8_t *)pixp = b;
				efb_priv->consinfo.kcmap_flags[stream][b] = 1;
				break;
			}
		}
	}
}

/*
 * This function implements show/hide cursor functions as appropriate
 * for the current screen depth.  Due to the difficulty of managing
 * a cursor with the multitude of foreground/background text colors,
 * and framebuffer depths with simple ALU operations, particularly
 * in 8-bit psuedocolor mode, the cursor is always displayed along
 * with the underlying pixels in monochrome (ie. black/white). That
 * retains good legibility for all ANSI fg and bg combinations.
 * in all depths.
 *
 * This approach requires saving the contents under the cursor so
 * they can be restored when the cursor moves by re-blitting them
 * onto the DFB, rather than using a heuristic.
 *
 * For the SHOW_CURSOR operation, the contents beneath the cursor
 * are saved before displaying the monochrome overlay in anticipation
 * of a HIDE_CURSOR operation over the same location, prior to moving
 * the cursor to a new location.
 *
 * The HIDE_CURSOR function simply replaces the text saved under
 * the cursor rectangle during the previous SHOW_CURSOR operation.
 *
 * This protocol necessitates tight cursor protocol agreement
 * with the terminal emulator.
 *
 */

#define	MASK24(u32) (u32)

static void
efb_termemu_cursor_32(efb_private_t *efb_priv, struct vis_conscursor *efb_cc)
{
	int x, y;
	int r	= efb_cc->row;
	int c	= efb_cc->col;
	int w	= efb_cc->width;
	int h	= efb_cc->height;

	uint32_t *pixp;
	uint32_t *csrp = (uint32_t *)efb_priv->consinfo.csrp;
	uint32_t rshift = efb_priv->consinfo.rshift;
	uint32_t gshift = efb_priv->consinfo.gshift;
	uint32_t bshift = efb_priv->consinfo.bshift;
	uint32_t fg, bg;

	ASSERT(efb_priv->depth[efb_priv->consinfo.stream] == 32);

	/*
	 * Convert fg/bg into DFB order for direct comparability
	 */
	fg = (efb_cc->fg_color.twentyfour[0] << rshift) |
	    (efb_cc->fg_color.twentyfour[1] << gshift) |
	    (efb_cc->fg_color.twentyfour[2] << bshift);

	bg  = (efb_cc->bg_color.twentyfour[0] << rshift) |
	    (efb_cc->bg_color.twentyfour[1] << gshift) |
	    (efb_cc->bg_color.twentyfour[2] << bshift);

	if (efb_cc->action == SHOW_CURSOR) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				pixp = DFB32ADR(efb_priv, r + y, c + x);
				csrp[(y * w + x) % CSRMAX] = *pixp;
				*pixp = (MASK24(*pixp) == MASK24(bg)) ? fg : bg;
			}
		}
	} else {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				pixp = DFB32ADR(efb_priv, r + y, c + x);
				*pixp = csrp[(y * w + x) % CSRMAX];
			}
		}
	}
}

static void
efb_termemu_cursor_8(efb_private_t *efb_priv, struct vis_conscursor *efb_cc)
{
	int x, y;
	int r	= efb_cc->row;
	int c	= efb_cc->col;
	int w	= efb_cc->width;
	int h	= efb_cc->height;
	uint8_t *pixp;
	uint8_t *csrp = (uint8_t *)efb_priv->consinfo.csrp;

	uint8_t fg = efb_cc->fg_color.eight;
	uint8_t bg = efb_cc->bg_color.eight;

	ASSERT(efb_priv->depth[efb_priv->consinfo.stream] == 8);

	if (efb_cc->action == SHOW_CURSOR) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				pixp = DFB8ADR(efb_priv, r + y, c + x);
				csrp[(y * w + x) % CSRMAX] = *pixp;
				*pixp = (*pixp == bg) ? fg : bg;
			}
		}
	} else {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				pixp = DFB8ADR(efb_priv, r + y, c + x);
				*pixp = csrp[(y * w + x) % CSRMAX];
			}
		}
	}
}

static void
efb_termemu_cursor(efb_private_t *efb_priv, struct vis_conscursor *efb_cc)
{
	switch (efb_priv->depth[efb_priv->consinfo.stream]) {
	case 32:
		efb_termemu_cursor_32(efb_priv, efb_cc);
		break;
	case 8:
		efb_termemu_cursor_8(efb_priv, efb_cc);
		break;
	}
}

/*
 * This function implements scrolling by copying a rectangular block of
 * pixels upward on the Y-axis. The caller provides block copy parameters
 * as a rectangle, defined by (s_col, s_row), (e_col, e_row) and target
 * coords (t_col, t_row).
 *
 * This implementation uses the Radeon GUI engine to accomplish this faster
 * than memory moves done from software.  It is left to the caller to
 * establish the kernel's graphic context prior to calling this function.
 */
static void
efb_termemu_copy(efb_private_t *efb_priv, struct vis_conscopy *efb_copydata)
{
	volatile caddr_t registers = efb_priv->registers;

	uint16_t srcx	= efb_copydata->s_col;
	uint16_t srcy	= efb_copydata->s_row;
	uint16_t dstx	= efb_copydata->t_col;
	uint16_t dsty	= efb_copydata->t_row;
	uint16_t height = (efb_copydata->e_row - efb_copydata->s_row) + 1;
	uint16_t width	= (efb_copydata->e_col - efb_copydata->s_col) + 1;
	uint32_t direction = DST_Y_BOTTOM_TO_TOP | DST_X_RIGHT_TO_LEFT;
	uint32_t gmc_bpp, dp_datatype, pitch_offset;
	uint32_t depth, pitch, offset;
	int	stream = efb_priv->consinfo.stream;
	int	 i;

	if (srcy < dsty) {
		dsty += height - 1;
		srcy += height - 1;
	} else
		direction |= DST_Y_TOP_TO_BOTTOM;

	if (srcx < dstx) {
		dstx += width - 1;
		srcx += width - 1;
	} else
		direction |= DST_X_LEFT_TO_RIGHT;

	depth = efb_priv->depth[stream];
	pitch = efb_priv->consinfo.pitch * depth / 8 / 64;
	offset = efb_priv->consinfo.offset / 1024;

	pitch_offset = pitch << DEFAULT_PITCH_OFFSET__DEFAULT_PITCH__SHIFT |
	    offset;

	switch (depth) {
	case 8:
		gmc_bpp = RADEON_GMC_DST_8BPP;
		dp_datatype = DST_8BPP | BRUSH_SOLIDCOLOR | SRC_DSTCOLOR;
		break;
	case 32:
		gmc_bpp = RADEON_GMC_DST_32BPP;
		dp_datatype = DST_32BPP | BRUSH_SOLIDCOLOR | SRC_DSTCOLOR;
	}

	/*
	 * Initialize GUI engine
	 */
	(void) efb_wait_idle(efb_priv, "termemu_copy", __LINE__);

	regw(DEFAULT_PITCH_OFFSET, pitch_offset);
	regw(RADEON_DST_PITCH_OFFSET, pitch_offset);
	regw(RADEON_SRC_PITCH_OFFSET, pitch_offset);

	regw(DEFAULT_SC_BOTTOM_RIGHT,  0x1fff1fff);

	regw(RADEON_DP_GUI_MASTER_CNTL,
	    GMC_BRUSH_SOLIDCOLOR	|
	    gmc_bpp			|
	    GMC_SRC_DSTCOLOR		|
	    ROP3_SRCCOPY		|
	    GMC_DP_SRC_RECT		|
	    GMC_DST_CLR_CMP_FCN_CLEAR	|
	    GMC_WRITE_MASK_LEAVE);

	regw(DST_LINE_START, 0);
	regw(DST_LINE_END, 0);
	regw(RB3D_CNTL, 0);
	regw(DP_WRITE_MSK, 0xffffffff);

	/*
	 * Switch access mode to PIO
	 */
	(void) efb_wait_idle(efb_priv, "termemu_copy", __LINE__);
	regw(DSTCACHE_CTLSTAT, 0xf);
	for (i = 0;
	    (regr(DSTCACHE_CTLSTAT) & RADEON_RB2D_DC_BUSY) && (i < 16384);
	    i++) {
		;
	}

	/*
	 * perform the copy
	 */
	(void) efb_wait_fifo(efb_priv, 3, "termemu_copy", __LINE__);
	regw(DP_MIX, ROP3_SRCCOPY | DP_SRC_RECT);
	regw(DP_DATATYPE, dp_datatype);
	regw(DP_CNTL, direction);
	(void) efb_wait_fifo(efb_priv, 3, "termemu_copy", __LINE__);
	regw(SRC_Y_X, (srcy << 16) | srcx);
	regw(DST_Y_X, (dsty << 16) | dstx);
	regw(DST_HEIGHT_WIDTH, (height << 16) |	 width);
	(void) efb_wait_idle(efb_priv, "termemu_copy", __LINE__);
}

/*
 * This function invalidates the user's GUI context.
 *
 * It MUST NOT be called in standalone (polled I/O mode).  Polled I/O
 * operates within an incompatible set of constraints and liberties.
 *
 * If there is a user context currently active, this function tears
 * down the user mappings and saves the user's context.	 The strategy
 * is to make  kernel operations uninterruptable from a user mapping,
 *
 * This routine exits HOLDING softc->softc_lock, WHICH THE *CALLER*
 * MUST RELEASE.  This makes each of the following console functions atomic,
 * ie. Draw one character, scroll one line, render one cursor.
 *
 */

static int
efb_invalidate_userctx(efb_private_t *efb_priv)
{
	if (efb_priv->cur_ctx != NULL) {
		/*
		 * Make sure it's safe to do the context switch
		 */
		efb_ctx_wait(efb_priv);
		return (efb_ctx_make_current(efb_priv, NULL));
	}
	return (DDI_SUCCESS);
}


/*
 * Validate the parameters for the data to be displayed on the
 * console.
 *
 * 1. Verify beginning (X,Y) coords are in the displayed area of fb.
 * 2. Verify that the character doesn't extend beyond displayed area.
 *
 * If characters exceed perimiter, clip if possible by adjusting
 * size of characters.	This allows the terminal emulator to clear
 * the full screen or reverse video by writing characters all the way
 * to the screen edge, merely clipping rather than rejecting all
 * but the most egregious overlap.
 */
static int
efb_chk_disp_params(efb_private_t *efb_priv, struct vis_consdisplay *disp)
{
	if ((disp->row > efb_priv->h[efb_priv->consinfo.stream]) ||
	    (disp->col > efb_priv->w[efb_priv->consinfo.stream]))
		return (EINVAL);

	if (((uint_t)disp->row + disp->height) >
	    efb_priv->h[efb_priv->consinfo.stream]) {
		int d = (disp->row + disp->height) -
		    efb_priv->h[efb_priv->consinfo.stream];
		if (d < disp->height)
			disp->height -= d;
		else
			return (EINVAL);
	}

	if (((uint_t)disp->col + disp->width) >
	    efb_priv->w[efb_priv->consinfo.stream]) {
		int d = (disp->col + disp->width) -
		    efb_priv->w[efb_priv->consinfo.stream];
		if (d < disp->width)
			disp->width -= d;
		else
			return (EINVAL);
	}

	return (DDI_SUCCESS);
}

/*
 * Validate the parameters for the data to be displayed on the
 * console.
 *
 * 1. Verify beginning (X,Y) coords are in the displayed area of fb.
 * 2. Verify that the character doesn't extend beyond displayed area.
 */
static int
efb_chk_cursor_params(efb_private_t *efb_priv, struct vis_conscursor *disp)
{
	if ((disp->row > efb_priv->h[efb_priv->consinfo.stream]) ||
	    (disp->col > efb_priv->w[efb_priv->consinfo.stream]))
		return (EINVAL);

	if (((uint_t)disp->row + disp->height) >
	    efb_priv->h[efb_priv->consinfo.stream])
		return (EINVAL);

	if (((uint_t)disp->col + disp->width) >
	    efb_priv->w[efb_priv->consinfo.stream])
		return (EINVAL);

	return (DDI_SUCCESS);
}

/*
 * Validate the parameters for the source and destination rectangles.
 */
static int
efb_chk_copy_params(efb_private_t *efb_priv, struct vis_conscopy *disp)
{
	int	width, height;
	int	s = efb_priv->consinfo.stream;

	if ((0 > disp->e_col) || (disp->e_col > efb_priv->w[s]) ||
	    (0 > disp->s_col) || (disp->s_col > efb_priv->w[s]) ||
	    (0 > disp->t_col) || (disp->t_col > efb_priv->w[s]))
		return (EINVAL);

	width  = disp->e_col - disp->s_col;
	height = disp->e_row - disp->s_row;

	if ((0 > width) || (width > efb_priv->w[s]) ||
	    (0 > height) || (height > efb_priv->h[s]))
		return (EINVAL);

	if ((disp->t_row + height) > efb_priv->h[s])
		return (EINVAL);

	return (DDI_SUCCESS);
}

/*
 * Since being in polled I/O mode, the kernel already has
 * exclusive access to hardware, so we do not need to
 * invalidate user context.
 */
static void
efb_polled_check_power(efb_private_t *efb_priv)
{
#if 0
	if (efb_priv->power_level[EFB_PM_BOARD] < EFB_PWR_ON) {
		efb_set_board_power(efb_priv, EFB_PWR_ON);
	}

	if (efb_priv->consinfo.stream == 0) {
		if (efb_priv->power_level[EFB_PM_MONITOR] < EFB_PWR_ON)
			efb_set_monitor_power1(efb_priv, EFB_PWR_ON);
	} else {
		if (efb_priv->power_level[EFB_PM_MONITOR2] < EFB_PWR_ON)
			efb_set_monitor_power2(efb_priv, EFB_PWR_ON);
	}
#else
	_NOTE(ARGUNUSED(efb_priv))
#endif
}

static void
efb_polledio_enter(void)
{
	efb_in_polledio = 1;
}

static void
efb_polledio_exit(void)
{
	efb_in_polledio = 0;
}

#endif /* VIS_CONS_REV */
