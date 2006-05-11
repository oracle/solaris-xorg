/* scf-smartcard.h --- Code to deal with smart card authentication
 * for xscreensaver, Copyright (c) 1993-1998, 2000 Jamie Zawinski <jwz@jwz.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 *
 *
 * written by Padraig O'Briain (padraig.obriain@sun.com) 
 *
 * Copyright 2003 Sun Microsystems, Inc.  All rights reserved.
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

#ifndef __SCF_SMARTCARD_H__
#define __SCF_SMARTCARD_H__

enum passwd_state { pw_read, pw_ok, pw_null, pw_fail, pw_cancel, pw_time , pw_another};

typedef struct scf_saver_info scf_saver_info;

typedef struct smartcard_info smartcard_info;

struct scf_saver_info {
  Bool use_smartcard;
  int card_removal_timeout;
  Bool ignore_card_removal;
  Bool reauth_after_card_removal;
  Time reauth_timeout;
  Time card_removal_logout_wait;
  Time first_card_event_timeout;
  smartcard_info* scdata;
};

enum scstate {
    SCD_AUTH_CARD_PRESENT,
    SCD_WAIT_REMOVAL_TIMEOUT,
    SCD_CARD_REMOVAL_IGNORED,
    SCD_WAIT_FOR_CARD,
    SCD_WAIT_REMOVAL_LOGOUT_WAIT,
    SCD_ANY_CARD_PRESENT,
    SCD_WAIT_REMOVAL_LOGOUT_EXPIRED,
    SCD_AUTH_REQUIRED,
    SCD_WRONG_USER,
    SCD_AUTH_IN_PROGRESS,
    SCD_NO_USER
};

struct smartcard_info {
  enum scstate state;
  int wait_removal_logout_timer;
  SCF_Event_t last_event;
  SCF_Session_t session_handle;
  SCF_Terminal_t terminal_handle;
  SCF_Card_t card_handle;
  SCF_ListenerHandle_t myListener;
  char * smartcard_username;
  Time message_timeout;
};

#endif /* __SCF_SMARTCARD_H__ */
