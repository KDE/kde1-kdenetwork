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
#ifndef kstrtable_included
#define kstrtable_included

#include <qstrlist.h>
#include <qstring.h>

class KStrTable : public QCollection
{
public:
    KStrTable();
    ~KStrTable();

    int read(QString text, QString titleSep=": ", QString lineSep="\n",
             QString start="", QString end="" );
    QString getItem(QString title);
    bool hasItem(QString title);
    void insert(QString title, QString content, bool overwrite=TRUE);
    void remove(QString title);
    QStrList titles();
    uint count() const;
    virtual void clear();

private:
    QStrList Titles;
    QStrList Contents;
};

#endif
