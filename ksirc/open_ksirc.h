/**********************************************************************

	--- Qt Architect generated file ---

	File: open_ksirc.h
	Last generated: Wed Nov 26 22:18:02 1997

 *********************************************************************/

#ifndef open_ksirc_included
#define open_ksirc_included

#include "open_ksircData.h"
#include "iocontroller.h"

class open_ksirc : public open_ksircData
{
    Q_OBJECT

public:

    open_ksirc
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~open_ksirc();

signals:
    void open_ksircprocess(QString);

protected slots:
    virtual void create_toplevel();
    void terminate()
      {
        reject();
      }

private:

};
#endif // open_ksirc_included
