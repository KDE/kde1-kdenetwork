#ifndef sirccontrol_included
#define sirccontrol_included

/* The whole reason for this widget is to make KTopLevelWidget compatible
 * with the normal Qt API.
 *
 * Change: 
 *          Constructor: allows you to specify parent, though not used
 *
 */ 

#include "ktopwidget.h"

class KSircControl : public KTopLevelWidget
{
    Q_OBJECT

public:

    KSircControl
    (
        QWidget*,                     // KTopLevelWidget without parent
        const char* name = NULL,
	int
    ) : KTopLevelWidget(name)
      {
      }

    ~KSircControl();

public slots:


protected slots:


protected:

};

#endif // sirctoplevel
