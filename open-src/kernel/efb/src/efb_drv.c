/*
 * Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
 */

/*
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * All Rights Reserved.
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
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Gareth Hughes <gareth@valinux.com>
 *
 */

#include "drmP.h"
#include "drm.h"
#include "radeon_drm.h"
#include "radeon_drv.h"
#include "drm_pciids.h"

#include "efb.h"
#include "efb_edid.h"

static void *radeon_statep;
extern struct cb_ops drm_cb_ops;

static int	efb_fifo_reset(efb_private_t *efb_priv);

/* Little-endian mapping with strict ordering for registers */
static	struct ddi_device_acc_attr littleEnd = {
	DDI_DEVICE_ATTR_V0,
	DDI_STRUCTURE_LE_ACC,
	DDI_STRICTORDER_ACC
};

void
efb_init(void *rstatep, struct cb_ops *efb_cb_ops, drm_driver_t *driver)
{
	radeon_statep = rstatep;

	efb_cb_ops->cb_open   = drm_cb_ops.cb_open;
	efb_cb_ops->cb_close  = drm_cb_ops.cb_close;
	efb_cb_ops->cb_devmap = drm_cb_ops.cb_devmap;

	driver->set_devmap_callbacks = efb_devmap_set_callbacks;
}

int
efb_attach(drm_device_t *statep)
{
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	/*
	 * setup mapping for later PCI config space access
	 */
	dev_priv = statep->dev_private;

	if (dev_priv->private_data == NULL) {
		dev_priv->private_data =
		    kmem_zalloc(sizeof (efb_private_t), KM_SLEEP);
	}

	efb_priv = (efb_private_t *)dev_priv->private_data;
	efb_priv->dip = statep->dip;

	if (pci_config_setup(statep->dip, &efb_priv->pci_handle)
	    != DDI_SUCCESS) {
		DRM_ERROR("efb_attach: "
		    "PCI configuration space setup failed");
		efb_detach(statep);
		return (DDI_FAILURE);
	}

	if (efb_map_registers(statep) != DDI_SUCCESS) {
		DRM_ERROR("efb_attach: "
		    "Map registers failed");
		efb_detach(statep);
		return (DDI_FAILURE);
	}

	efb_priv->primary_stream = 0;
	efb_priv->power_level[EFB_PM_BOARD] = EFB_PWR_ON;

#if VIS_CONS_REV > 2
	/*
	 * setup for coherent console
	 */
	if (ddi_prop_update_int(DDI_DEV_T_NONE, statep->dip,
	    "tem-support", 1) != DDI_PROP_SUCCESS) {
		DRM_ERROR("efb_attach: "
		    "Unable to set tem-support property");
		return (DDI_FAILURE);
	}
#endif

	return (DDI_SUCCESS);
}

void
efb_detach(drm_device_t *statep)
{
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)statep->dev_private;
	efb_priv = (efb_private_t *)dev_priv->private_data;

	if (efb_priv != NULL) {
		efb_unmap_registers(statep);
		kmem_free(dev_priv->private_data, sizeof (efb_private_t));
		dev_priv->private_data = NULL;
	}
}

