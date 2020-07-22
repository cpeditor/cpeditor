(* Modula-2 R10 Syntax highlighting test *)

(* dialect tags *)

(*!m2r10*)


(* pragmas *)

<*foo*>


(* line comments *)

! foo bar baz bam boo


(* foldable comments *)

(* foo bar baz
   (* nested comment *)
   bam boo bee bop *)


(* reserved words *)

ALIAS AND ARGLIST ARRAY BEGIN BLUEPRINT BY CASE CONST COPY DEFINITION DIV DO
ELSE ELSIF END EXIT FOR FROM GENLIB IF IMPLEMENTATION IMPORT IN LOOP MOD
MODULE NEW NOT OF OPAQUE OR POINTER PROCEDURE READ RECORD REFERENTIAL RELEASE
REPEAT RETAIN RETURN SET THEN TO TYPE UNTIL VAR WHILE WRITE YIELD


(* built-in constants *)

FALSE NIL TRUE


(* built-in types *)

BOOLEAN CARDINAL CHAR INTEGER LONGCARD LONGINT LONGREAL OCTET REAL


(* built-in procedures *)

ABS APPEND CAPACITY CHR COUNT ENTIER INSERT LENGTH LOG2 MAX MIN ODD ORD
POW2 PRED PTR REMOVE SGN SUCC


(* UNSAFE facilities *)

(* module identifier *)

UNSAFE

(* reserved words *)

CAST

(* constants *)

BitsPerAddress BitsPerByte BytesPerWord BytesPerLongWord

(* types *)

ADDRESS BYTE WORD LONGWORD OCTETSEQ

(* procedures *)

ADD ADR BIT BWAND BWNOT BWOR HALT SETBIT SHL SHR SUB


(* number literals *)

CONST
  b = 0b0110'1001; (* base-2 *)

  n = 1000'0000; (* base-10 *)

  x = 0xDEAD'BEEF; (* base-16 *)

  c = 0u2A; (* character code *)

  r = 1.234; (* real number *)


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
