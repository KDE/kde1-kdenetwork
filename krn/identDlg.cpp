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
#include "identDlg.h"
#include <Kconfig.h>

extern KConfig *conf;


#define Inherited QDialog

IdentDlg::IdentDlg
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE ),
	identDlgData( this )
{
    setCaption( "KRN-Identity Configuration" );
    conf->setGroup("Identity");
    address->setText(conf->readEntry("Address").data());
    realname->setText(conf->readEntry("RealName").data());
    organization->setText(conf->readEntry("Organization").data());
    QObject::connect (b1,SIGNAL(clicked()),SLOT(accept()));
    QObject::connect (b2,SIGNAL(clicked()),SLOT(reject()));
}


IdentDlg::~IdentDlg()
{
}
