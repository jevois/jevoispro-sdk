#!/usr/bin/perl

open F, "IMX290_regs.h";
while ($line = <F>) {
    chomp $line;
    next if $line !~ m/^#define IMX290_REG_/;
    @data = split(/\s+/, $line);
    $reg{$data[2]} = $data[1];
}
close F;

open F, $ARGV[0];
while ($line = <F>) {
    foreach $r (keys(%reg)) {
        $n = $reg{$r};
        $line =~ s/\{\s*$r,/\{ $n,/g;
        $r = "0x" . uc(substr($r, 2));
        $line =~ s/\{\s*$r,/\{ $n,/g;
    }
    print $line;
}
close F;

    
