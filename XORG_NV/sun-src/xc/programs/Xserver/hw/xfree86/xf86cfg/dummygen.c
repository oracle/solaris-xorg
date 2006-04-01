/*
 * Copyright (c) 2005 Sun Microsystems (http://www.sun.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * CONECTIVA LINUX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Conectiva Linux shall
 * not be used in advertising or otherwise to promote the sale, use or other
 * dealings in this Software without prior written authorization from
 * Conectiva Linux.
 *
 *
 * $XFree86: xc/programs/Xserver/hw/xfree86/xf86cfg/dummy.c v 1.1 2005/04/08 23:58:52 Exp $
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSYMS 256 
#define MAXLINE 512

static char *dixsymfile = "../loader/dixsym.c";
static char *misymfile = "../loader/misym.c";
static char *fontsymfile = "../loader/fontsym.c";
static char *extsymfile = "../loader/extsym.c";
static char *xf86symfile = "../loader/xf86sym.c";
static char *xorgsymfile = "loadmod.c";
static char *outputfile = "dummy.c";

static char	*xf86excludesyms[MAXSYMS];
static char	line[MAXLINE];
static int 	endfile;
static FILE	*ofp;

static char *dixexcludesyms[ ] = {
		"Xstrdup",
		"XNFstrdup",
		"LogVWrite",
		"FatalError",
		"ErrorF",
		"VErrorF",
		"Error",
		"kNFalloc",
		"XNFalloc",
		"XNFcalloc",
		"XNFrealloc",
		"Xalloc",
		"Xcalloc",
		"Xfree",
		"Xrealloc",
		NULL
};

static char *xf86initexcludesyms[ ] = {
		"xf86GetErrno",
		"xf86ErrorF",
		"xf86strlcat",
		"xf86strlcpy",
		"xf86strncat",
		"xf86HUGE_VAL",
		NULL
};

typedef enum {
    RET_EOF = 0,
    RET_DEF,
    RET_FUNC,
    RET_VAR,
    RET_COM,
    RET_OTHER
} rettype;

static void
errexit(char *format, char *str) {
	int	i;
	char 	*p;

	if (str)
		fprintf (stderr, format, str);
	else
		fprintf (stderr, format);

	i = 0;
	while (xf86excludesyms[i]) 
		free(xf86excludesyms[i++]); 
	exit (1);
}

static rettype
readline(FILE *fp) {
        char    ch;
	int	i = 0;

	endfile = 0;

        /* skip leading spaces */
        while ((ch = fgetc(fp)) == ' ');

	line[i] = ch;
	while (((line[i] != '\n') && (line[i] != EOF)) ||
		((line[i] == '\n') && (line[i-1] == '\\'))) {
		if ((line[i] == '\n') && (line[i-1] == '\\'))
			i--;
		else
			if (++i >= MAXLINE)
				errexit("Line exceeds maximum.\n", NULL);
		line[i] = fgetc(fp);
	}

	if (line[i] == '\n')
		line[i] = '\0';
	else
		if (line[i] == EOF) {
			if (i == 0)
				return (RET_EOF);
			else 
				endfile = 1;
		}

	/* Processing line */
	if ((line[0] == '/' && line[1] == '*') || (line[0] == '*'))
		return (RET_COM);
	if (!strncmp(line, "#if", 3) || !strncmp(line, "#else", 5) ||
		!strncmp(line, "#endif", 6) || !strncmp(line, "# if", 4) ||
		!strncmp(line, "# else", 6) || !strncmp(line, "# endif", 7))
		return (RET_DEF);
	if (!strncmp(line, "SYMFUNC", 7) && strncmp(line, "SYMFUNCALIAS", 12))	
		return (RET_FUNC);
	if (!strncmp(line, "SYMVAR", 6))
		return (RET_VAR);

	return (RET_OTHER);
}

void 
processsyms(char *file, char **exclude) {
	FILE 	*fp;
	int	i;
	int	skip;
	rettype	ret;
	char	*p, *q;

	if (!(fp = fopen(file, "r")))
		errexit("Open %s failed\n", file);

	fprintf(ofp, "\n/* Functions and variable from %s */\n", file);

	while ((ret = readline(fp)) != RET_EOF) {
		if ((ret == RET_COM) || (ret == RET_OTHER))
			continue;
		
		if (ret == RET_DEF)
			fprintf(ofp, "%s\n", line);
			
		if ((ret == RET_FUNC) || (ret == RET_VAR)) {
			if (!(p = strchr(line, '(')))
				errexit("Format error: %s\n", line);
			if (!(q = strchr(p, ')')))
				errexit("Format error: %s\n", line);
			*q = '\0';
			p++;
			if (exclude) {
				i = 0;
				skip = 0;
				while (exclude[i]) {
					if (!strcmp(p, exclude[i])) {
						skip = 1;
						break;
					}
					i++;
				}
				if (skip)
					continue;
			}

			if (ret == RET_FUNC)
				fprintf(ofp, "    void %s( ) { }\n", p);
			else
				fprintf(ofp, "    int %s;\n", p);

			if (endfile) {
				fclose(fp);
				return;
			}
		}
	}

	fclose(fp);
	return;
}

int
main(int argc, char *argv[]) {
	int 	i, len;
	rettype	ret;
	char	*p, *q;
	FILE	*fp;

	/* Initialize xf86excludesyms[] */
	for (i = 0; i < MAXSYMS; i++)
		xf86excludesyms[i] = 0;

	/* Create xf86excludesyms[] */
	i = 0;
	while (xf86initexcludesyms[i]) {
		if (i >= MAXSYMS)
			errexit("Error: xf86 exclude symbols exceeds MAXSYMS\n", NULL);
		len = strlen(xf86initexcludesyms[i]);
		if (xf86excludesyms[i] = malloc(len + 1)) {
			memcpy((void *)xf86excludesyms[i], 
			(void *)xf86initexcludesyms[i], len);
			xf86excludesyms[i][len] = '\0';
			i++;
		}
		else
			errexit("Error: malloc failed\n", NULL);
	}

	if (!(fp = fopen(xorgsymfile, "r")))
		errexit("Open %s failed\n", xorgsymfile);

	while ((ret = readline(fp)) != RET_EOF) {
		if (i >= MAXSYMS)
			errexit("Error: xf86 exclude symbols exceeds MAXSYMS\n", NULL);
		if ((ret == RET_COM) || (ret == RET_OTHER))
			continue;

		if ((ret == RET_FUNC) || (ret == RET_VAR)) {
			if (!(p = strchr(line, '(')))
				errexit("Format error: %s\n", line);
			if (!(q = strchr(p, ')')))
				errexit("Format error: %s\n", line);
			*q = '\0';
			p++;
			len = strlen(p);
			if (xf86excludesyms[i] = malloc(len + 1)) {
				memcpy((void *)xf86excludesyms[i], p, len);
				xf86excludesyms[i][len] = '\0';
				i++;
			}
			else
				errexit("Error: malloc failed\n", NULL);
		}
		if (endfile)
			break;
	}


	if (!(ofp = fopen(outputfile, "w")))
		errexit("Open %s failed\n", outputfile);

	processsyms(dixsymfile, dixexcludesyms);
	processsyms(misymfile, NULL);
	processsyms(fontsymfile, NULL);
	processsyms(extsymfile, NULL);
	processsyms(xf86symfile, xf86excludesyms);

	fclose(ofp);

	i = 0;
	while (xf86excludesyms[i]) 
		free(xf86excludesyms[i++]);
 
	return (0);
}
