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

#include "quickhelp.h"
#include <qpainter.h>
#include <qpixmap.h>
#include <stdio.h>
#include <kapp.h>
#include <qpopmenu.h>
#include <qbitmap.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/shape.h>

#define F_BOLD 1
#define F_ITALIC 2
#define F_UNDERLINE 3
#define X_BORDER 10
#define Y_BORDER 6

QPopupMenu *QuickHelp::menu = 0;
QuickHelp *QuickHelp::instance = 0;
QList<QuickTip> QuickHelp::tips;
QuickHelpWindow *QuickHelp::window = 0;

QuickHelp::QuickHelp() : QObject(0) {
}

void QuickHelp::add(QWidget *w, QString s) {  
  if(w) {
    // make sure we have  a class instance running
    if(instance == 0) {
      instance = new QuickHelp();
      window = new QuickHelpWindow();
      menu = new QPopupMenu;
      menu->insertItem(i18n("Quickhelp"));
      connect(menu, SIGNAL(activated(int)),
	      instance, SLOT(getQuickHelp(int)));
      tips.setAutoDelete(TRUE);
    }

    QuickTip *qt = new QuickTip;
    qt->widget = w;
    qt->txt = s;
    qt->txt.detach();
    tips.append(qt);
    connect(w, SIGNAL(destroyed()),
	    instance, SLOT(widgetDestroyed()));

    w->installEventFilter(instance);
  }
}

void QuickHelp::add(QWidget *w, const char *s) {
  QuickHelp::add(w, QString(s));
}

void QuickHelp::remove(QWidget *w) {
  for(unsigned i = 0; i < tips.count(); i++)
    if(tips.at(i)->widget == w) {
      tips.remove(i);
      return;
    }      
}

void QuickHelp::widgetDestroyed() {
  remove((QWidget *)sender());
}

bool QuickHelp::eventFilter(QObject *o, QEvent *e) {
  if(e->type() == Event_MouseButtonPress && ((QMouseEvent *)e)->button() == RightButton) {
    for(unsigned i = 0; i < tips.count(); i++) {
      if(tips.at(i)->widget == (QWidget *)o) {
	current = tips.at(i)->widget;
	currentText = tips.at(i)->txt;
	currentPos = QCursor::pos();
	menu->popup(currentPos);
	return TRUE;
      }
    }
  }
  
  return FALSE;
}

void QuickHelp::getQuickHelp(int) {
  window->popup(currentText, currentPos.x(), currentPos.y());
}

QuickHelpWindow::QuickHelpWindow() : QFrame(0, 0, WStyle_Customize|WStyle_Tool) {
  setBackgroundColor(QColor(255, 255, 220));
}


void QuickHelpWindow::newText() {
  txtColor = black;
  txtFlags = 0;

  QPixmap pm(1, 1);
  QPainter p;
  p.begin(&pm);

  int w = 0, h = 0;
  paint(&p, w, h);
  p.end();

  resize(w + X_BORDER, h + Y_BORDER);
}

void QuickHelpWindow::mousePressEvent(QMouseEvent *e) {
  hide();
}


void QuickHelpWindow::keyPressEvent(QKeyEvent *e) {
  e->accept();
  hide();
}


void QuickHelpWindow::show() {
  QFrame::show();
  grabMouse();
  grabKeyboard();
}


void QuickHelpWindow::hide() {
  QFrame::hide();
  releaseMouse();
  releaseKeyboard();
}


void QuickHelpWindow::popup(QString text, int x, int y) {
  txt = text.copy();
  newText();

  if(x + width() > QApplication::desktop()->width() - 8)
    x = QApplication::desktop()->width() - 8 - width();
  if(y + height() > QApplication::desktop()->height() - 4)
    y = QApplication::desktop()->height() - 4 - height();
  move(x, y);
  show();
}
  
  
void QuickHelpWindow::paintEvent(QPaintEvent *) {
  QPainter p;

  p.begin(this);
  int x, y;
  paint(&p, x, y);
  p.end();

  {
  QBitmap bm(100, 100);
  QPainter p;
  p.begin(&bm);
  p.setPen(black);
  bm.fill(color0);
  for(int x = 0; x < 100; x++)
    for(int y = 0; y < 100; y++)
      if((x+y) & 1 == 0)
	p.drawPoint(x, y);    
  p.end();

  XShapeCombineMask( x11Display(), winId(), ShapeBounding, 0, 0, bm.handle(), ShapeSet );
  }
}

void QuickHelpWindow::paint(QPainter *p, int &w, int &h) {
  int posx = X_BORDER, posy = Y_BORDER;
  unsigned txtIndex = 0;
  int maxy = 0, maxx = 0;

  int pointsize = p->font().pointSize();
  p->setPen(black);

  while(txtIndex < txt.length()) {
    // fetch token
    QString token = "";
    bool tokencomplete = FALSE;

    while(txtIndex < txt.length() && !tokencomplete) {
      switch(txt[txtIndex]) {
      case '\n':
	if(token.length() == 0) {
	  token = "\n";
	  txtIndex++;
	}
	tokencomplete = TRUE;
	break;
      case '<':
	if(token.length() == 0)
	  token += txt[txtIndex];
	else
	  tokencomplete = TRUE;
	break;
      case '>':
	token += '>';
	tokencomplete = TRUE;
	txtIndex++;
	break;
      default:
	token += txt[txtIndex];
      }

      if(!tokencomplete)
	txtIndex++;
    }
    
    //printf("TOKEN=%s, INDEX=%d\n", token.data(), txtIndex);

    if(token == "<bold>")
      txtFlags ^= F_BOLD;
    else if(token == "<italic>")
      txtFlags ^= F_ITALIC;
    else if(token == "<underline>")
      txtFlags ^= F_UNDERLINE;
    else if(token == "<red>")
      txtColor = red;
    else if(token == "<blue>")
      txtColor = blue;
    else if(token == "<green>")
      txtColor = green;
    else if(token == "<black>")
      txtColor = black;
    else if(token == "<magenta>")
      txtColor = magenta;
    else if(token == "<+>") {
      QFont f = p->font();
      while(QFontInfo(f).pointSize() == pointsize)
	f.setPointSize(f.pointSize() + 1);
      pointsize = QFontInfo(f).pointSize();
    } else if(token == "<->") {
      QFont f = p->font();
      while(QFontInfo(f).pointSize() == pointsize)
	f.setPointSize(f.pointSize() - 1);
      pointsize = QFontInfo(f).pointSize();
    } else if(token == "\n") {
      posx = X_BORDER;
      posy += maxy;
      maxy = 10;
    } else {
      if(token.left(1) == "<") {
	printf("UNKNOWN TOKEN %s\n", token.data());
	continue;
      }

      p->setPen(txtColor);
      QFont f = font();
      f.setPointSize(pointsize);
      f.setBold((txtFlags & F_BOLD)!=0);
      f.setItalic((txtFlags & F_ITALIC)!=0);
      f.setUnderline((txtFlags & F_UNDERLINE)!=0);
      p->setFont(f);
      QRect r;
      p->drawText(posx, posy, 1024, 1024, AlignLeft|AlignTop, token.data(), -1, &r, 0);
      posx += r.width();
      maxy = QMAX(maxy, p->fontMetrics().lineSpacing());
      maxx = QMAX(maxx, r.right());
    }
  }

  w = maxx;
  h = posy + maxy;

  // draw frame
  p->setPen(QPen(black, 1));
  p->drawRect(0, 0, w + X_BORDER, h + Y_BORDER);
}

#include "quickhelp.moc"
