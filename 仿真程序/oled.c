#include "oled.h"
#include "oledfont.h"  	 

//初始化SSD1306					    
void OLED_Init(void)
{
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//	
//	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}
/**********************************************
//IIC Start
**********************************************/
void OLED_IIC_Start(void)
{
	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}
/**********************************************
//IIC Stop
**********************************************/
void OLED_IIC_Stop(void)
{
	OLED_SCLK_Set();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();	
}
void OLED_IIC_Wait_Ack(void)
{
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_OLED_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
			OLED_SDIN_Set();
		else 
			OLED_SDIN_Clr();		
		da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	OLED_IIC_Start();
	Write_IIC_OLED_Byte(0x78);            //Slave address,SA0=0
	OLED_IIC_Wait_Ack();	
	Write_IIC_OLED_Byte(0x00);			//write command
	OLED_IIC_Wait_Ack();	
	Write_IIC_OLED_Byte(IIC_Command); 
	OLED_IIC_Wait_Ack();	
	OLED_IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	OLED_IIC_Start();
	Write_IIC_OLED_Byte(0x78);			//D/C#=0; R/W#=0
	OLED_IIC_Wait_Ack();	
	Write_IIC_OLED_Byte(0x40);			//write data
	OLED_IIC_Wait_Ack();	
	Write_IIC_OLED_Byte(IIC_Data);
	OLED_IIC_Wait_Ack();	
	OLED_IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);  
	}
	else
	{
		Write_IIC_Command(dat);		
	}
}

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
////开启OLED显示    
//void OLED_Display_On(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
//	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
//	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
//}
////关闭OLED显示     
//void OLED_Display_Off(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
//	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
//	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
//}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

void OLED_Clearline(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<26;n++)OLED_WR_Byte(0,OLED_DATA); 
	}
}

void OLED_ClearPart(u8 hang,u8 line)
{
	u8 i,n;		    
	for(i=hang;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=line;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	}
}

void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else 
		{	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);				
		}
}

//m^n函数
u16 OLED_pow(u8 m,u8 n)
{
	u16 result=1;	 
	while(n--)result*=m;    
	return result;
}

//函数功能：显示无符号num数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowUnNum(u8 x,u8 y,u16 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,'0',size);
				continue;
			}else enshow=1; 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size); 
	}
}

/*
函数功能：显示数字
参数：xy坐标，num显示数字，len是数字长度，size数字大小模式
返回值：无
*/
void OLED_ShowNum(u8 x, u8 y, int num, u8 len, u8 size)
{
	if(num>=0)
	{
		OLED_ShowChar(x,y,' ',size);
		OLED_ShowUnNum(x+(size/2),y,num,len,size);
		return;
	}
	
	OLED_ShowChar(x,y,'-',size);
	OLED_ShowUnNum(x+(size/2),y,-num,len,size);
}

/*
函数功能：显示无符号float数字
参数：xy坐标，num显示数字，precise显示数字的精度，size数字大小模式
返回值：无
*/
void OLED_ShowUnFloat(u8 x, u8 y, double num, u8 precisenum, u8 precisefloat, u8 size)
{
	u8 i = 0;

	u16 integer;
	double decimal;
	
	integer = (int)num;//整数部分
	decimal = (double)(num - integer);//小数部分
	
	OLED_ShowNum(x,y,integer,precisenum,size);//显示整数部分
	OLED_ShowChar(x+(size/2)*(precisenum+1),y,'.',size);//显示小数点
	
	x = x+(size/2)*(precisenum+2);
	while (precisefloat)//显示几位小数
	{
		decimal = decimal * 10;
		integer = (int)decimal;//取出一位小数
		decimal = (double)(decimal - integer);
		OLED_ShowChar(x+(size/2)*i, y, integer + '0', size);//循环显示每位数字字符，从高位显示
		i++;
		precisefloat--;
	}
}

/*显示float型数据,含负数*/
void OLED_ShowFloat(u8 x, u8 y, double num, u8 precisenum, u8 precisefloat, u8 size)
{
	if (num < 0)
	{
		OLED_ShowChar(x, y, '-', size);
		num = -num;
		OLED_ShowUnFloat(x+(size/2),y,num,precisenum,precisefloat,size);
		return;
	}
	OLED_ShowChar(x, y, ' ', size);
	OLED_ShowUnFloat(x+(size/2),y,num,precisenum,precisefloat,size);
}

//显示一个字符号串
//void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
//{
//	unsigned char j=0;
//	while (chr[j]!='\0')
//	{		OLED_ShowChar(x,y,chr[j],Char_Size);
//			x+=8;
//			if(x>120){x=0;y+=2;}
//			j++;
//	}
//}
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
