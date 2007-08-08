/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_BSM_AUDITWRITE_H
#define	_BSM_AUDITWRITE_H

#pragma ident	"@(#)auditwrite.h	1.3	06/03/05 SMI"

#include <bsm/libbsm.h>
#include <tsol/label.h>
#include <sys/tsol/label_macro.h>

#ifdef	__cplusplus
extern "C" {
#endif


/*
 * This file contains declarations and defines for use with
 * auditwrite(3).
 */

/* Mandatory last arg on auditwrite(3) invocation line */

#define	AW_END		(0)

/* control commands */

#define	AW_ABORT	(1) /* Stop everything.			*/
#define	AW_APPEND	(2) /* Append to recbuf.		*/
#define	AW_DEFAULTRD	(3) /* Use default rd.			*/
#define	AW_DISCARD	(4) /* Discard all audit recs.		*/
#define	AW_DISCARDRD	(5) /* Discard one audit rec.		*/
#define	AW_FLUSH	(6) /* Flush queued recbufs.		*/
#define	AW_GETRD	(7) /* Get a recbuf descriptor.   	*/
#define	AW_NOPRESELECT	(8) /* No user level preselection	*/
#define	AW_NOQUEUE	(9) /* Stop queueing. Flush.		*/
#define	AW_NOSAVE	(10) /* Don't attach save buffer.	*/
#define	AW_NOSERVER	(11) /* We're not a trusted server.	*/
#define	AW_PRESELECT	(12) /* Do user level preselection.	*/
#define	AW_QUEUE	(13) /* Buffer all records.		*/
#define	AW_SAVERD	(14) /* Attach save buffer.		*/
#define	AW_SERVER	(15) /* We're a trusted server.		*/
#define	AW_USERD	(16) /* Use recbuf descriptor.		*/
#define	AW_WRITE	(17) /* Write to trail.			*/

/*
 * Attribute commands. These tell audiwrite(3) what kind
 * of data to expect.
 */

#define	AW_ACL		(25)
#define	AW_ARG		(26)
#define	AW_ATTR		(27)
#define	AW_DATA		(28)
#define	AW_EVENT	(29)
#define	AW_EVENTNUM	(30)
#define	AW_EXEC_ARGS	(31)
#define	AW_EXEC_ENV	(32)
#define	AW_EXIT		(33)
#define	AW_GROUPS	(34)
#define	AW_INADDR	(35)
#define	AW_IN_ADDR	AW_INADDR
#define	AW_IPC		(36)
#define	AW_IPC_PERM	(37)
#define	AW_IPORT	(38)
#define	AW_OPAQUE	(39)
#define	AW_PATH		(40)
#define	AW_PROCESS	(41)
#define	AW_RETURN	(42)
#define	AW_SOCKET	(43)
#define	AW_SUBJECT	(44)
#define	AW_TEXT		(45)
#define	AW_UAUTH	(46)
#define	AW_CMD		(47)

#define	AW_LEVEL	(52)
#define	AW_LIAISON	(53)
#define	AW_PRIVILEGE	(54)	/* OBSOLETE */
#define	AW_SLABEL	(55)
#define	AW_USEOFPRIV	(56)

#define	AW_XATOM	(60)
#define	AW_XCOLORMAP	(61)
#define	AW_XCURSOR	(62)
#define	AW_XFONT	(63)
#define	AW_XGC		(64)
#define	AW_XOBJ		(65)	/* OBSOLETE */
#define	AW_XPIXMAP	(66)
#define	AW_XPROPERTY	(67)
#define	AW_XPROTO	(68)	/* OBSOLETE */
#define	AW_XSELECT	(69)
#define	AW_XWINDOW	(70)
#define	AW_XCLIENT	(71)
#define	AW_PROCESS_EX	(72)
#define	AW_SUBJECT_EX	(73)
/*
 * The next is the last and highest numbered valid command code; if more
 * are added, remember to update AW_CMD_MAX in auditwrite.c.
 */
#define	AW_IN_ADDR_EX	(74)

/*
 * describe data specified with AW_DATA
 */

#define	AWD_BYTE	((char)1)
#define	AWD_CHAR	((char)2)
#define	AWD_SHORT	((char)3)
#define	AWD_INT		((char)4)
#define	AWD_LONG	((char)5)
#define	AWD_INT32	((char)6)
#define	AWD_INT64	((char)7)

/*
 * describe how to print data specified with AW_DATA
 */

#define	AWD_BINARY	((char)1)
#define	AWD_OCTAL	((char)2)
#define	AWD_DECIMAL	((char)3)
#define	AWD_HEX		((char)4)
#define	AWD_STRING	((char)5)

/*
 * auditwrite(3) error indicators
 */

#define	AW_ERR_NO_ERROR			(0)

#define	AW_ERR_ADDR_INVALID		(1)
#define	AW_ERR_ALLOC_FAIL		(2)
#define	AW_ERR_AUDITON_FAIL		(3)
#define	AW_ERR_AUDIT_FAIL		(4)
#define	AW_ERR_CMD_INCOMPLETE		(5)
#define	AW_ERR_CMD_INVALID		(6)
#define	AW_ERR_CMD_IN_EFFECT		(7)
#define	AW_ERR_CMD_NOT_IN_EFFECT	(8)
#define	AW_ERR_CMD_TOO_MANY		(9)
#define	AW_ERR_EVENT_ID_INVALID		(10)
#define	AW_ERR_EVENT_ID_NOT_SET		(11)
#define	AW_ERR_GETAUDIT_FAIL		(12)
#define	AW_ERR_QUEUE_SIZE_INVALID	(13)
#define	AW_ERR_RD_INVALID		(14)
#define	AW_ERR_REC_TOO_BIG		(15)
#define	AW_ERR_NO_PLABEL		(16)

#ifdef __STDC__
extern int auditwrite(int, ...);	/* writes audit records */
extern int aw_errno;			/* error number */
extern void aw_perror(const char *);	/* print error */
extern void aw_perror_r(const int, const char *);	/* print error for */
							/* record descriptor */
extern int aw_geterrno(const int);	/* get error for record descriptor */
extern char *aw_strerror(const int);	/* format error into string */
#else
extern int auditwrite();
extern int aw_errno;
extern void aw_perror();
extern void aw_perror_r();
extern int aw_geterrno();
extern char *aw_strerror();
#endif /* __STDC__ */


extern token_t *au_to_upriv(char, char *);

extern token_t *au_to_tsol_xatom(char *);
extern token_t *au_to_tsol_xclient(uint32_t);
extern token_t *au_to_tsol_xcolormap(int32_t, uid_t);
extern token_t *au_to_tsol_xcursor(int32_t, uid_t);
extern token_t *au_to_tsol_xfont(int32_t, uid_t);
extern token_t *au_to_tsol_xgc(int32_t, uid_t);
extern token_t *au_to_tsol_xpixmap(int32_t, uid_t);
extern token_t *au_to_tsol_xproperty(int32_t, uid_t, char *);
extern token_t *au_to_tsol_xselect(char *, char *, char *);
extern token_t *au_to_tsol_xwindow(int32_t, uid_t);


#ifdef	__cplusplus
}
#endif

#endif /* _BSM_AUDITWRITE_H */
