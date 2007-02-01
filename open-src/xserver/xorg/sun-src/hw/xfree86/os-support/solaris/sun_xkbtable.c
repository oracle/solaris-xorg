/* Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident   "@(#)sun_xkbtable.c	1.5	06/11/20 SMI"

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#ifdef HAVE_XKB_CONFIG_H
#include <xkb-config.h>
#endif

#ifdef XKB
#include "xf86.h"
#include "sun_xkbtable.h"

#ifndef DFLT_XKB_CONFIG_ROOT
#define DFLT_XKB_CONFIG_ROOT XKB_BASE_DIRECTORY
#endif

#define MAXLINELEN              256

static char    *getaline(FILE *);
static char    *skipwhite(char *);

static int		global_linenumber = 0;
#define	commentchar '#'/* Comment char */
static char		line[MAXLINELEN + 1];
static char	       *whitespace = " \t\n";

static char *
getaline(
    FILE    *fp
)
{
    char    *ptr;
    char    *tmp;
    int      index;
    int      c;


    while(1) {
	ptr = fgets(line, MAXLINELEN, fp);
        if (!ptr) {
	    (void) fclose(fp);
            return NULL;
        }

        global_linenumber++;

        if (ptr[0] == commentchar) {        /* Comment line */
            continue; 
        }

        if (ptr[0] == '\n') {               /* Blank line */
            continue;
        }

        tmp = strchr(ptr, '#');
        if (tmp != NULL) {
            *tmp = '\0';
        }

        if (ptr[strlen(ptr) - 1] == '\n') {
            ptr[strlen(ptr) - 1] = '\0';    /* get rid of '\n' */
        }

        ptr = skipwhite(ptr);
        if (*ptr) {
            break;
        }

    }
    return ptr;
}


/*
 * Skips over the white space character in the string. 
 */

static char *
skipwhite(
    char *ptr
)
{
    while ((*ptr == ' ') || (*ptr == '\t')) {
        ptr++;
    }

    if (*ptr == '\n') {             /* This should not occur. but .. */
        ptr = '\0';
    }

    return ptr;
}

/*
 * Looks up in the .../etc/keytables/xkbtable.map file for the appropriate 
 * XKB names for the keyboard
 */

int
sun_find_xkbnames(
    int          kb_type,	/* input */
    int          kb_layout,	/* input */
    char       **xkb_keymap,	/* output */
    char       **xkb_model,	/* output */
    char       **xkb_layout	/* output */
)
{
    const char  *type, *layout;
    char	*keymap, *defkeymap = NULL;	/* XKB Keymap name */
    char	*model , *defmodel = NULL;	/* XKB model name */
    char	*xkblay, *defxkblay = NULL;	/* XKB layout name */
    FILE        *fp;
    int          found_error,
                 found_keytable;

    fp = fopen(DFLT_XKB_CONFIG_ROOT "/xkbtable.map", "r");
    if (fp == NULL) {
	xf86Msg(X_WARNING, DFLT_XKB_CONFIG_ROOT "/xkbtable.map not found.\n"
	  "\tCannot automap keyboard layout.\n");
        return !Success;
    } 

    global_linenumber = 0;
    found_error = 0;
    found_keytable = 0;

    while (getaline(fp)) { 
	type = strtok(line, " \t\n");
	if (type == NULL) {
	    found_error = 1;
	}
	
	layout = strtok(NULL, " \t\n");
	if (layout == NULL) {
	    found_error = 1;
	}
	
	keymap = strtok(NULL, " \t\n");
	if (keymap == NULL) {
	    found_error = 1;
	}

	/* These two are optional entries */
	model = strtok(NULL, " \t\n");
	if ((model == NULL) || (*model == commentchar)) {
	    model = xkblay = NULL;
	} else {
	    xkblay = strtok(NULL, " \t\n");
	    if ((xkblay !=NULL) && (*xkblay == commentchar)) {
		xkblay = NULL;
	    }
	}

	if (found_error) {
	    found_error = 0;
	    continue;
	}
	
	/* record default entry if/when found */
	if (*type == '*') {
	    if (defkeymap == NULL) {
		defkeymap = keymap;
		defmodel = model;
		defxkblay = xkblay;
	    }
	} else if (atoi(type) == kb_type) {
	    if (*type == '*') {
		if (defkeymap == NULL) {
		    defkeymap = keymap;
		    defmodel = model;
		    defxkblay = xkblay;
		}
	    } else if (atoi(layout) == kb_layout) {
		found_keytable = 1;
		break;
	    }
	}
    }
    (void) fclose(fp);
             
    if (! found_keytable ) {
	keymap = defkeymap;
	model = defmodel;
	xkblay = defxkblay;
    }

    if ((keymap != NULL) && (strcmp(keymap,"-") !=0)) {
	xf86Msg(X_PROBED, "XKB: keymap: \"%s\"\n", keymap);
	*xkb_keymap = keymap;
    }
    if ((model != NULL) && (strcmp(model,"-") !=0)) {
	*xkb_model = model;
	xf86Msg(X_PROBED, "XKB: model: \"%s\"\n", model);
    }
    if ((xkblay != NULL) && (strcmp(xkblay,"-") !=0)) {
	*xkb_layout = xkblay;
	xf86Msg(X_PROBED, "XKB: layout: \"%s\"\n", xkblay);
    }
    return Success;
}
#endif
