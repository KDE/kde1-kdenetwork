#include <pws.h>
#include <pws.moc>

PWS::PWS(QWidget *parent, const char *name)
	: KTopLevelWidget(name)
{
    createdUI = FALSE;
    menuBar = 0x0;
    statusBar = 0x0;
    toolBar = 0x0;
    view = 0x0;
}

PWS::~PWS()
{
}

void PWS::invokeHelp()
{
	kapp->invokeHTMLHelp("pws/pws.html", "");
}

void PWS::show(){
    if(createdUI == FALSE){
        QPopupMenu *file_menu = new QPopupMenu();
        file_menu->insertItem(i18n("E&xit"), kapp, SLOT(quit()));

        QPopupMenu *help_menu = new QPopupMenu();
        help_menu->insertItem(i18n("&Help"), this, SLOT(invokeHelp()));

        //	menuBar = new KMenuBar(this, "menubar");
        //	menuBar->insertItem(i18n("&File"), file_menu);
        //	menuBar->insertItem(i18n("&Help"), help_menu);

        //	setMenu(menuBar);

        //	toolBar = new KToolBar(this);
        //	addToolBar(toolBar);

        //	statusBar = new KStatusBar(this);
        //	setStatusBar(statusBar);

        //        resize(600,440);
        view = new PWSWidget(this);
        setView(view);

        connect(view, SIGNAL(quitPressed(QObject *)),
                this, SLOT(closeView(QObject *)));

        createdUI = TRUE;
    }
    KTopLevelWidget::show();
}

void PWS::closeView(QObject *obj){
  emit quitPressed(obj);
  /*
   obj = 0x0;
   delete view;
   view = 0x0;
   createdUI = FALSE;
   */
}