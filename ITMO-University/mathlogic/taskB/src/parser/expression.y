%{
#include <iostream>
#include <string>
#include "../expression.h"
#include <vector>

int yylex(void);
void yyerror(const char *);

expression* result = nullptr;
std::vector<variable*> vars;
%}

%union {
    expression* e;
    std::string* name;
}

%token <name> NAME
%token IMP OR AND NEG
%token LEFT RIGHT
%token END

%right IMP
%left OR
%left AND
%left NEG

%type <e> Input Expression

%start Input

%%
Input: Expression { result = $1; }
;

Expression:
     NAME {
     variable* v=new variable(*$1);
     int i = 0;
     if (vars.empty()) {
         $$ = v;
         vars.push_back(v);
     }
     else for(auto &s:vars){
     	 if(s->prefix_form() == v->prefix_form()){
     	     $$ = s;
     	     break;
     	 } else if (i == vars.size()-1) {
     	     $$ = v;
     	     vars.push_back(v);
     	 }
     	 i++;
       }
     }
| Expression IMP Expression { $$=new implication($1, $3); }
| Expression OR Expression { $$=new disjunction($1, $3); }
| Expression AND Expression { $$=new conjunction($1, $3); }
| NEG Expression { $$=new negation($2); }
| LEFT Expression RIGHT { $$=$2; }
;

%%
