#include "kbiff.h"
#include <kapp.h>

int main(int argc, char *argv[])
{
	KApplication app(argc, argv, "kbiff");
	KBiff widget;

	app.setMainWidget(&widget);
	app.setTopWidget(&widget);

	widget.show();

	return app.exec();
}
