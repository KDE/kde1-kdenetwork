/****************************************************************************
** aListBox meta object code from reading C++ file 'alistbox.h'
**
** Created: Sat Nov 29 12:16:46 1997
**      by: The Qt Meta Object Compiler ($Revision$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "alistbox.h"
#include <qmetaobj.h>


const char *aListBox::className() const
{
    return "aListBox";
}

QMetaObject *aListBox::metaObj = 0;

void aListBox::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QListBox::className(), "QListBox") != 0 )
	badSuperclassWarning("aListBox","QListBox");
    if ( !QListBox::metaObject() )
	QListBox::initMetaObject();
    typedef void(aListBox::*m2_t0)(int);
    m2_t0 v2_0 = &aListBox::rightButtonPress;
    QMetaData *signal_tbl = new QMetaData[1];
    signal_tbl[0].name = "rightButtonPress(int)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = new QMetaObject( "aListBox", "QListBox",
	0, 0,
	signal_tbl, 1 );
}

// SIGNAL rightButtonPress
void aListBox::rightButtonPress( int t0 )
{
    activate_signal( "rightButtonPress(int)", t0 );
}
