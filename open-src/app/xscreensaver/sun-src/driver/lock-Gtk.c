/* lock-Gtk.c -- a GTK+ password dialog for xscreensaver
 * xscreensaver, Copyright (c) 1993-1998 Jamie Zawinski <jwz@jwz.org>
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

/* GTK+ locking code written by Jacob Berkman  <jacob@ximian.com> for
 *  Sun Microsystems.
 *
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_GTK2 /* whole file */

#include <xscreensaver-intl.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

/* AT-enabled */
#include <stdio.h>
#include <ctype.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/WinUtil.h>

#include <gconf/gconf-client.h>
#include <libbonobo.h>
#include <login-helper/Accessibility_LoginHelper.h>
#include <atk/atkobject.h>

#include "remote.h"
#include "trusted-utils.h"

/* AT-enabled */
void write_null(int n);


static Atom XA_UNLOCK_RATIO;
Atom XA_VROOT;
Atom XA_SCREENSAVER, XA_SCREENSAVER_RESPONSE, XA_SCREENSAVER_VERSION;
Atom XA_SCREENSAVER_ID, XA_SCREENSAVER_STATUS, XA_SELECT, XA_DEMO;
Atom XA_ACTIVATE, XA_BLANK, XA_LOCK, XA_RESTART, XA_EXIT;

typedef struct {
  AtkObject *atk_dialog;
  GtkWidget *dialog;
  GtkWidget *passwd_entry;
  GtkWidget *progress;
  GtkWidget *button;
  GtkWidget *msg_label;
  GtkWidget *user_prompt_label;
} PasswdDialog;

static void ok_clicked_cb (GtkWidget *button, PasswdDialog *pwd);
/*
** 6182506: scr dialog is obscurred byg MAG window
*/
static PasswdDialog * atk_make_dialog (gboolean center_pos);
/*Global info */
#define MAXRAISEDWINS	2
#define KEY "/desktop/gnome/interface/accessibility"
gboolean	 at_enabled  = FALSE;
/*
** 6182506: scr dialog is obscurred byg MAG window
*/
gboolean	 center_postion = FALSE; 	
char *progname = 0;
Bonobo_ServerInfoList *server_list = NULL;
CORBA_Environment ev;
Accessibility_LoginHelper helper;
Accessibility_LoginHelper *helper_list = NULL;
CORBA_boolean safe;
 
#define FD_TO_PARENT  9

static ssize_t
write_string (const char *s)
{
  ssize_t len;

/*****mali99 remove this ****
  fprintf (stderr, "-->Child write_string() string to send parent is:%s\n",s);
  fflush (stderr);
**/

  g_return_val_if_fail (s != NULL, -1);

 do_write:
  len = write (FD_TO_PARENT, s, strlen (s));
  if (len < 0 && errno == EINTR)
    goto do_write;

  return len;
}

void
write_null(int n)
{
Window w = 0;
char *s;
int i;

if (n == 1 || n == 2)
{
	for (i =0; i < n; i++)
	{	
		s = g_strdup_printf ("%lu\n", w);
		write_string(s);
		g_free(s);
	}
}
else 
{
  w = 1;
  /* tell xscreensaver, MAG and GOK not running,
     Ass. Tech support is still selected(SPEECH), reset timer for each 
     passwd char.
  */
  s = g_strdup_printf ("%lu\n", w);
                write_string(s);
                g_free(s);
  w = 0;
  s = g_strdup_printf ("%lu\n", w);
                write_string(s);
                g_free(s);
	
}

}

static GtkWidget *
load_unlock_logo_image(void)
{
  const char *logofile;
  struct stat statbuf;

  if ( tsol_is_multi_label_session() )
      logofile = DEFAULT_ICONDIR "/trusted-logo.png";
  else
      logofile = DEFAULT_ICONDIR "/unlock-logo.png";

  if (stat(logofile, &statbuf) != 0) {
      logofile = DEFAULT_ICONDIR "/logo-180.gif"; /* fallback */
  }
  
  return gtk_image_new_from_file (logofile);
}

/*
** 6182506: scr dialog is obscurred byg MAG window
*/

