/*
 * Copyright (c) 1993, 2002, Oracle and/or its affiliates. All rights reserved.
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
 *
 */


#include <sys/types.h>
#include <sys/param.h>
#include <sys/signal.h>
#include <sys/stream.h>
#include <sys/stropts.h>
#include <sys/kmem.h>
#ifndef _DDICT
#include <sys/vuid_event.h>
#else
#include "vuid_event.h"
#endif /* _DDICT */
#include <sys/modctl.h>
#include <sys/cmn_err.h>
#ifndef _DDICT
#include <sys/fbio.h>
#else
#include "fbio.h"
#endif /* _DDICT */
#include <sys/ddi.h>
#include <sys/file.h>
#include <sys/sunddi.h>
#include <sys/systm.h>

#include "hwcio.h"

/* NOTE: datamodel size-invariant ILP32/LP64 */
struct msctrl {		/* same as R5 PtrCtrl struct */
	int num;
	int den;
	int threshold;
	unsigned char id;
};

/* NOTE: datamodel size-invariant ILP32/LP64 */
struct hwcrec {
	struct fbcurpos		cursor_position;
	struct hwc_limits	limits;
	struct msctrl		ctrl;
	dev_t			screen_dev_t;
	int			hwc_enabled;
	int			at_border;
};


static int hwcopen(queue_t *q, dev_t *dev, int oflag, int sflag, cred_t *credp);
static int hwcclose(queue_t *);
static int hwcwput(queue_t *, mblk_t *);
static int hwcrput(queue_t *, mblk_t *);
static void hwcioctl(queue_t *, mblk_t *);
static void hwciocdata(queue_t *, mblk_t *);
static void hwc_spos(queue_t *q, mblk_t *mp);
static void hwc_scursor(queue_t *q, mblk_t *mp);
#ifndef _DDICT
static void hwc_set_cursor_pos(dev_t, struct fbcurpos *);
static void hwc_set_cursor(dev_t, struct fbcursor *);
#endif /* _DDICT */
static void hwc_state_free(struct copyresp *s);

static struct module_info hwcmiinfo = {
	0,
	"hwc",
	0,
	INFPSZ,
	2048,
	128
};

static struct qinit hwcrinit = {
	hwcrput,
	(int (*)())NULL,
	hwcopen,
	hwcclose,
	(int (*)())NULL,
	&hwcmiinfo,
	NULL
};

static struct module_info hwcmoinfo = {
	0,
	"hwc",
	0,
	INFPSZ,
	2048,
	128
};

static struct qinit hwcwinit = {
	hwcwput,
	(int (*)())NULL,
	hwcopen,
	hwcclose,
	(int (*)())NULL,
	&hwcmoinfo,
	NULL
};

static struct streamtab hwc_info = {
	&hwcrinit,
	&hwcwinit,
	NULL,
	NULL,
};

static struct fmodsw fsw = {
	"hwc",
	&hwc_info,
	(D_NEW|D_MP|D_MTPERMOD)
};

/*
 * Module linkage information for the kernel.
 */
extern struct mod_ops mod_strmodops;

static struct modlstrmod modlstrmod = {
	&mod_strmodops, "streams module for hardware cursor support", &fsw
};

static struct modlinkage modlinkage = {
	MODREV_1, &modlstrmod, NULL
};

int
_init(void)
{
	return (mod_install(&modlinkage));
}

int
_info(struct modinfo *modinfop)
{
	return (mod_info(&modlinkage, modinfop));
}

int
_fini(void)
{
	return (mod_remove(&modlinkage));
}


#define	ABS(x)	((x) < 0 ? -(x) : (x))

#define	ACKNAK(q, mp, iocp, type) {		\
	mp->b_datap->db_type = type;		\
	iocp->ioc_count = 0;			\
	qreply(q, mp);				\
	}
#define	IOCACK(q, mp, iocp)	ACKNAK(q, mp, iocp, M_IOCACK)
#define	IOCNAK(q, mp, iocp)	ACKNAK(q, mp, iocp, M_IOCNAK)

