/* 
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * Quickhelp: a tooltip like help system for kppp
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __QUICKHELP__H__
#define __QUICKHELP__H__

#include <qframe.h>
#include <qlist.h>
#include <qpopmenu.h>

struct QuickTip {
  QWidget *widget;
  QString txt;
};

class QuickHelpWindow : public QFrame {
  Q_OBJECT
public:
  QuickHelpWindow();

  void newText();
  void popup(QString text, int atX, int atY);

  virtual void mousePressEvent(QMouseEvent *);
  virtual void keyPressEvent(QKeyEvent *);
  virtual void paintEvent(QPaintEvent *);
  void paint(QPainter *p, int &, int &);  
  virtual void show();
  virtual void hide();

private:
  QString txt;
  QColor txtColor;
  int txtFlags;
};

class QuickHelp : public QObject {
  Q_OBJECT
public:  
  QuickHelp();
  
  static void add(QWidget *, QString);
  static void add(QWidget *, const char *);
  static void remove(QWidget *);

private:
  virtual bool eventFilter(QObject *, QEvent *);
  
private slots:
  void getQuickHelp(int);
  void widgetDestroyed();

private:
  QWidget *current;
  QString currentText;
  QPoint currentPos;

  static QList<QuickTip> tips;
  static QuickHelp *instance;
  static QPopupMenu *menu;
  static QuickHelpWindow *window;
};


#endif
