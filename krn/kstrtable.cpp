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
#include "kstrtable.h"

KStrTable::KStrTable()
{
}

KStrTable::~KStrTable()
{
}

int KStrTable::read(QString text, QString titleSep=": ", QString lineSep="\n",
             QString start="", QString end="" )
{
    int startpos=text.find(start)+start.length();
    int endpos=text.findRev(end);

    text=text.mid(startpos, endpos);
    int pos=0, tend, cend;

    while(pos<text.length()) {
        //Find the end of the title field
        tend=text.find(titleSep, pos);
        if(tend==-1) break;
        
        //Find the end of the content field
        cend=text.find(lineSep, tend+titleSep.length());
        if(cend==-1) break;
        
        //Create the title and content strings by copying the parts
        //between the separators to them. This would be much clearer if
        //there was a mid() that took start and end values, instead of
        //start and length.
        QString t=text.mid(pos,tend-pos);
        QString c=text.mid(tend+titleSep.length(),
                           cend-(tend+titleSep.length()));
        insert(t, c);
        
        pos=cend+1;
    }
}

QString KStrTable::getItem(QString title)
{
    return Contents.at(Titles.find(title));
}

bool KStrTable::hasItem(QString title)
{
    return Titles.find(title)!=-1;
}

void KStrTable::insert(QString title, QString content, bool)
{
    int pos=Titles.count();
    Titles.insert(pos,title);
    Contents.insert(pos,content);
}

void KStrTable::remove(QString title)
{
    int pos=Titles.find(title);
    Titles.remove(pos);
    Contents.remove(pos);
}

QStrList KStrTable::titles()
{
    return Titles;
}
