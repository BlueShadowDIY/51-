#include<reg52.h>
#include<zdy.h>
#include<ds1302.h>


void xshs(uchar *p,uchar q)//显示函数
{
	int i=0;
	lcd_wcom(q);
	while(p[i]!='\0')
	{
		lcd_wdat(p[i]);
		i++;
	}
}

void ds1302xg(uchar *p,uchar *q)
{
	uchar i,zs[15];
	ds1302zh(zs);
	for(i=0;i<15;i++)
		zs[i]=zs[i]+'0';
	p[6]=zs[0];
	p[7]=zs[1];
	p[8]=zs[2];
	p[9]=zs[3];
	p[11]=zs[4];
	p[12]=zs[5];
	p[14]=zs[6];
	p[15]=zs[7];
	q[1]=zs[8];
	q[2]=zs[9];
	q[4]=zs[10];
	q[5]=zs[11];
	q[7]=zs[12];
	q[8]=zs[13];
	q[10]=zs[14];
}

void main()
{
	uchar cdis1[ ] = {" TIME 0000-00-00"};//2010年6月1号23点59分55秒 星期二
	uchar cdis2[ ] = {" 00:00:00 0"};
	 ds1302_init();  //DS1302初始化
	lcd_init();           //液晶初始化

	while(1)
	{
		ds1302xg(cdis1,cdis2);
		xshs(cdis1,0x80);//第一行显示修改密码界面
		xshs(cdis2,0x80+0x40);//第二行显示修改密码界面
	}		
}