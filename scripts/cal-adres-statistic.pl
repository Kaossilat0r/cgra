#!/usr/bin/perl

use strict;
use warnings;
use POSIX;

my $dresc_energy_file = "/opt/dresc/share/adres_energy_statistic";
#$dresc_energy_file = "./adres_energy_statistic";

my $statout = $ARGV[0];
my $arch = $ARGV[1];
my $extended = 0;

if (@ARGV < 2) {
	die "Not enough parameters! Call with parameters: <stat.out-path> <dresc_arch-path>";
}
if (!-s $statout) {
	die "Statistics file does not exist or is empty!";
}
if (!-s $arch) {
	die "Arch file does not exist or is empty!";
}

if (@ARGV >= 3) {
	if ($ARGV[2] eq "-e") {
		$extended = 1;
	}
}

my @blocks = ();
my %instructions;
my @sections = ();
my @cycles = ();
my @cgacycles = ();
my @weight = ();
my @reads = ();
my @writes = ();
my @nops = ();
my @nopPercent = ();
my @energy = ();
my $cgraEnergy = 0;
my $fuType;
my $fuCount;

# Build instruction table
if ( -e $dresc_energy_file ) {
	open (my $fh, "<", $dresc_energy_file) or die "Can not open energy file ($dresc_energy_file)!";
	while (<$fh>) {
		if (length($_) > 1) {
			if ($_ =~ m/(\w+):(\d+(\.\d+)?) (\d+)/o) {
				my %entry;
				$entry{name} = $1;
				$entry{energyPerCycle} = $2;
				$entry{cyclesPerInstruction} = $4;
				$entry{energyPerInstruction} = $2 * $4;
				$entry{usage} = [];
				$instructions{$1} = \%entry;
			} else {
				print "Did not match \"$_\"\n";
			}
		}
	}
	close $fh;
} else {
	die "Energy info file ($dresc_energy_file) does not exist!"
}

# Read CGRA FU setup
open (my $fh, "<", $arch) or die "Can not open arch file ($arch)!";
while (<$fh>) {
	if ($_ =~ m/instance: ((\d+)x(\d+))/o) {
		$fuType = $1;
		$fuCount = $2 * $3;
		last;
	}
}
close $fh;



# Parse statistics
my $section = "";
my $use = 0;
my $curEnergy = 0;

sub startSection {
	my $name = shift;
	push(@sections, $name);
	$use = 1;
	$curEnergy = 0;
	foreach my $name (sort keys %instructions) {
		push($instructions{$name}->{usage}, 0);
	}
}

open ($fh, "<", $statout) or die "Can not open statistics file ($statout)!";
while (<$fh>) {
	if ($_ =~ m/\*\* ([\w ]+) \*\*/o) {
		$section = $1;
		if ($section eq "Overall") {
			&startSection("Total");
		} elsif ($section =~ m/Cga loop DRESC_(\w+)_cga_(\d+)/o) {
			&startSection("$1 \@$2");
		} else {
			$use = 0;
		}
	}
	elsif ($use) {
		if ($_ =~ m/Total (\w+)\.ins = (\d+)/o) {
			if ($2 > 0) {
				if ($instructions{$1}) {
					$instructions{$1}->{usage}[-1] = $2;
					$nops[-1] -= $2 * $instructions{$1}->{cyclesPerInstruction};
					$curEnergy += $2 * $instructions{$1}->{energyPerInstruction};
				} else {
					print "WARNING: No energy data for instruction: $1. Ignoring\n";
				}
			}
		} elsif ($_ =~ m/Total cycles = (\d+)/o) {
			push(@nops, $1 * $fuCount);
			push(@cycles, $1);
		} elsif ($_ =~ m/Total cga cycles = (\d+)/o) {
			push(@cgacycles, $1);
		} elsif ($_ =~ m/Weight = (\d+\.\d*)/o) {
			push(@weight, $1);
		} elsif ($_ =~ m/Total reads = (\d+)/o) {
			push(@reads, $1);
		} elsif ($_ =~ m/Total writes = (\d+)/o) {
			$instructions{"nop"}->{usage}[-1] = $nops[-1];
			$curEnergy += $nops[-1] * $instructions{"nop"}->{energyPerInstruction};
			push(@writes, $1);
			push(@energy, sprintf("%10.0f", $curEnergy));
		}
	}
}
close $fh;

for my $i (0 .. $#sections) {
	$nopPercent[$i] = sprintf "%5.1f%%", 100 * $nops[$i] / ($cycles[$i] * $fuCount);
}
for my $i (1 .. $#sections) {
	$cgraEnergy += $energy[$i];
}



if ($extended == 0) {
	@sections = $sections[0];
}

