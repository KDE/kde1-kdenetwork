
&::PukeSendMessage($PUKE_WIDGET_LOAD, 
		   0, 
		   $PWIDGET_BUTTON,
		   "pbutton.so",
		   sub { my %ARG = %{shift()};
			 if($ARG{'iArg'} == 1){
			   print "*E* PButton Load failed!\n";
			 }
		       }
		  );

package PButton;
@ISA = qw(PWidget);
use strict;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->{widgetType} = $::PWIDGET_BUTTON;

  if($class eq 'PButton'){
    $self->create();
  }

  $self->installHandler($::PUKE_BUTTON_PRESSED_ACK, sub{});
  $self->installHandler($::PUKE_BUTTON_RELEASED_ACK, sub{});
  $self->installHandler($::PUKE_BUTTON_CLICKED_ACK, sub{});
  $self->installHandler($::PUKE_BUTTON_TOGGLED_ACK, sub{});

  return $self;

}

sub setText {
  my $self = shift;

  my $text = shift;

  $self->sendMessage('iCommand' => $::PUKE_BUTTON_SET_TEXT,
		     'cArg' => $text,
		     'CallBack' => 
		     sub {
		       my %ARG = %{shift()};
		       $self->{text} = $ARG{'cArg'};
		     }
		    );
}

sub setPixmap {
  my $self = shift;

  my $file = shift;

  $self->sendMessage('iCommand' => $::PUKE_BUTTON_SET_PIXMAP,
		     'cArg' => $file,
		     'CallBack' => 
                     sub {
                       my $arg = shift();
                       #                       if($arg == undef){ return; }
                       #my %ARG = %{shift()};
                       #if($ARG{'iArg'} == 0){
                       #print "*E* Pixmap set failed\n";
                         #}
		     }
		    );
}

sub setAutoResize {
  my $self = shift;

  my $resize = shift;

  $self->sendMessage('iCommand' => $::PUKE_BUTTON_SET_PIXMAP,
		     'iArg' => $resize,
		     'CallBack' =>  sub {}
		    );
}

package main;
