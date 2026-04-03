#include <REGX52.H>
#include "IIC.h"

// IIC 微秒级延时 @11.0592MHz 10us
void IIC_Delay(void)	{
	unsigned char data i;

	i = 2;
	while (--i);
}


// 起始信号
void IIC_Start(){
    SDA = 1;
    SCL = 1;
    IIC_Delay();
    SDA = 0;
    IIC_Delay();
    SCL = 0;
}

// 停止信号
void IIC_Stop(){
	
    SDA = 0;
    SCL = 1;
    IIC_Delay();
    SDA = 1;
    IIC_Delay();
}

// 发送一个字节
void IIC_SendByte(unsigned char dat){
    unsigned char i;
    for(i=0; i<8; i++)
    {
        SCL = 0;
        IIC_Delay();
        SDA = (dat & 0x80) ? 1 : 0;
        dat <<= 1;
        SCL = 1;
        IIC_Delay();
    }
    SCL = 0;
}

// 读取一个字节
unsigned char IIC_ReadByte(void){
    unsigned char i, dat = 0;
    SDA = 1;
    for(i=0; i<8; i++)
    {
        SCL = 1;
        IIC_Delay();
        dat <<= 1;
        if(SDA) dat |= 0x01;
        SCL = 0;
        IIC_Delay();
    }
    return dat;
}

// 等待应答
bit IIC_WaitAck(void){
    bit ack;
    SDA = 1;
    SCL = 1;
    IIC_Delay();
    ack = SDA;
    SCL = 0;
    return ack;
}

// 发送应答
void IIC_SendAck(bit ack){
    SCL = 0;
    SDA = ack;
    IIC_Delay();
    SCL = 1;
    IIC_Delay();
    SCL = 0;
}