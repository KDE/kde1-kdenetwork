#ifndef RULES_DLG_H
#define RULES_DLG_H

#include <qdialog.h>

class QPushButton;
class QComboBox;
class QCheckBox;
class QLineEdit;

class rulesDlg : public QDialog
{
    Q_OBJECT
        
public:
    
    rulesDlg();
    
    virtual ~rulesDlg();

};

#endif