//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This file os part of KRN, a newsreader for the KDE project.              //
// KRN is distributed under the GNU General Public License.                 //
// Read the acompanying file COPYING for more info.                         //
//                                                                          //
// KRN wouldn't be possible without these libraries, whose authors have     //
// made free to use on non-commercial software:                             //
//                                                                          //
// MIME++ by Doug Sauder                                                    //
// Qt     by Troll Tech                                                     //
//                                                                          //
// This file is copyright 1997 by                                           //
// Roberto Alsina <ralsina@unl.edu.ar>                                      //
// Magnus Reftel  <d96reftl@dtek.chalmers.se>                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#ifndef kmessage_included
#define kmessage_included

#undef Unsorted
#include <html.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qscrbar.h>
#include "multipar.h"
#include "kformatter.h"

class Kmessage : public QWidget
{
    Q_OBJECT

public:

    Kmessage
    (
	QWidget* parent = NULL,
	const char* name = NULL
    );

    virtual ~Kmessage();

public slots:
    void loadMessage (QString message);
    void pageUp();
    void pageDown();     
    void scrollUp();
    void scrollDown();

signals:
    void spawnGroup(QString);
    void spawnArticle(QString);

private slots:
    void URLClicked(const char*,int);
    void renderWidgets();
    void adjustScrollers();

private:
    void resizeEvent(class QResizeEvent *e=NULL);

    KFormatter* format;    

    bool dump(int part, QString fileName);
    MultipartMessage* multi;
    QString saveWidgetName, viewWidgetName;
    
    QStrList tmpFiles;

    KHTMLWidget *view;
    QScrollBar *vertScroller, *horzScroller;
};

#endif // kmessage_included
