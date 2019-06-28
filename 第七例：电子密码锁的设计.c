#include<reg52.h>    //包含头文件
#include<zdy.h>
#include<string.h>
#define uint unsigned int  //预定义一下  
#define uchar unsigned char   
uchar code tab[]="welcome to";//
uchar code tab1[]="BlueShadowDIY";//
uchar code dlcg[]="success";//登陆成功
uchar code tcdl[]="quit";//退出登陆
uchar code dlsb[]="fail";//登陆失败
uchar code cshwc[]="complete";//初始化完成
uchar code tab3[]="modify";//修改密码
uchar csmm[16]="1234";//初始化密码
uchar bcmm[16]="1234";//保存密码
uchar xssz[];//显示数组
bit dl=0;//登录标志位

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

void dtxs()//动态函数
{
	uchar dt[]="Electronic";
		lcd_wcom(0x01);
		delay(5);
	   	xshs(tab,0x80);//第一行显示tab1
		delay(5);
		xshs(dt,0x80+0x40);//第二行显示登陆成功
		delay(5);	
}

void dlcgdh()//登陆成功动画
{
		lcd_wcom(0x01);
		delay(5);
	   	xshs(tab1,0x80);//第一行显示tab1
		delay(5);
		xshs(dlcg,0x80+0x40);//第二行显示登陆成功
		delay(5);	
}

void dldh()//登陆动画
{
		lcd_wcom(0x01);
		delay(5);
	   	xshs(tab,0x80);//第一行显示tab
		delay(5);
		xshs(tab1,0x80+0x40);//第二行显示tab1
		delay(5);	
}

uchar jpxg()//键码修复函数
{
	uchar i=0;
	i=jpsm(0);
	while(!i)
		i=jpsm(0);
	delay(300);
	return i-1;
}

bit qrhs(uchar *p,uchar *q)//字符串比较函数
{
	int i=0;
	for(i=0;i<16;i++)
		if(p[i]!=q[i])return 1;	

	return 0;
}

void fzhs(uchar *p,uchar *q) //字符串赋值函数
{
	int i=0;
	while(p[i]!='\0')
	{
		q[i]=p[i];
		i++;   
	}
}

void qkhs(uchar *p,uchar ws)//清空函数
{
	int i=0;
	for(i=0;i<ws;i++)
		p[i]='\0';
}



void dlhs()//登陆函数
{
	int j=0;
	uchar zc,mmzc[16]="";//键码保存密码暂存函数
	lcd_wcom(0x01);
	if(dl) 
	{
		xshs(tcdl,0x80);//第一行显示退出登陆
		dl=0;
	}
	else
	{
		dldh();//显示登录动画
		delay(5000);//显示5秒左右
		lcd_wcom(0x01);//清空
	   for(j=0;;j++)//密码长度为16位
	   {
	   		lcd_wcom(0x01);
	   		xshs(tab1,0x80);//第一行显示登陆界面
			xshs(mmzc,0x80+0x40);//第二行显示密码
			zc=jpxg();//键码保存
			if(zc<10)//记录数值按钮
				mmzc[j]=zc+'0';	
			else if(zc==13)//全部清除功能
			{
				qkhs(mmzc,16);//清空密码
				j=-1;
			}
			else if(zc==14)//清除一位
			{
				mmzc[j-1]='\0';
				j=j-2;
			}
			else if(zc==15)//确认按钮
			{
				 if(qrhs(mmzc,bcmm))//密码不同则清空重新开始
				 {
				 	qkhs(mmzc,16);//清空密码
					j=-1;
				 }
				 else 
				 {
				 	dl=1;
					dlcgdh();//显示登陆成功动画
					delay(5000);//显示5秒
					break;
				 }
			}
			else j--;//无效按钮
			xshs(mmzc,0x80+0x40);//第二行显示密码

	   }
	} 

}

void xiug()//密码修改函数
{
	int i=0,j=0;
	uchar zc=0;
DL:	if(!dl) dlhs();//未登陆时进入登陆函数
	else
	{
			qkhs(bcmm,16);//清空密码
	   for(j=0;;j++)//密码长度为16位
	   {
	   		lcd_wcom(0x01);
	   		xshs(tab3,0x80);//第一行显示修改密码界面
			xshs(bcmm,0x80+0x40);//第二行显示密码
			zc=jpxg();
			if(dl==0) goto DL;//非登陆情况下强行跳转
			if(zc<10)//记录数值按钮
				bcmm[j]=zc+'0';	
			else if(zc==13)//全部清除功能
			{
				qkhs(bcmm,16);//清空密码
				j=-1;
			}
			else if(zc==14)//清除一位
			{
				bcmm[j-1]='\0';
				j=j-2;
			}
			else if(zc==15)//确认按钮
				dl=0;//修改完成退出登陆
			else j--;//无效按钮
			xshs(bcmm,0x80+0x40);//第二行显示密码

	   }
	}
}


void csh()//初始化程序
{
	int i=0;
	if(dl) 
	{
		dl=0;
		qkhs(bcmm,16);
		fzhs(csmm,bcmm);
		lcd_wcom(0x01);
		lcd_wcom(0x80+0x40);
		while(cshwc[i]!='\0')
		{
			lcd_wdat(cshwc[i]);
			i++;
		}
		i=0;
		delay(2000);
		lcd_wcom(0x01);
		lcd_wcom(0x80);
		while(tab[i]!='\0')
		{
			lcd_wdat(tab[i]);
			i++;
		}
		i=0;
		lcd_wcom(0x80+0x40);
		while(tab1[i]!='\0')
		{
			lcd_wdat(tab1[i]);
			i++;
		}
	}
	else dlhs();	
}

void main()            //主函数  
{   uchar zc,m=0;       
   lcd_init();           //液晶初始化
     dldh();//开机动画
	 delay(3000);
	 while(1)//
	 {
	 	lcd_wcom(0x01);
		if(dl)
	 		dlcgdh();
		else
			dtxs();
	 	zc=jpxg();//键码保存
		if(zc==10) csh();//初始化登陆
		else if(zc==11)	dlhs();//登陆函数
		else if(zc==12) xiug();//修改密码
	 }       
} 
