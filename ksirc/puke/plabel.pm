
&::PukeSendMessage($PUKE_WIDGET_LOAD, 
		   0, 
		   $PWIDGET_LABEL,
		   "plabel.so",
		   sub { my %ARG = %{shift()};
			 if($ARG{'iArg'} == 1){
			   print "*E* PLabel Load failed!\n";
			 }
		       }
		  );

package PLabel;
@ISA = qw(PFrame);
use strict;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->{widgetType} = $::PWIDGET_LABEL;

  if($class eq 'PLabel'){
    $self->create();
  }

  return $self;

}

sub setText {
  my $self = shift;
  
  my $text = shift;

  if(length($text) > 49){
    print "*E* Puke Line Edit: set text with over 49 characters.\n";
    $text =~ s/^(.{49,49}).*/$1/;
  }
  
  $self->{text} = $text;

  # Don't need the ouput since GET_TEXT_ACK will be called and
  # we'll set it there
  $self->sendMessage('iCommand' => $::PUKE_LABEL_SETTEXT,
		     'cArg' => $text,
		     'CallBack' => sub {});

}

sub setPixmap {
  my $self = shift;
  
  my $text = shift;

  if(length($text) > 49){
    print "*E* Puke Line Edit: set text with over 49 characters.\n";
    $text =~ s/^(.{49,49}).*/$1/;
  }
  
  $self->{text} = "***PIXMAP***" . $text;

  # Don't need the ouput since GET_TEXT_ACK will be called and
  # we'll set it there
  $self->sendMessage('iCommand' => $::PUKE_LABEL_SETPIXMAP,
		     'cArg' => $text,
		     'CallBack' => sub {});

}

sub setMovie {
  my $self = shift;
  
  my $text = shift;

  if(length($text) > 49){
    print "*E* Puke Line Edit: set text with over 49 characters.\n";
    $text =~ s/^(.{49,49}).*/$1/;
  }
  
  $self->{text} = "***MOVIE***" . $text;

  # Don't need the ouput since GET_TEXT_ACK will be called and
  # we'll set it there
  $self->sendMessage('iCommand' => $::PUKE_LABEL_SETMOVIE,
		     'cArg' => $text,
		     'CallBack' => sub {});

}


sub text {
  my $self = shift;

  return $self->{text};
}



sub setAlignment {
  my $self = shift;
  
  my $align = shift;

  
  $self->{align} = $align;

  # Don't need the ouput since GET_TEXT_ACK will be called and
  # we'll set it there
  $self->sendMessage('iCommand' => $::PUKE_LABEL_SETALIGNMENT,
		     'iArg' => $align,
		     'CallBack' => sub {});

}



package main;

