#ifndef AHTMLVIEW_H
#define AHTMLVIEW_H
#include <html.h>

#include <qobject.h>

class aHTMLView : public KHTMLWidget
{
  Q_OBJECT;
 public:
  aHTMLView(QWidget *parent=0L, const char *name=0L, int flags=0, KHTMLView *parent_view = 0L);
public slots:
  void slotScrollVert(int);
};

#endif
