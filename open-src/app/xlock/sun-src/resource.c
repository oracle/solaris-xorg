/*
 * Copyright (c) 1988-91 by Patrick J. Naughton.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * This file is provided AS IS with no warranties of any kind.  The author
 * shall have no liability with respect to the infringement of copyrights,
 * trade secrets or any patents by this file or any part thereof.  In no
 * event will the author be liable for any lost revenue or profits or
 * other special, indirect and consequential damages.
 */

/*
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
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
/*-
 * resource.c - resource management for xlock, the X Window System lockscreen.
 *
 * Revision History:
 * 24-Jun-91: changed name to username.
 * 06-Jun-91: Added flame mode.
 * 24-May-91: Added -name and -usefirst and -resources.
 * 16-May-91: Added random mode and pyro mode.
 * 26-Mar-91: CheckResources: delay must be >= 0.
 * 29-Oct-90: Added #include <ctype.h> for missing isupper() on some OS revs.
 *	      moved -mode option, reordered Xrm database evaluation.
 * 28-Oct-90: Added text strings.
 * 26-Oct-90: Fix bug in mode specific options.
 * 31-Jul-90: Fix ':' handling in parsefilepath
 * 07-Jul-90: Created from resource work in xlock.c
 *
 */
#include <stdio.h>
#include "xlock.h"
#include <netdb.h>
#include <math.h>
#include <ctype.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <X11/Xresource.h>

/*
 * Declare external interface routines for supported screen savers.
 */

extern void inithop();
extern void drawhop();

extern void initlife();
extern void drawlife();

extern void initqix();
extern void drawqix();

extern void initimage();
extern void drawimage();

extern void initblank();
extern void drawblank();

extern void initswarm();
extern void drawswarm();

extern void initrotor();
extern void drawrotor();

extern void initpyro();
extern void drawpyro();

extern void initflame();
extern void drawflame();

typedef struct {
    char       *cmdline_arg;
    void        (*lp_init) ();
    void        (*lp_callback) ();
    int         def_delay;
    int         def_batchcount;
    float       def_saturation;
    char       *desc;
}           LockStruct;

static char randomstring[] = "random";

static LockStruct LockProcs[] = {
    {"hop", inithop, drawhop, 0, 1000, 1.0, "Hopalong iterated fractals"},
    {"qix", initqix, drawqix, 30000, 64, 1.0, "Spinning lines a la Qix(tm)"},
    {"image", initimage, drawimage, 2000000, 8, 0.3, "Random bouncing image"},
    {"life", initlife, drawlife, 1000000, 100, 1.0, "Conway's game of Life"},
    {"swarm", initswarm, drawswarm, 10000, 100, 1.0, "Swarm of bees"},
    {"rotor", initrotor, drawrotor, 10000, 4, 0.4, "Tom's Roto-Rooter"},
    {"pyro", initpyro, drawpyro, 15000, 40, 1.0, "Fireworks"},
    {"flame", initflame, drawflame, 10000, 20, 1.0, "Flame Fractals"},
    {"blank", initblank, drawblank, 5000000, 1, 1.0, "Blank screen"},
    {randomstring, NULL, NULL, 0, 0, 0.0, "Random mode"},
};
#define NUMPROCS (sizeof LockProcs / sizeof LockProcs[0])

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64	/* SunOS 3.5 does not define this */
#endif

#ifdef X_NOT_STDC_ENV
extern char *getenv();
#endif
extern char *inet_ntoa();

#ifndef DEF_FILESEARCHPATH
#define DEF_FILESEARCHPATH "/usr/lib/X11/%T/%N%S"
#endif
#define DEF_DISPLAY	":0"
#define DEF_MODE	"life"
#define DEF_FONT	"-b&h-lucida-medium-r-normal-sans-24-*-*-*-*-*-iso8859-1"
#define DEF_BG		"White"
#define DEF_FG		"Black"
#define DEF_NAME	"Name: "
#define DEF_PASS	"Password: "
#define DEF_INFO	"Enter password to unlock; select icon to lock."
#define DEF_VALID	"Validating login..."
#define DEF_INVALID	"Invalid login."
#define DEF_TIMEOUT	"30"	/* secs till password entry times out */
#define DEF_BC		"100"	/* vectors (or whatever) per batch */
#define DEF_DELAY	"200000"/* microseconds between batches */
#define DEF_NICE	"10"	/* xlock process nicelevel */
#define DEF_SAT		"1.0"	/* color ramp saturation 0->1 */
#define DEF_CLASSNAME	"XLock"

