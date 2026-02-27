#include <reg51.h>
#include <intrins.h>

unsigned char rec = 0;
bit flag = 0;


void init_scon() {
    SCON = 0X50; //0101 0000
    TMOD |= 0x20; //0010 0000
    TL1 = 0xFD;
    TH1 = 0xFD;
    ES = 0;
    EA = 1;
    TR1 = 1;
}

void delay(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void send() {
    SBUF = rec;
    while (!TI);
    TI = 0;
}

void main() {
    init_scon();
    while (1) {
         char *str = "Hello 51! Hello QG!\r\n";
        while(*str){
            rec=*str++;
            send();
           //delay()
        }
				  delay();
    }
}

