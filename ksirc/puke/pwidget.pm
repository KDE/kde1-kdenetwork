
package PWidget;
@ISA = qw(PBase);
use strict;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->{widgetType} = $::PWIDGET_WIDGET;

  # Default handlers
  $self->installHandler($::PUKE_WIDGET_EVENT_RESIZE, 
			sub {$self->resizeEvent(@_)});
  $self->installHandler($::PUKE_WIDGET_EVENT_PAINT, 
			sub {$self->paintEvent(@_)});
  $self->installHandler($::PUKE_WIDGET_EVENT_MOVE, 
			sub {$self->moveEvent(@_)});
  $self->installHandler($::PUKE_EVENT_UNKOWN, 
			sub {$self->miscEvent(@_)});


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

sub setMinimumSize {
  my $self = shift;

  my $w = shift;
  my $h = shift;

  my $iarg = $w * 2**16 + $h;

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_SETMINSIZE,
		     'iArg' => $iarg,
		     'CallBack' => sub {});

}

sub setMaximumSize {
  my $self = shift;

  my $w = shift;
  my $h = shift;

  my $iarg = $w * 2**16 + $h;

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_SETMAXSIZE,
		     'iArg' => $iarg,
		     'CallBack' => sub {});

}


sub setCaption {
  my $self = shift;

  my $text = shift;
  
  $self->sendMessage('iCommand' => $::PUKE_WIDGET_SETCAPTION,
		     'cArg' => $text,
		     'CallBack' => sub {});

}

sub resizeEvent {
  my $self = shift;

  my %ARG = %{shift()};

  my($h, $w, $oh, $ow) = unpack("iiii", $ARG{'cArg'});
  $self->{'height'} = $h;
  $self->{'width'} = $w;

}

sub paintEvent {
}

sub moveEvent {
  my $self = shift;

  my %ARG = %{shift()};

  my($x, $y, $ox, $oy) = unpack("iiii", $ARG{'cArg'});
  $self->{'x'} = $x;
  $self->{'y'} = $y;
  
}

sub miscEvent {
}

package main;

1;
