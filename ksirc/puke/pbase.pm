
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
  my $i;
  for($i = 0; $i < 8; $i++){
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
  $self->{cmdQueue} = ();

  if($::PUKE_FETCH_WIDGET == 1) {
    $self->{Fetch} = 1;
  }

  #  $self->installHandler($::PUKE_WIDGET_DELETE_ACK, sub{$self->DESTROY});
  
  return $self;

}

sub create {
  my $self = shift;
  
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
    
    $self->clearQueue();
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
    return -1;
  }
  my $winid;
  my $cmd;
  foreach $cmd (%::PUKE_W_HANDLER){
    foreach $winid (%{$::PUKE_W_HANDLER{$cmd}}){
      if($winid == $self->{'iWinId'}){
        $::PUKE_W_HANDLER{$cmd}{$REPLY{iWinId}} = $::PUKE_W_HANDLER{$cmd}{$self->{iWinId}};
        delete $::PUKE_W_HANDLER{$cmd}{$self->{iWinId}};
      }
    }
  }
  
  $self->ackWinId(%REPLY);
  #  $self->setRunable(0);
  return 0;
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

  #  print "*I* Widget Deleted\n";
  $self->hide();

  #  $self->setRunable(1);

  delete($::PBASE_IMORTALS{$self->{IMMORTAL}});

  if($self->{DESTROYED} != 1 && $self->{Parent} == 0){
    $self->sendMessage('iCommand' => $::PUKE_WIDGET_DELETE,
                       'CallBack' => sub {});
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

sub ackWinId {
  my $self = shift;
  my %ARG = @_;

  if($ARG{'iWinId'} <= 1){
      die("Failed on ack'ing Window ID, stopping!");
  }
  
  $self->{iWinId} = $ARG{'iWinId'};
}


sub installHandler {
  my $self = shift;

  my $command = shift;
  my $handler = shift;

  my $cmd =
    sub {
      $::PUKE_W_HANDLER{$command}{$self->{iWinId}} = $handler;
    };

  if($self->{iWinId} == -1){
    $self->addQueue($cmd);
  }
  else{
    &$cmd();
  }

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

sub addQueue {
  my $self = shift;

  push(@{$self->{cmdQueue}}, shift());
}

sub clearQueue {
  my $self = shift;
  
  my $cmd;
  while($cmd = pop(@{$self->{cmdQueue}})){
    &$cmd;
  }
}

package main;

1;
