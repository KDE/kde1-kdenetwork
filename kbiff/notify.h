/*
 * notify.h
 * Copyright (C) 1998 Kurt Granroth <granroth@kde.org>
 *
 * This file contains the declaration of the KBiffNotify
 * widget.
 *
 * $Id$
 */
#ifndef KBIFFNOTIFY_H 
#define KBIFFNOTIFY_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <qdialog.h>

class KBiffNotify : public QDialog
{
	Q_OBJECT
public:
	KBiffNotify(const int num_new, const QString& mailbox);
	virtual ~KBiffNotify();
};

#endif // KBIFFNOTIFY_H
