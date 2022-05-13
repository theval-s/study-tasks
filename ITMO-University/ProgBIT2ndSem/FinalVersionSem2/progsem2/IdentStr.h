#ifndef __IDENTSTR__
#define __IDENTSTR__
#include "Stroka.h"
class IdentStr :public Stroka {
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
#endif