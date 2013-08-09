/*
 * Copyright (c) 2006, 2013, Oracle and/or its affiliates. All rights reserved.
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

#ifndef _LIBVTSSUNWEFB_H
#define	_LIBVTSSUNWEFB_H

#include <sys/types.h>
#include <sys/param.h>		/* MAXPATHLEN */
#include <errno.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <stropts.h>		/* ioctl() */
#include <sys/fbio.h>
#include <sys/mman.h>
#include <sys/systeminfo.h>	/* sysinfo() */
#include <sys/pci.h>
#include <sys/time.h>
#include <sys/visual_io.h>
#include <X11/Xmd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/dpms.h>
#include "libvtsSUNWxfb.h"	/* Common VTS library definitions */
#undef Status
#define	Status pciStatus
#include "gfx_common.h"
#undef Status
#define	Status int
#include "graphicstest.h"
#include "gfx_vts.h"

typedef struct efb_info {
	char const *efb_name;
	int efb_fd;
	uint16_t efb_vendor;
	uint16_t efb_device;
	uint_t efb_width;
	uint_t efb_height;
	uint_t efb_depth;
	uint_t efb_pixelsize;
	uint_t efb_linesize;
	offset_t efb_fb_addr;
	size_t efb_fb_size;
	uchar_t *efb_fb_ptr;
	offset_t efb_mmio_addr;
	size_t efb_mmio_size;
	uchar_t *efb_mmio_ptr;
	int efb_palet_changed;
	uint32_t efb_palet[256];
	} efb_info_t;

typedef struct efb_xw_struct {
	sigset_t xw_procmask;
	sigjmp_buf xw_sigjmpbuf;
	char const *xw_dispname;
	Display *xw_display;
	int xw_screen;
	Window xw_window;
	Cursor xw_cursor;
	Bool xw_grab_server;
	Bool xw_grab_keyboard;
	Bool xw_grab_pointer;
	Bool xw_ss_saved;
	Bool xw_ss_disabled;
	int xw_ss_timeout;
	int xw_ss_interval;
	int xw_ss_prefer_blanking;
	int xw_ss_allow_exposures;
	Status xw_dpms_saved;
	CARD16 xw_dpms_power;
	BOOL xw_dpms_state;
	Bool xw_dpms_disabled;
	} efb_xw_t;

#ifdef __cplusplus
extern "C" {
#endif

/* chip.c */

extern return_packet *efb_test_chip(
    int const fd);

extern int chip_test(
    return_packet *const rp,
    int const fd);

extern int draw_lines(
    uint_t const width,
    uint_t const height);

extern int draw_cascaded_box(
    uint_t const width,
    uint_t const height);

/* mapper.c */

extern return_packet *efb_test_open(
    int const fd);

extern int map_me(
    return_packet *const rp,
    int const fd);

extern int efb_test_semaphore(
    return_packet *const rp,
    int const test);

/* memory.c */

extern return_packet *efb_test_memory(
    int const fd);

extern int memory_test(
    return_packet *const rp,
    int const fd);

extern void check_plane(
    int const num_planes,
    int const access_mode,
    int const fb_pitch,
    int const fb_height,
    int const fb_width,
    int const bytepp,
    caddr_t const base);

extern void init_data(
    int const num_planes);

extern uint_t test_data(
    void);

extern boolean_t write_read(
    int const xoff,
    int const yoff,
    boolean_t const complement,
    int const access_mode,
    boolean_t const pass,
    int const fb_pitch,
    int const bytepp,
    caddr_t const base);

/* tools.c */

extern int efb_map_mem(
    return_packet *const rp,
    int const test);

extern int efb_get_pci_info(
    void);

extern int efb_map_mmio(
    void);

extern int efb_map_fb(
    void);

extern int efb_init_info(
    return_packet *const rp,
    int const test);

extern int efb_unmap_mem(
    return_packet *const rp,
    int const test);

extern int efb_unmap_fb(
    void);

extern int efb_unmap_mmio(
    void);

extern int efb_init_graphics(
    void);

extern void efb_save_palet(
    void);

extern int efb_set_palet(
    void);

extern int efb_restore_palet(
    void);

extern uint_t efb_color(
    uint_t const red,
    uint_t const green,
    uint_t const blue);

extern int efb_fill_solid_rect(
    uint_t const x1,
    uint_t const y1,
    uint_t const x2,
    uint_t const y2,
    uint_t const fg);

extern int efb_draw_solid_line(
    uint_t const x1,
    uint_t const y1,
    uint_t const x2,
    uint_t const y2,
    uint_t const fg);

extern int efb_flush_pixel_cache(
    void);

extern void efb_reset_engine(
    void);

extern int efb_wait_fifo(
    int const c);

extern int efb_wait_idle(
    void);

/* libvtsSUNWefb.c */

extern efb_info_t efb_info;

extern efb_xw_t efb_xw;

extern void efb_block_signals(
    void);

extern void efb_restore_signals(
    void);

extern int efb_lock_display(
    void);

extern int efb_unlock_display(
    void);

extern int efb_open_display(
    void);

extern int efb_create_cursor(
    void);

extern int efb_create_window(
    void);

extern int efb_grab_server(
    void);

extern int efb_ungrab_server(
    void);

extern int efb_grab_keyboard(
    void);

extern int efb_ungrab_keyboard(
    void);

extern int efb_grab_pointer(
    void);

extern int efb_ungrab_pointer(
    void);

extern int efb_disable_screensaver(
    void);

extern int efb_restore_screensaver(
    void);

extern int efb_disable_dpms(
    void);

extern int efb_restore_dpms(
    void);

extern int efb_sleep(
    uint_t const seconds);

extern void efb_signal_routine(
    int const signo);

extern int efb_check_for_interrupt(
    void);

extern void graphicstest_finish(
    int const flag);

/* dma.c */

extern return_packet *efb_test_dma(
    int const fd);

#ifdef __cplusplus
}
#endif

#endif	/* _LIBVTSSUNWEFB_H */

/* End of libvtsSUNWefb.h */
