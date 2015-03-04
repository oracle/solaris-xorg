/*
 * Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
 */

#include "efb.h"

static	int efb_ctx_mgt(devmap_cookie_t, void *, offset_t, size_t, uint_t,
		uint_t);

static void efb_ctx_unload_mappings(efb_context_t *);

static int  efb_devmap_access(devmap_cookie_t, void *, offset_t,
				size_t, uint_t, uint_t);
static int  efb_devmap_dup(devmap_cookie_t, void *, devmap_cookie_t, void **);
static int  efb_devmap_map(devmap_cookie_t, dev_t, uint_t,
			    offset_t, size_t, void **);
static void efb_devmap_unmap(devmap_cookie_t, void *, offset_t, size_t,
				devmap_cookie_t, void **,
				devmap_cookie_t, void **);


static struct devmap_callback_ctl efb_devmap_callbacks = {
	DEVMAP_OPS_REV,			/* devmap_rev */
	efb_devmap_map,			/* devmap_map */
	efb_devmap_access,		/* devmap_access */
	efb_devmap_dup,			/* devmap_dup */
	efb_devmap_unmap		/* devmap_unmap */
};

void *
efb_devmap_set_callbacks(int type)
{
	_NOTE(ARGUNUSED(type))

	return (&efb_devmap_callbacks);
}

void *
efb_new_mapping(drm_inst_state_t *mstate,
		devmap_cookie_t dhp, offset_t off, size_t len,
		efb_context_t *ctx)
{
	efb_mapinfo_t *mi;

	mi = kmem_zalloc(sizeof (efb_mapinfo_t), KM_SLEEP);

	mi->next = ctx->mappings;
	if (mi->next != NULL)
		mi->next->prev = mi;
	mi->prev = NULL;
	mi->mstate = mstate;
	mi->ctx = ctx;
	mi->dhp = dhp;
	mi->off = off;
	mi->len = len;
	ctx->mappings = mi;

	return (mi);
}

/* Process new user mapping */

static	int
efb_devmap_map(devmap_cookie_t dhp, dev_t dev, uint_t flags,
		offset_t off, size_t len, void **pvtp)
{
	_NOTE(ARGUNUSED(flags))

	drm_inst_state_t	*mstate;
	efb_context_t		*ctx;
	drm_cminor_t		*md;
	struct drm_radeon_driver_file_fields *radeon_priv;

	mstate = (drm_inst_state_t *)drm_sup_devt_to_state(dev);

	if (((md = drm_find_minordev(mstate, dev)) == NULL) ||
	    (md->fpriv == NULL)) {
		return (EIO);
	}

	radeon_priv = (struct drm_radeon_driver_file_fields *)
	    (md->fpriv->driver_priv);

	if (radeon_priv == NULL) {
		return (EIO);
	}

	mutex_enter(&mstate->dis_ctxlock);
	ctx = (efb_context_t *)(radeon_priv->private_data);
	if (ctx == NULL) {
		ctx = kmem_zalloc(sizeof (efb_context_t), KM_SLEEP);
		ctx->mstate = mstate;
		ctx->mappings = NULL;
		ctx->md = radeon_priv;
		ctx->next = NULL;
		radeon_priv->private_data = (void *)ctx;

		*pvtp = efb_new_mapping(mstate, dhp, off, len, ctx);
		devmap_set_ctx_timeout(dhp, drv_usectohz(1000));
	} else {
		*pvtp = efb_new_mapping(mstate, dhp, off, len, ctx);
	}
	mutex_exit(&mstate->dis_ctxlock);

	return (0);
}


void
efb_ctx_wait(efb_private_t *efb_priv)
{
	int	limit;

	/*
	 * Wait until it's safe to do a context switch.	 Mutex
	 * must be established at this time.
	 */
	for (limit = 100;
	    efb_wait_host_data(efb_priv, "ctx_wait", __LINE__) == 1 &&
	    limit >= 0; limit--) {
		/*
		 * Let the current context fault back in to give it
		 * a chance to finish the host_data write.
		 */
		efb_delay(1);
	}
}


/*
 * Page fault handler.
 */
