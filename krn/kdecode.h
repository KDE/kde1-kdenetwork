#ifndef kdecode_included
#define kdecode_included

#include <qobject.h>
#include <qdict.h>
#include <qlist.h>
#include <qstrlist.h>
#include <qstring.h>

#include "decoderDlg.h"


class KDecode : public QObject
{
    Q_OBJECT;
public:
    KDecode();
    ~KDecode();

    void load(char *filenames);
    void showWindow();
    static class DwString * decodeString(const char* data, QString type);
    static const char* encodeString(const char* data, QString type);

public slots:
    void decode(int ID,int);

signals:
    void message();
    void busy();

private:
    decoderDlg *dialog;
    QStrList filenames;
};

#endif

