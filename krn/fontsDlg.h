#ifndef fontsDlg_included
#define fontsDlg_included

#include <qdialog.h>

#include "fontsDlgData.h"

class fontsDlg : public QDialog, private fontsDlgData
{
    Q_OBJECT

public:

    fontsDlg
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~fontsDlg();

private slots:
    void save();
    void syncFonts(int);

};
#endif // fontsDlg_included
