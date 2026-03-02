#ifndef _UART_H
#define _UART_H

void Uart_Init();
void Uart_Send(unsigned char dat);
void Uart_result();
void Send_grade(unsigned char count);
void Uart_assess(char *str);

#endif