static void
efb_read_pci_config(drm_device_t *statep, void *pBuffer, int offset,
			int length)
{
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;
	int  i;
	unsigned short	*pshort = (unsigned short *) pBuffer;
	unsigned int	*pint   = (unsigned int *)   pBuffer;
	unsigned long	*plong  = (unsigned long *)  pBuffer;
	char		*pBuf   = (char *)pBuffer;

	dev_priv = (drm_radeon_private_t *)statep->dev_private;
	efb_priv = (efb_private_t *)dev_priv->private_data;

#ifdef _BIG_ENDIAN
	switch (length) {
	case 1:
		*pBuf = pci_config_get8(efb_priv->pci_handle, offset);
		break;
	case 2:
		*pshort = pci_config_get16(efb_priv->pci_handle, offset);
		break;
	case 4:
		*pint = pci_config_get32(efb_priv->pci_handle, offset);
		break;
	case 8:
		*plong = pci_config_get64(efb_priv->pci_handle, offset);
		break;
	default:

		for (i = offset; i < offset + length; ) {
			switch (i) {

			case 0x00: case 0x02:
			case 0x04: case 0x06:
			case 0x2c: case 0x2e:
			case 0x4e: case 0x50:
			{
				*(pBuf+1) =
				    pci_config_get8(efb_priv->pci_handle, i++);
				*(pBuf+0) =
				    pci_config_get8(efb_priv->pci_handle, i++);
				pBuf += 2;
				break;
			}
			case 0x08: case 0x09:
			case 0x0a: case 0x0b:
			case 0x0c: case 0x0d:
			case 0x0e: case 0x0f:
			case 0x34: case 0x35:
			case 0x36: case 0x37:
			case 0x3c: case 0x3d:
			case 0x3e: case 0x3f:
			case 0x40: case 0x41:
			case 0x42: case 0x43:
			case 0x4c: case 0x4d:
			case 0x53:
			{
				*pBuf =
				    pci_config_get8(efb_priv->pci_handle, i++);
				pBuf += 1;
				break;
			}
			case 0x52:
			{
				*pBuf =
				    pci_config_get8(efb_priv->pci_handle, i++);
				pBuf += 1;
				break;
			}
			default:
			{
				*(pBuf+3) =
				    pci_config_get8(efb_priv->pci_handle, i++);
				*(pBuf+2) =
				    pci_config_get8(efb_priv->pci_handle, i++);
				*(pBuf+1) =
				    pci_config_get8(efb_priv->pci_handle, i++);
				*(pBuf+0) =
				    pci_config_get8(efb_priv->pci_handle, i++);
				pBuf += 4;
				break;
			}
			}
		}
	}
#endif /* _BIG_ENDIAN */
}

int
efb_get_pci_config(drm_device_t *statep, dev_t dev, intptr_t arg, int mode)
{
	_NOTE(ARGUNUSED(dev))

	struct gfx_pci_cfg efb_pci_cfg;

	efb_read_pci_config(statep, &efb_pci_cfg, (uint_t)0,
	    sizeof (struct gfx_pci_cfg));

	if (ddi_copyout(&efb_pci_cfg, (void *)arg,
	    sizeof (efb_pci_cfg), mode)) {
		return (EFAULT);
	}
	return (0);
}

int
efb_map_registers(drm_device_t *statep)
{
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)statep->dev_private;
	efb_priv = (efb_private_t *)dev_priv->private_data;

	if (ddi_regs_map_setup(statep->dip, 3, (caddr_t *)&efb_priv->registers,
	    0L, EFB_REG_SIZE, &littleEnd, &efb_priv->registersmap)
	    != DDI_SUCCESS) {
		return (DDI_FAILURE);
	}
	return (DDI_SUCCESS);
}

void
efb_unmap_registers(drm_device_t *statep)
{
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)statep->dev_private;
	efb_priv = (efb_private_t *)dev_priv->private_data;

	if (efb_priv->registers != NULL) {
		ddi_regs_map_free(&efb_priv->registersmap);
		efb_priv->registers = NULL;
	}
}

int
efb_get_gfx_identifier(drm_device_t *statep, dev_t dev, intptr_t arg, int mode)
{
	_NOTE(ARGUNUSED(dev))

	struct gfx_identifier radeon_gfx_identifier = {
		.version = GFX_IDENT_VERSION,
		.flags = 0
	};
	char *buf;
	int proplen;

	if (ddi_getlongprop(DDI_DEV_T_ANY, statep->dip, DDI_PROP_DONTPASS,
	    "name", (caddr_t)&buf, &proplen) == DDI_SUCCESS) {
		(void) strlcpy(radeon_gfx_identifier.model_name, buf,
		    sizeof (radeon_gfx_identifier.model_name));
		radeon_gfx_identifier.flags |= GFX_IDENT_MODELNAME;
		kmem_free(buf, proplen);
	}

	if (ddi_getlongprop(DDI_DEV_T_ANY, statep->dip, DDI_PROP_DONTPASS,
	    "model", (caddr_t)&buf, &proplen) == DDI_SUCCESS) {
		(void) strlcpy(radeon_gfx_identifier.part_number, buf,
		    sizeof (radeon_gfx_identifier.part_number));
		radeon_gfx_identifier.flags |= GFX_IDENT_PARTNUM;
		kmem_free(buf, proplen);
	}

	if (ddi_copyout(&radeon_gfx_identifier, (caddr_t)arg,
	    sizeof (radeon_gfx_identifier), mode)) {
		return (EFAULT);
	}

	return (0);
}

