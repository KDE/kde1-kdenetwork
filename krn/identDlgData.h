#ifndef identDlgData_included
#define identDlgData_included

#include <qlined.h>
#include <qpushbt.h>

class identDlgData
{
public:

    identDlgData
    (
        QWidget* parent
    );


    QLineEdit* realname;
    QLineEdit* address;
    QLineEdit* organization;
    QPushButton* b1;
    QPushButton* b2;

};

#endif // identDlgData_included
