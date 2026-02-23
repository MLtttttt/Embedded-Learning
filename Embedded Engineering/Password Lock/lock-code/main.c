#include "reg51.h"
#include <intrins.h>

sbit LED = P3^0;

unsigned char code s[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                          0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};


unsigned char code password[4] = {1,2,3,4};

unsigned char input[4];  
unsigned char count = 0; 


void delay(void)	//@11.0592MHz
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


void delay70ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 242;
	k = 7;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

unsigned char scan(void) {
    P1 = 0xFE;  
    if ((P1 & 0xF0) != 0xF0) {
        delay();
        if ((P1 & 0xF0) != 0xF0) {
            if ((P1 & 0xE0) == 0xE0) return 0;
            if ((P1 & 0xD0) == 0xD0) return 1;
            if ((P1 & 0xB0) == 0xB0) return 2;
            if ((P1 & 0x70) == 0x70) return 3;
        }
    }

    P1 = 0xFD; 
    if ((P1 & 0xF0) != 0xF0) {
        delay();
        if ((P1 & 0xF0) != 0xF0) {
            if ((P1 & 0xE0) == 0xE0) return 4;
            if ((P1 & 0xD0) == 0xD0) return 5;
            if ((P1 & 0xB0) == 0xB0) return 6;
            if ((P1 & 0x70) == 0x70) return 7;
        }
    }

    P1 = 0xFB; 
    if ((P1 & 0xF0) != 0xF0) {
        delay();
        if ((P1 & 0xF0) != 0xF0) {
            if ((P1 & 0xE0) == 0xE0) return 8;
            if ((P1 & 0xD0) == 0xD0) return 9;
            if ((P1 & 0xB0) == 0xB0) return 10;   
            if ((P1 & 0x70) == 0x70) return 11;  
        }
    }

    P1 = 0xF7; 
    if ((P1 & 0xF0) != 0xF0) {
        delay();
        if ((P1 & 0xF0) != 0xF0) {
            if ((P1 & 0xE0) == 0xE0) return 12;   
            if ((P1 & 0xD0) == 0xD0) return 13; 
            if ((P1 & 0xB0) == 0xB0) return 14;  
            if ((P1 & 0x70) == 0x70) return 15;  
        }
    }
    return 16; 
}

void main(void) {
    unsigned char key;
    unsigned char i;
    LED = 1;     
    P2 = 0x00;  

    while (1) {
        key = scan();

        if (key < 16) {  
            if (key == 15) {  
                count = 0;
                LED = 1;
                P2 = 0x00;
             
                while(scan() != 16);
                continue;
            }

            if(count < 4) {  
                input[count] = key;
                P2 = s[key];        
                delay70ms();        
                P2 = 0x00;        
                count++;
              
                while(scan() != 16);
            }

            if (count == 4) { 
                for (i = 0; i < 4; i++) {
                    if (input[i] != password[i]) break;
                }
                if (i == 4) {       
                    LED = 0;         
                } else {             
                    for (i = 0; i < 3; i++) {
                        LED = 0;
                        delay70ms();
                        LED = 1;
                        delay70ms();
                    }
                }
                count = 0;          
                while(scan() != 16);
            }
        }
    }
}