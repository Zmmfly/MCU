#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef ul
#define ul unsigned long int
#endif

#define ADC_POWER 0x80	//ADC电源
#define ADC_FLAG  0x10	//ADC转换完成标志
#define ADC_START 0x08	//ADC启动控制
#define ADC_SPEEDHH 0x60	//90clk
#define ADC_SPEEDH	0x40	//180clk
#define ADC_SPEEDL	0x20	//360clk
#define ADC_SPEEDLL 0x00	//540clk


//交换数据,用于滤波排序
void ADC_Swap(uint *a,uint *b){
	ul temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void ADC_Bubble(uint dat[7],uchar n){
	int low,up,index,i;
	low=0;
	up =n-1;
	index=low;
	
	while(up>low){
		for(i=low;i<up;i++){
			if(dat[i]>dat[i+1]){
				ADC_Swap(&dat[i],&dat[i+1]);
				index=i;
			}
		}
		
		up=index;
		for(i=up;i>low;i--){
			if(dat[i]<dat[i-1]){
				ADC_Swap(&dat[i],&dat[i-1]);
				index=i;
			}
		}
		low=index;
	}
}

//ADC初始化
//参数ch为要设置为ADC通道的IO口,16进制，0为普通IO，1为ADC入口
void ADC_Init(uchar ch){
	P1ASF = ch;		//设置I/O口功能
	ADC_RES = 0;
	ADC_RESL= 0;
	ADC_CONTR = ADC_POWER|ADC_SPEEDHH;	//打开电源并设置速度
	us480();
}
//获取转换数据
//ch,通道选择
//w,ADC位数
uint ADC_GET(uchar ch,uchar w){
	uint tmp;
	ADC_CONTR |= ADC_START|ch|ADC_SPEEDHH|ADC_POWER;
	//ADC_RES = 0;	//清空转换结果
	//ADC_RESL= 0;
	us10();
	while(!(ADC_CONTR&ADC_FLAG));	//等待AD转换完成
	ADC_CONTR &= ~ADC_START;	//置ADC_START=0,关闭AD转换
	if(w >= 10){
		tmp = (ADC_RES<<2)|ADC_RESL;
	}else{
		tmp = ADC_RES;
	}
	//printf("Value:%d\n",tmp);
	return tmp;
}

//获取滤波后的数据,参数w为设置ADC位数
//7次采样，去掉最大和最小，求剩余五个平均数
ul ADC_Filter(uint w){
	int i;
	uint dat[7];
	ul result = 0;
	for(i=0;i<7;i++){
		dat[i] = ADC_GET(0,w);
	}
	ADC_Bubble(dat,7);
	for(i=1;i<6;i++){
		result = result + dat[i];
	}
	//printf("%ld\n",result);
	result = result/5;
	//printf("%ld\n",result);
	return result*(w>=10 ? 48829: 1960);//返回滤波后放大一千万倍的10位ADC转换数据，或者放大十万倍的8位数据
}
