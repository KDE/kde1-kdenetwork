#include <iostream.h>
#include <qpainter.h>
#include <qpaintd.h>

#include <kfontdialog.h>

#include "ksticker.h"
#include "speeddialog.h"


KSTicker::KSTicker(QWidget * parent=0, const char * name=0, WFlags f=0) 
: QFrame(parent, name, f)
{

  pHeight = 1;

    setFont(QFont("fixed"));
  // ring = "Hi";
  ring = "";
  SInfo *si = new SInfo;
  si->length = 0;
  StrInfo.append(si);  
  setMinimumSize(100, 10);
  setFixedHeight((fontMetrics().ascent()+2)*pHeight);
  onechar = fontMetrics().width("X");

  tickStep = 2;
  cOffset = 0;

  tickRate = 30;

  currentChar = 0;
  chars = this->width() / onechar;
  StrInfo.setAutoDelete( TRUE );

  popup = new QPopupMenu();
  popup->insertItem("Font...", this, SLOT(fontSelector()));
  popup->insertItem("Scroll Rate...", this, SLOT(scrollRate()));
}

KSTicker::~KSTicker()
{
  killTimers();
}

void KSTicker::show()
{
  QFrame::show();
  startTicker();
  currentChar = display.length() + 1;
  repaint(TRUE);
}

void KSTicker::hide()
{
  killTimers();
  QFrame::hide();
}

void KSTicker::iconify()
{
  QFrame::iconify();
  killTimers();
}

void KSTicker::setString(QString str)
{
  ring.truncate(0);
  ring = str;
  StrInfo.clear();
  SInfo *si = new SInfo;
  si->length = str.length();
  StrInfo.append(si);
  repaint(TRUE);
  startTicker();
}

void KSTicker::mergeString(QString str)
{
  ring += str;
  SInfo *si = new SInfo;
  si->length = str.length();
  StrInfo.append(si);
  while((ring.length() > (uint) 2*chars + 10) && 
	((ring.length() - StrInfo.at(0)->length) > (uint) (chars + chars/2)) &&
	(StrInfo.count() > 1)){
    ring.remove(0, StrInfo.at(0)->length);
    StrInfo.removeFirst();
  }
}

void KSTicker::timerEvent(QTimerEvent *)
{
  if((uint)currentChar >= display.length()){
    display_old = qstrdup(display.data());
    display = qstrdup(ring.data());
    currentChar = 0;
  }
  if(ring.length() > (uint) chars){
    QPainter p(this);
    bitBlt(this, -tickStep, 0, this);
    cOffset += tickStep;
    if(cOffset >= onechar){
      p.setBackgroundMode(OpaqueMode);
      p.drawText(this->width() - onechar - cOffset,
		 this->height() / 4 + p.fontMetrics().height() / 2,
		 display.mid(currentChar, 1),
		 1);
      p.drawText(this->width() - cOffset,
		 this->height() / 4 + p.fontMetrics().height() / 2,
		 " ",
		 1);
      currentChar++;
      cOffset -= onechar;
    }
    p.end();
  }
  else{
    //    cerr << "timer\n";
    repaint(TRUE);
    //    killTimers();
    //    cerr << "Stopped timer\n";
  }
}

void KSTicker::paintEvent( QPaintEvent *)
{
  if(isVisible() == FALSE)
    return;
  QPainter p(this);
  //  p.setFont(QFont("fixed"));
  if(ring.length() > (uint) chars){
    if(currentChar > chars){
      p.setBackgroundMode(OpaqueMode);
      p.drawText(this->width() - chars*onechar - cOffset - onechar,
		 pHeight*(this->height() / 4 + p.fontMetrics().height() / 2),
		 display.data() + currentChar - chars,
		 chars);
    }
    else{
      p.setBackgroundMode(OpaqueMode);
      p.drawText(this->width() - currentChar*onechar - cOffset - onechar,
		 pHeight*(this->height() / 4 + p.fontMetrics().height() / 2),
		 display.data(),
		 display.length() - currentChar); 
      p.drawText(this->width() - currentChar*onechar -display_old.length()*onechar - cOffset - onechar,
		 pHeight*(this->height() / 4 + p.fontMetrics().height() / 2),
		 display_old.data(),
		 display_old.length()); 
    }
  }
  else {
    p.drawText(0,
	       this->height() / 2 + p.fontMetrics().height() / 2,
	       ring,
	       ring.length());
  }
  p.end();
}

void KSTicker::resizeEvent( QResizeEvent *e)
{
  QFrame::resizeEvent(e);
  onechar = fontMetrics().width("X");
  chars = this->width() / onechar;
  killTimers();
  //  if(ring.length() > (uint) chars)
    startTicker();
}

void KSTicker::closeEvent( QCloseEvent *)
{
  emit closing();
  delete this;
}

void KSTicker::startTicker()
{
  killTimers();
  startTimer(tickRate);
}

void KSTicker::mouseDoubleClickEvent( QMouseEvent * ) 
{
  emit doubleClick();
}

void KSTicker::mousePressEvent( QMouseEvent *e)
{
  if(e->button() == RightButton){
    popup->popup(this->cursor().pos());
  }
  else{
    QFrame::mousePressEvent(e);
  }
}
void KSTicker::fontSelector()
{
  KFontDialog *kfd = new KFontDialog();
  kfd->setFont(font());
  connect(kfd, SIGNAL(fontSelected(const QFont &)),
	  this, SLOT(updateFont(const QFont &)));
  kfd->show();
}

void KSTicker::scrollRate()
{
  SpeedDialog *sd = new SpeedDialog(tickRate, tickStep);
  sd->setLimit(5, 200, 1, onechar);
  connect(sd, SIGNAL(stateChange(int, int)),
	  this, SLOT(setSpeed(int, int)));
  sd->show();
}

void KSTicker::updateFont(const QFont &font){
  setFont(font);
  setFixedHeight((fontMetrics().ascent()+2)*pHeight);
  resize(fontMetrics().width("X")*chars, (fontMetrics().ascent()+2)*pHeight);
}

void KSTicker::setSpeed(int _tickRate, int _tickStep){
  tickRate = _tickRate;
  tickStep = _tickStep;
  startTicker();
}

void KSTicker::speed(int *_tickRate, int *_tickStep){
  *_tickRate = tickRate;
  *_tickStep = tickStep;
}
