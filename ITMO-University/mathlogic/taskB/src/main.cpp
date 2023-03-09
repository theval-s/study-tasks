#include <iostream>
#include <vector>
#include "expression.h"
#include "parser/expression.tab.h"
#include "parser/expression.lexer.h"
using namespace std;

extern expression* result;
extern vector<variable*> vars;

void yyerror(const char *error) {
    cerr << error;
}

int yywrap() {
    return 1;
}

void assign_values(int val) {
	for(auto &v : vars){
		v->set_value(val%2);
		val/=2;
	}
}

int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    string expression;
    cin >> expression;
    yy_scan_string(expression.c_str());
    yyparse();
    int total_cases = 1 << vars.size();
	int true_cases = 0;
	for(int i = 0; i < total_cases; i++){
		assign_values(i);
		true_cases += result->solve();
	}
    if(true_cases == total_cases) cout << "Valid" << endl;
	else if (true_cases == 0) cout << "Unsatisfiable" << endl;
	else cout << "Satisfiable and invalid, " << true_cases << " true and " << total_cases - true_cases << " false cases";
    return 0;
}