static	int
efb_devmap_access(devmap_cookie_t dhp, void *pvtp,
		offset_t off, size_t len, uint_t type, uint_t rw)
{
	int rval;
	efb_mapinfo_t		*mi = pvtp;
	drm_inst_state_t	*mstate;
	drm_device_t		*statep;
	drm_radeon_private_t	*dev_priv;
	efb_private_t		*efb_priv;

	if (mi == NULL) {
		rval =	devmap_default_access(dhp, pvtp, off, len, type, rw);
		return (rval);
	}

	mstate = mi->mstate;
	statep = (drm_device_t *)(mstate->mis_devp);
	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);

	/*
	 * If this mapping is not associated with a context, or that
	 * context is already current, simply validate the mapping.
	 */
	if (mi->ctx == NULL || mi->ctx == efb_priv->cur_ctx) {
		rval =	devmap_default_access(dhp, pvtp, off, len, type, rw);
		return (rval);
	}

	/*
	 * This is a context switch.  Call devmap_do_ctxmgt().
	 */

	/*
	 * Release the mutex before calling devmap_do_ctxmgt(), because
	 * devmap_do_ctxmgt() may sleep for a while.
	 */
	return (devmap_do_ctxmgt(dhp, pvtp, off, len, type, rw, efb_ctx_mgt));
}


/*
 * Called by efb_devmap_access() (via devmap_do_ctxmgt), this function
 * calls efb_ctx_make_current() and then validates the page.
 */

static	int
efb_ctx_mgt(devmap_cookie_t dhp, void *pvtp,
	    offset_t off, size_t len, uint_t type, uint_t rw)
{
	int rval;
	efb_mapinfo_t		*mi = pvtp;
	efb_context_t		*ctx;
	drm_inst_state_t	*mstate;
	drm_device_t		*statep, *dev;
	drm_radeon_private_t	*dev_priv;
	efb_private_t		*efb_priv;

	mstate = mi->mstate;
	statep = (drm_device_t *)(mstate->mis_devp);
	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);
	dev = statep;

	DRM_LOCK();

	ctx = mi->ctx;

	/* Make sure it's safe to do the context switch */
	efb_ctx_wait(efb_priv);

	rval = efb_ctx_make_current(efb_priv, ctx);

	if (rval == 0) {
		rval = devmap_default_access(dhp, pvtp, off, len, type, rw);
	}

	DRM_UNLOCK();

	return (rval);
}

void
efb_ctx_save(efb_private_t *efb_priv, efb_context_t *ctx)
{
#if VIS_CONS_REV > 2
	volatile caddr_t registers = efb_priv->registers;
	int stream = efb_priv->consinfo.stream;
	int i;

	efb_cmap_read(efb_priv, 0, EFB_CMAP_ENTRIES, stream);
	for (i = 0; i < EFB_CMAP_ENTRIES; i++) {
		ctx->colormap[0][i] = efb_priv->colormap[stream][0][i];
		ctx->colormap[1][i] = efb_priv->colormap[stream][1][i];
		ctx->colormap[2][i] = efb_priv->colormap[stream][2][i];
	}

	ctx->default_pitch_offset = regr(DEFAULT_PITCH_OFFSET);
	ctx->src_pitch_offset = regr(RADEON_SRC_PITCH_OFFSET);
	ctx->dst_pitch_offset = regr(RADEON_DST_PITCH_OFFSET);
	ctx->default_sc_bottom_right = regr(DEFAULT_SC_BOTTOM_RIGHT);
	ctx->dp_gui_master_cntl = regr(RADEON_DP_GUI_MASTER_CNTL);
	ctx->dst_line_start = regr(DST_LINE_START);
	ctx->dst_line_end = regr(DST_LINE_END);
	ctx->rb3d_cntl = regr(RB3D_CNTL);
	ctx->dp_write_mask = regr(DP_WRITE_MSK);
	ctx->dp_mix = regr(DP_MIX);
	ctx->dp_datatype = regr(DP_DATATYPE);
	ctx->dp_cntl = regr(DP_CNTL);
	ctx->src_y = regr(SRC_Y);
	ctx->src_x = regr(SRC_X);
	ctx->dst_y = regr(DST_Y);
	ctx->dst_x = regr(DST_X);

#endif /* VIS_CONS_REV */
}

