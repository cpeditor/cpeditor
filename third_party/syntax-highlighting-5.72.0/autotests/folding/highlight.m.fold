%%=====================================================
% MATLAB test code for Kate/Kwrite syntax highlighting
%%=====================================================

% Numbers _____________________________________________
5, 5.5, .1, 0.1, 0.4e5, 1.2e-5, 5i, 5.3i, 6j, .345+3i
5', 5.5', .1', 0.1', 0.4e5', 1.2e-5', 5i', 5.3i', 6j', .345+3i'

% Operators ___________________________________________
% relational operators
'asdf'~=4, c<=4, d>=4, a<b, a>b, a==b, b||c, b&&c
% elementwise arithmetic operators
a.^b, a.*b a./b, 1:4:5
% single-character binary arithmetic
a+3, a-3, a*2, a^3, a/3, a\3, a|b, a&b
% unary operators
a = ~g; g = @sfdgdsf(sdfsd,345); g.' + 1i.' - ('asdf').'
% separators and delimiter
(asd),[sadf];{asdf},;;,;;;()
% continuation
a = 1+ ...
  2;

% Character vectors, strings and adjoint ______________
% incomplete character vectors
'string
'str''
'str''ing
% complete character vectors
'string' % simple string
'''' % string containing '
'str''ing' % one string containing '
'"' % string containing "
'str"ing' % one string containing "
'string' 'string'  % two character vectors
'asdf'  'asdf''' variable  % two character vectors and a variable
'asdf''asdf''   fsgdfg' + (asdf)' - 'asdf'.' + []''''.';''
'sadf'.' % string transpose
% adjoint
{'asdf'}' + 1
('asdf')' + 1
['asdf']' + 1
'' var''' % empty string, var with >1 adjoints
[5]'*{5}'*('asd')'.'+(5)'*6'*asdf'*asdf'.'  % many adjoints
A'*B + 1      % adjoint
A.'*B + 1     % transpose
A.'.'*B + 1   % double transpose
A'.' + 1      % adjoint, then transpose
A.''          % transpose, then adjoint
% incomplete strings
"string
"str""
"str""ing
% complete strings
"string" % simple string
"'" % string containing '
"str'ing" % one string containing '
"""" % string containing "
"str""ing" % one string containing "
"string" "string"  % two strings
"asdf"  "asdf""" variable  % two strings and a variable

% System command ______________________________________
!hostname
!cp * /tmp

% Reserved keywords ___________________________________
function, persistent, global
switch, case, otherwise
if, else, elseif
try, catch
for, while, break, continue
end, return
function, FUNCTION, Function  % case sensitive!