static char *classname;
static char modename[BUFSIZ];	/* BUFSIZ is 1024, defined in stdio.h */
static char modeclass[BUFSIZ];

static XrmOptionDescRec genTable[] = {
    {"-mode", ".mode", XrmoptionSepArg, (caddr_t) NULL},
    {"-nolock", ".nolock", XrmoptionNoArg, (caddr_t) "on"},
    {"+nolock", ".nolock", XrmoptionNoArg, (caddr_t) "off"},
    {"-remote", ".remote", XrmoptionNoArg, (caddr_t) "on"},
    {"+remote", ".remote", XrmoptionNoArg, (caddr_t) "off"},
    {"-mono", ".mono", XrmoptionNoArg, (caddr_t) "on"},
    {"+mono", ".mono", XrmoptionNoArg, (caddr_t) "off"},
    {"-allowroot", ".allowroot", XrmoptionNoArg, (caddr_t) "on"},
    {"+allowroot", ".allowroot", XrmoptionNoArg, (caddr_t) "off"},
    {"-enablesaver", ".enablesaver", XrmoptionNoArg, (caddr_t) "on"},
    {"+enablesaver", ".enablesaver", XrmoptionNoArg, (caddr_t) "off"},
    {"-allowaccess", ".allowaccess", XrmoptionNoArg, (caddr_t) "on"},
    {"+allowaccess", ".allowaccess", XrmoptionNoArg, (caddr_t) "off"},
    {"-echokeys", ".echokeys", XrmoptionNoArg, (caddr_t) "on"},
    {"+echokeys", ".echokeys", XrmoptionNoArg, (caddr_t) "off"},
    {"-usefirst", ".usefirst", XrmoptionNoArg, (caddr_t) "on"},
    {"+usefirst", ".usefirst", XrmoptionNoArg, (caddr_t) "off"},
    {"-v", ".verbose", XrmoptionNoArg, (caddr_t) "on"},
    {"+v", ".verbose", XrmoptionNoArg, (caddr_t) "off"},
    {"-nice", ".nice", XrmoptionSepArg, (caddr_t) NULL},
    {"-timeout", ".timeout", XrmoptionSepArg, (caddr_t) NULL},
    {"-font", ".font", XrmoptionSepArg, (caddr_t) NULL},
    {"-bg", ".background", XrmoptionSepArg, (caddr_t) NULL},
    {"-fg", ".foreground", XrmoptionSepArg, (caddr_t) NULL},
    {"-background", ".background", XrmoptionSepArg, (caddr_t) NULL},
    {"-foreground", ".foreground", XrmoptionSepArg, (caddr_t) NULL},
    {"-username", ".username", XrmoptionSepArg, (caddr_t) NULL},
    {"-password", ".password", XrmoptionSepArg, (caddr_t) NULL},
    {"-info", ".info", XrmoptionSepArg, (caddr_t) NULL},
    {"-validate", ".validate", XrmoptionSepArg, (caddr_t) NULL},
    {"-invalid", ".invalid", XrmoptionSepArg, (caddr_t) NULL},
};
#define genEntries (sizeof genTable / sizeof genTable[0])

static XrmOptionDescRec modeTable[] = {
    {"-delay", NULL, XrmoptionSepArg, (caddr_t) NULL},
    {"-batchcount", NULL, XrmoptionSepArg, (caddr_t) NULL},
    {"-saturation", NULL, XrmoptionSepArg, (caddr_t) NULL},
};

#define MODESPECIFIER0 ".delay"
#define MODESPECIFIER1 ".batchcount"
#define MODESPECIFIER2 ".saturation"

#define modeEntries (sizeof modeTable / sizeof modeTable[0])

