/* Copyright (c) 1990, Oracle and/or its affiliates. All rights reserved.
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
