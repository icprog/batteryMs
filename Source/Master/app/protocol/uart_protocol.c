#include "protocol/uart_protocol.h"
#include "stdio.h"
#include "mgclib/mgcfifo.h"
#include <string.h>
#include "voltage/voltage.h"
#include "temp/temperature.h"
#include "protocol/vcu_protocol.h"
#include "public/public.h"
#include "mgclib/mgcfifo.h"
#include "ctl/charge.h"
#include "public/ParameterManager.h"
#include "public/public.h"
#include "public/drivers.h"
#include "middleLayer/simulateDrivers.h"

/***************************************************************
 * const values
 ****************************************************************/
static const UINT8 sonframehead = '<';
static const UINT8 sonframeend = '>';
static const UINT8 sonframelensize = 2;
static const UINT16 maxsizeofsonframevaliddata = 250;
static const UINT8 g_srcAddr = CAN_ADDR_BMS;
static const UINT8 g_dstAddr = CAN_ADDR_VCU;

#pragma pack(push)
#pragma pack(1)
struct bm06_protocol
{
	UINT8 PF;
	UINT8 PS;
	UINT8 SA;
	UINT8 DATA[8];
};

struct bm06_sonFrame
{
	UINT8 head;
	UINT8 len;
	UINT8 data[200];
};

struct bm06_cmd_ioctl
{
	union
	{
		UINT8 BYTES;
		struct
		{
			UINT8 reversed :6;
			UINT8 dischargeCmd :1;
			UINT8 chargeCmd :1;
		} BITS;
	} chargeControl;
	union
	{
		UINT8 BYTES;
		struct
		{
			UINT8 fan2 :1;
			UINT8 fan1 :1;
			UINT8 reversed :6;
		} BITS;
	} fansControl;
	UINT8 heatControl;
	union
	{
		UINT8 BYTES;
		struct
		{
			UINT8 A7 :1;
			UINT8 A6 :1;
			UINT8 A5 :1;
			UINT8 A4 :1;
			UINT8 A3 :1;
			UINT8 A2 :1;
			UINT8 A1 :1;
			UINT8 A0 :1;
		} BITS;
	} CMDControl;
	union
	{
		UINT8 BYTES;
		struct
		{
				UINT8 AF :1;
				UINT8 AE :1;
				UINT8 AD :1;
				UINT8 AC :1;
				UINT8 AB :1;
				UINT8 AA :1;
				UINT8 A9 :1;
				UINT8 A8 :1;
		} BITS;
	} CMDControl1;
	union
	{
		UINT8 BYTES;
		struct
		{
			UINT8 B7 :1;
			UINT8 B6 :1;
			UINT8 B5 :1;
			UINT8 B4 :1;
			UINT8 B3 :1;
			UINT8 B2 :1;
			UINT8 B1 :1;
			UINT8 B0 :1;
		} BITS;
	} CMDControl2;
	union
	{
		UINT8 BYTES;
		struct
		{
			UINT8 BF :1;
			UINT8 BE :1;
			UINT8 BD :1;
			UINT8 BC :1;
			UINT8 BB :1;
			UINT8 BA :1;
			UINT8 B9 :1;
			UINT8 B8 :1;
		} BITS;
	} CMDControl3;

	UINT8 reversed[1];
};

struct sonframeData
{
	UINT16 len;
	UINT8 VaidValue[maxsizeofsonframevaliddata];
	UINT16 dataCount;
	UINT8 sonframe_status;
};
#pragma pack(pop)

enum sonframe_state
{
	sonframe_state_head,
	sonframe_state_len,
	sonframe_state_data,
	sonframe_state_end
};

enum rxsonframe_process
{
	sonframe_rec_start, sonframe_rec_ok
};

enum bm06_protocol_id
{
	UART_ID_CONTROL = 0x07,
	UART_ID_GET_CELL_VOL = 0x15,
	UART_ID_GET_CELL_TEMP = 0x16,
	UART_ID_GET_BMS_STATUS = 0x21,
	UART_ID_GET_BATTERY_STATUS1 = 0x22,
	UART_ID_GET_BATTERY_STATUS2 = 0x23,
	UART_ID_GET_BATTERY_WARNING_INFO = 0x24,
	UART_ID_GET_BMS_HARDWARE_INFO = 0xA0,
	UART_ID_SET_BMS_PIN = 0xA1,
	UART_ID_SET_VOL_TH_VALUE,
	UART_ID_GET_VOL_TH_VALUE ,
	UART_ID_SET_TEMP_TH_VALUE,
	UART_ID_GET_TEMP_TH_VALUE,
	UART_ID_SET_CUR_TH_VALUE ,
	UART_ID_GET_CUR_TH_VALUE = 0xA7,
	UART_ID_SET_DEV_INFO,
	UART_ID_GET_DEV_INFO,
	UART_ID_SET_SIMULATE_VOL = 0xB0,
	UART_ID_SET_SIMULATE_TEMP = 0xB1,
	UART_ID_SET_SIMULATE_AD = 0xB2,
	UART_ID_SET_SIMULATE_PIN = 0xB3
};
/*
 **************************************************************
 * private value 
 * **************************************************************/
