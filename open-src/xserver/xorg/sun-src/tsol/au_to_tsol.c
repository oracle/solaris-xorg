/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)au_to_tsol.c	1.3	06/03/05 SMI"

#include <sys/types.h>
#include <unistd.h>
#include <bsm/audit.h>
#include <bsm/audit_record.h>
#include <bsm/libbsm.h>
#include <priv.h>
#include <sys/ipc.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/vnode.h>
#include <sys/tsol/label.h>
#include <malloc.h>
#include <net/route.h>
#include <netinet/in.h>
#include <netinet/in_pcb.h>
#include <string.h>


static token_t *
get_token(int s)
{
	token_t *token;	/* Resultant token */

	if ((token = (token_t *)malloc(sizeof (token_t))) == NULL)
		return (NULL);
	if ((token->tt_data = malloc(s)) == NULL) {
		free(token);
		return (NULL);
	}
	token->tt_size = s;
	token->tt_next = NULL;
	return (token);
}

/*
 * au_to_in_addr_ex
 * returns:
 *	pointer to an extended IP address token
 */
token_t *
au_to_in_addr_ex(int32_t *internet_addr)
{
	token_t *token;			/* local token */
	adr_t adr;			/* adr memory stream header */
	char data_header_v4 = AUT_IN_ADDR;	/* header for v4 token */
	char data_header_v6 = AUT_IN_ADDR_EX;	/* header for v6 token */
	int32_t type = AU_IPv6;

	if (IN6_IS_ADDR_V4MAPPED((in6_addr_t *)internet_addr)) {
		struct in_addr	ip;

		IN6_V4MAPPED_TO_INADDR((struct in6_addr *)internet_addr, &ip);

		token = get_token(sizeof (char) +
		    (sizeof (char) * sizeof (struct in_addr)));
		if (token == (token_t *)0)
			return ((token_t *)0);
		adr_start(&adr, token->tt_data);
		adr_char(&adr, &data_header_v4, 1);
		adr_char(&adr, (char *)&ip, sizeof (struct in_addr));
	} else {
		token = get_token(sizeof (char) + sizeof (uint32_t) +
		    (sizeof (char) * sizeof (struct in6_addr)));
		if (token == (token_t *)0)
			return ((token_t *)0);
		adr_start(&adr, token->tt_data);
		adr_char(&adr, &data_header_v6, 1);
		adr_int32(&adr, (int32_t *)&type, 1);
		adr_char(&adr, (char *)internet_addr, sizeof (struct in6_addr));
	}

	return (token);
}

/*
 * au_to_tsol_xclient
 * return s:
 *	pointer to a xclient token.
 */
token_t *
au_to_tsol_xclient(uint32_t client)
{
	token_t *token;			/* local token */
	adr_t adr;			/* adr memory stream header */
	char data_header = AUT_XCLIENT;	/* header for this token */

	token = get_token(sizeof (char) + sizeof (int32_t));
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_int32(&adr, (int32_t *)&client, 1);

	return (token);
}

/*
 * au_to_ipc_perm
 * return s:
 *	pointer to token containing a System V IPC attribute token.
 */
token_t *
au_to_ipc_perm(struct ipc_perm *perm)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_IPC_PERM;	/* header for this token */
	int32_t value;

	token = get_token(sizeof (char) + (sizeof (int32_t)*7));
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	value = (int32_t)perm->uid;
	adr_int32(&adr, &value, 1);
	value = (int32_t)perm->gid;
	adr_int32(&adr, &value, 1);
	value = (int32_t)perm->cuid;
	adr_int32(&adr, &value, 1);
	value = (int32_t)perm->cgid;
	adr_int32(&adr, &value, 1);
	value = (int32_t)perm->mode;
	adr_int32(&adr, &value, 1);
	value = (int32_t)perm->seq;
	adr_int32(&adr, &value, 1);
	value = (int32_t)perm->key;
	adr_int32(&adr, &value, 1);

	return (token);
}

/*
 * au_to_upriv
 * return s:
 *	pointer to token chain containing a use of a privilege token.
 */
token_t *
au_to_upriv(char flag, char *priv)
{
	token_t *token;			/* local token */
	adr_t adr;			/* adr memory stream header */
	char data_header = AUT_UPRIV;	/* header for this token */
	short bytes;			/* length of string */

	bytes = strlen(priv) + 1;

	token = get_token(sizeof (char) + sizeof (char) + sizeof (ushort_t) +
	    bytes);
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_char(&adr, &flag, 1);		/* success/failure */
	adr_short(&adr, &bytes, 1);
	adr_char(&adr, priv, bytes);

	return (token);
}

/*
 * au_to_tsol_xatom
 * return s:
 *	pointer to token chain containing a XATOM token.
 */
