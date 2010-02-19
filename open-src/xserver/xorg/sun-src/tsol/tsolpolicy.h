/* Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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

#ifndef	_TSOL_POLICY_H
#define	_TSOL_POLICY_H


#ifdef	__cplusplus
extern "C" {
#endif

#include <assert.h>

#define	PASSED		0	/* success code 0 */
#ifndef FAILED
#define	FAILED		1	/* failed is non-zero (could be error no) */
#endif /* FAILED */

#define AUDIT_SUCCESS 	1
#define AUDIT_FAILURE 	0

/*
 * Policy checking flags
 */

enum xpolicy_flags {
	TSOL_MAC   = 0x00000001,	/* MAC policy */
	TSOL_DAC   = 0x00000002,	/* DAC floating */
	TSOL_FLOAT = 0x00000004,	/* float ILs */
	TSOL_AUDIT = 0x00000008,	/* perform auditing */
	TSOL_PRIV  = 0x00000010,	/* privilege check */
	TSOL_TP    = 0x00000020,	/* Trusted Path check */
	TSOL_READOP  = 0x00000040,	/* read operation */
	TSOL_WRITEOP = 0x00000080,	/* write operation */
	TSOL_OWNER = 0x00000100,	/* Check for workstation owner */
	TSOL_DOMINATE    = 0x00000200,	/* Check for default uid */
	TSOL_ALL   = 0x0fffffff		/* do them all */
};

typedef enum xpolicy_flags xpolicy_t;

#define	XTSOL_FAIL	1	/* Replaces SecurityErrorOperation */
#define	XTSOL_ALLOW	2	/* Replaces SecurityAllowOperation */
#define	XTSOL_IGNORE	3	/* Replaces SecurityIgnoreOperation */


#ifndef	MAJOROP
#define	MAJOROP ((xReq *)client->requestBuffer)->reqType
#endif	/* MAJOROP */
#define	RES_TYPE(xid)	((xid) & (0x8000000F))


/*
 * Function prototypes
 */

void init_win_privsets(void);
void free_win_privsets(void);

#ifdef	__cplusplus
}
#endif

#endif	/* _TSOL_POLICY_H */