int
efb_set_video_mode(drm_device_t *statep, dev_t dev, intptr_t arg, int mode)
{
	_NOTE(ARGUNUSED(dev))

	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)statep->dev_private;
	efb_priv = (efb_private_t *)dev_priv->private_data;

	if (ddi_copyin((caddr_t)arg, &efb_priv->videomode,
	    sizeof (struct gfx_video_mode), mode)) {
		return (EFAULT);
	}

#if VIS_CONS_REV > 2
	efb_termemu_callback(statep);
#endif /* VIS_CONS_REV */

	return (0);
}

int
efb_get_video_mode(drm_device_t *statep, dev_t dev, intptr_t arg, int mode)
{
	_NOTE(ARGUNUSED(dev))

	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;

	dev_priv = (drm_radeon_private_t *)statep->dev_private;
	efb_priv = (efb_private_t *)dev_priv->private_data;

	if (ddi_copyout(&efb_priv->videomode, (caddr_t)arg,
	    sizeof (struct gfx_video_mode), mode)) {
		return (EFAULT);
	}

	return (0);
}

int
efb_get_edid_length(drm_device_t *statep, dev_t dev1, intptr_t arg, int mode)
{
	_NOTE(ARGUNUSED(dev1))

	drm_device_t *dev = statep;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;
	uint8_t *results;
	uint_t length = GFX_EDID_BLOCK_SIZE;
	gfx_edid_t edid_buf;
	int ret;
	int stream = 0;

	dev_priv = (drm_radeon_private_t *)statep->dev_private;
	efb_priv = (efb_private_t *)dev_priv->private_data;

	if (ddi_copyin((void *)arg, &edid_buf, sizeof (gfx_edid_t), mode)) {
		return (EFAULT);
	}

	/* validate head */
	stream = edid_buf.head;
	if (stream < GFX_EDID_HEAD_ONE || stream > GFX_EDID_HEAD_TWO) {
		return (EINVAL);
	}

	/* map to the internal stream number */
	stream--;

	results = kmem_alloc(GFX_EDID_BLOCK_SIZE, KM_SLEEP);

	DRM_LOCK();

	ret = efb_read_edid(efb_priv, stream, results, &length);

	DRM_UNLOCK();

	if (!ret) {

		/* the 127th byte specifies the extension block count */
		edid_buf.length = GFX_EDID_BLOCK_SIZE * (1 + results[126]);

		if (ddi_copyout(&edid_buf, (caddr_t)arg, sizeof (gfx_edid_t),
		    mode)) {
			ret = EFAULT;
		}
	}

	kmem_free(results, GFX_EDID_BLOCK_SIZE);
	return (ret);
}

int
efb_get_edid(drm_device_t *statep, dev_t dev1, intptr_t arg, int mode)
{
	_NOTE(ARGUNUSED(dev1))

	drm_device_t *dev = statep;
	drm_radeon_private_t *dev_priv;
	efb_private_t *efb_priv;
	int stream;
	int ret;
	gfx_edid_t edid_buf;
	uint8_t *results;
	int length;

	dev_priv = (drm_radeon_private_t *)statep->dev_private;
	efb_priv = (efb_private_t *)dev_priv->private_data;

	if (ddi_copyin((void *)arg, &edid_buf, sizeof (gfx_edid_t), mode)) {
		return (EFAULT);
	}

	length = edid_buf.length;
	if (length <= 0) {
		return (EINVAL);
	}

	/* validate head */
	stream = edid_buf.head;
	if (stream < GFX_EDID_HEAD_ONE || stream > GFX_EDID_HEAD_TWO) {
		return (EINVAL);
	}

	/* map to the internal stream number */
	stream--;

	results = kmem_alloc(length, KM_SLEEP);

#ifdef _MULTI_DATAMODEL
	if (ddi_model_convert_from(mode & FMODELS) == DDI_MODEL_ILP32) {
		edid_buf.data = (caddr_t)
		    ((unsigned long)edid_buf.data & 0xffffffff);
	}
#endif /* _MULTI_DATAMODEL */

	DRM_LOCK();

	ret = efb_read_edid(efb_priv, stream, results, &edid_buf.length);

	DRM_UNLOCK();

	if (!ret) {
		ret = ddi_copyout(results, edid_buf.data,
		    edid_buf.length, mode);
	}

	if (!ret) {
		ret = ddi_copyout(&edid_buf, (caddr_t)arg,
		    sizeof (gfx_edid_t), mode);
	}

	if (ret) {
		ret = EFAULT;
	}

	kmem_free(results, length);
	return (ret);
}