#define	CKTRANSPARENT(q, mp)			\
	if (iocp->ioc_count != TRANSPARENT) {	\
		if (mp->b_cont) {		\
			freemsg(mp->b_cont);	\
			mp->b_cont = NULL;	\
		}				\
		mp->b_datap->db_type = M_IOCNAK; \
		qreply(q, mp);			\
		return;				\
	}

#define	COPYIN(q, mp, cqp, cpsize) {		\
	cqp = (struct copyreq *)mp->b_rptr;	\
	cqp->cq_addr = (caddr_t)*(long *)mp->b_cont->b_rptr; \
	if (mp->b_cont)				\
		freemsg(mp->b_cont);		\
	mp->b_cont = NULL;			\
	cqp->cq_size = cpsize;			\
	cqp->cq_flag = 0;			\
	mp->b_datap->db_type = M_COPYIN;	\
	mp->b_wptr = mp->b_rptr + sizeof (struct copyreq); \
	qreply(q, mp);				\
	}

#define	COPYACK(q, mp, iocp) {			\
	iocp->ioc_error = 0;			\
	iocp->ioc_rval = 0;			\
	if (mp->b_cont)				\
		freemsg(mp->b_cont);		\
	mp->b_cont = NULL;			\
	IOCACK(q, mp, iocp);			\
	}

#define	COPYNACK(q, mp, iocp) {			\
	iocp->ioc_error = 0;			\
	iocp->ioc_rval = 0;			\
	if (mp->b_cont)				\
		freemsg(mp->b_cont);		\
	mp->b_cont = NULL;			\
	IOCNAK(q, mp, iocp);			\
	}

/*bugid: 4338558 hwc mem. leaks */

#define	CK_RVAL(csp,mp)	\
	if (csp->cp_rval) {			\
		hwc_state_free(csp); \
		freemsg(mp);	\
		return;				\
	}


/*ARGSUSED*/
static int
hwcopen(queue_t *q, dev_t *dev, int oflag, int sflag, cred_t *credp)
{
	struct hwcrec *hwcptr;

	if (q->q_ptr != NULL)
		return (0);		/* already attached */

	if (!groupmember(0, ddi_get_cred()))
		return (EPERM);

	if ((hwcptr = kmem_alloc(sizeof (struct hwcrec), KM_SLEEP)) == NULL)
		return (ENOMEM);

	hwcptr->hwc_enabled = 0;
	hwcptr->at_border = 0;
	hwcptr->ctrl.num = 2;
	hwcptr->ctrl.den = 1;
	hwcptr->ctrl.threshold = 4;

	/* save the instance pointer in private data  */
	q->q_ptr = (void *)hwcptr;
	WR(q)->q_ptr = (void *)hwcptr;

	qprocson(q);

	return (0);
}


static int
hwcclose(queue_t *q)
{
	struct hwcrec *hwcptr = (struct hwcrec *)q->q_ptr; /* instance data */

	qprocsoff(q);

#ifndef _DDICT
	if (hwcptr->hwc_enabled) {
		struct fbcursor fbcursor;

		fbcursor.set = FB_CUR_SETCUR;
		fbcursor.enable = 0;
		hwc_set_cursor(hwcptr->screen_dev_t, &fbcursor);
	}
#endif /* _DDICT */

	/* free the per-instance data */
	kmem_free(hwcptr, sizeof (*hwcptr));
	q->q_ptr = NULL;
	WR(q)->q_ptr = NULL;

	return (0);
}


/*
 * read queue put procedure.
 */
