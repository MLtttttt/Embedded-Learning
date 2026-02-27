#include <REGX52.H>

unsigned char NixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71}; 
/**
  * @brief  数码管显示函数
  * @param   要显示的数字(字符)
  * @retval  None
  */
void ShowNum(unsigned char Num)
{	
	switch(Num)
			{
				case '0' : P2=NixieTable[0]; break;
				case '1' : P2=NixieTable[1]; break;
				case '2' : P2=NixieTable[2]; break;
				case '3' : P2=NixieTable[3]; break;
				case '4' : P2=NixieTable[4]; break;
				case '5' : P2=NixieTable[5]; break;
				case '6' : P2=NixieTable[6]; break;
				case '7' : P2=NixieTable[7]; break;
				case '8' : P2=NixieTable[8]; break;
				case '9' : P2=NixieTable[9]; break;
			}
}