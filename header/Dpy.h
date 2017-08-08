void switch_bit(int b)
{
	switch(b)
	{
		case 0:
			P2=0x0;
			break;
		case 1:
			P2=0x7;
			break;
		case 2:
			P2=0xb;
			break;
		case 3:
			P2=0xd;
			break;
		case 4:
			P2=0xe;
			break;
		case 5:
			P2=0xf;
		default:
			P2=0xf;
			break;
	}
}
//b,数码管段位
//num,数码管显示数字
//xs,是否有小数点，0为无，1为有
//xy,是否消影，0否，1是
void display(int b,uint num,int xs,int xy)
{
	int tb1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
	int tb2[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
	switch_bit(b);
	P0= (xs==1) ? tb2[num] : tb1[num];
	if(xy){
		us480();//消影
		us480();us480();
		switch_bit(5);
	}
	P0=0xff;
}
