&docommand("/load pbase.pm");
&docommand("/load pwidget.pm");
&docommand("/load pframe.pm");
&docommand("/load ptablevw.pm");
&docommand("/load plistbox.pm");
&docommand("/load pboxlayout.pm");
&docommand("/load plabel.pm");
&docommand("/load pmenudta.pm");
&docommand("/load ppopmenu.pm");

use Net::SMTP;

my $WHO = "$ENV{HOME}/who_online.pl";

my %ALLOW_MULT = ();
$ALLOW_MULT{'asj'} = 1;

package UserList;

@ISA = qw(PFrame);

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->create();

  my $gm = new PBoxLayout($self, $PBoxLayout::TopToBottom, 5);

  my $list_box = new PListBox($self);
  $gm->addWidget($list_box, 5);

  my $event_list = new PListBox($self);
  $event_list->setMaximumSize(110, 2000);
  $event_list->setMinimumSize(110, 1);
  #  $event_list->setFrameStyle($PFrame::Box|$PFrame::Raised);
  $event_list->setFrameStyle(0);
  $event_list->setBackgroundColor($self->backgroundColor());
  # Turn off the widget so people can't click in it
  $event_list->setEnabled(0);
  $event_list->setAutoScrollBar(0);
  $event_list->setScrollBar(0);
  $gm->addWidget($event_list, 5);

  my $user_count = new PLabel($self);
  $user_count->setMaximumSize(25, 2000);
  $user_count->setMinimumSize(25, 25);
  $user_count->setFrameStyle($PFrame::Box|$PFrame::Raised);
  $gm->addWidget($user_count, 5);

  $user_count->setText($list_box->{count});

  my $menu = new PPopupMenu();
  my $menu_online = $menu->insertText("Online Time");
  $menu->installMenu($menu_online, sub { &::say("online " . $self->{'list_box'}->currentText() . "\n"); });
  my $menu_info = $menu->insertText("User Information");
  $menu->installMenu($menu_info, sub { my $exec = "$WHO info " . $self->{'list_box'}->currentText(); print `$exec`; });
  my $menu_rem = $menu->insertText("Remove User From List");
  $menu->installMenu($menu_rem, sub {
     my $user = $self->{'list_box'}->currentText();
     my $count = $self->{'list_box'}->current();
     $self->{'list_box'}->removeItem($count);
     if($users_online{$user} > 0){
         $users_online{$user}--;
     }
  });
  my $menu_kill = $menu->insertText("Disconnect User");
  $menu->installMenu($menu_kill, sub { my $exec = "$WHO reset " . $self->{'list_box'}->currentText(); print "Running: $exec\n"; system($exec); });

  # Install listner for mouse events
  
  $list_box->installHandler($::PUKE_WIDGET_EVENT_MOUSEBUTTONPRESS,
			      sub {$self->popupMenu(@_)});
  $list_box->installHandler($::PUKE_WIDGET_EVENT_MOUSEBUTTONRELEASE,
			      sub {$self->popdownMenu(@_)});


  @$self{'gm', 'list_box', 'user_count', 'max', 'event_list', 'menu'} = ($gm, $list_box, $user_count, 0, $event_list, $menu);

  return $self;

}

sub DESTROY {
    #  $self->hide();
  $self->{'gm'}->DESTROY;
  $self->{'list_box'}->DESTROY;
  $self->{'use_count'}->DESTROY;

  $self->SUPER::DESTROY();
}

sub popupMenu {
  my $self = shift;
  my $arg = shift;

  my($x, $y, $b, $s) = unpack("i4", $arg->{'cArg'});
  #  print "$x $y $b $s\n";

  # Only popup for button 2
  $self->{'menu'}->popupAtCurrent() if $b == 2;
}

sub popdownMenu {
  my $self = shift;

  $self->{'menu'}->hide();
}

sub addEvent {
    my $self = shift;

    my $event_list = $self->{'event_list'};
    
    while($event_list->{count} >= 10){
      $event_list->removeItem($event_list->{count});
    }

    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
    
    my $text = sprintf("%02d:%02d:%02d " . shift(), $hour, $min, $sec);
    
    $event_list->insertText($text, 0);
}

sub insertText {
  my $self = shift;
  
  $self->{'list_box'}->insertText(@_);

  if($self->{'list_box'}->{count} > $self->{'max'}){
      $self->{'max'} = $self->{'list_box'}->{count};
  }

  $self->{'user_count'}->setText("C: " . $self->{'list_box'}->{count} . " M: " . $self->{'max'});
}

