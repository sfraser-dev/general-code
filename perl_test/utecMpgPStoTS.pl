#!/usr/bin/perl -w
use strict;
use warnings;
use feature qw(say);
use File::Find; 
use File::Basename;

my $name;
my $dir;
my $ext;
my $tsName;
my $mpgOrig;
my @content;

# find recursively from this directory
find( \&mpgWanted, '.');
foreach my $mpgName (@content) {
    ($name,$dir,$ext) = fileparse($mpgName,'\..*');
    $tsName = $dir . $name . ".ts";
    
    # no PTS generated, just convert container from PS to TS
    #system("ffmpeg -y -i $mpgName -c:v copy -c:a copy -f mpegts $tsName");

    # convert .mpg from PS to TS and generate PTS
    system("ffmpeg -fflags +genpts -y -i $mpgName -c:v copy -c:a copy -f mpegts $tsName");
   
    # cleanup 
    #system("rm $tsName");
}

exit;

# subroutie to recursively find all files with ".mpg" extension
sub mpgWanted {
    if ($File::Find::name =~ /.mpg/){
        push @content, $File::Find::name;
    }
    return;
}
