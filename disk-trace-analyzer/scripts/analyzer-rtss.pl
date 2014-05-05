#!/usr/bin/env perl

use strict;
use warnings;

# Current state of the trace
my $cur_hour = 0;

# Tracking write intervals between within a flash block
my $BLOCK_SIZE = 256*1024; # block size = 256 KB
my $cur_page = 0; # currently written bytes within a block
my $block_start = 0; # start hour


# Tracking retention time
my $PAGE_SIZE = 512;
my @last_write = (0) x ;

# Statistics
my @write_intervals = ();

my $dir = '../ds-bzip2/';

opendir(DIR, $dir) or die $!;

my @dates 
= grep { 
/^1999/             # Begins with a period
#/.cpp$/            # Ends with a .pl
#&& -f "$dir/$_"   # and is a file
} readdir(DIR);

closedir(DIR);

my @sorted_dates = sort(@dates);

# Loop through the array printing out the filenames
foreach my $folder (@sorted_dates) {
  #print "$folder\n";
  $folder = $dir.$folder."/";

  opendir(DIR, $folder) or die $!;
  my @hours = grep {/-bz2.ds.out$/} readdir(DIR);
  closedir(DIR);

  my @sorted_hours = sort(@hours);

  foreach my $file (@sorted_hours) {
    #print "$file\n";
    $file = $folder.$file;
    #print "$file\n";
    open(FILE, $file) or die $!;
    while (<FILE>) {
      chomp;
      my $line = $_;
      print "$line\n";
      my @fields = split(/\ /);
        my $type = $fields[-1];
        my $size = $fields[-2];
        my $block = $fields[-3];
      if ($type eq "T") {
        # Is Write
        if ($cur_page == 0) {
          $block_start = $cur_hour;
        }
        $cur_page += $size;
        if ($cur_page >= $BLOCK_SIZE) {
          # Wrap around
          push(@write_intervals, $cur_hour - $block_start + 1);
          $cur_page %= $BLOCK_SIZE;
          if ($cur_page > 0) {
            $block_start = $cur_hour;
          }
        }
        if ($size % $PAGE_SIZE != 0) {
          # assertion
          print "$line: has size $size\n";
          exit -1;
        }
        while ($size > 0) {
          print $block;
          $last_write[$block] = $cur_hour;
          $size -= $PAGE_SIZE;
          $block += $PAGE_SIZE;
        }
      } elsif ($type eq "F") {
        # Is Read
        while ($size > 0) {
          my $write_time = 0;
          print $block;
          if (defined($last_write[$block])) {
            $write_time = $last_write[$block];
          }
          my $retention = $cur_hour - $write_time;
          print $retention;
          $size -= $PAGE_SIZE;
          $block += $PAGE_SIZE;
        }
      }
    } # end FILE
    close(FILE);
    $cur_hour++;
    last;
  } # end files
  last;
} # end folders

# Print statistics
local $" = "\n";
#print "write interval: \n@write_intervals\n";

exit 0;
