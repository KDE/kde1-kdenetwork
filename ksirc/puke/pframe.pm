
package PFrame;
@ISA = qw(PWidget);
use strict;

$PFrame::NoFrame  = 0;
$PFrame::Box      = 0x0001;
$PFrame::Panel    = 0x0002;
$PFrame::WinPanel = 0x0003;
$PFrame::HLine    = 0x0004;
$PFrame::VLine    = 0x0005;
$PFrame::MShape   = 0x000f;
$PFrame::Plain    = 0x0010; 
$PFrame::Raised   = 0x0020; 
$PFrame::Sunken   = 0x0030; 
$PFrame::MShadow  = 0x00f0;

sub new {
  my $proto = shift;
  my $class = ref($proto) || $proto;
  my $self  = {};

  my $parent = shift;


  $self->{iWinId} = -1;
  $self->{Parent} = $parent->{iWinId} if $parent != undef;


  bless($self, $class);

  my $string = $self->rndchr();

  $::PUKE_CREATOR{$string} = sub {$self->{iWinId} = ${_[0]}{iWinId}; };

  $self->sendMessage('iCommand' => $::PUKE_WIDGET_CREATE,
		     'iArg' => $::PWIDGET_FRAME,
		     'iWinId' => $self->{Parent},
		     'cArg' => $string,
		     'CallBack' => sub {});

  print "Done Creating PFrame\n";

  return $self;

}

sub setFrameStyle {
  my $self = shift;

  my $frame = shift;
  my $repaint = shift;

  $self->sendMessage('iCommand' => $::PUKE_QFRAME_SET_FRAME,
		     'iArg' => $frame);

  $self->repaint(1) if($repaint == 1);

}

package main;