static PasswdDialog *
atk_make_dialog (gboolean center_pos)
{
  GtkWidget *dialog;
  GtkWidget *frame1,*frame2;
  GtkWidget *vbox;
  GtkWidget *hbox,*hbox1,*hbox2;
  GtkWidget *bbox;
  GtkWidget *vbox2;
  AtkObject *atk_frame1,*atk_frame2;
  AtkObject *atk_vbox;
  AtkObject *atk_hbox,*atk_hbox1,*atk_hbox2;
  AtkObject *atk_vbox2;
  AtkObject *atk_button,*atk_dialog;
  GtkWidget *entry1,*entry2;
  AtkObject *atk_entry1,*atk_entry2;
  GtkWidget *label1,*label2,*label3,*label4,*label5;
  AtkObject *atk_label1,*atk_label2,*atk_label3,*atk_label4,*atk_label5;
  GtkWidget *button;
  GtkWidget *image;
  AtkObject *atk_image;
  GtkWidget *progress;
  GdkPixbuf *pb;
  char *version;
  char *user;
  char *host;
  char *s;
  gchar *format_string_locale, *format_string_utf8;
  PasswdDialog *pwd;


  /* taken from lock.c */
  char buf[256];
  gchar *utf8_format;
  time_t now = time (NULL);
  struct tm* tm;

  server_xscreensaver_version (GDK_DISPLAY (), &version, &user, &host);

  if (!version)
    {
      fprintf (stderr, "%s: no xscreensaver running on display %s, exiting.\n", progname, gdk_get_display ());
      exit (1);
    }
  
/* PUSH */
  gtk_widget_push_colormap (gdk_rgb_get_cmap ());


  pwd = g_new0 (PasswdDialog, 1);

  dialog = gtk_window_new (GTK_WINDOW_POPUP);
/* 
** bugid: 5077989(P2)Bug 147580: password input dialogue obscures GOK
   gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
   bugid: 5002244:  scr unlock dialog incompatible with MAG technique
** 6182506: scr dialog is obscurred byg MAG window
*/

  if (center_pos)
   gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ALWAYS);
  else
   gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
/* AT-eabled dialog role = frame*/
  atk_dialog = gtk_widget_get_accessible(dialog);
  atk_object_set_description(atk_dialog, _("AT-enabled lock dialog"));
  pwd->atk_dialog = atk_dialog;

 
  /* frame */
  frame1 = g_object_new (GTK_TYPE_FRAME,
			"shadow", GTK_SHADOW_OUT,
			NULL);
  gtk_container_add (GTK_CONTAINER (dialog), frame1);
  pwd->dialog = dialog;


  /*AT role = panel */

  atk_frame1 = gtk_widget_get_accessible(frame1);


  /* vbox */
  vbox = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
  gtk_container_add (GTK_CONTAINER (frame1), vbox);

  /* AT role= filler(default) */
  atk_vbox = gtk_widget_get_accessible(vbox);

  
  /* hbox */
  hbox = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), hbox,
		      TRUE, TRUE, 0);


  /* image frame */
  frame2 = g_object_new (GTK_TYPE_FRAME,
			"shadow", GTK_SHADOW_ETCHED_IN,
			NULL);
  gtk_box_pack_start (GTK_BOX (hbox), frame2,
		      TRUE, TRUE, 0);
 
  /* AT role= filler(default) */
  atk_hbox = gtk_widget_get_accessible(hbox);


  /* image */
  image = load_unlock_logo_image();

  gtk_container_add (GTK_CONTAINER (frame2), image);

  /* AT role=panel */

  atk_frame2 = gtk_widget_get_accessible(frame2);


  /* AT role = icon */

  atk_image = gtk_widget_get_accessible(image);


   /* progress thingie */
  progress = g_object_new (GTK_TYPE_PROGRESS_BAR,
                           "orientation", GTK_PROGRESS_BOTTOM_TO_TOP,
                           "fraction", 1.0,
                           NULL);
  gtk_box_pack_start (GTK_BOX (hbox), progress,
                      FALSE, FALSE, 0);
  pwd->progress = progress;
  atk_object_set_description (gtk_widget_get_accessible (progress),
            _("Percent of time you have to enter the password.  "));


  /* text fields */

  vbox2 = gtk_vbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2,
		      TRUE, TRUE, 0);

  s = g_strdup_printf ("<span size=\"xx-large\"><b>%s </b></span>", _("Screensaver"));
  /* XScreenSaver foo label */
  label1 = g_object_new (GTK_TYPE_LABEL,
			"use-markup", TRUE,
			"label", s,
			NULL);
  gtk_box_pack_start (GTK_BOX (vbox2), label1,
		      FALSE, FALSE, 0);

  /*AT role =filler*/
  
  atk_vbox2 = gtk_widget_get_accessible(vbox2);

  /*AT role =label prog name */
  atk_label1 = gtk_widget_get_accessible(label1);


  g_free (s);

  /* This display is locked. */
  label2 = g_object_new (GTK_TYPE_LABEL,
			"use-markup", TRUE,
			"label", _("<b>This display is locked.</b>"),
			NULL);
  gtk_box_pack_start (GTK_BOX (vbox2), label2,
		      FALSE, FALSE, 0);
  /* AT role = label , msg */

  atk_label2 = gtk_widget_get_accessible(label2);



  /* table with password things */
    hbox1 = gtk_widget_new (GTK_TYPE_HBOX,
                         "border_width", 3,
                         "visible",TRUE,
                        "homogeneous",FALSE,
                        "spacing",2,
                        NULL);


  /* User: */
  label3 = g_object_new (GTK_TYPE_LABEL,
                        "label", _("        User:"),
                        "use_underline", TRUE,
			"justify",GTK_JUSTIFY_FILL,
			"xalign", 1.0,
			NULL);
  
  /* name */
  entry1 = g_object_new (GTK_TYPE_LABEL,
                        "use-markup", TRUE,
                        "label", user,
                        "justify",GTK_JUSTIFY_CENTER,
                        NULL);

  gtk_box_pack_start ( GTK_BOX(hbox1), label3,  FALSE, FALSE, 0);
  gtk_box_pack_end  ( GTK_BOX(hbox1), entry1, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox1,
		      FALSE, FALSE, 0);



   /* Password or Pin */


  hbox2 = gtk_widget_new (GTK_TYPE_HBOX,
                         "border_width", 5,
                         "visible",TRUE,
                        "homogeneous",FALSE,
                        "spacing",1,
                        NULL);

  /* Password: */
    label4 = g_object_new (GTK_TYPE_LABEL,
	"label", _("         "), /*blank space for prompt*/
                           "use_underline", TRUE,
                        "use_markup",FALSE,
                        "justify",GTK_JUSTIFY_RIGHT,
                        "wrap", FALSE,
                        "selectable",TRUE,
                        "xalign", 1.0,
                        "yalign",0,
                        "xpad",0,
                        "ypad",0,
                        "visible", FALSE,
			  NULL);


 entry2 = g_object_new (GTK_TYPE_ENTRY,
			"activates-default", TRUE,
			"visible", TRUE,
                        "editable", TRUE,
			"visibility", FALSE,
                      "can_focus", TRUE,
		NULL);


  gtk_box_pack_start ( GTK_BOX(hbox2), label4,  FALSE, FALSE, 0);
  gtk_box_pack_end  ( GTK_BOX(hbox2), entry2, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, FALSE, FALSE, 0);
  pwd->user_prompt_label = label4;
 
  pwd->passwd_entry = entry2;
  
 
  /* AT role=panel */
  atk_hbox1 = gtk_widget_get_accessible(hbox1);

  /* AT role=label */
  atk_label3= gtk_widget_get_accessible(label3);

  /* AT role=label */
  atk_label4= gtk_widget_get_accessible(label4);

  /* AT role = text */
  atk_entry1 = gtk_widget_get_accessible(entry1);

  /* hbox1 for password/pin and text entry */
 
  atk_hbox2 = gtk_widget_get_accessible(hbox2);


