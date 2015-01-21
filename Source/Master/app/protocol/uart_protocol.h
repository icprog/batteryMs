
/*********************************************************
*
* (UCT)
*
* File Description
*
*   $Revision: 2522 $
*   $Author: huhai $
*   $this file is used for uart-rs232 communication between BMS-BOARD and PC
*   $Date: 2014-08-05 13:50:35 +0800 (ÖÜ¶þ, 05 °ËÔÂ 2014) $
*
*********************************************************/
#ifndef UART_PROTOCOL_H_
#define UART_PROTOCOL_H_
//#include "protocol\cmd_class.h"
#include "public/public.h"
#include "mgclib/mgcstring.h"
//#include "driver/uart/uart.h"
#include "mgclib/mgcbytesorder.h"
#include "mgclib/mgcprintf.h"


static const UCHAR head[] = {  '{', '<'  };
static const UCHAR tail_size = 2;
static const UCHAR tail[tail_size] = {  '>', '}' };

#pragma push
//#pragma align off        
struct uart_cmd_pkg
{
    UCHAR cmd;
    UINT16 len;
    UCHAR *param;
};
//#pragma align off   
struct uart_protocl
{
    UCHAR head[2];
    struct uart_cmd_pkg cmd;
    UCHAR tail[2];
};

/******************************************************************************** 
 *this module's interfaces 
 ********************************************************************************/
void protocol_rx ( UCHAR ch ); //shuld use this func in isr
void protocol_tx ( UCHAR cmdwords, const UCHAR *pcmd, UINT16 cmd_len );
INT16 process_rs232_cmd ( void ); //please use this func in main loop
int uart_protocol_init(void); //use this func before enable the intrrupt
void uart_isr_getch( UCHAR ch );  //ISR process
int uart_communication(void);

/*******************************************************************************
 *external funcs
 ********************************************************************************/
extern INT16 ProcessCmdByName ( INT16 argc, const char *argv[] ); //cmd_process.h
extern INT16 ProcessCmdByWords ( const UCHAR* cmds, UCHAR **cmd_process_result );     //cmd_process.h

/********************************************************************************
 * serial send funcs
 ********************************************************************************/
#define PROTOCOL_PUTARRAY( array, arraylen ) uart_send( array, arraylen ) // printf_array_hex( (char *)(array), (arraylen) )
#define PROTOCOL_PUTC( ch ) mprintf("%c", ch);
#define PROTOCOL_PUTS( string ) mprintf("%s", (char *)string);
#define PROTOCOL_PUTHEX( ch ) printf_ch_hex(ch)     
#define MAX_PARAM_ACK_LEN  200 
#define MAX_CMD_PARAM_SIZE 80
#define FIFO_SIZE 2000
#define FIFO_ELEMENTS_SIZE 1


#define BMS_GetSlaveNumbers() N_SLAVES
#define BMS_GetNCellsPerSlave() NCLEES_EACH_SLAVE
#define BMS_GetNSensorsPerSlave() NTEMPS_EACH_SLAVE
int test_reverse_bytes_order(void);
//int test_rx_sonframe(void);
int uartProtocolTest(void);
#endif

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
