/**********************************************************************

	--- Dlgedit generated file ---

	File: PostDialog.h
	Last generated: Fri Jul 11 21:12:45 1997

 *********************************************************************/

#ifndef PostDialog_included
#define PostDialog_included

#include "PostDialogData.h"
#include <qtooltip.h>
#include "ktempfile.h"

struct messagePart
{
    QString name;
    QString type;
    QString encoding;
};

class PostDialog : public PostDialogData
{
    Q_OBJECT

public:

    PostDialog
    (
        QString defGroup,
        QString defSubject,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PostDialog();
    void edit(char* type, char* name);

private slots:
    virtual void editPart(int);
    virtual void editPart(const char* f);
    virtual void propsPart();
    virtual void addPart();
    virtual void postLater();
    virtual void showHelp();
    virtual void removePart();
    virtual void editPart();
    virtual void select(int);
    virtual void cancel();
    virtual void postNow();

private:
    void enableButtons(bool);
    QList<messagePart> part;
    KTempFile* temp;
    QToolTipGroup* tip;
};
#endif // PostDialog_included
