
package PBoxLayout;
@ISA = qw(PBase);
use strict;

# setup default handlers

$PBoxLayout::LeftToRight = 0;
$PBoxLayout::RightToLeft = 1;
$PBoxLayout::TopToBottom = 2;
$PBoxLayout::BottomToTop = 3;

$PBoxLayout::AlignLeft        = 0x0001;
$PBoxLayout::AlignRight       = 0x0002;
$PboxLayout::AlignHCenter     = 0x0004;
$PBoxLayout::AlignTop         = 0x0008;
$PBoxLayout::AlignBottom      = 0x0010;
$PBoxLayout::AlignVCenter     = 0x0020;
$PBoxLayout::AlignCenter      = $PBoxLayout::AlignVCenter | 
                                $PBoxLayout::AlignHCenter;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  my $widget = shift;

  print "Widget: " . ref($widget) . "\n";

  if(ref($widget) eq ''){
    print "*E* Error Creating PBoxLayout, did not give valid parent\n";
    return;
  }
  elsif(ref($widget) eq 'PBoxLayout'){
    $self->{Parent} = $widget;
    $self->{ParentType} = 'Layout';
    $self->{Direction} = shift;
    $self->{Border} = shift;
    $self->{Added} = 0;
  }
  else{
    print "*\cbE\cb* Generic Widget type\n";
    $self->{Parent} = $widget;
    $self->{ParentType} = 'Widget';
    $self->{Direction} = shift;
    $self->{Border} = shift;
    $self->{Added} = 1;
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

  #  my @ARG = @_;
  $self->{Parent}->canRun($self, \&PBoxLayout::create, ()) || return;
  print "*I* Making Box Layout\n";


#  print "*I* Createing Layout with parent " . $self->{Parent}->{iWinId} . "\n";

  $::PUKE_CREATOR{$self->{initId}} =  sub{$self->ackWinId(@_);};

  $self->{runable} = 1;
  $self->sendMessage('iCommand' => $::PUKE_LAYOUT_NEW,
		     'iWinId' => $self->{ParentType} == 'Widget' ? $self->{Parent}->{iWinId} : undef,
		     'cArg' => $self->{initId},
		     'iArg' => $self->{Direction} + 65536 * $self->{Border},
		     'CallBack' => sub { });
  $self->setRunable(0);
}

sub addWidget {
  my $self = shift;

  my $widget = shift;
  my $stretch = shift;
  my $align = shift;

  if($self->{Added} == 0){
    print "*I* Delaying Add widget\n";
    $self->setRunable(0);
  }

  # make sure we can run, and the widget we want to add can run.
  my @ARG = ($widget, $stretch, $align);
  $self->canRun($self, \&PBoxLayout::addWidget, \@ARG) || return;
  $widget->canRun($self, \&PBoxLayout::addWidget, \@ARG) || return;
  
#  print "*I* Sending add arg for id: " . $widget->{iWinId} . "\n";

  $align = $PBoxLayout::AlignCenter if($align == 0);

  $self->sendMessage('iCommand' => $::PUKE_LAYOUT_ADDWIDGET,
		     'iWinId' => $self->{iWinId},
		     'iArg' => $widget->{iWinId},
		     'cArg' => pack("CC", $stretch, $align),
		     'CallBack' => sub { });

  
}

sub addLayout {
  my $self = shift;

  if($self->{Added} == 0){
    print "*I* In AddLayout: Warning Not added to any layouts/widgets yet\n";
  }

  
  my $layout = shift;
  if(ref($layout) ne 'PBoxLayout'){
    print "*E* Passed non layout type to addLayout\n";
    return 1;
  }

  # make sure we can run, and the widget we want to add can run.
  my @ARG = ($layout);
  $self->canRun($self, \&PBoxLayout::addLayout, \@ARG) || return;
  $layout->canRun($self, \&PBoxLayout::addLayout, \@ARG) || return;

  $self->sendMessage('iCommand' => $::PUKE_LAYOUT_ADDLAYOUT,
		     'iWinId' => $self->{iWinId},
		     'iArg' => $layout->{iWinId},
		     'CallBack' => sub {
                       my %ARG = %{shift()};
                       print "*I* In ADDLAYOUT CALLBACK\n";
		       if($ARG{'iArg'} != 0){
			 print "*E* AddLayout call failed\n";
                       }
                       else{
                         print "*I* Added new Layout for " . $layout->{iWinId} . "\n";
                         $layout->{Added} = 1;
                       }
		     });
  
}

#sub canRun {
#  my $self = shift;
#
#  my $override = 0;
#  my $first = shift;
#  if(ref($first) eq ''){
#    $override = $first;
#    $first = shift;
#    print "Doing override\n";
#  }
#
#  if($override == 0 && $self->{Added} == 0) {
#  $self->setRunable(0);
#}
#return PBase::canRun($self, $first, @_);
#}
package main;
1;
