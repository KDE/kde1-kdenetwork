use Socket;
use Fcntl;

#
# Clean up if this is the second load.
#
if($PUKEFd != undef){
  &remsel($PUKEFd);
  close($PUKEFd);
  sleep(1);
  $PUKEFd = undef;
}

# 
# Multi operation level handler, winId Based.
# 
# PUKE_HANDLER{Cmd}{winId} = sub();

%PUKE_HANDLER = ();

# 
# Default handler is called if no handler defined
# Default handlers defined in commands-handler.pl
# Single level PUKE_DEF_HANDLER{$cmd};
#

#%PUKE_DEF_HANDLER = ();

require 'commands-perl.pl';
require 'commands-handler.pl';

$PukePacking = "iiia50aa";  # aa padding to fill to 64 bytes
$PukeMSize = 64;
$sock = $ENV{'HOME'} . "/.ksirc.socket";

$PUKEFd = &newfh;
$proto = getprotobyname('tcp');
socket($PUKEFd, PF_UNIX, SOCK_STREAM, 0) || print "PUKE: Sock failed: $!\n";
$sun = sockaddr_un($sock);
print "PUKE: Connecting to $sock\n";
connect($PUKEFd,$sun) || warn "Puke: Connect failed: $!\n",$PUKEFailed=1;
select($PUKEFd); $| = 1; select(STDOUT);
fcntl($PUKEFd, F_SETFL, O_NONBLOCK);

# Arg1: Command
# Arg2: WinId
# Arg3: iArg
# Arg4: cArg
sub PukeSendMessage {
  my($cmd, $winid, $iarg, $carg, $handler) = @_;
  print("PUKE: cArg message too long $cArg\n") if(length($carg) > 50);
  $PUKE_HANDLER{$cmd}{$winid} = $handler;
  syswrite($PUKEFd, pack($PukePacking, $cmd, $winid, $iarg, $carg), $PukeMSize);
}

sub sel_PukeRecvMessage {
  my($m);
  my($cmd, $winid, $iarg, $carg, $junk);
  $len = sysread($PUKEFd, $m, $PukeMSize);
  if($len== 0){
    &remsel($PUKEFd);
    close($PUKEFd);
    return;
  }
#  print "Length: $len " . length($m) . "\n";
  ($cmd, $winid, $iarg, $carg) = unpack($PukePacking, $m);
#  print("PUKE: Got => $PUKE_NUM2NAME{$cmd}/$cmd\n");
#  print("PUKE: Got: $cmd, $winid, $iarg, $carg\n");
  if($winid == undef){ $winid = 0; }
  #
  # Check both $cmd and the correct reply -$cmd
  #
  my(%ARG) = ('iCommand' => $cmd,
	      'iWinId' => $winid,
	      'iArg' => $iarg,
	      'cArg' => $carg);

#  print "*I* Def handler: $PUKE_DEF_HANDLER{$cmd}\n";

  if($PUKE_HANDLER{$cmd}{$winid}){
    &{$PUKE_HANDLER{$cmd}{$winid}}(\%ARG);
  } elsif ($PUKE_HANDLER{-$cmd}{$winid}){
    &{$PUKE_HANDLER{-$cmd}{$winid}};
  } elsif ($PUKE_DEF_HANDLER{"$cmd"}) {
    &{$PUKE_DEF_HANDLER{"$cmd"}}(\%ARG);
  }
  else {
    # No handler at all, unkown reply
    print("*E* PUKE: Got unkown command: $cmd/$PUKE_NUM2NAME{$cmd}\n");
    print("PUKE: Got: $cmd, $winid, $iarg, $carg\n");
  }
}

&addsel($PUKEFd, "PukeRecvMessage", 0);

# Basics are up and running, now init Puke/Ksirc Interface.

&PukeSendMessage($PUKE_SETUP, 0, 0, $server, undef);
