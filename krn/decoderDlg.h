#ifndef decoderDlg_included
#define decoderDlg_included

#include <qdialog.h>

#include "ktablistbox.h"


class decoderDlg : public QDialog
{
    Q_OBJECT
        
public:
    
    decoderDlg( const char* name = NULL );
    
    virtual ~decoderDlg();
    KTabListBox *list;

private slots:
    void actions(int);
    
};
#endif // decoderDlg_included
