&docommand("/load pbase.pm");
&docommand("/load pwidget.pm");
&docommand("/load pframe.pm");
&docommand("/load ptablevw.pm");
&docommand("/load plistbox.pm");
&docommand("/load pboxlayout.pm");
&docommand("/load plabel.pm");

package UserList;

@ISA = qw(PFrame);

sub new {
  my $class = shift;
  my $self = $class->SUPER::new($class, @_);

  $self->create();

  my $gm = new PBoxLayout($self, $PBoxLayout::TopToBottom, 5);

  my $list_box = new PListBox($self);
  $gm->addWidget($list_box, 5);

  my $user_count = new PLabel($self);
  $user_count->setMaximumSize(25, 2000);
  $user_count->setMinimumSize(25, 25);
  $user_count->setFrameStyle($PFrame::Box|$PFrame::Raised);
  $gm->addWidget($user_count, 5);

  $user_count->setText($list_box->{count});

  @$self{'gm', 'list_box', 'user_count'} = ($gm, $list_box, $user_count);

  return $self;

}

sub DESTROY {
  $self->hide();
  $self->{'gm'}->DESTROY;
  $self->{'list_box'}->DESTROY;
  $self->{'use_count'}->DESTROY;

  $self->SUPER::DESTROY();
}

sub insertText {
  my $self = shift;

  $self->{'list_box'}->insertText(@_);

  $self->{'user_count'}->setText($self->{'list_box'}->{count});
}

sub removeItem {
  my $self = shift;

  $self->{'list_box'}->removeItem(@_);

  $self->{'user_count'}->setText($self->{'list_box'}->{count});
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
    $nick =~ s/(\S+)\@\S+/$1/g;
    if($users_online{$nick} > 0){
      my $text = "Duplicate login for: $nick, logged on " . $users_online{$nick} + 1 . " times";
      print $text . "\n";
      my $date;
      chomp($date = `date`);
      &docommand("/msg #polarcom $date Duplicate login for $nick , logged on " . $users_online{$nick} + 1 . "times") if $nick eq 'action';
    }
    # Make the list sorted
    my $i = 0;
    SEARCH: while($online->text($i) ne undef){
      
      if(($online->text($i) cmp $nick) >= 0){
        #        $i -= 1 if $i != 0;
        last SEARCH;
      }
      $i++;
      #      &tell("Searching... $i");
    }
    #    &tell("Doing insert at $i");
    $online->insertText($nick, $i);
    $users_online{$nick}++;
  }
  elsif($msg =~ /ice: Logoff (\S+)/){
    my $i = 0;
    my $nick = $1;
    $nick =~ s/(\S+)\@\S+/$1/g;
    #    print "Trying to logoff: $nick\n";
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