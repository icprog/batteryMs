#ifndef _MGCSTRING_H_
#define _MGCSTRING_H_

//#include "Gpio/gpio.h"
#include "public/public.h"
//#include "uart/uart.h"

#define debug_uart_send SCIA_putChar

extern int printf_array_hex( const char* hexarray, int len );

extern void printf_ch_hex( char ch );

void uart_send( const UCHAR *data, UINT16 len );

void
debug_uart_send_str(char *data);
#endif

