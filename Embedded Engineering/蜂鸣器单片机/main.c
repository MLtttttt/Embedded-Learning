#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"

sbit Buzzer = P2^5;


// 播放速度（四分音符时长，单位 ms），可根据需要修改


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

// 频率对照表（保持不变）
unsigned int FreqTable[] = {
	0,
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

// 乐谱（保持不变）
unsigned char code Music[] = {
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
	0xFF	// 终止标志
};

unsigned int FreqSelect;   // 当前音符对应的频率索引
unsigned int MusicSelect;  // 乐谱索引
unsigned int Speed = 500;  

void main(void) {
	Timer0Init();          // 初始化定时器0
	
	while(1) {
		if (Music[MusicSelect] != 0xFF) {          // 未到结束标志
			FreqSelect = Music[MusicSelect];       // 获取音符频率索引
			MusicSelect++;                         // 指向时值
			Delay(Speed / 4 * Music[MusicSelect]); // 根据时值延时
			MusicSelect++;                         // 指向下一个音符
			TR0 = 0;                               // 暂停定时器
			Delay(5);                              // 音符间短暂停顿
			TR0 = 1;                               // 启动定时器，播放下一个音符
		} else {                                   // 遇到结束标志，重新播放
			MusicSelect = 0;                       // 重置乐谱索引，从头开始
		}
	}
}

// 定时器0中断服务函数（保持不变）
void Timer0_Routine() interrupt 1 {
	if (FreqTable[FreqSelect]) {                  // 如果不是休止符
		TL0 = FreqTable[FreqSelect] % 256;        // 设置定时初值
		TH0 = FreqTable[FreqSelect] / 256;
		Buzzer = !Buzzer;                         // 翻转蜂鸣器IO口
	}
}