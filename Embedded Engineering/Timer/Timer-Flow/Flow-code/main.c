#include <reg52.h>

unsigned int  cnt = 0; 
unsigned char num = 0;

void init() {
    TMOD &= 0xF0;       
    TMOD |= 0x01;     
    TL0 = 0x66;
    TH0 = 0xFC;
    TF0 = 0;
    TR0 = 1;            
    ET0 = 1;          
    EA  = 1;     
     PT0 = 0;       
}

void isr(void) interrupt 1 {
    TL0 = 0x66;
    TH0 = 0xFC; 
    cnt++;

    if (cnt >= 1000) {   // 1s=1000 * 1ms
        cnt = 0;
        num++;      
        if (num >= 8) num = 0;
        P2 = ~(0x01 << num); 
    }
}

void main() {
    init();
    P2 = ~0x01;          
    while (1) {
    
    }
}