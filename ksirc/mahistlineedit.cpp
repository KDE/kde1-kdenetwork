/****************************************************************************
** aHistLineEdit meta object code from reading C++ file 'ahistlineedit.h'
**
** Created: Sat Nov 29 12:32:43 1997
**      by: The Qt Meta Object Compiler ($Revision$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "ahistlineedit.h"
#include <qmetaobj.h>


const char *aHistLineEdit::className() const
{
    return "aHistLineEdit";
}

QMetaObject *aHistLineEdit::metaObj = 0;

void aHistLineEdit::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QLineEdit::className(), "QLineEdit") != 0 )
	badSuperclassWarning("aHistLineEdit","QLineEdit");
    if ( !QLineEdit::metaObject() )
	QLineEdit::initMetaObject();
    typedef void(aHistLineEdit::*m2_t0)();
    typedef void(aHistLineEdit::*m2_t1)();
    m2_t0 v2_0 = &aHistLineEdit::gotFocus;
    m2_t1 v2_1 = &aHistLineEdit::lostFocus;
    QMetaData *signal_tbl = new QMetaData[2];
    signal_tbl[0].name = "gotFocus()";
    signal_tbl[1].name = "lostFocus()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    signal_tbl[1].ptr = *((QMember*)&v2_1);
    metaObj = new QMetaObject( "aHistLineEdit", "QLineEdit",
	0, 0,
	signal_tbl, 2 );
}

// SIGNAL gotFocus
void aHistLineEdit::gotFocus()
{
    activate_signal( "gotFocus()" );
}

// SIGNAL lostFocus
void aHistLineEdit::lostFocus()
{
    activate_signal( "lostFocus()" );
}
