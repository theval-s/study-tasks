/*
Код на второй семестр с Безруковым
обновляю после каждой пары
*/
#include <iostream> 
#include <malloc.h>
#include <string>
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

class stroka {
protected:
	int len;
	char* pCh;
public:
	stroka(int = 0);
	stroka(char);
	stroka(const char*);
	stroka(const stroka&);
	~stroka();
	char* GetStr(void) const {
		return pCh;
	}
	int GetLen(void) const {
		return len;
	}
	void Show(void);
};
stroka::stroka(int val) :len(val), pCh(new char[len + 1]) {
	len = val;
	if (val == 0) pCh[0] = '\0';
	else pCh = new char[len + 1];
	cout << "stroka::stroka(int val)" << endl;
}
stroka::stroka(char ch) :len(1), pCh(new char[len + 1])
{
	pCh[0] = ch;
	pCh[1] = '\0';
	cout << "stroka::stroka(char ch)" << endl;
}
stroka::stroka(const char* S) :len(0)
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
	cout << "stroka::stroka(const char* S)" << endl;
}
stroka::stroka(const stroka& from) :len(0)
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
	cout << "stroka::stroka(const stroka& from)" << endl;
}
stroka::~stroka()
{
	if (pCh) delete[] pCh;
	cout << "stroka::~stroka()" << endl;
}
void stroka::Show(void)
{
	cout << "pCh=" << pCh << endl;
	cout << "len=" << len << endl;
}