/* AT role = password-text */
  /* gtk_widget_grab_focus (entry2);
   */
  atk_entry2 = gtk_widget_get_accessible(entry2);

  /* bugid 5079870 */
   atk_object_set_role(atk_entry2, ATK_ROLE_PASSWORD_TEXT);
  
  tm = localtime (&now);
  memset (buf, 0, sizeof (buf));
  format_string_utf8 = _("%d-%b-%y (%a); %I:%M %p");
  format_string_locale = g_locale_from_utf8 (format_string_utf8, -1,
					     NULL, NULL, NULL);
  strftime (buf, sizeof (buf) - 1, format_string_locale, tm);
  g_free (format_string_locale);

  /*bug 4783832  s = g_strdup_printf ("<small>%s</small>", buf);**/
  utf8_format = g_locale_to_utf8 (buf, -1, NULL, NULL, NULL);

  s = g_strdup_printf ("<small>%s</small>", utf8_format);

if (utf8_format)
  g_free (utf8_format);

  /* date string */
  label5 = g_object_new (GTK_TYPE_LABEL,
			"use-markup", TRUE,
			"label",  s,
			NULL);
  gtk_box_pack_start (GTK_BOX (vbox2), label5,
		      FALSE, FALSE, 0);
  /* AT role=label */
  atk_label5= gtk_widget_get_accessible(label5);


if (s)
  g_free (s);

  /* button box */
  bbox = g_object_new (GTK_TYPE_HBUTTON_BOX,
		       "layout-style", GTK_BUTTONBOX_END,
		       "spacing", 10,
		       NULL);

  /* Ok button */
  button = gtk_button_new_from_stock (GTK_STOCK_OK);
  pwd->button = button;

  gtk_box_pack_end (GTK_BOX (bbox), button,
		    FALSE, TRUE, 0);


  atk_button = gtk_widget_get_accessible(button);

  free (user);
  free (version);
  free (host);

 /* POP */
 gtk_widget_pop_colormap ();

  return pwd;
}