static int
hwcrput(queue_t *q, mblk_t *mp)
{
	static Firm_event *ev;
	struct hwcrec *hwcptr = (struct hwcrec *)q->q_ptr; /* instance data */

	switch (mp->b_datap->db_type) {
	case M_FLUSH:
		if (*mp->b_rptr & FLUSHW)
			flushq(WR(q), FLUSHDATA);
		if (*mp->b_rptr & FLUSHR)
			flushq(q, FLUSHDATA);
		putnext(q, mp);
		return (0);

	case M_DATA:
		ev = (Firm_event *)mp->b_rptr;

		if (ev->id == LOC_X_DELTA) {
			int tmpx;

			/* check for acceleration threshold */
			if (ABS(ev->value) > hwcptr->ctrl.threshold)
				tmpx = hwcptr->cursor_position.x +
				    ((ev->value * hwcptr->ctrl.num) /
				    hwcptr->ctrl.den);
			else
				tmpx = hwcptr->cursor_position.x + ev->value;

			/* bound cursor to screen */
			if (tmpx >= hwcptr->limits.x2) {
				if (hwcptr->limits.confined) {
					tmpx = hwcptr->limits.x2 - 1;

					/*
					 * if the cursor was already at
					 * the limit just toss event.
					 */
					if (hwcptr->cursor_position.x == tmpx) {
						freemsg(mp);
						return (0);
					}
					hwcptr->cursor_position.x = (short)tmpx;
				} else {
					/*
					 * If we're already on an X
					 * boundary of the screen, only
					 * send change border events
					 * until server has acknowledged
					 * crossing.
					 */
					if (hwcptr->at_border&0x1) {
						freemsg(mp);
						return (0);
					}

					hwcptr->cursor_position.x =
					    hwcptr->limits.x2;
					hwcptr->at_border |= 0x1;
				}
			} else if (tmpx < hwcptr->limits.x1) {
				if (hwcptr->limits.confined) {
					if (hwcptr->cursor_position.x ==
					    hwcptr->limits.x1) {
						freemsg(mp);
						return (0);
					}
					hwcptr->cursor_position.x =
					    hwcptr->limits.x1;
				} else {
					if (hwcptr->at_border&0x1) {
						freemsg(mp);
						return (0);
					}
					hwcptr->cursor_position.x = -1;
					hwcptr->at_border |= 0x1;
				}
			} else {
				hwcptr->cursor_position.x = (short)tmpx;
				hwcptr->at_border &= ~0x1;
			}

			if (ev->value) {
				ev->id = LOC_X_ABSOLUTE;
				ev->value = hwcptr->cursor_position.x;
#ifndef _DDICT
				if (hwcptr->hwc_enabled)
					hwc_set_cursor_pos(hwcptr->screen_dev_t,
					    &hwcptr->cursor_position);
#endif /* _DDICT */
			}
		}

		if (ev->id == LOC_Y_DELTA) {
			int tmpy;

			/* need to invert y-axis */
			if (ABS(ev->value) > hwcptr->ctrl.threshold)
				tmpy = hwcptr->cursor_position.y -
				    ((ev->value * hwcptr->ctrl.num) /
				    hwcptr->ctrl.den);
			else
				tmpy = hwcptr->cursor_position.y - ev->value;

			if (tmpy >= hwcptr->limits.y2) {
				if (hwcptr->limits.confined) {
					tmpy = hwcptr->limits.y2 - 1;

					/*
					 * if the cursor was already at
					 * the limit just toss event.
					 */
					if (hwcptr->cursor_position.y == tmpy) {
						freemsg(mp);
						return (0);
					}
					hwcptr->cursor_position.y = (short)tmpy;
				} else {
					if (hwcptr->at_border&0x2) {
						freemsg(mp);
						return (0);
					}
					hwcptr->cursor_position.y =
					    hwcptr->limits.y2;
					hwcptr->at_border |= 0x2;
				}
			} else if (tmpy < hwcptr->limits.y1) {
				if (hwcptr->limits.confined) {
					if (hwcptr->cursor_position.y ==
					    hwcptr->limits.y1) {
						freemsg(mp);
						return (0);
					}
					hwcptr->cursor_position.y =
					    hwcptr->limits.y1;
				} else {
					if (hwcptr->at_border&0x2) {
						freemsg(mp);
						return (0);
					}
					hwcptr->cursor_position.y = -1;
					hwcptr->at_border |= 0x2;
				}
			} else {
				hwcptr->cursor_position.y = (short)tmpy;
				hwcptr->at_border &= ~0x2;
			}

			if (ev->value) {
				ev->id = LOC_Y_ABSOLUTE;
				ev->value = hwcptr->cursor_position.y;
#ifndef _DDICT
				if (hwcptr->hwc_enabled)
					hwc_set_cursor_pos(hwcptr->screen_dev_t,
					    &hwcptr->cursor_position);
#endif /* _DDICT */
			}
		}

		(void) putnext(q, mp);
		return (0);
	default:
		(void) putnext(q, mp);
		return (0);
	}
}


