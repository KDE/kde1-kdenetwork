/**********************************************************************

	--- Qt Architect generated file ---

	File: defaultfiltersdata.cpp
	Last generated: Tue Jul 28 02:30:57 1998

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include "defaultfiltersdata.h"

#define Inherited QFrame

#include <qlabel.h>
#include <qbttngrp.h>

defaultfiltersdata::defaultfiltersdata
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, 0 )
{
	QButtonGroup* dlgedit_ButtonGroup_4;
	dlgedit_ButtonGroup_4 = new("QButtonGroup") QButtonGroup( this, "ButtonGroup_4" );
	dlgedit_ButtonGroup_4->setGeometry( 15, 240, 370, 55 );
	dlgedit_ButtonGroup_4->setMinimumSize( 10, 10 );
	dlgedit_ButtonGroup_4->setMaximumSize( 32767, 32767 );
	dlgedit_ButtonGroup_4->setFrameStyle( 49 );
	dlgedit_ButtonGroup_4->setTitle( "Highlight Your Nick" );
	dlgedit_ButtonGroup_4->setAlignment( 1 );

	QButtonGroup* dlgedit_ButtonGroup_2;
	dlgedit_ButtonGroup_2 = new("QButtonGroup") QButtonGroup( this, "ButtonGroup_2" );
	dlgedit_ButtonGroup_2->setGeometry( 15, 115, 370, 120 );
	dlgedit_ButtonGroup_2->setMinimumSize( 10, 10 );
	dlgedit_ButtonGroup_2->setMaximumSize( 32767, 32767 );
	dlgedit_ButtonGroup_2->setFrameStyle( 49 );
	dlgedit_ButtonGroup_2->setTitle( "Colour Nicks in Channels" );
	dlgedit_ButtonGroup_2->setAlignment( 1 );

	QButtonGroup* dlgedit_ButtonGroup_1;
	dlgedit_ButtonGroup_1 = new("QButtonGroup") QButtonGroup( this, "ButtonGroup_1" );
	dlgedit_ButtonGroup_1->setGeometry( 15, 5, 370, 105 );
	dlgedit_ButtonGroup_1->setMinimumSize( 10, 10 );
	dlgedit_ButtonGroup_1->setMaximumSize( 32767, 32767 );
	dlgedit_ButtonGroup_1->setFrameStyle( 49 );
	dlgedit_ButtonGroup_1->setTitle( "Allow External Colours" );
	dlgedit_ButtonGroup_1->setAlignment( 1 );

	kSircColours = new("QCheckBox") QCheckBox( this, "CheckBox_1" );
	kSircColours->setGeometry( 110, 30, 170, 30 );
	kSircColours->setMinimumSize( 10, 10 );
	kSircColours->setMaximumSize( 32767, 32767 );
	kSircColours->setText( "Allow &kSirc Colour codes" );
	kSircColours->setAutoRepeat( FALSE );
	kSircColours->setAutoResize( FALSE );
	kSircColours->setChecked( TRUE );

	mircColours = new("QCheckBox") QCheckBox( this, "CheckBox_2" );
	mircColours->setGeometry( 110, 60, 170, 30 );
	mircColours->setMinimumSize( 10, 10 );
	mircColours->setMaximumSize( 32767, 32767 );
	mircColours->setText( "&Allow &mirc Colour codes" );
	mircColours->setAutoRepeat( FALSE );
	mircColours->setAutoResize( FALSE );
	mircColours->setChecked( TRUE );

	LabelNickB = new("QLabel") QLabel( this, "Label_3" );
	LabelNickB->setGeometry( 35, 185, 100, 30 );
	LabelNickB->setMinimumSize( 10, 10 );
	LabelNickB->setMaximumSize( 32767, 32767 );
	LabelNickB->setText( "Background" );
	LabelNickB->setAlignment( 290 );
	LabelNickB->setMargin( -1 );

	QLabel* dlgedit_Label_5;
	dlgedit_Label_5 = new("QLabel") QLabel( this, "Label_5" );
	dlgedit_Label_5->setGeometry( 35, 145, 100, 30 );
	dlgedit_Label_5->setMinimumSize( 10, 10 );
	dlgedit_Label_5->setMaximumSize( 32767, 32767 );
	dlgedit_Label_5->setText( "Foreground" );
	dlgedit_Label_5->setAlignment( 290 );
	dlgedit_Label_5->setMargin( -1 );

	QLabel* dlgedit_Label_6;
	dlgedit_Label_6 = new("QLabel") QLabel( this, "Label_6" );
	dlgedit_Label_6->setGeometry( 35, 255, 100, 30 );
	dlgedit_Label_6->setMinimumSize( 10, 10 );
	dlgedit_Label_6->setMaximumSize( 32767, 32767 );
	dlgedit_Label_6->setText( "Colour" );
	dlgedit_Label_6->setAlignment( 290 );
	dlgedit_Label_6->setMargin( -1 );

	SLE_SampleColourNick = new("QLineEdit") QLineEdit( this, "LineEdit_1" );
	SLE_SampleColourNick->setGeometry( 285, 165, 50, 30 );
	SLE_SampleColourNick->setMinimumSize( 10, 10 );
	SLE_SampleColourNick->setMaximumSize( 32767, 32767 );
	SLE_SampleColourNick->setText( "Sample" );
	SLE_SampleColourNick->setMaxLength( 32767 );
	SLE_SampleColourNick->setEchoMode( QLineEdit::Normal );
	SLE_SampleColourNick->setFrame( TRUE );

	SLE_SampleHighlight = new("QLineEdit") QLineEdit( this, "LineEdit_2" );
	SLE_SampleHighlight->setGeometry( 285, 255, 50, 30 );
	SLE_SampleHighlight->setMinimumSize( 10, 10 );
	SLE_SampleHighlight->setMaximumSize( 32767, 32767 );
	SLE_SampleHighlight->setText( "Sample" );
	SLE_SampleHighlight->setMaxLength( 32767 );
	SLE_SampleHighlight->setEchoMode( QLineEdit::Normal );
	SLE_SampleHighlight->setFrame( TRUE );

	usHighlightColour = new("QComboBox") QComboBox( FALSE, this, "ComboBox_1" );
	usHighlightColour->setGeometry( 150, 255, 100, 30 );
	usHighlightColour->setMinimumSize( 10, 10 );
	usHighlightColour->setMaximumSize( 32767, 32767 );
	usHighlightColour->setSizeLimit( 17 );
	usHighlightColour->setAutoResize( FALSE );

	NickFColour = new("QComboBox") QComboBox( FALSE, this, "ComboBox_2" );
	NickFColour->setGeometry( 150, 145, 100, 30 );
	NickFColour->setMinimumSize( 10, 10 );
	NickFColour->setMaximumSize( 32767, 32767 );
	NickFColour->setSizeLimit( 17 );
	NickFColour->setAutoResize( FALSE );

	NickBColour = new("QComboBox") QComboBox( FALSE, this, "ComboBox_3" );
	NickBColour->setGeometry( 150, 185, 100, 30 );
	NickBColour->setMinimumSize( 10, 10 );
	NickBColour->setMaximumSize( 32767, 32767 );
	NickBColour->setSizeLimit( 17 );
	NickBColour->setAutoResize( FALSE );



	dlgedit_ButtonGroup_1->insert( kSircColours );
	dlgedit_ButtonGroup_1->insert( mircColours );

	resize( 400,300 );
	setMinimumSize( 400, 300 );
	setMaximumSize( 400, 300 );
}


defaultfiltersdata::~defaultfiltersdata()
{
}