static const struct vis_identifier visId = {"SUNWefb"};

/* default structure for FBIOGATTR ioctl */
static const struct fbgattr fb_attr = {
	FBTYPE_LASTPLUSONE + 1,		/* real type */
	0,				/* owner */
	{ FBTYPE_LASTPLUSONE + 1,	/* emulated type */
	    25, 80, 1,			/* w, h, depth */
	    256,			/* colormap size */
	    0,				/* total size */
	},
	0,				/* flags */
	FBTYPE_LASTPLUSONE + 1,		/* current emulated type */
	{ 0 },				/* dev_specific */
	{ -1, -1, -1, -1 },		/* Emulation types */
};


int
efb_ioctl(dev_t dev, int cmd, intptr_t arg, int mode, cred_t *credp,
		int *rvalp)
{

	drm_device_t	*statep;
	int		unit;
	int		ret = 0;

	unit = DEV2INST(dev);
	statep = ddi_get_soft_state(radeon_statep, unit);
	if (statep == NULL)
		return (DDI_FAILURE);

	switch (cmd) {

#if VIS_CONS_REV > 2
		case VIS_DEVINIT:
		{
			ret = efb_vis_devinit(statep, (caddr_t)arg, mode);
			break;
		}

		case VIS_DEVFINI:
		{
			ret = efb_vis_devfini(statep, (caddr_t)arg, mode);
			break;
		}

		case VIS_CONSDISPLAY:
		{
			ret = efb_vis_consdisplay(statep, (caddr_t)arg, mode);
			break;
		}

		case VIS_CONSCURSOR:
		{
			ret = efb_vis_conscursor(statep, (caddr_t)arg, mode);
			break;
		}

		case VIS_CONSCOPY:
		{
			ret = efb_vis_conscopy(statep, (caddr_t)arg, mode);
			break;
		}

		case VIS_PUTCMAP:
		{
			ret = efb_vis_putcmap(statep, (caddr_t)arg, mode);
			break;
		}
#endif /* VIS_CONS_REV */

		case VIS_GETIDENTIFIER:
		{
			if (ddi_copyout(&visId, (void *)arg,
			    sizeof (struct vis_identifier), mode))
				ret = EFAULT;
			break;
		}

		case FBIOGATTR:
		{
			if (ddi_copyout(&fb_attr, (void *)arg,
			    sizeof (struct fbgattr), mode))
				ret = EFAULT;
			break;
		}

		case FBIOGTYPE:
		{
			if (ddi_copyout(&fb_attr.fbtype, (void *)arg,
			    sizeof (struct fbtype), mode))
				ret = EFAULT;
			break;
		}

		case FBIOGXINFO:
		{
			ret = EFAULT;
			break;
		}

		case GFX_IOCTL_GET_IDENTIFIER:
		{
			ret = efb_get_gfx_identifier(statep, dev, arg, mode);
			break;
		}

		case GFX_IOCTL_GET_PCI_CONFIG:
		{
			ret = efb_get_pci_config(statep, dev, arg, mode);
			break;
		}

		case GFX_IOCTL_SET_VIDEO_MODE:
		{
			ret = efb_set_video_mode(statep, dev, arg, mode);
			break;
		}

		case GFX_IOCTL_GET_CURRENT_VIDEO_MODE:
		{
			ret = efb_get_video_mode(statep, dev, arg, mode);
			break;
		}

		case GFX_IOCTL_GET_EDID_LENGTH:
		{
			ret = efb_get_edid_length(statep, dev, arg, mode);
			break;
		}

		case GFX_IOCTL_GET_EDID:
		{
			ret = efb_get_edid(statep, dev, arg, mode);
			break;
		}

		default:
		{
			ret = drm_cb_ops.cb_ioctl(dev, cmd, arg, mode,
			    credp, rvalp);
			break;
		}
	}
	return (ret);
}


/*
 * Read width, height, depth, etc. parameters from hardware into softc.
 */
static	void
size_compute(efb_private_t *efb_priv, int stream,
    uint32_t h_total_disp, uint32_t v_total_disp)
{
	int	v2;

	v2 = (h_total_disp & CRTC_H_TOTAL_DISP__CRTC_H_DISP_MASK) >>
	    CRTC_H_TOTAL_DISP__CRTC_H_DISP__SHIFT;
	efb_priv->w[stream] = (v2 + 1) * 8;

	v2 = (v_total_disp & CRTC_V_TOTAL_DISP__CRTC_V_DISP_MASK) >>
	    CRTC_V_TOTAL_DISP__CRTC_V_DISP__SHIFT;
	efb_priv->h[stream] = v2 + 1;
}