/*
 * Line discipline output queue put procedure: handles M_IOCTL
 * messages.
 */
static int
hwcwput(queue_t *q, mblk_t *mp)
{
	switch (mp->b_datap->db_type) {
	case M_FLUSH:
		if (*mp->b_rptr & FLUSHW)
			flushq(q, FLUSHDATA);
		if (*mp->b_rptr & FLUSHR)
			flushq(RD(q), FLUSHDATA);
		putnext(q, mp);
		break;

	case M_IOCTL:		/* process user ioctl() */
		hwcioctl(q, mp);
		break;

	case M_IOCDATA:		/* retrieve ioctl() data */
		hwciocdata(q, mp);
		break;

	default:
		(void) putnext(q, mp);	/* pass it down the line */
		break;
	}
	return (0);
}


static void
hwcioctl(queue_t *q, mblk_t *mp)
{
	struct iocblk *iocp;
	struct copyreq *cqp;
	mblk_t	*tmp;
	struct hwc_state *stp;

	iocp = (struct iocblk *)mp->b_rptr;

	switch (iocp->ioc_cmd) {
	case HWCGVERSION: {	/* Get the HWC module version */
		int transparent = 0;

		if (iocp->ioc_count == TRANSPARENT) {
			transparent = 1;
			cqp = (struct copyreq *)mp->b_rptr;
			cqp->cq_size = sizeof (int);
			cqp->cq_addr = (caddr_t)*(long *)mp->b_cont->b_rptr;
			cqp->cq_flag = 0;
		}
		if (mp->b_cont)
			freemsg(mp->b_cont); /* over written below */
		if ((mp->b_cont = allocb(sizeof (int), BPRI_MED)) == NULL) {
			mp->b_datap->db_type = M_IOCNAK;
			iocp->ioc_error = EAGAIN;
			qreply(q, mp);
			break;
		}

		*(int *)mp->b_cont->b_wptr = HWCVERSION;
		mp->b_cont->b_wptr += sizeof (int);
		if (transparent) {
			mp->b_cont->b_datap->db_type = M_DATA;
			mp->b_datap->db_type = M_COPYOUT;
			mp->b_wptr = mp->b_rptr + sizeof (struct copyreq);
		} else {
			mp->b_datap->db_type = M_IOCACK;
			iocp->ioc_count = sizeof (int);
		}
		qreply(q, mp);
		break;
	}

	case HWCSPOS:		/* Set Cursor Position */
		CKTRANSPARENT(q, mp);
		COPYIN(q, mp, cqp, sizeof (struct fbcurpos));
		break;

	case HWCSCURSOR:	/* Set Cursor Image */
	case FBIOSCURSOR:
		CKTRANSPARENT(q, mp);
		tmp = allocb(sizeof (struct hwc_state), BPRI_MED);
		if (tmp == NULL) {
			mp->b_datap->db_type = M_IOCNAK;
			iocp->ioc_error = EAGAIN;
			qreply(q, mp);
			break;
		}
		tmp->b_wptr += sizeof (struct hwc_state);
		stp = (struct hwc_state *)tmp->b_rptr;
		stp->st_state = GETFBCURSOR;
/*
	bugid:  4500611 (P1/S1) USB mouse crashed
*/
		stp->image = NULL;
                stp->mask = NULL;
                stp->cmap_red = NULL;
                stp->cmap_green = NULL;
                stp->cmap_blue = NULL;

		cqp = (struct copyreq *)mp->b_rptr;
		cqp->cq_private = tmp;

		cqp->cq_addr = (caddr_t)*(uintptr_t *)mp->b_cont->b_rptr;

#ifdef _SYSCALL32_IMPL
		if ((iocp->ioc_flag & DATAMODEL_MASK) != DATAMODEL_NATIVE)
			cqp->cq_size = sizeof (struct fbcursor32);
		else
#endif
		cqp->cq_size = sizeof (struct fbcursor);

		cqp->cq_flag = 0;
		freemsg(mp->b_cont);
		mp->b_cont = NULL;
		mp->b_datap->db_type = M_COPYIN;
		mp->b_wptr = mp->b_rptr + sizeof (struct copyreq);
		qreply(q, mp);
		break;

	case HWCSSPEED:		/* Set Cursor Acceleration */
		CKTRANSPARENT(q, mp);
		COPYIN(q, mp, cqp, sizeof (struct msctrl));
		break;

	case HWCSSCREEN:	/* Set Framebuffer */
		CKTRANSPARENT(q, mp);

#ifdef _SYSCALL32_IMPL
		if ((iocp->ioc_flag & DATAMODEL_MASK) != DATAMODEL_NATIVE) {
			COPYIN(q, mp, cqp, sizeof (dev32_t));
		} else
#endif
		{
			COPYIN(q, mp, cqp, sizeof (dev_t));
		}
		break;

	case HWCENABLE:		/* Enable cursor */
		CKTRANSPARENT(q, mp);
		COPYIN(q, mp, cqp, sizeof (int));
		break;

	case HWCSBOUND:		/* Set Cursor Boundary */
		CKTRANSPARENT(q, mp);
		COPYIN(q, mp, cqp, sizeof (struct hwc_limits));
		break;

	default:		/* pass it down the line */
		(void) putnext(q, mp);
		break;
	}
}


