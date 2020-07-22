/* Yacc / Bison hl test file.
 * It won't compile :-) Sure !
 */

%{

#include <iostream>
using namespace std;

extern KateParser *parser;

%}

%locations

%union { 
   int int_val;
   double double_val;
   bool bool_val;
   char *string_val;
   char *ident_val;
   struct var *v;
   void *ptr;
}

%token <int_val>      TOK_NOT_EQUAL  "!="
%token <int_val>      TOK_LESSER_E   "<="
%token <int_val>      TOK_GREATER_E  ">="
%token <int_val>      TOK_EQUAL_2    "==" //comment
%token
   PERCENT_DEBUG           "%debug"
   PERCENT_DEFAULT_PREC    "%default-prec"
   PERCENT_DEFINE          "%define"
;

%type <int_val>       type type_proc

%code top {
  #define _GNU_SOURCE
  #include <stdio.h>
  int val;
}

%destructor { free ($$); printf ("%d", @$.first_line); } <*>
%lex-param   {scanner_mode *mode};
%parse-param {int *nastiness} {int *randomness}
%initial-action {
  @$.initialize (file_name);
};

%%

prog:                 KW_PROGRAM ident { parser->start($2); } prog_beg_glob_decl instructions { parser->endproc(0); } dev_procedures KW_ENDP ;

number:               integer_number
                      | TOK_DOUBLE
                      {
                         $$ = new var;
                         $$->type = KW_REEL;
                         $$->cl = var::LITTERAL;
                         $$->real = $<int_val>1;
                      };
words:
                      %empty
                      | words word
                      ;

%type <type> word;
%printer { fprintf (yyo, "%s", word_string ($$)); } <type>;
word:
                      %?{ boom(1); }
                      | "hello"  { $$ = hello; }
                      | "bye"  { $$ = bye; }
                      ;

foo:                  { $$ = 0 }
                      | number { $$ = $1 | $2; }
                      | hello { $$ = $1 | $3; } // without a comma

hello:
                      gram1 { $$ = "hi" };
                      | gram2
                      ;;
                      
%%

#include <stdio.h>

int main(void)
{
  puts("Hello, World!");
  return 0;
}

// ALERT NOTE
