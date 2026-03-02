#include <reg52.h>
extern unsigned char buf[];


/**
 * 串口初始化函数
 */
void Uart_Init(void) { //9600bps @11.0592MHz
    SCON = 0x50;          // 设置串口控制寄存器：模式1（8位UART），允许接收（REN=1）
    TMOD &= 0x0F;         // 清除定时器1的模式位（保留定时器0的设置）
    TMOD |= 0x20;         // 设置定时器1为模式2（8位自动重装）
    TH1 = 0xFD;           // 波特率重装值高位
    TL1 = 0xFD;           // 定时器1初值，与TH1相同（模式2下自动重装）
    TR1 = 1;              // 启动定时器1
}

/**
 * 串口发送单字节函数（查询方式）
 * @param dat 待发送的字节数据
 */
void Uart_Send(unsigned char dat) {
    SBUF = dat;  
    while (!TI); 
    TI = 0; 
}

/**
 * 串口发送当前时间函数
 */
void Uart_result(void) {
    Uart_Send(buf[0] + '0');   // 分钟十位
    Uart_Send(buf[1] + '0');   // 分钟个位
    Uart_Send(':');            // 分隔符
    Uart_Send(buf[3] + '0');   // 秒十位
    Uart_Send(buf[4] + '0');   // 秒个位
    Uart_Send('.');            // 小数点
    Uart_Send(buf[6] + '0');   // 毫秒十位（实际为厘秒十位）
    Uart_Send(buf[7] + '0');   // 毫秒个位（厘秒个位）
    Uart_Send('\r');           // 回车
    Uart_Send('\n');           // 换行
}

/**
 * 串口发送字符串函数
 * @param str 以'\0'结尾的字符串指针
 */
void Uart_assess(char *str) {
    while (*str) {             // 遍历字符串直到结束符
        Uart_Send(*str++);     // 逐个发送字符，并移动指针
    }
}

/**
 * 发送评级函数
 * 根据LED亮灯数量（count）发送对应的等级字符串
 * @param count LED亮灯个数（0~3）
 */
void Send_grade(unsigned char count) {
    if (count == 3)
        Uart_assess("Excellent!");   // 3灯亮：Excellent!
    else if (count == 2)
        Uart_assess("Good!");        // 2灯亮：Good!
    else if (count == 1)
        Uart_assess("Pass");         // 1灯亮：Pass
    else
        Uart_assess("Unqualified");  // 其他情况（0灯）：Unqualified
}
