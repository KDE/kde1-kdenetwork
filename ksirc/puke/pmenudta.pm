&::PukeSendMessage($PUKE_WIDGET_LOAD, 
		   $PUKE_CONTROLLER, 
		   $PWIDGET_MENUDATA,
		   "pmenudta.so",
		   sub { my %ARG = %{shift()};
			 if($ARG{'iArg'} == 1){
			   print "*E* PMenuData Load failed!\n";
			 }
		       }
		  );

package PMenuData;
use strict;

#
# Only methods, constructor must create $self elsewhere and must of PBase type
#

sub new {
  print "*E* Can't call new for this class\n";
}

sub insertText {
  my $self = shift;

  my $text = shift;
  
  my %ARG = $self->sendMessage('iCommand' => $::PUKE_MENUDATA_INSERT_TEXT,
                               'cArg' => $text,
                               'WaitFor' => 1);

  return $ARG{'iArg'};
}

sub insertPixmap {
  my $self = shift;

  my $text = shift;
  
  my %ARG = $self->sendMessage('iCommand' => $::PUKE_MENUDATA_INSERT_PIXMAP,
                               'cArg' => $text,
                               'WaitFor' => 1);

  return $ARG{'iArg'};

}

package main;

1;
