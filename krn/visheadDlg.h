#ifndef visheadDlg_included
#define visheadDlg_included

#include <qdialog.h>

#include "visheadDlgData.h"

class visheadDlg : public QDialog,
			private visheadDlgData
{
    Q_OBJECT

public:

    visheadDlg
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~visheadDlg();

};
#endif // visheadDlg_included