static void
hwciocdata(queue_t *q, mblk_t *mp)
{
	struct iocblk *iocp;
	struct hwcrec *hwcptr = (struct hwcrec *)q->q_ptr; /* instance data */
	caddr_t data;
	struct copyresp *csp;

	csp = (struct copyresp *)mp->b_rptr;
	iocp = (struct iocblk *)mp->b_rptr;

	if (mp->b_cont != NULL)
		data = (caddr_t)mp->b_cont->b_rptr;

	switch (csp->cp_cmd) {
	case HWCGVERSION:	/* Get hwc version number */
		CK_RVAL(csp,mp);
		COPYACK(q, mp, iocp);
		break;

	case HWCSPOS:		/* Set Cursor Position */
		CK_RVAL(csp,mp);
		hwc_spos(q, mp);
		break;

	case HWCSCURSOR:	/* Set Cursor Image */
	case FBIOSCURSOR:
		CK_RVAL(csp,mp);
		hwc_scursor(q, mp);
		break;

	case HWCSSPEED:		/* Set Cursor Acceleration */
		CK_RVAL(csp,mp);

		hwcptr->ctrl = *(struct msctrl *)data;
		if (!hwcptr->ctrl.den)
			hwcptr->ctrl.den = 1;
		COPYACK(q, mp, iocp);
		break;

	case HWCSSCREEN:	/* Set Framebuffer */
		CK_RVAL(csp,mp);
#ifdef _SYSCALL32_IMPL
		if ((iocp->ioc_flag & DATAMODEL_MASK) != DATAMODEL_NATIVE)
			hwcptr->screen_dev_t = expldev(*(dev32_t *)data);
		else
#endif
			hwcptr->screen_dev_t = *(dev_t *)data;
		hwcptr->at_border = 0;
		COPYACK(q, mp, iocp);
		break;

	case HWCENABLE:		/* Enable cursor */
		CK_RVAL(csp,mp);
		hwcptr->hwc_enabled = *(int *)data;
		COPYACK(q, mp, iocp);
		break;

	case HWCSBOUND:		/* Set Cursor Boundary */
		CK_RVAL(csp,mp);
		hwcptr->limits = *(struct hwc_limits *)data;
		hwcptr->at_border = 0;
		COPYACK(q, mp, iocp);
		break;

	default:		/* pass it down the line */
		(void) putnext(q, mp);
		break;
	}
}

