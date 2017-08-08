//需要头文件：intrins.h，stdio.h
#define LCD_DA P2
#define LCD_RS P07
#define LCD_RW P06
#define LCD_EN P05

//写命令或数据
//type: 0,写命令; 1,写数据
//RS=0,RW=0,写命令
//RS=1,RW=0,写数据

void LCD_WCD(bit type,uchar com){
	us10();us10();us10();
	LCD_RW = 0;
	LCD_RS = type ;
	LCD_EN = 0;
	LCD_DA = com;
	us10();
	LCD_EN = 1;
	us10();
	LCD_EN = 0;
}

//读状态或数据
//type：0,读状态; 1,读数据
//RS=0,RW=1,读状态
//RS=1,RW=1,读数据
uchar LCD_RSD(bit type){
	uchar result;
	us10();us10();us10();
	LCD_RW = 1;
	LCD_RS = type;
	_nop_();
	LCD_EN = 1;
	_nop_();
	result = LCD_DA;
	LCD_EN = 0;
	return result;
}

//LCD初始化
//2行，5x8
//整体显示，关光标，不闪烁
//增量不移位
void LCD_INIT(){
	LCD_RW=0;
	LCD_EN=0;
	LCD_WCD(0,0x38);
	us10();us10();
	LCD_WCD(0,0x0c);
	us10();us10();
	LCD_WCD(0,0x01);
	us10();us10();
	LCD_WCD(0,0x06);
	us10();us10();
}

//LCD坐标设置setDisplayPosition
void LCD_SDP(uint x, uint y){
	uchar addr;
	if(y==1){
		addr = 0xc0+x;
	}else{
		addr = 0x80+x;
	}
	LCD_WCD(0,addr);
}
//x:起始位置
//y:0,第一行; 1,第二行
//a:需要显示的字符串数组
void LCD_DIS(int x,int y,uchar *a){
	int i;
	LCD_SDP(x,y);
	for(i=0;i<17;i++){
		if(a[i]!='\0'){
			us10();
			LCD_WCD(1,a[i]);
		}else{
			break;
		}
	}
}
