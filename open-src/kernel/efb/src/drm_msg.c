/*
 * Copyright (c) 2006, 2012, Oracle and/or its affiliates. All rights reserved.
 */

/*
 * Copyright (c) 2012 Intel Corporation.  All rights reserved.
 */

#include "drmP.h"

void
drm_debug_print(int cmn_err, const char *func, int line, const char *fmt, ...)
{
	va_list ap;
	char format[256];

	(void) snprintf(format, sizeof (format), "[drm:%s:%d] %s",
	    func, line, fmt);

	va_start(ap, fmt);
	vcmn_err(cmn_err, format, ap);
	va_end(ap);
}

void
drm_debug(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vcmn_err(CE_NOTE, fmt, ap);
	va_end(ap);
}

void
drm_error(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vcmn_err(CE_WARN, fmt, ap);
	va_end(ap);
}

void
drm_info(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vcmn_err(CE_NOTE, fmt, ap);
	va_end(ap);
}
