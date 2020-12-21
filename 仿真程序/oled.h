#ifndef __OLED_H
#define __OLED_H

#include "reg52.h"
#include "math.h"	 

sbit SDA = P2^0;
sbit SCL = P2^1;

#define u8 unsigned char
#define u16 unsigned int
	
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC�˿ڶ���----------------  					   

#define OLED_SCLK_Clr() SCL=0 //SDA IIC�ӿڵ�ʱ���ź�
#define OLED_SCLK_Set() SCL=1

#define OLED_SDIN_Clr() SDA=0 //SCL IIC�ӿڵ������ź�
#define OLED_SDIN_Set() SDA=1
	     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�����ú���
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
//void OLED_Display_On(void);
//void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowUnNum(u8 x,u8 y,u16 num,u8 len,u8 size);
void OLED_ShowNum(u8 x, u8 y, int num, u8 len, u8 size);
void OLED_ShowUnFloat(u8 x, u8 y, double num, u8 precisenum, u8 precisefloat, u8 size);
void OLED_ShowFloat(u8 x, u8 y, double num, u8 precisenum, u8 precisefloat, u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_OLED_Byte(unsigned char IIC_Byte);
void OLED_IIC_Wait_Ack(void);
void OLED_Clearline(void);
void OLED_ClearPart(u8 hang,u8 line);
#endif  
	 



