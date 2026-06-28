#!/usr/bin/perl
###!/usr/local/perl518/bin/perl -w

use Date::ICal;
use Date::Format;

sub trim($)
{
	my $string = shift;
	$string =~ s/^\s+//;
	$string =~ s/\s+$//;
	return $string;
}

#mertek: hany oraval allitjak el es merre (+2 ket ora plusz) (01 pozitiv ab negativ)
#ido: az atallitas ideje (ora, pl 2 azaz hajnali ketto) (0-9ab)
#->datlight
# honap: A-L
# tipus: 
#  X-pontos datum
#  D-valahanyadik elofordulasa egy napnak (utolso kedd pl)
#  A-valahanyadika utan az elso nap (12-e utan az elso szerda)
# tipusfuggo:
#  X: 00 .. 31
#  D: elso szamjegy: hanyadik elofordulas 
#    (1,2,3,4 elso, masodik vagy a,b,c,d utolso utolso elotti stb)
#     masidk: 1 vasarnap .. 7 szombat
#  A: datum: a-z 1..26-ig 0-5 : 27-31-e
#      nap: mint D
#->std
#(ua)
#
# FREQ=YEARLY;BYMONTH=10;BYDAY=-1SU
# FREQ=YEARLY;BYMONTH=4;BYDAY=1SU
# FREQ=YEARLY;BYMONTH=10;BYMONTHDAY=24,25,26,27,28,29,30;BYDAY=SA

%wd=(
  "SU" => 1,
  "MO" => 2,
  "TU" => 3,
  "WE" => 4,
  "TH" => 5,
  "FR" => 6,
  "SA" => 7,
);

sub process($)
{
  $_=$_[0];
#  print $_;
  /BYMONTH=([0-9]+)/;
  $mon=chr(ord('A')+int($1)-1);
  $typ='X';
  if(/BYMONTHDAY/) {
    $typ='A';
    /BYMONTHDAY=([0-9]+)/;
    if(int($1)<27) { $d1=chr(ord('a')+int($1)-1); }
    else {   $d1=chr(ord('0')+int($1)-27); }
    /BYDAY=([A-Z][A-Z])/;
    $d2=$wd{$1};
  } else {
    $typ='D';
    /BYDAY=([-0-9A-Z]+)/;
    $_=$1;
    /([A-Z][A-Z])/;
    $d2=$wd{$1};
    /([-0-9]+)/;
    if(int($1)<0) {  $d1=chr(ord('a')+(-1*int($1))-1); }
    else {  $d1=chr(ord('0')+int($1)); }
  }
  return($mon.$typ.$d1.$d2);
}

sub parse_offset($)
{
  # parse offset like +0000 or -1130 to minutes
  return( (int($_[0]/100))*60 + (int($_[0])%100) );
}

#print process("FREQ=YEARLY;BYMONTH=10;BYDAY=1SA")."\n";
#exit 0;

$now=int(time2str("%Y%m%d",time()));

my %hash = ();

$mode="none";

%fin_dstart=(
   "DAYLIGHT" => -1,
   "STANDARD" => -1,
   "none" => -1,
);
%fin_rdate=(
   "DAYLIGHT" => [],
   "STANDARD" => [],
);
%fin_rrule=(
   "DAYLIGHT" => "",
   "STANDARD" => "",
);
%fin_tzfrom=(
   "DAYLIGHT" => "",
   "STANDARD" => "",
);
%fin_tzto=(
   "DAYLIGHT" => "",
   "STANDARD" => "",
);
$fin_isdaylight=0;

$cur_dstart=-1;
@cur_rdate=();
$cur_rrule="";
$cur_tzfrom="";
$cur_tzto="";

$loc="";

#open(INFO, ">>/tmp/ical2gg");
#print INFO "ical2gg started\n";

LINE: while(defined($line=<>)) 
{
  @fl=split(/:/,$line);
  $_=$fl[0];
  if(/X-LIC-LOCATION/) {
    $loc=trim($fl[1]);
    next LINE;
  }
  $_=$mode;
  if($fl[0] eq "BEGIN")  {
    $mode=trim($fl[1]);
    if($mode eq "DAYLIGHT") { $fin_isdaylight=1; }
    next LINE;
  }
  if($fl[0] eq "END")
  {
    #print $cur_dstart."\n";
    if($fin_dstart{$mode}<$cur_dstart&&$now>=$cur_dstart)
    {
      #print "cs ".$cur_rrule." [".$mode."]\n";
      $fin_dstart{$mode}=$cur_dstart;
      $fin_rrule{$mode}=$cur_rrule;
      @{$fin_rdate{$mode}}=@cur_rdate;
      $fin_tzfrom{$mode}=$cur_tzfrom;
      $fin_tzto{$mode}=$cur_tzto;
      #print "cs tzfrom=".parse_offset($cur_tzfrom)." tzto=".parse_offset($cur_tzto)."\n";
    }
    $cur_dstart=-1;
    @cur_rdate=();
    $cur_tzfrom="";
    $cur_tzto="";
    $mode="none";
    next LINE;
  } 
  if(/DAYLIGHT/ || /STANDARD/)
  {
    $_=trim($fl[0]);
    if(/RDATE/) { 
      push(@cur_rdate,trim($fl[1])); 
    }
    elsif(/DTSTART/) 
    { 
      $cur_dstart=int(substr $fl[1],0,8);
    }
    elsif(/RRULE/) {
      $cur_rrule=trim($fl[1]);
    }
    elsif(/TZOFFSETFROM/) {
      $cur_tzfrom=trim($fl[1]);
    }
    elsif(/TZOFFSETTO/) {
      $cur_tzto=trim($fl[1]);
    }
  }
}   

if(0==$fin_isdaylight) {
  print "0---------\n";
} 
elsif($fin_tzfrom{'STANDARD'} eq $fin_tzto{'STANDARD'}) 
{
  print "0---------\n";
}
elsif(length $fin_rrule{'STANDARD'}>3)
{
  $_=$fin_rrule{'DAYLIGHT'};
  if(/BYDAY/)
  {
  $_=$fin_rrule{'STANDARD'};
  if(/BYDAY/) { 
    $when="c"; # a=0 b=1 c=2
    if($loc=~/America/) { $when="b"; }
    $std = parse_offset($fin_tzto{'STANDARD'});
    $day = parse_offset($fin_tzto{'DAYLIGHT'});
    $save = $day - $std;
    #print "std:".$std." day:".$day." save:".$save."\n";
    die "unsupported DST adjustment" unless (0 == ($save % 30));
    $savestp = abs( $save / 30);
    if($save<0) {
      print chr(ord('a')+int($savestp)-1);
    } elsif($save>0) {
      print chr(ord('0')+int($savestp));
    } else { print "0"; }
    print $when.process($_).process($fin_rrule{'DAYLIGHT'})."\n"; 
  }
  else { print "0---------\n"; }
  }
  else  { print "0---------\n"; }
}
else
{
  print "0---------\n";
}

#close(INFO);
