#ifndef __DEFS_H
#define __DEFS_H

#include "includ.h"

 /* Configuration file-read buffer sizes */
#define S_CFGLINE 80

   /* from answmach/io.c */
#define S_MESSG 120
#define S_COMMAND 100

#define S_INVITE_LINES 200

#define DISPLAYS_LIST_MAX 200
#define DISPLAY_MAX 50

/*
 * Invitation displayed by the answering machine when the caller connects.
 * Add \n at the end of any line you want it to display,
 *   and \ at the end of any line of the definition of INVITE_LINES
 * They are not necessarily the same ...
 *
 * ANNOUNCE* are the 3 lines displayed on the terminal / on 
 * the console, to announce the request.
 */

#define ANNOUNCE1 "Message from Talk_Daemon at %d:%02d ..."
#define ANNOUNCE2 "talk: connection requested by %s."
#define ANNOUNCE3 "talk: respond with: talk %s"
#define INVITE_LINES "Hello. You're connected to a talk answering \
machine.\nThe person you have paged isn't there at the moment.\n\
Please leave a message and quit normally when finished.\n"

/*Please check that INVITE_LINES is max S_INVITE_LINES chars*/

/* Default message if N.E.U (non existent user) called */
#define NEU_BANNER1 "The person you're asking to talk with is unknown at this host."
#define NEU_BANNER2 "You may have mistyped the name, or network address. Try again"
#define NEU_BANNER3 "or leave a message which will be sent to the system administrator."

/*  User-configurable "To:" E-Mail address */
#define USER_SETS_EMAIL

extern char OPTsoundplayer[];
extern char OPTsoundplayeropt[];
extern char OPTsoundfile[];
extern int  OPTsound;
extern int  OPTXAnnounce;
extern char OPTannounce1[];
extern char OPTannounce2[];
extern char OPTannounce3[];
extern int OPTNEU_behaviour;
extern char OPTNEUBanner1[];
extern char OPTNEUBanner2[];
extern char OPTNEUBanner3[];
extern char OPTNEU_user[];
extern char	OPTNEU_set_user_name[];
extern char	callee_name[];
extern int OPTanswmach;
extern int OPTtime_before_answmach;
extern char OPTinvitelines[];
extern char OPTmailprog[];
extern char OPTextprg[];

extern char hostname[];
extern int debug_mode;

/* return value from process_request : */
#define PROC_REQ_OK 0
#define PROC_REQ_ANSWMACH 1
#define PROC_REQ_ANSWMACH_NOT_LOGGED 2
#define PROC_REQ_ANSWMACH_NOT_HERE 3
#define PROC_REQ_ERR 4
/* Min and max to launch answer machine : */
#define PROC_REQ_MIN_A 1
#define PROC_REQ_MAX_A 3

#endif /* __DEFS_H */