static void
hwc_spos(queue_t *q, mblk_t *mp)
{
	struct iocblk *iocp = (struct iocblk *)mp->b_rptr;
	struct hwcrec *hwcptr = (struct hwcrec *)q->q_ptr; /* instance data */
	mblk_t *xmp, *ymp;
	Firm_event *ev;

	hwcptr->cursor_position = *(struct fbcurpos *)mp->b_cont->b_rptr;
	hwcptr->at_border = 0;
#ifndef _DDICT
	if (hwcptr->hwc_enabled) {
		hwc_set_cursor_pos(hwcptr->screen_dev_t,
		    &hwcptr->cursor_position);
	}
#endif /* _DDICT */
	iocp->ioc_error = 0;
	iocp->ioc_rval = 0;
	mp->b_datap->db_type = M_IOCACK;
	iocp->ioc_count = 0;
	if (mp->b_cont)
		freemsg(mp->b_cont);
	mp->b_cont = NULL;

	xmp = allocb(sizeof (Firm_event), BPRI_MED);
	if (xmp == NULL) {
		mp->b_datap->db_type = M_IOCNAK;
		iocp->ioc_error = EAGAIN;
		qreply(q, mp);
		return;
	}
	xmp->b_datap->db_type = M_DATA;
	ev = (Firm_event *)xmp->b_rptr;
	ev->id = LOC_X_ABSOLUTE;
	ev->value = hwcptr->cursor_position.x;
#ifndef _DDICT
	uniqtime32(&ev->time);
#endif /* _DDICT */
	xmp->b_wptr = xmp->b_rptr + sizeof (Firm_event);
	ymp = allocb(sizeof (Firm_event), BPRI_MED);
	if (ymp == NULL) {
		mp->b_datap->db_type = M_IOCNAK;
		iocp->ioc_error = EAGAIN;
		freeb(xmp);
		qreply(q, mp);
		return;
	}
	ymp->b_datap->db_type = M_DATA;
	ev = (Firm_event *)ymp->b_rptr;
	ev->id = LOC_Y_ABSOLUTE;
	ev->value = hwcptr->cursor_position.y;
#ifndef _DDICT
	uniqtime32(&ev->time);
#endif /* _DDICT */
	ymp->b_wptr = ymp->b_rptr + sizeof (Firm_event);

	qreply(q, mp);
	qreply(q, xmp);
	qreply(q, ymp);
}

static void
hwc_state_free(struct copyresp *s)
{
#ifdef _SYSCALL32_IMPL
        STRUCT_HANDLE(fbcursor, fbc);
#endif
        if (s->cp_cmd == HWCSCURSOR || s->cp_cmd == FBIOSCURSOR) {
        struct hwc_state *stp = NULL;
        if (s->cp_private != NULL)
        	stp = (struct hwc_state *) s->cp_private->b_rptr;
	if (stp != NULL)
	{
	if (stp->image)
	  kmem_free(stp->image, stp->count);
	if (stp->mask)
	  kmem_free(stp->mask, stp->count);
	if (stp->cmap_red)
	  kmem_free(stp->cmap_red, stp->fbcursor.cmap.count);
	if (stp->cmap_green)
	  kmem_free(stp->cmap_green, stp->fbcursor.cmap.count);
	if (stp->cmap_blue)
	  kmem_free(stp->cmap_blue, stp->fbcursor.cmap.count);
/* bugid: 4500611 remove kmem_free for stp */
	}
	}
}