void
efb_ctx_restore(efb_private_t *efb_priv, efb_context_t *ctx)
{
#if VIS_CONS_REV > 2
	volatile caddr_t registers = efb_priv->registers;
	int stream = efb_priv->consinfo.stream;
	int i;

	for (i = 0; i < EFB_CMAP_ENTRIES; i++) {
		efb_priv->colormap[stream][0][i] = ctx->colormap[0][i];
		efb_priv->colormap[stream][1][i] = ctx->colormap[1][i];
		efb_priv->colormap[stream][2][i] = ctx->colormap[2][i];
		efb_priv->cmap_flags[stream][i] = 1;
	}
	efb_cmap_write(efb_priv, stream);


	if (efb_wait_fifo(efb_priv, 20, "restore", __LINE__) != DDI_SUCCESS)
		return;

	regw(DEFAULT_PITCH_OFFSET, ctx->default_pitch_offset);
	regw(RADEON_DST_PITCH_OFFSET, ctx->dst_pitch_offset);
	regw(RADEON_SRC_PITCH_OFFSET, ctx->src_pitch_offset);
	regw(DEFAULT_SC_BOTTOM_RIGHT, ctx->default_sc_bottom_right);
	regw(RADEON_DP_GUI_MASTER_CNTL, ctx->dp_gui_master_cntl);
	regw(DST_LINE_START, ctx->dst_line_start);
	regw(DST_LINE_END, ctx->dst_line_end);
	regw(RB3D_CNTL, ctx->rb3d_cntl);
	regw(DP_WRITE_MSK, ctx->dp_write_mask);
	regw(DP_MIX, ctx->dp_mix);
	regw(DP_DATATYPE, ctx->dp_datatype);
	regw(DP_CNTL, ctx->dp_cntl);
	regw(SRC_Y, ctx->src_y);
	regw(SRC_X, ctx->src_x);
	regw(DST_Y, ctx->dst_y);
	regw(DST_X, ctx->dst_x);

	if (!efb_wait_idle(efb_priv, "ctx_restore", __LINE__)) {
		cmn_err(CE_WARN, "efb: idle timeout in context restore");
	}
#endif /* VIS_CONS_REV */
}


/*
 * Context management.	This function does the actual context-swapping.
 */
int
efb_ctx_make_current(efb_private_t *efb_priv, efb_context_t *ctx)
{
	efb_context_t	*old_ctx;

	if (efb_priv->cur_ctx == ctx)
		return (0);

	old_ctx = efb_priv->cur_ctx;

	/*
	 * Invalidate all other context mappings.
	 * Finally, validate this mapping.
	 *
	 * Implementation note:
	 * When a brand new context faults in for the first time,
	 * all register settings are considered to be undefined.
	 * (The new context inherits whatever register values were
	 * left behind by the old context.)
	 */

	/* Unload the current context mappings */
	if (old_ctx != NULL) {
		efb_ctx_unload_mappings(old_ctx);
	}

	if (!efb_wait_idle(efb_priv, "ctx_current", __LINE__)) {
		cmn_err(CE_WARN, "efb: idle timeout in context swap");
	}

	/* Save current context */
	if (old_ctx != NULL) {
		efb_ctx_save(efb_priv, old_ctx);
	}

	/* Load new context */
	if (ctx != NULL) {
		efb_ctx_restore(efb_priv, ctx);
	}

	efb_priv->cur_ctx = ctx;

	return (0);
}



/* Here when user process forks */
static	int
efb_devmap_dup(devmap_cookie_t dhp, void *pvtp,
		devmap_cookie_t new_dhp, void **new_pvtp)
{
	efb_mapinfo_t		*mi = pvtp;
	drm_inst_state_t	*mstate;

	if ((mi == NULL) || (mi->dhp != dhp)) {
		*new_pvtp = NULL;	/* no context */
		return (0);
	}

	mstate = mi->mstate;

	mutex_enter(&mstate->dis_ctxlock);

	/*
	 * For now, we do NOT create a new context.  Since the parent
	 * and child processes share one minordev structure, they share
	 * the context.
	 */

	*new_pvtp = efb_new_mapping(mi->mstate, new_dhp, mi->off, mi->len,
	    mi->ctx);

	mutex_exit(&mstate->dis_ctxlock);

	return (0);
}

