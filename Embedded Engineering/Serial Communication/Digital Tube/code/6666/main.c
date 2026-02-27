#include <REGX52.H>
#include "UART.h"
#include "Nixie.h"
#include "Delay.h"
 
void main()
{
	 UART_Init();
  
	while(1)
  {
		ShowNum(0);
  }
}

void UART_Rountine() interrupt 4
{
  if(RI==1)
	{
	 ShowNum(0);
	 RI=0;
	}
}