#include<reg52.h>    //����ͷ�ļ�
#include<zdy.h>
#include<string.h>
#define uint unsigned int  //Ԥ����һ��  
#define uchar unsigned char   
uchar code tab[]="welcome to";//
uchar code tab1[]="BlueShadowDIY";//
uchar code dlcg[]="success";//��½�ɹ�
uchar code tcdl[]="quit";//�˳���½
uchar code dlsb[]="fail";//��½ʧ��
uchar code cshwc[]="complete";//��ʼ�����
uchar code tab3[]="modify";//�޸�����
uchar csmm[16]="1234";//��ʼ������
uchar bcmm[16]="1234";//��������
uchar xssz[];//��ʾ����
bit dl=0;//��¼��־λ

void xshs(uchar *p,uchar q)//��ʾ����
{
	int i=0;
	lcd_wcom(q);
	while(p[i]!='\0')
	{
		lcd_wdat(p[i]);
		i++;
	}
}

void dtxs()//��̬����
{
	uchar dt[]="Electronic";
		lcd_wcom(0x01);
		delay(5);
	   	xshs(tab,0x80);//��һ����ʾtab1
		delay(5);
		xshs(dt,0x80+0x40);//�ڶ�����ʾ��½�ɹ�
		delay(5);	
}

void dlcgdh()//��½�ɹ�����
{
		lcd_wcom(0x01);
		delay(5);
	   	xshs(tab1,0x80);//��һ����ʾtab1
		delay(5);
		xshs(dlcg,0x80+0x40);//�ڶ�����ʾ��½�ɹ�
		delay(5);	
}

void dldh()//��½����
{
		lcd_wcom(0x01);
		delay(5);
	   	xshs(tab,0x80);//��һ����ʾtab
		delay(5);
		xshs(tab1,0x80+0x40);//�ڶ�����ʾtab1
		delay(5);	
}

uchar jpxg()//�����޸�����
{
	uchar i=0;
	i=jpsm(0);
	while(!i)
		i=jpsm(0);
	delay(300);
	return i-1;
}

bit qrhs(uchar *p,uchar *q)//�ַ����ȽϺ���
{
	int i=0;
	for(i=0;i<16;i++)
		if(p[i]!=q[i])return 1;	

	return 0;
}

void fzhs(uchar *p,uchar *q) //�ַ�����ֵ����
{
	int i=0;
	while(p[i]!='\0')
	{
		q[i]=p[i];
		i++;   
	}
}

void qkhs(uchar *p,uchar ws)//��պ���
{
	int i=0;
	for(i=0;i<ws;i++)
		p[i]='\0';
}



void dlhs()//��½����
{
	int j=0;
	uchar zc,mmzc[16]="";//���뱣�������ݴ溯��
	lcd_wcom(0x01);
	if(dl) 
	{
		xshs(tcdl,0x80);//��һ����ʾ�˳���½
		dl=0;
	}
	else
	{
		dldh();//��ʾ��¼����
		delay(5000);//��ʾ5������
		lcd_wcom(0x01);//���
	   for(j=0;;j++)//���볤��Ϊ16λ
	   {
	   		lcd_wcom(0x01);
	   		xshs(tab1,0x80);//��һ����ʾ��½����
			xshs(mmzc,0x80+0x40);//�ڶ�����ʾ����
			zc=jpxg();//���뱣��
			if(zc<10)//��¼��ֵ��ť
				mmzc[j]=zc+'0';	
			else if(zc==13)//ȫ���������
			{
				qkhs(mmzc,16);//�������
				j=-1;
			}
			else if(zc==14)//���һλ
			{
				mmzc[j-1]='\0';
				j=j-2;
			}
			else if(zc==15)//ȷ�ϰ�ť
			{
				 if(qrhs(mmzc,bcmm))//���벻ͬ��������¿�ʼ
				 {
				 	qkhs(mmzc,16);//�������
					j=-1;
				 }
				 else 
				 {
				 	dl=1;
					dlcgdh();//��ʾ��½�ɹ�����
					delay(5000);//��ʾ5��
					break;
				 }
			}
			else j--;//��Ч��ť
			xshs(mmzc,0x80+0x40);//�ڶ�����ʾ����

	   }
	} 

}

void xiug()//�����޸ĺ���
{
	int i=0,j=0;
	uchar zc=0;
DL:	if(!dl) dlhs();//δ��½ʱ�����½����
	else
	{
			qkhs(bcmm,16);//�������
	   for(j=0;;j++)//���볤��Ϊ16λ
	   {
	   		lcd_wcom(0x01);
	   		xshs(tab3,0x80);//��һ����ʾ�޸��������
			xshs(bcmm,0x80+0x40);//�ڶ�����ʾ����
			zc=jpxg();
			if(dl==0) goto DL;//�ǵ�½�����ǿ����ת
			if(zc<10)//��¼��ֵ��ť
				bcmm[j]=zc+'0';	
			else if(zc==13)//ȫ���������
			{
				qkhs(bcmm,16);//�������
				j=-1;
			}
			else if(zc==14)//���һλ
			{
				bcmm[j-1]='\0';
				j=j-2;
			}
			else if(zc==15)//ȷ�ϰ�ť
				dl=0;//�޸�����˳���½
			else j--;//��Ч��ť
			xshs(bcmm,0x80+0x40);//�ڶ�����ʾ����

	   }
	}
}


void csh()//��ʼ������
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

void main()            //������  
{   uchar zc,m=0;       
   lcd_init();           //Һ����ʼ��
     dldh();//��������
	 delay(3000);
	 while(1)//
	 {
	 	lcd_wcom(0x01);
		if(dl)
	 		dlcgdh();
		else
			dtxs();
	 	zc=jpxg();//���뱣��
		if(zc==10) csh();//��ʼ����½
		else if(zc==11)	dlhs();//��½����
		else if(zc==12) xiug();//�޸�����
	 }       
} 
