#ifndef GLOBAL_CONFIG
#define GLOBAL_CONFIG

#include <qcolor.h>

class global_config {
 public:
  QColor *colour_text;
  QColor *colour_info;
  QColor *colour_chan;
  QColor *colour_error;

  QColor *background;

  QFont defaultfont;

  bool autocreate;
};

extern global_config *kSircConfig;

#endif