static XrmOptionDescRec cmdlineTable[] = {
    {"-display", ".display", XrmoptionSepArg, (caddr_t) NULL},
    {"-nolock", ".nolock", XrmoptionNoArg, (caddr_t) "on"},
    {"+nolock", ".nolock", XrmoptionNoArg, (caddr_t) "off"},
    {"-remote", ".remote", XrmoptionNoArg, (caddr_t) "on"},
    {"+remote", ".remote", XrmoptionNoArg, (caddr_t) "off"},
};
#define cmdlineEntries (sizeof cmdlineTable / sizeof cmdlineTable[0])

static XrmOptionDescRec nameTable[] = {
    {"-name", ".name", XrmoptionSepArg, (caddr_t) NULL},
};


typedef struct {
    char       *opt;
    char       *desc;
}           OptionStruct;

static OptionStruct opDesc[] = {
    {"-help", "print out this message"},
    {"-resources", "print default resource file to standard output"},
    {"-display displayname", "X server to contact"},
    {"-name resourcename", "class name to use for resources (default is XLock)"},
    {"-/+mono", "turn on/off monochrome override"},
    {"-/+nolock", "turn on/off no password required mode"},
    {"-/+remote", "turn on/off remote host access"},
    {"-/+allowroot", "turn on/off allow root password mode"},
    {"-/+enablesaver", "turn on/off enable X server screen saver"},
    {"-/+allowaccess", "turn on/off allow new clients to connect"},
    {"-/+echokeys", "turn on/off echo '?' for each password key"},
    {"-/+usefirst", "turn on/off using the first char typed in password"},
    {"-/+v", "turn on/off verbose mode"},
    {"-delay usecs", "microsecond delay between screen updates"},
    {"-batchcount num", "number of things per batch"},
    {"-nice level", "nice level for xlock process"},
    {"-timeout seconds", "number of seconds before password times out"},
    {"-saturation value", "saturation of color ramp"},
    {"-font fontname", "font to use for password prompt"},
    {"-bg color", "background color to use for password prompt"},
    {"-fg color", "foreground color to use for password prompt"},
    {"-name string", "text string to use for Name prompt"},
    {"-password string", "text string to use for Password prompt"},
    {"-info string", "text string to use for instructions"},
    {"-validate string", "text string to use for validating password message"},
    {"-invalid string", "text string to use for invalid password message"},
};
#define opDescEntries (sizeof opDesc / sizeof opDesc[0])

char       *display;
char       *mode;
char       *fontname;
char       *background;
char       *foreground;
char       *text_name;
char       *text_pass;
char       *text_info;
char       *text_valid;
char       *text_invalid;
float       saturation;
int         nicelevel;
int         delay;
int         batchcount;
int         timeout;
Bool        mono;
Bool        nolock;
Bool        remote;
Bool        allowroot;
Bool        enablesaver;
Bool        allowaccess;
Bool        echokeys;
Bool        usefirst;
Bool        verbose;

#define t_String	0
#define t_Float		1
#define t_Int		2
#define t_Bool		3

typedef struct {
    caddr_t    *var;
    char       *name;
    char       *class;
    char       *def;
    int         type;
}           argtype;

static argtype genvars[] = {
    {(caddr_t *) &fontname, "font", "Font", DEF_FONT, t_String},
    {(caddr_t *) &background, "background", "Background", DEF_BG, t_String},
    {(caddr_t *) &foreground, "foreground", "Foreground", DEF_FG, t_String},
    {(caddr_t *) &text_name, "username", "Username", DEF_NAME, t_String},
    {(caddr_t *) &text_pass, "password", "Password", DEF_PASS, t_String},
    {(caddr_t *) &text_info, "info", "Info", DEF_INFO, t_String},
    {(caddr_t *) &text_valid, "validate", "Validate", DEF_VALID, t_String},
    {(caddr_t *) &text_invalid, "invalid", "Invalid", DEF_INVALID, t_String},
    {(caddr_t *) &nicelevel, "nice", "Nice", DEF_NICE, t_Int},
    {(caddr_t *) &timeout, "timeout", "Timeout", DEF_TIMEOUT, t_Int},
    {(caddr_t *) &mono, "mono", "Mono", "off", t_Bool},
    {(caddr_t *) &nolock, "nolock", "NoLock", "off", t_Bool},
    {(caddr_t *) &remote, "remote", "Remote", "off", t_Bool},
    {(caddr_t *) &allowroot, "allowroot", "AllowRoot", "off", t_Bool},
    {(caddr_t *) &enablesaver, "enablesaver", "EnableSaver", "off", t_Bool},
    {(caddr_t *) &allowaccess, "allowaccess", "AllowAccess", "off", t_Bool},
    {(caddr_t *) &echokeys, "echokeys", "EchoKeys", "off", t_Bool},
    {(caddr_t *) &usefirst, "usefirst", "Usefirst", "off", t_Bool},
    {(caddr_t *) &verbose, "verbose", "Verbose", "off", t_Bool},
};
#define NGENARGS (sizeof genvars / sizeof genvars[0])

