#include <REGX52.H>
#include "UART.H"
#include "Delay.H"
#include "IIC.h"

#define MAX_LEN 50   // 最大字符串长度

unsigned char recv_buf[MAX_LEN];   // 接收缓冲区
unsigned char recv_len = 0;   // 已接收长度
bit recv_flag = 0;    // 接收完成标志

void main(void){
    unsigned char i;
    unsigned char str_len; // 从 EEPROM 读出的长度

    UART_Init();
	

    // 1. 上电读出上次保存的字符串长度和内容
    IIC_Start();
    IIC_SendByte(0xA0);
    IIC_WaitAck();
    IIC_SendByte(0x00);  // 地址0存长度
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(0xA1);
    IIC_WaitAck();
    str_len = IIC_ReadByte();
    IIC_SendAck(1);
    IIC_Stop();

    if(str_len > 0 && str_len < MAX_LEN)
    {
        UART_SendStr("last saved: ");
        for(i = 0; i < str_len; i++){
                    
            // 依次读出每个字符
            IIC_Start();
            IIC_SendByte(0xA0);
            IIC_WaitAck();
            IIC_SendByte(0x01 + i);   // 地址1开始存字符
            IIC_WaitAck();
            IIC_Start();
            IIC_SendByte(0xA1);
            IIC_WaitAck();
            UART_SendByte(IIC_ReadByte());
            IIC_SendAck(1);
            IIC_Stop();
        }
        UART_SendStr("\r\n");
    }
    else  UART_SendStr("last saved: No data\r\n");
    
    UART_SendStr("Please send:\r\n");

    // 2. 主循环：等待串口接收完成
    while(1){
            
        if(recv_flag){
                    
            recv_flag = 0;

            // 先存长度到地址0
            IIC_Start();
            IIC_SendByte(0xA0);
            IIC_WaitAck();
            IIC_SendByte(0x00);
            IIC_WaitAck();
            IIC_SendByte(recv_len);
            IIC_WaitAck();
            IIC_Stop();
            Delay(5);

            // 再存每个字符到地址1开始
            for(i = 0; i < recv_len; i++) {
                            
                IIC_Start();
                IIC_SendByte(0xA0);
                IIC_WaitAck();
                IIC_SendByte(0x01 + i);
                IIC_WaitAck();
                IIC_SendByte(recv_buf[i]);
                IIC_WaitAck();
                IIC_Stop();
                Delay(5);
            }

            UART_SendStr("Saved: ");
            for(i = 0; i < recv_len; i++) UART_SendByte(recv_buf[i]);
            UART_SendStr("\r\n");

            recv_len = 0;
            UART_SendStr("Finsh!\r\n");
        }
    }
}

void UART_Isr(void) interrupt 4
{
    unsigned char c;
    if (RI) {
        RI = 0;
        c = SBUF;

        SBUF = c;
        while (!TI); 
        TI = 0;

        if (c == '\r' || c == '\n') {
            if (recv_len > 0) {
                if (recv_len < MAX_LEN) recv_buf[recv_len] = '\0';
                recv_flag = 1;
            }
        } else {
            if (recv_len < MAX_LEN - 1) {
                recv_buf[recv_len++] = c;
            }
        }
    }
}