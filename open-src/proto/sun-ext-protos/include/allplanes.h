/* Copyright 1990 Sun Microsystems, Inc.  All rights reserved.
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



#ifndef _ALLPLANES_H_
#define _ALLPLANES_H_

#define X_AllPlanesQueryVersion			0
#define X_AllPlanesPolyPoint			1
#define X_AllPlanesPolyLine			2
#define X_AllPlanesPolySegment			3
#define X_AllPlanesPolyRectangle		4
#define X_AllPlanesPolyFillRectangle		5

#define AllPlanesNumberEvents			0

#ifndef _ALLPLANES_SERVER_

extern Bool XAllPlanesQueryExtension();
extern Status XAllPlanesQueryVersion();
extern void XAllPlanesDrawPoints();
extern void XAllPlanesDrawLines();
extern void XAllPlanesDrawSegments();
extern void XAllPlanesDrawRectangles();
extern void XAllPlanesFillRectangles();

#endif /* _ALLPLANES_SERVER_ */

#endif /* _ALLPLANES_H_ */
