#ifndef EXCP_PARSE_H
#define EXCP_PRASE_H

/*
 * This file defines all the parser exceptions generated
 * by the toplevel parse_input function.
 */

#include <qstring.h>

class parseError        // Parsing ran into a fatal error, bail out.
{
public:
  int display;
  QString str;
  char *msg;

  parseError(int _d, QString _s, char *_m = 0)
    {
      display = _d;
      str = _s;
      msg = _m;
    }
};

class badModeChangeError // Mode change barfed up, exit out
{
public:
  QString str;
  char *msg;

  badModeChangeError(QString _str, char *_msg)
    {
      str = _str;
      msg = _msg;
    }
};

class wrongChannelError // Message was not intended for us, display str and continue
{
public:
  int display;
  
  wrongChannelError(int _display)
    {
      display = _display;
    }
};

#endif
