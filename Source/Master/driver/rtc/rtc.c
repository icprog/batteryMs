/*
 * rtc.c
 *
 *  Created on: Dec 25, 2014
 *      Author: ZHULEIr
 */

#include "driver/rtc/rtc.h"
#include "public/types.h"
#include "driver/timer/timer.h"
#include "MPC5634M.h"
#include "driver/Gpio/gpio.h"
#include "string.h"
 UINT8 g8563_Store[7];    /*时间交换区,全局变量声明*/
UINT8  c8563_Store[4]={0x00,0x59,0x07,0x01};  /*写入时间初值：星期一  07:59:00*/
static void IIC_init()
{
	gpio_init(SCL_PORT,GPIO_OUTPUT);
	gpio_init(SDA_PORT,GPIO_INANDOUT);

}

static void Delay()
{
    // {P10=1;_nop_();P10=0;}
	delay_us(2) ;        /*根据晶振频率制定延时时间*/
}

//内部函数，I2C开始
static void Start()
{   
	gpio_set(SDA_PORT,1);
	gpio_set(SCL_PORT,1);
//	 SDA=1;
//     SCL=1;
     Delay();
     gpio_set(SDA_PORT,0);
     Delay();
     gpio_set(SCL_PORT,0);
}
//*内部函数，I2C结束
static void Stop()
{
	gpio_set(SDA_PORT,0);
    gpio_set(SCL_PORT,0);
    Delay();
    gpio_set(SCL_PORT,1);
    Delay();
    gpio_set(SDA_PORT,1);
    Delay();
  
}
//*内部函数，输出ACK=0
static void WriteACK()
{
	  gpio_set(SDA_PORT,0);
    Delay();
    gpio_set(SCL_PORT,1);
    Delay();
    gpio_set(SCL_PORT,0);
}

