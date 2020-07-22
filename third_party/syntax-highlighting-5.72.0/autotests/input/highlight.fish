### Comments
# TODO
# BEGIN
#    fg sfg sfh sft
# END
#
# text in (parenthesis) is ok, (even
# over several lines)
# escaped \) and \( are ok
# an unbalanced single ) is not ok
# This is due to fish comment handling in command substitutions
# which cannot reasonably handled by kate syntax highlighting


### Commands and paths
ls -l |sort
ls -1
/usr/bin/ls -l /usr/local/test/ -xyz       # option as indicator of CommandArgs context
./bin/ls -l /usr/abc**/tes?/lkhlk	-xyz   # tabulator as separator
../bin/ls -l /usr/abc**/tes?/lkhlk
~/bin/ls -l ../usr/lo\ cal/test
~user/bin/ls -l /usr/{local,remote}}/test|cat  # show error
/usr/bin/../lib/ls -l /usr/local/test.???|cat
/usr/bin\ sdfg/ls -l /usr/local/test.*
usr/bin/other -l ~user/local/test
\ usr/bin/other -l ~user/local/test
/usr/bin/lsother
../test/root/bin/fish -c "echo $t8"
test/root/bin/fish -c "echo $t8"
u?r/bin/ls
usr/b?n/ls
u*ls        # doesn't work, but ok
"ls" /tmp/testtest                         # Quotes in commands are expanded
/us"r/bin"/xy/ls
abcd$a      # show error
other) abc  # show error
<other abc  # show error
other> abc  # ok
oth>er abc  # ok
ot(her) abc # show error

ls /ho"me/te"st'user'/abc$dir[3]/ab(echo cdefg)cd/xyz
cat\ alog -abc
ls path_not_detected
ls * no_path
ls *path_detected
ls/
ls;echo xy
flex++

#specials
command -s ls
builtin --names jobs
exec bash
exec $test
set -e i[-1]
set test1[-1..1] $test; echo $test1
set test1[1..$n] $test; echo $test1
set test1[$n..1] $test; echo $test1
set test1[2..4 -2..-4] $test1[4..2 -4..-2]; echo $test1
set page_url http://fishshell.com/docs/$version_string/$fish_help_page    # ???
string split . example.com
set b (string repeat -n 512 x)

read -gi test    # ok
read -gi $test
read -gi "test"
read -gi (echo test)

