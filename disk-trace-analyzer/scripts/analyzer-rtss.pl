#!/usr/bin/env perl

use strict;
use warnings;

my $dir = '../ds-bzip2/';

opendir(DIR, $dir) or die $!;

my @dates 
= grep { 
/^1999/             # Begins with a period
#/.cpp$/            # Ends with a .pl
#&& -f "$dir/$_"   # and is a file
} readdir(DIR);

closedir(DIR);

my @sortedDates = sort(@dates);

# Loop through the array printing out the filenames
foreach my $folder (@sortedDates) {
  #print "$folder\n";
  $folder = $dir.$folder."/";

  opendir(DIR, $folder) or die $!;
  my @hours = grep {/-bz2.ds.out$/} readdir(DIR);
  closedir(DIR);

  my @sortedHours = sort(@hours);

  foreach my $file (@sortedHours) {
    #print "$file\n";
    $file = $folder.$file;
    #print "$file\n";
    open(FILE, $file) or die $!;
    while (<FILE>) {
      chomp;
      my $line = $_;
      print "$line\n";
      my @fields = split(/\ /);

      #debug
      exit 0;
    }
  }
}

exit 0;
