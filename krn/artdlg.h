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
#ifndef Artdlg_included
#define Artdlg_included

#include "kmessage.h"
#include "NNTP.h"

#include <qlistbox.h>
#include <qstrlist.h>
#include <qaccel.h>
#include <qevent.h>
#include <qlayout.h>

#include <ktopwidget.h>
#include <kpanner.h>
#include "kmessage.h"
#include "ktablistbox.h"
#include "kiconloader.h"



#include "newsgroup.h"

class Artdlg : public KTopLevelWidget 
{
    Q_OBJECT

public:

    Artdlg (NewsGroup *_group, NNTP* server);
    virtual ~Artdlg();

    Kmessage *messwin;
protected:
    virtual void closeEvent (QCloseEvent *e);

private slots:
    bool actions(int);
    bool taggedActions(int);
    void fillTree();
    void getSubjects();
    void loadArt (int index,int column);
    bool loadArt (QString id);
    void markArt (int index,int column);
    void decArt (int index,int column);
    void saveArt (QString id);

signals:
    void needConnection ( );
    
private:
    NNTP *server;
    KTabListBox *list;
    char *groupname;
    QAccel *acc;
    NewsGroup *group;
    ArticleList artList;
    KMenuBar *menu;
    KToolBar *tool;
    KPanner *panner;
    QGridLayout *gl;
    KStatusBar *status;
};

#endif // Artdlg_included