set x (string split --max 1 --right / (status current-filename) #Comment test)
set test (exec $cmd)

# direct path
test -d $tmpldir/resources

### Options
cat -l /usr/local/test>logfile
ls --longopt=/test
ls -l"kasdflakjs" -r/usr/test -x../test/../bla -z~xyz/hello

### Arguments
echo abc"def"ghi
echo abc/def
echo abc[de]
echo abc(echo def)ghi
echo abc{de,f}ghi

# Path Detection
cd abc*efghi  # fish assumes * is glob -> path
cd abc?efghi
cd ..
cd .. #
cd .
cd . #
cd ~
cd ~abc/abc/*.txt
cd ./abc
cd ../abc
cd \ abc/tmp\ .txt

test .. abc

### Escapes
touch a\?bc\[\]ax\~dsf\#gsd\af\(\)dg\?sd\ fg\bhh\%j\$k\\l\?sed\*jk\>hj\<hou\&h\;hgkj\^kj\"kjh\'jhg
touch sdf\xa5lh\Xb7kll\157j\158hk\ua47bz\U123b5678fs\cxzdfga

touch "a\?bc\[\]a"      # no escaping in Quotes
touch fgh\y1            # \y is no escape sequence in fish, show it as normal text like fish itself

### Builtins
. hallo
source hallo 
eval ls . /tmp
eval ls .../tmp
eval $var
eval abc
eval {l,a}s  # lists ./as. colours not ok, but we cut here
eval "echo bla"

history merge

true ;ls
echo (true)
echo (true #comment)
echo (true>>xyz/abc)
true -error     # show error
pwd > abc

command -s ls;cat test
builtin -n;builtin echo

complete -c myprog -s o -l output -a "yes no"
builtin jobs

math -s0 10.0 / 6.0

#special keywords as directory names:
/usr/function/testtest /usr/function/testtest
/usr/for/testtest /usr/for/testtest
/usr/begin/testtest /usr/begin/testtest
/usr/switch/testtest /usr/switch/testtest
/usr/if/testtest /usr/if/testtest
/usr/while/testtest /usr/while/testtest
/usr/command/testtest /usr/command/testtest
/usr/builtin/testtest /usr/builtin/testtest
/usr/eval/testtest /usr/eval/testtest
/usr/exec/testtest /usr/exec/testtest
/usr/set/testtest /usr/set/testtest
/usr/test/testtest /usr/test/testtest

### test
test NUM1 -eq NUM2
test -f /tmp/abc -a -e /tmp/def;cat /tmp/abc;

if test \( -f /foo -o -f /bar \) -a \( -f /baz -o -f /bat \)
    echo Success.
end

[ 25 -eq $NUM2 ]
[ "abc" = "$STRING2" ]

if [ ! \(STRING1 != STRING2\) ];ls;end
if [ -f abc ]# show error
end
if [ -f abc ] # ok
end

if [ ! \(STRING1 != STRING2\) ] ls;end # show error
if [ "abc" != "def" ] # hallo
   echo x;end

### Redirections
ls < SOURCE_FILE         # stdin
ls > DESTINATION         # stdout
ls ^ DESTINATION         # stderr
ls >> DESTINATION_FILE   # append
ls ^^ DESTINATION_FILE   # append
ls >? DESTINATION        # noclobber
ls ^? DESTINATION

ls ^? ~
ls ^? ~testuser/abc
ls ^?/DESTINATION
ls ^?#DESTINATION       # show error

ls > $abc[5]
ls >"abc"/defg$v/xyz

ls >&                   # show error
ls > &1                 # show error
ls abc>&25 xyz /tmp/xyz $abc
ls /tmp/tst>&25?        # show error
ls /tmp/tst>&25 ?       # ok
ls 3>&- xyz
ls 1<DESTINATION
echo Hello > all_output.txt ^&1 xyz
ls 2>&-;echo end
ls xyz 2>|cat

ls>x
abs>x
/bin/ls>x
/bin/abs>x

### Quoting
echo 'abcdefg'
echo 'abc\'de\\fg'
echo 'abc\defg'

echo "1\"23\$45\\6"
echo '1\"23\$45\\6'
echo '
hallo
'
echo "$i: '$$name[1][$i]'"


### Piping, Pipelines and background jobs
ls -l /tmp/hello 1<DEST| sort
comand_1;command_2
comand_1 ;command_2
ls;ls
run_long_job & ls  # yes & is a command sequencing operator like | and ;
cat foo.txt | head
make fish 2>| less #show error

### Variable Expansion
echo $PATH
echo $PATH[1..3]
echo $PATH[-1..3 5 9..15]
echo $PATH[$n[2]]
echo $test[1..$n]
echo $test[$n..1]
echo The plural of $WORD is "$WORD"s
echo The plural of $WORD is {$WORD}s
echo $$foo[$i]
echo $test[(count $test)..1]
echo $test[1..(count $test[3])]

 #show error
echo $PATH[error
echo $PATH[1..error]

echo $PATH[1 2 34 ]
echo $PATH[1.. 2]
echo $PATH[1 ..2]

### Brace Expansion
echo input.{c,12h,TXT}
echo input.{abc$test[(count $test)..1],(echo $txt)}
echo input.{a{b,c},12h,TXT}}  # show error


### Index range expansion
echo (echo $a)[2..5]
echo (echo $a)[2..5 1..3]
echo (echo $a)[-1..1]

### Process expansion
fg %ema               # ???


### Command substitution
echo -n -s "$USER" @ "$__fish_prompt_hostname" ' ' (set_color $color_cwd) (/usr/bin/ls) (set_color normal) 

echo (#hallo)abc      # ok
echo (ls #hallo)abc   # ok, die context-Tiefe ist anders
echo (ls;#hallo)abc   #     als hier.
echo (ls#hallo)abc    # ok: # may be inner part of function names!
echo (/usr/bin/ls)
echo (ls >>mem/abc)
echo (echo $test[2])
echo (/usr"/bi"n/ls /tmp)

# multline command substitution
set x (echo dirlist; # blabla
       ls)

## same, but commented out:

# set x (echo dirlist; # blabla
#        ls)

set pi (math "scale=10; 4*a(1)")

### Continuation lines
ls \
  /usr/bin

test "STRING1"="STRING2" -a \
     "STRING3" = (echo "STRING4")

echo input.{abc\
,12h,\
TXT}

echo (\
      ls)

# no line continuation within single quotes
echo 'abcdefg\
       s$dfg sdf'
# but within double quotes
echo "123456\
      78910"
echo $PATH[1..3\
          ]
ls /usr\
local/bin
echo foo\ bar
echo foo\
bar

for i \
    in \
    a b c
    echo $i
end

# Bug?
# Depending on weakDeliminators, either this works:
cat\ alog -abc
# or this:
builtin\
 --names jobs
builtin \
--names jobs

command \
   ls

eval \
   ls . /tmp
eval\
   ls .../tmp

set \
  -e i[-1]
set \
   test1[-1..1] \
   $test; \
   echo $test1

read \
  -gi test



# see https://github.com/fish-shell/fish-shell/pull/1987
# this should work, but I found no way to implement it:
echo before comment \
  # comment
  # comment2
  after comment # error: after should be just another argument
from root

echo "hello" | \
#remove 'l'
#and more
tr -d 'l'

### Trouble
make; and make install; or make clean              # ok
printf '%s\t%s\n' flounder fish                    # ok
diff (sort a.txt | psub) (sort b.txt | psub)
source-highlight -f esc (cpp main.c | psub -s .c)

while test -f foo.txt  # comment
  echo hallo
end

for i in {1,(ls),3}    # comment
  echo $i
end

# function
function __hidden      # comment
  cd /
end

function ll \
  -d "multiline
  description"
    ls -l $argv
end

function ls --description="Description" -a a b c # comment
    command ls --color=auto $argv
    if test -f foo.txt
        echo foo.txt exists
    else if test -f bar.txt          # comment
        echo bar.txt exists
    else
        echo foo.txt and bar.txt do not exist
    end                              # comment
end

function test;echo hallo;end         # comment

switch $animal   # comment
    case cat     # comment
        echo evil
    case wolf \
        dog
        echo mammal
    case '*'
        echo I have no idea what a $animal is
end

if test (count $argv) = 1
  echo $argv
else
  echo "?"
end

if not test -f spoon   #comment
  echo There is no spoon
else
  exit 1
end

for file in $cfgfiles
    if test -f $file
        string match -r '\[Dialer' < $file | string replace -r '\[Dialer (.+)\]' '$1'
    end
end | sort -u | string match -v Defaults

for i in *.c
    if grep smurf $i     # comment
        echo Smurfs are present in $i
        break
    end
end

if begin contains -- $argv --force   # ohne ; nach begin: geht
        or not status --is-interactive and not status --is-login
    end
    echo ""
end

begin                    # comment
  for i in *.tmp
      if grep smurf $i
          continue
      end
      rm $i
  end
end

if [ $status -eq 1 ]
	set sta pass
else
	set sta fail
end

exec "$test"


# Process Expansion
# see fish issue 4230
kill -STOP %self

