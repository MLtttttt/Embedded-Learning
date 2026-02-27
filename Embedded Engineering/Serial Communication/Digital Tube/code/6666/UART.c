#include <REGX52.H>

/**
  * @brief  UART模块的初始化
  * @param   None
  * @retval  None
  */
void UART_Init()//4800bps 12.000MHz
{
		SCON=0x50;   //八位数据,用于发送和接收
	  PCON |=0x80; //波特率倍速
		TMOD &=0x0F; //清除定时器1模式位
	  TMOD |=0x20; //设置定时器1为8为自动重装方式
		TL1=0xF3;    //设定定时初值 
		TH1=0xF3;    //设置定时器重装值
		ET1=0;       //禁止定时器1中断
		TR1=1;       //启动定时器1
	  EA=1;        //打开总中断开关
	  ES=1;        //打开串口中断
}


/**
  * @brief  串口发送一个字节
  * @param   Byte(要发送的字节)
  * @retval  None
  */
void UART_SendByte(unsigned char Byte)
{
		SBUF=Byte;
		while(TI==0);
		TI=0;
}


/**
  * @brief  串口发送字符串
  * @param   str(要发送的字符串)
  * @retval  None
  */
void UART_SendString(unsigned char *str) 
{
    while(*str!='\0')     //判断字符串截止位则退出
	{
		UART_SendByte(*str);  
		str++;
	}
}


/**
  * @brief  发送换行(回车+换行)
  * @param   None
  * @retval  None
  */
void UART_SendNewLine() 
{
    UART_SendByte('\r');  // 回车
    UART_SendByte('\n');  // 换行
}


/**
  * @brief  串口中断函数模板
  * @param   None
  * @retval  None
  */
//void UART_Rountine() interrupt 4
//{
// 
//	
//}
