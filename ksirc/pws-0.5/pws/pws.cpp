#include <pws.h>
#include <pws.moc>

PWS::PWS(QWidget *parent, const char *name)
	: KTopLevelWidget(name)
{
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
                this, SIGNAL(quitPressed(QObject *)));
}

PWS::~PWS()
{
}

void PWS::invokeHelp()
{
	kapp->invokeHTMLHelp("pws/pws.html", "");
}

