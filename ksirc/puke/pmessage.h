#ifndef PUKE_MESSAGE_H
#define PUKE_MESSAGE_H

typedef struct {
  int iCommand;
  int iWinId;
  int iArg;
  char cArg[50];
} PukeMessage;

typedef struct {
  int fd;
  int iWinId;
} widgetId;

#endif
