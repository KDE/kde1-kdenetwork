#use lib "/opt/kde/share/apps/ksirc";

&docommand("/load pbase.pm");
&docommand("/load pwidget.pm");
&docommand("/load pframe.pm");
&docommand("/load ptablevw.pm");
&docommand("/load plistbox.pm");
&docommand("/load pbutton.pm");;
&docommand("/load pboxlayout.pm");
&docommand("/load plabel.pm");
&docommand("/load pbutton.pm");
&docommand("/load ppushbt.pm");

&docommand("/load dcc_progress.pm");

package DCCStatus;
@ISA = qw(PFrame);
#use strict;

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);
  $self->create();  

  my $gm_main = new PBoxLayout($self, $PBoxLayout::TopToBottom, 5);
  my $gm_a = new PBoxLayout($PBoxLayout::LeftToRight, 5);       
  $gm_main->addLayout($gm_a);

  my $gm_aa = new PBoxLayout($PBoxLayout::TopToBottom, 5);
  $gm_a->addLayout($gm_aa);
  my $label = new PLabel($self);
  $label->setText("Pending Gets");
  $label->setMaximumSize(20,1000);
  $gm_aa->addWidget($label, 5, $PBoxLayout::AlignCenter);  
  my $lb_get = new PListBox($self);
  $gm_aa->addWidget($lb_get, 5, $PBoxLayout::AlignCenter);
  my $button_get = new PPushButton($self);
  $button_get->setText("Get &File");
  $button_get->setMaximumSize(30,1000);
  $button_get->setMinimumSize(30,10);
  $button_get->installHandler($::PUKE_BUTTON_RELEASED_ACK, sub{});
  $button_get->installHandler($::PUKE_BUTTON_PRESSED_ACK, sub{});
  $button_get->installHandler($::PUKE_BUTTON_CLICKED_ACK, sub{$self->getClicked});
  $gm_aa->addWidget($button_get, 5);

  my $gm_ab = new PBoxLayout($PBoxLayout::TopToBottom, 5);
  $gm_a->addLayout($gm_ab);
  my $label2 = new PLabel($self);
  $label2->setText("Pending Chats");
  $label2->setMaximumSize(20,1000);
  $gm_ab->addWidget($label2, 5, $PBoxLayout::AlignCenter);
  my $lb_send = new PListBox($self);
  $gm_ab->addWidget($lb_send, 5, $PBoxLayout::AlignCenter);
  my $button_send = new PPushButton($self);
  $button_send->setText("&Open Chat");
  $button_send->setMaximumSize(30,1000);
  $button_send->setMinimumSize(30,10);
  $button_send->installHandler($::PUKE_BUTTON_RELEASED_ACK, sub{});
  $button_send->installHandler($::PUKE_BUTTON_PRESSED_ACK, sub{});
  $button_send->installHandler($::PUKE_BUTTON_CLICKED_ACK, sub{$self->chatClicked});
  $gm_ab->addWidget($button_send, 5);

  $gm_main->activate();

  $self->resize(400, 275);
  
  @$self{'gm_main', 'gm_a', 'gm_aa', 'label1', 'lb_get', 'gm_ab', 'label2', 'lb_chat', 'button_get', 'button_chat' }
      = ( $gm_main, $gm_a,  $gm_aa,  $label,   $lb_get,  $gm_ab,  $label2,  $lb_send,  $button_get,  $button_send  );


  print "*I* Finished creating DCCStatus\n";

  return $self;
}

sub getClicked {
    my $self = shift;

    my $text = $self->{'lb_get'}->currentText();
    if($text eq ''){
        return;
    }

    $text =~ /^(\S+) offered (\S+) at size (\S+)/;
    my($who, $file, $size) = ($1, $2, $3);
    $::KSIRC_FILE_SIZES{$file} = $size;

    $self->{'lb_get'}->removeItem($self->{'lb_get'}->current);
    $self->{'lb_get'}->setCurrentItem($self->{'lb_get'}->current);
    
    &::docommand("dcc get $who $file");
    print "*I* File: $file saved size as: $size\n";
}

sub chatClicked {
    my $self = shift;

    my $text = $self->{'lb_chat'}->currentText();
    if($text eq ''){
        return;
    }

    $self->{'lb_chat'}->removeItem($self->{'lb_chat'}->current);
    $self->{'lb_chat'}->setCurrentItem($self->{'lb_chat'}->current);
    &::docommand("dcc chat $text");
}

sub DESTROY {
  print "*E* Destroying dcc status widget\n";
  shift()->close();
}