token_t *
au_to_tsol_xatom(char *atom)
{
	token_t *token;			/* local token */
	adr_t adr;			/* adr memory stream header */
	char data_header = AUT_XATOM;	/* header for this token */
	short bytes;			/* length of string */

	bytes = strlen(atom) + 1;

	token = get_token(sizeof (char) + sizeof (ushort_t) + bytes);
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_short(&adr, &bytes, 1);
	adr_char(&adr, atom, bytes);

	return (token);
}

/*
 * au_to_tsol_xcolormap
 * return s:
 *	pointer to token chain containing a XCOLORMAP token.
 */
token_t *
au_to_tsol_xcolormap(int32_t xid, uid_t cuid)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_XCOLORMAP;	/* header for this token */

	token = get_token(sizeof (char) + (2 * sizeof (int32_t)));
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_int32(&adr, &xid, 1);
	adr_int32(&adr, (int32_t *)&cuid, 1);

	return (token);
}

/*
 * au_to_tsol_xcursor
 * return s:
 *	pointer to token chain containing a XCURSOR token.
 */
token_t *
au_to_tsol_xcursor(int32_t xid, uid_t cuid)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_XCURSOR;		/* header for this token */

	token = get_token(sizeof (char) + (2 * sizeof (int32_t)));
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_int32(&adr, &xid, 1);
	adr_int32(&adr, (int32_t *)&cuid, 1);

	return (token);
}

/*
 * au_to_tsol_xfont
 * return s:
 *	pointer to token chain containing a XFONT token.
 */
token_t *
au_to_tsol_xfont(int32_t xid, uid_t cuid)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_XFONT;		/* header for this token */

	token = get_token(sizeof (char) + (2 * sizeof (int32_t)));
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_int32(&adr, &xid, 1);
	adr_int32(&adr, (int32_t *)&cuid, 1);

	return (token);
}

/*
 * au_to_tsol_xgc
 * return s:
 *	pointer to token chain containing a XGC token.
 */
token_t *
au_to_tsol_xgc(int32_t xid, uid_t cuid)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_XGC;		/* header for this token */

	token = get_token(sizeof (char) + (2 * sizeof (int32_t)));
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_int32(&adr, &xid, 1);
	adr_int32(&adr, (int32_t *)&cuid, 1);

	return (token);
}

/*
 * au_to_tsol_xpixmap
 * return s:
 *	pointer to token chain containing a XPIXMAP token.
 */
token_t *
au_to_tsol_xpixmap(int32_t xid, uid_t cuid)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_XPIXMAP;		/* header for this token */

	token = get_token(sizeof (char) + (2 * sizeof (int32_t)));
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_int32(&adr, &xid, 1);
	adr_int32(&adr, (int32_t *)&cuid, 1);

	return (token);
}

/*
 * au_to_tsol_xproperty
 * return s:
 *	pointer to token chain containing a ... token.
 */
token_t *
au_to_tsol_xproperty(int32_t xid, uid_t cuid, char *name)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_XPROPERTY;	/* header for this token */
	short bytes;				/* length of string */

	bytes = strlen(name) + 1;

	token = get_token(sizeof (char) + (2 * sizeof (int32_t))
			+ sizeof (short) + bytes);
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_int32(&adr, &xid, 1);
	adr_int32(&adr, (int32_t *)&cuid, 1);
	adr_short(&adr, &bytes, 1);
	adr_char(&adr, name, bytes);

	return (token);
}

/*
 * au_to_tsol_xselect
 * return s:
 *	pointer to token chain containing a ... token.
 */
token_t *
au_to_tsol_xselect(char *propname, char *proptype, char *windata)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_XSELECT;		/* header for this token */
	short bytes1, bytes2, bytes3;		/* length of string */

	bytes1 = strlen(propname) + 1;
	bytes2 = strlen(proptype) + 1;
	bytes3 = strlen(windata) + 1;

	token = get_token(sizeof (char) + (3 * sizeof (short)) +
			bytes1 + bytes2 + bytes3);
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_short(&adr, &bytes1, 1);
	adr_char(&adr, propname, bytes1);
	adr_short(&adr, &bytes2, 1);
	adr_char(&adr, proptype, bytes2);
	adr_short(&adr, &bytes3, 1);
	adr_char(&adr, windata, bytes3);

	return (token);
}

/*
 * au_to_tsol_xwindow
 * return s:
 *	pointer to token chain containing a XWINDOW token.
 */
token_t *
au_to_tsol_xwindow(int32_t xid, uid_t cuid)
{
	token_t *token;				/* local token */
	adr_t adr;				/* adr memory stream header */
	char data_header = AUT_XWINDOW;		/* header for this token */

	token = get_token(sizeof (char) + (2 * sizeof (int32_t)));
	if (token == (token_t *)0)
		return ((token_t *)0);
	adr_start(&adr, token->tt_data);
	adr_char(&adr, &data_header, 1);
	adr_int32(&adr, &xid, 1);
	adr_int32(&adr, (int32_t *)&cuid, 1);

	return (token);
}