&printInstructionStatistics;
&printGeneralStatistics;



# Fit string in given width, either centered if shorter or shortened
# with an ellipsis (...) in the middle if longer
sub fitString {
	my $name = shift;
	my $reqLen = shift;

	if (length($name) == $reqLen) {
		return $name;
	} elsif (length($name) < $reqLen) {
		my $pad = $reqLen - length($name);
		my $frontChars = ceil($pad / 2);
		my $backChars = floor($pad / 2);
		return " "x$frontChars . $name . " "x$backChars;
	} else {
		my $charsToShow = $reqLen - 3;
		my $frontChars = floor($charsToShow / 2) - 1;
		my $backChars = ceil($charsToShow / 2) + 1;
		return substr($name, 0, $frontChars) . "..." . substr($name, length($name) - $backChars);
	}
}


# Print statistics table for single instructions
sub printInstructionStatistics {
	my @colwidths = (10, 4, 7);
	my @colwidthsRepeat = (7, 11);

	local *printDelimiter = sub {
		printf "+-%s-+-%s-%s-+", "-"x$colwidths[0], "-"x$colwidths[1], "-"x$colwidths[2];
		for my $i (0 .. $#sections) {
			printf "-%s-%s-+", "-"x$colwidthsRepeat[0], "-"x$colwidthsRepeat[1];
		}
		print "\n";
	};

	local *printHeader = sub {
		printf "| %-$colwidths[0]s | %+$colwidths[1]s %+$colwidths[2]s |", "Ins", "Clk/", "Energy/";
		for my $i (0 .. $#sections) {
			printf " %+" . ($colwidthsRepeat[0] + $colwidthsRepeat[1] + 1) . "s |", &fitString($sections[$i], ($colwidthsRepeat[0] + $colwidthsRepeat[1] + 1));
		}
		print "\n";

		printf "| %-$colwidths[0]s | %+$colwidths[1]s %+$colwidths[2]s |",, "", "Ins", "Clk";
		for my $i (0 .. $#sections) {
			printf " %+$colwidthsRepeat[0]s %+$colwidthsRepeat[1]s |", "Num", "Energy";
		}
		print "\n";
	};

	local *printInstrLine = sub {
		my $instr = shift;

		printf "| %-$colwidths[0]s | %+$colwidths[1]s %$colwidths[2].4f |", $instr->{name}, $instr->{cyclesPerInstruction}, $instr->{energyPerCycle};
		for my $i (0 .. $#sections) {
			printf " %$colwidthsRepeat[0]s %$colwidthsRepeat[1].1f |", , $instr->{usage}[$i], $instr->{cyclesPerInstruction} * $instr->{energyPerCycle} * $instr->{usage}[$i];
		}
		print "\n";
	};

	&printDelimiter;
	&printHeader;
	&printDelimiter;
	foreach my $name (sort keys %instructions) {
		if ($instructions{$name}->{usage}[0] > 0) {
			&printInstrLine($instructions{$name});
		}
	}
	&printDelimiter;
}


# Print statistics table for general information
sub printGeneralStatistics {
#print "\nType: $fuType, count: $fuCount\n\n";

	my @colwidths = (13);
	my @colwidthsRepeat = (19);

	local *printDelimiter = sub {
		printf "+-%s-+", "-"x$colwidths[0];
		for my $i (0 .. $#sections) {
			printf "-%s-+", "-"x$colwidthsRepeat[0];
		}
		print "\n";
	};

	local *printHeader = sub {
		printf "| %-$colwidths[0]s |", "Section";
		for my $i (0 .. $#sections) {
			printf " %+$colwidthsRepeat[0]s |", &fitString($sections[$i], $colwidthsRepeat[0]);
		}
		print "\n";
	};

	local *printLine = sub {
		my $caption = shift;
		my $data = shift;

		printf "| %-$colwidths[0]s |", $caption;
		for my $i (0 .. $#sections) {
			if (@{$data}[$i]) {
				printf " %$colwidthsRepeat[0]s |", @{$data}[$i];
			} else {
				printf " %$colwidthsRepeat[0]s |", " "x$colwidthsRepeat[0];
			}
		}
		print "\n";
	};

	&printDelimiter;
	&printHeader;
	&printDelimiter;
	&printLine("FU setup", [$fuType]);
	&printLine("Cycles", \@cycles);
	&printLine("CGRA cycles", \@cgacycles);
	&printLine("NOPs", \@nops);
	&printLine("NOP %", \@nopPercent);
	&printLine("Reads", \@reads);
	&printLine("Writes", \@writes);
	&printLine("Total Energy", \@energy);
	&printLine("CGRA Energy", [$cgraEnergy]);
	&printDelimiter;
}

