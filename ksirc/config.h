#ifndef GLOBAL_CONFIG
#define GLOBAL_CONFIG

#include <qcolor.h>
#include <qfont.h>

#define KSIRC_VERSION "980914"

class global_config {
 public:
  QColor *colour_text;
  QColor *colour_info;
  QColor *colour_chan;
  QColor *colour_error;

  QColor *colour_background;

  QFont defaultfont;

  bool AutoCreateWin;
  bool BeepNotify;
  bool NickCompletion;
  bool ColourPicker;
  bool AutoRejoin;
  bool BackgroundPix;
  QString BackgroundFile;
  bool transparent;

  bool filterKColour;
  bool filterMColour;
  int nickFHighlight; // Foreground
  int nickBHighlight; // Background
  int usHighlight; // Background

  QString kdedir;

  QString pukeSocket;
};

extern global_config *kSircConfig;

#endif
