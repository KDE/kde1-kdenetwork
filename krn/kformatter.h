#ifndef kformatter_included
#define kformatter_included

#include <qstring.h>
#include <qlist.h>
#include "multipar.h"

class KFormatter
{
public:
    KFormatter(QString saveWidget, QString viewWidget, QString msg);
    ~KFormatter();
    QString htmlPart(QList<int> partno);
    QString htmlHeader();
    QString htmlAll();
    bool isMultiPart(QList<int>);

private:
    DwBodyPart* ffwdPart(int n, DwBodyPart* body);
    DwBodyPart* getPartPrim(QList<int> partno, DwBodyPart* body);
    DwBodyPart* getPart(QList<int> partno);
    QString getType(class QListT<int>);
    unsigned int rateType(QString baseType, QString subType);

    QString image_jpegFormatter(QByteArray data, QList<int> part);
    QString text_plainFormatter(QString data, QList<int> part);
    QString text_x_vcardFormatter(QString data, QList<int> part);
    QString text_richtextFormatter(QString data, QList<int> part);
                
    QString saveLink(QList<int> part, char* text);
    QString mailLink(QString reciptent, char* text);
    bool dump(QList<int> part, QString fileName);
    QString listToStr(QList<int>);

    QString saveWidgetName, viewWidgetName;
    DwMessage* message;
    QString* dateFmt;
};

#endif