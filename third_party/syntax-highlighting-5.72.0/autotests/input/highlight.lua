#!/usr/bin/env lua

-- Metatables
t = {
    __add=function(a,b)return a+b end,
    __sub=function(a,b)return a-b end,
    __mul=function(a,b)return a*b end,
    __div=function(a,b)return a/b end,
    __mod=function(a,b)return a%b end,
    __pow=function(a,b)return a^b end,
    __unm=function(a)return -a end,
    __idiv=function(a,b)return a//b end,
    __band=function(a,b)return a&b end,
    __bor=function(a,b)return a|b end,
    __bxor=function(a,b)return a~b end,
    __bnot=function(a)return ~a end,
    __shl=function(a,b)return a<<b end,
    __shr=function(a,b)return a>>b end,
    __concat=function(a,b)return a..b end,
    __len=function(a)return #a end,
    __eq=function(a,b)return a==b end,
    __lt=function(a,b)return a<b end,
    __le=function(a,b)return a<=b end,
    __index=function(t,k)return t[k] end,
    __newindex=function(t,k,v)return t[k]=v end,
    __call=function(f, ...)return f(...) end,

   __tostring=function(a)return tostring(a) end,
   __pairs=function(t)return pairs(a) end,
   -- setmetatable
   __metatable=true
   -- Garbage collector
   __gc=function() end
   -- Weak table
   __mode='k' -- or 'v'
}

a or b
a and b
a~=b
a>=b
true or false
a or nil
a::m
a.m
a;a


-- String
'\a'
'\b'
'\f'
'\n'
'\r'
'\t'
'\v'
'\\'
'\"'
'\''
'\z'
'\xff'
'\xFF'
'\231'
'\23'
'\2'
'\u{100201}' -- max 6 digits
'\2a\ks' -- error

'multi\
line'
'multi\z
line'
'multi\z    line\
2'

a = 'alo\n123"'
a = "alo\n123\""
a = '\97lo\10\04923"'
a = [[alo
123"]]
a = [==[
alo
123"]==]


-- Decimal
3
345
0xff
0xBEBADA

-- Float
3.
.3
3.0
3.1416
314.16e-2
314.e+2
0.31416E1
34e1
0.e3
0x0.1E
0xA23p-4
0xA.p+4
0x.ap4
0X1.921FB54442D18P+1
-- error
32p
0xp-4
0x.p-4


-- single comment
xyz()
--[[
 long comment
]]
xyz()


a = { [f(1)] = g; "x", "y"; x = 1, f(x), [30] = 23; 45 }

-- is equivalent to

do
  local t = {}
  t[f(1)] = g
  t[1] = "x" -- 1st exp
  t[2] = "y" -- 2nd exp
  t.x = 1 -- t["x"] = 1
  t[3] = f(x) -- 3rd exp
  t[30] = 23
  t[4] = 45 -- 4th exp
  a = t
end

32-0x43+0x2-5
return"a"
return'a'
return{}
f(3)
f'a'
f"a"
f{s=2}
f[[s]]
f[=[s]=]
#a

local CONSTANT = a

a = {}
local x = 20
for i=1,10 do
  local y = 0
  a[i] = function () y=y+1; return x+y end
end

--! \brief gfind is deprecated
string.gfind('s')
string.gmatch('f')

function foo()
  if x then
    function() end
    bar=function()
      if y then
        if z then end
      end
    end
  end
end
