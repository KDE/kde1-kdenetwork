
&::PukeSendMessage($PUKE_WIDGET_LOAD, 
		   0, 
		   $PWIDGET_PUSHBT,
		   "ppushbt.so",
		   sub { my %ARG = %{shift()};
			 if($ARG{'iArg'} == 1){
			   print "*E* PPushButton Load failed!\n";
			 }
		       }
		  );

package PPushButton;
@ISA = qw(PButton);
use strict;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->{widgetType} = $::PWIDGET_PUSHBT;

  if($class eq 'PPushButton'){
    $self->create();
  }
  return $self;

}

package main;
