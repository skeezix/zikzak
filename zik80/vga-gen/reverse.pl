#!/usr/bin/perl

open FILE, "screen-font.h" ||
    die ( "Couldn't open: $!\n" );

while ( $inline = <FILE> ) {
    chomp $inline;

    if ( $inline =~ /.+0x.+/ ) {
        # 0x12, 0x34, ...
        $inline =~ s/^\s+|\s+$//g;
        #print "hex $inline\n";
        print "\t";
        foreach $hex ( split ( ',', $inline ) ) {
            $reversed = 255 - hex ( substr ( $hex, 2 ) );
            #print "$hex\t", $reversed, "\n";                 # debug
            print sprintf ( "0x%02X", $reversed ), ", ";       # actual
        }
        print "\n";
    } else {
        # regular line
        print "$inline\n";
    }

}

close FILE;
