#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef ul
#define ul unsigned long int
#endif

#define ADC_POWER 0x80	//ADC��Դ
#define ADC_FLAG  0x10	//ADCת����ɱ�־
#define ADC_START 0x08	//ADC��������
#define ADC_SPEEDHH 0x60	//90clk
#define ADC_SPEEDH	0x40	//180clk
#define ADC_SPEEDL	0x20	//360clk
#define ADC_SPEEDLL 0x00	//540clk


//��������,�����˲�����
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

//ADC��ʼ��
//����chΪҪ����ΪADCͨ����IO��,16���ƣ�0Ϊ��ͨIO��1ΪADC���
void ADC_Init(uchar ch){
	P1ASF = ch;		//����I/O�ڹ���
	ADC_RES = 0;
	ADC_RESL= 0;
	ADC_CONTR = ADC_POWER|ADC_SPEEDHH;	//�򿪵�Դ�������ٶ�
	us480();
}
//��ȡת������
//ch,ͨ��ѡ��
//w,ADCλ��
uint ADC_GET(uchar ch,uchar w){
	uint tmp;
	ADC_CONTR |= ADC_START|ch|ADC_SPEEDHH|ADC_POWER;
	//ADC_RES = 0;	//���ת�����
	//ADC_RESL= 0;
	us10();
	while(!(ADC_CONTR&ADC_FLAG));	//�ȴ�ADת�����
	ADC_CONTR &= ~ADC_START;	//��ADC_START=0,�ر�ADת��
	if(w >= 10){
		tmp = (ADC_RES<<2)|ADC_RESL;
	}else{
		tmp = ADC_RES;
	}
	//printf("Value:%d\n",tmp);
	return tmp;
}

//��ȡ�˲��������,����wΪ����ADCλ��
//7�β�����ȥ��������С����ʣ�����ƽ����
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
	return result*(w>=10 ? 48829: 1960);//�����˲���Ŵ�һǧ�򱶵�10λADCת�����ݣ����߷Ŵ�ʮ�򱶵�8λ����
}
