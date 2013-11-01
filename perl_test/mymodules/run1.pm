use strict;
use warnings;

package mymodules::run1;

sub zombify {
    my $word = shift @_;
    $word =~ s/[aeiou]/r/g;
    return $word;
}
return 1;
