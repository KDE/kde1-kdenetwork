/*
 * status.h
 * Copyright (C) 1999 Kurt Granroth <granroth@kde.org>
 *
 * This file contains the declaration of the KBiffStatus
 * widget.
 *
 * $Id$
 */
#ifndef KBIFFSTATUS_H 
#define KBIFFSTATUS_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <qframe.h>
#include <qlist.h>
#include <qstring.h>

class QListView;
class QPoint;

class KBiffStatusItem
{
public:
	KBiffStatusItem(const QString& mailbox, const int num_new);

	QString mailbox() const { return _mailbox; }
	QString newMessages() const { return _newMessages; }

protected:
	QString _mailbox;
	QString _newMessages;
};

typedef QList<KBiffStatusItem> KBiffStatusList;

class KBiffStatus : public QFrame
{
	Q_OBJECT
public:
	KBiffStatus(const QString& profile, const KBiffStatusList& list);
	virtual ~KBiffStatus();

	void popup(const QPoint& pos);

protected:
	QListView *_listView;
};

#endif // KBIFFSTATUS_H
