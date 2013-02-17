#! /usr/bin/ksh -- # This comment tells perl not to loop!
#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)h_conv.sh	9.2	07/10/09 SMI"
#

eval "/bin/cat > /dev/null" << '__END__'

if 0;

use 5.005;
use strict;

######### Start of perl script #########

my $cmd_HELP			= 'help';
my $cmd_STDOUT			= 'stdout';
my $cmd_OUTDIR			= 'dir_out';
my $cmd_FILELIST		= 'in_file';
my $cmd_OUT_FILE		= 'out_file';
my $cmd_OUT_PREFIX		= 'out_prefix';
my $cmd_MODE			= 'mode';
my $cmd_SUNSOL			= 'solaris';

my $mode_CP			= 'copy';
my $mode_CONVERT		= 'convert';

my $STDOUT_FILE			= 'stdout';

my $MEDIALIB_CLOSE_OLD_NAMES	= '__MEDIALIB_OLD_NAMES';		# Do Not Edit!
my $MEDIALIB_DOUBLE_PROTO	= '__MEDIALIB_OLD_NAMES_ADDED';

my $NAME_PREF			= '__';

my $PRG_NAME = $0;
$PRG_NAME =~ s,^.*/([^/]+)$,$1,o;

Main();

###############################################################
sub Main
{
	my $file;
	my %file_list = ();

	my %cmd_line = (
		$cmd_OUTDIR		=> [ '-d'	, '.', '-d <dir>	', 'Set directory for output files to <dir>' ],
		$cmd_OUT_FILE		=> [ '-o'	, '' , '-o <outputfile>	', 'Set name of output file to <output_file>' ],
		$cmd_STDOUT		=> [ '-c'	, 0  , '-c		', 'Write output on standard output' ],
		$cmd_HELP		=> [ '-h[elp]*'	,    , '-h		', 'Print help (this message)' ],
		$cmd_OUT_PREFIX		=> [ '-p'	, '' , '-p <PREFIX>	', 'Set prefix for include files' ],
		$cmd_SUNSOL		=> [ '-sol'	, 0  , '-sol		', 'Set parameters for Solaris Common Build Environment (CBE)' ],
		$cmd_FILELIST		=> [],
		$cmd_MODE		=> $mode_CP,
	);

	ParseCmdLine( \@ARGV, \%cmd_line );

	Msg(2, 'Ambiguous reference between input and output files.')
		if ( $cmd_line{$cmd_OUT_FILE}->[1] && $#{$cmd_line{$cmd_FILELIST}} != 0);

	if ( $cmd_line{$cmd_OUT_FILE}->[1] ) {
		$file_list{${$cmd_line{$cmd_FILELIST}}[0]} = $cmd_line{$cmd_OUT_FILE}->[1];
	}
	else {
		foreach $file ( @{$cmd_line{$cmd_FILELIST}} ) {
			my $new_name;

			if ( $cmd_line{$cmd_STDOUT}->[1] ) {
				$file_list{$file} = $STDOUT_FILE;
				next;
			}

			($new_name = $file ) =~ s/.*(mlib_\w+proto\.h)$/$1/o;
			$file_list{$file} = "$cmd_line{$cmd_OUTDIR}->[1]/$new_name";
	  	}
	}

	foreach $file ( keys %file_list ) {
		ModeDispath($file, $file_list{$file},
			    $cmd_line{$cmd_MODE},
			    $cmd_line{$cmd_OUT_PREFIX}->[1],
			    $cmd_line{$cmd_SUNSOL}->[1] );
	}
}

###############################################################
sub ParseCmdLine
{
	my ( $__argv, $__cmd_line ) = @_;
	my @argv = @$__argv;

	Msg(0, $__cmd_line) if ( $#argv < 0);

	while ( @argv ) {
		my $line = shift @argv;

		if ( -f $line && $line !~ /^-/o ) {
			push (@{$$__cmd_line{$cmd_FILELIST}}, $line);

                	$$__cmd_line{$cmd_MODE} = $mode_CONVERT
				if ( $line =~ /.*mlib_\w+_proto\.h$/o );
		}
		elsif( $line =~ /^-*$$__cmd_line{$cmd_HELP}->[0]$/o ) {
			@{$$__cmd_line{$cmd_FILELIST}} = ();
			Msg(0, $__cmd_line);
		}
		elsif ( $line =~ /^-*$$__cmd_line{$cmd_STDOUT}->[0]$/o ) {
			$$__cmd_line{$cmd_STDOUT}->[1] = 1;
		}
		elsif ( $line =~ /^-*$$__cmd_line{$cmd_OUT_FILE}->[0]$/o ) {
			my $arg = shift @argv;
			my $dir = $arg;

			Msg(0, $__cmd_line) unless $arg;

			$dir =~ s/\/?[\w\.\s]+$//o;
			Msg(2, $dir . ': No such file or directory') if ( ! -d "$dir/./" );
			$$__cmd_line{$cmd_OUT_FILE}->[1] = $arg;
		}
		elsif ( $line =~ /^-*$$__cmd_line{$cmd_OUTDIR}->[0]$/o ) {
			my $arg = shift @argv;
			Msg(0, $__cmd_line) unless $arg;
			Msg(2, $arg . ': No such file or directory') if ( ! -d $arg );

			$$__cmd_line{$cmd_OUTDIR}->[1] = $arg;
		}
		elsif ( $line =~ /^-*$$__cmd_line{$cmd_SUNSOL}->[0]$/o ) {
			$$__cmd_line{$cmd_OUT_PREFIX}->[1] = '__';
			$$__cmd_line{$cmd_SUNSOL}->[1] = 1;
		}
		elsif ( $line =~ /^-*$$__cmd_line{$cmd_OUT_PREFIX}->[0]$/o ) {
			my $arg = shift @argv;

			if ( !$arg ) {
				@{$$__cmd_line{$cmd_FILELIST}} = ();
				Msg(0, $__cmd_line);
			}

			$$__cmd_line{$cmd_OUT_PREFIX}->[1] = $arg
#				unless $$__cmd_line{$cmd_OUT_PREFIX}->[1]
			;
		}
		else {
			Msg(1, 'illegal option --- ' . $line);
		}
	}

	$$__cmd_line{$cmd_OUT_FILE}->[1] = ''
		if ( $$__cmd_line{$cmd_STDOUT}->[1] );
}

###############################################################
sub ModeDispath
{
	my ( $in , $out, $mode, $prefix, $sol ) = @_;
	my @out = ();

	if ( $mode =~ /$mode_CONVERT/o ) {
		ConvertHeader( $in, \@out, $prefix, $sol );
	}
	elsif ( $mode =~ /$mode_CP/o ) {
		open IN, "< $in";
		@out = <IN>;
		close IN;
	}
	else {
		Msg(1, 'illegal mode --- ' . $mode );
	}

	if ( $out !~ /$STDOUT_FILE/o ) {
		unlink $out;
		open OUT, "> $out" or Msg(2, "$out: $!");;
		select OUT;
	}
	else  {
		@out = ( ':'x5 . "\t$in", @out, ':'x40 , '' , '' );
	}

	grep {	s/^(\s*#\s*include\s+<)(mlib[\w\.]+>)/$1$prefix$2/o		||
		s/^(\s*#\s*pragma\s+ident\s+.*)(mlib[\w\.]+\s+)/$1$prefix$2/o	||
		1 } @out
		if ( $prefix );

	chomp @out;
	print map { "$_\n" } @out;

	close OUT if ( $out !~ /$STDOUT_FILE/o );
}

###############################################################
sub ConvertHeader
{
	my ( $in_file, $out_file, $prefix, $sol ) = @_;
	my ( $line, @buf, @in, @out ) = ();
	my $flg_if = 0;
	my $flg_up_if = 0;
	my $comm_def = 1;
	my @comm_def_2 = (
		"#if defined(_MSC_VER)",
		"#if ! defined($MEDIALIB_CLOSE_OLD_NAMES)",
		"#define\t$MEDIALIB_CLOSE_OLD_NAMES",
		"#endif",
		"#endif",
	);

# temporary define
	my @comm_def_2_tmp = (
		"#if ! defined($MEDIALIB_CLOSE_OLD_NAMES)",
		"#define\t$MEDIALIB_CLOSE_OLD_NAMES",
		"#endif",
	);

#	my @comm_def = ( @comm_def_2_tmp , @comm_def_2 );
	my @comm_def = ( @comm_def_2 );

	@comm_def = () if ( $sol );

	open IN, "< $in_file" or Msg(2, '$in_file: ' . $!);
	@in = <IN>;
	close IN;
	chomp @in;

	my @ifndef = grep { /^\s*#\s*ifndef\s+(\w+)/o } @in;
	if ( @ifndef ) {
		@ifndef = grep { s/^\s*#\s*ifndef\s+(\w+).*$/$1/o } @ifndef;
		my $ifndef = $ifndef[0];
		chomp $ifndef;
		@in = grep { s/\b$ifndef\b/__ORIG_$ifndef/ || 1 } @in;
	}

	my $flg_inc = 0;
	my $pos = -1;
	foreach $line ( @in ) {
		$pos ++;
		if ( $line =~ /^\s*#\s*include\s+/o ||
		     $line =~ /^\s*#\s*if.*__cplusplus/o ) {
			$flg_inc ++;
			$flg_up_if = 1 if ( $line =~ /^\s*#\s*if.*__cplusplus/o );
		}
		else {
			next if ( $flg_inc == 0 );
			my $inc_name = $in_file;
			$inc_name =~ s,^.*/([^/]+)$,$1,o;

			my @old_names = (
				"#if defined($MEDIALIB_DOUBLE_PROTO)",
				"#include <../include/$inc_name>",
				"#endif",
			);

			@old_names = () if ( $prefix );

			if ( $flg_up_if ) {
				$pos --;
				push @old_names, '';
			}

			splice(@in, $pos, 0, @old_names);
			last;
		}
	}

        $flg_up_if = 0;
	foreach $line ( @in ) {

		if( $line =~ /^\s*#\s*if.*__cplusplus/o ) {
			$flg_up_if = 1 if ( $flg_if == 0 );
			$flg_if++;
		}

		if ( $line =~ /(^\s*(#|\/\/|\/\*|$)|;\s*$)/o ) {

			if ( $line !~ /;\s*$/o  ) {
				push @out, @buf if ( @buf );
				push @out, $line;
			}
			else {
				push @buf, $line;

				chomp @buf;
				my @buf_tmp = split ' ', (split '\(', (join '', @buf))[0];
				my $name = $buf_tmp[$#buf_tmp];
				$name =~ s/\*//og;
				grep { s/($name\s*\()/$NAME_PREF$1/ || 1 } @buf;

				my @insert_block = (
					"",
					"#if defined($MEDIALIB_CLOSE_OLD_NAMES)",
					"#define\t$NAME_PREF$name \\",
					"\t$name",
					"#endif",
				);
				@insert_block = ( '' ) if ( $sol );

				push @out, @insert_block, @buf;
			}
			@buf = ();
		}
		else {
			push @buf, $line;
		}

		$flg_if -- if( $line =~ /^\s*#\s*endif/o && $flg_if > 0 );

		if ( $flg_up_if && $flg_if == 0 && $comm_def ) {
			push @out, '', @comm_def;
			$flg_up_if = 0;
			$comm_def = 0;
		}
	}

	my $space = 0;
	my @in = ();
	for $line ( @out ) {
		if ( $line =~ /^\s*$/o ){ $space  ++; }
		else 			{ $space = 0; }
		push @in, $line if ( $space <= 1 );
	}

	@out =  @in;

	if ( $sol && $#out > 2 ) {
		my $i = 1;
next_iter:
		if ( $out[$i - 1] =~ /^\s*#\s*ifdef\s+__SUNPRO_C/o &&
		     $out[$i + 0] =~ /^\s*#\s*pragma\s+ident\s+/o &&
		     $out[$i + 1] =~ /^\s*#\s*endif/o ) {
			splice @out, $i + 1, 1;
			splice @out, $i - 1, 1;
			$i --;
		}
		$i ++;
		goto next_iter if ( $i < $#out );
	}

# check pragma returns_new_memory

	@in = @out;
	@out = ();
	for $line ( @in ) {
		if ( $line !~ /^\s*#\s*pragma\s+returns_new_memory/o ) {
			push @out, $line;
			next;
		}

		my $orig_line = $line;
		$line =~ s/\b(mlib_)/$NAME_PREF$1/og;

		my @insert_block = ( $line );

		if ( ! $sol ) {
			unshift @insert_block,
				"#if defined($MEDIALIB_CLOSE_OLD_NAMES)",
				$orig_line,
				"#else";
			push @insert_block,
				"#endif";
		}

		push @out, @insert_block;
	}

	@$out_file = @out;
}

###############################################################
sub Msg
{
	my ( $msgfl ) = shift;

	if( $msgfl == -1 ) {
		return;
	}

	if( $msgfl == 0 ) {
		my ( $in_param ) = @_;
		my $list;

		print "\nUsage: $PRG_NAME [options] <files> \n";

		exit 0 if( ref $in_param ne "HASH" );

		foreach $list ( keys %$in_param ) {
			next if ( ref $$in_param{$list} ne "ARRAY" );

			my $val = $#{$$in_param{$list}};
			next if ( $val == -1 );
			print "\t$$in_param{$list}->[$val - 1] -- $$in_param{$list}->[$val]\n";
		}
		exit 0;
	}

	if( $msgfl == 1 ) {
		print "\nWARNING: @_\n";
		exit 0;
	}

	if( $msgfl == 2 ) {
		print "\nFATAL ERROR: @_\n";
		exit 0;
	}

	print "\nUnknown status.\n";
	exit 0;
}

###############################################################

exit 0;

######### End of perl script   #########
__END__

##########################################################################
#  shell script goes here :
#

PATH=/bin:/usr/bin:/usr/local/bin:$PATH

##########  This is a shell wrapper; the perl script goes ealier #########
#
# Try to find appropriate perl.
#

try_perl () {
	ver_output=`"$1" -v 2> /dev/null`
	if [ $? != 0 ]; then

	# there is no such perl
		return 1
	fi

	if [ `echo "$ver_output" | /bin/grep -c -i 'this is perl.*v[ersion]*[	 ]*5' ` \
		= 0 ]; then
	# not a perl or perl 4.*
		return 2
	fi

	return 0;
}

#
# My own perl :
[ -z "$PERL5" ] && PERL5=`whence perl`

if [ -n "$PERL5" ] && try_perl "$PERL5" ; then
	:
elif [ -n "$PERL" ] && try_perl "$PERL" ; then
	PERL5="$PERL"
elif [ -n "$HYDRA_PERL_PATH" ] && try_perl "$HYDRA_PERL_PATH/perl" ; then
	PERL5="$HYDRA_PERL_PATH/perl"
elif [ -n "$PERL_PATH" ] && try_perl "$PERL_PATH/perl" ; then
	PERL5="$PERL_PATH/perl"
elif try_perl "perl" ; then
	PERL5=perl
elif try_perl "perl5" ; then
	PERL5=perl5
else
	echo "Can't find perl version 5.x . Exitting" >&2
	exit 1
fi

exec $PERL5 $0 ${1+"$@"}

###############################################################
# vi:ft=perl ai si noet ts=8 sw=8
