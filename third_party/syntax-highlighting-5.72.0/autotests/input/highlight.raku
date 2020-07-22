#!/usr/bin/perl6

use v6;

if True {
    say "Hello";
}

if True {
say "Hello"; # Bad indentation intended
        }

if True { say "Hello" }

if True {say "Hello"}

if True { say "Hello" } else { say "Goodbye" }; say "world";

## BEGIN Comments

say 1 #`( blah TODO blah
here? ) 2 ;
say 1 #`{{ blah TODO blah
here? }} 2 ;
say 1 #`[[ blah TODO blah ]
here? ]] 2 ;
say 1 #`««« blah TODO blah
here? »»» 2 ;
say 1 #` ( blah TODO blah
2 ;

#`[
  This is another multi-line comment. ]
my @sorted = @names.sort({ %sets{$_} }).sort({ %matches{$_} }).reverse;
#`{ So is this, though it's not actually multi-line. }

say 'code again';

#`(  Multiple line comments `( sub-comment )`)`

## END Comments

#`[
And this is how a multi would work.
That says why we do what we do below.
]

say "No more";

#`{{a}a}} say ok

say "No more";

say "this is code";

=begin pod

A very simple Pod6 document

=end pod

=begin head1
Top Level Heading
=end head1

=head1 Top level heading

=for head1
Top Level Heading

=cutsdads

=begin pod

=head1 This is a head1 title

This is a paragraph.

=head2 Subsection
# plop
Here some text for the subsection.

=end pod

=begin table :caption<My Tasks>
mow lawn
take out trash
=end table

=head1 This is a heading block
This is an ordinary paragraph.
Its text will be squeezed and
short lines filled. It is terminated by
the first blank line.

=head2 This is another heading block
This is yet another ordinary paragraph,
at the first virtual column set by the
previous directive

#| Base class for magicians
class Magician {
  has Int $.level;
  has Str @.spells;
}

#| Fight mechanics
sub duel(Magician $a, Magician $b) {
}
#=«<((
 <Magicians only, no mortals. >
 Magicians only, no mortals.
))>»

say Magician.WHY;

#|<< This is an example of stringification:
    * Numbers turn into strings
 >   * Regexes operate on said strings
    * C<with> topicalizes and places result into $_
>>
sub search-in-seq( Int $end, Int $number ) {
    with (^$end).grep( /^$number/ ) {
        .say for $_<>;
    }
}
#= Uses     * topic    * decont operator


=head1 plop
abc

=head1 plop
=para
abc

=head1 plop
 abc
abc

=head1 plop
abc

say 1;

=head1 plop

say 1;

=for head1
plop

=for table :conf[str, 'str', "str", 1, True] :conf(str, 'str', "str", 1, True)
=for table :conf<str 'str' "str" 1 True>
=for table :conf{k => str, k=>'str', k=>"str", k=>1, k=>True}
=for table :a :!b :42k :+s
abc

say 2;

=begin a
abc
=end a

=begin b sa
=end b
say 2;

=begin a
abc

abc

abc
=end a
say 0 ;
=begin a:config{42}
abc

=head1 The
Title

abc
=end ab
say 1 ;

=begin b sa
=end b
=begin a
abc
=begin b
abc
=end b a
abc

abc
=end a
say 3 ;

=item a
=item b
=begin code
 =item a
=end code

=begin code
 =item a
 b
=end code d
=end code

=begin comment
Here are several
lines
of comment
=end comment

=begin pod
=head1 acB<<I<a>>>a B<c> U<d> BB<a>

C<my $var = 1; say $var;>
Perl 6 homepage L<https://perl6.org> L<Perl 6 homepage|https://perl6.org>
Comments L<#Comments> L<Comments|#Comments>
Perl 6 is awesome Z<Of course it is!>
Perl 6 is multi-paradigmatic N<Supporting Procedural, Object Oriented, and Functional programming>
Enter your name K<John Doe> E<0xBB> characters.

A X<hash|hashes, definition of; associative arrays>

=DISCLAIMER
P<http://www.MegaGigaTeraPetaCorp.com/std/disclaimer.txt>

A X<hash|hashes, definition of; associative arrays>
is an unordered collection of scalar values indexed by their
associated string key.

    my @names = <Foo Bar Baz>;
    my @upper-case-names = @names.map: { .uc }    # OUTPUT: [FOO BAR BAZ]

