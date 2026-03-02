#include <reg52.h>

/**
 * 10ms延时函数，用于按键消抖
 */
void Delay10ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}
