#ifndef IdentDlg_included
#define IdentDlg_included

#include <qdialog.h>

#include "identDlgData.h"

class IdentDlg : public QDialog,
			public identDlgData
{
    Q_OBJECT

public:

    IdentDlg
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~IdentDlg();

};
#endif // IdentDlg_included