=end pod

=begin table :caption<My Tasks>
mow lawn
take out trash
=end table

=begin table :config{caption => "My Tasks"}
mow lawn
take out trash
=end table


say "\c999 \c999999999 \c[LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER B] \c77s \c[77,22]";
say "\x0 \x00 \x[0] \x[00] \x[f] \xaaa \xfffffff \xffh \x[ffff] \x[fffffff] \x[42,42]";
say "\o0 \o00 \o[0] \o[00] \o[7] \o333 \o77777777 \o77h \o[333] \o[77777777] \o[42,42]";

say Q[A literal string] ;
say ｢More plainly.｣ ;
say Q ^Almost any non-word character can be a delimiter!^ ;
say Q ｢｢Delimiters can be repeated/nested if they are adjacent.｣｣ ;

say Q (this is fine, because of space after Q) ;
say Q 'and so is this' ;
say Q<Make sure you <match> opening and closing delimiters> ;
say Q{This is still a closing curly brace → \} ;

say Q;yes, this is fine; ;
say Q('this is a function') ;

say 'Very plain';
say q[This back\slash stays];
say q[This back\\slash stays]; # Identical output
say q{This is not a closing curly brace → \}, but this is → };
say Q :q $There are no backslashes here, only lots of \$\$\$>!$;
say '(Just kidding. There\'s no money in that string)';
say 'No $interpolation {here}!';
say Q:q!Just a literal "\n" here!;
say Q:q[a\[]
say Q[A literal string] ;
say ｢More plainly.｣ ;
say Q ^Almost any non-word character can be a delimiter!^ ;
say Q ｢｢Delimiters can be repeated/nested if they are adjacent.｣｣ ;

say qq[My favorite color is {$n+2}!];
say qq{My favorite color\- is {$n+2}!};
say Q:qq{My favorite color\- is {$n+2}!};
say "My $color[0].uc(
) $n+$n.^name favorite color \q[1is] {$n+2}!abc&uc('a')";

say qqww|a|;
say qq:w:w|a|;
say q:a:v|a|;

say(qq:to/TERM INATOR/, 1+1);
 blah blah
 TERM INATOR


my $don't-do-that = 1;
my $piece_of_π = 3.14;
my $駱駝道    = 1;
my $lexical   = 1;
my $*dynamic1 = 10;
my $*dynamic2 = 100;

sub say-all() {
    say "$lexical, $*dynamic1, $*dynamic2";
}

say-all();

{
    my $lexical   = 2;
    my $*dynamic1 = 11;
    $*dynamic2    = 101;
}

my $square = 9 ** 2;
my @array  = 1, 2, 3;   # Array variable with three elements
my %hash   = London => 'UK', Berlin => 'Germany';

class FailHash is Hash {
    has Bool $!final = False;
    multi method AT-KEY ( ::?CLASS:D: Str:D \key ){
        fail X::OutOfRange.new(:what("Hash key"), :got(key),
          :range(self.keys)) if $!final && !self.EXISTS-KEY(key);
        callsame
    }

    method finalize() {
        $!final = True
    }
}

my %h is FailHash = oranges => "round", bananas => "bendy";

say %h<oranges>;
%h.finalize;
say %h<cherry>;
CATCH { default { put .^name, ': ', .Str } }

my ( @foo, $bar );
@foo = ($bar) = 42, "str";

say anon class þ {};
say anon sub þ  { 42 };

sub a {
    state @x;
    state $l = 'A';
    @x.push($l++);
};

say a for 1..6;

sub foo($x) {
    my $v = @;
    $v[$x] = $x;
    say $v;
}

foo($_) for ^3;

use MONKEY-TYPING;
augment class Int {
    method is-answer { self == 42 }
}
say 42.is-answer;

my $in = 0;

sub f(*@c) {
    (temp $in)++;
     "<f>\n"
     ~ @c».indent($in).join("\n")
     ~ (+@c ?? "\n" !! "")
     ~ '</f>'
};

for <ab:c d$e fgh ij*> {
    .say if m/<-alpha>/;
}

for '.' {
    .Str.say when !.IO.d;
    .IO.dir()».&?BLOCK when .IO.d # lets recurse a little!
}

