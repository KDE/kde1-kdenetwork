#ifndef OPTIONS_H
#define OPTIONS_H
/** This class handles global options. Not user ones */

#include <sys/utsname.h>
#ifndef SYS_NMLN
#define SYS_NMLN  65             /* max hostname size */
#endif

class Options
{ 
  
  public:

    static int answmach; /* used in talkd.cpp */
    static int time_before_answmach; /* in machines/answmach.cpp */
    /* used in announce.cpp : */
    static int sound;
    static int XAnnounce;
    static char soundfile [S_CFGLINE];
    static char soundplayer [S_CFGLINE];
    static char soundplayeropt [S_CFGLINE];
    static char announce1 [S_CFGLINE];
    static char announce2 [S_CFGLINE];
    static char announce3 [S_CFGLINE];
    static char invitelines [S_INVITE_LINES];/* used in machines/answmach.cpp */
    static char mailprog [S_CFGLINE]; /* used in machines/answmach.cpp */
    static int NEU_behaviour;
    static char NEU_user[S_CFGLINE];
    static char NEUBanner1 [S_CFGLINE];
    static char NEUBanner2 [S_CFGLINE];
    static char NEUBanner3 [S_CFGLINE];
    static char extprg [S_CFGLINE];
    // No really an option, but it's useful to have it here.
    static char hostname[SYS_NMLN];
    static int debug_mode;
};

#endif
