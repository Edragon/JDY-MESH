#include <reg51.h>
#include <intrins.h>
#include <string.h>



#define INT8 	  		char
#define INT16 			int
#define UINT8 	unsigned  	char
#define UINT16 	unsigned 	int


UINT8 ReceivedString[20];
UINT8 ReceivedStringPosition;


//延时函数,us,最多255 us-------------------------------------------------------
void Delay_us(UINT8 n)
{
	for(;n>0;n--)
		_nop_();
}


//延时函数,ms------------------------------------------------------------------
void Delay_ms(UINT16 n)
{
    UINT16 i;
    for(i=0; i<n; i++)
    {
	 	Delay_us(250);
		Delay_us(250);
		Delay_us(250);
		Delay_us(250);
    }
}
			
				

	   
//串口初始化
void UART_Init(void)
{ 
	SCON = 0x40;                	//串口方式1
	PCON = 0;                   	//SMOD=0
	REN = 1;                     	//允许接收
	
	TMOD = 0x20;               		//定时器1定时方式2

	//TH1 = 0xe6;                		//12MHz 1200波特率
	//TL1 = 0xe6;			   	   
	//TH1 = 0xF3;                		//12MHz 2400波特率
	//TL1 = 0xF3;			   	   
	//TH1 = 0xFE;                		//12MHz 19200波特率
	//TL1 = 0xFE;			   	   
	TH1 = 0xFD;                		//12MHz 9600波特率
	TL1 = 0xFD;			   	   
	//TH1 = 0xE8;					//11.0592MHz,1200波特率
	//TL1 = 0xE8;

	TR1 = 1;                  		//启动定时器

	ES = 1;							//UART中断
	EA = 1;							//中断使能

}

//发送一个Char
void UART_Send_Char(INT8 aChar)
{
 	SBUF = aChar;
 	while(TI == 0);//等待，直到发送成功
	TI = 0;
}


void UART_Send_String(INT8 *aString, UINT16 StringLength)
{
	UINT8 i;
	for ( i = 0; i< StringLength; i++ )
	{
		UART_Send_Char( aString[i] );
	}

}


char code Sdata[] = "lxr";
INT8 Receive[1] = "*";
INT16 index =0;
//主函数-----------------------------------------------------------------------

#define STAT_pinn P00

uchar *name = "AT+NANE123\r\n";
uchar *NETID = "AT+NETID123456789ABC\r\n";
uchar all_io_H[7]={0xAA,0xfc,0xff,0xff,0xe7,0xff,0xff};//此指令功能为将所有OUT引脚设置为高电平
uchar all_io_L[7]={0xAA,0xfc,0xff,0xff,0xe7,0xf0,0x00};//此指令功能为将所有OUT引脚设置为低电平
uchar MESH_UART_data[7]={0xAA,0xFB,0xFF,0xFF,0x31,0x32,0x33};//此指认表示所所有模块串口广播123数据，并从串口输出数据
uchar MESH_UART_02_data[7]={0xAA,0xFB,0x00,0x03,0x37,0x38,0x39};//此指认表示只向短地址为0X03的模块发送0X373839数据，并从串口输出



void main(void)//此代码只做思路参考，未实际运行测试过，需要技术支持联系QQ：3411947569
{
	UART_Init();  //初始化UART
	Delay_ms(1000);//500MS
	UART_Send_String( name,strlen(name) );//设置蓝牙广播名为112233
	Delay_ms(500);
	UART_Send_String( NETID,strlen(NETID) );//设置设置组网ID号   一般设置了广播名与组网ID号后，重启模块，模块将会自动组网
	Delay_ms(500);
	UART_Send_String( all_io_H,7 );//此指令功能为将所有OUT引脚设置为高电平
	Delay_ms(500);
	UART_Send_String( all_io_L,7 );//此指令功能为将所有OUT引脚设置为低电平
	Delay_ms(500);
	UART_Send_String( MESH_UART_data,7 );//此指认表示所所有模块串口广播123数据，并从串口输出数据
	Delay_ms(1000);
	UART_Send_String( MESH_UART_02_data7 );//此指认表示只向短地址为0X03的模块发送0X373839数据，并从串口输出
	Delay_ms(1000);
	
	while (1)
	{

		
	}
}

//UART中断服务程序-------------------------------------------------------------
void UART_Receive_Int() interrupt 4
{	
	if( RI == 1)
	{
		RI	= 0;		
		if( index < 1 )
		{
			Receive[index] = SBUF;
			index++;
		}else{
			index = 0;
		}		
	}	
}
