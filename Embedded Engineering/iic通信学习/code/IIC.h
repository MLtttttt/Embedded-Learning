#include <REGX52.H>

#ifndef __IIC_H__
#define __IIC_H__

sbit SDA = P1^0;   // 数据线
sbit SCL = P1^1;   // 时钟线

// 函数声明...
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(unsigned char dat);
unsigned char IIC_ReadByte(void);
bit IIC_WaitAck(void);
void IIC_SendAck(bit ack);

#endif