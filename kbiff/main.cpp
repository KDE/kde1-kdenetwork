#include "kbiff.h"
#include <kapp.h>

#include "setupdlg.h"

int main(int argc, char *argv[])
{
	KApplication app(argc, argv, "kbiff");
	KBiff kbiff;
	KBiffSetup setup;

	if (setup.exec())
	{
		kbiff.setMailboxList(setup.getMailboxList());
	}
	else
		return 0;

	app.setMainWidget(&kbiff);
	app.setTopWidget(&kbiff);

	kbiff.show();

	return app.exec();
}
