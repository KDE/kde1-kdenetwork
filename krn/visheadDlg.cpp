#include "visheadDlg.h"

#define Inherited QDialog

visheadDlg::visheadDlg
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE ),
	visheadDlgData( this )
{
	setCaption( "Visible Headers" );
}


visheadDlg::~visheadDlg()
{
}
