#ifndef kformatter_included
#define kformatter_included

#include <qstring.h>
#include <qlist.h>
#include <mimelib/body.h>
#include <mimelib/headers.h>
#include <mimelib/bodypart.h>
#include <mimelib/message.h>
#include "ktempfile.h"

class KFormatter
{
public:
    KFormatter(QString saveWidget, QString viewWidget, QString msg, bool complete);
    ~KFormatter();
    QString htmlPart(QList<int> partno);
    QString htmlHeader();
    QString htmlAll();
    bool isMultiPart(QList<int>);
    const char* rawPart(QList<int> partno);
    QList<int> strToList(QString);

private:
    QString listToStr(QList<int>);
    DwBodyPart* ffwdPart(int n, DwBodyPart* body);
    DwBodyPart* getPartPrim(QList<int> partno, DwBodyPart* body);
    DwBodyPart* getPart(QList<int> partno);
    QString getType(class QListT<int>);
    unsigned int rateType(QString baseType, QString subType);

    QString image_jpegFormatter(QByteArray data, QList<int> part);
    QString text_plainFormatter(QString data, QList<int> part);
    QString text_x_vcardFormatter(QString data, QList<int> part);
    QString text_richtextFormatter(QString data, QList<int> part);
    QString text_htmlFormatter(QString data, QList<int> partno);

    QString saveLink(QList<int> part, char* text);
    QString viewLink(QList<int> part, char* text);
    QString searchLink(QString subj, QString group);
    QString mailLink(QString reciptent, char* text);
    bool dump(QList<int> part, QString fileName);

    QString saveWidgetName, viewWidgetName;
    DwMessage* message;
    QString* dateFmt;
    KTempFile tempfile;
    bool mcomplete;
};

#endif
