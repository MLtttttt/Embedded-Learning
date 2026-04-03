#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"


    //×Ô¶¨Òå×Ö·û¶¨Òå
    unsigned char code eye[8] = {
        0x04, 0x0E, 0x1F, 0x0E, 0x04, 0x0A, 0x04, 0x00
    };
    unsigned char code face[8] = {
        0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    unsigned char code left_lip[8] = {
        0x00, 0x00, 0x00, 0x08, 0x18, 0x04, 0x02, 0x01
    };
    unsigned char code right_lip[8] = {
        0x00, 0x00, 0x00, 0x02, 0x03, 0x04, 0x08, 0x10
    };
    unsigned char code lip_smile[8] = { 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x0A, 0x04
    };
    unsigned char code lip_open[8] = {   
        0x00, 0x00, 0x00, 0x0E, 0x1F, 0x1F, 0x0E, 0x0E
    };
    unsigned char code nose[8] = {
        0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04
    };
    unsigned char code heart[8] = {
        0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00
    };


void main(void)
{


    LCD_Init();

    //Ð´Èë CGRAM
    LCD_WriteCGRAM(0, eye);
    LCD_WriteCGRAM(1, face);
    LCD_WriteCGRAM(2, left_lip);
    LCD_WriteCGRAM(3, lip_smile);
    LCD_WriteCGRAM(4, right_lip);
    LCD_WriteCGRAM(5, nose);
    LCD_WriteCGRAM(6, heart);
    LCD_WriteCGRAM(7, lip_open);
		
    LCD_ShowString(1, 1, "MLT");             
    LCD_ShowString(2, 1, "dynamic face");  
    Delay(1500);
	
     LCD_Init(); //ÇåÆÁ
    while (1)
    { 
			
        // ----- ±íÇé1£ºÐ¦Á³£¨ÑÛ¾¦+Î¢Ð¦£© -----
        LCD_ShowChar(1, 7, 0x00);      // ×óÑÛ
        LCD_ShowChar(1, 11, 0x00);     // ÓÒÑÛ
        LCD_ShowChar(1, 9, 0x05);      // ±Ç×Ó
        LCD_ShowChar(2, 6, 0x01);      // Á³¼Õ×ó
        LCD_ShowChar(2, 12, 0x01);     // Á³¼ÕÓÒ
        LCD_ShowChar(2, 8, 0x02);      // ×ó×ì½Ç
        LCD_ShowChar(2, 9, 0x03);      // Î¢Ð¦×ì´½
        LCD_ShowChar(2, 10, 0x04);     // ÓÒ×ì½Ç
        Delay(800);
			


        // ----- ±íÇé2£º°®ÐÄÑÛ + ÕÅ×ì -----
        LCD_ShowChar(1, 7, 0x06);      // ×ó°®ÐÄÑÛ
        LCD_ShowChar(1, 11, 0x06);     // ÓÒ°®ÐÄÑÛ
        LCD_ShowChar(1, 9, 0x05);      // ±Ç×Ó
        LCD_ShowChar(2, 6, 0x01);      // Á³¼Õ×ó
        LCD_ShowChar(2, 12, 0x01);     // Á³¼ÕÓÒ
        LCD_ShowChar(2, 8, 0x02);      // ×ó×ì½Ç
        LCD_ShowChar(2, 9, 0x07);      // ÕÅ¿ª×ì´½
        LCD_ShowChar(2, 10, 0x04);     // ÓÒ×ì½Ç
        Delay(800);
    }
}