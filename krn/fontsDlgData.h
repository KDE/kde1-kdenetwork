#ifndef fontsDlgData_included
#define fontsDlgData_included

#include <qcombo.h>
#include <qlined.h>
#include <qpushbt.h>

class fontsDlgData
{
public:

    fontsDlgData
    (
        QWidget* parent
    );


    QPushButton* b1;
    QPushButton* b2;
    QComboBox* fontSize;
    QLineEdit* samp1;
    QLineEdit* samp2;
    QComboBox* stdFontName;
    QComboBox* fixedFontName;

};

#endif // fontsDlgData_included
