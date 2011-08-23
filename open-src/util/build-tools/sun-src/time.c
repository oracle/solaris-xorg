/*
 * Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.
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

/*
 * This compiles to a module that can be preloaded during a build.  If this
 * is preloaded, it interposes on time(2), gettimeofday(3C), and
 * clock_gethrtime(3C) and returns a constant number of seconds since epoch
 * when the execname matches one of the desired "programs" and TIME_CONSTANT
 * contains an integer value to be returned.
 */

#include <stdlib.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <strings.h>
#include <time.h>

/* The list of programs that we want to use a constant time. */
static char *programs[] = { "autogen", "bash", "cpp", "cc1", "date", "doxygen",
	"erl", "javadoc", "ksh", "ksh93", "ld", "perl", "perl5.8.4", "perl5.10",
	"ruby", "sh", "uil", NULL };

static int
stack_info(uintptr_t pc, int signo, void *arg)
{
	Dl_info info;
	void *sym;

	if (dladdr1((void *)pc, &info, &sym, RTLD_DL_SYMENT) != NULL) {
		if (strstr(info.dli_fname, ".so") == NULL)
			*(char **)arg = (char *)info.dli_fname;
	}

	return (0);
}

static char *
my_execname()
{
	static char *execname;

	if (execname == NULL) {
		ucontext_t ctx;

		if (getcontext(&ctx) == 0)
			walkcontext(&ctx, stack_info, &execname);

		if (execname != NULL) {
			char *s = strrchr(execname, '/');

			if (s != NULL)
				execname = ++s;
		}
	}

	return (execname);
}

static time_t
time_constant()
{
	char *execname = my_execname();
	time_t result = -1;

	if (execname != NULL) {
		int i;

		for (i = 0; programs[i] != NULL; i++)
			if (strcmp(execname, programs[i]) == 0) {
				static char *time_string;

				if (time_string == NULL)
					time_string = getenv("TIME_CONSTANT");

				if (time_string != NULL)
					result = atoll(time_string);

				break;
			}
	}

	return (result);
}

time_t
time(time_t *ptr)
{
	time_t result = time_constant();

	if (result == (time_t)-1) {
		static time_t (*fptr)(time_t *);

		if (fptr == NULL)
			fptr = (time_t (*)(time_t *))dlsym(RTLD_NEXT, "time");

		result = (fptr)(ptr);
	} else if (ptr != NULL)
			*ptr = result;

	return (result);
}

int
gettimeofday(struct timeval *tp, void *tzp)
{
	static int (*fptr)(struct timeval *, void *);
	int result = -1;

	if (fptr == NULL)
		fptr = (int (*)(struct timeval *, void *))dlsym(RTLD_NEXT,
				"gettimeofday");

	if ((result = (fptr)(tp, tzp)) == 0) {
		time_t curtime = time_constant();

		if (curtime != (time_t)-1)
			tp->tv_sec = curtime;
	}

	return (result);
}

int
clock_gettime(clockid_t clock_id, struct timespec *tp)
{
	static int (*fptr)(clockid_t, struct timespec *);
	int result = -1;

	if (fptr == NULL)
		fptr = (int (*)(clockid_t, struct timespec *))dlsym(RTLD_NEXT,
				"clock_gettime");

	if ((result = (fptr)(clock_id, tp)) == 0) {
		time_t curtime = time_constant();

		if (curtime != (time_t)-1)
			tp->tv_sec = curtime;
	}

	return (result);
}
