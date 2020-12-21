#include <reg52.h>	         //调用单片机头文件
#include <intrins.h>
#include "string.h"
#include "stdio.h"
#include "oled.h"

#define uchar unsigned char  
#define uint  unsigned int	 

sbit KEY0=P3^7;
sbit KEY1=P3^6;

int key_scan()
{
	static uchar key_up=1;//按键按松开标志
 
	if(key_up&&(KEY0==0||KEY1==0))//||WK_UP==1))
	{
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
	}else if(KEY0==1&&KEY1==1)key_up=1; 	    
 	return 0;// 无按键按下
}



void init_UART()
{
	PCON &= 0x7F;		//波特率不倍速
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
  SCON = 0x50;   //串口方式一
  TH1 = 0XFD;
  TL1 = 0XFD;
	RI = 0;
  EA = 1;        //运行总中断
  ES = 1;        //运行串口中断
  TR1 = 1;       //定时器启动
}

/*-----------------发送一个字节-----------------*/
void SendByte(unsigned char dat)
{
	 TI = 0;
	 SBUF = dat;
	 while(!TI);
	 	TI = 0;
}


/*-----------------发送一个字符串---------------*/
void SendStr(unsigned char *s)
{

	 while(*s!='\0')// \0 表示字符串结束标志，通过检测是否字符串末尾
	 {
	  	SendByte(*s);
	  	s++;
	 }

}

char temp;								
char Rec[34];																	//接收数组
char data_count = 0;													//接收次数
char temp_length;  														//数据长度
bit flag_r = 0;											//标志量	

void UART1_Interrupt() interrupt 4	using 3	//串口中断函数接收程序
{
//		temp = 0;
		if (RI == 1)
		{
			temp = SBUF;
			RI = 0;
			if(temp!='\n') //判断是否接收到结束符
			{
					Rec[data_count]=temp;// 否，就存到RevBuf【】数组中
					data_count++;
			}
			else if (temp == '\n')
			{
					temp_length=data_count;//是，记录其数据长度
					data_count=0;
				  flag_r = 1;							//结束接收标志位

			}
		}
}

/***************主函数*****************/
void main()
{
	char key = 0, i = 0;
	char j= 0;							
	char str[10]=0;
	
	KEY0 = 1;KEY1 =1;
	
	init_UART();								//初始化串口
	
	OLED_Init();								//oled初始化
	OLED_Clear();								//清屏
	OLED_ShowNum(0,0,0,2,12);
	OLED_Clear();

	while(1)
	{
		key = key_scan();					//独立按键程序
		
		if (key == 1)							
		{							
			SendStr("testTEST1234\n");		//发送数据		
		}
		
		else if (key == 2)
		{	
			sprintf(str,"%d \n",j);        //按下后串口发送数据加1
			SendStr(str);
			j++;
		}
	
		if (flag_r == 1)
		{
			OLED_Clear();										//oled清屏	
			OLED_ShowString(0,0,Rec,12);		//显示接收数据
			memset(Rec, 0, sizeof (Rec));		//清空接收数组
			flag_r = 0;
		}
	}
}