static UINT8 g_protocolinit = 0;
volatile UCHAR g_rx_frame_size = 0;
UCHAR g_rx_cmd_buff[MAX_CMD_PARAM_SIZE];
char fifo_buff[MGCFIFO_BUFF_CALCULATE(FIFO_ELEMENTS_SIZE, FIFO_SIZE)];
UCHAR *presult = NULL;
UCHAR *prxbuff = g_rx_cmd_buff;
static UINT8 sonFrameBuff[500];
struct mgcfifo * p_uart_rx_fifo;
static struct bm06_cmd_ioctl gCmdControl;
static UINT8 sendbuff[50] =
{
	0 };
static UINT8 g_cmd = 0;
static struct sonframeData gSonFrameData;
static struct uart_cmd_pkg g_msg =
{
	0,
	0,
	g_rx_cmd_buff };

/***************************************************************
 * function declares
 * **************************************************************/
static int BMSHardwareInfoReport(void);
static int BMSQueryVolThValueReport(void);
static int BMSQueryTempThValueReport(void);
static int BMSQueryCurThValueReport(void);
static int BMSQueryDevInfoThValueReport(void);
/***************************************************************
 * bodys
 * ***************************************************************/
/*
 static int
 protocol_frame_size(void)
 {
 return g_rx_frame_size;
 }
 */
static UINT8 nRxFrameSize(void)
{
	return g_rx_frame_size;
}
static void nRxFrameUseOne(void)
{
	if (g_rx_frame_size)
		g_rx_frame_size--;
}
static void nRxFrameInc(void)
{
	g_rx_frame_size++;	
}

static void protocol_frame_clear(void)
{
	g_rx_frame_size = 0;
}

int uart_protocol_init(void)
{
	int ret = 0;
	memset(fifo_buff, 0x55, sizeof(fifo_buff));
	memset(g_rx_cmd_buff, 0x55, sizeof(g_rx_cmd_buff));
	ret = mgcfifo_alloc(&p_uart_rx_fifo, fifo_buff, sizeof(fifo_buff),
			FIFO_ELEMENTS_SIZE);
	if (ret <= 0)
		return -1;
	protocol_frame_clear();
	g_protocolinit = 1;
	return ret;
}

static int BYTES_ORDER_B2S_16(UINT8 *dst, UINT16 data)
{
	dst[0] = (UINT8) data & 0xff;
	dst[1] = (UINT8) (data / 256);
	return 0;
}
static int BYTES_ORDER_S2B_16(UINT8 *dst)
{
	UINT16 data = 0;
	data = (UINT16) (dst[0] + dst[1] * 256);
	return data;
}
static UINT32 BYTES_ORDER_S2H_32(UINT8 *dst)
{
	UINT32 data = 0;
	memset(dst, 0, 4);
	data = (UINT32) (dst[0] | dst[1] << 8 | dst[1] << 16 | dst[1] << 24);
	return data;
}
static int BYTES_ORDER_H2S_32(UINT32 data, UINT8 *dst)
{
	assert(dst!=0);
	dst[0] = (UINT8) (data & 0x0ff);
	dst[1] = (UINT8) (data >> 8);
	dst[2] = (UINT8) (data >> 16);
	dst[3] = (UINT8) (data >> 24);
	return 0;
}

static UINT16 reverseBytsOrder16(UINT16 data)
{
	UINT16 ret = 0;
	ret = ((data >> 8) & 0x0ff);
	ret |= ((data << 8) & 0xff00);
	return ret;
}

/*
 * len is 2,4,8,16...
 * */
static void reverseNBytsOrder(UINT8 *data, UINT8 len)
{
	UINT8 t = 0, i =0;
	assert(len%2==0);
	assert(data);
	for(i=0;i<len/2;i++)
	{
		t=data[i];
		data[i]=data[len - 1 - i];
		data[len - 1 - i] = t;
	}
}

static UINT32 reverseBytsOrder32(UINT32 data)
{
	UINT32 ret = 0;
	ret = (data & 0x00ff) << 24;
	ret |= (((data >> 8) & 0x0ff) << 16);
	ret |= (((data >> 16) & 0x0ff) << 8);
	ret |= (((data >> 24) & 0x0ff));
	return ret;
}
static UINT32 ReverseBytsOrderFloat(UINT8 *data)
{
	UINT32 ret = 0;
	UINT8 d = 0;
	d = data[0];
	data[0] = data[3];
	data[3] = d;
	
	d = data[1];
		data[1] = data[2];
		data[2] = d;

	return ret;
}
/*********************************************************
 * 协锟介发锟酵猴拷锟斤拷
 *********************************************************/
void protocol_tx(UCHAR cmdwords, const UCHAR *pcmd, UINT16 cmd_len)
{
	UCHAR len[2];
	BYTESORDER_H2S_16(cmd_len, len);
	//PROTOCOL_PUTS(head, sizeof(head));
	(void) PROTOCOL_PUTS ( "tx:" )
	;
	//PROTOCOL_PUTS(head);
	//(void)PROTOCOL_PUTC ( head[0] );
	//(void)PROTOCOL_PUTC ( head[1] );
	(void) PROTOCOL_PUTARRAY(head,sizeof(head));
	(void) PROTOCOL_PUTARRAY ( &cmdwords, 1 );
	//(void)PROTOCOL_PUTHEX ( cmdwords );
	(void) PROTOCOL_PUTARRAY ( len, sizeof ( len ) );

	if (pcmd)
		(void) PROTOCOL_PUTARRAY ( pcmd, cmd_len );

	(void) PROTOCOL_PUTARRAY(tail, sizeof(tail));
	//(void)PROTOCOL_PUTC ( tail[0] );
	//(void)PROTOCOL_PUTC ( tail[1] );

	(void) PROTOCOL_PUTS ( "    \r\n" )
	;
}

