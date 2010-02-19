/* Copyright 1993 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * of the copyright holder.
 */


/* 
** draw_db.c - Drawable veneer for DGA window buffer control routines
*/

#ifdef SERVER_DGA
#include <X11/Xlib.h>
#endif  /* SERVER_DGA */
#include "dga_incls.h"

int
dga_draw_db_grab (Dga_drawable dgadraw, int nbuffers, 
		  int (*vrtfunc)(Dga_drawable), u_int *vrtcounterp)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    return (0);
	}
	return (dga_db_grab((Dga_window)dgawin, nbuffers, vrtfunc, vrtcounterp));
    }
    case DGA_DRAW_PIXMAP:
	return (0);
    }
}

int
dga_draw_db_ungrab (Dga_drawable dgadraw)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    return (0);
	}
	return (dga_db_ungrab((Dga_window)dgawin));
    }
    case DGA_DRAW_PIXMAP:
	return (0);
    }
}

void
dga_draw_db_write (Dga_drawable dgadraw, int buffer,
		   int (*writefunc)(void*, Dga_drawable, int), 
		   void *data)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    break;
	}
	dga_db_write((Dga_window)dgawin, buffer, writefunc, data);
	break;
    }
    case DGA_DRAW_PIXMAP:
	break;
    }
}

void
dga_draw_db_read (Dga_drawable dgadraw, int buffer,
		  int (*readfunc)(void*, Dga_drawable, int), 
		  void *data)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    break;
	}
	dga_db_read((Dga_window)dgawin, buffer, readfunc, data);
	break;
    }
    case DGA_DRAW_PIXMAP:
	break;
    }
}

void
dga_draw_db_display (Dga_drawable dgadraw, int buffer,
		    int (*visfunc)(void*, Dga_drawable, int), 
		     void *data)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    break;
	}
	dga_db_display((Dga_window)dgawin, buffer, visfunc, data);
	break;
    }
    case DGA_DRAW_PIXMAP:
	break;
    }
}

void
dga_draw_db_interval (Dga_drawable dgadraw, int interval)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    break;
	}
	dga_db_interval((Dga_window)dgawin, interval);
	break;
    }
    case DGA_DRAW_PIXMAP:
	break;
    }
}

void
dga_draw_db_interval_wait (Dga_drawable dgadraw)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    break;
	}
	dga_db_interval_wait((Dga_window)dgawin);
	break;
    }
    case DGA_DRAW_PIXMAP:
	break;
    }
}

int
dga_draw_db_interval_check (Dga_drawable dgadraw)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    /* not applicable to multibuffers - consider interval always expired */
	    return (1);
	}
	return (dga_db_interval_check((Dga_window)dgawin));
    }
    case DGA_DRAW_PIXMAP:
	/* not applicable to pixmaps - consider interval always expired */
	return (1);
    }
}

int
dga_draw_db_write_inquire (Dga_drawable dgadraw)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    /* not applicable to multibuffers - always return an invalid buffer index */
	    return (-1);
	}
	return (dga_db_write_inquire((Dga_window)dgawin));
    }
    case DGA_DRAW_PIXMAP:
	/* not applicable to pixmaps - always return an invalid buffer index */
	return (-1);
    }
}

int
dga_draw_db_read_inquire (Dga_drawable dgadraw)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    /* not applicable to multibuffers - always return an invalid buffer index */
	    return (-1);
	}
	return (dga_db_read_inquire((Dga_window)dgawin));
    }
    case DGA_DRAW_PIXMAP:
	/* not applicable to pixmaps - always return an invalid buffer index */
	return (-1);
    }
}

int
dga_draw_db_display_inquire (Dga_drawable dgadraw)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    /* not applicable to multibuffers - always return an invalid buffer index */
	    return (-1);
	}
	return (dga_db_display_inquire((Dga_window)dgawin));
    }
    case DGA_DRAW_PIXMAP:
	/* not applicable to pixmaps - always return an invalid buffer index */
	return (-1);
    }
}

int
dga_draw_db_display_done (Dga_drawable dgadraw, int flag, 
			  int (*display_done_func)(Dga_drawable))
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    /* not applicable to multibuffers - consider display done (to prevent hangs) */
	    return (1);
	}
	return (dga_db_display_done((Dga_window)dgawin, flag, display_done_func));
    }
    case DGA_DRAW_PIXMAP:
	/* not applicable to pixmaps - consider display done (to prevent hangs) */
	return (1);
    }
}

Dga_dbinfo *
dga_draw_db_dbinfop (Dga_drawable dgadraw)
{
    switch ( ((_Dga_drawable)dgadraw)->drawable_type) {
    case DGA_DRAW_WINDOW: {
	_Dga_window dgawin = (_Dga_window)dgadraw;
	if (!DGA_LOCKSUBJ_WINDOW(dgawin, dgawin->eLockSubj)) {
	    /* not applicable to multibuffers - return NULL */
	    return (NULL);
	}
	return (dga_win_dbinfop((Dga_window)dgawin));
    }
    case DGA_DRAW_PIXMAP:
	/* not applicable to pixmaps - return NULL */
	return (NULL);
    }
}