static argtype modevars[] = {
    {(caddr_t *) &delay, "delay", "Delay", DEF_DELAY, t_Int},
    {(caddr_t *) &batchcount, "batchcount", "BatchCount", DEF_BC, t_Int},
    {(caddr_t *) &saturation, "saturation", "Saturation", DEF_SAT, t_Float},
};
#define NMODEARGS (sizeof modevars / sizeof modevars[0])


static void
Syntax(badOption)
    char       *badOption;
{
    int         col, len, i;

    fprintf(stderr, "%s:  bad command line option \"%s\"\n\n",
	    ProgramName, badOption);

    fprintf(stderr, "usage:  %s", ProgramName);
    col = 8 + strlen(ProgramName);
    for (i = 0; i < opDescEntries; i++) {
	len = 3 + strlen(opDesc[i].opt);	/* space [ string ] */
	if (col + len > 79) {
	    fprintf(stderr, "\n   ");	/* 3 spaces */
	    col = 3;
	}
	fprintf(stderr, " [%s]", opDesc[i].opt);
	col += len;
    }

    len = 8 + strlen(LockProcs[0].cmdline_arg);
    if (col + len > 79) {
	fprintf(stderr, "\n   ");	/* 3 spaces */
	col = 3;
    }
    fprintf(stderr, " [-mode %s", LockProcs[0].cmdline_arg);
    col += len;
    for (i = 1; i < NUMPROCS; i++) {
	len = 3 + strlen(LockProcs[i].cmdline_arg);
	if (col + len > 79) {
	    fprintf(stderr, "\n   ");	/* 3 spaces */
	    col = 3;
	}
	fprintf(stderr, " | %s", LockProcs[i].cmdline_arg);
	col += len;
    }
    fprintf(stderr, "]\n");

    fprintf(stderr, "\nType %s -help for a full description.\n\n",
	    ProgramName);
    exit(1);
}

static void
Help()
{
    int         i;

    fprintf(stderr, "usage:\n        %s [-options ...]\n\n", ProgramName);
    fprintf(stderr, "where options include:\n");
    for (i = 0; i < opDescEntries; i++) {
	fprintf(stderr, "    %-28s %s\n", opDesc[i].opt, opDesc[i].desc);
    }

    fprintf(stderr, "    %-28s %s\n", "-mode mode", "animation mode");
    fprintf(stderr, "    where mode is one of:\n");
    for (i = 0; i < NUMPROCS; i++) {
	fprintf(stderr, "          %-23s %s\n",
		LockProcs[i].cmdline_arg, LockProcs[i].desc);
    }
    putc('\n', stderr);

    exit(0);
}

static void
DumpResources()
{
    int         i;

    printf("%s.mode: %s\n", classname, DEF_MODE);

    for (i = 0; i < NGENARGS; i++)
	printf("%s.%s: %s\n",
	       classname, genvars[i].name, genvars[i].def);

    for (i = 0; i < NUMPROCS - 1; i++) {
	printf("%s.%s.%s: %d\n", classname, LockProcs[i].cmdline_arg,
	       "delay", LockProcs[i].def_delay);
	printf("%s.%s.%s: %d\n", classname, LockProcs[i].cmdline_arg,
	       "batchcount", LockProcs[i].def_batchcount);
	printf("%s.%s.%s: %g\n", classname, LockProcs[i].cmdline_arg,
	       "saturation", LockProcs[i].def_saturation);
    }
    exit(0);
}


static void
LowerString(s)
    char       *s;
{

    while (*s) {
	if (isupper(*s))
	    *s += ('a' - 'A');
	s++;
    }
}