/*********************************************************
 * 协锟斤拷锟斤拷眨锟斤拷卸锟斤拷械锟斤拷锟? *
 *********************************************************/
void uart_isr_getch(UCHAR ch)
{
	if (g_protocolinit != 1)
		return;
	(void) mgcfifo_in(p_uart_rx_fifo, (char *) &ch);
}

static int uart_isr_fifo_size(void)
{
	return mgcfifo_nvalid(p_uart_rx_fifo);
}

void protocol_rx(UCHAR ch)
{
	enum _searia_rx_frm_state
	{
		state_head = 1, state_cmd, state_len, state_cmdparam, state_tail
	};
	//UINT16  i = 0;
	//static UCHAR err_count = 0;
	static enum _searia_rx_frm_state rx_state = state_head;
	static UINT16 count = 0;

	switch (rx_state)
	{
		/*前锟斤拷锟斤拷*/
		case state_head:
		{
			if (count >= sizeof(head))
			{
				count = 0;
			}

			if (ch == head[count])
			{
				if (count == sizeof(head) - 1)
				{
					count = 0;
					g_msg.len = 8 + 3;  // for bm06 protocol
					rx_state = state_cmdparam;
					break;
				}

				count++;
			}
			else
			{
				count = 0;
			}

			break;
		}
			/*锟斤拷锟斤拷锟斤拷*/
		case state_cmd:
		{
			g_msg.cmd = ch;
			rx_state = state_len;
			break;
		}
			/*锟斤拷锟斤拷*/
		case state_len:
		{

			if (count >= 2)
			{
				count = 0;
			}

			/*锟竭帮拷位*/
			if (count == 0)
			{
				g_msg.len = ch;
				count++;
			}
			/*锟酵帮拷位*/
			else
			{
				g_msg.len = (UINT16) (g_msg.len * 256 + ch);
				//* ( ( UCHAR * ) &g_msg.len + 1 ) = ( ch );
				count = 0;

				if (g_msg.len == 0)
					rx_state = state_tail;

				else if (g_msg.len >= MAX_CMD_PARAM_SIZE)
					rx_state = state_head;
				else
					rx_state = state_cmdparam;
			}

			break;
		}
		case state_cmdparam:
		{
			if (count >= sizeof(g_rx_cmd_buff))
			{
				count = 0;
			}

			if (count < g_msg.len - 1) //count 锟斤拷0锟斤拷始 锟斤拷锟揭伙拷锟斤拷纸锟斤拷锟絜lse锟斤拷锟斤拷
			{
				g_msg.param[count] = ch;
				count++;
			}
			else
			{
				g_msg.param[count] = ch;
				rx_state = state_tail;
				count = 0;
			}

			break;
		}
		case state_tail:
		{
//			if (count >= tail_size)
//			{
//				count = 0;
//			}
			if (ch == tail[count])
			{
				count++;

				if (count == tail_size)
				{
					count = 0;

					//    (void)mprintf( "rx:{ < %02x %02x %02x ", g_msg.cmd & 0x0ff, g_msg.len / 0xff & 0x0ff, g_msg.len & 0x0ff );
					//    (void)printf_array_hex ( (char *)g_msg.param, g_msg.len );
					//    (void)mprintf ( "> } \r\n" );
//					memcpy( &sonFrameBuff[g_rx_frame_size], g_msg.param, 8);
					g_rx_frame_size++;
					rx_state = state_head;
					break;
				}
			}
			else
			{
				count = 0;
				rx_state = state_head;
			}

			break;
		}
		default:
		{
			count = 0;
			rx_state = state_head;
			//(void)DEBUG_UART_PRINTF( "err:serial rec unkown state!\n" );
		}
			break;
	}
}

/*********************************************************
 * 锟斤拷锟节达拷锟斤拷锟斤拷锟斤拷
 * 锟斤拷锟斤拷0 锟斤拷示未锟斤拷锟斤拷
 * 锟斤拷锟斤拷<0 锟斤拷示锟斤拷锟斤拷
 * 锟斤拷锟斤拷>0锟斤拷示锟缴癸拷
 *********************************************************/
//UCHAR tbuff[MAX_CMD_PARAM_SIZE];
static const UINT8 recCmdHaveSonFrameId[] =
{
	UART_ID_SET_VOL_TH_VALUE,
	UART_ID_SET_TEMP_TH_VALUE,
	UART_ID_SET_CUR_TH_VALUE,
	UART_ID_SET_DEV_INFO,
	UART_ID_SET_SIMULATE_VOL ,
	UART_ID_SET_SIMULATE_TEMP ,
	UART_ID_SET_SIMULATE_AD ,
	UART_ID_SET_SIMULATE_PIN 
};
/*
 * if return 1 is a sonFrame
 * else none
 * */
