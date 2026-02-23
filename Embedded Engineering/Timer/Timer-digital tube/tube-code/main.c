#include <reg52.h>
#include <intrins.h>

sbit dig_ten = P3^0;
sbit dig_unit = P3^1;

#define SEG P2

unsigned char code seg[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
unsigned char cnt = 0;
unsigned char buf[2] = {0x3f, 0x3f};
unsigned int times = 0;

void init(void){
    TMOD &= 0xF0;
    TMOD |= 0x01;
    TL0 = 0x00;
    TH0 = 0x4C;      // 11.0592MHz 定时50ms
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    EA  = 1;
}

void delay(void)    // 约3ms @11.0592MHz
{
    unsigned char i, j;
    _nop_();
    _nop_();
    i = 33;
    j = 66;
    do {
        while (--j);
    } while (--i);
}

void main(void){
    init();
    while(1) {

        SEG = 0x00;
        dig_ten = 1;
        dig_unit = 1;
        SEG = buf[0];
        dig_ten = 0;
        delay();

        SEG = 0x00;
        dig_ten = 1;
        dig_unit = 1;
        SEG = buf[1];
        dig_unit = 0;
        delay();
    }
}

void isr(void) interrupt 1{
    TL0 = 0x00;
    TH0 = 0x4C;

    times++;
    if(times >= 20){
        times = 0;
        cnt++;
        if(cnt >= 100)  cnt = 0;
        buf[0] = seg[cnt / 10];
        buf[1] = seg[cnt % 10];
    }
}