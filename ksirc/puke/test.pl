&docommand("/load pbase.pm");
&docommand("/load pwidget.pm");
&docommand("/load pframe.pm");
&docommand("/load pboxlayout.pm");
&docommand("/load plined.pm");

$pw = new PWidget;
$pw->resize(250,500);

$pf = new PFrame($pw);
$pf->setFrameStyle($PFrame::Panel|$PFrame::Sunken, 1);
$pf->move(50,50);
$pf->resize(150,400);

$pf2 = new PFrame($pw);
$pf2->setFrameStyle($PFrame::Box|$PFrame::Raised, 1);
$pf2->move(50,50);
$pf2->resize(50,300);

$pb = new PBoxLayout($pw, $PBoxLayout::TopToBottom, 5);
$pb->addWidget($pf, 10); 
$pb->addWidget($pf2, 5);

$pl = new PLineEdit($pw);
$pl->setMinimumSize(35,200);
$pl->setMaximumSize(35,5000);
$pl->setText("Blah blah");
#$pl->resize(50,75);
$pb->addWidget($pl, 10, $PBoxLayout::AlignLeft);

$pw->onNext(sub{$pw->show()});
#$pf->show();
#$pf2->show();