static int isASonFrame(UINT8 id)
{
	UINT8 i = 0;
	for (i = 0; i < sizeof(recCmdHaveSonFrameId); i++)
	{
		if (id == recCmdHaveSonFrameId[i])
			return 1;
	}
	return 0;
}

static int getsonframeData(UINT8 data)
{
	static UINT8 state = sonframe_state_head;
	static UINT8 tempbuff[2];
	static UINT8 dataCount = 0;
	switch (state)
	{
		case sonframe_state_head:
			if (data == sonframehead)
			{
				state = sonframe_state_len;
				memset(tempbuff, 0, sizeof(tempbuff));
				memset(gSonFrameData.VaidValue, 0,
						sizeof(gSonFrameData.VaidValue));
				gSonFrameData.dataCount = 0;
				dataCount = 0;
			}
			break;

		case sonframe_state_len:
			if (dataCount < sonframelensize - 1)
			{
				tempbuff[dataCount] = data;
				dataCount++;
				break;
			}

			tempbuff[dataCount] = data;
			gSonFrameData.len = (UINT16) BYTES_ORDER_S2B_16(tempbuff);

			if (gSonFrameData.len > sizeof(gSonFrameData.VaidValue))
			{
				goto sonframe_state_err;
			}

			state = sonframe_state_data;
			break;

		case sonframe_state_data:
			if (gSonFrameData.dataCount < gSonFrameData.len - 1)
			{
				gSonFrameData.VaidValue[gSonFrameData.dataCount] = data;
				gSonFrameData.dataCount++;
				break;
			}
			gSonFrameData.VaidValue[gSonFrameData.dataCount] = data;
			state = sonframe_state_end;
			break;

		case sonframe_state_end:
			if (data == sonframeend)
			{
				state = sonframe_state_head;
				gSonFrameData.sonframe_status = sonframe_rec_ok;
//				mprintf("get a son frame!\r\n");
			}
			else
			{
				goto sonframe_state_err;
			}
			break;

		default:
			break;
	}
	return 0;

	sonframe_state_err:
	//TODO err process
	state = sonframe_state_head;
	return -1;
}

static int ReverseBytesOrderVoltage(struct voltage_const_vars *vol)
{
	int i = 0;
	UINT32 *data = (UINT32 *) vol;
	for (i = 0; i < sizeof(struct voltage_const_vars);i+=4)
	{
		*data = reverseBytsOrder32(*data);
		data++;
	}
	//TODO
	return 0;
}

static int ReverseBytesOrderTemperature(struct temperature_const_vars *temp)
{
	int i = 0;
	UINT16 *data = (UINT16 *) temp;
	for (i = 0; i < sizeof(struct temperature_const_vars); i+=2)
	{
		*data = reverseBytsOrder16(*data);
		data++;
	}
	//TODO
	return 0;
}

static int ReverseBytesOrderSleepTime(struct time_sleep *time)
{
	UINT8 i = 0;
	UINT32 *data = (UINT32*) time;
	for (i = 0; i < sizeof(struct time_sleep); i++)
	{
		*data = reverseBytsOrder32(*data);
		i += 4;
		data++;
	}
	return 0;
}

static int ReversBytsOrderCurrent(struct CCurrentsThresholdConstValue *cur)
{
	UINT8 i = 0;
	for (i = 0; i < TOTAL_CURRENTS; i++)
	{
		cur->mCurrentThreshold[i].mThreatholdOCLEVEL1 = (TYPE_CURRENT)reverseBytsOrder16(
				(UINT16)cur->mCurrentThreshold[i].mThreatholdOCLEVEL1);
		cur->mCurrentThreshold[i].mThreatholdOCLEVEL2 = (TYPE_CURRENT)reverseBytsOrder16(
				(UINT16)cur->mCurrentThreshold[i].mThreatholdOCLEVEL2);
	}
	return 0;
}

static int ReverseBytesOrderDevinfo(struct dev_info *dev)
{
	int i = 0;

	dev->COMMBYTE = reverseBytsOrder16(dev->COMMBYTE); //通信版本     V1.0					
	dev->BARTYPE = reverseBytsOrder16(dev->BARTYPE); //电池类型   NCM					
	dev->CHARGE_RATECURRENT = (TYPE_CURRENT) reverseBytsOrder16(
			(UINT16) dev->CHARGE_RATECURRENT);    //额定电流  充电45A，放电65A；			
	dev->DISCHARGE_RATECURRENT = (TYPE_CURRENT) reverseBytsOrder16(
			(UINT16) dev->DISCHARGE_RATECURRENT);    //
	dev->CHARGI = (TYPE_CURRENT) reverseBytsOrder16((UINT16) dev->CHARGI); // = ,//最高允许充电电流                180A				
	dev->RATEVOLTE = (TYPE_VOLTAGE) reverseBytsOrder32(dev->RATEVOLTE); // = ,      //额定电压  288V					
	dev->SCHGV = (TYPE_VOLTAGE) reverseBytsOrder32(dev->SCHGV); // = , //单体动力蓄电池最高允许充电电压  4.2V					
	dev->ALVOLT = (TYPE_VOLTAGE) reverseBytsOrder32(dev->ALVOLT); //整车动力蓄电池总电压  这个就是我们测得的总电压					
	dev->VREQUEST = (TYPE_VOLTAGE) reverseBytsOrder32(dev->VREQUEST); //,  //电压需求        ？？这块不知道				
	dev->ACHATEV = (TYPE_VOLTAGE) reverseBytsOrder32(dev->ACHATEV); // = ,//最高允许充电总电压          4.2x80=336v				
	dev->AWEIGHT = (UINT32) reverseBytsOrder32(dev->AWEIGHT); // = ,//动力蓄电池标称总重量   目前不知道        								
	dev->ALTEMP = (TYPE_TEMP) reverseBytsOrder16((UINT16) dev->ALTEMP); // = ,//最高允许温度         一级报警温度					
	//TYPE_SOC ELCSTATE;// = ; //整车动力蓄电池荷电状态  这个就是SOC		
	dev->BATTERY_Q = (UINT32) reverseBytsOrder32(dev->BATTERY_Q);//电池额定容量			90Ah			
	ReverseBytesOrderSleepTime(&dev->sleepCtlTime);
	for(i=0;i<sizeof(dev->COVARANCE);i++)
		ReverseBytsOrderFloat( (UINT8 *)&dev->COVARANCE[i]);
	//float COVARANCE[9];// 初始误差协方差			3x3矩阵，单精度		TODO
	//TODO
	return 0;
}

