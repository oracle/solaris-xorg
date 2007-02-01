/* Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident   "@(#)tsol.h 1.6     07/01/24 SMI"

/*
 * tsol.h server side extension
 */

#define NEED_REPLIES
#define NEED_EVENTS
#define _XTSOL_SERVER

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <X11/X.h>
#include <X11/Xproto.h>
#include "misc.h"
#include "os.h"
#include "windowstr.h"
#include "input.h"
#include "scrnintstr.h"
#include "pixmapstr.h"
#include "extnsionst.h"
#include "dixstruct.h"
#include "gcstruct.h"
#include "propertyst.h"
#include "validate.h"
#include <X11/extensions/Xtsol.h>
#include <X11/extensions/Xtsolproto.h>

#include "tsolinfo.h"

extern int tsolWindowPrivateIndex;  /* declared in tsol.c */
extern int tsolPixmapPrivateIndex; 
extern int SpecialName(char *string, int len);
extern TsolInfoPtr GetClientTsolInfo();
extern bslabel_t *lookupSL_low();
extern int PolyPropReadable(PropertyPtr pProp, ClientPtr client);
extern void ReflectStackChange(WindowPtr pWin, WindowPtr pSib, VTKind  kind);
extern WindowPtr TsolPointerWindow();

#ifdef PANORAMIX
extern int PanoramiXGetInputFocus(ClientPtr client);
#endif
