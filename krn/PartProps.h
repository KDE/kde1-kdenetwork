/**********************************************************************

	--- Dlgedit generated file ---

	File: PartProps.h
	Last generated: Fri Jul 11 22:21:38 1997

 *********************************************************************/

#ifndef PartProps_included
#define PartProps_included

#include "PartPropsData.h"
#include "PostDialog.h"

class PartProps : public PartPropsData
{
    Q_OBJECT

public:

    PartProps
    (
	messagePart* p,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PartProps();

protected slots:
    virtual void ok();
    virtual void cancel();
        
};
#endif // PartProps_included
