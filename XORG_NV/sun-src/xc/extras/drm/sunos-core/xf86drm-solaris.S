/* Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident	"@(#)xf86drm-solaris.S	1.2	06/08/21 SMI"

#if defined(lint) || defined(__lint)
#include <sys/types.h>
#else
#include <sys/asm_linkage.h>
#endif

#if defined(lint) || defined(__lint)

/*ARGSUSED*/
int
atomic_cmpset_int(volatile unsigned int *dst,
		  unsigned int exp, unsigned int src)
{
	return (0);
}

#else	/* lint */

#if defined(__amd64)

	ENTRY(atomic_cmpset_int)
	movl	%esi, %eax
	lock
	cmpxchgl	%edx, (%rdi)
	sete	%al
	movzbl	%al, %eax
	ret
	SET_SIZE(atomic_cmpset_int)

#elif defined(__i386)

	ENTRY(atomic_cmpset_int)
	movl	0x4(%esp), %edx
	movl	0x8(%esp), %eax
	movl	0xc(%esp), %ecx
	lock
	cmpxchgl	%ecx, (%edx)
	sete	%al
	movzbl	%al, %eax
	ret
	SET_SIZE(atomic_cmpset_int)

#endif	/* __i386 */

#endif /* lint */