static void
hwc_scursor(queue_t *q, mblk_t *mp)
{
	struct iocblk *iocp = (struct iocblk *)mp->b_rptr;
	struct copyresp *csp = (struct copyresp *)mp->b_rptr;
	struct hwcrec *hwcptr = (struct hwcrec *)q->q_ptr; /* instance data */
	struct copyreq *cqp;
	struct hwc_state *stp;
#ifdef _SYSCALL32_IMPL
	STRUCT_HANDLE(fbcursor, fbc);
#endif
	int wbytes;

	stp = (struct hwc_state *)csp->cp_private->b_rptr;

	switch (stp->st_state) {
	case GETFBCURSOR:
#ifndef _SYSCALL32_IMPL
		stp->fbcursor = *(struct fbcursor *)mp->b_cont->b_rptr;
#else
		STRUCT_SET_HANDLE(fbc, iocp->ioc_flag,
		    (void *)mp->b_cont->b_rptr);
		stp->fbcursor.set = STRUCT_FGET(fbc, set);
		stp->fbcursor.enable = STRUCT_FGET(fbc, enable);
		stp->fbcursor.pos = STRUCT_FGET(fbc, pos);
		stp->fbcursor.hot = STRUCT_FGET(fbc, hot);
		stp->fbcursor.cmap.index = STRUCT_FGET(fbc, cmap.index);
		stp->fbcursor.cmap.count = STRUCT_FGET(fbc, cmap.count);
		stp->fbcursor.cmap.red = STRUCT_FGETP(fbc, cmap.red);
		stp->fbcursor.cmap.green = STRUCT_FGETP(fbc, cmap.green);
		stp->fbcursor.cmap.blue = STRUCT_FGETP(fbc, cmap.blue);
		stp->fbcursor.size = STRUCT_FGET(fbc, size);
		stp->fbcursor.image = STRUCT_FGETP(fbc, image);
		stp->fbcursor.mask = STRUCT_FGETP(fbc, mask);
#endif /* _SYSCALL32_IMPL */
		if (stp->fbcursor.set & FB_CUR_SETSHAPE) {
			/* compute cursor bitmap bytes */
			wbytes = ((stp->fbcursor.size.x + 31) >> 5) *
			    (int)sizeof (uint32_t);
			stp->count = stp->fbcursor.size.y * wbytes;

			/* allocate necessary bytes for image and mask */
			stp->image = kmem_alloc(stp->count, KM_SLEEP);
			stp->mask = kmem_alloc(stp->count, KM_SLEEP);
		}

		if (stp->fbcursor.set & FB_CUR_SETCMAP) {
			/* allocate necessary bytes for cmap structs */
			stp->cmap_red =
			    kmem_alloc(stp->fbcursor.cmap.count, KM_SLEEP);
			stp->cmap_green =
			    kmem_alloc(stp->fbcursor.cmap.count, KM_SLEEP);
			stp->cmap_blue =
			    kmem_alloc(stp->fbcursor.cmap.count, KM_SLEEP);
		}

		freemsg(mp->b_cont);
		mp->b_cont = NULL;
		/* Reuse M_IOCDATA to copyin data */
		mp->b_datap->db_type = M_COPYIN;
		cqp = (struct copyreq *)mp->b_rptr;
		cqp->cq_flag = 0;
		if (stp->fbcursor.set & FB_CUR_SETSHAPE) {
			cqp->cq_size = stp->count;
			cqp->cq_addr = stp->fbcursor.image;
			stp->st_state = GETIMAGE;
		} else if (stp->fbcursor.set & FB_CUR_SETCMAP) {
			cqp->cq_size = stp->fbcursor.cmap.count;
			cqp->cq_addr = (char *)stp->fbcursor.cmap.red;
			stp->st_state = GETCMAPRED;
		} else {
			goto done;
		}
		qreply(q, mp);
		break;

	case GETIMAGE: /* compute cursor bitmap bytes */
		wbytes = ((stp->fbcursor.size.x + 31) >> 5) *
		    (int)sizeof (uint_t);
		stp->count = stp->fbcursor.size.y * wbytes;

		/* copy the image to state structure */
		bcopy(mp->b_cont->b_rptr, stp->image, stp->count);
		stp->fbcursor.image = stp->image;
		freemsg(mp->b_cont);
		mp->b_cont = NULL;
		/* Reuse M_IOCDATA to copyin data */
		mp->b_datap->db_type = M_COPYIN;
		cqp = (struct copyreq *)mp->b_rptr;
		cqp->cq_size = stp->count;
		cqp->cq_addr = stp->fbcursor.mask;
		cqp->cq_flag = 0;
		stp->st_state = GETMASK;
		qreply(q, mp);
		break;

	case GETMASK:
		/* copy the mask to state structure */
		bcopy(mp->b_cont->b_rptr, stp->mask, stp->count);
		stp->fbcursor.mask = stp->mask;
		freemsg(mp->b_cont);
		mp->b_cont = NULL;
		/* Reuse M_IOCDATA to copyin data */
		mp->b_datap->db_type = M_COPYIN;
		cqp = (struct copyreq *)mp->b_rptr;
		if (!(stp->fbcursor.set & FB_CUR_SETCMAP))
			goto done;

		cqp->cq_size = stp->fbcursor.cmap.count;
		cqp->cq_addr = (caddr_t)stp->fbcursor.cmap.red;
		cqp->cq_flag = 0;
		stp->st_state = GETCMAPRED;

		qreply(q, mp);
		break;

	case GETCMAPRED:
		/* copy the red cmap to state structure */
		bcopy(mp->b_cont->b_rptr, stp->cmap_red,
		    stp->fbcursor.cmap.count);
		stp->fbcursor.cmap.red = (unsigned char *)
		    stp->cmap_red;

		freemsg(mp->b_cont);
		mp->b_cont = NULL;
		/* Reuse M_IOCDATA to copyin data */
		mp->b_datap->db_type = M_COPYIN;
		cqp = (struct copyreq *)mp->b_rptr;
		cqp->cq_size = stp->fbcursor.cmap.count;
		cqp->cq_addr = (caddr_t)stp->fbcursor.cmap.green;
		cqp->cq_flag = 0;
		stp->st_state = GETCMAPGREEN;
		qreply(q, mp);
		break;

	case GETCMAPGREEN:
		/* copy the green cmap to state structure */
		bcopy(mp->b_cont->b_rptr, stp->cmap_green,
		    stp->fbcursor.cmap.count);
		stp->fbcursor.cmap.green = (unsigned char *)stp->cmap_green;

		freemsg(mp->b_cont);
		mp->b_cont = NULL;
		/* Reuse M_IOCDATA to copyin data */
		mp->b_datap->db_type = M_COPYIN;
		cqp = (struct copyreq *)mp->b_rptr;
		cqp->cq_size = stp->fbcursor.cmap.count;
		cqp->cq_addr = (caddr_t)stp->fbcursor.cmap.blue;
		cqp->cq_flag = 0;
		stp->st_state = GETCMAPBLUE;
		qreply(q, mp);
		break;

	case GETCMAPBLUE:
		/* copy the blue cmap to state structure */
		bcopy(mp->b_cont->b_rptr, stp->cmap_blue,
		    stp->fbcursor.cmap.count);
		stp->fbcursor.cmap.blue = stp->cmap_blue;

	done:
		/*
		 * TODO: For some reason the FB_CUR_SETPOS bit is
		 * set but there is junk in fbcursor.pos.x and
		 * fbcursor.pos.y
		 */
		if (stp->fbcursor.set & FB_CUR_SETPOS) {
			hwcptr->cursor_position.x =
			    stp->fbcursor.pos.x;
			hwcptr->cursor_position.y =
			    stp->fbcursor.pos.y;
		}

		hwcptr->at_border = 0;
#ifndef _DDICT
		if (hwcptr->hwc_enabled)
			hwc_set_cursor(hwcptr->screen_dev_t,
			    &stp->fbcursor);
#endif /* _DDICT */
		                
		if (stp->image)
                        kmem_free(stp->image, stp->count);
                if (stp->mask)
                        kmem_free(stp->mask, stp->count);
                if (stp->cmap_red)
                        kmem_free(stp->cmap_red, stp->fbcursor.cmap.count);
                if (stp->cmap_green)
                        kmem_free(stp->cmap_green, stp->fbcursor.cmap.count);
                if (stp->cmap_blue)
                        kmem_free(stp->cmap_blue, stp->fbcursor.cmap.count);
                freemsg(csp->cp_private);
		COPYACK(q, mp, iocp);
	}
}

#ifndef _DDICT

static void
hwc_set_cursor_pos(dev_t dev, struct fbcurpos *cp)
{
	int rval;

	(void) cdev_ioctl(dev, FBIOSCURPOS, (uintptr_t)cp,
	    (FREAD | FWRITE | FKIOCTL | FNATIVE), ddi_get_cred(), &rval);
}


static void
hwc_set_cursor(dev_t dev, struct fbcursor *cp)
{
	int rval;

	(void) cdev_ioctl(dev, FBIOSCURSOR, (uintptr_t)cp,
	    (FREAD | FWRITE | FKIOCTL | FNATIVE), ddi_get_cred(), &rval);
}

#endif /* _DDICT */
