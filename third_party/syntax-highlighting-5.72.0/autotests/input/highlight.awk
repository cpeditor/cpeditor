#!/usr
# AWK hl test

# BEGIN and END are also matched as patterns
BEGIN {
	p = 0;
}

/some pattern/ {
	p++;
}

# / inside brackets is not considered end of expression
# a loose division operator (/) is not mismatched as a pattern.
$1 =~ /[^abc/]def/ || b == 3 / 5 {

	gsub ( FILENAME );

}

# TODO and FIXME also work in comments in Awk.

# Also backslash in patterns works.
/\/usr\/bin\/awk/ { print "This is me"; }

END {
	print p;
}

function myfunc()
{
    print 42
}

/abc/,/a[b]c/{
	# parameter with a regex
	if (match($0, /a/)) {}

	"x\ax\nx\ex\zx\023x\2x\xffx\xFf\xax\12x"

	a =~ /[[:alpha:]]/
	a =~ /[xx[:alpha:]xx]/
	a =~ /[a-z[:alpha:]xx-]/
	a =~ /[]a]/
	a =~ /[-a]/
	a =~ /[^]a]/
	a =~ /[^-a]/
	a =~ /[a]a\/\n/
	# incomplete regex
	a =~ /[xx[:alph
	a = 23.2
}
