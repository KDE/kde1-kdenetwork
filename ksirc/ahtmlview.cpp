#include "ahtmlview.h"

aHTMLView::aHTMLView(QWidget *parent=0L, const char *name=0L)
  : KHTMLWidget(parent, name)
{
}

void aHTMLView::slotScrollVert(int i)
{
  KHTMLView::slotScrollVert(i);
}
