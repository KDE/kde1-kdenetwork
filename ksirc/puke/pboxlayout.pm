
package PBoxLayout;
@ISA = qw(PBase);
use strict;

# setup default handlers

$PBoxLayout::LeftToRight = 0;
$PBoxLayout::RightToLeft = 1;
$PBoxLayout::TopToBottom = 2;
$PBoxLayout::BottomToTop = 3;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  my $widget = shift;

  print "Widget: " . ref($widget) . "\n";

  if(ref($widget)){
    $self->{Parent} = $widget;
    $self->{Direction} = shift;
    $self->{Border} = shift;
  }
  else {
    $self->{Parent} = {};
    $self->{Direction} = $widget;
    $self->{Border} = shift;    
  }

  $self->{IAmALayout} = 1;

  $self->create();

  return $self;

}

sub create {
  my $self = shift;
  
  # If the parent doesn't have it's iWinId set, it's still being
  # created, So add ourselves to _*it's*_ canRun list.  Assuming it's not
  # runable, which it shouldn't be if it's iWinId is -1, it'll call
  # our creator after it get's it's iWinId set.

  if($self->{Parent}->{iWinId} == -1){
    $self->{Parent}->canRun($self, \&PBoxLayout::create, \@_) || return;
  }

  print "*I* Createing Layout with parent " . $self->{Parent}->{iWinId} . "\n";

  $::PUKE_CREATOR{$self->{initId}} =  sub{$self->ackWinId(@_);};

  $self->{runable} = 1;
  $self->sendMessage('iCommand' => $::PUKE_LAYOUT_NEW,
		     'iWinId' => $self->{Parent}->{iWinId},
		     'cArg' => $self->{initId},
		     'iArg' => $self->{Direction} + 65536 * $self->{Border},
		     'CallBack' => sub { });
  $self->setRunable(0);
}

sub addWidget {
  my $self = shift;

  my $widget = shift;

  # make sure we can run, and the widget we want to add can run.
  my @ARG = ($widget);
  $self->canRun($self, \&PBoxLayout::addWidget, \@ARG) || return;
  $widget->canRun($self, \&PBoxLayout::addWidget, \@ARG) || return;
  
  print "*I* Sending add arg for id: " . $widget->{iWinId} . "\n";

  $self->sendMessage('iCommand' => $::PUKE_LAYOUT_ADDWIDGET,
		     'iWinId' => $self->{iWinId},
		     'iArg' => $widget->{iWinId});
#		     'CallBack' => sub { });

  
}

sub addLayout {
  my $self = shift;

  my $layout = shift;

  # make sure we can run, and the widget we want to add can run.
  my @ARG = ($layout);
  $self->canRun($self, \&PBoxLayout::addLayout, \@ARG) || return;
  $layout->canRun($self, \&PBoxLayout::addWidget, \@ARG) || return;

  $self->sendMessage('iCommand' => $::PUKE_LAYOUT_ADDLAYOUT,
		     'iWinId' => $self->{iWinId},
		     'iArg' => $layout->{iWinId},
		     'CallBack' => sub { });

  
}

package main;
