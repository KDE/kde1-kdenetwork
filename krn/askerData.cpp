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

#include "askerData.h"

#include <kapp.h>

AskerData::AskerData
(
	QWidget* parent
)
{
	label = new QLabel( parent, "Label_1" );
	label->setGeometry( 10, 30, 330, 30 );
	label->setText( "" );

	entry = new QLineEdit( parent, "LineEdit_1" );
	entry->setGeometry( 10, 80, 330, 30 );
	entry->setText( "" );

	b1 = new QPushButton( parent, "PushButton_1" );
	b1->setGeometry( 130, 130, 100, 30 );
	b1->setText( klocale->translate( "OK" ));
	b1->setAutoDefault( TRUE );

	parent->resize( 350, 170 );
}