sub close {
  my $self = shift;
  $self->sendMessage('iCommand' => $::PUKE_WIDGET_DELETE,
                     'CallBack' => sub {},
                     'WaitFor'  => 1);
  $self->{'gm_main'}->DESTROY;
  delete $self->{'gm_main'};
}

package main;

sub hook_ksirc_dcc_request {
  my($type) = shift;
  my($machine) = shift;
  my($port) = shift;
  my($file) = shift;
  my($size) = shift;
  if($KSIRC_DCCSTATUS == undef){
    $KSIRC_DCCSTATUS = new DCCStatus;
    $KSIRC_DCCSTATUS->resize(400, 275);
  }
  my($listbox) = '';
  if($type eq 'SEND'){
    $listbox = $KSIRC_DCCSTATUS->{'lb_get'};
    $listbox->insertText("$who offered $file at size $size");
  }
  else{
    $listbox = $KSIRC_DCCSTATUS->{'lb_chat'};
    $listbox->insertText("$who");
  }
  $listbox->setCurrentItem(0);
  $KSIRC_DCCSTATUS->show;
}

&addhook("dcc_request", "ksirc_dcc_request");

sub hook_ksirc_dcc_send {
  my $nick = shift;
  my $file = shift;
  my $size = shift;
  my $fh = shift;

  #  print "*I* Starting dcc into with: $nick, $file, $size, $fh\n";
  
  my($window) =  new DCCProgress;
  $window->setRange(0, $size);
  $window->setCaption("$file=>$nick");
  $window->setTopText("Sending: $file Size: $size");
  $window->setBotText("Status: pending");
  $window->setCancel("dcc close send $nick $file");
  $KSIRC_DCC{$fh}{$file}{'Window'} = $window;
  $KSIRC_DCC{$fh}{$file}{'StartTime'} = time() - 1;
  $window->show;
}

&addhook("dcc_send", "ksirc_dcc_send");

sub hook_ksirc_dcc_send_status {
  my $file = shift;
  my $bytes = shift;
  my $fh = shift;

  my $window = $KSIRC_DCC{$fh}{$file}{'Window'};
  if($window == undef){
    print "*E* No such window for $file, $bytes, $fh\n";
    return;
  }
  $window->setBotText("BPS: " . int($bytes/(time() -  $KSIRC_DCC{$fh}{$file}{'StartTime'})));
  $window->setValue($bytes);
}

&addhook("dcc_send_status", "ksirc_dcc_send_status");

sub hook_ksirc_dcc_get {
  my $nick = shift;
  my $file = shift;
  my $fh = shift;

  print "*I* Starting dcc into with: $nick, $file, $size, $fh\n";

  my $size = $::KSIRC_FILE_SIZES{$file};
  
  my($window) =  new DCCProgress;
  $window->setRange(0, $size);
  $window->setCaption("$file<=$nick");
  $window->setTopText("Receiver: $file Size: $size");
  $window->setBotText("Status: pending");
  $window->setCancel("dcc close get $nick $file");
  $KSIRC_DCC{$fh}{$file}{'Window'} = $window;
  $KSIRC_DCC{$fh}{$file}{'StartTime'} = time() - 1;
  $window->show;
}

&addhook("dcc_get", "ksirc_dcc_get");

sub hook_ksirc_dcc_get_status {
  my $file = shift;
  my $bytes = shift;
  my $fh = shift;

  my $window = $KSIRC_DCC{$fh}{$file}{'Window'};
  if($window == undef){
    print "*E* No such window for $file, $bytes, $fh\n";
    return;
  }
  $window->setBotText("BPS: " . int($bytes/(time() -  $KSIRC_DCC{$fh}{$file}{'StartTime'})));
  $window->setValue($bytes);
}

&addhook("dcc_get_status", "ksirc_dcc_get_status");


sub hook_ksirc_dcc_disconnect {
  my $nick = shift;
  my $file = shift;
  my $bytes = shift;
  my $time = shift;
  my $fh = shift;

  my $window = $KSIRC_DCC{$fh}{$file}{'Window'};
  print "*E* Window ref: $fh, $file, $window\n";
  $window->DESTROY;
  $KSIRC_DCC{$fh}{$file}{'Window'}->DESTROY;
  delete $KSIRC_DCC{$fh}{$file};
  delete $KSIRC_DCC{$fh};
}

addhook("dcc_disconnect", "ksirc_dcc_disconnect");


&print("*I* Done DCC Status");
#$::test = new DCCStatus;
#$::test->resize(400, 275);
#$::test->show();

1;