#include "defs.h" // includes options.h

int Options::answmach = 1;
int Options::time_before_answmach = 20;
int Options::sound = 0;
int Options::XAnnounce = 1;
char Options::soundfile [S_CFGLINE];
char Options::soundplayer [S_CFGLINE];
char Options::soundplayeropt [S_CFGLINE];
char Options::announce1 [S_CFGLINE] = ANNOUNCE1;
char Options::announce2 [S_CFGLINE] = ANNOUNCE2;
char Options::announce3 [S_CFGLINE] = ANNOUNCE3;
char Options::invitelines [S_INVITE_LINES] = INVITE_LINES;
char Options::mailprog [S_CFGLINE] = "mail.local";
int Options::NEU_behaviour = 2; /* default non-existent-user behaviour */
char Options::NEU_user[S_CFGLINE];
char Options::NEUBanner1 [S_CFGLINE] = NEU_BANNER1;
char Options::NEUBanner2 [S_CFGLINE] = NEU_BANNER2;
char Options::NEUBanner3 [S_CFGLINE] = NEU_BANNER3;
char Options::extprg [S_CFGLINE];
char Options::hostname[SYS_NMLN];
int Options::debug_mode = 0;
