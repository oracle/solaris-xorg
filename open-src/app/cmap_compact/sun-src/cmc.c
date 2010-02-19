/*
*
* Copyright 1990 Sun Microsystems, Inc.  All rights reserved.
* Use subject to license terms.
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
*
*/


#include <stdio.h>
#include "cmc.h"


/*
** Options and Arguments
*/

char	*display_name 	= NULL;		/* -display */
int	warn		= 0;		/* -warn */
char	*basename 	= NULL;		/* optional argument */

char *program;


/*VARARGS1*/
void
fatal_error (format, arg1, arg2, arg3, arg4)
char	*format;

{
	(void) fprintf(stderr, "%s: error: ", program);
	(void) fprintf(stderr, format, arg1, arg2, arg3, arg4);
	(void) fprintf(stderr, "\n");
	exit(1);
}


/*VARARGS1*/
void
warning (format, arg1, arg2, arg3, arg4)
char	*format;

{
	(void) fprintf(stderr, "Warning: ");
	(void) fprintf(stderr, format, arg1, arg2, arg3, arg4);
	(void) fprintf(stderr, "\n");
	exit(1);
}

static void
usage ()

{
	/* Note: optional filename arg explicitly not documented */
	fprintf(stderr, "usage: %s <op> [-display name] [-warn]\n", program);
	fprintf(stderr, "<op> = save | init | discard | dealloc | show | NeWSinit\n");
	exit(1);
}


/* 
** Parse arguments 
*/

void
process_arguments (argv)
char	**argv;

{
	register char	**a;

	for (a = argv; *a; a++) {
		if (**a == '-') {
			if        (!strcmp(*a, "-warn")) {
			    warn = 1;
			} else if (!strcmp(*a, "-display")) {
			    if (*++a)
				display_name = *a;
			    else {
				fprintf(stderr, "error: -display needs an argument\n");
				usage();
			    }
			} else {
				fprintf(stderr, "error: unrecognized option '%s'\n", *a);
				usage();
			}
	        } else {
		    if (basename) {
			fprintf(stderr, "error: unrecognized argument '%s'\n", *a);
			usage();
		    } else
			basename = *a;
		}
	}
}


/*ARGSUSED*/
void
main (argc,argv)
int 	argc;
char    **argv;

{
	void	(*op)();

	/* Initialize error handling */
	program = argv[0];

	/* determine operation */
	if (argc <= 1)
		usage();
	++argv;
	if      (!strcmp("save", *argv)) 
		op = cmc_save;
	else if (!strcmp("init", *argv)) 
		op = cmc_init;
	else if (!strcmp("show", *argv)) 
		op = cmc_show;
	else if (!strcmp("discard", *argv)) 
		op = cmc_discard;
	else if (!strcmp("dealloc", *argv)) 
		op = cmc_dealloc;
	else if (!strcmp("NeWSinit", *argv)) 
		op = cmc_NeWSinit;
	else
		usage();
	
	/* parse rest of arguments */
	process_arguments(++argv);

	/* invoke operation */
	op();

	exit(0);
}