/* User has released a mapping (or part of one). */
static	void
efb_devmap_unmap(devmap_cookie_t dhp, void *pvtp,
		offset_t off, size_t len, devmap_cookie_t new_dhp1,
		void **new_pvtp1, devmap_cookie_t new_dhp2, void **new_pvtp2)
{
	efb_mapinfo_t		*mi = pvtp;
	drm_inst_state_t	*mstate;
	drm_device_t		*statep;
	drm_radeon_private_t	*dev_priv;
	efb_private_t		*efb_priv;
	efb_context_t		*ctx;
	struct drm_radeon_driver_file_fields *radeon_priv;

	if ((mi == NULL) || (mi->dhp != dhp)) {
		if (new_pvtp1 != NULL)
			*new_pvtp1 = NULL;

		if (new_pvtp2 != NULL)
			*new_pvtp2 = NULL;

		return;
	}

	mstate = mi->mstate;
	statep = (drm_device_t *)(mstate->mis_devp);
	dev_priv = (drm_radeon_private_t *)(statep->dev_private);
	efb_priv = (efb_private_t *)(dev_priv->private_data);

	mutex_enter(&mstate->dis_ctxlock);

	ctx = mi->ctx;
	radeon_priv = ctx->md;

	/*
	 * Part or all of this mapping is going away.
	 *
	 * If this mapinfo structure is destroyed, and this is the last
	 * mapinfo that belongs to the context, destroy the context.
	 */

	/* Unmapping after beginning? */
	if (new_dhp1 != NULL && new_pvtp1 != NULL) {
		*new_pvtp1 = efb_new_mapping(mstate, new_dhp1,
		    mi->off, off - mi->off, ctx);
	}

	/* Unmapping before end? */
	if (new_dhp2 != NULL && new_pvtp2 != NULL) {
		*new_pvtp2 = efb_new_mapping(mstate, new_dhp2, off+len,
		    mi->off + mi->len - (off+len), ctx);
	}

	/* Now destroy the original */
	if (mi->prev != NULL)
		mi->prev->next = mi->next;
	else if (ctx != NULL)
		ctx->mappings = mi->next;

	if (mi->next != NULL)
		mi->next->prev = mi->prev;

	/* All gone? */
	if (ctx != NULL && ctx->mappings == NULL) {
		efb_context_t *ptr;

		ptr = (efb_context_t *)(radeon_priv->private_data);

		if (ptr == ctx) {
			radeon_priv->private_data = (void *)(ctx->next);
		} else {
			for (; ptr != NULL && ptr->next != ctx;
			    ptr = ptr->next)
				;

			if (ptr == NULL) {
				cmn_err(CE_WARN, "efb: context %p not found",
				    (void *)ctx);
			} else {
				ptr->next = ctx->next;
			}
		}

		if (efb_priv->cur_ctx == ctx) {
			efb_priv->cur_ctx = NULL;
		}
		kmem_free(ctx, sizeof (efb_context_t));
	}

	kmem_free(mi, sizeof (efb_mapinfo_t));

	mutex_exit(&mstate->dis_ctxlock);
}


/* Unload all of the mappings to this context */
static	void
efb_ctx_unload_mappings(efb_context_t *ctx)
{
	efb_mapinfo_t	*mi;
	int		rval;

	for (mi = ctx->mappings; mi != NULL; mi = mi->next) {

		rval = devmap_unload(mi->dhp, mi->off, mi->len);
		if (rval != 0) {
			cmn_err(CE_WARN,
			    "efb: devmap_unload(%lx,%lx) returns %d",
			    (unsigned long) mi->off, mi->len, rval);
		}
	}
}



/*
 * Invalidate all user context mappings.  It should be
 * sufficient to unload the current context, if any, as
 * the current context is the only one that is loaded.
 * However, just to be sure, we unload everything.
 *
 * Note that this function only unloads mappings; the
 * current context is still in hardware.
 */
void
efb_unload_all(efb_private_t *efb_priv)
{
	efb_context_t	*ctx;

	for (ctx = efb_priv->contexts; ctx != NULL; ctx = ctx->next) {
		efb_ctx_unload_mappings(ctx);
	}
}
