#include <iostream> 
#include <malloc.h>
#include <string>
#include "IdentStr.h"
#include "Stroka.h"
#include "DecStr.h"
using namespace std;
int main()
{

	//разные блоки под разные тесты - чтоб не было слишком большой кучи мусора в выводе
	//раскомментить нужное и протестить 

	//Stroka obj("FBIT");
	//Stroka obj1 = obj;
	//obj.Show();
	//obj1.Show();
	//Stroka obj2('F');
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
	DecStr obj50("-2147483648");
	obj50 = obj50 + (-2);
	DecStr obj51("2147483647");
	obj51 = obj51 + 1;
	obj50.Show();
	obj51.Show();
	return 0;
}