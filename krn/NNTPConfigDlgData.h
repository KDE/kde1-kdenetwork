#ifndef NNTPConfigDlgData_included
#define NNTPConfigDlgData_included

#include <qchkbox.h>
#include <qlined.h>
#include <qpushbt.h>

class NNTPConfigDlgData
{
public:

    NNTPConfigDlgData
    (
        QWidget* parent
    );


    QPushButton* b1;
    QPushButton* b2;
    QLineEdit* servername;
    QCheckBox* connectatstart;
    QCheckBox* authenticate;
    QLineEdit* username;
    QLineEdit* password;
    QLineEdit* smtpserver;

};

#endif // NNTPConfigDlgData_included