/*
 * Read width, height, depth, stride, pixfreq, h/v sync, fporch, bporch,
 * freq parameters from hardware into softc.
 */
void
efb_getsize(efb_private_t *efb_priv)
{
	volatile caddr_t registers = efb_priv->registers;
	uint32_t	v;
	uint32_t	h_total_disp;
	uint32_t	v_total_disp;
	int		v2;
	static int	depths[] = {0, 4, 8, 15, 16, 24, 32, 16, 16};

	if (efb_priv->power_level[EFB_PM_BOARD] <= EFB_PWR_OFF)
		return;

	/*
	 * Stream 1
	 */
	v = regr(CRTC_GEN_CNTL);
	v2 = (v & CRTC_GEN_CNTL__CRTC_PIX_WIDTH_MASK) >>
	    CRTC_GEN_CNTL__CRTC_PIX_WIDTH__SHIFT;
	efb_priv->depth[0] = depths[v2];

	v = regr(CRTC_PITCH);
	efb_priv->stride[0] = (v & CRTC_PITCH__CRTC_PITCH_MASK) * 8;

	h_total_disp = regr(CRTC_H_TOTAL_DISP);
	v_total_disp = regr(CRTC_V_TOTAL_DISP);

	size_compute(efb_priv, 0, h_total_disp, v_total_disp);


	/*
	 * Stream 2
	 */
	v = regr(CRTC2_GEN_CNTL);
	v2 = (v & CRTC_GEN_CNTL__CRTC_PIX_WIDTH_MASK) >>
	    CRTC_GEN_CNTL__CRTC_PIX_WIDTH__SHIFT;
	efb_priv->depth[1] = depths[v2];

	v = regr(CRTC2_PITCH);
	efb_priv->stride[1] = (v & CRTC_PITCH__CRTC_PITCH_MASK) * 8;

	h_total_disp = regr(CRTC2_H_TOTAL_DISP);
	v_total_disp = regr(CRTC2_V_TOTAL_DISP);

	size_compute(efb_priv, 1, h_total_disp, v_total_disp);
}

/*
 * Write all pending entries in softc colormap to hardware
 */
void
efb_cmap_write(efb_private_t *efb_priv, int cmap)
{
	volatile caddr_t registers = efb_priv->registers;
	int	index, oflag;
	uint8_t	*flags;
	uint16_t *red, *green, *blue;
	uint32_t dac_cntl2, v;

	int	keep = 1;
	int	map = cmap & 0x1;

	/*
	 * if restoring the previous saved colormap entries,
	 * reset the flag
	 */
	if (cmap > 1)
		keep = 0;

	if (efb_priv->power_level[EFB_PM_BOARD] <= EFB_PWR_OFF)
		return;

	red   = efb_priv->colormap[cmap][0];
	green = efb_priv->colormap[cmap][1];
	blue  = efb_priv->colormap[cmap][2];
	flags = efb_priv->cmap_flags[map];

	v  = dac_cntl2 = regr(DAC_CNTL2);
	v &= ~DAC_CNTL2__PALETTE_ACCESS_CNTL_MASK;
	v |= map<<5;
	regw(DAC_CNTL2, v);

	oflag = 0;
	for (index = 0; index < EFB_CMAP_ENTRIES; ++index) {

		if (*flags) {
			if (!oflag) {
				regw8(PALETTE_INDEX, index);
			}

			v = ((*red << PALETTE_30_DATA__PALETTE_DATA_R__SHIFT)
			    & PALETTE_30_DATA__PALETTE_DATA_R_MASK) |
			    ((*green << PALETTE_30_DATA__PALETTE_DATA_G__SHIFT)
			    & PALETTE_30_DATA__PALETTE_DATA_G_MASK) |
			    ((*blue << PALETTE_30_DATA__PALETTE_DATA_B__SHIFT)
			    & PALETTE_30_DATA__PALETTE_DATA_B_MASK);

			regw(PALETTE_30_DATA, v);
		}

		oflag = *flags;
		*flags &= keep;
		flags++;
		++red;
		++green;
		++blue;
	}

	regw(DAC_CNTL2, dac_cntl2);
}