//*内部函数，输出ACK=1
static void writenoack()
{
	gpio_set(SDA_PORT,1);
    Delay();
    gpio_set(SCL_PORT,1);
    Delay();
    gpio_set(SCL_PORT,0);
}
//*内部函数，等待ACK
static void WaitACK()
{  
	UINT8 errtime=20;
	gpio_set(SDA_PORT,1);
    Delay();               /*读ACK*/
    gpio_set(SCL_PORT,1);
    Delay();
    while(gpio_read(SDA_PORT))
    {  
    	errtime--;
       if(!errtime) 
       {
    	   Stop();
          break;
       }
    }
    gpio_set(SCL_PORT,0);
    Delay();
}
//**内部函数.输出数据字节  入口:B=数据
static void writebyte(UINT8 wdata)
{
	UINT8 i;
    for(i=0;i<8;i++)
    {
        if(wdata&0x80) 
        gpio_set(SDA_PORT,1);
         else
         gpio_set(SDA_PORT,0);
          wdata<<=1;
          gpio_set(SCL_PORT,1);
          Delay();
          gpio_set(SCL_PORT,0);
    }
    WaitACK();
}
//*内部函数.输入数据  出口:B
static UINT8 Readbyte()
{
	UINT8 i,bytedata;
	gpio_set(SDA_PORT,1);
    for(i=0;i<8;i++)
    {
    	gpio_set(SCL_PORT,1);
        bytedata<<=1;
        bytedata|=gpio_read(SDA_PORT);
        gpio_set(SCL_PORT,0);
        Delay();
    }
    return(bytedata);
}
//**输出数据->pcf8563
void writeData(UINT8 address,UINT8 mdata)
{
    Start();
    writebyte(0xa2);          /*写命令*/
    writebyte(address);          /*写地址*/
    writebyte(mdata);          /*写数据*/
    Stop();
}
//*输入数据<-pcf8563
UINT8 ReadData(UINT8 address)               /*单字节*/
{  
	UINT8 rdata;
    Start();
    writebyte(0xa2);             /*写命令*/
    writebyte(address);          /*写地址*/
    Start();
     writebyte(0xa3);             /*读命令*/
    rdata=Readbyte();
    writenoack();
    Stop();
    return(rdata);
 }
  void ReadData1(UINT8 address,UINT8 count,UINT8 * buff)   /*多字节*/
  {   
	 UINT8 i;
      Start();
      writebyte(0xa2);             /*写命令*/
      writebyte(address);          /*写地址*/
      Start();
      writebyte(0xa3);             /*读命令*/
      for(i=0;i<count;i++)
      {
          buff[i]=Readbyte();
          if(i<count-1) WriteACK();
      }
      writenoack();
      Stop();
  }
  //*内部函数,读入时间到内部缓冲区
  void P8563_Read()
  { 
	  UINT8  time[7];
      ReadData1(0x02,0x07,time);
      g8563_Store[0]=time[0]&0x7f;        /*秒*/
      g8563_Store[1]=time[1]&0x7f;        /*分*/
      g8563_Store[2]=time[2]&0x3f;        /*小时*/
      g8563_Store[3]=time[3]&0x3f;
      g8563_Store[4]=time[4]&0x07;        /*星期*/
      g8563_Store[5]=time[5]&0x9f;
      g8563_Store[6]=time[6]&0xff;
  }
  //*读入时间到内部缓冲区----外部调用 
  void P8563_gettime()
  {
	  P8563_Read();
	  if(g8563_Store[0]==0)
	   P8563_Read();             /*如果为秒=0，为防止时间变化，再读一次*/
 }
	//**写时间修改值
   void P8563_settime()
  {
	   UINT8 i;
	   for(i=2;i<=4;i++) {  writeData(i,g8563_Store[i-2]); }
	    writeData(6,g8563_Store[3]);
  }
   
 void set_date(struct DATE date )
 {
	 UINT8 i;
	 memcpy(g8563_Store,&date,sizeof(date));
	 for(i = 2;i<=8;i++)
	 writeData(i,g8563_Store[i-2]); 
	 
 }
 
 struct DATE get_date(void )
 {
	
	 struct DATE date;
	 P8563_Read();
	 memcpy(&date,g8563_Store,sizeof(g8563_Store));
	 return date;
 }
 
 struct DATE calculate_date(struct DATE b_date,struct DATE a_date)
 {
	 UINT8 tempdate1[7],tempdate2[7],tempdate3[7];
	 UINT8 i;
	 UINT32 second_a,second_b,second1,second2,day1,day2;
	 struct DATE  misdate;
	 memcpy(tempdate1,&b_date,sizeof(b_date));
	 memcpy(tempdate2,&a_date,sizeof(a_date));
	 for(i = 6; i>=0; i--)
	 {
		 bcd_convert_hex(tempdate1[i]);
		 bcd_convert_hex(tempdate2[i]);
		 tempdate3[i]= 0;
	 }
	 second1 = tempdate1[2];
	 second1 = second1*60;
	 second1 = second1*60;
	 second2 = tempdate1[1];
	 second2 = second2*60;
	 second_a = tempdate1[0];
	 second_a = second_a + second1 +second2;
	 second1 = tempdate2[2];
	 second1 = second1*60;
	 second1 = second1*60;
	 second2 = tempdate2[1];
	 second2 = second2*60;
	 second_b = tempdate2[0];
	 second_b = second_b + second1 +second2;
     day1 = tempdate1[6];
     day2 = tempdate2[6];
     if(second_a>second_b)
     {
    	 second_a = second_a - second_b;
    	 second1 = second_a/60;
    	 second2 = second1/60;
    	 second1 = second1%60;
    	 tempdate3[0] = (UINT8)(0x000000ff&(second_a%60));
    	 tempdate3[1] = (UINT8)(0x000000ff&second1);
    	 tempdate3[2] = (UINT8)(0x000000ff&second2);
     }
     else
     {
    	 second_a = second_a + 0x00023820 - second_b;
    	 second1 = second_a/60;
    	 second2 = second1/60;
    	 second1 = second1%60;
    	 tempdate3[0] = (UINT8)(0x000000ff&(second_a%60));
    	 tempdate3[1] = (UINT8)(0x000000ff&second1);
    	 tempdate3[2] = (UINT8)(0x000000ff&second2);
    	 tempdate3[3] =   tempdate3[3]-1;
     }
     if(tempdate2[3]> tempdate1[3] )
     tempdate3[3]= tempdate2[3]> tempdate1[3];  
     else
     {
    	  tempdate3[5] =   tempdate3[5]-1;
    	  tempdate3[3] =   tempdate2[3] + 0x1e - tempdate1[3];
     }
     if(tempdate2[5]> tempdate1[5] )
    	 tempdate3[5]= tempdate2[5]> tempdate1[5]; 
     else
     {
    	  tempdate3[6] =   tempdate3[6]-1;
    	  tempdate3[5] =   tempdate2[5] + 0x0c - tempdate1[5];
     }
     tempdate3[6] = tempdate2[6] - tempdate1[6];
	 memcpy(&misdate,tempdate3,sizeof(tempdate3));
	 return misdate;
 }
 

 //*功能: BCD码转16进制				     						
 //*参数: bcd_data  要转换的BCD码数据(0-100)						
 //*返回: 转换后的16进制数据              						
UINT8  bcd_convert_hex(UINT8 bcd_data)
 	{
	UINT8   temp;
 	  temp=((bcd_data>>4)*10)+(bcd_data&0x0F);
 	  return temp;
 	
 	}
 //*名称：  unsigned char hex_convert_bcd(unsigned char hex_dataa)   	
 //*功能：  16进制转BCD码				     					
 //*参数：  hex_data  要转换的16进制数据(0-100)					
 //*返回：  转换后的BCD码数据              						
UINT8   hex_convert_bcd(UINT8  hex_data)
 	{
	  UINT8   temp;
	  UINT8   bcd_data;
 	  temp=hex_data%100;
 	  bcd_data=(temp/10<<4)|(temp%10);
 	  return bcd_data;
 	}
	  //**P8563的初始化-----外部调用
 void P8563_init()
	{
	  UINT8 i;
	 if((ReadData(0xa)&0x3f)!=0x8)                          /*检查是否第一次启动，是则初始化时间*/
	    {
	        for(i=0;i<=3;i++)  g8563_Store[i]=c8563_Store[i]; /*初始化时间*/
	        P8563_settime();
	        writeData(0x0,0x00);
	        writeData(0xa,0x8);                               /*8:00报警*/
	        writeData(0x1,0x12);                              /*报警有效*/
	        writeData(0xd,0xf0);
	     }
	}


void RTC_test()
{
	struct DATE date1 = {00,24,16,07,3,1,15};
	struct DATE date2 = {0};
	struct DATE date3 = {0};
	while(1){
	UINT8 i;
	IIC_init();
	//set_date(date);
	//P8563_init();
	for(i = 0;i<7;i++)
	g8563_Store[i] = 0;
	 date2 = get_date();
	
}}


