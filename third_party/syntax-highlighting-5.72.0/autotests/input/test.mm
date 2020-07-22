$( Modified version of demo0.mm from 1-Jan-04 $)

$(
                      PUBLIC DOMAIN DEDICATION

This file is placed in the public domain per the Creative Commons Public
Domain Dedication. http://creativecommons.org/licenses/publicdomain/

Norman Megill - email: nm at alum.mit.edu
$)

$c 0 + = -> ( ) term wff |- $.

$v t r s P Q $.

tt $f term t $.
tr $f term r $.
ts $f term s $.
wp $f wff P $.
wq $f wff Q $.

tze $a term 0 $.
tpl $a term ( t + r ) $.

weq $a wff t = r $.
wim $a wff ( P -> Q ) $.

a1 $a |- ( t = r -> ( t = s -> r = s ) ) $.
a2 $a |- ( t + 0 ) = t $.
${
    $( Define the modus ponens inference rule $)
    min $e |- P $.
    maj $e |- ( P -> Q ) $.
    mp  $a |- Q $.
$}

th1 $p |- t = t $=
    $( Here is its proof: $)
    tt tze tpl tt weq tt tt weq tt a2 tt tze tpl
    tt weq tt tze tpl tt weq tt tt weq wim tt a2
    tt tze tpl tt tt a1 mp mp
$.
