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

    static int answmach = 1 ; /* used in talkd.cpp */
    static int time_before_answmach = 20; /* in machines/answmach.cpp */
    /* used in announce.cpp : */
    static int sound = 0;
    static int XAnnounce = 1;
    static char soundfile [S_CFGLINE];
    static char soundplayer [S_CFGLINE];
    static char soundplayeropt [S_CFGLINE];
    static char announce1 [S_CFGLINE] = ANNOUNCE1;
    static char announce2 [S_CFGLINE] = ANNOUNCE2;
    static char announce3 [S_CFGLINE] = ANNOUNCE3;
    static char invitelines [S_INVITE_LINES] = INVITE_LINES;/* used in machines/answmach.cpp */
    static char mailprog [S_CFGLINE] = "mail.local"; /* used in machines/answmach.cpp */
    static int NEU_behaviour = 2; /* default non-existent-user behaviour */
    static char NEU_user[S_CFGLINE];
    static char NEUBanner1 [S_CFGLINE] = NEU_BANNER1;
    static char NEUBanner2 [S_CFGLINE] = NEU_BANNER2;
    static char NEUBanner3 [S_CFGLINE] = NEU_BANNER3;
    static char extprg [S_CFGLINE];
    // No really an option, but it's useful to have it here.
    static char hostname[SYS_NMLN];
    static int debug_mode;
};

#endif
