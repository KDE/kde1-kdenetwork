/**********************************************************************

	--- Dlgedit generated file ---

	File: NewMsgDlg.h
	Last generated: Fri Jul 11 20:18:40 1997

 *********************************************************************/

#ifndef NewMsgDlg_included
#define NewMsgDlg_included

#include "NewMsgDlgData.h"
#include "PostDialog.h"

class NewMsgDlg : public NewMsgDlgData
{
    Q_OBJECT

public:

    NewMsgDlg
    (
        messagePart *p,
        KTempFile *temp,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~NewMsgDlg();

protected slots:
    virtual void medit(int);
    virtual void medit();
    virtual void cancel();
    virtual void read();
            
signals:
    void edit(const char*);

private:
    messagePart* part;
    KTempFile* t;
};
#endif // NewMsgDlg_included
