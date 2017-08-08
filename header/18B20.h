//依赖于intrins.h,delay.h,typed.h
#define WAR_TL 0xff
#define WAR_TH 0xff
#define DQ P20

bit DS18B20_Init(){
	bit ACK = 1;
	EA = 0; //关中断
	DQ = 0;
	delay(377);
	DQ = 1;
	delay(47);
	ACK = DQ;
	delay(330);
	EA = 1;	//开中断
	return ~ACK;
}

void DS18B20_WR_Bit(bit b){
	DQ = 1;
	delay(1);
	DQ = 0;
	delay(1);
	DQ = b;
	delay(47);
	DQ = 1;
	delay(1);
}


void DS18B20_Write(uchar dat){
	uchar i;
	EA = 0; //关中断
	for(i=0;i<8;i++){
		DS18B20_WR_Bit(0x01&dat);
		dat>>=1;
	}
	EA = 1; //开中断
}


bit DS18B20_RD_Bit(){
	bit ret;
	DQ = 1;
	delay(1);
	DQ = 0;
	delay(1);
	DQ = 1;
	delay(3);
	ret = DQ;
	delay(47);
	return ret;
}


uchar DS18B20_Read(){
	uchar i;
	uchar dat = 0;
	EA = 0; //关中断
	for(i=0;i<8;i++){
		dat>>=1;
		if(DS18B20_RD_Bit())					//读总线
			dat|=0x80;
	}
	EA = 1; //开中断
	return dat;
}

void DS18B20_START(){
	DQ = 1;
	DS18B20_Init();
	DS18B20_Write(0xcc);
	DS18B20_Write(0x44);
}


/*
uint DS18B20_Rtem(){
	uint tem = 0;
	DS18B20_START();
	DS18B20_Init();
	DS18B20_Write(0xcc);
	DS18B20_Write(0xbe);
	tem = DS18B20_Read();
	tem |= (((uint)DS18B20_Read())<<8);
	return tem;
}

void DS18B20_SetAcc(uchar b){
	uchar conf;
	DS18B20_Init();
	DS18B20_Write(0xcc);
	DS18B20_Write(0x4e);
	
	DS18B20_Write(WAR_TL);
	DS18B20_Write(WAR_TH);
	switch(b){
		case 12:
			conf = 0x7f;	//0.0625
			break;
		case 11:
			conf = 0x5f;	//0.125
			break;
		case 10:
			conf = 0x3f;	//0.25
			break;
		case  9:
			conf = 0x1f;	//0.5
			break;
		default:
			conf = 0x7f;	//0.0625
			break;
	}
	DS18B20_Write(conf);
	DS18B20_Write(0x48);
	DS18B20_Write(0xb8);
}

void DS18B20_SaveConf(){
	DS18B20_Init();
	DS18B20_Write(0xcc);
	DS18B20_Write(0x48);
}

*/