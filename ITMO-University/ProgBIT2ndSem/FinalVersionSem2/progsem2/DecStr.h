#ifndef __DECSTR__
#define __DECSTR__
#include "Stroka.h"
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
#endif