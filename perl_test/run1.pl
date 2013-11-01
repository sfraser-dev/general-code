#!/usr/bin/perl
use strict;
use warnings;
use feature qw(say);

# "perl in 2.5 hours"
# perl is context-sensitive, every expression is evaluated  either in scalar or kist context
# variables are either $scalars (string, number, reference or undef)
# lists are either @arrays (list of scalars indexed by integers) or %hashes (list of scalars indexed by strings)

say "Hello, World";
say"";

my @array = ("print", "these", "strings", "out", "for", "me");
say @array;
say"";

foreach my $i (@array) {
    say $i;
}
say"... reversing ...";

for(my $i=-1; $i>=-6; $i--){
    say $array[$i];
}

say"this array has ".(scalar @array)." elements";
say"the last populated element is ".$#array;
say"";

say"john\@fake.com";
say"";

my %scientists = (
    "Newton" => "Isaac", 
    "Einstein" => "Albert",
    "Darwin" => "Charles",
);

foreach my $key (sort keys %scientists) {
    say $key, ": ", $scientists{$key};
}
say"";

my %owner1 = (
    "name" => "Santa Claus",
    "DOB" => "1882-12-25",
);
my %owner2 = (
    "name" => "Mickey Mouse",
    "DOB" => "1928-11-18",
);
# creating references with backslash
my $owner1ref = \%owner1;
my $owner2ref = \%owner2;
# cannot have arrays / hashes with nested arrays/ hashes, use references instead
my @owners = ($owner1ref, $owner2ref);
my %account = (
    "number" => "12345678",
    "opened" => "2000-01-01",
    "owners" => [ $owner1ref, $owner2ref ],
);

say "Account #", $account{"number"};
say "Opened: ", $account{"opened"};
say "Owner: ", $account{"owners"}->[0]->{"name"}, "\tDOB: ", $account{"owners"}->[0]->{"DOB"};
say "Owner: ", $account{"owners"}->[1]->{"name"}, "\tDOB: ", $account{"owners"}->[1]->{"DOB"};
say"";
                    

my @stack = ("Fred", "Eileen", "Denise", "Charlie");
say @stack;
say pop @stack;
say @stack;
push @stack, "Bob", "Alice";
say @stack;
say shift @stack;
say @stack;
unshift @stack, "Hank", "Grace";
say @stack;
say splice(@stack, 1, 4, "<<<", ">>>");
say @stack;
say"";

my @elements = ("Antimony", "Arsenic", "Aluminium", "Selenium");
say @elements;
say "@elements";
say join(", ", @elements);
say"";

say reverse ("Hello", "World");
say reverse ("HelloWorld");
say scalar reverse ("HelloWorld");
say scalar reverse ("Hello", "World");
say"";

my @capitals = ("Rome", "Paris", "London", "Berlin");
say @capitals;
say join (", ", @capitals);
say join (", ", map { uc $_ } @capitals);
say join (", ", map { lc $_ } @capitals);
say"";

say join ", ", grep { length $_ == 4 } @capitals;
say scalar grep { $_ eq "London" } @capitals;
say"";

my @elevations = (19, 1, 2, 100, 3, 98, 100, 1056);
say join ", ", sort @elevations;
# $a and $b are global variables (like $_). Perl knows instinctively the best sorting algorithm to use
say join ", ", sort { $a cmp $b } @elevations; # string sort
say join ", ", sort { $a <=> $b } @elevations; # number sort
say"";

# subroutines
sub hyphenate {
    # subtract the first argument from the array, ignore everything else
    my $word = shift @_;
    # an overly clever list comprehension
    $word = join "*", map { substr $word, $_, 1 } (0 .. (length $word)-1);
    return $word;
}
say hyphenate("exterminate");
say"";

# perl calls by reference
my $x = 7;
sub reassign {
    $_[0] = 42;
}
reassign($x);
say "x=$x";
say"";

