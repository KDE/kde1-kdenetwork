
&::PukeSendMessage($PUKE_WIDGE_LOAD, 
		   0, 
		   $PWIDGET_FRAME,
		   "pframe.so");

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
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->{widgetType} = $::PWIDGET_FRAME;

  if($class eq 'PFrame'){
    $self->create();
  }

  return $self;

}

sub setFrameStyle {
  my $self = shift;

  my $frame = shift;
  my $repaint = shift;

  $self->sendMessage('iCommand' => $::PUKE_QFRAME_SET_FRAME,
		     'iArg' => $frame,
		    'CallBack' => sub {});

  $self->repaint(1) if($repaint == 1);

}

package main;
