#use lib "/opt/kde/share/apps/ksirc";

use pbase;
use pwidget;
use pframe;
use ptablevw;
use plistbox;
use pbutton;
use pboxlayout;
use plabel;

package DCCStatus;
@ISA = qw(PFrame);
use strict;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  my($gm_main, $gm_a, $lb_get, $lb_send);
  $gm_main = new PBoxLayout($self, $PBoxLayout::TopToBottom, 5);
  $gm_a = new PBoxLayout($gm_main, $PBoxLayout::LeftToRight, 5);       
  $gm_main->addLayout($gm_a);

  #  my $gm_aa = new PBoxLayout($gm_a, $PBoxLayout::TopToBottom, 5);
  #  $gm_a->addLayout($gm_aa);
  my $label = new PLabel($self);
  $label->setText("Pending Gets");
  $label->setMaximumSize(30,1000);
  $gm_a->addWidget($label, 5, $PBoxLayout::AlignCenter);
  
  $lb_get = new PListBox($self);
  $gm_a->addWidget($lb_get, 5, $PBoxLayout::AlignCenter);

  my $label2 = new PLabel($self);
  $label2->setText("Pending Chats");
  $label2->setMaximumSize(30,1000);
  $gm_main->addWidget($label2, 5, $PBoxLayout::AlignCenter);
  $lb_send = new PListBox($self);
  $gm_main->addWidget($lb_send, 5, $PBoxLayout::AlignCenter);

  $self->create();

  return $self;
}

package main;

$::test = new DCCStatus;
$::test->onNext(sub { $::test->show()} );

1;