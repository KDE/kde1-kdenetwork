/****************************************************************************
** ksprogressData meta object code from reading C++ file 'ksprogressdata.h'
**
** Created: Mon Jul 27 14:30:33 1998
**      by: The Qt Meta Object Compiler ($Revision$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "ksprogressdata.h"
#include <qmetaobject.h>


const char *ksprogressData::className() const
{
    return "ksprogressData";
}

QMetaObject *ksprogressData::metaObj = 0;

void ksprogressData::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QFrame::className(), "QFrame") != 0 )
	badSuperclassWarning("ksprogressData","QFrame");
    if ( !QFrame::metaObject() )
	QFrame::initMetaObject();
    typedef void(ksprogressData::*m1_t0)();
    m1_t0 v1_0 = &ksprogressData::cancelPressed;
    QMetaData *slot_tbl = new QMetaData[1];
    slot_tbl[0].name = "cancelPressed()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    metaObj = new QMetaObject( "ksprogressData", "QFrame",
	slot_tbl, 1,
	0, 0 );
}