static int processSonFrame(UINT8 id)
{
	switch (id)
	{
		case UART_ID_SET_VOL_TH_VALUE:
			if (gSonFrameData.len != sizeof(gParameters.e_vol))
			{
				//TODO err;
//				while(1);
				break;
			}
			memcpy((UINT8 *) &gParameters.e_vol, gSonFrameData.VaidValue,
					gSonFrameData.len);
			ReverseBytesOrderVoltage(&gParameters.e_vol);	//
			break;
		case UART_ID_GET_VOL_TH_VALUE:
			break;
		case UART_ID_SET_TEMP_TH_VALUE:
			if (gSonFrameData.len != sizeof(gParameters.e_temp))
			{
				//TODO err;
				break;
			}
			memcpy((UINT8 *) &gParameters.e_temp, gSonFrameData.VaidValue,
					gSonFrameData.len);
			ReverseBytesOrderTemperature(&gParameters.e_temp);	//TODO
			break;
		case UART_ID_GET_TEMP_TH_VALUE:
			break;
		case UART_ID_SET_CUR_TH_VALUE:
			if (gSonFrameData.len != sizeof(gParameters.e_cur))
			{
				//TODO err;
				break;
			}
			memcpy((UINT8 *) &gParameters.e_cur, gSonFrameData.VaidValue,
					gSonFrameData.len);

			ReversBytsOrderCurrent(&gParameters.e_cur);	//TODO
			break;
		case UART_ID_GET_CUR_TH_VALUE:
			break;
		case UART_ID_SET_DEV_INFO:
			if (gSonFrameData.len != sizeof(gParameters.e_devInfo))
			{
				//TODO err;
				break;
			}
			memcpy((UINT8 *) &gParameters.e_devInfo, gSonFrameData.VaidValue,
					gSonFrameData.len);
			ReverseBytesOrderDevinfo(&gParameters.e_devInfo);	//TODO
			break;

		case UART_ID_GET_DEV_INFO:
			break;

		case UART_ID_SET_SIMULATE_VOL:
		{
			UINT8 CellLen = gSonFrameData.VaidValue[0];
			UINT16 t_data16 = 0;
			UINT8 i = 0;
//			if (CellLen > MAX_ALLOWED_CELLS)
//				break; //TODO err.
			for (i = 0; i < CellLen; i++)
			{
				memcpy(&t_data16, &gSonFrameData.VaidValue[i * 2 + 1], 2);
				t_data16 = reverseBytsOrder16(t_data16);
				SetSimulateVolById(i, t_data16);
			}
			mprintf("got a simulate voltage cmd!\r\n");
			break;
		}
		case UART_ID_SET_SIMULATE_TEMP:
		{
			UINT8 TempLen = gSonFrameData.VaidValue[0];
			UINT8 i = 0;
			INT8 t1 = 0;
			TYPE_TEMP t = 0;
			for (i = 0; i < TempLen; i++)
			{
//				memcpy(&t_data16, &gSonFrameData.VaidValue[i * 2 + 1], 2);
//				t_data16 = reverseBytsOrder16(t_data16);
				t1 = (INT8)(gSonFrameData.VaidValue[i  + 1]);
//				mprintf("%02x,%02x\r\n", data[0],data[1]);
//				reverseNBytsOrder(data,2);
//				mprintf("%02x,%02x\r\n", data[0],data[1]);
//				mprintf("%d\r\n", t);
				t=t1;
				SetSimulateTempById(i, t);
			}
			break;
		}

		case UART_ID_SET_SIMULATE_AD:
		{
			UINT8 adslen = gSonFrameData.VaidValue[0];
			UINT16 t_data16 = 0;
			UINT8 i = 0;
			for (i = 0; i < adslen; i++)
			{
				memcpy(&t_data16, &gSonFrameData.VaidValue[i * 2 + 1], 2);
				t_data16 = reverseBytsOrder16(t_data16);
				SetSimulateAdmvById(i, (UINT16) t_data16);
			}
			break;
		}
		case UART_ID_SET_SIMULATE_PIN:
		{
			UINT8 pinlen = gSonFrameData.VaidValue[0];
			UINT16 t_data16 = 0;
			UINT8 i = 0;
			for (i = 0; i < pinlen; i++)
			{
				SetSimulateGpioById(i, (UINT8) gSonFrameData.VaidValue[i + 1]);
			}
			break;
		}
		default:
			break;
	}
	return 0;
}
typedef int (*ControlCmdProcess)(void);

