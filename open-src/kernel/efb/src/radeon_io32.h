/*
 * Copyright (c) 2008, Oracle and/or its affiliates. All rights reserved.
 */

#ifndef __RADEON_IO32_H__
#define	__RADEON_IO32_H__


#ifdef	_MULTI_DATAMODEL
/*
 * For radeon_cp_init()
 */
typedef struct drm_radeon_init_32 {
	int	func;
	unsigned int sarea_priv_offset;
	int is_pci; /* for overriding only */
	int cp_mode;
	int gart_size;
	int ring_size;
	int usec_timeout;

	unsigned int fb_bpp;
	unsigned int front_offset, front_pitch;
	unsigned int back_offset, back_pitch;
	unsigned int depth_bpp;
	unsigned int depth_offset, depth_pitch;

	unsigned int fb_offset DEPRECATED;
	unsigned int mmio_offset DEPRECATED;
	unsigned int ring_offset;
	unsigned int ring_rptr_offset;
	unsigned int buffers_offset;
	unsigned int gart_textures_offset;
} drm_radeon_init_32_t;

/*
 * radeon_cp_buffers()
 */
typedef struct drm_dma_32 {
	int		context;
	int		send_count;
	uint32_t	send_indices;
	uint32_t	send_sizes;
	drm_dma_flags_t flags;
	int		request_count;
	int		request_size;
	uint32_t	request_indices;
	uint32_t	request_sizes;
	int granted_count;
} drm_dma_32_t;

/*
 * drm_radeon_clear()
 */
typedef	struct drm_radeon_clear_32 {
	unsigned int	flags;
	unsigned int	clear_color;
	unsigned int	clear_depth;
	unsigned int	color_mask;
	unsigned int	depth_mask;
	uint32_t	depth_boxes;
} drm_radeon_clear_32_t;

/*
 * For radeon_cp_texture()
 */
typedef struct drm_radeon_tex_image_32 {
	unsigned int	x, y;
	unsigned int	width, height;
	uint32_t	data;
} drm_radeon_tex_image_32_t;

typedef struct drm_radeon_texture_32 {
	unsigned int offset;
	int		pitch;
	int		format;
	int		width;
	int		height;
	uint32_t	image;
} drm_radeon_texture_32_t;

/*
 * for radeon_cp_stipple()
 */
typedef struct drm_radeon_stipple_32 {
	uint32_t mask;
} drm_radeon_stipple_32_t;

/*
 * radeon_cp_vertex2()
 */
typedef struct drm_radeon_vertex2_32 {
	int		idx;
	int		discard;
	int		nr_states;
	uint32_t	state;
	int 	nr_prims;
	uint32_t prim;
} drm_radeon_vertex2_32_t;

/*
 * radeon_cp_cmdbuf()
 */
typedef struct drm_radeon_kcmd_buffer_32 {
	int 	bufsz;
	uint32_t buf;
	int 	nbox;
	uint32_t boxes;
} drm_radeon_kcmd_buffer_32_t;

/*
 * radeon_cp_getparam()
 */
typedef struct drm_radeon_getparam_32 {
	int 	param;
	uint32_t value;
} drm_radeon_getparam_32_t;


/*
 * radeon_mem_alloc()
 */
typedef struct drm_radeon_mem_alloc_32 {
	int 	region;
	int 	alignment;
	int 	size;
	uint32_t region_offset;	/* offset from start of fb or GART */
} drm_radeon_mem_alloc_32_t;


/*
 * radeon_irq_emit()
 */
typedef struct drm_radeon_irq_emit_32 {
	uint32_t 	irq_seq;
} drm_radeon_irq_emit_32_t;


/*
 * radeon_cp_setparam()
 */
#pragma pack(1)
typedef struct drm_radeon_setparam_32 {
	unsigned int param;
	uint64_t 	value;
} drm_radeon_setparam_32_t;
#pragma pack()

#endif	/* _MULTI_DATAMODEL */
#endif	/* __RADEON_IO32_H__ */
