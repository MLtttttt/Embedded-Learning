#include <REGX52.H>
#include <INTRINS.H>

#define MAX_TASKS 2 //任务数量
#define MAX_TASK_DEP 32 //32字节 每个任务堆栈深度

unsigned char idata task_sp[MAX_TASKS];//保存每个任务的堆栈指针
unsigned char idata task_stack[MAX_TASKS][MAX_TASK_DEP];//每个任务的私有堆栈区
unsigned char task_id;//任务id

typedef enum{
  TASK_RUNNING,//任务运行状态
	TASK_SUSPENDED //任务挂起状态
}TaskStatus;


typedef struct{
  unsigned char id;//任务id
	TaskStatus status;//任务状态
  unsigned int delay_count;//延迟计数器
	unsigned int delay_time;//延迟时间
}Task;


Task idata tasks[MAX_TASKS] = {
    {0,TASK_RUNNING,0,0},
    {1,TASK_RUNNING,0,0}
};

//void Delay1000ms(void);
//改用定时器中断，降低消耗cpu
void Timer0_Init(void);
void Timer1_Init(void);//用来兜底保证任务有序切换进行
void sleep(unsigned int,unsigned int);//任务id,delay_ms
void task_switch(void);//任务切换
void task0(void);
void task1(void);
void task_load(unsigned int fn,unsigned char tid);

void main()
{   
	  Timer0_Init();
		task_load(task0,0);//加载任务0，将其地址放入任务0的堆
	  task_load(task1,1);//加载任务1，将其地址放入任务0的堆
	  task_id = 0;//从任务0开始运行
	  SP=task_sp[0];//将堆栈指针指向任务0的堆栈
}

/*void Delay1000ms()		
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 154;
	k = 122;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
*/

void task_switch()
{
   task_sp[task_id]=SP;//保存当前任务的SP
	
	task_id=task_id+1;//切换下一个任务
	if(task_id==MAX_TASKS)
	{
	  task_id=0;//到达最大任务数后，继续从任务0开始
	}
	SP=task_sp[task_id];//栈指针指向下一任务的堆栈
}

void task0()
{
	P1_0 = 0;//初始化低电平LED点亮
  while(1)
	{
		//如果该任务仍处于挂起状态，则切换任务，跳过任务后续操作
		if(tasks[0].status == TASK_SUSPENDED){
		   task_switch();
			continue;
		}
		//挂起自身，让出CPU给其他任务
		sleep(0,500);//LED1是0.5s亮一次
		//Delay1000ms();
		P1_0=~P1_0;
		task_switch();//主动切换任务
	}
}

void task1()
{
	P1_7=0;
  while(1)
	{
		if(tasks[1].status == TASK_SUSPENDED){
		  task_switch();
			continue;
		}
		
		sleep(1,1000);
		//Delay1000ms();
		P1_7=~P1_7;
		task_switch();
	}
}


// fn:任务函数的入口地址，tid:任务ID
void task_load(unsigned int fn,unsigned char tid)
{
	
  //task_sp[tid]=  task_stack[tid]+1;
	task_stack[tid][1] = (unsigned char)(fn >> 8); //高8位存入堆栈[1]
  task_stack[tid][0] = (unsigned char)fn; //低8位存入堆栈[0]
	
	task_sp[tid] = (unsigned char)&task_stack[tid][1];// 堆栈指针指向该任务的内存地址
	// SP指向高字节，RET时从高地址向低地址弹出任务地址
	//从 SP 指向的地址取出高字节[1]
  //SP 自动减 1，指向[0]
  //再从 SP 指向的地址取出低字节[0]
	
}


void sleep(unsigned int task_id,unsigned delay_ms){
    
	tasks[task_id].status = TASK_SUSPENDED;// 任务进入挂起状态
	tasks[task_id].delay_count = 0;//0开始起计时
	tasks[task_id].delay_time = delay_ms;//设置需要延时的时间

}



void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA = 1;
}


void Timer0_Isr(void) interrupt 1
{
	unsigned char i; 
	
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	
	for(i = 0;i<MAX_TASKS;i++){
	  if(tasks[i].status == TASK_SUSPENDED) tasks[i].delay_count++;//任务处于挂起状态则延时计数器+1
    	if(tasks[i].delay_count>= tasks[i].delay_time){//达到需要的延时时间
	    tasks[i].status =TASK_RUNNING;//睡眠结束，设为运行状态
		  tasks[i].delay_count = 0;//清0复位
	}
	}
}


void Timer1_Init(void)		//100微秒@11.0592MHz
{

	TMOD &= 0x0F;			//设置定时器模式
	TMOD |= 0x10;			//设置定时器模式
	TL1 = 0xA4;				//设置定时初始值
	TH1 = 0xFF;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	EA = 1;
	ET1 = 1;
}

void Timer1_Isr(void) interrupt 3//低优先级，不干扰核心调度
{
	TL1 = 0xA4;				//设置定时初始值
	TH1 = 0xFF;				//设置定时初始值
	task_switch();//确保
}

