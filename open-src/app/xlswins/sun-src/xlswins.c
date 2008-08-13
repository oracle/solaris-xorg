/*
 * $XConsortium: xlswins.c,v 1.12 89/12/10 17:14:02 rws Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

char *ProgramName;
static char *output_format = "0x%lx";
static char widget_name_buf[1024], widget_class_buf[1024];

static void usage ()
{
    static char *help[] = {
"    -display displayname             X server to contact",
"    -format {hex, decimal, octal}    format used to print window id",
"    -indent number                   amount to indent per level",
"    -long                            print a long listing",
"    -resources                       print a listing of widget resources",
"",
NULL};
    char **cpp;

    fprintf (stderr, "usage:\n        %s [-options ...] [windowid] ...\n\n",
	     ProgramName);
    fprintf (stderr, "where options include:\n");
    for (cpp = help; *cpp; cpp++) {
	fprintf (stderr, "%s\n", *cpp);
    }
    exit (1);
}

static long parse_long (s)
    char *s;
{
    char *fmt = "%lu";
    long retval = 0;

    if (s && *s) {
	if (*s == '0') s++, fmt = "%lo";
	if (*s == 'x' || *s == 'X') s++, fmt = "%lx";
	(void) sscanf (s, fmt, &retval);
    }
    return retval;
}

static int got_xerror = 0;

static int myxerror (dpy, rep)
    Display *dpy;
    XErrorEvent *rep;
{
    char buffer[BUFSIZ];
    char mesg[BUFSIZ];
    char number[32];
    char *mtype = "XlibMessage";

    got_xerror++;
    if (rep->error_code == BadWindow) {
	fflush (stdout);
	fprintf (stderr, "%s:  no such window ", ProgramName);
	fprintf (stderr, output_format, rep->resourceid);
	fprintf (stderr, "\n");
	return 0;
    }
    XGetErrorText(dpy, rep->error_code, buffer, BUFSIZ);
    XGetErrorDatabaseText(dpy, mtype, "XError", "X Error", mesg, BUFSIZ);
    (void) fprintf(stderr, "%s: %s\n  ", mesg, buffer);
    XGetErrorDatabaseText(dpy, mtype, "MajorCode", "Request Major code %d", 
        mesg, BUFSIZ);
    (void) fprintf(stderr, mesg, rep->request_code);
    sprintf(number, "%d", rep->request_code);
    XGetErrorDatabaseText(dpy, "XRequest", number, "",  buffer, BUFSIZ);
    (void) fprintf(stderr, " %s", buffer);
    fputs("\n  ", stderr);
    XGetErrorDatabaseText(dpy, mtype, "MinorCode", "Request Minor code", 
        mesg, BUFSIZ);
    (void) fprintf(stderr, mesg, rep->minor_code);
    fputs("\n  ", stderr);
    XGetErrorDatabaseText(dpy, mtype, "ResourceID", "ResourceID 0x%x",
        mesg, BUFSIZ);
    (void) fprintf(stderr, mesg, rep->resourceid);
    fputs("\n  ", stderr);
    XGetErrorDatabaseText(dpy, mtype, "ErrorSerial", "Error Serial #%d", 
        mesg, BUFSIZ);
    (void) fprintf(stderr, mesg, rep->serial);
    fputs("\n  ", stderr);
    XGetErrorDatabaseText(dpy, mtype, "CurrentSerial", "Current Serial #%d",
        mesg, BUFSIZ);
    (void) fprintf(stderr, mesg, NextRequest(dpy)-1);
    fputs("\n", stderr);
    if (rep->error_code == BadImplementation) return 0;
    exit (1);
}


main (argc, argv)
    int argc;
    char *argv[];
{
    char *displayname = NULL;
    Display *dpy;
    Bool long_version = False;
    Bool print_resources = False;
    int i;
    int indent = 2;

    ProgramName = argv[0];

    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    switch (arg[1]) {
	      case 'd':			/* -display displayname */
		if (++i >= argc) usage ();
		displayname = argv[i];
		continue;
	      case 'i':			/* -indent number */
		if (++i >= argc) usage ();
		indent = atoi (argv[i]);
		continue;
	      case 'l':
		long_version = True;	/* -long */
		continue;
	      case 'r':			/* -resources */
		print_resources = True;
		continue;
	      case 'f':			/* -format [odh] */
		if (++i >= argc) usage ();
		switch (argv[i][0]) {
		  case 'd':		/* decimal */
		    output_format = "%lu";
		    continue;
		  case 'o':		/* octal */
		    output_format = "0%lo";
		    continue;
		  case 'h':		/* hex */
		    output_format = "0x%lx";
		    continue;
		}
		usage ();

	      default:
		usage ();
	    }
	} else {
	    break;			/* out of for loop */
	}
    }

    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display \"%s\"\n",
		 ProgramName, XDisplayName (displayname));
	exit (1);
    }

    XSetErrorHandler (myxerror);

    widget_name_buf[0] = widget_class_buf[0] = '\0';
    if (i >= argc) {
	list_window (dpy, RootWindow (dpy, DefaultScreen (dpy)), 0, 
		     indent, long_version, print_resources, 
		     widget_name_buf, widget_class_buf);
    } else {
	for (; i < argc; i++) {
	    Window w = parse_long (argv[i]);
	    if (w == 0) {
		fprintf (stderr, "%s:  bad window number \"%s\"\n",
			 ProgramName, argv[i]);
	    } else {
		list_window (dpy, w, 0, indent, long_version, print_resources,
			     widget_name_buf, widget_class_buf);
	    }
	}
    }

    XCloseDisplay (dpy);
    exit (0);
}


