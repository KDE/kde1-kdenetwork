#ifndef GLOBAL_CONFIG
#define GLOBAL_CONFIG

#include <qcolor.h>
#include <qfont.h>

#if (__GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
#error "You need GCC 2.8 or EGCS to compile kSirc.  See README.gcc28 for more information"
#endif

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
  bool autorejoin;

  bool filterKColour;
  bool filterMColour;
  int nickFHighlight; // Foreground
  int nickBHighlight; // Background
  int usHighlight; // Background

  QString kdedir;
};

extern global_config *kSircConfig;

#endif
