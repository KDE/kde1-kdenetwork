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
#include <htmlview.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qscrbar.h>
#include "kformatter.h"

class Kmessage : public KHTMLView
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
    void loadMessage (QString message, bool complete=TRUE);

signals:
    void spawnGroup(QString);
    void spawnArticle(QString);

private slots:
    void URLClicked(const char*,int);
    void renderWidgets();

private:

    KFormatter* format;

    bool dump(char* part, QString fileName);
    QString saveWidgetName, viewWidgetName;

    QStrList tmpFiles;

    KHTMLView *view;
};

#endif // kmessage_included