static ControlCmdProcess gCMDControl[2][16] =
{
	/*A0~A15*/
	{
		BMSHardwareInfoReport/*0xA0*/,
		0,
		0,
		BMSQueryVolThValueReport/*0XA3*/,
		0,
		BMSQueryTempThValueReport/*0xA5 temp*/,
		0,/*0xa7 cur*/
		BMSQueryCurThValueReport,
		0,/*0XA9 devinfo*/
		BMSQueryDevInfoThValueReport,
		0,
		0,
		0,
		0,
		0,
		0 },
	/*B0~B15*/
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0 } };

static int processControlCmd(struct bm06_cmd_ioctl *cmd)
{
	assert(cmd);

	if (cmd->fansControl.BITS.fan1)
	{
		FAN_OP(1);
	}
	else
	{
		FAN_OP(0);
	}

	if (cmd->heatControl)
	{
		HEAT_OP(1);
	}
	else
	{
		HEAT_OP(0);
	}

	if (cmd->chargeControl.BITS.chargeCmd)
	{
		StartChargeByUart();
	}
	else
	{
		StopChargeByUart();
	}

	if (cmd->chargeControl.BITS.dischargeCmd)
	{
		BMS_SetSimulatorVcuPower(1);
	}
	else
	{
		BMS_SetSimulatorVcuPower(0);
	}

	if (cmd->CMDControl.BITS.A0)
	{
		BMSHardwareInfoReport();
	}

	if (cmd->CMDControl.BITS.A3)
	{
		BMSQueryVolThValueReport();
	}

	if (cmd->CMDControl.BITS.A5)
	{
		BMSQueryTempThValueReport();
	}
	
//	if ( cmd->CMDControl.BYTES & 0x80)//cmd->CMDControl.BITS.A7)
	{
		BMSQueryCurThValueReport();
	}
	
	if (cmd->CMDControl1.BITS.A9)
	{
		BMSQueryDevInfoThValueReport();
	}

	if (cmd->CMDControl2.BITS.B0)
	{

	}
	setSimulateVolEnable(cmd->CMDControl2.BITS.B0);
	setSimulateTempEnable(cmd->CMDControl2.BITS.B1);
	setSimulateGpioEnable(cmd->CMDControl2.BITS.B3);
	setSimulateAdEnable(cmd->CMDControl2.BITS.B2);
	return 0;
}

static int rxSonFrame(UINT8 *data)
{
	UINT8 i = 0;
	for (i = 0; i < 8; i++)
	{
		getsonframeData(data[i]);
		mprintf("%02x ", data[i]);
	}
//	mprintf("\r\n");
	return 0;
}

static void processOneRxFrame(void)
{
	struct uart_cmd_pkg *tmsg = &g_msg;
	struct bm06_protocol *pt = NULL;
	struct bm06_cmd_ioctl *cmd = NULL;

	//(void)mgcfifo_out( pfifo, (char *)tmsg);
	pt = (struct bm06_protocol *) tmsg->param;

	if (isASonFrame(pt->PF))
	{
		rxSonFrame(pt->DATA);
		if (gSonFrameData.sonframe_status == sonframe_rec_ok)
		{
			processSonFrame(pt->PF);
			gSonFrameData.sonframe_status = sonframe_rec_start;
		}
		return;
	}


	switch (pt->PF)
	{
		case UART_ID_CONTROL:
			cmd = (struct bm06_cmd_ioctl *) pt->DATA;
//				gCmdControl = cmd;
			processControlCmd(cmd);
			break;

		default:
			break;
	}
//		protocol_frame_clear();
}

INT16 process_rs232_cmd(void) //锟斤拷循锟斤拷锟斤拷锟斤拷
{
	UCHAR ch = 0;
	int left = uart_isr_fifo_size();
	static UINT8 xframe_rx = 0;
	while (left > 0)
	{
		(void) mgcfifo_out(p_uart_rx_fifo, (char *) &ch);
		left = uart_isr_fifo_size();
		protocol_rx(ch);
		if (nRxFrameSize() > 0) //protocol_frame_size() > 0 )
		{
			nRxFrameUseOne();
			xframe_rx++;
//			mprintf("rx %d frame now!\r\n", xframe_rx);
			processOneRxFrame();
		}
	}

	return 0;
}

static int test_rx_sonframe(void)
{
	static const unsigned char datas[] =
	{
		'{',
		'<',
		UART_ID_SET_VOL_TH_VALUE,
		'0',
		'0',
		'<',
		10,
		0,
		0,
		1,
		2,
		3,
		4,
		'>',
		'}',
		'{',
		'<',
		UART_ID_SET_VOL_TH_VALUE,
		'0',
		'0',
		6,
		7,
		8,
		9,
		10,
		'>',
		0xaa,
		0xaa,
		'>',
		'}' };

	unsigned char i = 0;
	uart_protocol_init();
	for (i = 0; i < sizeof(datas); i++)
		uart_isr_getch(datas[i]);
	process_rs232_cmd();
	return 0;
}

