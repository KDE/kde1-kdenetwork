#
# Structure format is:
# $var[filter_number]{field}
#
# Defined fields: (* is required)
#    SEARCH(*): search filter
#    FROM(*): intput to subtitution regex
#    TO(*): resultant to substitution regex
#    DESC: description of filter rule
#

$#KSIRC_FILTER = 0;

$KSIRC_FILTER[0]{'SEARCH'} = "^[^~]";
$KSIRC_FILTER[0]{'FROM'} = "^";
$KSIRC_FILTER[0]{'TO'} = "~!all~";

print "Loading filter parser...";

sub hook_ksircfilter {
  my($i) = 0;
  for(; $i <= $#KSIRC_FILTER; $i++){
    if($_[0] =~ m/${$KSIRC_FILTER[$i]{'SEARCH'}}/){
       $_[0] =~ s/${$KSIRC_FILTER[$i]{'FROM'}}/${$KSIRC_FILTER[$i]{'TO'}}/;
    }
  }
}

print "Done\n";

addhook("print", "ksircfilter");

sub cmd_ksircprintrule {
  my($i) = 0;
  for(; $i <= $#KSIRC_FILTER; $i++){
    print STDOUT "*** Rule $i: if ". $KSIRC_FILTER[$i]{SEARCH} . " then ". $KSIRC_FILTER[$i]{FROM} . " -> " . $KSIRC_FILTER[$i]{TO} . "\n";
  }
}

addcmd("ksircprintrule");
&docommand("^alias prule ksircprintrule");

# 
# Addrule command takes 4 arguments seperated by " !!! "
# 1. Name of rule
# 2. Pattern to search for
# 3. Substitution to take
# 4. Sub to make 

sub cmd_ksircappendrule {
  my(@args) = split(/ !!! /, $args);
  if($#args == 3){
    my($i) = $#KSIRC_FILTER + 1;
    $KISRC_FILTER[$i]{'DESC'} = $args[0];
    $KSIRC_FILTER[$i]{'SEARCH'} = $args[1];
    $KSIRC_FILTER[$i]{'FROM'} = $args[2];
    $KSIRC_FILTER[$i]{'TO'} = $args[3];
  }
  else{
    print STDOUT "*E* Parse Error in Rule, format is: name !!! search !!! from !!! to\n";
  }
}

addcmd("ksircappendrule");
&docommand("^alias arule ksircappendrule");

sub cmd_ksircclearrule {
  @KSIRC_FILTER = ();
  print "*** ALL FILTER RULES ERASED\n";
}

addcmd("ksircclearrule");
&docommand("^alias crule ksircclearrule");

sub cmd_ksircdelrule {
  if($args =~ /^\d+$/){
    splice(@KSIRC_FILTER, $args, 1);
    print STDOUT "*** Deleted rule: $args\n";
  }
  else {
    print STDOUT "*E* Syntax is ksircdelrule <rule number>\n";
  }
}

addcmd("ksircdelrule");
&docommand("^alias drule ksircdelrule");

