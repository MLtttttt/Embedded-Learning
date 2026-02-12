#include "reg52.h"

sbit key = P1^0;   
sbit LED0 = P2^0;

bit direction = 0;   // 0: left->right  1: right->left
bit flag = 0;        // 0: stop flow 1: run flow

void delay(unsigned int n){
    unsigned int i = 0, j = 0;
    for(i = 0; i < n; i++) {
        for(j = 0; j < 120; j++);
    }
}

void deb(void){
    delay(20); 
}

void l_to_r(void){
    unsigned char i;
    for(i = 0; i < 8; i++){
        P2 = ~(0x01 << i);  
        delay(100);
        
        if(key == 0){
            deb();
            if(key == 0){
                flag = 0;       
                P2 = 0xFF;       
                while(key == 0);
                deb();
                return;       
            }
        }
    }
}

void r_to_l(void){
    unsigned char i;
    for(i = 0; i < 8; i++){
        P2 = ~(0x80 >> i);   
        delay(100);
        
        if(key == 0){
            deb();
            if(key == 0){
                flag = 0;
                P2 = 0xFF;
                while(key == 0);
                deb();
                return;
            }
        }
    }
}

void main(){
    P2 = 0xFF;   
    
    while(1){
     
        if(key == 0){
            deb();
            if(key == 0){
                if(flag == 0){
                    flag = 1;
                    direction = ~direction; 
                }
                else{
                    flag = 0;
                    P2 = 0xFF;
                }
                while(key == 0);  
                deb();
            }
        }
  
        if(flag == 1){
            if(direction == 0){
                l_to_r(); 
            }
            else{
                r_to_l();
            }
        }
    }
}