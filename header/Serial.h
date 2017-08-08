/*
void Uart_Init(){
	SCON 	= 0x50;
	REN		= 1;
	TMOD	|= 0x20;
	PCON	|= 0x80;
	//TL1		= 0xfd;
	TH1		= 0xfd;
	//ES		= 1;
	//EA		= 1;
	TR1		= 1;
	TI		= 1;
}
*/

void Uart_Init(){
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
	BRT = 0xFD;		//�趨���������ʷ�������װֵ
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	TI		= 1;
}


uchar Uart_Send(uchar a){
	ES = 0;
	TI = 0;
	SBUF = a;
	while(TI == 0);	//�ȴ�TI��1�������
	TI = 0;
	ES = 1;
	return SBUF;
}


uchar Uart_Recv(){
	while(!RI);
	if(RI){
		RI=0;
		return SBUF;
	}
	return 0;
}