/*
 * Copyright (c) 1999, Oracle and/or its affiliates. All rights reserved.
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xcms.h>
#include <X11/Xmu/XmuSolaris.h>

/* from X11/Xcmsint.h */
#define XDCCC_CORRECT_ATOM_NAME            "XDCCC_LINEAR_RGB_CORRECTION"

#define XSOLARIS_STD_GAMMA	2.22

#define MAX_SAMPLES            	16


/*
** The following two routines are taken from libX11/XcmsProp.c.
*/

/*
 *	NAME
 *		getElement -- get an element value from the property passed
 *
 *	SYNOPSIS
 */
static unsigned long
getElement (int format, char **pValue, unsigned long *pCount) 
/*
 *	DESCRIPTION
 *	    Get the next element from the property and return it.
 *	    Also increment the pointer the amount needed.
 *
 *	Returns
 *	    unsigned long
 */
{
    unsigned long value;

    switch (format) {
      case 32:
	value = *((unsigned long *)(*pValue));
	*pValue += 4;
	*pCount -= 1;
	break;
      case 16:
	value = *((unsigned short *)(*pValue));
	*pValue += 2;
	*pCount -= 1;
	break;
      case 8:
	value = *((unsigned char *) (*pValue));
	*pValue += 1;
	*pCount -= 1;
	break;
      default:
	value = 0;
	break;
    }
    return(value);
}


/*
 *	NAME
 *		getProperty -- Determine the existance of a property
 *
 *	SYNOPSIS
 */
static int
getProperty (Display *pDpy, Window w, Atom property, int *pFormat, unsigned long *pNItems, 
	     unsigned long *pNBytes, char **pValue) 
/*
 *	DESCRIPTION
 *
 *	Returns
 *	    0 if property does not exist.
 *	    1 if property exists.
 */
{
    char *prop_ret;
    int format_ret;
    long len = 6516;
    unsigned long nitems_ret, after_ret;
    Atom atom_ret;
    
    while (XGetWindowProperty (pDpy, w, property, 0, len, False, 
			       XA_INTEGER, &atom_ret, &format_ret, 
			       &nitems_ret, &after_ret, 
			       (unsigned char **)&prop_ret)) {
	if (after_ret > 0) {
	    len += nitems_ret * (format_ret >> 3);
	    XFree (prop_ret);
	} else {
	    break;
	}
    }
    if (format_ret == 0 || nitems_ret == 0) { 
	/* the property does not exist or is of an unexpected type */
	return(XcmsFailure);
    }

    *pFormat = format_ret;
    *pNItems = nitems_ret;
    *pNBytes = nitems_ret * (format_ret >> 3);
    *pValue = prop_ret;
    return(XcmsSuccess);
}

/*
** The following three routines are derived from code in bin/xcmsdb/xcmsdb.c.
*/

static int
QueryTableType0 (unsigned int maxcolor, int format, char **pChar, unsigned long *pCount, 
		 unsigned int *nelem, unsigned short **pph, unsigned int **ppf)
{
    unsigned int nElements;
    unsigned short *ph = NULL;
    unsigned int   *pf = NULL;

    nElements = getElement(format, pChar, pCount) + 1;
    *nelem = nElements;
    if (!(ph = (unsigned short *) malloc(nElements * sizeof(unsigned short))) ||
        !(pf = (unsigned int *) malloc(nElements * sizeof(unsigned int)))) {
	goto Bad;
    }
    *ppf = pf;
    *pph = ph;

    switch (format) {

      case 8:
	while (nElements--) {
	    /* 0xFFFF/0xFF = 0x101 */
	    *ph++ = getElement (format, pChar, pCount) * 0x101;
	    *pf++ = (getElement (format, pChar, pCount)
		    / (XcmsFloat)255.0) * maxcolor;
	}
	break;

      case 16:
	while (nElements--) {
	    *ph++ = (unsigned short)getElement (format, pChar, pCount);
	    *pf++ = (getElement (format, pChar, pCount)
		    / (XcmsFloat)65535.0) * maxcolor;
	}
	break;

      case 32:
	while (nElements--) {
	    *ph++ = (unsigned short)getElement (format, pChar, pCount);
	    *pf++ = (getElement (format, pChar, pCount)
	            / (XcmsFloat)4294967295.0) * maxcolor;
	}
	break;

      default:
	goto Bad;
    }

    return (1);

Bad:
    if (ph) { 
	free(ph);
    }
    if (pf) {
	free(pf);
    }
    *pph = NULL;
    *ppf = NULL;
    return (0);
}

