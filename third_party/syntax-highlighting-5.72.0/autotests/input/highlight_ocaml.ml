(* ocaml test file -- a big stew of Objective Caml syntax to use to
   test Kate's syntax highlighting. This will not run! :-) *)

(* First a little piece of real OCaml that should look right: *)

    #load "basic";;
    (* Return a default value for a BASIC variable given its identifer. *)
    let default_value (ident : string) : basic_value =
       assert (String.length ident > 0);
       match ident.[String.length ident - 1] with
       | '$' -> Str ""
       | '%' -> Int 0
       | '!' -> Flt 0.0
       | _   -> Flt 0.0
    ;;

(* Directives: *)
#load "pa_o";;
  #load "pa_o";;
object # meth ;;  (* not a directive - a method call *)
object
   # meth ;;  (* not a directive - a method call *)

(* OCaml keywords: *)
and as assert asr (* etc. there so many... *)

(* Additional OCaml Revised Syntax keywords: *)
(* These are in a seperate category so they can be coloured to look
   like identifiers when ordinary OCaml syntax is being used: *)
declare where value

(* There's no way to reliably highlight all OCaml type expressions,
   (they can be very complex) so just the built-in type names are highlighted.*)
exn lazy_t format unit int real char string ref array bool list option


let integers : int list = [
    123456789;              (* decimal *)
    -0xabcedf0123456789;    (* hexadecimal *)
    0xABCDEF0123456789;     (* hexadecimal *)
    -0o1234567;             (* octal *)
    0b01001010101010;       (* binary *)
    -0Xabcedf0123456789;    (* hexadecimal *)
    0XABCDEF0123456789;     (* hexadecimal *)
    -0O1234567;             (* octal *)
    0B01001010101010;       (* binary *)
    -123_456_789;           (* Underscores are allowed in numeric constants. *)
    0x_abce_df01_2345_6789;
    -0o12_34_567;
    0b_0100_1010_1010_1101;
];;

let floats : real list = [
    12345.6789;
    -1.23456789e4;      (* All variations of the exponent form *)
    1.23456789e+4;
    -1.23456789e-4;
    1.23456789E-4;
    -1.23456789E+4;
    12_345.6789;       (* Underscores are allowed in numeric constants. *)
    -1.23_456_789e+4;
    12_345.6789;
];;

let characters : char list = [
    'a';
    ' ';
    'ä';
    '\n'; '\r'; '\t'; '\b';    (* Control characters. Only these four: not the full C-language range. *)
    '\000'; '\128';            (* Decimal character codes. These are always 3 digits. *)
    '\x02'; '\xff'; '\xFF';    (* Hexadecimal character codes. These are always 3 digits. *)
    '\\'; '\''; '\"'; '"'      (* Quote character escapes. *)
];;

(* Quotes used to mark constants in parsers should
   not be confused with character constant quotes.
   "Ticks" at the end of identifiers should
   not be confused with character constant quotes.  *)
let basic_identifier =
  parser
      [< ''F'; ''N'; name = s >] -> ID (s, 'f')
    | [< name = s' >] -> ID (s','i')
;;

let strings : string list = [
    ""; (* Empty string *)
    "a"; " ";  "ä";   "ab";
    "A\nB"; "A\rB"; "A\tB"; "A\bB";  (* Control characters. Only these four: not the full C-language range. *)
    "A\000B"; "A\128B";              (* Decimal character codes. These are always 3 digits. *)
    "A\x02B"; "A\xffB"; "A\xFFB";    (* Hexadecimal character codes. These are always 3 digits. *)
    "A\\B"; "A\'B"; "A'B";  "A\"B";  (* Quote character escapes. *)
    "A multiline\
    string";
];

let camlp4_quotations = [
    <<A Camlp4 source code quotation.>> ;
    <:QUOTE<A labelled Camlp4 source code quotation.>> ;
    <:QUÖTÈ<A labelled Camlp4 source code quotation. (Latin-1 identifier.)>> ;
    << A quote with an escape: \>> (end-quote symbol) >> ;
    << A quote with an escape: \<< (plain start quote-symbol) >> ;
    << A quote with an escape: \<:Trouvé< (labelled start-quote symbol) >> ;
];;

(* end *)
