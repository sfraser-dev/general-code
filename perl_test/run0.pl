#!/usr/bin/perl
use strict;
use warnings;
while (<>) {    # Read input into default variable $_
    print $_;
    s/in/IN/g;
    print $_;   
}
