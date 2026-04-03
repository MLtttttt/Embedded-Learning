#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"

//蜂鸣器端口定义
sbit Buzzer=P3^0;

sbit Key_start = P2^0;//开始or暂停按钮
sbit Key_reset = P2^1;// 重新播放按钮

sbit Key_up = P1^0;// 加快按钮
sbit Key_down = P1^1;// 减慢按钮

bit Playing = 0; // 1=播放中，0=暂停

unsigned int Speed = 500; // 播放速度，值为四分音符的时长(ms),可调
//#define SPEED	500//播放速度，不可按键调

//音符与索引对应表，P：休止符，L：低音，M：中音，H：高音，下划线：升半音符号#
#define P	0
#define L1	1
#define L1_	2
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12
#define M1	13
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24
#define H1	25
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36

//索引与频率对照表
unsigned int FreqTable[]={
	0,
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

//乐谱
unsigned char code Music[]={
	//音符,时值,
	
	//1
	P,	4,
	P,	4,
	M1,	2,
	M2,	2,
	
	M3, 2,
	M1, 2,
	M5, 4+2,
	M3, 2,
	
	M2, 4,
	M5, 4,
	M2, 4,
	
	M1, 2,
	L6, 2,
	M3, 4,
	M1, 2,
	
	//2
	L7, 4+4,
	P, 2,
	L5, 2,
	
	L6, 4,
	L7, 4,
	M1, 2,
	M2, 2,
	
	L5, 4,
	M1, 4,
	M2, 2,
	M3, 2,
	
	M4,4,
	P, 2,
	M3, 2,
	M2, 2,
	M1,2,
	
	//3
	M2, 4+2,
	P, 2,
	M1, 2,
	M2, 2,
	
	M3,2,
	M1,2,
	M5,4+2,
	M3,2,
	
	M2,4,
	M5,4,
	M2,4,
	
	M1,2,
	L6,2,
	L6,4,
	L7,2,
	M1,2,
	
	//4
	L5,4+4,
	P,2,
	L5,2,
	
	L6,4,
	L7,4,
	M1,2,
	M2,2,
	
	L5,4,
	M1,4,
	M2,2,
	M3,2,
	
	M4,4,
	M4,2,
	M3,2,
	M2,2,
	M1,2,
	
	//5
	M1,4+4+4,
	
	P,4,
	P,4,
	M3,2,
	M4,2,
	
	M5,4,
	M5,4,
	M5,4,
	
	M5,4,
	M5,2,
	M6,2,
	M5,2,
	M4,2,
	
	//6
	M3,4,
	M3,4,
	M3,4,
	
	M3,2,
	P,2,
	M3,2,
	M4,2,
	M3,2,
	M2,2,
	
	M1,4,
	M1,2,
	P,2,
	M1,2,
	L7,2,
	
	L6,4,
	L7,4,
	L7,2,
	M1,2,
	
	//7
	M2,4,
	M2,2,
	M3,2,
	M2,2,
	M3,2,
	
	M2,4+4,
	M3,2,
	M4,4,
	
	M5,4,
	M5,4,
	M5,4,
	
	M5,4,
	M5,2,
	M6,2,
	M5,2,
	M4,2,
	
	//8
	M3,4,
	M3,4,
	M3,2,
	P,2,
	
	M3,2,
	M4,2,
	M3,2,
	M2,2,
	M1,2,
	L7,2,
	
	L6,4,
	L7,2,
	P,2,
	M1,2,
	M2,2,
	
	L5,4,
	M1,2,
	P,2,
	M2,2,
	M3,2,
	
	//9
	M2,4,
	P,2,
	M2,2,
	M2,2,
	M1,2,
	
	M1,4+4+4,
	
	P,4,
	P,4,
		0xFF,
	0xFF	//终止标志
};


unsigned char FreqSelect,MusicSelect;

void Change_Switch(void) {
	
    if (Key_start == 0) {//检测加速键按下
        Delay(10);
        if (Key_start == 0) {//再次检测
           Playing = !Playing; // 切换播放/暂停状态
					
					  if (Playing == 1 && Music[MusicSelect] == 0xFF)  MusicSelect = 0;// 如果当前是结束状态，按开始从头播放
            
				   if (Playing == 0)  TR0 = 0; // 暂停，停止发声
           else TR0 = 1;  // 恢复，重新启动定时器
            
          while(Key_start == 0);// 等待按键释放
        }
    }
		
		
    if (Key_reset == 0) {//检测减速键按下
        Delay(10);
        if (Key_reset == 0) {//再次检测
            MusicSelect = 0;// 重置乐谱索引
					  Playing = 1; // 开始重新播放
					  TR0 = 1; //确保定时器开启
            while(Key_reset == 0);// 等待按键释放
        }
    }
}



void Change_Speed(void) {
	
    if (Key_up == 0) {//检测加速键按下
        Delay(10);
        if (Key_up == 0) {//再次检测
            if (Speed > 100) Speed -= 100;//下限 100：四分音符最快 0.1 秒
            while(Key_up == 0);// 等待按键释放
        }
    }
		
    if (Key_down == 0) {//检测减速键按下
        Delay(10);
        if (Key_down == 0) {//再次检测
            if (Speed < 2000) Speed += 100;//上限 2000：四分音符最慢 2 秒
            while(Key_down == 0);// 等待按键释放
        }
    }
}



void main(void) {
	Timer0Init();
	
	while(1){
		
		Change_Switch();
		Change_Speed(); 
		
		if (Playing) { 
		if(Music[MusicSelect]!=0xFF) {//如果不是停止标志位	
			FreqSelect=Music[MusicSelect];	//选择音符对应的频率
			MusicSelect++;//到该音符对应的时值
			
			Delay(Speed/4*Music[MusicSelect]);//SPEED/4	就等于四分音符的毫秒数,然后乘对应时值
			
			MusicSelect++;//又到下一个音符
			TR0=0;//停止定时器 
			Delay(5);	//音符间短暂停顿
			TR0=1;// 启动定时器（蜂鸣器发声，播放下一个音符）
		}
	
		else{	//如果是停止标志位
			TR0=0;//停止定时器 
			Playing = 0;
		//while(1);不循环播放
		}
	 }
	}
}

void Timer0_Routine() interrupt 1{
	if(FreqTable[FreqSelect])	//如果不是休止符
	{
		TL0 = FreqTable[FreqSelect]%256;		//设置定时初值
		TH0 = FreqTable[FreqSelect]/256;		//设置定时初值
		Buzzer=!Buzzer;	//翻转蜂鸣器IO口
	}
}
