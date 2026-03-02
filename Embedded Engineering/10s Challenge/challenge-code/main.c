#include <reg52.h>
#include <string.h>
#include "Timer0.h" 
#include "Delay.h"   
#include "UART.h" 

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

sbit Key_start = P3^2;  
sbit Key_clear = P3^3;  


sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;

// 数码管段码表（共阳极，0~9及横线）
u8 seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};

// 数码管位选码（高电平有效）
u8 wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

// 显示缓冲区
u8 buf[]={0,0,10,0,0,10,0,0};


u8 min, sec;       // 分钟、秒
u16 msec;          // 毫秒（0~999）
u8 dig_index;      // 数码管动态扫描当前位索引
u8 LED_count = 0;  // 用于记录LED亮灯数量，供串口评级使用

bit flag;          // 计时标志：1运行，0暂停

/**
 * 更新显示缓冲区函数
 * 根据当前的min、sec、msec计算并填充buf数组
 */
void Update_buf(void) {
    u16 temp_ms = msec / 10;// 取毫秒的十位和个位（0~99）
    buf[0] = min / 10;
    buf[1] = min % 10;
    buf[3] = sec / 10;
    buf[4] = sec % 10;
    buf[6] = temp_ms / 10;
    buf[7] = temp_ms % 10;
    // buf[2]和buf[5]固定为10（横线），无需更新
}

/**
 * 成绩判断及LED控制函数
 * 根据当前总时间来决定LED亮灭状态，并记录亮灯数量
 */
void Judge_LED(void) {
    u32 total_time = (u32)min * 60 * 1000 + (u32)sec * 1000 + msec; // 总毫秒数
    u32 t = total_time / 10; // 转换为厘秒（0.01秒为单位）提高可读性

    if(t >= 950 && t <= 1050) { // 9.50~10.50秒
        LED1 = 1, LED2 = 0, LED3 = 0;  
        LED_count = 1; // 记录亮灯数
    }
    else if(t >= 850 && t < 950) {// 8.50~9.50秒
        LED1 = 1, LED2 = 1, LED3 = 0; 
        LED_count = 2;
    }
    else if(t>=750 && t < 850) {// 7.50~8.50秒
        LED1 = 1, LED2 = 1, LED3 = 1;
        LED_count = 3;
    }
}


/**
 * 按键扫描函数（阻塞式）
 * 检测启动/暂停键和清零键，带消抖和等待释放
 */
void Key_Scan(void) {
    // 启动/暂停键处理
    if( Key_start == 0) {     
        Delay10ms();                  
        if(Key_start == 0) {             
            flag = !flag;                 
            if(flag == 0) {               
                Judge_LED();// 成绩判断，点亮LED
                Uart_result();// 串口发送当前时间
                Send_grade(LED_count);// 串口发送评级
                Uart_Send('\r');// 发送回车
                Uart_Send('\n');// 发送换行
            }
            while(Key_start == 0);
        }
    }
		
    // 清零键处理
    else if(Key_clear == 0) { 
        Delay10ms();
        if(Key_clear == 0) {
            flag = 0; 
            min = 0, sec = 0, msec = 0;     // 计时归零
            LED1 = 0, LED2 = 0, LED3 = 0;  
            while(Key_clear == 0);
        }
    }
}

/**
 * 主函数
 */
void main(void) {

    P1 = 0x00;
    P2 = 0x00;
    min = 0, sec = 0, msec = 0;
    flag = 0;
    dig_index = 0;

    Uart_Init();      // 串口初始化
    Timer0_init();    // 定时器0初始化（1ms中断）
    Update_buf();     // 更新显示缓冲区

    while(1) {
        Key_Scan();   // 循环扫描按键
    }
}

/**
 * 定时器0中断服务函数（1ms一次）
 * 负责数码管动态扫描和计时更新
 */
void Timer0_isr(void) interrupt 1 {
    TH0 = 0xFC;       // 11.0592MHz 1ms
    TL0 = 0x66;

    // 数码管动态扫描（每次中断显示一位）
    P2 = 0x00;                        // 消隐，关闭所有位选
    P0 = seg[buf[dig_index]];         // 送当前位的段码
    P2 = wei[dig_index];              // 送当前位的位选
    dig_index = (dig_index + 1) % 8;  // 指向下一位

    // 计时处理（仅在flag为1时累加）
    if(flag) {
        msec++;
        if(msec >= 1000) {            // 满1000ms进1秒
            msec = 0;
            sec++;
            if(sec >= 60) {           // 满60秒进1分钟
                sec = 0;
                min++;
                if(min >= 60) min = 0; // 分钟封顶60
            }
        }
    }
		
    Update_buf();   // 每次中断更新缓冲区，保证显示实时
}