class IdentStr :public stroka {
public:
	IdentStr(int = 0);
	IdentStr(char);
	IdentStr(const char*);
	IdentStr(const IdentStr&);
	~IdentStr();
	IdentStr& operator=(const IdentStr&);
	char& operator[](int);
	IdentStr operator~();
	friend IdentStr operator+(const IdentStr&, const IdentStr&);
	friend IdentStr operator+(const IdentStr&, const char*);
	friend IdentStr operator+(const char*, const IdentStr&);
};
IdentStr::IdentStr(int val) :stroka(val) {
	cout << "IdentStr::IdentStr(int val)" << endl;
}
IdentStr::IdentStr(char ch) : stroka(ch) {
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
IdentStr::IdentStr(const char* Str) :stroka(Str) {
	for (int i = 0; i < 91; i++) {
		if (strcmp(Str, keyword[i]) == 0) {
			cout << "bad stroka, keyword. pCh =" << pCh << endl;
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
			cout << "bad stroka[" << i << "]=" << pCh[i] << endl;
			if (pCh) delete[] pCh;
			len = 0;
			pCh = new char[len + 1];
			pCh[0] = '\0';
			return;
		}
	}
	cout << "IdentStr::IdentStr(const char* Str)" << endl;
}
IdentStr::IdentStr(const IdentStr& from) :stroka(from) {
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
	if (!((pCh[len-1] >= 'a' && pCh[len-1] <= 'z') || (pCh[len-1] >= 'A' && pCh[len-1] <= 'Z') || pCh[len-1] == '_')) {
		cout << "invalid op, emptying stroka" << endl;
		delete[] pCh;
		len = 0;
		pCh = new char[len + 1];
		pCh[0] = '\0';
		return *this;
	}
	for (int i = 0, j = len-1; i < len / 2; i++, j--) {
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
	cout <<"IdentStr operator+(const IdentStr& id1, const char* id2)"<< endl;
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
class DecStr : public Stroka {
public:
	DecStr(int = 0);
	DecStr(char);
	DecStr(const char*);
	DecStr(const DecStr&);
	~DecStr();
	DecStr& operator=(const DecStr&);
	friend DecStr operator+(const DecStr&, const DecStr&);
	friend DecStr operator+(const DecStr&, const int);
};
DecStr::DecStr(int val) :Stroka(val) {
	cout << "DecStr::DecStr(int val) :Stroka(val)" << endl;
}
DecStr::DecStr(char sym) : Stroka(sym) {
	if (!(sym >= '0' && sym <= '9')) {
		cout << "Bad symbol pCh[0] =" << sym << endl;
		if (pCh) delete[]pCh;
		len = 0;
		pCh = new char[len + 1];
		pCh[0] = '\0';
		return;
	}
	cout << "DecStr::DecStr(char sym)" << endl;
}
DecStr::DecStr(const char* ds) :Stroka(ds) {
	if (!((pCh[0] >= '0' && pCh[0] <= '9') || (pCh[0] == '-') || (pCh[0] == '+'))) {
		cout << "Bad symbol pCh[0]=" << pCh[0] << endl;
		if (pCh) delete[]pCh;
		len = 0;
		pCh = new char[len + 1];
		pCh[0] = '\0';
		return;
	}
	for (int i = 1; i < len; i++) {
		if (!(pCh[i] >= '0' && pCh[i] <= '9')) {
			cout << "Bad symbol pCh[" << i << "]=" << pCh[i] << endl;
			if (pCh) delete[]pCh;
			len = 0;
			pCh = new char[len + 1];
			pCh[0] = '\0';
		}
	}
}
DecStr::DecStr(const DecStr& from) : Stroka(from) {
	cout << "DecStr::DecStr(const DecStr& from) : Stroka(from)" << endl;
}
DecStr::~DecStr() {
	cout << "DecStr::~DecStr()" << endl;
}
DecStr operator+(const DecStr& ob1, const DecStr& ob2)
{
	int n1, n2;
	DecStr tmp(ob1);
	n1 = atoi(tmp.GetStr());
	n2 = atoi(ob2.GetStr());
	long long int A = long long int(n1) + n2;
	if (A < -2147483648 || A > 2147483647) {
		delete[] tmp.pCh;
		tmp.len = 0;
		tmp.pCh = new char[tmp.len + 1];
		tmp.pCh[0] = '\0';
	}
	else {
		char* tmpCh;
		if (tmp.len >= ob2.len) {
			tmpCh = new char[tmp.len + 2];
			_itoa_s(A, tmpCh, tmp.len + 2, 10);
		}
		else {
			tmpCh = new char[ob2.len + 2];
			_itoa_s(A, tmpCh, ob2.len + 2, 10);
		}
		if (tmp.pCh) delete[] tmp.pCh;
		tmp.pCh = tmpCh;
		tmp.len = strlen(tmp.pCh);
	}
	cout << "DecStr operator+(const DecStr& ob1, const DecStr& ob2)" << endl;
	return tmp;
}
DecStr& DecStr::operator=(const DecStr& ds)
{
	if (&ds != this) {
		delete[] pCh;
		len = ds.len;
		pCh = new char[len + 1];
		strcpy_s(pCh, len + 1, ds.pCh);
	}
	cout << "DecStr& DecStr::operator=(const DecStr& ds)" << endl;
	return *this;
}
DecStr operator+(const DecStr& ob1, const int ob2)
{
	DecStr tmp(ob1);
	int n1 = atoi(tmp.GetStr()), n2 = ob2;
	long long int A = long long(n1) + long long(n2);
	if (A < -2147483648 || A > 2147483647) {
		delete[] tmp.pCh;
		tmp.len = 0;
		tmp.pCh = new char[tmp.len + 1];
		tmp.pCh[0] = '\0';
	}
	else {
		char* tmpCh;
		int len2 = 0, tmpn = ob2;
		while (tmpn > 0) {
			len2++;
			tmpn /= 10;
		}
		if (tmp.len < len2) tmp.len = len2;
		tmpCh = new char[tmp.len + 2];
		_itoa_s(A, tmpCh, tmp.len + 2, 10);
		if (tmp.pCh) delete[] tmp.pCh;
		tmp.pCh = tmpCh;
		tmp.len = strlen(tmp.pCh);
	}
	cout << "DecStr operator+(const DecStr& ob1, const int ob2)" << endl;
	return tmp;
}
int main()
{

	//разные блоки под разные тесты - чтоб не было слишком большой кучи мусора в выводе
	//раскомментить нужное и протестить 

	//stroka obj("FBIT");
	//stroka obj1 = obj;
	//obj.Show();
	//obj1.Show();
	//stroka obj2('F');
	//obj2.Show();
	//IdentStr obj7("FBIT");
	//obj7.Show();
	//IdentStr obj123("FI&&&T");
	//obj123.Show();
	//IdentStr kwt("case");
	//kwt.Show();
	//IdentStr obj = "FBIT";
	//obj.Show();
	//IdentStr obj2 = "ITMO";
	//obj2.Show();
	//IdentStr obj3 = " " + obj + "N3149";
	//obj3.Show();
	//DecStr obj1 = "+1234";
	//obj1.Show();
	//DecStr obj2 = "a";
	//obj2.Show();
	//DecStr obj3 = "+12 34";
	//obj3.Show();
	//DecStr testobj1 = '1';
	//testobj1.Show();
	//DecStr testobj2 = 'a';
	//testobj2.Show();
	//DecStr pt1 = "-5";
	//DecStr pt2 = "10";
	//DecStr pt3 = pt1 + pt2;
	//DecStr pt4 = "0123";
	//pt1.Show();
	//pt2.Show();
	//pt3.Show();
	//pt4.Show();
	DecStr pt1 = "2147483648";
	DecStr pt2 = "10";
	pt1.Show();
	pt2.Show();
	pt1 = pt1 + pt2;
	pt1.Show();
	pt2.Show();
	DecStr obj("45535");
	obj = (obj + 45535);
	obj.Show();
	return 0;
}
