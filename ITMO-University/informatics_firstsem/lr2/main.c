#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//вариант 17
//Назовем сверткой байта его арифметический(?) сдвиг вправо
//на количество содержащихся в нем нулевых битов.
//Выполнить свертку всех байтов числа.
//
// /0xDEADBEEF -> /0x37152F77
void printBinary(unsigned int n){
	int a[32] = {0};
	int i = 0;
	while(n > 0){
		a[i] = n%2;
		n/=2;
		i++;
	}
	for(i = 32;i>=0;i--) printf("%d", a[i]);
	printf("\n");

}
int main() {
	//srand(time(NULL));
    //int n = rand();
    int n = 0xDEADBEEF;
    printf("Original Number: %x\nBinary form: ", n);
    printBinary(n);
    for(int i = 3; i >= 0; i--) {
        unsigned int x1 = n;
        x1 = x1 << i*8 >> 24; //берем i-тый с конца байт
        short cnt = 0; //колво нулевых битов
        for(int j = 0;j < 8; j++) {
            //считаем количество нулевых битов в байте
            int tmp = 1 << j;
            cnt += ((x1 & tmp) == 0);
        }
        x1 >>= cnt; //делаем сдвиг вправо на cnt
        x1<<=(3-i)*8; //передвигаем в маске байт на его позицию в изнач. числе
        int wndw = 0xFF << (3-i)*8;
        //делаем маску вида 0x0000FF00 где FF находится на месте байта с которым работаем
        n&= ~wndw; // обнуляем этот байт в числе
        n|= x1; //устанавливаем новое значение
    }

    printf("Result: %x\nBinary form: ", n);
    printBinary(n);
    return 0;
}