list_window (dpy, w, depth, indent, long_version, print_resources,
	     pname, pclass)
    Display *dpy;
    Window w;
    int depth;
    int indent;
    Bool long_version;
    Bool print_resources;
    char *pname, *pclass;
{
    Window root, parent;
    unsigned int nchildren;
    Window *children = NULL;
    int n;
    char *name = NULL;
    XClassHint ch;
    int pnamelen, pclasslen;

    if (!print_resources) {
	if (long_version) {
	    printf ("%d:  ", depth);
	}
	for (n = depth * indent; n > 0; n--) {
	    putchar (' ');
	}
	printf (output_format, w);
    }

    /*
     * if we put anything before the XFetchName then we'll have to change the
     * error handler
     */
    got_xerror = 0;
    (void) XFetchName (dpy, w, &name);

    ch.res_name = ch.res_class = NULL;
    if (!print_resources) {
	printf ("  (%s)", name ? name : "");

	if (long_version && got_xerror == 0) {
	    int x, y, rx, ry;
	    unsigned int width, height, bw, depth;

	    if (XGetClassHint (dpy, w, &ch)) {
		printf ("; (%s)(%s)", 
			ch.res_name ? ch.res_name : "nil",
			ch.res_class ? ch.res_class : "nil");
	    } else {
		printf ("; ()()");
	    }

	    if (XGetGeometry(dpy, w, &root, &x,&y,&width,&height,&bw,&depth)) {
		Window child;

		printf ("    %ux%u+%d+%d", width, height, x, y);
		if (XTranslateCoordinates (dpy, w,root,0,0,&rx,&ry,&child)) {
		    printf ("  +%d+%d", rx - bw, ry - bw);
		}
	    }
	}
    } else {
	(void) XGetClassHint (dpy, w, &ch);
	if (got_xerror == 0) {
	    if (*pname) strcat (pname, ".");
	    if (ch.res_name) strcat (pname, ch.res_name);
	    if (*pclass) strcat (pclass, ".");
	    if (ch.res_class) strcat (pclass, ch.res_class);

	    printf ("%s  =  %s", pclass, pname);
	}
    }
    putchar ('\n');

    if (got_xerror) goto done;

    if (!XQueryTree (dpy, w, &root, &parent, &children, &nchildren))
      goto done;

    pnamelen = strlen(pname);
    pclasslen = strlen(pclass);
    for (n = 0; n < nchildren; n++) {
	pname[pnamelen] = '\0';
	pclass[pclasslen] = '\0';
	list_window (dpy, children[n], depth + 1, indent, long_version,
		     print_resources, pname, pclass);
    }

  done:
    if (ch.res_name) XFree (ch.res_name);
    if (ch.res_class) XFree (ch.res_class);
    if (name) XFree ((char *) name);
    if (children) XFree ((char *) children);
    return;
}

