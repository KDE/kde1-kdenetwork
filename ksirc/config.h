#ifndef GLOBAL_CONFIG
#define GLOBAL_CONFIG

#include <qcolor.h>
#include <qfont.h>

class global_config {
 public:
  QColor *colour_text;
  QColor *colour_info;
  QColor *colour_chan;
  QColor *colour_error;

  QColor *colour_background;

  QFont defaultfont;

  bool message_window;
  bool autocreate;
  bool nickcompletion;

  QString kdedir;
};

extern global_config *kSircConfig;

#endif