static PasswdDialog *
make_dialog (void)
{
  GtkWidget *dialog;
  GtkWidget *frame;
  GtkWidget *vbox;
  GtkWidget *hbox,*hbox1,*hbox2;
  GtkWidget *bbox;
  GtkWidget *vbox2;
  GtkWidget *entry;
  GtkWidget *label;
  GtkWidget *scrolled_text;
  GtkWidget *button;
  GtkWidget *image;
  GtkWidget *progress;
  GdkPixbuf *pb;
  PasswdDialog *pwd;
  char *version;
  char *user;
  char *host;
  char *s;
  gchar *format_string_locale, *format_string_utf8;

  /* taken from lock.c */
  char buf[256];
  gchar *utf8_format;
  time_t now = time (NULL);
  struct tm* tm;

  server_xscreensaver_version (GDK_DISPLAY (), &version, &user, &host);

  if (!version)
    {
      fprintf (stderr, "%s: no xscreensaver running on display %s, exiting.\n", progname, gdk_get_display ());
      exit (1);
    }
  
/* PUSH */
 gtk_widget_push_colormap (gdk_rgb_get_cmap ());

 /**colormap = gtk_widget_get_colormap (parent);**/

  pwd = g_new0 (PasswdDialog, 1);

  dialog = gtk_window_new (GTK_WINDOW_POPUP);
  pwd->dialog = dialog;
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ALWAYS);
  
  gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE); /*mali99 irritating*/

  
  /* frame */
  frame = g_object_new (GTK_TYPE_FRAME,
			"shadow", GTK_SHADOW_OUT,
			NULL);
  gtk_container_add (GTK_CONTAINER (dialog), frame);

  /* vbox */
  vbox = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  
  /* hbox */
  hbox = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), hbox,
		      TRUE, TRUE, 0);

  /* image frame */
  frame = g_object_new (GTK_TYPE_FRAME,
			"shadow", GTK_SHADOW_ETCHED_IN,
			NULL);
  gtk_box_pack_start (GTK_BOX (hbox), frame,
		      TRUE, TRUE, 0);

  /* image */
  image = load_unlock_logo_image();

  gtk_container_add (GTK_CONTAINER (frame), image);

  /* progress thing */
  progress = g_object_new (GTK_TYPE_PROGRESS_BAR,
                           "orientation", GTK_PROGRESS_BOTTOM_TO_TOP,
                           "fraction", 1.0,
                           NULL);
  pwd->progress = progress;
  gtk_box_pack_start (GTK_BOX (hbox), progress,
                      FALSE, FALSE, 0);

  /* text fields */
  vbox2 = gtk_vbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2,
		      TRUE, TRUE, 0);

  s = g_strdup_printf ("<span size=\"xx-large\"><b>%s </b></span>", _("Screensaver"));
  /* XScreenSaver foo label */
  label = g_object_new (GTK_TYPE_LABEL,
			"use-markup", TRUE,
			"label", s,
			NULL);
  gtk_box_pack_start (GTK_BOX (vbox2), label,
		      FALSE, FALSE, 0);
  g_free (s);

  /* This display is locked. */
  label = g_object_new (GTK_TYPE_LABEL,
			"use-markup", TRUE,
			"label", _("<b>This display is locked.</b>"),
			NULL);
  pwd->msg_label = label;
  gtk_box_pack_start (GTK_BOX (vbox2), label,
		      FALSE, FALSE, 0);

    /* hbox1 table with password things */
    hbox1 = gtk_widget_new (GTK_TYPE_HBOX,
                         "border_width", 3,
                         "visible",TRUE,
                        "homogeneous",FALSE,
                        "spacing",1,
                        NULL);

  /* User: */
  label = g_object_new (GTK_TYPE_LABEL,
                  	"label", _("        User:"),
                        "use_underline", TRUE,
                        "justify",GTK_JUSTIFY_FILL,
                        "xalign", 1.0,
                        NULL);

  /* name */
  entry = g_object_new (GTK_TYPE_LABEL,
                        "use-markup", TRUE,
                        "label", user,
                        "justify",GTK_JUSTIFY_CENTER,
                        NULL);

  gtk_box_pack_start ( GTK_BOX(hbox1), label,  FALSE, FALSE, 0);
  gtk_box_pack_end  ( GTK_BOX(hbox1), entry, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, FALSE, 0);


    /* Password or Pin */


  hbox2 = gtk_widget_new (GTK_TYPE_HBOX,
                         "border_width", 5,
                         "visible",TRUE,
                        "homogeneous",FALSE,
                        "spacing",1,
                        NULL);

    label = g_object_new (GTK_TYPE_LABEL,
                        "label", "        ", /*blank space for prompt*/
			"use-markup", TRUE,
                        "use_underline", TRUE,
                        "justify",GTK_JUSTIFY_RIGHT,
                        "wrap", FALSE,
                        "selectable",FALSE,
                        "xalign", 1.0,
                        "yalign",0.5,
                        "xpad",0,
                        "ypad",0,
                        "visible", FALSE,
                          NULL);

  pwd->user_prompt_label = label;  /**mali99*/


   entry = g_object_new (GTK_TYPE_ENTRY,
                        "activates-default", TRUE,
                        "visible", TRUE,
                        "editable", TRUE,
                        "visibility", FALSE,
                      "can_focus", TRUE,
                NULL);


  gtk_box_pack_start ( GTK_BOX(hbox2), label,  FALSE, FALSE, 0);
  gtk_box_pack_end  ( GTK_BOX(hbox2), entry, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, FALSE, FALSE, 0);

  /* passwd */
  pwd->passwd_entry = entry;
  gtk_widget_grab_focus (entry);
  
  tm = localtime (&now);
  memset (buf, 0, sizeof (buf));
  format_string_utf8 = _("%d-%b-%y (%a); %I:%M %p");
  format_string_locale = g_locale_from_utf8 (format_string_utf8, -1,
					     NULL, NULL, NULL);
  strftime (buf, sizeof (buf) - 1, format_string_locale, tm);
  g_free (format_string_locale);

  /*bug 4783832  s = g_strdup_printf ("<small>%s</small>", buf);**/
  utf8_format = g_locale_to_utf8 (buf, -1, NULL, NULL, NULL);

  s = g_strdup_printf ("<small>%s</small>", utf8_format);

