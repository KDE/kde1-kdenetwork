/****************************************************************************
** KSProgress meta object code from reading C++ file 'ksprogress.h'
**
** Created: Thu Dec 18 08:34:27 1997
**      by: The Qt Meta Object Compiler ($Revision$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "ksprogress.h"
#include <qmetaobj.h>


const char *KSProgress::className() const
{
    return "KSProgress";
}

QMetaObject *KSProgress::metaObj = 0;

void KSProgress::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(ksprogressData::className(), "ksprogressData") != 0 )
	badSuperclassWarning("KSProgress","ksprogressData");
    if ( !ksprogressData::metaObject() )
	ksprogressData::initMetaObject();
    typedef void(KSProgress::*m1_t0)(int);
    typedef void(KSProgress::*m1_t1)();
    m1_t0 v1_0 = &KSProgress::setValue;
    m1_t1 v1_1 = &KSProgress::cancelPressed;
    QMetaData *slot_tbl = new QMetaData[2];
    slot_tbl[0].name = "setValue(int)";
    slot_tbl[1].name = "cancelPressed()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    typedef void(KSProgress::*m2_t0)();
    typedef void(KSProgress::*m2_t1)(QString);
    m2_t0 v2_0 = &KSProgress::cancel;
    m2_t1 v2_1 = &KSProgress::cancel;
    QMetaData *signal_tbl = new QMetaData[2];
    signal_tbl[0].name = "cancel()";
    signal_tbl[1].name = "cancel(QString)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    signal_tbl[1].ptr = *((QMember*)&v2_1);
    metaObj = new QMetaObject( "KSProgress", "ksprogressData",
	slot_tbl, 2,
	signal_tbl, 2 );
}

// SIGNAL cancel
void KSProgress::cancel()
{
    activate_signal( "cancel()" );
}

#if !defined(Q_MOC_CONNECTIONLIST_DECLARED)
#define Q_MOC_CONNECTIONLIST_DECLARED
#include <qlist.h>
#if defined(Q_DECLARE)
Q_DECLARE(QListM,QConnection);
Q_DECLARE(QListIteratorM,QConnection);
#else
// for compatibility with old header files
declare(QListM,QConnection);
declare(QListIteratorM,QConnection);
#endif
#endif

// SIGNAL cancel
void KSProgress::cancel( QString t0 )
{
    QConnectionList *clist = receivers("cancel(QString)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef RT0 *PRT0;
    typedef void (QObject::*RT1)(QString);
    typedef RT1 *PRT1;
    RT0 r0;
    RT1 r1;
    QConnectionListIt it(*clist);
    QConnection   *c;
    QSenderObject *object;
    while ( (c=it.current()) ) {
	++it;
	object = (QSenderObject*)c->object();
	object->setSender( this );
	switch ( c->numArgs() ) {
	    case 0:
		r0 = *((PRT0)(c->member()));
		(object->*r0)();
		break;
	    case 1:
		r1 = *((PRT1)(c->member()));
		(object->*r1)(t0);
		break;
	}
    }
}
