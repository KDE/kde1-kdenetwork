#include "defs.h" // includes options.h

int Options::answmach;
int Options::time_before_answmach;
int Options::sound;
int Options::XAnnounce;
char Options::soundfile [S_CFGLINE];
char Options::soundplayer [S_CFGLINE];
char Options::soundplayeropt [S_CFGLINE];
char Options::announce1 [S_CFGLINE];
char Options::announce2 [S_CFGLINE];
char Options::announce3 [S_CFGLINE];
char Options::invitelines [S_INVITE_LINES];
char Options::mailprog [S_CFGLINE];
int Options::NEU_behaviour;
char Options::NEU_user[S_CFGLINE];
char Options::NEUBanner1 [S_CFGLINE] = NEU_BANNER1;
char Options::NEUBanner2 [S_CFGLINE] = NEU_BANNER2;
char Options::NEUBanner3 [S_CFGLINE] = NEU_BANNER3;
char Options::extprg [S_CFGLINE];
char Options::hostname[SYS_NMLN];
int Options::debug_mode;
