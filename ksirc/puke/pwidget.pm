
package PWidget;
use strict;


sub sendMessage {
  my $self = shift;
  
  my %ARG = @_;


#  if($ARG{"CallBack"} == undef){
#    $ARG{"CallBack"} = sub {};
#  }

  $ARG{"iWinId"} = $self->{iWinId} if($ARG{"iWinId"} == undef);

  print("$ARG{iCommand}, $self->{iWinId}, $ARG{iArg}, $ARG{cArg}, $ARG{CallBack}\n");

  &::PukeSendMessage($ARG{"iCommand"}, 
		     $ARG{"iWinId"}, 
		     $ARG{"iArg"},
		     $ARG{"cArg"},
		     $ARG{"CallBack"});
}

sub rndchr {
  my $string = "";
  for(my $i = 0; $i < 8; $i++){
    $string .= chr(int(rand(94)) + 0x20);
  }
  return $string;
}

sub new {
  my $proto = shift;
  my $class = ref($proto) || $proto;
  my $self  = {};

  my $parent = shift;


  $parent = 0 if($parent == undef);

  $self->{iWinId} = -1;
  $self->{Parent} = $parent->{iWinId} if $parent != undef;

  bless($self, $class);
  
  my $string = $self->rndchr();

  $::PUKE_CREATOR{$string} = sub {$self->{iWinId} = ${_[0]}{iWinId}; };

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_CREATE,
		     'iArg' => $::PWIDGET_WIDGET,
		     'iWinId' => $self->{Parent},
		     'cArg' => $string,
		     'CallBack' => sub { });

  return $self;

}

sub DESTROY {
  my $self = shift;

  if($self->{DESTROYED} != 1){
    $self->sendMessage('iCommand' => $::PUKE_WIDGET_DELETE,
		       'CallBack' => sub { print "Deleted\n"; });
  }
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
