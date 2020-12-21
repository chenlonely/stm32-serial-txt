#include <reg52.h>	         //���õ�Ƭ��ͷ�ļ�
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
	static uchar key_up=1;//�������ɿ���־
 
	if(key_up&&(KEY0==0||KEY1==0))//||WK_UP==1))
	{
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
	}else if(KEY0==1&&KEY1==1)key_up=1; 	    
 	return 0;// �ް�������
}



void init_UART()
{
	PCON &= 0x7F;		//�����ʲ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
  SCON = 0x50;   //���ڷ�ʽһ
  TH1 = 0XFD;
  TL1 = 0XFD;
	RI = 0;
  EA = 1;        //�������ж�
  ES = 1;        //���д����ж�
  TR1 = 1;       //��ʱ������
}

/*-----------------����һ���ֽ�-----------------*/
void SendByte(unsigned char dat)
{
	 TI = 0;
	 SBUF = dat;
	 while(!TI);
	 	TI = 0;
}


/*-----------------����һ���ַ���---------------*/
void SendStr(unsigned char *s)
{

	 while(*s!='\0')// \0 ��ʾ�ַ���������־��ͨ������Ƿ��ַ���ĩβ
	 {
	  	SendByte(*s);
	  	s++;
	 }

}

char temp;								
char Rec[34];																	//��������
char data_count = 0;													//���մ���
char temp_length;  														//���ݳ���
bit flag_r = 0;											//��־��	

void UART1_Interrupt() interrupt 4	using 3	//�����жϺ������ճ���
{
//		temp = 0;
		if (RI == 1)
		{
			temp = SBUF;
			RI = 0;
			if(temp!='\n') //�ж��Ƿ���յ�������
			{
					Rec[data_count]=temp;// �񣬾ʹ浽RevBuf����������
					data_count++;
			}
			else if (temp == '\n')
			{
					temp_length=data_count;//�ǣ���¼�����ݳ���
					data_count=0;
				  flag_r = 1;							//�������ձ�־λ

			}
		}
}

/***************������*****************/
void main()
{
	char key = 0, i = 0;
	char j= 0;							
	char str[10]=0;
	
	KEY0 = 1;KEY1 =1;
	
	init_UART();								//��ʼ������
	
	OLED_Init();								//oled��ʼ��
	OLED_Clear();								//����
	OLED_ShowNum(0,0,0,2,12);
	OLED_Clear();

	while(1)
	{
		key = key_scan();					//������������
		
		if (key == 1)							
		{							
			SendStr("testTEST1234\n");		//��������		
		}
		
		else if (key == 2)
		{	
			sprintf(str,"%d \n",j);        //���º󴮿ڷ������ݼ�1
			SendStr(str);
			j++;
		}
	
		if (flag_r == 1)
		{
			OLED_Clear();										//oled����	
			OLED_ShowString(0,0,Rec,12);		//��ʾ��������
			memset(Rec, 0, sizeof (Rec));		//��ս�������
			flag_r = 0;
		}
	}
}

