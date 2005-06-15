#! /usr/bin/perl -w

#############################################################################
##
##  This file is part of CuteFig project
##
##  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.de
##
##  This program is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License version 2
##  as published by the Free Software Foundation.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##############################################################################


my $layoutfile = "layout.tmpl";
my $menufile = "menu.lst";
my $tmplfile = $ARGV[0];

use HTML::Template;

open MENU, "<$menufile";
my @menustrings;
while (<MENU>) {
	chop;
	push @menustrings, $_;
}
close MENU;

select STDERR;
$currenttmpl = $tmplfile;
$currenttmpl =~ s/\.tmpl//;
print "$currenttmpl\n";
my @menu = parsemenufile( 1, 0 );
pop @menu;


my $template = HTML::Template->new( filename => $layoutfile );

$template->param(menuloop => \@menu );

my $content;
while (<>) {
	next if /^#/;
	if ( /^\@([a-z]*): (.*)/ ) {
		$template->param( $1 => $2 );
	} else {
		$content .= $_;
	}
}

$template->param(content => $content);
$template->param(date => timestamp() );

select STDOUT;
print $template->output;


my $index;
sub parsemenufile {
	my $level;
	($level, $index) = @_;
	my $newlevel = $level;
	my $activeentryfound;
	my @menuarray;

	my @fields = split ':', $menustrings[$index];
	my $tmpl = pop @fields;
	$newlevel = @fields;
	while( $newlevel >= $level) {
		print "$level $newlevel $index $menustrings[$index]\n";
		my $nextlevel = 0;
		if (@menustrings gt $index+1) {
			my @nextarray = split ':', $menustrings[$index+1];

			print "splitting $menustrings[$index+1]\n";
			$nextlevel = @nextarray - 1;
		}

		print "$nextlevel\n";

		my @submenu;
		my $entryfound;
		if ($nextlevel > $newlevel) {
			@submenu = parsemenufile( $nextlevel, ++$index );
			$entryfound = pop @submenu;
#			$index += @submenu-1;
		} else {
			$index++;
			$nextlevel = 1;
		}

		my $entry;
		my $endtag;
		for ( my $i=1; $i<$newlevel; $i++ ) {
			$entry.="&nbsp;&nbsp;";
		}
		$entry .= "--&nbsp;" if $newlevel > 1;
		print "$tmpl $currenttmpl\n";
		if ( $tmpl eq $currenttmpl ) {
			$entry .= "<b>";
			$endtag = "</b>";
			$activeentryfound = 1;
		} else {
			$entry .= "<a href=\"$tmpl.html\">";
			$endtag = "</a>";
		}
		$entry .= pop @fields;
		$entry .= $endtag;
		print "$entry\n";

		my %menuentry;
		$menuentry{entry} = $entry;

		push @menuarray, \%menuentry;
		push @menuarray, @submenu if $entryfound || $tmpl eq $currenttmpl;
		print "end $index $newlevel $level $nextlevel\n";

		if (@menustrings > $index) {
			@fields = split ':', $menustrings[$index];
			$tmpl = pop @fields;
			$newlevel = @fields;
		} else {
			$newlevel = 0;
		}
	}
	print "return\n";
	return (@menuarray, $activeentryfound);
}


use File::stat;
sub timestamp {
	@months = ("January","February","March","April","May","June","July",
		   "August","September","October","November","December");

	my $st = stat($tmplfile);
	my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime $st->ctime;

	my $ret = sprintf("%s %2d. %4d", $months[$mon], $mday, $year+1900);
	return $ret;
}
