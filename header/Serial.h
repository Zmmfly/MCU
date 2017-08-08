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
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = 0xFD;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	TI		= 1;
}


uchar Uart_Send(uchar a){
	ES = 0;
	TI = 0;
	SBUF = a;
	while(TI == 0);	//等待TI置1发送完毕
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