use Dog:auth<Somebody>:ver<2.0>;

infix:<+>
infix:<*>
infix:«<=»

postfix:<²>
WOW:That'sAwesome
WOW:That's<<🆒>>
party:sweet<16>

infix:<+>
infix:<<+>>
infix:«+»
infix:['+']
infix:('+')

my $a:b<c>:d<e> = 100;
my $a:d<e>:b<c> = 200;
say $a:b<c>:d<e>;

use Test; plan 1; constant &term:<👍> = &ok.assuming(True);
👍

my $x = do if True { 42 };

say 1000000, 1_000_000, 10_00000, 100_00_00;
say -2, 12345, 0xBEEF, 0o755, :3<1201>;
say 1.0, 3.14159, -2.5, :3<21.0012>;
say 1e0, 6.022e23, 1e-9, -2e48, 2e2i, .42;
say 2.e2, .2, 0o39, 0xfF3u, 0oi, 0xi, :3<>, :23<gg ; # error


for $size «[r/]« (2**60, 2**50, 2**40, 2**30, 2**20, 2**10)
          Z      <EB     PB     TB     GB     MB     KB> -> [\v,\suffix]

my $a = 32;
$a += 10;
$a -= 2;
$a = 3;
$a min= 5;
$s ~= 'b';

sub infix:<space-concat> ($a, $b) { $a ~ " " ~ $b };
my $a = 'word1';
$a space-concat= 'word2';

my Real $a = 1/2;
$a = 3.14;
$a .= round;

my $a = True;
say so $a != True;
my $i = 10;

my $release = Date.new(:2015year, :12month, :24day);
my $today = Date.today;
say so $release !before $today;

say 4 R/ 12;
say [R/] 2, 4, 16;
say [RZ~] <1 2 3>,<4 5 6>

say (1, 2, 3) »*» 2;
say (1, 2, 3, 4) »~» <a b>;
say (1, 2, 3) »+« (4, 5, 6);
say (&sin, &cos, &sqrt)».(0.5);

say @a »+=» 1;
my ($a, $b, $c);
(($a, $b), $c) «=» ((1, 2), 3);

say !« @wisdom;
@a»++;
say -« [[1, 2], 3];

@slops».?this-method-may-not-exist();

my %outer = 1, 2, 3 Z=> <a b c>;
my %inner = 1, 2 Z=> <x z>;
say %outer «~» %inner;

say $neighbors »>>+<<» ($p, *);

sub plus { $^a + $^b };
say [[&plus]] 1, 2, 3;

my @n = [\~] 1..*;
say @n[^5];

@l = <a b c d> Z~ 1, 2, *;
say so 1 S& 2 S& 3;
@a X[+=] @b;
multi sub postfix:<++>($x is rw) is assoc<non>
say $filename++ for 1..3;
$x % $y == $x - floor($x / $y) * $y
say <a a b c a d> ⊍ bag(<a a b c c>);
say -« <1 2 3>

# This is wrong: creates a Hash of Mixes, not Mix:
my Mix %mix;
# Works with $ sigil:
my Mix $mix;
# Can be typed:
my Mix[Int] $mix-of-ints;

my $x;
my $x = 7;
my Int $x = 7;
my Int:D $x = 7;
ndef)
my Int $x where { $_ > 3 } = 7;
my Int $x where * > 3 = 7;

$str ~~ tr:d:c!dol!wne!;
$str ~~ TR:c/dol/wne/;
$str ~~ s!foo!fox!;
$str ~~ /foo/bar/;
$str ~~ ///;
$str ~~ rx/foo/bar/;
$str ~~ Q :regex /foo/;
$str ~~ s{b(.)r} = " d$0n";
$str ~~ regex{fox}; # error
$str ~~ regex {fox};
rx/ ^ab /;
/ ^ ab /;
rx/ \d ** 2/;
$str ~~ tr:d:c!dol!wne!;
$str ~~ TR:c/dol/wne/;
$str ~~ s!foo!fox!;
$str ~~ rx/foo/;
$str ~~ regex:ds {fox};
$str ~~ regex{fox};
my a = /a/;
rx/ ^ab /;
a = / ^ ab 'a' "$a" \d \n\n <:L :Script<Latin>> <:Block('Basic Latin')>  /;
//;
 rx:sigspace.\d+ < :Script +:Block "Basic Latin" + :L> #plop
