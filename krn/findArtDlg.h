#ifndef findArtDlg_included
#define findArtDlg_included

#include <qwidget.h>

#include "findArtDlgData.h"

class findArtDlg : public QWidget,
    private findArtDlgData
{
    Q_OBJECT
        
public:
    
    findArtDlg
        (
         QWidget* parent = NULL,
         const char* name = NULL
        );
    
    virtual ~findArtDlg();

signals:
    void FindThis(const char *,const char *);
    
private slots:
    void close();
    void find();
    
};
#endif // findArtDlg_included
