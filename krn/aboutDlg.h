#ifndef aboutDlg_included
#define aboutDlg_included

#include <qdialog.h>

#include "aboutDlgData.h"

class aboutDlg : public QDialog,
			private aboutDlgData
{
    Q_OBJECT

public:

    aboutDlg
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~aboutDlg();

};
#endif // aboutDlg_included
