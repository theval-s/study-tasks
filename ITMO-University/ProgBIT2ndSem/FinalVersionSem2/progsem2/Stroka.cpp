#include "Stroka.h"
#include <iostream>
#include <malloc.h>
using namespace std;

Stroka::Stroka(int val) :len(val), pCh(new char[len + 1]) {
	len = val;
	if (val == 0) pCh[0] = '\0';
	else pCh = new char[len + 1];
	cout << "Stroka::Stroka(int val)" << endl;
}
Stroka::Stroka(char ch) :len(1), pCh(new char[len + 1])
{
	pCh[0] = ch;
	pCh[1] = '\0';
	cout << "Stroka::Stroka(char ch)" << endl;
}
Stroka::Stroka(const char* S) :len(0)
{
	//strcpy_s(pCh, len + 1, S);
	const char* cptr = S;
	while (*(cptr++) != '\0') len++;
	pCh = new char[len + 1];
	char* ptr1 = pCh;
	const char* ptr2 = S;
	for (int i = 0; i <= len; i++) {
		*ptr1 = *ptr2;
		ptr1++;
		ptr2++;
	}
	cout << "Stroka::Stroka(const char* S)" << endl;
}
Stroka::Stroka(const Stroka& from) :len(0)
{
	const char* cptr = from.pCh;
	while (*(cptr++) != '\0') len++;
	pCh = new char[len + 1];
	char* ptr1 = pCh;
	const char* ptr2 = from.pCh;
	for (int i = 0; i <= len; i++) {
		*ptr1 = *ptr2;
		ptr1++;
		ptr2++;
	}
	cout << "Stroka::Stroka(const Stroka& from)" << endl;
}
Stroka::~Stroka()
{
	if (pCh) delete[] pCh;
	cout << "Stroka::~Stroka()" << endl;
}
void Stroka::Show(void)
{
	cout << "pCh=" << pCh << endl;
	cout << "len=" << len << endl;
}
