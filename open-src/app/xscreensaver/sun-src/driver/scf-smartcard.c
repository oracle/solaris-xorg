/* scf-smartcard.c --- Code to deal with smart card authentication
 *  for  xscreensaver, Copyright (c) 1993-1997, 1998, 2000
 *  Jamie Zawinski <jwz@jwz.org>
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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef NO_LOCKING  /* whole file */

#include <thread.h>
#include <signal.h>
#include <security/pam_appl.h>
#include <pwd.h>
#include <ctype.h>

#include <X11/Intrinsic.h>

#include "xscreensaver.h"
#include "resources.h"
#include "scf-smartcard.h"

#define CF_FILE_PATH	"/etc/smartcard/desktop.properties"
#define BUF_LEN		256
#define SEPARATOR	" \t\n"

struct pam_data {
  const char *user;
  const char *typed_passwd;
  Bool verbose_p;
};

extern  Bool Ignore_SmartCard;
extern  scf_saver_info the_scf_si;
extern  smartcard_info the_smi;
extern  void destroy_passwd_window (saver_info *si);

static int SmartCardGetProperty(FILE *fp, char *name, char *value, int max_value_len);
static void * smartcard_authenticate (void *);
static void set_smartcard_lock_state (saver_info *);
static int run_PAMsession (char *, saver_info*);
static int conversation_function (int, struct pam_message**,
        struct pam_response **, void *);
static void block_signals (void);

static char * get_username(void);
static Bool is_valid_unlock_user(char *);
static void wait_for_got_message (saver_info *si);
static char * get_smartcard_username (saver_info *si);
static void 
scfevent_handler (SCF_Event_t event,SCF_Terminal_t eventTerminal, void *data);
static void card_removal_timeout (XtPointer, XtIntervalId *);
static void start_wait_removal_timer (saver_info *);
static void card_removal_logout_wait (XtPointer, XtIntervalId *);
static void force_exit (saver_info *si);

static void dont_block_for_pam (XtPointer ptr, XtIntervalId *invId);
static XtIntervalId card_removal_logout_wait_id;
static XtIntervalId card_removal_timeout_id;

static void report_smartcard_init_error (saver_info*, char*, uint32_t);
static void report_smartcard_error (char*, uint32_t);


static struct pam_message safe_pam_message;
static struct pam_message *saved_pam_message = &safe_pam_message;

static Bool b_PAM_prompted;
static Bool b_first_prompt;
static thread_t pam_thread = (thread_t) NULL;

static cond_t c_got_response = DEFAULTCV;
static mutex_t m_got_response;
static Bool b_got_response = False;

static cond_t c_validated_response = DEFAULTCV;
static mutex_t m_validated_response;
static Bool b_validated_response = False;

static cond_t c_got_username = DEFAULTCV;
static mutex_t m_got_username;
static Bool b_got_username = False;

static cond_t c_got_message = DEFAULTCV;
static mutex_t m_got_message;
static Bool b_got_message = False;

static cond_t c_event_processed = DEFAULTCV;
static mutex_t m_event_processed = DEFAULTMUTEX;
static Bool b_event_processed = True;

static Bool  b_validation_succeeded;
static Bool b_validation_finished;
static Bool b_got_PAM_message;

static char *saved_user_input = NULL;

static Bool b_card_event_occurred = False;
static XtSignalId sigusr2_id;
static thread_t main_thread;
static void sigusr2_handler(void);
static void sigusr2_callback (XtPointer client_data, XtSignalId *id);
static void synthetic_event_callback (XtPointer, XtIntervalId *);

/*
 * This function is called when set_locked_p() is called, i.e when the
 * screen is being locked or unlocked. If the screen is being unlocked 
 * nothing is done. If the screen is being locked the authentication
 * thread is created if it does not already exist.
 */
void scf_set_locked_p (saver_info *si, Bool locked_p)
{
  if ((si->scf_si) && (si->scf_si->use_smartcard))
  {
   if (locked_p)
    {
      /*
       * Check that we have the name of the user for unlocking;
       * otherwise do not lock
       */
      if (!get_username())
      {
        fprintf (real_stderr, 
                 "%s: Unable to get username for authentication.\n", 
                 blurb());
        return;
      }
      /* 
       * When locking the screen we create the Authentication thread
       * if it does not already exist
       */ 
     if (!pam_thread)
      {
        int b_created_auth_thread = 1;
        set_smartcard_lock_state(si);

        thr_setconcurrency (5);
        if (thr_create (NULL, 0, smartcard_authenticate, (void *) si, 
          THR_BOUND | THR_NEW_LWP | THR_DETACHED, &pam_thread) != 0)
        {
          b_created_auth_thread = 0;
        }
        if (!b_created_auth_thread)
        {
          /*
           * Unable to create a thread so output an error
           * and do not lock
           */
          fprintf (real_stderr, 
                 "%s: Unable to create a thread for authentication.\n", 
                 blurb());
          return;
        }
      }
    }
    else
    {
     if (si->prefs.verbose_p)
      fprintf(stderr,"in else of locked_p i.e. screen not already locked\n");

      pam_thread = (thread_t) NULL;
      /*
       * The screen is being unlocked so update the state
       */
      si->scf_si->scdata->state = SCD_AUTH_CARD_PRESENT;
      if (si->scf_si->scdata->wait_removal_logout_timer)
      {
        if (card_removal_logout_wait_id != (XtIntervalId) 0)
              XtRemoveTimeOut (card_removal_logout_wait_id);
        si->scf_si->scdata->wait_removal_logout_timer = 1;
      }
    }
  }
}

/*
 * This function is called in handle_passwd_key() to do SCF specific
 * processing of the data entred by the user.
 */ 
enum passwd_state 
scf_passwd_valid_p (saver_info *si, char *typed_passwd)
{
  saver_preferences *p = &si->prefs;
  enum passwd_state return_state;

/*mali  if (si->scf_si->use_smartcard)***/
  if (0)
  {
    /*
     * When using smart card the user name is determined from the card
     * so there is no need to call PAM with a different user if the 
     * validation failed.
     * When using smart card more than one item of data may be 
     * prompted for to complete the validation.
     */
    saved_user_input = typed_passwd;
    mutex_lock (&m_got_response);
    b_got_response = True;
    b_validated_response = False;
    b_validation_finished = False;
    cond_signal (&c_got_response);
    mutex_unlock (&m_got_response);
    /*
     * We now wait for the validation to complete
     */
/**mali
  if (si->prefs.verbose_p)
     fprintf (stderr, "---> scf_passwd_valid_p use_smartcard is True\n");
  XtAppAddTimeOut (si->app,
                   1000,
                   dont_block_for_pam, (XtPointer) si);
    mutex_lock (&m_validated_response);
    while (b_validated_response == False)
      cond_wait (&c_validated_response, &m_validated_response);
    mutex_unlock (&m_validated_response);
****/

    if ((!b_validation_finished) || (!b_validation_succeeded))
    {
      if (!b_validation_finished)
      {
        return_state = pw_another;
      }
      else
      {
        return_state = pw_fail;
        /*
         * PAM will be called again so tell the thread that we have
         * user to authenticate
         */
        mutex_lock (&m_got_username); 
        b_validation_finished = False;
        b_got_username = True;
        cond_signal (&c_got_username);
        mutex_unlock (&m_got_username);
      }
      /*
       * We wait for PAM to generate the next prompt before
       * continuing
       */
      wait_for_got_message(si);
    }
    else
    {
      return_state = pw_ok;
    } 
  }
  else {
   if (si->prefs.verbose_p)
   {
     fprintf (stderr, "---> scf_passwd_valid_p calling passwd_valid_p\n");
   }
   if (passwd_valid_p ())
      return (pw_ok);
    else
      return (pw_fail);
  }
  return return_state;
}

