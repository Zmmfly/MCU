#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif


//功能开关,注释关闭
#define DELAY					//n=(us-1.26)/1.27
#define DELAY_US
//#define DELAY_MS
//#define DELAY_US10   //us10()依赖于delayus()
//#define DELAY_US480

//汇编延时,使用时需
//#define ASM_US60
//#define ASM_US480



#ifdef DELAY
void delay(uint us){
	while(--us);
}
#endif

#ifdef DELAY_US
void delayus(){
	_nop_();
	_nop_();
}
#endif

#ifdef DELAY_MS
void delayms(int ms){
	int i,j,k;
	while((ms--)!=0){
		for(i=0;i<79;i++)
		for(j=0;j<7;j++);
		for(k=0;k<75;k++);
		_nop_();
		_nop_();
	}
}
#endif

#ifdef DELAY_US10
void us10(){
	delayus();delayus();delayus();
	delayus();delayus();delayus();
	delayus();delayus();delayus();
	_nop_();_nop_();
}
#endif


#ifdef DELAY_US480
void us480(void)   //误差 -0.037615740741us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=46;b>0;b--)
            for(a=10;a>0;a--);
}
#endif

#ifdef ASM_US60
void asm_us60(){
	#pragma asm
		MOV R7,#163
		D1:
			DJNZ R7,D1
	#pragma endasm
}
#endif

#ifdef ASM_US480
void asm_us480(){
	#pragma asm
		MOV R7,#242	//DJNZ1
	D1480:
		MOV R6,#4	//DJNZ2
		DJNZ R6,$
	DJNZ R7,D1480
	#pragma endasm
}
#endif





/*
原型一：
void us(){
	#pragma asm
		MOV R7,#x
		D1:
			DJNZ R7,D1
	#pragma endasm
}
延时范围1.45us-92.89us
延时计算公式:
Delay(us)=0.55 + 0.18 + 0.36*x + 0.36
x = (us-0.55-0.18-0.36)  /  0.36
x = (us-1.09)  /  0.36     

原型二：
void delay(){
	#pragma asm
	MOV R7,#m	//DJNZ1
	D1:
		MOV R6,#n	//DJNZ2
		DJNZ R6,$
	D2:DJNZ R7,D1
	#pragma endasm
}
延时范围1.99us - 23547.79us
延时计算公式：
Delay(us)=0.55 + 0.18 + (0.18 + n*0.36) * m + 0.36 * m + 0.36
us = (0.54 + n*0.36) * m + 1.09
us = 0.54 * m + m * n * 0.36 +1.09
已知us,m,求n, n = (us - 1.09 - 0.54 * m )/m*0.36
已知us,n,求m, m = (us - 1.09)/(0.54 + n*0.36) 

原型三：
void delay(int d){
	while(--d);
}
延时范围2.53us-83230.71us
延时计算公式：
Delay(us)=2.53+(n-1)*1.27
us=2.53+1.27n-1.27
us=1.26+1.27n
已知us,求n, n=(us-1.26)/1.27

*/