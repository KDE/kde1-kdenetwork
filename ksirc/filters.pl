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

$KSIRC_FILTER[0]{'SEARCH'} = "^";
$KSIRC_FILTER[0]{'FROM'} = "^~*[^~]*~*";
$KSIRC_FILTER[0]{'TO'} = "~!all~";

print "*** Loading filter parser...\n";

sub hook_ksircfilter {
  my($i) = 0;
  for(; $i <= $#KSIRC_FILTER; $i++){
    if($_[0] =~ m/$KSIRC_FILTER[$i]{'SEARCH'}/){
       print STDERR "from $_[0]";
       $_[0] =~ s/$KSIRC_FILTER[$i]{'FROM'}/$KSIRC_FILTER[$i]{'TO'}/;
       print STDERR "=> $_[0]\n";
    }
  }
}

print "*** Filter Parser Loaded\n";

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
# Addrule command takes 4 or arguments seperated by " key==value !!! key2==value2 ||| etc" 
# 1. Name of rule
# 2. Pattern to search for
# 3. Substitution to take
# 4. Sub to make 

sub cmd_ksircappendrule {
  my($rule, %PARSED);
  foreach $rule (split(/ !!! /, $args)){
    my($key,$value) = split(/==/, $rule);
    $PARSED{$key} = $value;
  }
  if($PARSED{'DESC'} && $PARSED{'SEARCH'} && $PARSED{'FROM'} && $PARSED{'TO'}){
    my($i) = $#KSIRC_FILTER + 1;
    my($key, $value);
    while(($key, $value) = each %PARSED){
      $KSIRC_FILTER[$i]{$key} = $value;
    }
    print "*** Added rule: " . $KSIRC_FILTER[$i]{'DESC'} . "\n";
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

