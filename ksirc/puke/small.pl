&docommand("/load pbase.pm");
&docommand("/load pwidget.pm");
&docommand("/load pframe.pm");
&docommand("/load pboxlayout.pm");
&docommand("/load plined.pm");
&docommand("/load pbutton.pm");
&docommand("/load ppushbt.pm");
&docommand("/load pprogress.pm");
&docommand("/load ptablevw.pm");
&docommand("/load plistbox.pm");

$pw = new PWidget;
$pw->resize(250,500);

$pf = new PFrame($pw);
$pf->setFrameStyle($PFrame::Panel|$PFrame::Sunken, 1);
$pf->move(50,50);
$pf->resize(150,400);

$pf2 = new PFrame($pw);
$pf2->setFrameStyle($PFrame::Panel|$PFrame::Raised, 1);


$pl = new PListBox($pw);
$pl->move(50,50);
$pl->resize(50,300);

$pb = new PBoxLayout($pw, $PBoxLayout::TopToBottom, 5);
$pb2 = new PBoxLayout($PBoxLayout::LeftToRight, 5);
$pb3 = new PBoxLayout($PBoxLayout::TopToBottom, 5);
$pb->addLayout($pb2);
$pb->addLayout($pb3);
$pb2->addWidget($pf, 10, $PBoxLayout::AlignCenter);
$pb2->addWidget($pl, 10, $PBoxLayout::AlignCenter);

$pb4 = new PBoxLayout($PBoxLayout::RightToLeft, 1);
$pb3->addLayout($pb4);

$pb4->addWidget($pf2, 0, $PBoxLayout::AlignCenter);

$pbutton = new PPushButton($pw);
$pbutton->setText("Hello");
$pbutton->setPixmap("/opt/kde/share/icons/ksirc.gif");
$pubtton->setMaximumSize(50,50);
$pb4->addWidget($pbutton, 0, $PBoxLayout::AlignCenter);

$pline = new PLineEdit($pw);
$pline->setMinimumSize(30,50);
$pline->setMaximumSize(30,1000);
$pline->setText("Hello!");
$pb->addWidget($pline, 0);

$pw->show;