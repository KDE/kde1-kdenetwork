#ifndef __INCLUD_H
#define __INCLUD_H

#include <config.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <sys/socket.h>
#include "talkd.h"
#include <netinet/in.h> 

/* Use ntalk protocol */
#define CTL_MSG NEW_CTL_MSG
#define CTL_RESPONSE NEW_CTL_RESPONSE

#ifdef HAVE_PATHS_H
#include <paths.h>
#else
#ifdef HAVE_UTMP_H
#include <utmp.h>
#define _PATH_DEV "/dev/"
#define _PATH_UTMP UTMP_FILE
#define _PATH_TMP "/tmp/"
#else
#warning paths.h not found : using /dev/ and /var/run/utmp as default
#define _PATH_DEV "/dev/"
#define _PATH_UTMP "/var/run/utmp"
#define _PATH_TMP "/tmp/"
#endif
#endif

#endif /* __INCLUD_H */