static int uartBm06ProtocolSend(const char *buff)
{
	assert(buff);
//	(void) PROTOCOL_PUTS ( "tx:" );
	(void) PROTOCOL_PUTARRAY(head,sizeof(head));
	(void) PROTOCOL_PUTARRAY ( &g_cmd, 1 );
	(void) PROTOCOL_PUTARRAY ( &g_srcAddr, 1 );
	(void) PROTOCOL_PUTARRAY ( &g_dstAddr, 1 );
	(void) PROTOCOL_PUTARRAY ( (const UINT8*)buff, 8 );
	(void) PROTOCOL_PUTARRAY(tail, sizeof(tail));
	(void) PROTOCOL_PUTS ( "    \r\n" )
	;
	return 0;
}

static int CellVolReport(void)
{
	UINT8 i = 0, k = 0;
	UINT16 v = 0;
	memset(sendbuff, 0, sizeof(sendbuff));
	g_cmd = UART_ID_GET_CELL_VOL;
	for (i = 0; i < TOTAL_VOLTAGES; i++)
	{
		v = (UINT16) getVoltageValueByCellIndex(i);
		v /= 10;
		v &= 0x01FF; // 0~12 means voltage
		v |= ((i + 1) << 9);
//		v |= (((i / 10 + 1 ) << 12) );  //number of cell voltage
		BYTES_ORDER_B2S_16(sendbuff + k, v);
		k += 2;

		if (k == 8)
		{
			k = 0;
			uartBm06ProtocolSend((const char *) sendbuff);
		}
	}
	v = (UINT16) (k % 8);
	if (v)
	{
		uartBm06ProtocolSend((const char *) sendbuff);
	}
	return 0;
}

static int CellTempReport(void)
{
	UINT8 i = 0, k = 0, t = 0;
	INT16 temp = 0;
	memset(sendbuff, 0, sizeof(sendbuff));
	g_cmd = UART_ID_GET_CELL_TEMP;
	for (i = 0; i < TOTAL_TEMPERATURE_SENSORS_NUM; i++)
	{
		temp = BMS_GetTemperatureValueById(i);
		temp += 50;
		if (temp < 0)
			temp = 0;
		if (temp > 200)
			temp = 200;
		t = (UINT8) temp & 0xff;
		sendbuff[k++] = i + 1;
		sendbuff[k++] = t;
		if (k == 8)
		{
			k = 0;
			uartBm06ProtocolSend((const char *) sendbuff);
		}
	}

	t = k % 8;
	if (t)
	{
		uartBm06ProtocolSend((const char *) sendbuff);
	}
	return 0;
}

static int BatteryStatus1Report(void)
{
	const char *buff;
	g_cmd = UART_ID_GET_BATTERY_STATUS1;
	packVCUCanB2VP2Package();
	buff = readPackageBuff();
	uartBm06ProtocolSend(buff);
	return 0;
}

static int BatteryStatus2Report(void)
{
	const char *buff;
	g_cmd = UART_ID_GET_BATTERY_STATUS2;
	packVCUCanB2VP3Package();
	buff = readPackageBuff();
	uartBm06ProtocolSend(buff);

	return 0;
}

static int BatteryWarningInfoReport(void)
{
	const char *buff;
	g_cmd = UART_ID_GET_BATTERY_WARNING_INFO;
	packVCUCanB2VP4Package();
	buff = readPackageBuff();
	uartBm06ProtocolSend(buff);
	return 0;
}
static int BMSStatusReport(void)
{

	const char *buff;
	g_cmd = UART_ID_GET_BMS_STATUS;
	packVCUCanB2VP1Package();
	buff = readPackageBuff();
	uartBm06ProtocolSend(buff);
	return 0;

}

/*
 * OUT 	IN 	ID 	周期(ms) 
 VCU 	BMS 	0xA0	0ms 
 BYTE 	BIT	数据名
 参数说明
 0		从机数量	当前BMS从机数量
 1		从机电芯数	当前BMS中单个从机的电芯数
 2		从机温度传感器数	当前BMS中单个从机的温度传感器数
 3		待定	
 4		待定	
 5		待定	
 6		待定	
 * 
 * */

static int packUartHardwareInforToBuff(UINT8 *buff)
{
	assert(buff);
	buff[0] = BMS_GetSlaveNumbers();
	buff[1] = BMS_GetNCellsPerSlave();
	buff[2] = BMS_GetNSensorsPerSlave();
	return 8;
}

static int BMSHardwareInfoReport(void)
{

	memset(sendbuff, 0, sizeof(sendbuff));
	g_cmd = UART_ID_GET_BMS_HARDWARE_INFO;

	packUartHardwareInforToBuff(sendbuff);
	uartBm06ProtocolSend((const char *) sendbuff);

	return 0;
}
/*
 * param:
 * 	@validDataLen --the DATA filed of the sonframe.
 * */
static int packSonFrameHeadToBuff(UINT8 *buff, UINT16 validDataLen)
{
	buff[0] = sonframehead;
	BYTES_ORDER_B2S_16(buff + 1, validDataLen);
	return 3;
}
/*
 * param:
 * 	@nlast: the last data haven't transfer
 * */
