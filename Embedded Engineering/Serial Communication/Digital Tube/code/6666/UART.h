#ifndef _UART_H_
#define _UART_H_

/**
  * @brief  UART模块的初始化
  * @param   None
  * @retval  None
  */
void UART_Init();//4800bps 12.000MHz


/**
  * @brief  串口发送一个字节
  * @param   Byte(要发送的字节)
  * @retval  None
  */
void UART_SendByte(unsigned char Byte);


/**
  * @brief  串口发送字符串
  * @param   str(要发送的字符串)
  * @retval  None
  */
void UART_SendString(unsigned char *str);

/**
  * @brief  发送换行(回车+换行)
  * @param   None
  * @retval  None
  */
void UART_SendNewLine() ;
#endif