static int
QueryTableType1 (unsigned int maxcolor, int format, char **pChar, unsigned long *pCount, 
		 unsigned int *nelem, unsigned short **pph, unsigned int **ppf)
{
    int count;
    unsigned int max_index;
    unsigned int nElements;
    unsigned short *ph = NULL;
    unsigned int   *pf = NULL;

    max_index = getElement(format, pChar, pCount);
    nElements = max_index + 1;
    *nelem = nElements;

    if (!(ph = (unsigned short *) malloc(nElements * sizeof(unsigned short))) ||
        !(pf = (unsigned int *) malloc(nElements * sizeof(unsigned int)))) {
	goto Bad;
    }
    *ppf = pf;
    *pph = ph;

    switch (format) {

      case 8:
	for (count = 0; count < nElements; count++) {
	    *ph++ = count;
	    *pf++ = ((XcmsFloat) getElement(format, pChar, pCount) / (XcmsFloat)255.0) * 
		     (XcmsFloat)maxcolor;
	}
	break;

      case 16:
	for (count = 0; count < nElements; count++) {
	    *ph++ = count;
	    *pf++ = ((XcmsFloat) getElement(format, pChar, pCount) / (XcmsFloat)65535.0) * 
	            (XcmsFloat)maxcolor;
	}
	break;

      case 32:
	for (count = 0; count < nElements; count++) {
	    *ph++ = count;
	    *pf++ = ((XcmsFloat) getElement (format, pChar, pCount) / (XcmsFloat)4294967295.0) 
	            * (XcmsFloat)maxcolor;
	}
	break;

      default:
	goto Bad;
    }

    return (1);

Bad:
    if (ph) { 
	free(ph);
    }
    if (pf) {
	free(pf);
    }
    *pph = NULL;
    *ppf = NULL;
    return (0);
}


/*
** If the given n pairs (hi, fi) approximate a power function with the
** relation h = f**exp, the exponent exp is returned.
*/

static double
exponentOfPowerFunc (unsigned int maxh, unsigned int maxf, int n, unsigned short *ph, 
		     unsigned int *pf)
{
    unsigned short h;
    unsigned int   f;
    double logh, logf;
    int    i, nsamples, incr;
    double sum, logmaxh, logmaxf, denom;

    incr = n / MAX_SAMPLES;
    if (incr < 1) {
	incr = 1;
    }
    logmaxh = log((double)maxh);
    logmaxf = log((double)maxf);
    sum = 0;
    nsamples = 0;
    
    for (i = (incr>>1); i < n; i += incr) {
	h = ph[i];
	f = pf[i];
	if (h == 0 || f == 0) {
	    continue;
	}
	logh = log((double) h);
	logf = log((double) f);
	denom = logh - logmaxh;
	if (denom) {
	    sum += (logf - logmaxf) / denom;
	    nsamples++;
	}
    }

    if (nsamples == 0) {
	/* Note: as good as anything else */
	return(XSOLARIS_STD_GAMMA);
    } else {
	return(sum/nsamples);
    }
}


/* 
** Updates the current prop element pointer to point beyond the current channel.
** Upon entry, pChar should point to the length element of the channel.
**
** Returns 0 if the property contents are invalid, 1 otherwise.
*/

static int
skipChannel (int cType, int format, char **pChar, unsigned long *pCount)
{
    unsigned int nElements, length;

    if ((long)*pCount <= 0) {
	return (0);
    }
    length = getElement(format, pChar, pCount) + 1;
    
    nElements = ((cType == 0) ? 2 : 1) * length;
    if ((long)nElements > *pCount) {
	return (0);
    }

    while (nElements--) {
	(void) getElement (format, pChar, pCount);
    }

    return (1);
}

/* 
** Updates the current prop element pointer to point beyond the current visual.
** Upon entry, pChar should point to the type element immediately following
** the visualID elemnt.
**
** Returns 0 if the property contents are invalid, 1 otherwise.
*/

static int
skipVisual (int format, char **pChar, unsigned long *pCount)
{
    int cType, nTables;

    /* Get table type */
    if ((long)*pCount <= 0) {
	return (0);
    }
    cType = (int)getElement(format, pChar, pCount);
    if (cType != 0 && cType != 1) {
	return (0);
    }

    /* Get number of channels */
    if ((long)*pCount <= 0) {
	return (0);
    }
    nTables = (int)getElement(format, pChar, pCount);
    if (nTables != 1 && nTables != 3) {
	return (0);
    }

    /* Skip red channel */
    if (!skipChannel(cType, format, pChar, pCount)) {
	return (0);
    }

    if (nTables > 1) {

	/* Skip green channel */
	if (!skipChannel(cType, format, pChar, pCount)) {
	    return (0);
	}

	/* Skip blue channel */
	if (!skipChannel(cType, format, pChar, pCount)) {
	    return (0);
	}
    }

    return (1);
}


static int XSolarisGetVisualGammaCalledFlag = 0;

int
XSolarisGetVisualGammaCalled (void)
{
    return (XSolarisGetVisualGammaCalledFlag);
}

