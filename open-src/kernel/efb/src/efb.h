/*
 * Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
 */

#ifndef _EFB_H
#define	_EFB_H

#include <sys/visual_io.h>
#include "drm_sunmod.h"
#include <sys/gfx_common.h>
#include "efb_vis.h"
#include "efb_reg.h"

#define	regr(a)			(*(volatile uint32_t *)(registers+(a)))
#define	regr8(a)		(*(volatile uint8_t *)(registers+(a)))
#define	regr16(a)		(*(volatile uint16_t *)(registers+(a)))

#define	regw(a, d)		(regr(a) = (d))
#define	regw8(a, d)		(regr8(a) = (d))

#define	EFB_REG_SIZE		(256 * 1024)
#define	EFB_CMAP_ENTRIES	256

#define	EFB_VRT_MASK		0x0000001f
#define	EFB_VRT_CMAP		0x00000002	/* colormap update	*/

/* Power */
#define	EFB_PM_COMPONENTS	2	/* number of components */
#define	EFB_PM_BOARD		0	/* component 0 : board */
#define	EFB_PM_MONITOR		1	/* component 1 : display */
#define	EFB_PM_MONITOR2		2	/* reserved */

/* Power levels */
#define	EFB_PWR_UNKNOWN		(-1)
#define	EFB_PWR_OFF		0
#define	EFB_PWR_SUSP		1
#define	EFB_PWR_STDBY		2
#define	EFB_PWR_ON		3

typedef struct efb_private efb_private_t;
typedef struct efb_context efb_context_t;
typedef struct efb_mapinfo efb_mapinfo_t;

struct efb_mapinfo {
	drm_inst_state_t	*mstate;
	efb_private_t		*efb_priv;
	devmap_cookie_t		*dhp;
	offset_t		off;
	size_t			len;
	efb_context_t		*ctx;
	efb_mapinfo_t		*next;
	efb_mapinfo_t		*prev;
};

struct efb_context {
	drm_inst_state_t	*mstate;
	void			*mappings;
	struct drm_radeon_driver_file_fields *md;
	uint32_t		default_pitch_offset;
	uint32_t		dst_pitch_offset;
	uint32_t		src_pitch_offset;
	uint32_t		default_sc_bottom_right;
	uint32_t		dp_gui_master_cntl;
	uint32_t		dst_line_start;
	uint32_t		dst_line_end;
	uint32_t		rb3d_cntl;
	uint32_t		dp_write_mask;
	uint32_t		dp_mix;
	uint32_t		dp_datatype;
	uint32_t		dp_cntl;
	uint32_t		src_y;
	uint32_t		src_x;
	uint32_t		dst_y;
	uint32_t		dst_x;
	uint16_t		colormap[3][EFB_CMAP_ENTRIES];

	efb_context_t		*next;
};


struct efb_private {
	dev_info_t		*dip;
	ddi_acc_handle_t	pci_handle;
	uint_t			flags;
	volatile caddr_t	registers;
	ddi_acc_handle_t	registersmap;
	int			w[2];
	int			h[2];
	int			depth[2];
	int			stride[2];

	uint16_t		colormap[4][3][EFB_CMAP_ENTRIES];
	uint8_t			cmap_flags[2][EFB_CMAP_ENTRIES];
	int			power_level[3];
	int			primary_stream;

	struct gfx_video_mode	videomode;

	struct video_state {
	    uint32_t		clock_cntl_index;
	    uint32_t		crtc_ext_cntl;
	    uint32_t		crtc_gen_cntl;
	    uint32_t		crtc_h_sync_strt_wid;
	    uint32_t		crtc_h_total_disp;
	    uint32_t		crtc_v_sync_strt_wid;
	    uint32_t		crtc_v_total_disp;
	    uint32_t		crtc_offset;
	    uint32_t		crtc_offset_cntl;
	    uint32_t		crtc_pitch;
	    uint32_t		dac_cntl;
	    uint32_t		dac_macro_cntl;
	    uint32_t		crtc2_gen_cntl;
	    uint32_t		crtc2_h_sync_strt_wid;
	    uint32_t		crtc2_h_total_disp;
	    uint32_t		crtc2_v_sync_strt_wid;
	    uint32_t		crtc2_v_total_disp;
	    uint32_t		crtc2_offset;
	    uint32_t		crtc2_offset_cntl;
	    uint32_t		crtc2_pitch;
	    uint32_t		dac_cntl2;
	    uint32_t		fp_gen_cntl;
	    uint32_t		fp_h_sync_strt_wid;
	    uint32_t		fp_h2_sync_strt_wid;
	    uint32_t		fp_v_sync_strt_wid;
	    uint32_t		fp_v2_sync_strt_wid;
	    uint32_t		lvds_gen_cntl;
	    uint32_t		lvds_pll_cntl;
	    uint32_t		tmds_cntl;
	    uint32_t		tmds_pll_cntl;
	    uint32_t		tmds_transmitter_cntl;
	    uint32_t		tv_dac_cntl;
	    uint32_t		disp_hw_debug;

	    /* DAC registers */
	    uint32_t		htotal_cntl;
	    uint32_t		htotal2_cntl;
	    uint32_t		pixclks_cntl;
	    uint32_t		ppll_cntl;
	    uint32_t		ppll_div_0;
	    uint32_t		ppll_ref_div;
	    uint32_t		p2pll_cntl;
	    uint32_t		p2pll_div_0;
	    uint32_t		p2pll_ref_div;
	    uint32_t		clk_pwrmgt_cntl;
	} saved_video_state;

	efb_context_t		*contexts;
	void			*cur_ctx;

#if VIS_CONS_REV > 2
	struct efb_consinfo	consinfo;
	int			setting_videomode;
#endif

};

extern void	efb_init(void *, struct cb_ops *, drm_driver_t *);
extern int	efb_attach(drm_device_t *);
extern void	efb_detach(drm_device_t *);
extern int	efb_ioctl(dev_t, int, intptr_t, int, cred_t *, int *);
extern int	efb_map_registers(drm_device_t *);
extern void	efb_unmap_registers(drm_device_t *);
extern void	efb_delay(clock_t);

extern void	efb_getsize(efb_private_t *);
extern void	efb_cmap_write(efb_private_t *, int);
extern void	efb_cmap_read(efb_private_t *, int, int, int);

extern int	efb_wait_fifo(efb_private_t *, int, const char *, int);
extern int	efb_wait_idle(efb_private_t *, const char *, int);
extern int	efb_wait_host_data(efb_private_t *, const char *, int);

extern int	efb_vis_devinit(drm_device_t *, caddr_t, int);
extern int	efb_vis_devfini(drm_device_t *, caddr_t, int);
extern int	efb_vis_consdisplay(drm_device_t *, caddr_t, int);
extern int	efb_vis_conscopy(drm_device_t *, caddr_t, int);
extern int	efb_vis_conscursor(drm_device_t *, caddr_t, int);
extern int	efb_vis_putcmap(drm_device_t *, caddr_t, int);
extern void	efb_termemu_callback(drm_device_t *);

extern void *	efb_devmap_set_callbacks(int);

extern int	efb_ctx_make_current(efb_private_t *efb_priv,
		    efb_context_t *ctx);
extern void	efb_ctx_wait(efb_private_t *);
extern void	efb_ctx_save(efb_private_t *, efb_context_t *);

#endif /* _EFB_H */
