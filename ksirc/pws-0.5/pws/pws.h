#ifndef PWS_H 
#define PWS_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <kapp.h>
#include <ktopwidget.h>

#include "pwswidget.h"

class PWS : public KTopLevelWidget
{
	Q_OBJECT
public:
	PWS(QWidget *parent = 0, const char *name = 0);
        virtual ~PWS();

        void startServer() {
            if(view != 0x0)
                view->restart();
        }

signals:
        void quitPressed(QObject *);

protected slots:
	void invokeHelp();

private:
	KMenuBar   *menuBar;
	KStatusBar *statusBar;
	KToolBar   *toolBar;

	PWSWidget *view;
};

#endif // PWS_H 
