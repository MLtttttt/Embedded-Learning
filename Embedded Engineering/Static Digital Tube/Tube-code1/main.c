#include "reg51.h"
#include <intrins.h>

unsigned char s[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Delay(void) {
    unsigned char data i, j, k;
    _nop_();
    _nop_();
    i = 22;
    j = 3;
    k = 227;
    do {
        do {
            while (--k);
        } while (--j);
    } while (--i);
}

void seg(void) {
    unsigned int i;
    for (i = 0; i < 10; i++) {
        P2 = s[i];
        Delay();
    }
}

void main(void) {
    while (1) {
        seg();
    }
}