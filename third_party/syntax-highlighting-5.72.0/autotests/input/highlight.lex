/* This test file tests kates Lex/Flex highlighting */

%option c++
%option yyclass="KateTester"
%option yylineno

 /* This is a C(++) comment */

/* This one is a lex comment ! */

%{
#include <iostream>
#include "realparser.hpp"
using namespace std;
%}

/* Some definitions */
DIGIT    [0-9]
LETTER   [_a-zA-Z]

%%

 /* Comment *shall be indented here* */
[ \t\n\r]+          

 /* Note: there is a bad } just here     vvv */
\/\*([^\*]|\*[^/])*\*\/ { foo(a, b, c); } }

 /* A start condition scope... */
<ESC>{
  "a" {
  
  /* C mode ! */
  return 0;
}

  "b" %{
  
  /* C mode, too ! */
  return 0;
%}
  
  "c" return 0; // C++ comment
}

 /* Big rule */
\"([^"\\]|\\.)*\" {

   yylval.string_val = new char[strlen(yytext) + 1];
   int j = 0, i = 1;
   
   while (yytext[i] != '"')
      if (yytext[i] != '\\')
         yylval.string_val[j++] = yytext[i++];
      else
         switch (yytext[i + 1])
         {
         case 'n':
            yylval.string_val[j++] = '\n'; i += 2;
            break;
         default:
            yylval.string_val[j++] << yytext[i + 1], i += 2;
         }
    
    yylval.string_val[j] = 0;   
    return TOK_STRING;

}

 /* Dot (match all) */
.             {return yylval.int_val = yytext[0];}

%%

// Here is pure C(++)
#include <iostream>

int main(void)
{
  std::cout << "Hello, World\n";
  return 0;
}