if (utf8_format)
  g_free (utf8_format);

  /* date string */
  label = g_object_new (GTK_TYPE_LABEL,
			"use-markup", TRUE,
			"label",  s,
			NULL);
  gtk_box_pack_start (GTK_BOX (vbox2), label,
		      FALSE, FALSE, 0);
if (s)
  g_free (s);

  /* button box */
  bbox = g_object_new (GTK_TYPE_HBUTTON_BOX,
		       "layout-style", GTK_BUTTONBOX_END,
		       "spacing", 10,
		       NULL);
/*
  gtk_box_pack_start (GTK_BOX (vbox), bbox,
		      FALSE, FALSE, 0);
*/

  /* Ok button */
  button = gtk_button_new_from_stock (GTK_STOCK_OK);
  pwd->button = button;
  gtk_box_pack_end (GTK_BOX (bbox), button,
		    FALSE, TRUE, 0);

  free (version);
  free (user);
  free (host);

 /* POP */
 gtk_widget_pop_colormap ();

  return pwd;
}

static void
desensitize_entry (PasswdDialog *pwd)
{
	g_object_set (pwd->passwd_entry,
		"editable", FALSE,
		NULL);
}



static void
ok_clicked_cb (GtkWidget *button, PasswdDialog *pwd)
{
  char *s;
  int i;
 


  g_object_set (pwd->msg_label, "label", _("<b>Checking...</b>"), NULL);
  s = g_strdup_printf ("%s\n", gtk_entry_get_text (GTK_ENTRY (pwd->passwd_entry)));
     
  write_string (s);

/* Reset password field to blank, else passwd field shows old passwd *'s, visible when
   passwd is expired, and pam is walking the user to change old passwd.
 */
  gtk_editable_delete_text (pwd->passwd_entry, 0, strlen(s));
  g_free (s);

/* #6178584 P1 "Xscreensaver needs to use ROLE_PASSWORD_TEXT 
   1st part: fixing "It is too early to invoke LoginHelper_setSafe(FALSE)"
   2nd part: located at GOK
   only GOK and MAG use loginhelper()
*/

  if (server_list)
  {
   for (i = 0; i < server_list->_length; i++)
    {
        helper = helper_list[i];
        /* really no need to check the return value this time */
        Accessibility_LoginHelper_setSafe (helper, FALSE, &ev);
        if (BONOBO_EX (&ev))
        {
            g_warning ("setSafe(FALSE) failed: %s",
                       bonobo_exception_get_text (&ev));
            CORBA_exception_free (&ev);
        }
        CORBA_Object_release (helper, &ev);
    }
    CORBA_free (server_list);
    bonobo_debug_shutdown ();
  }

}

static void
connect_signals (PasswdDialog *pwd)
{
  g_signal_connect (pwd->button, "clicked",
		    G_CALLBACK (ok_clicked_cb),
		    pwd);

  g_signal_connect (pwd->passwd_entry, "activate",
		    G_CALLBACK (ok_clicked_cb),
		    pwd);

  g_signal_connect (pwd->dialog, "delete-event",
		    G_CALLBACK (gtk_main_quit),
		    NULL);
}
 


static GdkFilterReturn
dialog_filter_func (GdkXEvent *xevent, GdkEvent *gevent, gpointer data)
{
  PasswdDialog *pwd = data;
  XEvent *event = xevent;
  gdouble ratio;
 
  if ((event->xany.type != ClientMessage || 
	event->xclient.message_type != XA_UNLOCK_RATIO))
    return GDK_FILTER_CONTINUE;
 
  ratio = event->xclient.data.l[0] / (gdouble)100.0;

  /* CR 6176524 passwdTimeoutEnable for disabled user */
  if (event->xclient.data.l[1] == 0)
    g_object_set (pwd->progress, "fraction", ratio, NULL);
  return GDK_FILTER_REMOVE;

}

