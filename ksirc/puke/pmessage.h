#ifndef PUKE_MESSAGE_H
#define PUKE_MESSAGE_H

typedef struct {
  unsigned int iHeader;   // Filled in durring in PukeController, do not set
  int iCommand;
  int iWinId;
  int iArg;
  int iTextSize; // Size of the text message that follows
  char *cArg;
} PukeMessage;

typedef struct {
  int fd;
  int iWinId;
} widgetId;

const uint iPukeHeader = 42U;

#endif
