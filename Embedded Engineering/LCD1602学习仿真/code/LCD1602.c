#include <REGX52.H>


sbit LCD_RS = P1^0;   // 寄存器选择
sbit LCD_RW = P1^1;   // 读/写选择
sbit LCD_E  = P1^2;   // 使能信号


// 数据端口：LM016L 的 D0~D7 接 P2.0~P2.7
#define LCD_DataPort P2

// 短延时函数（12MHz 晶振）
void LCD_Delay(void)
{
    unsigned char data i, j;
    i = 2;
    j = 239;
    do
    {
        while (--j);
    } while (--i);
}

// 写命令
void LCD_WriteCommand(unsigned char Command)
{
    LCD_RS = 0;               // 命令模式
    LCD_RW = 0;               // 写模式
    LCD_DataPort = Command;   // 放置命令
    LCD_E = 1;                // 产生一个高脉冲
    LCD_Delay();
    LCD_E = 0;
    LCD_Delay();
}

// 写数据
void LCD_WriteData(unsigned char Data)
{
    LCD_RS = 1;               // 数据模式
    LCD_RW = 0;               // 写模式
    LCD_DataPort = Data;      // 放置数据
    LCD_E = 1;                // 产生一个高脉冲
    LCD_Delay();
    LCD_E = 0;
    LCD_Delay();
}

// LCD 初始化
void LCD_Init(void)
{
    LCD_WriteCommand(0x38);   // 8 位数据接口，2 行显示，5×7 点阵
    LCD_WriteCommand(0x0C);   // 显示开，光标关，闪烁关
    LCD_WriteCommand(0x06);   // 写入后地址自动加 1，光标右移
    LCD_WriteCommand(0x01);   // 清屏
}

// 在指定位置显示一个字符
void LCD_ShowChar(unsigned char Line, unsigned char Column, unsigned char Char)
{
    // 计算 DDRAM 地址
    if (Line == 1)
        LCD_WriteCommand(0x80 | (Column - 1));
    else
        LCD_WriteCommand(0x80 | ((Column - 1) + 0x40));

    LCD_WriteData(Char);
}


void LCD_ShowString(unsigned char row, unsigned char col, unsigned char *str)
{
    while (*str != '\0')
    {
        LCD_ShowChar(row, col++, *str++);
    }
}



// 写入自定义字符到 CGRAM
// index: 0~7（自定义字符编号）
// pattern: 长度为 8 的数组，每个元素对应一行点阵（低5位有效）
void LCD_WriteCGRAM(unsigned char index, unsigned char *pattern)
{
    unsigned char i;
    // CGRAM 地址 = 0x40 + index * 8
    LCD_WriteCommand(0x40 | (index << 3));
    for (i = 0; i < 8; i++)
    {
        LCD_WriteData(pattern[i]);
			//每写一个 LCD_WriteData，LCD内部地址指针会自动+1，所以不需要重复设置地址

    }
}