static gboolean
handle_input (GIOChannel *source, GIOCondition cond, gpointer data)
{
  PasswdDialog *pwd = data;
  GIOStatus status;
  char *str;
  char *label;
  char* hmsg= (char*) NULL;  /* This is the heading of lock dialog..shows status**/

  if (cond & G_IO_HUP) /* daemon crashed/exited/was killed */
      gtk_main_quit();

 read_line:
  status = g_io_channel_read_line (source, &str, NULL, NULL, NULL);
  if (status == G_IO_STATUS_AGAIN)
    goto read_line;
/* debug only 
  if (status == G_IO_STATUS_ERROR)
      g_message("handle input() status_error %s\n",str);
  if (status == G_IO_STATUS_EOF)
      g_message("handle input() status_eof %s\n",str);
  if (status == G_IO_STATUS_NORMAL)
      g_message("handle input() status_normal %s\n",str);
  Most likely, the returned error msg of g_io_channel_read_line(),
  i.e str will not be translated into other locales ...

*/
   if (str)
    {
     /**fprintf (stderr,">>>>>Child..in handle_input..string is:%s\n",str);
     fflush (stderr);
     **/

     /* This is an ugly code, imp to remember strncmp is a must here,
      * the string sent by parent is in weird stage only strncmp works,
      * i believe need to add a \n to the end of string supplied by parent
      * for it to work with other string operations, but strncmp works fine.
      */

     if( ((strncmp(str,"pw_", 3)) == 0) )
      {
       if ( (strncmp(str,"pw_ok",5)) == 0 )
        {
          hmsg = strdup(_("Authentication Successful!")); 
        }
       else if ( (strncmp(str,"pw_acct_ok",10)) == 0 )
        {
          hmsg = strdup(_("PAM Account Management Also Successful!")); 
        }
       else if ( (strncmp(str,"pw_setcred_fail",15)) == 0 )
        {
          hmsg = strdup(_("Just a Warning PAM Set Credential Failed!")); 
        }
       else if ( (strncmp(str,"pw_setcred_ok",13)) == 0 )
        {
          hmsg = strdup(_("PAM Set Credential Also Successful!")); 
        }
       else if ((strncmp(str,"pw_acct_fail",12)) == 0 ) 
        {
          hmsg = strdup (_("Your Password has expired."));
        }
       else if ((strncmp(str,"pw_fail",7)) == 0 ) 
        {
          hmsg = strdup (_("Sorry!"));
        }
       else if ( strncmp(str,"pw_read",7) == 0 )
        {
          hmsg = strdup(_("Waiting for user input!"));
        }
       else if ( strncmp(str,"pw_time",7) == 0 )
        {
          hmsg = strdup(_("Timed Out!"));
        }
       else if ( strncmp(str,"pw_null",7) == 0 )
        {
          hmsg = strdup(_("Still Checking!"));
        }
       else if ( strncmp(str,"pw_cancel",9) == 0) 
        {
         hmsg =strdup(_("Authentication Cancelled!"));
        }
       else 
        {
         hmsg =strdup(_("Dont know whats up!"));
        }

       if (hmsg)
        {
         label = g_strdup_printf ("<b>%s\n</b>", hmsg);
         g_object_set (pwd->msg_label, "label", label, NULL);
         free (hmsg);
         g_free (label);
        }
      }
     if ( strncmp(str,"pw_", 3) != 0) 
      {
      gtk_label_set_text (GTK_LABEL(pwd->user_prompt_label), str); 
/*
** 6478362(P3) When the AT support is enabled, the input focus
** is located at password label
*/
      gtk_widget_grab_focus(pwd->passwd_entry);
      XSync (GDK_DISPLAY(), False);
      }

      g_free (str);
    }

  return (status == G_IO_STATUS_NORMAL);
}

