#include<reg52.h>
#include<zdy.h>//常用器件的自定义库文件
/*****************************************
zdy.h文件是本人学习51单片机自己整理的库文件
需要的可以到下面网站下载，
https://github.com/BlueShadowDIY/51-/blob/master/zdy.h
******************************************/
main()
{
	int i=0;
	while(smgm[i]!='\0')
	{
		P2=smgm[i]; //共阴极数码管码
		P0=~smgm[i];//共阳极数码管码
		delay(1000);
		i++;
	}
}

