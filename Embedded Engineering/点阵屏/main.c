#include "reg52.h"

sbit key0 = P1^0; 

unsigned char code tab[]={
    0x00,0x7E,0xFF,0xC3,0xC3,0xFF,0x7E,0x00, //0
    0x00,0x00,0x43,0xFF,0xFF,0x03,0x00,0x00, //1
    0x00,0x63,0xC7,0xCF,0xDB,0xF3,0x63,0x00, //2
    0x00,0x42,0xDB,0xDB,0xDB,0xFF,0x66,0x00, //3
    0x00,0x3E,0x46,0xFF,0xFF,0x06,0x06,0x00, //4
    0x00,0xF6,0xFF,0xD3,0xD3,0xDF,0xDE,0x00, //5
    0x00,0x7E,0xFF,0xDB,0xDB,0xDF,0x4E,0x00, //6
    0x00,0xC0,0xC0,0xC7,0xFF,0xF8,0xC0,0x00, //7
    0x00,0xFF,0xFF,0xDB,0xDB,0xFF,0x7E,0x00, //8 
    0x00,0x72,0xFB,0xDB,0xDB,0xDF,0x7E,0x00, //9
};

unsigned char row[]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
unsigned char num = 0;


void Delay1ms(void)	//@11.0592MHz
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


void Delay10ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}



void key() {
    static bit flag = 0;         
    if(key0 == 0 && flag == 0) {             
            Delay10ms();            // 延时去抖
            if(key0 == 0) {         // 再次确认按下
                num++;
                if(num >= 10) num = 0;  // 限制0~9循环
                flag = 1;          // 标记已处理本次按下
            }
        
    } else  press = 0;   // 清除标记
    
}

// 点阵扫描显示
void matrix() {
    unsigned char i;
    for(i = 0; i < 8; i++) {                // 扫描8行
        P3 = ~row[i];                         // 输出行选（取反，低电平有效）
        P2 = tab[8 * num + i];                 // 输出当前行的列数据
        Delay1ms();                            // 每行停留1ms，保证亮度均匀
    }
    // 可选：关闭所有显示，避免拖影
    // P3 = 0xFF;
    // P2 = 0x00;
}

void main() {
    while(1) {
        matrix();   // 不断刷新点阵
        key();      // 检测按键
    }
}