/*
 * Read hardware colormap into softc
 */
void
efb_cmap_read(efb_private_t *efb_priv, int start, int count, int map)
{
	volatile caddr_t registers = efb_priv->registers;
	int	idx, i;
	uint32_t dac_cntl2, v;

	if (efb_priv->power_level[EFB_PM_BOARD] <= EFB_PWR_OFF)
		return;

	v  = dac_cntl2 = regr(DAC_CNTL2);
	v &= ~DAC_CNTL2__PALETTE_ACCESS_CNTL_MASK;
	v |= map<<5;
	regw(DAC_CNTL2, v);

	regw8(PALETTE_INDEX + 2, start);

	for (idx = start, i = count; --i >= 0; ++idx) {
		v = regr(PALETTE_30_DATA);
		efb_priv->colormap[map][0][idx] =
		    (v & PALETTE_30_DATA__PALETTE_DATA_R_MASK) >>
		    PALETTE_30_DATA__PALETTE_DATA_R__SHIFT;

		efb_priv->colormap[map][1][idx] =
		    (v & PALETTE_30_DATA__PALETTE_DATA_G_MASK) >>
		    PALETTE_30_DATA__PALETTE_DATA_G__SHIFT;

		efb_priv->colormap[map][2][idx] =
		    (v & PALETTE_30_DATA__PALETTE_DATA_B_MASK) >>
		    PALETTE_30_DATA__PALETTE_DATA_B__SHIFT;
	}

	regw(DAC_CNTL2, dac_cntl2);
}



/*
 * Wait for draw engine idle.  Return 1 on success, 0 on
 * failure (busy bit never goes away)
 */

#define	MASK		RBBM_STATUS__CMDFIFO_AVAIL_MASK

/* Let's try a much simpler loop */
int
efb_wait_fifo(efb_private_t *efb_priv, int n, const char *func, int line)
{
	volatile caddr_t registers = efb_priv->registers;
	long		limit;
	long		dt;

	for (limit = 10000;
	    ((regr(RBBM_STATUS) & MASK) < n) && (limit > 0); limit--) {
		;
	}

	if ((regr(RBBM_STATUS) & MASK) >= n) {
		return (DDI_SUCCESS);
	}

	/* Short loop timed out, make a slower loop up to 1 second */
	dt = drv_hztousec(1);
	for (limit = 1000000;
	    ((regr(RBBM_STATUS) & MASK) < n) && (limit > 0); limit -= dt) {
		efb_delay(1);
	}

	if ((regr(RBBM_STATUS) & MASK) >= n) {
		return (DDI_SUCCESS);
	}

	cmn_err(CE_WARN, "efb: %s:%d: fifo timeout (%d), status=%x",
	    func, line, n, regr(RBBM_STATUS));

	return (DDI_FAILURE);
}


int
efb_wait_idle(efb_private_t *efb_priv, const char *func, int line)
{
	volatile caddr_t registers = efb_priv->registers;
	uint32_t	status;
	long		limit;
	long		dt;

	status = regr(RBBM_STATUS);

	/* If it's already idle, nothing to do */
	if ((status & MASK) >= 64 && !(status & GUI_ACTIVE)) {
		return (1);
	}

	if (efb_wait_fifo(efb_priv, 64, func, line) != DDI_SUCCESS) {

		/* OK, now reset the FIFO */
		if (efb_fifo_reset(efb_priv) != DDI_SUCCESS) {
			return (0);	/* This is really amazingly bad */
		}

		if (efb_wait_fifo(efb_priv, 64, func, line) != DDI_SUCCESS) {
			return (0);
		}
	}

	for (limit = 10000; (regr(RBBM_STATUS) & GUI_ACTIVE) && --limit > 0; ) {
		;
	}

	if (!(regr(RBBM_STATUS) & GUI_ACTIVE)) {
		return (1);
	}

	/* Short loop timed out, make a slower loop up to 3 seconds */
	dt = drv_hztousec(1);
	for (limit = 1000000;
	    (regr(RBBM_STATUS) & GUI_ACTIVE) && (limit > 0); limit -= dt) {
		efb_delay(1);
	}

	if (!(regr(RBBM_STATUS) & GUI_ACTIVE)) {
		return (1);
	}

	cmn_err(CE_WARN,
	    "efb: %s:%d: efb_wait_idle: idle timeout status=%x",
	    func, line, regr(RBBM_STATUS));

	if (efb_fifo_reset(efb_priv) != DDI_SUCCESS) {
		return (0);	/* This is really amazingly bad */
	}

	return (1);
}


