#include "MPC5634M.h"
#include "driver/Gpio/gpio.h"
#include "driver/pll/pll.h"
#include "string.h"
#include "driver/Gpio/port_defs.h"
#include "IntcInterrupts.h"   
#include "app/protocol/uart_protocol.h"

UINT8 data_receive;
static const UINT64 FMCLK =  64*1000*1000;

static UINT16 GetSBRByBaundrate(UINT64 baundrate)
{
	UINT16 sbr = (UINT16)(FMCLK / (16 * baundrate));
	return sbr;
}
void SCIA_ISAR(void);
/************************************************************/
/*                           初始化SCI_A                    */
/************************************************************/
static void initESCI_A(void)
{
	ESCI_A .CR2.R = 0x2000;      // SCI_A 模块使能
//  ESCI_A .CR1.R = 0x01A1000C;
	ESCI_A .CR1.R = 0x01A1002C;  //SCI_A 中断使能 波特率9600
	/* 波特率9600 , 8位数据, 无奇偶校验位,发送接收使能
	 (3~15:110100001=417,buad=64M/(417*16))*/
//    ESCI_A.CR1.R = 0x34000C;// 
	ESCI_A .CR1.R = ((UINT32)(GetSBRByBaundrate((UINT64)115200)<<16)) | 0x002C;
	SIU .PCR[89].R = 0x0400;      // 配置引脚为基本功能: TxDA
	SIU .PCR[90].R = 0x0400;      // 配置引脚为基本功能: RxDA
	INTC .PSR[146].R = 1;                //实时中断优先级
	INTC_InstallINTCInterruptHandler(SCIA_ISAR, 146, 1);  //设置中断号
}

/************************************************************/
/*                     SCI_A 发送一字节                     */
/************************************************************/
 void SCIA_putChar(UINT8 ch)
{
	ESCI_A .DR.B.D = ch;             // 写入需发送的数据
	while (ESCI_A .SR.B.TDRE == 0)
	{
	}    // 等待发送完成
	ESCI_A .SR.R = 0x80000000;         // 清除发送完成标志位

}

/*************************************************************/
/*                        SCI_A接收一字节                    */
/*************************************************************/
static UINT8 SCIA_getChar()
{
	while (ESCI_A .SR.B.RDRF == 0)
	{
	} /* Wait for receive data reg full = 1 */
	ESCI_A .SR.R = 0X20000000; /* Clear RDRF flag */
	return ESCI_A .DR.B.D; /* Read byte of Data*/
}

int scia_send(const char *data, UINT8 datalen)
{
	int ret = 0;
	while (datalen-- && data)
		SCIA_putChar(*data++);
	ret = datalen;
	return ret;
}

int scia_init(void)
{

	initESCI_A();
	return 0;
}

void SCIA_ISAR(void)
{
	UINT8 rec_data = 0;
	if (ESCI_A .SR.B.RDRF != 0)
	{
		ESCI_A .SR.R = 0X20000000; /* Clear RDRF flag */
		rec_data = ESCI_A .DR.B.D; /* Read byte of Data*/
//		scia_send((char *) &rec_data, 1);
		uart_isr_getch(rec_data);
	} /* Wait for receive data reg full = 1 */
}

/************************************************************/
/*                       主函数                             */
/************************************************************/
void test_SCIA(void)
{
	static const char s[] = "hello world!\r\n";
	UINT8 flag = 0;

	Config_PLL();
	initESCI_A();
	gpio_init(IO_CTL_LED1, GPIO_OUTPUT);
	/* Loop forever */
	for (;;)
	{
		scia_send(s, sizeof(s));
		gpio_set(IO_CTL_LED1, flag);
		flag = 1 - flag;

//      data_receive=SCIA_getChar();
//      if(data_receive=='O')
//      {
//      	SCIA_putChar('Y');
//        gpio_set(LED_PORT, 0);
//      }
//      if(data_receive=='C')
//      {
//      	SCIA_putChar('V');
//        gpio_set(LED_PORT, 1);
//      }
	}
}
