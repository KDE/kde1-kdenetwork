#ifndef KSIRCTOPLEVEL_H
#define KSIRCTOPLEVEL_H

class KSircTopLevel;
class UserControlMenu;

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>  

#include <qwidget.h>
#include <qsocknot.h>
#include <qlined.h>
#include <qmlined.h>
#include <qlayout.h>
#include <qframe.h>
#include <qlistbox.h>
#include <qscrbar.h>
#include <qregexp.h>
#include <qaccel.h>

#include <ktopwidget.h>
#include <kmenubar.h>
#include <kfm.h>
#include <ksimpleconfig.h>
#include <knewpanner.h>

//#include "ahtmlview.h"
#include "irclistitem.h"
#include "ahistlineedit.h"
#include "alistbox.h"
#include "messageReceiver.h"
#include "ksircprocess.h"
#include "KSTicker/ksticker.h"

class UserControlMenu {
 public:
  UserControlMenu(char *_title = 0, 
		  char *_action = 0, 
		  int _accel = 0, 
		  int _type = 0, 
		  bool _op_only = FALSE) 
    { title = _title; 
      action = _action; 
      accel = _accel; 
      type = (itype) _type; 
      op_only = _op_only; }
  char *title;
  char *action;
  int accel;
  bool op_only;
  enum itype { Seperator, Text } type;
};

#include "UserMenuRef.h"
#include "irclistbox.h"

class KSircTopLevel : public KTopLevelWidget,
		      public KSircMessageReceiver
{
  Q_OBJECT;
public:
  KSircTopLevel(KSircProcess *_proc, char *cname=0L, const char * name=0);
  ~KSircTopLevel();

signals:
  void outputLine(QString);
  void open_toplevel(QString);
  void closing(KSircTopLevel *, char *);
  void changeChannel(QString, QString);
  void currentWindow(KSircTopLevel *);
  void changeSize();

public slots:
  void sirc_receive(QString str);
//  void sirc_stop(bool STOP = FALSE);
  void sirc_line_return(); 

  void control_message(QString str); 

protected slots:
   void URLSelected(const char *, int); 
   void UserSelected(int index); 
   void UserParseMenu(int id);
   void AccelScrollDownPage();
   void AccelScrollUpPage();
   void AccelPriorMsgNick();
   void AccelNextMsgNick();
   void terminate() { close(1); }
   void startUserMenuRef();
   void UserUpdateMenu();
   void newWindow();
   void gotFocus();
   void lostFocus();
   void showTicker();
   void unHide();

protected:
   virtual void closeEvent(QCloseEvent *);
   virtual void resizeEvent(QResizeEvent *);

private:
  int sirc_stdin, sirc_stdout, sirc_stderr;
  char line_buf[1024];
  bool continued_line;
  KNewPanner *pan;
  KSircListBox *mainw;
  aHistLineEdit *linee;
  QSocketNotifier *sirc_r_notif;
  aListBox *nicks;

  KSircProcess *proc;

  QList<QString> contents;
  int lines;

  bool Buffer;
  QStrList *LineBuffer;

  ircListItem *parse_input(QString &string);
  void sirc_write(QString &str);

  QPopupMenu *user_controls;
  static QList<QPopupMenu> user_menu_list;
  static QList<UserControlMenu> user_menu;
  static void initPopUpMenu();
  static void writePopUpMenu();
  int opami;
  bool popup_have_control;

  QAccel *accel;
  QStrIList nick_ring;

  static QPixmap *pix_info;
  static QPixmap *pix_star;
  static QPixmap *pix_bball;
  static QPixmap *pix_greenp;
  static QPixmap *pix_bluep;
  static QPixmap *pix_madsmile;

  char *channel_name;
  QString caption;

  int have_focus;

  KSTicker *ticker;
  QRect myrect;
  QPoint mypoint;
  QRect tickerrect;
  QPoint tickerpoint;

};

#endif
