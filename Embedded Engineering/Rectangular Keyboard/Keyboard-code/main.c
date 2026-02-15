#include "reg51.h"  
#include <intrins.h>  
  
unsigned char code s[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};  
unsigned char num = 0;  
  
void delay(void) {  
    unsigned char i, j;  
    i = 108;  
    j = 145;  
    do {  
        while (--j);  
    } while (--i);  
}  
  
unsigned char scan(void) {  
    unsigned char row, col, key;  
    P1 = 0xF0;  
    if ((P1 & 0xF0) != 0xF0) {  
        delay();  
  
        if ((P1 & 0xF0) != 0xF0) {  
            col = P1 & 0xF0;  
            P1 = 0x0F;  
            row = P1 & 0x0F;  
            switch(col) {  
                case 0xE0: key = 0; break;  
                case 0xD0: key = 1; break;  
                case 0xB0: key = 2; break;  
                case 0x70: key = 3; break;  
                default: return 16;  
            }            switch(row) {  
                case 0x0E: key += 0; break;  
                case 0x0D: key += 4; break;  
                case 0x0B: key += 8; break;  
                case 0x07: key += 12; break;  
                default: return 16;  
            } 
            // ?? key = row*4 + col
                   while((P1 & 0x0F) != 0x0F);  
            return key;  
        }    }    return 16;  
}  
  
void display(void) {  
    P2 = s[num];  
}  
  
void main(void) {  
    unsigned char key;  
    P2 = 0x00;  
    while(1) {  
        key = scan();  
        if(key < 16) {  
            num = key;  
        }        display();  
    }}