#include <REGX52.H>


// 串口初始化 9600波特率 @11.0592MHz
void UART_Init() {
	
SCON = 0x50;    
TMOD = (TMOD & 0x0F) | 0x20; 
TH1 = 0xFD;         // 9600 @11.0592MHz
TL1 = TH1;
TR1 = 1;
ES = 1;             // 开串口中断
EA = 1;             // 开总中断
TI = 0; 
	RI = 0;
}



void UART_SendByte(unsigned char Byte){
	
	SBUF=Byte;
	while(TI==0);
	TI=0;
}


void UART_SendStr(unsigned char *str){
	
    while(*str)
    {
        UART_SendByte(*str);
        str++;
    }
}