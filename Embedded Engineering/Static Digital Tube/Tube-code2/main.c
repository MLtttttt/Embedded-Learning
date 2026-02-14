#include "reg51.h"
#include <intrins.h>

unsigned char s[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned char str[] = {0x76, 0x79, 0x38, 0x38, 0x3F};
unsigned char wei[] = {0x00,0x01, 0x02, 0x03, 0x04, 0x05,0x06,0x07};

void Delay(void)	//@11.0592MHz
{
	unsigned char data i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}



void seg(void) {
    unsigned int i;
    for (i = 0; i < 5; i++) {
      P3=wei[i];
			P2 = str[i];
        Delay();
    }
}

void main(void) {
    while (1) {
        seg();
    }
}