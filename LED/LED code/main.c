#include"reg52.h"
sbit LED0 = P2^0;


void delay(unsigned int n){
    unsigned int i = 0, j = 0;
    for(i = 0; i < n; i++){
        for(j = 0; j < 120; j++);
    }
}


void l_to_r(){
    unsigned char i = 0;
    for(i = 0; i < 8; i++){
        P2 = ~(0x01 << i); 
        delay(80);
    }
}

void r_to_l(){
    unsigned char i = 0;
    for(i = 0; i < 8; i++){
        P1 = (0x80 >> i);
        delay(80);
    }
}

void main(){
    while(1){
        l_to_r();
        r_to_l();
    }
}