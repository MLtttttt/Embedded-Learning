#ifndef __LCD1602_H__
#define __LCD1602_H__

void LCD_Init(void);
void LCD_ShowChar(unsigned char Line, unsigned char Column, unsigned char Char);
void LCD_ShowString(unsigned char row, unsigned char col, unsigned char *str);
void LCD_WriteCGRAM(unsigned char index, unsigned char *pattern);

#endif