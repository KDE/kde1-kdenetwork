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
#include <kconfig.h>
#include <kapp.h>

#include "kmidentity.h"

//extern KConfig *conf;
extern KMIdentity *identity;

#include "identDlg.moc"

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
    setCaption(klocale->translate("KRN-Identity Configuration") );
//    conf->setGroup("Identity");
    address->setText(identity->emailAddr());
    realname->setText(identity->fullName());
    organization->setText(identity->organization());
    QObject::connect (b1,SIGNAL(clicked()),SLOT(accept()));
    QObject::connect (b2,SIGNAL(clicked()),SLOT(reject()));
}


IdentDlg::~IdentDlg()
{
}
