#ifndef decoderDlg_included
#define decoderDlg_included

#include <qdialog.h>
#include <ktopwidget.h>

#include "ktablistbox.h"


class decoderDlg : public KTopLevelWidget
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
