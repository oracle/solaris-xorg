/* Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
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
 * Custom hack for Solaris to report pkg/patch info - should not be put into
 * X.Org Community release since they don't use our packages/patches, and
 * since it cheats a bit to get the information quickly.   This could easily
 * break because we're using private/undocumented interfaces which may change
 * at any time, but since it's just displaying the information for human
 * consumption by people reading the logfiles, and we don't depend on it in
 * any way, we can live with that.
 */

#include <stdio.h>
#include <string.h>
#include "os.h"

_X_HIDDEN void
sunLogInfo(void)
{
    char pibuf[16384]; /* Should be enough for even the longest patch list */
    const char *pkgs[] = { "SUNWxorg-server", "SUNWxorg-graphics-ddx", NULL };
    const char *p;
    int i;
    FILE *pkginfo;
    
    for (i = 0; pkgs[i] != NULL; i++) {
	p = pkgs[i];
	snprintf(pibuf, sizeof(pibuf), "/var/sadm/pkg/%s/pkginfo", p);
	pkginfo = fopen(pibuf, "r");

	if (pkginfo != NULL) {
	    while(fgets(pibuf, sizeof(pibuf), pkginfo) != NULL) {
		if (strncmp(pibuf, "VERSION=", 8) == 0) {
		    ErrorF("%s package version: %s", p, pibuf+8);
		} else if (strncmp(pibuf, "PATCHLIST=", 10) == 0) {
		    ErrorF("%s patches applied: %s", p, pibuf+10);
		}
	    }
	    fclose(pkginfo);
	}
    }
}

