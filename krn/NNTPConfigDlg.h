#ifndef NNTPConfigDlg_included
#define NNTPConfigDlg_included

#include <qdialog.h>

#include "NNTPConfigDlgData.h"

class NNTPConfigDlg : public QDialog,
			public NNTPConfigDlgData
{
    Q_OBJECT

public:

    NNTPConfigDlg
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~NNTPConfigDlg();

};
#endif // NNTPConfigDlg_included
