#include "Stroka.h"
#include "DecStr.h"
#include <iostream>
#include <malloc.h>
using namespace std;
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