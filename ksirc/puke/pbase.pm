
package PBase;
use Carp;
#use Data::Dumper;
use strict;

$PBase::NO_WIDGET = -1;

$::AlignLeft  = 0x0001;
$::AlignRight = 0x0002;
$::AlignHCenter = 0x0004;
$::AlignTop = 0x0008;
$::AlignBottom = 0x0010;
$::AlignVCenter = 0x0020;
$::AlignCenter = $::AlignVCenter | $::AlignHCenter;

sub sendMessage {
  my $self = shift;
  
  my %ARG = @_;
  $ARG{"iWinId"} = $self->{iWinId} if($ARG{"iWinId"} == undef);
  
  return &::PukeSendMessage($ARG{"iCommand"}, 
	                    $ARG{"iWinId"}, 
		            $ARG{"iArg"},
                            $ARG{"cArg"},
                            $ARG{"CallBack"},
                            $ARG{"WaitFor"}
                           );
}

sub rndchr {
  my $string = "";
  for(my $i = 0; $i < 8; $i++){
    $string .= chr(int(rand(93)) + 0x21);  # 0x21 since we don't want spaces and 0x20 is space.
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

  if($::PUKE_FETCH_WIDGET == 1) {
    $self->{Fetch} = 1;
  }

  #  $self->installHandler($::PUKE_WIDGET_DELETE_ACK, sub{$self->DESTROY});
  
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

  my $parent = $self->{Parent} ?  $self->{Parent}->{iWinId} : 0;
  
  #  print "*I* Createing widget of type: " . $self->{widgetType} . " with parent " . $parent . "\n";

  $self->{runable} = 1;

  my %REPLY = $self->sendMessage('iCommand' => $::PUKE_WIDGET_CREATE,
                                 'iArg' => $self->{widgetType} +  $parent * 2**16,
                                 'iWinId' => $::PUKE_CONTROLLER,
                                 'cArg' => $self->{initId},
                                 'CallBack' => sub { },
                                 'WaitFor' => 1);

    if($REPLY{iWinId} <= 0){
    print "*E* Widget Create Failed!\n";
    }

    $self->ackWinId(%REPLY);
    #  $self->setRunable(0);
}

sub fetchWidget {
  my $self = shift;

  $self->{objName} = shift;
  my $regex = shift;

#  $self->sendMessage('iCommand' => $::PUKE_WIDGET_DELETE,
#                     'CallBack' => sub { print "Deleted\n"; });

  my %REPLY = $self->sendMessage('iCommand' => $::PUKE_FETCHWIDGET,
                                 'iArg' => $self->{widgetType} +  $regex * 2**16,
                                 'iWinId' => $::PUKE_CONTROLLER,
                                 'cArg' => $self->{initId} . "\t" . $self->{objName} ,
                                 'CallBack' => sub { },
                                 'WaitFor' => 1);

  if($REPLY{iWinId} <= 0){
    print "*E* Widget Fetch Failed!\n";
  }

  $self->ackWinId(%REPLY);
  #  $self->setRunable(0);

}

sub treeInfo {
  my $self = shift;
  
  my %REPLY = $self->sendMessage('iCommand' => $::PUKE_DUMPTREE,
                                 'iWinId' => $::PUKE_CONTROLLER,
                                 'CallBack' => sub { },
                                 'WaitFor' => 0);

}


sub DESTROY {
  my $self = shift;

  print "*I* Widget Deleted\n";
  $self->hide();

  #  $self->setRunable(1);

  delete($::PBASE_IMORTALS{$self->{IMMORTAL}});

  if($self->{DESTROYED} != 1 && $self->{Parent} == 0){
        $self->sendMessage('iCommand' => $::PUKE_WIDGET_DELETE,
        	       'CallBack' => sub { print "Deleted\n"; });
  }

  #  $self->setRunable(0);
  $self->{iWinId} = -1;
  $self->{DESTROYED} = 1;

}

sub close {
  my $self = shift;
  
  $self->hide();

  $self->DESTROY;
  
}



#sub runable {
#  my $self = shift;
#  return $self->{runable};
#}
#
#sub setRunable {
#  my $self = shift;
#
#  my $run = shift;
#
#  if($run == 0){
#    $self->{runable} = 0;
#  }
#  else {
#    $self->{runable} = 1;
#    $self->runDelayed();
#  }
#}
#
#sub delayMessage {
#  my $self = shift;
#  my %ARG = @_;
#
#
#  my $i = $#{$self->{messageQueue}} + 1;
##  if($i = -1){ $i = 0 }
#
#  $self->{messageQueue}->[$i] = \%ARG;
#
##  print "*I* Delaying " . $#{$self->{messageQueue}} . " messages\n";
#
##  print Dumper($self);
#
#
#}
#
#sub runDelayed {
#  my $self = shift;
#
#  # Run the commandQueue first since this will run the constructors of
#  # sub widgets.
#
#  for(my $i=0; $i <= $#{$self->{commandQueue}}; $i++) {
#    my $obj = $self->{commandQueueObj}->[$i];
#    my $ARG = $self->{commandQueueArgs}->[$i];
#    &{$self->{commandQueue}->[$i]}($obj, @$ARG);
#  }
#  $self->{commandQueueObj} = ();
#  $self->{commandQueue} = ();
#  $self->{commandQueueArgs} = ();
#
#  for(my $i=0; $i <= $#{$self->{messageQueue}}; $i++) {
#    my %ARG = %{$self->{messageQueue}->[$i]};
#    $self->sendMessage(%ARG);
#  }
#  $self->{messageQueue} = ();
#
#}

sub ackWinId {
  my $self = shift;
  my %ARG = @_;

  if($ARG{'iWinId'} <= 1){
      die("Failed on ack'ing Window ID, stopping!");
  }
  $self->{iWinId} = $ARG{'iWinId'};
}

#sub canRun {
#
#  #
#  # Not needed anymore
#  #
#
#  return 1;
#  
#  my $self = shift;
#
#  if($self->runable() == 1){
#    return 1;
#  }
#
#  # we can't process the command now, so delay it.
#  my $i = $#{$self->{commandQueue}} + 1;
#
#  $self->{commandQueueObj}->[$i] = shift;
#  $self->{commandQueue}->[$i] = shift;
#  $self->{commandQueueArgs}->[$i] = shift;
#
#  return 0;
#
#}

sub installHandler {
  my $self = shift;

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

sub immortal {
  my $self = shift;
  $self->{IMMORTAL} = &rndchr;
  $::PBASE_IMORTALS{$self->{IMMORTAL}} = $self;
  return $self;
}

package main;

1;
