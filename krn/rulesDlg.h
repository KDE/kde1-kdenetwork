#ifndef RULES_DLG_H
#define RULES_DLG_H

#include <qwidget.h>

class QPushButton;
class QComboBox;
class QCheckBox;
class QLineEdit;

class rulesDlg : public QWidget
{
    Q_OBJECT
        
public:
    
    rulesDlg();
    
    virtual ~rulesDlg();

};

#endif