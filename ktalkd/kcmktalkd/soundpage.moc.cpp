/****************************************************************************
** KSoundPageConfig meta object code from reading C++ file 'soundpage.h'
**
** Created: Wed Jun 10 00:38:13 1998
**      by: The Qt Meta Object Compiler ($Revision$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "soundpage.h"
#include <qmetaobj.h>


const char *KSoundPageConfig::className() const
{
    return "KSoundPageConfig";
}

QMetaObject *KSoundPageConfig::metaObj = 0;

void KSoundPageConfig::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(KConfigWidget::className(), "KConfigWidget") != 0 )
	badSuperclassWarning("KSoundPageConfig","KConfigWidget");
    if ( !KConfigWidget::metaObject() )
	KConfigWidget::initMetaObject();
    typedef void(KSoundPageConfig::*m1_t0)();
    typedef void(KSoundPageConfig::*m1_t1)(KDNDDropZone*);
    typedef void(KSoundPageConfig::*m1_t2)();
    m1_t0 v1_0 = &KSoundPageConfig::soundOnOff;
    m1_t1 v1_1 = &KSoundPageConfig::soundDropped;
    m1_t2 v1_2 = &KSoundPageConfig::playCurrentSound;
    QMetaData *slot_tbl = new QMetaData[3];
    slot_tbl[0].name = "soundOnOff()";
    slot_tbl[1].name = "soundDropped(KDNDDropZone*)";
    slot_tbl[2].name = "playCurrentSound()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    metaObj = new QMetaObject( "KSoundPageConfig", "KConfigWidget",
	slot_tbl, 3,
	0, 0 );
}
