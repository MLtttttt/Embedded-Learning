#include "reg51.h"

sbit Led = P1^0;
unsigned char ch;      

void Uart1_Init(void)        // 9600bps@11.0592MHz
{
    SCON = 0x50; 
	   TMOD &= 0x0F;
    TMOD = 0x21;              
    TH1 = 0xFD;           
    TL1 = 0xFD;
    ES = 1;                  
    TR1 = 1;                 
    ET1 = 0;                  
    EA = 1;                 
}

void Delay10ms(void)          // @11.0592MHz
{
    unsigned char data i, j;
    i = 108;
    j = 145;
    do {
        while (--j);
    } while (--i);
}

void Send(unsigned char dat) {
    SBUF = dat;
    while (!TI);
    TI = 0;
}

void main() {
    Uart1_Init();
    Led = 0;                 
    while(1) {

    }
}

void isr() interrupt 4 {
    ch = SBUF;                
    RI = 0;                   
    if (ch == '1') Led = 1;   
    else if (ch == '0') Led = 0; 
}