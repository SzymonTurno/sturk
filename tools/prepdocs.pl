#!/usr/bin/env perl

use File::Basename;

package CommentProc;
sub new {
	my $class = shift;
	my $self = {
	    commented => 0
	};

	bless $self, $class;
	return $self;

} sub prep {
	my ($self, $text) = @_;
	my $size = length($text);
	my $i = 0;
	my $out = "";

	while ($i <= $size) {
		if ($self{commented} && rindex($text, "-->", $i) == $i) {
			$self{commented} = 0;
			$i += length("-->");
		} elsif (rindex($text, "<!--!", $i) == $i) {
			$self{commented} = 1;
			$i += length("<!--!");
		}
		$i += 1;
		if ($i <= $size) {
			$out .= substr($text, $i - 1, 1);
		}
	}
	return $out;

}

package AlertProc;
sub new {
	my $class = shift;
	my $self = {
		enabled => 0
	};

	bless $self, $class;
	return $self;

} sub trim {
	my $s = shift;

	$s =~ s/^\s+|\s+$//g;
	return $s;

} sub prep {
	my ($self, $line) = @_;
	my %alerts = (
		'> [!note]',
		'> [!tip]',
		'> [!important]',
		'> [!warning]',
		'> [!caution]'
	);

	if (exists($alerts{trim($line)})) {
		$self{enabled} = 1;
		$line =~ s/^> \[!/@/;
		$line =~ s/\]//;
	} elsif ($self{enabled} && index($line, "> ") == 0) {
		$line =~ s/^> //;
	} else {
		$self{enabled} = 0;
	}
	return $line;

}

package main;
sub preprocess {
	my $file = shift;
	my $out = "";
	my @procs = (
		CommentProc->new(),
		AlertProc->new()
	);

	open my $info, $file or die $!;
	while (my $line = <$info>) {
		foreach my $proc (@procs) {
			$line = $proc->prep($line);
		}
		$out .= $line;
	}
	close $info;
	return $out;

} sub main {
	my $tools_dir = dirname(__FILE__);
	my $sturk_dir = (fileparse($tools_dir))[1];
	my @files = `find $sturk_dir -name README.md`;

	chomp @files;
	foreach my $file (@files) {
		$out = preprocess($file);
		open(fd, '>', $file) or die $!;
		print fd $out;
		close(fd);
	}

} main();
