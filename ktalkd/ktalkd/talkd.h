#ifndef _TALKD_H
#define _TALKD_H

/* talkd.h */

/*
 * This header file was taken from Brukhard Lehner's ktalk, which comes from
 * ytalk version 3.2, file socket.h,  merged with the header file
 * protocols/talkd.h version 8.1 (Berkeley)
 * Original author is Britt Yenne
 * Thanks very much for "lending" this code :-)
 */

/* #include <qglobal.h>   */  /* for definition of Q_INTxx and Q_UINTxx */
/* ktalkd doesn't rely on Qt, so the Qt types used here will be hardcoded.
   All machines now should have 8-bit chars and 32-bit int, I guess.
   Is int 32 bit even on Alphas ? */

#define Q_UINT8 unsigned char
#define Q_INT8 char
#define Q_UINT32 unsigned int
#define Q_INT32 int

/* ---- talk daemon I/O structures ---- */

#define TTY_SIZE 16

#ifdef OTALK
/* Control Message structure for old talk protocol (earlier than BSD4.2) */

#define NAME_SIZE 9

typedef struct {
  Q_UINT8  type;                  /* request type, see below */
  char     l_name [NAME_SIZE];    /* caller's name */
  char     r_name [NAME_SIZE];    /* callee's name */
  Q_INT8   pad;
  Q_UINT32 id_num;                /* message id */
  Q_INT32  pid;                   /* caller's process id */
  char     r_tty [TTY_SIZE];      /* callee's tty name */
  struct sockaddr addr;           /* socket address for connection */
  struct sockaddr ctl_addr;       /* control socket address */
} CTL_MSG;

/* Control Response structure for old talk protocol (earlier than BSD4.2) */

typedef struct {
  Q_UINT8  type;         /* type of request message, see below */
  Q_UINT8  answer;       /* respose to request message, see below */
  Q_INT8  pad1;
  Q_INT8  pad2;
  Q_UINT32 id_num;       /* message id */
  struct sockaddr addr;  /* address for establishing conversation */ 
} CTL_RESPONSE;

#else /* not OTALK */

/* Control Message structure for new talk protocol (BSD4.2 and later) */

#define NAME_SIZE 12

typedef struct {
  Q_UINT8   vers;                  /* protocol version */
  Q_UINT8   type;                  /* request type, see below */
  Q_INT8  pad1;
  Q_INT8  pad2;
  Q_UINT32  id_num;                /* message id */
  struct sockaddr addr;            /* socket address for connection */
  struct sockaddr ctl_addr;        /* control socket address */
  Q_INT32   pid;                   /* caller's process id */
  char	    l_name[NAME_SIZE];     /* caller's name */
  char	    r_name[NAME_SIZE];     /* callee's name */
  char	    r_tty[TTY_SIZE];       /* callee's tty name */
} CTL_MSG;

/* Control Response structure for new talk protocol (BSD4.2 and later) */

typedef struct {
  Q_UINT8  vers;         /* protocol version */
  Q_UINT8  type;         /* type of request message, see below */
  Q_UINT8  answer;       /* respose to request message, see below */
  Q_INT8   pad;
  Q_UINT32 id_num;       /* message id */
  struct sockaddr addr;  /* address for establishing conversation */
} CTL_RESPONSE;

#define	TALK_VERSION	1		/* protocol version */

#endif /* Protocol switch */

/* Dgram Types.
 *
 * These are the "type" values for xxx_CLT_MSG and xxx_CLT_RESPONSE.  Each acts
 * either on the remote daemon or the local daemon, as marked.
 */

#define LEAVE_INVITE	0	/* leave an invitation (local) */
#define LOOK_UP		1	/* look up an invitation (remote) */
#define DELETE		2	/* delete erroneous invitation (remote) */
#define ANNOUNCE	3	/* ring a user (remote) */
/* Not supported by ktalkd : (who uses them ?) */
#define DELETE_INVITE	4	/* delete my invitation (local) */
#define AUTO_LOOK_UP	5	/* look up auto-invitation (remote) */
#define AUTO_DELETE	6	/* delete erroneous auto-invitation (remote) */

/* answer values.
 *
 * These are the values that are returned in "answer" of CTL_RESPONSE.
 */

#define SUCCESS         0       /* operation completed properly */
#define NOT_HERE        1       /* callee not logged in */
#define FAILED          2       /* operation failed for unexplained reason */
#define MACHINE_UNKNOWN 3       /* caller's machine name unknown */
#define PERMISSION_DENIED 4     /* callee's tty doesn't permit announce */
#define UNKNOWN_REQUEST 5       /* request has invalid type value */
#define BADVERSION      6       /* request has invalid protocol version */
#define BADADDR         7       /* request has invalid addr value */
#define BADCTLADDR      8       /* request has invalid ctl_addr value */

/* Operational parameters. */

#define MAX_LIFE        60      /* max time daemon saves invitations */
#define RING_WAIT       30      /* time to wait before resending invitation */
/* RING_WAIT should be 10's of seconds less than MAX_LIFE */

#endif
