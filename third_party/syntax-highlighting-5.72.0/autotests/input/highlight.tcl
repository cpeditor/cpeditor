# tcl syntax highlighting sample script for Kate
#
# author: JM. Philippe 15/03/04

# escaped characters
set String \{
set String \{
set String \"
set String " \" "
set String " \{ "

#comments and not comments
# is comments
;#is comments
	# is comments
# <h1> is html comment </h1>
puts ok;	# is comments
set String [string map {</a> {<span>&#187;is not comments</span></a>}} $String]
set String \#not_a_comment

# blocks
proc test {arg1 {arg2 {}} {arg3 {fr fq r}}} {
	if {1} {;	#comments
		set String \{;	# not a block start
	}
}

proc test args {
	set String \};	# not a block end
}

# BEGIN - collapsable comments
# blablabla
# END

# strings
set String "feqr feqr $gqer gqe"
set String "feqr
feqr \" $gqer \
gqe
"
set String {feqr
feqr \{ $gqer \
gqe
}

# variables
set b+1 [incr b]
set {incr-b} ${b+1}
puts ${incr-b}
