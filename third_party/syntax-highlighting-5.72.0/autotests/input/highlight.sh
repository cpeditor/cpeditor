#! /bin/sh
# This is a test script for the Katepart Bash Syntax Highlighting by
#	Wilbert Berendsen.  This is not runnable !!!


# The highlighting recognizes basic types of input, and has a few special cases that
# are all in FindCommands.  The main objective is to create really proper nesting of
# (multiline) strings, variables, expressions, etc.



# ============== Tests: ===============

# basic types:
echo 'single quoted string'
echo "double quoted string"
echo $'string with esc\apes\x0din it'
echo $"string meant to be translated"


# comments:
# this is a comment
#this too
echo this is#nt a comment
dcop kate EditInterface#1 #this is
grep -e "^default/linux/amd64/" |\ #this is a comment
mkdir this\ isnt\ #a\ comment
mkdir this\ isnt\\\;#a\ comment
mkdir this\\ #is a comment

# brace expansion
mv my_file.{JPG,jpg}


# special characters are escaped:
echo \(output\) \&\| \> \< \" \' \*


# variable substitution:
echo $filename.ext
echo $filename_ext
echo ${filename}_ext
echo text${array[$subscript]}.text
echo text${array["string"]}.text
echo ${!prefix*}
echo ${!redir}
echo short are $_, $$, $?, ${@}, etc.
echo ${variable/a/d}
echo ${1:-default}


# expression subst:
echo $(( cd << ed + 1 ))


# command subst:
echo $(ls -l)
echo `cat myfile`


# file subst:
echo $(<$filename)
echo $(</path/to/myfile)

# process subst:
sort <(show_labels) | sed 's/a/bg' > my_file.txt 2>&1


# All substitutions also work in strings:
echo "subst ${in}side string"  'not $inside this ofcourse'
echo "The result is $(( $a + $b )). Thanks!"
echo "Your homedir contains `ls $HOME |wc -l` files."


# Escapes in strings:
p="String \` with \$ escapes \" ";


# keywords are black, builtins dark purple and common commands lighter purple
set
exit
login


# Other colorings:
error() {
	cat /usr/bin/lesspipe.sh
	runscript >& redir.bak
	exec 3>&4
}


# do - done make code blocks
while [ $p -lt $q ] 
do
	chown 0644 $file.$p
done


# braces as well
run_prog | sort -u |
{
	echo Header
	while read a b d
	do
		echo $a/$b/$c
	done
	echo Footer
}


# Any constructions can be nested:
echo "A long string with $(
	if [ $count -gt 100 ] ; then
		echo "much"
	else
		echo "not much"
	fi ) substitutions." ;


# Even the case construct is correctly folded:
test -f blaat &&
(	do_something
	case $p in
		*bak)
			do_bak $p
			;;
		*)
			dont_bak $p
			;;
	esac
) # despite the extra parentheses in the case construction.


# variable assignments:
DIR=/dev
p=`ls`
LC_ALL="nl" dcop 'kate*'
_VAR=val
ARR=(this is an array)
ARR2=([this]=too [and]="this too")
usage="$0 -- version $VERSION
Multiple lines of output
can be possible."
ANSWER=yes	# here 'yes' isn't highlighed as command


# Some commands expect variable names, these are colored correctly:
export PATH=/my/bin:$PATH BLAAT
export A B D
local p=3  x  y='\'
read x y z <<< $hallo
unset B
declare -a VAR1 VAR2 && exit
declare less a && b 

# options are recoqnized:
zip -f=file.zip
./configure  --destdir=/usr
make  destdir=/usr/


# [[ and [ correctly need spaces to be regarded as structure,
# otherwise they are patterns (currently treated as normal text)
if [ "$p" == "" ] ; then
	ls /usr/bin/[a-z]*
elif [[ $p == 0 ]] ; then
	ls /usr/share/$p
fi

# Fixed:
ls a[ab]*		# dont try to interprete as assignment with subscript (fixed)
a[ab]
a[ab]=sa


# Here documents are difficult to catch:
cat > myfile << __EOF__
You're right, this is definitely no bash code
But ls more $parameters should be expanded.
__EOF__


# quoted:
cat << "EOF" | egrep "this" >&4   # the rest of the line is still considered bash source
You're right, this is definitely no bash code
But ls more $parameters should be expanded. :->
EOF


# indented:
if true
then
	cat <<- EOF
		Indented text with a $dollar or \$two
	EOF
elif [ -d $file ]; then
	cat <<- "EOF"
		Indented text without a $dollar
	EOF
fi


case 1 in 
2) echo xxx;
;;
1) echo yyy;
esac

ls #should be outside of case 1 folding block