/*
 * This function checks what type of dialog should be displayed
 * based on the card state.
 */
Bool 
check_smartcard_dialog (saver_info *si)
{
  /*
   * ret_val == False means display authentication dialog
   * ret_val == True means display message 
   */
  Bool ret_val = False;

  if (si->prefs.verbose_p)
   {
     fprintf (stderr, "---> check_smartcard_dialog()\n");
     fprintf (stderr, "use_smartcard=%d state=%d\n", si->scf_si->use_smartcard, si->scf_si->scdata->state);
   }

  if (si->scf_si->use_smartcard)
  {
    switch (si->scf_si->scdata->state)
    {
      /* if card is present dont put any message on screen **/
      case SCD_AUTH_CARD_PRESENT:
	ret_val = False;
	break;

      case SCD_WAIT_FOR_CARD:
      case SCD_WAIT_REMOVAL_LOGOUT_WAIT:
      case SCD_NO_USER:
      case SCD_WRONG_USER:
        ret_val = True;
        break;

      case SCD_AUTH_REQUIRED:
      {
        char *sm_user;

        sm_user = get_smartcard_username(si); 

        /* if ((sm_user) && is_valid_unlock_user (sm_user)) */
        if ((sm_user))
        {
          /*
           * Tell the authentication thread to continue as we now have a 
           * valid user name to authenticate
           */
	if (si->prefs.verbose_p)
         fprintf (stderr, "---> got valid user continue to authenticate!!!\n");

          mutex_lock (&m_got_username);
          b_got_username = True;
          cond_signal (&c_got_username);
          mutex_unlock (&m_got_username);

          si->scf_si->scdata->state = SCD_AUTH_IN_PROGRESS;

          /*
           * Wait for PAM to prompt
           */

          wait_for_got_message (si);

          if (!b_got_PAM_message)
          {
            /*
             * The PAM conversation function was not called, probably because
             * the card was removed very quickly
             */
/*mali no need for this..causing AUTH_REQUIRED message to pop up..
	    if (si->prefs.verbose_p)
               fprintf (stderr, "---> setting SCD_AUTH_REQUIRED!!!\n");
            si->scf_si->scdata->state = SCD_AUTH_REQUIRED;
****/
          si->scf_si->scdata->state = SCD_AUTH_IN_PROGRESS;
            ret_val = False;
          }
        }
        else
        {
          si->scf_si->scdata->state = (sm_user) ? SCD_WRONG_USER : SCD_NO_USER;
	  if (si->prefs.verbose_p)
            fprintf (stderr, "in else state=%d\n", si->scf_si->scdata->state);
          ret_val = True;
        }
        break;
      } 
      default:
        break;
    }      
  }
  if (si->prefs.verbose_p)
   {
    fprintf (stderr, "<--- check_smartcard_dialog ret_val=%d state=%d\n", 
				     ret_val, si->scf_si->scdata->state);
   }
  return ret_val;
}

/*
 * This function returns the message which should be displayed to the user
 * for the smart card state in an array of character strings, each one
 * corresponding to one line.
 */
