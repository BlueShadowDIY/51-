#include<intrins.h>  //包含_nop_()函数定义的头文件
#define uint unsigned int
#define uchar unsigned char
#define jpk P1
uchar smgm[]={0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F,0X77,0X7C,0X39,0X5E,0X79,0X71,0X73,0X3E,0X31,0X6E,0X76,0X38,0X00}; //共阴极
uchar jp[]={0x01,0x02,0x04,0x08};
sbit RS=P2^0;    //寄存器选择位，将RS位定义为P2.0引脚
sbit RW=P2^1;    //读写选择位，将RW位定义为P2.1引脚
sbit E=P2^2;     //使能信号位，将E位定义为P2.2引脚
sbit BF=P0^7;    //忙碌标志位，，将BF位定义为P0.7引脚
// WriteInstruction(0x01);//清显示：清屏幕指令
// WriteAddress(0x00);  // 设置显示位置为第一行的第1个字
//	WriteData(dyk(jpsm()));	写入数据
void delay(uint t)//延时毫秒	 
{
	uint y;
	for (t;t>0;t--)
		for (y=120;y>0;y--);
}
void ckcsh(uchar a,uchar b,uchar c,uchar d)//串口通信初始化0x50,0x20,0x00,0xfd
{
	SCON = a;
	TMOD = b;
	PCON = c;
	TH1  = d;
	TL1  = d;
	TI   = 0;
	RI   = 0;
	TR1  = 1;
}
void fsd(uint a)//发送数据端
{
 	SBUF = a;
	while(TI == 0);
	TI = 0;
}
uint jsd()//接收数据端
{
	 	if(RI)
	{
	 	RI = 0;
		return SBUF;
	}
} 
/*****************************************************
函数功能：判断液晶模块的忙碌状态
返回值：result。result=1，忙碌;result=0，不忙
***************************************************/
 unsigned char BusyTest(void)
  {
    bit result;
	RS=0;       //根据规定，RS为低电平，RW为高电平时，可以读状态
    RW=1;
    E=1;        //E=1，才允许读写
    _nop_();   //空操作
    _nop_();
    _nop_(); 
    _nop_();   //空操作四个机器周期，给硬件反应时间	
    result=BF;  //将忙碌标志电平赋给result
	E=0;
    return result;
  }
/*****************************************************
函数功能：将模式设置指令或显示地址写入液晶模块
入口参数：dictate
***************************************************/
void WriteInstruction (unsigned char dictate)
{   
    while(BusyTest()==1); //如果忙就等待
	 RS=0;                  //根据规定，RS和R/W同时为低电平时，可以写入指令
	 RW=0;   
	 E=0;                   //E置低电平(根据表8-6，写指令时，E为高脉冲，
                             // 就是让E从0到1发生正跳变，所以应先置"0"
	 _nop_();
	 _nop_();             //空操作两个机器周期，给硬件反应时间
	 P0=dictate;            //将数据送入P0口，即写入指令或地址
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //空操作四个机器周期，给硬件反应时间
	 E=1;                   //E置高电平
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //空操作四个机器周期，给硬件反应时间
	  E=0;                  //当E由高电平跳变成低电平时，液晶模块开始执行命令
 }
/*****************************************************
函数功能：指定字符显示的实际地址
入口参数：x
***************************************************/
 void WriteAddress(unsigned char x)
 {
     WriteInstruction(x|0x80); //显示位置的确定方法规定为"80H+地址码x"
 }
/*****************************************************
函数功能：将数据(字符的标准ASCII码)写入液晶模块
入口参数：y(为字符常量)
***************************************************/
 void WriteData(unsigned char y)
 {
    while(BusyTest()==1);  
	  RS=1;           //RS为高电平，RW为低电平时，可以写入数据
	  RW=0;
	  E=0;            //E置低电平(根据表8-6，写指令时，E为高脉冲，
                       // 就是让E从0到1发生正跳变，所以应先置"0"
	  P0=y;           //将数据送入P0口，即将数据写入液晶模块
	  _nop_();
	  _nop_();
 	  _nop_();
     _nop_();       //空操作四个机器周期，给硬件反应时间
	  E=1;          //E置高电平
	  _nop_();
	  _nop_();
	  _nop_();
	 _nop_();        //空操作四个机器周期，给硬件反应时间
	 E=0;            //当E由高电平跳变成低电平时，液晶模块开始执行命令
 }
/*****************************************************
函数功能：对LCD的显示模式进行初始化设置
***************************************************/
void LcdInitiate(void)
{
   delay(15);             //延时15ms，首次写指令时应给LCD一段较长的反应时间
   WriteInstruction(0x38);  //显示模式设置：16×2显示，5×7点阵，8位数据接口
	delay(5);   //延时5ms　
	WriteInstruction(0x38);
	delay(5);
	WriteInstruction(0x38);
	delay(5);
	WriteInstruction(0x0F);  //显示模式设置：显示开，有光标，光标闪烁
	delay(5);
	WriteInstruction(0x06);  //显示模式设置：光标右移，字符不移
	delay(5);
	WriteInstruction(0x01);  //清屏幕指令，将以前的显示内容清除
	delay(5);
 }
uchar jpsm()//键盘扫描函数
{
	uint i,j,a=0x00;//
	for(i=0;i<4;i++)
	{
		jpk=~jp[i];
		if(jpk<0xf0) 
		{
			delay(20);				
			a=jpk/0X10;
			for(j=0;j<4;j++)
			if(a==~jp[j]%0x10)
			{ 	
				jpk=~jp[j]%0x10;
				//while(jpk<0xf0);
				return (i+1)*10+j+1;
			}
		}
	}
}