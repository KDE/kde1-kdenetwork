#include "kbiff.h"
#include <kwm.h>
#include <kapp.h>

#include "setupdlg.h"
#include "Trace.h"

int main(int argc, char *argv[])
{
TRACEINIT("main()");
	KApplication app(argc, argv, "kbiff");
	KBiff kbiff;
	KBiffSetup* setup;
	bool have_profile = false;
	bool do_debug = false;

	app.setMainWidget(&kbiff);

	// parse the args for the -debug option
	if (argc == 2)
	{
		// do something with this later
		if (QString(argv[1]) == "-debug")
			do_debug = true;
	}

	// parse the args to see if there is the -profile option
	if (argc == 3)
	{
		// check if this is the argument we are looking for
		if (QString(argv[1]) == "-profile")
			have_profile = true;
	}      

	// restore this app if it is
	if (kapp->isRestored())
	{
		TRACE("isRestored()");
		kbiff.readSessionConfig();
	}
	else
	{
		TRACE("notRestored()");
		// do we have the profile option?
		if (have_profile)
			setup = new KBiffSetup(argv[2]);
		else
		{
			setup = new KBiffSetup();
			if (!setup->exec())
				return 0;
		}
		kbiff.processSetup(setup, true);
	}

	// check if we are docked (only if restored)
	if (kbiff.isDocked())
	{
		kapp->setTopWidget(new QWidget);
		KWM::setDockWindow(kbiff.winId());
	}
	else
		kapp->setTopWidget(&kbiff);

	// enable session management
	kapp->enableSessionManagement();

	kbiff.show();

	return app.exec();
}