static int packSOnframeTailToBuff(UINT8 *buff, UINT16 nlast)
{
	buff[nlast] = sonframeend;
	return 0;
}

static int BMSSonFrameReport(UINT8 cmd, UINT8 *ptx, UINT16 data_len)
{
	UINT16 nSonframe = 0, i = 0, nlastframe = 0;
	memset(sendbuff, 0, sizeof(sendbuff));
	g_cmd = cmd;
	packSonFrameHeadToBuff(sendbuff, data_len);
	nSonframe = (data_len + 3) / 8;
	nlastframe = (data_len + 3) % 8;
	memcpy(sendbuff + 3, ptx, 5);
	uartBm06ProtocolSend((const char *) sendbuff);
	nSonframe--;
	ptx += 5;

	for (i = 0; i < nSonframe; i++)
	{
		uartBm06ProtocolSend((const char *) ptx);
		ptx += 8;
	}

	memcpy(sendbuff, ptx, nlastframe);
	packSOnframeTailToBuff((UINT8 *) sendbuff, nlastframe);
	uartBm06ProtocolSend((const char *) sendbuff);
}

#define REPORT_TH_VALUE_TO_S( reversFunc,value, types, id )\
	do\
	{\
		reversFunc(&value);\
		BMSSonFrameReport(id, (UINT8 *)&value, sizeof(types));\
		reversFunc(&value);\
	}\
while(0)

static int BMSQueryVolThValueReport(void)
{
	ReverseBytesOrderVoltage(&gParameters.e_vol);	//TODO
	BMSSonFrameReport(UART_ID_GET_VOL_TH_VALUE, (UINT8 *) &gParameters.e_vol,
			sizeof(struct voltage_const_vars));
	ReverseBytesOrderVoltage(&gParameters.e_vol);
	return 0;
}

static int BMSQueryTempThValueReport(void)
{
	REPORT_TH_VALUE_TO_S( ReverseBytesOrderTemperature, gParameters.e_temp,
			struct temperature_const_vars, UART_ID_GET_TEMP_TH_VALUE);
	return 0;
}

static int BMSQueryCurThValueReport(void)
{
	REPORT_TH_VALUE_TO_S( ReversBytsOrderCurrent, gParameters.e_cur,
			struct CCurrentsThresholdConstValue, UART_ID_GET_CUR_TH_VALUE);
	return 0;
}

static int BMSQueryDevInfoThValueReport(void)
{
	REPORT_TH_VALUE_TO_S( ReverseBytesOrderDevinfo, gParameters.e_devInfo,
			struct dev_info, UART_ID_GET_DEV_INFO);
	return 0;
}

int uart_communication(void)
{

	static UINT32 lastTimems = 0;
	static UINT32 time1Second = 0;
	static UINT8 count = 0;
	UINT32 timeNow = getSystemTimeMs();
	process_rs232_cmd();
	if (TIME_TRHESHOLD_CAL(timeNow, lastTimems) > 200)
	{
		lastTimems = timeNow;
		count++;
		if (count == 1)
			CellVolReport();
		else if (count == 2)
			CellTempReport();
		else if (count == 3)
			BMSStatusReport();
		else if (count == 4)
			BatteryStatus1Report();
		else if (count == 5)
			BatteryStatus2Report();
		else if (count == 6)
		{
			BatteryWarningInfoReport();
			count = 0;
		}
//		else if(count == 7)
//		{
//			BMSHardwareInfoReport();
//			count = 0;
//		}
	}

}



static void TestSetValues(char *data, int datalen)
{
	int i = 0;
	for(i=0; i<datalen; i++)
	{
		data[i] = (char)i;
	}
}
int uartProtocolTest(void)
{
	static struct voltage_const_vars gtestConstVoltages;
	static struct dev_info gtestConstdevinfo;
	static struct temperature_const_vars gtesttemp;
	static struct CCurrentsThresholdConstValue gTestConstCurValue;
	TestSetValues((char *)&gtestConstVoltages, sizeof(gtestConstVoltages));
	TestSetValues((char *)&gtestConstdevinfo, sizeof(gtestConstdevinfo));
	TestSetValues((char *)&gtesttemp, sizeof(gtesttemp));
	TestSetValues((char *)&gTestConstCurValue, sizeof(gTestConstCurValue));
	
	ReverseBytesOrderVoltage(&gtestConstVoltages);
	ReverseBytesOrderDevinfo(&gtestConstdevinfo);
	ReverseBytesOrderTemperature(&gtesttemp);
	ReversBytsOrderCurrent(&gTestConstCurValue);
	

	ReverseBytesOrderVoltage(&gtestConstVoltages);
	ReverseBytesOrderDevinfo(&gtestConstdevinfo);
	ReverseBytesOrderTemperature(&gtesttemp);
	ReversBytsOrderCurrent(&gTestConstCurValue);
	

	ReverseBytesOrderVoltage(&gtestConstVoltages);
	ReverseBytesOrderDevinfo(&gtestConstdevinfo);
	ReverseBytesOrderTemperature(&gtesttemp);
	ReversBytsOrderCurrent(&gTestConstCurValue);
	
	test_rx_sonframe();
	
	BMSQueryVolThValueReport();
	BMSQueryTempThValueReport();
	BMSQueryCurThValueReport();
	BMSQueryDevInfoThValueReport();
	
	return 0;
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
