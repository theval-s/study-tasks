%{
#include <string>
#include "../expression.h"
#include "expression.tab.h"
%}

%option outfile="lex.c" header-file="lex.h"


white [ \t]+
digit [0-9]
letter [A-Z]
char '
symbol {letter}({letter}|{digit}|{char})*


%%
{symbol} {
    yylval.name=new std::string(yytext);
    return NAME;
}
"&" return AND;
"|" return OR;
"->" return IMP;
"!" return NEG;
"(" return LEFT;
")" return RIGHT;
"\n" return END;
%%