static void
GetResource(database, parentname, parentclass,
	    name, class, valueType, def, valuep)
    XrmDatabase database;
    char       *parentname;
    char       *parentclass;
    char       *name;
    char       *class;
    int         valueType;
    char       *def;
    caddr_t    *valuep;		/* RETURN */
{
    char       *type;
    XrmValue    value;
    char       *string;
    char        buffer[BUFSIZ];
    char        fullname[BUFSIZ];
    char        fullclass[BUFSIZ];
    int         len;

/* 4036289. Limit the size of string to print. */
    snprintf(fullname, BUFSIZ, "%s.%s", parentname, name);
    snprintf(fullclass, BUFSIZ, "%s.%s", parentclass, class);
    if (XrmGetResource(database, fullname, fullclass, &type, &value)) {
	string = value.addr;
	len = value.size;
    } else {
	string = def;
	len = strlen(string);
    }
    (void) strncpy(buffer, string, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    switch (valueType) {
    case t_String:
	{
	    char       *s = (char *) malloc(len + 1);
	    if (s == (char *) NULL)
		error("%s: GetResource - couldn't allocate memory");
	    (void) strncpy(s, string, len);
	    s[len] = '\0';
	    *((char **) valuep) = s;
	}
	break;
    case t_Bool:
	LowerString(buffer);
	*((int *) valuep) = (!strcmp(buffer, "true") ||
			     !strcmp(buffer, "on") ||
			     !strcmp(buffer, "enabled") ||
			     !strcmp(buffer, "yes")) ? True : False;
	break;
    case t_Int:
	*((int *) valuep) = atoi(buffer);
	break;
    case t_Float:
	*((float *) valuep) = atof(buffer);
	break;
    }
}


static      XrmDatabase
parsefilepath(xfilesearchpath, TypeName, ClassName)
    char       *xfilesearchpath;
    char       *TypeName;
    char       *ClassName;
{
    XrmDatabase database = NULL;
/* 4036289, do not use stack buffer, allocate this buffer from the heap*/
    char       *appdefaults;
    char       *src;
    char       *dst;
    int	       buflen;
    int	       bufsize = BUFSIZ;

    src = xfilesearchpath;

    appdefaults = (char *)malloc(BUFSIZ);
    if (!appdefaults)
	return NULL;

    appdefaults[0] = '\0';
    dst = appdefaults;
    while (1) {
	/* Scan through source, expanding % strings as necessary, and
	   passing completed paths to XrmGetFileDatabase when ':' or
	   end of string is found.  To prevent buffer overflows (bug 
	   4483090) each time we decide to append to the string, we
	   set appenddata to point to the data to be appended & 
	   appendsize to the size to be appended, and then do all the
	   appending & size checking in one place at the end.
	 */
	char *appenddata;
	int appendsize = 0;

	if (*src == '%') {
	    src++;
	    switch (*src) {
	    case '%':
	    case ':':
		appenddata = src++;
		appendsize = 1;
		break;
	    case 'T':
		appenddata = TypeName;
		appendsize = strlen(TypeName);
		src++;
		break;
	    case 'N':
		appenddata = ClassName;
		appendsize = strlen(ClassName);
		src++;
		break;
	    case 'S':
		src++;
		break;
	    default:
		src++;
		break;
	    }
	} else if (*src == ':') {
	    database = XrmGetFileDatabase(appdefaults);
	    if (database == NULL) {
		dst = appdefaults;
		*dst = '\0';
		src++;
	    } else
		break;
	} else if (*src == '\0') {
	    database = XrmGetFileDatabase(appdefaults);
	    break;
	} else {
	    appenddata = src++;
	    appendsize = 1;
	}
	if (appendsize > 0) {
	    buflen = dst - appdefaults;
	    if (buflen + appendsize >= bufsize) {
		int newsize;
		
		/* Grow by a bit more than we need so we don't have to
		   realloc constantly. */
		if (appendsize > BUFSIZ) {
		    newsize = bufsize + appendsize;
		} else {
		    newsize = bufsize + BUFSIZ;
		}
		
		appdefaults = realloc(appdefaults, newsize);
		if (appdefaults) {
		    dst = appdefaults + buflen;
		    bufsize = newsize;
		} else {
		    error("%s: parsefilepath - couldn't allocate memory");
		    exit(1);
		}
	    }
	    if (appendsize == 1) {
		*dst++ = *appenddata;
	    } else {
		strncat(dst, appenddata, appendsize);
		dst += appendsize;
	    }
	    *dst = '\0';
	}
    }
    if (appdefaults)
	free(appdefaults);
    return database;
}


static void
open_display()
{
    if (display != NULL) {
	char       *colon = strrchr(display, ':');
	int         n = colon - display;

	if (colon == NULL)
	    error("%s: Malformed -display argument, \"%s\"\n", display);

	/*
	 * only restrict access to other displays if we are locking and if the
	 * Remote resource is not set.
	 */
	if (nolock)
	    remote = True;
	if (!remote && n
		&& strncmp(display, "unix", n)
		&& strncmp(display, "localhost", n)) {
    /* 1183055(rfe):  xlock doesnt accept long display/host name 
     *         Get the ip address of $DISPLAY and the machine name.       
     *	       If both are matching then allow to open display.
     *         ( also check against the ip addr list returned by gethostbyname.
     *	       Otherwise, report error and exit.
     *         New Local Variables:
     *            display_ip   contains IP address of the $DISPLAY 
     *            host_ip      contains IP address of the machine_name
     *            tmp_display  contains the $DISPLAY - [:0.0 or :0]
     *     This fix will take care of long host name and IP address form.
     */
#ifdef IPv6
	    struct addrinfo *localhostaddr;
	    struct addrinfo *otherhostaddr;
	    struct addrinfo *i, *j;
#else
	    struct hostent *host;
	    char      **hp;
	    char        display_ip[MAXHOSTNAMELEN];
	    char        host_ip[MAXHOSTNAMELEN];
	    struct hostent *host_display;
#endif
	    int         badhost = 1;
	    char       *tmp_display = malloc(n + 1);
	    char        hostname[MAXHOSTNAMELEN];

            strncpy(tmp_display, display, n);
            tmp_display[n] = '\0';

	    if (gethostname(hostname, MAXHOSTNAMELEN))
		error("%s: Can't get local hostname.\n");

#ifdef IPv6
	    if (getaddrinfo(hostname, NULL, NULL, &localhostaddr) != 0) 
		error("%s: Can't get address information for %s.\n", hostname);

	    if (getaddrinfo(tmp_display, NULL, NULL, &otherhostaddr) != 0)
		error("%s: Can't get address information for %s.\n", 
		  tmp_display);

	    for (i = localhostaddr; i != NULL && badhost; i = i->ai_next) {
		for (j = otherhostaddr; j != NULL && badhost; j = j->ai_next) {
		    if (i->ai_family == j->ai_family) {
			if (i->ai_family == AF_INET) {
			    struct sockaddr_in *sinA 
			      = (struct sockaddr_in *) i->ai_addr;
			    struct sockaddr_in *sinB
			      = (struct sockaddr_in *) j->ai_addr;
			    struct in_addr *A = &sinA->sin_addr;
			    struct in_addr *B = &sinB->sin_addr;
			    
			    if (memcmp(A,B,sizeof(struct in_addr)) == 0) {
				badhost = 0;
			    }
			} else if (i->ai_family == AF_INET6) {
			    struct sockaddr_in6 *sinA 
			      = (struct sockaddr_in6 *) i->ai_addr;
			    struct sockaddr_in6 *sinB 
			      = (struct sockaddr_in6 *) j->ai_addr;
			    struct in6_addr *A = &sinA->sin6_addr;
			    struct in6_addr *B = &sinB->sin6_addr;
			    
			    if (memcmp(A,B,sizeof(struct in6_addr)) == 0) {
				badhost = 0;
			    }
			}
		    }
		}
	    }

	    freeaddrinfo(localhostaddr);
	    freeaddrinfo(otherhostaddr);

#else
	    if (!(host_display = gethostbyname(tmp_display)))
		error("%s: Can't get hostbyname %s.\n", tmp_display);

            if ( host_display->h_addrtype == AF_INET )
                strcpy(display_ip,inet_ntoa (*host_display->h_addr_list) );
            else
                error("%s: Unknown address type for %s.\n", tmp_display);

	    if (!(host = gethostbyname(hostname)))
		error("%s: Can't get hostbyname.\n");

            if ( host->h_addrtype != AF_INET )
                error("%s: Unknown address type for %s.\n", hostname);

            for ( ;*host->h_addr_list; host->h_addr_list++ ) {
                strcpy ( host_ip, inet_ntoa(*host->h_addr_list ) );
		if (!strcmp(display_ip, host_ip ) ) { 
		    /* check against the list of Internet address */
		    badhost = 0;  
		    break;
                 }
            }

            if ( badhost ) {
		for (hp = host->h_aliases; *hp; hp++) {
		    if (!strncmp(tmp_display, *hp, n)) { 
		    /* display has been replaced tmp_display because 
		     * display will be in :0.0 format and tmp_display
		     * will have only the hostname/ip_address form.
		     */
			badhost = 0;
			break;
		    }
                }
            }
#endif /* IPv6 */
	
	    if (badhost) {
	        *colon = (char) 0;
	        error("%s: can't lock %s's display\n", display);
            }
	    free(tmp_display);
	}
    } else
	display = ":0.0";
    if (!(dsp = XOpenDisplay(display)))
	error("%s: unable to open display %s.\n", display);
}


void
printvar(class, var)
    char       *class;
    argtype     var;
{
    switch (var.type) {
    case t_String:
	fprintf(stderr, "%s.%s: %s\n",
		class, var.name, *((char **) var.var));
	break;
    case t_Bool:
	fprintf(stderr, "%s.%s: %s\n",
		class, var.name, *((int *) var.var)
		? "True" : "False");
	break;
    case t_Int:
	fprintf(stderr, "%s.%s: %d\n",
		class, var.name, *((int *) var.var));
	break;
    case t_Float:
	fprintf(stderr, "%s.%s: %g\n",
		class, var.name, *((float *) var.var));
	break;
    }
}


void
GetResources(argc, argv)
    int         argc;
    char       *argv[];
{
    XrmDatabase RDB = NULL;
    XrmDatabase nameDB = NULL;
    XrmDatabase modeDB = NULL;
    XrmDatabase cmdlineDB = NULL;
    XrmDatabase generalDB = NULL;
    XrmDatabase homeDB = NULL;
    XrmDatabase applicationDB = NULL;
    XrmDatabase serverDB = NULL;
    XrmDatabase userDB = NULL;
    char        userfile[BUFSIZ];
    char       *homeenv;
    char       *userpath;
    char       *env;
    char       *serverString;
    int         i;
    int		modeLength;

    XrmInitialize();

    /*
     * get -name arg from command line so you can have different resource
     * files for different configurations/machines etc...
     */
    XrmParseCommand(&nameDB, nameTable, 1, ProgramName,
		    &argc, argv);
    GetResource(nameDB, ProgramName, "*", "name", "Name", t_String,
		DEF_CLASSNAME, &classname);


    homeenv = getenv("HOME");
    if (!homeenv)
	homeenv = "";

    env = getenv("XFILESEARCHPATH");
    applicationDB = parsefilepath(env ? env : DEF_FILESEARCHPATH,
				  "app-defaults", classname);

    XrmParseCommand(&cmdlineDB, cmdlineTable, cmdlineEntries, ProgramName,
		    &argc, argv);

    userpath = getenv("XUSERFILESEARCHPATH");
    if (!userpath) {
	env = getenv("XAPPLRESDIR");
	if (env)
/* 4036289, Limit the string to print */
	    snprintf(userfile, BUFSIZ, "%s/%%N:%s/%%N", env, homeenv);
	else
/* 4036289, Limit the string to print */
	    snprintf(userfile, BUFSIZ, "%s/%%N", homeenv);
	userpath = userfile;
    }
    userDB = parsefilepath(userpath, "app-defaults", classname);

    (void) XrmMergeDatabases(applicationDB, &RDB);
    (void) XrmMergeDatabases(userDB, &RDB);
    (void) XrmMergeDatabases(cmdlineDB, &RDB);

    env = getenv("DISPLAY");
    GetResource(RDB, ProgramName, classname, "display", "Display", t_String,
		env ? env : DEF_DISPLAY, &display);
    GetResource(RDB, ProgramName, classname, "nolock", "NoLock", t_Bool,
		"off", (caddr_t *) &nolock);
    GetResource(RDB, ProgramName, classname, "remote", "Remote", t_Bool,
		"off", (caddr_t *) &remote);

    open_display();
    serverString = XResourceManagerString(dsp);
    if (serverString) {
	serverDB = XrmGetStringDatabase(serverString);
	(void) XrmMergeDatabases(serverDB, &RDB);
    } else {
	char        buf[BUFSIZ];
/* 4036289, Limit the string to print */
	snprintf(buf, BUFSIZ, "%s/.Xdefaults", homeenv);
	homeDB = XrmGetFileDatabase(buf);
	(void) XrmMergeDatabases(homeDB, &RDB);
    }

    XrmParseCommand(&generalDB, genTable, genEntries, ProgramName, &argc, argv);
    (void) XrmMergeDatabases(generalDB, &RDB);

    GetResource(RDB, ProgramName, classname, "mode", "Mode", t_String,
		DEF_MODE, (caddr_t *) &mode);

    /*
     * if random mode, then just grab a random entry from the table
     */
    if (!strncmp(mode, randomstring, strlen(mode)))
	mode = LockProcs[random() % (NUMPROCS - 2)].cmdline_arg;

/* 4036289, Limit the string to print */
    snprintf(modename, BUFSIZ, "%s.%s", ProgramName, mode);
    snprintf(modeclass, BUFSIZ, "%s.%s", classname, mode);

    modeLength = strlen(mode);
    modeTable[0].specifier = (char *)malloc(modeLength + 
					    strlen(MODESPECIFIER0) + 2);
    modeTable[1].specifier = (char *)malloc(modeLength + 
					    strlen(MODESPECIFIER1) + 2);
    modeTable[2].specifier = (char *)malloc(modeLength + 
					    strlen(MODESPECIFIER2) + 2);
    
    sprintf(modeTable[0].specifier, "%s.%s", mode, MODESPECIFIER0);
    sprintf(modeTable[1].specifier, "%s.%s", mode, MODESPECIFIER1);
    sprintf(modeTable[2].specifier, "%s.%s", mode, MODESPECIFIER2);

    XrmParseCommand(&modeDB, modeTable, modeEntries, ProgramName, &argc, argv);
    (void) XrmMergeDatabases(modeDB, &RDB);

    /* Parse the rest of the command line */
    for (argc--, argv++; argc > 0; argc--, argv++) {
	if (**argv != '-')
	    Syntax(*argv);
	switch (argv[0][1]) {
	case 'h':
	    Help();
	    /* NOTREACHED */
	case 'r':
	    DumpResources();
	    /* NOTREACHED */
	default:
	    Syntax(*argv);
	    /* NOTREACHED */
	}
    }

    /* the RDB is set, now query load the variables from the database */

    for (i = 0; i < NGENARGS; i++)
	GetResource(RDB, ProgramName, classname,
		    genvars[i].name, genvars[i].class,
		    genvars[i].type, genvars[i].def, genvars[i].var);

    for (i = 0; i < NMODEARGS; i++)
	GetResource(RDB, modename, modeclass,
		    modevars[i].name, modevars[i].class,
		    modevars[i].type, modevars[i].def, modevars[i].var);

    (void) XrmDestroyDatabase(RDB);

    if (verbose) {
	for (i = 0; i < NGENARGS; i++)
	    printvar(classname, genvars[i]);
	for (i = 0; i < NMODEARGS; i++)
	    printvar(modename, modevars[i]);
    }
}


CheckResources()
{
    int         i;

    if (batchcount < 1)
	Syntax("-batchcount argument must be positive.");
    if (saturation < 0.0 || saturation > 1.0)
	Syntax("-saturation argument must be between 0.0 and 1.0.");
    if (delay < 0)
	Syntax("-delay argument must be positive.");

    for (i = 0; i < NUMPROCS; i++) {
	if (!strncmp(LockProcs[i].cmdline_arg, mode, strlen(mode))) {
	    init = LockProcs[i].lp_init;
	    callback = LockProcs[i].lp_callback;
	    break;
	}
    }
    if (i == NUMPROCS) {
	fprintf(stderr, "Unknown mode: ");
	Syntax(mode);
    }
}
