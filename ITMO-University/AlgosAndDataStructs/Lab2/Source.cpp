#include <iostream>
#include <stack>
#include <string> 
#include <map>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
using namespace std;

/// <summary>
/// Конвертирует выражение в обратную польскую нотацию 
/// (числа с минусом не выносятся как отдельная операция, а преобразуются в токен числа с минусом)
/// </summary>
/// <param name="s">Исходное выражение</param>
/// <returns>Вектор токенов, представляющий собой обратную польскую нотацию исх. выражения</returns>
vector<string> ConvertToRPN(string &s) {
	vector<string> out;
	map<char, int> prio{ {'+',1}, {'-',1}, {'*',2}, {'/',2}, {'^',3}, {'~',4}, {'(', 0} };
	stack<char> ops;
	for (int i = 0; i < s.length(); i++) {
		char a = s[i];
		switch (a) {
		case '+': {
			while(!ops.empty() && prio[ops.top()] >= prio[a]){
			out.push_back(string(1, ops.top()));
			ops.pop();
			}
			ops.push(a);
			break;
		}

		case '-': {
			//checks if this is unary minus
			if (i == 0 || i >= 1 && !(s[i - 1] >= '0' && s[i - 1] <= '9')) {
				s[i] = '~';
				i--;
				continue;
			}
			 
			//if this is binary minus 
			else {
				while (!ops.empty() && prio[ops.top()] >= prio[a]) {
					out.push_back(string(1, ops.top()));
					ops.pop();
				}
				ops.push(a);
			}
			break;
		}

		case '*': {
			while (!ops.empty() && prio[ops.top()] >= prio[a]) {
				out.push_back(string(1, ops.top()));
				ops.pop();
			}
			ops.push(a);
			break;
		}

		case '/': {
			while (!ops.empty() && prio[ops.top()] >= prio[a]) {
				out.push_back(string(1, ops.top()));
				ops.pop();
			}
			ops.push(a);
			break;
		}

		case '^': {
			while (!ops.empty() && prio[ops.top()] >= prio[a]) {
				out.push_back(string(1, ops.top()));
				ops.pop();
			}
			ops.push(a);
			break;
		}

		case '(': {
			ops.push(a);
			break;
		}

		case ')': {
			//идём вверх по стеку операций, пока не встретим '(' (или конец)
			while(ops.top() != '(') {
				out.push_back(string(1, ops.top()));
				ops.pop();
			}
			ops.pop();
			break;
		}

		case '~': {
			string num = "-";
			i++;
			a = s[i];
			while (i < s.length() && a >= '0' && a <= '9' || a == '.') {
				num += a;
				i++;
				a = s[i];
			}
			i--;
			out.push_back(num);
			break;
		}
		default: {
			string num = "";
			while(i < s.length() && a >= '0' && a <= '9' || a == '.'){
				num += a;
				i++;
				a = s[i];
			}
			i--;
			out.push_back(num);
			break;
		}
		}
	}
	while (!ops.empty()) {
		out.push_back(string(1,ops.top()));
		ops.pop();
	}
	return out;
}
/// <summary>
/// Рассчитывает значение выражения, полученного после использования ConvertToRPN
/// </summary>
/// <param name="tok">Вектор токенов</param>
/// <returns>Числовой результат выражения</returns>
long double EvaluateRPN(const vector<string> &tok) {
	stack<long double> nums;
	for (string i : tok) {
		if (i == "+") {
			long double a, b;
			a = nums.top();
			nums.pop();
			b = nums.top();
			nums.pop();
			nums.push(b+a);
		}
		else if (i == "-") {
			long double a, b;
			a = nums.top();
			nums.pop();
			b = nums.top();
			nums.pop();
			nums.push(b-a);
		}
		else if (i == "*") {
			long double a, b;
			a = nums.top();
			nums.pop();
			b = nums.top();
			nums.pop();
			nums.push(b*a);
		}
		else if (i == "/") {
			long double a, b;
			a = nums.top();
			nums.pop();
			b = nums.top();
			nums.pop();
			nums.push(b/a);
		}
		else if (i == "^") {
			long double a, b;
			a = nums.top();
			nums.pop();
			b = nums.top();
			nums.pop();
			nums.push(powl(b,a));
		}
		else nums.push(strtod(i.c_str(), NULL));
	}
	return nums.top();
}



int main() {
	//freopen("input.txt", "r", stdin);

	cout << "Input: ";
	string s;
	cin >> s;
	cout << endl;

	vector<string> expr = ConvertToRPN(s);
	cout << "Parse result: ";
	for (auto i : expr) {
		cout << i << " ";
	}
	cout << endl;

	double ans = EvaluateRPN(expr);
	cout << "Output: " << ans << endl;
	return 0;
}

/*
questions:
-можно ли использовать stl структуры?

-если есть унарный минус '~' почему в тестовом примере для унарного минуса используется просто '-'?
получается необходимо различать где '-' это унарный оператор, а где бинарный?

- c какой точностью выводить ответ?
*/