/****************************************************************************
** KAnswmachPageConfig meta object code from reading C++ file 'answmachpage.h'
**
** Created: Thu Jun 11 23:38:50 1998
**      by: The Qt Meta Object Compiler ($Revision$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "answmachpage.h"
#include <qmetaobj.h>


const char *KAnswmachPageConfig::className() const
{
    return "KAnswmachPageConfig";
}

QMetaObject *KAnswmachPageConfig::metaObj = 0;

void KAnswmachPageConfig::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(KConfigWidget::className(), "KConfigWidget") != 0 )
	badSuperclassWarning("KAnswmachPageConfig","KConfigWidget");
    if ( !KConfigWidget::metaObject() )
	KConfigWidget::initMetaObject();
    typedef void(KAnswmachPageConfig::*m1_t0)();
    m1_t0 v1_0 = &KAnswmachPageConfig::answmachOnOff;
    QMetaData *slot_tbl = new QMetaData[1];
    slot_tbl[0].name = "answmachOnOff()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    metaObj = new QMetaObject( "KAnswmachPageConfig", "KConfigWidget",
	slot_tbl, 1,
	0, 0 );
}
