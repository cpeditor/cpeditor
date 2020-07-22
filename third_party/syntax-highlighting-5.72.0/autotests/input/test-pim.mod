(* PIM Modula-2 Syntax highlighting test *)

(* dialect tags *)

(*!m2pim*)


(* pragmas *)

(*$foo*)


(* foldable comments *)

(* foo bar baz
   (* nested comment *)
   bam boo bee bop *)


(* reserved words *)

AND ARRAY BEGIN BY CASE CONST DEFINITION DIV DO ELSE ELSIF END EXIT EXPORT FOR
FROM IF IMPLEMENTATION IMPORT IN LOOP MOD MODULE NOT OF OR POINTER PROCEDURE
QUALIFIED RECORD REPEAT RETURN SET THEN TO TYPE UNTIL VAR WHILE WITH


(* built-in constants *)

FALSE NIL TRUE


(* built-in types *)

BOOLEAN BITSET CARDINAL CHAR INTEGER LONGINT LONGREAL PROC REAL


(* built-in procedures *)

ABS ALLOCATE CAP CHR DEALLOCATE DEC DISPOSE EXCL FLOAT HALT HIGH
INC INCL MAX MIN NEW ODD ORD SIZE TRUNC VAL


(* unsafe facilities *)

(* module *)

SYSTEM

(* types *)

ADDRESS WORD PROCESS

(* procedures *)

ADR NEWPROCESS TRANSFER TSIZE


(* number literals *)

CONST
  n = 1000; r = 1.234; x = 0FFFFH; c = 077C;


(* quoted literals *)

CONST
  apostrophe = "'"; doublequote = '"';
  single ='foobar'; double = "bazbam";


(* sample code *)

TYPE Foo = RECORD
  bar : Baz;
  bam : Boo
END; (* Foobar *)


VAR
  foo, bar, baz : CARDINAL;


IF foo > bar THEN
  baz := bam
ELSE
  baz := boo
END;


(* EOF *)