# unpacking arguments 
# unpack @_ entry by entry
sub left_pad0 {
    my $oldString = $_[0];
    my $width = $_[1];
    my $padChar = $_[2];
    my $newString = ($padChar x ($width - length $oldString)) . $oldString;
    return $newString;
}
say left_pad0("hello",10,"+");

# unpack @_ by removing data via shift
sub left_pad1 {
    my $oldString = shift @_;
    my $width = shift @_;
    my $padChar = shift @_;
    my $newString = ($padChar x ($width - length $oldString)) . $oldString;
    return $newString;
}
say left_pad1("guten tag",10,"+");

# unpack via shift (@_ inferred)
sub left_pad2 {
    my $oldString = shift;
    my $width = shift;
    my $padChar = shift;
    my $newString = ($padChar x ($width - length $oldString)) . $oldString;
    return $newString;
}
say left_pad1("bonjour",10,"+");

# unpack @_ all in one go 
sub left_pad3 {
    my ($oldString, $width, $padChar) = @_;
    my $newString = ($padChar x ($width - length $oldString)) . $oldString;
    return $newString;
}
say left_pad1("hola",10,"+");

# for large number of arguments, best practice is for the user to provide a hash
sub left_pad4 {
    my %args = @_;
    my $newString = ($args{"padChar"} x ($args{"width"} - length $args{"oldString"})) . $args{"oldString"};
    return $newString;
}
say left_pad1("ciao",10,"+");
say"";

# return values via context (scalar or list) 
sub contextSub {
    # caller wants a list, return a list
    return ("Everest", "K2", "Etna") if wantarray;
    # caller wants a scalar, return a scalar
    return 3;
}

my @theArray = contextSub();
say "@theArray";
my $theScalar = contextSub();
say $theScalar;
say"";

# file handling
my $fin = "twostrings.txt";
open (my $fh_in, "<", $fin) || die "Couldn't open '".$fin."' for reading because: ".$!;
while(my $line = <$fh_in>) {
    chomp $line;
    say "$line";
}
my $fout= "output.txt";
open (my $fh_out, ">", $fout) || die "Couldn't open '".$fout."' for reading because: ".$!;
print $fh_out "Houston, we have a problem";
close $fh_in;
close $fh_out;
say"";

# regex
my $theStr = "Hello   world";
if($theStr =~ m/(\w+)(\s{1,4})(\w+)/) {
    say "success";
}
else {
    say "failed";
}
# parentheses perform sub-matches. after a successful match, the sub-matches get put into
# $1, $2, $3 etc
say $1;
say $2;
say $3;
# list context
my $string = "colourless green ideas sleep furiously";
my @matches = $string =~ m/(\w+)\s+((\w+)\s+(\w+))\s+(\w+)\s+(\w+)/;
say join ", ", map { "'".$_."'" } @matches;

$string = "Good morning world";
$string =~ s/world/Vietnam/;
say $string;

# /g indicates a group match
# /g in scalar context, return the matches one at a time
$string = "a tonne of feathers or a tonne of bricks";
while ($string =~ m/(\w+)/g) {
    say "'".$1."'\n";
}

# /g in list context, return the matches all at once
@matches = $string =~ m/(\w+)/g;
say join ", ", map { "'".$_."'" } @matches;

# an =~ s///g call performs a global search/replace and returns the number of matches
# try without /g
$string =~ s/[aeiou]/r/;
say $string;
# once more
$string =~ s/[aeiou]/r/;
say $string;
# and do the rest using /g
$string =~ s/[aeiou]/r/g;
say $string;
# /i makes matches and substitutions case insensitive
# /x allows regex to contain white spaces (ie: line breaks)
"Hello world" =~ m/
    (\w+)   # one or more word characters
    ([ ])     # sigle literal space, stored inside character class
    (world)   # literal "world"
/x;
say $1;
say $2;
say $3;