int
efb_wait_host_data(efb_private_t *efb_priv, const char *func, int line)
{
	volatile caddr_t registers = efb_priv->registers;
	uint32_t	status;
	long		limit;
	long		dt;

	status = regr(RBBM_STATUS);

	/* If it's already idle, nothing to do */
	if ((status & MASK) >= 64 && !(status & GUI_ACTIVE)) {
		return (0);
	}

	if (efb_wait_fifo(efb_priv, 64, func, line) != DDI_SUCCESS) {

		/* OK, now reset the FIFO */
		if (efb_fifo_reset(efb_priv) != DDI_SUCCESS) {
			return (2);	/* This is really amazingly bad */
		}

		if (efb_wait_fifo(efb_priv, 64, func, line) != DDI_SUCCESS) {
			return (2);
		}
	}

	for (limit = 10000;
	    ((status = regr(RBBM_STATUS)) & GUI_ACTIVE) &&
	    ((status & 0x800200ff) != 0x80020040) &&
	    ((status & 0x800100ff) != 0x80010040) &&
	    limit > 0; limit--) {
		;
	}

	if (!(status & GUI_ACTIVE)) {
		return (0);
	}

	if ((status & 0x800200ff) == 0x80020040 ||
	    (status & 0x800100ff) == 0x80010040) {
		return (1);
	}

	/* Short loop timed out, make a slower loop up to 3 seconds */
	dt = drv_hztousec(1);
	for (limit = 1000000;
	    ((status = regr(RBBM_STATUS)) & GUI_ACTIVE) &&
	    ((status & 0x800200ff) != 0x80020040) &&
	    ((status & 0x800100ff) != 0x80010040) &&
	    limit > 0; limit -= dt) {
		efb_delay(1);
	}

	if (!(status & GUI_ACTIVE)) {
		return (0);
	}

	if ((status & 0x800200ff) == 0x80020040 ||
	    (status & 0x800100ff) == 0x80010040) {
		return (1);
	}
	cmn_err(CE_WARN,
	    "efb: %s:%d: efb_wait_idle: idle timeout status=%x",
	    func, line, regr(RBBM_STATUS));

	if (efb_fifo_reset(efb_priv) != DDI_SUCCESS) {
		return (2);	/* This is really amazingly bad */
	}

	return (0);
}



static int
efb_fifo_reset(efb_private_t *efb_priv)
{
	volatile caddr_t registers = efb_priv->registers;
	dev_info_t	*devi = efb_priv->dip;
	int		rval;
	uint32_t	s;

#define	peek2(a, v) ddi_peek32(devi, (int32_t *)registers + (a), (int32_t *)(v))
#define	poke2(a, v) ddi_poke32(devi, (int32_t *)registers + (a), (int32_t)(v))

	if ((rval = poke2(RBBM_SOFT_RESET,
	    RBBM_SOFT_RESET__SOFT_RESET_CP |
	    RBBM_SOFT_RESET__SOFT_RESET_E2)) != DDI_SUCCESS) {

		cmn_err(CE_WARN, "efb: bus error resetting fifo");

		return (rval);
	}

	/* dummy read to flush the write */
	if ((rval = peek2(RBBM_SOFT_RESET, &s)) != DDI_SUCCESS) {

		cmn_err(CE_WARN, "efb: bus error resetting fifo");
		return (rval);
	}

	if ((rval = poke2(RBBM_SOFT_RESET, 0)) != DDI_SUCCESS) {

		cmn_err(CE_WARN, "efb: bus error resetting fifo");
		return (rval);
	}

	if ((rval = peek2(RBBM_SOFT_RESET, &s)) != DDI_SUCCESS) {

		cmn_err(CE_WARN, "efb: bus error resetting fifo");
		return (rval);
	}

	return (rval);
}

void
efb_delay(clock_t ticks)
{
#if VIS_CONS_REV > 2
	extern int efb_in_polledio;

	if (efb_in_polledio) {
		if (ticks <= 0)
			ticks = 1;
		drv_usecwait(TICK_TO_USEC(ticks));
	} else {
		delay(ticks);
	}
#else
	delay(ticks);
#endif
}
