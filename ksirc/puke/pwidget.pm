
package PWidget;
@ISA = qw(PBase);
use strict;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->{widgetType} = $::PWIDGET_WIDGET;

  if($class eq 'PWidget'){
    $self->create();
  }

  return $self;

}

sub close {
  my $self = shift;
  
  $self->hide();

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_DELETE,
		    'CallBack' => sub {});

  $self->{DESTROYED} = 1;
  
}

sub show {
  my $self = shift;

  # make sure we are runable then show continue.
  my @ARG = ();
  $self->canRun($self, \&PWidget::show, \@ARG) || return;

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_SHOW);
}

sub hide {
  my $self = shift;
  $self->sendMessage('iCommand' => $::PUKE_WIDGET_HIDE);
}

sub repaint {
  my $self = shift;
  
  my $erase = shift; # 1 for erase and reapint

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_REPAINT,
		     'iArg' => $erase);
}

sub resize {
  my $self = shift;

  my $width = shift;
  my $height = shift;

  my $size = $height * 2**16 + $width;

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_RESIZE,
		     'iArg' => $size);

}

sub move {
  my $self = shift;

  my $x = shift;
  my $y = shift;

  my $pos = $y * 2**16 + $x;

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_MOVE,
		     'iArg' => $pos);

}

package main;

1;
