#!/bin/tcsh -f

[ a != b ] && echo a || echo b
# + not an operator
[ 1+2 == '1+2' ] && echo a || echo b
[ 1 != '1+2' ] && echo a || echo b
[ a =~ b ] && echo a || echo b
[ a !~ b ] && echo a || echo b

set a = 1
set b = 2

# arithmetic operators
if ($a < $b) echo 1
if ($a << $b) echo 1
if ($a > $b - 2) echo 1
if ($a + 11 >> $b) echo 1
if ($a <= $b) echo 1
if ($a + 1 >= $b) echo 1
if ($a / $b + 1) echo 1
if ($a % $b) echo 1
if ($a * $b) echo 1

# string operators
if ($a == $b) echo s
if ($a != $b) echo s
if ($a =~ $b) echo s
if ($a !~ $b) echo s

set val = 0

foreach scen ( a b )
  echo $scen

  if ($scen == a) then
    echo $scen
  else if ($scen == x) then
    echo x
  else
    echo $val
  endif
end
