#ifndef findArtDlgData_included
#define findArtDlgData_included

#include <qcombo.h>
#include <qlined.h>
#include <qpushbt.h>

class findArtDlgData
{
public:
    
    findArtDlgData
        (
         QWidget* parent
        );
    
    
    QPushButton* b1;
    QPushButton* b2;
    QComboBox* field;
    QLineEdit* expr;
    
};

#endif // findArtDlgData_included
