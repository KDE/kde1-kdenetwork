
&::PukeSendMessage($::PUKE_WIDGET_LOAD,
                     0,
                     $::PWIDGET_LISTBOX,
                     "plistbox.so",
                     sub { my %ARG = %{shift()};
                     if($ARG{'iArg'} == 1){
                       print "*E* PListBox Load failed!\n";
                     }
                     }
                  );


package PListBox; #FOLD00
@ISA = qw(PTableView);
use strict;

if($PListBox::usage == undef){
    $PListBox::usage = 0;
}

sub new { #FOLD00
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->{widgetType} = $::PWIDGET_LISTBOX;

  if($class eq 'PListBox'){
    $self->create();
  }

  $self->{count} = 0;
  $self->{items} = ();

  $self->installHandler($::PUKE_LISTBOX_SELECTED_ACK, sub{$self->selected(@_)});

  return $self;

}

sub DESTROY { #FOLD00
    my $self = shift;
    $self->SUPER::DESTROY(@_);
    $PListBox::usage--;
    if($PListBox::usage == 0){
        &::PukeSendMessage($::PUKE_WIDGET_UNLOAD,
                           0,
                           $::PWIDGET_LISTBOX,
                           "",
                           sub {}
                          );

    }
}

sub insertText { #FOLD00
  my $self = shift;
  
  my $text = shift;
  my $index = shift;
  my $rindex = $index;

  if(length($text) > 49){
    print "*E* Puke Line Edit: set text with over 49 characters.\n";
    $text =~ s/^(.{49,49}).*/$1/;
  }
  
  if($index < 0 || $index >= $self->{count}){
    $rindex = $self->{count};
  }
  $self->{items}->[$rindex] = $text;
  $self->{count} ++;


  # Don't need the ouput since GET_TEXT_ACK will be called and
  # we'll set it there
  $self->sendMessage('iCommand' => $::PUKE_LISTBOX_INSERT,
                     'iArg' => $rindex,
		     'cArg' => $text,
                     'CallBack' => sub {});

}

sub text { #FOLD00
  my $self = shift;
  my $index = shift;

  return $self->{items}->[$index];
}

sub insertPixmap { #FOLD00
  my $self = shift;
  
  my $file = shift;
  my $index = shift;
  my $rindex = $index;

  if(length($file) > 49){
    print "*E* PukeListBox: set pixmap filename with over 49 characters.\n";
    $file =~ s/^(.{49,49}).*/$1/;
  }
  
  if($index < 0 || $index >= $self->{count}){
    $rindex = $self->{count};
  }
  $self->{items}->[$rindex] = "***PIXMAP***" . $file;
  $self->{count} ++;


  # Don't need the ouput since GET_TEXT_ACK will be called and
  # we'll set it there
  $self->sendMessage('iCommand' => $::PUKE_LISTBOX_INSERT_PIXMAP,
                     'iArg' => $rindex,
		     'cArg' => $file,
                     'CallBack' => sub {});

}
sub selected {
  my $self = shift;
  my %ARGS = %{shift()};

  $self->{current} = $ARGS{'iArg'};
  $self->{currentText} = $ARGS{'cArg'};
}

package main; #FOLD00

1;
