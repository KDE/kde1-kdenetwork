#ifndef kdecode_included
#define kdecode_included

#include <qobject.h>
#include <qdict.h>
#include <qlist.h>
#include <qstrlist.h>
#include <mimelib/message.h>

#include "decoderDlg.h"


class KDecode : public QObject
{
    Q_OBJECT;
public:
    KDecode();
    ~KDecode();

    void load(char *filenames);
    void showWindow();

public slots:
    void decode(int ID,int);

signals:
    void message();
    void busy();

private:
    decoderDlg *dialog;
};

#endif