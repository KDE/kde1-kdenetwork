#ifndef visheadDlgData_included
#define visheadDlgData_included

#include <qchkbox.h>
#include <qlined.h>
#include <qlistbox.h>
#include <qpushbt.h>

class visheadDlgData
{
public:

    visheadDlgData
    (
        QWidget* parent
    );


    QLineEdit* entry;
    QListBox* headerlist;
    QPushButton* b3;
    QPushButton* b1;
    QPushButton* b2;
    QPushButton* b4;
    QCheckBox* all;

};

#endif // visheadDlgData_included
