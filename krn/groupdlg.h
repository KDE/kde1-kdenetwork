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
#ifndef Groupdlg_included
#define Groupdlg_included

#include <qwidget.h>
#include <qlistbox.h>
#include <qlist.h>
#include <qpopmenu.h>

#include <ktopwidget.h>
#include <ktreelist.h>

#include "NNTP.h"
#include "newsgroup.h"

class Groupdlg : public KTopLevelWidget 
{
    Q_OBJECT

public:

    Groupdlg
    (
	const char* name = NULL
    );

    virtual ~Groupdlg();

public slots:
    void openGroup(QString name);
    void online();
    void offline();
    bool needsConnect();

    
private slots:
    bool actions(int);
    void fillTree();
    void tag();
    void subscribe();
    void openGroup(int);
    void checkUnread();

private:
    NNTP *server;
    KTreeList *list;
    bool loadSubscribed();
    bool saveSubscribed();
    bool loadActive();
};
#endif // Groupdlg_included
