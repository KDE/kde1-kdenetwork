
package PBase;
use Carp;
use Data::Dumper;
use strict;

$PBase::NO_WIDGET = -1;

sub sendMessage {
  my $self = shift;
  
  my %ARG = @_;

  if($self->runable() == 0){
    $self->delayMessage(%ARG);
    return;
  }

  $ARG{"iWinId"} = $self->{iWinId} if($ARG{"iWinId"} == undef);
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
  my $class = shift;
  my $parent = $_[$#_];

  my $self  = {};


#  print "Parent: $parent\n";

  bless($self, $class);

  $parent = 0 if($parent == undef);

  $self->{iWinId} = -1;
  $self->{Parent} = $parent if $parent != 0;
  $self->{initId} = $self->rndchr();
  $self->{widgetType} = $PBase::NO_WIDGET;
  $self->{messageQueue} = ();

  $self->installHandler($::PUKE_WIDGET_DELETE_ACK, sub{$self->DESTROY});
  
  return $self;

}

sub create {
  my $self = shift;
  

#  if($self->{widgetType} == $PBase::NO_WIDGET){
#    print("*E* PBase: No Widget type set!!! Using PWidget\n");
#    $self->{widgetType} = $::PUKE_WIDGET_WIDGET;
#  }
  if($self->{widgetType} == undef ||
     $self->{widgetType} == $PBase::NO_WIDGET) {
    print("*E* PBase: Widget type was undefined, $self is really broken\n");
    print("*E* PBase: Giving up\n");
    return;
  }

  # If the parent doesn't have it's iWinId set, it's still being
  # created, So add ourselves to _*it's*_ canRun list.  Assuming it's not
  # runable, which it shouldn't be if it's iWinId is -1, it'll call
  # our creator after it get's it's iWinId set.

  if($self->{Parent}->{iWinId} == -1){
    $self->{Parent}->canRun($self, \&PBase::create, \@_) || return;
  }

#  print "*I* Createing widget of type: " . $self->{widgetType} . " with parent " . $self->{Parent}->{iWinId} . "\n";

  $::PUKE_CREATOR{$self->{initId}} =  sub{$self->ackWinId(@_);};

  #$self->setRunable(1);
  $self->{runable} = 1;

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_CREATE,
		     'iArg' => $self->{widgetType},
		     'iWinId' => $self->{Parent}->{iWinId},
		     'cArg' => $self->{initId},
		     'CallBack' => sub { });

  $self->setRunable(0);
}

sub DESTROY {
  my $self = shift;

  print "*I* Widget Deleted\n";
  $self->hide();

  $self->setRunable(1);

  if($self->{DESTROYED} != 1){
    $self->sendMessage('iCommand' => $::PUKE_WIDGET_DELETE,
		       'CallBack' => sub { print "Deleted\n"; });
  }

  $self->setRunable(0);
  $self->{iWinId} = -1;
  $self->{DESTROYED} = 1;

}

sub close {
  my $self = shift;
  
  $self->hide();

  $self->DESTROY;
  
}

sub runable {
  my $self = shift;
  return $self->{runable};
}

sub setRunable {
  my $self = shift;

  my $run = shift;

  if($run == 0){
    $self->{runable} = 0;
  }
  else {
    $self->{runable} = 1;
    $self->runDelayed();
  }
}

sub delayMessage {
  my $self = shift;
  my %ARG = @_;


  my $i = $#{$self->{messageQueue}} + 1;
#  if($i = -1){ $i = 0 }

  $self->{messageQueue}->[$i] = \%ARG;

#  print "*I* Delaying " . $#{$self->{messageQueue}} . " messages\n";

#  print Dumper($self);


}

sub runDelayed {
  my $self = shift;

  # Run the commandQueue first since this will run the constructors of
  # sub widgets.

  for(my $i=0; $i <= $#{$self->{commandQueue}}; $i++) {
    my $obj = $self->{commandQueueObj}->[$i];
    my $ARG = $self->{commandQueueArgs}->[$i];
    &{$self->{commandQueue}->[$i]}($obj, @$ARG);
  }
  $self->{commandQueueObj} = ();
  $self->{commandQueue} = ();
  $self->{commandQueueArgs} = ();

  for(my $i=0; $i <= $#{$self->{messageQueue}}; $i++) {
    my %ARG = %{$self->{messageQueue}->[$i]};
    $self->sendMessage(%ARG);
  }
  $self->{messageQueue} = ();

}

sub ackWinId {
  my $self = shift;
  my %ARG = @_;

#  print "*E* ackWinId called, got args\n" . Dumper(\%ARG);


  $self->{iWinId} = $ARG{'iWinId'};
  $self->setRunable(1);
  $self->runDelayed();
}

sub canRun {
  my $self = shift;

  if($self->runable() == 1){
    return 1;
  }

  # we can't process the command now, so delay it.
  my $i = $#{$self->{commandQueue}} + 1;

  $self->{commandQueueObj}->[$i] = shift;
  $self->{commandQueue}->[$i] = shift;
  $self->{commandQueueArgs}->[$i] = shift;

  return 0;

}

sub installHandler {
  my $self = shift;

  my @ARG = @_;
  $self->canRun($self, \&PBase::installHandler, \@ARG) || return;

  my $command = shift;
  my $handler = shift;

  $::PUKE_W_HANDLER{$command}{$self->{iWinId}} = $handler;

#  print Dumper($::PUKE_W_HANDLER{$command});

#  print "*I* Installed for $self->{iWinId}, $command handler to $handler\n";
}

sub onNext {
  my $self = shift;

  my $cb = shift;
  
  $self->sendMessage('iCommand' => $::PUKE_ECHO,
		     'iArg' => 0,
		     'iWinId' => $self->{iWinId},
		     'cArg' => "",
		     'CallBack' => $cb);  
}

package main;

1;