char **
get_scf_message (saver_info *si)
{
  char *msg, *lockmsg;
  char *puser;

  if (si->prefs.verbose_p)
    fprintf (stderr, "---> get_scf_message()\n");

  /*si->scf_si->scdata->message_timeout = si->prefs.passwd_timeout;**/
  si->scf_si->scdata->message_timeout = 10000;
  puser = get_username ();
  if (!puser) puser = "";

  if (si->prefs.verbose_p)
   {
    fprintf(stderr, "*********SMARTCARD PROPS ***********\n");
    fprintf(stderr, "\t--> username=%s \n", puser);
    fprintf(stderr,"\t\tmsg timeout=%d\n",si->scf_si->scdata->message_timeout);
    fprintf(stderr,"\t\t STATE=%d\n",si->scf_si->scdata->state);
    fprintf(stderr,"\t\tlogoutWait=%d\n",si->scf_si->card_removal_logout_wait);
    fprintf(stderr, "************************************\n");
   }


/***mali we are getting timeout=60000 which is too long*/ 
/* This is set from message_timeout = si->prefs.passwd_timeout
   which is usually in minutes hence it is too long setting it 10 secs
   No...people filed bug against this 4771114 so not resetting it to 10sec
  si->scf_si->scdata->message_timeout = 10000;
*******/

  switch (si->scf_si->scdata->state) {

    case SCD_WAIT_FOR_CARD:
      /* If ignore card removal is True then no need to display logout msg */
      if (si->scf_si->reauth_after_card_removal)
       {
      	msg = get_string_resource ("message.SCF.waitforcard", 
					"Dialog.Label.Label");
      	if (msg == NULL)
		msg = strdup ("Please insert your smartcard..");
        si->scf_si->scdata->message_timeout = si->scf_si->reauth_timeout; 
        /*mali si->scf_si->reauth_timeout; use this instead of 10000 */
        /*si->scf_si->scdata->message_timeout = si->scf_si->reauth_timeout;*/
        lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
        sprintf (lockmsg, msg, puser);
       }
      else /* ignore_card_removal is False, we tell user he will be logged out*/
       {
        msg = get_string_resource ("message.SCF.waitremovallogoutwait", 
                                   "Dialog.Label.Label");
        if (msg == NULL)
	    msg = strdup ("Waiting to Log you out...in %d secs. Locked by user %s");
        lockmsg = (char *) malloc (strlen (msg) + 10 /* for integer */ + 
               				+ strlen (puser) + 1);
        si->scf_si->scdata->message_timeout = 
			si->scf_si->card_removal_logout_wait * 1000;
        sprintf (lockmsg, msg, si->scf_si->card_removal_logout_wait/1000, puser);
       }
      break;

    case SCD_WAIT_REMOVAL_LOGOUT_WAIT:
      msg = get_string_resource ("message.SCF.waitremovallogoutwait", 
           "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup ("Waiting to Log you out...");

      si->scf_si->scdata->message_timeout = 
				si->scf_si->card_removal_logout_wait * 1000;
/*mali too long		si->scf_si->card_removal_logout_wait * 1000;
***/
      lockmsg = (char *) malloc (strlen (msg) + 10 /* for integer */ + 
               + strlen (puser) + 1);
      sprintf (lockmsg, msg, si->scf_si->card_removal_logout_wait/1000, puser);
      break;

    case SCD_AUTH_REQUIRED:
      msg = get_string_resource ("message.SCF.authrequired", 
                "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup ("Authorization is required...");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

    case SCD_WRONG_USER:
      msg = get_string_resource ("message.SCF.wronguser", 
                "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup ("You are the wrong user system already locked by another user...");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

   case SCD_NO_USER:
      msg = get_string_resource ("message.SCF.nouser", "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup ("No such user found...");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

/*mali all cases added*/
   case SCD_AUTH_CARD_PRESENT:
      msg = get_string_resource ("message.SCF.authcardpresent", 
				     "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup ("Could not get message string for SCD_AUTH_CARD_PRESENT");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

   case SCD_WAIT_REMOVAL_TIMEOUT:
      msg = get_string_resource ("message.SCF.waitremovaltimeout", 
					"Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup 
		("Could not get message string for SCD_WAIT_REMOVAL_TIMEOUT");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

   case SCD_CARD_REMOVAL_IGNORED:
      msg = get_string_resource ("message.SCF.cardremovalignored", 
					"Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup 
		("Could not get message string for SCD_CARD_REMOVAL_IGNORED");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

   case SCD_ANY_CARD_PRESENT:
      msg = get_string_resource ("message.SCF.anycardpresent", 
				        "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup ("Could not get message string for SCD_ANY_CARD_PRESENT");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

   case SCD_WAIT_REMOVAL_LOGOUT_EXPIRED:
      msg = get_string_resource ("message.SCF.waitremovallogoutexpired", 
				 "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup 
	  ("Could not get message string for SCD_WAIT_REMOVAL_LOGUT_EXPIRED");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

   case SCD_AUTH_IN_PROGRESS:
      msg = get_string_resource ("message.SCF.authinprogress", 
				 "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup ("Could not get message string for SCD_AUTH_IN_PROGRESS");
      lockmsg = (char *) malloc (strlen (msg) + strlen (puser) +1);
      sprintf (lockmsg, msg, puser);
      break;

   default:
      msg = get_string_resource ("message.SCF.default", "Dialog.Label.Label");
      if (msg == NULL)
	msg = strdup 
	        ("Could not get message string for default smartcard state");
      lockmsg = (char *) malloc (strlen (msg) + 10 /* for integer */ + 
               + strlen (puser) + 1);
      sprintf (lockmsg, msg, si->scf_si->scdata->state, puser);
      break;
  } 

  if (si->prefs.verbose_p)
   {
     fprintf(stderr, "\t\tlockmsg is %s \n", lockmsg);
     fprintf(stderr,"\t\t<--scf_get_message  STATE=%d\n",
				si->scf_si->scdata->state);
   }

  free (msg);
  {
    /*
     * Split the string into an array of character strings, one per line
     */
    int i = 0;
    char ** return_array;
    char * pchar;

    pchar = lockmsg;

    while ((pchar = strchr (pchar, '\n')))
    {
      i++;
      pchar++;
    }

    return_array = (char **) malloc ((i+1) * sizeof (char *));
    if (return_array)
    {
       i = 0;
       return_array[i] = lockmsg;
       pchar = lockmsg;

       while ((pchar = strchr (pchar, '\n')))
       {
         *pchar = '\0';
         return_array[++i] = ++pchar;
       }

       return_array[++i] = NULL;
    }
    return return_array;
  }
}

/*************************************<->*************************************
 *
 * SmartCardGetProperty(fp name value max_value_len)
 *
 *
 *  Description:
 *  -----------
 *  Retrieves specified smartcard property value from a file.
 *
 *
 *  Inputs:
 *  ------
 *  fp		  = File handle, in this case : /etc/smartcard/desktop.properties
 *  name	  = Name of the property to retrieve a value for
 *  value	  = Char buffer to store retrieved values into.
 *  max_value_len = Maximum number of bytes to be written to value buffer.
 *
 *
 *  Outputs:
 *  -------
 *  Return = 0 on error or if property (name) not found, 'n' where n is the number
 *      	of bytes written to char buffer "value".
 *
 *
 *  Comments:
 *  --------
 *  This is a replacement for SCF_GetClientProperty() see bugid #4499187.
 *
 *************************************<->***********************************/

static int SmartCardGetProperty(FILE *fp, char *name, char *value, int max_value_len)
{
    register int i = 0;
    char buf[BUF_LEN];
    char *ptr1, *ptr2, *tmp_buf, *ptr3 = NULL;

    if (fp == NULL)
	return (0);
 

    /*
     * Reset file offset to origin of file
     * so stuff doesn't get skipped over.
     */
    fseek(fp, 0, SEEK_SET);

    while (fgets(buf, BUF_LEN, fp) != NULL) {
            /* Skip over white space first */
            ptr3 = buf;
            i = 0;
            while (buf[i] && isspace (buf[i])) {
		i++;
                    ptr3++;
		}
	/* Then get rid of # */
	for (i = 0; i < strlen(ptr3); i++) {
            if (ptr3[i] == '#') {
		ptr3[i] = NULL;
		break;
            }
	}

	/*
         * then, divide ptr3 by =
	 * ptr1 points to the top of the left-hand side
	 * ptr2 points to the top of the right-hand side
	 */
	if (ptr3[0] == NULL)
            continue;

	ptr1 = ptr3;
	for (i = 0; i < strlen(ptr3); i++) {
            if (ptr3[i] == '=') {
		ptr3[i] = NULL;
		ptr2 = ptr3 + i + 1;
		break;
            }
	}
        if (*ptr1 == NULL || *ptr2 == NULL)
            break;

	tmp_buf = strtok(ptr1, SEPARATOR);

	/* more than 1 tokens in the left -> error */
        if (strtok(NULL, SEPARATOR) != NULL)
            break;

	if (strcmp(name, tmp_buf) == 0) {
            /* property name matched. */
            tmp_buf = strtok(ptr2, SEPARATOR);
            /* more than 1 tokens in the right -> error */
            if (strtok(NULL, SEPARATOR) != NULL)
		break;
            if (strlcpy(value, tmp_buf, max_value_len) >= max_value_len)
	     {
               fprintf(stderr, "Error Not enough space to write smartcard property name=%s value=%s max_val_len=%d tmp_buf=%s\n", name, value, max_value_len, tmp_buf);
                            return 0;
	     }
            return (strlen(value));
	}
    }
/**** if debugging *****
    fprintf(stderr, "Error fgets failed for property name=%s buf=%s\n", name, buf);
**/
    return (0);
}


/*
 * The function must be called while we are still root.
 *
 * It gets the values for SCF properties used when screen locking
 */
void
init_scf_properties (saver_info *si)
{
  char str_buf[BUF_LEN];
  int i, rv;
  FILE *fp = NULL;

#define TYPE_INT 0
#define TYPE_BOOL 1

  struct ocfProperties {
    char* name;
    int type;
    char ** value;
  } ;

struct ocfProperties* OcfProperties;


/***************************************************************************
   Solaris 8 and Solaris 9 have different names for these properties
   We are no longer using SCF_GetClientProperty call instead we have our
   own function SmartCardGetProperty() which is much more reliable.
   Hence we need two sets of property names to look for in appropriate files
   for solaris8 and solaris9

   IMP in future dtsession should be changed to xscreensaver for sol8 props.
****************************************************************************/
struct ocfProperties OcfProperties_sol8[] = {
    "ocf.client.dtsession.root.useSmartCard",		TYPE_BOOL,
    (char **) &(the_scf_si.use_smartcard),
    "ocf.client.dtsession.root.cardRemovalTimeout",	TYPE_INT,
    (char **) &(the_scf_si.card_removal_timeout),
    "ocf.client.dtsession.root.ignoreCardRemoval",	TYPE_BOOL,
    (char **) &(the_scf_si.ignore_card_removal),
    "ocf.client.dtsession.root.reauthAfterCardRemoval",	TYPE_BOOL,
    (char **) &(the_scf_si.reauth_after_card_removal),
    "ocf.client.dtsession.root.reauthTimeout",		TYPE_INT,
    (char **) &(the_scf_si.reauth_timeout),
    "ocf.client.dtsession.root.cardRemovalLogoutWait",	TYPE_INT,
    (char **) &(the_scf_si.card_removal_logout_wait),
    "ocf.client.dtsession.root.firstCardEventTimeout",	TYPE_INT,
    (char **) &(the_scf_si.first_card_event_timeout),
  };

struct ocfProperties OcfProperties_sol9[] = {
    "desktop.useSmartCard",		TYPE_BOOL,
    (char **) &(the_scf_si.use_smartcard),
    "desktop.cardRemovalTimeout",	TYPE_INT,
    (char **) &(the_scf_si.card_removal_timeout),
    "desktop.ignoreCardRemoval",	TYPE_BOOL,
    (char **) &(the_scf_si.ignore_card_removal),
    "desktop.reauthAfterCardRemoval",	TYPE_BOOL,
    (char **) &(the_scf_si.reauth_after_card_removal),
    "desktop.reauthTimeout",		TYPE_INT,
    (char **) &(the_scf_si.reauth_timeout),
    "desktop.cardRemovalLogoutWait",	TYPE_INT,
    (char **) &(the_scf_si.card_removal_logout_wait),
    "desktop.firstCardEventTimeout",	TYPE_INT,
    (char **) &(the_scf_si.first_card_event_timeout),
  };

  if (si->prefs.verbose_p)
     fprintf (stderr, "---> init_scf_properties()\n");

#define NUM_SCF_PROPERTIES  7

/* Assume Solaris 9 try to open /etc/smartcard/desktop.properties file */

  OcfProperties = OcfProperties_sol9;

  fp = fopen(CF_FILE_PATH, "r");
  if (fp == NULL)
   {
    OcfProperties = OcfProperties_sol8;

  if (si->prefs.verbose_p)
     {
      fprintf (stderr,"-->Could not open %s to read ocf props\n",
					CF_FILE_PATH);
      fprintf (stderr,"-->Instead Trying to open opencard.properties file.\n");
     }

    fp = fopen("/etc/smartcard/opencard.properties", "r");

    if (fp == NULL)
     {
       fprintf (stderr, "---> Failed to open opencard.properties file!! \n");
       fprintf (stderr, "---> Failed to open desktop.properties file!! \n");
       fprintf (stderr, "---> SmartCard is not going to work!! \n");
     }
   }

  if (fp != NULL) 
    {
      /*
       * First check the value of "use_smart_card". If this is set to false,
       * garbage or not specified at all then there's no point in  retrieving
       * the rest of the smartcard settings
       */
      rv = SmartCardGetProperty (fp,OcfProperties[0].name,	
				            str_buf,sizeof(str_buf));

      if (si->prefs.verbose_p)
         fprintf (stderr, "---> rv = %d prop=%s value=%s\n\n",
			rv, OcfProperties[0].name, str_buf);
      if (rv > 0) 
	{
	  if (!strcasecmp (str_buf, "true"))
	   {
	    *((Boolean *) OcfProperties[0].value) = 1;
           }
	  else 
	    {
	      *((Boolean *) OcfProperties[0].value) = 0;
	      the_scf_si.use_smartcard = 0;
	      return;
	    }
	} 
      else 
	{
	  /*SmartCardGetProperty Failed*/
	  the_scf_si.use_smartcard = 0;
	  if (si->prefs.verbose_p)
	   {
              fprintf (stderr, "Could not read smartcard vals from /etc/");
	      fprintf (stderr, "smartcard file using default values!!!\n");
           }
          goto FORCEDEFAULT;
	}

      for (i=1; i < NUM_SCF_PROPERTIES; i++) 
	{
	  rv = SmartCardGetProperty (fp, OcfProperties[i].name, str_buf, 
					sizeof(str_buf));

          if (si->prefs.verbose_p)
            fprintf (stderr, "---> rv = %d prop=%s value=%s\n",
			rv, OcfProperties[i].name, str_buf);

	  if (rv > 0) 
	    {
	      switch (OcfProperties[i].type) 
		{
		case TYPE_INT:
		  *((int *) OcfProperties[i].value) = atoi (str_buf);
		  break;
		case TYPE_BOOL:
		  if (!strcasecmp (str_buf, "true"))
                    *((Boolean *) OcfProperties[i].value) = 1;
		  else
                    /* If value is false or any non-true value, set to 0 */
                    *((Boolean *) OcfProperties[i].value) = 0;
		  break;
		default:
          	 if (si->prefs.verbose_p)
		   fprintf(stderr,"Unknown Type for prop=%s\n",OcfProperties[i].name);
		
		}
	    }
	  else /* rv == 0 Error */
	   {
            if (si->prefs.verbose_p)
             fprintf (stderr, "---> rv = %d Error Reading prop=%s value=%s\n",
			rv, OcfProperties[i].name, str_buf);

	    /* Setting negative will be set to default by FORCEDEFAULT */
	    *((int*)OcfProperties[i].value) = -1;
	   }
	     
	}
      fclose(fp);

    } /* if fp !=NULL */

  /*
   * Print Smart Card resource values
   */

 if (si->prefs.verbose_p) 
  {
   fprintf(stderr,"\nSmartCard Values from desktop/opencard properties file\n");
   fprintf(stderr,"\t\tcardRemovalTimout:%d\n",the_scf_si.card_removal_timeout);
   fprintf(stderr,"\t\tignoreCardRemoval=%d\n",the_scf_si.ignore_card_removal);
   fprintf(stderr,"\t\treauthAfterCardRmv=%d\n",(Bool)the_scf_si.reauth_after_card_removal);
   fprintf(stderr,"\t\treauthTimeout=%d\n",the_scf_si.reauth_timeout);
   fprintf(stderr,"\t\tcardRemovalLogoutWait=%d\n\n",the_scf_si.card_removal_logout_wait);
  }


  /*
   * Sanity check for Smart Card resource values
   *
   * If values are non-zero multiply by 1000 to be ready for use 
   * in Xlib calls
   *
   */
FORCEDEFAULT:
  if (!the_scf_si.ignore_card_removal)
   {
     Ignore_SmartCard = False;
     if (si->prefs.verbose_p)
      fprintf(stderr, "***Setting ignore_smartcard --> False\n");
   }

  if (the_scf_si.card_removal_timeout < 0)
    the_scf_si.card_removal_timeout = 0;
  else
    the_scf_si.card_removal_timeout *= 1000;
  if (the_scf_si.reauth_timeout < 0)
    the_scf_si.reauth_timeout = 0;
  else
    the_scf_si.reauth_timeout *= 1000;
  if (the_scf_si.card_removal_logout_wait <= 0)
    /* logout wait should be long else it just logs you out*/
    the_scf_si.card_removal_logout_wait = 1000;/*default*/
  else 
    the_scf_si.card_removal_logout_wait *= 1000;

  si->scf_si = &the_scf_si;  /*just to make sure */
}

/*
 * This function initializes the SCF event handler so that we are informed
 * of events such as card insertion and card removal.
 */
void
init_scf_handler (saver_info *si)
{
  uint64_t time_since_validated;
  Bool b_card_present = True;
  struct sigaction sigact;
  
	SCF_Status_t status;
        SCF_Session_t session;
        SCF_Terminal_t terminal;
        SCF_Card_t card;
        SCF_ListenerHandle_t listenerHandle;

  if (si->prefs.verbose_p)
     fprintf (stderr, "---> init_scf_handler()\n");

  if (!si->scf_si->use_smartcard)
   {
    if (si->prefs.verbose_p)
     {
      fprintf (stderr, "---> returning from init_scf_handler() since use_");
      fprintf (stderr, "smartcard is false: %d\n", si->scf_si->use_smartcard);
     }
    return;
   }

  main_thread = thr_self();
  /*
   * Register the signal callback with Xt and get the signal Id for use in 
   * the signal handler.
   */
  sigusr2_id = XtAppAddSignal (si->app, sigusr2_callback, si);
  /*
   * Register a signal handler so that we are informed when a card event 
   * occurs
   */
  sigact.sa_handler = sigusr2_handler;
  sigfillset(&sigact.sa_mask);
  sigact.sa_flags = 0;
  (void)sigaction(SIGUSR2, &sigact, NULL);

  si->scf_si->scdata->session_handle = NULL;
  si->scf_si->scdata->smartcard_username = (char *)NULL;
   
  status = SCF_Session_getSession(&(si->scf_si->scdata->session_handle));
  if (status != SCF_STATUS_SUCCESS) {
	report_smartcard_init_error (si, "SCF_Session_getSession",status);
        return;
  }
/* it needs to invoke smartcard GUI to config the smartcard dev first */
        
  status = SCF_Session_getTerminal(si->scf_si->scdata->session_handle,
	NULL, 
	&(si->scf_si->scdata->terminal_handle));
  if (status != SCF_STATUS_SUCCESS) {
	report_smartcard_init_error (si, "SCF_Session_getTerminal",status);
	SCF_Session_close(si->scf_si->scdata->session_handle);
	return;
  }

  status = SCF_Terminal_addEventListener(si->scf_si->scdata->terminal_handle, 
	(SCF_EVENT_CARDINSERTED | SCF_EVENT_CARDREMOVED),
	&scfevent_handler,
	(void *)si, 
	&si->scf_si->scdata->myListener);
  if (status != SCF_STATUS_SUCCESS) {
	report_smartcard_init_error (si,"SCF_Terminal_addEventListener",status);
	SCF_Session_close(si->scf_si->scdata->session_handle);
	return;
  }


/*
	smartcard is used 
*/
  si->scf_si->use_smartcard = True;
  b_card_present = False;
  time_since_validated = 0;
  si->scf_si->scdata->state = SCD_WAIT_FOR_CARD;
 
        /*
         * Check that there is a card in the reader
	 * and validated card ?
         */
        
  status = SCF_Terminal_getCard(si->scf_si->scdata->terminal_handle,
		&(si->scf_si->scdata->card_handle));
  if (status == SCF_STATUS_NOCARD ) {
	return;
  }
  else if (status == SCF_STATUS_SUCCESS) 
  {
  si->scf_si->scdata->state = SCD_AUTH_CARD_PRESENT;
  b_card_present = True;
  get_smartcard_username (si);
   /*
   * We reach here if the card was removed or there is an unauthenticated 
   * card in the reader. The value of b_card_present tells us which one.
   */

      /*
       * There is an unauthenticated card in the reader so a card must
       * have been removed, We set the state accordingly.
       */
      if (si->scf_si->ignore_card_removal)
      {
        si->scf_si->scdata->state = SCD_CARD_REMOVAL_IGNORED;
      }
      else 
      {
	if (si->prefs.verbose_p)
	 {
	   fprintf(stderr, "init_scf_handler() locking screen and");
           fprintf(stderr, " setting SCD_CARD_PRESENT\n"); 
	 }
        set_locked_p (si, True);
      }
  } /* SCF_STATUS_SUCCESS */
  else {
	report_smartcard_init_error (si, "SCF_Session_getCard",status);
	SCF_Session_close(si->scf_si->scdata->session_handle);
	return;
  }

}

/*
 * This event takes the appropriate action following a card event, i.e.
 * a card removal or card insertion.
 */
void
handle_scf_event (saver_info *si)
{
  if (si->prefs.verbose_p)
     fprintf (stderr, "---> handle_scf_event() state=%d\n",
				si->scf_si->scdata->state);

  if (si->scf_si->scdata->state != SCD_WAIT_REMOVAL_LOGOUT_EXPIRED)
  {
    /*
     * Ignore card events when card state is SCD_WAIT_REMOVAL_LOGOUT_EXIRED
     * as the user should be forced out
     */
    if (si->scf_si->scdata->last_event == SCF_EVENT_CARDREMOVED)
    {
      /*
       * The smart card has been removed so clear the cached user name
       */
      if (si->scf_si->scdata->smartcard_username)
      {
        free (si->scf_si->scdata->smartcard_username);
        si->scf_si->scdata->smartcard_username = NULL;
      }
      switch (si->scf_si->scdata->state)
      {
        case SCD_WAIT_FOR_CARD:
	 if (!si->scf_si->ignore_card_removal)
	  {
            card_removal_timeout_id = XtAppAddTimeOut (si->app,
                                 si->scf_si->card_removal_timeout,
                                 card_removal_timeout, (XtPointer) si);
            si->scf_si->scdata->state = SCD_WAIT_FOR_CARD;

	    /*mali added 12.13 to lock if card is removed
              this introduced the bug 4771083, i.e. we should wait
	      for card_removal_timeout before locking display, so
	      moving this call in card_removal_timeout function..
	    set_locked_p(si, True);
	    ******************************/
         
            idle_timer ((XtPointer) si, 0);
	  }
          break;
        case SCD_AUTH_CARD_PRESENT:
        case SCD_ANY_CARD_PRESENT:
          card_removal_timeout_id = XtAppAddTimeOut (si->app,
                                 si->scf_si->card_removal_timeout,
                                 card_removal_timeout, (XtPointer) si);
          si->scf_si->scdata->state = SCD_WAIT_REMOVAL_TIMEOUT;
          break;

        case SCD_WRONG_USER:
        case SCD_NO_USER:
        case SCD_AUTH_IN_PROGRESS:
        case SCD_AUTH_REQUIRED:
          card_removal_timeout_id = XtAppAddTimeOut (si->app,
                                 si->scf_si->card_removal_timeout,
                                 card_removal_timeout, (XtPointer) si);
          /*does not work si->scf_si->scdata->state = SCD_WAIT_REMOVAL_TIMEOUT;*/
          si->scf_si->scdata->state = SCD_WAIT_FOR_CARD;
          /*
           * Generate synthetic event so that dialog is displayed
           */
          idle_timer ((XtPointer) si, 0);
          break;
        default:
          break;
      }
    } 
    else if (si->scf_si->scdata->last_event == SCF_EVENT_CARDINSERTED)
    {
      if (si->scf_si->scdata->state == SCD_WAIT_REMOVAL_TIMEOUT)
      {
        XtRemoveTimeOut (card_removal_timeout_id);
        si->scf_si->scdata->state = SCD_ANY_CARD_PRESENT;
      }
      else if (si->scf_si->scdata->state == SCD_CARD_REMOVAL_IGNORED)
      {
        si->scf_si->scdata->state = SCD_ANY_CARD_PRESENT;
      }
      else
      {
        if (si->scf_si->scdata->state == SCD_WAIT_REMOVAL_LOGOUT_WAIT)
          si->scf_si->scdata->wait_removal_logout_timer = 1;
        si->scf_si->scdata->state = SCD_AUTH_REQUIRED;
        /*
         * Generate synthetic event so that dialog is displayed
         */
        idle_timer ((XtPointer) si, 0);
      }
    }
  }
  mutex_lock (&m_event_processed);
  b_event_processed = True;
  cond_signal (&c_event_processed);
  mutex_unlock (&m_event_processed);
  cond_init (&c_event_processed, NULL, NULL); /*initialize again */
  mutex_init (&m_event_processed, NULL, NULL); /*initialize again */
}

/*
 * This function sets the label on the xscreensaver passwd dialog to
 * the text returned by th PAM prompt
 */
char *
scf_set_passwd_label (saver_info *si)
{
  char *label = 0;

  if (si->prefs.verbose_p)
     fprintf (stderr, "---> scf_set_passwd_label()\n");

  if (si->scf_si->use_smartcard)
  {
    if ((saved_pam_message->msg_style == PAM_PROMPT_ECHO_OFF) ||
      (saved_pam_message->msg_style == PAM_PROMPT_ECHO_ON))
    {
        label = strdup(saved_pam_message->msg);
        if (si->prefs.verbose_p)
          fprintf (stderr, "$$$$--->$$$ \t passwd_prompt_label= %s\n",label);
    }
  }
  return label;
}

static void* 
smartcard_authenticate (void *xx)
{
  saver_info *si = xx;
  int status;

  if (si->prefs.verbose_p)
     fprintf (stderr, "---> smartcard_authenticate()\n");
  block_signals();

  b_validation_succeeded = False;
  b_validation_finished = False;

  while (!b_validation_succeeded)
  {
    /*
     * We wait here for a user name to authenticate
     */
  if (si->prefs.verbose_p)
     fprintf (stderr, "---> \tinside while of smartcard_authenticate()\n");

    mutex_lock (&m_got_username);
    while (b_got_username == False) 
      cond_wait (&c_got_username, &m_got_username);
    b_got_username= False;
    mutex_unlock (&m_got_username);
 
    b_got_PAM_message = False;
 
    status = run_PAMsession (get_smartcard_username(si), si);
/*mali1117    status = pam_passwd_valid_p (get_smartcard_username(si), si);**/

    if (!b_got_PAM_message)
    {
      /*
       * The conversation function was probably interrupted so we need to 
       * reset b_got_response.
       */
      b_got_response = False;
      if (si->prefs.verbose_p)
         fprintf (stderr, "---> conv func interrupted resetting b_got_response i.e. did not get PAM_message in smartcard_authenticate()\n");
      /*
       * The conversation function did not send any message to the user
       * so we need to restart the UI thread which is waiting.
       */
      mutex_lock (&m_got_message);
      b_got_message = True;
      mutex_unlock (&m_got_message);
      cond_signal (&c_got_message);
      continue;
    }
    mutex_lock (&m_validated_response);
    b_validation_succeeded = (status == PAM_SUCCESS);
    b_validated_response = True;
    b_validation_finished = True;
    cond_signal (&c_validated_response);
    mutex_unlock (&m_validated_response);
  } /* end while */
  return 0;
}

static void set_smartcard_lock_state(saver_info *si)
{
  /*
   * This function sets the smart card state when the screen is being locked.
   * It is also called when the lock dialog is being displayed
   * as the card state may not be correct.
   */
 if (si->prefs.verbose_p)
    fprintf (stderr, "---> set_smartcard_lock_state() state:%d\n", 
					si->scf_si->scdata->state);

  switch (si->scf_si->scdata->state)
  {
    case SCD_AUTH_CARD_PRESENT:
    case SCD_ANY_CARD_PRESENT:
     {
      si->scf_si->scdata->state = SCD_AUTH_REQUIRED; 
      if (si->prefs.verbose_p)
        fprintf (stderr, "---> SCD_AUTH/ANY_CARD_PRESENT\n");
      break;
     }
    case SCD_WAIT_REMOVAL_TIMEOUT:
      XtRemoveTimeOut (card_removal_timeout_id);
      if (si->prefs.verbose_p)
        fprintf (stderr, "---> SCD_WAIT_REMOVAL_TIMEOUT\n");
      /* FALL THRU */
    case SCD_CARD_REMOVAL_IGNORED:
      si->scf_si->scdata->state = SCD_WAIT_FOR_CARD;
      /*
       * Generate synthetic event so that dialog is displayed
       */
      if (si->prefs.verbose_p)
         fprintf (stderr, "---> SCD_CARD_REMOVAL_IGNORED\n");
      idle_timer ((XtPointer) si, 0);
      break;
/*** mali
    case SCD_WAIT_FOR_CARD:
     {
      si->scf_si->scdata->state = SCD_WAIT_FOR_CARD;
      fprintf (stderr, "---> SCD_WAIT_FOR_CARD\n");
      break;
     }
    case SCD_WAIT_REMOVAL_LOGOUT_WAIT:
     {
      si->scf_si->scdata->state = SCD_WAIT_REMOVAL_LOGOUT_WAIT;
      fprintf (stderr, "---> SCD_WAIT_REMOVAL_LOGOUT_WAIT\n");
      idle_timer ((XtPointer) si, 0);
      break;
     }
    case SCD_WAIT_REMOVAL_LOGOUT_EXPIRED:
     {
      si->scf_si->scdata->state = SCD_WAIT_REMOVAL_LOGOUT_EXPIRED;
      fprintf (stderr, "---> SCD_WAIT_REMOVAL_LOGOUT_EXPIRED\n");
      idle_timer ((XtPointer) si, 0);
      break;
     }
    case SCD_AUTH_REQUIRED:
     {
      si->scf_si->scdata->state = SCD_AUTH_REQUIRED;
      fprintf (stderr, "---> SCD_AUTH_REQUIRED\n");
      idle_timer ((XtPointer) si, 0);
      break;
     }
    case SCD_WRONG_USER:
     {
      si->scf_si->scdata->state = SCD_WRONG_USER;
      fprintf (stderr, "---> SCD_WRONG_USER\n");
      idle_timer ((XtPointer) si, 0);
      break;
     }
    case SCD_AUTH_IN_PROGRESS:
     {
      si->scf_si->scdata->state = SCD_AUTH_IN_PROGRESS;
      fprintf (stderr, "---> SCD_AUTH_IN_PROGRESS\n");
      idle_timer ((XtPointer) si, 0);
      break;
     }
    case SCD_NO_USER:
     {
      si->scf_si->scdata->state = SCD_NO_USER;
      fprintf (stderr, "---> SCD_NO_USER\n");
      idle_timer ((XtPointer) si, 0);
      break;
     }
****/

    default:
      /* Unexpected card state */
      if (si->prefs.verbose_p)
        fprintf (stderr, "---> !! unexpected card state\n");
      break;
  }
}


static int run_PAMsession (char *user_name, saver_info *si)
{
/* it is SCF_PAM_SERVICE_NAME(dtsession) defined by config.h */
  const char *service = SCF_PAM_SERVICE_NAME;
  struct pam_conv pc = {conversation_function, (void *) NULL };
  struct pam_data c;
  pam_handle_t *pamh = NULL;
  int status;
  saver_preferences *p = &si->prefs;

  if (si->prefs.verbose_p)
    fprintf (stderr, "---> run_PAMsession\n");

  c.user = strdup (user_name);
  c.typed_passwd = strdup ("");
  c.verbose_p = p->verbose_p;

  pc.appdata_ptr = (void*) &c;

  /*
   * This will be set to True in the conversation function
   */
  b_PAM_prompted = False;

/* PAM needs root privileges****/

  status = pam_start (service, user_name,  &pc, &pamh);
  if (p->verbose_p)
  {
    fprintf (real_stderr, "%s: pam_start( \"%s\", \"%s\", ...) ===> "
        "%d (%s)\n", blurb(), service, user_name,
        status, pam_strerror(pamh, status)); 
  }
  if (status == PAM_SUCCESS)
  {
#ifdef PAM_MSG_VERSION
    /*
     * This allows the Solaris PAM module to use the new PAM message types
     * required to support smart cards.
     */

    status = pam_set_item (pamh, PAM_MSG_VERSION, PAM_MSG_VERSION_V2);
    if (status != PAM_SUCCESS)
    {
      if (p->verbose_p)
      {
        fprintf (stderr, "%s: pam_set_item(...) ===> "
          "%d (%s)\n", blurb(), 
          status, pam_strerror(pamh, status)); 
      }
    }
#endif /* PAM_MSG_VERSION */

fprintf(stderr,"\n\n DUDE Calling pam_authenticate in run_PAMsession\n\n");

    status = pam_authenticate (pamh, 0);
    if (si->prefs.verbose_p)
      fprintf (stderr, "---> pam_authenticate returned status= %d\n",status);
    pam_end (pamh, PAM_ABORT);
  }
 else /*pam_start FAILED */
  if (si->prefs.verbose_p)
     fprintf (stderr, "---> pam_start returned status= %d\n",status);

  return status;

}


static int
conversation_function (int num_msg, struct pam_message **msg,
        struct pam_response **response, void *appdata_ptr)
{
  struct pam_message *m;
  struct pam_response *r;
  int k;
  char *temp;
  char errbuf[PAM_MAX_MSG_SIZE];

  fprintf (stderr, "---> conversation_function -->num_msg= %d\n",num_msg);
  if (num_msg < 0)
    return (PAM_CONV_ERR);

  *response = (struct pam_response*)
    calloc (num_msg, sizeof (struct pam_response));

  if (*response == NULL)
    return (PAM_CONV_ERR);

  k = num_msg;
  m = *msg;
  r = *response;
  while (k--)
  {
    switch (m->msg_style)
    {
      case PAM_PROMPT_ECHO_ON:
      case PAM_PROMPT_ECHO_OFF:
                 
	if ((the_scf_si.use_smartcard)) {
		char *smUser;
		pam_handle_t *pamh = *((pam_handle_t **) appdata_ptr);
		if (! pamh
			|| pam_get_item(pamh, PAM_USER,
			(void **) &smUser) != PAM_SUCCESS
			|| smUser == NULL) {
			the_smi.state = (enum scstate) SCD_NO_USER;
                           return (PAM_ABORT);
                       }
                       
		if (! is_valid_unlock_user(smUser)) {
			the_smi.state = (enum scstate) SCD_WRONG_USER;
			return (PAM_ABORT);
                       }
		the_smi.smartcard_username =(char *) strdup(smUser);
		}

        b_PAM_prompted = True;
        b_got_PAM_message = True;
        memcpy (saved_pam_message, m, sizeof(*m));
        /*
         * PAM has requested some input so we tell the UI thread
         */
        if (b_first_prompt)
        {
          b_first_prompt = False;
        }
        else
        {
          /*
           * Tell the UI thread that the first prompt was validated and that
           * there is more to come
           */
          mutex_lock (&m_validated_response);
          b_validated_response = True;
          b_validation_finished = False;
          cond_signal (&c_validated_response);
          mutex_unlock (&m_validated_response);
        }
        mutex_lock (&m_got_message);
        b_got_message = True;
        mutex_unlock (&m_got_message);
        cond_signal (&c_got_message);
        /*
         * We must now wait for the user input to be supplied
         */
        mutex_lock (&m_got_response);
        while (b_got_response == False)
          cond_wait (&c_got_response, &m_got_response);
        b_got_response = False;
        mutex_unlock (&m_got_response);
        /*
         * saved_user_input being NULL means that the PAM conversation
         * should be interrupted because converstion function was called
         * in another thread with message style PAM_CONV_INTERRUPT.
         * This ususlly means someone removed the smart card.
         */
        if (saved_user_input == NULL)
        {
          return (PAM_ABORT);
        }

        r->resp = (char *) malloc (strlen (saved_user_input) + 1);
        if (r->resp == NULL) 
        {
          /*
           * We need to free all previous responses
           * __pam_free_resp (num_msg, *response);
           */
          struct pam_response	*rr;
          int i;

          rr = *response;
          for (i = 0; i < num_msg; i++, rr++) 
          {
            if (rr->resp) 
            {
              free(r->resp);
              rr->resp = NULL;
            }
          }
          free(response);
          *response = NULL;
          return PAM_CONV_ERR;
        }
        (void) strcpy (r->resp, saved_user_input);
        r->resp_retcode = 0;
        m++;
        r++;
        break;

      case PAM_ERROR_MSG:
        m++;
        r++;
        break;

      case PAM_TEXT_INFO:
        m++;
        r++;
        break;

#ifdef PAM_MSG_VERSION
      case PAM_CONV_INTERRUPT:
        memcpy (saved_pam_message, m, sizeof(*m));
        if (thr_self() != pam_thread)
        {
          /*
           * We need to interrupt the other thread running the PAM
           * conversation function as it will be waiting on a condition 
           * variable. 
           */
          saved_user_input = NULL;
          mutex_lock (&m_got_response);
          b_got_response = True;
          b_validated_response = False;
          cond_signal (&c_got_response);
          mutex_unlock (&m_got_response);
          return (PAM_SUCCESS);
        }
        m++;
        r++;
        break;
#endif /* PAM_MSG_VERSION */
        
      default:
        fprintf (real_stderr, "%s, Unknown PAM Message type %d\n",
            blurb(), m->msg_style);
        break; 
    }
  }
  fprintf (stderr, "---> conversation_function returning PAM_SUCCESS\n");
  return(PAM_SUCCESS);
}

static void  
block_signals (void)
{
  sigset_t new;
  /*
   * We block any signal which the main thread deals with to make sure it is
   * caught by the main thread
   *
   * The list of signals was got from windows.c
   */
  sigemptyset (&new);
  sigaddset (&new, SIGCHLD);
  sigaddset (&new, SIGHUP);
  sigaddset (&new, SIGINT);
  sigaddset (&new, SIGQUIT);
  sigaddset (&new, SIGILL);
  sigaddset (&new, SIGTRAP);
  sigaddset (&new, SIGIOT);
  sigaddset (&new, SIGABRT);
#ifdef SIGEMT
  sigaddset (&new, SIGEMT);
#endif
  sigaddset (&new, SIGFPE);
  sigaddset (&new, SIGBUS);
  sigaddset (&new, SIGSEGV);
#ifdef SIGSYS
  sigaddset (&new, SIGSYS);
#endif
  sigaddset (&new, SIGTERM);
#ifdef SIGXCPU
  sigaddset (&new, SIGXCPU);
#endif
#ifdef SIGXFSZ
  sigaddset (&new, SIGXFSZ);
#endif
#ifdef SIGDANGER
  sigaddset (&new, SIGDANGER);
#endif
  thr_sigsetmask (SIG_BLOCK, &new, NULL);
}

static char * 
get_username (void)
{
  static char *puser = NULL;

  if (!puser)
  {
    struct passwd * pwd;
    pwd = getpwuid (getuid ());
    if (pwd) 
    {
      puser = pwd->pw_name;
    }
  }
  return (puser);
}

static Bool
is_valid_unlock_user (char * sm_user)
{
  char *loggedin_user = get_username ();
  Bool b_ret = True;

  if (sm_user == NULL)
  {
    b_ret = False;
  }
  else if (loggedin_user != NULL)
  {
    if (strcmp (sm_user, loggedin_user))
    {
      if (strcmp (sm_user, "root"))
      {
        b_ret = False;
      }
    }
  }
  else 
  {
    b_ret = False;
  }
  return b_ret;    
}

static void
dont_block_for_pam (XtPointer ptr, XtIntervalId *invId)
{
  saver_info *si = (saver_info *) ptr;
  
    b_got_message = True; 
    b_got_PAM_message = True; 
    b_validated_response = True;
    b_validation_finished = True;
/*    cond_signal (&c_validated_response);*/
}

static void 
wait_for_got_message (saver_info *si)
{
  if (si->prefs.verbose_p)
     fprintf (stderr, "---> wait_for_got_message()\n");
  XtAppAddTimeOut (si->app,
                   100,
                   dont_block_for_pam, (XtPointer) si);
    mutex_lock (&m_got_message);
    while (b_got_message == False)
     {
	cond_wait (&c_got_message, &m_got_message);
	if (si->prefs.verbose_p)
           fprintf (stderr, "---> \t while of wait_for_got_message()\n");
     }
    b_got_message = False;
    mutex_unlock (&m_got_message);
}

static char * 
get_smartcard_username (saver_info *si)
{
/*
**	 Since OCF_UserInfoCardService() is not availabe
**	 it can be obtained from pam_smarcard module
*/
    return ((char *)the_smi.smartcard_username) ; 
}


static void 
scfevent_handler(SCF_Event_t event, SCF_Terminal_t eventTerminal,void *data)
{
  SCF_Status_t status;
  saver_info *si = (saver_info *) data;

  if (si->prefs.verbose_p)
     fprintf (stderr, "---> scfevent_handler() state=%d\n",
				si->scf_si->scdata->state);

  /*
   * We wait for the previous event to be dealt with before proceeding
   */
  mutex_lock (&m_event_processed);
  while (!b_event_processed)
    cond_wait (&c_event_processed, &m_event_processed);
  b_event_processed = False;
  mutex_unlock (&m_event_processed);

  if (event == SCF_EVENT_CARDINSERTED)
  {
    si->scf_si->scdata->state = SCD_AUTH_REQUIRED;
    if (si->prefs.verbose_p)
       fprintf (stderr, "---> EVENT==> CARD INSERTED()\n");
  }
  else if (event == SCF_EVENT_CARDREMOVED)
  {

    si->scf_si->scdata->state = SCD_WAIT_FOR_CARD;
    b_event_processed = True; /* reinitialize to True */

    if (si->prefs.verbose_p)
     {
       fprintf (stderr, "---> EVENT==> CARD REMOVED()\n");
       fprintf (stderr, "---> setting state=%d\n",si->scf_si->scdata->state);
     }
  }
  else
  {
    if (si->prefs.verbose_p)
       fprintf (stderr, "---> EVENT==> NO CARD INSERTED OR REMOVED()\n");
    return;
  }
  /*
   * Store the SCF event for card insert or remove
   */
  si->scf_si->scdata->last_event = event;
  thr_kill (main_thread, SIGUSR2);
#if 1
  /*
   * We send a ClientMessage which corresponds to this event
   * so that the UI thread can deal with it.
   */
  {
    XEvent xevent;

    /*
     * Store the SCF event
     */
    si->scf_si->scdata->last_event = event;

    xevent.xany.type = ClientMessage;
    xevent.xclient.display = si->dpy;
    xevent.xclient.window = 0;
    xevent.xclient.message_type = XA_SCREENSAVER;
    xevent.xclient.format = 32;
    memset (&xevent.xclient.data, 0, sizeof (xevent.xclient.data));
    xevent.xclient.data.l[0] = (long) XA_SMARTCARD;
    if (! XSendEvent (si->dpy, si->screens[0].screensaver_window, 
        False, 0L, &xevent))
    {
        fprintf (real_stderr, "XSendEvent failed\n");
    }
    XFlush (si->dpy);
  }
#endif

  if (si->prefs.verbose_p)
   fprintf (stderr, "<--- scfevent_handler() state=%d\n",
	si->scf_si->scdata->state);

}

static void
card_removal_timeout (XtPointer ptr, XtIntervalId *invId)
{
  saver_info *si = (saver_info *) ptr;
  /*
   * This callback decides what to do when cardRemovalTimeout has expired
   */
  if (si->prefs.verbose_p)
     fprintf (stderr, "---> card_removal_timeout state:%d\n",
				    si->scf_si->scdata->state);

  if (!si->scf_si->ignore_card_removal)
    set_locked_p(si, True);

  if (!si->scf_si->ignore_card_removal && 
       si->scf_si->scdata->state != SCD_AUTH_IN_PROGRESS &&
       !si->scf_si->reauth_after_card_removal)
    {
        start_wait_removal_timer (si);
    }

#if OLDCODE
  if (si->scf_si->scdata->state == SCD_WAIT_REMOVAL_TIMEOUT)
  {
    /*
     * We make sure that we are in the expected state
     */
    if (si->scf_si->ignore_card_removal)
    {
      if (si->locked_p)
      {
        si->scf_si->scdata->state = SCD_WAIT_FOR_CARD;
        /*
         * Generate synthetic event so that dialog is displayed
         */
        idle_timer ((XtPointer) si, invId);
      }
      else
      {
        si->scf_si->scdata->state = SCD_CARD_REMOVAL_IGNORED;
      }
    }
    else if (si->scf_si->reauth_after_card_removal)
    {
      si->scf_si->scdata->state = SCD_WAIT_FOR_CARD;
      if (si->prefs.verbose_p)
         fprintf (stderr, "SCD_WAIT_FOR_CARD calling set_locked_p \n");
      set_locked_p (si, True);
      /*
       * Generate synthetic event so that dialog is displayed
       */
      idle_timer ((XtPointer) si, invId);
    }
    else
    {
        start_wait_removal_timer (si);
    }
  }
#endif /*oldcode*/
}

static void 
start_wait_removal_timer (saver_info *si)
{
  if (si->prefs.verbose_p)
    fprintf (stderr, "---> start_wait_removal_timeout value:%d \n",
                              si->scf_si->card_removal_logout_wait);

  si->scf_si->scdata->state = SCD_WAIT_REMOVAL_LOGOUT_WAIT;

/*mali somehow timer is too short and calls force_exit()**/

  card_removal_logout_wait_id = XtAppAddTimeOut (si->app,
                                 si->scf_si->card_removal_logout_wait,
				 /*10000, mali*/
                                 card_removal_logout_wait,
                                 (XtPointer) si);
  set_locked_p (si, True);
  /*
   * Generate synthetic event so that dialog is displayed
   */
  idle_timer ((XtPointer) si, 0);
}

static void
card_removal_logout_wait (XtPointer ptr, XtIntervalId *invId)
{
  saver_info *si = (saver_info *) ptr;

  if (si->prefs.verbose_p)
    fprintf (stderr, "---> card_removal_logout_wait\n");

/*mali  si->scf_si->scdata->state = SCD_WAIT_REMOVAL_LOGOUT_EXPIRED;**
  we dont really want to log user out do we and if we do..how to do it?
  pkill gnome-session or /usr/dt/bin/dtconfig -kill or /etc/init.d/dtlogin 
  stop so for now setting state to AUTH_REQUIRED 
  si->scf_si->scdata->state = SCD_AUTH_REQUIRED; */

  si->scf_si->scdata->state = SCD_WAIT_REMOVAL_LOGOUT_EXPIRED;
  force_exit(si);
}

static void
force_exit (saver_info *si)
{
  /*
   * This function is called after cardRemovalLogoutWait has expired and
   * should cause the user to be logged out
   */
  if (si->prefs.verbose_p)
     fprintf (stderr, "---> force_exit\n");
/*  saver_exit (si, -1, 0); */
/*mali */ destroy_passwd_window (si);
  exec_command (si->prefs.shell, "pkill gnome-session", 0);
}
  
static void
report_smartcard_init_error (saver_info *si, char *name, uint32_t code)
{
  char *message = "%s: Smart Card initialization failed\n"
              "The function %s returned error code %d - disabling locking.\n";
  fprintf (real_stderr ? real_stderr : stderr, message, blurb(), name, code);

  if (si->prefs.verbose_p)
    fprintf (stderr, "---> report_smartcard_init_error\n");

  if (!si->locking_disabled_p)
  {
    si->locking_disabled_p = True;
    si->nolock_reason = "SCF function failure";
  }
}

static void
report_smartcard_error (char *name, uint32_t code)
{
  char *message = "%s: Call to SCF failed\n"
                 "    The function %s returned error code %d\n";
  fprintf (real_stderr ? real_stderr : stderr, message, blurb(), name, code);
}

/*
 * The Unix signal handler that is registering with sigaction.
 * It uses XtNoticeSignal to tell the Intrinsics to invoke the signal
 * handler assocaited with the signal ID.
 */
static void sigusr2_handler (void)
{
  XtNoticeSignal (sigusr2_id);
}

static void sigusr2_callback (XtPointer client_data, XtSignalId *id)
{
  /*
   * This function generates an X event by calling XtAppAddTimeOut with
   * a timeout value of 0. It is called so that the event loop is called
   * after the card event has been processed.
   */
  saver_info *si = (saver_info *) client_data;

  if (si->prefs.verbose_p)
    fprintf (stderr, "---> siguser2_callback\n");

  XtAppAddTimeOut (si->app,
                   1,
                   synthetic_event_callback,
                   (XtPointer) si);
}    

static void synthetic_event_callback (XtPointer client_data, XtIntervalId * id)
{
  set_smartcard_event_occurred (True);
  idle_timer ( (saver_info *) client_data, 0);
}

Bool has_smartcard_event_occurred (void)
{
  Bool ret_val = b_card_event_occurred;

  if (b_card_event_occurred)
  {
    b_card_event_occurred = False;
  }
  return ret_val;
}

void set_smartcard_event_occurred (Bool val)
{
  b_card_event_occurred = val;
}
#endif /* NO_LOCKING -- whole file */
