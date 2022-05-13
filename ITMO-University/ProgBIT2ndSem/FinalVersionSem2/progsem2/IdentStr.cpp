#include "Stroka.h"
#include "IdentStr.h"
#include <iostream>
#include <malloc.h>
using namespace std;

const char* keyword[] = {
"alignas"					 ,
"alignof"					 ,
"and"						 ,
"and_eq"					 ,
"asm"						 ,
"auto"						 ,
"bitand"					 ,
"bitor"						 ,
"bool"						 ,
"break"						 ,
"case"						 ,
"catch"						 ,
"char"						 ,
"char8_t"					 ,
"char16_t"					 ,
"char32_t"					 ,
"class"						 ,
"compl"						 ,
"concept"					 ,
"const"						 ,
"const_cast"				 ,
"consteval"					 ,
"constexpr"					 ,
"constinit"					 ,
"continue"					 ,
"co_await"					 ,
"co_return"					 ,
"co_yield"					 ,
"decltype"					 ,
"default"					 ,
"delete"					 ,
"do"						 ,
"double"					 ,
"dynamic_cast"				 ,
"else"						 ,
"enum"						 ,
"explicit"					 ,
"export"					 ,
"extern"					 ,
"false"						 ,
"float"						 ,
"for"						 ,
"friend"					 ,
"goto"						 ,
"if"						 ,
"inline"					 ,
"int"						 ,
"long"						 ,
"mutable"					 ,
"namespace"					 ,
"new"						 ,
"noexcept"					 ,
"not"						 ,
"not_eq"					 ,
"nullptr"					 ,
"operator"					 ,
"or"						 ,
"or_eq"						 ,
"private"					 ,
"protected"					 ,
"public"					 ,
"register reinterpret_cast"	 ,
"requires"					 ,
"return"					 ,
"short"						 ,
"signed"					 ,
"sizeof"					 ,
"static"					 ,
"static_assert"				 ,
"static_cast"				 ,
"struct"					 ,
"switch"					 ,
"template"					 ,
"this"						 ,
"thread_local"				 ,
"throw"						 ,
"true"						 ,
"try"						 ,
"typedef"					 ,
"typeid"					 ,
"typename"					 ,
"union"						 ,
"unsigned"					 ,
"using"						 ,
"virtual"					 ,
"void"						 ,
"volatile"					 ,
"wchar_t"					 ,
"while"						 ,
"xor"						 ,
"xor_eq"
};
IdentStr::IdentStr(int val) :Stroka(val) {
	cout << "IdentStr::IdentStr(int val)" << endl;
}
IdentStr::IdentStr(char ch) : Stroka(ch) {
	if (!((pCh[0] >= 'a' && pCh[0] <= 'z') || (pCh[0] >= 'A' && pCh[0] <= 'Z') || pCh[0] == '_')) {
		cout << "Bad symbol, pCh[0] =" << pCh[0] << endl;
		if (pCh) delete[] pCh;
		len = 0;
		pCh = new char[len + 1];
		pCh[0] = '\0';
		return;
	}
	cout << "IdentStr::IdentStr(char ch)" << endl;
}
IdentStr::IdentStr(const char* Str) :Stroka(Str) {
	for (int i = 0; i < 91; i++) {
		if (strcmp(Str, keyword[i]) == 0) {
			cout << "bad Stroka, keyword. pCh =" << pCh << endl;
			delete[] pCh;
			len = 0;
			pCh = new char[len + 1];
			pCh[0] = '\0';
			return;
		}
	}
	if (!((pCh[0] >= 'a' && pCh[0] <= 'z') || (pCh[0] >= 'A' && pCh[0] <= 'Z') || pCh[0] == '_')) {
		cout << "Bad symbol, pCh[0] =" << pCh[0] << endl;
		if (pCh) delete[] pCh;
		len = 0;
		pCh = new char[len + 1];
		pCh[0] = '\0';
		return;
	}
	for (int i = 1; i < len; i++) {
		if (!((pCh[i] >= 'a' && pCh[i] <= 'z') || (pCh[i] >= 'A' && pCh[i] <= 'Z') || (pCh[i] == '_') || (pCh[i] >= '0' && pCh[i] <= '9'))) {
			cout << "bad Stroka[" << i << "]=" << pCh[i] << endl;
			if (pCh) delete[] pCh;
			len = 0;
			pCh = new char[len + 1];
			pCh[0] = '\0';
			return;
		}
	}
	cout << "IdentStr::IdentStr(const char* Str)" << endl;
}
IdentStr::IdentStr(const IdentStr& from) :Stroka(from) {
	cout << "IdentStr::IdentStr(const IdentStr& from)" << endl;
}
IdentStr::~IdentStr() {
	cout << "IdentStr::~IdentStr()" << endl;
}
IdentStr& IdentStr::operator=(const IdentStr& S)
{
	if (&S != this) {
		delete[] pCh;
		len = S.len;
		pCh = new char[len + 1];
		strcpy_s(pCh, len + 1, S.pCh);
	}
	cout << "IdentStr&IdentStr::operator=(const IdentStr& S)" << endl;
	return *this;
}
char& IdentStr::operator[](int index)
{
	if (index >= 0 && index < len) {
		cout << "char& IdentStr::operator[](int index)" << endl;
		return pCh[index];
	}
	return pCh[0];
}
IdentStr IdentStr::operator~()
{
	if (!((pCh[len - 1] >= 'a' && pCh[len - 1] <= 'z') || (pCh[len - 1] >= 'A' && pCh[len - 1] <= 'Z') || pCh[len - 1] == '_')) {
		cout << "invalid op, emptying Stroka" << endl;
		delete[] pCh;
		len = 0;
		pCh = new char[len + 1];
		pCh[0] = '\0';
		return *this;
	}
	for (int i = 0, j = len - 1; i < len / 2; i++, j--) {
		char tmp = pCh[i];
		pCh[i] = pCh[j];
		pCh[j] = tmp;
	}
	cout << "IdentStr IdentStr::operator~()" << endl;
	return *this;
}
IdentStr operator+(const IdentStr& id1, const IdentStr& id2)
{
	IdentStr res(id1.len + id2.len);
	//strcpy_s(res.pCh, res.len + 1, id1.GetStr());
	char* sptr = id1.pCh;
	char* dptr = res.pCh;
	while (*sptr != '\0') {
		*dptr = *sptr;
		dptr++;
		sptr++;
	}
	sptr = id2.pCh;
	while (*sptr != '\0') {
		*dptr = *sptr;
		dptr++;
		sptr++;
	}
	*dptr = '\0';
	cout << "IdentStr operator+(const IdentStr& id1, const IdentStr& id2)" << endl;
	return res;
}
IdentStr operator+(const IdentStr& id1, const char* id2)
{
	IdentStr tmp(id2);
	IdentStr res(id1.len + tmp.len);
	int i = 0, j = 0;
	while (res.pCh[i++] = id1.pCh[j++]);
	i--;
	j = 0;
	while (res.pCh[i++] = tmp.pCh[j++]);
	//это - реализация не через стд функции, такая, как надо безрукову
	//использовать можно для ВСЕХ случаев когда нужно копирование/конкатенация, но мне лень переписывать остальное
	cout << "IdentStr operator+(const IdentStr& id1, const char* id2)" << endl;
	return res;
}
IdentStr operator+(const char* id1, const IdentStr& id2)
{
	IdentStr tmp(id1);
	IdentStr res(id2.len + tmp.len);
	int i = 0, j = 0;
	while (res.pCh[i++] = tmp.pCh[j++]);
	i--;
	j = 0;
	while (res.pCh[i++] = id2.pCh[j++]);
	cout << "IdentStr operator+(const IdentStr& id1, const char* id2)" << endl;
	return res;
	return IdentStr();
}
