#ifndef fontsDlgData_included
#define fontsDlgData_included

#include <qcombo.h>
#include <qlined.h>
#include <qpushbt.h>

#include <kcolorbtn.h>

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
    KColorButton* bgColor;
    KColorButton* fgColor;
    KColorButton* linkColor;
    KColorButton* followColor;

};

#endif // fontsDlgData_included
