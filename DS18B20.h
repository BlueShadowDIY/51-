sbit DQ=P2^7; //���ݴ����߽ӵ�Ƭ������Ӧ������  
bit  fg=1;        //�¶�������־
//*****************************��ʼ������ *********************************//
 void delay13(unsigned char i)
{
 for(i;i>0;i--);
}
void Init_DS18B20(void) 
{
	unsigned char x=0;
	DQ=1; //DQ���ø� 
	delay13(8); //����ʱ
	DQ=0; //���͸�λ���� 
	delay13(80); //��ʱ��>480us) 
	DQ=1; //���������� 
	delay13(5); //�ȴ���15~60us) 
	x=DQ; //��X��ֵ���жϳ�ʼ����û�гɹ���18B20���ڵĻ�X=0������X=1 
	delay13(20); 
} 


//**********��һ���ֽ�************//

ReadOneChar(void)  //�����������ȴӸ������͵�ƽ1us���ϣ���ʹ��������Ϊ�ߵ�ƽ���Ӷ��������ź�
{
	unsigned char i=0; //ÿ����������̵ĳ���ʱ��Ϊ60us������������֮�������1us���ϵĸߵ�ƽ�ָ���
	unsigned char dat=0; 
	for (i=8;i>0;i--) //һ���ֽ���8λ 
	{
		DQ=1; 
		delay13(1); 
		DQ=0;
		dat>>=1; 
		DQ=1; 
		if(DQ) 
			dat|=0x80; 
		delay13(4);
	} 
	return(dat);
} 



//*********************** **дһ���ֽ�**************************//

void WriteOneChar(unsigned char dat) 
{ 
  unsigned char i=0; //�����ߴӸߵ�ƽ�����͵�ƽ������д��ʼ�źš�15us֮�ڽ�����д��λ�͵��������ϣ�
  for(i=8;i>0;i--) //��15~60us֮��������߽��в���������Ǹߵ�ƽ��д1����д0������ 
  {
   DQ=0; //�ڿ�ʼ��һ��д����ǰ������1us���ϵĸߵ�ƽ�ָ��ڡ� 
   DQ=dat&0x01; 
   delay13(5); 
   DQ=1; 
   dat>>=1;
  } 
  delay13(4);
} 


//���¶�ֵ
int ReadTemperature(void) 
{
unsigned char udat=0x00,TH,TL;	 
 Init_DS18B20(); //��ʼ��
 WriteOneChar(0xcc); //���������кŵĲ���
 WriteOneChar(0x44); //�����¶�ת��
 delay(125); //ת����Ҫһ��ʱ�䣬��ʱ 
 Init_DS18B20(); //��ʼ��
 WriteOneChar(0xcc); //���������кŵĲ��� 
 WriteOneChar(0xbe); //���¶ȼĴ�����ͷ����ֵ�ֱ�Ϊ�¶ȵĵ�λ�͸�λ�� 
 TL=ReadOneChar(); //�����¶ȵĵ�λLSB
 TH=ReadOneChar(); //�����¶ȵĸ�λMSB 
		if(TH>0x7f)      //���λΪ1ʱ�¶��Ǹ�
		{
		 TL=~TL;         //����ת����ȡ����һ
		 TH=~TH+1;       
		 fg=0;      //��ȡ�¶�Ϊ��ʱfg=0
       }
	   else fg=1;
	udat=TL>>4|TH<<4;
	if(!fg) udat=udat-15;
	return udat;
}
