#ifndef EXPIRE_DLG
#define EXPIRE_DLG

#include <qdialog.h>
#include <keditcl.h>

class ExpireDlg: public QDialog
{
    Q_OBJECT
public:
    ExpireDlg();
    ~ExpireDlg();
public slots:
    void save();
private:
    KIntLineEdit *bodies;
    KIntLineEdit *headers;
};

#endif