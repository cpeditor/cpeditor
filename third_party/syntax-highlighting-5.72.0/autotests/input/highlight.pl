#!/usr/bin/perl -w
# This is a pseudo Perl file to test Kate's Perl syntax highlighting.
# TODO: this is incomplete, add more syntax examples!

sub prg($)
{
	my $var = shift;

	$var =~ s/bla/foo/igs;
	$var =~ s!bla!foo!igs;
	$var =~ s#bla#foo#igs;
	$var =~ tr/a-z/A-Z/;
	($match) = ($var =~ m/(.*?)/igs);

	$test = 2/453453.21;
	$test /= 2;

	print qq~d fsd fsdf sdfl sd~
	
	$" = '/';
	
	$foo = <<__EOF;
d ahfdklf klsdfl sdf sd
fsd sdf sdfsdlkf sd
__EOF

	$x = "dasds";

	next if( $match eq "two" );
	next if( $match =~ /go/i );

	@array = (1,2,3);		# a comment
	@array = qw(apple foo bar);
	push(@array, 4);
	%hash = (red => 'rot',
		blue => 'blau');
	print keys(%hash);
}

sub blah {
    my $str = << '    EOS';
        this is my string
        and it's continuation
    EOS

    $str = "hello world";

    $str = << "    EOS";
        this is my string
        and it's continuation
    EOS
}

&blah;
prg("test");

# Bracket closures in RegExp patterns (bug #364866)
qr{ ${var} aa{aa{a}a} aa*b?};
qr(aa(a(a(a(b|c)a)a)a)aa*b?);
s{aaa {aaa} a \x{A2} *b?}{aa};
s(aa(a(a(a(b|c)a)a)a)aa)(aa);

# Strings as scalar references (bug #348765)
$x = \'Reference of a String';
$y = \"Reference of a String";

# Variables that start with underscore (bug #355300)
$_variable
$_ # Reserved var.

for my $x ($hash->{arr}->@*) {
    for my $k (keys $k->%*) {
        ...
    }
}

# Highlight correctly operator // (bug #407327)
$x = ns // "";
print $x;
