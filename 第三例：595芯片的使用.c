#include<reg52.h>
#include<zdy.h>//常用器件的自定义库文件
/*****************************************
zdy.h文件是本人学习51单片机自己整理的库文件
需要的可以到下面网站下载，
https://github.com/BlueShadowDIY/51-/blob/master/zdy.h
******************************************/
/*****************************************
本例中，控制数码管的数据全部由595芯片传输
原本需要采用8个IO口的数据传输简化到3个IO口
利用5
******************************************/

main()
{
	int i=0;
	char a=0x11;
	do{
		out595(~smgm[i]); //595芯片输出数据
		if(i%2) P2=P2&0X7F;//单数显示点，双数无点
		a= _crol_(a,1);//循环位移指令
		P2=P2&0x0f|a;
		delay(1000);
		i++;
	}
	while(smgm[i-1]!='\0');	
}