int
main (int argc, char *argv[])
{
  GIOChannel *ioc;
  PasswdDialog *pwd;
  char *s;
  char *real_progname = argv[0];
  GConfClient *client;
  const char *modulesptr = NULL;
  int j;

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  textdomain (GETTEXT_PACKAGE);

# ifdef HAVE_GTK2
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
# else /* ! HAVE_GTK2 */
  if (!setlocale (LC_ALL, ""))
    fprintf (stderr, "%s: locale not supported by C library\n", real_progname);
# endif /* ! HAVE_GTK2 */

#endif /* ENABLE_NLS */

  s = strrchr (real_progname, '/');
  if (s) real_progname = s+1;
  progname = real_progname;

  gtk_init (&argc, &argv);

  /* Intern the atoms that xscreensaver_command() needs.
   */
  XA_VROOT = XInternAtom (GDK_DISPLAY (), "__SWM_VROOT", FALSE);
  XA_SCREENSAVER = XInternAtom (GDK_DISPLAY (), "SCREENSAVER", FALSE);
  XA_SCREENSAVER_VERSION = XInternAtom (GDK_DISPLAY (), "_SCREENSAVER_VERSION",FALSE);
  XA_SCREENSAVER_STATUS = XInternAtom (GDK_DISPLAY (), "_SCREENSAVER_STATUS", FALSE);
  XA_SCREENSAVER_ID = XInternAtom (GDK_DISPLAY (), "_SCREENSAVER_ID", FALSE);
  XA_SCREENSAVER_RESPONSE = XInternAtom (GDK_DISPLAY (), "_SCREENSAVER_RESPONSE", FALSE);
  XA_SELECT = XInternAtom (GDK_DISPLAY (), "SELECT", FALSE);
  XA_DEMO = XInternAtom (GDK_DISPLAY (), "DEMO", FALSE);
  XA_ACTIVATE = XInternAtom (GDK_DISPLAY (), "ACTIVATE", FALSE);
  XA_BLANK = XInternAtom (GDK_DISPLAY (), "BLANK", FALSE);
  XA_LOCK = XInternAtom (GDK_DISPLAY (), "LOCK", FALSE);
  XA_EXIT = XInternAtom (GDK_DISPLAY (), "EXIT", FALSE);
  XA_RESTART = XInternAtom (GDK_DISPLAY (), "RESTART", FALSE);
  XA_UNLOCK_RATIO = XInternAtom (GDK_DISPLAY (), "UNLOCK_RATIO", FALSE);

/* bugid 6346056(P1):
ATOK pallet sometimes appears in screensave/lock-screen mode
*/
  putenv ("GTK_IM_MODULE=gtk-im-context-simple");


  /* AT-enable mode ? */
  client = gconf_client_get_default ();
  at_enabled = gconf_client_get_bool (client, KEY, NULL);
     
/*CR6205224 disable AT support temp.
  hardwired, at_enabled is False
  at_enabled = False;
* 6240938 screensaver-lock's password timer needs to to be reset for each key
          (all users) and enabling AT support
*/
   
  if (at_enabled) {


 /* GTK Accessibility Module initialized */
  modulesptr = g_getenv ("GTK_MODULES");
  if (!modulesptr || modulesptr [0] == '\0')
        putenv ("GTK_MODULES=gail:atk-bridge");




  






















    Accessibility_LoginHelper_WindowList *windows;
    int i, wid_count = 0;


  /*
if there is any running GOK or MAG, using GTK_WIN_POS_MOUSE
*/











      CORBA_exception_init (&ev);
    if (!bonobo_init (&argc, argv))
    {
        g_error ("Can't initialize Bonobo");
    }

        /* bonobo-activation query lists existing instances */
    server_list = bonobo_activation_query (
        "(repo_ids.has('IDL:Accessibility/LoginHelper:1.0')) AND _active",
        NULL, &ev);

    if (BONOBO_EX (&ev))
    {
        bonobo_debug_shutdown ();
        g_error ("LoginHelper query failed : %s",
                 bonobo_exception_get_text (&ev));
        /* not reached (below) because g_error exits */
        CORBA_exception_free (&ev);
    }

 
/*
** 6182506: scr dialog is obscurred byg MAG window
*/

   if (server_list && server_list->_length)
        center_postion = FALSE;
   else
        center_postion = TRUE;    /* center position of screen */
        
  pwd = atk_make_dialog(center_postion);
  connect_signals (pwd);

  gtk_widget_show_all (pwd->dialog);
  gtk_window_present (GTK_WINDOW (pwd->dialog));
  gtk_widget_map (pwd->dialog);

  XSync(GDK_DISPLAY(), False);


  gdk_window_add_filter (pwd->dialog->window, dialog_filter_func, pwd);
  s = g_strdup_printf ("%lu\n", GDK_WINDOW_XID (pwd->dialog->window));
  write_string (s);
  g_free (s);
  /*CR 5039878 2 3 "Password:" field should be focused / have flashing caret  on the xscreensaver site, it should be focus on passwd_entry
  */
  s = g_strdup_printf ("%lu\n", GDK_WINDOW_XID (pwd->passwd_entry->window));  write_string (s);
  g_free (s);




     g_message ("%d LoginHelpers are running.",
               server_list ? server_list->_length : 0);


    helper_list = g_new0 (Accessibility_LoginHelper, server_list->_length);

    /* for each instance... */
    for (i = 0; i < server_list->_length; i++)
    {
	Accessibility_LoginHelper helper;
	Bonobo_Unknown server;
	Bonobo_ServerInfo info = server_list->_buffer[i];

	server = bonobo_activation_activate_from_id (
	    info.iid, Bonobo_ACTIVATION_FLAG_EXISTING_ONLY, NULL, &ev);

	if (BONOBO_EX (&ev))
	{
	    g_warning ("Error activating server %d: %s", i, bonobo_exception_get_text (&ev));
	    CORBA_exception_free (&ev);
	    continue;
	}
	else if (server == CORBA_OBJECT_NIL)
	{
	    g_warning ("Activated server %d is NIL!", i);
	    continue;
	}

	bonobo_activate ();

	helper = Bonobo_Unknown_queryInterface (
	    server, 
	    "IDL:Accessibility/LoginHelper:1.0",
	    &ev);

	if (BONOBO_EX (&ev))
	{
	    g_warning ("Error performing interface query: %s", bonobo_exception_get_text (&ev));
	    CORBA_exception_free (&ev);
	    continue;
	}
	else if (helper == CORBA_OBJECT_NIL)
	{
	    g_warning ("Activated an object which advertised LoginHelper but does not implement it!");
	    continue;
	}

	helper_list[i] = helper;
	bonobo_object_release_unref (server, &ev);

	if (helper && !BONOBO_EX (&ev))
	{
	    /* ask the helper to go into safe mode */
 
	    safe = Accessibility_LoginHelper_setSafe (helper, TRUE, &ev);
	    if (BONOBO_EX (&ev))
	    {
		g_warning ("setSafe(TRUE) failed: %s", 
			   bonobo_exception_get_text (&ev));
		CORBA_exception_free (&ev);
	    }
	    /* get the raise window list (if the program went into safe mode) */
	    if (safe) 
	    {
		int j;
		gboolean needs_windows_raised = FALSE;
		Accessibility_LoginHelper_DeviceReqList *list;

		g_message ("safe");

		/* does this helper need to have windows raised? */
		list = Accessibility_LoginHelper_getDeviceReqs (helper, &ev);

		if (BONOBO_EX (&ev)) {
		    g_warning ("Bonobo exception getting Device Requirements: %s",
			     bonobo_exception_get_text (&ev));
		    CORBA_exception_free (&ev);
		}
		else
		{
		    g_message ("LoginHelper device requirements: ");
		    if (list->_length == 0) 
			g_message ("\tNone.");

		    for (j = 0; j < list->_length; j++) 
		    {
			switch (list->_buffer[j])
			{
			    case Accessibility_LoginHelper_GUI_EVENTS:
				g_message ("\tNeeds access to the GUI event subsystem (e.g. Xserver)");
				break;
			    case Accessibility_LoginHelper_EXT_INPUT:
				g_message ("\tReads XInput extended input devices");
				break;
			    case Accessibility_LoginHelper_POST_WINDOWS:
				g_message ("\tPosts windows");
				needs_windows_raised = TRUE;
				break;
			    case Accessibility_LoginHelper_AUDIO_OUT:
				g_message ("\tWrites to audio device");
				break;
			    case Accessibility_LoginHelper_AUDIO_IN:
				g_message ("\tReads from audio device");
				break;
			    case Accessibility_LoginHelper_LOCALHOST:
				g_message ("\tNeeds LOCALHOST network connection");
				break;
			    case Accessibility_LoginHelper_SERIAL_OUT:
				g_message ("\tNeeds to write to one or more serial ports");
				break;
			    default:
				break;
			}
		    }
		    CORBA_free (list);
		}
		if (needs_windows_raised) 
		{
		    /* don't raise in this test, but do something with each wid */
		    windows = Accessibility_LoginHelper_getRaiseWindows (helper, &ev);
		    if (BONOBO_EX (&ev))
		    {
			g_warning ("getRaiseWindows failed: %s", 
				   bonobo_exception_get_text (&ev));
			CORBA_exception_free (&ev);
		    }
		    g_message ("%d windows need raising", windows->_length);
		    for (j = 0; j < windows->_length; j++)	
		    {
			Window wid;
			wid = windows->_buffer[j].winID;
			g_message ("Window ID = x%x",  wid);
	        	if (wid_count < MAXRAISEDWINS && wid) {
			wid_count++;
			write_string(g_strdup_printf ("%lu\n", wid));

			}


		    }    
		}
	    }
	    else
	    {
		g_warning ("LoginHelper %d did not go into safe mode", i);
	    }
	}
	else
	{
	    if (BONOBO_EX (&ev))
	    {
		g_warning ("Error activating %s: %s", 
			   info.iid, bonobo_exception_get_text (&ev));
		CORBA_exception_free (&ev);
	    }
	    else
	    {
		g_warning ("no active instance of %s found", info.iid);
	    }
	}
    }




	
    if (wid_count == 0)
	write_null(3);
    if (wid_count == 1)
	write_null(1);


  } /* at-enable mode */

  else 
  {			/* non at-enabled mode */
    pwd = make_dialog ();
  connect_signals (pwd);



  gtk_widget_show_all (pwd->dialog);
  gtk_window_present (GTK_WINDOW (pwd->dialog));
  gtk_widget_map (pwd->dialog);

                                                                                
  XSync(GDK_DISPLAY(), False);
                                                                                
  gdk_window_add_filter (pwd->dialog->window, dialog_filter_func, pwd);

  s = g_strdup_printf ("%lu\n", GDK_WINDOW_XID (pwd->dialog->window));
  write_string (s);
  g_free (s);
    
/*CR 5039878 2 3 "Password:" field should be focused / have flashing caret */
  s = g_strdup_printf ("%lu\n", GDK_WINDOW_XID (pwd->passwd_entry->window));
  write_string (s);
  g_free (s);

 
  /* put the dummy for compactible with at-enable mode , change later */
  write_null(MAXRAISEDWINS);
  }

  ioc = g_io_channel_unix_new (0);
  g_io_add_watch (ioc, G_IO_IN | G_IO_HUP, handle_input, pwd);

  gtk_main ();
  
  return 0;
}


#endif /* HAVE_GTK2 */