'-'.;
rx/ \d ** 2 <[\d a d]+[\x233..\] " \c[dsds]]>/;
say $str ~~ m:g/[(<[ACGT]> **: 3) \s*]+ \s+ (<[A..Z a a..z \s]>+)/;
say '$333' ~~ m/^^ <?[$]> . \d+ /;
say '/foo/o/bar/' ~~ /\/.**!{1..10}\//;
rx(a);
rx (a);
$str ~~ regex {fox};
$str ~~ s{b(.)r} = " d$0n";
if 'abc' ~~ / [a||b] (c) / {
    say ~$0;                # OUTPUT: «c␤»
}
if 'abc' ~~ / $<myname> = [ \w+ ] / {
    say ~$<myname>      # OUTPUT: «abc␤»
}
say 'abc' ~~ / a <( b )> c/;
say 'abc' ~~ / <(a <( b )> c)>/;
say "abc" ~~ /a. | ab { print "win" } /;

so 'hello world' ~~ m:Perl5/^hello (world)/;   # OUTPUT: «True␤»
so 'hello world' ~~ m/^hello (world)/;         # OUTPUT: «False␤»
so 'hello world' ~~ m/^ 'hello ' ('world')/;   # OUTPUT: «True␤»

say "Abra abra CADABRA" ~~ m:exhaustive/:i a \w+ a/;

my regex ipv4-octet { \d ** 1..3 <?{ $/.Int <= 255 && $/.Int >= 0 }> }
my regex ipv4-octet { \d ** 1..3 <?{ True }> }
say 'abc' ~~ / <?before a> && . /;
say 'abcdefg' ~~ rx{ abc <[email protected]ending_letters> };
s:g[\d+ <?before \s* @units>] = 5 * $/;


sub walk(\thing, *@keys) is rw {
    my $current := thing;
    for @keys -> $k {
        if $k ~~ Int {
            $current := $current[$k];
        }
        else {
            $current := $current{$k};
        }
    }
    $current;
}

my %hash;
walk(%hash, 'some', 'key', 1, 2) = 'autovivified';

say %hash.perl;

class X::WithoutLineNumber is X::AdHoc {
    multi method gist(X::WithoutLineNumber:D:) {
        $.payload
    }
}
die X::WithoutLineNumber.new(payload => "message")

{ return; CATCH { default { $*ERR.say: .^name, ': ', .Str } } }

multi sub trait_mod:<is>(Routine $r, :$export!)

react {
    whenever signal(SIGINT) {
        say "goodbye";
        done
    }
}

signal(SIGINT).tap: { say "bye"; exit }; loop {}

method base-repeating(Rational:D: Int:D() $base = 10)

multi sub prefix:<-->($x is rw) is assoc<non>


multi MAIN(Bool :$man) {
    run $*EXECUTABLE, '--doc', $*PROGRAM;
}

for $file.lines -> $line {
    next unless $line; # ignore any empty lines

    my ($pairing, $result) = $line.split(' | ');
    my ($p1, $p2)          = $pairing.words;
    my ($r1, $r2)          = $result.split(':');

    %sets{$p1} += $r1;
    %sets{$p2} += $r2;

    if $r1 > $r2 {
        %matches{$p1}++;
    } else {
        %matches{$p2}++;
    }
}

for @sorted -> $n {
    my $match-noun = %matches{$n} == 1 ?? 'match' !! 'matches';
    my $set-noun   = %sets{$n} == 1 ?? 'set' !! 'sets';
    say "$n has won %matches{$n} $match-noun and %sets{$n} $set-noun";
}

say "Math: { 1 + 2 }";

my @people = <Luke Matthew Mark>;
say "The synoptics are: {@people}";

say "{%sets}";
say "we have @flavors[0]";
say "we have @flavors[]";
say "we have @flavors.sort()";
say "we have @flavors.sort.join(', ')";

my @valid-players = $file.get.words;

for $file.lines -> $line {
    my ($pairing, $result) = $line.split(' | ');
    my ($p1, $p2)          = $pairing.split(' ');
    if $p1 ∉ @valid-players {
        say "Warning: '$p1' is not on our list!";
    }
    if $p2 ∉ @valid-players {
        say "Warning: '$p2' is not on our list!";
    }
}