sub removeItem {
  my $self = shift;

  $self->{'list_box'}->removeItem(@_);

  $self->{'user_count'}->setText("C: " . $self->{'list_box'}->{count} . " M: " . $self->{'max'});
}

sub text {
  my $self = shift;

  return $self->{'list_box'}->text(@_);
}

sub count {
  my $self = shift;

  return $self->{'list_box'}->{count};
}

package main;


if($online == undef){
  $online = new UserList();
  $online->setCaption("Users Online");
  $online->resize(250, 450);
  $online->show();

  %users_online = ();
}

sub hook_online_mon {
  my $channel = shift;
  my $msg = shift;
  return unless $channel eq '#polarcom';
  return unless $msg =~ /Login|Logoff/;

  if($msg =~ /ice: Login (\S+)/){
    my $nick = $1;
    #return if $nick =~ /administrator/;
    $nick =~ s/(\S+)\@\S+/$1/g;
    if($users_online{$nick} > 0){
      my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
      my $date;
      chomp($date = `date`);
      my $times = $users_online{$nick} + 1;
      &say("$date Duplicate login for $nick, logged on $times times") if (($main::nick eq 'action') || ($nick eq 'administrator'));
      $online->addEvent("Duplicate: $nick");
      if($ALLOW_MULT{$nick} != 1){
        my $exec = "$WHO reset " . $nick;
        print "*I* Running: $exec\n";
        system($exec);
        my $smtp = new Net::SMTP->new("mail.polarcom.com");
        $smtp->mail("sysadmin\@polarcom.com");
        $smtp->to("$nick\@polarcom.com");
        $smtp->data();
        $smtp->datasend("Subject: Security Alert\n\n\n");
        $smtp->datasend("Your account was simultaneously access by 2 or more users.\n");
        $smtp->datasend("The second user was automatically terminated.\n\n");
        $smtp->datasend("-- Security Monitor\n");
        $smtp->datasend();
        $smtp->quit();
      }
      else {
        print "*I* $nick not being removed, in exclude list\n";
      }
    }
    # Make the list sorted
    my $i = 0;
    SEARCH: while($online->text($i) ne undef){
      
      if(($online->text($i) cmp $nick) >= 0){
        last SEARCH;
      }
      $i++;
    }
    $online->insertText($nick, $i);
    $online->addEvent("On: $nick");
    $users_online{$nick}++;
  }
  elsif($msg =~ /ice: Logoff (\S+)/){
    my $i = 0;
    my $nick = $1;
    $nick =~ s/(\S+)\@\S+/$1/g;
    #    print "Trying to logoff: $nick\n";
    $online->addEvent("Off: $nick");
    if($users_online{$nick} > 0){
      # print "$nick in list\n";
      $users_online{$nick}--;
      while($online->text($i) ne undef){
	if($online->text($i) eq $nick){
          #   print "Removing $i for $nick which is really: " . $online->text($i) . "\n";
            $online->removeItem($i);
	  return;
	}
	$i++;
      }
    }
  }
}

&addhook("public", "online_mon");

sub hook_get_users {
  my $mesg = shift;

  if($mesg =~ /^send users online/){
    my $reply = 'Online: ';
    for($i = 0; $i <= $online->count(); $i++){
      $reply .= $online->text($i) . " ";
    }
    &msg($who, $reply);
  }
  if($mesg = /Online: (.+)\s+$/){
    &tell("*I* Updating user list from $who");
    $online->addEvent("Update user list: $who");
    my $users = $1;
    while($online->count() > 0){
        $online->removeItem(0);
    }
    %users_online = ();
    foreach $user (split(/\s+/, $users)){
      $users_online{$user}++;
      $online->insertText($user, -1);
    }
  }
}

&addhook("msg", "get_users");

sub cmd_refresh_users {
    my $output = `$WHO list`;
    my @users = split(/\n/, $output);
    %users_online = ();
    while($online->count() > 0){
        $online->removeItem(0);
    }
    foreach $user (@users){
      $user =~ s/(\S+)\@\S+/$1/g;
      next if $user eq '';
      next if $user =~ /administrator/;
      $users_online{$user}++;
      $online->insertText($user, -1);
    }
}

&addcmd("refresh_users");