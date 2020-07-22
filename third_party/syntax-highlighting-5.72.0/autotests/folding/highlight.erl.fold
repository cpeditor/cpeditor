% testing for the erlang syntax highlighter
% NOTE alerts work in comments to TODO !

% pragmas (show as keywords)
-module
-export
-define
-undef
-ifdef
-ifndef
-else
-endif
-include
-include_lib

% key words
after begin case catch cond  end fun if let of query receive all_true some_true 

% operators
div rem or xor bor bxor bsl bsr and band not bnot
+ - * / == /= =:= =/= < =< > >= ++ -- = ! <-

% separators (show as functions)
( ) { } [ ] . : | || ; , ? -> #

% functions - predefined (part of erlang module) - show as functions
abs accept alarm apply atom_to_list binary_to_list binary_to_term check_process_code
concat_binary date delete_module disconnect_node element erase exit float float_to_list
garbage_collect get get_keys group_leader halt hd integer_to_list is_alive is_atom is_binary
is_boolean is_float is_function is_integer is_list is_number is_pid is_port is_process_alive
is_record is_reference is_tuple length link list_to_atom list_to_binary list_to_float list_to_integer
list_to_pid list_to_tuple load_module loaded localtime make_ref module_loaded node nodes now
open_port pid_to_list port_close port_command port_connect port_control ports pre_loaded process_flag
process_info processes purge_module put register registered round self setelement size
spawn spawn_link spawn_opt split_binary statistics term_to_binary throw time tl trunc tuple_to_list
unlink unregister whereis

% functions - inferred
module:function
function()

% atoms (show as "char")
% begin with underscore, lowercase, contain numbers letters and @ - or anything between ''
middle_underscore
abc ab4d a@cd8 a@
'And this is (\012) an atom \' Atoo' Variable 'atom again' 

% variables (begin with capital letter or underscore, contain numbers, letters and @)
_leadingUnderscore AbdD@ B45@c

% this is a string 
"a string sits between \" double quotes" atom "more string"

% integers (decimal)
1. 234 $A

% integers (specified base)
2#10101 34#567

% float
12.23 12.9e-67 12.8E+89 33.34e89

% and finally some real code, so we can see what it looks like...
-module(codetest).			% everything is in a module
-export([fac/1]).		% name and number of arguments - need this to be called outside of the module

fac(N) when N > 0  -> N * fac(N-1);
fac(N) when N == 0 -> 1.