/*
** Note: this function ignores the green and blue intensity correction
** data; it assumes that these are the same as the red channel.
*/

Status
XSolarisGetVisualGamma (Display *dpy, int screen_number, Visual *visual,
			double *gamma)
{
    char *property_return = NULL, *pChar;
    int  count, format, cType, nTables;
    unsigned long nitems = 0;
    unsigned long nbytes_return;
    Atom CorrectAtom;
    VisualID visualID;
    unsigned int maxcolor, nelem;
    unsigned short *ph;
    unsigned int   *pf;

    XSolarisGetVisualGammaCalledFlag = 1;

    /*
     * Get Intensity Tables
     */
    CorrectAtom = XInternAtom (dpy, XDCCC_CORRECT_ATOM_NAME, False);
    if (CorrectAtom != None) {
	if (getProperty (dpy, XRootWindow(dpy, screen_number), CorrectAtom, 
			      &format, &nitems, &nbytes_return, 
			      &property_return) == XcmsFailure) {

	    *gamma = XSOLARIS_STD_GAMMA;
	    return (Success);

	} else if ((long)nitems <= 0) {

	    /* use standard gamma */
	    if (property_return) {
		XFree (property_return);
	    }

	    *gamma = XSOLARIS_STD_GAMMA;
	    return (Success);
	}
    }

    pChar = property_return;

    /* Note: nitems is decremented as a side-effect of calling getElement */
    while (nitems) {
	
	switch (format) {

	case 8:

	    /*
	     * Must have at least:
	     *		VisualID0
	     *		VisualID1
	     *		VisualID2
	     *		VisualID3
	     *		type
	     *		count
	     *		length
	     *		intensity1
	     *		intensity2
	     */
	    if (nitems < 9) {
		XFree (property_return);
		return (BadMatch);
	    }
	    count = 3;
	    break;

	      case 16:
	    /*
	     * Must have at least:
	     *		VisualID0
	     *		VisualID3
	     *		type
	     *		count
	     *		length
	     *		intensity1
	     *		intensity2
	     */
	    if (nitems < 7) {
		XFree (property_return);
		return (BadMatch);
	    }
	    count = 1;
	    break;

	case 32:
	    /*
	     * Must have at least:
	     *		VisualID0
	     *		type
	     *		count
	     *		length
	     *		intensity1
	     *		intensity2
	     */
	    if (nitems < 6) {
		XFree (property_return);
		return (BadMatch);
	    }
	    count = 0;
	    break;
	    
	default:
	    XFree (property_return);
	    return (BadMatch);
	}
	
	/* Get VisualID */
	visualID = getElement(format, &pChar, &nitems);
	while (count--) {
	    visualID = visualID << format;
	    visualID |= getElement(format, &pChar, &nitems);
	}
	if (visual->visualid != visualID) {
	    if (!skipVisual(format, &pChar, &nitems)) {
		XFree (property_return);
		return (BadMatch);
	    }
	    continue;
	}

	/* Found the visual */
	maxcolor = 0xffff;

	/* Get table type and number of channels */
	cType = (int)getElement(format, &pChar, &nitems);
	nTables = (int)getElement(format, &pChar, &nitems);

	/* 
	** Note: it is assumed that the per-channel maps in the table all have
	** the same length.  This is a safe bet for most hardware, as it depends
	** not on the visual masks but on the visual bits_per_rgb.  Thus, we
	** extract the information from the red channel map and ignore the rest.
	*/

	switch (cType) {

	case 0:
	    if (!QueryTableType0(maxcolor, format, &pChar, &nitems, &nelem, &ph, &pf)) {
		XFree (property_return);
		return (BadMatch);
	    }
            if (nelem == 2 && 
		*ph == 0 && *pf == 0 &&
		*(ph+1) == maxcolor && *(pf+1) == maxcolor) {
		/* exactly linear */
		*gamma = 1.0;
	    } else {
		*gamma = exponentOfPowerFunc(maxcolor, maxcolor, (int)nelem, ph, pf);
	    }
	    break;

	case 1:
	    if (!QueryTableType1(maxcolor, format, &pChar, &nitems, &nelem, &ph, &pf)) {
		XFree (property_return);
		return (BadMatch);
	    }
	    *gamma = exponentOfPowerFunc((1<<visual->bits_per_rgb)-1, maxcolor, (int)nelem, ph, pf);
	    break;

	default:
	    XFree (property_return);
	    return (BadMatch);
	}

	XFree (property_return);

	/* These were allocated in the QueryTableType<n> routines and 
	   must be freed now */
	if (ph) {
	    free(ph);
	}
	if (pf) {
	    free(pf);
	}

	return (Success);
    }    
    /* bug fix for 4248958: OPENGL program shows mem leak in libdga*/
    if(property_return) XFree (property_return);

    *gamma = XSOLARIS_STD_GAMMA;
    return (Success);
}

