#include <qapp.h> 
#include <qsocknot.h>
#include "ksticker.h"

class StdInTicker : public KSTicker
{
  Q_OBJECT
public:
  StdInTicker();
  ~StdInTicker();

protected:
  void closeEvent ( QCloseEvent * );

public slots:
void readsocket(int socket);
  
};
