
#i am macro

library "a"

function myfunc ( foo as Integer , bar as Float = 3.5 ) as Void
end function

sub mysub ( foo as Integer , bar as Float = 3.5 ) as Void
end sub

' my comment

Function MyCamelCaseFunction()
    lvalue = rvalue
a=5
a=5!
a=5#
a=5%
a=5&

a$=5
a!=5
a#=5
a%=5
a&=5

a=a$
a=a!
a=a#
a=a%
a=a&

foo()
a.foo()
a=a.foo()

eval=5
eval()
a.eval()

a=eval
a=eval()
a=a.eval()

sleep=5
sleep()
a.sleep()

a=sleep
a=sleep()
a=a.sleep()

if a=b c=d
if a foo()
if a then foo()

if a
end if

if a then
end if

if a
else if
else
end if

if a
elseif
endif

for each a in b
end for

for a=1 to 10 step 2
endFor

for each a in b
next

while a
end while

while a
endWhile

a [ "a" , 1, foo() ] = 1
a.foo()[1]=1

a = {
    a : 1
    b : "2"
    c : function() : end function
}

b.foo("string",1,{a:b},1.5,[1,2,3])
a = b.foo("string",1,{a:b},1.5,[1,2,3])

unnamed_function = function ( a as Integer ) as Float

a = +-b
a = &h42abcdef
c = var@attr
end function

unnamed_sub = sub ( a as Integer ) as Void
end sub
endFunction
